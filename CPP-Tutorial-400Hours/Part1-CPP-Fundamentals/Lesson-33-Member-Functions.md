# Lesson 33: Member Functions

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Types of Member Functions
2. Const Member Functions
3. Inline Functions
4. Function Overloading in Classes
5. Returning Objects from Functions
6. Passing Objects to Functions
7. Member Function Best Practices
8. Exercises
9. Practice Projects

---

## 1. Types of Member Functions

Member functions can be categorized based on their purpose and behavior.

### Categories of Member Functions:

1. **Accessor (Getter) Functions**: Return member data
2. **Mutator (Setter) Functions**: Modify member data
3. **Helper/Utility Functions**: Perform calculations or operations
4. **Display Functions**: Output object information
5. **Static Member Functions**: Belong to class, not objects

### Example: All Types Together

```cpp
#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    string name;
    int id;
    double salary;
    static int employeeCount;  // Shared by all objects

public:
    // Constructor
    Employee(string n, int i, double s) : name(n), id(i), salary(s) {
        employeeCount++;
    }

    // ACCESSOR (Getter) - Read-only access to private data
    string getName() const {
        return name;
    }

    int getID() const {
        return id;
    }

    double getSalary() const {
        return salary;
    }

    // MUTATOR (Setter) - Modify private data with validation
    void setName(string n) {
        if (!n.empty()) {
            name = n;
        }
    }

    void setSalary(double s) {
        if (s > 0) {
            salary = s;
        }
    }

    // HELPER/UTILITY - Perform calculations
    double calculateAnnualSalary() const {
        return salary * 12;
    }

    double calculateTax() const {
        return salary * 0.2;  // 20% tax
    }

    // DISPLAY - Output information
    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Monthly Salary: $" << salary << endl;
        cout << "Annual Salary: $" << calculateAnnualSalary() << endl;
    }

    // STATIC - Class-level function
    static int getEmployeeCount() {
        return employeeCount;
    }
};

// Initialize static member
int Employee::employeeCount = 0;

int main() {
    Employee emp1("Alice", 1001, 5000);
    Employee emp2("Bob", 1002, 6000);

    emp1.displayInfo();
    cout << "\nTotal Employees: " << Employee::getEmployeeCount() << endl;

    return 0;
}
```

**Output**:
```
Name: Alice
ID: 1001
Monthly Salary: $5000
Annual Salary: $60000

Total Employees: 2
```

---

## 2. Const Member Functions

**Const member functions** promise not to modify the object's state. They can be called on const objects.

### Syntax:
```cpp
returnType functionName() const {
    // Cannot modify member variables
}
```

### Why Use Const Functions?

1. **Safety**: Prevents accidental modification
2. **Documentation**: Shows intent clearly
3. **Const Correctness**: Required for const objects
4. **Optimization**: Compiler can optimize better

### Example 1: Basic Const Functions

```cpp
#include <iostream>
using namespace std;

class Rectangle {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // Const member functions - don't modify object
    double getWidth() const {
        return width;
    }

    double getHeight() const {
        return height;
    }

    double area() const {
        return width * height;
    }

    double perimeter() const {
        return 2 * (width + height);
    }

    // Non-const member function - modifies object
    void setDimensions(double w, double h) {
        width = w;
        height = h;
    }

    // This would cause ERROR if marked const:
    // void scale(double factor) const {
    //     width *= factor;   // ERROR: Can't modify in const function
    //     height *= factor;  // ERROR: Can't modify in const function
    // }
};

int main() {
    const Rectangle rect(10.0, 5.0);  // Const object

    // Can call const functions
    cout << "Width: " << rect.getWidth() << endl;
    cout << "Area: " << rect.area() << endl;

    // ERROR: Can't call non-const functions on const object
    // rect.setDimensions(20, 10);  // Compilation error!

    return 0;
}
```

### Example 2: Const Correctness

