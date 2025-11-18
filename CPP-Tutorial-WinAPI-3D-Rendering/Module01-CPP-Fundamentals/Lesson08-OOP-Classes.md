# Lesson 08: Object-Oriented Programming - Classes
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to OOP](#introduction-to-oop)
2. [Classes and Objects](#classes-and-objects)
3. [Member Variables](#member-variables)
4. [Member Functions](#member-functions)
5. [Access Specifiers](#access-specifiers)
6. [Encapsulation](#encapsulation)
7. [Getters and Setters](#getters-and-setters)
8. [Const Member Functions](#const-member-functions)
9. [Static Members](#static-members)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to OOP

Object-Oriented Programming (OOP) is a programming paradigm based on objects that contain data and code.

### Four Pillars of OOP:

1. **Encapsulation**: Bundle data and methods that operate on that data
2. **Abstraction**: Hide complex implementation details
3. **Inheritance**: Create new classes from existing ones
4. **Polymorphism**: Objects can take many forms

### Why OOP?

**Procedural approach:**
```cpp
struct BankAccount
{
    std::string owner;
    double balance;
};

void deposit(BankAccount& account, double amount)
{
    account.balance += amount;
}

void withdraw(BankAccount& account, double amount)
{
    account.balance -= amount;
}
```

**OOP approach:**
```cpp
class BankAccount
{
private:
    std::string owner;
    double balance;

public:
    void deposit(double amount)
    {
        balance += amount;
    }

    void withdraw(double amount)
    {
        balance -= amount;
    }
};
```

### Benefits:
- **Modularity**: Code organized into logical units
- **Reusability**: Classes can be reused
- **Maintainability**: Easier to update and fix
- **Security**: Data hiding through encapsulation

## Classes and Objects

### Defining a Class

```cpp
#include <iostream>
#include <string>

class Person
{
public:
    std::string name;
    int age;

    void introduce()
    {
        std::cout << "Hi, I'm " << name << " and I'm " << age << " years old.\n";
    }
};

int main()
{
    // Create object
    Person person1;
    person1.name = "Alice";
    person1.age = 25;
    person1.introduce();

    // Create another object
    Person person2;
    person2.name = "Bob";
    person2.age = 30;
    person2.introduce();

    return 0;
}
```

### Class vs Object

- **Class**: Blueprint/template for creating objects
- **Object**: Instance of a class (actual entity in memory)

```cpp
class Car  // Class (blueprint)
{
public:
    std::string brand;
    int year;
};

int main()
{
    Car car1;  // Object 1 (instance)
    Car car2;  // Object 2 (instance)
    Car car3;  // Object 3 (instance)

    // Each object has its own copy of member variables
    car1.brand = "Toyota";
    car2.brand = "Honda";
    car3.brand = "Ford";

    return 0;
}
```

### Class Declaration and Definition

```cpp
// Method 1: Everything in class
class Rectangle
{
public:
    double width;
    double height;

    double area()  // Definition inside class
    {
        return width * height;
    }
};

// Method 2: Declaration in class, definition outside
class Circle
{
public:
    double radius;

    double area();  // Declaration only
};

// Definition outside class
double Circle::area()
{
    return 3.14159 * radius * radius;
}
```

## Member Variables

### Data Members

```cpp
#include <iostream>
#include <string>

class Student
{
public:
    // Member variables (data members)
    std::string name;
    int id;
    double gpa;
    int credits;

    void display()
    {
        std::cout << "Name: " << name << "\n";
        std::cout << "ID: " << id << "\n";
        std::cout << "GPA: " << gpa << "\n";
        std::cout << "Credits: " << credits << "\n";
    }
};

int main()
{
    Student student;
    student.name = "Alice Johnson";
    student.id = 12345;
    student.gpa = 3.8;
    student.credits = 90;

    student.display();

    return 0;
}
```

### Initializing Member Variables

```cpp
#include <iostream>

class Point
{
public:
    int x;
    int y;

    // Method 1: Initialize in member function
    void set(int newX, int newY)
    {
        x = newX;
        y = newY;
    }

    void display()
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1;
    p1.set(10, 20);
    p1.display();

    // Direct access (if public)
    Point p2;
    p2.x = 30;
    p2.y = 40;
    p2.display();

    return 0;
}
```

### Different Data Types

```cpp
#include <iostream>
#include <string>
#include <vector>

class Employee
{
public:
    // Basic types
    std::string name;
    int age;
    double salary;
    bool isFullTime;

    // Arrays
    int ratings[5];

    // STL containers
    std::vector<std::string> skills;

    // Pointers
    Employee* manager;
};
```

## Member Functions

### Methods (Member Functions)

```cpp
#include <iostream>

class Calculator
{
public:
    int add(int a, int b)
    {
        return a + b;
    }

    int subtract(int a, int b)
    {
        return a - b;
    }

    int multiply(int a, int b)
    {
        return a * b;
    }

    double divide(int a, int b)
    {
        if (b != 0)
            return static_cast<double>(a) / b;
        return 0.0;
    }
};

int main()
{
    Calculator calc;

    std::cout << "10 + 5 = " << calc.add(10, 5) << "\n";
    std::cout << "10 - 5 = " << calc.subtract(10, 5) << "\n";
    std::cout << "10 * 5 = " << calc.multiply(10, 5) << "\n";
    std::cout << "10 / 5 = " << calc.divide(10, 5) << "\n";

    return 0;
}
```

### Accessing Member Variables

```cpp
#include <iostream>
#include <string>

class Book
{
public:
    std::string title;
    std::string author;
    int pages;

    // Member function accessing member variables
    void display()
    {
        std::cout << "Title: " << title << "\n";
        std::cout << "Author: " << author << "\n";
        std::cout << "Pages: " << pages << "\n";
    }

    bool isLong()
    {
        return pages > 500;
    }

    void addPages(int additional)
    {
        pages += additional;
    }
};

int main()
{
    Book book;
    book.title = "C++ Programming";
    book.author = "Bjarne Stroustrup";
    book.pages = 1376;

    book.display();

    if (book.isLong())
    {
        std::cout << "This is a long book!\n";
    }

    book.addPages(50);
    std::cout << "After adding pages: " << book.pages << "\n";

    return 0;
}
```

### The this Pointer

```cpp
#include <iostream>

class Counter
{
public:
    int count;

    void set(int count)  // Parameter shadows member variable
    {
        // Use 'this' to disambiguate
        this->count = count;
    }

    void increment()
    {
        this->count++;  // 'this->' optional here but explicit
    }

    Counter* getThis()
    {
        return this;  // Return pointer to current object
    }
};

int main()
{
    Counter c;
    c.set(10);
    c.increment();

    std::cout << "Count: " << c.count << "\n";
    std::cout << "Address of c: " << &c << "\n";
    std::cout << "this pointer: " << c.getThis() << "\n";

    return 0;
}
```

## Access Specifiers

### public, private, protected

```cpp
#include <iostream>
#include <string>

class BankAccount
{
public:
    // Public members: accessible from anywhere
    std::string accountHolder;

    void deposit(double amount)
    {
        balance += amount;
    }

    void displayBalance()
    {
        std::cout << "Balance: $" << balance << "\n";
    }

private:
    // Private members: only accessible within class
    double balance;
    std::string accountNumber;

    void logTransaction(std::string type, double amount)
    {
        std::cout << "[LOG] " << type << ": $" << amount << "\n";
    }

protected:
    // Protected members: accessible in derived classes (covered later)
    int accountType;
};

int main()
{
    BankAccount account;

    // OK: public member
    account.accountHolder = "Alice";
    account.deposit(1000);
    account.displayBalance();

    // ERROR: private members not accessible
    // account.balance = 999999;  // Compile error!
    // account.accountNumber = "123";  // Compile error!

    return 0;
}
```

### Default Access

```cpp
// class: default is private
class MyClass
{
    int x;  // private by default
public:
    int y;  // public
};

// struct: default is public
struct MyStruct
{
    int x;  // public by default
private:
    int y;  // private
};
```

### Why Use Private?

```cpp
#include <iostream>

class Temperature
{
private:
    double celsius;  // Internal representation

public:
    // Controlled access ensures valid data
    void setCelsius(double temp)
    {
        if (temp < -273.15)
        {
            std::cout << "Error: Temperature below absolute zero!\n";
            celsius = -273.15;
        }
        else
        {
            celsius = temp;
        }
    }

    double getCelsius()
    {
        return celsius;
    }

    double getFahrenheit()
    {
        return celsius * 9.0 / 5.0 + 32.0;
    }
};

int main()
{
    Temperature temp;

    temp.setCelsius(25);
    std::cout << "Celsius: " << temp.getCelsius() << "\n";
    std::cout << "Fahrenheit: " << temp.getFahrenheit() << "\n";

    // Validation prevents invalid state
    temp.setCelsius(-500);  // Prevented!

    return 0;
}
```

## Encapsulation

Encapsulation bundles data and methods, restricting direct access to some components.

### Data Hiding

```cpp
#include <iostream>
#include <string>

class Password
{
private:
    std::string password;  // Hidden from external access

public:
    void set(const std::string& pwd)
    {
        // Can add validation here
        if (pwd.length() >= 8)
        {
            password = pwd;
            std::cout << "Password set successfully\n";
        }
        else
        {
            std::cout << "Password too short!\n";
        }
    }

    bool verify(const std::string& attempt)
    {
        return password == attempt;
    }

    // Intentionally no getter - password is write-only!
};

int main()
{
    Password pwd;

    pwd.set("secret123");

    if (pwd.verify("secret123"))
    {
        std::cout << "Access granted!\n";
    }
    else
    {
        std::cout << "Access denied!\n";
    }

    // Cannot directly access password
    // std::cout << pwd.password;  // Error!

    return 0;
}
```

### Controlled Access

```cpp
#include <iostream>

class Counter
{
private:
    int count;
    int maxCount;

public:
    Counter(int max) : count(0), maxCount(max) {}

    void increment()
    {
        if (count < maxCount)
        {
            count++;
        }
        else
        {
            std::cout << "Max count reached!\n";
        }
    }

    void decrement()
    {
        if (count > 0)
        {
            count--;
        }
        else
        {
            std::cout << "Count cannot be negative!\n";
        }
    }

    int getCount() const
    {
        return count;
    }
};

int main()
{
    Counter counter(5);

    for (int i = 0; i < 7; i++)
    {
        counter.increment();
    }

    std::cout << "Count: " << counter.getCount() << "\n";

    // Direct access prevented
    // counter.count = 999;  // Error!

    return 0;
}
```

## Getters and Setters

### Basic Getters and Setters

```cpp
#include <iostream>
#include <string>

class Person
{
private:
    std::string name;
    int age;

public:
    // Setter for name
    void setName(const std::string& n)
    {
        name = n;
    }

    // Getter for name
    std::string getName() const
    {
        return name;
    }

    // Setter for age with validation
    void setAge(int a)
    {
        if (a >= 0 && a <= 150)
        {
            age = a;
        }
        else
        {
            std::cout << "Invalid age!\n";
        }
    }

    // Getter for age
    int getAge() const
    {
        return age;
    }
};

int main()
{
    Person person;

    person.setName("Alice");
    person.setAge(25);

    std::cout << "Name: " << person.getName() << "\n";
    std::cout << "Age: " << person.getAge() << "\n";

    person.setAge(200);  // Validation prevents invalid data

    return 0;
}
```

### Getters and Setters with Logic

```cpp
#include <iostream>
#include <string>

class Product
{
private:
    std::string name;
    double price;
    int stock;

public:
    void setName(const std::string& n)
    {
        if (!n.empty())
        {
            name = n;
        }
    }

    std::string getName() const
    {
        return name;
    }

    void setPrice(double p)
    {
        if (p >= 0)
        {
            price = p;
        }
        else
        {
            std::cout << "Price cannot be negative!\n";
        }
    }

    double getPrice() const
    {
        return price;
    }

    void setStock(int s)
    {
        if (s >= 0)
        {
            stock = s;
        }
    }

    int getStock() const
    {
        return stock;
    }

    bool isAvailable() const
    {
        return stock > 0;
    }

    double getTotalValue() const
    {
        return price * stock;
    }
};

int main()
{
    Product product;

    product.setName("Laptop");
    product.setPrice(999.99);
    product.setStock(50);

    std::cout << "Product: " << product.getName() << "\n";
    std::cout << "Price: $" << product.getPrice() << "\n";
    std::cout << "Stock: " << product.getStock() << "\n";
    std::cout << "Available: " << (product.isAvailable() ? "Yes" : "No") << "\n";
    std::cout << "Total value: $" << product.getTotalValue() << "\n";

    return 0;
}
```

### Read-Only and Write-Only Properties

```cpp
#include <iostream>
#include <string>
#include <ctime>

class BlogPost
{
private:
    std::string title;
    std::string content;
    time_t createdAt;
    int views;

public:
    BlogPost() : views(0)
    {
        createdAt = time(nullptr);
    }

    // Read-write: title and content
    void setTitle(const std::string& t) { title = t; }
    std::string getTitle() const { return title; }

    void setContent(const std::string& c) { content = c; }
    std::string getContent() const { return content; }

    // Read-only: creation time (no setter)
    time_t getCreatedAt() const { return createdAt; }

    // Read-only: views (increment through method)
    int getViews() const { return views; }
    void incrementViews() { views++; }
};

int main()
{
    BlogPost post;

    post.setTitle("My First Post");
    post.setContent("Hello, World!");

    // Can read creation time but not modify it
    std::cout << "Created at: " << post.getCreatedAt() << "\n";

    // Can increment views but not set directly
    post.incrementViews();
    post.incrementViews();
    std::cout << "Views: " << post.getViews() << "\n";

    return 0;
}
```

## Const Member Functions

### Const Correctness

```cpp
#include <iostream>

class Point
{
private:
    int x, y;

public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    // Const member function: doesn't modify object
    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void display() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
        // x = 10;  // Error: cannot modify in const function!
    }

    // Non-const member function: can modify object
    void setX(int xVal)
    {
        x = xVal;
    }

    void setY(int yVal)
    {
        y = yVal;
    }
};

void printPoint(const Point& p)
{
    p.display();  // OK: display() is const
    // p.setX(10);  // Error: setX() is not const!
}

int main()
{
    Point p(10, 20);

    p.display();
    p.setX(30);

    const Point constPoint(50, 60);
    constPoint.display();  // OK: const function
    // constPoint.setX(70);  // Error: non-const function!

    return 0;
}
```

### Why Const Member Functions?

```cpp
#include <iostream>
#include <vector>

class Container
{
private:
    std::vector<int> data;

public:
    void add(int value)
    {
        data.push_back(value);
    }

    // Const: read-only access
    int size() const
    {
        return data.size();
    }

    int get(int index) const
    {
        return data[index];
    }

    // Non-const: modifying operation
    void clear()
    {
        data.clear();
    }
};

void processContainer(const Container& c)
{
    std::cout << "Size: " << c.size() << "\n";  // OK
    std::cout << "First: " << c.get(0) << "\n"; // OK
    // c.clear();  // Error: clear() is not const
}

int main()
{
    Container cont;
    cont.add(10);
    cont.add(20);

    processContainer(cont);

    return 0;
}
```

## Static Members

### Static Member Variables

```cpp
#include <iostream>

class Counter
{
private:
    static int count;  // Shared by all objects
    int id;

public:
    Counter()
    {
        count++;
        id = count;
    }

    int getID() const
    {
        return id;
    }

    static int getCount()  // Static member function
    {
        return count;
    }
};

// Define static member (required!)
int Counter::count = 0;

int main()
{
    std::cout << "Initial count: " << Counter::getCount() << "\n";

    Counter c1, c2, c3;

    std::cout << "c1 ID: " << c1.getID() << "\n";
    std::cout << "c2 ID: " << c2.getID() << "\n";
    std::cout << "c3 ID: " << c3.getID() << "\n";
    std::cout << "Total count: " << Counter::getCount() << "\n";

    return 0;
}
```

### Static Member Functions

```cpp
#include <iostream>
#include <cmath>

class MathUtils
{
public:
    // Static functions: don't need object to call
    static double square(double x)
    {
        return x * x;
    }

    static double cube(double x)
    {
        return x * x * x;
    }

    static double distance(double x1, double y1, double x2, double y2)
    {
        double dx = x2 - x1;
        double dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }
};

int main()
{
    // Call without creating object
    std::cout << "Square of 5: " << MathUtils::square(5) << "\n";
    std::cout << "Cube of 3: " << MathUtils::cube(3) << "\n";
    std::cout << "Distance: " << MathUtils::distance(0, 0, 3, 4) << "\n";

    return 0;
}
```

## Complete Working Examples

### Example 1: BankAccount Class

```cpp
#include <iostream>
#include <string>
#include <vector>

class BankAccount
{
private:
    std::string accountNumber;
    std::string owner;
    double balance;
    std::vector<std::string> transactions;

    void logTransaction(const std::string& type, double amount)
    {
        std::string log = type + ": $" + std::to_string(amount);
        transactions.push_back(log);
    }

public:
    BankAccount(const std::string& number, const std::string& name, double initial = 0)
        : accountNumber(number), owner(name), balance(initial)
    {
        if (initial > 0)
        {
            logTransaction("Initial deposit", initial);
        }
    }

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            logTransaction("Deposit", amount);
            std::cout << "Deposited $" << amount << "\n";
        }
        else
        {
            std::cout << "Invalid deposit amount!\n";
        }
    }

    void withdraw(double amount)
    {
        if (amount <= 0)
        {
            std::cout << "Invalid withdrawal amount!\n";
            return;
        }

        if (amount > balance)
        {
            std::cout << "Insufficient funds!\n";
            return;
        }

        balance -= amount;
        logTransaction("Withdrawal", amount);
        std::cout << "Withdrawn $" << amount << "\n";
    }

    void transfer(BankAccount& recipient, double amount)
    {
        if (amount <= 0)
        {
            std::cout << "Invalid transfer amount!\n";
            return;
        }

        if (amount > balance)
        {
            std::cout << "Insufficient funds for transfer!\n";
            return;
        }

        balance -= amount;
        recipient.balance += amount;

        logTransaction("Transfer to " + recipient.accountNumber, amount);
        recipient.logTransaction("Transfer from " + accountNumber, amount);

        std::cout << "Transferred $" << amount << " to " << recipient.owner << "\n";
    }

    double getBalance() const
    {
        return balance;
    }

    std::string getOwner() const
    {
        return owner;
    }

    std::string getAccountNumber() const
    {
        return accountNumber;
    }

    void displayInfo() const
    {
        std::cout << "\n--- Account Information ---\n";
        std::cout << "Account: " << accountNumber << "\n";
        std::cout << "Owner: " << owner << "\n";
        std::cout << "Balance: $" << balance << "\n";
    }

    void displayTransactions() const
    {
        std::cout << "\n--- Transaction History ---\n";
        for (const auto& trans : transactions)
        {
            std::cout << trans << "\n";
        }
    }
};

int main()
{
    BankAccount alice("ACC001", "Alice Johnson", 1000);
    BankAccount bob("ACC002", "Bob Smith", 500);

    alice.displayInfo();
    bob.displayInfo();

    alice.deposit(500);
    alice.withdraw(200);
    alice.transfer(bob, 300);

    std::cout << "\nAfter transactions:\n";
    alice.displayInfo();
    bob.displayInfo();

    alice.displayTransactions();
    bob.displayTransactions();

    return 0;
}
```

### Example 2: Student Class

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

class Student
{
private:
    std::string name;
    int id;
    std::vector<double> grades;
    std::vector<std::string> courses;

public:
    Student(const std::string& studentName, int studentId)
        : name(studentName), id(studentId) {}

    void enrollCourse(const std::string& course)
    {
        courses.push_back(course);
        std::cout << name << " enrolled in " << course << "\n";
    }

    void addGrade(double grade)
    {
        if (grade >= 0 && grade <= 100)
        {
            grades.push_back(grade);
        }
        else
        {
            std::cout << "Invalid grade!\n";
        }
    }

    double getAverage() const
    {
        if (grades.empty())
            return 0.0;

        double sum = std::accumulate(grades.begin(), grades.end(), 0.0);
        return sum / grades.size();
    }

    std::string getLetterGrade() const
    {
        double avg = getAverage();

        if (avg >= 90) return "A";
        if (avg >= 80) return "B";
        if (avg >= 70) return "C";
        if (avg >= 60) return "D";
        return "F";
    }

    void displayInfo() const
    {
        std::cout << "\n--- Student Information ---\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "ID: " << id << "\n";
        std::cout << "Courses (" << courses.size() << "):\n";
        for (const auto& course : courses)
        {
            std::cout << "  - " << course << "\n";
        }

        std::cout << "Grades (" << grades.size() << "):\n";
        for (size_t i = 0; i < grades.size(); i++)
        {
            std::cout << "  " << (i + 1) << ". " << grades[i] << "\n";
        }

        std::cout << "Average: " << getAverage() << "\n";
        std::cout << "Letter Grade: " << getLetterGrade() << "\n";
    }

    std::string getName() const { return name; }
    int getID() const { return id; }
};

int main()
{
    Student student("Alice Johnson", 12345);

    student.enrollCourse("Data Structures");
    student.enrollCourse("Algorithms");
    student.enrollCourse("Database Systems");

    student.addGrade(92);
    student.addGrade(88);
    student.addGrade(95);

    student.displayInfo();

    return 0;
}
```

### Example 3: Rectangle Class

```cpp
#include <iostream>

class Rectangle
{
private:
    double width;
    double height;

    static int count;  // Count of rectangles created

public:
    Rectangle(double w = 0, double h = 0) : width(w), height(h)
    {
        count++;
    }

    void setWidth(double w)
    {
        if (w >= 0)
            width = w;
    }

    void setHeight(double h)
    {
        if (h >= 0)
            height = h;
    }

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    double area() const
    {
        return width * height;
    }

    double perimeter() const
    {
        return 2 * (width + height);
    }

    bool isSquare() const
    {
        return width == height;
    }

    void scale(double factor)
    {
        width *= factor;
        height *= factor;
    }

    void display() const
    {
        std::cout << "Rectangle: " << width << " x " << height << "\n";
        std::cout << "Area: " << area() << "\n";
        std::cout << "Perimeter: " << perimeter() << "\n";
        std::cout << "Is square: " << (isSquare() ? "Yes" : "No") << "\n";
    }

    static int getCount()
    {
        return count;
    }
};

int Rectangle::count = 0;

int main()
{
    Rectangle r1(5, 10);
    Rectangle r2(7, 7);

    std::cout << "Rectangle 1:\n";
    r1.display();

    std::cout << "\nRectangle 2:\n";
    r2.display();

    std::cout << "\nScaling rectangle 1 by 2:\n";
    r1.scale(2);
    r1.display();

    std::cout << "\nTotal rectangles created: " << Rectangle::getCount() << "\n";

    return 0;
}
```

### Example 4: Time Class

```cpp
#include <iostream>
#include <iomanip>

class Time
{
private:
    int hours;
    int minutes;
    int seconds;

    void normalize()
    {
        if (seconds >= 60)
        {
            minutes += seconds / 60;
            seconds %= 60;
        }

        if (minutes >= 60)
        {
            hours += minutes / 60;
            minutes %= 60;
        }

        hours %= 24;  // 24-hour format
    }

public:
    Time(int h = 0, int m = 0, int s = 0)
        : hours(h), minutes(m), seconds(s)
    {
        normalize();
    }

    void setTime(int h, int m, int s)
    {
        hours = h;
        minutes = m;
        seconds = s;
        normalize();
    }

    void addSeconds(int s)
    {
        seconds += s;
        normalize();
    }

    void addMinutes(int m)
    {
        minutes += m;
        normalize();
    }

    void addHours(int h)
    {
        hours += h;
        normalize();
    }

    int toSeconds() const
    {
        return hours * 3600 + minutes * 60 + seconds;
    }

    void display() const
    {
        std::cout << std::setfill('0')
                  << std::setw(2) << hours << ":"
                  << std::setw(2) << minutes << ":"
                  << std::setw(2) << seconds << "\n";
    }

    int getHours() const { return hours; }
    int getMinutes() const { return minutes; }
    int getSeconds() const { return seconds; }
};

int main()
{
    Time t1(10, 30, 45);

    std::cout << "Initial time: ";
    t1.display();

    t1.addSeconds(30);
    std::cout << "After adding 30 seconds: ";
    t1.display();

    t1.addMinutes(45);
    std::cout << "After adding 45 minutes: ";
    t1.display();

    t1.addHours(5);
    std::cout << "After adding 5 hours: ";
    t1.display();

    std::cout << "Total seconds: " << t1.toSeconds() << "\n";

    return 0;
}
```

### Example 5: Inventory Management System

```cpp
#include <iostream>
#include <string>
#include <vector>

class Product
{
private:
    std::string name;
    int id;
    double price;
    int quantity;

    static int nextID;

public:
    Product(const std::string& productName, double productPrice, int qty)
        : name(productName), id(nextID++), price(productPrice), quantity(qty) {}

    void addStock(int amount)
    {
        if (amount > 0)
        {
            quantity += amount;
            std::cout << "Added " << amount << " units to " << name << "\n";
        }
    }

    bool removeStock(int amount)
    {
        if (amount > quantity)
        {
            std::cout << "Insufficient stock!\n";
            return false;
        }

        quantity -= amount;
        std::cout << "Removed " << amount << " units from " << name << "\n";
        return true;
    }

    void updatePrice(double newPrice)
    {
        if (newPrice >= 0)
        {
            price = newPrice;
            std::cout << "Price updated to $" << newPrice << "\n";
        }
    }

    double getTotalValue() const
    {
        return price * quantity;
    }

    void display() const
    {
        std::cout << "ID: " << id
                  << " | Name: " << name
                  << " | Price: $" << price
                  << " | Qty: " << quantity
                  << " | Value: $" << getTotalValue() << "\n";
    }

    int getID() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
};

int Product::nextID = 1000;

class Inventory
{
private:
    std::vector<Product> products;

public:
    void addProduct(const Product& product)
    {
        products.push_back(product);
        std::cout << "Product added to inventory\n";
    }

    Product* findByID(int id)
    {
        for (auto& product : products)
        {
            if (product.getID() == id)
            {
                return &product;
            }
        }
        return nullptr;
    }

    void displayAll() const
    {
        std::cout << "\n--- Inventory ---\n";
        for (const auto& product : products)
        {
            product.display();
        }
    }

    double getTotalValue() const
    {
        double total = 0;
        for (const auto& product : products)
        {
            total += product.getTotalValue();
        }
        return total;
    }

    int getProductCount() const
    {
        return products.size();
    }
};

int main()
{
    Inventory inventory;

    Product p1("Laptop", 999.99, 10);
    Product p2("Mouse", 29.99, 50);
    Product p3("Keyboard", 79.99, 25);

    inventory.addProduct(p1);
    inventory.addProduct(p2);
    inventory.addProduct(p3);

    inventory.displayAll();

    std::cout << "\nTotal inventory value: $" << inventory.getTotalValue() << "\n";
    std::cout << "Product count: " << inventory.getProductCount() << "\n";

    // Find and modify product
    Product* found = inventory.findByID(1001);
    if (found)
    {
        found->addStock(10);
        found->updatePrice(24.99);
    }

    inventory.displayAll();

    return 0;
}
```

## Common Mistakes

### 1. Not Using Access Specifiers

```cpp
// WRONG - everything public by default
class BadAccount
{
    double balance;  // Anyone can modify!

public:
    void deposit(double amount)
    {
        balance += amount;
    }
};

// CORRECT - use private for data
class GoodAccount
{
private:
    double balance;  // Protected from external access

public:
    void deposit(double amount)
    {
        balance += amount;
    }
};
```

### 2. Forgetting to Initialize Members

```cpp
// WRONG - uninitialized members
class Bad
{
    int x;  // Garbage value!
public:
    void display() { std::cout << x << "\n"; }  // Undefined!
};

// CORRECT - initialize in constructor (covered next lesson)
class Good
{
    int x;
public:
    Good() : x(0) {}  // Initialized to 0
    void display() { std::cout << x << "\n"; }
};
```

### 3. Const Incorrectness

```cpp
// WRONG - getter not const
class Bad
{
    int value;
public:
    int getValue()  // Should be const!
    {
        return value;
    }
};

void process(const Bad& obj)
{
    // int x = obj.getValue();  // Error: getValue not const!
}

// CORRECT
class Good
{
    int value;
public:
    int getValue() const  // Const member function
    {
        return value;
    }
};
```

### 4. Not Validating in Setters

```cpp
// WRONG - no validation
class BadPerson
{
    int age;
public:
    void setAge(int a)
    {
        age = a;  // No check!
    }
};

// CORRECT - validate input
class GoodPerson
{
    int age;
public:
    void setAge(int a)
    {
        if (a >= 0 && a <= 150)
            age = a;
        else
            std::cout << "Invalid age!\n";
    }
};
```

### 5. Forgetting Static Member Definition

```cpp
// WRONG
class Bad
{
    static int count;  // Declared but not defined!
};
// Linker error!

// CORRECT
class Good
{
    static int count;
};
int Good::count = 0;  // Definition required!
```

## Exercises

### Exercise 1: Date Class
Create a Date class with:
- Private members: day, month, year
- Constructor to set date
- Validation (valid day/month/year)
- Methods: nextDay(), previousDay(), daysBetween(Date other)
- Display in multiple formats (MM/DD/YYYY, DD-MM-YYYY, etc.)

### Exercise 2: Library Book System
Implement:
- Book class (title, author, ISBN, available)
- Methods: checkout(), return(), display()
- Library class to manage multiple books
- Search by title or author
- Display all available books

### Exercise 3: Shopping Cart
Create:
- Item class (name, price, quantity)
- ShoppingCart class
- Methods: addItem(), removeItem(), updateQuantity()
- Calculate subtotal, tax, total
- Apply discount codes

### Exercise 4: Employee Management
Implement:
- Employee class (name, ID, salary, department)
- Static counter for total employees
- Methods: giveRaise(), changeDepartment()
- Calculate annual salary, monthly pay
- Display employee information

### Exercise 5: Game Character
Create a Character class for an RPG:
- Attributes: name, health, mana, level, experience
- Methods: attack(), defend(), castSpell()
- Level up system (experience thresholds)
- Status display (health bar, etc.)
- Inventory system (list of items)

## Summary

You learned:
- ✅ OOP fundamentals and benefits
- ✅ Classes and objects
- ✅ Member variables and functions
- ✅ Access specifiers (public, private, protected)
- ✅ Encapsulation and data hiding
- ✅ Getters and setters
- ✅ Const member functions
- ✅ Static members
- ✅ Complete class examples

## Next Lesson Preview

In Lesson 09, we'll explore:
- Constructors (default, parameterized, copy)
- Destructors and cleanup
- Constructor initialization lists
- RAII pattern in depth
- Copy constructor and assignment operator
- Rule of Three/Five
- Complete object lifecycle management

---

**Congratulations on completing Lesson 08!**

**Practice time**: Spend 4+ hours designing and implementing your own classes. OOP is fundamental to C++ - practice until you can model real-world problems using classes!
