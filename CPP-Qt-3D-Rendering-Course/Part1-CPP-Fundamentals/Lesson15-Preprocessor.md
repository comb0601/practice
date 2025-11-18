# Lesson 15: C++ Preprocessor

**Duration**: 2.5-3 hours

## Overview

The C++ preprocessor is a text processing tool that runs before compilation. It handles directives like `#include`, `#define`, and conditional compilation (`#ifdef`). The preprocessor manipulates source code text before it reaches the compiler, enabling code reuse, conditional compilation, and macro definitions.

## Topics Covered

1. Introduction to the Preprocessor
2. #include Directive
3. #define and Macros
4. Conditional Compilation
5. Predefined Macros
6. Include Guards
7. Pragma Directives
8. Best Practices and Common Pitfalls

## 1. Introduction to the Preprocessor

The preprocessor processes directives (lines starting with `#`) before compilation:
- **#include** - Insert file contents
- **#define** - Define macros
- **#ifdef, #ifndef, #if** - Conditional compilation
- **#pragma** - Compiler-specific directives

**Preprocessing Flow:**
1. Source code → Preprocessor → Expanded code → Compiler → Object code

## 2. #include Directive

### Example 1: Include Directives - Complete Program

**File: `include_demo.cpp`**

```cpp
// System includes - use angle brackets
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

// User includes would use quotes: #include "myheader.h"

int main() {
    std::cout << "=== Include Directive Demo ===" << std::endl;

    // Demonstrates that standard library headers were included
    std::cout << "\n--- Using <iostream> ---" << std::endl;
    std::cout << "Standard output and input" << std::endl;

    std::cout << "\n--- Using <string> ---" << std::endl;
    std::string message = "String class from <string> header";
    std::cout << message << std::endl;

    std::cout << "\n--- Using <vector> ---" << std::endl;
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Vector size: " << numbers.size() << std::endl;
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "\n--- Using <cmath> ---" << std::endl;
    double value = 16.0;
    std::cout << "sqrt(" << value << ") = " << std::sqrt(value) << std::endl;
    std::cout << "pow(2, 10) = " << std::pow(2, 10) << std::endl;

    std::cout << "\n--- Using <iomanip> ---" << std::endl;
    double pi = 3.14159265359;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Pi (2 decimals): " << pi << std::endl;
    std::cout << std::setprecision(5);
    std::cout << "Pi (5 decimals): " << pi << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall include_demo.cpp -o include_demo
./include_demo
```

## 3. #define and Macros

### Example 2: Object-like Macros - Complete Program

**File: `object_macros.cpp`**

```cpp
#include <iostream>
#include <string>

// Object-like macros (constants)
#define PI 3.14159265359
#define MAX_SIZE 100
#define BUFFER_SIZE 1024
#define APP_NAME "MacroDemo"
#define APP_VERSION "1.0.0"
#define DEBUG_MODE

// Macro for array size
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main() {
    std::cout << "=== Object-like Macros Demo ===" << std::endl;

    // Using constant macros
    std::cout << "\n--- Constant Macros ---" << std::endl;
    std::cout << "PI = " << PI << std::endl;
    std::cout << "MAX_SIZE = " << MAX_SIZE << std::endl;
    std::cout << "BUFFER_SIZE = " << BUFFER_SIZE << std::endl;

    // Using string macros
    std::cout << "\n--- String Macros ---" << std::endl;
    std::cout << "Application: " << APP_NAME << std::endl;
    std::cout << "Version: " << APP_VERSION << std::endl;

    // Using PI in calculations
    std::cout << "\n--- Using PI ---" << std::endl;
    double radius = 5.0;
    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;
    std::cout << "Circle with radius " << radius << std::endl;
    std::cout << "Circumference: " << circumference << std::endl;
    std::cout << "Area: " << area << std::endl;

    // Using ARRAY_SIZE macro
    std::cout << "\n--- Array Size Macro ---" << std::endl;
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "Array has " << ARRAY_SIZE(numbers) << " elements" << std::endl;

    double values[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    std::cout << "Values array has " << ARRAY_SIZE(values) << " elements" << std::endl;

    // Checking if macro is defined
    std::cout << "\n--- Checking Defined Macros ---" << std::endl;
#ifdef DEBUG_MODE
    std::cout << "DEBUG_MODE is defined" << std::endl;
#else
    std::cout << "DEBUG_MODE is not defined" << std::endl;
#endif

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall object_macros.cpp -o object_macros
./object_macros
```

### Example 3: Function-like Macros - Complete Program

**File: `function_macros.cpp`**