```cpp
#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    string author;
    int pages;
    mutable int readCount;  // mutable allows modification in const functions

public:
    Book(string t, string a, int p)
        : title(t), author(a), pages(p), readCount(0) {}

    // Const functions
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getPages() const { return pages; }

    // Can modify mutable member even in const function
    void recordRead() const {
        readCount++;
        cout << "Book read count: " << readCount << endl;
    }

    int getReadCount() const {
        return readCount;
    }

    // Non-const function
    void setPages(int p) {
        pages = p;
    }

    void display() const {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Pages: " << pages << endl;
        cout << "Times Read: " << readCount << endl;
    }
};

void printBookInfo(const Book& book) {  // Const reference parameter
    book.display();      // OK: display() is const
    book.recordRead();   // OK: Can modify mutable member
    // book.setPages(500);  // ERROR: Can't call non-const function
}

int main() {
    Book myBook("C++ Primer", "Stanley Lippman", 976);

    printBookInfo(myBook);
    myBook.recordRead();
    printBookInfo(myBook);

    return 0;
}
```

---

## 3. Inline Functions

**Inline functions** suggest to the compiler to insert the function's code directly at the call site, avoiding function call overhead.

### When to Use Inline:
- Small, frequently called functions
- Simple getters and setters
- Performance-critical code

### Syntax:
```cpp
// Method 1: Define inside class (automatically inline)
class MyClass {
public:
    int getValue() { return value; }  // Implicitly inline
private:
    int value;
};

// Method 2: Use inline keyword
class MyClass {
public:
    int getValue();
private:
    int value;
};

inline int MyClass::getValue() {
    return value;
}
```

### Example 1: Inline Member Functions

```cpp
#include <iostream>
using namespace std;

class Point {
private:
    int x, y;

public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    // Inline functions (defined in class)
    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }

    // Larger function - defined outside (can still be inline)
    double distanceFromOrigin() const;

    void display() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

// Explicitly marked inline
inline double Point::distanceFromOrigin() const {
    return sqrt(x * x + y * y);
}

int main() {
    Point p(3, 4);
    p.display();
    cout << "Distance from origin: " << p.distanceFromOrigin() << endl;

    return 0;
}
```

### Example 2: Performance Comparison

```cpp
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

class Calculator {
public:
    // Inline function
    inline int addInline(int a, int b) {
        return a + b;
    }

    // Non-inline function
    int addNonInline(int a, int b);
};

int Calculator::addNonInline(int a, int b) {
    return a + b;
}

int main() {
    Calculator calc;
    const int ITERATIONS = 10000000;

    // Test inline function
    auto start1 = high_resolution_clock::now();
    long long sum1 = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sum1 += calc.addInline(i, i + 1);
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end1 - start1);

    // Test non-inline function
    auto start2 = high_resolution_clock::now();
    long long sum2 = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sum2 += calc.addNonInline(i, i + 1);
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end2 - start2);

    cout << "Inline function time: " << duration1.count() << "ms" << endl;
    cout << "Non-inline function time: " << duration2.count() << "ms" << endl;

    return 0;
}
```

---

## 4. Function Overloading in Classes

Member functions can be overloaded just like regular functions.

### Example 1: Constructor-like Initialization

```cpp
#include <iostream>
#include <string>
using namespace std;

class Display {
public:
    // Overloaded display functions
    void show(int value) {
        cout << "Integer: " << value << endl;
    }

    void show(double value) {
        cout << "Double: " << value << endl;
    }

    void show(string value) {
        cout << "String: " << value << endl;
    }

    void show(int value, string label) {
        cout << label << ": " << value << endl;
    }
};

int main() {
    Display d;

    d.show(42);
    d.show(3.14159);
    d.show("Hello World");
    d.show(100, "Score");

    return 0;
}
```

**Output**:
```
Integer: 42
Double: 3.14159
String: Hello World
Score: 100
```

### Example 2: Complex Number Class

