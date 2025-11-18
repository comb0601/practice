# Lesson 51: Templates Introduction

**Duration:** 2 hours

## Learning Objectives
- Understand what templates are and why they're useful
- Learn function template syntax and usage
- Master template type deduction
- Implement generic algorithms using templates
- Understand template instantiation and compilation

## Introduction

Templates are C++'s answer to generic programming. They allow you to write code that works with any data type, making your code more reusable and type-safe. Templates are compile-time constructs that generate type-specific code automatically.

## Function Templates Basics

### Simple Function Template

```cpp
#include <iostream>
#include <string>

// Basic function template
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    // Template instantiation with int
    std::cout << "Max of 10 and 20: " << maximum(10, 20) << std::endl;

    // Template instantiation with double
    std::cout << "Max of 5.5 and 3.2: " << maximum(5.5, 3.2) << std::endl;

    // Template instantiation with string
    std::string s1 = "apple";
    std::string s2 = "banana";
    std::cout << "Max of apple and banana: " << maximum(s1, s2) << std::endl;

    return 0;
}
```

**Output:**
```
Max of 10 and 20: 20
Max of 5.5 and 3.2: 5.5
Max of apple and banana: banana
```

### Multiple Template Parameters

```cpp
#include <iostream>
#include <string>

template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::cout << "First: " << first << ", Second: " << second << std::endl;
}

template <typename T1, typename T2, typename T3>
T1 multiply(T2 a, T3 b) {
    return static_cast<T1>(a * b);
}

int main() {
    printPair(42, "answer");
    printPair(3.14, 100);
    printPair(std::string("Hello"), 'X');

    // Explicit template arguments
    int result = multiply<int, double, double>(3.5, 2.8);
    std::cout << "Result: " << result << std::endl;

    // Mix of explicit and deduced
    double dresult = multiply<double>(5, 7.5);
    std::cout << "Double result: " << dresult << std::endl;

    return 0;
}
```

## Template Type Deduction

```cpp
#include <iostream>
#include <vector>

// Template with automatic type deduction
template <typename T>
void printValue(T value) {
    std::cout << "Value: " << value << std::endl;
}

// Template with reference parameter
template <typename T>
void printReference(const T& value) {
    std::cout << "Reference value: " << value << std::endl;
}

// Template with pointer parameter
template <typename T>
void printPointer(T* ptr) {
    if (ptr) {
        std::cout << "Pointer value: " << *ptr << std::endl;
    }
}

// Template with array parameter
template <typename T, size_t N>
void printArray(T (&arr)[N]) {
    std::cout << "Array size: " << N << std::endl;
    for (size_t i = 0; i < N; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int x = 42;
    printValue(x);              // T deduced as int
    printValue(3.14);           // T deduced as double

    std::string str = "Hello";
    printReference(str);         // T deduced as std::string

    printPointer(&x);            // T deduced as int

    int numbers[] = {1, 2, 3, 4, 5};
    printArray(numbers);         // T deduced as int, N deduced as 5

    return 0;
}
```

## Practical Example 1: Generic Swap Function

```cpp
#include <iostream>
#include <string>
#include <utility> // for std::move

template <typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// Specialized version for arrays
template <typename T, size_t N>
void swap(T (&a)[N], T (&b)[N]) {
    for (size_t i = 0; i < N; ++i) {
        swap(a[i], b[i]);
    }
}

int main() {
    // Swap integers
    int x = 10, y = 20;
    std::cout << "Before: x=" << x << ", y=" << y << std::endl;
    swap(x, y);
    std::cout << "After: x=" << x << ", y=" << y << std::endl;

    // Swap strings
    std::string s1 = "Hello", s2 = "World";
    std::cout << "\nBefore: s1=" << s1 << ", s2=" << s2 << std::endl;
    swap(s1, s2);
    std::cout << "After: s1=" << s1 << ", s2=" << s2 << std::endl;

    // Swap arrays
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    std::cout << "\nBefore swap arrays:" << std::endl;
    std::cout << "arr1: ";
    for (int val : arr1) std::cout << val << " ";
    std::cout << "\narr2: ";
    for (int val : arr2) std::cout << val << " ";

    swap(arr1, arr2);
    std::cout << "\n\nAfter swap arrays:" << std::endl;
    std::cout << "arr1: ";
    for (int val : arr1) std::cout << val << " ";
    std::cout << "\narr2: ";
    for (int val : arr2) std::cout << val << " ";
    std::cout << std::endl;

    return 0;
}
```

