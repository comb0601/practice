# Lesson 02: Variables and Data Types

**Duration: 3 hours**

## Table of Contents
1. Understanding Variables
2. Fundamental Data Types
3. Variable Declaration and Initialization
4. Constants
5. Type Modifiers
6. Type Casting
7. Auto Keyword
8. Practical Exercises

## 1. Understanding Variables

A variable is a named storage location in memory that holds a value.

### Syntax:
```cpp
data_type variable_name = value;
```

### Naming Rules:
- Must start with letter or underscore
- Can contain letters, digits, underscores
- Case-sensitive
- Cannot use C++ keywords
- Use meaningful names

### Good vs Bad Names:
```cpp
// Good names
int age;
double salary;
std::string firstName;
int studentCount;

// Bad names
int a;        // Too short, unclear
int 123abc;   // Starts with number (ERROR)
int my-var;   // Hyphens not allowed (ERROR)
int int;      // Keyword (ERROR)
```

## 2. Fundamental Data Types

### Integer Types

```cpp
#include <iostream>
#include <climits>  // For INT_MIN, INT_MAX, etc.

int main() {
    // Integer types and their sizes

    // char: 1 byte (-128 to 127 or 0 to 255)
    char letter = 'A';
    std::cout << "char size: " << sizeof(char) << " byte" << std::endl;
    std::cout << "char value: " << letter << std::endl;

    // short: 2 bytes (-32,768 to 32,767)
    short smallNumber = 1000;
    std::cout << "short size: " << sizeof(short) << " bytes" << std::endl;
    std::cout << "short range: " << SHRT_MIN << " to " << SHRT_MAX << std::endl;

    // int: 4 bytes (-2,147,483,648 to 2,147,483,647)
    int number = 100000;
    std::cout << "int size: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "int range: " << INT_MIN << " to " << INT_MAX << std::endl;

    // long: 4 or 8 bytes (platform dependent)
    long bigNumber = 1000000L;
    std::cout << "long size: " << sizeof(long) << " bytes" << std::endl;
    std::cout << "long range: " << LONG_MIN << " to " << LONG_MAX << std::endl;

    // long long: 8 bytes (guaranteed)
    long long veryBigNumber = 9223372036854775807LL;
    std::cout << "long long size: " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "long long range: " << LLONG_MIN << " to " << LLONG_MAX << std::endl;

    return 0;
}
```

### Floating-Point Types

```cpp
#include <iostream>
#include <iomanip>  // For setprecision
#include <cfloat>   // For FLT_MIN, DBL_MAX, etc.

int main() {
    // float: 4 bytes, ~7 decimal digits precision
    float pi_float = 3.14159265359f;  // 'f' suffix for float
    std::cout << "float size: " << sizeof(float) << " bytes" << std::endl;
    std::cout << std::setprecision(10);
    std::cout << "float value: " << pi_float << std::endl;
    std::cout << "float range: " << FLT_MIN << " to " << FLT_MAX << std::endl;

    // double: 8 bytes, ~15 decimal digits precision
    double pi_double = 3.14159265359;
    std::cout << "double size: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "double value: " << pi_double << std::endl;
    std::cout << "double range: " << DBL_MIN << " to " << DBL_MAX << std::endl;

    // long double: 8-16 bytes (platform dependent)
    long double pi_long_double = 3.14159265359L;  // 'L' suffix
    std::cout << "long double size: " << sizeof(long double) << " bytes" << std::endl;
    std::cout << "long double value: " << pi_long_double << std::endl;

    return 0;
}
```

### Boolean Type

```cpp
#include <iostream>

int main() {
    bool isTrue = true;
    bool isFalse = false;

    std::cout << "bool size: " << sizeof(bool) << " byte" << std::endl;
    std::cout << "isTrue: " << isTrue << std::endl;        // Prints: 1
    std::cout << "isFalse: " << isFalse << std::endl;      // Prints: 0

    // Better output with boolalpha
    std::cout << std::boolalpha;
    std::cout << "isTrue: " << isTrue << std::endl;        // Prints: true
    std::cout << "isFalse: " << isFalse << std::endl;      // Prints: false

    return 0;
}
```

### Character Types

