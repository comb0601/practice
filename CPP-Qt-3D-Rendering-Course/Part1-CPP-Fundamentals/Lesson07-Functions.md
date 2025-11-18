# Lesson 07: Functions and Function Overloading

**Duration: 3 hours**

## Table of Contents
1. Introduction to Functions
2. Function Declaration and Definition
3. Function Parameters
4. Return Values
5. Function Overloading
6. Default Parameters
7. Inline Functions
8. Function Prototypes
9. Recursive Functions
10. Practical Exercises

## 1. Introduction to Functions

Functions are reusable blocks of code that perform specific tasks. They help organize code, avoid repetition, and make programs more maintainable.

### Why Use Functions?

```cpp
#include <iostream>

// Without functions - repetitive code
int main() {
    // Calculate area of rectangle 1
    int width1 = 5, height1 = 10;
    int area1 = width1 * height1;
    std::cout << "Area 1: " << area1 << std::endl;

    // Calculate area of rectangle 2 - same code repeated!
    int width2 = 7, height2 = 3;
    int area2 = width2 * height2;
    std::cout << "Area 2: " << area2 << std::endl;

    return 0;
}
```

```cpp
#include <iostream>

// With functions - reusable code
int calculateArea(int width, int height) {
    return width * height;
}

int main() {
    int area1 = calculateArea(5, 10);
    int area2 = calculateArea(7, 3);

    std::cout << "Area 1: " << area1 << std::endl;
    std::cout << "Area 2: " << area2 << std::endl;

    return 0;
}
```

## 2. Function Declaration and Definition

### Function Syntax

```cpp
return_type function_name(parameter_list) {
    // function body
    // statements
    return value;  // if return_type is not void
}
```

### Simple Function Example

```cpp
#include <iostream>

// Function definition
void greet() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    greet();  // Function call
    greet();  // Can call multiple times
    return 0;
}
```

### Function with Return Value

```cpp
#include <iostream>

int add(int a, int b) {
    int sum = a + b;
    return sum;
}

int main() {
    int result = add(5, 3);
    std::cout << "5 + 3 = " << result << std::endl;

    // Can use directly in expression
    std::cout << "10 + 20 = " << add(10, 20) << std::endl;

    return 0;
}
```

## 3. Function Parameters

### Pass by Value

```cpp
#include <iostream>

void increment(int x) {
    x = x + 1;
    std::cout << "Inside function: " << x << std::endl;
}

int main() {
    int num = 10;
    std::cout << "Before: " << num << std::endl;
    increment(num);
    std::cout << "After: " << num << std::endl;  // Still 10!

    return 0;
}
```

### Pass by Reference

```cpp
#include <iostream>

void increment(int& x) {  // Note the &
    x = x + 1;
    std::cout << "Inside function: " << x << std::endl;
}

int main() {
    int num = 10;
    std::cout << "Before: " << num << std::endl;
    increment(num);
    std::cout << "After: " << num << std::endl;  // Now 11!

    return 0;
}
```

### Pass by Const Reference

```cpp
#include <iostream>
#include <string>

// Efficient for large objects, prevents modification
void printMessage(const std::string& message) {
    std::cout << message << std::endl;
    // message = "Changed";  // ERROR: cannot modify const reference
}

int main() {
    std::string msg = "Hello, Functions!";
    printMessage(msg);
    return 0;
}
```

### Multiple Parameters

```cpp
#include <iostream>

double calculateRectangleArea(double width, double height) {
    return width * height;
}

double calculateCircleArea(double radius) {
    const double PI = 3.14159;
    return PI * radius * radius;
}

void displayInfo(std::string name, int age, double height) {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << " cm" << std::endl;
}

int main() {
    double rectArea = calculateRectangleArea(5.0, 10.0);
    double circleArea = calculateCircleArea(7.0);

    std::cout << "Rectangle area: " << rectArea << std::endl;
    std::cout << "Circle area: " << circleArea << std::endl;

    displayInfo("John Doe", 25, 175.5);

    return 0;
}
```

## 4. Return Values

### Returning Different Types

