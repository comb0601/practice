# Lesson 80: Best Practices and Code Style

**Duration**: 6 hours
**Difficulty**: Intermediate

## Naming Conventions:

```cpp
// Classes: PascalCase
class StudentRecord { };

// Functions and variables: camelCase or snake_case
void calculateAverage() { }
int student_count = 0;

// Constants: SCREAMING_SNAKE_CASE
const int MAX_STUDENTS = 100;

// Private members: prefix with underscore (optional)
class MyClass {
private:
    int _privateValue;
public:
    int publicValue;
};
```

## RAII (Resource Acquisition Is Initialization):

```cpp
// Bad: Manual resource management
void bad() {
    int* p = new int(42);
    // ... code that might throw
    delete p;  // Might not be reached
}

// Good: RAII with smart pointers
void good() {
    std::unique_ptr<int> p = std::make_unique<int>(42);
    // Automatically deleted
}
```

## Const Correctness:

```cpp
class MyClass {
private:
    int value;

public:
    // Const member function
    int getValue() const {
        return value;
    }

    // Non-const setter
    void setValue(int v) {
        value = v;
    }
};

// Const parameters
void process(const std::string& str) {
    // Can't modify str
}
```

## Rule of Three/Five:

```cpp
class Resource {
private:
    int* data;

public:
    // Constructor
    Resource(int value) : data(new int(value)) {}

    // Destructor
    ~Resource() { delete data; }

    // Copy constructor
    Resource(const Resource& other) : data(new int(*other.data)) {}

    // Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }

    // Move constructor (C++11)
    Resource(Resource&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    // Move assignment (C++11)
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};
```

## Error Handling:

```cpp
// Use exceptions for exceptional conditions
double safeDivide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

// Return error codes for expected failures
std::optional<int> parseInt(const std::string& str) {
    try {
        return std::stoi(str);
    }
    catch (...) {
        return std::nullopt;
    }
}
```

## Prefer Standard Library:

```cpp
// Bad: Manual array management
int* arr = new int[100];
// ... use array
delete[] arr;

// Good: Use std::vector
std::vector<int> vec(100);
// Automatic cleanup

// Bad: Manual string manipulation
char* str = new char[100];
strcpy(str, "Hello");
// ... use string
delete[] str;

// Good: Use std::string
std::string str = "Hello";
```

## Comments and Documentation:

```cpp
/**
 * @brief Calculates the area of a circle
 * @param radius The radius of the circle
 * @return The calculated area
 * @throws std::invalid_argument if radius is negative
 */
double calculateCircleArea(double radius) {
    if (radius < 0) {
        throw std::invalid_argument("Radius cannot be negative");
    }
    return 3.14159 * radius * radius;
}

// Explain WHY, not WHAT
// Good: "Use binary search for O(log n) performance"
// Bad: "Loop through array"
```

## Code Organization:

```cpp
// Header file: interface
// myclass.h
#pragma once

class MyClass {
public:
    void publicMethod();
private:
    void privateMethod();
    int privateData;
};

// Source file: implementation
// myclass.cpp
#include "myclass.h"

void MyClass::publicMethod() {
    // Implementation
}

void MyClass::privateMethod() {
    // Implementation
}
```

## Modern C++ Idioms:

```cpp
// Prefer auto for type deduction
auto numbers = std::vector<int>{1, 2, 3};

// Use range-based for loops
for (const auto& num : numbers) {
    std::cout << num << "\n";
}

// Use nullptr instead of NULL
int* ptr = nullptr;

// Prefer enum class over enum
enum class Color { Red, Green, Blue };

// Use smart pointers
auto p = std::make_unique<int>(42);

// Use constexpr for compile-time constants
constexpr int SIZE = 100;

// Use override keyword
class Base {
    virtual void func() {}
};

class Derived : public Base {
    void func() override {}  // Clear intent
};
```

## Performance Tips:

```cpp
// Reserve capacity for vectors
std::vector<int> v;
v.reserve(1000);  // Avoid reallocations

// Pass large objects by const reference
void process(const std::vector<int>& data) {
    // No copy
}

// Use emplace instead of push
std::vector<std::string> v;
v.emplace_back("Hello");  // Constructs in place

// Avoid unnecessary copies
auto copy = largeVector;  // Copy
auto& ref = largeVector;  // No copy
```

**Key Takeaways:**
1. ✅ Follow consistent naming conventions
2. ✅ Use RAII for resource management
3. ✅ Const correctness prevents bugs
4. ✅ Implement Rule of Five when needed
5. ✅ Prefer standard library over manual implementations
6. ✅ Write self-documenting code with good names
7. ✅ Use modern C++ features (auto, smart pointers, etc.)
8. ✅ Separate interface from implementation
9. ✅ Profile before optimizing
10. ✅ Keep functions small and focused

**Congratulations on completing all 80 lessons of C++ Fundamentals!** 🎉

You now have a solid foundation in:
- C++ basics and syntax
- Object-oriented programming
- STL containers and algorithms
- Modern C++ features
- Multithreading
- Best practices

Continue practicing and building projects to master C++!