```cpp
#include <iostream>

int main() {
    // char: 1 byte
    char letter = 'A';
    char digit = '5';
    char symbol = '$';

    std::cout << "Characters: " << letter << digit << symbol << std::endl;

    // ASCII values
    std::cout << "ASCII value of 'A': " << static_cast<int>(letter) << std::endl;

    // Special characters
    char newline = '\n';
    char tab = '\t';
    char backslash = '\\';
    char quote = '\'';

    std::cout << "Special" << tab << "Characters" << newline;

    // Wide character types (for Unicode)
    wchar_t wideChar = L'Ω';
    char16_t char16 = u'Ω';
    char32_t char32 = U'Ω';

    return 0;
}
```

## 3. Variable Declaration and Initialization

### Declaration Without Initialization
```cpp
#include <iostream>

int main() {
    int x;              // Declared but not initialized (contains garbage)
    std::cout << x << std::endl;  // Undefined behavior!

    return 0;
}
```

### Initialization Methods

```cpp
#include <iostream>

int main() {
    // Method 1: C-like initialization
    int a = 10;

    // Method 2: Constructor initialization
    int b(20);

    // Method 3: Uniform initialization (C++11) - RECOMMENDED
    int c{30};

    // Method 4: Copy initialization
    int d = {40};

    // Multiple declarations
    int x = 1, y = 2, z = 3;

    // Mixed declaration (not recommended)
    int i = 5, j, k = 10;  // j is uninitialized!

    std::cout << "a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << std::endl;

    return 0;
}
```

### Uniform Initialization Benefits

```cpp
#include <iostream>

int main() {
    // Prevents narrowing conversions
    int x{3.14};        // ERROR: narrowing conversion
    // int y = 3.14;    // WARNING: allowed but loses precision

    // Initialization with zero
    int zero{};         // Initialized to 0
    double dzero{};     // Initialized to 0.0
    bool bzero{};       // Initialized to false

    std::cout << "zero=" << zero << ", dzero=" << dzero << std::endl;

    return 0;
}
```

## 4. Constants

### Using const

```cpp
#include <iostream>

int main() {
    const int MAX_STUDENTS = 100;
    const double PI = 3.14159265359;
    const char GRADE = 'A';

    std::cout << "Max students: " << MAX_STUDENTS << std::endl;

    // MAX_STUDENTS = 200;  // ERROR: cannot modify const

    return 0;
}
```

### Using constexpr (C++11)

```cpp
#include <iostream>

// constexpr ensures compile-time evaluation
constexpr int square(int x) {
    return x * x;
}

int main() {
    constexpr int SIZE = 10;
    constexpr int AREA = square(5);  // Evaluated at compile time

    int array[SIZE];  // OK: SIZE is known at compile time

    std::cout << "Area: " << AREA << std::endl;

    return 0;
}
```

### Using #define (Preprocessor Macro)

```cpp
#include <iostream>

#define PI 3.14159
#define MAX_SIZE 100
#define SQUARE(x) ((x) * (x))  // Macro function

int main() {
    std::cout << "PI: " << PI << std::endl;
    std::cout << "Square of 5: " << SQUARE(5) << std::endl;

    // Problem with macros
    std::cout << "Square of 2+3: " << SQUARE(2+3) << std::endl;
    // Expands to: ((2+3) * (2+3)) = 25, not 2 + 9 = 11

    return 0;
}
```

### Const vs Constexpr vs #define

```cpp
#include <iostream>

#define MACRO_PI 3.14159          // Preprocessor replacement
const double CONST_PI = 3.14159;  // Runtime constant
constexpr double EXPR_PI = 3.14159;  // Compile-time constant

int main() {
    // CONST_PI: Type-safe, scoped, debuggable
    // EXPR_PI: Compile-time evaluation, type-safe
    // MACRO_PI: No type safety, no scope, hard to debug

    // RECOMMENDATION: Use constexpr for constants

    return 0;
}
```

## 5. Type Modifiers

### Signed and Unsigned

```cpp
#include <iostream>
#include <climits>

int main() {
    // Signed (default) - can be negative
    signed int signedInt = -100;
    int defaultInt = -50;  // signed by default

    // Unsigned - only positive (0 and above)
    unsigned int unsignedInt = 100;

    std::cout << "Signed int range: " << INT_MIN << " to " << INT_MAX << std::endl;
    std::cout << "Unsigned int range: 0 to " << UINT_MAX << std::endl;

    // Overflow example
    unsigned int x = 0;
    x = x - 1;  // Wraps around!
    std::cout << "0 - 1 for unsigned: " << x << std::endl;  // Prints: 4294967295

    return 0;
}
```

