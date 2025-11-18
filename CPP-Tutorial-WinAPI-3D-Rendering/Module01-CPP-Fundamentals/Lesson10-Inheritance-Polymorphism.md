# Lesson 10: Inheritance and Polymorphism
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Inheritance](#introduction-to-inheritance)
2. [Basic Inheritance](#basic-inheritance)
3. [Protected Members](#protected-members)
4. [Types of Inheritance](#types-of-inheritance)
5. [Function Overriding](#function-overriding)
6. [Virtual Functions](#virtual-functions)
7. [Pure Virtual Functions](#pure-virtual-functions)
8. [Abstract Classes](#abstract-classes)
9. [Polymorphism](#polymorphism)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to Inheritance

Inheritance allows a class to inherit properties and methods from another class.

### Why Inheritance?

**Without inheritance (code duplication):**
```cpp
class Dog
{
    std::string name;
    int age;
    void eat() { }
    void sleep() { }
    void bark() { }
};

class Cat
{
    std::string name;  // Duplicate!
    int age;           // Duplicate!
    void eat() { }     // Duplicate!
    void sleep() { }   // Duplicate!
    void meow() { }
};
```

**With inheritance (code reuse):**
```cpp
class Animal  // Base class
{
protected:
    std::string name;
    int age;
public:
    void eat() { }
    void sleep() { }
};

class Dog : public Animal  // Derived class
{
public:
    void bark() { }
};

class Cat : public Animal  // Derived class
{
public:
    void meow() { }
};
```

### Inheritance Terminology:
- **Base class** / **Parent class** / **Superclass**: Animal
- **Derived class** / **Child class** / **Subclass**: Dog, Cat
- **Inheritance** / **Derivation**: Process of creating derived classes

## Basic Inheritance

### Simple Inheritance

```cpp
#include <iostream>
#include <string>

class Animal
{
protected:
    std::string name;

public:
    Animal(const std::string& n) : name(n)
    {
        std::cout << "Animal constructor: " << name << "\n";
    }

    void eat()
    {
        std::cout << name << " is eating\n";
    }

    void sleep()
    {
        std::cout << name << " is sleeping\n";
    }
};

class Dog : public Animal
{
public:
    Dog(const std::string& n) : Animal(n)
    {
        std::cout << "Dog constructor: " << name << "\n";
    }

    void bark()
    {
        std::cout << name << " says: Woof!\n";
    }
};

int main()
{
    Dog dog("Rex");

    dog.eat();    // Inherited from Animal
    dog.sleep();  // Inherited from Animal
    dog.bark();   // Dog's own method

    return 0;
}
```

### Inheritance Syntax

```cpp
class DerivedClass : access_specifier BaseClass
{
    // Class body
};

// Examples:
class Dog : public Animal { };      // Public inheritance
class Cat : private Animal { };     // Private inheritance
class Bird : protected Animal { };  // Protected inheritance
```

### Constructor and Destructor Order

```cpp
#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base constructor\n";
    }

    ~Base()
    {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        std::cout << "Derived constructor\n";
    }

    ~Derived()
    {
        std::cout << "Derived destructor\n";
    }
};

int main()
{
    std::cout << "Creating object:\n";
    Derived obj;

    std::cout << "\nDestroying object:\n";
    return 0;
}
```

Output:
```
Creating object:
Base constructor
Derived constructor

Destroying object:
Derived destructor
Base destructor
```

## Protected Members

Protected members are accessible in the class and derived classes.

### Access Levels

```cpp
#include <iostream>

class Base
{
public:
    int publicVar;

protected:
    int protectedVar;

private:
    int privateVar;

public:
    Base() : publicVar(1), protectedVar(2), privateVar(3) {}
};

class Derived : public Base
{
public:
    void showAccess()
    {
        std::cout << "publicVar: " << publicVar << "\n";      // OK
        std::cout << "protectedVar: " << protectedVar << "\n"; // OK
        // std::cout << "privateVar: " << privateVar << "\n";  // ERROR!
    }
};

int main()
{
    Derived obj;
    obj.showAccess();

    std::cout << "From main: " << obj.publicVar << "\n";      // OK
    // std::cout << "From main: " << obj.protectedVar << "\n"; // ERROR!
    // std::cout << "From main: " << obj.privateVar << "\n";   // ERROR!

    return 0;
}
```

### When to Use Protected

```cpp
#include <iostream>

class Shape
{
protected:
    double width, height;  // Accessible to derived classes

public:
    void setDimensions(double w, double h)
    {
        width = w;
        height = h;
    }

    virtual double area() = 0;  // Pure virtual (covered later)
};

class Rectangle : public Shape
{
public:
    double area() override
    {
        return width * height;  // Can access protected members
    }
};

class Triangle : public Shape
{
public:
    double area() override
    {
        return 0.5 * width * height;  // Can access protected members
    }
};
```

## Types of Inheritance

### Public Inheritance (Most Common)

```cpp
class Base
{
public:
    int publicMember;
protected:
    int protectedMember;
private:
    int privateMember;
};

class Derived : public Base
{
    // publicMember remains public
    // protectedMember remains protected
    // privateMember inaccessible
};
```

### Protected Inheritance

```cpp
class Derived : protected Base
{
    // publicMember becomes protected
    // protectedMember remains protected
    // privateMember inaccessible
};
```

### Private Inheritance

```cpp
class Derived : private Base
{
    // publicMember becomes private
    // protectedMember becomes private
    // privateMember inaccessible
};
```

### Comparison Example

```cpp
#include <iostream>

class Base
{
public:
    void publicMethod() { std::cout << "Public method\n"; }
};

class PublicDerived : public Base
{
    // publicMethod is public here
};

class ProtectedDerived : protected Base
{
    // publicMethod is protected here
};

class PrivateDerived : private Base
{
    // publicMethod is private here
};

int main()
{
    PublicDerived pd;
    pd.publicMethod();  // OK

    ProtectedDerived prd;
    // prd.publicMethod();  // ERROR: protected

    PrivateDerived prvd;
    // prvd.publicMethod();  // ERROR: private

    return 0;
}
```

## Function Overriding

Derived class can provide its own implementation of base class method.

### Basic Override

```cpp
#include <iostream>

class Animal
{
public:
    void makeSound()
    {
        std::cout << "Animal makes a sound\n";
    }
};

class Dog : public Animal
{
public:
    void makeSound()  // Overrides Animal::makeSound()
    {
        std::cout << "Dog barks: Woof!\n";
    }
};

class Cat : public Animal
{
public:
    void makeSound()  // Overrides Animal::makeSound()
    {
        std::cout << "Cat meows: Meow!\n";
    }
};

int main()
{
    Dog dog;
    Cat cat;

    dog.makeSound();  // Calls Dog::makeSound()
    cat.makeSound();  // Calls Cat::makeSound()

    return 0;
}
```

### Calling Base Class Method

```cpp
#include <iostream>

class Base
{
public:
    void display()
    {
        std::cout << "Base display\n";
    }
};

class Derived : public Base
{
public:
    void display()
    {
        Base::display();  // Call base class version
        std::cout << "Derived display\n";
    }
};

int main()
{
    Derived obj;
    obj.display();

    obj.Base::display();  // Explicitly call base version

    return 0;
}
```

## Virtual Functions

Virtual functions enable runtime polymorphism.

### Problem Without Virtual

```cpp
#include <iostream>

class Animal
{
public:
    void makeSound()  // Non-virtual
    {
        std::cout << "Animal sound\n";
    }
};

class Dog : public Animal
{
public:
    void makeSound()
    {
        std::cout << "Woof!\n";
    }
};

int main()
{
    Dog dog;
    Animal* ptr = &dog;

    ptr->makeSound();  // Calls Animal::makeSound() - WRONG!

    return 0;
}
```

### Solution: Virtual Functions

```cpp
#include <iostream>

class Animal
{
public:
    virtual void makeSound()  // Virtual function
    {
        std::cout << "Animal sound\n";
    }
};

class Dog : public Animal
{
public:
    void makeSound() override  // Override keyword (C++11)
    {
        std::cout << "Woof!\n";
    }
};

class Cat : public Animal
{
public:
    void makeSound() override
    {
        std::cout << "Meow!\n";
    }
};

int main()
{
    Dog dog;
    Cat cat;

    Animal* ptr1 = &dog;
    Animal* ptr2 = &cat;

    ptr1->makeSound();  // Calls Dog::makeSound() - CORRECT!
    ptr2->makeSound();  // Calls Cat::makeSound() - CORRECT!

    return 0;
}
```

### Virtual Destructor

```cpp
#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base constructor\n";
    }

    virtual ~Base()  // Virtual destructor
    {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base
{
private:
    int* data;

public:
    Derived()
    {
        data = new int[100];
        std::cout << "Derived constructor\n";
    }

    ~Derived()
    {
        delete[] data;
        std::cout << "Derived destructor\n";
    }
};

int main()
{
    Base* ptr = new Derived();
    delete ptr;  // Calls both destructors if Base destructor is virtual

    return 0;
}
```

**Rule**: Always make destructor virtual if class has virtual functions or will be used polymorphically.

## Pure Virtual Functions

Pure virtual functions have no implementation in base class.

### Syntax

```cpp
class Base
{
public:
    virtual void pureVirtual() = 0;  // Pure virtual function
};
```

### Example

```cpp
#include <iostream>

class Shape
{
protected:
    std::string name;

public:
    Shape(const std::string& n) : name(n) {}

    // Pure virtual functions
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Regular virtual function
    virtual void display() const
    {
        std::cout << "Shape: " << name << "\n";
    }
};

class Rectangle : public Shape
{
private:
    double width, height;

public:
    Rectangle(double w, double h)
        : Shape("Rectangle"), width(w), height(h) {}

    double area() const override
    {
        return width * height;
    }

    double perimeter() const override
    {
        return 2 * (width + height);
    }

    void display() const override
    {
        Shape::display();
        std::cout << "Area: " << area() << "\n";
        std::cout << "Perimeter: " << perimeter() << "\n";
    }
};

class Circle : public Shape
{
private:
    double radius;

public:
    Circle(double r)
        : Shape("Circle"), radius(r) {}

    double area() const override
    {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override
    {
        return 2 * 3.14159 * radius;
    }

    void display() const override
    {
        Shape::display();
        std::cout << "Area: " << area() << "\n";
        std::cout << "Perimeter: " << perimeter() << "\n";
    }
};

int main()
{
    // Shape s;  // ERROR: Cannot instantiate abstract class

    Rectangle rect(5, 3);
    Circle circ(4);

    rect.display();
    std::cout << "\n";
    circ.display();

    return 0;
}
```

## Abstract Classes

Class with at least one pure virtual function.

### Abstract Class Characteristics

1. **Cannot be instantiated**
2. **Can have constructors and destructors**
3. **Can have regular member variables and functions**
4. **Serves as interface/contract for derived classes**

### Interface Example

```cpp
#include <iostream>
#include <string>

// Abstract class (interface)
class Drawable
{
public:
    virtual void draw() const = 0;
    virtual ~Drawable() = default;
};

class Printable
{
public:
    virtual void print() const = 0;
    virtual ~Printable() = default;
};

// Concrete class implementing interfaces
class Document : public Drawable, public Printable
{
private:
    std::string content;

public:
    Document(const std::string& c) : content(c) {}

    void draw() const override
    {
        std::cout << "Drawing document: " << content << "\n";
    }

    void print() const override
    {
        std::cout << "Printing document: " << content << "\n";
    }
};

int main()
{
    Document doc("Hello, World!");

    doc.draw();
    doc.print();

    Drawable* d = &doc;
    d->draw();

    Printable* p = &doc;
    p->print();

    return 0;
}
```

### Abstract Base Class with Shared Code

```cpp
#include <iostream>
#include <string>

class Employee
{
protected:
    std::string name;
    int id;

public:
    Employee(const std::string& n, int i)
        : name(n), id(i) {}

    virtual ~Employee() = default;

    // Pure virtual function
    virtual double getSalary() const = 0;

    // Regular function (shared by all employees)
    void display() const
    {
        std::cout << "Name: " << name << "\n";
        std::cout << "ID: " << id << "\n";
        std::cout << "Salary: $" << getSalary() << "\n";
    }
};

class HourlyEmployee : public Employee
{
private:
    double hourlyRate;
    int hoursWorked;

public:
    HourlyEmployee(const std::string& n, int i, double rate, int hours)
        : Employee(n, i), hourlyRate(rate), hoursWorked(hours) {}

    double getSalary() const override
    {
        return hourlyRate * hoursWorked;
    }
};

class SalariedEmployee : public Employee
{
private:
    double annualSalary;

public:
    SalariedEmployee(const std::string& n, int i, double salary)
        : Employee(n, i), annualSalary(salary) {}

    double getSalary() const override
    {
        return annualSalary / 12;  // Monthly salary
    }
};

int main()
{
    HourlyEmployee hourly("Alice", 101, 25.0, 160);
    SalariedEmployee salaried("Bob", 102, 80000);

    hourly.display();
    std::cout << "\n";
    salaried.display();

    return 0;
}
```

## Polymorphism

Polymorphism allows objects of different classes to be treated as objects of a common base class.

### Runtime Polymorphism

```cpp
#include <iostream>
#include <vector>

class Animal
{
protected:
    std::string name;

public:
    Animal(const std::string& n) : name(n) {}

    virtual void makeSound() const
    {
        std::cout << name << " makes a sound\n";
    }

    virtual ~Animal() = default;
};

class Dog : public Animal
{
public:
    Dog(const std::string& n) : Animal(n) {}

    void makeSound() const override
    {
        std::cout << name << " barks: Woof!\n";
    }
};

class Cat : public Animal
{
public:
    Cat(const std::string& n) : Animal(n) {}

    void makeSound() const override
    {
        std::cout << name << " meows: Meow!\n";
    }
};

class Bird : public Animal
{
public:
    Bird(const std::string& n) : Animal(n) {}

    void makeSound() const override
    {
        std::cout << name << " chirps: Chirp!\n";
    }
};

int main()
{
    std::vector<Animal*> animals;

    animals.push_back(new Dog("Rex"));
    animals.push_back(new Cat("Whiskers"));
    animals.push_back(new Bird("Tweety"));
    animals.push_back(new Dog("Buddy"));

    std::cout << "All animals make sounds:\n";
    for (Animal* animal : animals)
    {
        animal->makeSound();  // Polymorphism!
    }

    // Cleanup
    for (Animal* animal : animals)
    {
        delete animal;
    }

    return 0;
}
```

### Polymorphic Function Parameters

```cpp
#include <iostream>

class Shape
{
public:
    virtual double area() const = 0;
    virtual void display() const = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape
{
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override
    {
        return width * height;
    }

    void display() const override
    {
        std::cout << "Rectangle: " << width << " x " << height;
    }
};

class Circle : public Shape
{
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double area() const override
    {
        return 3.14159 * radius * radius;
    }

    void display() const override
    {
        std::cout << "Circle with radius: " << radius;
    }
};

// Polymorphic function
void printShapeInfo(const Shape& shape)
{
    shape.display();
    std::cout << ", Area: " << shape.area() << "\n";
}

int main()
{
    Rectangle rect(5, 3);
    Circle circ(4);

    printShapeInfo(rect);  // Works with Rectangle
    printShapeInfo(circ);  // Works with Circle

    return 0;
}
```

## Complete Working Examples

### Example 1: Vehicle Hierarchy

```cpp
#include <iostream>
#include <string>
#include <vector>

class Vehicle
{
protected:
    std::string brand;
    int year;
    double price;

public:
    Vehicle(const std::string& b, int y, double p)
        : brand(b), year(y), price(p) {}

    virtual ~Vehicle() = default;

    virtual void display() const
    {
        std::cout << year << " " << brand;
    }

    virtual double getInsurance() const = 0;

    double getPrice() const { return price; }
};

class Car : public Vehicle
{
private:
    int numDoors;
    bool isElectric;

public:
    Car(const std::string& b, int y, double p, int doors, bool electric)
        : Vehicle(b, y, p), numDoors(doors), isElectric(electric) {}

    void display() const override
    {
        Vehicle::display();
        std::cout << " Car (" << numDoors << " doors, "
                  << (isElectric ? "Electric" : "Gas") << ")\n";
    }

    double getInsurance() const override
    {
        double base = price * 0.05;
        if (isElectric)
            base *= 0.9;  // 10% discount for electric
        return base;
    }
};

class Motorcycle : public Vehicle
{
private:
    int engineCC;

public:
    Motorcycle(const std::string& b, int y, double p, int cc)
        : Vehicle(b, y, p), engineCC(cc) {}

    void display() const override
    {
        Vehicle::display();
        std::cout << " Motorcycle (" << engineCC << "cc)\n";
    }

    double getInsurance() const override
    {
        return price * 0.08 + engineCC * 0.5;
    }
};

class Truck : public Vehicle
{
private:
    double cargoCapacity;

public:
    Truck(const std::string& b, int y, double p, double capacity)
        : Vehicle(b, y, p), cargoCapacity(capacity) {}

    void display() const override
    {
        Vehicle::display();
        std::cout << " Truck (Capacity: " << cargoCapacity << " tons)\n";
    }

    double getInsurance() const override
    {
        return price * 0.07 + cargoCapacity * 100;
    }
};

int main()
{
    std::vector<Vehicle*> fleet;

    fleet.push_back(new Car("Tesla Model 3", 2023, 45000, 4, true));
    fleet.push_back(new Car("Honda Civic", 2022, 25000, 4, false));
    fleet.push_back(new Motorcycle("Harley Davidson", 2021, 18000, 1200));
    fleet.push_back(new Truck("Ford F-150", 2023, 55000, 3.5));

    std::cout << "=== Vehicle Fleet ===\n\n";

    double totalValue = 0;
    double totalInsurance = 0;

    for (const auto& vehicle : fleet)
    {
        vehicle->display();
        std::cout << "  Price: $" << vehicle->getPrice() << "\n";
        std::cout << "  Insurance: $" << vehicle->getInsurance() << "\n\n";

        totalValue += vehicle->getPrice();
        totalInsurance += vehicle->getInsurance();
    }

    std::cout << "Total Fleet Value: $" << totalValue << "\n";
    std::cout << "Total Annual Insurance: $" << totalInsurance << "\n";

    // Cleanup
    for (auto& vehicle : fleet)
    {
        delete vehicle;
    }

    return 0;
}
```

### Example 2: Banking System

```cpp
#include <iostream>
#include <string>
#include <vector>

class Account
{
protected:
    std::string accountNumber;
    std::string owner;
    double balance;

public:
    Account(const std::string& num, const std::string& own, double bal)
        : accountNumber(num), owner(own), balance(bal) {}

    virtual ~Account() = default;

    virtual void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            std::cout << "Deposited $" << amount << "\n";
        }
    }

    virtual bool withdraw(double amount) = 0;  // Pure virtual

    virtual void display() const
    {
        std::cout << "Account: " << accountNumber << "\n";
        std::cout << "Owner: " << owner << "\n";
        std::cout << "Balance: $" << balance << "\n";
    }

    virtual double getMonthlyFee() const = 0;

    double getBalance() const { return balance; }
};

class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    SavingsAccount(const std::string& num, const std::string& own,
                   double bal, double rate)
        : Account(num, own, bal), interestRate(rate) {}

    bool withdraw(double amount) override
    {
        if (amount > balance)
        {
            std::cout << "Insufficient funds!\n";
            return false;
        }

        balance -= amount;
        std::cout << "Withdrawn $" << amount << "\n";
        return true;
    }

    void applyInterest()
    {
        double interest = balance * interestRate / 12;
        balance += interest;
        std::cout << "Interest applied: $" << interest << "\n";
    }

    double getMonthlyFee() const override
    {
        return 0;  // No fee for savings account
    }

    void display() const override
    {
        std::cout << "=== Savings Account ===\n";
        Account::display();
        std::cout << "Interest Rate: " << (interestRate * 100) << "%\n";
    }
};

class CheckingAccount : public Account
{
private:
    double overdraftLimit;

public:
    CheckingAccount(const std::string& num, const std::string& own,
                    double bal, double limit)
        : Account(num, own, bal), overdraftLimit(limit) {}

    bool withdraw(double amount) override
    {
        if (amount > balance + overdraftLimit)
        {
            std::cout << "Exceeds overdraft limit!\n";
            return false;
        }

        balance -= amount;
        std::cout << "Withdrawn $" << amount << "\n";

        if (balance < 0)
        {
            std::cout << "Warning: Overdraft! Balance: $" << balance << "\n";
        }

        return true;
    }

    double getMonthlyFee() const override
    {
        if (balance < 1000)
            return 10.0;
        return 0;
    }

    void display() const override
    {
        std::cout << "=== Checking Account ===\n";
        Account::display();
        std::cout << "Overdraft Limit: $" << overdraftLimit << "\n";
    }
};

int main()
{
    std::vector<Account*> accounts;

    accounts.push_back(new SavingsAccount("SAV001", "Alice", 5000, 0.04));
    accounts.push_back(new CheckingAccount("CHK001", "Bob", 1500, 500));
    accounts.push_back(new SavingsAccount("SAV002", "Charlie", 10000, 0.04));

    std::cout << "=== Account Information ===\n\n";

    for (auto& account : accounts)
    {
        account->display();
        std::cout << "Monthly Fee: $" << account->getMonthlyFee() << "\n\n";
    }

    // Perform transactions
    std::cout << "=== Transactions ===\n\n";

    accounts[0]->deposit(1000);
    accounts[1]->withdraw(2000);

    if (SavingsAccount* savingsAcc = dynamic_cast<SavingsAccount*>(accounts[0]))
    {
        savingsAcc->applyInterest();
    }

    // Cleanup
    for (auto& account : accounts)
    {
        delete account;
    }

    return 0;
}
```

### Example 3: Game Characters

```cpp
#include <iostream>
#include <string>
#include <vector>

class Character
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int level;

public:
    Character(const std::string& n, int hp, int lvl)
        : name(n), health(hp), maxHealth(hp), level(lvl) {}

    virtual ~Character() = default;

    virtual void attack(Character& target) = 0;
    virtual void specialAbility() = 0;

    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;

        std::cout << name << " takes " << damage << " damage. ";
        std::cout << "HP: " << health << "/" << maxHealth << "\n";

        if (health == 0)
        {
            std::cout << name << " has been defeated!\n";
        }
    }

    void heal(int amount)
    {
        health += amount;
        if (health > maxHealth)
            health = maxHealth;

        std::cout << name << " heals " << amount << " HP. ";
        std::cout << "HP: " << health << "/" << maxHealth << "\n";
    }

    bool isAlive() const
    {
        return health > 0;
    }

    virtual void displayStats() const
    {
        std::cout << "=== " << name << " ===\n";
        std::cout << "Level: " << level << "\n";
        std::cout << "HP: " << health << "/" << maxHealth << "\n";
    }

    std::string getName() const { return name; }
};

class Warrior : public Character
{
private:
    int armor;

public:
    Warrior(const std::string& n, int lvl)
        : Character(n, 150 + lvl * 20, lvl), armor(10 + lvl * 2) {}

    void attack(Character& target) override
    {
        int damage = 20 + level * 5;
        std::cout << name << " swings sword at " << target.getName() << "!\n";
        target.takeDamage(damage);
    }

    void specialAbility() override
    {
        std::cout << name << " uses Shield Block! Armor increased temporarily.\n";
        armor += 10;
    }

    void displayStats() const override
    {
        Character::displayStats();
        std::cout << "Armor: " << armor << "\n";
    }
};

class Mage : public Character
{
private:
    int mana;
    int maxMana;

public:
    Mage(const std::string& n, int lvl)
        : Character(n, 80 + lvl * 10, lvl),
          mana(100 + lvl * 15),
          maxMana(100 + lvl * 15) {}

    void attack(Character& target) override
    {
        if (mana >= 10)
        {
            int damage = 25 + level * 7;
            mana -= 10;
            std::cout << name << " casts Fireball at " << target.getName() << "!\n";
            target.takeDamage(damage);
        }
        else
        {
            std::cout << name << " is out of mana!\n";
        }
    }

    void specialAbility() override
    {
        if (mana >= 50)
        {
            mana -= 50;
            int healAmount = 30 + level * 5;
            std::cout << name << " casts Healing Spell!\n";
            heal(healAmount);
        }
        else
        {
            std::cout << name << " doesn't have enough mana!\n";
        }
    }

    void displayStats() const override
    {
        Character::displayStats();
        std::cout << "Mana: " << mana << "/" << maxMana << "\n";
    }
};

class Rogue : public Character
{
private:
    int criticalChance;

public:
    Rogue(const std::string& n, int lvl)
        : Character(n, 100 + lvl * 15, lvl),
          criticalChance(20 + lvl * 3) {}

    void attack(Character& target) override
    {
        int damage = 15 + level * 4;

        if (rand() % 100 < criticalChance)
        {
            damage *= 2;
            std::cout << name << " performs a CRITICAL backstab on "
                      << target.getName() << "!\n";
        }
        else
        {
            std::cout << name << " stabs " << target.getName() << "!\n";
        }

        target.takeDamage(damage);
    }

    void specialAbility() override
    {
        std::cout << name << " uses Stealth! Next attack guaranteed critical.\n";
        criticalChance = 100;  // Guaranteed crit
    }

    void displayStats() const override
    {
        Character::displayStats();
        std::cout << "Critical Chance: " << criticalChance << "%\n";
    }
};

int main()
{
    std::vector<Character*> party;

    party.push_back(new Warrior("Aragorn", 5));
    party.push_back(new Mage("Gandalf", 5));
    party.push_back(new Rogue("Legolas", 5));

    std::cout << "=== Party Members ===\n\n";

    for (const auto& character : party)
    {
        character->displayStats();
        std::cout << "\n";
    }

    std::cout << "=== Combat Simulation ===\n\n";

    party[0]->attack(*party[1]);  // Warrior attacks Mage
    party[2]->attack(*party[0]);  // Rogue attacks Warrior
    party[1]->specialAbility();   // Mage heals

    // Cleanup
    for (auto& character : party)
    {
        delete character;
    }

    return 0;
}
```

### Example 4: File System Hierarchy

```cpp
#include <iostream>
#include <string>
#include <vector>

class FileSystemItem
{
protected:
    std::string name;
    int size;

public:
    FileSystemItem(const std::string& n, int s)
        : name(n), size(s) {}

    virtual ~FileSystemItem() = default;

    virtual void display(int indent = 0) const = 0;
    virtual int getSize() const = 0;

    std::string getName() const { return name; }
};

class File : public FileSystemItem
{
private:
    std::string extension;

public:
    File(const std::string& n, const std::string& ext, int s)
        : FileSystemItem(n, s), extension(ext) {}

    void display(int indent = 0) const override
    {
        std::string spaces(indent, ' ');
        std::cout << spaces << "File: " << name << "." << extension
                  << " (" << size << " bytes)\n";
    }

    int getSize() const override
    {
        return size;
    }
};

class Directory : public FileSystemItem
{
private:
    std::vector<FileSystemItem*> contents;

public:
    Directory(const std::string& n)
        : FileSystemItem(n, 0) {}

    ~Directory()
    {
        for (auto item : contents)
        {
            delete item;
        }
    }

    void addItem(FileSystemItem* item)
    {
        contents.push_back(item);
    }

    void display(int indent = 0) const override
    {
        std::string spaces(indent, ' ');
        std::cout << spaces << "Directory: " << name
                  << " (" << getSize() << " bytes)\n";

        for (const auto& item : contents)
        {
            item->display(indent + 2);
        }
    }

    int getSize() const override
    {
        int totalSize = 0;
        for (const auto& item : contents)
        {
            totalSize += item->getSize();
        }
        return totalSize;
    }
};

int main()
{
    Directory* root = new Directory("root");

    Directory* docs = new Directory("Documents");
    docs->addItem(new File("report", "pdf", 1024000));
    docs->addItem(new File("notes", "txt", 5000));

    Directory* pics = new Directory("Pictures");
    pics->addItem(new File("vacation", "jpg", 3000000));
    pics->addItem(new File("family", "png", 2500000));

    Directory* code = new Directory("Code");
    code->addItem(new File("main", "cpp", 15000));
    code->addItem(new File("utils", "h", 3000));

    root->addItem(docs);
    root->addItem(pics);
    root->addItem(code);
    root->addItem(new File("readme", "md", 2000));

    std::cout << "=== File System ===\n\n";
    root->display();

    delete root;

    return 0;
}
```

### Example 5: Shape Drawing Application

```cpp
#include <iostream>
#include <vector>
#include <cmath>

class Point
{
public:
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Shape
{
protected:
    Point position;
    std::string color;

public:
    Shape(const Point& pos, const std::string& col)
        : position(pos), color(col) {}

    virtual ~Shape() = default;

    virtual void draw() const = 0;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    void move(double dx, double dy)
    {
        position.x += dx;
        position.y += dy;
    }

    virtual void displayInfo() const
    {
        std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
        std::cout << "Color: " << color << "\n";
        std::cout << "Area: " << area() << "\n";
        std::cout << "Perimeter: " << perimeter() << "\n";
    }
};

class Circle : public Shape
{
private:
    double radius;

public:
    Circle(const Point& pos, const std::string& col, double r)
        : Shape(pos, col), radius(r) {}

    void draw() const override
    {
        std::cout << "Drawing circle at (" << position.x << ", "
                  << position.y << ") with radius " << radius << "\n";
    }

    double area() const override
    {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override
    {
        return 2 * 3.14159 * radius;
    }

    void displayInfo() const override
    {
        std::cout << "=== Circle ===\n";
        Shape::displayInfo();
        std::cout << "Radius: " << radius << "\n";
    }
};

class Rectangle : public Shape
{
private:
    double width, height;

public:
    Rectangle(const Point& pos, const std::string& col, double w, double h)
        : Shape(pos, col), width(w), height(h) {}

    void draw() const override
    {
        std::cout << "Drawing rectangle at (" << position.x << ", "
                  << position.y << ") with size " << width << "x" << height << "\n";
    }

    double area() const override
    {
        return width * height;
    }

    double perimeter() const override
    {
        return 2 * (width + height);
    }

    void displayInfo() const override
    {
        std::cout << "=== Rectangle ===\n";
        Shape::displayInfo();
        std::cout << "Size: " << width << " x " << height << "\n";
    }
};

class Triangle : public Shape
{
private:
    double base, height;

public:
    Triangle(const Point& pos, const std::string& col, double b, double h)
        : Shape(pos, col), base(b), height(h) {}

    void draw() const override
    {
        std::cout << "Drawing triangle at (" << position.x << ", "
                  << position.y << ") with base " << base
                  << " and height " << height << "\n";
    }

    double area() const override
    {
        return 0.5 * base * height;
    }

    double perimeter() const override
    {
        // Simplified (assuming right triangle)
        double hypotenuse = std::sqrt(base * base + height * height);
        return base + height + hypotenuse;
    }

    void displayInfo() const override
    {
        std::cout << "=== Triangle ===\n";
        Shape::displayInfo();
        std::cout << "Base: " << base << ", Height: " << height << "\n";
    }
};

int main()
{
    std::vector<Shape*> shapes;

    shapes.push_back(new Circle(Point(100, 100), "Red", 50));
    shapes.push_back(new Rectangle(Point(200, 150), "Blue", 80, 60));
    shapes.push_back(new Triangle(Point(300, 200), "Green", 70, 90));

    std::cout << "=== Drawing All Shapes ===\n\n";

    for (const auto& shape : shapes)
    {
        shape->draw();
    }

    std::cout << "\n=== Shape Information ===\n\n";

    for (const auto& shape : shapes)
    {
        shape->displayInfo();
        std::cout << "\n";
    }

    double totalArea = 0;
    for (const auto& shape : shapes)
    {
        totalArea += shape->area();
    }

    std::cout << "Total area of all shapes: " << totalArea << "\n";

    // Cleanup
    for (auto& shape : shapes)
    {
        delete shape;
    }

    return 0;
}
```

## Common Mistakes

### 1. Forgetting Virtual Destructor

```cpp
// WRONG
class Base
{
public:
    ~Base() { }  // Not virtual!
};

class Derived : public Base
{
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; }
};

Base* ptr = new Derived();
delete ptr;  // Only Base destructor called - MEMORY LEAK!

// CORRECT
class Base
{
public:
    virtual ~Base() { }  // Virtual destructor
};
```

### 2. Slicing Problem

```cpp
// WRONG - Object slicing
class Base
{
    int x;
};

class Derived : public Base
{
    int y;  // This gets sliced off!
};

Derived d;
Base b = d;  // Slicing! Only Base part copied

// CORRECT - Use pointers or references
Base* ptr = &d;  // No slicing
Base& ref = d;   // No slicing
```

### 3. Not Using override Keyword

```cpp
// WRONG - Typo not caught
class Base
{
public:
    virtual void display() const { }
};

class Derived : public Base
{
public:
    void display()  // Missing const - doesn't actually override!
    {
    }
};

// CORRECT - Use override keyword
class Derived : public Base
{
public:
    void display() const override  // Compiler error if not actually overriding
    {
    }
};
```

### 4. Calling Virtual Function in Constructor

```cpp
// WRONG
class Base
{
public:
    Base()
    {
        init();  // Calls Base::init(), not Derived::init()!
    }

    virtual void init() { }
};

class Derived : public Base
{
public:
    void init() override
    {
        // Never called from Base constructor!
    }
};

// CORRECT - Don't call virtual functions in constructor
// Or use a factory pattern
```

### 5. Not Implementing All Pure Virtual Functions

```cpp
// WRONG
class Base
{
public:
    virtual void func1() = 0;
    virtual void func2() = 0;
};

class Derived : public Base
{
public:
    void func1() override { }
    // Missing func2()!  Derived is still abstract!
};

// CORRECT - Implement all pure virtual functions
class Derived : public Base
{
public:
    void func1() override { }
    void func2() override { }  // Now concrete
};
```

## Exercises

### Exercise 1: Employee Management System
Create a complete employee hierarchy:
- Abstract Employee base class
- HourlyEmployee, SalariedEmployee, CommissionEmployee
- Calculate pay for each type differently
- Display employee information polymorphically
- Bonus system based on performance

### Exercise 2: Geometric Shapes
Implement a comprehensive shape system:
- Abstract Shape base class
- Concrete shapes: Circle, Rectangle, Triangle, Polygon
- Pure virtual: area(), perimeter(), draw()
- Add: rotate(), scale(), translate()
- Calculate total area of collection of shapes

### Exercise 3: Media Library
Build a media library system:
- Abstract Media base class
- Book, Movie, Music, Podcast classes
- Properties: title, creator, duration/pages
- Methods: play/read, display info
- Search and filter capabilities

### Exercise 4: RPG Character System
Create an RPG character system:
- Abstract Character base
- Warrior, Mage, Rogue, Healer classes
- Combat system with different attack types
- Leveling and experience system
- Equipment and inventory management

### Exercise 5: Transportation System
Design a transportation hierarchy:
- Abstract Vehicle base
- Land (Car, Bicycle), Water (Boat), Air (Plane)
- Calculate travel time, fuel consumption
- Passenger capacity
- Polymorphic travel simulation

## Summary

You learned:
- ✅ Inheritance basics and benefits
- ✅ Protected members and access control
- ✅ Types of inheritance (public, protected, private)
- ✅ Function overriding
- ✅ Virtual functions for runtime polymorphism
- ✅ Pure virtual functions and abstract classes
- ✅ Polymorphism and its applications
- ✅ Complete OOP design patterns

## Next Steps

Congratulations on completing Module 1! You now have a solid foundation in C++ fundamentals.

**What's Next:**
1. Practice building complete programs using OOP
2. Explore the Standard Template Library (STL)
3. Learn advanced C++ features (templates, exceptions, etc.)
4. Start Module 2: Windows API Programming
5. Build real-world applications

**Recommended Practice:**
- Build a complete application combining all concepts
- Read other people's C++ code
- Contribute to open-source projects
- Solve competitive programming problems

---

**Congratulations on completing Lesson 10 and Module 1: C++ Fundamentals!**

**Practice time**: Spend 4+ hours designing class hierarchies and implementing polymorphic systems. OOP mastery is essential for professional C++ development. Build complete applications to solidify your understanding!
