# Lesson 23: Inline Functions

## Duration
2 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand the concept and purpose of inline functions
- Know when to use inline functions for optimization
- Understand the difference between inline suggestions and forced inlining
- Use inline functions effectively in header files
- Understand the trade-offs of inlining
- Debug inline-related issues
- Write performance-critical code using inline functions

## Introduction

Inline functions are a performance optimization feature in C++ that suggests to the compiler to insert the function's code directly at the call site, eliminating the overhead of function calls. This lesson explores when and how to use inline functions effectively.

## 1. Basic Inline Functions

### Simple Inline Functions

```cpp
#include <iostream>
using namespace std;

// Regular function
int regularAdd(int a, int b) {
    return a + b;
}

// Inline function
inline int inlineAdd(int a, int b) {
    return a + b;
}

// Inline function for square
inline int square(int x) {
    return x * x;
}

// Inline function for max
inline int maximum(int a, int b) {
    return (a > b) ? a : b;
}

// Inline function for absolute value
inline int absolute(int x) {
    return (x < 0) ? -x : x;
}

int main() {
    cout << "Regular add: " << regularAdd(5, 3) << endl;
    cout << "Inline add: " << inlineAdd(5, 3) << endl;
    cout << "Square of 7: " << square(7) << endl;
    cout << "Maximum of 10 and 15: " << maximum(10, 15) << endl;
    cout << "Absolute of -42: " << absolute(-42) << endl;

    // The inline function calls may be replaced with:
    // cout << "Inline add: " << (5 + 3) << endl;
    // This eliminates function call overhead

    return 0;
}
```

## 2. Inline Functions in Classes

### Implicitly Inline Member Functions

```cpp
#include <iostream>
using namespace std;

class Rectangle {
private:
    double width;
    double height;

public:
    // Constructor (implicitly inline)
    Rectangle(double w, double h) : width(w), height(h) {}

    // Member functions defined inside class are implicitly inline
    double getWidth() const { return width; }
    double getHeight() const { return height; }

    double area() const {
        return width * height;
    }

    double perimeter() const {
        return 2 * (width + height);
    }

    bool isSquare() const {
        return width == height;
    }
};

class Circle {
private:
    double radius;
    static constexpr double PI = 3.14159265359;

public:
    Circle(double r) : radius(r) {}

    // Inline member functions
    double getRadius() const { return radius; }
    void setRadius(double r) { radius = r; }

    double area() const {
        return PI * radius * radius;
    }

    double circumference() const {
        return 2 * PI * radius;
    }

    double diameter() const {
        return 2 * radius;
    }
};

int main() {
    Rectangle rect(5.0, 10.0);
    cout << "Rectangle:" << endl;
    cout << "  Width: " << rect.getWidth() << endl;
    cout << "  Height: " << rect.getHeight() << endl;
    cout << "  Area: " << rect.area() << endl;
    cout << "  Perimeter: " << rect.perimeter() << endl;
    cout << "  Is square? " << (rect.isSquare() ? "Yes" : "No") << endl;
    cout << endl;

    Circle circle(5.0);
    cout << "Circle:" << endl;
    cout << "  Radius: " << circle.getRadius() << endl;
    cout << "  Diameter: " << circle.diameter() << endl;
    cout << "  Area: " << circle.area() << endl;
    cout << "  Circumference: " << circle.circumference() << endl;

    return 0;
}
```

### Explicitly Inline Member Functions

```cpp
#include <iostream>
using namespace std;

class Point {
private:
    int x, y;

public:
    Point(int xVal = 0, int yVal = 0);

    // Inline getter declarations
    inline int getX() const;
    inline int getY() const;

    // Inline setter declarations
    inline void setX(int xVal);
    inline void setY(int yVal);

    // Inline utility functions
    inline double distanceFromOrigin() const;
    inline void print() const;
};

// Inline definitions outside class
inline Point::Point(int xVal, int yVal) : x(xVal), y(yVal) {}

inline int Point::getX() const {
    return x;
}

inline int Point::getY() const {
    return y;
}

inline void Point::setX(int xVal) {
    x = xVal;
}

inline void Point::setY(int yVal) {
    y = yVal;
}

inline double Point::distanceFromOrigin() const {
    return sqrt(x * x + y * y);
}

inline void Point::print() const {
    cout << "(" << x << ", " << y << ")";
}

int main() {
    Point p1(3, 4);
    cout << "Point: ";
    p1.print();
    cout << "\nDistance from origin: " << p1.distanceFromOrigin() << endl;

    p1.setX(6);
    p1.setY(8);
    cout << "Updated point: ";
    p1.print();
    cout << "\nNew distance: " << p1.distanceFromOrigin() << endl;

    return 0;
}
```

