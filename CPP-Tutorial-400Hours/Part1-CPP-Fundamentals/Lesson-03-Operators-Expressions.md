# Lesson 3: Operators and Expressions

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Arithmetic Operators
2. Compound Assignment Operators
3. Increment and Decrement Operators
4. Relational Operators
5. Logical Operators
6. Operator Precedence
7. Type Casting
8. Exercises and Projects

---

## 1. Arithmetic Operators

### Basic Arithmetic:

```cpp
#include <iostream>

int main()
{
    int a = 10, b = 3;

    std::cout << "a = " << a << ", b = " << b << "\n\n";

    std::cout << "Addition: " << a << " + " << b << " = " << (a + b) << "\n";
    std::cout << "Subtraction: " << a << " - " << b << " = " << (a - b) << "\n";
    std::cout << "Multiplication: " << a << " * " << b << " = " << (a * b) << "\n";
    std::cout << "Division: " << a << " / " << b << " = " << (a / b) << "\n";
    std::cout << "Modulus: " << a << " % " << b << " = " << (a % b) << "\n";

    return 0;
}
```

**Output**:
```
a = 10, b = 3

Addition: 10 + 3 = 13
Subtraction: 10 - 3 = 7
Multiplication: 10 * 3 = 30
Division: 10 / 3 = 3
Modulus: 10 % 3 = 1
```

### Integer vs. Floating-Point Division:

```cpp
#include <iostream>

int main()
{
    // Integer division (truncates)
    int int1 = 7, int2 = 2;
    std::cout << "Integer division: " << int1 << " / " << int2 << " = " << (int1 / int2) << "\n";
    // Output: 3 (not 3.5!)

    // Floating-point division
    double dbl1 = 7.0, dbl2 = 2.0;
    std::cout << "Float division: " << dbl1 << " / " << dbl2 << " = " << (dbl1 / dbl2) << "\n";
    // Output: 3.5

    // Mixed (converts to double)
    std::cout << "Mixed division: " << 7.0 << " / " << 2 << " = " << (7.0 / 2) << "\n";
    // Output: 3.5

    return 0;
}
```

### Modulus Operator (Remainder):

```cpp
#include <iostream>

int main()
{
    std::cout << "10 % 3 = " << (10 % 3) << "\n";   // 1
    std::cout << "15 % 4 = " << (15 % 4) << "\n";   // 3
    std::cout << "20 % 5 = " << (20 % 5) << "\n";   // 0
    std::cout << "7 % 2 = " << (7 % 2) << "\n";     // 1 (odd)
    std::cout << "8 % 2 = " << (8 % 2) << "\n";     // 0 (even)

    // Use case: Check if number is even or odd
    int number = 17;
    if (number % 2 == 0)
        std::cout << number << " is even\n";
    else
        std::cout << number << " is odd\n";

    return 0;
}
```

---

## 2. Compound Assignment Operators

### Overview:

```cpp
#include <iostream>

int main()
{
    int x = 10;

    std::cout << "Initial x: " << x << "\n";

    x += 5;  // Same as: x = x + 5
    std::cout << "After x += 5: " << x << "\n";  // 15

    x -= 3;  // Same as: x = x - 3
    std::cout << "After x -= 3: " << x << "\n";  // 12

    x *= 2;  // Same as: x = x * 2
    std::cout << "After x *= 2: " << x << "\n";  // 24

    x /= 4;  // Same as: x = x / 4
    std::cout << "After x /= 4: " << x << "\n";  // 6

    x %= 4;  // Same as: x = x % 4
    std::cout << "After x %= 4: " << x << "\n";  // 2

    return 0;
}
```

### Complete List:

| Operator | Example | Equivalent |
|----------|---------|------------|
| `+=` | `a += b` | `a = a + b` |
| `-=` | `a -= b` | `a = a - b` |
| `*=` | `a *= b` | `a = a * b` |
| `/=` | `a /= b` | `a = a / b` |
| `%=` | `a %= b` | `a = a % b` |

---

## 3. Increment and Decrement Operators

### Prefix vs. Postfix:

```cpp
#include <iostream>

int main()
{
    int a = 5;
    int b = 5;

    // Prefix increment (increment first, then use)
    std::cout << "a before: " << a << "\n";
    std::cout << "++a returns: " << ++a << "\n";  // 6
    std::cout << "a after: " << a << "\n";        // 6

    std::cout << "\n";

    // Postfix increment (use first, then increment)
    std::cout << "b before: " << b << "\n";
    std::cout << "b++ returns: " << b++ << "\n";  // 5
    std::cout << "b after: " << b << "\n";        // 6

    return 0;
}
```

