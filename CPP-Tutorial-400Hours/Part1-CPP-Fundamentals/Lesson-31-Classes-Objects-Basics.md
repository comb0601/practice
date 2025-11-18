# Lesson 31: Classes and Objects Basics

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Object-Oriented Programming
2. What are Classes and Objects?
3. Defining Classes
4. Creating Objects
5. Accessing Class Members
6. Class Methods vs Functions
7. The 'this' Pointer
8. Exercises
9. Practice Projects

---

## 1. Introduction to Object-Oriented Programming

Object-Oriented Programming (OOP) is a programming paradigm that organizes code around **objects** rather than functions and logic. C++ is a multi-paradigm language that fully supports OOP.

### The Four Pillars of OOP:
1. **Encapsulation**: Bundling data and methods that operate on that data
2. **Abstraction**: Hiding complex implementation details
3. **Inheritance**: Creating new classes from existing ones
4. **Polymorphism**: Objects of different types responding to the same interface

### Why Use OOP?

**Procedural Programming** (what we've learned so far):
```cpp
// Data and functions are separate
struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) +
                (p2.y - p1.y) * (p2.y - p1.y));
}
```

**Object-Oriented Programming**:
```cpp
// Data and functions are bundled together
class Point {
private:
    double x, y;

public:
    Point(double x, double y) : x(x), y(y) {}

    double distanceTo(const Point& other) const {
        return sqrt((other.x - x) * (other.x - x) +
                    (other.y - y) * (other.y - y));
    }
};
```

---

## 2. What are Classes and Objects?

### Class Definition:
A **class** is a blueprint or template for creating objects. It defines:
- **Data members** (attributes/properties)
- **Member functions** (methods/behaviors)

### Object Definition:
An **object** is an instance of a class. It's a concrete entity created from the class blueprint.

### Analogy:
- **Class** = Blueprint of a house
- **Object** = Actual house built from that blueprint

```cpp
class Car {
    // Class blueprint
};

Car myCar;      // Object 1
Car yourCar;    // Object 2
Car policeCar;  // Object 3
```

Each object has its own copy of data members but shares the same member functions.

---

## 3. Defining Classes

### Basic Class Syntax:

```cpp
class ClassName {
    // Access specifier (public, private, protected)
    // Data members
    // Member functions
};  // Don't forget the semicolon!
```

### Example 1: Simple Rectangle Class

```cpp
#include <iostream>
using namespace std;

class Rectangle {
public:
    // Data members
    double width;
    double height;

    // Member function
    double calculateArea() {
        return width * height;
    }

    double calculatePerimeter() {
        return 2 * (width + height);
    }
};

int main() {
    Rectangle rect;
    rect.width = 10.5;
    rect.height = 5.0;

    cout << "Area: " << rect.calculateArea() << endl;
    cout << "Perimeter: " << rect.calculatePerimeter() << endl;

    return 0;
}
```

**Output**:
```
Area: 52.5
Perimeter: 31
```

### Example 2: Student Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    // Data members
    string name;
    int rollNumber;
    double marks;

    // Member functions
    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Marks: " << marks << endl;
    }

    char getGrade() {
        if (marks >= 90) return 'A';
        else if (marks >= 80) return 'B';
        else if (marks >= 70) return 'C';
        else if (marks >= 60) return 'D';
        else return 'F';
    }
};

int main() {
    Student student1;
    student1.name = "Alice Johnson";
    student1.rollNumber = 101;
    student1.marks = 85.5;

    student1.displayInfo();
    cout << "Grade: " << student1.getGrade() << endl;

    return 0;
}
```

**Output**:
```
Name: Alice Johnson
Roll Number: 101
Marks: 85.5
Grade: B
```

---

## 4. Creating Objects

### Three Ways to Create Objects:

#### Method 1: Automatic/Stack Allocation
```cpp
class Car {
public:
    string model;
    int year;
};

