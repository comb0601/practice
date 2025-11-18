# Lesson 03: Variables and Data Types
**Duration: 3 hours**

## Learning Objectives
By the end of this lesson, you will:
- Understand what variables are and how to declare them
- Know all fundamental data types in C++
- Understand type sizes and ranges
- Work with constants
- Perform type conversions
- Use the auto keyword
- Apply proper variable naming conventions

## 1. What is a Variable?

A variable is a named storage location in memory that holds a value.

```cpp
int age = 25;
//  │    │  └─ Value (what's stored)
//  │    └─ Assignment operator
//  └─ Variable name (identifier)
// └─ Data type (what kind of data)
```

### Variable Declaration vs Initialization

```cpp
// Declaration (creating the variable)
int x;              // Declared but not initialized (contains garbage)

// Initialization (first assignment)
int y = 10;         // Declaration + initialization

// Declaration then assignment
int z;              // Declaration
z = 20;             // Assignment (not initialization)

// Multiple declarations
int a, b, c;        // Three variables declared

// Multiple with initialization
int d = 1, e = 2, f = 3;
```

## 2. Fundamental Data Types

### Integer Types

```cpp
#include <iostream>
#include <climits>  // For INT_MAX, INT_MIN, etc.

int main() {
    // short: Small integers (at least 16 bits)
    short smallNum = 32000;
    std::cout << "short size: " << sizeof(short) << " bytes" << std::endl;
    std::cout << "short range: " << SHRT_MIN << " to " << SHRT_MAX << std::endl;

    // int: Standard integers (usually 32 bits)
    int normalNum = 2147483647;
    std::cout << "int size: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "int range: " << INT_MIN << " to " << INT_MAX << std::endl;

    // long: Large integers (at least 32 bits)
    long bigNum = 2147483647L;
    std::cout << "long size: " << sizeof(long) << " bytes" << std::endl;
    std::cout << "long range: " << LONG_MIN << " to " << LONG_MAX << std::endl;

    // long long: Very large integers (at least 64 bits)
    long long veryBigNum = 9223372036854775807LL;
    std::cout << "long long size: " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "long long range: " << LLONG_MIN << " to " << LLONG_MAX << std::endl;

    return 0;
}
```

### Unsigned Integer Types

```cpp
#include <iostream>
#include <climits>

int main() {
    // unsigned: No negative numbers, double the positive range
    unsigned short us = 65535;
    unsigned int ui = 4294967295U;
    unsigned long ul = 4294967295UL;
    unsigned long long ull = 18446744073709551615ULL;

    std::cout << "unsigned short max: " << USHRT_MAX << std::endl;
    std::cout << "unsigned int max: " << UINT_MAX << std::endl;
    std::cout << "unsigned long max: " << ULONG_MAX << std::endl;
    std::cout << "unsigned long long max: " << ULLONG_MAX << std::endl;

    // When to use unsigned
    unsigned int arraySize = 100;    // Sizes are never negative
    unsigned int age = 25;           // Age is never negative

    // Warning: Unsigned arithmetic wraps around!
    unsigned int x = 0;
    x = x - 1;  // x is now UINT_MAX, not -1!
    std::cout << "0 - 1 (unsigned) = " << x << std::endl;

    return 0;
}
```

### Floating-Point Types

```cpp
#include <iostream>
#include <cfloat>
#include <iomanip>  // For std::setprecision

int main() {
    // float: Single precision (32 bits, ~7 decimal digits)
    float smallDecimal = 3.14159f;  // Note the 'f' suffix
    std::cout << "float size: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "float precision: ~" << FLT_DIG << " decimal digits" << std::endl;
    std::cout << "float range: " << FLT_MIN << " to " << FLT_MAX << std::endl;

    // double: Double precision (64 bits, ~15 decimal digits)
    double normalDecimal = 3.14159265358979;
    std::cout << "double size: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "double precision: ~" << DBL_DIG << " decimal digits" << std::endl;
    std::cout << "double range: " << DBL_MIN << " to " << DBL_MAX << std::endl;

    // long double: Extended precision (varies, at least 64 bits)
    long double bigDecimal = 3.14159265358979323846L;
    std::cout << "long double size: " << sizeof(long double) << " bytes" << std::endl;
    std::cout << "long double precision: ~" << LDBL_DIG << " decimal digits" << std::endl;

    // Demonstrating precision
    float f = 0.123456789f;
    double d = 0.123456789012345;

    std::cout << std::setprecision(10);
    std::cout << "float:  " << f << std::endl;  // Less precision
    std::cout << "double: " << d << std::endl;  // More precision

    return 0;
}
```