### Short and Long

```cpp
#include <iostream>

int main() {
    short int shortInt = 32767;
    long int longInt = 2147483647L;
    long long int longLongInt = 9223372036854775807LL;

    unsigned short ushort = 65535;
    unsigned long ulong = 4294967295UL;
    unsigned long long ulonglong = 18446744073709551615ULL;

    std::cout << "short: " << sizeof(shortInt) << " bytes" << std::endl;
    std::cout << "long: " << sizeof(longInt) << " bytes" << std::endl;
    std::cout << "long long: " << sizeof(longLongInt) << " bytes" << std::endl;

    return 0;
}
```

## 6. Type Casting

### Implicit Casting (Automatic)

```cpp
#include <iostream>

int main() {
    int intVal = 10;
    double doubleVal = intVal;  // Automatic: int -> double

    std::cout << "doubleVal: " << doubleVal << std::endl;  // 10.0

    // Data loss example
    double pi = 3.14159;
    int truncated = pi;  // Automatic: double -> int (loses decimal)

    std::cout << "truncated: " << truncated << std::endl;  // 3

    return 0;
}
```

### Explicit Casting (C-style)

```cpp
#include <iostream>

int main() {
    double pi = 3.14159;

    // C-style cast
    int intPi = (int)pi;

    std::cout << "intPi: " << intPi << std::endl;  // 3

    return 0;
}
```

### C++ Style Casts (Recommended)

```cpp
#include <iostream>

int main() {
    // static_cast: Compile-time type conversion
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);

    std::cout << "intPi: " << intPi << std::endl;

    // Converting char to int (ASCII value)
    char letter = 'A';
    int ascii = static_cast<int>(letter);

    std::cout << "ASCII of 'A': " << ascii << std::endl;  // 65

    // Converting int to char
    int num = 66;
    char converted = static_cast<char>(num);

    std::cout << "Char from 66: " << converted << std::endl;  // B

    return 0;
}
```

### Other C++ Casts

```cpp
#include <iostream>

int main() {
    // const_cast: Remove const qualifier (use with caution!)
    const int constVal = 100;
    int* modifiable = const_cast<int*>(&constVal);
    // *modifiable = 200;  // Undefined behavior!

    // reinterpret_cast: Low-level reinterpretation (dangerous!)
    int num = 65;
    char* charPtr = reinterpret_cast<char*>(&num);

    // dynamic_cast: Runtime polymorphic casting (covered later)

    return 0;
}
```

## 7. Auto Keyword (C++11)

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Auto type deduction
    auto intVal = 42;           // int
    auto doubleVal = 3.14;      // double
    auto charVal = 'A';         // char
    auto stringVal = "Hello";   // const char*

    std::cout << "intVal type: int" << std::endl;
    std::cout << "doubleVal type: double" << std::endl;

    // Useful with complex types
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Instead of:
    // std::vector<int>::iterator it = numbers.begin();
    // Use:
    auto it = numbers.begin();

    // Auto with const and references
    const auto constAuto = 100;
    auto& refAuto = intVal;

    return 0;
}
```

## 8. Practical Exercises

### Exercise 1: Variable Declaration Practice

```cpp
#include <iostream>

int main() {
    // Declare variables of different types
    int age = 25;
    double height = 175.5;
    char grade = 'A';
    bool isPassing = true;

    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << " cm" << std::endl;
    std::cout << "Grade: " << grade << std::endl;
    std::cout << std::boolalpha << "Passing: " << isPassing << std::endl;

    return 0;
}
```

### Exercise 2: Temperature Converter

```cpp
#include <iostream>

int main() {
    double celsius = 25.0;
    double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

    std::cout << celsius << "°C = " << fahrenheit << "°F" << std::endl;

    return 0;
}
```

### Exercise 3: Type Sizes and Ranges

```cpp
#include <iostream>
#include <climits>
#include <cfloat>

