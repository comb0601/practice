# Lesson 61: Exception Handling Basics

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Exceptions
2. try-catch Blocks
3. Throwing Exceptions
4. Standard Exception Types
5. Exception Propagation
6. Multiple catch Blocks
7. Catching All Exceptions
8. Exercises and Projects

---

## 1. Introduction to Exceptions

Exceptions provide a way to handle runtime errors and exceptional conditions.

### Why Use Exceptions?

```cpp
// Without exceptions
int divide(int a, int b) {
    if (b == 0) return -1;  // Error code
    return a / b;
}

// With exceptions
int divide(int a, int b) {
    if (b == 0) throw std::runtime_error("Division by zero");
    return a / b;
}
```

---

## 2. try-catch Blocks

### Basic Syntax:

```cpp
#include <iostream>
#include <stdexcept>

int main()
{
    try {
        int x = 10;
        int y = 0;
        if (y == 0) {
            throw std::runtime_error("Cannot divide by zero");
        }
        int result = x / y;
        std::cout << "Result: " << result << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    std::cout << "Program continues\n";
    return 0;
}
```

---

## 3. Throwing Exceptions

### Throw Statement:

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

double squareRoot(double x) {
    if (x < 0) {
        throw std::invalid_argument("Negative value");
    }
    return std::sqrt(x);
}

int main()
{
    try {
        double result = squareRoot(-4);
        std::cout << result << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 4. Standard Exception Types

### Exception Hierarchy:

```cpp
/*
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   └── std::out_of_range
├── std::runtime_error
│   ├── std::range_error
│   ├── std::overflow_error
│   └── std::underflow_error
└── std::bad_alloc
*/
```

### Using Standard Exceptions:

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

int main()
{
    std::vector<int> v = {1, 2, 3};

    try {
        // out_of_range exception
        int x = v.at(10);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << "\n";
    }

    try {
        // invalid_argument exception
        int num = std::stoi("abc");
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 5. Exception Propagation

### Call Stack Unwinding:

```cpp
#include <iostream>
#include <stdexcept>

void functionC() {
    throw std::runtime_error("Error in C");
}

void functionB() {
    functionC();
}

void functionA() {
    functionB();
}

int main()
{
    try {
        functionA();
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 6. Multiple catch Blocks

### Handling Different Types:

```cpp
#include <iostream>
#include <stdexcept>

void riskyFunction(int x) {
    if (x == 0) throw std::runtime_error("Runtime error");
    if (x == 1) throw std::invalid_argument("Invalid argument");
    if (x == 2) throw std::out_of_range("Out of range");
}

int main()
{
    for (int i = 0; i < 3; i++) {
        try {
            riskyFunction(i);
        }
        catch (const std::runtime_error& e) {
            std::cout << "Runtime: " << e.what() << "\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid: " << e.what() << "\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "Range: " << e.what() << "\n";
        }
    }

    return 0;
}
```

---

## 7. Catching All Exceptions

### Catch-All Handler:

```cpp
#include <iostream>
#include <stdexcept>

int main()
{
    try {
        throw std::runtime_error("Some error");
    }
    catch (const std::exception& e) {
        // Catches all standard exceptions
        std::cout << "Exception: " << e.what() << "\n";
    }
    catch (...) {
        // Catches everything else
        std::cout << "Unknown exception\n";
    }

    return 0;
}
```

---

## 8. Exercises and Projects

### Project: Safe Calculator

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class Calculator {
public:
    static double divide(double a, double b) {
        if (b == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }

    static double squareRoot(double x) {
        if (x < 0) {
            throw std::domain_error("Negative square root");
        }
        return std::sqrt(x);
    }

    static void calculate() {
        std::string op;
        double a, b;

        std::cout << "Enter operation (div/sqrt): ";
        std::cin >> op;

        try {
            if (op == "div") {
                std::cout << "Enter two numbers: ";
                std::cin >> a >> b;
                std::cout << "Result: " << divide(a, b) << "\n";
            }
            else if (op == "sqrt") {
                std::cout << "Enter number: ";
                std::cin >> a;
                std::cout << "Result: " << squareRoot(a) << "\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
};

int main()
{
    Calculator::calculate();
    return 0;
}
```

---

## Key Takeaways

1. ✅ Exceptions separate error handling from normal code
2. ✅ Use try-catch blocks to handle exceptions
3. ✅ throw keyword raises exceptions
4. ✅ Standard exception types in <stdexcept>
5. ✅ Exceptions propagate up the call stack
6. ✅ Multiple catch blocks for different types
7. ✅ catch(...) for unknown exceptions
8. ✅ Always catch by const reference

---

## Next Lesson Preview

In **Lesson 62**, we'll learn about:
- Custom exception classes
- Exception hierarchies
- Rich exception information
- Best practices

**Great work on exception basics!**