### Character Types

```cpp
#include <iostream>

int main() {
    // char: Single character (8 bits)
    char letter = 'A';              // Single quotes for char
    char digit = '5';               // Characters, not numbers!
    char symbol = '@';

    std::cout << "Character: " << letter << std::endl;
    std::cout << "ASCII value: " << static_cast<int>(letter) << std::endl;

    // char can also store small integers
    char smallInt = 65;             // Same as 'A' in ASCII
    std::cout << "smallInt as char: " << smallInt << std::endl;

    // Special characters (escape sequences)
    char newline = '\n';            // Newline
    char tab = '\t';                // Tab
    char backslash = '\\';          // Backslash
    char quote = '\'';              // Single quote
    char null = '\0';               // Null character

    std::cout << "Line1" << newline << "Line2" << std::endl;
    std::cout << "Column1" << tab << "Column2" << std::endl;

    // Wide characters (for Unicode)
    wchar_t wideChar = L'あ';       // Japanese character
    char16_t utf16Char = u'€';      // Euro symbol (UTF-16)
    char32_t utf32Char = U'😀';     // Emoji (UTF-32)

    return 0;
}
```

### Boolean Type

```cpp
#include <iostream>

int main() {
    // bool: true or false (1 byte, but only uses 1 bit conceptually)
    bool isStudent = true;
    bool hasLicense = false;

    std::cout << "bool size: " << sizeof(bool) << " byte" << std::endl;
    std::cout << "isStudent: " << isStudent << std::endl;  // Prints 1
    std::cout << "hasLicense: " << hasLicense << std::endl;  // Prints 0

    // Print as true/false instead of 1/0
    std::cout << std::boolalpha;
    std::cout << "isStudent: " << isStudent << std::endl;  // Prints true
    std::cout << "hasLicense: " << hasLicense << std::endl;  // Prints false

    // Any non-zero value is true
    bool test1 = 42;        // true
    bool test2 = -1;        // true
    bool test3 = 0;         // false

    std::cout << "test1 (42): " << test1 << std::endl;
    std::cout << "test2 (-1): " << test2 << std::endl;
    std::cout << "test3 (0): " << test3 << std::endl;

    return 0;
}
```

## 3. Type Sizes and Ranges

### sizeof Operator

```cpp
#include <iostream>

int main() {
    std::cout << "=== Data Type Sizes ===" << std::endl;
    std::cout << "char: " << sizeof(char) << " byte" << std::endl;
    std::cout << "short: " << sizeof(short) << " bytes" << std::endl;
    std::cout << "int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "long: " << sizeof(long) << " bytes" << std::endl;
    std::cout << "long long: " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "float: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "double: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "long double: " << sizeof(long double) << " bytes" << std::endl;
    std::cout << "bool: " << sizeof(bool) << " byte" << std::endl;

    // sizeof can be used on variables too
    int x = 10;
    std::cout << "sizeof(x): " << sizeof(x) << " bytes" << std::endl;

    // sizeof returns size_t (unsigned integer type)
    std::cout << "sizeof returns type: size_t" << std::endl;

    return 0;
}
```

### Complete Type Reference Table