```cpp
#include <iostream>
#include <string>

// Function-like macros
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_EVEN(n) ((n) % 2 == 0)
#define IS_ODD(n) ((n) % 2 != 0)

// String manipulation macros
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b

// Debug print macro
#define DEBUG_PRINT(var) std::cout << #var << " = " << (var) << std::endl

// Multi-line macro
#define PRINT_ARRAY(arr, size) \
    do { \
        std::cout << #arr << " = ["; \
        for (int i = 0; i < (size); ++i) { \
            std::cout << (arr)[i]; \
            if (i < (size) - 1) std::cout << ", "; \
        } \
        std::cout << "]" << std::endl; \
    } while(0)

int main() {
    std::cout << "=== Function-like Macros Demo ===" << std::endl;

    // Math macros
    std::cout << "\n--- Math Macros ---" << std::endl;
    int x = 5, y = 3;
    std::cout << "x = " << x << ", y = " << y << std::endl;
    std::cout << "SQUARE(" << x << ") = " << SQUARE(x) << std::endl;
    std::cout << "MAX(" << x << ", " << y << ") = " << MAX(x, y) << std::endl;
    std::cout << "MIN(" << x << ", " << y << ") = " << MIN(x, y) << std::endl;

    int negativeNum = -42;
    std::cout << "ABS(" << negativeNum << ") = " << ABS(negativeNum) << std::endl;

    // Testing even/odd macros
    std::cout << "\n--- Even/Odd Testing ---" << std::endl;
    for (int i = 0; i <= 5; ++i) {
        if (IS_EVEN(i)) {
            std::cout << i << " is even" << std::endl;
        } else {
            std::cout << i << " is odd" << std::endl;
        }
    }

    // Stringify macro
    std::cout << "\n--- Stringify Macro ---" << std::endl;
    std::cout << "STRINGIFY(Hello World) = " << STRINGIFY(Hello World) << std::endl;
    std::cout << "STRINGIFY(123) = " << STRINGIFY(123) << std::endl;

    // Concatenation macro
    std::cout << "\n--- Concatenation Macro ---" << std::endl;
    int value1 = 10, value2 = 20;
    std::cout << "value1 = " << value1 << ", value2 = " << value2 << std::endl;
    // CONCAT(value, 1) expands to value1
    std::cout << "CONCAT(value, 1) = " << CONCAT(value, 1) << std::endl;
    std::cout << "CONCAT(value, 2) = " << CONCAT(value, 2) << std::endl;

    // Debug print macro
    std::cout << "\n--- Debug Print Macro ---" << std::endl;
    int debugVar = 100;
    double pi = 3.14159;
    std::string name = "MacroTest";
    DEBUG_PRINT(debugVar);
    DEBUG_PRINT(pi);
    DEBUG_PRINT(name);

    // Array print macro
    std::cout << "\n--- Array Print Macro ---" << std::endl;
    int numbers[] = {1, 2, 3, 4, 5};
    PRINT_ARRAY(numbers, 5);

    double values[] = {1.1, 2.2, 3.3};
    PRINT_ARRAY(values, 3);

    // Demonstrating why parentheses matter
    std::cout << "\n--- Macro Pitfall Demo ---" << std::endl;
    std::cout << "SQUARE(2 + 3) = " << SQUARE(2 + 3) << std::endl;  // (2+3)*(2+3) = 25
    // Without parentheses would be: 2 + 3 * 2 + 3 = 11

    std::cout << "SQUARE(x++) would cause issues (don't use ++ with macros)" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall function_macros.cpp -o function_macros
./function_macros
```

## 4. Conditional Compilation

### Example 4: Conditional Compilation - Complete Program

**File: `conditional_compilation.cpp`**

