#!/bin/bash
################################################################################
#  C++ Tutorial 400 Hours - Complete Linux Build Script
################################################################################
#  This script builds ALL parts of the C++ tutorial course
#  Parts: 1=Fundamentals, 3=3D Rendering, 4=Optimization
#  Note: Part 2 (MFC) is Windows-only and will be skipped
################################################################################

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print header
echo ""
echo "============================================================================"
echo "  C++ Tutorial 400 Hours - Complete Build System (Linux)"
echo "============================================================================"
echo ""
echo "  Building 280+ lessons across 3 major parts"
echo ""
echo "  Part 1: C++ Fundamentals (85 lessons)"
echo "  Part 2: MFC UI Development - SKIPPED (Windows only)"
echo "  Part 3: 3D Rendering (100 lessons)"
echo "  Part 4: Optimization & Advanced (100 lessons)"
echo ""
echo "============================================================================"
echo ""

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}[ERROR]${NC} CMake not found! Please install CMake or run setup_environment.sh"
    exit 1
fi

# Check for C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo -e "${RED}[ERROR]${NC} C++ compiler not found! Please install g++ or clang++"
    echo "  Ubuntu/Debian: sudo apt install build-essential"
    echo "  Fedora/RHEL:   sudo dnf install gcc-c++"
    echo "  Arch:          sudo pacman -S base-devel"
    exit 1
fi

# Parse command line arguments
BUILD_TYPE="Release"
NUM_JOBS=$(nproc)
VERBOSE=0

show_help() {
    echo ""
    echo "Usage: ./build_all_linux.sh [options]"
    echo ""
    echo "Options:"
    echo "  debug       Build in Debug mode"
    echo "  release     Build in Release mode (default)"
    echo "  -j N        Use N parallel jobs (default: $NUM_JOBS)"
    echo "  -v          Verbose build output"
    echo "  -h, --help  Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./build_all_linux.sh"
    echo "  ./build_all_linux.sh debug -j 8"
    echo "  ./build_all_linux.sh release -v"
    echo ""
    exit 0
}

while [[ $# -gt 0 ]]; do
    case $1 in
        debug|Debug|DEBUG)
            BUILD_TYPE="Debug"
            shift
            ;;
        release|Release|RELEASE)
            BUILD_TYPE="Release"
            shift
            ;;
        -j)
            NUM_JOBS="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -h|--help)
            show_help
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_help
            ;;
    esac
done

echo ""
echo -e "${BLUE}[CONFIGURATION]${NC}"
echo "  Build Type: $BUILD_TYPE"
echo "  Parallel Jobs: $NUM_JOBS"
echo "  Compiler: $(which c++ || which g++ || which clang++)"
echo "  CMake Version: $(cmake --version | head -n1)"
echo ""

# Create build directory
BUILD_DIR="build-linux"
echo -e "${GREEN}[STEP 1/4]${NC} Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure CMake
echo ""
echo -e "${GREEN}[STEP 2/4]${NC} Configuring CMake..."
echo "============================================================================"

CMAKE_ARGS=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE
    -DBUILD_PART1_FUNDAMENTALS=ON
    -DBUILD_PART2_MFC=OFF
    -DBUILD_PART3_3D_RENDERING=ON
    -DBUILD_PART4_OPTIMIZATION=ON
    -DBUILD_ALL_PARTS=ON
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

cmake "${CMAKE_ARGS[@]}"

if [ $? -ne 0 ]; then
    echo ""
    echo -e "${RED}[ERROR]${NC} CMake configuration failed!"
    exit 1
fi

# Build
echo ""
echo -e "${GREEN}[STEP 3/4]${NC} Building all projects..."
echo "============================================================================"
echo "  This may take 10-30 minutes depending on your system"
echo "  Using $NUM_JOBS parallel jobs"
echo "============================================================================"

BUILD_ARGS=(
    --build "$BUILD_DIR"
    --config $BUILD_TYPE
    -j $NUM_JOBS
)

if [ $VERBOSE -eq 1 ]; then
    BUILD_ARGS+=(--verbose)
fi

cmake "${BUILD_ARGS[@]}"

if [ $? -ne 0 ]; then
    echo ""
    echo -e "${RED}[ERROR]${NC} Build failed!"
    exit 1
fi

# Organize output
echo ""
echo -e "${GREEN}[STEP 4/4]${NC} Organizing build output..."
echo "============================================================================"

OUTPUT_DIR="output-linux-$BUILD_TYPE"
mkdir -p "$OUTPUT_DIR"

echo "  Copying executables to $OUTPUT_DIR..."
if [ -d "$BUILD_DIR/bin" ]; then
    cp -r "$BUILD_DIR/bin" "$OUTPUT_DIR/"
fi
if [ -d "$BUILD_DIR/lib" ]; then
    cp -r "$BUILD_DIR/lib" "$OUTPUT_DIR/"
fi

# Generate summary
echo ""
echo "============================================================================"
echo -e "${GREEN}  BUILD COMPLETE!${NC}"
echo "============================================================================"
echo ""
echo "  Build Type: $BUILD_TYPE"
echo "  Build Directory: $BUILD_DIR"
echo "  Output Directory: $OUTPUT_DIR"
echo ""

# Count executables
TOTAL_EXES=0
if [ -d "$OUTPUT_DIR/bin" ]; then
    TOTAL_EXES=$(find "$OUTPUT_DIR/bin" -type f -executable | wc -l)
fi

echo "  Total Executables Built: $TOTAL_EXES"
echo ""
echo "  To run examples:"
echo "    1. Navigate to $OUTPUT_DIR/bin"
echo "    2. Run any lesson executable, e.g.:"
echo "       cd $OUTPUT_DIR/bin/Part1-Fundamentals"
echo "       ./Lesson01_HelloWorld"
echo "    3. Or use the interactive runner:"
echo "       python3 run_examples.py"
echo ""
echo "  To run all unit tests (if available):"
echo "    cd $BUILD_DIR"
echo "    ctest --output-on-failure"
echo ""
echo "============================================================================"

exit 0
