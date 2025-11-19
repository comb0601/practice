# Build Instructions - Part 1 Fundamentals

This directory contains **83 complete C++ lesson examples** covering fundamental C++ programming concepts. Each lesson has its own Visual Studio project file (.vcxproj) and can be built independently or as part of the complete collection.

## Table of Contents

- [Quick Start](#quick-start)
- [Prerequisites](#prerequisites)
- [Building on Windows](#building-on-windows)
- [Building on Linux/Mac](#building-on-linuxmac)
- [Running Examples](#running-examples)
- [Project Structure](#project-structure)
- [Individual Project Files](#individual-project-files)
- [Troubleshooting](#troubleshooting)

## Quick Start

### Windows (Visual Studio)
```batch
build_all.bat
run_all.bat
```

### Linux/Mac
```bash
./build_all.sh
./run_all.sh
```

## Prerequisites

### Windows
- **Visual Studio 2019 or later** (Community Edition works fine)
  - Download: https://visualstudio.microsoft.com/downloads/
  - Install "Desktop development with C++" workload
- **CMake 3.10 or later** (optional, for build scripts)
  - Download: https://cmake.org/download/

### Linux
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake

# Fedora/RHEL
sudo dnf install gcc-c++ cmake
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake
```

## Building on Windows

### Method 1: Using Build Script (Recommended)

1. Open Command Prompt or PowerShell
2. Navigate to this directory
3. Run the build script:
   ```batch
   build_all.bat
   ```

This will:
- Configure CMake for Visual Studio
- Build all 83 lessons in both Debug and Release configurations
- Place executables in `build/bin/Debug/` and `build/bin/Release/`

### Method 2: Using Visual Studio IDE

1. Open Visual Studio
2. File → Open → Project/Solution
3. Navigate to this directory
4. Open any `.vcxproj` file (e.g., `Lesson01_HelloWorld.vcxproj`)
5. Press `F5` to build and run, or `Ctrl+Shift+B` to build only

### Method 3: Using MSBuild (Command Line)

```batch
# Build a specific lesson
msbuild Lesson01_HelloWorld.vcxproj /p:Configuration=Release /p:Platform=x64

# Build with maximum verbosity
msbuild Lesson01_HelloWorld.vcxproj /p:Configuration=Debug /p:Platform=x64 /v:detailed
```

### Method 4: Using CMake Manually

```batch
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

For Visual Studio 2022:
```batch
cmake .. -G "Visual Studio 17 2022" -A x64
```

## Building on Linux/Mac

### Method 1: Using Build Script (Recommended)

```bash
chmod +x build_all.sh
./build_all.sh
```

This will:
- Configure CMake with GCC/Clang
- Build all 83 lessons in Release mode
- Place executables in `build/bin/`
- Use parallel compilation for faster builds

### Method 2: Using CMake Manually

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

For Debug build:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Method 3: Using G++ Directly

```bash
# Compile a single lesson
g++ -std=c++17 -Wall -Wextra -O2 -o Lesson01_HelloWorld Lesson01_HelloWorld.cpp

# With debugging symbols
g++ -std=c++17 -Wall -Wextra -g -o Lesson01_HelloWorld Lesson01_HelloWorld.cpp

# For threading lessons (71-75), add -pthread
g++ -std=c++17 -Wall -Wextra -O2 -pthread -o Lesson71_Threads Lesson71_Threads.cpp
```

## Running Examples

### Run All Lessons Sequentially

**Windows:**
```batch
run_all.bat
```

**Linux/Mac:**
```bash
./run_all.sh
```

These scripts will:
- Run all 83 lessons in order
- Pause between each lesson for review
- Display error counts at the end
- Allow Ctrl+C to stop at any time

### Run Individual Lessons

**Windows (Debug):**
```batch
build\bin\Debug\Lesson01_HelloWorld.exe
build\bin\Debug\Lesson15_DefaultArgs.exe
build\bin\Debug\Lesson45_AbstractClasses.exe
```

**Windows (Release):**
```batch
build\bin\Release\Lesson01_HelloWorld.exe
```

**Linux/Mac:**
```bash
./build/bin/Lesson01_HelloWorld
./build/bin/Lesson15_DefaultArgs
./build/bin/Lesson45_AbstractClasses
```

## Project Structure

```
Part1-Fundamentals/
├── Lesson01_HelloWorld.cpp          # Source files (83 total)
├── Lesson01_HelloWorld.vcxproj      # Visual Studio projects (83 total)
├── Lesson02_Variables.cpp
├── Lesson02_Variables.vcxproj
│   ... (80 more lessons)
├── Lesson80_BestPractices.cpp
├── Lesson80_BestPractices.vcxproj
│
├── CMakeLists.txt                   # Master build configuration
├── build_all.bat                    # Windows build script
├── build_all.sh                     # Linux/Mac build script
├── run_all.bat                      # Windows run script
├── run_all.sh                       # Linux/Mac run script
├── BUILD_INSTRUCTIONS.md            # This file
├── README.md                        # Course overview
│
└── build/                           # Generated build directory
    ├── bin/                         # Compiled executables
    │   ├── Debug/                   # Debug builds (Windows)
    │   └── Release/                 # Release builds (Windows)
    └── obj/                         # Intermediate build files
```

## Individual Project Files

Each `.vcxproj` file is configured with:

- **Platform:** x64 (64-bit)
- **Configurations:** Debug and Release
- **C++ Standard:** C++17
- **Toolset:** v142 (Visual Studio 2019) or later
- **Character Set:** Unicode
- **Warning Level:** Level 4 (/W4)
- **Optimizations:**
  - Debug: Disabled, full debug info
  - Release: Maximum speed (/O2), whole program optimization
- **Multi-processor Compilation:** Enabled
- **Include Directories:** Current project directory

You can open any `.vcxproj` file directly in Visual Studio 2019 or later.

## Complete Lesson List

### Basics (Lessons 1-10)
- 01: Hello World, First Program
- 02: Variables, Data Types, Input/Output
- 03: Operators, Calculator
- 04: If/Else, Grading System
- 05: Switch Statement
- 06: While Loops
- 07: For Loops
- 08: Break and Continue
- 09: Nested Loops
- 10: Pattern Printing

### Functions (Lessons 11-15)
- 11: Functions Basics
- 12: Function Parameters
- 13: Function Overloading
- 14: Recursion
- 15: Default Arguments

### Arrays and Strings (Lessons 16-20)
- 16: Arrays
- 17: 2D Arrays
- 18: Array Algorithms
- 19: C-Style Strings
- 20: std::string

### Pointers and Memory (Lessons 21-30)
- 21: Pointers Basics
- 22: Pointer Arithmetic
- 23: Pointers and Arrays
- 24: Pointers and Functions
- 25: Dynamic Memory
- 26: References
- 27: Const Pointers
- 28: Pointer vs Reference
- 29: Null Pointers
- 30: Smart Pointers

### Object-Oriented Programming (Lessons 31-45)
- 31: Classes
- 32: Constructors
- 33: Member Functions
- 34: Access Specifiers
- 35: Encapsulation
- 36: Static Members
- 37: Friend Functions
- 38-39: Operator Overloading
- 40: Copy Constructor
- 41: Inheritance
- 42: Inheritance Types
- 43: Multiple Inheritance
- 44: Polymorphism
- 45: Abstract Classes

### Templates (Lessons 46-49)
- 46: Function Templates
- 47: Class Templates
- 48: Template Specialization
- 49: Variadic Templates

### STL - Standard Template Library (Lessons 50-60)
- 50: STL Overview
- 51: Vector
- 52: List
- 53: Stack
- 54: Set
- 55: Map
- 56: Unordered Map
- 57-58: Algorithms
- 59: Iterators
- 60: Lambdas

### Exception Handling (Lessons 61-63)
- 61: Exceptions
- 62: Custom Exceptions
- 63: Exception Safety

### File I/O (Lessons 64-67)
- 64: File I/O
- 65: Binary File I/O
- 66: File Streams
- 67: String Streams

### Modern C++ (Lessons 68-70)
- 68: Modern C++ Features
- 69: Move Semantics
- 70: Smart Pointers Advanced

### Multithreading (Lessons 71-75)
- 71: Threads
- 72: Thread Synchronization
- 73: Mutex
- 74: Condition Variables
- 75: Async and Future

### Advanced Topics (Lessons 76-80)
- 76: Namespaces
- 77: Preprocessor
- 78: Compilation Process
- 79: Debugging Techniques
- 80: Best Practices

## Troubleshooting

### Windows Issues

**Problem:** "Visual Studio version not found"
```batch
# List available generators
cmake --help

# Use specific version
cmake .. -G "Visual Studio 17 2022" -A x64
```

**Problem:** "LINK : fatal error LNK1104: cannot open file"
- Close Visual Studio
- Delete the `build` directory
- Run `build_all.bat` again

**Problem:** "MSBuild not found"
- Open "Developer Command Prompt for VS 2019"
- Navigate to this directory
- Run build commands

### Linux/Mac Issues

**Problem:** "g++: command not found"
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install
```

**Problem:** "cmake: command not found"
```bash
# Ubuntu/Debian
sudo apt-get install cmake

# macOS
brew install cmake
```

**Problem:** Threading examples fail to link
```bash
# Add -pthread flag
g++ -std=c++17 -pthread -o Lesson71_Threads Lesson71_Threads.cpp
```

**Problem:** Permission denied on scripts
```bash
chmod +x build_all.sh run_all.sh
```

### CMake Issues

**Problem:** "CMake Error: Could not find CMAKE_ROOT"
```bash
# Reinstall CMake or add to PATH
export PATH=$PATH:/path/to/cmake/bin
```

**Problem:** "No CMAKE_CXX_COMPILER could be found"
- Install a C++ compiler (g++, clang, or MSVC)
- Set compiler explicitly:
  ```bash
  cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++
  ```

### General Issues

**Problem:** Files created but won't execute
- Windows: Check antivirus settings
- Linux/Mac: Make files executable (`chmod +x`)

**Problem:** "Out of memory" during compilation
```bash
# Reduce parallel jobs
cmake --build . -j2  # Use only 2 cores instead of all
```

**Problem:** Different behavior between Debug and Release
- This is normal due to optimizations
- Use Debug for development and debugging
- Use Release for performance testing

## Additional Resources

### Documentation
- C++ Reference: https://en.cppreference.com/
- CMake Documentation: https://cmake.org/documentation/
- Visual Studio Documentation: https://docs.microsoft.com/visualstudio/

### Learning Resources
- See main course README at: `CPP-Tutorial-400Hours/README.md`
- Individual lesson descriptions in each `.cpp` file
- Completion summary: `COMPLETION_SUMMARY.txt`

### Getting Help
- Check the individual lesson source files for comments
- Review compiler error messages carefully
- Search for specific error messages online
- Refer to C++ standard documentation

## License

These examples are part of the 400-Hour C++ Tutorial Course.
Free to use for educational purposes.

---

**Happy Coding!**

For questions or issues, refer to the main course documentation or individual lesson files.