**Output**:
```
a before: 5
++a returns: 6
a after: 6

b before: 5
b++ returns: 5
b after: 6
```

### Practical Example:

```cpp
#include <iostream>

int main()
{
    int counter = 0;

    std::cout << "Counter: " << counter << "\n";
    counter++;
    std::cout << "Counter: " << counter << "\n";
    counter++;
    std::cout << "Counter: " << counter << "\n";
    counter++;
    std::cout << "Counter: " << counter << "\n";

    // Decrement
    counter--;
    std::cout << "After decrement: " << counter << "\n";

    return 0;
}
```

### Complex Example:

```cpp
#include <iostream>

int main()
{
    int x = 10;
    int y;

    y = x++;  // y = 10, x = 11
    std::cout << "y = x++: y = " << y << ", x = " << x << "\n";

    x = 10;  // Reset
    y = ++x;  // y = 11, x = 11
    std::cout << "y = ++x: y = " << y << ", x = " << x << "\n";

    return 0;
}
```

---

## 4. Relational Operators

### Comparison Operators:

```cpp
#include <iostream>

int main()
{
    int a = 10, b = 20;

    std::cout << std::boolalpha;  // Print true/false instead of 1/0

    std::cout << "a = " << a << ", b = " << b << "\n\n";

    std::cout << "a == b: " << (a == b) << "\n";  // false
    std::cout << "a != b: " << (a != b) << "\n";  // true
    std::cout << "a < b: " << (a < b) << "\n";    // true
    std::cout << "a > b: " << (a > b) << "\n";    // false
    std::cout << "a <= b: " << (a <= b) << "\n";  // true
    std::cout << "a >= b: " << (a >= b) << "\n";  // false

    return 0;
}
```

### Complete List:

| Operator | Meaning | Example |
|----------|---------|---------|
| `==` | Equal to | `a == b` |
| `!=` | Not equal to | `a != b` |
| `<` | Less than | `a < b` |
| `>` | Greater than | `a > b` |
| `<=` | Less than or equal | `a <= b` |
| `>=` | Greater than or equal | `a >= b` |

---

## 5. Logical Operators

### AND, OR, NOT:

```cpp
#include <iostream>

int main()
{
    bool a = true, b = false;

    std::cout << std::boolalpha;

    std::cout << "a = " << a << ", b = " << b << "\n\n";

    // AND (&&) - Both must be true
    std::cout << "a && b: " << (a && b) << "\n";     // false
    std::cout << "a && a: " << (a && a) << "\n";     // true
    std::cout << "b && b: " << (b && b) << "\n";     // false

    std::cout << "\n";

    // OR (||) - At least one must be true
    std::cout << "a || b: " << (a || b) << "\n";     // true
    std::cout << "a || a: " << (a || a) << "\n";     // true
    std::cout << "b || b: " << (b || b) << "\n";     // false

    std::cout << "\n";

    // NOT (!) - Inverts the value
    std::cout << "!a: " << !a << "\n";               // false
    std::cout << "!b: " << !b << "\n";               // true

    return 0;
}
```

### Truth Tables:

**AND (&&)**:
| A | B | A && B |
|---|---|--------|
| F | F | F |
| F | T | F |
| T | F | F |
| T | T | T |

**OR (||)**:
| A | B | A \|\| B |
|---|---|--------|
| F | F | F |
| F | T | T |
| T | F | T |
| T | T | T |

**NOT (!)**:
| A | !A |
|---|-----|
| F | T |
| T | F |

### Practical Examples:

```cpp
#include <iostream>

int main()
{
    int age = 25;
    bool hasLicense = true;

    // Can drive?
    bool canDrive = (age >= 18) && hasLicense;
    std::cout << std::boolalpha;
    std::cout << "Can drive: " << canDrive << "\n";

    // Teenager?
    bool isTeenager = (age >= 13) && (age <= 19);
    std::cout << "Is teenager: " << isTeenager << "\n";

    // Weekend?
    int day = 6;  // 1=Mon, 2=Tue, ..., 6=Sat, 7=Sun
    bool isWeekend = (day == 6) || (day == 7);
    std::cout << "Is weekend: " << isWeekend << "\n";

    return 0;
}
```

---

## 6. Operator Precedence

### Precedence Levels (High to Low):

