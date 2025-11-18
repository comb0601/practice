# Lesson 04: Operators in C++

**Duration: 2.5 hours**

## Table of Contents
1. Arithmetic Operators
2. Relational Operators
3. Logical Operators
4. Assignment Operators
5. Increment/Decrement Operators
6. Bitwise Operators
7. Other Operators
8. Operator Precedence

## 1. Arithmetic Operators

### Basic Arithmetic

```cpp
#include <iostream>

int main() {
    int a = 10, b = 3;

    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "Addition: " << (a + b) << std::endl;        // 13
    std::cout << "Subtraction: " << (a - b) << std::endl;     // 7
    std::cout << "Multiplication: " << (a * b) << std::endl;  // 30
    std::cout << "Division: " << (a / b) << std::endl;        // 3 (integer division)
    std::cout << "Modulus: " << (a % b) << std::endl;         // 1 (remainder)

    return 0;
}
```

### Integer vs Floating-Point Division

```cpp
#include <iostream>

int main() {
    int x = 7, y = 2;
    double dx = 7.0, dy = 2.0;

    std::cout << "Integer division: " << x / y << std::endl;      // 3
    std::cout << "Float division: " << dx / dy << std::endl;      // 3.5
    std::cout << "Mixed: " << x / dy << std::endl;                // 3.5
    std::cout << "Cast division: " << static_cast<double>(x) / y << std::endl;  // 3.5

    return 0;
}
```

### Modulus Operator

```cpp
#include <iostream>

int main() {
    // Check if even or odd
    int num = 17;
    if (num % 2 == 0) {
        std::cout << num << " is even" << std::endl;
    } else {
        std::cout << num << " is odd" << std::endl;
    }

    // Extract last digit
    int number = 12345;
    int lastDigit = number % 10;
    std::cout << "Last digit: " << lastDigit << std::endl;

    // Cycle through values
    for (int i = 0; i < 10; i++) {
        std::cout << i % 3 << " ";  // 0 1 2 0 1 2 0 1 2 0
    }
    std::cout << std::endl;

    return 0;
}
```

## 2. Relational Operators

### Comparison Operators

```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;

    std::cout << std::boolalpha;  // Print true/false instead of 1/0

    std::cout << "a == b: " << (a == b) << std::endl;  // false
    std::cout << "a != b: " << (a != b) << std::endl;  // true
    std::cout << "a < b: " << (a < b) << std::endl;    // true
    std::cout << "a > b: " << (a > b) << std::endl;    // false
    std::cout << "a <= b: " << (a <= b) << std::endl;  // true
    std::cout << "a >= b: " << (a >= b) << std::endl;  // false

    return 0;
}
```

### Comparing Different Types

```cpp
#include <iostream>

int main() {
    int intVal = 5;
    double doubleVal = 5.0;

    std::cout << std::boolalpha;
    std::cout << "5 == 5.0: " << (intVal == doubleVal) << std::endl;  // true

    // Be careful with floating-point comparison
    double a = 0.1 + 0.2;
    double b = 0.3;
    std::cout << "0.1 + 0.2 == 0.3: " << (a == b) << std::endl;  // May be false!

    // Better way to compare floats
    double epsilon = 0.0001;
    bool areEqual = std::abs(a - b) < epsilon;
    std::cout << "Are equal (with epsilon): " << areEqual << std::endl;

    return 0;
}
```

## 3. Logical Operators

### AND, OR, NOT

```cpp
#include <iostream>

int main() {
    bool a = true, b = false;

    std::cout << std::boolalpha;

    // AND (&&) - true if both are true
    std::cout << "true && true: " << (true && true) << std::endl;    // true
    std::cout << "true && false: " << (true && false) << std::endl;  // false

    // OR (||) - true if at least one is true
    std::cout << "true || false: " << (true || false) << std::endl;  // true
    std::cout << "false || false: " << (false || false) << std::endl;// false

    // NOT (!) - inverts boolean value
    std::cout << "!true: " << (!true) << std::endl;   // false
    std::cout << "!false: " << (!false) << std::endl; // true

    return 0;
}
```

