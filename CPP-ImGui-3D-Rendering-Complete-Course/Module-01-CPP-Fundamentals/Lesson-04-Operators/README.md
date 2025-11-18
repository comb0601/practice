# Lesson 04: Operators and Expressions
**Duration: 2.5 hours**

## Learning Objectives
By the end of this lesson, you will:
- Understand and use arithmetic operators
- Work with comparison and logical operators
- Master assignment operators
- Use increment and decrement operators
- Understand operator precedence
- Work with bitwise operators
- Use the ternary operator

## 1. Arithmetic Operators

### Basic Arithmetic

```cpp
#include <iostream>

int main() {
    int a = 10, b = 3;

    // Addition
    std::cout << a << " + " << b << " = " << (a + b) << std::endl;  // 13

    // Subtraction
    std::cout << a << " - " << b << " = " << (a - b) << std::endl;  // 7

    // Multiplication
    std::cout << a << " * " << b << " = " << (a * b) << std::endl;  // 30

    // Division
    std::cout << a << " / " << b << " = " << (a / b) << std::endl;  // 3 (integer division)

    // Modulus (remainder)
    std::cout << a << " % " << b << " = " << (a % b) << std::endl;  // 1

    return 0;
}
```

### Integer vs Floating-Point Division

```cpp
#include <iostream>

int main() {
    // Integer division
    int x = 7, y = 2;
    std::cout << "Integer division: " << x << " / " << y << " = " << (x / y) << std::endl;  // 3

    // Floating-point division
    double dx = 7.0, dy = 2.0;
    std::cout << "Float division: " << dx << " / " << dy << " = " << (dx / dy) << std::endl;  // 3.5

    // Mixed types
    std::cout << "Mixed: " << x << " / " << dy << " = " << (x / dy) << std::endl;  // 3.5

    // Explicit casting
    std::cout << "Cast: " << x << " / " << y << " = "
              << (static_cast<double>(x) / y) << std::endl;  // 3.5

    return 0;
}
```

### Modulus Operator Details

```cpp
#include <iostream>

int main() {
    // Modulus gives the remainder
    std::cout << "10 % 3 = " << (10 % 3) << std::endl;  // 1
    std::cout << "15 % 4 = " << (15 % 4) << std::endl;  // 3
    std::cout << "20 % 5 = " << (20 % 5) << std::endl;  // 0

    // Checking even/odd
    int num = 17;
    if (num % 2 == 0) {
        std::cout << num << " is even" << std::endl;
    } else {
        std::cout << num << " is odd" << std::endl;
    }

    // Negative numbers
    std::cout << "-10 % 3 = " << (-10 % 3) << std::endl;    // -1
    std::cout << "10 % -3 = " << (10 % -3) << std::endl;    // 1
    std::cout << "-10 % -3 = " << (-10 % -3) << std::endl;  // -1

    return 0;
}
```

## 2. Comparison (Relational) Operators

### Basic Comparisons

```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;

    // Equal to
    std::cout << a << " == " << b << " : " << (a == b) << std::endl;  // 0 (false)

    // Not equal to
    std::cout << a << " != " << b << " : " << (a != b) << std::endl;  // 1 (true)

    // Less than
    std::cout << a << " < " << b << " : " << (a < b) << std::endl;    // 1 (true)

    // Greater than
    std::cout << a << " > " << b << " : " << (a > b) << std::endl;    // 0 (false)

    // Less than or equal
    std::cout << a << " <= " << b << " : " << (a <= b) << std::endl;  // 1 (true)

    // Greater than or equal
    std::cout << a << " >= " << b << " : " << (a >= b) << std::endl;  // 0 (false)

    return 0;
}
```

### Using boolalpha for Readable Output

```cpp
#include <iostream>

int main() {
    std::cout << std::boolalpha;  // Print true/false instead of 1/0

    int x = 5, y = 10;

    std::cout << "x == y: " << (x == y) << std::endl;  // false
    std::cout << "x != y: " << (x != y) << std::endl;  // true
    std::cout << "x < y: " << (x < y) << std::endl;    // true
    std::cout << "x > y: " << (x > y) << std::endl;    // false

    return 0;
}
```

## 3. Logical Operators

### AND, OR, NOT