```cpp
#include <iostream>

int main()
{
    int result;

    // Multiplication before addition
    result = 2 + 3 * 4;
    std::cout << "2 + 3 * 4 = " << result << "\n";  // 14 (not 20)

    // Use parentheses to change order
    result = (2 + 3) * 4;
    std::cout << "(2 + 3) * 4 = " << result << "\n";  // 20

    // Complex expression
    result = 10 + 5 * 2 - 8 / 4;
    std::cout << "10 + 5 * 2 - 8 / 4 = " << result << "\n";  // 18
    // Calculation: 10 + (5*2) - (8/4) = 10 + 10 - 2 = 18

    return 0;
}
```

### Precedence Table:

| Level | Operator | Description |
|-------|----------|-------------|
| 1 | `++`, `--` | Increment/Decrement |
| 2 | `!` | Logical NOT |
| 3 | `*`, `/`, `%` | Multiplication, Division, Modulus |
| 4 | `+`, `-` | Addition, Subtraction |
| 5 | `<`, `<=`, `>`, `>=` | Relational |
| 6 | `==`, `!=` | Equality |
| 7 | `&&` | Logical AND |
| 8 | `\|\|` | Logical OR |
| 9 | `=`, `+=`, `-=`, etc. | Assignment |

### Best Practice: Use Parentheses!

```cpp
#include <iostream>

int main()
{
    int a = 5, b = 10, c = 15;

    // Hard to read
    int result1 = a + b * c - a / b + c % a;

    // Easy to read
    int result2 = a + (b * c) - (a / b) + (c % a);

    std::cout << "result1: " << result1 << "\n";
    std::cout << "result2: " << result2 << "\n";

    return 0;
}
```

---

## 7. Type Casting

### Implicit Casting (Automatic):

```cpp
#include <iostream>

int main()
{
    int i = 10;
    double d = i;  // int automatically converted to double

    std::cout << "int i = " << i << "\n";
    std::cout << "double d = " << d << "\n";

    // Mixed arithmetic
    int x = 5;
    double y = 2.5;
    double result = x + y;  // x converted to double
    std::cout << "5 + 2.5 = " << result << "\n";

    return 0;
}
```

### Explicit Casting (Manual):

```cpp
#include <iostream>

int main()
{
    double pi = 3.14159;

    // C-style cast
    int piInt1 = (int)pi;
    std::cout << "C-style cast: " << piInt1 << "\n";  // 3

    // C++ style cast (preferred)
    int piInt2 = static_cast<int>(pi);
    std::cout << "static_cast: " << piInt2 << "\n";   // 3

    // Fix integer division
    int a = 7, b = 2;
    double result = static_cast<double>(a) / b;
    std::cout << "7 / 2 = " << result << "\n";  // 3.5

    return 0;
}
```

### Common Casting Scenarios:

```cpp
#include <iostream>

int main()
{
    // char to int (ASCII value)
    char ch = 'A';
    int ascii = static_cast<int>(ch);
    std::cout << "ASCII of 'A': " << ascii << "\n";  // 65

    // int to char
    int num = 66;
    char letter = static_cast<char>(num);
    std::cout << "Character for 66: " << letter << "\n";  // B

    // double to int (truncation)
    double price = 19.99;
    int dollars = static_cast<int>(price);
    std::cout << "$19.99 truncated: $" << dollars << "\n";  // 19

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Arithmetic Calculator

```cpp
#include <iostream>

int main()
{
    double num1, num2;
    char op;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> op;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << "\nResult: ";

    if (op == '+')
        std::cout << num1 + num2;
    else if (op == '-')
        std::cout << num1 - num2;
    else if (op == '*')
        std::cout << num1 * num2;
    else if (op == '/')
        std::cout << num1 / num2;
    else
        std::cout << "Invalid operator!";

    std::cout << "\n";

    return 0;
}
```

### Exercise 2: Even or Odd Checker

```cpp
#include <iostream>

int main()
{
    int number;

    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number % 2 == 0)
        std::cout << number << " is EVEN\n";
    else
        std::cout << number << " is ODD\n";

    return 0;
}
```

### Exercise 3: Grade Calculator

```cpp
#include <iostream>

