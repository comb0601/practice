# Lesson 23: Lambda Expressions

## Introduction
Lambda expressions (introduced in C++11) provide a concise way to create anonymous function objects. They are essential for modern C++ programming, especially with STL algorithms, callbacks, and asynchronous operations.

## Basic Syntax
```cpp
[capture](parameters) -> return_type { body }
```

## Capture Modes
- **[=]**: Capture all by value
- **[&]**: Capture all by reference
- **[x]**: Capture x by value
- **[&x]**: Capture x by reference
- **[this]**: Capture this pointer
- **[=, &x]**: Capture all by value except x by reference

## Mutable Lambdas
```cpp
auto lambda = [x]() mutable { x++; };  // Can modify captured value
```

## Generic Lambdas (C++14)
```cpp
auto lambda = [](auto x, auto y) { return x + y; };
```

## Common Use Cases
1. STL algorithms (sort, find_if, transform)
2. Callbacks and event handlers
3. Thread functions
4. Custom comparators
5. Immediate invocation

## Best Practices
- Prefer capture by reference for large objects
- Use mutable when modifying captured values
- Explicit return type for complex lambdas
- Be careful with capture lifetime
- Use generic lambdas for templates

## See code examples for detailed implementations.