```cpp
#include <iostream>
#include <string>

// Define various configuration macros
#define FEATURE_A_ENABLED
#define VERSION 2
// #define DEBUG  // Uncomment to enable debug mode

// Conditional compilation based on platform
#ifdef _WIN32
    #define PLATFORM "Windows"
#elif __APPLE__
    #define PLATFORM "macOS"
#elif __linux__
    #define PLATFORM "Linux"
#else
    #define PLATFORM "Unknown"
#endif

// Feature flags
#ifdef FEATURE_A_ENABLED
    void featureA() {
        std::cout << "Feature A is enabled" << std::endl;
    }
#endif

#ifdef FEATURE_B_ENABLED
    void featureB() {
        std::cout << "Feature B is enabled" << std::endl;
    }
#endif

// Version-specific behavior
#if VERSION == 1
    #define VERSION_STRING "Version 1.0"
    void versionSpecificCode() {
        std::cout << "Running Version 1 code" << std::endl;
    }
#elif VERSION == 2
    #define VERSION_STRING "Version 2.0"
    void versionSpecificCode() {
        std::cout << "Running Version 2 code (enhanced)" << std::endl;
    }
#else
    #define VERSION_STRING "Version Unknown"
    void versionSpecificCode() {
        std::cout << "Running default version code" << std::endl;
    }
#endif

// Debug macros
#ifdef DEBUG
    #define LOG(msg) std::cout << "[DEBUG] " << msg << std::endl
    #define LOG_VAR(var) std::cout << "[DEBUG] " << #var << " = " << (var) << std::endl
#else
    #define LOG(msg)
    #define LOG_VAR(var)
#endif

// Complex conditional compilation
#if defined(FEATURE_A_ENABLED) && VERSION >= 2
    void advancedFeature() {
        std::cout << "Advanced feature (requires Feature A and Version 2+)" << std::endl;
    }
#endif

int main() {
    std::cout << "=== Conditional Compilation Demo ===" << std::endl;

    // Platform information
    std::cout << "\n--- Platform Information ---" << std::endl;
    std::cout << "Platform: " << PLATFORM << std::endl;
    std::cout << "Version: " << VERSION_STRING << std::endl;

    // Version-specific code
    std::cout << "\n--- Version-Specific Code ---" << std::endl;
    versionSpecificCode();

    // Feature flags
    std::cout << "\n--- Feature Flags ---" << std::endl;
#ifdef FEATURE_A_ENABLED
    featureA();
#else
    std::cout << "Feature A is disabled" << std::endl;
#endif

#ifdef FEATURE_B_ENABLED
    featureB();
#else
    std::cout << "Feature B is disabled" << std::endl;
#endif

#if defined(FEATURE_A_ENABLED) && VERSION >= 2
    std::cout << "\n--- Advanced Features ---" << std::endl;
    advancedFeature();
#endif

    // Debug logging
    std::cout << "\n--- Debug Logging ---" << std::endl;
#ifdef DEBUG
    std::cout << "Debug mode is ENABLED" << std::endl;
#else
    std::cout << "Debug mode is DISABLED" << std::endl;
#endif

    int x = 42;
    std::string name = "Test";
    LOG("This is a debug message");
    LOG_VAR(x);
    LOG_VAR(name);

    // Compiler detection
    std::cout << "\n--- Compiler Information ---" << std::endl;
#ifdef __GNUC__
    std::cout << "Compiled with GCC/G++" << std::endl;
    std::cout << "GCC version: " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#elif _MSC_VER
    std::cout << "Compiled with Microsoft Visual C++" << std::endl;
#elif __clang__
    std::cout << "Compiled with Clang" << std::endl;
#else
    std::cout << "Unknown compiler" << std::endl;
#endif

    // C++ standard detection
    std::cout << "\n--- C++ Standard ---" << std::endl;
    std::cout << "__cplusplus = " << __cplusplus << std::endl;
#if __cplusplus >= 202002L
    std::cout << "C++20 or later" << std::endl;
#elif __cplusplus >= 201703L
    std::cout << "C++17" << std::endl;
#elif __cplusplus >= 201402L
    std::cout << "C++14" << std::endl;
#elif __cplusplus >= 201103L
    std::cout << "C++11" << std::endl;
#else
    std::cout << "Pre-C++11" << std::endl;
#endif

    return 0;
}
```

**Compile and run:**
```bash
# Normal compilation
g++ -std=c++20 -Wall conditional_compilation.cpp -o conditional_compilation
./conditional_compilation

# With DEBUG defined
g++ -std=c++20 -Wall -DDEBUG conditional_compilation.cpp -o conditional_compilation_debug
./conditional_compilation_debug
```

## 5. Predefined Macros

### Example 5: Predefined Macros - Complete Program

**File: `predefined_macros.cpp`**