int main()
{
    double test1, test2, test3;

    std::cout << "Enter score for test 1: ";
    std::cin >> test1;

    std::cout << "Enter score for test 2: ";
    std::cin >> test2;

    std::cout << "Enter score for test 3: ";
    std::cin >> test3;

    double average = (test1 + test2 + test3) / 3.0;

    std::cout << "\n===== RESULTS =====\n";
    std::cout << "Test 1: " << test1 << "\n";
    std::cout << "Test 2: " << test2 << "\n";
    std::cout << "Test 3: " << test3 << "\n";
    std::cout << "Average: " << average << "\n";

    return 0;
}
```

### Exercise 4: Currency Converter

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    const double USD_TO_EUR = 0.85;
    const double USD_TO_GBP = 0.73;
    const double USD_TO_JPY = 110.0;

    double usd;

    std::cout << "Enter amount in USD: $";
    std::cin >> usd;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n===== CONVERSIONS =====\n";
    std::cout << "USD: $" << usd << "\n";
    std::cout << "EUR: €" << (usd * USD_TO_EUR) << "\n";
    std::cout << "GBP: £" << (usd * USD_TO_GBP) << "\n";
    std::cout << "JPY: ¥" << (usd * USD_TO_JPY) << "\n";

    return 0;
}
```

### Exercise 5: Time Converter

```cpp
#include <iostream>

int main()
{
    int totalSeconds;

    std::cout << "Enter time in seconds: ";
    std::cin >> totalSeconds;

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::cout << "\n" << totalSeconds << " seconds = ";
    std::cout << hours << "h " << minutes << "m " << seconds << "s\n";

    return 0;
}
```

---

## Practice Project: Compound Interest Calculator

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>  // For pow() function

int main()
{
    double principal, rate, time;
    int n;  // Compounds per year

    std::cout << "===== COMPOUND INTEREST CALCULATOR =====\n\n";

    std::cout << "Enter principal amount: $";
    std::cin >> principal;

    std::cout << "Enter annual interest rate (as decimal, e.g., 0.05 for 5%): ";
    std::cin >> rate;

    std::cout << "Enter time period (years): ";
    std::cin >> time;

    std::cout << "Enter number of times interest compounds per year: ";
    std::cin >> n;

    // Formula: A = P(1 + r/n)^(nt)
    double amount = principal * pow((1 + rate / n), n * time);
    double interest = amount - principal;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n===== RESULTS =====\n";
    std::cout << "Principal: $" << principal << "\n";
    std::cout << "Rate: " << (rate * 100) << "%\n";
    std::cout << "Time: " << time << " years\n";
    std::cout << "Compounds: " << n << " times per year\n";
    std::cout << "\nFinal Amount: $" << amount << "\n";
    std::cout << "Interest Earned: $" << interest << "\n";

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Integer Division
```cpp
❌ double result = 5 / 2;  // result = 2.0 (not 2.5!)
✅ double result = 5.0 / 2.0;  // result = 2.5
```

### Mistake 2: Comparison vs. Assignment
```cpp
❌ if (x = 5)  // Assigns 5 to x, always true!
✅ if (x == 5)  // Compares x with 5
```

### Mistake 3: Modulus with Floats
```cpp
❌ double x = 5.5 % 2.0;  // ERROR: % only works with integers
✅ int x = 5 % 2;  // Correct
```

### Mistake 4: Prefix vs. Postfix Confusion
```cpp
int x = 5;
int y = x++;  // y = 5, x = 6 (not y = 6!)
```

---

## Key Takeaways

1. ✅ Use `+`, `-`, `*`, `/`, `%` for arithmetic
2. ✅ Compound operators (`+=`, `-=`) are shortcuts
3. ✅ `++` and `--` increment/decrement
4. ✅ Relational operators (`==`, `!=`, `<`, `>`) compare values
5. ✅ Logical operators (`&&`, `||`, `!`) combine conditions
6. ✅ Use parentheses to clarify precedence
7. ✅ `static_cast<type>()` for explicit type conversion
8. ✅ Integer division truncates (7/2 = 3, not 3.5)

---

## Summary Checklist

Before moving to Lesson 4, ensure you can:
- [ ] Use all arithmetic operators
- [ ] Apply compound assignment operators
- [ ] Understand prefix vs. postfix increment
- [ ] Compare values with relational operators
- [ ] Combine conditions with logical operators
- [ ] Understand operator precedence
- [ ] Cast between types (int, double, char)
- [ ] Avoid integer division pitfalls
- [ ] Write complex expressions correctly

---

## Next Lesson Preview

In **Lesson 4**, we'll learn about:
- if statements
- if-else statements
- else if ladders
- Nested if statements
- Switch statements
- Ternary operator

**Excellent work on Lesson 3!** 💪
