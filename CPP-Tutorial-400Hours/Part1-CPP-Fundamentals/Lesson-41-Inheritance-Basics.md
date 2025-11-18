# Lesson 41: Inheritance Basics

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. What is Inheritance?
2. Base and Derived Classes
3. Access Control in Inheritance
4. Constructor and Destructor Order
5. Member Function Overriding
6. Protected Members
7. Real-World Examples
8. Exercises
9. Practice Projects

---

## 1. What is Inheritance?

**Inheritance** allows a class to inherit properties and methods from another class, promoting code reuse and establishing relationships.

### Key Concepts:

- **Base Class** (Parent/Super class): The class being inherited from
- **Derived Class** (Child/Sub class): The class that inherits
- **"IS-A" Relationship**: Derived class IS-A type of base class

### Benefits:
1. Code reuse
2. Extensibility
3. Polymorphism support
4. Hierarchical organization

### Syntax:
```cpp
class Base {
    // Base class members
};

class Derived : public Base {
    // Derived class members + inherited members
};
```

---

## 2. Base and Derived Classes

### Example 1: Simple Inheritance

```cpp
#include <iostream>
#include <string>
using namespace std;

// Base class
class Animal {
protected:
    string name;
    int age;

public:
    Animal(const string& n, int a) : name(n), age(a) {
        cout << "Animal constructor: " << name << endl;
    }

    void eat() {
        cout << name << " is eating" << endl;
    }

    void sleep() {
        cout << name << " is sleeping" << endl;
    }

    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << " years" << endl;
    }
};

// Derived class
class Dog : public Animal {
private:
    string breed;

public:
    Dog(const string& n, int a, const string& b)
        : Animal(n, a), breed(b) {
        cout << "Dog constructor: " << name << endl;
    }

    void bark() {
        cout << name << " says: Woof! Woof!" << endl;
    }

    void displayDogInfo() {
        displayInfo();  // Inherited method
        cout << "Breed: " << breed << endl;
    }
};

int main() {
    Dog myDog("Buddy", 3, "Golden Retriever");

    myDog.eat();    // Inherited from Animal
    myDog.sleep();  // Inherited from Animal
    myDog.bark();   // Dog's own method

    cout << "\nDog Information:" << endl;
    myDog.displayDogInfo();

    return 0;
}
```

**Output**:
```
Animal constructor: Buddy
Dog constructor: Buddy
Buddy is eating
Buddy is sleeping
Buddy says: Woof! Woof!

Dog Information:
Name: Buddy
Age: 3 years
Breed: Golden Retriever
```

### Example 2: Multiple Levels of Inheritance

```cpp
#include <iostream>
#include <string>
using namespace std;

// Level 1: Base class
class Vehicle {
protected:
    string brand;
    int year;

public:
    Vehicle(const string& b, int y) : brand(b), year(y) {
        cout << "Vehicle created: " << brand << endl;
    }

    void start() {
        cout << brand << " is starting..." << endl;
    }

    void stop() {
        cout << brand << " has stopped" << endl;
    }
};

// Level 2: Derived from Vehicle
class Car : public Vehicle {
protected:
    int numDoors;

public:
    Car(const string& b, int y, int doors)
        : Vehicle(b, y), numDoors(doors) {
        cout << "Car created with " << numDoors << " doors" << endl;
    }

    void honk() {
        cout << "Beep! Beep!" << endl;
    }
};

// Level 3: Derived from Car
class SportsCar : public Car {
private:
    int topSpeed;

public:
    SportsCar(const string& b, int y, int doors, int speed)
        : Car(b, y, doors), topSpeed(speed) {
        cout << "SportsCar with top speed " << topSpeed << " km/h" << endl;
    }

    void turboBoost() {
        cout << "Turbo boost activated! Maximum speed: "
             << topSpeed << " km/h!" << endl;
    }

    void displayInfo() {
        cout << "\n=== Sports Car Info ===" << endl;
        cout << "Brand: " << brand << endl;
        cout << "Year: " << year << endl;
        cout << "Doors: " << numDoors << endl;
        cout << "Top Speed: " << topSpeed << " km/h" << endl;
    }
};

int main() {
    SportsCar ferrari("Ferrari", 2024, 2, 340);

    ferrari.start();        // From Vehicle
    ferrari.honk();         // From Car
    ferrari.turboBoost();   // From SportsCar
    ferrari.displayInfo();  // From SportsCar
    ferrari.stop();         // From Vehicle

    return 0;
}
```

---

## 3. Access Control in Inheritance

### Three Types of Inheritance:

```cpp
class Derived : public Base {};     // Most common
class Derived : protected Base {};  // Rare
class Derived : private Base {};    // Default for class
```

