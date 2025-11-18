# Lesson 79: Debugging Techniques

**Duration**: 5 hours
**Difficulty**: Intermediate

Essential debugging techniques and tools for C++ development.

## Print Debugging:

```cpp
#include <iostream>

int buggyFunction(int x) {
    std::cout << "DEBUG: Entering buggyFunction, x = " << x << "\n";

    int result = x * 2;
    std::cout << "DEBUG: result = " << result << "\n";

    return result;
}

int main()
{
    int value = 5;
    std::cout << "DEBUG: value = " << value << "\n";

    int output = buggyFunction(value);
    std::cout << "DEBUG: output = " << output << "\n";

    return 0;
}
```

## Assertions:

```cpp
#include <cassert>
#include <iostream>

double divide(double a, double b) {
    assert(b != 0 && "Division by zero");
    return a / b;
}

int main()
{
    std::cout << divide(10, 2) << "\n";
    // divide(10, 0);  // Assertion fails

    return 0;
}
```

## Debug Macros:

```cpp
#include <iostream>

#ifdef DEBUG
    #define LOG(x) std::cout << "LOG: " << x << "\n"
    #define DEBUG_PRINT(var) std::cout << #var << " = " << var << "\n"
#else
    #define LOG(x)
    #define DEBUG_PRINT(var)
#endif

int main()
{
    int x = 42;
    LOG("Starting program");
    DEBUG_PRINT(x);

    return 0;
}
```

## GDB Basics:

```bash
# Compile with debug symbols
g++ -g program.cpp -o program

# Run with GDB
gdb ./program

# GDB commands:
(gdb) break main        # Set breakpoint at main
(gdb) run              # Start program
(gdb) next             # Step over
(gdb) step             # Step into
(gdb) print variable   # Print variable value
(gdb) continue         # Continue execution
(gdb) backtrace        # Show call stack
(gdb) quit             # Exit GDB
```

## Visual Studio Debugger:

```
- F9: Set/remove breakpoint
- F5: Start debugging
- F10: Step over
- F11: Step into
- Shift+F11: Step out
- Watch window: Monitor variables
- Call stack: View function calls
- Immediate window: Execute expressions
```

## Common Bugs:

```cpp
// Uninitialized variables
int x;  // Garbage value
// Fix: int x = 0;

// Off-by-one errors
for (int i = 0; i <= 10; i++)  // 11 iterations
// Fix: i < 10

// Memory leaks
int* p = new int(5);
// Missing: delete p;

// Dangling pointers
int* p = new int(5);
delete p;
*p = 10;  // Accessing deleted memory

// Buffer overflow
char buffer[10];
strcpy(buffer, "This is way too long");
```

**Key Takeaways:**
- Print debugging for quick insights
- Assertions catch logic errors
- Debug macros for conditional logging
- GDB for command-line debugging
- Visual Studio debugger for IDE
- Watch for common pitfalls
- Use sanitizers (AddressSanitizer, UndefinedBehaviorSanitizer)
