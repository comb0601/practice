# Lesson 01: Introduction and Development Environment Setup
**Duration: 2 hours**

## Learning Objectives
By the end of this lesson, you will:
- Understand what C++ is and its applications
- Know the history and evolution of C++
- Set up a complete C++ development environment
- Understand the compilation process
- Write and compile your first program

## 1. What is C++?

C++ is a general-purpose programming language created by Bjarne Stroustrup as an extension of the C programming language. It's known for:

- **Performance**: C++ compiles to machine code, offering near-metal performance
- **Control**: Fine-grained control over memory and hardware
- **Versatility**: Used in game engines, operating systems, browsers, databases, embedded systems
- **Object-Oriented**: Supports classes, inheritance, polymorphism
- **Multi-paradigm**: Procedural, OOP, functional, and generic programming

## 2. History of C++

- **1979**: Bjarne Stroustrup begins work on "C with Classes"
- **1983**: Language renamed to C++
- **1998**: C++98 - First ISO standard
- **2003**: C++03 - Bug fix release
- **2011**: C++11 - Major update (auto, lambdas, smart pointers)
- **2014**: C++14 - Minor improvements
- **2017**: C++17 - Significant features (structured bindings, std::optional)
- **2020**: C++20 - Huge update (concepts, ranges, coroutines, modules)
- **2023**: C++23 - Latest standard

## 3. Why Learn C++?

### Industry Applications
1. **Game Development**: Unreal Engine, Unity (parts), custom engines
2. **Graphics**: OpenGL, DirectX, Vulkan applications
3. **Operating Systems**: Windows, Linux kernel modules, macOS components
4. **Browsers**: Chrome (Chromium), Firefox
5. **Databases**: MySQL, MongoDB, PostgreSQL
6. **Financial Systems**: High-frequency trading platforms
7. **Embedded Systems**: IoT devices, automotive software
8. **Artificial Intelligence**: TensorFlow, PyTorch backends

### For This Course
We're learning C++ specifically for:
- **ImGui**: Immediate mode GUI library for tools and debugging
- **3D Rendering**: OpenGL/Vulkan graphics programming
- **Performance**: Understanding optimization for graphics applications

## 4. Development Environment Setup

### Option 1: Windows with Visual Studio

#### Step 1: Download Visual Studio
1. Go to https://visualstudio.microsoft.com/
2. Download "Visual Studio Community" (free)
3. Run the installer

#### Step 2: Select Workloads
- ✓ Desktop development with C++
- ✓ Game development with C++ (optional, includes useful libraries)

#### Step 3: Individual Components (verify these are selected)
- MSVC C++ compiler
- Windows 10/11 SDK
- C++ CMake tools
- C++ profiling tools

#### Step 4: Install
- Click "Install" and wait (requires ~7 GB)

### Option 2: Windows with MinGW-w64 and VS Code

#### Step 1: Install MinGW-w64
1. Download from https://www.mingw-w64.org/
2. Or use MSYS2 (recommended):
   - Download from https://www.msys2.org/
   - Install to C:\msys64
   - Open MSYS2 terminal and run:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-gdb
   pacman -S mingw-w64-x86_64-cmake
   ```
3. Add to PATH: `C:\msys64\mingw64\bin`

#### Step 2: Install VS Code
1. Download from https://code.visualstudio.com/
2. Install extensions:
   - C/C++ (Microsoft)
   - C/C++ Extension Pack
   - CMake Tools
   - CMake Language Support

### Option 3: Linux (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install build essentials
sudo apt install build-essential

# Install GCC/G++
sudo apt install gcc g++

# Install CMake
sudo apt install cmake

# Install GDB debugger
sudo apt install gdb

# Install Git
sudo apt install git

# Install VS Code (optional)
sudo snap install code --classic

# Verify installations
gcc --version
g++ --version
cmake --version
```

### Option 4: macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake

# Install VS Code (optional)
brew install --cask visual-studio-code

# Verify installations
clang++ --version
cmake --version
```

## 5. Understanding the Compilation Process

C++ is a **compiled language**. Here's what happens:

```
Source Code (.cpp) → Preprocessor → Compiler → Assembler → Linker → Executable
```

### Step-by-Step Breakdown

#### 1. Preprocessing
- Handles directives starting with `#` (like `#include`, `#define`)
- Includes header files
- Expands macros
- Removes comments

```cpp
#include <iostream>  // Preprocessor copies iostream contents here
#define PI 3.14159   // Preprocessor replaces PI with 3.14159
```

#### 2. Compilation
- Translates C++ code to assembly language
- Performs syntax checking
- Optimizes code
- Creates object files (.o or .obj)