### Access Table:

| Base Member | public inheritance | protected inheritance | private inheritance |
|-------------|-------------------|----------------------|---------------------|
| public      | public            | protected            | private             |
| protected   | protected         | protected            | private             |
| private     | not accessible    | not accessible       | not accessible      |

### Example: Inheritance Modes

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    int publicVar;

protected:
    int protectedVar;

private:
    int privateVar;

public:
    Base() : publicVar(1), protectedVar(2), privateVar(3) {}
};

// Public inheritance
class PublicDerived : public Base {
public:
    void access() {
        publicVar = 10;      // OK: public in derived
        protectedVar = 20;   // OK: protected in derived
        // privateVar = 30;  // ERROR: not accessible
    }
};

// Protected inheritance
class ProtectedDerived : protected Base {
public:
    void access() {
        publicVar = 10;      // OK: protected in derived
        protectedVar = 20;   // OK: protected in derived
        // privateVar = 30;  // ERROR: not accessible
    }
};

// Private inheritance
class PrivateDerived : private Base {
public:
    void access() {
        publicVar = 10;      // OK: private in derived
        protectedVar = 20;   // OK: private in derived
        // privateVar = 30;  // ERROR: not accessible
    }
};

int main() {
    PublicDerived pd;
    pd.publicVar = 100;      // OK: public member

    ProtectedDerived protd;
    // protd.publicVar = 100;  // ERROR: protected in derived

    PrivateDerived privd;
    // privd.publicVar = 100;   // ERROR: private in derived

    return 0;
}
```

---

## 4. Constructor and Destructor Order

### Rules:
1. **Construction**: Base → Derived
2. **Destruction**: Derived → Base (reverse order)

### Example: Order Demonstration

```cpp
#include <iostream>
using namespace std;

class Grandparent {
public:
    Grandparent() {
        cout << "1. Grandparent constructor" << endl;
    }

    ~Grandparent() {
        cout << "6. Grandparent destructor" << endl;
    }
};

class Parent : public Grandparent {
public:
    Parent() {
        cout << "2. Parent constructor" << endl;
    }

    ~Parent() {
        cout << "5. Parent destructor" << endl;
    }
};

class Child : public Parent {
public:
    Child() {
        cout << "3. Child constructor" << endl;
    }

    ~Child() {
        cout << "4. Child destructor" << endl;
    }
};

int main() {
    cout << "Creating Child object:" << endl;
    Child c;

    cout << "\nExiting main (destruction starts):" << endl;
    return 0;
}
```

**Output**:
```
Creating Child object:
1. Grandparent constructor
2. Parent constructor
3. Child constructor

Exiting main (destruction starts):
4. Child destructor
5. Parent destructor
6. Grandparent destructor
```

### Example: Passing Arguments to Base Constructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int age;

public:
    Person(const string& n, int a) : name(n), age(a) {
        cout << "Person constructor: " << name << ", " << age << endl;
    }
};

class Student : public Person {
private:
    int studentID;
    double gpa;

public:
    // Must call base constructor explicitly
    Student(const string& n, int a, int id, double g)
        : Person(n, a), studentID(id), gpa(g) {
        cout << "Student constructor: ID " << studentID << ", GPA " << gpa << endl;
    }

    void display() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Student ID: " << studentID << endl;
        cout << "GPA: " << gpa << endl;
    }
};

int main() {
    Student student("Alice", 20, 12345, 3.8);
    student.display();

    return 0;
}
```

---

## 5. Member Function Overriding

Derived classes can override base class methods.

### Example: Method Overriding

```cpp
#include <iostream>
#include <string>
using namespace std;

class Shape {
protected:
    string color;

public:
    Shape(const string& c) : color(c) {}

    void displayColor() {
        cout << "Color: " << color << endl;
    }

    // This method will be overridden
    void draw() {
        cout << "Drawing a generic shape" << endl;
    }

    void info() {
        cout << "This is a shape" << endl;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(const string& c, double r) : Shape(c), radius(r) {}

    // Override draw method
    void draw() {
        cout << "Drawing a circle with radius " << radius << endl;
    }

    // Can still call base class version
    void drawWithBase() {
        Shape::draw();  // Call base class draw
        draw();         // Call derived class draw
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(const string& c, double w, double h)
        : Shape(c), width(w), height(h) {}

    // Override draw method
    void draw() {
        cout << "Drawing a rectangle " << width << "x" << height << endl;
    }
};

int main() {
    Circle circle("Red", 5.0);
    Rectangle rect("Blue", 10.0, 5.0);

    circle.displayColor();  // Inherited
    circle.draw();          // Overridden

    cout << endl;

    rect.displayColor();    // Inherited
    rect.draw();            // Overridden

    cout << "\nCalling both versions:" << endl;
    circle.drawWithBase();

    return 0;
}
```