## Practical Example 2: Generic Container Operations

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

// Find element in any container
template <typename Container, typename Value>
bool contains(const Container& container, const Value& value) {
    return std::find(container.begin(), container.end(), value)
           != container.end();
}

// Print any container
template <typename Container>
void printContainer(const Container& container, const std::string& name) {
    std::cout << name << ": ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// Sum elements in any numeric container
template <typename Container>
auto sumContainer(const Container& container) ->
    decltype(*container.begin()) {

    decltype(*container.begin()) sum = 0;
    for (const auto& item : container) {
        sum += item;
    }
    return sum;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<double> lst = {1.1, 2.2, 3.3, 4.4};

    printContainer(vec, "Vector");
    printContainer(lst, "List");

    std::cout << "Vector contains 3: "
              << (contains(vec, 3) ? "Yes" : "No") << std::endl;
    std::cout << "Vector contains 10: "
              << (contains(vec, 10) ? "Yes" : "No") << std::endl;

    std::cout << "Vector sum: " << sumContainer(vec) << std::endl;
    std::cout << "List sum: " << sumContainer(lst) << std::endl;

    return 0;
}
```

## Practical Example 3: Generic Pair Class

```cpp
#include <iostream>
#include <string>

template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    Pair(T1 f, T2 s) : first(f), second(s) {}

    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }

    void setFirst(T1 f) { first = f; }
    void setSecond(T2 s) { second = s; }

    void print() const {
        std::cout << "(" << first << ", " << second << ")" << std::endl;
    }
};

// Helper function to create pairs
template <typename T1, typename T2>
Pair<T1, T2> makePair(T1 first, T2 second) {
    return Pair<T1, T2>(first, second);
}

int main() {
    Pair<int, std::string> p1(42, "answer");
    p1.print();

    Pair<double, double> p2(3.14, 2.71);
    p2.print();

    auto p3 = makePair(std::string("x"), 100);
    p3.print();

    std::cout << "First: " << p3.getFirst()
              << ", Second: " << p3.getSecond() << std::endl;

    return 0;
}
```

## Practical Example 4: Generic Array Class

```cpp
#include <iostream>
#include <stdexcept>

template <typename T, size_t Size>
class Array {
private:
    T data[Size];

public:
    Array() : data{} {}

    Array(std::initializer_list<T> init) {
        size_t i = 0;
        for (const auto& item : init) {
            if (i >= Size) break;
            data[i++] = item;
        }
    }

