# Lesson 13: Header Files and Multiple Files

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand the purpose and structure of header files
- Split programs into multiple source files
- Use include guards and `#pragma once`
- Understand forward declarations
- Organize code effectively across multiple files
- Avoid common issues with header files

## 1. Introduction to Header Files

Header files (.h or .hpp) contain declarations that can be shared across multiple source files. They help organize code and enable modular programming.

### 1.1 Basic Header File Structure

**math_utils.h:**
```cpp
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Function declarations
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
double divide(double a, double b);

#endif
```

**math_utils.cpp:**
```cpp
#include "math_utils.h"

// Function definitions
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(double a, double b) {
    if (b != 0) {
        return a / b;
    }
    return 0.0;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include "math_utils.h"

using namespace std;

int main() {
    int x = 10, y = 5;

    cout << "Addition: " << add(x, y) << endl;
    cout << "Subtraction: " << subtract(x, y) << endl;
    cout << "Multiplication: " << multiply(x, y) << endl;
    cout << "Division: " << divide(x, y) << endl;

    return 0;
}
```

### 1.2 Include Guards

Include guards prevent multiple inclusion of the same header file.

**Method 1: Traditional Include Guards**
```cpp
#ifndef MY_HEADER_H
#define MY_HEADER_H

// Header content here

#endif
```

**Method 2: #pragma once (Modern)**
```cpp
#pragma once

// Header content here
```

## 2. Organizing Code Across Multiple Files

### 2.1 Complete Example: Student Management System

**student.h:**
```cpp
#ifndef STUDENT_H
#define STUDENT_H

struct Student {
    int id;
    char name[50];
    float gpa;
};

// Function declarations
void initStudent(Student& s, int id, const char* name, float gpa);
void printStudent(const Student& s);
void updateGPA(Student& s, float newGPA);
bool isHonorStudent(const Student& s);

#endif
```

**student.cpp:**
```cpp
#include "student.h"
#include <iostream>
#include <cstring>

using namespace std;

void initStudent(Student& s, int id, const char* name, float gpa) {
    s.id = id;
    strncpy(s.name, name, 49);
    s.name[49] = '\0';
    s.gpa = gpa;
}

void printStudent(const Student& s) {
    cout << "ID: " << s.id << endl;
    cout << "Name: " << s.name << endl;
    cout << "GPA: " << s.gpa << endl;
}

void updateGPA(Student& s, float newGPA) {
    s.gpa = newGPA;
}

bool isHonorStudent(const Student& s) {
    return s.gpa >= 3.5;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include "student.h"

using namespace std;

int main() {
    Student student1;
    initStudent(student1, 1001, "Alice Johnson", 3.8);

    cout << "Student Information:" << endl;
    printStudent(student1);

    if (isHonorStudent(student1)) {
        cout << "This is an honor student!" << endl;
    }

    updateGPA(student1, 3.9);
    cout << "\nAfter GPA update:" << endl;
    printStudent(student1);

    return 0;
}
```

### 2.2 Multiple Headers Example

**geometry.h:**
```cpp
#pragma once

struct Point {
    double x;
    double y;
};

struct Circle {
    Point center;
    double radius;
};

struct Rectangle {
    Point topLeft;
    double width;
    double height;
};

// Point operations
void initPoint(Point& p, double x, double y);
double distanceBetweenPoints(const Point& p1, const Point& p2);

// Circle operations
void initCircle(Circle& c, const Point& center, double radius);
double circleArea(const Circle& c);
double circlePerimeter(const Circle& c);

// Rectangle operations
void initRectangle(Rectangle& r, const Point& topLeft, double width, double height);
double rectangleArea(const Rectangle& r);
double rectanglePerimeter(const Rectangle& r);
```

**geometry.cpp:**
```cpp
#include "geometry.h"
#include <cmath>

void initPoint(Point& p, double x, double y) {
    p.x = x;
    p.y = y;
}

double distanceBetweenPoints(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

void initCircle(Circle& c, const Point& center, double radius) {
    c.center = center;
    c.radius = radius;
}

double circleArea(const Circle& c) {
    return M_PI * c.radius * c.radius;
}

double circlePerimeter(const Circle& c) {
    return 2 * M_PI * c.radius;
}

void initRectangle(Rectangle& r, const Point& topLeft, double width, double height) {
    r.topLeft = topLeft;
    r.width = width;
    r.height = height;
}

double rectangleArea(const Rectangle& r) {
    return r.width * r.height;
}

double rectanglePerimeter(const Rectangle& r) {
    return 2 * (r.width + r.height);
}
```

