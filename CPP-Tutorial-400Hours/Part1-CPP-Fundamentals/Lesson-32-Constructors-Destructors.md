# Lesson 32: Constructors and Destructors

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. What are Constructors?
2. Default Constructor
3. Parameterized Constructor
4. Constructor Overloading
5. Member Initializer Lists
6. Destructors
7. Order of Construction and Destruction
8. Exercises
9. Practice Projects

---

## 1. What are Constructors?

A **constructor** is a special member function that is automatically called when an object is created. It's used to initialize the object's data members.

### Characteristics of Constructors:
- Same name as the class
- No return type (not even void)
- Called automatically when object is created
- Can be overloaded
- Can have default parameters

### Why Use Constructors?

**Without Constructor**:
```cpp
class Rectangle {
public:
    double width;
    double height;
};

int main() {
    Rectangle rect;
    // width and height contain garbage values!
    // Must manually initialize:
    rect.width = 10.0;
    rect.height = 5.0;
}
```

**With Constructor**:
```cpp
class Rectangle {
public:
    double width;
    double height;

    Rectangle() {  // Constructor
        width = 0.0;
        height = 0.0;
    }
};

int main() {
    Rectangle rect;  // Automatically initialized to 0
}
```

---

## 2. Default Constructor

A **default constructor** takes no parameters. If you don't define any constructor, C++ provides one automatically (but it doesn't initialize primitive types).

### Example 1: Explicit Default Constructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    string name;
    int age;
    double gpa;

    // Default constructor
    Student() {
        name = "Unknown";
        age = 0;
        gpa = 0.0;
        cout << "Default constructor called!" << endl;
    }

    void display() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "GPA: " << gpa << endl;
    }
};

int main() {
    Student s1;  // Calls default constructor
    s1.display();

    Student s2;  // Calls default constructor again
    s2.name = "Alice";
    s2.age = 20;
    s2.gpa = 3.8;
    s2.display();

    return 0;
}
```

**Output**:
```
Default constructor called!
Name: Unknown
Age: 0
GPA: 0
Default constructor called!
Name: Alice
Age: 20
GPA: 3.8
```

### Example 2: Implicit Default Constructor

```cpp
class Point {
public:
    int x;
    int y;
    // No constructor defined - C++ provides one
};

int main() {
    Point p;  // Uses implicit default constructor
    // x and y contain garbage values!
    p.x = 10;
    p.y = 20;
}
```

---

## 3. Parameterized Constructor

A **parameterized constructor** accepts arguments to initialize the object with specific values.

### Example 1: Basic Parameterized Constructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class Car {
public:
    string brand;
    string model;
    int year;
    double price;

    // Parameterized constructor
    Car(string b, string m, int y, double p) {
        brand = b;
        model = m;
        year = y;
        price = p;
        cout << "Car object created: " << brand << " " << model << endl;
    }

    void display() {
        cout << "\n--- Car Details ---" << endl;
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
        cout << "Year: " << year << endl;
        cout << "Price: $" << price << endl;
    }
};

int main() {
    // Create objects with initial values
    Car car1("Tesla", "Model 3", 2024, 45000);
    Car car2("BMW", "M3", 2023, 75000);
    Car car3("Toyota", "Camry", 2024, 28000);

    car1.display();
    car2.display();
    car3.display();

    return 0;
}
```

**Output**:
```
Car object created: Tesla Model 3
Car object created: BMW M3
Car object created: Toyota Camry

--- Car Details ---
Brand: Tesla
Model: Model 3
Year: 2024
Price: $45000

--- Car Details ---
Brand: BMW
Model: M3
Year: 2023
Price: $75000

--- Car Details ---
Brand: Toyota
Model: Camry
Year: 2024
Price: $28000
```

### Example 2: Using 'this' Pointer in Constructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string holderName;
    double balance;

public:
    // Constructor with same parameter names as members
    BankAccount(string accountNumber, string holderName, double balance) {
        this->accountNumber = accountNumber;
        this->holderName = holderName;
        this->balance = balance;
    }

    void display() {
        cout << "Account: " << accountNumber << endl;
        cout << "Holder: " << holderName << endl;
        cout << "Balance: $" << balance << endl;
    }
};

int main() {
    BankAccount account("ACC001", "John Doe", 5000.0);
    account.display();

    return 0;
}
```

---

## 4. Constructor Overloading

You can have multiple constructors with different parameter lists (constructor overloading).

### Example 1: Multiple Constructors

```cpp
#include <iostream>
#include <string>
using namespace std;

