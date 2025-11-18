# Lesson 21: Move Semantics

## Table of Contents
1. [Introduction to Move Semantics](#introduction-to-move-semantics)
2. [Understanding Lvalues and Rvalues](#understanding-lvalues-and-rvalues)
3. [Rvalue References](#rvalue-references)
4. [Move Constructor](#move-constructor)
5. [Move Assignment Operator](#move-assignment-operator)
6. [std::move](#stdmove)
7. [When to Use Move Semantics](#when-to-use-move-semantics)
8. [The Rule of Five](#the-rule-of-five)
9. [Perfect Forwarding Introduction](#perfect-forwarding-introduction)
10. [Move Semantics and STL](#move-semantics-and-stl)
11. [Performance Considerations](#performance-considerations)
12. [Common Pitfalls](#common-pitfalls)
13. [Best Practices](#best-practices)

## Introduction to Move Semantics

Move semantics is one of the most important features introduced in C++11. It addresses a fundamental performance problem in C++: unnecessary copying of temporary objects. Before C++11, when you returned an object from a function or passed a temporary to another function, the object would be copied, even if the original was about to be destroyed.

### The Problem Before C++11

```cpp
std::vector<int> createLargeVector() {
    std::vector<int> result(1000000);
    // fill vector...
    return result;  // Expensive copy!
}

std::vector<int> v = createLargeVector();  // Another copy!
```

Before move semantics, this code would perform multiple expensive deep copies of a million integers, even though the temporary vector returned from the function is immediately destroyed.

### The Solution: Move Semantics

Move semantics allows us to "steal" resources from temporary objects that are about to be destroyed anyway, avoiding expensive copies:

```cpp
std::vector<int> v = createLargeVector();  // Move, not copy!
```

With move semantics, instead of copying all million integers, we just transfer ownership of the internal pointer, which is extremely fast.

## Understanding Lvalues and Rvalues

To understand move semantics, you must first understand the distinction between lvalues and rvalues.

### Lvalues

An **lvalue** (locator value) is an expression that refers to a memory location and allows us to take its address:

```cpp
int x = 42;        // x is an lvalue
int* p = &x;       // Can take address of lvalue
x = 100;           // Can assign to lvalue
```

**Characteristics of lvalues:**
- Has a persistent address
- Can appear on the left side of an assignment
- Named variables, array elements, dereferenced pointers
- Can be const or non-const

### Rvalues

An **rvalue** (right value) is a temporary expression that doesn't have a persistent address:

```cpp
int x = 42;        // 42 is an rvalue
int y = x + 5;     // (x + 5) is an rvalue
int* p = &(x + 5); // ERROR: Can't take address of rvalue
```

**Characteristics of rvalues:**
- Temporary, about to be destroyed
- Typically appears on the right side of an assignment
- Literals, temporary objects, expressions
- Cannot take its address (with &&, we can bind references to them)

### Categories in Detail

Modern C++ actually has several value categories:

1. **lvalue**: Has identity, cannot be moved from
2. **xvalue** (expiring value): Has identity, can be moved from
3. **prvalue** (pure rvalue): No identity, can be moved from
4. **glvalue** (generalized lvalue): lvalue or xvalue
5. **rvalue**: xvalue or prvalue

For practical purposes, the main distinction is:
- **lvalues**: Named objects that persist
- **rvalues**: Temporary objects that will be destroyed

## Rvalue References

C++11 introduces a new type of reference: the **rvalue reference**, denoted by `&&`.

### Syntax

```cpp
int&& rref = 42;           // Bind rvalue reference to rvalue
int x = 10;
int&& rref2 = x;           // ERROR: Can't bind rvalue ref to lvalue
int&& rref3 = std::move(x);// OK: std::move casts lvalue to rvalue
```

### Lvalue References vs Rvalue References

```cpp
void func(int& lref) {
    std::cout << "Lvalue reference\n";
}

void func(int&& rref) {
    std::cout << "Rvalue reference\n";
}

int main() {
    int x = 42;
    func(x);           // Calls lvalue version
    func(42);          // Calls rvalue version
    func(x + 1);       // Calls rvalue version
    func(std::move(x));// Calls rvalue version
}
```

### Overload Resolution

The compiler prefers the rvalue reference overload when:
1. The argument is a temporary (prvalue)
2. The argument is explicitly cast to rvalue with std::move
3. The argument is an xvalue (expiring value)

## Move Constructor

The **move constructor** transfers resources from a temporary object to a new object.

### Syntax

```cpp
class MyClass {
public:
    // Move constructor
    MyClass(MyClass&& other) noexcept {
        // Transfer ownership of resources
    }
};
```

### Example: String Class

```cpp
class MyString {
private:
    char* data;
    size_t size;

public:
    // Copy constructor (expensive)
    MyString(const MyString& other) : size(other.size) {
        data = new char[size + 1];
        std::strcpy(data, other.data);
        std::cout << "Copy constructor called\n";
    }

    // Move constructor (cheap)
    MyString(MyString&& other) noexcept
        : data(other.data), size(other.size) {
        // Steal the data
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor called\n";
    }

    ~MyString() {
        delete[] data;
    }
};
```

### Key Points

1. **noexcept**: Move constructors should be marked `noexcept` because:
   - Enables optimizations (especially in STL containers)
   - Indicates the operation won't throw exceptions
   - STL containers will use move operations only if they're noexcept

2. **Leave source in valid state**: After moving, the source object should be left in a valid but unspecified state:
   - Safe to destroy
   - Safe to assign to
   - Don't assume any particular value

3. **No allocation**: Move constructor should not allocate memory (that would defeat the purpose!)

## Move Assignment Operator

The **move assignment operator** transfers resources from one existing object to another.

### Syntax

```cpp
class MyClass {
public:
    // Move assignment operator
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            // Release current resources
            // Transfer ownership from other
            // Leave other in valid state
        }
        return *this;
    }
};
```

### Example: Complete Implementation

```cpp
class MyString {
private:
    char* data;
    size_t size;

public:
    // Constructor
    MyString(const char* str = "") {
        size = std::strlen(str);
        data = new char[size + 1];
        std::strcpy(data, str);
    }

    // Copy constructor
    MyString(const MyString& other) : size(other.size) {
        data = new char[size + 1];
        std::strcpy(data, other.data);
    }

    // Move constructor
    MyString(MyString&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // Copy assignment
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    // Move assignment
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;  // Release current resources

            // Transfer ownership
            data = other.data;
            size = other.size;

            // Leave other in valid state
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Destructor
    ~MyString() {
        delete[] data;
    }
};
```

### Self-Assignment Check

Always check for self-assignment in move assignment:

```cpp
MyString& operator=(MyString&& other) noexcept {
    if (this != &other) {  // Critical!
        // Move logic
    }
    return *this;
}
```

Without this check, moving from an object to itself would be dangerous.

## std::move

`std::move` is a standard library function that casts an lvalue to an rvalue reference, enabling move semantics.

### What It Does

```cpp
template<typename T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}
```

**Important**: `std::move` doesn't actually move anything! It's just a cast. The actual moving happens in the move constructor or move assignment operator.

### Basic Usage

```cpp
std::string str1 = "Hello";
std::string str2 = std::move(str1);  // Move, not copy

// str1 is now in valid but unspecified state
// Don't use str1 unless you assign to it first
```

### When to Use std::move

1. **Returning local variables**: Usually not needed due to RVO/NRVO
```cpp
std::vector<int> func() {
    std::vector<int> result;
    // fill result
    return result;  // Don't use std::move here!
}
```

2. **Moving into containers**:
```cpp
std::vector<std::string> vec;
std::string str = "Hello";
vec.push_back(std::move(str));  // Move instead of copy
```

3. **Implementing move operations**:
```cpp
MyClass(MyClass&& other) noexcept
    : member1(std::move(other.member1)),
      member2(std::move(other.member2)) {
}
```

4. **Transferring ownership**:
```cpp
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::unique_ptr<int> p2 = std::move(p1);  // Transfer ownership
```

### Common Mistakes

```cpp
// BAD: Using moved-from object
std::string str1 = "Hello";
std::string str2 = std::move(str1);
std::cout << str1;  // WRONG! str1 is in unspecified state

// BAD: Unnecessary move on return
std::string func() {
    std::string result;
    return std::move(result);  // Don't do this!
}

// GOOD: Let compiler use RVO
std::string func() {
    std::string result;
    return result;  // Perfect!
}
```

## When to Use Move Semantics

### Scenarios Where Move Semantics Help

1. **Large Objects**: Moving is beneficial when copying is expensive
```cpp
std::vector<int> large(1000000);
std::vector<int> copy = std::move(large);  // Fast!
```

2. **Non-Copyable Resources**: Some resources can only be moved
```cpp
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::unique_ptr<int> p2 = std::move(p1);  // Can't copy unique_ptr
```

3. **Temporary Objects**: Compiler automatically uses move for temporaries
```cpp
std::vector<int> v = createVector();  // Automatic move
```

4. **Transferring Ownership**: When you want to explicitly transfer ownership
```cpp
void process(std::unique_ptr<Widget> widget) {
    // Takes ownership
}

auto w = std::make_unique<Widget>();
process(std::move(w));  // Transfer ownership
```

### When NOT to Use Move Semantics

1. **Small Objects**: Move isn't faster than copy for small types
```cpp
int x = 42;
int y = std::move(x);  // No benefit, just copies
```

2. **Return Values**: Compiler already optimizes with RVO
```cpp
// DON'T
std::vector<int> func() {
    std::vector<int> result;
    return std::move(result);  // Prevents RVO!
}

// DO
std::vector<int> func() {
    std::vector<int> result;
    return result;  // Let compiler optimize
}
```

3. **Const Objects**: Can't move from const objects
```cpp
const std::string str = "Hello";
std::string str2 = std::move(str);  // Copies, not moves!
```

## The Rule of Five

With move semantics, the Rule of Three becomes the **Rule of Five**:

If you define any of:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

You should consider defining all five (or explicitly delete them).

### Example

```cpp
class Resource {
private:
    int* data;

public:
    // Constructor
    Resource(int value) : data(new int(value)) {}

    // 1. Destructor
    ~Resource() {
        delete data;
    }

    // 2. Copy constructor
    Resource(const Resource& other)
        : data(new int(*other.data)) {}

    // 3. Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }

    // 4. Move constructor
    Resource(Resource&& other) noexcept
        : data(other.data) {
        other.data = nullptr;
    }

    // 5. Move assignment
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

### Rule of Zero

Better yet, follow the **Rule of Zero**: Don't define any of these special members if possible. Use RAII and standard library containers:

```cpp
class Resource {
private:
    std::unique_ptr<int> data;  // Handles everything automatically!

public:
    Resource(int value) : data(std::make_unique<int>(value)) {}
    // Compiler generates optimal copy/move operations
};
```

## Perfect Forwarding Introduction

**Perfect forwarding** allows you to write functions that forward arguments to other functions while preserving their value category (lvalue or rvalue).

### The Problem

```cpp
template<typename T>
void wrapper(T arg) {
    func(arg);  // Always passes as lvalue!
}
```

Even if you call `wrapper` with an rvalue, `arg` inside the function is an lvalue (it has a name).

### The Solution: Forwarding References

```cpp
template<typename T>
void wrapper(T&& arg) {  // Forwarding reference (universal reference)
    func(std::forward<T>(arg));  // Perfect forwarding
}
```

### Forwarding References vs Rvalue References

```cpp
// Rvalue reference (T is known)
void func(std::string&& str);

// Forwarding reference (T is deduced)
template<typename T>
void func(T&& arg);
```

A `T&&` is a forwarding reference only when:
1. `T` is a template parameter being deduced
2. No cv-qualifiers are present

### std::forward

```cpp
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

// Preserves value category:
std::string str = "Hello";
wrapper(str);              // Forwards as lvalue
wrapper(std::string("Hi"));// Forwards as rvalue
```

### Detailed Example

```cpp
class Widget {
public:
    Widget() { std::cout << "Default constructor\n"; }
    Widget(const Widget&) { std::cout << "Copy constructor\n"; }
    Widget(Widget&&) noexcept { std::cout << "Move constructor\n"; }
};

void process(Widget& w) {
    std::cout << "Lvalue overload\n";
}

void process(Widget&& w) {
    std::cout << "Rvalue overload\n";
}

template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}

int main() {
    Widget w;
    wrapper(w);                    // Calls lvalue overload
    wrapper(Widget());             // Calls rvalue overload
    wrapper(std::move(w));         // Calls rvalue overload
}
```

## Move Semantics and STL

The C++ Standard Library extensively uses move semantics for performance.

### Containers

```cpp
std::vector<std::string> vec;

// Old way: copy
std::string str1 = "Hello";
vec.push_back(str1);  // Copies str1

// New way: move
std::string str2 = "World";
vec.push_back(std::move(str2));  // Moves str2

// Best way: construct in-place
vec.emplace_back("!");  // Constructs directly in container
```

### Moving Containers

```cpp
std::vector<int> v1(1000000);
std::vector<int> v2 = std::move(v1);  // Just transfers pointer!

// v1 is now empty (valid but unspecified state)
// v2 owns all the elements
```

### Return Values

```cpp
std::vector<int> createVector() {
    std::vector<int> result(1000);
    return result;  // Move or RVO, never copy
}

std::vector<int> v = createVector();  // Fast!
```

### Algorithms

Many STL algorithms benefit from move semantics:

```cpp
std::vector<std::string> vec1 = {"apple", "banana", "cherry"};
std::vector<std::string> vec2;

// Move elements instead of copying
std::move(vec1.begin(), vec1.end(), std::back_inserter(vec2));
// vec1 now contains moved-from strings
```

## Performance Considerations

### When Move is Fast

1. **Pointer-based classes**: Just copy pointers
```cpp
std::vector, std::string, std::unique_ptr, etc.
```

2. **Large objects**: Avoid deep copies
```cpp
class BigData {
    std::vector<double> data;  // Million elements
};
```

### When Move is Not Faster

1. **Small objects**: Already fit in CPU cache
```cpp
struct Point { int x, y; };  // Move same as copy
```

2. **Stateful allocators**: May need to copy anyway
3. **SSO (Small String Optimization)**: Short strings don't allocate

### Measuring Performance

```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
// Operation to measure
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
```

### Example: Copy vs Move

```cpp
class BigData {
    std::vector<int> data;
public:
    BigData() : data(1000000) {}
};

// Copy: ~10 milliseconds
BigData data1;
BigData data2 = data1;

// Move: ~1 microsecond (10,000x faster!)
BigData data3;
BigData data4 = std::move(data3);
```

## Common Pitfalls

### 1. Using Moved-From Objects

```cpp
std::string str = "Hello";
std::string str2 = std::move(str);
std::cout << str;  // WRONG! Undefined behavior
```

**Fix**: Don't use moved-from objects (except to assign or destroy).

### 2. Moving Const Objects

```cpp
const std::string str = "Hello";
std::string str2 = std::move(str);  // Copies! Can't move const
```

**Fix**: Don't try to move const objects.

### 3. Forgetting noexcept

```cpp
class MyClass {
public:
    MyClass(MyClass&&) { }  // Missing noexcept!
};

std::vector<MyClass> vec;
vec.push_back(MyClass());  // May copy instead of move!
```

**Fix**: Always mark move operations as `noexcept`.

### 4. Moving Return Values

```cpp
std::string func() {
    std::string result;
    return std::move(result);  // Prevents RVO!
}
```

**Fix**: Let the compiler optimize. Just return by value.

### 5. Not Implementing Both Move Operations

```cpp
class MyClass {
public:
    MyClass(MyClass&&) noexcept { }  // Move constructor only
    // Missing move assignment!
};
```

**Fix**: Implement both move constructor and move assignment (or neither).

## Best Practices

### 1. Mark Move Operations noexcept

```cpp
class MyClass {
public:
    MyClass(MyClass&&) noexcept;
    MyClass& operator=(MyClass&&) noexcept;
};
```

### 2. Follow the Rule of Zero When Possible

```cpp
// Good: No special members needed
class Resource {
    std::unique_ptr<int> data;
    std::vector<std::string> names;
};
```

### 3. Don't std::move Return Values

```cpp
// Good
std::vector<int> createVector() {
    std::vector<int> result;
    return result;
}

// Bad
std::vector<int> createVector() {
    std::vector<int> result;
    return std::move(result);  // Don't!
}
```

### 4. Use std::move for Member Initialization

```cpp
class MyClass {
    std::string name;
public:
    MyClass(std::string n) : name(std::move(n)) { }
};
```

### 5. Pass Sink Arguments by Value

```cpp
// Good: Single implementation, supports both copy and move
class MyClass {
    std::string name;
public:
    void setName(std::string n) {  // By value
        name = std::move(n);
    }
};

// Usage:
std::string str = "Hello";
obj.setName(str);              // Copies str, moves into name
obj.setName("World");          // No copy of temporary
```

### 6. Document Moved-From State

```cpp
class MyClass {
    // After move, object is in valid but unspecified state
    // Can be safely destroyed or assigned to
    // Don't call other member functions
};
```

### 7. Test Your Move Operations

```cpp
void testMoveSemantics() {
    MyClass obj1;
    MyClass obj2 = std::move(obj1);

    // Verify obj1 can be destroyed
    // Verify obj1 can be assigned to
    obj1 = MyClass();
}
```

## Summary

Move semantics is a powerful feature that enables:

1. **Performance**: Eliminate unnecessary copies of large objects
2. **Unique ownership**: Model resources that can't be copied (unique_ptr)
3. **Flexibility**: Choose between copying and moving based on context

**Key concepts:**
- **Lvalues**: Named objects that persist
- **Rvalues**: Temporary objects about to be destroyed
- **Rvalue references (`&&`)**: Enable binding to temporaries
- **Move constructor**: Transfers resources from temporary
- **Move assignment**: Transfers resources between existing objects
- **std::move**: Casts lvalue to rvalue (enables move)
- **Perfect forwarding**: Preserves value category in templates
- **Rule of Five**: Manage all special members together
- **noexcept**: Critical for optimal performance in STL

**Remember:**
- Move operations should be `noexcept`
- Don't use moved-from objects
- Let compiler optimize returns (don't use std::move)
- Follow Rule of Zero when possible
- Move doesn't always mean faster (measure!)

Move semantics is fundamental to modern C++ and is used extensively in the standard library. Mastering it will make you a more effective C++ programmer and enable you to write more efficient code.

## Exercises

1. Implement a custom vector class with move semantics
2. Measure copy vs move performance for different data sizes
3. Implement a move-only class (like unique_ptr)
4. Write a function that uses perfect forwarding
5. Analyze when the compiler uses move automatically
6. Create a class following the Rule of Five
7. Implement move semantics for a binary tree
8. Benchmark std::move vs copy in a real application
9. Debug a program with moved-from object usage
10. Optimize legacy code by adding move semantics

## Further Reading

- "Effective Modern C++" by Scott Meyers (Items 23-30)
- "C++ Move Semantics - The Complete Guide" by Nicolai Josuttis
- C++11/14/17 Standard specifications
- CppCon talks on move semantics
- "A Brief Introduction to Rvalue References" by Howard Hinnant

---

**Next Lesson**: [Lesson 22: Smart Pointers](../Lesson22-Smart-Pointers/Lesson22-Smart-Pointers.md)

**Previous Module**: [Module 02: Intermediate C++](../../Module02-Intermediate-CPP/)

**Course Home**: [C++ Tutorial - WinAPI 3D Rendering](../../README.md)
