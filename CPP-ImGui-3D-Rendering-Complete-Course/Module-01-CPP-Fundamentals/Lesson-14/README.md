# Lesson 14: Inline Functions and Const Functions

**Duration:** 2 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand inline functions and their benefits
- Use const functions effectively
- Understand when to use inline vs regular functions
- Apply const correctness in function parameters and return values
- Recognize const member function patterns
- Optimize code with inline functions appropriately

## 1. Inline Functions

### 1.1 Introduction to Inline Functions

Inline functions are expanded at the call site instead of performing a function call. This can eliminate function call overhead for small, frequently-called functions.

```cpp
#include <iostream>
using namespace std;

// Regular function
int addRegular(int a, int b) {
    return a + b;
}

// Inline function
inline int addInline(int a, int b) {
    return a + b;
}

int main() {
    int result1 = addRegular(5, 3);  // Function call overhead
    int result2 = addInline(5, 3);   // Potentially expanded inline

    cout << "Regular: " << result1 << endl;
    cout << "Inline: " << result2 << endl;

    return 0;
}
```

### 1.2 Inline Function Syntax

```cpp
#include <iostream>
using namespace std;

// Inline function declaration and definition
inline int square(int x) {
    return x * x;
}

inline double cube(double x) {
    return x * x * x;
}

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

int main() {
    cout << "Square of 5: " << square(5) << endl;
    cout << "Cube of 3: " << cube(3.0) << endl;
    cout << "Max(10, 20): " << max(10, 20) << endl;
    cout << "Min(10, 20): " << min(10, 20) << endl;

    return 0;
}
```

### 1.3 Inline Functions in Headers

**math_inline.h:**
```cpp
#pragma once

inline int add(int a, int b) {
    return a + b;
}

inline int multiply(int a, int b) {
    return a * b;
}

inline double circleArea(double radius) {
    const double PI = 3.14159265358979323846;
    return PI * radius * radius;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include "math_inline.h"

using namespace std;

int main() {
    cout << "5 + 3 = " << add(5, 3) << endl;
    cout << "5 * 3 = " << multiply(5, 3) << endl;
    cout << "Circle area (r=5): " << circleArea(5.0) << endl;

    return 0;
}
```

### 1.4 When to Use Inline Functions

**Good candidates for inline:**
```cpp
// Small functions
inline int abs(int x) {
    return (x < 0) ? -x : x;
}

// Simple getters/setters
inline int getX() { return x; }
inline void setX(int value) { x = value; }

// Simple mathematical operations
inline double square(double x) { return x * x; }
```

**NOT good candidates for inline:**
```cpp
// Large functions (compiler will likely ignore inline)
inline void processLargeData() {
    // 50+ lines of code
    // Complex logic
    // Multiple loops
}

// Recursive functions
inline int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);  // Cannot be truly inlined
}
```

## 2. Const Functions

### 2.1 Const Function Parameters

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// Const parameter prevents modification
void printMessage(const char* message) {
    cout << message << endl;
    // message[0] = 'X';  // ERROR: Cannot modify const parameter
}

// Const reference parameter (efficient and safe)
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
        // arr[i] = 0;  // ERROR: Cannot modify const array
    }
    cout << endl;
}

int main() {
    printMessage("Hello, World!");

    int numbers[] = {1, 2, 3, 4, 5};
    printArray(numbers, 5);

    return 0;
}
```

### 2.2 Const Return Values

```cpp
#include <iostream>
using namespace std;

// Return const value (less common for primitives)
const int getValue() {
    return 42;
}

// Return const pointer
const char* getMessage() {
    return "Hello";
}

int main() {
    const int value = getValue();
    cout << "Value: " << value << endl;

    const char* msg = getMessage();
    cout << "Message: " << msg << endl;
    // msg[0] = 'h';  // ERROR: Cannot modify const data

    return 0;
}
```

### 2.3 Const Correctness

```cpp
#include <iostream>
using namespace std;

struct Point {
    double x;
    double y;
};