int main() {
    Car myCar;  // Created on stack
    myCar.model = "Tesla Model 3";
    myCar.year = 2024;

    // Automatically destroyed when out of scope
    return 0;
}
```

#### Method 2: Dynamic/Heap Allocation
```cpp
int main() {
    Car* carPtr = new Car();  // Created on heap
    carPtr->model = "BMW M3";
    carPtr->year = 2023;

    delete carPtr;  // Must manually delete
    return 0;
}
```

#### Method 3: Array of Objects
```cpp
int main() {
    Car fleet[3];  // Array of 3 Car objects

    fleet[0].model = "Honda Civic";
    fleet[1].model = "Toyota Camry";
    fleet[2].model = "Ford Mustang";

    return 0;
}
```

### Example: Bank Account Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
public:
    string accountHolder;
    string accountNumber;
    double balance;

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: $" << amount << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    void displayBalance() {
        cout << "Account: " << accountNumber << endl;
        cout << "Holder: " << accountHolder << endl;
        cout << "Balance: $" << balance << endl;
    }
};

int main() {
    // Create first account
    BankAccount account1;
    account1.accountHolder = "John Doe";
    account1.accountNumber = "ACC001";
    account1.balance = 1000.0;

    // Create second account
    BankAccount account2;
    account2.accountHolder = "Jane Smith";
    account2.accountNumber = "ACC002";
    account2.balance = 2500.0;

    // Operations on account1
    account1.deposit(500.0);
    account1.withdraw(200.0);
    account1.displayBalance();

    cout << "\n";

    // Operations on account2
    account2.deposit(1000.0);
    account2.withdraw(4000.0);  // Should fail
    account2.displayBalance();

    return 0;
}
```

**Output**:
```
Deposited: $500
Withdrawn: $200
Account: ACC001
Holder: John Doe
Balance: $1300

Deposited: $1000
Insufficient funds!
Account: ACC002
Holder: Jane Smith
Balance: $3500
```

---

## 5. Accessing Class Members

### Dot Operator (.) for Stack Objects:
```cpp
Rectangle rect;
rect.width = 10.0;          // Access data member
rect.calculateArea();        // Call member function
```

### Arrow Operator (->) for Pointer Objects:
```cpp
Rectangle* rectPtr = new Rectangle();
rectPtr->width = 10.0;      // Access via pointer
rectPtr->calculateArea();    // Call via pointer

// Equivalent to:
(*rectPtr).width = 10.0;
(*rectPtr).calculateArea();

delete rectPtr;
```

### Example: Both Access Methods

```cpp
#include <iostream>
using namespace std;

class Circle {
public:
    double radius;
    const double PI = 3.14159;

    double area() {
        return PI * radius * radius;
    }

    double circumference() {
        return 2 * PI * radius;
    }
};

int main() {
    // Stack object - use dot operator
    Circle circle1;
    circle1.radius = 5.0;
    cout << "Circle 1 Area: " << circle1.area() << endl;

    // Heap object - use arrow operator
    Circle* circle2 = new Circle();
    circle2->radius = 7.5;
    cout << "Circle 2 Area: " << circle2->area() << endl;
    cout << "Circle 2 Circumference: " << circle2->circumference() << endl;

    delete circle2;
    return 0;
}
```

**Output**:
```
Circle 1 Area: 78.5397
Circle 2 Area: 176.714
Circle 2 Circumference: 47.1238
```

---

## 6. Class Methods vs Functions

### Member Functions (Methods):
- Defined inside or outside the class
- Have access to class members
- Called on objects

### Outside Definition Syntax:

```cpp
class Calculator {
public:
    int add(int a, int b);
    int subtract(int a, int b);
};

// Define outside class using scope resolution operator ::
int Calculator::add(int a, int b) {
    return a + b;
}

int Calculator::subtract(int a, int b) {
    return a - b;
}
```

### Example: Complete Class with Outside Definitions

