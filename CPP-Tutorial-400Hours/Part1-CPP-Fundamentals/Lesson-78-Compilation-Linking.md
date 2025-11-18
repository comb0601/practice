# Lesson 78: Compilation and Linking

**Duration**: 5 hours
**Difficulty**: Intermediate

Understanding the build process from source to executable.

## Build Process:

```
1. Preprocessing:  .cpp → (preprocessed) → .i
   - Handle #include, #define, #ifdef
   - Remove comments

2. Compilation:    .i → .s (assembly)
   - Parse and analyze code
   - Generate assembly

3. Assembly:       .s → .o (object file)
   - Convert to machine code

4. Linking:        .o files → executable
   - Combine object files
   - Resolve symbols
   - Link libraries
```

## Multiple File Project:

```cpp
// math.h
#ifndef MATH_H
#define MATH_H

int add(int a, int b);
int multiply(int a, int b);

#endif

// math.cpp
#include "math.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

// main.cpp
#include <iostream>
#include "math.h"

int main()
{
    std::cout << "5 + 3 = " << add(5, 3) << "\n";
    std::cout << "5 * 3 = " << multiply(5, 3) << "\n";
    return 0;
}
```

## Compilation Commands:

```bash
# Compile to object files
g++ -c math.cpp -o math.o
g++ -c main.cpp -o main.o

# Link object files
g++ math.o main.o -o program

# Or all at once
g++ main.cpp math.cpp -o program
```

## Static Libraries:

```bash
# Create static library
ar rcs libmath.a math.o

# Link with static library
g++ main.cpp -L. -lmath -o program
```

## Dynamic Libraries:

```bash
# Create shared library
g++ -shared -fPIC math.cpp -o libmath.so

# Link with shared library
g++ main.cpp -L. -lmath -o program

# Run with library path
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./program
```

**Key Takeaways:**
- Compilation: source → object files
- Linking: object files → executable
- Header files declare interfaces
- Source files implement functionality
- Static libraries (.a/.lib) linked at compile time
- Dynamic libraries (.so/.dll) loaded at runtime
- Include paths with -I, library paths with -L
