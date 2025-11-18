# Lesson 04: Functions and Modular Programming
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Functions](#introduction-to-functions)
2. [Function Basics](#function-basics)
3. [Function Parameters](#function-parameters)
4. [Return Values](#return-values)
5. [Function Prototypes and Declarations](#function-prototypes-and-declarations)
6. [Pass by Value vs Pass by Reference](#pass-by-value-vs-pass-by-reference)
7. [Function Overloading](#function-overloading)
8. [Default Parameters](#default-parameters)
9. [Inline Functions](#inline-functions)
10. [Scope and Lifetime](#scope-and-lifetime)
11. [Complete Working Examples](#complete-working-examples)
12. [Common Mistakes](#common-mistakes)
13. [Exercises](#exercises)

## Introduction to Functions

Functions are the building blocks of modular programming. They allow you to:
- Break complex problems into smaller, manageable pieces
- Reuse code without duplication
- Make code easier to read, test, and maintain
- Abstract implementation details

### What is a Function?

A function is a named block of code that performs a specific task. Think of it as a mini-program within your program.

### Benefits of Functions:
1. **Code Reusability**: Write once, use many times
2. **Modularity**: Divide program into logical parts
3. **Maintainability**: Fix bugs in one place
4. **Readability**: Self-documenting code with good function names
5. **Testing**: Test individual pieces independently

## Function Basics

### Anatomy of a Function

```cpp
return_type function_name(parameter_list)
{
    // Function body
    // Statements to execute
    return value;  // if return_type is not void
}
```

**Components:**
1. **Return Type**: Data type of value returned (int, double, void, etc.)
2. **Function Name**: Descriptive identifier following naming rules
3. **Parameter List**: Input values (can be empty)
4. **Function Body**: Code to execute
5. **Return Statement**: Value sent back to caller (optional for void)

### Simple Function Example

```cpp
#include <iostream>

// Function definition
void sayHello()
{
    std::cout << "Hello, World!\n";
}

int main()
{
    sayHello();  // Function call
    sayHello();  // Can call multiple times
    sayHello();

    return 0;
}
```

Output:
```
Hello, World!
Hello, World!
Hello, World!
```

### Function with Return Value

```cpp
#include <iostream>

// Returns an integer
int getNumber()
{
    return 42;
}

int main()
{
    int num = getNumber();  // Store returned value
    std::cout << "Number: " << num << "\n";

    // Can use directly in expressions
    std::cout << "Double: " << (getNumber() * 2) << "\n";

    return 0;
}
```

## Function Parameters

Parameters allow functions to receive input data.

### Single Parameter

```cpp
#include <iostream>

void greet(std::string name)
{
    std::cout << "Hello, " << name << "!\n";
}

int main()
{
    greet("Alice");
    greet("Bob");
    greet("Charlie");

    return 0;
}
```

Output:
```
Hello, Alice!
Hello, Bob!
Hello, Charlie!
```

### Multiple Parameters

```cpp
#include <iostream>

// Multiple parameters separated by commas
int add(int a, int b)
{
    return a + b;
}

void printRectangleArea(double width, double height)
{
    double area = width * height;
    std::cout << "Rectangle area: " << area << "\n";
}

int main()
{
    int sum = add(10, 20);
    std::cout << "Sum: " << sum << "\n";

    printRectangleArea(5.0, 3.0);
    printRectangleArea(7.5, 2.5);

    return 0;
}
```

### Parameter vs Argument

**Important terminology:**
- **Parameter**: Variable in function definition (formal parameter)
- **Argument**: Actual value passed when calling (actual parameter)

```cpp
void display(int number)  // 'number' is a parameter
{
    std::cout << number << "\n";
}

int main()
{
    display(42);  // '42' is an argument

    int x = 100;
    display(x);   // 'x' is an argument

    return 0;
}
```

## Return Values

### Basic Return

```cpp
#include <iostream>

int square(int n)
{
    return n * n;
}

double divide(double a, double b)
{
    return a / b;
}

bool isEven(int n)
{
    return (n % 2 == 0);
}

int main()
{
    std::cout << "Square of 5: " << square(5) << "\n";
    std::cout << "10 / 3 = " << divide(10, 3) << "\n";
    std::cout << "Is 7 even? " << (isEven(7) ? "Yes" : "No") << "\n";

    return 0;
}
```

### Multiple Return Statements

```cpp
#include <iostream>

// Return different values based on conditions
int getSign(int number)
{
    if (number > 0)
        return 1;
    else if (number < 0)
        return -1;
    else
        return 0;
}

std::string getGrade(int score)
{
    if (score >= 90)
        return "A";
    else if (score >= 80)
        return "B";
    else if (score >= 70)
        return "C";
    else if (score >= 60)
        return "D";
    else
        return "F";
}

int main()
{
    std::cout << "Sign of -5: " << getSign(-5) << "\n";
    std::cout << "Sign of 0: " << getSign(0) << "\n";
    std::cout << "Sign of 10: " << getSign(10) << "\n";

    std::cout << "Grade for 85: " << getGrade(85) << "\n";

    return 0;
}
```

### Void Functions (No Return)

```cpp
#include <iostream>

void printLine()
{
    std::cout << "------------------------\n";
    // No return statement needed
}

void printBox(int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }
    // Implicit return at end
}

int main()
{
    printLine();
    std::cout << "Welcome\n";
    printLine();

    printBox(5, 3);

    return 0;
}
```

## Function Prototypes and Declarations

### The Problem

```cpp
// This will cause an error!
int main()
{
    int result = add(5, 3);  // Error: 'add' not declared
    return 0;
}

int add(int a, int b)  // Defined after use
{
    return a + b;
}
```

### Solution: Function Prototype

```cpp
#include <iostream>

// Function prototype (declaration)
int add(int a, int b);
int multiply(int a, int b);
void printResult(int value);

int main()
{
    int sum = add(5, 3);
    int product = multiply(5, 3);

    printResult(sum);
    printResult(product);

    return 0;
}

// Function definitions (implementations)
int add(int a, int b)
{
    return a + b;
}

int multiply(int a, int b)
{
    return a * b;
}

void printResult(int value)
{
    std::cout << "Result: " << value << "\n";
}
```

### Prototype Syntax

```cpp
// Full prototype with parameter names (recommended for clarity)
double calculateArea(double width, double height);

// Prototype without parameter names (also valid)
double calculateArea(double, double);

// Multiple prototypes
int max(int a, int b);
int min(int a, int b);
double average(double a, double b);
```

## Pass by Value vs Pass by Reference

### Pass by Value (Default)

When you pass by value, a **copy** of the argument is made.

```cpp
#include <iostream>

void modifyValue(int x)
{
    x = 100;  // Modifies the copy, not the original
    std::cout << "Inside function: " << x << "\n";
}

int main()
{
    int number = 5;
    std::cout << "Before: " << number << "\n";

    modifyValue(number);

    std::cout << "After: " << number << "\n";  // Still 5!

    return 0;
}
```

Output:
```
Before: 5
Inside function: 100
After: 5
```

### Pass by Reference

Using `&` passes a reference (alias) to the original variable.

```cpp
#include <iostream>

void modifyReference(int& x)  // & makes it a reference
{
    x = 100;  // Modifies the original!
    std::cout << "Inside function: " << x << "\n";
}

int main()
{
    int number = 5;
    std::cout << "Before: " << number << "\n";

    modifyReference(number);

    std::cout << "After: " << number << "\n";  // Changed to 100!

    return 0;
}
```

Output:
```
Before: 5
Inside function: 100
After: 100
```

### When to Use Each

**Pass by Value:**
- Small data types (int, double, char, bool)
- When you don't want to modify the original
- When you need a copy anyway

**Pass by Reference:**
- Large objects (avoid copying overhead)
- When you want to modify the original
- When returning multiple values

### Const Reference (Read-Only)

```cpp
#include <iostream>
#include <string>

// Pass by const reference: efficient but prevents modification
void printString(const std::string& str)
{
    std::cout << str << "\n";
    // str = "Modified";  // ERROR: cannot modify const reference
}

double calculateArea(const double& width, const double& height)
{
    return width * height;
    // width = 10;  // ERROR: cannot modify
}

int main()
{
    std::string message = "Hello, World!";
    printString(message);  // Efficient, no copy

    double area = calculateArea(5.0, 3.0);
    std::cout << "Area: " << area << "\n";

    return 0;
}
```

### Multiple Return Values Using References

```cpp
#include <iostream>

// Return multiple values via reference parameters
void getMinMax(int a, int b, int c, int& min, int& max)
{
    min = a;
    max = a;

    if (b < min) min = b;
    if (b > max) max = b;

    if (c < min) min = c;
    if (c > max) max = c;
}

void divideWithRemainder(int dividend, int divisor, int& quotient, int& remainder)
{
    quotient = dividend / divisor;
    remainder = dividend % divisor;
}

int main()
{
    int minimum, maximum;
    getMinMax(10, 5, 15, minimum, maximum);
    std::cout << "Min: " << minimum << ", Max: " << maximum << "\n";

    int quot, rem;
    divideWithRemainder(17, 5, quot, rem);
    std::cout << "17 / 5 = " << quot << " remainder " << rem << "\n";

    return 0;
}
```

## Function Overloading

Function overloading allows multiple functions with the same name but different parameters.

### Basic Overloading

```cpp
#include <iostream>

// Overloaded functions: same name, different parameters

int add(int a, int b)
{
    std::cout << "Adding two integers\n";
    return a + b;
}

double add(double a, double b)
{
    std::cout << "Adding two doubles\n";
    return a + b;
}

int add(int a, int b, int c)
{
    std::cout << "Adding three integers\n";
    return a + b + c;
}

int main()
{
    std::cout << add(5, 3) << "\n";           // Calls int version
    std::cout << add(5.5, 3.2) << "\n";       // Calls double version
    std::cout << add(1, 2, 3) << "\n";        // Calls three-parameter version

    return 0;
}
```

### Practical Overloading Example

```cpp
#include <iostream>
#include <string>

// Print different types
void print(int value)
{
    std::cout << "Integer: " << value << "\n";
}

void print(double value)
{
    std::cout << "Double: " << value << "\n";
}

void print(const std::string& value)
{
    std::cout << "String: " << value << "\n";
}

void print(bool value)
{
    std::cout << "Boolean: " << (value ? "true" : "false") << "\n";
}

int main()
{
    print(42);
    print(3.14);
    print("Hello");
    print(true);

    return 0;
}
```

### Overloading Rules

1. **Different number of parameters**: ✅
2. **Different types of parameters**: ✅
3. **Different parameter order**: ✅
4. **Only different return type**: ❌ (Not allowed)

```cpp
// VALID overloads
void func(int x);
void func(double x);
void func(int x, int y);
void func(double x, int y);
void func(int x, double y);

// INVALID - same signature, only return type differs
int getValue();
double getValue();  // ERROR: redefinition
```

## Default Parameters

Default parameters provide default values when arguments are not supplied.

### Basic Default Parameters

```cpp
#include <iostream>

// Default parameter must be rightmost
void greet(std::string name, std::string greeting = "Hello")
{
    std::cout << greeting << ", " << name << "!\n";
}

void printNumber(int num, int base = 10)
{
    std::cout << "Number " << num << " in base " << base << "\n";
    // (Simplified - actual conversion not shown)
}

int main()
{
    greet("Alice");              // Uses default "Hello"
    greet("Bob", "Hi");          // Overrides default
    greet("Charlie", "Welcome"); // Overrides default

    printNumber(42);             // Uses default base 10
    printNumber(42, 16);         // Uses base 16

    return 0;
}
```

### Multiple Default Parameters

```cpp
#include <iostream>

// All defaults must be rightmost parameters
void displayMessage(std::string msg,
                   int repeat = 1,
                   char separator = '\n')
{
    for (int i = 0; i < repeat; i++)
    {
        std::cout << msg;
        if (i < repeat - 1)
            std::cout << separator;
    }
    std::cout << "\n";
}

int main()
{
    displayMessage("Hello");              // Uses all defaults
    displayMessage("Hello", 3);           // Custom repeat
    displayMessage("Hello", 3, '-');      // Custom repeat and separator

    return 0;
}
```

Output:
```
Hello
Hello-Hello-Hello
Hello-Hello-Hello
```

### Default Parameters Rules

```cpp
// VALID: Defaults on the right
void func1(int a, int b = 10, int c = 20);

// INVALID: Non-default after default
void func2(int a, int b = 10, int c);  // ERROR

// VALID: Can specify in prototype only
void func3(int a, int b = 10);  // In header

void func3(int a, int b)  // Implementation (no defaults here)
{
    // body
}
```

### Practical Example: Rectangle Class Helper

```cpp
#include <iostream>

void drawRectangle(int width, int height, char fillChar = '*', bool filled = true)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (filled || i == 0 || i == height - 1 || j == 0 || j == width - 1)
                std::cout << fillChar << " ";
            else
                std::cout << "  ";
        }
        std::cout << "\n";
    }
}

int main()
{
    std::cout << "Default rectangle:\n";
    drawRectangle(5, 3);

    std::cout << "\nCustom char:\n";
    drawRectangle(5, 3, '#');

    std::cout << "\nOutline only:\n";
    drawRectangle(7, 4, '*', false);

    return 0;
}
```

## Inline Functions

Inline functions suggest to the compiler to insert the function's code directly at each call site, potentially improving performance.

### Basic Inline Function

```cpp
#include <iostream>

// Inline suggestion for small, frequently called functions
inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

inline int min(int a, int b)
{
    return (a < b) ? a : b;
}

inline int square(int x)
{
    return x * x;
}

int main()
{
    std::cout << "Max of 10 and 20: " << max(10, 20) << "\n";
    std::cout << "Min of 10 and 20: " << min(10, 20) << "\n";
    std::cout << "Square of 5: " << square(5) << "\n";

    return 0;
}
```

### When to Use Inline

**Good candidates:**
- Small functions (1-3 lines)
- Frequently called functions
- Simple calculations
- Getters and setters

**Avoid inline for:**
- Large functions
- Functions with loops
- Recursive functions
- Functions with static variables

### Inline vs Macro

```cpp
// Old C-style macro (avoid in C++)
#define SQUARE(x) ((x) * (x))

// Better: inline function
inline int square(int x)
{
    return x * x;
}

int main()
{
    // Macro problem: double evaluation
    int a = 5;
    int result1 = SQUARE(a++);  // a is incremented twice!

    // Inline function: safe
    int b = 5;
    int result2 = square(b++);  // b incremented once, as expected

    return 0;
}
```

### Compiler Discretion

```cpp
inline void complexFunction()
{
    // Even with 'inline' keyword, compiler may choose not to inline
    for (int i = 0; i < 1000; i++)
    {
        // Complex logic
    }
}

// Compiler may inline even without keyword if it decides to optimize
int simpleAdd(int a, int b)
{
    return a + b;  // Very likely to be inlined automatically
}
```

## Scope and Lifetime

### Local Variables

```cpp
#include <iostream>

void function1()
{
    int x = 10;  // Local to function1
    std::cout << "function1: x = " << x << "\n";
}

void function2()
{
    int x = 20;  // Different variable, same name
    std::cout << "function2: x = " << x << "\n";
}

int main()
{
    int x = 30;  // Local to main
    std::cout << "main: x = " << x << "\n";

    function1();
    function2();

    std::cout << "main: x = " << x << "\n";  // Still 30

    return 0;
}
```

### Block Scope

```cpp
#include <iostream>

int main()
{
    int x = 10;
    std::cout << "Outer x: " << x << "\n";

    {  // New block
        int x = 20;  // Shadows outer x
        std::cout << "Inner x: " << x << "\n";

        int y = 30;  // Only exists in this block
        std::cout << "Inner y: " << y << "\n";
    }

    std::cout << "Outer x: " << x << "\n";  // Back to 10
    // std::cout << y << "\n";  // ERROR: y doesn't exist here

    return 0;
}
```

### Global Variables

```cpp
#include <iostream>

// Global variable - accessible everywhere
int globalCounter = 0;

void incrementCounter()
{
    globalCounter++;
    std::cout << "Counter: " << globalCounter << "\n";
}

void resetCounter()
{
    globalCounter = 0;
}

int main()
{
    std::cout << "Initial: " << globalCounter << "\n";

    incrementCounter();
    incrementCounter();
    incrementCounter();

    resetCounter();
    std::cout << "After reset: " << globalCounter << "\n";

    return 0;
}
```

### Static Local Variables

```cpp
#include <iostream>

void countCalls()
{
    static int count = 0;  // Initialized only once
    count++;
    std::cout << "This function has been called " << count << " times\n";
}

int generateID()
{
    static int nextID = 1000;
    return nextID++;
}

int main()
{
    countCalls();  // 1
    countCalls();  // 2
    countCalls();  // 3

    std::cout << "ID: " << generateID() << "\n";  // 1000
    std::cout << "ID: " << generateID() << "\n";  // 1001
    std::cout << "ID: " << generateID() << "\n";  // 1002

    return 0;
}
```

## Complete Working Examples

### Example 1: Temperature Converter

```cpp
#include <iostream>
#include <iomanip>

// Function prototypes
double celsiusToFahrenheit(double celsius);
double fahrenheitToCelsius(double fahrenheit);
double celsiusToKelvin(double celsius);
double kelvinToCelsius(double kelvin);
void printMenu();
void displayTemperature(double temp, const std::string& unit);

int main()
{
    int choice;
    double temperature, result;

    do
    {
        printMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 0)
        {
            std::cout << "Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 4)
        {
            std::cout << "Invalid choice!\n";
            continue;
        }

        std::cout << "Enter temperature: ";
        std::cin >> temperature;

        switch (choice)
        {
            case 1:
                result = celsiusToFahrenheit(temperature);
                displayTemperature(result, "Fahrenheit");
                break;
            case 2:
                result = fahrenheitToCelsius(temperature);
                displayTemperature(result, "Celsius");
                break;
            case 3:
                result = celsiusToKelvin(temperature);
                displayTemperature(result, "Kelvin");
                break;
            case 4:
                result = kelvinToCelsius(temperature);
                displayTemperature(result, "Celsius");
                break;
        }

        std::cout << "\n";

    } while (true);

    return 0;
}

void printMenu()
{
    std::cout << "\n=== Temperature Converter ===\n";
    std::cout << "1. Celsius to Fahrenheit\n";
    std::cout << "2. Fahrenheit to Celsius\n";
    std::cout << "3. Celsius to Kelvin\n";
    std::cout << "4. Kelvin to Celsius\n";
    std::cout << "0. Exit\n";
}

double celsiusToFahrenheit(double celsius)
{
    return (celsius * 9.0 / 5.0) + 32.0;
}

double fahrenheitToCelsius(double fahrenheit)
{
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double celsiusToKelvin(double celsius)
{
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin)
{
    return kelvin - 273.15;
}

void displayTemperature(double temp, const std::string& unit)
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Result: " << temp << " " << unit;
}
```

### Example 2: Prime Number Utilities

```cpp
#include <iostream>

// Function prototypes
bool isPrime(int n);
void printPrimesInRange(int start, int end);
int countPrimes(int start, int end);
int nextPrime(int n);
void printPrimeFactors(int n);

int main()
{
    std::cout << "=== Prime Number Utilities ===\n\n";

    // Test isPrime
    std::cout << "Testing isPrime:\n";
    for (int i = 1; i <= 20; i++)
    {
        if (isPrime(i))
            std::cout << i << " ";
    }
    std::cout << "\n\n";

    // Print primes in range
    std::cout << "Primes between 50 and 100:\n";
    printPrimesInRange(50, 100);
    std::cout << "\n";

    // Count primes
    std::cout << "Count of primes from 1 to 100: " << countPrimes(1, 100) << "\n\n";

    // Next prime
    int num = 50;
    std::cout << "Next prime after " << num << ": " << nextPrime(num) << "\n\n";

    // Prime factors
    std::cout << "Prime factors of 84:\n";
    printPrimeFactors(84);

    return 0;
}

bool isPrime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

void printPrimesInRange(int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        if (isPrime(i))
            std::cout << i << " ";
    }
    std::cout << "\n";
}

int countPrimes(int start, int end)
{
    int count = 0;
    for (int i = start; i <= end; i++)
    {
        if (isPrime(i))
            count++;
    }
    return count;
}

int nextPrime(int n)
{
    int candidate = n + 1;
    while (!isPrime(candidate))
    {
        candidate++;
    }
    return candidate;
}

void printPrimeFactors(int n)
{
    std::cout << n << " = ";

    bool first = true;

    // Handle 2s
    while (n % 2 == 0)
    {
        if (!first) std::cout << " × ";
        std::cout << "2";
        first = false;
        n = n / 2;
    }

    // Handle odd factors
    for (int i = 3; i * i <= n; i += 2)
    {
        while (n % i == 0)
        {
            if (!first) std::cout << " × ";
            std::cout << i;
            first = false;
            n = n / i;
        }
    }

    // If n is still > 1, it's a prime factor
    if (n > 1)
    {
        if (!first) std::cout << " × ";
        std::cout << n;
    }

    std::cout << "\n";
}
```

### Example 3: String Manipulation Library

```cpp
#include <iostream>
#include <string>

// Function prototypes
std::string toUpperCase(const std::string& str);
std::string toLowerCase(const std::string& str);
std::string reverse(const std::string& str);
bool isPalindrome(const std::string& str);
int countVowels(const std::string& str);
int countWords(const std::string& str);
std::string trim(const std::string& str);

int main()
{
    std::string text = "  Hello World  ";

    std::cout << "Original: \"" << text << "\"\n";
    std::cout << "Uppercase: \"" << toUpperCase(text) << "\"\n";
    std::cout << "Lowercase: \"" << toLowerCase(text) << "\"\n";
    std::cout << "Reversed: \"" << reverse(text) << "\"\n";
    std::cout << "Trimmed: \"" << trim(text) << "\"\n";
    std::cout << "Vowel count: " << countVowels(text) << "\n";
    std::cout << "Word count: " << countWords(text) << "\n";

    std::string word1 = "racecar";
    std::string word2 = "hello";

    std::cout << "\n\"" << word1 << "\" is palindrome: "
              << (isPalindrome(word1) ? "Yes" : "No") << "\n";
    std::cout << "\"" << word2 << "\" is palindrome: "
              << (isPalindrome(word2) ? "Yes" : "No") << "\n";

    return 0;
}

std::string toUpperCase(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.length(); i++)
    {
        if (result[i] >= 'a' && result[i] <= 'z')
        {
            result[i] = result[i] - 32;  // Convert to uppercase
        }
    }
    return result;
}

std::string toLowerCase(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.length(); i++)
    {
        if (result[i] >= 'A' && result[i] <= 'Z')
        {
            result[i] = result[i] + 32;  // Convert to lowercase
        }
    }
    return result;
}

std::string reverse(const std::string& str)
{
    std::string result = str;
    int left = 0;
    int right = result.length() - 1;

    while (left < right)
    {
        char temp = result[left];
        result[left] = result[right];
        result[right] = temp;
        left++;
        right--;
    }

    return result;
}

bool isPalindrome(const std::string& str)
{
    int left = 0;
    int right = str.length() - 1;

    while (left < right)
    {
        if (str[left] != str[right])
            return false;
        left++;
        right--;
    }

    return true;
}

int countVowels(const std::string& str)
{
    int count = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        char ch = toLowerCase(std::string(1, str[i]))[0];
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
        {
            count++;
        }
    }
    return count;
}

int countWords(const std::string& str)
{
    if (str.empty())
        return 0;

    int count = 0;
    bool inWord = false;

    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            if (!inWord)
            {
                count++;
                inWord = true;
            }
        }
        else
        {
            inWord = false;
        }
    }

    return count;
}

std::string trim(const std::string& str)
{
    size_t start = 0;
    size_t end = str.length();

    // Find first non-whitespace
    while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
    {
        start++;
    }

    // Find last non-whitespace
    while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\n'))
    {
        end--;
    }

    return str.substr(start, end - start);
}
```

### Example 4: Mathematical Functions Library

```cpp
#include <iostream>
#include <cmath>

// Function prototypes
long long factorial(int n);
long long fibonacci(int n);
double power(double base, int exponent);
int gcd(int a, int b);
int lcm(int a, int b);
bool isPerfectSquare(int n);
double average(const int arr[], int size);
int sumArray(const int arr[], int size);

int main()
{
    std::cout << "=== Mathematical Functions ===\n\n";

    // Factorial
    std::cout << "Factorial of 5: " << factorial(5) << "\n";
    std::cout << "Factorial of 10: " << factorial(10) << "\n\n";

    // Fibonacci
    std::cout << "First 10 Fibonacci numbers:\n";
    for (int i = 0; i < 10; i++)
    {
        std::cout << fibonacci(i) << " ";
    }
    std::cout << "\n\n";

    // Power
    std::cout << "2^10 = " << power(2, 10) << "\n";
    std::cout << "1.5^3 = " << power(1.5, 3) << "\n\n";

    // GCD and LCM
    std::cout << "GCD(48, 18) = " << gcd(48, 18) << "\n";
    std::cout << "LCM(48, 18) = " << lcm(48, 18) << "\n\n";

    // Perfect square
    std::cout << "Is 16 a perfect square? " << (isPerfectSquare(16) ? "Yes" : "No") << "\n";
    std::cout << "Is 20 a perfect square? " << (isPerfectSquare(20) ? "Yes" : "No") << "\n\n";

    // Array operations
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    std::cout << "Array: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    std::cout << "Sum: " << sumArray(numbers, size) << "\n";
    std::cout << "Average: " << average(numbers, size) << "\n";

    return 0;
}

long long factorial(int n)
{
    if (n <= 1)
        return 1;

    long long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

long long fibonacci(int n)
{
    if (n <= 1)
        return n;

    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++)
    {
        long long temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

double power(double base, int exponent)
{
    if (exponent == 0)
        return 1.0;

    if (exponent < 0)
    {
        base = 1.0 / base;
        exponent = -exponent;
    }

    double result = 1.0;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

int gcd(int a, int b)
{
    // Euclidean algorithm
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

bool isPerfectSquare(int n)
{
    if (n < 0)
        return false;

    int root = static_cast<int>(std::sqrt(n));
    return (root * root == n);
}

double average(const int arr[], int size)
{
    if (size == 0)
        return 0.0;

    return static_cast<double>(sumArray(arr, size)) / size;
}

int sumArray(const int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}
```

### Example 5: Menu-Driven Calculator

```cpp
#include <iostream>
#include <iomanip>
#include <string>

// Function prototypes
void displayMenu();
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double modulus(int a, int b);
double calculatePercentage(double value, double percent);
void displayResult(const std::string& operation, double result);
bool getNumbers(double& a, double& b);

int main()
{
    int choice;
    double num1, num2, result;

    std::cout << std::fixed << std::setprecision(2);

    do
    {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 0)
        {
            std::cout << "\nThank you for using the calculator!\n";
            break;
        }

        if (choice < 1 || choice > 6)
        {
            std::cout << "\nInvalid choice! Please try again.\n\n";
            continue;
        }

        if (!getNumbers(num1, num2))
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                result = add(num1, num2);
                displayResult("Addition", result);
                break;

            case 2:
                result = subtract(num1, num2);
                displayResult("Subtraction", result);
                break;

            case 3:
                result = multiply(num1, num2);
                displayResult("Multiplication", result);
                break;

            case 4:
                if (num2 == 0)
                {
                    std::cout << "\nError: Division by zero!\n\n";
                }
                else
                {
                    result = divide(num1, num2);
                    displayResult("Division", result);
                }
                break;

            case 5:
                if (num2 == 0)
                {
                    std::cout << "\nError: Modulus by zero!\n\n";
                }
                else
                {
                    result = modulus(static_cast<int>(num1), static_cast<int>(num2));
                    displayResult("Modulus", result);
                }
                break;

            case 6:
                result = calculatePercentage(num1, num2);
                std::cout << "\n" << num2 << "% of " << num1 << " = " << result << "\n\n";
                break;
        }

    } while (true);

    return 0;
}

void displayMenu()
{
    std::cout << "========================================\n";
    std::cout << "           CALCULATOR MENU              \n";
    std::cout << "========================================\n";
    std::cout << "1. Addition (+)\n";
    std::cout << "2. Subtraction (-)\n";
    std::cout << "3. Multiplication (*)\n";
    std::cout << "4. Division (/)\n";
    std::cout << "5. Modulus (%)\n";
    std::cout << "6. Percentage\n";
    std::cout << "0. Exit\n";
    std::cout << "========================================\n";
}

bool getNumbers(double& a, double& b)
{
    std::cout << "Enter first number: ";
    if (!(std::cin >> a))
    {
        std::cout << "Invalid input!\n\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }

    std::cout << "Enter second number: ";
    if (!(std::cin >> b))
    {
        std::cout << "Invalid input!\n\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }

    return true;
}

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    return a / b;
}

double modulus(int a, int b)
{
    return a % b;
}

double calculatePercentage(double value, double percent)
{
    return (value * percent) / 100.0;
}

void displayResult(const std::string& operation, double result)
{
    std::cout << "\n" << operation << " result: " << result << "\n\n";
}
```

## Common Mistakes

### 1. Forgetting Return Statement

```cpp
// WRONG - function should return a value
int add(int a, int b)
{
    int sum = a + b;
    // Missing return statement - undefined behavior!
}

// CORRECT
int add(int a, int b)
{
    int sum = a + b;
    return sum;  // Or simply: return a + b;
}
```

### 2. Mismatched Parameter Types

```cpp
// WRONG - passing wrong types
int divide(int a, int b)
{
    return a / b;  // Integer division loses decimal part
}

int main()
{
    std::cout << divide(5, 2);  // Prints 2, not 2.5
}

// CORRECT - use double for decimal results
double divide(double a, double b)
{
    return a / b;
}
```

### 3. Not Using Function Prototypes

```cpp
// WRONG - function used before declaration
int main()
{
    printMessage();  // ERROR: printMessage not declared
    return 0;
}

void printMessage()
{
    std::cout << "Hello\n";
}

// CORRECT - add prototype
void printMessage();  // Prototype

int main()
{
    printMessage();  // OK
    return 0;
}

void printMessage()
{
    std::cout << "Hello\n";
}
```

### 4. Confusing Pass by Value and Reference

```cpp
// WRONG - expecting to modify original
void increment(int x)
{
    x++;  // Only modifies local copy
}

int main()
{
    int num = 5;
    increment(num);
    std::cout << num;  // Still 5!
}

// CORRECT - use reference
void increment(int& x)
{
    x++;  // Modifies original
}
```

### 5. Default Parameter Errors

```cpp
// WRONG - default parameter not rightmost
void func(int a = 10, int b);  // ERROR

// WRONG - defaults in both prototype and definition
void func(int a = 10);  // Prototype
void func(int a = 10)   // ERROR: redefinition of default
{
}

// CORRECT
void func(int a, int b = 10);  // Defaults on right
void func(int a = 10);         // Prototype
void func(int a)               // Definition (no default)
{
}
```

### 6. Infinite Recursion (Preview)

```cpp
// WRONG - no base case
int factorial(int n)
{
    return n * factorial(n - 1);  // Never stops!
}

// CORRECT - has base case
int factorial(int n)
{
    if (n <= 1)
        return 1;  // Base case
    return n * factorial(n - 1);
}
```

## Exercises

### Exercise 1: Array Statistics
Write a program with the following functions:
- `double findMax(const double arr[], int size)` - Find maximum value
- `double findMin(const double arr[], int size)` - Find minimum value
- `double calculateMean(const double arr[], int size)` - Calculate average
- `double calculateMedian(double arr[], int size)` - Calculate median (sort first)
- `void displayStats(const double arr[], int size)` - Display all statistics

Test with an array of 10 numbers.

### Exercise 2: Password Validator
Create a password validation system with functions:
- `bool hasMinLength(const string& pwd, int minLen)` - Check minimum length
- `bool hasUpperCase(const string& pwd)` - Has uppercase letter
- `bool hasLowerCase(const string& pwd)` - Has lowercase letter
- `bool hasDigit(const string& pwd)` - Has digit
- `bool hasSpecialChar(const string& pwd)` - Has special character
- `bool isValidPassword(const string& pwd)` - Combines all checks
- `string getPasswordStrength(const string& pwd)` - Returns "Weak", "Medium", or "Strong"

### Exercise 3: Geometry Calculator
Create a menu-driven program with functions:
- `double circleArea(double radius)`
- `double circleCircumference(double radius)`
- `double rectangleArea(double width, double height)`
- `double rectanglePerimeter(double width, double height)`
- `double triangleArea(double base, double height)`
- `double sphereVolume(double radius)`
- `double cylinderVolume(double radius, double height)`

### Exercise 4: Number System Converter
Write functions to convert between number systems:
- `string decimalToBinary(int n)` - Convert to binary string
- `string decimalToOctal(int n)` - Convert to octal string
- `string decimalToHex(int n)` - Convert to hexadecimal string
- `int binaryToDecimal(const string& binary)` - Convert from binary
- `void displayAllFormats(int n)` - Show all conversions

### Exercise 5: Text Analyzer
Create a comprehensive text analysis tool:
- `int countSentences(const string& text)` - Count sentences (. ! ?)
- `int countParagraphs(const string& text)` - Count paragraphs (newlines)
- `double averageWordLength(const string& text)` - Average word length
- `string findLongestWord(const string& text)` - Find longest word
- `void displayAnalysis(const string& text)` - Show complete analysis

Include frequency count of each letter (a-z).

## Summary

You learned:
- ✅ Function basics: definition, declaration, and calling
- ✅ Parameters and arguments: passing data to functions
- ✅ Return values: getting results from functions
- ✅ Function prototypes: forward declarations
- ✅ Pass by value vs pass by reference
- ✅ Function overloading: same name, different parameters
- ✅ Default parameters: optional arguments
- ✅ Inline functions: performance optimization
- ✅ Scope and lifetime: local, global, static variables
- ✅ Complete practical examples

## Next Lesson Preview

In Lesson 05, we'll explore:
- C-style arrays and multidimensional arrays
- Array manipulation and algorithms
- C-style strings and string manipulation
- Character arrays and null terminators
- Common array operations: searching, sorting, reversing
- Working with 2D arrays (matrices)

---

**Congratulations on completing Lesson 04!**

**Practice time**: Spend 4+ hours creating your own function library. Try building a math library, string utility library, or game utility functions. Focus on writing reusable, well-documented functions.