**main.cpp:**
```cpp
#include <iostream>
#include <iomanip>
#include "geometry.h"

using namespace std;

int main() {
    Point p1, p2;
    initPoint(p1, 0, 0);
    initPoint(p2, 3, 4);

    cout << "Distance between points: " << distanceBetweenPoints(p1, p2) << endl;

    Circle circle;
    initCircle(circle, p1, 5.0);

    cout << fixed << setprecision(2);
    cout << "\nCircle:" << endl;
    cout << "Area: " << circleArea(circle) << endl;
    cout << "Perimeter: " << circlePerimeter(circle) << endl;

    Rectangle rect;
    initRectangle(rect, p2, 10.0, 5.0);

    cout << "\nRectangle:" << endl;
    cout << "Area: " << rectangleArea(rect) << endl;
    cout << "Perimeter: " << rectanglePerimeter(rect) << endl;

    return 0;
}
```

## 3. Constants and Global Variables in Headers

### 3.1 Constants in Headers

**constants.h:**
```cpp
#pragma once

// Constants can be in headers
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;
const int MAX_STUDENTS = 100;
const int MAX_NAME_LENGTH = 50;
```

### 3.2 Global Variables (Use with Caution)

**globals.h:**
```cpp
#pragma once

// Declaration (use extern)
extern int globalCounter;
extern double globalSum;

void incrementCounter();
void addToSum(double value);
```

**globals.cpp:**
```cpp
#include "globals.h"

// Definition (no extern)
int globalCounter = 0;
double globalSum = 0.0;

void incrementCounter() {
    globalCounter++;
}

void addToSum(double value) {
    globalSum += value;
}
```

## 4. Forward Declarations

Forward declarations allow you to reference types before they're fully defined.

**types.h:**
```cpp
#pragma once

// Forward declaration
struct Teacher;

struct Student {
    int id;
    char name[50];
    Teacher* advisor;  // Pointer to Teacher (forward declared)
};

struct Teacher {
    int id;
    char name[50];
    Student* students[10];  // Array of pointers to Students
    int studentCount;
};
```

## 5. Practical Multi-File Project Example

### 5.1 Bank Account System

**account.h:**
```cpp
#pragma once

struct Account {
    int accountNumber;
    char ownerName[50];
    double balance;
};

void initAccount(Account& acc, int number, const char* name, double initialBalance);
void deposit(Account& acc, double amount);
bool withdraw(Account& acc, double amount);
void printAccount(const Account& acc);
double getBalance(const Account& acc);
```

**account.cpp:**
```cpp
#include "account.h"
#include <iostream>
#include <cstring>

using namespace std;

void initAccount(Account& acc, int number, const char* name, double initialBalance) {
    acc.accountNumber = number;
    strncpy(acc.ownerName, name, 49);
    acc.ownerName[49] = '\0';
    acc.balance = initialBalance;
}

void deposit(Account& acc, double amount) {
    if (amount > 0) {
        acc.balance += amount;
        cout << "Deposited: $" << amount << endl;
    } else {
        cout << "Invalid deposit amount" << endl;
    }
}

bool withdraw(Account& acc, double amount) {
    if (amount <= 0) {
        cout << "Invalid withdrawal amount" << endl;
        return false;
    }

    if (amount > acc.balance) {
        cout << "Insufficient funds" << endl;
        return false;
    }

    acc.balance -= amount;
    cout << "Withdrawn: $" << amount << endl;
    return true;
}

void printAccount(const Account& acc) {
    cout << "Account #" << acc.accountNumber << endl;
    cout << "Owner: " << acc.ownerName << endl;
    cout << "Balance: $" << acc.balance << endl;
}

double getBalance(const Account& acc) {
    return acc.balance;
}
```

**transaction.h:**
```cpp
#pragma once
#include "account.h"

void transfer(Account& from, Account& to, double amount);
void printTransactionSummary(const Account& acc, int transactions);
```

