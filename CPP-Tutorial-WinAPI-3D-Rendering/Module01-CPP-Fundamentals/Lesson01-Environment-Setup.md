# Lesson 01: Environment Setup & First Program
**Estimated Time: 4 hours**

## Table of Contents
1. [Introduction](#introduction)
2. [Installing Visual Studio](#installing-visual-studio)
3. [Understanding the IDE](#understanding-the-ide)
4. [Creating Your First Project](#creating-your-first-project)
5. [Understanding the Code](#understanding-the-code)
6. [Compilation Process](#compilation-process)
7. [Debugging Basics](#debugging-basics)
8. [Exercises](#exercises)

## Introduction

Welcome to your C++ journey! This lesson will guide you through setting up your development environment and creating your first C++ program. By the end of this lesson, you'll understand:
- How to install and configure Visual Studio
- The structure of a C++ program
- How to compile and run code
- Basic debugging techniques

## Installing Visual Studio

### Step 1: Download Visual Studio
1. Visit https://visualstudio.microsoft.com/
2. Download "Visual Studio Community" (free version)
3. Run the installer

### Step 2: Select Workloads
During installation, select:
- **Desktop development with C++**
- This includes:
  - MSVC C++ compiler
  - Windows SDK
  - CMake tools
  - C++ profiling tools

### Step 3: Individual Components
Make sure these are checked:
- Windows 10 SDK (latest version)
- C++ CMake tools for Windows
- C++ Clang tools for Windows
- Test Adapter for Google Test

## Understanding the IDE

### Main Components:
1. **Solution Explorer**: Shows your project files
2. **Code Editor**: Where you write code
3. **Output Window**: Shows build messages
4. **Error List**: Displays compilation errors
5. **Debugging Windows**: Locals, Watch, Call Stack

### Keyboard Shortcuts (Essential):
- `Ctrl + F5`: Run without debugging
- `F5`: Start debugging
- `F9`: Toggle breakpoint
- `F10`: Step over
- `F11`: Step into
- `Ctrl + K, C`: Comment selection
- `Ctrl + K, U`: Uncomment selection

## Creating Your First Project

### Step-by-Step Guide:

1. Open Visual Studio
2. Click "Create a new project"
3. Select "Console App" (C++)
4. Name: "HelloWorld"
5. Click "Create"

Visual Studio will generate a basic template.

## Understanding the Code

Let's break down the generated code:

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}
```

### Line-by-Line Explanation:

#### Line 1: `#include <iostream>`
- `#include`: Preprocessor directive
- `<iostream>`: Header file for input/output operations
- This gives us access to `std::cout` and `std::cin`

#### Line 3: `int main()`
- `int`: Return type (integer)
- `main`: Special function - program entry point
- `()`: Parameter list (empty here)
- Every C++ program MUST have exactly one `main()` function

#### Line 5: `std::cout << "Hello World!\n";`
- `std::cout`: Character Output stream
- `<<`: Stream insertion operator
- `"Hello World!\n"`: String literal
- `\n`: Newline character
- `;`: Statement terminator (required!)

### Complete First Program with Comments:

```cpp
// Single-line comment: This is ignored by compiler
/*
   Multi-line comment
   Also ignored by compiler
*/

#include <iostream>  // Include input/output stream library

// Main function - program execution starts here
int main()
{
    // Output text to console
    std::cout << "Hello World!\n";

    // Return 0 indicates successful execution
    return 0;
}
```

## Compilation Process

### Understanding the Build Process:

1. **Preprocessing**
   - Handles `#include`, `#define`, etc.
   - Creates expanded source file

2. **Compilation**
   - Converts C++ code to assembly
   - Then to machine code (object file .obj)

3. **Linking**
   - Combines object files
   - Links with libraries
   - Creates executable (.exe)

### Build Commands in Visual Studio:

- `Ctrl + Shift + B`: Build solution
- `Ctrl + F7`: Compile current file only
- `Build > Clean Solution`: Remove build artifacts

## Debugging Basics

### What is Debugging?
Finding and fixing errors in your code.

### Types of Errors:

1. **Syntax Errors**: Code doesn't follow C++ rules
   ```cpp
   std::cout << "Missing semicolon"  // ERROR: missing ;
   ```

2. **Linking Errors**: Can't find function/library
   ```cpp
   // ERROR: Function not defined
   void myFunction();  // Declaration
   int main() {
       myFunction();   // ERROR: No definition found
   }
   ```

3. **Runtime Errors**: Crash during execution
   ```cpp
   int* ptr = nullptr;
   *ptr = 5;  // ERROR: Null pointer dereference
   ```

4. **Logic Errors**: Code runs but gives wrong results
   ```cpp
   int average = (10 + 20) * 2;  // Wrong! Should divide, not multiply
   ```

### Using Breakpoints:

1. Click left margin to set breakpoint (red dot)
2. Press `F5` to start debugging
3. Program pauses at breakpoint
4. Use `F10` to step through code
5. Hover over variables to see values

### Example Debugging Session:

```cpp
#include <iostream>

int main()
{
    int x = 10;        // Set breakpoint here (F9)
    int y = 20;        // Step to here (F10)
    int sum = x + y;   // Step to here (F10)

    std::cout << "Sum: " << sum << "\n";

    return 0;
}
```

## Complete Example Programs

### Example 1: Enhanced Hello World

```cpp
#include <iostream>

int main()
{
    // Multiple output statements
    std::cout << "=================================\n";
    std::cout << "   Welcome to C++ Programming!  \n";
    std::cout << "=================================\n";
    std::cout << "\n";
    std::cout << "This is my first C++ program.\n";
    std::cout << "I'm learning step by step!\n";

    return 0;
}
```

### Example 2: Using Different Escape Sequences

```cpp
#include <iostream>

int main()
{
    std::cout << "Escape sequences:\n";
    std::cout << "New line: \\n\n";
    std::cout << "Tab: \\t\t\tlike this\n";
    std::cout << "Backslash: \\\\ \n";
    std::cout << "Quote: \" \n";
    std::cout << "Single quote: \' \n";

    return 0;
}
```

Output:
```
Escape sequences:
New line: \n
Tab: \t			like this
Backslash: \
Quote: "
Single quote: '
```

### Example 3: ASCII Art

```cpp
#include <iostream>

int main()
{
    std::cout << "    /\\_/\\  \n";
    std::cout << "   ( o.o ) \n";
    std::cout << "    > ^ <  \n";
    std::cout << "   /|   |\\ \n";
    std::cout << "  (_|   |_)\n";
    std::cout << "\n";
    std::cout << "ASCII Cat by ChatGPT\n";

    return 0;
}
```

## Project Organization Best Practices

### Folder Structure:
```
MySolution/
├── MySolution.sln
└── MyProject/
    ├── MyProject.vcxproj
    ├── main.cpp
    ├── Source Files/
    └── Header Files/
```

### File Naming Conventions:
- Use descriptive names: `main.cpp`, not `a.cpp`
- Header files: `.h` or `.hpp`
- Source files: `.cpp`
- No spaces in filenames: Use `MyFile.cpp` not `My File.cpp`

## Common Beginner Mistakes

### 1. Forgetting Semicolon
```cpp
std::cout << "Hello"  // ERROR: Missing semicolon
```

### 2. Case Sensitivity
```cpp
#include <iostream>
Int main()  // ERROR: Should be 'int' (lowercase)
{
    std::Cout << "Hello";  // ERROR: Should be 'cout' (lowercase)
}
```

### 3. Missing Return Statement
```cpp
int main()
{
    std::cout << "Hello\n";
    // Warning: No return statement (should have 'return 0;')
}
```

### 4. Wrong Main Signature
```cpp
void main()  // WRONG: Should return int
{
    // ...
}
```

## Exercises

### Exercise 1: Modify Hello World
Modify the Hello World program to display your name and favorite programming language.

Expected output:
```
Hello! My name is [Your Name]
My favorite language is C++
```

### Exercise 2: Create a Banner
Write a program that displays a banner with your name using ASCII art.

### Exercise 3: Multiple Lines
Write a program that outputs a short poem or quote (at least 4 lines).

### Exercise 4: Debug Practice
Find and fix the errors in this code:

```cpp
#include <iostream>

int Main()
{
    std::cout << "Line 1\n"
    std::cout << "Line 2\n";
    std::Cout << "Line 3\n";
    return 0
}
```

### Exercise 5: Experiment
Create a program that uses at least 5 different escape sequences.

## Summary

In this lesson, you learned:
- ✅ How to install and configure Visual Studio
- ✅ The structure of a basic C++ program
- ✅ What `#include`, `main()`, and `std::cout` do
- ✅ How to compile and run programs
- ✅ Basic debugging with breakpoints
- ✅ Common beginner mistakes to avoid

## Next Lesson Preview

In Lesson 02, we'll dive into:
- Variables and data types
- Getting user input with `std::cin`
- Arithmetic operations
- Type conversion

## Additional Resources

- Visual Studio Documentation: https://docs.microsoft.com/en-us/visualstudio/
- C++ Reference: https://en.cppreference.com/
- ISO C++ Guidelines: https://isocpp.org/

## Checklist

Before moving to the next lesson, make sure you can:
- [ ] Create a new C++ Console project
- [ ] Write and run a "Hello World" program
- [ ] Explain what `#include`, `main()`, and `std::cout` do
- [ ] Set breakpoints and step through code
- [ ] Identify and fix basic syntax errors

---

**Congratulations on completing Lesson 01!**

**Time to practice**: Spend at least 1 hour experimenting with variations of the Hello World program. Try breaking it and fixing it to understand how errors work.
