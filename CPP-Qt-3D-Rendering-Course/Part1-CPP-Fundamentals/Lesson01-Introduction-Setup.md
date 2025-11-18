# Lesson 01: Introduction to C++ and Development Environment Setup

**Duration: 2 hours**

## Table of Contents
1. What is C++?
2. History and Evolution
3. Setting Up Development Environment
4. Your First C++ Program
5. Compilation Process
6. Practical Exercises

## 1. What is C++?

C++ is a general-purpose programming language created by Bjarne Stroustrup as an extension of the C programming language. It supports:
- **Procedural Programming**: Step-by-step instructions
- **Object-Oriented Programming (OOP)**: Classes and objects
- **Generic Programming**: Templates
- **Low-Level Memory Manipulation**: Pointers and references

### Key Features:
- **Performance**: Close to hardware, minimal overhead
- **Portability**: Write once, compile anywhere
- **Rich Standard Library**: STL (Standard Template Library)
- **Multi-Paradigm**: Supports multiple programming styles

## 2. History and Evolution

- **1979**: Bjarne Stroustrup starts "C with Classes"
- **1983**: Renamed to C++
- **1998**: C++98 (First ISO standard)
- **2011**: C++11 (Major update: auto, lambda, smart pointers)
- **2014**: C++14 (Refinements)
- **2017**: C++17 (Filesystem, optional, variant)
- **2020**: C++20 (Concepts, ranges, coroutines, modules)
- **2023**: C++23 (Latest standard)

## 3. Setting Up Development Environment

### Windows Setup:

#### Option 1: Visual Studio (Recommended for beginners)
```bash
# Download Visual Studio Community (Free)
# https://visualstudio.microsoft.com/downloads/
# Select: Desktop development with C++
```

#### Option 2: MinGW-w64 + VS Code
```bash
# 1. Download MinGW-w64
# https://sourceforge.net/projects/mingw-w64/

# 2. Install and add to PATH
# 3. Install VS Code
# 4. Install C++ extension
```

### Linux Setup:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential g++ gdb cmake

# Fedora
sudo dnf install gcc-c++ gdb cmake

# Arch Linux
sudo pacman -S base-devel gdb cmake

# Verify installation
g++ --version
```

### macOS Setup:
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or install via Homebrew
brew install gcc cmake

# Verify
g++ --version
```

## 4. Your First C++ Program

### hello_world.cpp
```cpp
// Include the iostream library for input/output operations
#include <iostream>

// Main function - entry point of every C++ program
int main() {
    // Output text to console
    std::cout << "Hello, World!" << std::endl;

    // Return 0 to indicate successful execution
    return 0;
}
```

### Detailed Explanation:

#### Line by Line:

```cpp
#include <iostream>
```
- **#include**: Preprocessor directive
- **iostream**: Input/Output Stream library
- Contains: std::cout, std::cin, std::endl

```cpp
int main() {
```
- **int**: Return type (integer)
- **main()**: Entry point function
- Every C++ program must have exactly one main() function

```cpp
std::cout << "Hello, World!" << std::endl;
```
- **std**: Standard namespace
- **cout**: Character Output (console output)
- **<<**: Stream insertion operator
- **"Hello, World!"**: String literal
- **std::endl**: End line (newline + flush buffer)

```cpp
return 0;
```
- Returns 0 to operating system
- 0 = success, non-zero = error

## 5. Compilation Process

### Understanding the Build Process:

```
Source Code (.cpp)
    ↓ [Preprocessor]
Expanded Source (.i)
    ↓ [Compiler]
Assembly Code (.s)
    ↓ [Assembler]
Object Code (.o/.obj)
    ↓ [Linker]
Executable (.exe/no extension)
```

### Step-by-Step Compilation:

#### Using g++:
```bash
# Basic compilation
g++ hello_world.cpp -o hello_world

# Run the program
./hello_world  # Linux/macOS
hello_world.exe  # Windows

# Detailed compilation steps
g++ -E hello_world.cpp -o hello_world.i  # Preprocessing
g++ -S hello_world.i -o hello_world.s    # Compilation
g++ -c hello_world.s -o hello_world.o    # Assembly
g++ hello_world.o -o hello_world         # Linking
```

#### Compiler Flags:
```bash
# With warnings
g++ -Wall -Wextra hello_world.cpp -o hello_world

# With debugging symbols
g++ -g hello_world.cpp -o hello_world

# With optimization
g++ -O2 hello_world.cpp -o hello_world

# C++ standard version
g++ -std=c++20 hello_world.cpp -o hello_world

# All together
g++ -Wall -Wextra -g -std=c++20 hello_world.cpp -o hello_world
```

## 6. Practical Exercises

### Exercise 1: Basic Output
```cpp
#include <iostream>

int main() {
    std::cout << "Welcome to C++ Programming!" << std::endl;
    std::cout << "This is line 2" << std::endl;
    std::cout << "This is line 3" << std::endl;
    return 0;
}
```

### Exercise 2: Multiple Output Statements
```cpp
#include <iostream>

int main() {
    std::cout << "Name: John Doe" << std::endl;
    std::cout << "Age: 25" << std::endl;
    std::cout << "Profession: Software Developer" << std::endl;
    return 0;
}
```

### Exercise 3: Escape Sequences
```cpp
#include <iostream>

int main() {
    std::cout << "Tab\tSeparated\tText" << std::endl;
    std::cout << "New\nLine\nCharacters" << std::endl;
    std::cout << "Quote: \"Hello World\"" << std::endl;
    std::cout << "Backslash: \\" << std::endl;
    return 0;
}
```

### Exercise 4: ASCII Art
```cpp
#include <iostream>

int main() {
    std::cout << "  *****  " << std::endl;
    std::cout << " *     * " << std::endl;
    std::cout << "*  C++  *" << std::endl;
    std::cout << " *     * " << std::endl;
    std::cout << "  *****  " << std::endl;
    return 0;
}
```

### Exercise 5: Using namespace std
```cpp
#include <iostream>

// Using namespace std (use with caution)
using namespace std;

int main() {
    cout << "No need for std:: prefix" << endl;
    cout << "But be careful in large projects!" << endl;
    return 0;
}
```

## Common Errors and Solutions

### Error 1: Missing semicolon
```cpp
// Wrong
std::cout << "Hello" << std::endl

// Correct
std::cout << "Hello" << std::endl;
```

### Error 2: Missing return statement
```cpp
// Wrong
int main() {
    std::cout << "Hello" << std::endl;
}

// Correct
int main() {
    std::cout << "Hello" << std::endl;
    return 0;
}
```

### Error 3: Wrong include
```cpp
// Wrong
#include <iostream.h>  // Old C++ style

// Correct
#include <iostream>    // Modern C++
```

## Project: Personal Information Display

Create a program that displays your information in a formatted way:

```cpp
#include <iostream>

int main() {
    std::cout << "================================" << std::endl;
    std::cout << "   PERSONAL INFORMATION CARD    " << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Name:       Your Name" << std::endl;
    std::cout << "Age:        25" << std::endl;
    std::cout << "Location:   City, Country" << std::endl;
    std::cout << "Occupation: Student" << std::endl;
    std::cout << "Goal:       Master C++ Programming" << std::endl;

    std::cout << std::endl;
    std::cout << "================================" << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- What C++ is and its history
- How to set up your development environment
- The structure of a C++ program
- How to compile and run C++ programs
- Basic output using std::cout
- Common errors and how to fix them

## Next Lesson Preview

In Lesson 02, we'll cover:
- Variables and data types
- Constants
- Type casting
- sizeof operator

## Additional Resources

- [cppreference.com](https://en.cppreference.com/)
- [cplusplus.com](http://www.cplusplus.com/)
- C++ Standard Documentation
- Compiler documentation (GCC, Clang, MSVC)
