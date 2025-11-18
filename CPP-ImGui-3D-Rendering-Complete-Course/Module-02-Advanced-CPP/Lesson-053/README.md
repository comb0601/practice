# Lesson 53: Variadic Templates

**Duration:** 2 hours

## Learning Objectives
- Understand variadic templates and parameter packs
- Learn recursive template expansion
- Master fold expressions (C++17)
- Implement variadic function templates
- Create variadic class templates

## Introduction

Variadic templates allow you to write templates that accept any number of arguments. This is essential for creating flexible, generic interfaces like `std::tuple`, `std::make_shared`, and other powerful C++ library features.

## Basic Variadic Function Template

### Simple Print Function

```cpp
#include <iostream>

// Base case: no arguments
void print() {
    std::cout << std::endl;
}

// Recursive case: at least one argument
template <typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << " ";
    print(rest...);  // Recursive call
}

int main() {
    print(1, 2, 3, 4, 5);
    print("Hello", "World", 42, 3.14);
    print("Single argument");
    print();  // Empty call

    return 0;
}
```

**Output:**
```
1 2 3 4 5
Hello World 42 3.14
Single argument
```

### Counting Arguments

```cpp
#include <iostream>

template <typename... Args>
void countArgs(Args... args) {
    std::cout << "Number of arguments: " << sizeof...(Args) << std::endl;
    std::cout << "Number of arguments (runtime): " << sizeof...(args) << std::endl;
}

int main() {
    countArgs();
    countArgs(1);
    countArgs(1, 2, 3);
    countArgs("a", "b", "c", "d", "e");

    return 0;
}
```

## Fold Expressions (C++17)

### Basic Fold Expressions

```cpp
#include <iostream>

// Sum using fold expression
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Unary right fold
}

// Product using fold expression
template <typename... Args>
auto product(Args... args) {
    return (args * ...);
}

// Logical AND
template <typename... Args>
bool allTrue(Args... args) {
    return (args && ...);
}

// Logical OR
template <typename... Args>
bool anyTrue(Args... args) {
    return (args || ...);
}

int main() {
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "Product: " << product(2, 3, 4) << std::endl;

    std::cout << "All true: " << allTrue(true, true, true) << std::endl;
    std::cout << "All true: " << allTrue(true, false, true) << std::endl;

    std::cout << "Any true: " << anyTrue(false, false, true) << std::endl;
    std::cout << "Any true: " << anyTrue(false, false, false) << std::endl;

    return 0;
}
```

### Print with Fold Expression

```cpp
#include <iostream>

template <typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

// With custom separator
template <typename... Args>
void printWithComma(Args... args) {
    bool first = true;
    ((std::cout << (first ? "" : ", ") << args, first = false), ...);
    std::cout << std::endl;
}

int main() {
    print(1, 2, 3, 4, 5);
    print("Hello", "World", 42, 3.14);

    printWithComma(1, 2, 3, 4, 5);
    printWithComma("Apple", "Banana", "Cherry");

    return 0;
}
```

## Variadic Class Templates

### Simple Tuple Implementation

```cpp
#include <iostream>

// Base case: empty tuple
template <typename... Types>
class Tuple {};

// Recursive case
template <typename T, typename... Rest>
class Tuple<T, Rest...> : private Tuple<Rest...> {
private:
    T value;

public:
    Tuple(T v, Rest... rest) : Tuple<Rest...>(rest...), value(v) {}

    T getFirst() const { return value; }

    Tuple<Rest...>& getRest() {
        return *this;
    }

    const Tuple<Rest...>& getRest() const {
        return *this;
    }
};

// Helper function
template <typename... Types>
Tuple<Types...> makeTuple(Types... args) {
    return Tuple<Types...>(args...);
}

int main() {
    auto t = makeTuple(42, 3.14, "Hello");

    std::cout << "First: " << t.getFirst() << std::endl;
    std::cout << "Second: " << t.getRest().getFirst() << std::endl;
    std::cout << "Third: " << t.getRest().getRest().getFirst() << std::endl;

    return 0;
}
```

### Type-safe Printer

```cpp
#include <iostream>
#include <string>

class Printer {
private:
    std::string separator;

public:
    Printer(const std::string& sep = " ") : separator(sep) {}

    // Base case
    void print() const {
        std::cout << std::endl;
    }

    // Recursive case
    template <typename T, typename... Args>
    void print(const T& first, const Args&... rest) const {
        std::cout << first;
        if (sizeof...(rest) > 0) {
            std::cout << separator;
        }
        print(rest...);
    }
};

int main() {
    Printer p1(" ");
    p1.print(1, 2, 3, 4, 5);

    Printer p2(", ");
    p2.print("Apple", "Banana", "Cherry");

    Printer p3(" -> ");
    p3.print("Start", "Middle", "End");

    return 0;
}
```