```cpp
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    bool a = true, b = false;

    // Logical AND (&&) - both must be true
    std::cout << "true && true = " << (true && true) << std::endl;    // true
    std::cout << "true && false = " << (true && false) << std::endl;  // false
    std::cout << "false && false = " << (false && false) << std::endl;// false

    // Logical OR (||) - at least one must be true
    std::cout << "true || true = " << (true || true) << std::endl;    // true
    std::cout << "true || false = " << (true || false) << std::endl;  // true
    std::cout << "false || false = " << (false || false) << std::endl;// false

    // Logical NOT (!) - inverts the value
    std::cout << "!true = " << !true << std::endl;    // false
    std::cout << "!false = " << !false << std::endl;  // true

    return 0;
}
```

### Practical Example: Range Checking

```cpp
#include <iostream>

int main() {
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;

    // Check if age is in valid range (0-120)
    if (age >= 0 && age <= 120) {
        std::cout << "Valid age" << std::endl;

        // Check if adult
        if (age >= 18) {
            std::cout << "You are an adult" << std::endl;
        }
    } else {
        std::cout << "Invalid age" << std::endl;
    }

    // Check if teenager (13-19)
    if (age >= 13 && age <= 19) {
        std::cout << "You are a teenager" << std::endl;
    }

    return 0;
}
```

### Short-Circuit Evaluation

```cpp
#include <iostream>

int main() {
    int x = 5;

    // With &&, if first is false, second is never evaluated
    if (false && (++x > 0)) {
        // This code never runs
    }
    std::cout << "x after false &&: " << x << std::endl;  // 5 (not incremented)

    // With ||, if first is true, second is never evaluated
    if (true || (++x > 0)) {
        // This code runs, but x is not incremented
    }
    std::cout << "x after true ||: " << x << std::endl;  // 5 (not incremented)

    // Both parts evaluated
    if ((++x > 0) && true) {
        // x is incremented
    }
    std::cout << "x after full evaluation: " << x << std::endl;  // 6

    return 0;
}
```

## 4. Assignment Operators

### Basic Assignment

```cpp
#include <iostream>

int main() {
    int x;
    x = 10;  // Assignment
    std::cout << "x = " << x << std::endl;

    // Chained assignment (right to left)
    int a, b, c;
    a = b = c = 5;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;

    return 0;
}
```

### Compound Assignment Operators

```cpp
#include <iostream>

int main() {
    int x = 10;

    // Addition assignment
    x += 5;  // Same as: x = x + 5
    std::cout << "After x += 5: " << x << std::endl;  // 15

    // Subtraction assignment
    x -= 3;  // Same as: x = x - 3
    std::cout << "After x -= 3: " << x << std::endl;  // 12

    // Multiplication assignment
    x *= 2;  // Same as: x = x * 2
    std::cout << "After x *= 2: " << x << std::endl;  // 24

    // Division assignment
    x /= 4;  // Same as: x = x / 4
    std::cout << "After x /= 4: " << x << std::endl;  // 6

    // Modulus assignment
    x %= 4;  // Same as: x = x % 4
    std::cout << "After x %= 4: " << x << std::endl;  // 2

    return 0;
}
```

## 5. Increment and Decrement Operators

### Prefix vs Postfix

```cpp
#include <iostream>

int main() {
    int x = 5;

    // Prefix increment (++x) - increment first, then use
    std::cout << "x = " << x << std::endl;           // 5
    std::cout << "++x = " << ++x << std::endl;       // 6 (incremented, then used)
    std::cout << "x = " << x << std::endl;           // 6

    x = 5;  // Reset

    // Postfix increment (x++) - use first, then increment
    std::cout << "x = " << x << std::endl;           // 5
    std::cout << "x++ = " << x++ << std::endl;       // 5 (used, then incremented)
    std::cout << "x = " << x << std::endl;           // 6

    return 0;
}
```

### Detailed Example

