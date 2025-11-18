# Lesson 11: Structures and Unions

**Duration**: 2.5 hours
**Prerequisite**: Lesson 10 - Dynamic Memory Allocation

---

## Table of Contents
1. [Introduction to Structures](#introduction)
2. [Defining and Using Structures](#defining-structures)
3. [Structure Members and Access](#structure-members)
4. [Structures with Functions](#structures-functions)
5. [Arrays of Structures](#arrays-structures)
6. [Nested Structures](#nested-structures)
7. [Pointers to Structures](#pointers-structures)
8. [Unions](#unions)
9. [Practical Exercises](#exercises)
10. [Complete Projects](#projects)

---

## 1. Introduction to Structures {#introduction}

### What is a Structure?

A **structure** is a user-defined data type that groups together variables of different types under a single name.

**Key Features:**
- Groups related data together
- Can contain different data types
- Members accessed using dot (.) operator
- Forms basis of object-oriented programming

### Why Use Structures?

```cpp
// Without structures - hard to manage
std::string studentName;
int studentID;
double studentGPA;
std::string studentMajor;

// With structures - organized and clear
struct Student {
    std::string name;
    int id;
    double gpa;
    std::string major;
};

Student s1;  // One variable holds all related data
```

---

## 2. Defining and Using Structures {#defining-structures}

### Basic Structure Definition

```cpp
#include <iostream>
#include <string>

// Define a structure
struct Point {
    int x;
    int y;
};

int main() {
    // Create structure variable
    Point p1;

    // Assign values
    p1.x = 10;
    p1.y = 20;

    // Access and display
    std::cout << "Point: (" << p1.x << ", " << p1.y << ")" << std::endl;

    return 0;
}
```

**Output:**
```
Point: (10, 20)
```

### Structure with Initialization

```cpp
#include <iostream>
#include <string>

struct Book {
    std::string title;
    std::string author;
    int pages;
    double price;
};

int main() {
    // Method 1: Initialize during declaration
    Book book1 = {"1984", "George Orwell", 328, 15.99};

    // Method 2: C++11 uniform initialization
    Book book2{"Brave New World", "Aldous Huxley", 268, 14.50};

    // Method 3: Individual assignment
    Book book3;
    book3.title = "Fahrenheit 451";
    book3.author = "Ray Bradbury";
    book3.pages = 158;
    book3.price = 12.99;

    // Display
    std::cout << "Book 1: " << book1.title << " by " << book1.author << std::endl;
    std::cout << "Price: $" << book1.price << std::endl;

    return 0;
}
```

### Multiple Structure Variables

```cpp
#include <iostream>
#include <string>

struct Employee {
    int id;
    std::string name;
    std::string department;
    double salary;
};

int main() {
    Employee emp1 = {101, "Alice Johnson", "Engineering", 75000.0};
    Employee emp2 = {102, "Bob Smith", "Marketing", 65000.0};
    Employee emp3 = {103, "Carol Davis", "HR", 60000.0};

    std::cout << "Employee Records:\n";
    std::cout << emp1.id << ": " << emp1.name << " - $" << emp1.salary << std::endl;
    std::cout << emp2.id << ": " << emp2.name << " - $" << emp2.salary << std::endl;
    std::cout << emp3.id << ": " << emp3.name << " - $" << emp3.salary << std::endl;

    return 0;
}
```

---

## 3. Structure Members and Access {#structure-members}

### Accessing Structure Members

```cpp
#include <iostream>
#include <string>

struct Rectangle {
    double length;
    double width;
};

int main() {
    Rectangle rect;

    // Write to members
    rect.length = 10.5;
    rect.width = 5.25;

    // Read from members
    double area = rect.length * rect.width;
    double perimeter = 2 * (rect.length + rect.width);

    std::cout << "Rectangle:\n";
    std::cout << "Length: " << rect.length << std::endl;
    std::cout << "Width: " << rect.width << std::endl;
    std::cout << "Area: " << area << std::endl;
    std::cout << "Perimeter: " << perimeter << std::endl;

    return 0;
}
```

### Copying Structures

```cpp
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
    std::string city;
};

int main() {
    Person person1 = {"John Doe", 30, "New York"};

    // Copy entire structure
    Person person2 = person1;

    // Modify copy
    person2.name = "Jane Doe";
    person2.age = 28;

    // Original unchanged
    std::cout << "Person 1: " << person1.name << ", " << person1.age << std::endl;
    std::cout << "Person 2: " << person2.name << ", " << person2.age << std::endl;

    return 0;
}
```

**Output:**
```
Person 1: John Doe, 30
Person 2: Jane Doe, 28
```

### Comparing Structures

```cpp
#include <iostream>

struct Point {
    int x;
    int y;
};

bool areEqual(const Point& p1, const Point& p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

int main() {
    Point p1 = {10, 20};
    Point p2 = {10, 20};
    Point p3 = {15, 25};

    // Note: Can't use p1 == p2 directly
    // Must compare member by member or write comparison function

    if (areEqual(p1, p2)) {
        std::cout << "p1 and p2 are equal\n";
    }

    if (!areEqual(p1, p3)) {
        std::cout << "p1 and p3 are different\n";
    }

    return 0;
}
```

---

## 4. Structures with Functions {#structures-functions}

### Passing Structures to Functions

```cpp
#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    double gpa;
};

// Pass by value (copy)
void displayStudent(Student s) {
    std::cout << "ID: " << s.id << std::endl;
    std::cout << "Name: " << s.name << std::endl;
    std::cout << "GPA: " << s.gpa << std::endl;
}

// Pass by reference (efficient for reading)
void displayStudentRef(const Student& s) {
    std::cout << "ID: " << s.id << std::endl;
    std::cout << "Name: " << s.name << std::endl;
    std::cout << "GPA: " << s.gpa << std::endl;
}

// Pass by reference (for modification)
void updateGPA(Student& s, double newGPA) {
    s.gpa = newGPA;
}

int main() {
    Student student = {"Alice Johnson", 12345, 3.75};

    std::cout << "Original:\n";
    displayStudent(student);

    updateGPA(student, 3.85);

    std::cout << "\nAfter update:\n";
    displayStudentRef(student);

    return 0;
}
```

### Returning Structures from Functions

```cpp
#include <iostream>
#include <string>
#include <cmath>

struct Point {
    double x;
    double y;
};

Point createPoint(double x, double y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Point addPoints(const Point& p1, const Point& p2) {
    Point result;
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    return result;
}

double distance(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main() {
    Point p1 = createPoint(3.0, 4.0);
    Point p2 = createPoint(6.0, 8.0);

    Point sum = addPoints(p1, p2);
    double dist = distance(p1, p2);

    std::cout << "p1: (" << p1.x << ", " << p1.y << ")\n";
    std::cout << "p2: (" << p2.x << ", " << p2.y << ")\n";
    std::cout << "Sum: (" << sum.x << ", " << sum.y << ")\n";
    std::cout << "Distance: " << dist << std::endl;

    return 0;
}
```

---

## 5. Arrays of Structures {#arrays-structures}

### Static Array of Structures

```cpp
#include <iostream>
#include <string>
#include <iomanip>

struct Product {
    int id;
    std::string name;
    double price;
    int quantity;
};

int main() {
    const int SIZE = 4;
    Product inventory[SIZE] = {
        {101, "Laptop", 999.99, 15},
        {102, "Mouse", 29.99, 50},
        {103, "Keyboard", 79.99, 30},
        {104, "Monitor", 299.99, 20}
    };

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Product Inventory:\n";
    std::cout << std::setw(5) << "ID" << " | ";
    std::cout << std::setw(15) << std::left << "Name" << " | ";
    std::cout << std::setw(8) << std::right << "Price" << " | ";
    std::cout << std::setw(8) << "Qty" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    double totalValue = 0.0;

    for (int i = 0; i < SIZE; i++) {
        std::cout << std::setw(5) << inventory[i].id << " | ";
        std::cout << std::setw(15) << std::left << inventory[i].name << " | ";
        std::cout << "$" << std::setw(7) << std::right << inventory[i].price << " | ";
        std::cout << std::setw(8) << inventory[i].quantity << std::endl;

        totalValue += inventory[i].price * inventory[i].quantity;
    }

    std::cout << "\nTotal Inventory Value: $" << totalValue << std::endl;

    return 0;
}
```

### Dynamic Array of Structures

```cpp
#include <iostream>
#include <string>

struct Task {
    int id;
    std::string description;
    bool completed;
};

int main() {
    int numTasks;
    std::cout << "How many tasks? ";
    std::cin >> numTasks;
    std::cin.ignore();

    // Allocate dynamic array
    Task* tasks = new Task[numTasks];

    // Input tasks
    for (int i = 0; i < numTasks; i++) {
        tasks[i].id = i + 1;

        std::cout << "\nTask " << (i + 1) << " description: ";
        std::getline(std::cin, tasks[i].description);

        tasks[i].completed = false;
    }

    // Display tasks
    std::cout << "\n=== Task List ===\n";
    for (int i = 0; i < numTasks; i++) {
        std::cout << tasks[i].id << ". ";
        std::cout << tasks[i].description;
        std::cout << " [" << (tasks[i].completed ? "✓" : " ") << "]\n";
    }

    // Clean up
    delete[] tasks;

    return 0;
}
```

---

## 6. Nested Structures {#nested-structures}

### Basic Nested Structure

```cpp
#include <iostream>
#include <string>

struct Date {
    int day;
    int month;
    int year;
};

struct Event {
    std::string name;
    Date date;
    std::string location;
};

void displayDate(const Date& d) {
    std::cout << d.month << "/" << d.day << "/" << d.year;
}

void displayEvent(const Event& e) {
    std::cout << "Event: " << e.name << std::endl;
    std::cout << "Date: ";
    displayDate(e.date);
    std::cout << std::endl;
    std::cout << "Location: " << e.location << std::endl;
}

int main() {
    Event concert;
    concert.name = "Rock Concert";
    concert.date.day = 15;
    concert.date.month = 7;
    concert.date.year = 2024;
    concert.location = "Madison Square Garden";

    displayEvent(concert);

    // Initialize with nested initialization
    Event conference = {
        "Tech Conference",
        {20, 9, 2024},
        "Convention Center"
    };

    std::cout << "\n";
    displayEvent(conference);

    return 0;
}
```

### Complex Nested Structure

```cpp
#include <iostream>
#include <string>

struct Address {
    std::string street;
    std::string city;
    std::string state;
    std::string zip;
};

struct Contact {
    std::string phone;
    std::string email;
};

struct Person {
    std::string name;
    int age;
    Address address;
    Contact contact;
};

void displayPerson(const Person& p) {
    std::cout << "Name: " << p.name << std::endl;
    std::cout << "Age: " << p.age << std::endl;
    std::cout << "Address: " << p.address.street << ", "
              << p.address.city << ", "
              << p.address.state << " " << p.address.zip << std::endl;
    std::cout << "Phone: " << p.contact.phone << std::endl;
    std::cout << "Email: " << p.contact.email << std::endl;
}

int main() {
    Person person = {
        "John Smith",
        35,
        {"123 Main St", "Springfield", "IL", "62701"},
        {"555-1234", "john@email.com"}
    };

    displayPerson(person);

    return 0;
}
```

---

## 7. Pointers to Structures {#pointers-structures}

### Basic Pointer to Structure

```cpp
#include <iostream>
#include <string>

struct Car {
    std::string brand;
    std::string model;
    int year;
    double price;
};

int main() {
    Car myCar = {"Toyota", "Camry", 2023, 28000.0};

    // Pointer to structure
    Car* ptr = &myCar;

    // Access using arrow operator
    std::cout << "Brand: " << ptr->brand << std::endl;
    std::cout << "Model: " << ptr->model << std::endl;
    std::cout << "Year: " << ptr->year << std::endl;
    std::cout << "Price: $" << ptr->price << std::endl;

    // Modify through pointer
    ptr->price = 27500.0;
    std::cout << "\nNew price: $" << myCar.price << std::endl;

    return 0;
}
```

### Dynamic Structure Allocation

```cpp
#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    double gpa;
};

int main() {
    // Allocate structure on heap
    Student* pStudent = new Student;

    // Access members using arrow operator
    pStudent->name = "Alice Johnson";
    pStudent->id = 12345;
    pStudent->gpa = 3.85;

    std::cout << "Student: " << pStudent->name << std::endl;
    std::cout << "ID: " << pStudent->id << std::endl;
    std::cout << "GPA: " << pStudent->gpa << std::endl;

    // Free memory
    delete pStudent;
    pStudent = nullptr;

    return 0;
}
```

### Array of Structure Pointers

```cpp
#include <iostream>
#include <string>

struct Book {
    std::string title;
    std::string author;
    int pages;
};

int main() {
    const int NUM_BOOKS = 3;

    // Array of pointers to structures
    Book* library[NUM_BOOKS];

    // Allocate each book
    for (int i = 0; i < NUM_BOOKS; i++) {
        library[i] = new Book;
    }

    // Initialize
    library[0]->title = "1984";
    library[0]->author = "George Orwell";
    library[0]->pages = 328;

    library[1]->title = "Brave New World";
    library[1]->author = "Aldous Huxley";
    library[1]->pages = 268;

    library[2]->title = "Fahrenheit 451";
    library[2]->author = "Ray Bradbury";
    library[2]->pages = 158;

    // Display
    std::cout << "Library:\n";
    for (int i = 0; i < NUM_BOOKS; i++) {
        std::cout << (i + 1) << ". " << library[i]->title
                  << " by " << library[i]->author
                  << " (" << library[i]->pages << " pages)\n";
    }

    // Clean up
    for (int i = 0; i < NUM_BOOKS; i++) {
        delete library[i];
    }

    return 0;
}
```

---

## 8. Unions {#unions}

### What is a Union?

A **union** is similar to a structure but can store only **one** of its members at a time.

**Key Differences from Structures:**
- All members share the same memory location
- Only one member can hold a value at any time
- Size equals the size of the largest member
- Used for memory efficiency

### Basic Union

```cpp
#include <iostream>

union Data {
    int i;
    float f;
    char c;
};

int main() {
    Data data;

    data.i = 42;
    std::cout << "Integer: " << data.i << std::endl;

    data.f = 3.14f;
    std::cout << "Float: " << data.f << std::endl;
    std::cout << "Integer (corrupted): " << data.i << std::endl;  // Corrupted!

    data.c = 'A';
    std::cout << "Char: " << data.c << std::endl;
    std::cout << "Float (corrupted): " << data.f << std::endl;   // Corrupted!

    std::cout << "\nSize of union: " << sizeof(Data) << " bytes" << std::endl;

    return 0;
}
```

### Union with Type Tracking

```cpp
#include <iostream>
#include <string>

struct Value {
    enum Type { INT, FLOAT, STRING } type;

    union {
        int i;
        float f;
        char str[20];
    } data;
};

void displayValue(const Value& v) {
    switch (v.type) {
        case Value::INT:
            std::cout << "Integer: " << v.data.i << std::endl;
            break;
        case Value::FLOAT:
            std::cout << "Float: " << v.data.f << std::endl;
            break;
        case Value::STRING:
            std::cout << "String: " << v.data.str << std::endl;
            break;
    }
}

int main() {
    Value v1;
    v1.type = Value::INT;
    v1.data.i = 42;
    displayValue(v1);

    Value v2;
    v2.type = Value::FLOAT;
    v2.data.f = 3.14159f;
    displayValue(v2);

    Value v3;
    v3.type = Value::STRING;
    std::strcpy(v3.data.str, "Hello");
    displayValue(v3);

    return 0;
}
```

### Practical Union Example: IP Address

```cpp
#include <iostream>
#include <iomanip>

union IPAddress {
    uint32_t address;  // 32-bit integer
    uint8_t bytes[4];  // 4 bytes
};

void displayIP(const IPAddress& ip) {
    std::cout << (int)ip.bytes[0] << "."
              << (int)ip.bytes[1] << "."
              << (int)ip.bytes[2] << "."
              << (int)ip.bytes[3] << std::endl;
}

int main() {
    IPAddress ip;

    // Set as 32-bit integer
    ip.address = 0xC0A80101;  // 192.168.1.1

    std::cout << "IP Address (hex): 0x" << std::hex << ip.address << std::endl;
    std::cout << "IP Address (decimal): ";
    displayIP(ip);

    // Modify individual bytes
    ip.bytes[3] = 254;  // Change to 192.168.1.254
    std::cout << "Modified IP: ";
    displayIP(ip);

    return 0;
}
```

---

## 9. Practical Exercises {#exercises}

### Exercise 1: Student Database

```cpp
#include <iostream>
#include <string>
#include <iomanip>

struct Student {
    int id;
    std::string name;
    double gpa;
    std::string major;
};

void displayStudent(const Student& s) {
    std::cout << std::setw(5) << s.id << " | ";
    std::cout << std::setw(20) << std::left << s.name << " | ";
    std::cout << std::fixed << std::setprecision(2) << s.gpa << " | ";
    std::cout << s.major << std::endl;
}

Student findTopStudent(const Student students[], int size) {
    Student top = students[0];
    for (int i = 1; i < size; i++) {
        if (students[i].gpa > top.gpa) {
            top = students[i];
        }
    }
    return top;
}

double calculateAverageGPA(const Student students[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += students[i].gpa;
    }
    return sum / size;
}

int main() {
    const int SIZE = 5;
    Student students[SIZE] = {
        {101, "Alice Johnson", 3.85, "Computer Science"},
        {102, "Bob Smith", 3.62, "Mathematics"},
        {103, "Carol Davis", 3.91, "Engineering"},
        {104, "David Lee", 3.45, "Physics"},
        {105, "Eve Wilson", 3.78, "Chemistry"}
    };

    std::cout << "Student Database:\n";
    std::cout << std::setw(5) << "ID" << " | ";
    std::cout << std::setw(20) << std::left << "Name" << " | ";
    std::cout << "GPA  | Major\n";
    std::cout << std::string(60, '-') << std::endl;

    for (int i = 0; i < SIZE; i++) {
        displayStudent(students[i]);
    }

    Student topStudent = findTopStudent(students, SIZE);
    double avgGPA = calculateAverageGPA(students, SIZE);

    std::cout << "\nTop Student: " << topStudent.name
              << " (GPA: " << std::fixed << std::setprecision(2)
              << topStudent.gpa << ")\n";
    std::cout << "Average GPA: " << avgGPA << std::endl;

    return 0;
}
```

### Exercise 2: Vector Math with Structures

```cpp
#include <iostream>
#include <cmath>

struct Vector3D {
    double x, y, z;
};

Vector3D add(const Vector3D& v1, const Vector3D& v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3D subtract(const Vector3D& v1, const Vector3D& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3D scale(const Vector3D& v, double scalar) {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

double dot(const Vector3D& v1, const Vector3D& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double magnitude(const Vector3D& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D normalize(const Vector3D& v) {
    double mag = magnitude(v);
    if (mag > 0.0) {
        return {v.x / mag, v.y / mag, v.z / mag};
    }
    return {0, 0, 0};
}

void displayVector(const Vector3D& v) {
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main() {
    Vector3D v1 = {3.0, 4.0, 0.0};
    Vector3D v2 = {1.0, 2.0, 2.0};

    std::cout << "v1 = ";
    displayVector(v1);
    std::cout << std::endl;

    std::cout << "v2 = ";
    displayVector(v2);
    std::cout << std::endl;

    Vector3D sum = add(v1, v2);
    std::cout << "v1 + v2 = ";
    displayVector(sum);
    std::cout << std::endl;

    Vector3D diff = subtract(v1, v2);
    std::cout << "v1 - v2 = ";
    displayVector(diff);
    std::cout << std::endl;

    double dotProduct = dot(v1, v2);
    std::cout << "v1 · v2 = " << dotProduct << std::endl;

    double mag1 = magnitude(v1);
    std::cout << "|v1| = " << mag1 << std::endl;

    Vector3D normalized = normalize(v1);
    std::cout << "normalize(v1) = ";
    displayVector(normalized);
    std::cout << std::endl;

    return 0;
}
```

---

## 10. Complete Projects {#projects}

### Project 1: Employee Management System

```cpp
#include <iostream>
#include <string>
#include <iomanip>

struct Date {
    int day;
    int month;
    int year;
};

struct Employee {
    int id;
    std::string name;
    std::string department;
    double salary;
    Date hireDate;
};

class EmployeeManager {
private:
    Employee* employees;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Employee* newEmp = new Employee[capacity];
        for (int i = 0; i < size; i++) {
            newEmp[i] = employees[i];
        }
        delete[] employees;
        employees = newEmp;
    }

public:
    EmployeeManager() : size(0), capacity(5) {
        employees = new Employee[capacity];
    }

    ~EmployeeManager() {
        delete[] employees;
    }

    void addEmployee(const Employee& emp) {
        if (size >= capacity) {
            resize();
        }
        employees[size++] = emp;
    }

    void displayAll() const {
        std::cout << "\n=== Employee Database ===\n";
        std::cout << std::setw(5) << "ID" << " | ";
        std::cout << std::setw(20) << std::left << "Name" << " | ";
        std::cout << std::setw(15) << "Department" << " | ";
        std::cout << std::setw(10) << std::right << "Salary" << " | ";
        std::cout << "Hire Date\n";
        std::cout << std::string(80, '-') << std::endl;

        for (int i = 0; i < size; i++) {
            std::cout << std::setw(5) << employees[i].id << " | ";
            std::cout << std::setw(20) << std::left << employees[i].name << " | ";
            std::cout << std::setw(15) << employees[i].department << " | ";
            std::cout << "$" << std::setw(9) << std::right
                      << std::fixed << std::setprecision(2)
                      << employees[i].salary << " | ";
            std::cout << employees[i].hireDate.month << "/"
                      << employees[i].hireDate.day << "/"
                      << employees[i].hireDate.year << std::endl;
        }
    }

    void displayByDepartment(const std::string& dept) const {
        std::cout << "\n=== Employees in " << dept << " ===\n";
        bool found = false;

        for (int i = 0; i < size; i++) {
            if (employees[i].department == dept) {
                std::cout << employees[i].id << ": " << employees[i].name
                          << " - $" << std::fixed << std::setprecision(2)
                          << employees[i].salary << std::endl;
                found = true;
            }
        }

        if (!found) {
            std::cout << "No employees found in this department.\n";
        }
    }

    void giveRaise(int empId, double percentage) {
        for (int i = 0; i < size; i++) {
            if (employees[i].id == empId) {
                employees[i].salary *= (1.0 + percentage / 100.0);
                std::cout << "Raised " << employees[i].name << "'s salary to $"
                          << std::fixed << std::setprecision(2)
                          << employees[i].salary << std::endl;
                return;
            }
        }
        std::cout << "Employee ID " << empId << " not found.\n";
    }

    double getTotalPayroll() const {
        double total = 0.0;
        for (int i = 0; i < size; i++) {
            total += employees[i].salary;
        }
        return total;
    }
};

int main() {
    EmployeeManager manager;

    // Add employees
    manager.addEmployee({101, "Alice Johnson", "Engineering", 85000.0, {15, 3, 2020}});
    manager.addEmployee({102, "Bob Smith", "Marketing", 65000.0, {1, 7, 2019}});
    manager.addEmployee({103, "Carol Davis", "Engineering", 90000.0, {10, 1, 2021}});
    manager.addEmployee({104, "David Lee", "HR", 60000.0, {5, 9, 2018}});
    manager.addEmployee({105, "Eve Wilson", "Marketing", 70000.0, {20, 11, 2020}});

    manager.displayAll();

    manager.displayByDepartment("Engineering");

    std::cout << "\nGiving 10% raise to employee 102:\n";
    manager.giveRaise(102, 10.0);

    std::cout << "\nTotal Payroll: $" << std::fixed << std::setprecision(2)
              << manager.getTotalPayroll() << std::endl;

    return 0;
}
```

### Project 2: Geometric Shapes Calculator

```cpp
#include <iostream>
#include <cmath>
#include <string>

struct Point {
    double x, y;
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

struct Triangle {
    Point p1, p2, p3;
};

double distanceBetweenPoints(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Circle functions
double circleArea(const Circle& c) {
    return M_PI * c.radius * c.radius;
}

double circlePerimeter(const Circle& c) {
    return 2 * M_PI * c.radius;
}

bool isPointInCircle(const Circle& c, const Point& p) {
    return distanceBetweenPoints(c.center, p) <= c.radius;
}

// Rectangle functions
double rectangleArea(const Rectangle& r) {
    return r.width * r.height;
}

double rectanglePerimeter(const Rectangle& r) {
    return 2 * (r.width + r.height);
}

// Triangle functions
double triangleArea(const Triangle& t) {
    // Using Heron's formula
    double a = distanceBetweenPoints(t.p1, t.p2);
    double b = distanceBetweenPoints(t.p2, t.p3);
    double c = distanceBetweenPoints(t.p3, t.p1);
    double s = (a + b + c) / 2;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

double trianglePerimeter(const Triangle& t) {
    double a = distanceBetweenPoints(t.p1, t.p2);
    double b = distanceBetweenPoints(t.p2, t.p3);
    double c = distanceBetweenPoints(t.p3, t.p1);
    return a + b + c;
}

int main() {
    std::cout << "=== Geometric Shapes Calculator ===\n\n";

    // Circle
    Circle circle = {{0, 0}, 5.0};
    std::cout << "Circle (center: " << circle.center.x << ", "
              << circle.center.y << ", radius: " << circle.radius << ")\n";
    std::cout << "Area: " << circleArea(circle) << std::endl;
    std::cout << "Perimeter: " << circlePerimeter(circle) << std::endl;

    Point testPoint = {3, 4};
    std::cout << "Is point (" << testPoint.x << ", " << testPoint.y
              << ") inside? " << (isPointInCircle(circle, testPoint) ? "Yes" : "No")
              << "\n\n";

    // Rectangle
    Rectangle rect = {{0, 10}, 6, 4};
    std::cout << "Rectangle (top-left: " << rect.topLeft.x << ", "
              << rect.topLeft.y << ", width: " << rect.width
              << ", height: " << rect.height << ")\n";
    std::cout << "Area: " << rectangleArea(rect) << std::endl;
    std::cout << "Perimeter: " << rectanglePerimeter(rect) << "\n\n";

    // Triangle
    Triangle tri = {{0, 0}, {4, 0}, {2, 3}};
    std::cout << "Triangle vertices: ("
              << tri.p1.x << ", " << tri.p1.y << "), ("
              << tri.p2.x << ", " << tri.p2.y << "), ("
              << tri.p3.x << ", " << tri.p3.y << ")\n";
    std::cout << "Area: " << triangleArea(tri) << std::endl;
    std::cout << "Perimeter: " << trianglePerimeter(tri) << std::endl;

    return 0;
}
```

---

## Summary

In this lesson, you learned:

### Key Concepts

1. **Structures**
   - Grouping related data together
   - Defining and initializing structures
   - Accessing members with dot (.) operator
   - Copying and comparing structures

2. **Structures with Functions**
   - Passing structures by value vs reference
   - Returning structures from functions
   - Const references for efficiency

3. **Arrays of Structures**
   - Static arrays of structures
   - Dynamic arrays of structures
   - Processing structure arrays

4. **Nested Structures**
   - Structures within structures
   - Multiple levels of nesting
   - Accessing nested members

5. **Pointers to Structures**
   - Arrow operator (->)
   - Dynamic structure allocation
   - Arrays of structure pointers

6. **Unions**
   - Memory-efficient data storage
   - Sharing memory between members
   - Type tracking with enums
   - Practical applications

### Best Practices

- Use `const` references when passing structures to functions
- Initialize structures during declaration when possible
- Use arrow operator (->) with pointers to structures
- Keep track of which union member is active
- Group related data into structures
- Use meaningful structure names

---

## Next Lesson

**Lesson 12: Enumerations and Type Definitions**

You'll learn about:
- Enumerated types (enum)
- Scoped enumerations (enum class)
- Type aliases (typedef, using)
- Strong typing
- Enum with structures

---

## Quick Reference

```cpp
// Define structure
struct Point {
    int x, y;
};

// Create and initialize
Point p1 = {10, 20};
Point p2{15, 25};  // C++11

// Access members
p1.x = 30;
int y = p1.y;

// Pass to function
void display(const Point& p) { }

// Pointer to structure
Point* ptr = &p1;
ptr->x = 50;

// Dynamic allocation
Point* p = new Point{10, 20};
delete p;

// Union
union Data {
    int i;
    float f;
};
```

**Compilation:**
```bash
g++ -std=c++20 -Wall lesson11.cpp -o lesson11
./lesson11
```

---

**End of Lesson 11**