**transaction.cpp:**
```cpp
#include "transaction.h"
#include <iostream>

using namespace std;

void transfer(Account& from, Account& to, double amount) {
    cout << "\n--- Transfer ---" << endl;
    if (withdraw(from, amount)) {
        deposit(to, amount);
        cout << "Transfer successful" << endl;
    } else {
        cout << "Transfer failed" << endl;
    }
}

void printTransactionSummary(const Account& acc, int transactions) {
    cout << "\n--- Transaction Summary ---" << endl;
    cout << "Account: " << acc.accountNumber << endl;
    cout << "Total transactions: " << transactions << endl;
    cout << "Final balance: $" << acc.balance << endl;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include "account.h"
#include "transaction.h"

using namespace std;

int main() {
    Account acc1, acc2;

    initAccount(acc1, 1001, "John Doe", 1000.0);
    initAccount(acc2, 1002, "Jane Smith", 500.0);

    cout << "Initial accounts:" << endl;
    printAccount(acc1);
    cout << endl;
    printAccount(acc2);

    cout << "\n--- Transactions ---" << endl;
    deposit(acc1, 200.0);
    withdraw(acc1, 150.0);

    transfer(acc1, acc2, 300.0);

    cout << "\n--- Final Status ---" << endl;
    printAccount(acc1);
    cout << endl;
    printAccount(acc2);

    return 0;
}
```

## 6. Compilation with Multiple Files

### 6.1 Compiling Manually

```bash
# Compile each .cpp file to object file
g++ -c main.cpp -o main.o
g++ -c account.cpp -o account.o
g++ -c transaction.cpp -o transaction.o

# Link object files together
g++ main.o account.o transaction.o -o program

# Run
./program
```

### 6.2 Compile All at Once

```bash
g++ main.cpp account.cpp transaction.cpp -o program
```

## 7. Best Practices

### 7.1 Header File Guidelines

```cpp
// Good header file structure
#pragma once  // or include guards

#include <necessary_system_headers>
// Avoid including unnecessary headers

// Forward declarations when possible
struct SomeType;

// Constants
const int MAX_SIZE = 100;

// Type definitions
struct MyStruct {
    // ...
};

// Function declarations
void myFunction();

// Inline functions (if small)
inline int square(int x) {
    return x * x;
}
```

### 7.2 What Goes in Headers vs Source Files

**Headers (.h/.hpp):**
- Declarations
- Struct/class definitions
- Function prototypes
- Inline functions
- Templates
- Constants

**Source Files (.cpp):**
- Function definitions
- Variable definitions
- Implementation details

## Exercises

### Exercise 1: Calculator Module
Create a calculator module with separate header and source files that provides functions for basic arithmetic, power, and square root operations.

### Exercise 2: String Utilities
Develop a string utility library with functions for string length, copy, concatenation, comparison, and reversal.

### Exercise 3: Array Operations
Create an array operations module that provides functions for finding min/max, sorting, searching, and reversing arrays.

### Exercise 4: Date Module
Implement a date module with a Date structure and functions to create dates, compare dates, calculate differences, and format dates.

### Exercise 5: Library System
Build a simple library system with separate modules for Book, Member, and Transaction, each with their own header and source files.

## Exercise Solutions

### Solution 1: Calculator Module

**calculator.h:**
```cpp
#pragma once

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double power(double base, int exponent);
double squareRoot(double n);
```

**calculator.cpp:**
```cpp
#include "calculator.h"
#include <cmath>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b != 0) {
        return a / b;
    }
    return 0.0;
}

double power(double base, int exponent) {
    return pow(base, exponent);
}

double squareRoot(double n) {
    if (n >= 0) {
        return sqrt(n);
    }
    return -1.0;  // Error indicator
}
```

**test_calculator.cpp:**
```cpp
#include <iostream>
#include <iomanip>
#include "calculator.h"

using namespace std;

int main() {
    cout << fixed << setprecision(2);

    cout << "10 + 5 = " << add(10, 5) << endl;
    cout << "10 - 5 = " << subtract(10, 5) << endl;
    cout << "10 * 5 = " << multiply(10, 5) << endl;
    cout << "10 / 5 = " << divide(10, 5) << endl;
    cout << "2^8 = " << power(2, 8) << endl;
    cout << "sqrt(16) = " << squareRoot(16) << endl;

    return 0;
}
```

### Solution 2: String Utilities

**string_utils.h:**
```cpp
#pragma once

int stringLength(const char* str);
void stringCopy(char* dest, const char* src);
void stringConcat(char* dest, const char* src);
int stringCompare(const char* s1, const char* s2);
void stringReverse(char* str);
bool stringContains(const char* str, const char* substr);
```