// Non-const function - can modify point
void movePoint(Point& p, double dx, double dy) {
    p.x += dx;
    p.y += dy;
}

// Const function - cannot modify point
double getDistance(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    // p1.x = 0;  // ERROR: Cannot modify const reference
    return sqrt(dx * dx + dy * dy);
}

// Const function for read-only operations
void printPoint(const Point& p) {
    cout << "(" << p.x << ", " << p.y << ")" << endl;
}

int main() {
    Point p1 = {0, 0};
    Point p2 = {3, 4};

    printPoint(p1);
    movePoint(p1, 1, 1);
    printPoint(p1);

    cout << "Distance: " << getDistance(p1, p2) << endl;

    return 0;
}
```

## 3. Combining Inline and Const

### 3.1 Inline Const Functions

```cpp
#include <iostream>
using namespace std;

struct Rectangle {
    double width;
    double height;
};

// Inline const functions for efficiency and safety
inline double getArea(const Rectangle& r) {
    return r.width * r.height;
}

inline double getPerimeter(const Rectangle& r) {
    return 2 * (r.width + r.height);
}

inline bool isSquare(const Rectangle& r) {
    return r.width == r.height;
}

int main() {
    Rectangle rect = {5.0, 10.0};

    cout << "Area: " << getArea(rect) << endl;
    cout << "Perimeter: " << getPerimeter(rect) << endl;
    cout << "Is square: " << (isSquare(rect) ? "Yes" : "No") << endl;

    return 0;
}
```

### 3.2 Complete Example with Inline and Const

```cpp
#include <iostream>
#include <cstring>
using namespace std;

struct Student {
    int id;
    char name[50];
    double gpa;
};

// Inline const getter functions
inline int getID(const Student& s) {
    return s.id;
}

inline double getGPA(const Student& s) {
    return s.gpa;
}

inline bool isHonorStudent(const Student& s) {
    return s.gpa >= 3.5;
}

// Non-const modifier function
inline void updateGPA(Student& s, double newGPA) {
    s.gpa = newGPA;
}

// Const print function
void printStudent(const Student& s) {
    cout << "ID: " << getID(s) << endl;
    cout << "Name: " << s.name << endl;
    cout << "GPA: " << getGPA(s) << endl;
    cout << "Honor Student: " << (isHonorStudent(s) ? "Yes" : "No") << endl;
}

int main() {
    Student student = {1001, "Alice Johnson", 3.8};

    printStudent(student);

    updateGPA(student, 3.9);

    cout << "\nAfter GPA update:" << endl;
    printStudent(student);

    return 0;
}
```

## 4. Practical Examples

### Example 1: Vector Math Library

```cpp
#include <iostream>
#include <cmath>
using namespace std;

struct Vector2D {
    double x;
    double y;
};

