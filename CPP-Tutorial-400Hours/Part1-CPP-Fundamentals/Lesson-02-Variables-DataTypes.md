# Lesson 2: Variables and Data Types

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. What are Variables?
2. Fundamental Data Types
3. Variable Declaration and Initialization
4. Input with std::cin
5. Type Sizes and Limits
6. Constants
7. Exercises and Projects

---

## 1. What are Variables?

A **variable** is a named storage location in memory that holds a value. Think of it as a labeled box where you can store data.

### Variable Anatomy:
```cpp
int age = 25;
//  ^   ^  ^
//  |   |  |
//  |   |  └─ Value
//  |   └──── Variable name
//  └──────── Data type
```

### Memory Representation:
```
Memory Address: 0x001  0x002  0x003  0x004
Variable 'age': [  25  ]
```

---

## 2. Fundamental Data Types

### Integer Types:

```cpp
#include <iostream>

int main()
{
    // Integer types (whole numbers)
    short s = 100;           // Small integers: -32,768 to 32,767
    int i = 50000;           // Standard integers: -2,147,483,648 to 2,147,483,647
    long l = 1000000L;       // Long integers (same as int on most systems)
    long long ll = 9223372036854775807LL;  // Very large integers

    // Unsigned versions (only positive)
    unsigned short us = 65535;
    unsigned int ui = 4294967295U;
    unsigned long ul = 4000000000UL;
    unsigned long long ull = 18446744073709551615ULL;

    std::cout << "short: " << s << "\n";
    std::cout << "int: " << i << "\n";
    std::cout << "long: " << l << "\n";
    std::cout << "long long: " << ll << "\n";
    std::cout << "unsigned int: " << ui << "\n";

    return 0;
}
```

### Floating-Point Types:

```cpp
#include <iostream>
#include <iomanip>  // For std::setprecision

int main()
{
    float f = 3.14159f;          // Single precision: ~7 decimal digits
    double d = 3.14159265359;    // Double precision: ~15 decimal digits
    long double ld = 3.14159265358979323846L;  // Extended precision

    std::cout << std::setprecision(15);  // Show more decimal places
    std::cout << "float: " << f << "\n";
    std::cout << "double: " << d << "\n";
    std::cout << "long double: " << ld << "\n";

    return 0;
}
```

**Output**:
```
float: 3.14159274101257
double: 3.14159265359
long double: 3.14159265358979
```

### Character Type:

```cpp
#include <iostream>

int main()
{
    char letter = 'A';           // Single character
    char digit = '5';            // Character '5', not number 5
    char newline = '\n';         // Special character

    std::cout << "Letter: " << letter << "\n";
    std::cout << "Digit: " << digit << "\n";
    std::cout << "ASCII value of A: " << (int)letter << "\n";

    // char is actually a small integer (-128 to 127)
    char smallNum = 65;
    std::cout << "Character for 65: " << smallNum << "\n";  // Prints 'A'

    return 0;
}
```

### Boolean Type:

```cpp
#include <iostream>

int main()
{
    bool isRaining = true;
    bool isSunny = false;

    std::cout << "Is it raining? " << isRaining << "\n";      // Prints 1
    std::cout << "Is it sunny? " << isSunny << "\n";          // Prints 0

    // Better output with std::boolalpha
    std::cout << std::boolalpha;
    std::cout << "Is it raining? " << isRaining << "\n";      // Prints true
    std::cout << "Is it sunny? " << isSunny << "\n";          // Prints false

    return 0;
}
```

### String Type:

```cpp
#include <iostream>
#include <string>  // Required for std::string

int main()
{
    std::string name = "John Doe";
    std::string greeting = "Hello";

    std::cout << greeting << ", " << name << "!\n";

    // String concatenation
    std::string fullGreeting = greeting + ", " + name + "!";
    std::cout << fullGreeting << "\n";

    // String length
    std::cout << "Length of name: " << name.length() << "\n";

    return 0;
}
```

---

## 3. Variable Declaration and Initialization

### Declaration vs. Initialization:

```cpp
#include <iostream>

int main()
{
    // Declaration only (uninitialized - dangerous!)
    int a;  // Contains garbage value

    // Declaration with initialization (RECOMMENDED)
    int b = 10;

    // C++11 uniform initialization (BEST PRACTICE)
    int c{20};
    int d{};  // Initialized to 0

    // Multiple declarations
    int x = 1, y = 2, z = 3;

    // Auto type deduction (C++11)
    auto autoInt = 42;         // int
    auto autoDouble = 3.14;    // double
    auto autoString = "Hello"; // const char*

    std::cout << "b: " << b << "\n";
    std::cout << "c: " << c << "\n";
    std::cout << "d: " << d << "\n";

    return 0;
}
```

### Naming Rules:

#### ✅ Valid Names:
```cpp
int age;
int playerScore;
int _privateVar;
int total2;
int MAX_SIZE;
```

#### ❌ Invalid Names:
```cpp
int 2total;      // Can't start with digit
int player-score; // No hyphens
int my var;      // No spaces
int int;         // Can't use keywords
```

### Naming Conventions:

```cpp
// camelCase (popular for variables)
int playerHealth;
int maxSpeed;

// snake_case
int player_health;
int max_speed;

// PascalCase (usually for classes)
int PlayerHealth;

// SCREAMING_SNAKE_CASE (for constants)
const int MAX_PLAYERS = 100;
```

---

## 4. Input with std::cin

### Basic Input:

```cpp
#include <iostream>
#include <string>

int main()
{
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cout << "You are " << age << " years old.\n";

    return 0;
}
```

### Multiple Inputs:

```cpp
#include <iostream>

int main()
{
    int x, y, z;
    std::cout << "Enter three numbers: ";
    std::cin >> x >> y >> z;

    std::cout << "You entered: " << x << ", " << y << ", " << z << "\n";

    return 0;
}
```

### String Input (Single Word):

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string name;
    std::cout << "Enter your first name: ";
    std::cin >> name;  // Reads until whitespace

    std::cout << "Hello, " << name << "!\n";

    return 0;
}
```

### String Input (Full Line):

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string fullName;
    std::cout << "Enter your full name: ";
    std::getline(std::cin, fullName);  // Reads entire line

    std::cout << "Hello, " << fullName << "!\n";

    return 0;
}
```

### Mixed Input (IMPORTANT):

```cpp
#include <iostream>
#include <string>

int main()
{
    int age;
    std::string name;

    std::cout << "Enter your age: ";
    std::cin >> age;

    // PROBLEM: cin leaves newline in buffer
    std::cin.ignore();  // Clear the newline

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::cout << name << " is " << age << " years old.\n";

    return 0;
}
```

---

## 5. Type Sizes and Limits

### Checking Type Sizes:

```cpp
#include <iostream>

int main()
{
    std::cout << "Size of char: " << sizeof(char) << " byte\n";
    std::cout << "Size of short: " << sizeof(short) << " bytes\n";
    std::cout << "Size of int: " << sizeof(int) << " bytes\n";
    std::cout << "Size of long: " << sizeof(long) << " bytes\n";
    std::cout << "Size of long long: " << sizeof(long long) << " bytes\n";
    std::cout << "Size of float: " << sizeof(float) << " bytes\n";
    std::cout << "Size of double: " << sizeof(double) << " bytes\n";
    std::cout << "Size of bool: " << sizeof(bool) << " byte\n";

    return 0;
}
```

**Typical Output (64-bit system)**:
```
Size of char: 1 byte
Size of short: 2 bytes
Size of int: 4 bytes
Size of long: 4 bytes
Size of long long: 8 bytes
Size of float: 4 bytes
Size of double: 8 bytes
Size of bool: 1 byte
```

### Type Limits:

```cpp
#include <iostream>
#include <limits>

int main()
{
    std::cout << "int min: " << std::numeric_limits<int>::min() << "\n";
    std::cout << "int max: " << std::numeric_limits<int>::max() << "\n";
    std::cout << "unsigned int max: " << std::numeric_limits<unsigned int>::max() << "\n";
    std::cout << "float min: " << std::numeric_limits<float>::min() << "\n";
    std::cout << "float max: " << std::numeric_limits<float>::max() << "\n";
    std::cout << "double max: " << std::numeric_limits<double>::max() << "\n";

    return 0;
}
```

---

## 6. Constants

### Using const:

```cpp
#include <iostream>

int main()
{
    const int MAX_PLAYERS = 100;
    const double PI = 3.14159265359;
    const std::string GAME_NAME = "Super Game";

    std::cout << "Max players: " << MAX_PLAYERS << "\n";
    std::cout << "PI: " << PI << "\n";

    // MAX_PLAYERS = 200;  // ERROR: Cannot modify const

    return 0;
}
```

### Using #define (Old Style):

```cpp
#include <iostream>

#define MAX_HEALTH 100
#define PI 3.14159

int main()
{
    std::cout << "Max health: " << MAX_HEALTH << "\n";
    std::cout << "PI: " << PI << "\n";

    return 0;
}
```

**Note**: Prefer `const` over `#define` in modern C++!

### constexpr (C++11):

```cpp
#include <iostream>

constexpr int square(int x)
{
    return x * x;
}

int main()
{
    constexpr int result = square(5);  // Computed at compile-time
    std::cout << "5 squared: " << result << "\n";

    return 0;
}
```

---

## 7. Exercises and Projects

### Exercise 1: Simple Calculator

```cpp
#include <iostream>

int main()
{
    double num1, num2;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << "\n===== RESULTS =====\n";
    std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
    std::cout << num1 << " - " << num2 << " = " << (num1 - num2) << "\n";
    std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << "\n";
    std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << "\n";

    return 0;
}
```

### Exercise 2: Temperature Converter

```cpp
#include <iostream>

int main()
{
    double celsius;

    std::cout << "Enter temperature in Celsius: ";
    std::cin >> celsius;

    double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
    double kelvin = celsius + 273.15;

    std::cout << "\n===== CONVERSIONS =====\n";
    std::cout << celsius << "°C = " << fahrenheit << "°F\n";
    std::cout << celsius << "°C = " << kelvin << "K\n";

    return 0;
}
```