```cpp
#include <iostream>

int main() {
    int a = 10, b = 10;

    // Prefix: increment, then use value
    int prefixResult = ++a;
    std::cout << "a after ++a: " << a << std::endl;               // 11
    std::cout << "prefixResult: " << prefixResult << std::endl;   // 11

    // Postfix: use value, then increment
    int postfixResult = b++;
    std::cout << "b after b++: " << b << std::endl;               // 11
    std::cout << "postfixResult: " << postfixResult << std::endl; // 10

    // In expressions
    int x = 5;
    int y = 2 * ++x;  // x becomes 6, then 2*6 = 12
    std::cout << "x = " << x << ", y = " << y << std::endl;  // x=6, y=12

    x = 5;
    y = 2 * x++;      // 2*5 = 10, then x becomes 6
    std::cout << "x = " << x << ", y = " << y << std::endl;  // x=6, y=10

    return 0;
}
```

### Decrement Operators

```cpp
#include <iostream>

int main() {
    int x = 10;

    // Prefix decrement
    std::cout << "--x = " << --x << std::endl;  // 9
    std::cout << "x = " << x << std::endl;      // 9

    // Postfix decrement
    std::cout << "x-- = " << x-- << std::endl;  // 9
    std::cout << "x = " << x << std::endl;      // 8

    return 0;
}
```

## 6. Operator Precedence

### Precedence Table

```cpp
/*
Operators (highest to lowest precedence):

1. () [] -> .                    // Parentheses, subscript, member access
2. ++ -- ! ~ + - * &             // Unary operators
3. * / %                         // Multiplicative
4. + -                           // Additive
5. << >>                         // Bitwise shift
6. < <= > >=                     // Relational
7. == !=                         // Equality
8. &                             // Bitwise AND
9. ^                             // Bitwise XOR
10. |                            // Bitwise OR
11. &&                           // Logical AND
12. ||                           // Logical OR
13. ?:                           // Ternary conditional
14. = += -= *= /= %=             // Assignment
*/

#include <iostream>

int main() {
    // Multiplication before addition
    int result = 2 + 3 * 4;
    std::cout << "2 + 3 * 4 = " << result << std::endl;  // 14, not 20

    // Use parentheses to override
    result = (2 + 3) * 4;
    std::cout << "(2 + 3) * 4 = " << result << std::endl;  // 20

    // Complex expression
    result = 10 + 5 * 2 - 3;
    std::cout << "10 + 5 * 2 - 3 = " << result << std::endl;  // 17

    // With parentheses for clarity
    result = 10 + (5 * 2) - 3;  // Same, but clearer
    std::cout << "10 + (5 * 2) - 3 = " << result << std::endl;  // 17

    return 0;
}
```

### Practical Examples

```cpp
#include <iostream>

int main() {
    int a = 5, b = 10, c = 15;

    // Without parentheses (precedence rules)
    bool result1 = a + b > c || c - b < a && b * 2 == c + a;
    // Breakdown:
    // 1. a + b = 15
    // 2. c - b = 5
    // 3. b * 2 = 20
    // 4. c + a = 20
    // 5. 15 > 15 = false
    // 6. 5 < 5 = false
    // 7. 20 == 20 = true
    // 8. false && true = false
    // 9. false || false = false
    std::cout << std::boolalpha << "Result1: " << result1 << std::endl;  // false

    // With parentheses (clearer)
    bool result2 = ((a + b) > c) || (((c - b) < a) && ((b * 2) == (c + a)));
    std::cout << "Result2: " << result2 << std::endl;  // false (same result, clearer)

    // When in doubt, use parentheses!
    return 0;
}
```

## 7. Bitwise Operators

