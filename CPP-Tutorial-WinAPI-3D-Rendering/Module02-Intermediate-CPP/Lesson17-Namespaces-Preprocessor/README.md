# Lesson 17: Namespaces & Preprocessor in C++

**Estimated Time**: 4 hours | **Difficulty**: Intermediate

## Table of Contents
1. [Namespaces](#namespaces) 2. [Using Directives](#using) 3. [Nested Namespaces](#nested)
4. [Anonymous Namespaces](#anonymous) 5. [Preprocessor Basics](#preprocessor)
6. [Macros](#macros) 7. [Conditional Compilation](#conditional) 8. [Include Guards](#guards)

## 1. Namespaces {#namespaces}
Namespaces prevent name collisions and organize code.

```cpp
namespace MyNamespace {
    int value = 42;
    void function() { }
}

int main() {
    MyNamespace::function();
    std::cout << MyNamespace::value;
}
```

## 2. Using Directives {#using}
```cpp
using namespace std;           // Entire namespace
using std::cout;              // Specific name
namespace fs = std::filesystem;  // Alias
```

**Warning**: Avoid `using namespace` in headers!

## 3. Nested Namespaces {#nested}
```cpp
namespace Outer {
    namespace Inner {
        void function() { }
    }
}

// C++17 syntax:
namespace Outer::Inner {
    void function() { }
}
```

## 4. Anonymous Namespaces {#anonymous}
Internal linkage - visible only in current translation unit:
```cpp
namespace {
    int internal_variable = 10;
    void internal_function() { }
}
```

Equivalent to `static` but preferred in C++.

## 5. Preprocessor Basics {#preprocessor}
Preprocessor directives process code before compilation.

**Common directives**:
- `#include`: Include files
- `#define`: Define macros
- `#ifdef`, `#ifndef`: Conditional compilation
- `#pragma`: Compiler-specific directives

## 6. Macros {#macros}
```cpp
#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

**Warning**: Prefer `const` and `inline` functions to macros!

**Macro operators**:
- `#`: Stringize
- `##`: Token pasting

```cpp
#define STRINGIZE(x) #x
#define CONCAT(a,b) a##b
```

## 7. Conditional Compilation {#conditional}
```cpp
#ifdef DEBUG
    #define LOG(x) std::cout << x << std::endl
#else
    #define LOG(x)
#endif

#if VERSION >= 2
    // Version 2+ code
#elif VERSION == 1
    // Version 1 code
#else
    // Default code
#endif
```

**Predefined macros**:
- `__FILE__`: Current file name
- `__LINE__`: Current line number
- `__DATE__`: Compilation date
- `__TIME__`: Compilation time
- `__cplusplus`: C++ standard version

## 8. Include Guards {#guards}
```cpp
#ifndef MY_HEADER_H
#define MY_HEADER_H
// Header content
#endif
```

**Modern alternative**:
```cpp
#pragma once
```

**Best Practices**:
1. Use namespaces to organize code
2. Avoid `using namespace` in headers
3. Prefer `const`/`inline` over macros
4. Use `#pragma once` for include guards
5. Use conditional compilation for platform-specific code
6. Keep preprocessor usage minimal

**Next**: Lesson 18 - Multiple Inheritance