### Practical Examples

```cpp
#include <iostream>

int main() {
    int age = 25;
    double income = 50000;
    bool hasLicense = true;

    // AND example
    if (age >= 18 && hasLicense) {
        std::cout << "Can drive" << std::endl;
    }

    // OR example
    if (age < 18 || age > 65) {
        std::cout << "Eligible for discount" << std::endl;
    }

    // Complex condition
    if ((age >= 21 && age <= 65) && (income > 30000)) {
        std::cout << "Eligible for loan" << std::endl;
    }

    // NOT example
    if (!hasLicense) {
        std::cout << "Cannot drive" << std::endl;
    }

    return 0;
}
```

### Short-Circuit Evaluation

```cpp
#include <iostream>

bool expensiveFunction() {
    std::cout << "Expensive function called!" << std::endl;
    return true;
}

int main() {
    // AND short-circuit
    if (false && expensiveFunction()) {
        // expensiveFunction() is NOT called because first condition is false
    }

    // OR short-circuit
    if (true || expensiveFunction()) {
        // expensiveFunction() is NOT called because first condition is true
    }

    return 0;
}
```

## 4. Assignment Operators

### Basic Assignment

```cpp
#include <iostream>

int main() {
    int x = 10;  // Basic assignment

    x += 5;   // x = x + 5 (15)
    x -= 3;   // x = x - 3 (12)
    x *= 2;   // x = x * 2 (24)
    x /= 4;   // x = x / 4 (6)
    x %= 5;   // x = x % 5 (1)

    std::cout << "Final value: " << x << std::endl;

    return 0;
}
```

### Compound Assignment Examples

```cpp
#include <iostream>

int main() {
    int score = 100;

    // Add bonus points
    score += 50;  // 150
    std::cout << "After bonus: " << score << std::endl;

    // Apply penalty
    score -= 20;  // 130
    std::cout << "After penalty: " << score << std::endl;

    // Double the score
    score *= 2;  // 260
    std::cout << "After doubling: " << score << std::endl;

    return 0;
}
```

## 5. Increment/Decrement Operators

### Pre and Post Increment

```cpp
#include <iostream>

int main() {
    int a = 5, b = 5;

    // Post-increment: use then increment
    int x = a++;  // x = 5, a = 6
    std::cout << "Post-increment: x = " << x << ", a = " << a << std::endl;

    // Pre-increment: increment then use
    int y = ++b;  // y = 6, b = 6
    std::cout << "Pre-increment: y = " << y << ", b = " << b << std::endl;

    return 0;
}
```

### Detailed Example

```cpp
#include <iostream>

int main() {
    int i = 10;

    std::cout << "Initial: " << i << std::endl;      // 10
    std::cout << "i++: " << i++ << std::endl;        // 10 (then i becomes 11)
    std::cout << "After i++: " << i << std::endl;    // 11
    std::cout << "++i: " << ++i << std::endl;        // 12 (i becomes 12 first)
    std::cout << "After ++i: " << i << std::endl;    // 12

    // Decrement
    std::cout << "i--: " << i-- << std::endl;        // 12 (then i becomes 11)
    std::cout << "--i: " << --i << std::endl;        // 10 (i becomes 10 first)

    return 0;
}
```

### Common Use in Loops