---

## 6. Protected Members

**Protected members** are accessible in derived classes but not from outside.

### Example: Protected Members in Action

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
protected:
    double balance;  // Protected: accessible in derived classes

private:
    string accountNumber;  // Private: not accessible in derived

public:
    BankAccount(const string& accNum, double bal)
        : accountNumber(accNum), balance(bal) {}

    void displayBalance() {
        cout << "Account: " << accountNumber << endl;
        cout << "Balance: $" << balance << endl;
    }

protected:
    // Protected method for derived classes
    void addInterest(double rate) {
        balance += balance * rate;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(const string& accNum, double bal, double rate)
        : BankAccount(accNum, bal), interestRate(rate) {}

    void applyMonthlyInterest() {
        // Can access protected member 'balance'
        cout << "Applying " << (interestRate * 100) << "% interest" << endl;
        addInterest(interestRate);  // Can call protected method
    }

    void deposit(double amount) {
        balance += amount;  // Can modify protected member
        cout << "Deposited: $" << amount << endl;
    }
};

int main() {
    SavingsAccount savings("SAV12345", 1000.0, 0.05);

    savings.displayBalance();

    savings.deposit(500);
    savings.applyMonthlyInterest();

    cout << "\nAfter operations:" << endl;
    savings.displayBalance();

    // savings.balance = 5000;  // ERROR: protected, not accessible

    return 0;
}
```

---

## 7. Real-World Examples

### Example: Employee Management System

```cpp
#include <iostream>
#include <string>
using namespace std;

class Employee {
protected:
    int employeeID;
    string name;
    double baseSalary;

public:
    Employee(int id, const string& n, double salary)
        : employeeID(id), name(n), baseSalary(salary) {}

    virtual double calculatePay() {
        return baseSalary;
    }

    void displayInfo() {
        cout << "ID: " << employeeID << endl;
        cout << "Name: " << name << endl;
        cout << "Base Salary: $" << baseSalary << endl;
    }
};

class Manager : public Employee {
private:
    double bonus;
    int teamSize;

public:
    Manager(int id, const string& n, double salary, double b, int team)
        : Employee(id, n, salary), bonus(b), teamSize(team) {}

    double calculatePay() {
        return baseSalary + bonus;
    }

    void displayManagerInfo() {
        displayInfo();
        cout << "Bonus: $" << bonus << endl;
        cout << "Team Size: " << teamSize << endl;
        cout << "Total Pay: $" << calculatePay() << endl;
    }
};

class Developer : public Employee {
private:
    string programmingLanguage;
    int projectsCompleted;

public:
    Developer(int id, const string& n, double salary,
              const string& lang, int projects)
        : Employee(id, n, salary),
          programmingLanguage(lang),
          projectsCompleted(projects) {}

    double calculatePay() {
        return baseSalary + (projectsCompleted * 100);  // Bonus per project
    }

    void displayDeveloperInfo() {
        displayInfo();
        cout << "Language: " << programmingLanguage << endl;
        cout << "Projects Completed: " << projectsCompleted << endl;
        cout << "Total Pay: $" << calculatePay() << endl;
    }
};