### Basic Bitwise Operations

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int a = 12;  // Binary: 1100
    unsigned int b = 10;  // Binary: 1010

    // Display as binary
    std::cout << "a = " << std::bitset<8>(a) << " (" << a << ")" << std::endl;
    std::cout << "b = " << std::bitset<8>(b) << " (" << b << ")" << std::endl;
    std::cout << std::endl;

    // Bitwise AND (&)
    unsigned int andResult = a & b;  // 1100 & 1010 = 1000 (8)
    std::cout << "a & b  = " << std::bitset<8>(andResult) << " (" << andResult << ")" << std::endl;

    // Bitwise OR (|)
    unsigned int orResult = a | b;   // 1100 | 1010 = 1110 (14)
    std::cout << "a | b  = " << std::bitset<8>(orResult) << " (" << orResult << ")" << std::endl;

    // Bitwise XOR (^)
    unsigned int xorResult = a ^ b;  // 1100 ^ 1010 = 0110 (6)
    std::cout << "a ^ b  = " << std::bitset<8>(xorResult) << " (" << xorResult << ")" << std::endl;

    // Bitwise NOT (~)
    unsigned int notResult = ~a;     // ~1100 = ...11110011
    std::cout << "~a     = " << std::bitset<8>(notResult) << " (" << notResult << ")" << std::endl;

    return 0;
}
```

### Bit Shift Operators

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int x = 5;  // Binary: 0101

    std::cout << "x      = " << std::bitset<8>(x) << " (" << x << ")" << std::endl;

    // Left shift (<<) - multiply by 2^n
    unsigned int leftShift1 = x << 1;  // 0101 << 1 = 1010 (10)
    std::cout << "x << 1 = " << std::bitset<8>(leftShift1) << " (" << leftShift1 << ")" << std::endl;

    unsigned int leftShift2 = x << 2;  // 0101 << 2 = 10100 (20)
    std::cout << "x << 2 = " << std::bitset<8>(leftShift2) << " (" << leftShift2 << ")" << std::endl;

    // Right shift (>>) - divide by 2^n
    unsigned int rightShift1 = x >> 1;  // 0101 >> 1 = 0010 (2)
    std::cout << "x >> 1 = " << std::bitset<8>(rightShift1) << " (" << rightShift1 << ")" << std::endl;

    unsigned int rightShift2 = x >> 2;  // 0101 >> 2 = 0001 (1)
    std::cout << "x >> 2 = " << std::bitset<8>(rightShift2) << " (" << rightShift2 << ")" << std::endl;

    // Quick multiplication/division by powers of 2
    int num = 7;
    std::cout << "\nQuick multiplication:" << std::endl;
    std::cout << num << " * 2 = " << (num << 1) << std::endl;  // 14
    std::cout << num << " * 4 = " << (num << 2) << std::endl;  // 28
    std::cout << num << " * 8 = " << (num << 3) << std::endl;  // 56

    return 0;
}
```

### Practical Uses of Bitwise Operators

```cpp
#include <iostream>

int main() {
    // Flags/permissions (common in graphics programming!)
    const unsigned int READ = 1 << 0;    // 0001 (1)
    const unsigned int WRITE = 1 << 1;   // 0010 (2)
    const unsigned int EXECUTE = 1 << 2; // 0100 (4)

    // Combine permissions
    unsigned int permissions = READ | WRITE;
    std::cout << "Permissions: " << permissions << std::endl;  // 3

    // Check if has permission
    if (permissions & READ) {
        std::cout << "Has READ permission" << std::endl;
    }

    if (permissions & EXECUTE) {
        std::cout << "Has EXECUTE permission" << std::endl;
    } else {
        std::cout << "No EXECUTE permission" << std::endl;
    }

    // Add permission
    permissions |= EXECUTE;
    std::cout << "After adding EXECUTE: " << permissions << std::endl;  // 7

    // Remove permission
    permissions &= ~WRITE;
    std::cout << "After removing WRITE: " << permissions << std::endl;  // 5

    // Toggle permission
    permissions ^= READ;
    std::cout << "After toggling READ: " << permissions << std::endl;  // 4

    return 0;
}
```

## 8. Ternary (Conditional) Operator

### Syntax and Usage

```cpp
#include <iostream>

int main() {
    // Syntax: condition ? value_if_true : value_if_false

    int age = 20;
    std::string status = (age >= 18) ? "Adult" : "Minor";
    std::cout << "Status: " << status << std::endl;  // Adult

    // Equivalent if-else:
    std::string status2;
    if (age >= 18) {
        status2 = "Adult";
    } else {
        status2 = "Minor";
    }

    // Finding maximum
    int a = 10, b = 20;
    int max = (a > b) ? a : b;
    std::cout << "Max: " << max << std::endl;  // 20

    // Nested ternary (use sparingly!)
    int score = 85;
    std::string grade = (score >= 90) ? "A" :
                       (score >= 80) ? "B" :
                       (score >= 70) ? "C" :
                       (score >= 60) ? "D" : "F";
    std::cout << "Grade: " << grade << std::endl;  // B

    return 0;
}
```