    T& operator[](size_t index) {
        if (index >= Size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= Size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t size() const { return Size; }

    T* begin() { return data; }
    T* end() { return data + Size; }

    const T* begin() const { return data; }
    const T* end() const { return data + Size; }

    void fill(const T& value) {
        for (size_t i = 0; i < Size; ++i) {
            data[i] = value;
        }
    }
};

int main() {
    Array<int, 5> arr1 = {1, 2, 3, 4, 5};

    std::cout << "Array elements: ";
    for (size_t i = 0; i < arr1.size(); ++i) {
        std::cout << arr1[i] << " ";
    }
    std::cout << std::endl;

    // Range-based for loop
    Array<double, 3> arr2;
    arr2[0] = 1.1;
    arr2[1] = 2.2;
    arr2[2] = 3.3;

    std::cout << "Double array: ";
    for (const auto& val : arr2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Fill array
    Array<std::string, 4> arr3;
    arr3.fill("Hello");

    std::cout << "String array: ";
    for (const auto& val : arr3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## Template Compilation Model

Templates are compiled in two phases:

1. **Definition phase**: The template syntax is checked
2. **Instantiation phase**: The compiler generates code for specific types

```cpp
#include <iostream>

// Template definition
template <typename T>
class Calculator {
public:
    T add(T a, T b) { return a + b; }
    T multiply(T a, T b) { return a * b; }
};

int main() {
    // Instantiation for int
    Calculator<int> intCalc;
    std::cout << "5 + 3 = " << intCalc.add(5, 3) << std::endl;

    // Instantiation for double
    Calculator<double> doubleCalc;
    std::cout << "5.5 * 2.0 = " << doubleCalc.multiply(5.5, 2.0) << std::endl;

    // Each instantiation creates a separate class
    return 0;
}
```

## Exercises

### Exercise 1: Generic Min Function
**Problem:** Create a template function `minimum` that returns the smaller of two values.

**Solution:**
```cpp
#include <iostream>
#include <string>

template <typename T>
T minimum(T a, T b) {
    return (a < b) ? a : b;
}

int main() {
    std::cout << "Min of 10 and 5: " << minimum(10, 5) << std::endl;
    std::cout << "Min of 3.14 and 2.71: " << minimum(3.14, 2.71) << std::endl;

    std::string s1 = "zebra";
    std::string s2 = "apple";
    std::cout << "Min of zebra and apple: " << minimum(s1, s2) << std::endl;

    return 0;
}
```

### Exercise 2: Generic Array Sum
**Problem:** Create a template function that calculates the sum of elements in an array of any numeric type.

**Solution:**
```cpp
#include <iostream>

template <typename T, size_t N>
T arraySum(const T (&arr)[N]) {
    T sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    int intArr[] = {1, 2, 3, 4, 5};
    std::cout << "Int array sum: " << arraySum(intArr) << std::endl;

    double doubleArr[] = {1.1, 2.2, 3.3};
    std::cout << "Double array sum: " << arraySum(doubleArr) << std::endl;

    return 0;
}
```

### Exercise 3: Generic Triple Class
**Problem:** Create a template class `Triple` that holds three values of potentially different types.

**Solution:**
```cpp
#include <iostream>
#include <string>

template <typename T1, typename T2, typename T3>
class Triple {
private:
    T1 first;
    T2 second;
    T3 third;

public:
    Triple(T1 f, T2 s, T3 t) : first(f), second(s), third(t) {}

    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
    T3 getThird() const { return third; }

    void print() const {
        std::cout << "(" << first << ", " << second << ", " << third << ")" << std::endl;
    }
};

int main() {
    Triple<int, double, std::string> t1(42, 3.14, "pi");
    t1.print();

    Triple<std::string, int, char> t2("answer", 42, '!');
    t2.print();

    std::cout << "First: " << t1.getFirst() << std::endl;
    std::cout << "Second: " << t1.getSecond() << std::endl;
    std::cout << "Third: " << t1.getThird() << std::endl;

    return 0;
}
```

### Exercise 4: Generic Print Function
**Problem:** Create a template function that can print any container that supports begin() and end().

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <list>
#include <set>

template <typename Container>
void print(const Container& container) {
    std::cout << "[ ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<double> lst = {1.1, 2.2, 3.3};
    std::set<std::string> s = {"apple", "banana", "cherry"};

    std::cout << "Vector: ";
    print(vec);

    std::cout << "List: ";
    print(lst);

    std::cout << "Set: ";
    print(s);

    return 0;
}
```

### Exercise 5: Generic Stack Class
**Problem:** Create a template-based stack class with push, pop, top, and isEmpty operations.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    void push(const T& item) {
        elements.push_back(item);
    }

    void pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        elements.pop_back();
    }

    T& top() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return elements.back();
    }

    const T& top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return elements.back();
    }

    bool isEmpty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }
};

int main() {
    // Stack of integers
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);

    std::cout << "Int stack top: " << intStack.top() << std::endl;
    std::cout << "Int stack size: " << intStack.size() << std::endl;

    intStack.pop();
    std::cout << "After pop, top: " << intStack.top() << std::endl;

    // Stack of strings
    Stack<std::string> strStack;
    strStack.push("Hello");
    strStack.push("World");

    std::cout << "\nString stack top: " << strStack.top() << std::endl;

    while (!strStack.isEmpty()) {
        std::cout << strStack.top() << " ";
        strStack.pop();
    }
    std::cout << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Templates enable generic programming in C++
- Function templates work with any type
- Template type deduction happens automatically
- Templates are instantiated at compile-time
- Multiple template parameters allow flexibility
- Templates make code more reusable and type-safe

## Checklist

- [ ] Understand template syntax and purpose
- [ ] Can write function templates
- [ ] Know how template type deduction works
- [ ] Can use multiple template parameters
- [ ] Understand template instantiation
- [ ] Can create template classes
- [ ] Completed all 5 exercises
- [ ] Can use templates for generic containers
- [ ] Understand compile-time nature of templates
- [ ] Ready to learn template specialization

## Next Lesson

In Lesson 52, we'll explore **Template Specialization** and learn how to customize template behavior for specific types.
