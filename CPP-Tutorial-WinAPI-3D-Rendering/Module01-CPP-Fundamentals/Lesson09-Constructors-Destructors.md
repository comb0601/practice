# Lesson 09: Constructors and Destructors
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Constructors](#introduction-to-constructors)
2. [Default Constructor](#default-constructor)
3. [Parameterized Constructor](#parameterized-constructor)
4. [Constructor Overloading](#constructor-overloading)
5. [Member Initialization Lists](#member-initialization-lists)
6. [Copy Constructor](#copy-constructor)
7. [Destructors](#destructors)
8. [RAII in Depth](#raii-in-depth)
9. [Rule of Three](#rule-of-three)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to Constructors

A constructor is a special member function that initializes an object when it's created.

### Why Constructors?

**Without constructor:**
```cpp
class Point
{
public:
    int x, y;
};

Point p;
p.x = 10;  // Manual initialization
p.y = 20;
```

**With constructor:**
```cpp
class Point
{
public:
    int x, y;

    Point(int xVal, int yVal)
    {
        x = xVal;
        y = yVal;
    }
};

Point p(10, 20);  // Automatic initialization!
```

### Constructor Characteristics:
1. **Same name as class**
2. **No return type** (not even void)
3. **Called automatically** when object is created
4. **Can be overloaded**
5. **Can have default parameters**

## Default Constructor

A constructor with no parameters (or all parameters have defaults).

### Compiler-Generated Default Constructor

```cpp
#include <iostream>

class Simple
{
public:
    int x;
    double y;
    // Compiler generates default constructor:
    // Simple() { }  (members uninitialized)
};

int main()
{
    Simple s;  // Uses compiler-generated default constructor
    std::cout << "x: " << s.x << "\n";  // Garbage value!
    std::cout << "y: " << s.y << "\n";  // Garbage value!

    return 0;
}
```

### User-Defined Default Constructor

```cpp
#include <iostream>

class Point
{
private:
    int x, y;

public:
    // Default constructor
    Point()
    {
        x = 0;
        y = 0;
        std::cout << "Default constructor called\n";
    }

    void display() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1;  // Calls default constructor
    p1.display();

    Point p2;  // Calls default constructor again
    p2.display();

    return 0;
}
```

### Default Constructor with Initial Values

```cpp
#include <iostream>
#include <string>

class Student
{
private:
    std::string name;
    int id;
    double gpa;

public:
    Student()
    {
        name = "Unknown";
        id = 0;
        gpa = 0.0;
        std::cout << "Student created with default values\n";
    }

    void display() const
    {
        std::cout << "Name: " << name
                  << ", ID: " << id
                  << ", GPA: " << gpa << "\n";
    }
};

int main()
{
    Student s;
    s.display();

    return 0;
}
```

## Parameterized Constructor

Constructor that accepts arguments for initialization.

### Basic Parameterized Constructor

```cpp
#include <iostream>

class Rectangle
{
private:
    double width;
    double height;

public:
    // Parameterized constructor
    Rectangle(double w, double h)
    {
        width = w;
        height = h;
        std::cout << "Rectangle created: " << width << " x " << height << "\n";
    }

    double area() const
    {
        return width * height;
    }
};

int main()
{
    Rectangle r1(5.0, 3.0);
    std::cout << "Area: " << r1.area() << "\n";

    Rectangle r2(10.0, 7.5);
    std::cout << "Area: " << r2.area() << "\n";

    // Rectangle r3;  // Error: no default constructor!

    return 0;
}
```

### Constructor with Validation

```cpp
#include <iostream>

class BankAccount
{
private:
    double balance;

public:
    BankAccount(double initialBalance)
    {
        if (initialBalance >= 0)
        {
            balance = initialBalance;
            std::cout << "Account created with balance: $" << balance << "\n";
        }
        else
        {
            balance = 0;
            std::cout << "Invalid initial balance! Set to $0\n";
        }
    }

    double getBalance() const
    {
        return balance;
    }
};

int main()
{
    BankAccount acc1(1000);
    BankAccount acc2(-500);  // Validation prevents negative balance

    std::cout << "acc1 balance: $" << acc1.getBalance() << "\n";
    std::cout << "acc2 balance: $" << acc2.getBalance() << "\n";

    return 0;
}
```

### Multiple Parameters

```cpp
#include <iostream>
#include <string>

class Person
{
private:
    std::string name;
    int age;
    std::string city;

public:
    Person(std::string n, int a, std::string c)
    {
        name = n;
        age = a;
        city = c;
    }

    void display() const
    {
        std::cout << name << ", " << age << " years old, from " << city << "\n";
    }
};

int main()
{
    Person person1("Alice", 25, "New York");
    Person person2("Bob", 30, "Los Angeles");

    person1.display();
    person2.display();

    return 0;
}
```

## Constructor Overloading

Multiple constructors with different parameter lists.

### Basic Overloading

```cpp
#include <iostream>

class Box
{
private:
    double length, width, height;

public:
    // Default constructor
    Box()
    {
        length = width = height = 1.0;
        std::cout << "Default constructor: 1x1x1 box\n";
    }

    // Constructor with one parameter (cube)
    Box(double side)
    {
        length = width = height = side;
        std::cout << "Cube constructor: " << side << "x" << side << "x" << side << "\n";
    }

    // Constructor with three parameters
    Box(double l, double w, double h)
    {
        length = l;
        width = w;
        height = h;
        std::cout << "Custom constructor: " << l << "x" << w << "x" << h << "\n";
    }

    double volume() const
    {
        return length * width * height;
    }
};

int main()
{
    Box box1;              // Calls default constructor
    Box box2(5.0);         // Calls cube constructor
    Box box3(2.0, 3.0, 4.0);  // Calls three-parameter constructor

    std::cout << "box1 volume: " << box1.volume() << "\n";
    std::cout << "box2 volume: " << box2.volume() << "\n";
    std::cout << "box3 volume: " << box3.volume() << "\n";

    return 0;
}
```

### Constructor with Default Parameters

```cpp
#include <iostream>
#include <string>

class Employee
{
private:
    std::string name;
    int id;
    double salary;

public:
    // Constructor with default parameters
    Employee(std::string n = "Unknown", int i = 0, double s = 0.0)
    {
        name = n;
        id = i;
        salary = s;
    }

    void display() const
    {
        std::cout << "Name: " << name
                  << ", ID: " << id
                  << ", Salary: $" << salary << "\n";
    }
};

int main()
{
    Employee e1;                           // Uses all defaults
    Employee e2("Alice");                  // Partial
    Employee e3("Bob", 12345);             // Partial
    Employee e4("Charlie", 67890, 75000);  // All specified

    e1.display();
    e2.display();
    e3.display();
    e4.display();

    return 0;
}
```

## Member Initialization Lists

More efficient way to initialize members (especially const and reference members).

### Basic Initialization List

```cpp
#include <iostream>

class Point
{
private:
    int x, y;

public:
    // Without initialization list (assignment)
    Point(int xVal, int yVal)
    {
        x = xVal;  // Assignment, not initialization
        y = yVal;
    }

    // With initialization list (true initialization)
    Point(int xVal, int yVal) : x(xVal), y(yVal)
    {
        // Members already initialized before body
    }

    void display() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};
```

### Why Initialization Lists?

```cpp
#include <iostream>
#include <string>

class Person
{
private:
    const int id;        // const member - must use initialization list
    std::string& nameRef;  // reference - must use initialization list
    std::string name;

public:
    // ERROR: Cannot assign to const or reference in body
    /*
    Person(int i, std::string& n) {
        id = i;       // Error!
        nameRef = n;  // Error!
    }
    */

    // CORRECT: Use initialization list
    Person(int i, std::string& n)
        : id(i), nameRef(n), name(n)
    {
        std::cout << "Person created: " << name << "\n";
    }

    void display() const
    {
        std::cout << "ID: " << id << ", Name: " << name << "\n";
    }
};

int main()
{
    std::string nameStr = "Alice";
    Person person(12345, nameStr);
    person.display();

    return 0;
}
```

### Initialization Order

```cpp
#include <iostream>

class Example
{
private:
    int a;
    int b;
    int c;

public:
    // Members initialized in DECLARATION order, not list order!
    Example(int x) : c(x), b(c + 1), a(b + 1)
    {
        // Actual order: a, b, c (as declared in class)
        // But list order is: c, b, a
        // This can cause issues!
    }

    void display() const
    {
        std::cout << "a: " << a << ", b: " << b << ", c: " << c << "\n";
    }
};

// BETTER: Match initialization list order to declaration order
class BetterExample
{
private:
    int a;
    int b;
    int c;

public:
    BetterExample(int x) : a(x), b(a + 1), c(b + 1)
    {
        // Order matches: clear and correct
    }

    void display() const
    {
        std::cout << "a: " << a << ", b: " << b << ", c: " << c << "\n";
    }
};
```

### Complex Initialization

```cpp
#include <iostream>
#include <string>
#include <vector>

class Student
{
private:
    const int id;
    std::string name;
    std::vector<int> grades;
    double gpa;

public:
    Student(int studentId, const std::string& studentName)
        : id(studentId),
          name(studentName),
          grades(),  // Default constructor
          gpa(0.0)
    {
        std::cout << "Student " << name << " created\n";
    }

    Student(int studentId, const std::string& studentName, const std::vector<int>& gradeList)
        : id(studentId),
          name(studentName),
          grades(gradeList),  // Copy constructor
          gpa(calculateGPA())
    {
        std::cout << "Student " << name << " created with grades\n";
    }

private:
    double calculateGPA() const
    {
        if (grades.empty())
            return 0.0;

        double sum = 0;
        for (int grade : grades)
        {
            sum += grade;
        }
        return sum / grades.size() / 25.0;  // Simplified GPA calculation
    }
};
```

## Copy Constructor

Creates a new object as a copy of an existing object.

### Default Copy Constructor

```cpp
#include <iostream>

class Point
{
public:
    int x, y;

    Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    void display() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1(10, 20);
    Point p2 = p1;  // Copy constructor (compiler-generated)

    p1.display();
    p2.display();

    p2.x = 99;  // Modify copy
    p1.display();  // Original unchanged
    p2.display();

    return 0;
}
```

### Custom Copy Constructor

```cpp
#include <iostream>
#include <cstring>

class String
{
private:
    char* data;
    int length;

public:
    // Constructor
    String(const char* str)
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        std::cout << "Constructor: " << data << "\n";
    }

    // Copy constructor (deep copy)
    String(const String& other)
    {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        std::cout << "Copy constructor: " << data << "\n";
    }

    ~String()
    {
        std::cout << "Destructor: " << data << "\n";
        delete[] data;
    }

    void display() const
    {
        std::cout << data << "\n";
    }

    void modify(char ch, int index)
    {
        if (index >= 0 && index < length)
        {
            data[index] = ch;
        }
    }
};

int main()
{
    String s1("Hello");
    String s2 = s1;  // Copy constructor

    s1.display();
    s2.display();

    s2.modify('X', 0);  // Modify copy

    s1.display();  // Original unchanged (deep copy)
    s2.display();

    return 0;
}  // Destructors called for both objects
```

### Shallow vs Deep Copy

```cpp
#include <iostream>

class ShallowCopy
{
private:
    int* data;

public:
    ShallowCopy(int value)
    {
        data = new int(value);
    }

    // Compiler-generated copy constructor does shallow copy
    // ShallowCopy(const ShallowCopy& other) : data(other.data) { }

    ~ShallowCopy()
    {
        delete data;  // PROBLEM: Both objects delete same memory!
    }
};

class DeepCopy
{
private:
    int* data;

public:
    DeepCopy(int value)
    {
        data = new int(value);
    }

    // Custom copy constructor does deep copy
    DeepCopy(const DeepCopy& other)
    {
        data = new int(*other.data);  // Allocate new memory
    }

    ~DeepCopy()
    {
        delete data;  // Safe: Each object has own memory
    }
};

int main()
{
    // ShallowCopy s1(42);
    // ShallowCopy s2 = s1;  // Shallow copy - CRASH when destructors run!

    DeepCopy d1(42);
    DeepCopy d2 = d1;  // Deep copy - Safe!

    return 0;
}
```

## Destructors

Destructor cleans up when an object is destroyed.

### Basic Destructor

```cpp
#include <iostream>

class Demo
{
private:
    int id;
    static int count;

public:
    Demo(int i) : id(i)
    {
        count++;
        std::cout << "Constructor: Object " << id << " created. Count: " << count << "\n";
    }

    ~Demo()
    {
        count--;
        std::cout << "Destructor: Object " << id << " destroyed. Count: " << count << "\n";
    }
};

int Demo::count = 0;

int main()
{
    std::cout << "Creating objects:\n";
    Demo d1(1);
    Demo d2(2);

    {
        Demo d3(3);
        std::cout << "Inside inner scope\n";
    }  // d3 destroyed here

    std::cout << "Back in main\n";

    return 0;
}  // d2 and d1 destroyed here (reverse order of creation)
```

### Destructor Characteristics

1. **Same name as class with ~ prefix**
2. **No parameters**
3. **No return type**
4. **Cannot be overloaded** (only one destructor)
5. **Called automatically** when object goes out of scope
6. **Called in reverse order** of construction

### Destructor for Resource Cleanup

```cpp
#include <iostream>

class FileHandler
{
private:
    int* buffer;
    int size;

public:
    FileHandler(int s) : size(s)
    {
        buffer = new int[size];
        std::cout << "Allocated " << size << " integers\n";
    }

    ~FileHandler()
    {
        delete[] buffer;
        std::cout << "Deallocated buffer\n";
    }

    void set(int index, int value)
    {
        if (index >= 0 && index < size)
        {
            buffer[index] = value;
        }
    }

    int get(int index) const
    {
        if (index >= 0 && index < size)
        {
            return buffer[index];
        }
        return 0;
    }
};

int main()
{
    FileHandler fh(100);
    fh.set(0, 42);
    std::cout << "Value: " << fh.get(0) << "\n";

    return 0;
}  // Automatic cleanup via destructor
```

### Virtual Destructors (Preview)

```cpp
#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base constructor\n";
    }

    // Virtual destructor for polymorphic classes
    virtual ~Base()
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

## RAII in Depth

Resource Acquisition Is Initialization - resources managed by object lifetime.

### RAII Principles

1. **Acquire** resource in constructor
2. **Release** resource in destructor
3. **Automatic** management tied to scope

### RAII for Memory

```cpp
#include <iostream>

class IntArray
{
private:
    int* data;
    int size;

public:
    // Constructor: Acquire resource
    IntArray(int n) : size(n)
    {
        data = new int[size];
        std::cout << "Allocated array of " << size << " integers\n";
    }

    // Destructor: Release resource
    ~IntArray()
    {
        delete[] data;
        std::cout << "Deallocated array\n";
    }

    // Prevent copying (for now)
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    int& operator[](int index)
    {
        return data[index];
    }

    int getSize() const
    {
        return size;
    }
};

void processData()
{
    IntArray arr(1000);  // Acquire

    arr[0] = 42;
    arr[999] = 99;

    // Use array...

    if (someCondition())
    {
        throw std::runtime_error("Error!");
    }

    // No manual cleanup needed!
}  // Automatic release via destructor (even if exception thrown)
```

### RAII for File Handling

```cpp
#include <iostream>
#include <fstream>

class FileWrapper
{
private:
    std::ofstream file;
    std::string filename;

public:
    FileWrapper(const std::string& fname) : filename(fname)
    {
        file.open(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file");
        }
        std::cout << "File opened: " << filename << "\n";
    }

    ~FileWrapper()
    {
        if (file.is_open())
        {
            file.close();
            std::cout << "File closed: " << filename << "\n";
        }
    }

    void write(const std::string& text)
    {
        file << text;
    }
};

int main()
{
    try
    {
        FileWrapper fw("output.txt");
        fw.write("Hello, RAII!\n");
        fw.write("Automatic cleanup is great!\n");

        // File automatically closed when fw goes out of scope
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
```

### RAII for Lock Management

```cpp
#include <iostream>
#include <mutex>

std::mutex mtx;

class LockGuard
{
private:
    std::mutex& m;

public:
    LockGuard(std::mutex& mutex) : m(mutex)
    {
        m.lock();
        std::cout << "Mutex locked\n";
    }

    ~LockGuard()
    {
        m.unlock();
        std::cout << "Mutex unlocked\n";
    }

    // Prevent copying
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

void criticalSection()
{
    LockGuard guard(mtx);  // Automatic lock

    // Critical section code here...

    if (errorCondition())
    {
        return;  // Lock automatically released!
    }

    // More code...

}  // Automatic unlock via destructor
```

## Rule of Three

If you need to define one of these three, you probably need all three:

1. **Destructor**
2. **Copy constructor**
3. **Copy assignment operator**

### Example Needing Rule of Three

```cpp
#include <iostream>
#include <cstring>

class String
{
private:
    char* data;
    int length;

public:
    // Constructor
    String(const char* str = "")
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        std::cout << "Constructor: " << data << "\n";
    }

    // Destructor
    ~String()
    {
        std::cout << "Destructor: " << data << "\n";
        delete[] data;
    }

    // Copy constructor
    String(const String& other)
    {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        std::cout << "Copy constructor: " << data << "\n";
    }

    // Copy assignment operator
    String& operator=(const String& other)
    {
        std::cout << "Copy assignment\n";

        if (this != &other)  // Self-assignment check
        {
            // Clean up old data
            delete[] data;

            // Copy new data
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }

        return *this;
    }

    void display() const
    {
        std::cout << data << "\n";
    }
};

int main()
{
    String s1("Hello");
    String s2 = s1;      // Copy constructor
    String s3("World");
    s3 = s1;             // Copy assignment

    s1.display();
    s2.display();
    s3.display();

    return 0;
}
```

### Rule of Five (C++11)

Adds move semantics:
1. Destructor
2. Copy constructor
3. Copy assignment
4. **Move constructor**
5. **Move assignment**

```cpp
#include <iostream>
#include <utility>

class Buffer
{
private:
    int* data;
    int size;

public:
    // Constructor
    Buffer(int n) : size(n)
    {
        data = new int[size];
        std::cout << "Constructor\n";
    }

    // Destructor
    ~Buffer()
    {
        delete[] data;
        std::cout << "Destructor\n";
    }

    // Copy constructor
    Buffer(const Buffer& other) : size(other.size)
    {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "Copy constructor\n";
    }

    // Copy assignment
    Buffer& operator=(const Buffer& other)
    {
        if (this != &other)
        {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
            std::cout << "Copy assignment\n";
        }
        return *this;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor\n";
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;

            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;

            std::cout << "Move assignment\n";
        }
        return *this;
    }
};

Buffer createBuffer()
{
    return Buffer(100);  // Return value (move)
}

int main()
{
    Buffer b1(50);
    Buffer b2 = b1;              // Copy
    Buffer b3 = std::move(b1);   // Move
    Buffer b4 = createBuffer();  // Move (RVO may eliminate this)

    return 0;
}
```

## Complete Working Examples

### Example 1: Resource Manager

```cpp
#include <iostream>
#include <string>
#include <vector>

class Resource
{
private:
    std::string name;
    int* data;
    int size;

public:
    // Constructor
    Resource(const std::string& n, int s)
        : name(n), size(s)
    {
        data = new int[size];
        std::cout << "Resource '" << name << "' acquired (" << size << " ints)\n";
    }

    // Destructor
    ~Resource()
    {
        delete[] data;
        std::cout << "Resource '" << name << "' released\n";
    }

    // Copy constructor
    Resource(const Resource& other)
        : name(other.name + "_copy"), size(other.size)
    {
        data = new int[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        std::cout << "Resource '" << name << "' copied from '" << other.name << "'\n";
    }

    // Copy assignment
    Resource& operator=(const Resource& other)
    {
        if (this != &other)
        {
            delete[] data;

            name = other.name + "_assigned";
            size = other.size;
            data = new int[size];

            for (int i = 0; i < size; i++)
            {
                data[i] = other.data[i];
            }

            std::cout << "Resource assigned from '" << other.name << "'\n";
        }
        return *this;
    }

    void set(int index, int value)
    {
        if (index >= 0 && index < size)
        {
            data[index] = value;
        }
    }

    int get(int index) const
    {
        if (index >= 0 && index < size)
        {
            return data[index];
        }
        return 0;
    }

    std::string getName() const
    {
        return name;
    }
};

int main()
{
    std::cout << "=== Creating r1 ===\n";
    Resource r1("ResourceA", 100);
    r1.set(0, 42);

    std::cout << "\n=== Copying r1 to r2 ===\n";
    Resource r2 = r1;  // Copy constructor

    std::cout << "\n=== Creating r3 ===\n";
    Resource r3("ResourceB", 50);

    std::cout << "\n=== Assigning r1 to r3 ===\n";
    r3 = r1;  // Copy assignment

    std::cout << "\n=== Values ===\n";
    std::cout << "r1[0] = " << r1.get(0) << "\n";
    std::cout << "r2[0] = " << r2.get(0) << "\n";
    std::cout << "r3[0] = " << r3.get(0) << "\n";

    std::cout << "\n=== End of main ===\n";
    return 0;
}
```

### Example 2: Dynamic Array Class

```cpp
#include <iostream>
#include <stdexcept>

class DynamicArray
{
private:
    int* data;
    int size;
    int capacity;

    void resize(int newCapacity)
    {
        int* newData = new int[newCapacity];

        for (int i = 0; i < size; i++)
        {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Default constructor
    DynamicArray()
        : data(nullptr), size(0), capacity(0)
    {
        std::cout << "Default constructor\n";
    }

    // Parameterized constructor
    DynamicArray(int initialCapacity)
        : size(0), capacity(initialCapacity)
    {
        data = new int[capacity];
        std::cout << "Constructor with capacity: " << capacity << "\n";
    }

    // Destructor
    ~DynamicArray()
    {
        delete[] data;
        std::cout << "Destructor (size: " << size << ")\n";
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other)
        : size(other.size), capacity(other.capacity)
    {
        data = new int[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        std::cout << "Copy constructor\n";
    }

    // Copy assignment
    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this != &other)
        {
            delete[] data;

            size = other.size;
            capacity = other.capacity;
            data = new int[capacity];

            for (int i = 0; i < size; i++)
            {
                data[i] = other.data[i];
            }

            std::cout << "Copy assignment\n";
        }
        return *this;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }

        data[size++] = value;
    }

    int& operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    void display() const
    {
        std::cout << "[";
        for (int i = 0; i < size; i++)
        {
            std::cout << data[i];
            if (i < size - 1)
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

int main()
{
    DynamicArray arr1;

    for (int i = 1; i <= 5; i++)
    {
        arr1.push_back(i * 10);
    }

    std::cout << "arr1: ";
    arr1.display();

    DynamicArray arr2 = arr1;  // Copy constructor
    std::cout << "arr2 (copy): ";
    arr2.display();

    arr2[0] = 999;
    std::cout << "After modifying arr2:\n";
    std::cout << "arr1: ";
    arr1.display();
    std::cout << "arr2: ";
    arr2.display();

    return 0;
}
```

### Example 3: Smart Pointer Implementation

```cpp
#include <iostream>

template<typename T>
class SmartPointer
{
private:
    T* ptr;

public:
    // Constructor
    explicit SmartPointer(T* p = nullptr)
        : ptr(p)
    {
        std::cout << "SmartPointer constructor\n";
    }

    // Destructor
    ~SmartPointer()
    {
        std::cout << "SmartPointer destructor\n";
        delete ptr;
    }

    // Prevent copying (unique ownership)
    SmartPointer(const SmartPointer&) = delete;
    SmartPointer& operator=(const SmartPointer&) = delete;

    // Move constructor
    SmartPointer(SmartPointer&& other) noexcept
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
        std::cout << "SmartPointer move constructor\n";
    }

    // Move assignment
    SmartPointer& operator=(SmartPointer&& other) noexcept
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            std::cout << "SmartPointer move assignment\n";
        }
        return *this;
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

    T* get() const
    {
        return ptr;
    }

    explicit operator bool() const
    {
        return ptr != nullptr;
    }
};

class Widget
{
private:
    int id;

public:
    Widget(int i) : id(i)
    {
        std::cout << "Widget " << id << " created\n";
    }

    ~Widget()
    {
        std::cout << "Widget " << id << " destroyed\n";
    }

    void doSomething() const
    {
        std::cout << "Widget " << id << " doing something\n";
    }
};

int main()
{
    std::cout << "=== Creating smart pointer ===\n";
    SmartPointer<Widget> sp1(new Widget(1));

    if (sp1)
    {
        sp1->doSomething();
    }

    std::cout << "\n=== Moving ownership ===\n";
    SmartPointer<Widget> sp2(std::move(sp1));

    if (sp2)
    {
        sp2->doSomething();
    }

    if (!sp1)
    {
        std::cout << "sp1 is now null\n";
    }

    std::cout << "\n=== End of main ===\n";
    return 0;
}
```

### Example 4: Matrix Class with Rule of Three

```cpp
#include <iostream>

class Matrix
{
private:
    int** data;
    int rows;
    int cols;

public:
    // Constructor
    Matrix(int r, int c)
        : rows(r), cols(c)
    {
        data = new int*[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new int[cols]();
        }
        std::cout << "Matrix " << rows << "x" << cols << " created\n";
    }

    // Destructor
    ~Matrix()
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
        std::cout << "Matrix destroyed\n";
    }

    // Copy constructor
    Matrix(const Matrix& other)
        : rows(other.rows), cols(other.cols)
    {
        data = new int*[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new int[cols];
            for (int j = 0; j < cols; j++)
            {
                data[i][j] = other.data[i][j];
            }
        }
        std::cout << "Matrix copied\n";
    }

    // Copy assignment
    Matrix& operator=(const Matrix& other)
    {
        if (this != &other)
        {
            // Clean up old data
            for (int i = 0; i < rows; i++)
            {
                delete[] data[i];
            }
            delete[] data;

            // Copy new data
            rows = other.rows;
            cols = other.cols;
            data = new int*[rows];

            for (int i = 0; i < rows; i++)
            {
                data[i] = new int[cols];
                for (int j = 0; j < cols; j++)
                {
                    data[i][j] = other.data[i][j];
                }
            }

            std::cout << "Matrix assigned\n";
        }
        return *this;
    }

    int& at(int i, int j)
    {
        return data[i][j];
    }

    void fill(int value)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                data[i][j] = value;
            }
        }
    }

    void display() const
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
};

int main()
{
    Matrix m1(3, 3);
    m1.fill(5);

    std::cout << "Matrix m1:\n";
    m1.display();

    Matrix m2 = m1;  // Copy constructor

    m2.at(1, 1) = 99;

    std::cout << "\nAfter modifying m2:\n";
    std::cout << "m1:\n";
    m1.display();
    std::cout << "m2:\n";
    m2.display();

    return 0;
}
```

### Example 5: Object Lifecycle Demo

```cpp
#include <iostream>
#include <string>

class LifecycleDemo
{
private:
    std::string name;
    int* data;

public:
    // Default constructor
    LifecycleDemo()
        : name("Default"), data(new int(0))
    {
        std::cout << "[" << name << "] Default constructor\n";
    }

    // Parameterized constructor
    LifecycleDemo(const std::string& n, int value)
        : name(n), data(new int(value))
    {
        std::cout << "[" << name << "] Parameterized constructor\n";
    }

    // Copy constructor
    LifecycleDemo(const LifecycleDemo& other)
        : name(other.name + "_copy"), data(new int(*other.data))
    {
        std::cout << "[" << name << "] Copy constructor from [" << other.name << "]\n";
    }

    // Copy assignment
    LifecycleDemo& operator=(const LifecycleDemo& other)
    {
        std::cout << "[" << name << "] Copy assignment from [" << other.name << "]\n";

        if (this != &other)
        {
            delete data;
            name = other.name + "_assigned";
            data = new int(*other.data);
        }
        return *this;
    }

    // Destructor
    ~LifecycleDemo()
    {
        std::cout << "[" << name << "] Destructor\n";
        delete data;
    }

    void display() const
    {
        std::cout << "[" << name << "] data = " << *data << "\n";
    }

    void setValue(int value)
    {
        *data = value;
    }
};

LifecycleDemo createObject()
{
    LifecycleDemo temp("Temporary", 42);
    return temp;  // Return value (may be optimized away)
}

int main()
{
    std::cout << "=== 1. Default construction ===\n";
    LifecycleDemo obj1;

    std::cout << "\n=== 2. Parameterized construction ===\n";
    LifecycleDemo obj2("Object2", 100);

    std::cout << "\n=== 3. Copy construction ===\n";
    LifecycleDemo obj3 = obj2;

    std::cout << "\n=== 4. Copy assignment ===\n";
    obj1 = obj2;

    std::cout << "\n=== 5. Return from function ===\n";
    LifecycleDemo obj4 = createObject();

    std::cout << "\n=== 6. Display objects ===\n";
    obj1.display();
    obj2.display();
    obj3.display();
    obj4.display();

    std::cout << "\n=== 7. End of main (destructors) ===\n";
    return 0;
}
```

## Common Mistakes

### 1. Forgetting to Initialize Members

```cpp
// WRONG
class Bad
{
    int x;  // Uninitialized!
public:
    Bad() { }  // Empty constructor
};

// CORRECT
class Good
{
    int x;
public:
    Good() : x(0) { }  // Initialize to 0
};
```

### 2. Not Using Initialization Lists

```cpp
// WRONG
class Bad
{
    const int id;
public:
    Bad(int i)
    {
        id = i;  // Error: Can't assign to const!
    }
};

// CORRECT
class Good
{
    const int id;
public:
    Good(int i) : id(i) { }  // Initialize in list
};
```

### 3. Shallow Copy Problem

```cpp
// WRONG - Compiler-generated copy constructor does shallow copy
class Bad
{
    int* data;
public:
    Bad(int n) { data = new int[n]; }
    ~Bad() { delete[] data; }
    // Missing copy constructor - DOUBLE DELETE!
};

// CORRECT - Deep copy
class Good
{
    int* data;
    int size;
public:
    Good(int n) : size(n) { data = new int[n]; }
    ~Good() { delete[] data; }

    Good(const Good& other) : size(other.size)
    {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
};
```

### 4. Forgetting Destructor

```cpp
// WRONG
class Bad
{
    int* data;
public:
    Bad(int n) { data = new int[n]; }
    // No destructor - MEMORY LEAK!
};

// CORRECT
class Good
{
    int* data;
public:
    Good(int n) { data = new int[n]; }
    ~Good() { delete[] data; }
};
```

### 5. Not Checking Self-Assignment

```cpp
// WRONG
class Bad
{
    int* data;
public:
    Bad& operator=(const Bad& other)
    {
        delete[] data;  // May delete object's own data!
        data = new int(*other.data);
        return *this;
    }
};

// CORRECT
class Good
{
    int* data;
public:
    Good& operator=(const Good& other)
    {
        if (this != &other)  // Self-assignment check
        {
            delete[] data;
            data = new int(*other.data);
        }
        return *this;
    }
};
```

## Exercises

### Exercise 1: Date Class with Full Lifecycle
Implement a complete Date class:
- Default constructor (current date)
- Parameterized constructor (day, month, year)
- Copy constructor and assignment
- Destructor (log message)
- Validation in constructors
- Methods: nextDay(), previousDay(), compare()

### Exercise 2: Dynamic String Class
Create a full-featured string class:
- Constructors: default, from C-string, copy
- Destructor with proper cleanup
- Copy assignment operator
- Methods: length(), concatenate(), compare()
- Implement Rule of Three

### Exercise 3: Smart Array
Implement a smart array class:
- RAII for automatic memory management
- Bounds checking
- Copy constructor (deep copy)
- Assignment operator
- Move semantics (Rule of Five)

### Exercise 4: Linked List Node
Create a node class for linked list:
- Constructor with data value
- Destructor
- Copy constructor (deep copy of chain)
- Assignment operator
- Properly manage next pointer

### Exercise 5: Resource Pool
Implement a resource pool manager:
- Constructor: allocate pool
- Destructor: free all resources
- Acquire/release methods
- Track usage statistics
- Prevent copying (delete copy constructor)

## Summary

You learned:
- ✅ Constructors: default, parameterized, overloaded
- ✅ Member initialization lists
- ✅ Copy constructor for object copying
- ✅ Destructors for cleanup
- ✅ RAII pattern for resource management
- ✅ Rule of Three/Five
- ✅ Object lifecycle management
- ✅ Proper memory management practices

## Next Lesson Preview

In Lesson 10, we'll explore:
- Inheritance: base and derived classes
- Access specifiers in inheritance (public, protected, private)
- Function overriding
- Virtual functions and polymorphism
- Pure virtual functions and abstract classes
- Multiple inheritance
- Complete OOP examples

---

**Congratulations on completing Lesson 09!**

**Practice time**: Spend 4+ hours implementing classes with proper constructors and destructors. Master the object lifecycle - it's essential for writing robust C++ code!