```cpp
#include <iostream>
#include <string>

int getInteger() {
    return 42;
}

double getDouble() {
    return 3.14159;
}

bool isEven(int num) {
    return num % 2 == 0;
}

std::string getGreeting() {
    return "Hello!";
}

int main() {
    std::cout << "Integer: " << getInteger() << std::endl;
    std::cout << "Double: " << getDouble() << std::endl;
    std::cout << "Is 10 even? " << std::boolalpha << isEven(10) << std::endl;
    std::cout << "Greeting: " << getGreeting() << std::endl;

    return 0;
}
```

### Void Functions (No Return)

```cpp
#include <iostream>

void printLine() {
    std::cout << "========================" << std::endl;
}

void printHeader(std::string title) {
    printLine();
    std::cout << "   " << title << std::endl;
    printLine();
}

int main() {
    printHeader("My Application");
    std::cout << "Content goes here..." << std::endl;
    printLine();

    return 0;
}
```

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

void processAge(int age) {
    if (age < 0) {
        std::cout << "Invalid age!" << std::endl;
        return;  // Early return from void function
    }

    std::cout << "Processing age: " << age << std::endl;
}

int main() {
    std::cout << "10 / 2 = " << divide(10, 2) << std::endl;
    std::cout << "10 / 0 = " << divide(10, 0) << std::endl;

    processAge(25);
    processAge(-5);

    return 0;
}
```

## 5. Function Overloading

### Same Name, Different Parameters

```cpp
#include <iostream>

// Overloaded functions - same name, different parameters
int add(int a, int b) {
    std::cout << "Adding two integers" << std::endl;
    return a + b;
}

double add(double a, double b) {
    std::cout << "Adding two doubles" << std::endl;
    return a + b;
}

int add(int a, int b, int c) {
    std::cout << "Adding three integers" << std::endl;
    return a + b + c;
}

int main() {
    std::cout << add(5, 3) << std::endl;           // Calls int version
    std::cout << add(5.5, 3.2) << std::endl;       // Calls double version
    std::cout << add(1, 2, 3) << std::endl;        // Calls three-parameter version

    return 0;
}
```

### Overloading with Different Types

```cpp
#include <iostream>
#include <string>

void display(int value) {
    std::cout << "Integer: " << value << std::endl;
}

void display(double value) {
    std::cout << "Double: " << value << std::endl;
}

void display(std::string value) {
    std::cout << "String: " << value << std::endl;
}

void display(char value) {
    std::cout << "Character: " << value << std::endl;
}

int main() {
    display(42);
    display(3.14);
    display("Hello");
    display('A');

    return 0;
}
```

### Practical Overloading Example

```cpp
#include <iostream>
#include <cmath>

// Calculate area - overloaded for different shapes
double area(double radius) {  // Circle
    return 3.14159 * radius * radius;
}

double area(double width, double height) {  // Rectangle
    return width * height;
}

double area(double base, double height, bool isTriangle) {  // Triangle
    if (isTriangle) {
        return 0.5 * base * height;
    }
    return 0.0;
}

int main() {
    std::cout << "Circle area (r=5): " << area(5.0) << std::endl;
    std::cout << "Rectangle area (5x10): " << area(5.0, 10.0) << std::endl;
    std::cout << "Triangle area (b=4, h=6): " << area(4.0, 6.0, true) << std::endl;

    return 0;
}
```

## 6. Default Parameters

### Basic Default Parameters

```cpp
#include <iostream>

void greet(std::string name = "Guest", std::string greeting = "Hello") {
    std::cout << greeting << ", " << name << "!" << std::endl;
}

int main() {
    greet();                          // Uses all defaults
    greet("Alice");                   // Uses default greeting
    greet("Bob", "Welcome");          // Uses no defaults

    return 0;
}
```

### Default Parameters Rules

```cpp
#include <iostream>

// Correct: Default parameters at the end
void func1(int a, int b = 10, int c = 20) {
    std::cout << a << ", " << b << ", " << c << std::endl;
}

// ERROR: Cannot have non-default after default
// void func2(int a = 10, int b, int c = 20) { }

int main() {
    func1(1);           // 1, 10, 20
    func1(1, 2);        // 1, 2, 20
    func1(1, 2, 3);     // 1, 2, 3

    return 0;
}
```

### Practical Default Parameters

```cpp
#include <iostream>
#include <iomanip>