int main() {
    Manager manager(101, "Alice Johnson", 80000, 20000, 5);
    Developer dev(102, "Bob Smith", 70000, "C++", 12);

    cout << "=== Manager Information ===" << endl;
    manager.displayManagerInfo();

    cout << "\n=== Developer Information ===" << endl;
    dev.displayDeveloperInfo();

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Vehicle Hierarchy
```cpp
class Vehicle {
    // Add: brand, year, start(), stop()
};

class Car : public Vehicle {
    // Add: numDoors, honk()
};

class Motorcycle : public Vehicle {
    // Add: hasSidecar, wheelie()
};
```

### Exercise 2: Academic System
```cpp
class Person {
    // name, age, dateOfBirth
};

class Student : public Person {
    // studentID, gpa, courses[]
};

class Teacher : public Person {
    // employeeID, salary, subject
};
```

### Exercise 3: Shape Hierarchy
```cpp
class Shape {
    // color, calculateArea(), calculatePerimeter()
};

class Rectangle : public Shape {
    // width, height
};

class Triangle : public Shape {
    // base, height, side1, side2, side3
};
```

---

## 9. Practice Projects

### Project: Complete Library System

```cpp
#include <iostream>
#include <string>
using namespace std;

class LibraryItem {
protected:
    string title;
    string itemID;
    bool isCheckedOut;
    int checkoutDuration;

public:
    LibraryItem(const string& t, const string& id, int duration)
        : title(t), itemID(id), isCheckedOut(false),
          checkoutDuration(duration) {}

    virtual void displayInfo() {
        cout << "Title: " << title << endl;
        cout << "ID: " << itemID << endl;
        cout << "Status: " << (isCheckedOut ? "Checked Out" : "Available") << endl;
        cout << "Checkout Duration: " << checkoutDuration << " days" << endl;
    }

    virtual double calculateLateFee(int daysLate) {
        return daysLate * 0.50;  // $0.50 per day default
    }

    void checkOut() {
        if (!isCheckedOut) {
            isCheckedOut = true;
            cout << title << " checked out successfully!" << endl;
        } else {
            cout << title << " is already checked out!" << endl;
        }
    }

    void returnItem() {
        if (isCheckedOut) {
            isCheckedOut = false;
            cout << title << " returned successfully!" << endl;
        } else {
            cout << title << " was not checked out!" << endl;
        }
    }
};

class Book : public LibraryItem {
private:
    string author;
    string ISBN;
    int pageCount;

public:
    Book(const string& t, const string& id, const string& auth,
         const string& isbn, int pages)
        : LibraryItem(t, id, 14), author(auth), ISBN(isbn), pageCount(pages) {}

    void displayInfo() override {
        cout << "\n=== BOOK ===" << endl;
        LibraryItem::displayInfo();
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Pages: " << pageCount << endl;
    }

    double calculateLateFee(int daysLate) override {
        return daysLate * 0.25;  // Books: $0.25 per day
    }
};

class DVD : public LibraryItem {
private:
    string director;
    int runtime;  // in minutes

public:
    DVD(const string& t, const string& id, const string& dir, int time)
        : LibraryItem(t, id, 7), director(dir), runtime(time) {}

    void displayInfo() override {
        cout << "\n=== DVD ===" << endl;
        LibraryItem::displayInfo();
        cout << "Director: " << director << endl;
        cout << "Runtime: " << runtime << " minutes" << endl;
    }

    double calculateLateFee(int daysLate) override {
        return daysLate * 1.00;  // DVDs: $1.00 per day
    }
};

class Magazine : public LibraryItem {
private:
    string publisher;
    int issueNumber;

public:
    Magazine(const string& t, const string& id, const string& pub, int issue)
        : LibraryItem(t, id, 7), publisher(pub), issueNumber(issue) {}

    void displayInfo() override {
        cout << "\n=== MAGAZINE ===" << endl;
        LibraryItem::displayInfo();
        cout << "Publisher: " << publisher << endl;
        cout << "Issue Number: " << issueNumber << endl;
    }

    double calculateLateFee(int daysLate) override {
        return daysLate * 0.10;  // Magazines: $0.10 per day
    }
};

int main() {
    Book book("C++ Programming", "B001", "Bjarne Stroustrup",
              "978-0321563842", 1376);
    DVD dvd("The Matrix", "D001", "Wachowski Brothers", 136);
    Magazine mag("National Geographic", "M001", "Nat Geo", 245);

    book.displayInfo();
    dvd.displayInfo();
    mag.displayInfo();

    cout << "\n=== Checkout Operations ===" << endl;
    book.checkOut();
    dvd.checkOut();

    cout << "\n=== Late Fee Calculation ===" << endl;
    cout << "Book late fee (5 days): $" << book.calculateLateFee(5) << endl;
    cout << "DVD late fee (5 days): $" << dvd.calculateLateFee(5) << endl;
    cout << "Magazine late fee (5 days): $" << mag.calculateLateFee(5) << endl;

    cout << "\n=== Return Operations ===" << endl;
    book.returnItem();
    dvd.returnItem();

    return 0;
}
```

---

## Key Takeaways

1. **Inheritance** enables code reuse and extensibility
2. **Base class** → **Derived class** relationship
3. **public inheritance** is most common
4. **protected members** accessible in derived classes
5. **Constructor order**: Base → Derived
6. **Destructor order**: Derived → Base
7. **Method overriding** allows customization
8. **IS-A relationship**: Derived IS-A Base

---

## Summary Checklist

Before moving to Lesson 42, ensure you can:
- [ ] Create base and derived classes
- [ ] Use public, protected, and private inheritance
- [ ] Understand constructor/destructor order
- [ ] Override base class methods
- [ ] Use protected members appropriately
- [ ] Pass arguments to base constructors
- [ ] Design inheritance hierarchies

---

## Next Lesson Preview

In **Lesson 42: Types of Inheritance**, we'll learn:
- Single inheritance
- Multiple inheritance
- Multilevel inheritance
- Hierarchical inheritance
- Hybrid inheritance
- Diamond problem

Excellent! You now understand inheritance fundamentals!
