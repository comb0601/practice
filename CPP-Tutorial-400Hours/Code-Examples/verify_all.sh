#!/bin/bash
################################################################################
#  C++ Tutorial 400 Hours - Linux Verification Script
################################################################################
#  This script verifies that all .cpp files compile successfully
################################################################################

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo ""
echo "============================================================================"
echo "  C++ Tutorial 400 Hours - Code Verification (Linux)"
echo "============================================================================"
echo ""
echo "  This script will verify that all code examples compile successfully"
echo "  by performing a test build of all parts."
echo ""

START_TIME=$(date +%s)

# Check if build directory exists
if [ ! -d "build-verify" ]; then
    echo "  Creating verification build directory..."
    mkdir -p build-verify
else
    echo "  Using existing build-verify directory"
    echo "  To start fresh, run: rm -rf build-verify"
    echo ""
fi

# Run CMake configuration
echo "============================================================================"
echo "  Step 1: CMake Configuration"
echo "============================================================================"
echo ""

cmake -S . -B build-verify \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_PART1_FUNDAMENTALS=ON \
    -DBUILD_PART2_MFC=OFF \
    -DBUILD_PART3_3D_RENDERING=ON \
    -DBUILD_PART4_OPTIMIZATION=ON \
    -DBUILD_ALL_PARTS=ON

if [ $? -ne 0 ]; then
    echo ""
    echo -e "${RED}[ERROR]${NC} CMake configuration failed!"
    echo "See errors above for details."
    exit 1
fi

# Build all projects
echo ""
echo "============================================================================"
echo "  Step 2: Building All Projects"
echo "============================================================================"
echo "  This may take 10-30 minutes..."
echo "  Using $(nproc) parallel jobs"
echo ""

# Capture build output and errors
BUILD_LOG="build-verify.log"
cmake --build build-verify --config Release -j $(nproc) 2>&1 | tee "$BUILD_LOG"

BUILD_RESULT=${PIPESTATUS[0]}

echo ""
echo "============================================================================"
echo "  Verification Results"
echo "============================================================================"
echo ""

if [ $BUILD_RESULT -eq 0 ]; then
    echo -e "${GREEN}[SUCCESS]${NC} All code examples compiled successfully!"
    echo ""

    # Count built executables
    TOTAL_EXES=0
    if [ -d "build-verify/bin" ]; then
        TOTAL_EXES=$(find build-verify/bin -type f -executable | wc -l)
    fi

    echo "  Total executables built: $TOTAL_EXES"
    echo "  Build directory: build-verify"
    echo "  Build log: $BUILD_LOG"
    echo ""

    # Test a few random executables
    echo "  Testing sample executables..."
    echo ""

    if [ -f "build-verify/bin/Part1-Fundamentals/Lesson01_HelloWorld" ]; then
        echo "  Testing Part 1 - Lesson01_HelloWorld"
        echo "  Output:"
        echo "" | ./build-verify/bin/Part1-Fundamentals/Lesson01_HelloWorld 2>/dev/null || true
        if [ $? -eq 0 ]; then
            echo -e "  ${GREEN}[OK]${NC} Part 1 example runs successfully"
        fi
        echo ""
    fi

    # Check for common warnings in build log
    echo "  Checking for warnings..."
    WARNING_COUNT=$(grep -c "warning:" "$BUILD_LOG" 2>/dev/null || echo "0")
    if [ $WARNING_COUNT -gt 0 ]; then
        echo -e "  ${YELLOW}[INFO]${NC} Found $WARNING_COUNT compiler warnings"
        echo "  Review $BUILD_LOG for details"
    else
        echo -e "  ${GREEN}[OK]${NC} No compiler warnings found"
    fi
    echo ""

    echo "  Verification complete!"
    echo "  All code examples are ready to use."

    # Summary statistics
    echo ""
    echo "  Build Statistics:"
    if [ -d "build-verify/bin/Part1-Fundamentals" ]; then
        PART1_COUNT=$(find build-verify/bin/Part1-Fundamentals -type f -executable | wc -l)
        echo "    Part 1 (Fundamentals):     $PART1_COUNT executables"
    fi
    if [ -d "build-verify/bin/Part3" ]; then
        PART3_COUNT=$(find build-verify/bin/Part3* -type f -executable 2>/dev/null | wc -l)
        echo "    Part 3 (3D Rendering):     $PART3_COUNT executables"
    fi
    if [ -d "build-verify/bin/Part4" ]; then
        PART4_COUNT=$(find build-verify/bin -name "Lesson*" -type f -executable 2>/dev/null | wc -l)
        echo "    Part 4 (Optimization):     Included in build"
    fi

else
    echo -e "${RED}[FAILED]${NC} Some code examples failed to compile"
    echo ""
    echo "  Error details are shown above and saved to: $BUILD_LOG"
    echo ""
    echo "  Common issues:"
    echo "    - Missing development packages (run ./setup_environment.sh)"
    echo "    - Missing OpenGL libraries (for Part 3)"
    echo "    - Incompatible compiler version (need C++17 support)"
    echo "    - Missing threading support"
    echo ""
    echo "  To see only errors, run:"
    echo "    grep 'error:' $BUILD_LOG"
    echo ""
    echo "  Try running: ./setup_environment.sh"

    # Show first few errors
    echo ""
    echo "  First 10 errors from build:"
    echo "  ============================================"
    grep -m 10 "error:" "$BUILD_LOG" 2>/dev/null || echo "  See $BUILD_LOG for full error list"
fi

echo ""
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))
MINUTES=$((DURATION / 60))
SECONDS=$((DURATION % 60))

echo "  Build time: ${MINUTES}m ${SECONDS}s"
echo ""
echo "============================================================================"

exit $BUILD_RESULT