## 3. Performance Comparison

### Inline vs Non-Inline Performance

```cpp
#include <iostream>
#include <chrono>
using namespace std;

// Non-inline function
int nonInlineSquare(int x) {
    return x * x;
}

// Inline function
inline int inlineSquare(int x) {
    return x * x;
}

// Template (always inline candidate)
template<typename T>
inline T templateSquare(T x) {
    return x * x;
}

void measurePerformance() {
    const int ITERATIONS = 10000000;
    int result = 0;

    // Measure non-inline
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result += nonInlineSquare(i);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Measure inline
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result += inlineSquare(i);
    }
    end = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Measure template
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result += templateSquare(i);
    }
    end = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Measure direct calculation
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result += i * i;
    }
    end = chrono::high_resolution_clock::now();
    auto duration4 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Performance Comparison (" << ITERATIONS << " iterations):" << endl;
    cout << "Non-inline function: " << duration1 << " ms" << endl;
    cout << "Inline function:     " << duration2 << " ms" << endl;
    cout << "Template function:   " << duration3 << " ms" << endl;
    cout << "Direct calculation:  " << duration4 << " ms" << endl;
    cout << "(Result to prevent optimization: " << result << ")" << endl;
}

int main() {
    measurePerformance();
    return 0;
}
```

## 4. When to Use Inline Functions

### Good Candidates for Inlining

```cpp
#include <iostream>
#include <string>
using namespace std;

class SmartInlining {
public:
    // Good: Small, frequently-called getters/setters
    inline int getValue() const { return value; }
    inline void setValue(int v) { value = v; }

    // Good: Simple calculations
    inline int double_value() const { return value * 2; }
    inline bool isPositive() const { return value > 0; }
    inline bool isEven() const { return value % 2 == 0; }

    // Good: Simple utility functions
    inline int min(int a, int b) const {
        return (a < b) ? a : b;
    }

    inline int max(int a, int b) const {
        return (a > b) ? a : b;
    }

    inline int clamp(int val, int minVal, int maxVal) const {
        return max(minVal, min(val, maxVal));
    }

    // Good: Wrapper functions
    inline void increment() { ++value; }
    inline void decrement() { --value; }
    inline void reset() { value = 0; }

private:
    int value = 0;
};

// Good: Mathematical operations
inline double toRadians(double degrees) {
    return degrees * 3.14159265359 / 180.0;
}

inline double toDegrees(double radians) {
    return radians * 180.0 / 3.14159265359;
}

// Good: Type conversions
inline int toInt(double d) {
    return static_cast<int>(d);
}

inline double toDouble(int i) {
    return static_cast<double>(i);
}

int main() {
    SmartInlining obj;

    obj.setValue(42);
    cout << "Value: " << obj.getValue() << endl;
    cout << "Doubled: " << obj.double_value() << endl;
    cout << "Is positive? " << (obj.isPositive() ? "Yes" : "No") << endl;
    cout << "Is even? " << (obj.isEven() ? "Yes" : "No") << endl;

    cout << "Min(10, 20): " << obj.min(10, 20) << endl;
    cout << "Max(10, 20): " << obj.max(10, 20) << endl;
    cout << "Clamp(150, 0, 100): " << obj.clamp(150, 0, 100) << endl;

    obj.increment();
    cout << "After increment: " << obj.getValue() << endl;

    cout << "90 degrees = " << toRadians(90) << " radians" << endl;
    cout << "PI radians = " << toDegrees(3.14159) << " degrees" << endl;

    return 0;
}
```