void printPrice(double price, std::string currency = "USD", int decimals = 2) {
    std::cout << std::fixed << std::setprecision(decimals);
    std::cout << currency << " " << price << std::endl;
}

void createUser(std::string username,
                std::string email = "not_provided@example.com",
                bool isActive = true) {
    std::cout << "Creating user:" << std::endl;
    std::cout << "  Username: " << username << std::endl;
    std::cout << "  Email: " << email << std::endl;
    std::cout << "  Active: " << (isActive ? "Yes" : "No") << std::endl;
}

int main() {
    printPrice(19.99);
    printPrice(29.99, "EUR");
    printPrice(39.99, "GBP", 3);

    std::cout << std::endl;

    createUser("john_doe");
    createUser("jane_smith", "jane@example.com");
    createUser("admin", "admin@example.com", false);

    return 0;
}
```

## 7. Inline Functions

### Inline Keyword

```cpp
#include <iostream>

// Inline function - suggests to compiler to insert code directly
inline int square(int x) {
    return x * x;
}

inline double max(double a, double b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << "Square of 5: " << square(5) << std::endl;
    std::cout << "Max of 3.5 and 7.2: " << max(3.5, 7.2) << std::endl;

    return 0;
}
```

## 8. Function Prototypes

### Declaration vs Definition

```cpp
#include <iostream>

// Function prototypes (declarations)
int add(int a, int b);
void printResult(int result);
double calculateAverage(int arr[], int size);

int main() {
    int sum = add(10, 20);
    printResult(sum);

    int numbers[] = {10, 20, 30, 40, 50};
    double avg = calculateAverage(numbers, 5);
    std::cout << "Average: " << avg << std::endl;

    return 0;
}

// Function definitions (implementations)
int add(int a, int b) {
    return a + b;
}

void printResult(int result) {
    std::cout << "Result: " << result << std::endl;
}

double calculateAverage(int arr[], int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}
```

## 9. Recursive Functions

### Basic Recursion

```cpp
#include <iostream>

// Factorial using recursion
int factorial(int n) {
    if (n <= 1) {
        return 1;  // Base case
    }
    return n * factorial(n - 1);  // Recursive case
}

int main() {
    std::cout << "5! = " << factorial(5) << std::endl;     // 120
    std::cout << "10! = " << factorial(10) << std::endl;   // 3628800

    return 0;
}
```

### Fibonacci Sequence

```cpp
#include <iostream>

int fibonacci(int n) {
    if (n <= 1) {
        return n;  // Base cases: fib(0) = 0, fib(1) = 1
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // Recursive case
}

int main() {
    std::cout << "Fibonacci sequence (first 10 numbers):" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << fibonacci(i) << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 10. Practical Exercises

### Exercise 1: Temperature Converter

```cpp
#include <iostream>
#include <iomanip>

double celsiusToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    double temp = 100.0;

    std::cout << temp << "°C = " << celsiusToFahrenheit(temp) << "°F" << std::endl;
    std::cout << temp << "°F = " << fahrenheitToCelsius(temp) << "°C" << std::endl;
    std::cout << temp << "°C = " << celsiusToKelvin(temp) << "K" << std::endl;
    std::cout << temp << "K = " << kelvinToCelsius(temp) << "°C" << std::endl;

    return 0;
}
```

### Exercise 2: String Utilities

```cpp
#include <iostream>
#include <string>
#include <algorithm>

// Count vowels in a string
int countVowels(const std::string& str) {
    int count = 0;
    std::string vowels = "aeiouAEIOU";

    for (char c : str) {
        if (vowels.find(c) != std::string::npos) {
            count++;
        }
    }

    return count;
}

// Reverse a string
std::string reverseString(std::string str) {
    std::reverse(str.begin(), str.end());
    return str;
}

// Check if palindrome
bool isPalindrome(const std::string& str) {
    int left = 0;
    int right = str.length() - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

int main() {
    std::string text = "Hello World";

    std::cout << "Text: " << text << std::endl;
    std::cout << "Vowels: " << countVowels(text) << std::endl;
    std::cout << "Reversed: " << reverseString(text) << std::endl;
    std::cout << "Is palindrome: " << std::boolalpha << isPalindrome(text) << std::endl;

    std::string palindrome = "racecar";
    std::cout << "\nText: " << palindrome << std::endl;
    std::cout << "Is palindrome: " << isPalindrome(palindrome) << std::endl;

    return 0;
}
```

### Exercise 3: Math Functions

```cpp
#include <iostream>
#include <cmath>

bool isPrime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void printPrimes(int n) {
    std::cout << "Prime numbers up to " << n << ": ";
    for (int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Is 17 prime? " << std::boolalpha << isPrime(17) << std::endl;
    std::cout << "Is 20 prime? " << isPrime(20) << std::endl;

    std::cout << "GCD(48, 18) = " << gcd(48, 18) << std::endl;
    std::cout << "LCM(12, 15) = " << lcm(12, 15) << std::endl;

    printPrimes(50);

    return 0;
}
```

## Complete Project: Calculator with Functions

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>

// Basic operations
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
        return 0;
    }
    return a / b;
}

