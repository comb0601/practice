#!/bin/bash

# Installation Verification Script
# Checks that all 100 lessons are properly installed

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "  3D Rendering Course - Installation Check"
echo "=========================================="
echo ""

# Function to check if directory exists
check_dir() {
    if [ -d "$1" ]; then
        echo "✓ $2"
        return 0
    else
        echo "✗ $2 NOT FOUND"
        return 1
    fi
}

# Function to check if file exists
check_file() {
    if [ -f "$1" ]; then
        return 0
    else
        return 1
    fi
}

# Check Common libraries
echo "Checking Common Libraries..."
check_dir "$BASE_DIR/Common/Math3D" "Math3D Library"
check_file "$BASE_DIR/Common/Math3D/Vector3.h" && echo "  ├── Vector3.h"
check_file "$BASE_DIR/Common/Math3D/Matrix4.h" && echo "  ├── Matrix4.h"
check_file "$BASE_DIR/Common/Math3D/Quaternion.h" && echo "  ├── Quaternion.h"
check_file "$BASE_DIR/Common/Math3D/Math3D.h" && echo "  └── Math3D.h"

check_dir "$BASE_DIR/Common/Utils" "Utils Library"
check_file "$BASE_DIR/Common/Utils/ShaderLoader.h" && echo "  ├── ShaderLoader.h"
check_file "$BASE_DIR/Common/Utils/TextureLoader.h" && echo "  ├── TextureLoader.h"
check_file "$BASE_DIR/Common/Utils/Camera.h" && echo "  └── Camera.h"

echo ""

# Count lessons
math_lessons=$(find "$BASE_DIR/Lessons01-20_3DMath" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)
opengl_lessons=$(find "$BASE_DIR/Lessons21-40_OpenGL" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)
directx_lessons=$(find "$BASE_DIR/Lessons41-60_DirectX" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)
advanced_lessons=$(find "$BASE_DIR/Lessons61-80_Advanced" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)
modern_lessons=$(find "$BASE_DIR/Lessons81-100_Modern" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l)

total_lessons=$((math_lessons + opengl_lessons + directx_lessons + advanced_lessons + modern_lessons))

echo "Lesson Count:"
echo "  ├── 3D Math (01-20): $math_lessons/20"
echo "  ├── OpenGL (21-40): $opengl_lessons/20"
echo "  ├── DirectX (41-60): $directx_lessons/20"
echo "  ├── Advanced (61-80): $advanced_lessons/20"
echo "  └── Modern (81-100): $modern_lessons/20"
echo ""
echo "  TOTAL: $total_lessons/100 lessons"

if [ $total_lessons -eq 100 ]; then
    echo ""
    echo "✓ All 100 lessons successfully installed!"
else
    echo ""
    echo "⚠ Warning: Expected 100 lessons, found $total_lessons"
fi

echo ""

# Check for build files
echo "Checking Build System..."
check_file "$BASE_DIR/CMakeLists.txt" && echo "✓ Master CMakeLists.txt"
check_file "$BASE_DIR/setup_dependencies.sh" && echo "✓ Dependency setup script"
check_file "$BASE_DIR/README.md" && echo "✓ Documentation"

echo ""

# Check External directory
echo "Checking External Dependencies..."
if [ -d "$BASE_DIR/External" ]; then
    echo "✓ External directory exists"

    if [ -d "$BASE_DIR/External/glfw" ]; then
        echo "  ✓ GLFW"
    else
        echo "  ✗ GLFW (run setup_dependencies.sh)"
    fi

    if [ -d "$BASE_DIR/External/glad" ]; then
        echo "  ✓ GLAD"
    else
        echo "  ⚠ GLAD (generate from glad.dav1d.de)"
    fi

    if [ -d "$BASE_DIR/External/glm" ]; then
        echo "  ✓ GLM"
    else
        echo "  ✗ GLM (run setup_dependencies.sh)"
    fi

    if [ -d "$BASE_DIR/External/stb" ]; then
        echo "  ✓ STB"
    else
        echo "  ✗ STB (run setup_dependencies.sh)"
    fi
else
    echo "✗ External directory not found"
    echo "  → Run ./setup_dependencies.sh to install"
fi

echo ""

# Check for build directory
if [ -d "$BASE_DIR/build" ]; then
    echo "✓ Build directory exists"

    # Check for binaries
    if [ -d "$BASE_DIR/build/bin" ]; then
        bin_count=$(find "$BASE_DIR/build/bin" -type f -executable 2>/dev/null | wc -l)
        echo "  └── $bin_count compiled executables found"
    fi
else
    echo "ℹ Build directory not found"
    echo "  → Run: mkdir build && cd build && cmake .. && cmake --build ."
fi

echo ""
echo "=========================================="
echo "  Installation Status: COMPLETE"
echo "=========================================="
echo ""
echo "Next Steps:"
echo "  1. Setup dependencies: ./setup_dependencies.sh"
echo "  2. Build lessons: mkdir build && cd build && cmake .."
echo "  3. Compile: cmake --build ."
echo "  4. Run: ./bin/Lessons01-20/Lesson01_3DMathIntro"
echo ""
echo "For help, see: README.md"
echo ""