int main() {
    std::cout << "=== Integer Types ===" << std::endl;
    std::cout << "char: " << sizeof(char) << " byte, range: "
              << CHAR_MIN << " to " << CHAR_MAX << std::endl;
    std::cout << "short: " << sizeof(short) << " bytes, range: "
              << SHRT_MIN << " to " << SHRT_MAX << std::endl;
    std::cout << "int: " << sizeof(int) << " bytes, range: "
              << INT_MIN << " to " << INT_MAX << std::endl;
    std::cout << "long: " << sizeof(long) << " bytes, range: "
              << LONG_MIN << " to " << LONG_MAX << std::endl;
    std::cout << "long long: " << sizeof(long long) << " bytes, range: "
              << LLONG_MIN << " to " << LLONG_MAX << std::endl;

    std::cout << "\n=== Floating-Point Types ===" << std::endl;
    std::cout << "float: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "double: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "long double: " << sizeof(long double) << " bytes" << std::endl;

    return 0;
}
```

### Exercise 4: Constant Pi Calculator

```cpp
#include <iostream>
#include <iomanip>

int main() {
    const double PI = 3.14159265359;
    double radius = 5.0;

    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Radius: " << radius << std::endl;
    std::cout << "Circumference: " << circumference << std::endl;
    std::cout << "Area: " << area << std::endl;

    return 0;
}
```

### Exercise 5: Type Casting Examples

```cpp
#include <iostream>

int main() {
    // Integer division vs floating-point division
    int a = 7, b = 2;

    std::cout << "Integer division: " << a / b << std::endl;  // 3
    std::cout << "Floating division: " << static_cast<double>(a) / b << std::endl;  // 3.5

    // Character to ASCII
    char letter = 'Z';
    std::cout << "ASCII of 'Z': " << static_cast<int>(letter) << std::endl;

    // ASCII to character
    int ascii = 72;
    std::cout << "Character from 72: " << static_cast<char>(ascii) << std::endl;

    return 0;
}
```

## Complete Project: Student Information System

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    // Student information
    const std::string SCHOOL_NAME = "C++ Programming Academy";
    std::string studentName = "John Doe";
    int studentId = 12345;
    int age = 20;
    char grade = 'A';
    double gpa = 3.85;
    bool isEnrolled = true;

    // Course information
    const int MAX_COURSES = 5;
    int coursesTaken = 4;
    double attendanceRate = 95.5;

    // Financial information
    const double TUITION_PER_CREDIT = 500.0;
    int creditsEnrolled = 15;
    double totalTuition = TUITION_PER_CREDIT * creditsEnrolled;

    // Display information
    std::cout << "=====================================" << std::endl;
    std::cout << "  " << SCHOOL_NAME << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Student Name:     " << studentName << std::endl;
    std::cout << "Student ID:       " << studentId << std::endl;
    std::cout << "Age:              " << age << std::endl;
    std::cout << "Grade:            " << grade << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "GPA:              " << gpa << std::endl;
    std::cout << std::boolalpha;
    std::cout << "Enrolled:         " << isEnrolled << std::endl;
    std::cout << std::endl;

    std::cout << "Courses Taken:    " << coursesTaken << " / " << MAX_COURSES << std::endl;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Attendance Rate:  " << attendanceRate << "%" << std::endl;
    std::cout << std::endl;

    std::cout << "Credits Enrolled: " << creditsEnrolled << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Tuition:    $" << totalTuition << std::endl;
    std::cout << "=====================================" << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Variable declaration and initialization
- Fundamental data types (int, double, char, bool)
- Type modifiers (signed, unsigned, short, long)
- Constants (const, constexpr, #define)
- Type casting (implicit and explicit)
- Auto keyword for type deduction
- Practical applications

## Next Lesson Preview

In Lesson 03, we'll cover:
- Basic input with std::cin
- String handling
- Getline for full line input
- Input validation

## Quiz

1. What is the size of int on most modern systems?
2. What's the difference between const and constexpr?
3. Why is uniform initialization {} preferred?
4. What happens when you assign a double to an int?
5. When should you use auto?

## Homework

1. Create a program that stores and displays personal information
2. Write a unit converter (meters to feet, kg to pounds)
3. Calculate the area of different shapes using constants
4. Experiment with different data types and their ranges
