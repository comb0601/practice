# C++ Tutorial 400 Hours - Build Instructions

Complete build guide for all 370+ code examples across all platforms.

---

## Table of Contents

- [Quick Start](#quick-start)
- [System Requirements](#system-requirements)
- [Windows Build Instructions](#windows-build-instructions)
- [Linux Build Instructions](#linux-build-instructions)
- [macOS Build Instructions](#macos-build-instructions)
- [Building Specific Parts](#building-specific-parts)
- [Running Examples](#running-examples)
- [Troubleshooting](#troubleshooting)
- [Advanced Build Options](#advanced-build-options)

---

## Quick Start

### Windows
```batch
# 1. Check environment
setup_environment.bat

# 2. Build everything
build_all_windows.bat

# 3. Run examples interactively
python run_examples.py
```

### Linux/macOS
```bash
# 1. Check environment
./setup_environment.sh

# 2. Build everything
./build_all_linux.sh

# 3. Run examples interactively
python3 run_examples.py
```

---

## System Requirements

### All Platforms
- **CMake** 3.15 or later
- **C++ Compiler** with C++17 support
- **5+ GB** free disk space for build artifacts
- **Python 3.6+** (optional, for interactive runner)

### Windows
- **Visual Studio 2019 or later** with:
  - Desktop development with C++
  - C++ MFC for latest build tools (optional, for Part 2)
- **Windows SDK** (usually installed with Visual Studio)

### Linux
- **GCC 7+** or **Clang 6+**
- Development packages:
  ```bash
  # Ubuntu/Debian
  sudo apt install build-essential cmake git python3
  sudo apt install libgl1-mesa-dev libglu1-mesa-dev
  sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

  # Fedora/RHEL
  sudo dnf groupinstall 'Development Tools'
  sudo dnf install cmake gcc-c++ git python3
  sudo dnf install mesa-libGL-devel mesa-libGLU-devel
  sudo dnf install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel

  # Arch Linux
  sudo pacman -S base-devel cmake git python
  sudo pacman -S mesa glu
  sudo pacman -S libx11 libxrandr libxinerama libxcursor libxi
  ```

### macOS
- **Xcode** Command Line Tools
- **Homebrew** (recommended)
  ```bash
  # Install Xcode Command Line Tools
  xcode-select --install

  # Install CMake
  brew install cmake
  ```

---

## Windows Build Instructions

### Step 1: Environment Setup

Run the environment checker:
```batch
setup_environment.bat
```

This script will:
- Check for required tools (CMake, Visual Studio, Python)
- Verify Visual Studio components
- Offer to install missing dependencies via winget

### Step 2: Build All Parts

**Option A: Automated Build (Recommended)**
```batch
build_all_windows.bat
```

**Option B: Build with Options**
```batch
# Debug build
build_all_windows.bat Debug

# Release build with MFC
build_all_windows.bat Release MFC
```

**Option C: Manual CMake Build**
```batch
# Configure
cmake -S . -B build-windows -G "Visual Studio 17 2022"

# Build
cmake --build build-windows --config Release -j
```

### Step 3: Verify Build

Check that all examples compile:
```batch
verify_all.bat
```

### Output Location

Executables will be in:
```
output-windows-Release/bin/
├── Part1-Fundamentals/
│   ├── Lesson01_HelloWorld.exe
│   ├── Lesson02_Variables.exe
│   └── ...
├── Part3-3D-Rendering/
└── Part4-Optimization-Advanced/
```

---

## Linux Build Instructions

### Step 1: Environment Setup

Run the environment checker:
```bash
./setup_environment.sh
```

This script will:
- Check for required tools (CMake, g++/clang++, Python)
- Detect your Linux distribution
- Offer to install missing dependencies

### Step 2: Build All Parts

**Option A: Automated Build (Recommended)**
```bash
./build_all_linux.sh
```

**Option B: Build with Options**
```bash
# Debug build
./build_all_linux.sh debug

# Release build with 8 parallel jobs
./build_all_linux.sh release -j 8

# Verbose build output
./build_all_linux.sh -v
```

**Option C: Manual CMake Build**
```bash
# Configure
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build-linux -j $(nproc)
```

### Step 3: Verify Build

Check that all examples compile:
```bash
./verify_all.sh
```

### Output Location

Executables will be in:
```
output-linux-Release/bin/
├── Part1-Fundamentals/
│   ├── Lesson01_HelloWorld
│   ├── Lesson02_Variables
│   └── ...
├── Part3-3D-Rendering/
└── Part4-Optimization-Advanced/
```

---

## macOS Build Instructions

macOS follows similar steps to Linux:

### Step 1: Install Dependencies
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake via Homebrew
brew install cmake
```

### Step 2: Build
```bash
# Use the Linux build script
./build_all_linux.sh
```

### Step 3: Run Examples
```bash
python3 run_examples.py
```

**Note:** Part 2 (MFC UI) and DirectX examples in Part 3 are Windows-only and will be skipped on macOS.

---

## Building Specific Parts

### Build Only Part 1 (Fundamentals)
```bash
cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART1_FUNDAMENTALS=ON
cmake --build build -j
```

### Build Only Part 3 (3D Rendering)
```bash
cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART3_3D_RENDERING=ON
cmake --build build -j
```

### Build Only Part 4 (Optimization)
```bash
cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART4_OPTIMIZATION=ON
cmake --build build -j
```

### Build Part 2 (MFC - Windows Only)
```batch
cmake -S . -B build -DBUILD_PART2_MFC=ON
cmake --build build --config Release
```

---

## Running Examples

### Method 1: Interactive Runner (Recommended)
```bash
# Python 3.6+ required
python3 run_examples.py
```

The interactive runner provides:
- Browse all examples by part
- Run examples directly from menu
- Build and verify from the interface
- Cross-platform support

### Method 2: Direct Execution

**Windows:**
```batch
cd output-windows-Release\bin\Part1-Fundamentals
Lesson01_HelloWorld.exe
```

**Linux/macOS:**
```bash
cd output-linux-Release/bin/Part1-Fundamentals
./Lesson01_HelloWorld
```

### Method 3: Using CMake
```bash
# After building
cd build-linux
ctest --output-on-failure
```

---

## Troubleshooting

### Common Issues

#### 1. CMake Not Found
**Windows:**
```batch
winget install Kitware.CMake
```

**Linux:**
```bash
sudo apt install cmake  # Ubuntu/Debian
sudo dnf install cmake  # Fedora
```

#### 2. Visual Studio Not Found (Windows)
- Download from: https://visualstudio.microsoft.com/downloads/
- Install "Desktop development with C++"
- Run `setup_environment.bat` to verify

#### 3. Compiler Not Found (Linux)
```bash
sudo apt install build-essential  # Ubuntu/Debian
sudo dnf groupinstall 'Development Tools'  # Fedora
```

#### 4. OpenGL Errors (Part 3)
**Linux:**
```bash
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
```

**Windows:**
- OpenGL should be included with graphics drivers
- Update your graphics drivers

#### 5. Missing X11 Libraries (Linux)
```bash
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

#### 6. Threading Errors
Ensure your compiler supports C++17 threading:
- GCC 7+
- Clang 6+
- MSVC 2019+

#### 7. Build Takes Too Long
```bash
# Use more parallel jobs
cmake --build build -j 16

# Or build specific parts only
cmake -S . -B build -DBUILD_PART1_FUNDAMENTALS=ON -DBUILD_ALL_PARTS=OFF
```

### Getting Help

1. Run environment checker: `setup_environment.bat` or `./setup_environment.sh`
2. Check build logs in `build-*.log` files
3. Run verification: `verify_all.bat` or `./verify_all.sh`
4. Review CMake output for specific error messages

---

## Advanced Build Options

### Custom Build Type
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Specify Compiler
```bash
# Use Clang instead of GCC
cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++

# Use specific GCC version
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-11
```

### Enable Verbose Output
```bash
cmake --build build --verbose
```

### Generate Compile Commands (for IDEs)
```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### Install Targets
```bash
cmake --build build --target install
```

### Clean Build
```bash
# Remove build directory
rm -rf build-*

# Full clean rebuild
./build_all_linux.sh
```

---

## Course Structure

### Part 1: C++ Fundamentals (85 lessons)
- Basic syntax and control flow
- Functions and arrays
- Pointers and memory management
- Object-oriented programming
- Templates and STL
- Modern C++ features
- Threading basics

**Platform:** All (Windows, Linux, macOS)

### Part 2: MFC UI Development (90 lessons)
- Windows GUI programming
- Dialog boxes and controls
- Graphics and drawing
- Database connectivity
- Network programming

**Platform:** Windows only

### Part 3: 3D Rendering (100 lessons)
- 3D mathematics
- OpenGL programming
- DirectX programming (Windows only)
- Advanced rendering techniques
- Modern graphics APIs

**Platform:** Windows (full), Linux/macOS (OpenGL only)

### Part 4: Optimization & Advanced (100 lessons)
- Performance profiling
- Code optimization techniques
- Multithreading and concurrency
- Design patterns
- Real-world projects

**Platform:** All (Windows, Linux, macOS)

---

## Build System Details

### CMake Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `BUILD_PART1_FUNDAMENTALS` | `ON` | Build Part 1 examples |
| `BUILD_PART2_MFC` | `OFF` | Build Part 2 (Windows only) |
| `BUILD_PART3_3D_RENDERING` | `ON` | Build Part 3 examples |
| `BUILD_PART4_OPTIMIZATION` | `ON` | Build Part 4 examples |
| `BUILD_ALL_PARTS` | `ON` | Build all parts (overrides individual) |
| `CMAKE_BUILD_TYPE` | `Release` | Build type (Debug/Release/RelWithDebInfo) |

### Generated Files

- `build-*/` - Build artifacts
- `output-*/` - Organized executables
- `*.log` - Build and verification logs
- `compile_commands.json` - IDE support (if enabled)

---

## Performance Tips

1. **Use Release builds** for better performance
2. **Parallel builds** with `-j` flag
3. **Build specific parts** if you don't need all 370+ examples
4. **SSD recommended** for faster compile times
5. **16GB+ RAM** recommended for full parallel build

---

## Integration with IDEs

### Visual Studio (Windows)
```batch
# Generate solution file
cmake -S . -B build -G "Visual Studio 17 2022"

# Open solution
start build\CPP_Tutorial_400_Hours.sln
```

### Visual Studio Code
```bash
# Generate compile commands
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Link to project root
ln -s build/compile_commands.json .
```

### CLion
- Open the root `CMakeLists.txt` directly
- CLion will automatically configure the project

---

## License and Support

This is an educational project. See the main README.md for license information.

For issues:
1. Check this guide's troubleshooting section
2. Run verification scripts
3. Review build logs

---

**Happy Coding!** 🚀