```cpp
#include <iostream>
#include <string>
#include <iomanip>

// Function to demonstrate __func__ predefined identifier
void demonstrateFunction() {
    std::cout << "Current function: " << __func__ << std::endl;
}

class Logger {
public:
    static void log(const std::string& message,
                   const char* file = __builtin_FILE(),
                   int line = __builtin_LINE(),
                   const char* func = __builtin_FUNCTION()) {
        std::cout << "[" << file << ":" << line << " in " << func << "] "
                  << message << std::endl;
    }
};

int main() {
    std::cout << "=== Predefined Macros Demo ===" << std::endl;

    // Standard predefined macros
    std::cout << "\n--- Standard Predefined Macros ---" << std::endl;
    std::cout << "__FILE__: " << __FILE__ << std::endl;
    std::cout << "__LINE__: " << __LINE__ << std::endl;
    std::cout << "__DATE__: " << __DATE__ << std::endl;
    std::cout << "__TIME__: " << __TIME__ << std::endl;

    // Function name
    std::cout << "\n--- Function Name ---" << std::endl;
    std::cout << "Current function: " << __func__ << std::endl;
    demonstrateFunction();

    // C++ version
    std::cout << "\n--- C++ Version ---" << std::endl;
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
#if __cplusplus == 202002L
    std::cout << "C++20" << std::endl;
#elif __cplusplus == 201703L
    std::cout << "C++17" << std::endl;
#elif __cplusplus == 201402L
    std::cout << "C++14" << std::endl;
#elif __cplusplus == 201103L
    std::cout << "C++11" << std::endl;
#endif

    // Standard library
    std::cout << "\n--- Standard Library ---" << std::endl;
#ifdef __STDC_HOSTED__
    std::cout << "__STDC_HOSTED__: " << __STDC_HOSTED__ << std::endl;
#endif

    // Compiler-specific macros
    std::cout << "\n--- Compiler Detection ---" << std::endl;
#ifdef __GNUC__
    std::cout << "GCC/G++ compiler detected" << std::endl;
    std::cout << "__GNUC__: " << __GNUC__ << std::endl;
    std::cout << "__GNUC_MINOR__: " << __GNUC_MINOR__ << std::endl;
    std::cout << "__GNUC_PATCHLEVEL__: " << __GNUC_PATCHLEVEL__ << std::endl;
    std::cout << "Full version: " << __GNUC__ << "."
              << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
#endif

#ifdef _MSC_VER
    std::cout << "Microsoft Visual C++ detected" << std::endl;
    std::cout << "_MSC_VER: " << _MSC_VER << std::endl;
#endif

#ifdef __clang__
    std::cout << "Clang compiler detected" << std::endl;
    std::cout << "__clang_major__: " << __clang_major__ << std::endl;
    std::cout << "__clang_minor__: " << __clang_minor__ << std::endl;
#endif

    // Operating system detection
    std::cout << "\n--- Operating System ---" << std::endl;
#ifdef _WIN32
    std::cout << "Windows detected" << std::endl;
    #ifdef _WIN64
        std::cout << "64-bit Windows" << std::endl;
    #else
        std::cout << "32-bit Windows" << std::endl;
    #endif
#endif

#ifdef __linux__
    std::cout << "Linux detected" << std::endl;
#endif

#ifdef __APPLE__
    std::cout << "macOS/iOS detected" << std::endl;
    #ifdef __MACH__
        std::cout << "Mach kernel" << std::endl;
    #endif
#endif

#ifdef __unix__
    std::cout << "Unix-like system detected" << std::endl;
#endif

    // Architecture detection
    std::cout << "\n--- Architecture ---" << std::endl;
#ifdef __x86_64__
    std::cout << "x86-64 (64-bit)" << std::endl;
#elif __i386__
    std::cout << "x86 (32-bit)" << std::endl;
#elif __arm__
    std::cout << "ARM" << std::endl;
#elif __aarch64__
    std::cout << "ARM64" << std::endl;
#endif

    // Build configuration
    std::cout << "\n--- Build Configuration ---" << std::endl;
#ifdef NDEBUG
    std::cout << "Release build (NDEBUG defined)" << std::endl;
#else
    std::cout << "Debug build (NDEBUG not defined)" << std::endl;
#endif

    // Line tracking demonstration
    std::cout << "\n--- Line Tracking ---" << std::endl;
    std::cout << "Line " << __LINE__ << ": First line" << std::endl;
    std::cout << "Line " << __LINE__ << ": Second line" << std::endl;
    std::cout << "Line " << __LINE__ << ": Third line" << std::endl;

    // Timestamp of compilation
    std::cout << "\n--- Compilation Timestamp ---" << std::endl;
    std::cout << "This program was compiled on " << __DATE__
              << " at " << __TIME__ << std::endl;

    // Using custom logger
    std::cout << "\n--- Custom Logger Demo ---" << std::endl;
    Logger::log("Application started");
    Logger::log("Processing data");
    Logger::log("Application finished");

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall predefined_macros.cpp -o predefined_macros
./predefined_macros
```

## 6. Include Guards

### Example 6: Include Guards - Complete Program

**File: `math_utils.h`**

```cpp
// Include guard - prevents multiple inclusion
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <iostream>

namespace MathUtils {
    // Constant
    const double PI = 3.14159265359;

    // Function declarations
    double circleArea(double radius);
    double circleCircumference(double radius);
    double sphereVolume(double radius);
    double cylinderVolume(double radius, double height);

    // Inline function
    inline int square(int x) {
        return x * x;
    }

    // Template function
    template<typename T>
    T max(T a, T b) {
        return (a > b) ? a : b;
    }
}

// Implementation of functions
inline double MathUtils::circleArea(double radius) {
    return PI * radius * radius;
}

inline double MathUtils::circleCircumference(double radius) {
    return 2 * PI * radius;
}

inline double MathUtils::sphereVolume(double radius) {
    return (4.0 / 3.0) * PI * radius * radius * radius;
}

inline double MathUtils::cylinderVolume(double radius, double height) {
    return PI * radius * radius * height;
}

#endif // MATH_UTILS_H
```

**File: `string_utils.h`**

```cpp
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <algorithm>
#include <cctype>

namespace StringUtils {
    // Convert string to uppercase
    inline std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    // Convert string to lowercase
    inline std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Trim whitespace from start
    inline std::string trimLeft(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : str.substr(start);
    }

    // Trim whitespace from end
    inline std::string trimRight(const std::string& str) {
        size_t end = str.find_last_not_of(" \t\n\r");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    // Trim whitespace from both ends
    inline std::string trim(const std::string& str) {
        return trimLeft(trimRight(str));
    }

    // Check if string starts with prefix
    inline bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
               str.compare(0, prefix.size(), prefix) == 0;
    }

    // Check if string ends with suffix
    inline bool endsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
}

#endif // STRING_UTILS_H
```

**File: `include_guards_demo.cpp`**