```cpp
#include <iostream>
using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Overloaded set functions
    void set(double r, double i) {
        real = r;
        imag = i;
    }

    void set(double r) {  // Set only real part
        real = r;
        imag = 0;
    }

    void set(const Complex& c) {  // Set from another complex number
        real = c.real;
        imag = c.imag;
    }

    // Overloaded add functions
    Complex add(double value) const {
        return Complex(real + value, imag);
    }

    Complex add(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    void display() const {
        if (imag >= 0)
            cout << real << " + " << imag << "i" << endl;
        else
            cout << real << " - " << -imag << "i" << endl;
    }
};

int main() {
    Complex c1(3, 4);
    Complex c2(1, 2);

    cout << "c1: ";
    c1.display();

    cout << "c2: ";
    c2.display();

    Complex c3 = c1.add(c2);
    cout << "c1 + c2: ";
    c3.display();

    Complex c4 = c1.add(5.0);
    cout << "c1 + 5: ";
    c4.display();

    return 0;
}
```

---

## 5. Returning Objects from Functions

Member functions can return objects by value, reference, or pointer.

### Example 1: Return by Value

```cpp
#include <iostream>
using namespace std;

class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Return new object by value
    Vector2D add(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D subtract(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D scale(double factor) const {
        return Vector2D(x * factor, y * factor);
    }

    void display() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

int main() {
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    Vector2D v3 = v1.add(v2);
    Vector2D v4 = v1.scale(2);

    cout << "v1: "; v1.display();
    cout << "v2: "; v2.display();
    cout << "v1 + v2: "; v3.display();
    cout << "v1 * 2: "; v4.display();

    return 0;
}
```

### Example 2: Return by Reference (for chaining)

```cpp
#include <iostream>
#include <string>
using namespace std;

class StringBuilder {
private:
    string text;

public:
    StringBuilder() : text("") {}

    // Return reference for method chaining
    StringBuilder& append(const string& str) {
        text += str;
        return *this;
    }

    StringBuilder& appendLine(const string& str) {
        text += str + "\n";
        return *this;
    }

    StringBuilder& clear() {
        text.clear();
        return *this;
    }

    string toString() const {
        return text;
    }

    void display() const {
        cout << text;
    }
};

int main() {
    StringBuilder sb;

    // Method chaining
    sb.append("Hello ")
      .append("World")
      .appendLine("!")
      .append("C++ ")
      .appendLine("Programming");

    sb.display();

    return 0;
}
```

**Output**:
```
Hello World!
C++ Programming
```

---

## 6. Passing Objects to Functions

Objects can be passed by value, reference, or pointer.

### Example: All Three Methods

```cpp
#include <iostream>
using namespace std;

class Counter {
private:
    int count;

public:
    Counter(int c = 0) : count(c) {}

    void increment() { count++; }

    int getValue() const { return count; }

    void display() const {
        cout << "Count: " << count << endl;
    }
};

// Pass by value - creates a copy
void incrementByValue(Counter c) {
    c.increment();
    cout << "Inside incrementByValue: ";
    c.display();
}

// Pass by reference - modifies original
void incrementByReference(Counter& c) {
    c.increment();
    cout << "Inside incrementByReference: ";
    c.display();
}

// Pass by const reference - efficient, read-only
void displayCounter(const Counter& c) {
    c.display();
    // c.increment();  // ERROR: Can't modify const object
}

// Pass by pointer
void incrementByPointer(Counter* c) {
    if (c != nullptr) {
        c->increment();
        cout << "Inside incrementByPointer: ";
        c->display();
    }
}

int main() {
    Counter counter(10);

    cout << "Original: ";
    counter.display();

    cout << "\n--- Pass by Value ---" << endl;
    incrementByValue(counter);
    cout << "After function: ";
    counter.display();  // Unchanged

    cout << "\n--- Pass by Reference ---" << endl;
    incrementByReference(counter);
    cout << "After function: ";
    counter.display();  // Changed!

    cout << "\n--- Pass by Pointer ---" << endl;
    incrementByPointer(&counter);
    cout << "After function: ";
    counter.display();  // Changed!

    return 0;
}
```

