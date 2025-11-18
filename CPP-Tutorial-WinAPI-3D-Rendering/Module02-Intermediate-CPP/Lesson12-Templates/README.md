# Lesson 12: Templates in C++

**Estimated Time**: 4-5 hours
**Difficulty**: Intermediate
**Prerequisites**: Classes, Objects, Operator Overloading

---

## Table of Contents
1. [Introduction to Templates](#introduction)
2. [Why Templates?](#why-templates)
3. [Function Templates](#function-templates)
4. [Class Templates](#class-templates)
5. [Template Specialization](#template-specialization)
6. [Partial Specialization](#partial-specialization)
7. [Non-Type Template Parameters](#non-type-parameters)
8. [Variadic Templates](#variadic-templates)
9. [Template Template Parameters](#template-template-parameters)
10. [Type Traits and SFINAE](#type-traits)
11. [Common Template Patterns](#common-patterns)
12. [Template Best Practices](#best-practices)
13. [Template Metaprogramming](#metaprogramming)
14. [Common Pitfalls](#pitfalls)
15. [Exercises](#exercises)

---

## 1. Introduction to Templates {#introduction}

**Templates** are a powerful feature in C++ that enable **generic programming**. They allow you to write code that works with any data type without sacrificing type safety or performance. Templates are resolved at **compile time**, resulting in zero runtime overhead.

### What Are Templates?

Templates are blueprints for creating functions or classes that can work with different types. Instead of writing separate functions for each type:

```cpp
int max(int a, int b) { return (a > b) ? a : b; }
double max(double a, double b) { return (a > b) ? a : b; }
string max(string a, string b) { return (a > b) ? a : b; }
```

You can write a single template:

```cpp
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}
```

### Key Characteristics

1. **Compile-Time Resolution**: Templates are instantiated at compile time
2. **Type Safety**: Full type checking is performed
3. **No Runtime Overhead**: No virtual function calls or dynamic dispatch
4. **Code Bloat**: Each instantiation creates separate code
5. **Header-Only**: Template definitions must typically be in headers

---

## 2. Why Templates? {#why-templates}

### Benefits

1. **Code Reusability**: Write once, use with many types
2. **Type Safety**: Compile-time type checking
3. **Performance**: Zero runtime overhead
4. **Flexibility**: Works with any type that satisfies requirements
5. **STL Foundation**: The entire Standard Template Library uses templates

### Generic Programming

Templates enable **generic programming** - writing algorithms that work independently of specific data types:

```cpp
// Generic swap function
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Works with any type
int x = 5, y = 10;
swap(x, y);  // int version

string s1 = "hello", s2 = "world";
swap(s1, s2);  // string version
```

---

## 3. Function Templates {#function-templates}

### Basic Syntax

```cpp
template <typename T>
ReturnType functionName(T parameter) {
    // Function body
}
```

Or using `class` keyword (equivalent):

```cpp
template <class T>
ReturnType functionName(T parameter) {
    // Function body
}
```

### Simple Example

```cpp
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Usage
int i = maximum(10, 20);           // T = int
double d = maximum(3.14, 2.71);    // T = double
string s = maximum("abc", "xyz");  // T = string
```

### Multiple Type Parameters

```cpp
template <typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

// Usage
auto result1 = add(5, 3.14);      // int + double = double
auto result2 = add(2.5, 10);      // double + int = double
```

### Explicit Template Arguments

```cpp
template <typename T>
T convert(double value) {
    return static_cast<T>(value);
}

// Explicit type specification
int i = convert<int>(3.14);
float f = convert<float>(2.71828);
```

### Template Type Deduction

The compiler can automatically deduce template types:

```cpp
template <typename T>
void print(T value) {
    cout << value << endl;
}

print(42);        // T deduced as int
print(3.14);      // T deduced as double
print("hello");   // T deduced as const char*
```

### Function Template Overloading

```cpp
// Generic version
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Specialized version for pointers
template <typename T>
T* max(T* a, T* b) {
    return (*a > *b) ? a : b;
}

// Specialized version for C-strings
const char* max(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}
```

### Constraints on Template Parameters

Templates require that operations used in the template are valid for the type:

```cpp
template <typename T>
T sum(T* array, size_t size) {
    T total = T();  // Requires default constructor
    for (size_t i = 0; i < size; i++) {
        total += array[i];  // Requires += operator
    }
    return total;
}
```

---

## 4. Class Templates {#class-templates}

### Basic Syntax

```cpp
template <typename T>
class ClassName {
private:
    T member;
public:
    ClassName(T value) : member(value) {}
    T getValue() const { return member; }
    void setValue(T value) { member = value; }
};
```

### Usage

```cpp
ClassName<int> obj1(42);
ClassName<double> obj2(3.14);
ClassName<string> obj3("hello");
```

### Complete Example: Array Template

```cpp
template <typename T>
class Array {
private:
    T* data;
    size_t size;

public:
    // Constructor
    Array(size_t s) : size(s) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = T();  // Default initialize
        }
    }

    // Destructor
    ~Array() {
        delete[] data;
    }

    // Copy constructor
    Array(const Array& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Subscript operator
    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const { return size; }
};
```

### Member Function Definitions Outside Class

When defining member functions outside the class template:

```cpp
template <typename T>
class Stack {
private:
    vector<T> elements;

public:
    void push(const T& element);
    void pop();
    T& top();
    bool empty() const;
    size_t size() const;
};

// Member function definitions
template <typename T>
void Stack<T>::push(const T& element) {
    elements.push_back(element);
}

template <typename T>
void Stack<T>::pop() {
    if (elements.empty()) {
        throw out_of_range("Stack is empty");
    }
    elements.pop_back();
}

template <typename T>
T& Stack<T>::top() {
    if (elements.empty()) {
        throw out_of_range("Stack is empty");
    }
    return elements.back();
}

template <typename T>
bool Stack<T>::empty() const {
    return elements.empty();
}

template <typename T>
size_t Stack<T>::size() const {
    return elements.size();
}
```

### Multiple Template Parameters

```cpp
template <typename K, typename V>
class Pair {
private:
    K key;
    V value;

public:
    Pair(K k, V v) : key(k), value(v) {}

    K getKey() const { return key; }
    V getValue() const { return value; }
    void setValue(V v) { value = v; }
};

// Usage
Pair<string, int> age("John", 30);
Pair<int, double> coordinates(10, 3.14);
```

---

## 5. Template Specialization {#template-specialization}

### Full Specialization

Provide a completely different implementation for a specific type:

```cpp
// Generic template
template <typename T>
class Storage {
private:
    T data;
public:
    Storage(T d) : data(d) {}
    void print() {
        cout << "Generic: " << data << endl;
    }
};

// Full specialization for bool
template <>
class Storage<bool> {
private:
    bool data;
public:
    Storage(bool d) : data(d) {}
    void print() {
        cout << "Bool: " << (data ? "true" : "false") << endl;
    }
};

// Full specialization for const char*
template <>
class Storage<const char*> {
private:
    string data;
public:
    Storage(const char* d) : data(d) {}
    void print() {
        cout << "String: " << data << endl;
    }
};
```

### Function Template Specialization

```cpp
// Generic template
template <typename T>
void print(T value) {
    cout << value << endl;
}

// Specialization for bool
template <>
void print<bool>(bool value) {
    cout << (value ? "true" : "false") << endl;
}

// Specialization for vector
template <>
void print<vector<int>>(vector<int> value) {
    cout << "[";
    for (size_t i = 0; i < value.size(); i++) {
        cout << value[i];
        if (i < value.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}
```

### When to Use Specialization

1. **Optimize for specific types**: Different algorithm for specific types
2. **Handle special cases**: Types that need different behavior
3. **Fix limitations**: Work around type-specific issues

---

## 6. Partial Specialization {#partial-specialization}

**Partial specialization** is only available for class templates (not function templates).

### Pointer Specialization

```cpp
// Generic template
template <typename T>
class SmartPointer {
private:
    T* ptr;
public:
    SmartPointer(T* p) : ptr(p) {}
    ~SmartPointer() { delete ptr; }
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
};

// Partial specialization for arrays
template <typename T>
class SmartPointer<T[]> {
private:
    T* ptr;
public:
    SmartPointer(T* p) : ptr(p) {}
    ~SmartPointer() { delete[] ptr; }  // Use delete[]
    T& operator[](size_t index) { return ptr[index]; }
};
```

### Specialization with Multiple Parameters

```cpp
// Generic template
template <typename T1, typename T2>
class Pair {
    T1 first;
    T2 second;
public:
    Pair(T1 f, T2 s) : first(f), second(s) {}
};

// Partial specialization when both types are the same
template <typename T>
class Pair<T, T> {
    T first;
    T second;
public:
    Pair(T f, T s) : first(f), second(s) {}
    bool areSame() const { return first == second; }
};

// Partial specialization when second type is pointer
template <typename T1, typename T2>
class Pair<T1, T2*> {
    T1 first;
    T2* second;
public:
    Pair(T1 f, T2* s) : first(f), second(s) {}
    bool secondIsNull() const { return second == nullptr; }
};
```

---

## 7. Non-Type Template Parameters {#non-type-parameters}

Templates can also take **constant values** as parameters:

```cpp
template <typename T, size_t N>
class FixedArray {
private:
    T data[N];

public:
    T& operator[](size_t index) {
        if (index >= N) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= N) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    constexpr size_t size() const { return N; }
};

// Usage
FixedArray<int, 10> arr1;     // Array of 10 ints
FixedArray<double, 5> arr2;   // Array of 5 doubles
```

### Valid Non-Type Parameters

- Integral types (`int`, `char`, `bool`, etc.)
- Enumerations
- Pointers
- References
- `nullptr_t`
- C++20: Floating-point types and class types

### Example: Compile-Time Calculations

```cpp
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const int value = 1;
};

// Computed at compile time
constexpr int fact5 = Factorial<5>::value;  // 120
```

---

## 8. Variadic Templates {#variadic-templates}

**Variadic templates** accept a variable number of template arguments (C++11).

### Basic Syntax

```cpp
template <typename... Args>
void func(Args... args) {
    // args is a parameter pack
}
```

### Example: Variadic Print Function

```cpp
// Base case: no arguments
void print() {
    cout << endl;
}

// Recursive case
template <typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // Recursive call with remaining arguments
}

// Usage
print(1, 2.5, "hello", 'c');  // Output: 1 2.5 hello c
```

### sizeof... Operator

Get the number of arguments in a parameter pack:

```cpp
template <typename... Args>
void printCount(Args... args) {
    cout << "Number of arguments: " << sizeof...(args) << endl;
}

printCount(1, 2, 3);           // 3
printCount("a", "b", "c", "d"); // 4
```

### Fold Expressions (C++17)

Simplify operations on parameter packs:

```cpp
// Sum all arguments
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Unary right fold
}

cout << sum(1, 2, 3, 4, 5);  // 15

// Print all arguments
template <typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);  // Binary left fold
    cout << endl;
}

print(1, 2.5, "hello");  // 1 2.5 hello
```

### Variadic Class Template

```cpp
template <typename... Types>
class Tuple;

// Specialization for empty tuple
template <>
class Tuple<> {};

// Recursive specialization
template <typename Head, typename... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...> {
    Head head;
public:
    Tuple(Head h, Tail... t) : Tuple<Tail...>(t...), head(h) {}

    Head getHead() const { return head; }
    Tuple<Tail...>& getTail() {
        return *this;
    }
};
```

---

## 9. Template Template Parameters {#template-template-parameters}

Templates that take other templates as parameters:

```cpp
template <typename T, template <typename> class Container>
class Stack {
private:
    Container<T> elements;

public:
    void push(const T& element) {
        elements.push_back(element);
    }

    void pop() {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    T& top() {
        return elements.back();
    }

    bool empty() const {
        return elements.empty();
    }
};

// Usage with different containers
Stack<int, vector> stack1;
Stack<double, list> stack2;
Stack<string, deque> stack3;
```

---

## 10. Type Traits and SFINAE {#type-traits}

### Type Traits

Type traits provide compile-time information about types:

```cpp
#include <type_traits>

template <typename T>
void process(T value) {
    if (is_integral<T>::value) {
        cout << "Integer type" << endl;
    } else if (is_floating_point<T>::value) {
        cout << "Floating point type" << endl;
    } else {
        cout << "Other type" << endl;
    }
}
```

### Common Type Traits

```cpp
is_integral<T>::value          // Is T an integral type?
is_floating_point<T>::value    // Is T a floating-point type?
is_pointer<T>::value           // Is T a pointer?
is_array<T>::value             // Is T an array?
is_class<T>::value             // Is T a class?
is_same<T1, T2>::value         // Are T1 and T2 the same?
is_base_of<Base, Derived>::value  // Is Base a base of Derived?
```

### SFINAE (Substitution Failure Is Not An Error)

Enable/disable templates based on conditions:

```cpp
// Only enabled for integral types
template <typename T>
typename enable_if<is_integral<T>::value, T>::type
square(T value) {
    return value * value;
}

// Only enabled for floating-point types
template <typename T>
typename enable_if<is_floating_point<T>::value, T>::type
square(T value) {
    return value * value;
}
```

### C++17: if constexpr

```cpp
template <typename T>
void process(T value) {
    if constexpr (is_integral<T>::value) {
        cout << "Processing integer: " << value << endl;
    } else if constexpr (is_floating_point<T>::value) {
        cout << "Processing float: " << value << endl;
    } else {
        cout << "Processing other: " << value << endl;
    }
}
```

---

## 11. Common Template Patterns {#common-patterns}

### CRTP (Curiously Recurring Template Pattern)

```cpp
template <typename Derived>
class Counter {
private:
    static int count;

public:
    Counter() { ++count; }
    ~Counter() { --count; }

    static int getCount() { return count; }
};

template <typename Derived>
int Counter<Derived>::count = 0;

class MyClass : public Counter<MyClass> {
    // MyClass has its own counter
};

class AnotherClass : public Counter<AnotherClass> {
    // AnotherClass has its own counter
};
```

### Policy-Based Design

```cpp
template <typename T>
class DefaultDeleter {
public:
    void operator()(T* ptr) {
        delete ptr;
    }
};

template <typename T>
class ArrayDeleter {
public:
    void operator()(T* ptr) {
        delete[] ptr;
    }
};

template <typename T, typename Deleter = DefaultDeleter<T>>
class SmartPtr {
private:
    T* ptr;
    Deleter deleter;

public:
    SmartPtr(T* p) : ptr(p) {}
    ~SmartPtr() { deleter(ptr); }

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
};

// Usage
SmartPtr<int> p1(new int(42));           // Uses DefaultDeleter
SmartPtr<int, ArrayDeleter<int>> p2(new int[10]);  // Uses ArrayDeleter
```

---

## 12. Template Best Practices {#best-practices}

### 1. Put Template Definitions in Headers

Templates must be defined in headers because they need to be instantiated at compile time:

```cpp
// MyTemplate.h
template <typename T>
class MyTemplate {
    // Definition goes here
};
```

### 2. Use Meaningful Names

```cpp
// Good
template <typename ElementType>
class Container { /*...*/ };

// Less clear
template <typename T>
class Container { /*...*/ };
```

### 3. Document Type Requirements

```cpp
/**
 * Sorts an array in ascending order.
 *
 * Template Requirements:
 * - T must support operator<
 * - T must be copy-assignable
 * - T must be copy-constructible
 */
template <typename T>
void sort(T* array, size_t size) {
    // Implementation
}
```

### 4. Use Concepts (C++20)

```cpp
#include <concepts>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
T add(T a, T b) {
    return a + b;
}
```

### 5. Prefer Type Deduction

```cpp
// Let compiler deduce types
auto result = max(5, 10);  // Better

// Explicit specification when necessary
auto result = max<double>(5, 10);
```

---

## 13. Template Metaprogramming {#metaprogramming}

### Compile-Time Computation

```cpp
// Compile-time factorial
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

constexpr int fact = Factorial<5>::value;  // Computed at compile time
```

### Compile-Time Fibonacci

```cpp
template <int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

constexpr int fib10 = Fibonacci<10>::value;  // 55
```

### Type Selection

```cpp
template <bool Condition, typename TrueType, typename FalseType>
struct Conditional {
    using type = TrueType;
};

template <typename TrueType, typename FalseType>
struct Conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

// Usage
using MyType = Conditional<sizeof(int) == 4, int, long>::type;
```

---

## 14. Common Pitfalls {#pitfalls}

### 1. Template Instantiation Bloat

Each template instantiation creates separate code:

```cpp
vector<int> v1;     // Generates vector<int> code
vector<double> v2;  // Generates vector<double> code
vector<string> v3;  // Generates vector<string> code
// Three complete implementations in the binary
```

### 2. Error Messages

Template errors can be verbose and hard to read:

```cpp
// Using concepts (C++20) helps
template <typename T>
concept Printable = requires(T t) {
    { std::cout << t } -> std::convertible_to<std::ostream&>;
};

template <Printable T>
void print(T value) {
    cout << value << endl;
}
```

### 3. Two-Phase Lookup

```cpp
template <typename T>
class MyClass {
    void func() {
        helper();  // When is this resolved?
    }
};

// Must be declared before template if not dependent
void helper() { }
```

### 4. typename Keyword

Must use `typename` for dependent type names:

```cpp
template <typename T>
class MyClass {
    // Error: need typename
    // T::iterator it;

    // Correct
    typename T::iterator it;
};
```

### 5. Template Instantiation in CPP Files

Don't do this unless you explicitly instantiate:

```cpp
// MyTemplate.cpp
template <typename T>
void MyTemplate<T>::func() {
    // Implementation
}

// Must explicitly instantiate
template class MyTemplate<int>;
template class MyTemplate<double>;
```

---

## 15. Exercises {#exercises}

### Exercise 1: Min/Max Templates
Create generic `min` and `max` functions that work with any comparable type.

### Exercise 2: Swap Template
Implement a generic `swap` function that exchanges two values of any type.

### Exercise 3: Array Template
Create a template class `Array<T, N>` that represents a fixed-size array with bounds checking.

### Exercise 4: Stack Template
Implement a generic stack using templates with push, pop, top, and empty operations.

### Exercise 5: Pair Template
Create a `Pair<T1, T2>` template that stores two values of potentially different types.

### Exercise 6: Find Template
Implement a generic `find` function that searches for an element in an array.

### Exercise 7: Sort Template
Create a generic sorting function that works with any type supporting comparison.

### Exercise 8: Unique Pointer Template
Implement a simple `UniquePtr<T>` smart pointer template.

### Exercise 9: Variadic Sum
Create a variadic template function that sums any number of arguments.

### Exercise 10: Type-Safe Container
Build a generic container class with iterator support.

---

## Summary

Templates are a cornerstone of modern C++ programming:

**Key Takeaways**:
1. Templates enable generic programming and code reuse
2. Function templates work with multiple types automatically
3. Class templates create generic data structures
4. Template specialization allows type-specific optimizations
5. Variadic templates handle variable numbers of arguments
6. Type traits enable compile-time type introspection
7. Template metaprogramming performs computations at compile time
8. Put template definitions in header files
9. Use concepts (C++20) for clearer requirements
10. Be aware of code bloat from multiple instantiations

**Next Steps**:
- Study the STL to see templates in action
- Practice creating reusable template classes
- Learn about C++20 concepts and constraints
- Explore template metaprogramming techniques
- Understand when to use templates vs. runtime polymorphism

---

**Continue to Lesson 13: STL Containers** to learn about the Standard Template Library's container classes.