```cpp
#include <iostream>
#include <string>

// Including the same header multiple times
// Without include guards, this would cause errors
#include "math_utils.h"
#include "math_utils.h"  // Safe - include guard prevents redefinition
#include "string_utils.h"
#include "string_utils.h"  // Safe - include guard prevents redefinition

int main() {
    std::cout << "=== Include Guards Demo ===" << std::endl;

    // Using MathUtils
    std::cout << "\n--- Math Utils ---" << std::endl;
    double radius = 5.0;
    std::cout << "Circle with radius " << radius << ":" << std::endl;
    std::cout << "  Area: " << MathUtils::circleArea(radius) << std::endl;
    std::cout << "  Circumference: " << MathUtils::circleCircumference(radius) << std::endl;

    std::cout << "\nSphere with radius " << radius << ":" << std::endl;
    std::cout << "  Volume: " << MathUtils::sphereVolume(radius) << std::endl;

    double height = 10.0;
    std::cout << "\nCylinder with radius " << radius << " and height " << height << ":" << std::endl;
    std::cout << "  Volume: " << MathUtils::cylinderVolume(radius, height) << std::endl;

    std::cout << "\nSquare of 7: " << MathUtils::square(7) << std::endl;
    std::cout << "Max(15, 23): " << MathUtils::max(15, 23) << std::endl;
    std::cout << "Max(3.14, 2.71): " << MathUtils::max(3.14, 2.71) << std::endl;

    // Using StringUtils
    std::cout << "\n--- String Utils ---" << std::endl;
    std::string text = "  Hello World  ";
    std::cout << "Original: '" << text << "'" << std::endl;
    std::cout << "Upper: '" << StringUtils::toUpper(text) << "'" << std::endl;
    std::cout << "Lower: '" << StringUtils::toLower(text) << "'" << std::endl;
    std::cout << "Trim: '" << StringUtils::trim(text) << "'" << std::endl;

    std::string filename = "document.txt";
    std::cout << "\nFilename: " << filename << std::endl;
    std::cout << "Starts with 'doc': " << StringUtils::startsWith(filename, "doc") << std::endl;
    std::cout << "Ends with '.txt': " << StringUtils::endsWith(filename, ".txt") << std::endl;
    std::cout << "Ends with '.pdf': " << StringUtils::endsWith(filename, ".pdf") << std::endl;

    std::cout << "\n=== Headers included multiple times safely ===" << std::endl;
    std::cout << "Include guards prevented redefinition errors" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall include_guards_demo.cpp -o include_guards_demo
./include_guards_demo
```

## 7. Pragma Directives

### Example 7: Pragma Directives - Complete Program

**File: `pragma_demo.cpp`**

```cpp
#include <iostream>
#include <string>

// #pragma once - alternative to include guards (not standard but widely supported)
// Would be used in a header file instead of include guards

// Suppress specific warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

void functionWithUnusedVariable() {
    int unused = 42;  // This would normally generate a warning
    std::cout << "Function with unused variable (warning suppressed)" << std::endl;
}

#pragma GCC diagnostic pop

// Pack structure to minimize padding
#pragma pack(push, 1)
struct PackedStruct {
    char a;     // 1 byte
    int b;      // 4 bytes
    char c;     // 1 byte
    // Total: 6 bytes (without padding)
};
#pragma pack(pop)

struct UnpackedStruct {
    char a;     // 1 byte + 3 bytes padding
    int b;      // 4 bytes
    char c;     // 1 byte + 3 bytes padding
    // Total: 12 bytes (with padding)
};

int main() {
    std::cout << "=== Pragma Directives Demo ===" << std::endl;

    // Structure packing demonstration
    std::cout << "\n--- Structure Packing ---" << std::endl;
    std::cout << "sizeof(PackedStruct): " << sizeof(PackedStruct) << " bytes" << std::endl;
    std::cout << "sizeof(UnpackedStruct): " << sizeof(UnpackedStruct) << " bytes" << std::endl;

    PackedStruct packed;
    packed.a = 'A';
    packed.b = 12345;
    packed.c = 'Z';
    std::cout << "\nPacked struct values:" << std::endl;
    std::cout << "  a = " << packed.a << std::endl;
    std::cout << "  b = " << packed.b << std::endl;
    std::cout << "  c = " << packed.c << std::endl;

    // Call function with suppressed warning
    std::cout << "\n--- Warning Suppression ---" << std::endl;
    functionWithUnusedVariable();

    // Demonstrate #pragma message (compile-time message)
#pragma message("This is a compile-time message")

    // Optimization hints (compiler-specific)
    std::cout << "\n--- Pragma Usage Notes ---" << std::endl;
    std::cout << "1. #pragma once - Alternative to include guards" << std::endl;
    std::cout << "2. #pragma pack - Control structure padding" << std::endl;
    std::cout << "3. #pragma GCC diagnostic - Control warnings" << std::endl;
    std::cout << "4. #pragma message - Compile-time messages" << std::endl;
    std::cout << "5. #pragma omp - OpenMP parallel programming" << std::endl;

    std::cout << "\nNote: Many pragma directives are compiler-specific" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall pragma_demo.cpp -o pragma_demo
./pragma_demo
```

## Complete Project 1: Configuration System with Preprocessor

**File: `config.h`**

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Application configuration
#define APP_NAME "PreprocessorDemo"
#define APP_VERSION_MAJOR 2
#define APP_VERSION_MINOR 5
#define APP_VERSION_PATCH 1

// Feature flags
#define FEATURE_LOGGING
#define FEATURE_DATABASE
// #define FEATURE_NETWORKING  // Disabled

