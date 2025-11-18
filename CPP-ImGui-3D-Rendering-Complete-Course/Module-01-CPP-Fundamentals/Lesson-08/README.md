# Lesson 08: Functions - Basics
**Duration: 3 hours**

## Learning Objectives
- Understand what functions are and why they're essential
- Declare and define functions
- Understand function syntax
- Pass parameters to functions
- Return values from functions
- Understand function prototypes
- Use void functions
- Master function scope

## 1. What are Functions?

Functions are reusable blocks of code that perform a specific task.

### Why Use Functions?

```cpp
#include <iostream>

// WITHOUT FUNCTIONS - Repetitive code
int main() {
    // Calculate area of rectangle 1
    int length1 = 5, width1 = 3;
    int area1 = length1 * width1;
    std::cout << "Area 1: " << area1 << std::endl;

    // Calculate area of rectangle 2
    int length2 = 7, width2 = 4;
    int area2 = length2 * width2;
    std::cout << "Area 2: " << area2 << std::endl;

    // Calculate area of rectangle 3
    int length3 = 6, width3 = 2;
    int area3 = length3 * width3;
    std::cout << "Area 3: " << area3 << std::endl;

    return 0;
}
```

```cpp
#include <iostream>

// WITH FUNCTIONS - Clean and reusable
int calculateArea(int length, int width) {
    return length * width;
}

int main() {
    std::cout << "Area 1: " << calculateArea(5, 3) << std::endl;
    std::cout << "Area 2: " << calculateArea(7, 4) << std::endl;
    std::cout << "Area 3: " << calculateArea(6, 2) << std::endl;

    return 0;
}
```

### Benefits of Functions
1. **Code Reusability** - Write once, use many times
2. **Modularity** - Break complex problems into smaller pieces
3. **Maintainability** - Fix bugs in one place
4. **Readability** - Self-documenting code
5. **Testing** - Test individual units independently

## 2. Function Syntax

### Basic Function Structure

```cpp
returnType functionName(parameterType parameterName) {
    // Function body
    return value;
}
```

### Parts of a Function

```cpp
#include <iostream>

// Return type: int
// Function name: add
// Parameters: int a, int b
// Return statement: return a + b
int add(int a, int b) {
    int sum = a + b;
    return sum;
}

int main() {
    int result = add(5, 3);
    std::cout << "5 + 3 = " << result << std::endl;
    return 0;
}
```

## 3. Simple Functions

### Function with No Parameters

```cpp
#include <iostream>

void sayHello() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    sayHello();  // Function call
    sayHello();  // Can call multiple times
    sayHello();

    return 0;
}
```

### Function Returning a Value

```cpp
#include <iostream>

int getNumber() {
    return 42;
}

int main() {
    int num = getNumber();
    std::cout << "The number is: " << num << std::endl;

    return 0;
}
```

### Function with Parameters

```cpp
#include <iostream>

void greet(std::string name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}

int main() {
    greet("Alice");
    greet("Bob");
    greet("Charlie");

    return 0;
}
```

### Function with Multiple Parameters

```cpp
#include <iostream>

int multiply(int a, int b) {
    return a * b;
}

int main() {
    std::cout << "3 * 4 = " << multiply(3, 4) << std::endl;
    std::cout << "7 * 8 = " << multiply(7, 8) << std::endl;

    return 0;
}
```

## 4. void Functions

Functions that don't return a value use `void`.

### Basic void Function

```cpp
#include <iostream>

void printSeparator() {
    std::cout << "==================" << std::endl;
}

int main() {
    std::cout << "Title" << std::endl;
    printSeparator();
    std::cout << "Content here" << std::endl;
    printSeparator();

    return 0;
}
```

### void Function with Parameters

```cpp
#include <iostream>

void printRectangle(int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }
}

int main() {
    printRectangle(5, 3);
    std::cout << std::endl;
    printRectangle(8, 2);

    return 0;
}
```

## 5. Return Statement

### Early Return

```cpp
#include <iostream>

int divide(int a, int b) {
    if (b == 0) {
        std::cout << "Error: Division by zero!" << std::endl;
        return 0;  // Early return
    }
    return a / b;
}

int main() {
    std::cout << "10 / 2 = " << divide(10, 2) << std::endl;
    std::cout << "10 / 0 = " << divide(10, 0) << std::endl;

    return 0;
}
```

### Multiple Return Points

```cpp
#include <iostream>

int maximum(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    } else if (b >= a && b >= c) {
        return b;
    } else {
        return c;
    }
}

int main() {
    std::cout << "Max of 5, 3, 7: " << maximum(5, 3, 7) << std::endl;
    std::cout << "Max of 10, 20, 15: " << maximum(10, 20, 15) << std::endl;

    return 0;
}
```

## 6. Function Prototypes (Forward Declaration)

### The Problem