## 9. Complete Practical Examples

### Example 1: Expression Calculator

```cpp
// expression_calc.cpp
#include <iostream>

int main() {
    double num1, num2, result;
    char op;

    std::cout << "=== Expression Calculator ===" << std::endl;
    std::cout << "Enter expression (e.g., 10 + 5): ";
    std::cin >> num1 >> op >> num2;

    switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            result = (num2 != 0) ? (num1 / num2) : 0;
            if (num2 == 0) {
                std::cout << "Error: Division by zero!" << std::endl;
                return 1;
            }
            break;
        case '%':
            result = static_cast<int>(num1) % static_cast<int>(num2);
            break;
        default:
            std::cout << "Error: Unknown operator!" << std::endl;
            return 1;
    }

    std::cout << num1 << " " << op << " " << num2 << " = " << result << std::endl;

    return 0;
}
```

### Example 2: Bitwise Flag Manager

```cpp
// flag_manager.cpp
#include <iostream>
#include <bitset>

int main() {
    // Graphics rendering flags (preview of future lessons!)
    const unsigned int VSYNC = 1 << 0;        // 0001
    const unsigned int FULLSCREEN = 1 << 1;   // 0010
    const unsigned int MSAA = 1 << 2;         // 0100
    const unsigned int HDR = 1 << 3;          // 1000

    unsigned int renderFlags = 0;

    std::cout << "=== Graphics Settings Manager ===" << std::endl;

    // Enable features
    renderFlags |= VSYNC;
    renderFlags |= FULLSCREEN;

    std::cout << "Flags: " << std::bitset<8>(renderFlags) << std::endl;
    std::cout << "VSync: " << ((renderFlags & VSYNC) ? "ON" : "OFF") << std::endl;
    std::cout << "Fullscreen: " << ((renderFlags & FULLSCREEN) ? "ON" : "OFF") << std::endl;
    std::cout << "MSAA: " << ((renderFlags & MSAA) ? "ON" : "OFF") << std::endl;
    std::cout << "HDR: " << ((renderFlags & HDR) ? "ON" : "OFF") << std::endl;

    // Toggle MSAA
    renderFlags ^= MSAA;
    std::cout << "\nAfter toggling MSAA:" << std::endl;
    std::cout << "MSAA: " << ((renderFlags & MSAA) ? "ON" : "OFF") << std::endl;

    return 0;
}
```

### Example 3: Operator Precedence Demo

```cpp
// precedence_demo.cpp
#include <iostream>

int main() {
    std::cout << "=== Operator Precedence Examples ===" << std::endl;

    int a = 5, b = 10, c = 15;

    // Example 1: Arithmetic precedence
    int result1 = a + b * c;
    std::cout << "a + b * c = " << result1 << std::endl;  // 155, not 225

    int result2 = (a + b) * c;
    std::cout << "(a + b) * c = " << result2 << std::endl;  // 225

    // Example 2: Mixed comparison and logical
    bool test1 = a < b && b < c;
    std::cout << std::boolalpha;
    std::cout << "a < b && b < c: " << test1 << std::endl;  // true

    // Example 3: Assignment and arithmetic
    int x = 0;
    x += 5 * 2;  // Same as: x = x + (5 * 2)
    std::cout << "x after x += 5 * 2: " << x << std::endl;  // 10

    return 0;
}
```

## 10. Exercises

### Exercise 1: Temperature Formula
Calculate: F = (9/5) * C + 32
Ask for Celsius, display Fahrenheit (watch for integer division!)

### Exercise 2: Even/Odd Checker
Use modulus operator to check if a number is even or odd.

### Exercise 3: Swap Without Temp
Swap two numbers using only arithmetic operators (no temporary variable).

### Exercise 4: Bitwise Practice
Write a program that sets, clears, and toggles specific bits in a number.

### Exercise 5: Grade Calculator
Use ternary operator to assign letter grades based on numeric scores.

## 11. Exercise Solutions

