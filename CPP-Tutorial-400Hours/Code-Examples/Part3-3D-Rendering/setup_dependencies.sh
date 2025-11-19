#!/bin/bash

# External Dependencies Setup Script
# Downloads and configures all required dependencies for the 3D Rendering course

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXTERNAL_DIR="$BASE_DIR/External"

echo "=========================================="
echo "  3D Rendering Course - Dependency Setup"
echo "=========================================="
echo ""
echo "This script will download and configure:"
echo "  - GLFW 3.3.8 (Window/Input)"
echo "  - GLAD (OpenGL Loader)"
echo "  - GLM 0.9.9.8 (Math Library)"
echo "  - STB Image (Image Loading)"
echo "  - Assimp (Model Loading)"
echo ""

mkdir -p "$EXTERNAL_DIR"
cd "$EXTERNAL_DIR"

# Function to download and extract
download_and_extract() {
    local url=$1
    local filename=$2
    local dirname=$3

    if [ -d "$dirname" ]; then
        echo "✓ $dirname already exists, skipping..."
        return
    fi

    echo "Downloading $filename..."
    curl -L "$url" -o "$filename"

    if [[ $filename == *.zip ]]; then
        echo "Extracting $filename..."
        unzip -q "$filename"
        rm "$filename"
    elif [[ $filename == *.tar.gz ]]; then
        echo "Extracting $filename..."
        tar -xzf "$filename"
        rm "$filename"
    fi

    echo "✓ $dirname installed"
}

# GLFW
echo ""
echo "[1/5] Installing GLFW..."
if [ ! -d "glfw" ]; then
    git clone --depth 1 --branch 3.3.8 https://github.com/glfw/glfw.git
    echo "✓ GLFW installed"
else
    echo "✓ GLFW already installed"
fi

# GLAD
echo ""
echo "[2/5] Installing GLAD..."
if [ ! -d "glad" ]; then
    mkdir -p glad/include/glad
    mkdir -p glad/include/KHR
    mkdir -p glad/src

    # Download GLAD files (OpenGL 3.3 Core)
    echo "Downloading GLAD files..."
    cat > glad/include/glad/glad.h << 'EOF'
// This is a placeholder - in a real setup, generate from https://glad.dav1d.de/
// For this course, you can use the web generator with:
// - Language: C/C++
// - Specification: OpenGL
// - API: gl Version 3.3
// - Profile: Core
#ifndef __GLAD_H_
#define __GLAD_H_
// Include actual GLAD header here
#endif
EOF

    cat > glad/src/glad.c << 'EOF'
// GLAD implementation
// Generate from https://glad.dav1d.de/
EOF

    echo "✓ GLAD structure created (generate full files from glad.dav1d.de)"
else
    echo "✓ GLAD already installed"
fi

# GLM
echo ""
echo "[3/5] Installing GLM..."
if [ ! -d "glm" ]; then
    git clone --depth 1 --branch 0.9.9.8 https://github.com/g-truc/glm.git
    echo "✓ GLM installed"
else
    echo "✓ GLM already installed"
fi

# STB
echo ""
echo "[4/5] Installing STB..."
if [ ! -d "stb" ]; then
    mkdir -p stb
    cd stb
    curl -L https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -o stb_image.h
    curl -L https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h -o stb_image_write.h

    # Create implementation file
    cat > stb_image_impl.cpp << 'EOF'
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
EOF

    cd ..
    echo "✓ STB installed"
else
    echo "✓ STB already installed"
fi

# Assimp (optional - for model loading)
echo ""
echo "[5/5] Installing Assimp (optional)..."
if command -v apt-get &> /dev/null; then
    echo "Debian/Ubuntu detected - install with: sudo apt-get install libassimp-dev"
elif command -v brew &> /dev/null; then
    echo "macOS detected - install with: brew install assimp"
elif command -v pacman &> /dev/null; then
    echo "Arch Linux detected - install with: sudo pacman -S assimp"
else
    echo "Manual installation required - visit: https://github.com/assimp/assimp"
fi

echo ""
echo "=========================================="
echo "  Dependency Setup Complete!"
echo "=========================================="
echo ""
echo "Directory structure:"
echo "  External/"
echo "    ├── glfw/          (Window/Input)"
echo "    ├── glad/          (OpenGL Loader)"
echo "    ├── glm/           (Math Library)"
echo "    ├── stb/           (Image Loading)"
echo "    └── assimp/        (Model Loading)"
echo ""
echo "Next steps:"
echo "  1. Generate full GLAD files from https://glad.dav1d.de/"
echo "  2. Run: mkdir build && cd build"
echo "  3. Run: cmake .."
echo "  4. Run: cmake --build ."
echo ""
echo "For GLAD generation:"
echo "  - Visit: https://glad.dav1d.de/"
echo "  - Language: C/C++"
echo "  - Specification: OpenGL"
echo "  - API: gl Version 3.3 or higher"
echo "  - Profile: Core"
echo "  - Generate a loader: Yes"
echo "  - Download and extract to External/glad/"
echo ""