// Debug configuration
// #define DEBUG_MODE  // Uncomment for debug build

// Build configuration
#ifdef DEBUG_MODE
    #define LOG_LEVEL 3  // Verbose
    #define ENABLE_ASSERTIONS
#else
    #define LOG_LEVEL 1  // Errors only
#endif

// Platform-specific settings
#ifdef _WIN32
    #define PATH_SEPARATOR '\\'
    #define DEFAULT_CONFIG_PATH "C:\\ProgramData\\config.ini"
#else
    #define PATH_SEPARATOR '/'
    #define DEFAULT_CONFIG_PATH "/etc/config.ini"
#endif

// Utility macros
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
#define VERSION_STRING STRINGIFY(APP_VERSION_MAJOR) "." \
                       STRINGIFY(APP_VERSION_MINOR) "." \
                       STRINGIFY(APP_VERSION_PATCH)

// Logging macros
#if LOG_LEVEL >= 3
    #define LOG_VERBOSE(msg) std::cout << "[VERBOSE] " << msg << std::endl
#else
    #define LOG_VERBOSE(msg)
#endif

#if LOG_LEVEL >= 2
    #define LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl
#else
    #define LOG_INFO(msg)
#endif

#if LOG_LEVEL >= 1
    #define LOG_ERROR(msg) std::cerr << "[ERROR] " << msg << std::endl
#else
    #define LOG_ERROR(msg)
#endif

// Assertion macro
#ifdef ENABLE_ASSERTIONS
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                std::cerr << "Assertion failed: " << #condition \
                          << " - " << message \
                          << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
                std::abort(); \
            } \
        } while(0)
#else
    #define ASSERT(condition, message)
#endif

#endif // CONFIG_H
```

**File: `config_system.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>
#include "config.h"

class Application {
private:
    std::string name;
    std::string version;
    bool running;

public:
    Application() : name(APP_NAME), version(VERSION_STRING), running(false) {
        LOG_VERBOSE("Application constructor called");
    }

    void initialize() {
        LOG_INFO("Initializing " + name + " v" + version);

        // Feature initialization
#ifdef FEATURE_LOGGING
        LOG_INFO("Logging feature enabled");
#endif

#ifdef FEATURE_DATABASE
        LOG_INFO("Database feature enabled");
#endif

#ifdef FEATURE_NETWORKING
        LOG_INFO("Networking feature enabled");
#else
        LOG_INFO("Networking feature disabled");
#endif

        // Platform-specific initialization
        std::cout << "Platform: " << PLATFORM << std::endl;
        std::cout << "Path separator: " << PATH_SEPARATOR << std::endl;
        std::cout << "Config path: " << DEFAULT_CONFIG_PATH << std::endl;

        running = true;
        LOG_VERBOSE("Initialization complete");
    }

    void processData(int value) {
        LOG_VERBOSE("Processing data: " + std::to_string(value));

        ASSERT(value >= 0, "Value must be non-negative");
        ASSERT(value <= 1000, "Value exceeds maximum");

        LOG_INFO("Data processed successfully: " + std::to_string(value));
    }

    void performTask(const std::string& taskName) {
        LOG_INFO("Performing task: " + taskName);

#ifdef FEATURE_DATABASE
        std::cout << "  Accessing database..." << std::endl;
#endif

#ifdef FEATURE_NETWORKING
        std::cout << "  Connecting to network..." << std::endl;
#endif

        std::cout << "  Task '" << taskName << "' completed" << std::endl;
    }

    void shutdown() {
        LOG_INFO("Shutting down application");
        running = false;
        LOG_VERBOSE("Application stopped");
    }

    bool isRunning() const { return running; }
    std::string getName() const { return name; }
    std::string getVersion() const { return version; }
};

// Conditional compilation for different build types
#ifdef DEBUG_MODE
void debugInfo() {
    std::cout << "\n=== DEBUG BUILD ===" << std::endl;
    std::cout << "Compiled: " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "File: " << __FILE__ << std::endl;
    std::cout << "C++ Standard: " << __cplusplus << std::endl;

#ifdef __GNUC__
    std::cout << "Compiler: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#endif
}
#endif