class Rectangle {
private:
    double width;
    double height;

public:
    // Constructor 1: Default
    Rectangle() {
        width = 1.0;
        height = 1.0;
        cout << "Default constructor called" << endl;
    }

    // Constructor 2: Square (one parameter)
    Rectangle(double side) {
        width = side;
        height = side;
        cout << "Square constructor called" << endl;
    }

    // Constructor 3: Rectangle (two parameters)
    Rectangle(double w, double h) {
        width = w;
        height = h;
        cout << "Rectangle constructor called" << endl;
    }

    double area() {
        return width * height;
    }

    void display() {
        cout << "Width: " << width << ", Height: " << height << endl;
        cout << "Area: " << area() << endl;
    }
};

int main() {
    Rectangle r1;              // Calls default constructor
    Rectangle r2(5.0);         // Calls square constructor
    Rectangle r3(4.0, 6.0);    // Calls rectangle constructor

    cout << "\nRectangle 1:" << endl;
    r1.display();

    cout << "\nRectangle 2 (Square):" << endl;
    r2.display();

    cout << "\nRectangle 3:" << endl;
    r3.display();

    return 0;
}
```

**Output**:
```
Default constructor called
Square constructor called
Rectangle constructor called

Rectangle 1:
Width: 1, Height: 1
Area: 1

Rectangle 2 (Square):
Width: 5, Height: 5
Area: 25

Rectangle 3:
Width: 4, Height: 6
Area: 24
```

### Example 2: Default Parameters in Constructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class Product {
private:
    string name;
    double price;
    int quantity;

public:
    // Constructor with default parameters
    Product(string name = "Unknown", double price = 0.0, int quantity = 0) {
        this->name = name;
        this->price = price;
        this->quantity = quantity;
    }

    void display() {
        cout << "Product: " << name << endl;
        cout << "Price: $" << price << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "---" << endl;
    }
};

int main() {
    Product p1;                           // All defaults
    Product p2("Laptop");                 // One argument
    Product p3("Mouse", 25.99);           // Two arguments
    Product p4("Keyboard", 89.99, 10);    // All arguments

    p1.display();
    p2.display();
    p3.display();
    p4.display();

    return 0;
}
```

**Output**:
```
Product: Unknown
Price: $0
Quantity: 0
---
Product: Laptop
Price: $0
Quantity: 0
---
Product: Mouse
Price: $25.99
Quantity: 0
---
Product: Keyboard
Price: $89.99
Quantity: 10
---
```

---

## 5. Member Initializer Lists

Member initializer lists are a more efficient way to initialize members, especially for const members, references, and objects.

### Syntax:
```cpp
ClassName(parameters) : member1(value1), member2(value2) {
    // Constructor body
}
```

### Example 1: Basic Initializer List

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
    const int id;  // const member - must use initializer list

public:
    // Using initializer list
    Person(string n, int a, int i) : name(n), age(a), id(i) {
        cout << "Person created: " << name << endl;
    }

    void display() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "ID: " << id << endl;
    }
};

int main() {
    Person person("Alice", 25, 12345);
    person.display();

    return 0;
}
```

### Example 2: Initializer List vs Assignment

```cpp
#include <iostream>
using namespace std;

class Demo {
private:
    int x;
    int y;

public:
    // Method 1: Assignment in constructor body (less efficient)
    Demo(int a, int b) {
        x = a;  // Assignment
        y = b;  // Assignment
    }

    // Method 2: Initializer list (more efficient)
    // Demo(int a, int b) : x(a), y(b) {
    //     // Members are initialized, not assigned
    // }

    void display() {
        cout << "x = " << x << ", y = " << y << endl;
    }
};
```

### Example 3: Complex Initializer List

```cpp
#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    const int employeeID;
    string name;
    string department;
    double salary;
    int yearsOfService;

public:
    Employee(int id, string n, string dept, double sal)
        : employeeID(id),
          name(n),
          department(dept),
          salary(sal),
          yearsOfService(0) {
        cout << "Employee " << name << " hired!" << endl;
    }

    void promote(double raise, int additionalYears) {
        salary += raise;
        yearsOfService += additionalYears;
    }

    void display() {
        cout << "\n=== Employee Record ===" << endl;
        cout << "ID: " << employeeID << endl;
        cout << "Name: " << name << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: $" << salary << endl;
        cout << "Years of Service: " << yearsOfService << endl;
    }
};

