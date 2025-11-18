# Lesson 15: Exception Handling in C++

**Estimated Time**: 4 hours | **Difficulty**: Intermediate

## Table of Contents
1. [Introduction](#intro) 2. [try-catch-throw](#trycatch) 3. [Exception Types](#types)
4. [Custom Exceptions](#custom) 5. [noexcept](#noexcept) 6. [RAII](#raii)
7. [Exception Safety](#safety) 8. [Best Practices](#practices)

## 1. Introduction {#intro}
Exception handling provides a way to handle runtime errors gracefully without cluttering code with error-checking logic.

**Benefits**: Separation of error handling, propagation up call stack, RAII support
**Cost**: Performance overhead, code size increase

## 2. try-catch-throw {#trycatch}
```cpp
try {
    throw std::runtime_error("Error occurred");
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
```

**Multiple catch blocks**:
```cpp
try {
    // code
} catch (const std::out_of_range& e) {
    // handle
} catch (const std::exception& e) {
    // handle general
} catch (...) {
    // catch all
}
```

## 3. Exception Types {#types}
Standard exceptions hierarchy:
- `std::exception` (base)
- `std::logic_error`: `invalid_argument`, `out_of_range`, `length_error`
- `std::runtime_error`: `overflow_error`, `underflow_error`, `range_error`

## 4. Custom Exceptions {#custom}
```cpp
class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "My exception message";
    }
};
```

## 5. noexcept {#noexcept}
```cpp
void safe_function() noexcept {
    // Guaranteed not to throw
}
```

## 6. RAII {#raii}
Resource Acquisition Is Initialization ensures cleanup on exceptions:
```cpp
class Resource {
public:
    Resource() { /* acquire */ }
    ~Resource() { /* release */ }
};
```

## 7. Exception Safety Guarantees {#safety}
- **No-throw**: Never throws, always succeeds
- **Strong**: Operation succeeds or has no effect
- **Basic**: Invariants preserved, no resource leaks
- **No guarantee**: May leave in inconsistent state

## 8. Best Practices {#practices}
1. Catch by const reference
2. Throw by value
3. Use standard exceptions when appropriate
4. Don't throw in destructors
5. Use RAII for resource management
6. Consider noexcept for performance-critical functions

**Next**: Lesson 16 - File I/O