**string_utils.cpp:**
```cpp
#include "string_utils.h"

int stringLength(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void stringCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void stringConcat(char* dest, const char* src) {
    int destLen = stringLength(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}

int stringCompare(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

void stringReverse(char* str) {
    int len = stringLength(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

bool stringContains(const char* str, const char* substr) {
    int strLen = stringLength(str);
    int subLen = stringLength(substr);

    for (int i = 0; i <= strLen - subLen; i++) {
        bool match = true;
        for (int j = 0; j < subLen; j++) {
            if (str[i + j] != substr[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}
```

### Solution 3: Array Operations

**array_ops.h:**
```cpp
#pragma once

int findMin(const int arr[], int size);
int findMax(const int arr[], int size);
void bubbleSort(int arr[], int size);
int linearSearch(const int arr[], int size, int target);
void reverseArray(int arr[], int size);
double calculateAverage(const int arr[], int size);
```

**array_ops.cpp:**
```cpp
#include "array_ops.h"

int findMin(const int arr[], int size) {
    int minVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

int findMax(const int arr[], int size) {
    int maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int linearSearch(const int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

void reverseArray(int arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

double calculateAverage(const int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}
```

### Solution 4: Date Module

**date.h:**
```cpp
#pragma once

struct Date {
    int day;
    int month;
    int year;
};

void initDate(Date& d, int day, int month, int year);
bool isValidDate(const Date& d);
int compareDate(const Date& d1, const Date& d2);
int daysBetween(const Date& d1, const Date& d2);
void printDate(const Date& d);
bool isLeapYear(int year);
```

**date.cpp:**
```cpp
#include "date.h"
#include <iostream>

using namespace std;

void initDate(Date& d, int day, int month, int year) {
    d.day = day;
    d.month = month;
    d.year = year;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const Date& d) {
    if (d.month < 1 || d.month > 12) return false;
    if (d.day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear(d.year)) {
        daysInMonth[1] = 29;
    }

    return d.day <= daysInMonth[d.month - 1];
}

int compareDate(const Date& d1, const Date& d2) {
    if (d1.year != d2.year) return d1.year - d2.year;
    if (d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}

int daysBetween(const Date& d1, const Date& d2) {
    // Simplified calculation
    int days1 = d1.year * 365 + d1.month * 30 + d1.day;
    int days2 = d2.year * 365 + d2.month * 30 + d2.day;
    return days2 - days1;
}

void printDate(const Date& d) {
    cout << d.day << "/" << d.month << "/" << d.year;
}
```

### Solution 5: Library System

This solution demonstrates a multi-module system. Due to space, here's a simplified version:

**book.h:**
```cpp
#pragma once

struct Book {
    int id;
    char title[100];
    char author[50];
    bool isAvailable;
};

void initBook(Book& b, int id, const char* title, const char* author);
void printBook(const Book& b);
```

**book.cpp:**
```cpp
#include "book.h"
#include <iostream>
#include <cstring>

using namespace std;

void initBook(Book& b, int id, const char* title, const char* author) {
    b.id = id;
    strncpy(b.title, title, 99);
    b.title[99] = '\0';
    strncpy(b.author, author, 49);
    b.author[49] = '\0';
    b.isAvailable = true;
}

void printBook(const Book& b) {
    cout << "Book ID: " << b.id << endl;
    cout << "Title: " << b.title << endl;
    cout << "Author: " << b.author << endl;
    cout << "Status: " << (b.isAvailable ? "Available" : "Borrowed") << endl;
}
```

## Summary

In this lesson, you learned:

- **Header files** contain declarations shared across multiple files
- **Include guards** prevent multiple inclusion
- **Source files** contain function definitions
- How to **organize code** into modules
- How to **compile** multi-file programs
- **Best practices** for header and source file organization
- **Forward declarations** for handling dependencies

## Checklist

- [ ] I understand the purpose of header files
- [ ] I can create header files with include guards
- [ ] I can split programs into multiple source files
- [ ] I understand what goes in headers vs source files
- [ ] I can compile multi-file programs
- [ ] I completed all exercises and understand the solutions
- [ ] I understand forward declarations
- [ ] I can organize code into logical modules

## Additional Resources

- **Next Lesson:** Inline Functions and Const Functions
- **Related Topics:** Preprocessor Directives, Linking, Makefiles
- **Practice:** Refactor existing single-file programs into multi-file projects

---

**Congratulations!** You've completed Lesson 13. You now know how to organize C++ code across multiple files, which is essential for larger projects.