### Poor Candidates for Inlining

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class PoorInlining {
public:
    // BAD: Large function body (code bloat)
    // inline void processLargeData() {
    //     // Many lines of code...
    //     // This would be copied at each call site
    //     // causing code bloat
    // }

    // BAD: Recursive function
    // inline int fibonacci(int n) {
    //     if (n <= 1) return n;
    //     return fibonacci(n-1) + fibonacci(n-2);
    // }

    // BAD: Functions with loops
    // inline void processArray(const vector<int>& arr) {
    //     for (const auto& item : arr) {
    //         // Complex processing
    //     }
    // }

    // BAD: Virtual functions (cannot be inlined when called via pointer/reference)
    // virtual inline void virtualFunction() { }

    // BETTER: Don't inline these
    void processLargeData() {
        vector<int> data(1000);
        for (size_t i = 0; i < data.size(); i++) {
            data[i] = i * i;
            // More processing...
        }
        sort(data.begin(), data.end());
        // More code...
    }

    int fibonacci(int n) {
        if (n <= 1) return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    void processArray(const vector<int>& arr) {
        for (const auto& item : arr) {
            if (item % 2 == 0) {
                cout << item << " is even" << endl;
            }
        }
    }
};

int main() {
    PoorInlining obj;

    // These should NOT be inlined due to their complexity
    obj.processLargeData();

    cout << "Fibonacci(10): " << obj.fibonacci(10) << endl;

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    obj.processArray(numbers);

    return 0;
}
```

## 5. Inline Functions in Header Files

### Proper Header Usage

```cpp
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>

namespace MathUtils {
    // Inline functions in header files
    inline double square(double x) {
        return x * x;
    }

    inline double cube(double x) {
        return x * x * x;
    }

    inline double pythagoras(double a, double b) {
        return sqrt(square(a) + square(b));
    }

    inline double lerp(double a, double b, double t) {
        return a + (b - a) * t;
    }

    inline double clamp01(double value) {
        if (value < 0.0) return 0.0;
        if (value > 1.0) return 1.0;
        return value;
    }

    inline bool approximately(double a, double b, double epsilon = 0.0001) {
        return fabs(a - b) < epsilon;
    }
}

#endif // MATH_UTILS_H
```

```cpp
// main.cpp
#include <iostream>
// #include "math_utils.h"  // Would be included if in separate file
using namespace std;

// For this example, we'll define the namespace inline
namespace MathUtils {
    inline double square(double x) { return x * x; }
    inline double cube(double x) { return x * x * x; }
    inline double pythagoras(double a, double b) {
        return sqrt(square(a) + square(b));
    }
    inline double lerp(double a, double b, double t) {
        return a + (b - a) * t;
    }
    inline double clamp01(double value) {
        if (value < 0.0) return 0.0;
        if (value > 1.0) return 1.0;
        return value;
    }
}

int main() {
    using namespace MathUtils;

    cout << "Square of 5: " << square(5) << endl;
    cout << "Cube of 3: " << cube(3) << endl;
    cout << "Pythagoras(3, 4): " << pythagoras(3, 4) << endl;
    cout << "Lerp(0, 100, 0.5): " << lerp(0, 100, 0.5) << endl;
    cout << "Clamp01(1.5): " << clamp01(1.5) << endl;
    cout << "Clamp01(-0.5): " << clamp01(-0.5) << endl;

    return 0;
}
```

## 6. Compiler Optimization and Inline

### Understanding Compiler Behavior

```cpp
#include <iostream>
using namespace std;

// Inline is a suggestion, not a command
inline void suggestedInline() {
    cout << "This is suggested to be inline" << endl;
    // Compiler may or may not inline this
}

// Modern compilers often inline without the keyword
void autoInlined() {
    // Very simple function - compiler will likely inline this
    // even without the inline keyword
}

// Force inline (compiler-specific)
#ifdef _MSC_VER
    #define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define FORCE_INLINE inline
#endif

FORCE_INLINE int forceInlinedAdd(int a, int b) {
    return a + b;
}

// Prevent inlining (compiler-specific)
#ifdef _MSC_VER
    #define NO_INLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
    #define NO_INLINE __attribute__((noinline))
#else
    #define NO_INLINE
#endif

NO_INLINE int neverInlined(int a, int b) {
    return a * b;
}

class OptimizationDemo {
public:
    // Compiler will decide based on optimization level
    inline int smartFunction(int x) {
        if (x < 0) return -x;
        return x;
    }

    // Too complex for inlining
    inline int complexFunction(int x) {
        int result = 0;
        for (int i = 0; i < x; i++) {
            result += i * i;
            if (result > 1000) break;
        }
        return result;
    }
};

int main() {
    cout << "Force inlined: " << forceInlinedAdd(10, 20) << endl;
    cout << "Never inlined: " << neverInlined(5, 6) << endl;

    OptimizationDemo demo;
    cout << "Smart function: " << demo.smartFunction(-42) << endl;
    cout << "Complex function: " << demo.complexFunction(50) << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Inline Math Library
Create a comprehensive inline math library with functions for common operations like min, max, abs, sign, clamp, and interpolation.

### Exercise 2: Inline String Utilities
Implement inline string utility functions for trimming, case conversion, and simple string checks.

### Exercise 3: Performance Analysis
Write a program that compares the performance of inline vs non-inline functions for various scenarios and measures the impact.

### Exercise 4: Smart Vector Class
Create a 2D/3D vector class with inline member functions for common vector operations.

### Exercise 5: Inline vs Macro
Compare inline functions with macros, showing the advantages of inline functions over preprocessor macros.

## Exercise Solutions

### Solution 1: Inline Math Library

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

namespace InlineMath {
    // Basic operations
    template<typename T>
    inline T min(T a, T b) {
        return (a < b) ? a : b;
    }

    template<typename T>
    inline T max(T a, T b) {
        return (a > b) ? a : b;
    }

    template<typename T>
    inline T abs(T x) {
        return (x < 0) ? -x : x;
    }

    template<typename T>
    inline int sign(T x) {
        if (x > 0) return 1;
        if (x < 0) return -1;
        return 0;
    }

    template<typename T>
    inline T clamp(T value, T minVal, T maxVal) {
        return max(minVal, min(value, maxVal));
    }

    // Interpolation functions
    template<typename T>
    inline T lerp(T a, T b, double t) {
        return a + (b - a) * t;
    }

    inline double smoothstep(double edge0, double edge1, double x) {
        double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * (3.0 - 2.0 * t);
    }

    inline double smootherstep(double edge0, double edge1, double x) {
        double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
    }

    // Range mapping
    template<typename T>
    inline T map(T value, T inMin, T inMax, T outMin, T outMax) {
        return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
    }

    // Power and roots
    inline double square(double x) { return x * x; }
    inline double cube(double x) { return x * x * x; }

    // Angle operations
    inline double toRadians(double degrees) {
        return degrees * M_PI / 180.0;
    }

    inline double toDegrees(double radians) {
        return radians * 180.0 / M_PI;
    }

    inline double normalizeAngle(double angle) {
        while (angle > 180.0) angle -= 360.0;
        while (angle < -180.0) angle += 360.0;
        return angle;
    }

    // Comparison with epsilon
    inline bool approximately(double a, double b, double epsilon = 1e-6) {
        return abs(a - b) < epsilon;
    }

    inline bool isZero(double x, double epsilon = 1e-6) {
        return abs(x) < epsilon;
    }
}

int main() {
    using namespace InlineMath;

    cout << "Min(10, 20): " << min(10, 20) << endl;
    cout << "Max(10, 20): " << max(10, 20) << endl;
    cout << "Abs(-15): " << abs(-15) << endl;
    cout << "Sign(-42): " << sign(-42) << endl;
    cout << "Clamp(150, 0, 100): " << clamp(150, 0, 100) << endl;
    cout << endl;

    cout << "Lerp(0, 100, 0.5): " << lerp(0.0, 100.0, 0.5) << endl;
    cout << "Smoothstep(0, 1, 0.5): " << smoothstep(0.0, 1.0, 0.5) << endl;
    cout << endl;

    cout << "Map(5, 0, 10, 0, 100): " << map(5.0, 0.0, 10.0, 0.0, 100.0) << endl;
    cout << endl;

    cout << "Square(7): " << square(7) << endl;
    cout << "Cube(3): " << cube(3) << endl;
    cout << endl;

    cout << "90° to radians: " << toRadians(90) << endl;
    cout << "π radians to degrees: " << toDegrees(M_PI) << endl;
    cout << "Normalize 370°: " << normalizeAngle(370) << endl;
    cout << endl;

    cout << "Approximately(1.0000001, 1.0)? " << approximately(1.0000001, 1.0) << endl;
    cout << "IsZero(0.0000001)? " << isZero(0.0000001) << endl;

    return 0;
}
```

### Solution 2: Inline String Utilities

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

namespace InlineStringUtils {
    // Case conversion
    inline string toUpper(string str) {
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    inline string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    // Trimming
    inline string trimLeft(const string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        return (start == string::npos) ? "" : str.substr(start);
    }

    inline string trimRight(const string& str) {
        size_t end = str.find_last_not_of(" \t\n\r");
        return (end == string::npos) ? "" : str.substr(0, end + 1);
    }

    inline string trim(const string& str) {
        return trimLeft(trimRight(str));
    }

    // String checks
    inline bool isEmpty(const string& str) {
        return str.empty();
    }

    inline bool isWhitespace(const string& str) {
        return trim(str).empty();
    }

    inline bool startsWith(const string& str, const string& prefix) {
        return str.size() >= prefix.size() &&
               str.compare(0, prefix.size(), prefix) == 0;
    }

    inline bool endsWith(const string& str, const string& suffix) {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    inline bool contains(const string& str, const string& substr) {
        return str.find(substr) != string::npos;
    }

    inline bool isNumeric(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    }

    inline bool isAlpha(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isalpha);
    }

    inline bool isAlphanumeric(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isalnum);
    }

    // String manipulation
    inline string reverse(string str) {
        std::reverse(str.begin(), str.end());
        return str;
    }

    inline string repeat(const string& str, int count) {
        string result;
        for (int i = 0; i < count; i++) {
            result += str;
        }
        return result;
    }

    inline int count(const string& str, char ch) {
        return std::count(str.begin(), str.end(), ch);
    }
}

int main() {
    using namespace InlineStringUtils;

    string test1 = "Hello World";
    string test2 = "  spaces  ";
    string test3 = "12345";

    cout << "To upper: " << toUpper(test1) << endl;
    cout << "To lower: " << toLower(test1) << endl;
    cout << endl;

    cout << "Original: [" << test2 << "]" << endl;
    cout << "Trimmed:  [" << trim(test2) << "]" << endl;
    cout << endl;

    cout << "Is empty? " << isEmpty("") << endl;
    cout << "Is whitespace? " << isWhitespace("   ") << endl;
    cout << "Starts with 'Hello'? " << startsWith(test1, "Hello") << endl;
    cout << "Ends with 'World'? " << endsWith(test1, "World") << endl;
    cout << "Contains 'lo Wo'? " << contains(test1, "lo Wo") << endl;
    cout << endl;

    cout << "Is numeric? " << isNumeric(test3) << endl;
    cout << "Is alpha? " << isAlpha(test1) << endl;
    cout << endl;

    cout << "Reverse: " << reverse(test1) << endl;
    cout << "Repeat: " << repeat("*-", 5) << endl;
    cout << "Count 'l': " << count(test1, 'l') << endl;

    return 0;
}
```

### Solution 3: Performance Analysis

```cpp
#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

// Non-inline versions
int nonInlineAdd(int a, int b) { return a + b; }
int nonInlineMultiply(int a, int b) { return a * b; }
bool nonInlineIsEven(int x) { return x % 2 == 0; }

// Inline versions
inline int inlineAdd(int a, int b) { return a + b; }
inline int inlineMultiply(int a, int b) { return a * b; }
inline bool inlineIsEven(int x) { return x % 2 == 0; }

// Template versions (always inline candidates)
template<typename T>
inline T templateAdd(T a, T b) { return a + b; }

template<typename T>
inline T templateMultiply(T a, T b) { return a * b; }

template<typename T>
inline bool templateIsEven(T x) { return x % 2 == 0; }

class PerformanceAnalyzer {
private:
    static const int ITERATIONS = 10000000;

    template<typename Func>
    long long measureTime(Func func) {
        auto start = chrono::high_resolution_clock::now();
        func();
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::microseconds>(end - start).count();
    }

public:
    void testAddition() {
        volatile int result = 0;  // volatile prevents optimization

        cout << "=== Addition Test (" << ITERATIONS << " iterations) ===" << endl;

        auto time1 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = nonInlineAdd(i, i + 1);
            }
        });

        auto time2 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = inlineAdd(i, i + 1);
            }
        });

        auto time3 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = templateAdd(i, i + 1);
            }
        });

        auto time4 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = i + (i + 1);
            }
        });

        cout << "Non-inline:  " << time1 << " μs" << endl;
        cout << "Inline:      " << time2 << " μs (" << ((time1-time2)*100.0/time1) << "% faster)" << endl;
        cout << "Template:    " << time3 << " μs" << endl;
        cout << "Direct:      " << time4 << " μs" << endl;
        cout << "(Result: " << result << ")" << endl << endl;
    }

    void testMultiplication() {
        volatile int result = 0;

        cout << "=== Multiplication Test (" << ITERATIONS << " iterations) ===" << endl;

        auto time1 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = nonInlineMultiply(i, 2);
            }
        });

        auto time2 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = inlineMultiply(i, 2);
            }
        });

        cout << "Non-inline:  " << time1 << " μs" << endl;
        cout << "Inline:      " << time2 << " μs" << endl;
        cout << "(Result: " << result << ")" << endl << endl;
    }

    void testConditional() {
        volatile bool result = false;

        cout << "=== Conditional Test (" << ITERATIONS << " iterations) ===" << endl;

        auto time1 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = nonInlineIsEven(i);
            }
        });

        auto time2 = measureTime([&]() {
            for (int i = 0; i < ITERATIONS; i++) {
                result = inlineIsEven(i);
            }
        });

        cout << "Non-inline:  " << time1 << " μs" << endl;
        cout << "Inline:      " << time2 << " μs" << endl;
        cout << "(Result: " << result << ")" << endl << endl;
    }
};

int main() {
    PerformanceAnalyzer analyzer;

    analyzer.testAddition();
    analyzer.testMultiplication();
    analyzer.testConditional();

    cout << "Note: Results may vary based on compiler optimizations." << endl;
    cout << "Compile with -O0 (no optimization) vs -O3 (full optimization) to see differences." << endl;

    return 0;
}
```

### Solution 4: Smart Vector Class

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Vector2D {
public:
    double x, y;

    // Constructors
    inline Vector2D() : x(0), y(0) {}
    inline Vector2D(double xVal, double yVal) : x(xVal), y(yVal) {}

    // Basic operations
    inline Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    inline Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    inline Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    inline Vector2D operator/(double scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    // Compound assignment
    inline Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Vector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Vector operations
    inline double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    inline double lengthSquared() const {
        return x * x + y * y;
    }

    inline double length() const {
        return sqrt(lengthSquared());
    }

    inline Vector2D normalized() const {
        double len = length();
        return (len > 0) ? (*this / len) : Vector2D();
    }

    inline void normalize() {
        double len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    inline double distance(const Vector2D& other) const {
        return (*this - other).length();
    }

    inline double angle() const {
        return atan2(y, x);
    }

    inline void print() const {
        cout << "(" << x << ", " << y << ")";
    }
};

// Free function for scalar * vector
inline Vector2D operator*(double scalar, const Vector2D& vec) {
    return vec * scalar;
}

class Vector3D {
public:
    double x, y, z;

    inline Vector3D() : x(0), y(0), z(0) {}
    inline Vector3D(double xVal, double yVal, double zVal)
        : x(xVal), y(yVal), z(zVal) {}

    inline Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    inline Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    inline Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    inline double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    inline Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    inline double lengthSquared() const {
        return x * x + y * y + z * z;
    }

    inline double length() const {
        return sqrt(lengthSquared());
    }

    inline Vector3D normalized() const {
        double len = length();
        return (len > 0) ? (*this * (1.0 / len)) : Vector3D();
    }

    inline void print() const {
        cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    cout << "v1 = "; v1.print(); cout << endl;
    cout << "v2 = "; v2.print(); cout << endl;
    cout << "v1 + v2 = "; (v1 + v2).print(); cout << endl;
    cout << "v1 - v2 = "; (v1 - v2).print(); cout << endl;
    cout << "v1 * 2 = "; (v1 * 2).print(); cout << endl;
    cout << "v1 . v2 = " << v1.dot(v2) << endl;
    cout << "|v1| = " << v1.length() << endl;
    cout << "normalized v1 = "; v1.normalized().print(); cout << endl;
    cout << "distance(v1, v2) = " << v1.distance(v2) << endl;
    cout << endl;

    Vector3D v3(1, 2, 3);
    Vector3D v4(4, 5, 6);

    cout << "v3 = "; v3.print(); cout << endl;
    cout << "v4 = "; v4.print(); cout << endl;
    cout << "v3 + v4 = "; (v3 + v4).print(); cout << endl;
    cout << "v3 . v4 = " << v3.dot(v4) << endl;
    cout << "v3 x v4 = "; v3.cross(v4).print(); cout << endl;
    cout << "|v3| = " << v3.length() << endl;

    return 0;
}
```

### Solution 5: Inline vs Macro

```cpp
#include <iostream>
using namespace std;

// MACROS (Problematic)
#define MACRO_SQUARE(x) ((x) * (x))
#define MACRO_MAX(a, b) ((a) > (b) ? (a) : (b))
#define MACRO_ABS(x) ((x) < 0 ? -(x) : (x))

// INLINE FUNCTIONS (Better)
inline int inlineSquare(int x) {
    return x * x;
}

inline int inlineMax(int a, int b) {
    return (a > b) ? a : b;
}

inline int inlineAbs(int x) {
    return (x < 0) ? -x : x;
}

// Template inline (Best for generic code)
template<typename T>
inline T templateSquare(T x) {
    return x * x;
}

template<typename T>
inline T templateMax(T a, T b) {
    return (a > b) ? a : b;
}

void demonstrateProblems() {
    cout << "=== Macro Problems ===" << endl;

    // Problem 1: Multiple evaluation
    int x = 5;
    cout << "MACRO_SQUARE(x++): " << MACRO_SQUARE(x++) << endl;
    cout << "x after macro: " << x << endl;  // x is 7, not 6!

    x = 5;
    cout << "inlineSquare(x++): " << inlineSquare(x++) << endl;
    cout << "x after inline: " << x << endl;  // x is 6, correct
    cout << endl;

    // Problem 2: No type checking
    // MACRO_SQUARE("hello");  // Would compile but cause issues
    // inlineSquare("hello");  // Compilation error - type safe!

    // Problem 3: Debugging difficulty
    cout << "Macro max: " << MACRO_MAX(10, 20) << endl;
    cout << "Inline max: " << inlineMax(10, 20) << endl;
    // With inline, you can step into the function in debugger
    cout << endl;
}

void demonstrateAdvantages() {
    cout << "=== Inline Function Advantages ===" << endl;

    // Type safety
    cout << "Int square: " << inlineSquare(5) << endl;
    cout << "Double square: " << templateSquare(3.5) << endl;

    // Scope respect
    {
        inline int localHelper(int x) { return x * 2; }
        // Can use inline in limited scope
    }

    // Const correctness
    class Test {
        int value = 42;
    public:
        inline int getValue() const { return value; }
        // Can have const member functions
    };

    Test t;
    cout << "Const method: " << t.getValue() << endl;

    // Namespace support
    namespace Math {
        inline int add(int a, int b) { return a + b; }
    }
    cout << "Namespaced: " << Math::add(10, 20) << endl;
}

void performanceComparison() {
    cout << "\n=== Performance (should be similar) ===" << endl;

    const int ITERATIONS = 10000000;
    volatile int result;

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result = MACRO_SQUARE(i);
    }
    auto end = chrono::high_resolution_clock::now();
    auto time1 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        result = inlineSquare(i);
    }
    end = chrono::high_resolution_clock::now();
    auto time2 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Macro:  " << time1 << " μs" << endl;
    cout << "Inline: " << time2 << " μs" << endl;
    cout << "(Result: " << result << ")" << endl;
}

int main() {
    demonstrateProblems();
    demonstrateAdvantages();
    performanceComparison();

    cout << "\n=== Summary ===" << endl;
    cout << "Prefer inline functions over macros for:" << endl;
    cout << "1. Type safety" << endl;
    cout << "2. No multiple evaluation" << endl;
    cout << "3. Better debugging" << endl;
    cout << "4. Scope and namespace support" << endl;
    cout << "5. Const correctness" << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- The purpose and syntax of inline functions
- When to use inline functions for optimization
- The difference between inline suggestions and compiler decisions
- How to use inline functions in classes and headers
- The trade-offs between inline functions and regular functions
- Best practices for effective use of inline functions
- How inline functions compare to macros

## Checklist

- [ ] I understand what inline functions are and how they work
- [ ] I know when to use inline functions for optimization
- [ ] I understand that inline is a suggestion, not a command
- [ ] I can properly use inline functions in header files
- [ ] I know which functions are good candidates for inlining
- [ ] I understand the trade-offs of inlining (code size vs speed)
- [ ] I can use compiler-specific forced inlining when necessary
- [ ] I completed all exercises and understand their solutions

## Next Steps
In the next lesson, we'll dive deep into Recursion, exploring advanced recursive techniques, optimization strategies, and real-world applications.