### Solution 1: Temperature Formula
```cpp
#include <iostream>

int main() {
    double celsius;
    std::cout << "Enter temperature in Celsius: ";
    std::cin >> celsius;

    // Use 9.0 and 5.0 to force floating-point division
    double fahrenheit = (9.0 / 5.0) * celsius + 32.0;

    std::cout << celsius << "°C = " << fahrenheit << "°F" << std::endl;

    return 0;
}
```

### Solution 2: Even/Odd Checker
```cpp
#include <iostream>

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number % 2 == 0) {
        std::cout << number << " is even" << std::endl;
    } else {
        std::cout << number << " is odd" << std::endl;
    }

    // Or using ternary:
    std::string result = (number % 2 == 0) ? "even" : "odd";
    std::cout << number << " is " << result << std::endl;

    return 0;
}
```

### Solution 3: Swap Without Temp
```cpp
#include <iostream>

int main() {
    int a, b;
    std::cout << "Enter two numbers: ";
    std::cin >> a >> b;

    std::cout << "Before swap: a = " << a << ", b = " << b << std::endl;

    // Method 1: Using arithmetic
    a = a + b;  // a now contains sum
    b = a - b;  // b now has original a
    a = a - b;  // a now has original b

    std::cout << "After swap: a = " << a << ", b = " << b << std::endl;

    // Method 2: Using XOR (works too!)
    // a = a ^ b;
    // b = a ^ b;
    // a = a ^ b;

    return 0;
}
```

### Solution 4: Bitwise Practice
```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int num = 0;  // 00000000
    int bitPosition = 3;

    std::cout << "Initial: " << std::bitset<8>(num) << std::endl;

    // Set bit (turn on bit at position)
    num |= (1 << bitPosition);
    std::cout << "After setting bit " << bitPosition << ": "
              << std::bitset<8>(num) << std::endl;

    // Clear bit (turn off bit at position)
    bitPosition = 3;
    num &= ~(1 << bitPosition);
    std::cout << "After clearing bit " << bitPosition << ": "
              << std::bitset<8>(num) << std::endl;

    // Toggle bit (flip bit at position)
    bitPosition = 5;
    num ^= (1 << bitPosition);
    std::cout << "After toggling bit " << bitPosition << ": "
              << std::bitset<8>(num) << std::endl;

    // Check if bit is set
    bitPosition = 5;
    if (num & (1 << bitPosition)) {
        std::cout << "Bit " << bitPosition << " is SET" << std::endl;
    } else {
        std::cout << "Bit " << bitPosition << " is CLEAR" << std::endl;
    }

    return 0;
}
```

### Solution 5: Grade Calculator
```cpp
#include <iostream>

int main() {
    int score;
    std::cout << "Enter numeric score (0-100): ";
    std::cin >> score;

    std::string grade = (score >= 90) ? "A" :
                       (score >= 80) ? "B" :
                       (score >= 70) ? "C" :
                       (score >= 60) ? "D" : "F";

    std::cout << "Score: " << score << " - Grade: " << grade << std::endl;

    return 0;
}
```

## Summary

### Operator Categories
1. **Arithmetic**: +, -, *, /, %
2. **Comparison**: ==, !=, <, >, <=, >=
3. **Logical**: &&, ||, !
4. **Assignment**: =, +=, -=, *=, /=, %=
5. **Increment/Decrement**: ++, --
6. **Bitwise**: &, |, ^, ~, <<, >>
7. **Ternary**: ? :

### Key Points
- Watch for integer vs floating-point division
- Use parentheses for clarity
- Prefix (++x) vs Postfix (x++) matters in expressions
- Bitwise operators are crucial for graphics and optimization
- Ternary operator is concise but can be hard to read when nested

## Checklist

- [ ] Understand all arithmetic operators
- [ ] Can use comparison operators correctly
- [ ] Master logical operators and short-circuit evaluation
- [ ] Know compound assignment operators
- [ ] Understand prefix vs postfix increment/decrement
- [ ] Know operator precedence rules
- [ ] Can use bitwise operators for flags
- [ ] Understand ternary conditional operator
- [ ] Completed all exercises

---

**Time to complete**: 2.5 hours
**Previous lesson**: [Lesson 03 - Variables and Data Types](../Lesson-03-Variables-DataTypes/README.md)
**Next lesson**: [Lesson 05 - Control Flow](../Lesson-05-Control-Flow/README.md)