```cpp
#include <iostream>
#include <string>
using namespace std;

class Book {
public:
    string title;
    string author;
    int pages;
    double price;

    // Declaration only
    void displayInfo();
    bool isExpensive();
    void applyDiscount(double percentage);
};

// Definitions outside class
void Book::displayInfo() {
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
    cout << "Pages: " << pages << endl;
    cout << "Price: $" << price << endl;
}

bool Book::isExpensive() {
    return price > 50.0;
}

void Book::applyDiscount(double percentage) {
    price = price - (price * percentage / 100.0);
    cout << "Discount applied! New price: $" << price << endl;
}

int main() {
    Book book;
    book.title = "C++ Programming";
    book.author = "Bjarne Stroustrup";
    book.pages = 1376;
    book.price = 65.99;

    book.displayInfo();

    if (book.isExpensive()) {
        cout << "\nThis is an expensive book." << endl;
        book.applyDiscount(20);  // 20% discount
    }

    return 0;
}
```

**Output**:
```
Title: C++ Programming
Author: Bjarne Stroustrup
Pages: 1376
Price: $65.99

This is an expensive book.
Discount applied! New price: $52.792
```

---

## 7. The 'this' Pointer

The `this` pointer is an implicit parameter to all member functions. It points to the object that invoked the member function.

### Why Use 'this'?

1. **Disambiguate parameters from members**
2. **Return the object itself**
3. **Pass the object to other functions**

### Example 1: Parameter Disambiguation

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;

public:
    void setData(string name, int age) {
        // Without 'this', parameters shadow members
        this->name = name;  // this->name refers to member
        this->age = age;    // age on right is parameter
    }

    void display() {
        cout << "Name: " << this->name << endl;
        cout << "Age: " << this->age << endl;
    }
};

int main() {
    Person person;
    person.setData("Alice", 25);
    person.display();

    return 0;
}
```

### Example 2: Method Chaining

```cpp
#include <iostream>
using namespace std;

class Counter {
private:
    int count;

public:
    Counter() {
        count = 0;
    }

    Counter& increment() {
        count++;
        return *this;  // Return reference to current object
    }

    Counter& decrement() {
        count--;
        return *this;
    }

    Counter& reset() {
        count = 0;
        return *this;
    }

    void display() {
        cout << "Count: " << count << endl;
    }
};

int main() {
    Counter c;

    // Method chaining
    c.increment().increment().increment().display();

    c.decrement().decrement().display();

    c.reset().increment().display();

    return 0;
}
```

**Output**:
```
Count: 3
Count: 1
Count: 1
```

### Example 3: Complete 'this' Demonstration

```cpp
#include <iostream>
using namespace std;

class Box {
private:
    double length;
    double width;
    double height;

public:
    Box(double length, double width, double height) {
        this->length = length;
        this->width = width;
        this->height = height;
    }

    double volume() {
        return this->length * this->width * this->height;
    }

    Box& scale(double factor) {
        this->length *= factor;
        this->width *= factor;
        this->height *= factor;
        return *this;
    }

    void display() {
        cout << "Dimensions: " << length << " x "
             << width << " x " << height << endl;
        cout << "Volume: " << volume() << endl;
    }

    bool isBiggerThan(Box& other) {
        return this->volume() > other.volume();
    }
};

int main() {
    Box box1(2.0, 3.0, 4.0);
    Box box2(1.0, 1.0, 1.0);

    box1.display();
    box2.display();

    if (box1.isBiggerThan(box2)) {
        cout << "\nBox 1 is bigger than Box 2" << endl;
    }

    cout << "\nScaling box2 by factor 3:" << endl;
    box2.scale(3.0).display();

    return 0;
}
```

**Output**:
```
Dimensions: 2 x 3 x 4
Volume: 24
Dimensions: 1 x 1 x 1
Volume: 1

Box 1 is bigger than Box 2

Scaling box2 by factor 3:
Dimensions: 3 x 3 x 3
Volume: 27
```

---

## 8. Exercises

### Exercise 1: Employee Class
Create an Employee class with name, ID, and salary. Include methods to give a raise and display information.

```cpp
#include <iostream>
#include <string>
using namespace std;