#### 3. Assembly
- Converts assembly to machine code
- Creates object files

#### 4. Linking
- Combines multiple object files
- Links external libraries
- Resolves function references
- Creates final executable

### Example Commands

```bash
# Single-step compilation (does everything)
g++ main.cpp -o program

# Multi-step (educational purposes)
g++ -E main.cpp -o main.i          # Preprocessing only
g++ -S main.i -o main.s            # Compilation to assembly
g++ -c main.s -o main.o            # Assembly to object code
g++ main.o -o program              # Linking
```

## 6. Verifying Your Setup

Create a test file to verify your installation.

### test_setup.cpp
```cpp
#include <iostream>

int main() {
    std::cout << "C++ setup successful!" << std::endl;
    std::cout << "C++ version: " << __cplusplus << std::endl;
    return 0;
}
```

### Compile and Run

#### Windows (Visual Studio)
1. Create new project: "Empty Project"
2. Add new file: test_setup.cpp
3. Press Ctrl+F5 to build and run

#### Windows (MinGW) / Linux / macOS
```bash
# Compile
g++ test_setup.cpp -o test_setup

# Run (Linux/Mac)
./test_setup

# Run (Windows)
test_setup.exe
```

### Expected Output
```
C++ setup successful!
C++ version: 202002
```

The version number indicates:
- 199711: C++98
- 201103: C++11
- 201402: C++14
- 201703: C++17
- 202002: C++20
- 202302: C++23

## 7. Project Structure Best Practices

For this course, we'll use this structure:

```
LessonXX-ProjectName/
├── src/
│   ├── main.cpp
│   ├── other_source.cpp
├── include/
│   ├── header.h
├── build/          (generated, not in version control)
├── CMakeLists.txt
└── README.md
```

## 8. Introduction to CMake

CMake is a build system generator that makes compilation consistent across platforms.

### Basic CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyFirstProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(my_program src/main.cpp)
```

### Using CMake

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build
cmake --build .

# Run
./my_program  # Linux/Mac
my_program.exe  # Windows
```

## 9. Setting Up Your Course Directory

Create your workspace:

```bash
mkdir cpp-learning
cd cpp-learning
mkdir lesson-01
cd lesson-01
```

## 10. Exercises

### Exercise 1: Environment Check
1. Install a C++ compiler on your system
2. Verify installation with `g++ --version` or `clang++ --version`
3. Install VS Code or your preferred IDE

### Exercise 2: First Compilation
1. Create `hello.cpp` with the test program above
2. Compile it using command line
3. Run the executable
4. Take note of the C++ version reported

### Exercise 3: CMake Practice
1. Create a directory with proper structure
2. Write a CMakeLists.txt file
3. Build using CMake
4. Verify the executable runs

### Exercise 4: IDE Setup
1. Open your IDE
2. Create a new C++ project
3. Build and run the test program
4. Familiarize yourself with the interface

## 11. Common Issues and Solutions

### Issue: "g++ is not recognized" (Windows)
**Solution**: Add compiler to PATH environment variable

### Issue: "Permission denied" (Linux/Mac)
**Solution**: Make executable with `chmod +x program_name`

### Issue: CMake not found
**Solution**: Install CMake via package manager or from cmake.org

### Issue: "iostream: No such file"
**Solution**: Install C++ standard library (usually comes with compiler)

## 12. Additional Resources

- **C++ Reference**: https://cppreference.com
- **Compiler Explorer**: https://godbolt.org (see compiled assembly)
- **C++ Insights**: https://cppinsights.io (understand what compiler does)
- **Quick Reference**: https://hackingcpp.com/cpp/cheat_sheets.html

## 13. What's Next?

In Lesson 02, we'll:
- Write our first complete C++ program
- Understand the structure of a C++ program
- Learn about the main() function
- Explore basic input/output
- Understand compilation errors and how to fix them

## Summary Checklist

- [ ] Understand what C++ is and its applications
- [ ] Know C++ history and evolution
- [ ] Have a working C++ compiler installed
- [ ] Can compile and run a simple program
- [ ] Understand the compilation process
- [ ] Have an IDE or text editor set up
- [ ] Understand basic project structure
- [ ] Know how to use CMake basics
- [ ] Successfully compiled and ran test_setup.cpp
- [ ] Ready to write more complex programs

---

**Time to complete**: 2 hours
**Next lesson**: [Lesson 02 - Your First C++ Program](../Lesson-02-First-Program/README.md)

**Congratulations!** You've set up your development environment and are ready to start your C++ journey!