```cpp
// complete_types.cpp
#include <iostream>
#include <climits>
#include <cfloat>

int main() {
    std::cout << "=== Integer Types ===" << std::endl;
    std::cout << "Type\t\t\tSize\tMin\t\t\tMax" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "char\t\t\t" << sizeof(char) << "\t" << CHAR_MIN << "\t\t\t" << CHAR_MAX << std::endl;
    std::cout << "unsigned char\t\t" << sizeof(unsigned char) << "\t" << 0 << "\t\t\t" << UCHAR_MAX << std::endl;
    std::cout << "short\t\t\t" << sizeof(short) << "\t" << SHRT_MIN << "\t\t" << SHRT_MAX << std::endl;
    std::cout << "unsigned short\t\t" << sizeof(unsigned short) << "\t" << 0 << "\t\t\t" << USHRT_MAX << std::endl;
    std::cout << "int\t\t\t" << sizeof(int) << "\t" << INT_MIN << "\t" << INT_MAX << std::endl;
    std::cout << "unsigned int\t\t" << sizeof(unsigned int) << "\t" << 0 << "\t\t\t" << UINT_MAX << std::endl;

    std::cout << "\n=== Floating-Point Types ===" << std::endl;
    std::cout << "Type\t\t\tSize\tPrecision" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "float\t\t\t" << sizeof(float) << "\t~" << FLT_DIG << " digits" << std::endl;
    std::cout << "double\t\t\t" << sizeof(double) << "\t~" << DBL_DIG << " digits" << std::endl;
    std::cout << "long double\t\t" << sizeof(long double) << "\t~" << LDBL_DIG << " digits" << std::endl;

    return 0;
}
```

## 4. Constants

### const Keyword

```cpp
#include <iostream>

int main() {
    // const variables cannot be changed
    const int MAX_STUDENTS = 30;
    const double PI = 3.14159265359;
    const char GRADE = 'A';

    std::cout << "Max students: " << MAX_STUDENTS << std::endl;
    std::cout << "Pi: " << PI << std::endl;

    // This would cause a compilation error:
    // MAX_STUDENTS = 40;  // ERROR: cannot modify const

    // const must be initialized
    // const int X;  // ERROR: uninitialized const

    // Naming convention: UPPERCASE for constants
    const int DAYS_IN_WEEK = 7;
    const double SPEED_OF_LIGHT = 299792458.0;  // m/s

    return 0;
}
```

### #define Preprocessor Directive

```cpp
#include <iostream>

// #define creates a macro (text replacement)
#define MAX_SIZE 100
#define PI 3.14159
#define GREETING "Hello, World!"

int main() {
    std::cout << "Max size: " << MAX_SIZE << std::endl;
    std::cout << "Pi: " << PI << std::endl;
    std::cout << GREETING << std::endl;

    // Prefer const over #define in modern C++
    // Reasons:
    // 1. Type safety
    // 2. Scope control
    // 3. Debugger support

    return 0;
}
```

### constexpr (C++11 and later)

```cpp
#include <iostream>

// constexpr: Evaluated at compile time
constexpr int square(int x) {
    return x * x;
}

int main() {
    constexpr int SIZE = 10;
    constexpr int AREA = square(5);  // Computed at compile time

    std::cout << "Size: " << SIZE << std::endl;
    std::cout << "Area: " << AREA << std::endl;

    // Creates compile-time array
    int array[SIZE];  // SIZE must be known at compile time

    return 0;
}
```

## 5. Type Conversions

### Implicit Conversion (Automatic)

```cpp
#include <iostream>

int main() {
    // Widening conversions (safe)
    int i = 42;
    long l = i;          // int to long - OK
    double d = i;        // int to double - OK

    std::cout << "int: " << i << std::endl;
    std::cout << "long: " << l << std::endl;
    std::cout << "double: " << d << std::endl;

    // Narrowing conversions (may lose data - warning)
    double pi = 3.14159;
    int truncated = pi;  // Loses decimal part - WARNING

    std::cout << "Original: " << pi << std::endl;
    std::cout << "Truncated: " << truncated << std::endl;  // Prints 3

    // Mixed type arithmetic
    int a = 5;
    double b = 2.5;
    double result = a + b;  // int promoted to double

    std::cout << "5 + 2.5 = " << result << std::endl;  // 7.5

    return 0;
}
```

### Explicit Conversion (Casting)