// Inline const operations
inline double magnitude(const Vector2D& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

inline double dotProduct(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline Vector2D add(const Vector2D& v1, const Vector2D& v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

inline Vector2D subtract(const Vector2D& v1, const Vector2D& v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

inline Vector2D scale(const Vector2D& v, double scalar) {
    return {v.x * scalar, v.y * scalar};
}

inline void normalize(Vector2D& v) {
    double mag = magnitude(v);
    if (mag != 0) {
        v.x /= mag;
        v.y /= mag;
    }
}

void printVector(const Vector2D& v) {
    cout << "(" << v.x << ", " << v.y << ")" << endl;
}

int main() {
    Vector2D v1 = {3.0, 4.0};
    Vector2D v2 = {1.0, 2.0};

    cout << "v1: "; printVector(v1);
    cout << "v2: "; printVector(v2);
    cout << "Magnitude of v1: " << magnitude(v1) << endl;
    cout << "Dot product: " << dotProduct(v1, v2) << endl;

    Vector2D sum = add(v1, v2);
    cout << "v1 + v2: "; printVector(sum);

    Vector2D diff = subtract(v1, v2);
    cout << "v1 - v2: "; printVector(diff);

    Vector2D scaled = scale(v1, 2.0);
    cout << "v1 * 2: "; printVector(scaled);

    normalize(v1);
    cout << "Normalized v1: "; printVector(v1);

    return 0;
}
```

### Example 2: Color Utilities

```cpp
#include <iostream>
using namespace std;

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

// Inline const color operations
inline Color mix(const Color& c1, const Color& c2) {
    return {
        static_cast<unsigned char>((c1.r + c2.r) / 2),
        static_cast<unsigned char>((c1.g + c2.g) / 2),
        static_cast<unsigned char>((c1.b + c2.b) / 2)
    };
}

inline int brightness(const Color& c) {
    return (c.r + c.g + c.b) / 3;
}

inline bool isGrayscale(const Color& c) {
    return c.r == c.g && c.g == c.b;
}

inline Color darken(const Color& c, double factor) {
    return {
        static_cast<unsigned char>(c.r * factor),
        static_cast<unsigned char>(c.g * factor),
        static_cast<unsigned char>(c.b * factor)
    };
}

void printColor(const Color& c) {
    cout << "RGB(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ")" << endl;
}

int main() {
    Color red = {255, 0, 0};
    Color blue = {0, 0, 255};

    cout << "Red: "; printColor(red);
    cout << "Blue: "; printColor(blue);

    Color purple = mix(red, blue);
    cout << "Mixed (purple): "; printColor(purple);

    cout << "Brightness of red: " << brightness(red) << endl;
    cout << "Is red grayscale: " << (isGrayscale(red) ? "Yes" : "No") << endl;

    Color darkRed = darken(red, 0.5);
    cout << "Dark red: "; printColor(darkRed);

    return 0;
}
```

### Example 3: Time Utilities

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

struct Time {
    int hours;
    int minutes;
    int seconds;
};

// Inline const time operations
inline int toSeconds(const Time& t) {
    return t.hours * 3600 + t.minutes * 60 + t.seconds;
}

inline Time fromSeconds(int totalSeconds) {
    Time t;
    t.hours = totalSeconds / 3600;
    totalSeconds %= 3600;
    t.minutes = totalSeconds / 60;
    t.seconds = totalSeconds % 60;
    return t;
}

inline bool isBefore(const Time& t1, const Time& t2) {
    return toSeconds(t1) < toSeconds(t2);
}

inline Time addTime(const Time& t1, const Time& t2) {
    return fromSeconds(toSeconds(t1) + toSeconds(t2));
}

inline int differenceInSeconds(const Time& t1, const Time& t2) {
    return toSeconds(t2) - toSeconds(t1);
}

void printTime(const Time& t) {
    cout << setfill('0') << setw(2) << t.hours << ":"
         << setw(2) << t.minutes << ":"
         << setw(2) << t.seconds << endl;
}

int main() {
    Time t1 = {9, 30, 0};
    Time t2 = {14, 45, 30};

    cout << "Time 1: "; printTime(t1);
    cout << "Time 2: "; printTime(t2);

    cout << "T1 in seconds: " << toSeconds(t1) << endl;
    cout << "T1 before T2: " << (isBefore(t1, t2) ? "Yes" : "No") << endl;

    Time sum = addTime(t1, t2);
    cout << "Sum: "; printTime(sum);

    cout << "Difference: " << differenceInSeconds(t1, t2) << " seconds" << endl;

    return 0;
}
```

## 5. Performance Considerations

### 5.1 Inline vs Regular Functions

```cpp
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Regular function
int regularAdd(int a, int b) {
    return a + b;
}

// Inline function
inline int inlineAdd(int a, int b) {
    return a + b;
}

int main() {
    const int iterations = 10000000;

    // Test regular function
    auto start = high_resolution_clock::now();
    int sum1 = 0;
    for (int i = 0; i < iterations; i++) {
        sum1 += regularAdd(i, i + 1);
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end - start);

    // Test inline function
    start = high_resolution_clock::now();
    int sum2 = 0;
    for (int i = 0; i < iterations; i++) {
        sum2 += inlineAdd(i, i + 1);
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end - start);

    cout << "Regular function time: " << duration1.count() << " microseconds" << endl;
    cout << "Inline function time: " << duration2.count() << " microseconds" << endl;

    return 0;
}
```

## 6. Best Practices

### 6.1 Inline Function Guidelines

1. **Use inline for small functions** (1-3 lines)
2. **Place inline functions in headers**
3. **Don't inline large or complex functions**
4. **Compiler decides** - `inline` is a suggestion, not a command
5. **Modern compilers are smart** - they'll inline when appropriate

### 6.2 Const Function Guidelines

1. **Mark parameters const** when they shouldn't be modified
2. **Use const references** for large objects
3. **Return const** when appropriate to prevent modification
4. **Practice const correctness** throughout your codebase

## Exercises

### Exercise 1: Math Utilities
Create a set of inline math utility functions: `abs`, `max`, `min`, `clamp`, and `lerp` (linear interpolation).

### Exercise 2: String Helpers
Write inline const functions for string operations: checking if empty, getting length, comparing, finding character.

### Exercise 3: Range Checking
Implement inline functions for range checking: `inRange`, `isPositive`, `isNegative`, `isZero`, `isEven`, `isOdd`.

### Exercise 4: Temperature Conversion
Create inline const functions for temperature conversions between Celsius, Fahrenheit, and Kelvin.

### Exercise 5: Geometric Shapes
Develop inline const functions for calculating properties of circles, rectangles, and triangles.

## Exercise Solutions

### Solution 1: Math Utilities

```cpp
#include <iostream>
using namespace std;

inline int abs(int x) {
    return (x < 0) ? -x : x;
}

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

inline double clamp(double value, double minVal, double maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

inline double lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

int main() {
    cout << "abs(-5): " << abs(-5) << endl;
    cout << "max(10, 20): " << max(10, 20) << endl;
    cout << "min(10, 20): " << min(10, 20) << endl;
    cout << "clamp(15, 0, 10): " << clamp(15, 0, 10) << endl;
    cout << "lerp(0, 100, 0.5): " << lerp(0, 100, 0.5) << endl;

    return 0;
}
```

### Solution 2: String Helpers

```cpp
#include <iostream>
using namespace std;

inline bool isEmpty(const char* str) {
    return str == nullptr || str[0] == '\0';
}

inline int getLength(const char* str) {
    if (isEmpty(str)) return 0;
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

inline bool areEqual(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return false;
        i++;
    }
    return s1[i] == s2[i];
}

inline bool contains(const char* str, char ch) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ch) return true;
    }
    return false;
}

int main() {
    const char* str1 = "Hello";
    const char* str2 = "Hello";
    const char* str3 = "World";

    cout << "Is empty: " << (isEmpty(str1) ? "Yes" : "No") << endl;
    cout << "Length: " << getLength(str1) << endl;
    cout << "str1 == str2: " << (areEqual(str1, str2) ? "Yes" : "No") << endl;
    cout << "str1 == str3: " << (areEqual(str1, str3) ? "Yes" : "No") << endl;
    cout << "Contains 'l': " << (contains(str1, 'l') ? "Yes" : "No") << endl;

    return 0;
}
```

### Solution 3: Range Checking

```cpp
#include <iostream>
using namespace std;

inline bool inRange(int value, int min, int max) {
    return value >= min && value <= max;
}

inline bool isPositive(int value) {
    return value > 0;
}

inline bool isNegative(int value) {
    return value < 0;
}

inline bool isZero(int value) {
    return value == 0;
}

inline bool isEven(int value) {
    return value % 2 == 0;
}

inline bool isOdd(int value) {
    return value % 2 != 0;
}

int main() {
    int num = 10;

    cout << "Number: " << num << endl;
    cout << "In range [5, 15]: " << (inRange(num, 5, 15) ? "Yes" : "No") << endl;
    cout << "Is positive: " << (isPositive(num) ? "Yes" : "No") << endl;
    cout << "Is negative: " << (isNegative(num) ? "Yes" : "No") << endl;
    cout << "Is zero: " << (isZero(num) ? "Yes" : "No") << endl;
    cout << "Is even: " << (isEven(num) ? "Yes" : "No") << endl;
    cout << "Is odd: " << (isOdd(num) ? "Yes" : "No") << endl;

    return 0;
}
```

### Solution 4: Temperature Conversion

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

inline double celsiusToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

inline double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

inline double celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

inline double kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

inline double fahrenheitToKelvin(double fahrenheit) {
    return celsiusToKelvin(fahrenheitToCelsius(fahrenheit));
}

inline double kelvinToFahrenheit(double kelvin) {
    return celsiusToFahrenheit(kelvinToCelsius(kelvin));
}

int main() {
    cout << fixed << setprecision(2);

    double temp = 25.0;

    cout << temp << "°C = " << celsiusToFahrenheit(temp) << "°F" << endl;
    cout << temp << "°C = " << celsiusToKelvin(temp) << "K" << endl;

    temp = 77.0;
    cout << temp << "°F = " << fahrenheitToCelsius(temp) << "°C" << endl;
    cout << temp << "°F = " << fahrenheitToKelvin(temp) << "K" << endl;

    temp = 300.0;
    cout << temp << "K = " << kelvinToCelsius(temp) << "°C" << endl;
    cout << temp << "K = " << kelvinToFahrenheit(temp) << "°F" << endl;

    return 0;
}
```

### Solution 5: Geometric Shapes

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

const double PI = 3.14159265358979323846;

// Circle functions
inline double circleArea(double radius) {
    return PI * radius * radius;
}

inline double circleCircumference(double radius) {
    return 2 * PI * radius;
}

// Rectangle functions
inline double rectangleArea(double width, double height) {
    return width * height;
}

inline double rectanglePerimeter(double width, double height) {
    return 2 * (width + height);
}

inline bool isSquare(double width, double height) {
    return width == height;
}

// Triangle functions
inline double triangleArea(double base, double height) {
    return 0.5 * base * height;
}

inline double trianglePerimeter(double a, double b, double c) {
    return a + b + c;
}

int main() {
    cout << fixed << setprecision(2);

    // Circle
    double radius = 5.0;
    cout << "Circle (radius " << radius << "):" << endl;
    cout << "  Area: " << circleArea(radius) << endl;
    cout << "  Circumference: " << circleCircumference(radius) << endl;

    // Rectangle
    double width = 4.0, height = 6.0;
    cout << "\nRectangle (" << width << "x" << height << "):" << endl;
    cout << "  Area: " << rectangleArea(width, height) << endl;
    cout << "  Perimeter: " << rectanglePerimeter(width, height) << endl;
    cout << "  Is square: " << (isSquare(width, height) ? "Yes" : "No") << endl;

    // Triangle
    double base = 5.0, triHeight = 4.0;
    cout << "\nTriangle (base " << base << ", height " << triHeight << "):" << endl;
    cout << "  Area: " << triangleArea(base, triHeight) << endl;
    cout << "  Perimeter: " << trianglePerimeter(3, 4, 5) << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Inline functions** eliminate function call overhead for small functions
- `inline` keyword is a **suggestion** to the compiler
- **Inline functions** are best for small, frequently-called functions
- **Const parameters** prevent modification and clarify intent
- **Const correctness** improves code safety and readability
- **Combining inline and const** creates efficient, safe helper functions
- Modern compilers often inline automatically with optimization enabled

## Checklist

- [ ] I understand what inline functions are and how they work
- [ ] I know when to use inline functions
- [ ] I can use const parameters effectively
- [ ] I understand const correctness
- [ ] I can combine inline and const appropriately
- [ ] I completed all exercises and understand the solutions
- [ ] I know the benefits and limitations of inline functions
- [ ] I can apply these concepts to optimize my code

## Additional Resources

- **Next Lesson:** Default Arguments and Function Pointers
- **Related Topics:** Compiler Optimization, Function Inlining, Const Expressions
- **Practice:** Refactor existing code to use inline and const appropriately

---

**Congratulations!** You've completed Lesson 14. You now understand inline functions and const correctness, which are important for writing efficient and safe C++ code.
