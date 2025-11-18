# Lesson 02: Variables, Data Types, and Operators
**Estimated Time: 4 hours**

## Table of Contents
1. [Introduction to Variables](#introduction-to-variables)
2. [Fundamental Data Types](#fundamental-data-types)
3. [Variable Declaration and Initialization](#variable-declaration-and-initialization)
4. [Constants](#constants)
5. [Operators](#operators)
6. [Type Conversion](#type-conversion)
7. [Input with cin](#input-with-cin)
8. [Complete Examples](#complete-examples)
9. [Exercises](#exercises)

## Introduction to Variables

### What is a Variable?
A variable is a named storage location in memory that holds a value. Think of it as a labeled box where you can store data.

```cpp
int age;        // Declare a variable named 'age'
age = 25;       // Store value 25 in the variable
```

### Variable Naming Rules:
1. Must start with letter or underscore: `myVar`, `_count`
2. Can contain letters, digits, underscores: `var1`, `my_variable`
3. Case-sensitive: `age` ≠ `Age` ≠ `AGE`
4. Cannot use keywords: `int`, `return`, `class`, etc.
5. No spaces: Use `myAge` not `my age`

### Good Naming Conventions:
```cpp
// Good names - descriptive and clear
int playerScore;
int numberOfLives;
double accountBalance;

// Bad names - unclear
int x;
int abc;
int n;
```

## Fundamental Data Types

### Integer Types

#### int (Integer)
- Stores whole numbers
- Typically 4 bytes (32 bits)
- Range: -2,147,483,648 to 2,147,483,647

```cpp
int age = 25;
int temperature = -10;
int population = 1000000;
```

#### short (Short Integer)
- Smaller range than int
- Typically 2 bytes (16 bits)
- Range: -32,768 to 32,767

```cpp
short year = 2024;
short dayOfMonth = 15;
```

#### long (Long Integer)
- Typically same as int on 32-bit systems
- 4 bytes minimum
- Range: at least ±2 billion

```cpp
long distance = 150000000L;  // L suffix for long literal
```

#### long long
- Guaranteed 8 bytes (64 bits)
- Range: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

```cpp
long long bigNumber = 9000000000000LL;  // LL suffix
```

#### unsigned variants
- Only positive numbers (and zero)
- Doubles the positive range

```cpp
unsigned int population = 4000000000U;  // U suffix for unsigned
unsigned short port = 8080;
unsigned long long fileSize = 10000000000ULL;
```

### Floating-Point Types

#### float (Single Precision)
- 4 bytes
- ~7 decimal digits precision
- Suffix: `f` or `F`

```cpp
float pi = 3.14159f;
float temperature = 98.6f;
```

#### double (Double Precision)
- 8 bytes
- ~15 decimal digits precision
- Default for decimal literals

```cpp
double pi = 3.14159265358979;
double scientificNumber = 1.23e-10;  // Scientific notation
```

#### long double
- Extended precision
- Typically 8, 12, or 16 bytes (platform-dependent)

```cpp
long double preciseValue = 3.141592653589793238L;
```

### Character Type

#### char
- 1 byte
- Stores single character
- Uses ASCII codes

```cpp
char grade = 'A';
char symbol = '$';
char newline = '\n';
```

### Boolean Type

#### bool
- Stores true or false
- Typically 1 byte

```cpp
bool isGameOver = false;
bool hasWon = true;
bool isAlive = 1;    // Non-zero is true
bool isDead = 0;      // Zero is false
```

### Size Comparison Table

| Type | Typical Size | Range |
|------|-------------|-------|
| bool | 1 byte | true or false |
| char | 1 byte | -128 to 127 |
| unsigned char | 1 byte | 0 to 255 |
| short | 2 bytes | -32,768 to 32,767 |
| unsigned short | 2 bytes | 0 to 65,535 |
| int | 4 bytes | -2.1B to 2.1B |
| unsigned int | 4 bytes | 0 to 4.2B |
| long | 4/8 bytes | Platform dependent |
| long long | 8 bytes | -9.2Q to 9.2Q |
| float | 4 bytes | ±3.4e38 (7 digits) |
| double | 8 bytes | ±1.7e308 (15 digits) |

## Variable Declaration and Initialization

### Declaration Only
```cpp
int x;              // Declared but not initialized (contains garbage)
double y;           // Dangerous! Has random value
```

### Declaration with Initialization
```cpp
int x = 10;         // C-style initialization
double y = 3.14;    // Recommended for simple types
char c = 'A';
```

### Modern C++ Initialization (Uniform Initialization)
```cpp
int x{10};          // Brace initialization (C++11)
double y{3.14};     // Prevents narrowing conversions
char c{'A'};

// Prevents implicit narrowing
int a{3.14};        // ERROR: Cannot convert double to int
```

### Multiple Declarations
```cpp
int x = 1, y = 2, z = 3;              // Multiple variables
int a, b, c;                           // All uninitialized
int p = 0, q = 0, r = 0;              // All initialized
```

### Auto Type Deduction (C++11)
```cpp
auto x = 10;        // x is int
auto y = 3.14;      // y is double
auto c = 'A';       // c is char
auto b = true;      // b is bool
```

## Constants

### const Keyword
```cpp
const int MAX_PLAYERS = 4;
const double PI = 3.14159265359;
const char GRADE_A = 'A';

// Attempting to modify causes error
MAX_PLAYERS = 5;    // ERROR: Cannot modify const
```

### constexpr (Compile-Time Constants)
```cpp
constexpr int ARRAY_SIZE = 100;
constexpr double RATIO = 3.14159 * 2;
```

### #define (Preprocessor Constant)
```cpp
#define MAX_SIZE 1000
#define PI 3.14159

// Not recommended in modern C++ (use const instead)
```

## Operators

### Arithmetic Operators

```cpp
int a = 10, b = 3;

int sum = a + b;         // Addition: 13
int diff = a - b;        // Subtraction: 7
int product = a * b;     // Multiplication: 30
int quotient = a / b;    // Division: 3 (integer division!)
int remainder = a % b;   // Modulus: 1

// Integer division truncates
int x = 10 / 3;          // Result: 3 (not 3.333...)
double y = 10.0 / 3.0;   // Result: 3.333...
```

### Assignment Operators

```cpp
int x = 10;

x += 5;     // x = x + 5;  → x is now 15
x -= 3;     // x = x - 3;  → x is now 12
x *= 2;     // x = x * 2;  → x is now 24
x /= 4;     // x = x / 4;  → x is now 6
x %= 4;     // x = x % 4;  → x is now 2
```

### Increment and Decrement

```cpp
int x = 5;

// Prefix increment
++x;        // Increment first, then use: x becomes 6
int a = ++x;// a = 7, x = 7

// Postfix increment
x++;        // Use first, then increment
int b = x++;// b = 7, x = 8

// Decrement works similarly
--x;        // Prefix decrement
x--;        // Postfix decrement
```

### Comparison Operators

```cpp
int a = 10, b = 20;

bool result;
result = (a == b);   // Equal to: false
result = (a != b);   // Not equal: true
result = (a < b);    // Less than: true
result = (a > b);    // Greater than: false
result = (a <= b);   // Less than or equal: true
result = (a >= b);   // Greater than or equal: false
```

### Logical Operators

```cpp
bool a = true, b = false;

bool result;
result = a && b;     // AND: false (both must be true)
result = a || b;     // OR: true (at least one must be true)
result = !a;         // NOT: false (inverts the value)

// Combining conditions
int age = 25;
bool canVote = (age >= 18) && (age <= 120);
```

### Operator Precedence (High to Low)

1. `()` - Parentheses
2. `++`, `--`, `!` - Unary operators
3. `*`, `/`, `%` - Multiplicative
4. `+`, `-` - Additive
5. `<`, `<=`, `>`, `>=` - Relational
6. `==`, `!=` - Equality
7. `&&` - Logical AND
8. `||` - Logical OR
9. `=`, `+=`, `-=`, etc. - Assignment

```cpp
int result = 2 + 3 * 4;        // 14, not 20 (* before +)
int result2 = (2 + 3) * 4;     // 20 (parentheses first)
```

## Type Conversion

### Implicit Conversion (Automatic)

```cpp
int x = 10;
double y = x;        // int → double: OK, no data loss

double a = 3.14;
int b = a;           // double → int: WARNING, loses decimal (b = 3)
```

### Explicit Conversion (Casting)

#### C-Style Cast
```cpp
double pi = 3.14159;
int x = (int)pi;     // x = 3
```

#### C++ Style Cast (Recommended)
```cpp
double pi = 3.14159;
int x = static_cast<int>(pi);  // x = 3

// More explicit and safer
```

### Conversion Examples

```cpp
// Integer division problem
int a = 5, b = 2;
double result = a / b;                    // 2.0 (integer division first!)

// Solution 1: Cast to double
double result2 = static_cast<double>(a) / b;  // 2.5

// Solution 2: Use double literals
double result3 = 5.0 / 2.0;              // 2.5
```

## Input with cin

### Basic Input

```cpp
#include <iostream>

int main()
{
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cout << "You are " << age << " years old.\n";

    return 0;
}
```

### Multiple Inputs

```cpp
int x, y;
std::cout << "Enter two numbers: ";
std::cin >> x >> y;
std::cout << "You entered: " << x << " and " << y << "\n";
```

### Different Data Types

```cpp
#include <iostream>

int main()
{
    int age;
    double height;
    char grade;

    std::cout << "Enter age: ";
    std::cin >> age;

    std::cout << "Enter height (in meters): ";
    std::cin >> height;

    std::cout << "Enter grade: ";
    std::cin >> grade;

    std::cout << "\n--- Summary ---\n";
    std::cout << "Age: " << age << "\n";
    std::cout << "Height: " << height << "m\n";
    std::cout << "Grade: " << grade << "\n";

    return 0;
}
```

## Complete Examples

### Example 1: Calculator

```cpp
#include <iostream>

int main()
{
    double num1, num2;

    std::cout << "Simple Calculator\n";
    std::cout << "=================\n";

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << "\nResults:\n";
    std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
    std::cout << num1 << " - " << num2 << " = " << (num1 - num2) << "\n";
    std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << "\n";
    std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << "\n";

    return 0;
}
```

### Example 2: Temperature Converter

```cpp
#include <iostream>

int main()
{
    double celsius;

    std::cout << "Celsius to Fahrenheit Converter\n";
    std::cout << "================================\n";

    std::cout << "Enter temperature in Celsius: ";
    std::cin >> celsius;

    double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

    std::cout << celsius << "°C = " << fahrenheit << "°F\n";

    return 0;
}
```

### Example 3: Circle Calculator

```cpp
#include <iostream>

int main()
{
    const double PI = 3.14159265359;
    double radius;

    std::cout << "Circle Calculator\n";
    std::cout << "=================\n";

    std::cout << "Enter radius: ";
    std::cin >> radius;

    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    std::cout << "\nResults:\n";
    std::cout << "Radius: " << radius << "\n";
    std::cout << "Circumference: " << circumference << "\n";
    std::cout << "Area: " << area << "\n";

    return 0;
}
```

### Example 4: Age Calculator

```cpp
#include <iostream>

int main()
{
    const int CURRENT_YEAR = 2024;
    int birthYear;

    std::cout << "Age Calculator\n";
    std::cout << "==============\n";

    std::cout << "Enter your birth year: ";
    std::cin >> birthYear;

    int age = CURRENT_YEAR - birthYear;
    int ageIn10Years = age + 10;
    int ageIn20Years = age + 20;

    std::cout << "\nYou are approximately " << age << " years old.\n";
    std::cout << "In 10 years, you'll be " << ageIn10Years << ".\n";
    std::cout << "In 20 years, you'll be " << ageIn20Years << ".\n";

    return 0;
}
```

### Example 5: Arithmetic with All Operators

```cpp
#include <iostream>

int main()
{
    int a = 17, b = 5;

    std::cout << "Integer Arithmetic Demo\n";
    std::cout << "a = " << a << ", b = " << b << "\n\n";

    std::cout << "Addition: a + b = " << (a + b) << "\n";
    std::cout << "Subtraction: a - b = " << (a - b) << "\n";
    std::cout << "Multiplication: a * b = " << (a * b) << "\n";
    std::cout << "Division: a / b = " << (a / b) << "\n";
    std::cout << "Modulus: a % b = " << (a % b) << "\n\n";

    std::cout << "Floating-point division:\n";
    double result = static_cast<double>(a) / b;
    std::cout << "a / b = " << result << "\n\n";

    std::cout << "Comparison operators:\n";
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "a != b: " << (a != b) << "\n";
    std::cout << "a > b: " << (a > b) << "\n";
    std::cout << "a < b: " << (a < b) << "\n";
    std::cout << "a >= b: " << (a >= b) << "\n";
    std::cout << "a <= b: " << (a <= b) << "\n";

    return 0;
}
```

## Common Mistakes and Pitfalls

### 1. Integer Division

```cpp
// WRONG
int result = 5 / 2;          // result = 2 (not 2.5!)

// CORRECT
double result = 5.0 / 2.0;   // result = 2.5
```

### 2. Uninitialized Variables

```cpp
// WRONG
int x;
std::cout << x;              // Undefined behavior! Random value

// CORRECT
int x = 0;
std::cout << x;              // Prints 0
```

### 3. Overflow

```cpp
// WRONG
short x = 32767;
x = x + 1;                   // Overflow! Wraps to -32768

// CORRECT
int x = 32767;               // Use larger type
x = x + 1;                   // x = 32768 (no overflow)
```

### 4. Comparison with Floating-Point

```cpp
// WRONG
double x = 0.1 + 0.1 + 0.1;
if (x == 0.3)                // Might be false due to precision!
{
    // ...
}

// CORRECT (for now, better methods exist)
const double EPSILON = 0.000001;
if (std::abs(x - 0.3) < EPSILON)
{
    // ...
}
```

## Exercises

### Exercise 1: Rectangle Calculator
Write a program that:
1. Asks user for length and width of a rectangle
2. Calculates and displays area and perimeter
3. Uses constants for any fixed values

### Exercise 2: BMI Calculator
Create a BMI calculator that:
1. Gets weight (kg) and height (meters) from user
2. Calculates BMI = weight / (height * height)
3. Displays the result

### Exercise 3: Time Converter
Write a program that:
1. Asks for time in seconds
2. Converts to hours, minutes, and seconds
3. Displays result as: "X hours, Y minutes, Z seconds"

Hint: Use integer division and modulus

### Exercise 4: Discount Calculator
Create a program that:
1. Gets original price from user
2. Gets discount percentage
3. Calculates final price
4. Shows amount saved

### Exercise 5: Variable Swap
Write a program that:
1. Creates two variables: a = 10, b = 20
2. Swaps their values WITHOUT using a third variable
3. Prints the result

Hint: Use arithmetic operations

## Summary

In this lesson, you learned:
- ✅ What variables are and how to name them
- ✅ Fundamental data types (int, double, char, bool)
- ✅ How to declare and initialize variables
- ✅ Constants with const and constexpr
- ✅ Arithmetic, comparison, and logical operators
- ✅ Type conversion (implicit and explicit)
- ✅ Getting user input with cin
- ✅ Common mistakes to avoid

## Next Lesson Preview

In Lesson 03, we'll explore:
- if statements
- else and else if
- switch statements
- Loops (while, for, do-while)
- Nested control structures

## Key Takeaways

1. **Always initialize variables** before using them
2. **Use const** for values that shouldn't change
3. **Be careful with integer division** - it truncates
4. **Choose appropriate data types** based on range and precision needs
5. **Use meaningful variable names** for code readability

---

**Congratulations on completing Lesson 02!**

**Practice time**: Spend 2 hours creating different calculator programs to solidify your understanding of variables and operators.