```cpp
#include <iostream>

int main() {
    // C-style cast (avoid in modern C++)
    double pi = 3.14159;
    int i = (int)pi;

    // C++ style casts (preferred)

    // 1. static_cast: Compile-time conversions
    double d = 3.14159;
    int x = static_cast<int>(d);
    std::cout << "static_cast: " << x << std::endl;

    // 2. For viewing char as integer
    char ch = 'A';
    std::cout << "Character: " << ch << std::endl;
    std::cout << "ASCII: " << static_cast<int>(ch) << std::endl;

    // 3. Integer division vs float division
    int a = 7;
    int b = 2;

    std::cout << "Integer division: " << a / b << std::endl;  // 3
    std::cout << "Float division: " << static_cast<double>(a) / b << std::endl;  // 3.5

    // 4. Explicit conversions for clarity
    double price = 19.99;
    int dollars = static_cast<int>(price);
    int cents = static_cast<int>((price - dollars) * 100);

    std::cout << "Price: $" << dollars << "." << cents << std::endl;

    return 0;
}
```

## 6. The auto Keyword (C++11)

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
    // auto: Compiler deduces the type
    auto x = 42;              // int
    auto y = 3.14;            // double
    auto z = 'A';             // char
    auto s = "Hello";         // const char*
    auto str = std::string("World");  // std::string
    auto flag = true;         // bool

    std::cout << "x (int): " << x << std::endl;
    std::cout << "y (double): " << y << std::endl;
    std::cout << "z (char): " << z << std::endl;

    // Useful for complex types
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto it = numbers.begin();  // std::vector<int>::iterator

    // Must be initialized (compiler needs to deduce type)
    // auto uninitialized;  // ERROR: cannot deduce type

    // Type is fixed after initialization
    auto value = 10;    // int
    // value = 3.14;    // Still int, will be truncated to 3

    return 0;
}
```

## 7. Variable Naming Conventions

### Rules (Enforced by Compiler)

```cpp
// VALID names
int age;
int studentAge;
int student_age;
int _private;
int value123;
int MAX_SIZE;

// INVALID names
// int 123value;     // Can't start with digit
// int student-age;  // Hyphens not allowed
// int student age;  // Spaces not allowed
// int int;          // Can't use keywords
// int for;          // Can't use keywords
```

### Conventions (Best Practices)

```cpp
// Camel Case (common in C++)
int studentAge;
int maxScore;
int numberOfStudents;

// Snake Case (also common)
int student_age;
int max_score;
int number_of_students;

// Pascal Case (for classes, we'll cover later)
// class StudentRecord { };
// class DatabaseConnection { };

// UPPERCASE for constants
const int MAX_STUDENTS = 100;
const double PI = 3.14159;

// Descriptive names
int s;          // BAD: What is s?
int score;      // GOOD: Clear meaning

int x;          // BAD: Generic
int playerX;    // GOOD: Specific purpose

// Avoid abbreviations (unless very common)
int num;        // OK: num is widely understood
int cnt;        // BAD: Use count
int msg;        // BAD: Use message

