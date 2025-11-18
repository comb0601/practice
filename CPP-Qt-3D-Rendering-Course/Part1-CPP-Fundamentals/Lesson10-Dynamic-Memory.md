# Lesson 10: Dynamic Memory Allocation

**Duration**: 3 hours
**Prerequisite**: Lesson 09 - Pointers and References

---

## Table of Contents
1. [Introduction to Dynamic Memory](#introduction)
2. [Stack vs Heap Memory](#stack-vs-heap)
3. [The new Operator](#new-operator)
4. [The delete Operator](#delete-operator)
5. [Dynamic Arrays](#dynamic-arrays)
6. [Memory Leaks and How to Prevent Them](#memory-leaks)
7. [Common Pitfalls](#common-pitfalls)
8. [RAII Principle Introduction](#raii)
9. [Practical Exercises](#exercises)
10. [Complete Projects](#projects)

---

## 1. Introduction to Dynamic Memory {#introduction}

### What is Dynamic Memory?

Dynamic memory allocation allows you to allocate memory at runtime, giving you flexibility when the size of data is not known at compile time.

**Why Use Dynamic Memory?**
- Size unknown at compile time
- Need memory that outlives the function scope
- Large data that might overflow the stack
- Creating data structures like linked lists, trees, graphs

### Memory Model in C++

```
┌─────────────────┐
│   Code Segment  │ ← Program instructions
├─────────────────┤
│   Data Segment  │ ← Global/static variables
├─────────────────┤
│      Heap       │ ← Dynamic memory (grows downward)
│        ↓        │
│                 │
│        ↑        │
│      Stack      │ ← Local variables (grows upward)
└─────────────────┘
```

---

## 2. Stack vs Heap Memory {#stack-vs-heap}

### Stack Memory

**Characteristics:**
- Automatic allocation and deallocation
- Limited size (typically 1-8 MB)
- Fast access
- LIFO (Last In, First Out)
- Managed by compiler

```cpp
#include <iostream>

void stackExample() {
    int x = 10;           // Allocated on stack
    double y = 3.14;      // Allocated on stack
    char arr[100];        // Allocated on stack

    std::cout << "Stack variables:\n";
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;

    // All variables automatically destroyed when function ends
}

int main() {
    stackExample();
    // x, y, arr no longer exist here
    return 0;
}
```

**Stack Memory Advantages:**
- Fast allocation/deallocation
- Automatic cleanup
- Good cache locality

**Stack Memory Disadvantages:**
- Limited size
- Size must be known at compile time
- Can't return pointers to local variables

### Heap Memory

**Characteristics:**
- Manual allocation and deallocation
- Large size (limited by system RAM)
- Slower access than stack
- No automatic cleanup
- Managed by programmer

```cpp
#include <iostream>

int* heapExample() {
    int* ptr = new int(42);  // Allocated on heap
    return ptr;              // Can return pointer safely
}

int main() {
    int* p = heapExample();
    std::cout << "Heap value: " << *p << std::endl;

    // Must manually delete
    delete p;
    p = nullptr;

    return 0;
}
```

**Heap Memory Advantages:**
- Large size
- Size can be determined at runtime
- Memory persists until explicitly freed

**Heap Memory Disadvantages:**
- Slower allocation/deallocation
- Must manually manage memory
- Risk of memory leaks
- Fragmentation possible

---

## 3. The new Operator {#new-operator}

### Basic Syntax

The `new` operator allocates memory on the heap and returns a pointer.

```cpp
#include <iostream>

int main() {
    // Allocate single integer
    int* ptr = new int;
    *ptr = 100;
    std::cout << "Value: " << *ptr << std::endl;
    delete ptr;

    // Allocate with initialization
    int* ptr2 = new int(42);
    std::cout << "Initialized value: " << *ptr2 << std::endl;
    delete ptr2;

    // Allocate double
    double* dptr = new double(3.14159);
    std::cout << "Double value: " << *dptr << std::endl;
    delete dptr;

    return 0;
}
```

**Output:**
```
Value: 100
Initialized value: 42
Double value: 3.14159
```

### Allocating Objects

```cpp
#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int id;
    double gpa;

public:
    // Constructor
    Student(std::string n, int i, double g)
        : name(n), id(i), gpa(g) {
        std::cout << "Student created: " << name << std::endl;
    }

    // Destructor
    ~Student() {
        std::cout << "Student destroyed: " << name << std::endl;
    }

    void display() const {
        std::cout << "Name: " << name << std::endl;
        std::cout << "ID: " << id << std::endl;
        std::cout << "GPA: " << gpa << std::endl;
    }
};

int main() {
    // Allocate single student object
    Student* s1 = new Student("Alice", 101, 3.85);
    s1->display();

    std::cout << "\n";

    // Must manually delete
    delete s1;

    return 0;
}
```

**Output:**
```
Student created: Alice
Name: Alice
ID: 101
GPA: 3.85

Student destroyed: Alice
```

### new with Different Types

```cpp
#include <iostream>
#include <string>

struct Point {
    double x, y;

    Point(double xVal, double yVal) : x(xVal), y(yVal) {
        std::cout << "Point(" << x << ", " << y << ") created\n";
    }

    ~Point() {
        std::cout << "Point(" << x << ", " << y << ") destroyed\n";
    }
};

int main() {
    // Allocate different types

    // 1. Primitive types
    char* ch = new char('A');
    bool* flag = new bool(true);
    long* big = new long(1234567890L);

    std::cout << "char: " << *ch << std::endl;
    std::cout << "bool: " << std::boolalpha << *flag << std::endl;
    std::cout << "long: " << *big << std::endl;

    delete ch;
    delete flag;
    delete big;

    std::cout << "\n";

    // 2. Struct
    Point* p = new Point(3.5, 7.2);
    std::cout << "Point coordinates: (" << p->x << ", " << p->y << ")\n";
    delete p;

    std::cout << "\n";

    // 3. String
    std::string* str = new std::string("Dynamic String");
    std::cout << "String: " << *str << std::endl;
    delete str;

    return 0;
}
```

---

## 4. The delete Operator {#delete-operator}

### Basic Usage

The `delete` operator frees memory allocated with `new`.

```cpp
#include <iostream>

int main() {
    // Allocate
    int* ptr = new int(42);
    std::cout << "Value: " << *ptr << std::endl;

    // Deallocate
    delete ptr;

    // Good practice: set to nullptr
    ptr = nullptr;

    // Safe to delete nullptr
    delete ptr;  // Does nothing, which is safe

    return 0;
}
```

### Why Setting to nullptr is Important

```cpp
#include <iostream>

int main() {
    int* ptr = new int(100);

    delete ptr;
    // ptr is now a dangling pointer

    // BAD: Using dangling pointer (undefined behavior)
    // std::cout << *ptr << std::endl;  // DANGEROUS!

    // GOOD: Set to nullptr
    ptr = nullptr;

    // Now we can safely check
    if (ptr != nullptr) {
        std::cout << *ptr << std::endl;
    } else {
        std::cout << "Pointer is null" << std::endl;
    }

    return 0;
}
```

### Delete vs Delete[]

**CRITICAL RULE:**
- Use `delete` for single objects allocated with `new`
- Use `delete[]` for arrays allocated with `new[]`

```cpp
#include <iostream>

class Item {
public:
    Item() { std::cout << "Item constructed\n"; }
    ~Item() { std::cout << "Item destroyed\n"; }
};

int main() {
    std::cout << "Single object:\n";
    Item* single = new Item();
    delete single;  // Correct: delete for single object

    std::cout << "\nArray of objects:\n";
    Item* array = new Item[3];
    delete[] array;  // Correct: delete[] for array

    // WRONG COMBINATIONS (undefined behavior):
    // delete[] single;  // WRONG!
    // delete array;     // WRONG!

    return 0;
}
```

**Output:**
```
Single object:
Item constructed
Item destroyed

Array of objects:
Item constructed
Item constructed
Item constructed
Item destroyed
Item destroyed
Item destroyed
```

---

## 5. Dynamic Arrays {#dynamic-arrays}

### Allocating Dynamic Arrays

```cpp
#include <iostream>

int main() {
    // Get size from user
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    // Allocate array dynamically
    int* arr = new int[size];

    // Initialize array
    for (int i = 0; i < size; i++) {
        arr[i] = i * 10;
    }

    // Display array
    std::cout << "Array elements: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // Must use delete[] for arrays
    delete[] arr;
    arr = nullptr;

    return 0;
}
```

### Dynamic Array with User Input

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int n;
    std::cout << "How many numbers do you want to enter? ";
    std::cin >> n;

    // Allocate array
    double* numbers = new double[n];

    // Input
    std::cout << "Enter " << n << " numbers:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "Number " << (i + 1) << ": ";
        std::cin >> numbers[i];
    }

    // Calculate statistics
    double sum = 0.0;
    double max = numbers[0];
    double min = numbers[0];

    for (int i = 0; i < n; i++) {
        sum += numbers[i];
        if (numbers[i] > max) max = numbers[i];
        if (numbers[i] < min) min = numbers[i];
    }

    double average = sum / n;

    // Display results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nStatistics:\n";
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Maximum: " << max << std::endl;
    std::cout << "Minimum: " << min << std::endl;

    // Clean up
    delete[] numbers;
    numbers = nullptr;

    return 0;
}
```

### 2D Dynamic Arrays

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int rows, cols;
    std::cout << "Enter rows: ";
    std::cin >> rows;
    std::cout << "Enter columns: ";
    std::cin >> cols;

    // Allocate 2D array
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // Fill matrix
    std::cout << "\nEnter matrix elements:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << "Element[" << i << "][" << j << "]: ";
            std::cin >> matrix[i][j];
        }
    }

    // Display matrix
    std::cout << "\nMatrix:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << std::setw(4) << matrix[i][j];
        }
        std::cout << std::endl;
    }

    // Deallocate 2D array (reverse order)
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;

    return 0;
}
```

---

## 6. Memory Leaks and How to Prevent Them {#memory-leaks}

### What is a Memory Leak?

A memory leak occurs when dynamically allocated memory is not properly deallocated.

### Common Memory Leak Scenarios

#### Scenario 1: Forgetting to Delete

```cpp
#include <iostream>

void memoryLeak() {
    int* ptr = new int(42);
    std::cout << "Value: " << *ptr << std::endl;
    // FORGOT TO DELETE - MEMORY LEAK!
}

void properCleanup() {
    int* ptr = new int(42);
    std::cout << "Value: " << *ptr << std::endl;
    delete ptr;  // Proper cleanup
    ptr = nullptr;
}

int main() {
    std::cout << "Calling memoryLeak() 3 times:\n";
    for (int i = 0; i < 3; i++) {
        memoryLeak();  // Leaks memory each time
    }

    std::cout << "\nCalling properCleanup() 3 times:\n";
    for (int i = 0; i < 3; i++) {
        properCleanup();  // No leaks
    }

    return 0;
}
```

#### Scenario 2: Losing Pointer Reference

```cpp
#include <iostream>

int main() {
    // Allocate memory
    int* ptr = new int(100);

    // Reassign pointer without deleting
    ptr = new int(200);  // MEMORY LEAK! Lost reference to first allocation

    delete ptr;  // Only deletes second allocation

    // Correct way:
    int* ptr2 = new int(100);
    delete ptr2;  // Delete first
    ptr2 = new int(200);  // Then reassign
    delete ptr2;  // Delete second

    return 0;
}
```

#### Scenario 3: Exception Before Delete

```cpp
#include <iostream>
#include <stdexcept>

void riskyFunction(bool throwError) {
    int* data = new int[1000];

    if (throwError) {
        // Exception thrown - data never deleted!
        throw std::runtime_error("Error occurred");
    }

    delete[] data;  // Never reached if exception thrown
}

// Better approach: Use RAII (covered later)
void safeFunction(bool throwError) {
    int* data = new int[1000];

    try {
        if (throwError) {
            throw std::runtime_error("Error occurred");
        }
        delete[] data;
    } catch (...) {
        delete[] data;  // Ensure cleanup even with exception
        throw;  // Re-throw exception
    }
}

int main() {
    try {
        safeFunction(true);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    return 0;
}
```

### Detecting Memory Leaks

#### Manual Tracking

```cpp
#include <iostream>

int allocationCount = 0;

void* operator new(size_t size) {
    allocationCount++;
    std::cout << "Allocating " << size << " bytes (Total allocations: "
              << allocationCount << ")\n";
    return malloc(size);
}

void operator delete(void* memory) noexcept {
    allocationCount--;
    std::cout << "Deallocating (Remaining allocations: "
              << allocationCount << ")\n";
    free(memory);
}

int main() {
    std::cout << "=== Starting program ===\n";

    int* p1 = new int(10);
    int* p2 = new int(20);

    delete p1;
    // Forgot to delete p2 - leak!

    std::cout << "\n=== Ending program ===\n";
    std::cout << "Memory leaks: " << allocationCount << " allocations not freed\n";

    return 0;
}
```

---

## 7. Common Pitfalls {#common-pitfalls}

### 1. Double Delete

```cpp
#include <iostream>

int main() {
    int* ptr = new int(42);

    delete ptr;
    // delete ptr;  // UNDEFINED BEHAVIOR - Double delete!

    // Safe approach:
    int* ptr2 = new int(42);
    delete ptr2;
    ptr2 = nullptr;
    delete ptr2;  // Safe - deleting nullptr does nothing

    return 0;
}
```

### 2. Deleting Stack Memory

```cpp
#include <iostream>

int main() {
    int x = 42;
    int* ptr = &x;

    // delete ptr;  // UNDEFINED BEHAVIOR - Can't delete stack memory!

    // Only delete heap memory:
    int* heapPtr = new int(42);
    delete heapPtr;  // Correct

    return 0;
}
```

### 3. Using Memory After Delete

```cpp
#include <iostream>

int main() {
    int* ptr = new int(100);
    std::cout << "Value: " << *ptr << std::endl;

    delete ptr;

    // std::cout << *ptr << std::endl;  // UNDEFINED BEHAVIOR
    // *ptr = 200;                      // UNDEFINED BEHAVIOR

    // Safe approach:
    ptr = nullptr;
    if (ptr != nullptr) {
        std::cout << *ptr << std::endl;
    } else {
        std::cout << "Pointer is null\n";
    }

    return 0;
}
```

### 4. Returning Pointer to Local Variable

```cpp
#include <iostream>

int* badFunction() {
    int x = 42;
    return &x;  // WRONG! x is destroyed when function returns
}

int* goodFunction() {
    int* ptr = new int(42);
    return ptr;  // Correct - heap memory persists
}

int main() {
    // int* p1 = badFunction();  // Dangling pointer
    // std::cout << *p1 << std::endl;  // UNDEFINED BEHAVIOR

    int* p2 = goodFunction();
    std::cout << *p2 << std::endl;  // Works correctly
    delete p2;

    return 0;
}
```

---

## 8. RAII Principle Introduction {#raii}

**RAII** = Resource Acquisition Is Initialization

The idea: Tie resource lifetime to object lifetime.

### Basic RAII Class

```cpp
#include <iostream>

class IntArray {
private:
    int* data;
    int size;

public:
    // Constructor acquires resource
    IntArray(int s) : size(s) {
        data = new int[size];
        std::cout << "Array of " << size << " ints allocated\n";
    }

    // Destructor releases resource
    ~IntArray() {
        delete[] data;
        std::cout << "Array deallocated\n";
    }

    // Accessors
    int& operator[](int index) {
        return data[index];
    }

    int getSize() const {
        return size;
    }
};

int main() {
    {
        IntArray arr(5);  // Constructor allocates memory

        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;

        std::cout << "First element: " << arr[0] << std::endl;

    }  // Destructor automatically deallocates memory here

    std::cout << "Array is cleaned up automatically!\n";

    return 0;
}
```

**Output:**
```
Array of 5 ints allocated
First element: 10
Array deallocated
Array is cleaned up automatically!
```

### RAII Benefits

```cpp
#include <iostream>
#include <stdexcept>

class SafeBuffer {
private:
    char* buffer;
    int size;

public:
    SafeBuffer(int s) : size(s) {
        buffer = new char[size];
        std::cout << "Buffer allocated\n";
    }

    ~SafeBuffer() {
        delete[] buffer;
        std::cout << "Buffer deallocated\n";
    }

    void riskyOperation() {
        throw std::runtime_error("Something went wrong!");
    }
};

int main() {
    try {
        SafeBuffer buf(1024);
        buf.riskyOperation();  // Throws exception
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // Buffer is automatically cleaned up even with exception!
    std::cout << "No memory leak!\n";

    return 0;
}
```

---

## 9. Practical Exercises {#exercises}

### Exercise 1: Dynamic Student Array

Create a program that manages student records using dynamic memory.

```cpp
#include <iostream>
#include <string>
#include <iomanip>

struct Student {
    int id;
    std::string name;
    double gpa;
};

void displayStudent(const Student& s) {
    std::cout << std::setw(5) << s.id << " | ";
    std::cout << std::setw(20) << std::left << s.name << " | ";
    std::cout << std::fixed << std::setprecision(2) << s.gpa << std::endl;
}

int main() {
    int numStudents;
    std::cout << "How many students? ";
    std::cin >> numStudents;
    std::cin.ignore();

    // Allocate array
    Student* students = new Student[numStudents];

    // Input
    for (int i = 0; i < numStudents; i++) {
        std::cout << "\nStudent " << (i + 1) << ":\n";
        std::cout << "ID: ";
        std::cin >> students[i].id;
        std::cin.ignore();

        std::cout << "Name: ";
        std::getline(std::cin, students[i].name);

        std::cout << "GPA: ";
        std::cin >> students[i].gpa;
        std::cin.ignore();
    }

    // Display all
    std::cout << "\n=== Student Records ===\n";
    std::cout << std::setw(5) << "ID" << " | ";
    std::cout << std::setw(20) << std::left << "Name" << " | ";
    std::cout << "GPA" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    for (int i = 0; i < numStudents; i++) {
        displayStudent(students[i]);
    }

    // Find highest GPA
    int topIndex = 0;
    for (int i = 1; i < numStudents; i++) {
        if (students[i].gpa > students[topIndex].gpa) {
            topIndex = i;
        }
    }

    std::cout << "\nTop student:\n";
    displayStudent(students[topIndex]);

    // Clean up
    delete[] students;
    students = nullptr;

    return 0;
}
```

### Exercise 2: Dynamic Matrix Operations

```cpp
#include <iostream>
#include <iomanip>

int** createMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

void deleteMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void fillMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j + 1;
        }
    }
}

void displayMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << std::setw(4) << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

int** transposeMatrix(int** matrix, int rows, int cols) {
    int** result = createMatrix(cols, rows);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}

int main() {
    int rows = 3, cols = 4;

    // Create and fill matrix
    int** matrix = createMatrix(rows, cols);
    fillMatrix(matrix, rows, cols);

    std::cout << "Original Matrix (" << rows << "x" << cols << "):\n";
    displayMatrix(matrix, rows, cols);

    // Transpose
    int** transposed = transposeMatrix(matrix, rows, cols);

    std::cout << "\nTransposed Matrix (" << cols << "x" << rows << "):\n";
    displayMatrix(transposed, cols, rows);

    // Clean up
    deleteMatrix(matrix, rows);
    deleteMatrix(transposed, cols);

    return 0;
}
```

---

## 10. Complete Projects {#projects}

### Project 1: Dynamic Contact Manager

```cpp
#include <iostream>
#include <string>
#include <iomanip>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

class ContactManager {
private:
    Contact* contacts;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Contact* newContacts = new Contact[capacity];

        for (int i = 0; i < size; i++) {
            newContacts[i] = contacts[i];
        }

        delete[] contacts;
        contacts = newContacts;

        std::cout << "Resized to capacity: " << capacity << std::endl;
    }

public:
    ContactManager() : size(0), capacity(2) {
        contacts = new Contact[capacity];
    }

    ~ContactManager() {
        delete[] contacts;
    }

    void addContact(const std::string& name, const std::string& phone,
                    const std::string& email) {
        if (size >= capacity) {
            resize();
        }

        contacts[size].name = name;
        contacts[size].phone = phone;
        contacts[size].email = email;
        size++;

        std::cout << "Contact added successfully!\n";
    }

    void displayAll() const {
        if (size == 0) {
            std::cout << "No contacts found.\n";
            return;
        }

        std::cout << "\n=== All Contacts ===\n";
        std::cout << std::setw(20) << std::left << "Name" << " | ";
        std::cout << std::setw(15) << "Phone" << " | ";
        std::cout << "Email" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (int i = 0; i < size; i++) {
            std::cout << std::setw(20) << std::left << contacts[i].name << " | ";
            std::cout << std::setw(15) << contacts[i].phone << " | ";
            std::cout << contacts[i].email << std::endl;
        }
    }

    void searchByName(const std::string& name) const {
        bool found = false;

        for (int i = 0; i < size; i++) {
            if (contacts[i].name.find(name) != std::string::npos) {
                if (!found) {
                    std::cout << "\n=== Search Results ===\n";
                    found = true;
                }
                std::cout << "Name: " << contacts[i].name << std::endl;
                std::cout << "Phone: " << contacts[i].phone << std::endl;
                std::cout << "Email: " << contacts[i].email << std::endl;
                std::cout << std::endl;
            }
        }

        if (!found) {
            std::cout << "No contacts found with name: " << name << std::endl;
        }
    }

    void deleteContact(const std::string& name) {
        for (int i = 0; i < size; i++) {
            if (contacts[i].name == name) {
                // Shift elements
                for (int j = i; j < size - 1; j++) {
                    contacts[j] = contacts[j + 1];
                }
                size--;
                std::cout << "Contact deleted successfully!\n";
                return;
            }
        }
        std::cout << "Contact not found.\n";
    }

    int getSize() const {
        return size;
    }
};

void displayMenu() {
    std::cout << "\n=== Contact Manager ===\n";
    std::cout << "1. Add Contact\n";
    std::cout << "2. Display All Contacts\n";
    std::cout << "3. Search Contact\n";
    std::cout << "4. Delete Contact\n";
    std::cout << "5. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    ContactManager manager;
    int choice;
    std::string name, phone, email;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "\nEnter name: ";
                std::getline(std::cin, name);
                std::cout << "Enter phone: ";
                std::getline(std::cin, phone);
                std::cout << "Enter email: ";
                std::getline(std::cin, email);
                manager.addContact(name, phone, email);
                break;

            case 2:
                manager.displayAll();
                break;

            case 3:
                std::cout << "\nEnter name to search: ";
                std::getline(std::cin, name);
                manager.searchByName(name);
                break;

            case 4:
                std::cout << "\nEnter name to delete: ";
                std::getline(std::cin, name);
                manager.deleteContact(name);
                break;

            case 5:
                std::cout << "Goodbye!\n";
                break;

            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != 5);

    return 0;
}
```

### Project 2: Dynamic Polynomial Calculator

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>

class Polynomial {
private:
    double* coefficients;
    int degree;

public:
    // Constructor
    Polynomial(int deg) : degree(deg) {
        coefficients = new double[degree + 1];
        for (int i = 0; i <= degree; i++) {
            coefficients[i] = 0.0;
        }
    }

    // Copy constructor
    Polynomial(const Polynomial& other) : degree(other.degree) {
        coefficients = new double[degree + 1];
        for (int i = 0; i <= degree; i++) {
            coefficients[i] = other.coefficients[i];
        }
    }

    // Destructor
    ~Polynomial() {
        delete[] coefficients;
    }

    // Set coefficient
    void setCoefficient(int power, double value) {
        if (power >= 0 && power <= degree) {
            coefficients[power] = value;
        }
    }

    // Get coefficient
    double getCoefficient(int power) const {
        if (power >= 0 && power <= degree) {
            return coefficients[power];
        }
        return 0.0;
    }

    // Evaluate polynomial at x
    double evaluate(double x) const {
        double result = 0.0;
        for (int i = 0; i <= degree; i++) {
            result += coefficients[i] * std::pow(x, i);
        }
        return result;
    }

    // Display polynomial
    void display() const {
        bool first = true;

        for (int i = degree; i >= 0; i--) {
            if (coefficients[i] != 0.0) {
                if (!first && coefficients[i] > 0) {
                    std::cout << " + ";
                } else if (coefficients[i] < 0) {
                    std::cout << " - ";
                }

                double absCoeff = std::abs(coefficients[i]);

                if (i == 0 || absCoeff != 1.0) {
                    std::cout << absCoeff;
                }

                if (i > 0) {
                    std::cout << "x";
                    if (i > 1) {
                        std::cout << "^" << i;
                    }
                }

                first = false;
            }
        }

        if (first) {
            std::cout << "0";
        }

        std::cout << std::endl;
    }

    // Add two polynomials
    Polynomial add(const Polynomial& other) const {
        int maxDegree = std::max(degree, other.degree);
        Polynomial result(maxDegree);

        for (int i = 0; i <= maxDegree; i++) {
            double c1 = (i <= degree) ? coefficients[i] : 0.0;
            double c2 = (i <= other.degree) ? other.coefficients[i] : 0.0;
            result.setCoefficient(i, c1 + c2);
        }

        return result;
    }

    // Multiply by scalar
    Polynomial multiply(double scalar) const {
        Polynomial result(degree);

        for (int i = 0; i <= degree; i++) {
            result.setCoefficient(i, coefficients[i] * scalar);
        }

        return result;
    }

    // Derivative
    Polynomial derivative() const {
        if (degree == 0) {
            Polynomial result(0);
            return result;
        }

        Polynomial result(degree - 1);

        for (int i = 1; i <= degree; i++) {
            result.setCoefficient(i - 1, coefficients[i] * i);
        }

        return result;
    }
};

int main() {
    std::cout << "=== Polynomial Calculator ===\n\n";

    // Create polynomial: 3x^2 + 2x + 1
    Polynomial p1(2);
    p1.setCoefficient(0, 1);
    p1.setCoefficient(1, 2);
    p1.setCoefficient(2, 3);

    std::cout << "P1(x) = ";
    p1.display();

    // Evaluate at x = 2
    double x = 2.0;
    std::cout << "P1(" << x << ") = " << p1.evaluate(x) << std::endl;

    std::cout << std::endl;

    // Create another polynomial: 5x + 3
    Polynomial p2(1);
    p2.setCoefficient(0, 3);
    p2.setCoefficient(1, 5);

    std::cout << "P2(x) = ";
    p2.display();

    // Add polynomials
    Polynomial sum = p1.add(p2);
    std::cout << "P1(x) + P2(x) = ";
    sum.display();

    std::cout << std::endl;

    // Multiply by scalar
    Polynomial scaled = p1.multiply(2.0);
    std::cout << "2 * P1(x) = ";
    scaled.display();

    std::cout << std::endl;

    // Derivative
    Polynomial deriv = p1.derivative();
    std::cout << "P1'(x) = ";
    deriv.display();

    return 0;
}
```

**Output:**
```
=== Polynomial Calculator ===

P1(x) = 3x^2 + 2x + 1
P1(2) = 17

P2(x) = 5x + 3
P1(x) + P2(x) = 3x^2 + 7x + 4

2 * P1(x) = 6x^2 + 4x + 2

P1'(x) = 6x + 2
```

### Project 3: Memory Pool Allocator

```cpp
#include <iostream>
#include <cstring>

class MemoryPool {
private:
    char* pool;
    bool* used;
    int blockSize;
    int numBlocks;

public:
    MemoryPool(int bSize, int nBlocks)
        : blockSize(bSize), numBlocks(nBlocks) {
        pool = new char[blockSize * numBlocks];
        used = new bool[numBlocks];

        std::memset(used, 0, numBlocks * sizeof(bool));

        std::cout << "Memory pool created: " << numBlocks
                  << " blocks of " << blockSize << " bytes each\n";
    }

    ~MemoryPool() {
        delete[] pool;
        delete[] used;
        std::cout << "Memory pool destroyed\n";
    }

    void* allocate() {
        for (int i = 0; i < numBlocks; i++) {
            if (!used[i]) {
                used[i] = true;
                void* ptr = pool + (i * blockSize);
                std::cout << "Allocated block " << i << std::endl;
                return ptr;
            }
        }

        std::cout << "Error: No free blocks available!\n";
        return nullptr;
    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) return;

        char* charPtr = static_cast<char*>(ptr);
        int index = (charPtr - pool) / blockSize;

        if (index >= 0 && index < numBlocks && used[index]) {
            used[index] = false;
            std::cout << "Deallocated block " << index << std::endl;
        } else {
            std::cout << "Error: Invalid pointer!\n";
        }
    }

    void displayStatus() const {
        std::cout << "\n=== Memory Pool Status ===\n";
        int freeCount = 0;

        for (int i = 0; i < numBlocks; i++) {
            std::cout << "Block " << i << ": "
                      << (used[i] ? "USED" : "FREE") << std::endl;
            if (!used[i]) freeCount++;
        }

        std::cout << "Free blocks: " << freeCount << "/" << numBlocks << std::endl;
    }
};

int main() {
    // Create memory pool with 5 blocks of 64 bytes each
    MemoryPool pool(64, 5);

    std::cout << "\nAllocating 3 blocks:\n";
    void* p1 = pool.allocate();
    void* p2 = pool.allocate();
    void* p3 = pool.allocate();

    pool.displayStatus();

    std::cout << "\nDeallocating second block:\n";
    pool.deallocate(p2);

    pool.displayStatus();

    std::cout << "\nAllocating 3 more blocks:\n";
    void* p4 = pool.allocate();
    void* p5 = pool.allocate();
    void* p6 = pool.allocate();  // This should fail (pool full)

    pool.displayStatus();

    std::cout << "\nCleaning up:\n";
    pool.deallocate(p1);
    pool.deallocate(p3);
    pool.deallocate(p4);
    pool.deallocate(p5);

    pool.displayStatus();

    return 0;
}
```

---

## Summary

In this lesson, you learned:

### Key Concepts
1. **Dynamic Memory Basics**
   - Stack vs heap memory
   - When to use dynamic allocation
   - Memory model in C++

2. **new and delete Operators**
   - Allocating single objects: `new Type(value)`
   - Allocating arrays: `new Type[size]`
   - Deallocating: `delete ptr` and `delete[] array`
   - Setting pointers to nullptr after delete

3. **Dynamic Arrays**
   - 1D arrays with runtime size
   - 2D arrays (arrays of pointers)
   - Proper deallocation order

4. **Memory Management**
   - Identifying memory leaks
   - Common pitfalls (double delete, dangling pointers)
   - Using nullptr for safety
   - RAII principle introduction

5. **Best Practices**
   - Always pair new with delete
   - Set pointers to nullptr after delete
   - Use delete[] for arrays
   - Consider RAII for automatic cleanup
   - Avoid returning pointers to local variables

### Critical Rules
- **Rule 1**: Every `new` must have a corresponding `delete`
- **Rule 2**: Use `delete[]` for arrays, `delete` for single objects
- **Rule 3**: Set pointers to `nullptr` after deleting
- **Rule 4**: Never delete the same pointer twice
- **Rule 5**: Don't delete stack memory
- **Rule 6**: Don't use pointers after deleting them

---

## Next Lesson

**Lesson 11: Structures and Unions**

You'll learn about:
- Defining and using structures
- Structure members and methods
- Unions and their use cases
- Structure arrays and pointers
- Nested structures
- Bitfields

---

## Additional Practice

### Challenge Problems

1. **Dynamic String Array**: Create a program that reads N strings of varying length and stores them efficiently.

2. **Sparse Matrix**: Implement a sparse matrix that only stores non-zero elements using dynamic memory.

3. **Circular Buffer**: Create a circular buffer class that uses dynamic memory and supports resize.

4. **Smart Pointer**: Implement a basic smart pointer class that automatically deletes memory.

---

## Quick Reference

```cpp
// Allocate single object
int* p = new int(42);
delete p;
p = nullptr;

// Allocate array
int* arr = new int[100];
delete[] arr;
arr = nullptr;

// 2D array
int** matrix = new int*[rows];
for (int i = 0; i < rows; i++) {
    matrix[i] = new int[cols];
}
// Deallocate
for (int i = 0; i < rows; i++) {
    delete[] matrix[i];
}
delete[] matrix;

// RAII class template
class Resource {
    Type* ptr;
public:
    Resource() : ptr(new Type()) {}
    ~Resource() { delete ptr; }
};
```

**Compilation:**
```bash
g++ -std=c++20 -Wall lesson10.cpp -o lesson10
./lesson10
```

---

**End of Lesson 10**