```cpp
#include <iostream>

int main() {
    // Pre-increment in loop
    for (int i = 0; i < 5; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Post-increment (same effect in loop)
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 6. Bitwise Operators

### Basic Bitwise Operations

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int a = 60;  // 0011 1100
    unsigned int b = 13;  // 0000 1101

    std::cout << "a = " << std::bitset<8>(a) << " (" << a << ")" << std::endl;
    std::cout << "b = " << std::bitset<8>(b) << " (" << b << ")" << std::endl;

    // AND (&)
    std::cout << "a & b = " << std::bitset<8>(a & b) << " (" << (a & b) << ")" << std::endl;

    // OR (|)
    std::cout << "a | b = " << std::bitset<8>(a | b) << " (" << (a | b) << ")" << std::endl;

    // XOR (^)
    std::cout << "a ^ b = " << std::bitset<8>(a ^ b) << " (" << (a ^ b) << ")" << std::endl;

    // NOT (~)
    std::cout << "~a = " << std::bitset<8>(~a) << " (" << (~a) << ")" << std::endl;

    // Left shift (<<)
    std::cout << "a << 2 = " << std::bitset<8>(a << 2) << " (" << (a << 2) << ")" << std::endl;

    // Right shift (>>)
    std::cout << "a >> 2 = " << std::bitset<8>(a >> 2) << " (" << (a >> 2) << ")" << std::endl;

    return 0;
}
```

### Practical Bitwise Examples

```cpp
#include <iostream>

int main() {
    // Check if number is even (last bit is 0)
    int num = 42;
    if ((num & 1) == 0) {
        std::cout << num << " is even" << std::endl;
    }

    // Multiply by 2 using left shift
    int x = 5;
    std::cout << x << " * 2 = " << (x << 1) << std::endl;  // 10

    // Divide by 2 using right shift
    int y = 16;
    std::cout << y << " / 2 = " << (y >> 1) << std::endl;  // 8

    // Swap two numbers without temp variable
    int a = 10, b = 20;
    std::cout << "Before: a = " << a << ", b = " << b << std::endl;
    a ^= b;
    b ^= a;
    a ^= b;
    std::cout << "After: a = " << a << ", b = " << b << std::endl;

    return 0;
}
```

## 7. Other Operators

### Ternary Operator

```cpp
#include <iostream>

int main() {
    int age = 20;

    // condition ? value_if_true : value_if_false
    std::string status = (age >= 18) ? "Adult" : "Minor";
    std::cout << "Status: " << status << std::endl;

    // Nested ternary (not recommended for readability)
    int score = 85;
    char grade = (score >= 90) ? 'A' : (score >= 80) ? 'B' : (score >= 70) ? 'C' : 'F';
    std::cout << "Grade: " << grade << std::endl;

    // Finding max
    int a = 10, b = 20;
    int max = (a > b) ? a : b;
    std::cout << "Max: " << max << std::endl;

    return 0;
}
```

### Comma Operator

```cpp
#include <iostream>

int main() {
    int a, b, c;

    // Comma operator evaluates left to right, returns rightmost
    c = (a = 10, b = 20, a + b);

    std::cout << "a = " << a << std::endl;  // 10
    std::cout << "b = " << b << std::endl;  // 20
    std::cout << "c = " << c << std::endl;  // 30

    // Common use in for loops
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << std::endl;
    }

    return 0;
}
```

### Sizeof Operator

```cpp
#include <iostream>

int main() {
    std::cout << "Size of char: " << sizeof(char) << " byte" << std::endl;
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;

    int arr[10];
    std::cout << "Size of array: " << sizeof(arr) << " bytes" << std::endl;
    std::cout << "Array length: " << sizeof(arr) / sizeof(arr[0]) << std::endl;

    return 0;
}
```

## 8. Operator Precedence

### Precedence Table (Highest to Lowest)

```cpp
#include <iostream>

int main() {
    // Example 1: Multiplication before addition
    int result1 = 2 + 3 * 4;  // 14, not 20
    std::cout << "2 + 3 * 4 = " << result1 << std::endl;

    // Example 2: Use parentheses for clarity
    int result2 = (2 + 3) * 4;  // 20
    std::cout << "(2 + 3) * 4 = " << result2 << std::endl;

    // Example 3: Mixed operators
    int result3 = 10 + 5 * 2 - 8 / 4;  // 10 + 10 - 2 = 18
    std::cout << "10 + 5 * 2 - 8 / 4 = " << result3 << std::endl;

    // Example 4: Relational and logical
    bool result4 = 5 > 3 && 10 < 20;  // true && true = true
    std::cout << std::boolalpha;
    std::cout << "5 > 3 && 10 < 20 = " << result4 << std::endl;

    return 0;
}
```