// Boolean names should be questions
bool isStudent;
bool hasLicense;
bool canVote;
bool isValid;
```

## 8. Practical Examples

### Example 1: Data Type Showcase

```cpp
// data_types_demo.cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    // Personal information using various types
    std::string name = "John Doe";
    int age = 25;
    double height = 1.75;  // meters
    float weight = 70.5f;  // kg
    char bloodType = 'O';
    bool isStudent = true;
    const int BIRTH_YEAR = 1998;

    // Display information
    std::cout << "=== Personal Information ===" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << " years" << std::endl;
    std::cout << "Height: " << std::fixed << std::setprecision(2) << height << " m" << std::endl;
    std::cout << "Weight: " << weight << " kg" << std::endl;
    std::cout << "Blood Type: " << bloodType << std::endl;
    std::cout << std::boolalpha;
    std::cout << "Student: " << isStudent << std::endl;
    std::cout << "Birth Year: " << BIRTH_YEAR << std::endl;

    // Calculate BMI
    double bmi = weight / (height * height);
    std::cout << "\nBMI: " << std::setprecision(1) << bmi << std::endl;

    return 0;
}
```

### Example 2: Scientific Calculator

```cpp
// scientific_calc.cpp
#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    const double PI = 3.14159265359;
    const double E = 2.71828182846;

    double radius, angle, temperature;

    std::cout << "=== Scientific Calculator ===" << std::endl;

    // Circle calculations
    std::cout << "\nEnter circle radius: ";
    std::cin >> radius;

    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Circumference: " << circumference << std::endl;
    std::cout << "Area: " << area << std::endl;

    // Trigonometry
    std::cout << "\nEnter angle in degrees: ";
    std::cin >> angle;

    double radians = angle * PI / 180.0;
    std::cout << "Sine: " << std::sin(radians) << std::endl;
    std::cout << "Cosine: " << std::cos(radians) << std::endl;
    std::cout << "Tangent: " << std::tan(radians) << std::endl;

    // Temperature conversion
    std::cout << "\nEnter temperature in Celsius: ";
    std::cin >> temperature;

    double fahrenheit = (temperature * 9.0 / 5.0) + 32.0;
    double kelvin = temperature + 273.15;

    std::cout << "Fahrenheit: " << fahrenheit << "°F" << std::endl;
    std::cout << "Kelvin: " << kelvin << " K" << std::endl;

    return 0;
}
```

### Example 3: Type Conversion Demonstration

```cpp
// conversions.cpp
#include <iostream>

int main() {
    std::cout << "=== Type Conversion Examples ===" << std::endl;

    // Integer division vs floating-point division
    int a = 7, b = 2;
    std::cout << "\nInteger division: " << a << " / " << b << " = " << (a / b) << std::endl;
    std::cout << "Float division: " << a << " / " << b << " = "
              << (static_cast<double>(a) / b) << std::endl;

    // Truncation
    double pi = 3.14159;
    int piTruncated = static_cast<int>(pi);
    std::cout << "\nOriginal: " << pi << std::endl;
    std::cout << "Truncated: " << piTruncated << std::endl;

    // Character to ASCII
    char letter = 'A';
    std::cout << "\nCharacter: " << letter << std::endl;
    std::cout << "ASCII value: " << static_cast<int>(letter) << std::endl;

    // ASCII to character
    int asciiValue = 66;
    char character = static_cast<char>(asciiValue);
    std::cout << "\nASCII " << asciiValue << " is character: " << character << std::endl;

    // Mixed arithmetic
    int intValue = 10;
    double doubleValue = 3.5;
    auto result = intValue + doubleValue;  // Result is double
    std::cout << "\nint(10) + double(3.5) = " << result << std::endl;

    return 0;
}
```

## 9. Exercises

### Exercise 1: Variable Declaration
Declare variables for:
1. Your age (integer)
2. Your GPA (floating-point)
3. Your first initial (character)
4. Whether you have a pet (boolean)
5. Your favorite number (constant integer)

### Exercise 2: Type Sizes
Write a program that displays the size of all fundamental types on your system.

### Exercise 3: Temperature Table
Create a program that displays a Celsius to Fahrenheit conversion table for 0°C to 100°C in steps of 10.

### Exercise 4: Circle Calculator
Write a program that:
1. Asks for a circle's radius
2. Calculates diameter, circumference, and area
3. Displays results with 2 decimal places

### Exercise 5: Type Conversions
Write a program demonstrating:
1. Integer to double conversion
2. Double to integer conversion (truncation)
3. Character to integer (ASCII)
4. Integer division vs float division

## 10. Exercise Solutions

### Solution 1: Variable Declaration
```cpp
#include <iostream>

int main() {
    int age = 25;
    double gpa = 3.75;
    char initial = 'J';
    bool hasPet = true;
    const int FAVORITE_NUMBER = 7;

    std::cout << "Age: " << age << std::endl;
    std::cout << "GPA: " << gpa << std::endl;
    std::cout << "Initial: " << initial << std::endl;
    std::cout << std::boolalpha << "Has pet: " << hasPet << std::endl;
    std::cout << "Favorite number: " << FAVORITE_NUMBER << std::endl;

    return 0;
}
```

### Solution 2: Type Sizes
```cpp
#include <iostream>