```cpp
#include <iostream>

int main() {
    int result = add(5, 3);  // ERROR: add not declared yet!
    std::cout << result << std::endl;
    return 0;
}

int add(int a, int b) {
    return a + b;
}
```

### Solution: Function Prototype

```cpp
#include <iostream>

// Function prototype (declaration)
int add(int a, int b);

int main() {
    int result = add(5, 3);  // OK: compiler knows add exists
    std::cout << result << std::endl;
    return 0;
}

// Function definition
int add(int a, int b) {
    return a + b;
}
```

### Multiple Prototypes

```cpp
#include <iostream>

// Function prototypes
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
double divide(int a, int b);

int main() {
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;
    std::cout << "5 - 3 = " << subtract(5, 3) << std::endl;
    std::cout << "5 * 3 = " << multiply(5, 3) << std::endl;
    std::cout << "5 / 2 = " << divide(5, 2) << std::endl;

    return 0;
}

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(int a, int b) {
    if (b == 0) return 0.0;
    return static_cast<double>(a) / b;
}
```

## 7. Pass by Value

By default, C++ passes copies of arguments.

```cpp
#include <iostream>

void modifyValue(int x) {
    x = 100;  // Modifies the copy, not the original
    std::cout << "Inside function: " << x << std::endl;
}

int main() {
    int num = 50;
    std::cout << "Before: " << num << std::endl;

    modifyValue(num);

    std::cout << "After: " << num << std::endl;  // Still 50!

    return 0;
}

// Output:
// Before: 50
// Inside function: 100
// After: 50
```

## 8. Function Scope

### Local Variables

```cpp
#include <iostream>

void function1() {
    int x = 10;  // Local to function1
    std::cout << "function1 x: " << x << std::endl;
}

void function2() {
    int x = 20;  // Different x, local to function2
    std::cout << "function2 x: " << x << std::endl;
}

int main() {
    int x = 30;  // Different x, local to main
    std::cout << "main x: " << x << std::endl;

    function1();
    function2();

    std::cout << "main x still: " << x << std::endl;

    return 0;
}
```

### Parameters are Local

```cpp
#include <iostream>

int calculate(int a, int b) {
    int result = a + b;  // result is local
    return result;
}

int main() {
    int x = 5, y = 3;
    int sum = calculate(x, y);

    std::cout << "Sum: " << sum << std::endl;
    // std::cout << result;  // ERROR: result doesn't exist here

    return 0;
}
```

## 9. Practical Function Examples

### Example 1: Temperature Converter

```cpp
#include <iostream>

double celsiusToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

int main() {
    double temp;

    std::cout << "Enter temperature in Celsius: ";
    std::cin >> temp;
    std::cout << temp << "°C = " << celsiusToFahrenheit(temp) << "°F" << std::endl;

    std::cout << "Enter temperature in Fahrenheit: ";
    std::cin >> temp;
    std::cout << temp << "°F = " << fahrenheitToCelsius(temp) << "°C" << std::endl;

    return 0;
}
```

### Example 2: Prime Number Checker

```cpp
#include <iostream>

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int main() {
    for (int i = 1; i <= 20; i++) {
        if (isPrime(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
```

### Example 3: Calculator Functions

```cpp
#include <iostream>

// Function prototypes
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
void displayMenu();

int main() {
    double num1, num2;
    int choice;

    do {
        displayMenu();
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 4) {
            std::cout << "Enter two numbers: ";
            std::cin >> num1 >> num2;

            switch (choice) {
                case 1:
                    std::cout << "Result: " << add(num1, num2) << std::endl;
                    break;
                case 2:
                    std::cout << "Result: " << subtract(num1, num2) << std::endl;
                    break;
                case 3:
                    std::cout << "Result: " << multiply(num1, num2) << std::endl;
                    break;
                case 4:
                    std::cout << "Result: " << divide(num1, num2) << std::endl;
                    break;
            }
        }

    } while (choice != 5);

    std::cout << "Goodbye!" << std::endl;
    return 0;
}

void displayMenu() {
    std::cout << "\n=== Calculator ===" << std::endl;
    std::cout << "1. Add" << std::endl;
    std::cout << "2. Subtract" << std::endl;
    std::cout << "3. Multiply" << std::endl;
    std::cout << "4. Divide" << std::endl;
    std::cout << "5. Exit" << std::endl;
}

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        std::cout << "Error: Division by zero!" << std::endl;
        return 0.0;
    }
    return a / b;
}
```

### Example 4: Grading System

```cpp
#include <iostream>

char getGrade(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

bool isPassing(int score) {
    return score >= 60;
}

void printReport(int score) {
    char grade = getGrade(score);

    std::cout << "\n=== Grade Report ===" << std::endl;
    std::cout << "Score: " << score << std::endl;
    std::cout << "Grade: " << grade << std::endl;

    if (isPassing(score)) {
        std::cout << "Status: PASS" << std::endl;
    } else {
        std::cout << "Status: FAIL" << std::endl;
    }
}

int main() {
    int score;

    std::cout << "Enter score (0-100): ";
    std::cin >> score;

    printReport(score);

    return 0;
}
```