int main() {
    Employee emp(1001, "John Smith", "Engineering", 75000);
    emp.display();

    emp.promote(10000, 1);
    emp.display();

    return 0;
}
```

---

## 6. Destructors

A **destructor** is a special member function that is automatically called when an object is destroyed. It's used to free resources (memory, file handles, etc.).

### Characteristics of Destructors:
- Same name as class, preceded by tilde (~)
- No return type
- No parameters
- Cannot be overloaded
- Called automatically when object goes out of scope

### Syntax:
```cpp
~ClassName() {
    // Cleanup code
}
```

### Example 1: Basic Destructor

```cpp
#include <iostream>
#include <string>
using namespace std;

class Resource {
private:
    string name;

public:
    Resource(string n) : name(n) {
        cout << "Resource '" << name << "' acquired" << endl;
    }

    ~Resource() {
        cout << "Resource '" << name << "' released" << endl;
    }
};

int main() {
    cout << "Program started" << endl;

    {  // Inner scope
        Resource r1("File Handle");
        Resource r2("Network Connection");
        cout << "Inside inner scope" << endl;
    }  // r1 and r2 destructors called here

    cout << "After inner scope" << endl;

    return 0;
}
```

**Output**:
```
Program started
Resource 'File Handle' acquired
Resource 'Network Connection' acquired
Inside inner scope
Resource 'Network Connection' released
Resource 'File Handle' released
After inner scope
```

### Example 2: Destructor with Dynamic Memory

```cpp
#include <iostream>
#include <string>
using namespace std;