int main() {
    std::cout << "=== Data Type Sizes ===" << std::endl;
    std::cout << "bool: " << sizeof(bool) << " byte(s)" << std::endl;
    std::cout << "char: " << sizeof(char) << " byte(s)" << std::endl;
    std::cout << "short: " << sizeof(short) << " byte(s)" << std::endl;
    std::cout << "int: " << sizeof(int) << " byte(s)" << std::endl;
    std::cout << "long: " << sizeof(long) << " byte(s)" << std::endl;
    std::cout << "long long: " << sizeof(long long) << " byte(s)" << std::endl;
    std::cout << "float: " << sizeof(float) << " byte(s)" << std::endl;
    std::cout << "double: " << sizeof(double) << " byte(s)" << std::endl;
    std::cout << "long double: " << sizeof(long double) << " byte(s)" << std::endl;
    return 0;
}
```

### Solution 3: Temperature Table
```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "Celsius\tFahrenheit" << std::endl;
    std::cout << "-------------------" << std::endl;

    for (int celsius = 0; celsius <= 100; celsius += 10) {
        double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
        std::cout << std::setw(7) << celsius << "\t"
                  << std::fixed << std::setprecision(1)
                  << fahrenheit << std::endl;
    }

    return 0;
}
```

### Solution 4: Circle Calculator
```cpp
#include <iostream>
#include <iomanip>

int main() {
    const double PI = 3.14159265359;
    double radius;

    std::cout << "Enter circle radius: ";
    std::cin >> radius;

    double diameter = 2 * radius;
    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Circle Properties ===" << std::endl;
    std::cout << "Radius: " << radius << std::endl;
    std::cout << "Diameter: " << diameter << std::endl;
    std::cout << "Circumference: " << circumference << std::endl;
    std::cout << "Area: " << area << std::endl;

    return 0;
}
```

### Solution 5: Type Conversions
```cpp
#include <iostream>

int main() {
    std::cout << "=== Type Conversion Demonstrations ===" << std::endl;

    // Int to double
    int intNum = 42;
    double doubleNum = static_cast<double>(intNum);
    std::cout << "\nInt to double: " << intNum << " -> " << doubleNum << std::endl;

    // Double to int (truncation)
    double pi = 3.14159;
    int truncated = static_cast<int>(pi);
    std::cout << "Double to int: " << pi << " -> " << truncated << std::endl;

    // Char to int (ASCII)
    char letter = 'Z';
    int ascii = static_cast<int>(letter);
    std::cout << "Char to ASCII: '" << letter << "' -> " << ascii << std::endl;

    // Integer division vs float division
    int a = 7, b = 2;
    std::cout << "\nInteger division: " << a << " / " << b << " = " << (a / b) << std::endl;
    std::cout << "Float division: " << a << " / " << b << " = "
              << (static_cast<double>(a) / b) << std::endl;

    return 0;
}
```

## Summary

### Key Concepts
- Variables store data in memory
- C++ has many fundamental types (int, double, char, bool, etc.)
- Each type has a specific size and range
- Use `const` for values that don't change
- Type conversions can be implicit or explicit
- `auto` lets the compiler deduce types
- Choose meaningful variable names

### Type Selection Guidelines
- **int**: Default choice for integers
- **double**: Default choice for decimals
- **float**: When memory is constrained
- **bool**: For true/false values
- **char**: For single characters
- **unsigned**: When values are never negative
- **const**: For values that never change

## Checklist

- [ ] Understand what variables are
- [ ] Know all fundamental data types
- [ ] Can use sizeof to check type sizes
- [ ] Understand signed vs unsigned integers
- [ ] Know when to use float vs double
- [ ] Can create and use constants
- [ ] Understand type conversions
- [ ] Can use auto keyword
- [ ] Follow proper naming conventions
- [ ] Completed all exercises

---

**Time to complete**: 3 hours
**Previous lesson**: [Lesson 02 - Your First C++ Program](../Lesson-02-First-Program/README.md)
**Next lesson**: [Lesson 04 - Operators and Expressions](../Lesson-04-Operators/README.md)