int main() {
    std::cout << "=== Configuration System Demo ===" << std::endl;

    // Display build information
    std::cout << "\n--- Build Information ---" << std::endl;
    std::cout << "Application: " << APP_NAME << std::endl;
    std::cout << "Version: " << VERSION_STRING << std::endl;
    std::cout << "Log Level: " << LOG_LEVEL << std::endl;

#ifdef DEBUG_MODE
    std::cout << "Build Type: Debug" << std::endl;
    debugInfo();
#else
    std::cout << "Build Type: Release" << std::endl;
#endif

    // Create and run application
    std::cout << "\n--- Application Execution ---" << std::endl;
    Application app;

    app.initialize();

    // Test logging at different levels
    LOG_ERROR("This is an error message");
    LOG_INFO("This is an info message");
    LOG_VERBOSE("This is a verbose message");

    // Process data with assertions
    std::cout << "\n--- Data Processing ---" << std::endl;
    app.processData(100);
    app.processData(500);

    // Test assertion (uncomment to see assertion failure)
    // app.processData(-10);  // Would trigger assertion
    // app.processData(2000);  // Would trigger assertion

    // Perform tasks with conditional features
    std::cout << "\n--- Task Execution ---" << std::endl;
    app.performTask("Data Analysis");
    app.performTask("Report Generation");

    // Shutdown
    std::cout << "\n--- Shutdown ---" << std::endl;
    app.shutdown();

    std::cout << "\n=== Features Summary ===" << std::endl;
    std::cout << "Compiled features:" << std::endl;
#ifdef FEATURE_LOGGING
    std::cout << "  - Logging: YES" << std::endl;
#else
    std::cout << "  - Logging: NO" << std::endl;
#endif

#ifdef FEATURE_DATABASE
    std::cout << "  - Database: YES" << std::endl;
#else
    std::cout << "  - Database: NO" << std::endl;
#endif

#ifdef FEATURE_NETWORKING
    std::cout << "  - Networking: YES" << std::endl;
#else
    std::cout << "  - Networking: NO" << std::endl;
#endif

    return 0;
}
```

**Compile and run:**
```bash
# Release build
g++ -std=c++20 -Wall config_system.cpp -o config_system
./config_system

# Debug build
g++ -std=c++20 -Wall -DDEBUG_MODE config_system.cpp -o config_system_debug
./config_system_debug
```

## Complete Project 2: Build System with Multiple Configurations

**File: `build_config.h`**

```cpp
#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

// Build targets
// #define BUILD_TARGET_WINDOWS
// #define BUILD_TARGET_LINUX
#define BUILD_TARGET_MACOS

// Build types
// #define BUILD_DEBUG
// #define BUILD_RELEASE
#define BUILD_PROFILE

// Detect build target if not specified
#if !defined(BUILD_TARGET_WINDOWS) && !defined(BUILD_TARGET_LINUX) && !defined(BUILD_TARGET_MACOS)
    #ifdef _WIN32
        #define BUILD_TARGET_WINDOWS
    #elif __linux__
        #define BUILD_TARGET_LINUX
    #elif __APPLE__
        #define BUILD_TARGET_MACOS
    #endif
#endif

// Configuration based on build type
#ifdef BUILD_DEBUG
    #define OPTIMIZATION_LEVEL 0
    #define ENABLE_DEBUG_SYMBOLS
    #define ENABLE_LOGGING
    #define ENABLE_PROFILING
#elif defined(BUILD_PROFILE)
    #define OPTIMIZATION_LEVEL 2
    #define ENABLE_DEBUG_SYMBOLS
    #define ENABLE_PROFILING
    #define ENABLE_LOGGING
#elif defined(BUILD_RELEASE)
    #define OPTIMIZATION_LEVEL 3
    #undef ENABLE_DEBUG_SYMBOLS
    #undef ENABLE_PROFILING
    #undef ENABLE_LOGGING
#else
    #define OPTIMIZATION_LEVEL 0
#endif

// Platform-specific definitions
#ifdef BUILD_TARGET_WINDOWS
    #define PLATFORM_NAME "Windows"
    #define DLL_EXPORT __declspec(dllexport)
    #define PATH_MAX 260
#elif defined(BUILD_TARGET_LINUX)
    #define PLATFORM_NAME "Linux"
    #define DLL_EXPORT __attribute__((visibility("default")))
    #define PATH_MAX 4096
#elif defined(BUILD_TARGET_MACOS)
    #define PLATFORM_NAME "macOS"
    #define DLL_EXPORT __attribute__((visibility("default")))
    #define PATH_MAX 1024
#else
    #define PLATFORM_NAME "Unknown"
    #define DLL_EXPORT
    #define PATH_MAX 256
#endif

// Performance macros
#ifdef ENABLE_PROFILING
    #include <chrono>
    #define PROFILE_SCOPE(name) ProfileTimer timer##__LINE__(name)

    class ProfileTimer {
        const char* name;
        std::chrono::high_resolution_clock::time_point start;
    public:
        ProfileTimer(const char* n) : name(n) {
            start = std::chrono::high_resolution_clock::now();
        }
        ~ProfileTimer() {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "[PROFILE] " << name << ": " << duration.count() << " μs" << std::endl;
        }
    };
#else
    #define PROFILE_SCOPE(name)
#endif

// Logging macros
#ifdef ENABLE_LOGGING
    #define LOG(msg) std::cout << "[LOG] " << msg << std::endl
#else
    #define LOG(msg)
#endif

#endif // BUILD_CONFIG_H
```

**File: `build_system_demo.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "build_config.h"

