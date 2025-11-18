#!/bin/bash

# Script to create all remaining lessons with comprehensive content

# Lesson 23: Lambda Expressions
cat > /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Lesson23-Lambda-Expressions.md << 'EOF'
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
EOF

# Create Lambda code examples
mkdir -p /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Code

cat > /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Code/01_basic_syntax.cpp << 'CPPEOF'
#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    // Basic lambda
    auto hello = []() { std::cout << "Hello Lambda!\n"; };
    hello();
    
    // Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << add(5, 3) << "\n";
    
    // Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0.0;
        return a / b;
    };
    std::cout << "10 / 3 = " << divide(10, 3) << "\n";
    
    // Immediate invocation
    int result = [](int x) { return x * x; }(5);
    std::cout << "5^2 = " << result << "\n";
    
    // With STL
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::for_each(vec.begin(), vec.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "\n";
    
    return 0;
}
CPPEOF

cat > /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Code/02_captures.cpp << 'CPPEOF'
#include <iostream>

int main() {
    int x = 10, y = 20;
    
    // Capture by value
    auto byValue = [x, y]() {
        std::cout << "Captured by value: " << x << ", " << y << "\n";
    };
    byValue();
    
    // Capture by reference
    auto byRef = [&x, &y]() {
        x += 10;
        y += 10;
        std::cout << "Modified via reference: " << x << ", " << y << "\n";
    };
    byRef();
    std::cout << "After lambda: x=" << x << ", y=" << y << "\n";
    
    // Capture all by value
    auto captureAll = [=]() {
        std::cout << "Capture all by value: " << x << ", " << y << "\n";
    };
    captureAll();
    
    // Mixed capture
    int z = 30;
    auto mixed = [=, &z]() {
        z += 10;
        std::cout << "Mixed capture: x=" << x << ", z=" << z << "\n";
    };
    mixed();
    
    return 0;
}
CPPEOF

cat > /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Code/03_mutable_lambdas.cpp << 'CPPEOF'
#include <iostream>

int main() {
    int x = 10;
    
    // Without mutable (ERROR if we try to modify x)
    // auto lambda1 = [x]() { x++; };  // Error!
    
    // With mutable
    auto lambda2 = [x]() mutable {
        x++;
        std::cout << "Inside lambda: " << x << "\n";
    };
    
    lambda2();
    lambda2();
    std::cout << "Outside lambda: " << x << "\n";  // Original unchanged
    
    // Mutable with reference
    auto lambda3 = [&x]() {  // No mutable needed with reference
        x++;
        std::cout << "With reference: " << x << "\n";
    };
    lambda3();
    std::cout << "After reference lambda: " << x << "\n";
    
    return 0;
}
CPPEOF

cat > /home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP/Lesson23-Lambda-Expressions/Code/README.md << 'MDEOF'
# Lesson 23: Lambda Expressions - Code Examples

## Files
1. **01_basic_syntax.cpp** - Lambda basics and syntax
2. **02_captures.cpp** - Capture modes (value, reference, all)
3. **03_mutable_lambdas.cpp** - Mutable lambdas

## Compilation
```bash
g++ -std=c++17 -Wall filename.cpp -o filename
```
MDEOF

# Continue with remaining lessons...
echo "Lesson 23 created"