class DynamicArray {
private:
    int* data;
    int size;

public:
    DynamicArray(int s) : size(s) {
        data = new int[size];
        cout << "Array of size " << size << " created" << endl;

        // Initialize to zeros
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~DynamicArray() {
        delete[] data;  // Free memory
        cout << "Array of size " << size << " destroyed" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }

    void display() {
        cout << "Array contents: ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    DynamicArray arr(5);
    arr.set(0, 10);
    arr.set(1, 20);
    arr.set(2, 30);
    arr.display();

    return 0;  // Destructor called here
}
```

**Output**:
```
Array of size 5 created
Array contents: 10 20 30 0 0
Array of size 5 destroyed
```

### Example 3: File Management with Destructor

```cpp
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileLogger {
private:
    ofstream logFile;
    string filename;

public:
    FileLogger(string fname) : filename(fname) {
        logFile.open(filename, ios::app);
        if (logFile.is_open()) {
            cout << "Log file '" << filename << "' opened" << endl;
            logFile << "=== Log Started ===" << endl;
        }
    }

    ~FileLogger() {
        if (logFile.is_open()) {
            logFile << "=== Log Ended ===" << endl;
            logFile.close();
            cout << "Log file '" << filename << "' closed" << endl;
        }
    }

    void log(string message) {
        if (logFile.is_open()) {
            logFile << message << endl;
            cout << "Logged: " << message << endl;
        }
    }
};

int main() {
    FileLogger logger("app.log");

    logger.log("Application started");
    logger.log("Processing data");
    logger.log("Operation completed");

    return 0;  // Destructor closes file automatically
}
```

---

## 7. Order of Construction and Destruction

Objects are destroyed in **reverse order** of construction.

### Example 1: Construction/Destruction Order

```cpp
#include <iostream>
using namespace std;

class Demo {
private:
    int id;

public:
    Demo(int i) : id(i) {
        cout << "Constructor " << id << " called" << endl;
    }

    ~Demo() {
        cout << "Destructor " << id << " called" << endl;
    }
};

int main() {
    cout << "Creating objects..." << endl;

    Demo obj1(1);
    Demo obj2(2);
    Demo obj3(3);

    cout << "All objects created" << endl;
    cout << "Exiting main..." << endl;

    return 0;  // Destructors called in reverse order: 3, 2, 1
}
```

**Output**:
```
Creating objects...
Constructor 1 called
Constructor 2 called
Constructor 3 called
All objects created
Exiting main...
Destructor 3 called
Destructor 2 called
Destructor 1 called
```

### Example 2: Stack vs Heap Objects

```cpp
#include <iostream>
using namespace std;

class Test {
private:
    int id;

public:
    Test(int i) : id(i) {
        cout << "Constructor " << id << endl;
    }

    ~Test() {
        cout << "Destructor " << id << endl;
    }
};

int main() {
    Test stack1(1);              // Stack object

    Test* heap1 = new Test(2);   // Heap object

    Test stack2(3);              // Stack object

    delete heap1;                // Manual deletion - destructor called

    Test* heap2 = new Test(4);   // Heap object

    cout << "End of main" << endl;

    return 0;  // stack2, stack1 destructors called (reverse order)
               // heap2 is leaked (destructor never called!)
}
```

**Output**:
```
Constructor 1
Constructor 2
Constructor 3
Destructor 2
Constructor 4
End of main
Destructor 3
Destructor 1
```

---

## 8. Exercises

### Exercise 1: Date Class
Create a Date class with day, month, year. Include constructors and validation.

```cpp
class Date {
private:
    int day;
    int month;
    int year;

public:
    // Default constructor - today's date
    Date() {
        // Set to January 1, 2024
    }

    // Parameterized constructor
    Date(int d, int m, int y) {
        // Validate and set values
    }

    void display() {
        // Display as DD/MM/YYYY
    }
};
```

### Exercise 2: Counter with Destructor
Create a Counter class that tracks how many objects exist.

```cpp
class Counter {
private:
    static int count;
    int id;

public:
    Counter() {
        // Increment count, assign id
    }

    ~Counter() {
        // Decrement count
    }

    static int getCount() {
        return count;
    }
};

int Counter::count = 0;
```

### Exercise 3: String Wrapper
Create a MyString class that manages a dynamic character array.

```cpp
class MyString {
private:
    char* str;
    int length;

public:
    MyString(const char* s) {
        // Allocate memory and copy string
    }

    ~MyString() {
        // Free memory
    }

    void display() {
        // Print string
    }
};
```

---

## 9. Practice Projects

### Project 1: Student Management System

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    int studentID;
    string name;
    double* grades;
    int numGrades;
    int capacity;

public:
    // Constructor
    Student(int id, string n, int cap = 5)
        : studentID(id), name(n), capacity(cap), numGrades(0) {
        grades = new double[capacity];
        cout << "Student " << name << " enrolled (ID: " << studentID << ")" << endl;
    }

    // Destructor
    ~Student() {
        delete[] grades;
        cout << "Student " << name << " record deleted" << endl;
    }

    void addGrade(double grade) {
        if (numGrades < capacity) {
            grades[numGrades++] = grade;
            cout << "Grade " << grade << " added for " << name << endl;
        } else {
            cout << "Cannot add more grades!" << endl;
        }
    }

    double getAverage() {
        if (numGrades == 0) return 0.0;

        double sum = 0.0;
        for (int i = 0; i < numGrades; i++) {
            sum += grades[i];
        }
        return sum / numGrades;
    }

    void displayReport() {
        cout << "\n=== Student Report ===" << endl;
        cout << "ID: " << studentID << endl;
        cout << "Name: " << name << endl;
        cout << "Grades: ";
        for (int i = 0; i < numGrades; i++) {
            cout << grades[i] << " ";
        }
        cout << endl;
        cout << "Average: " << getAverage() << endl;
        cout << "=====================" << endl;
    }
};

int main() {
    Student s1(1001, "Alice Johnson");
    s1.addGrade(85.5);
    s1.addGrade(92.0);
    s1.addGrade(78.5);
    s1.displayReport();

    {
        Student s2(1002, "Bob Smith");
        s2.addGrade(88.0);
        s2.addGrade(95.5);
        s2.displayReport();
    }  // s2 destructor called here

    cout << "\nProgram ending..." << endl;
    return 0;  // s1 destructor called here
}
```

### Project 2: Bank Account with Transaction History

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccountWithHistory {
private:
    const int accountNumber;
    string holderName;
    double balance;
    string* transactions;
    int numTransactions;
    int capacity;

public:
    BankAccountWithHistory(int accNum, string name, double initialBalance)
        : accountNumber(accNum), holderName(name), balance(initialBalance),
          numTransactions(0), capacity(10) {
        transactions = new string[capacity];
        cout << "Account " << accountNumber << " created for " << holderName << endl;

        addTransaction("Account opened with balance: $" + to_string(initialBalance));
    }

    ~BankAccountWithHistory() {
        delete[] transactions;
        cout << "Account " << accountNumber << " closed" << endl;
    }

    void addTransaction(string trans) {
        if (numTransactions < capacity) {
            transactions[numTransactions++] = trans;
        }
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            addTransaction("Deposited: $" + to_string(amount));
            cout << "Deposited $" << amount << " to account " << accountNumber << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            addTransaction("Withdrawn: $" + to_string(amount));
            cout << "Withdrawn $" << amount << " from account " << accountNumber << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    void displayStatement() {
        cout << "\n=== Account Statement ===" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Holder: " << holderName << endl;
        cout << "Current Balance: $" << balance << endl;
        cout << "\nTransaction History:" << endl;
        for (int i = 0; i < numTransactions; i++) {
            cout << (i + 1) << ". " << transactions[i] << endl;
        }
        cout << "========================" << endl;
    }
};

int main() {
    BankAccountWithHistory account(12345, "John Doe", 1000.0);

    account.deposit(500.0);
    account.withdraw(200.0);
    account.deposit(300.0);

    account.displayStatement();

    return 0;
}
```

### Project 3: Simple Game Character

```cpp
#include <iostream>
#include <string>
using namespace std;

class GameCharacter {
private:
    string name;
    int health;
    int maxHealth;
    int level;
    int* inventory;
    int inventorySize;

public:
    GameCharacter(string n, int lvl = 1)
        : name(n), level(lvl), inventorySize(10) {
        maxHealth = 100 * level;
        health = maxHealth;
        inventory = new int[inventorySize];

        // Initialize inventory
        for (int i = 0; i < inventorySize; i++) {
            inventory[i] = 0;
        }

        cout << "Character '" << name << "' created at level " << level << endl;
    }

    ~GameCharacter() {
        delete[] inventory;
        cout << "Character '" << name << "' deleted" << endl;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;

        cout << name << " took " << damage << " damage! ";
        cout << "Health: " << health << "/" << maxHealth << endl;

        if (health == 0) {
            cout << name << " has been defeated!" << endl;
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;

        cout << name << " healed " << amount << " HP! ";
        cout << "Health: " << health << "/" << maxHealth << endl;
    }

    void levelUp() {
        level++;
        maxHealth = 100 * level;
        health = maxHealth;
        cout << name << " leveled up to level " << level << "!" << endl;
        cout << "Max health increased to " << maxHealth << endl;
    }

    void displayStats() {
        cout << "\n=== Character Stats ===" << endl;
        cout << "Name: " << name << endl;
        cout << "Level: " << level << endl;
        cout << "Health: " << health << "/" << maxHealth << endl;
        cout << "======================" << endl;
    }
};

int main() {
    GameCharacter hero("Warrior", 1);
    hero.displayStats();

    hero.takeDamage(30);
    hero.heal(20);

    hero.levelUp();
    hero.displayStats();

    hero.takeDamage(150);

    return 0;
}
```

---

## Key Takeaways

1. **Constructors** initialize objects automatically
2. **Default constructors** take no parameters
3. **Parameterized constructors** accept initialization values
4. **Constructor overloading** allows multiple initialization methods
5. **Initializer lists** are more efficient than assignment
6. **Destructors** clean up resources automatically
7. Destructors are called in **reverse order** of construction
8. Always **delete** dynamically allocated memory in destructors

---

## Common Mistakes

### Mistake 1: Returning from Constructor
```cpp
class Wrong {
public:
    int Wrong() {  // ERROR: Constructor can't have return type
        return 0;
    }
};
```

### Mistake 2: Forgetting to Delete Dynamic Memory
```cpp
class MemoryLeak {
private:
    int* data;
public:
    MemoryLeak() {
        data = new int[100];
    }
    // ERROR: No destructor - memory leak!
};
```

### Mistake 3: Calling Destructor Manually
```cpp
MyClass obj;
obj.~MyClass();  // BAD: Don't call destructor manually!
// Destructor will be called again automatically!
```

---

## Summary Checklist

Before moving to Lesson 33, ensure you can:
- [ ] Write default and parameterized constructors
- [ ] Overload constructors
- [ ] Use member initializer lists
- [ ] Write destructors to free resources
- [ ] Understand construction and destruction order
- [ ] Manage dynamic memory with constructors/destructors
- [ ] Use const members with initializer lists
- [ ] Debug constructor/destructor calls

---

## Next Lesson Preview

In **Lesson 33: Member Functions**, we'll learn:
- Const member functions
- Inline functions
- Static member functions
- Function overloading in classes
- Returning objects from functions

Excellent work! You now understand how objects are born and die in C++!