class Employee {
public:
    string name;
    int employeeID;
    double salary;

    void giveRaise(double percentage) {
        // Your code here
    }

    void displayInfo() {
        // Your code here
    }
};

int main() {
    Employee emp;
    emp.name = "Bob Smith";
    emp.employeeID = 1001;
    emp.salary = 50000;

    emp.displayInfo();
    emp.giveRaise(10);  // 10% raise
    emp.displayInfo();

    return 0;
}
```

### Exercise 2: Temperature Converter
Create a Temperature class that stores temperature in Celsius and provides methods to convert to Fahrenheit and Kelvin.

```cpp
class Temperature {
public:
    double celsius;

    double toFahrenheit() {
        // Formula: F = C * 9/5 + 32
        // Your code here
    }

    double toKelvin() {
        // Formula: K = C + 273.15
        // Your code here
    }
};
```

### Exercise 3: Shopping Cart
Create a ShoppingCart class that can add items, calculate total, and display contents.

```cpp
class ShoppingCart {
public:
    string items[100];
    double prices[100];
    int itemCount;

    void addItem(string item, double price) {
        // Your code here
    }

    double calculateTotal() {
        // Your code here
    }

    void displayCart() {
        // Your code here
    }
};
```

---

## 9. Practice Projects

### Project 1: Library Book Manager

```cpp
#include <iostream>
#include <string>
using namespace std;

class LibraryBook {
public:
    string title;
    string author;
    string ISBN;
    bool isAvailable;
    string borrowerName;

    void borrowBook(string borrower) {
        if (isAvailable) {
            isAvailable = false;
            borrowerName = borrower;
            cout << "Book borrowed successfully by " << borrower << endl;
        } else {
            cout << "Book is already borrowed by " << borrowerName << endl;
        }
    }

    void returnBook() {
        if (!isAvailable) {
            cout << "Book returned by " << borrowerName << endl;
            isAvailable = true;
            borrowerName = "";
        } else {
            cout << "Book was not borrowed." << endl;
        }
    }

    void displayStatus() {
        cout << "\n--- Book Information ---" << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
        if (!isAvailable) {
            cout << "Borrowed by: " << borrowerName << endl;
        }
    }
};

int main() {
    LibraryBook book;
    book.title = "The C++ Programming Language";
    book.author = "Bjarne Stroustrup";
    book.ISBN = "978-0321563842";
    book.isAvailable = true;
    book.borrowerName = "";

    book.displayStatus();

    book.borrowBook("Alice");
    book.displayStatus();

    book.borrowBook("Bob");  // Should fail

    book.returnBook();
    book.displayStatus();

    return 0;
}
```

### Project 2: Simple Banking System

```cpp
#include <iostream>
#include <string>
using namespace std;

class SimpleBankAccount {
private:
    string accountNumber;
    string holderName;
    double balance;
    int transactionCount;

public:
    void initialize(string accNum, string name, double initialBalance) {
        accountNumber = accNum;
        holderName = name;
        balance = initialBalance;
        transactionCount = 0;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactionCount++;
            cout << "Deposited: $" << amount << endl;
            cout << "New balance: $" << balance << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            transactionCount++;
            cout << "Withdrawn: $" << amount << endl;
            cout << "New balance: $" << balance << endl;
        } else if (amount > balance) {
            cout << "Insufficient funds! Current balance: $" << balance << endl;
        } else {
            cout << "Invalid withdrawal amount!" << endl;
        }
    }

    void displayAccountInfo() {
        cout << "\n=== Account Information ===" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Holder Name: " << holderName << endl;
        cout << "Current Balance: $" << balance << endl;
        cout << "Total Transactions: " << transactionCount << endl;
        cout << "===========================" << endl;
    }

    double getBalance() {
        return balance;
    }
};