## Practical Example 1: Min/Max Functions

```cpp
#include <iostream>

// Minimum of two values
template <typename T>
T minimum(T a, T b) {
    return a < b ? a : b;
}

// Minimum of multiple values
template <typename T, typename... Args>
T minimum(T first, Args... rest) {
    T restMin = minimum(rest...);
    return first < restMin ? first : restMin;
}

// Maximum of two values
template <typename T>
T maximum(T a, T b) {
    return a > b ? a : b;
}

// Maximum of multiple values
template <typename T, typename... Args>
T maximum(T first, Args... rest) {
    T restMax = maximum(rest...);
    return first > restMax ? first : restMax;
}

int main() {
    std::cout << "Min of 5, 3, 8, 1, 9: " << minimum(5, 3, 8, 1, 9) << std::endl;
    std::cout << "Max of 5, 3, 8, 1, 9: " << maximum(5, 3, 8, 1, 9) << std::endl;

    std::cout << "Min of 3.14, 2.71, 1.41: "
              << minimum(3.14, 2.71, 1.41) << std::endl;

    return 0;
}
```

## Practical Example 2: Function Composer

```cpp
#include <iostream>
#include <functional>

// Apply functions in sequence
template <typename T>
T compose(T value) {
    return value;  // Base case
}

template <typename T, typename Func, typename... Funcs>
auto compose(T value, Func func, Funcs... funcs) {
    return compose(func(value), funcs...);
}

int main() {
    auto addOne = [](int x) { return x + 1; };
    auto multiplyByTwo = [](int x) { return x * 2; };
    auto square = [](int x) { return x * x; };

    int result = compose(5, addOne, multiplyByTwo, square);
    std::cout << "Result: " << result << std::endl;
    // 5 -> 6 -> 12 -> 144

    auto addTen = [](int x) { return x + 10; };
    int result2 = compose(3, addTen, square);
    std::cout << "Result2: " << result2 << std::endl;
    // 3 -> 13 -> 169

    return 0;
}
```

## Practical Example 3: Variadic Average

```cpp
#include <iostream>

template <typename... Args>
auto average(Args... args) {
    return (args + ...) / static_cast<double>(sizeof...(args));
}

// Type-safe version
template <typename T, typename... Args>
T averageTyped(T first, Args... rest) {
    static_assert((std::is_same_v<T, Args> && ...),
                  "All arguments must be the same type");
    return (first + ... + rest) / static_cast<T>(1 + sizeof...(rest));
}

int main() {
    std::cout << "Average of 1, 2, 3, 4, 5: "
              << average(1, 2, 3, 4, 5) << std::endl;

    std::cout << "Average of 2.5, 3.5, 4.5: "
              << average(2.5, 3.5, 4.5) << std::endl;

    std::cout << "Typed average: "
              << averageTyped(10, 20, 30, 40) << std::endl;

    return 0;
}
```

## Practical Example 4: All Of / Any Of

```cpp
#include <iostream>
#include <functional>

// Check if all values satisfy a predicate
template <typename Pred, typename... Args>
bool allOf(Pred pred, Args... args) {
    return (pred(args) && ...);
}

// Check if any value satisfies a predicate
template <typename Pred, typename... Args>
bool anyOf(Pred pred, Args... args) {
    return (pred(args) || ...);
}

// Check if none satisfy a predicate
template <typename Pred, typename... Args>
bool noneOf(Pred pred, Args... args) {
    return (!pred(args) && ...);
}

int main() {
    auto isEven = [](int x) { return x % 2 == 0; };
    auto isPositive = [](int x) { return x > 0; };

    std::cout << "All even (2, 4, 6): "
              << allOf(isEven, 2, 4, 6) << std::endl;

    std::cout << "All even (2, 3, 4): "
              << allOf(isEven, 2, 3, 4) << std::endl;

    std::cout << "Any even (1, 3, 4, 5): "
              << anyOf(isEven, 1, 3, 4, 5) << std::endl;

    std::cout << "None negative (1, 2, 3): "
              << noneOf([](int x) { return x < 0; }, 1, 2, 3) << std::endl;

    return 0;
}
```

## Parameter Pack Expansion

```cpp
#include <iostream>
#include <vector>

// Expand into vector
template <typename... Args>
std::vector<int> makeVector(Args... args) {
    return {args...};
}

// Expand with transformation
template <typename... Args>
std::vector<int> makeVectorDoubled(Args... args) {
    return {(args * 2)...};
}

// Multiple expansions
template <typename... Args>
void printPairs(Args... args) {
    ((std::cout << args << ":" << args * args << " "), ...);
    std::cout << std::endl;
}

int main() {
    auto vec = makeVector(1, 2, 3, 4, 5);
    std::cout << "Vector: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << std::endl;

    auto vec2 = makeVectorDoubled(1, 2, 3, 4, 5);
    std::cout << "Doubled: ";
    for (int v : vec2) std::cout << v << " ";
    std::cout << std::endl;

    printPairs(1, 2, 3, 4, 5);

    return 0;
}
```

