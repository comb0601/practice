#!/bin/bash
# ============================================================================
# Build All Script for Part 1 Fundamentals - Linux/Mac
# ============================================================================
# This script builds all 83 C++ lesson examples using CMake and GCC/Clang
# ============================================================================

echo "============================================================================"
echo "Building C++ Tutorial Part 1 - Fundamentals"
echo "Total Lessons: 83 executables"
echo "============================================================================"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake is not installed"
    echo "Please install CMake:"
    echo "  Ubuntu/Debian: sudo apt-get install cmake"
    echo "  macOS:         brew install cmake"
    exit 1
fi

# Check if C++ compiler is installed
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "ERROR: No C++ compiler found"
    echo "Please install a C++ compiler:"
    echo "  Ubuntu/Debian: sudo apt-get install build-essential"
    echo "  macOS:         xcode-select --install"
    exit 1
fi

# Create build directory
mkdir -p build
cd build

echo "[1/3] Configuring CMake..."
echo ""

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake configuration failed"
    exit 1
fi

echo ""
echo "[2/3] Building all lessons..."
echo ""

# Build all targets
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed"
    exit 1
fi

echo ""
echo "[3/3] Build complete!"
echo ""

cd ..

echo "============================================================================"
echo "BUILD SUCCESSFUL!"
echo "============================================================================"
echo "Executables location: build/bin/"
echo ""
echo "To run all examples:      ./run_all.sh"
echo "To run a specific lesson: ./build/bin/Lesson01_HelloWorld"
echo "============================================================================"
echo ""