**Output**:
```
Original: Count: 10

--- Pass by Value ---
Inside incrementByValue: Count: 11
After function: Count: 10

--- Pass by Reference ---
Inside incrementByReference: Count: 11
After function: Count: 11

--- Pass by Pointer ---
Inside incrementByPointer: Count: 12
After function: Count: 12
```

---

## 7. Member Function Best Practices

### Best Practice Guidelines:

1. **Make getters const**
2. **Use const references for input parameters**
3. **Return references for chaining**
4. **Keep functions small and focused**
5. **Use inline for simple functions**
6. **Validate input in setters**

### Example: Well-Designed Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string ownerName;
    double balance;

    // Private helper function
    bool isValidAmount(double amount) const {
        return amount > 0;
    }

public:
    // Constructor
    BankAccount(const string& accNum, const string& owner, double initialBalance)
        : accountNumber(accNum), ownerName(owner), balance(initialBalance) {
        if (balance < 0) balance = 0;
    }

    // Const getters
    string getAccountNumber() const { return accountNumber; }
    string getOwnerName() const { return ownerName; }
    double getBalance() const { return balance; }

    // Mutator with validation
    bool deposit(double amount) {
        if (isValidAmount(amount)) {
            balance += amount;
            return true;
        }
        return false;
    }

    bool withdraw(double amount) {
        if (isValidAmount(amount) && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // Chaining example
    BankAccount& applyInterest(double rate) {
        if (rate > 0) {
            balance += balance * (rate / 100.0);
        }
        return *this;
    }

    // Display (const)
    void display() const {
        cout << "Account: " << accountNumber << endl;
        cout << "Owner: " << ownerName << endl;
        cout << "Balance: $" << balance << endl;
    }
};

int main() {
    BankAccount account("ACC001", "John Doe", 1000.0);

    account.display();

    account.deposit(500);
    account.withdraw(200);

    // Method chaining
    account.applyInterest(5).applyInterest(2);

    cout << "\nAfter transactions:" << endl;
    account.display();

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Temperature Class
Create a Temperature class with conversion methods.

```cpp
class Temperature {
private:
    double celsius;

public:
    Temperature(double c = 0) : celsius(c) {}

    // Add these methods:
    double toFahrenheit() const;
    double toKelvin() const;
    void setCelsius(double c);
    double getCelsius() const;
};
```

### Exercise 2: String Manipulator
Create a StringUtil class with various string operations.

```cpp
class StringUtil {
private:
    string text;

public:
    StringUtil(const string& str) : text(str) {}

    // Implement these:
    StringUtil& toUpper();
    StringUtil& toLower();
    StringUtil& reverse();
    int countVowels() const;
    bool isPalindrome() const;
    string getText() const;
};
```

### Exercise 3: Circle Class
Complete circle class with overloaded methods.

```cpp
class Circle {
private:
    double radius;
    const double PI = 3.14159;

public:
    Circle(double r = 1.0) : radius(r) {}

    // Implement:
    double area() const;
    double circumference() const;
    void scale(double factor);
    Circle scaled(double factor) const;  // Return new circle
    bool isLargerThan(const Circle& other) const;
};
```

---

## 9. Practice Projects

### Project 1: Math Vector Class

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Vector3D {
private:
    double x, y, z;

public:
    Vector3D(double x = 0, double y = 0, double z = 0)
        : x(x), y(y), z(z) {}

    // Getters (const)
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // Setters
    void setX(double val) { x = val; }
    void setY(double val) { y = val; }
    void setZ(double val) { z = val; }

    // Vector operations (return new vectors)
    Vector3D add(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D subtract(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D scale(double factor) const {
        return Vector3D(x * factor, y * factor, z * factor);
    }

    // Dot product
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Magnitude
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    // Normalize (return new unit vector)
    Vector3D normalize() const {
        double mag = magnitude();
        if (mag > 0) {
            return Vector3D(x / mag, y / mag, z / mag);
        }
        return Vector3D();
    }

    // Display
    void display() const {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
};

int main() {
    Vector3D v1(3, 4, 0);
    Vector3D v2(1, 0, 2);

    cout << "v1: "; v1.display();
    cout << "v2: "; v2.display();
    cout << "Magnitude of v1: " << v1.magnitude() << endl;

    Vector3D v3 = v1.add(v2);
    cout << "v1 + v2: "; v3.display();

    Vector3D v4 = v1.normalize();
    cout << "v1 normalized: "; v4.display();
    cout << "Magnitude: " << v4.magnitude() << endl;

    cout << "Dot product: " << v1.dot(v2) << endl;

    return 0;
}
```

### Project 2: Date Class with Validation

```cpp
#include <iostream>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

    bool isLeapYear() const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int daysInMonth() const {
        int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear()) {
            return 29;
        }
        return days[month - 1];
    }

    bool isValidDate(int d, int m, int y) const {
        if (y < 1 || m < 1 || m > 12 || d < 1) return false;

        Date temp(1, m, y);
        return d <= temp.daysInMonth();
    }

public:
    Date(int d = 1, int m = 1, int y = 2024) {
        if (isValidDate(d, m, y)) {
            day = d;
            month = m;
            year = y;
        } else {
            day = 1;
            month = 1;
            year = 2024;
        }
    }

    // Getters (const)
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    // Setters with validation
    bool setDay(int d) {
        if (isValidDate(d, month, year)) {
            day = d;
            return true;
        }
        return false;
    }

    bool setMonth(int m) {
        if (isValidDate(day, m, year)) {
            month = m;
            return true;
        }
        return false;
    }

    void setYear(int y) {
        if (y > 0) year = y;
    }

    // Advance date by one day
    void nextDay() {
        day++;
        if (day > daysInMonth()) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    // Display
    void display() const {
        cout << day << "/" << month << "/" << year << endl;
    }

    // String representation
    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
};

int main() {
    Date date1(28, 2, 2024);  // Leap year
    cout << "Date: ";
    date1.display();

    cout << "Next 5 days:" << endl;
    for (int i = 0; i < 5; i++) {
        date1.nextDay();
        date1.display();
    }

    return 0;
}
```

---

## Key Takeaways

1. Use **const** for functions that don't modify the object
2. **Inline functions** improve performance for small functions
3. **Function overloading** provides flexible interfaces
4. **Return by reference** enables method chaining
5. **Pass by const reference** is efficient and safe
6. **Getters** should be const
7. **Setters** should validate input
8. Keep functions **focused and small**

---

## Common Mistakes

### Mistake 1: Forgetting const on Getters
```cpp
int getValue() { return value; }  // Bad
int getValue() const { return value; }  // Good
```

### Mistake 2: Returning Reference to Local
```cpp
MyClass& badFunction() {
    MyClass local;
    return local;  // ERROR: Returns reference to destroyed object
}
```

### Mistake 3: Not Validating in Setters
```cpp
void setAge(int a) {
    age = a;  // Bad: No validation
}

void setAge(int a) {
    if (a >= 0 && a <= 150) {  // Good: Validation
        age = a;
    }
}
```

---

## Summary Checklist

Before moving to Lesson 34, ensure you can:
- [ ] Write const member functions
- [ ] Use inline functions appropriately
- [ ] Overload member functions
- [ ] Return objects from functions
- [ ] Pass objects efficiently
- [ ] Implement method chaining
- [ ] Write proper getters and setters
- [ ] Validate input in mutator functions

---

## Next Lesson Preview

In **Lesson 34: Access Specifiers**, we'll learn:
- Public, private, and protected access levels
- Data hiding principles
- Access control in inheritance
- When to use each access specifier

Great progress! You now understand member functions in depth!