int main() {
    SimpleBankAccount account;
    account.initialize("ACC12345", "John Doe", 1000.0);

    account.displayAccountInfo();

    account.deposit(500.0);
    account.withdraw(200.0);
    account.withdraw(2000.0);  // Should fail

    account.displayAccountInfo();

    return 0;
}
```

### Project 3: Student Grade Manager

```cpp
#include <iostream>
#include <string>
using namespace std;

class StudentRecord {
public:
    string studentName;
    int studentID;
    double scores[5];  // 5 test scores
    int numScores;

    void initialize(string name, int id) {
        studentName = name;
        studentID = id;
        numScores = 0;
        for (int i = 0; i < 5; i++) {
            scores[i] = 0.0;
        }
    }

    void addScore(double score) {
        if (numScores < 5 && score >= 0 && score <= 100) {
            scores[numScores] = score;
            numScores++;
            cout << "Score " << numScores << " added: " << score << endl;
        } else {
            cout << "Cannot add score (invalid or array full)" << endl;
        }
    }

    double calculateAverage() {
        if (numScores == 0) return 0.0;

        double sum = 0.0;
        for (int i = 0; i < numScores; i++) {
            sum += scores[i];
        }
        return sum / numScores;
    }

    char getLetterGrade() {
        double avg = calculateAverage();
        if (avg >= 90) return 'A';
        else if (avg >= 80) return 'B';
        else if (avg >= 70) return 'C';
        else if (avg >= 60) return 'D';
        else return 'F';
    }

    void displayReport() {
        cout << "\n=== Student Report ===" << endl;
        cout << "Name: " << studentName << endl;
        cout << "ID: " << studentID << endl;
        cout << "Scores: ";
        for (int i = 0; i < numScores; i++) {
            cout << scores[i] << " ";
        }
        cout << endl;
        cout << "Average: " << calculateAverage() << endl;
        cout << "Grade: " << getLetterGrade() << endl;
        cout << "=====================" << endl;
    }
};

int main() {
    StudentRecord student;
    student.initialize("Alice Johnson", 12345);

    student.addScore(85.0);
    student.addScore(92.0);
    student.addScore(78.0);
    student.addScore(88.0);
    student.addScore(95.0);

    student.displayReport();

    return 0;
}
```

---

## Key Takeaways

1. **Classes** are blueprints; **Objects** are instances
2. Use **dot operator (.)** for stack objects
3. Use **arrow operator (->)** for pointer objects
4. Member functions can be defined inside or outside the class
5. The **this** pointer refers to the current object
6. Classes bundle data and functions together (encapsulation)
7. Each object has its own copy of data members
8. Member functions are shared among all objects

---

## Common Mistakes

### Mistake 1: Forgetting Semicolon After Class
```cpp
class MyClass {
    // ...
}  // ERROR: Missing semicolon

// Correct:
class MyClass {
    // ...
};  // Semicolon required!
```

### Mistake 2: Confusing . and ->
```cpp
Rectangle* ptr = new Rectangle();
ptr.width = 10;  // ERROR: Use -> for pointers
ptr->width = 10; // Correct
```

### Mistake 3: Accessing Private Members
```cpp
class MyClass {
private:
    int value;  // Private by default in classes
};

MyClass obj;
obj.value = 5;  // ERROR: Cannot access private member
```

---

## Summary Checklist

Before moving to Lesson 32, ensure you can:
- [ ] Define a class with data members and member functions
- [ ] Create objects (stack and heap)
- [ ] Access class members using . and ->
- [ ] Define member functions outside the class
- [ ] Understand and use the 'this' pointer
- [ ] Implement method chaining
- [ ] Create arrays of objects
- [ ] Build complete classes with multiple methods

---

## Next Lesson Preview

In **Lesson 32: Constructors and Destructors**, we'll learn:
- What constructors are and why they're needed
- Default, parameterized, and copy constructors
- Constructor overloading
- Destructors and resource cleanup
- Constructor initialization lists

Great job completing Lesson 31! You've taken your first major step into Object-Oriented Programming!