## Exercises

### Exercise 1: Variadic Sum and Product
**Problem:** Create variadic templates for computing sum and product with proper type deduction.

**Solution:**
```cpp
#include <iostream>

template <typename T>
T sum(T value) {
    return value;
}

template <typename T, typename... Args>
auto sum(T first, Args... rest) {
    return first + sum(rest...);
}

template <typename T>
T product(T value) {
    return value;
}

template <typename T, typename... Args>
auto product(T first, Args... rest) {
    return first * product(rest...);
}

int main() {
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "Sum: " << sum(1.5, 2.5, 3.0) << std::endl;

    std::cout << "Product: " << product(2, 3, 4) << std::endl;
    std::cout << "Product: " << product(1.5, 2.0, 3.0) << std::endl;

    return 0;
}
```

### Exercise 2: Contains Function
**Problem:** Create a variadic template that checks if a value is in a list of values.

**Solution:**
```cpp
#include <iostream>

template <typename T, typename... Args>
bool contains(T value, Args... args) {
    return ((value == args) || ...);
}

int main() {
    std::cout << "Contains 3 in (1,2,3,4,5): "
              << contains(3, 1, 2, 3, 4, 5) << std::endl;

    std::cout << "Contains 10 in (1,2,3,4,5): "
              << contains(10, 1, 2, 3, 4, 5) << std::endl;

    std::cout << "Contains 'c' in (a,b,c,d): "
              << contains('c', 'a', 'b', 'c', 'd') << std::endl;

    return 0;
}
```

### Exercise 3: Variadic Max
**Problem:** Create a variadic template function to find the maximum value.

**Solution:**
```cpp
#include <iostream>

template <typename T>
T max(T value) {
    return value;
}

template <typename T, typename... Args>
T max(T first, Args... rest) {
    T restMax = max(rest...);
    return first > restMax ? first : restMax;
}

int main() {
    std::cout << "Max of 3, 7, 2, 9, 1: " << max(3, 7, 2, 9, 1) << std::endl;
    std::cout << "Max of 4.5, 2.3, 8.7, 1.2: "
              << max(4.5, 2.3, 8.7, 1.2) << std::endl;

    return 0;
}
```

### Exercise 4: Type Counter
**Problem:** Create a variadic template that counts how many arguments are of a specific type.

**Solution:**
```cpp
#include <iostream>
#include <type_traits>

template <typename T, typename... Args>
constexpr size_t countType() {
    return ((std::is_same_v<T, Args> ? 1 : 0) + ...);
}

template <typename T>
struct TypeCounter {
    template <typename... Args>
    static constexpr size_t count() {
        return countType<T, Args...>();
    }
};

int main() {
    std::cout << "Number of ints: "
              << TypeCounter<int>::count<int, double, int, char, int>()
              << std::endl;

    std::cout << "Number of doubles: "
              << TypeCounter<double>::count<int, double, int, char, double>()
              << std::endl;

    return 0;
}
```

### Exercise 5: Variadic String Joiner
**Problem:** Create a variadic template that joins values into a string with a separator.

**Solution:**
```cpp
#include <iostream>
#include <sstream>
#include <string>

template <typename T>
void joinImpl(std::ostringstream& oss, const std::string& sep, T value) {
    oss << value;
}

template <typename T, typename... Args>
void joinImpl(std::ostringstream& oss, const std::string& sep,
              T first, Args... rest) {
    oss << first << sep;
    joinImpl(oss, sep, rest...);
}

template <typename... Args>
std::string join(const std::string& separator, Args... args) {
    if (sizeof...(args) == 0) {
        return "";
    }

    std::ostringstream oss;
    joinImpl(oss, separator, args...);
    return oss.str();
}

int main() {
    std::cout << join(", ", 1, 2, 3, 4, 5) << std::endl;
    std::cout << join(" - ", "Apple", "Banana", "Cherry") << std::endl;
    std::cout << join("::", "std", "vector", "iterator") << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Variadic templates accept any number of arguments
- Parameter packs and pack expansion
- Recursive template techniques
- Fold expressions (C++17)
- Creating variadic functions and classes
- Type-safe variadic operations

## Checklist

- [ ] Understand parameter packs
- [ ] Can write recursive variadic templates
- [ ] Know fold expressions
- [ ] Can create variadic functions
- [ ] Can create variadic classes
- [ ] Understand sizeof... operator
- [ ] Completed all 5 exercises
- [ ] Can use parameter pack expansion
- [ ] Understand base case pattern
- [ ] Ready to learn perfect forwarding

## Next Lesson

In Lesson 54, we'll explore **Perfect Forwarding and Move Semantics** to learn how to efficiently pass arguments through templates.