### Example 5: Factorial Calculator

```cpp
#include <iostream>

long long factorial(int n) {
    if (n < 0) {
        return -1;  // Error: negative number
    }

    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

void printFactorial(int n) {
    long long fact = factorial(n);

    if (fact == -1) {
        std::cout << "Error: Cannot calculate factorial of negative number" << std::endl;
    } else {
        std::cout << n << "! = " << fact << std::endl;
    }
}

int main() {
    for (int i = 0; i <= 10; i++) {
        printFactorial(i);
    }

    return 0;
}
```

## 10. Exercises

### Exercise 1: Even/Odd Checker
Write a function `bool isEven(int n)` that returns true if n is even.

### Exercise 2: Maximum of Three
Write a function `int max3(int a, int b, int c)` that returns the maximum of three numbers.

### Exercise 3: Power Function
Write a function `double power(double base, int exponent)` that calculates base^exponent.

### Exercise 4: Digit Sum
Write a function `int sumDigits(int n)` that returns the sum of digits of n.

### Exercise 5: Perfect Number
Write a function `bool isPerfect(int n)` that checks if n is a perfect number (sum of divisors equals n).

## 11. Solutions

### Solution 1: Even/Odd Checker

```cpp
#include <iostream>

bool isEven(int n) {
    return n % 2 == 0;
}

int main() {
    for (int i = 1; i <= 10; i++) {
        if (isEven(i)) {
            std::cout << i << " is even" << std::endl;
        } else {
            std::cout << i << " is odd" << std::endl;
        }
    }
    return 0;
}
```

### Solution 2: Maximum of Three

```cpp
#include <iostream>

int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

int main() {
    std::cout << "Max of 5, 3, 7: " << max3(5, 3, 7) << std::endl;
    std::cout << "Max of 10, 20, 15: " << max3(10, 20, 15) << std::endl;
    std::cout << "Max of 100, 50, 75: " << max3(100, 50, 75) << std::endl;
    return 0;
}
```

### Solution 3: Power Function

```cpp
#include <iostream>

double power(double base, int exponent) {
    double result = 1.0;

    if (exponent > 0) {
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
    } else if (exponent < 0) {
        for (int i = 0; i < -exponent; i++) {
            result *= base;
        }
        result = 1.0 / result;
    }

    return result;
}

int main() {
    std::cout << "2^3 = " << power(2.0, 3) << std::endl;
    std::cout << "5^0 = " << power(5.0, 0) << std::endl;
    std::cout << "2^-2 = " << power(2.0, -2) << std::endl;
    return 0;
}
```

### Solution 4: Digit Sum

```cpp
#include <iostream>

int sumDigits(int n) {
    int sum = 0;

    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }

    return sum;
}

int main() {
    std::cout << "Sum of digits of 1234: " << sumDigits(1234) << std::endl;
    std::cout << "Sum of digits of 9876: " << sumDigits(9876) << std::endl;
    return 0;
}
```

### Solution 5: Perfect Number

```cpp
#include <iostream>

bool isPerfect(int n) {
    if (n <= 1) return false;

    int sum = 1;  // 1 is always a divisor

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }

    return sum == n;
}

int main() {
    std::cout << "Perfect numbers up to 10000:" << std::endl;

    for (int i = 1; i <= 10000; i++) {
        if (isPerfect(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
// Output: 6 28 496 8128
```

## Summary

### Key Points
- Functions are reusable blocks of code
- Functions have: return type, name, parameters, body
- `void` functions don't return a value
- Function prototypes declare functions before definition
- Parameters are passed by value (copies)
- Local variables exist only within their function
- Functions improve code organization and reusability

### Function Syntax Reminder

```cpp
// Prototype
returnType functionName(parameterType parameterName);

// Definition
returnType functionName(parameterType parameterName) {
    // Function body
    return value;  // if not void
}

// Call
result = functionName(argumentValue);
```

### Best Practices
- Use meaningful function names (verbs: calculate, get, print, check)
- Keep functions short and focused (one task)
- Use prototypes for better code organization
- Comment complex functions
- Validate input parameters
- Use consistent naming conventions

## Checklist
- [ ] Understand what functions are
- [ ] Can declare and define functions
- [ ] Understand parameters and return values
- [ ] Can use function prototypes
- [ ] Understand void functions
- [ ] Know about pass by value
- [ ] Understand function scope
- [ ] Completed all exercises

---

**Time to complete**: 3 hours
**Previous lesson**: [Lesson 07 - Break and Continue](../Lesson-07/README.md)
**Next lesson**: [Lesson 09 - Function Parameters and Return Values](../Lesson-09/README.md)