void simulateWork(int ms) {
    PROFILE_SCOPE("simulateWork");
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void processData(const std::vector<int>& data) {
    PROFILE_SCOPE("processData");

    LOG("Processing " + std::to_string(data.size()) + " elements");

    int sum = 0;
    for (int value : data) {
        sum += value;
        simulateWork(1);  // Simulate processing time
    }

    LOG("Sum: " + std::to_string(sum));
}

void performCalculations() {
    PROFILE_SCOPE("performCalculations");

    LOG("Performing complex calculations");

    double result = 0.0;
    for (int i = 0; i < 1000; ++i) {
        result += i * 0.5;
    }

    LOG("Calculation result: " + std::to_string(result));
}

int main() {
    std::cout << "=== Build System Demo ===" << std::endl;

    // Display build configuration
    std::cout << "\n--- Build Configuration ---" << std::endl;
    std::cout << "Platform: " << PLATFORM_NAME << std::endl;
    std::cout << "Optimization Level: " << OPTIMIZATION_LEVEL << std::endl;

#ifdef BUILD_DEBUG
    std::cout << "Build Type: Debug" << std::endl;
#elif defined(BUILD_PROFILE)
    std::cout << "Build Type: Profile" << std::endl;
#elif defined(BUILD_RELEASE)
    std::cout << "Build Type: Release" << std::endl;
#else
    std::cout << "Build Type: Unknown" << std::endl;
#endif

    std::cout << "\n--- Enabled Features ---" << std::endl;
#ifdef ENABLE_DEBUG_SYMBOLS
    std::cout << "  - Debug Symbols: YES" << std::endl;
#else
    std::cout << "  - Debug Symbols: NO" << std::endl;
#endif

#ifdef ENABLE_LOGGING
    std::cout << "  - Logging: YES" << std::endl;
#else
    std::cout << "  - Logging: NO" << std::endl;
#endif

#ifdef ENABLE_PROFILING
    std::cout << "  - Profiling: YES" << std::endl;
#else
    std::cout << "  - Profiling: NO" << std::endl;
#endif

    // Platform-specific information
    std::cout << "\n--- Platform Specific ---" << std::endl;
    std::cout << "PATH_MAX: " << PATH_MAX << std::endl;

    // Run profiled code
    std::cout << "\n--- Performance Testing ---" << std::endl;

    {
        PROFILE_SCOPE("main_execution");

        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        processData(data);

        performCalculations();

        simulateWork(50);
    }

    std::cout << "\n--- Compilation Info ---" << std::endl;
    std::cout << "Compiled: " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "Compiler: "
#ifdef __GNUC__
              << "GCC " << __GNUC__ << "." << __GNUC_MINOR__
#elif _MSC_VER
              << "MSVC " << _MSC_VER
#elif __clang__
              << "Clang " << __clang_major__ << "." << __clang_minor__
#else
              << "Unknown"
#endif
              << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
# Profile build (default in build_config.h)
g++ -std=c++20 -Wall build_system_demo.cpp -o build_system_demo
./build_system_demo

# Debug build
g++ -std=c++20 -Wall -DBUILD_DEBUG -UBUILD_PROFILE build_system_demo.cpp -o build_system_demo_debug
./build_system_demo_debug

# Release build
g++ -std=c++20 -Wall -DBUILD_RELEASE -UBUILD_PROFILE build_system_demo.cpp -o build_system_demo_release -O3
./build_system_demo_release
```

## Best Practices and Common Pitfalls

### Best Practices:

1. **Use const or constexpr instead of #define for constants**
   ```cpp
   // Prefer:
   constexpr double PI = 3.14159;
   // Over:
   #define PI 3.14159
   ```

2. **Use include guards or #pragma once**
   - Prevents multiple inclusion

3. **Parenthesize macro arguments**
   ```cpp
   #define SQUARE(x) ((x) * (x))  // Good
   // Not: #define SQUARE(x) x * x  // Bad
   ```

4. **Use inline functions instead of function macros when possible**
   ```cpp
   // Prefer:
   inline int square(int x) { return x * x; }
   // Over:
   #define SQUARE(x) ((x) * (x))
   ```

5. **Use conditional compilation for platform-specific code**

6. **Document your macros clearly**

7. **Avoid macro side effects**
   ```cpp
   int x = 5;
   SQUARE(x++);  // BAD: x incremented twice
   ```

### Common Pitfalls:

1. **Macro side effects** - Arguments evaluated multiple times
2. **No type safety** - Macros don't check types
3. **Debugging difficulty** - Macros expanded before compilation
4. **Name collisions** - Macros don't respect namespaces
5. **Missing parentheses** - Operator precedence issues

## Summary

In this lesson, you learned:

- **Preprocessor basics** - Text processing before compilation
- **#include directive** - File inclusion
- **#define macros** - Object-like and function-like macros
- **Conditional compilation** - #ifdef, #ifndef, #if, #else, #elif
- **Predefined macros** - __FILE__, __LINE__, __DATE__, __TIME__
- **Include guards** - Preventing multiple inclusion
- **#pragma directives** - Compiler-specific directives
- **Best practices** - When to use macros vs. modern C++ features

The preprocessor is a powerful tool for code organization and conditional compilation, but modern C++ provides better alternatives for many use cases.

## Next Lesson

Lesson 16: Introduction to Object-Oriented Programming (Classes and Objects)

## Additional Resources

- C++ Reference: Preprocessor
- C++ Core Guidelines: Avoid macros
- Modern C++ Design patterns
- Practice: Refactor macros to use modern C++ features

---

**Exercise**: Create a cross-platform logging system using preprocessor directives that compiles differently for Windows, Linux, and macOS, with debug and release build configurations.