### Exercise 3: Circle Properties

```cpp
#include <iostream>

int main()
{
    const double PI = 3.14159265359;
    double radius;

    std::cout << "Enter circle radius: ";
    std::cin >> radius;

    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    std::cout << "\n===== CIRCLE PROPERTIES =====\n";
    std::cout << "Radius: " << radius << "\n";
    std::cout << "Circumference: " << circumference << "\n";
    std::cout << "Area: " << area << "\n";

    return 0;
}
```

### Exercise 4: Personal Profile

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string firstName, lastName;
    int age;
    double height;

    std::cout << "Enter your first name: ";
    std::cin >> firstName;

    std::cout << "Enter your last name: ";
    std::cin >> lastName;

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Enter your height (in meters): ";
    std::cin >> height;

    std::cout << "\n===== YOUR PROFILE =====\n";
    std::cout << "Name: " << firstName << " " << lastName << "\n";
    std::cout << "Age: " << age << " years old\n";
    std::cout << "Height: " << height << " meters\n";

    return 0;
}
```

### Exercise 5: Shopping Receipt

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    std::string item1, item2, item3;
    double price1, price2, price3;

    std::cout << "Enter item 1 name: ";
    std::cin >> item1;
    std::cout << "Enter item 1 price: ";
    std::cin >> price1;

    std::cout << "Enter item 2 name: ";
    std::cin >> item2;
    std::cout << "Enter item 2 price: ";
    std::cin >> price2;

    std::cout << "Enter item 3 name: ";
    std::cin >> item3;
    std::cout << "Enter item 3 price: ";
    std::cin >> price3;

    double subtotal = price1 + price2 + price3;
    double tax = subtotal * 0.08;  // 8% tax
    double total = subtotal + tax;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n===== RECEIPT =====\n";
    std::cout << item1 << ": $" << price1 << "\n";
    std::cout << item2 << ": $" << price2 << "\n";
    std::cout << item3 << ": $" << price3 << "\n";
    std::cout << "-------------------\n";
    std::cout << "Subtotal: $" << subtotal << "\n";
    std::cout << "Tax (8%): $" << tax << "\n";
    std::cout << "TOTAL: $" << total << "\n";

    return 0;
}
```

---

## Practice Project: BMI Calculator

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    std::string name;
    double weight, height;

    std::cout << "===== BMI CALCULATOR =====\n\n";

    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "Enter your weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter your height (m): ";
    std::cin >> height;

    // Calculate BMI
    double bmi = weight / (height * height);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n===== RESULTS FOR " << name << " =====\n";
    std::cout << "Weight: " << weight << " kg\n";
    std::cout << "Height: " << height << " m\n";
    std::cout << "BMI: " << bmi << "\n\n";

    std::cout << "BMI Categories:\n";
    std::cout << "  Underweight: < 18.5\n";
    std::cout << "  Normal: 18.5 - 24.9\n";
    std::cout << "  Overweight: 25 - 29.9\n";
    std::cout << "  Obese: >= 30\n";

    return 0;
}
```

---

## Common Mistakes and Solutions

### Mistake 1: Uninitialized Variables
```cpp
❌ int x;
   std::cout << x;  // Undefined behavior!

✅ int x = 0;
   std::cout << x;  // Safe
```

### Mistake 2: Integer Division
```cpp
❌ int result = 5 / 2;  // result = 2 (not 2.5!)

✅ double result = 5.0 / 2.0;  // result = 2.5
```

### Mistake 3: Overflow
```cpp
❌ short x = 32767;
   x = x + 1;  // Overflow! x becomes -32768

✅ int x = 32767;
   x = x + 1;  // x = 32768 (safe)
```

### Mistake 4: Float Precision
```cpp
❌ float pi = 3.14159265359;  // Loses precision

✅ double pi = 3.14159265359;  // Better precision
```

---

## Key Takeaways

1. ✅ Variables store data in memory
2. ✅ Choose appropriate data types (int, double, string, etc.)
3. ✅ Always initialize variables
4. ✅ Use `const` for constants
5. ✅ `std::cin` reads input, `std::cout` writes output
6. ✅ Be careful with integer division
7. ✅ Use `std::cin.ignore()` when mixing >> and getline()
8. ✅ Use meaningful variable names

---

## Summary Checklist

Before moving to Lesson 3, ensure you can:
- [ ] Declare and initialize variables
- [ ] Choose appropriate data types
- [ ] Use integer, floating-point, character, boolean, and string types
- [ ] Read input with std::cin and std::getline
- [ ] Display output with std::cout
- [ ] Understand type sizes with sizeof()
- [ ] Create constants with const
- [ ] Avoid common mistakes (uninitialized variables, overflow, etc.)
- [ ] Format output with std::setprecision and std::fixed

---

## Next Lesson Preview

In **Lesson 3**, we'll learn about:
- Arithmetic operators (+, -, *, /, %)
- Compound assignment operators (+=, -=, etc.)
- Increment and decrement (++, --)
- Operator precedence
- Type casting

**Great job completing Lesson 2!** 🚀