// Advanced operations
double power(double base, double exponent) {
    return std::pow(base, exponent);
}

double squareRoot(double num) {
    if (num < 0) {
        std::cout << "Error: Cannot calculate square root of negative number!" << std::endl;
        return 0;
    }
    return std::sqrt(num);
}

double percentage(double value, double percent) {
    return (value * percent) / 100.0;
}

// Display menu
void displayMenu() {
    std::cout << "\n=== Scientific Calculator ===" << std::endl;
    std::cout << "1. Addition" << std::endl;
    std::cout << "2. Subtraction" << std::endl;
    std::cout << "3. Multiplication" << std::endl;
    std::cout << "4. Division" << std::endl;
    std::cout << "5. Power" << std::endl;
    std::cout << "6. Square Root" << std::endl;
    std::cout << "7. Percentage" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "Choice: ";
}

// Get two numbers from user
void getTwoNumbers(double& a, double& b) {
    std::cout << "Enter first number: ";
    std::cin >> a;
    std::cout << "Enter second number: ";
    std::cin >> b;
}

// Get one number from user
void getOneNumber(double& a) {
    std::cout << "Enter number: ";
    std::cin >> a;
}

// Display result
void displayResult(double result) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Result: " << result << std::endl;
}

int main() {
    int choice;
    double num1, num2, result;

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                getTwoNumbers(num1, num2);
                result = add(num1, num2);
                displayResult(result);
                break;

            case 2:
                getTwoNumbers(num1, num2);
                result = subtract(num1, num2);
                displayResult(result);
                break;

            case 3:
                getTwoNumbers(num1, num2);
                result = multiply(num1, num2);
                displayResult(result);
                break;

            case 4:
                getTwoNumbers(num1, num2);
                result = divide(num1, num2);
                displayResult(result);
                break;

            case 5:
                getTwoNumbers(num1, num2);
                result = power(num1, num2);
                displayResult(result);
                break;

            case 6:
                getOneNumber(num1);
                result = squareRoot(num1);
                displayResult(result);
                break;

            case 7:
                std::cout << "Enter value: ";
                std::cin >> num1;
                std::cout << "Enter percentage: ";
                std::cin >> num2;
                result = percentage(num1, num2);
                std::cout << num2 << "% of " << num1 << " = ";
                displayResult(result);
                break;

            case 8:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }

    } while (choice != 8);

    return 0;
}
```

## Summary

In this lesson, you learned:
- ✅ How to create and use functions
- ✅ Function parameters (pass by value, reference, const reference)
- ✅ Return values and void functions
- ✅ Function overloading
- ✅ Default parameters
- ✅ Inline functions
- ✅ Function prototypes
- ✅ Recursive functions
- ✅ Practical applications with complete projects

## Key Takeaways

1. **Functions organize code** - Make programs modular and maintainable
2. **Pass by reference** - Modify original values
3. **Pass by const reference** - Efficient for large objects
4. **Overloading** - Same name, different parameters
5. **Recursion** - Functions calling themselves (needs base case!)

## Next Lesson

Lesson 08: Arrays and Multidimensional Arrays

## Practice Problems

1. Write a function that checks if a year is a leap year
2. Create functions to find min and max of three numbers
3. Implement a function that reverses an integer (e.g., 123 → 321)
4. Write recursive and iterative functions to calculate sum of digits
5. Create a menu-driven program using functions for different operations