### Precedence Order
1. () [] -> . (highest)
2. ! ~ ++ -- + - * & sizeof (unary)
3. * / %
4. + -
5. << >>
6. < <= > >=
7. == !=
8. & (bitwise AND)
9. ^
10. |
11. &&
12. ||
13. ?:
14. = += -= *= /= (lowest)

## Practical Exercises

### Exercise 1: Calculator

```cpp
#include <iostream>

int main() {
    double num1, num2;
    char operation;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> operation;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    double result;

    switch (operation) {
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
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                std::cout << "Error: Division by zero!" << std::endl;
                return 1;
            }
            break;
        default:
            std::cout << "Invalid operator!" << std::endl;
            return 1;
    }

    std::cout << num1 << " " << operation << " " << num2 << " = " << result << std::endl;

    return 0;
}
```

### Exercise 2: Grade Calculator

```cpp
#include <iostream>

int main() {
    int score;

    std::cout << "Enter score (0-100): ";
    std::cin >> score;

    char grade = (score >= 90) ? 'A' :
                 (score >= 80) ? 'B' :
                 (score >= 70) ? 'C' :
                 (score >= 60) ? 'D' : 'F';

    std::string status = (score >= 60) ? "Pass" : "Fail";

    std::cout << "Grade: " << grade << std::endl;
    std::cout << "Status: " << status << std::endl;

    return 0;
}
```

### Exercise 3: Leap Year Checker

```cpp
#include <iostream>

int main() {
    int year;

    std::cout << "Enter year: ";
    std::cin >> year;

    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    std::cout << year << " is "
              << (isLeap ? "" : "not ")
              << "a leap year" << std::endl;

    return 0;
}
```

## Complete Project: Unit Converter

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Unit Converter ===" << std::endl;
    std::cout << "1. Temperature (C to F)" << std::endl;
    std::cout << "2. Temperature (F to C)" << std::endl;
    std::cout << "3. Distance (km to miles)" << std::endl;
    std::cout << "4. Distance (miles to km)" << std::endl;
    std::cout << "5. Weight (kg to pounds)" << std::endl;
    std::cout << "6. Weight (pounds to kg)" << std::endl;

    int choice;
    double value, result;

    std::cout << "Choice: ";
    std::cin >> choice;

    std::cout << "Enter value: ";
    std::cin >> value;

    std::cout << std::fixed << std::setprecision(2);

    switch (choice) {
        case 1:
            result = (value * 9.0 / 5.0) + 32.0;
            std::cout << value << "°C = " << result << "°F" << std::endl;
            break;
        case 2:
            result = (value - 32.0) * 5.0 / 9.0;
            std::cout << value << "°F = " << result << "°C" << std::endl;
            break;
        case 3:
            result = value * 0.621371;
            std::cout << value << " km = " << result << " miles" << std::endl;
            break;
        case 4:
            result = value / 0.621371;
            std::cout << value << " miles = " << result << " km" << std::endl;
            break;
        case 5:
            result = value * 2.20462;
            std::cout << value << " kg = " << result << " lbs" << std::endl;
            break;
        case 6:
            result = value / 2.20462;
            std::cout << value << " lbs = " << result << " kg" << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return 1;
    }

    return 0;
}
```

## Summary

You learned:
- Arithmetic operators (+, -, *, /, %)
- Relational operators (==, !=, <, >, <=, >=)
- Logical operators (&&, ||, !)
- Assignment operators (=, +=, -=, *=, /=, %=)
- Increment/Decrement (++, --)
- Bitwise operators (&, |, ^, ~, <<, >>)
- Ternary operator (?:)
- Operator precedence

## Next Lesson

Lesson 05: Control Flow (if, else, switch)
