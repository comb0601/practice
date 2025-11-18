# Lesson 77: Preprocessor Directives

**Duration**: 5 hours
**Difficulty**: Intermediate

Preprocessor directives are processed before compilation.

## Include Guards:

```cpp
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

class MyClass {
    // ...
};

#endif  // MYHEADER_H

// Or use #pragma once (non-standard but widely supported)
#pragma once

class MyClass {
    // ...
};
```

## Macros:

```cpp
#include <iostream>

#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    std::cout << "PI: " << PI << "\n";
    std::cout << "SQUARE(5): " << SQUARE(5) << "\n";
    std::cout << "MAX(10, 20): " << MAX(10, 20) << "\n";

    // Note: Prefer const and inline functions over macros

    return 0;
}
```

## Conditional Compilation:

```cpp
#include <iostream>

#define DEBUG

int main()
{
    #ifdef DEBUG
        std::cout << "Debug mode\n";
    #else
        std::cout << "Release mode\n";
    #endif

    #if defined(DEBUG) && defined(VERBOSE)
        std::cout << "Debug and verbose\n";
    #endif

    return 0;
}
```

## Predefined Macros:

```cpp
#include <iostream>

int main()
{
    std::cout << "File: " << __FILE__ << "\n";
    std::cout << "Line: " << __LINE__ << "\n";
    std::cout << "Date: " << __DATE__ << "\n";
    std::cout << "Time: " << __TIME__ << "\n";

    #ifdef __cplusplus
        std::cout << "C++ standard: " << __cplusplus << "\n";
    #endif

    return 0;
}
```

**Key Takeaways:**
- Preprocessor runs before compilation
- #include for header files
- #define for macros and constants
- #ifdef for conditional compilation
- Include guards prevent multiple inclusion
- Predefined macros for metadata
- Modern C++ prefers const over #define
