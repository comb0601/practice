#!/bin/bash
################################################################################
#  C++ Tutorial 400 Hours - Linux Environment Setup Script
################################################################################
#  This script checks and helps setup the build environment for Linux
################################################################################

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo ""
echo "============================================================================"
echo "  C++ Tutorial 400 Hours - Environment Setup (Linux)"
echo "============================================================================"
echo ""
echo "  Checking required tools and dependencies..."
echo ""

ERROR_COUNT=0
WARNING_COUNT=0

################################################################################
# Detect Linux distribution
################################################################################
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO=$ID
    DISTRO_VERSION=$VERSION_ID
else
    DISTRO="unknown"
fi

echo -e "${BLUE}Detected OS: $PRETTY_NAME${NC}"
echo ""

################################################################################
# Check for CMake
################################################################################
echo "[1/7] Checking for CMake..."
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    echo -e "  ${GREEN}[OK]${NC} CMake found: version $CMAKE_VERSION"

    # Check if version is sufficient (>= 3.15)
    CMAKE_MAJOR=$(echo $CMAKE_VERSION | cut -d. -f1)
    CMAKE_MINOR=$(echo $CMAKE_VERSION | cut -d. -f2)
    if [ "$CMAKE_MAJOR" -lt 3 ] || ([ "$CMAKE_MAJOR" -eq 3 ] && [ "$CMAKE_MINOR" -lt 15 ]); then
        echo -e "  ${YELLOW}[WARNING]${NC} CMake version should be >= 3.15"
        ((WARNING_COUNT++))
    fi
else
    echo -e "  ${RED}[MISSING]${NC} CMake not found!"
    echo "  Install command for your distribution:"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt update && sudo apt install cmake"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf install cmake"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S cmake"
            ;;
        opensuse*)
            echo "    sudo zypper install cmake"
            ;;
        *)
            echo "    Please install CMake from your package manager"
            ;;
    esac
    ((ERROR_COUNT++))
fi

################################################################################
# Check for C++ compiler
################################################################################
echo ""
echo "[2/7] Checking for C++ compiler..."
COMPILER_FOUND=0

if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | grep -oP '\d+\.\d+\.\d+' | head -n1)
    echo -e "  ${GREEN}[OK]${NC} g++ found: version $GCC_VERSION"
    COMPILER_FOUND=1
fi

if command -v clang++ &> /dev/null; then
    CLANG_VERSION=$(clang++ --version | head -n1 | grep -oP '\d+\.\d+\.\d+' | head -n1)
    echo -e "  ${GREEN}[OK]${NC} clang++ found: version $CLANG_VERSION"
    COMPILER_FOUND=1
fi

if [ $COMPILER_FOUND -eq 0 ]; then
    echo -e "  ${RED}[MISSING]${NC} No C++ compiler found!"
    echo "  Install command for your distribution:"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt update && sudo apt install build-essential"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf groupinstall 'Development Tools'"
            echo "    sudo dnf install gcc-c++"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S base-devel"
            ;;
        opensuse*)
            echo "    sudo zypper install -t pattern devel_C_C++"
            ;;
        *)
            echo "    Please install g++ or clang++ from your package manager"
            ;;
    esac
    ((ERROR_COUNT++))
fi

################################################################################
# Check for Make
################################################################################
echo ""
echo "[3/7] Checking for Make..."
if command -v make &> /dev/null; then
    MAKE_VERSION=$(make --version | head -n1 | grep -oP '\d+\.\d+' | head -n1)
    echo -e "  ${GREEN}[OK]${NC} Make found: version $MAKE_VERSION"
else
    echo -e "  ${YELLOW}[WARNING]${NC} Make not found (optional, CMake can use Ninja)"
    echo "  Install: Usually included with build-essential or base-devel"
    ((WARNING_COUNT++))
fi

################################################################################
# Check for Git
################################################################################
echo ""
echo "[4/7] Checking for Git..."
if command -v git &> /dev/null; then
    GIT_VERSION=$(git --version | cut -d' ' -f3)
    echo -e "  ${GREEN}[OK]${NC} Git found: version $GIT_VERSION"
else
    echo -e "  ${YELLOW}[WARNING]${NC} Git not found"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt install git"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf install git"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S git"
            ;;
        *)
            echo "    Install from your package manager"
            ;;
    esac
    ((WARNING_COUNT++))
fi

################################################################################
# Check for Python (for run_examples.py)
################################################################################
echo ""
echo "[5/7] Checking for Python..."
if command -v python3 &> /dev/null; then
    PYTHON_VERSION=$(python3 --version | cut -d' ' -f2)
    echo -e "  ${GREEN}[OK]${NC} Python3 found: version $PYTHON_VERSION"
else
    echo -e "  ${YELLOW}[WARNING]${NC} Python3 not found"
    echo "  Required for run_examples.py interactive script"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt install python3"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf install python3"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S python"
            ;;
        *)
            echo "    Install from your package manager"
            ;;
    esac
    ((WARNING_COUNT++))
fi

################################################################################
# Check for OpenGL development libraries (for Part 3)
################################################################################
echo ""
echo "[6/7] Checking for OpenGL development libraries..."
OPENGL_FOUND=0

# Check for GL headers
if [ -f /usr/include/GL/gl.h ] || [ -f /usr/local/include/GL/gl.h ]; then
    echo -e "  ${GREEN}[OK]${NC} OpenGL headers found"
    OPENGL_FOUND=1
else
    echo -e "  ${YELLOW}[WARNING]${NC} OpenGL development headers not found"
    echo "  Required for Part 3 (3D Rendering) OpenGL examples"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt install libgl1-mesa-dev libglu1-mesa-dev"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf install mesa-libGL-devel mesa-libGLU-devel"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S mesa glu"
            ;;
        *)
            echo "    Install OpenGL development libraries from your package manager"
            ;;
    esac
    ((WARNING_COUNT++))
fi

# Check for X11 libraries (needed for GLFW)
if pkg-config --exists x11 2>/dev/null; then
    echo -e "  ${GREEN}[OK]${NC} X11 libraries found"
else
    echo -e "  ${YELLOW}[WARNING]${NC} X11 development libraries not found"
    echo "  Required for GLFW window creation in Part 3"
    case $DISTRO in
        ubuntu|debian)
            echo "    sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
            ;;
        fedora|rhel|centos)
            echo "    sudo dnf install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel"
            ;;
        arch|manjaro)
            echo "    sudo pacman -S libx11 libxrandr libxinerama libxcursor libxi"
            ;;
        *)
            echo "    Install X11 development libraries from your package manager"
            ;;
    esac
    ((WARNING_COUNT++))
fi

################################################################################
# Check disk space
################################################################################
echo ""
echo "[7/7] Checking disk space..."
FREE_SPACE_KB=$(df . | tail -1 | awk '{print $4}')
FREE_SPACE_GB=$((FREE_SPACE_KB / 1024 / 1024))

if [ $FREE_SPACE_GB -gt 5 ]; then
    echo -e "  ${GREEN}[OK]${NC} Sufficient disk space: ~${FREE_SPACE_GB} GB free"
else
    echo -e "  ${YELLOW}[WARNING]${NC} Low disk space: ~${FREE_SPACE_GB} GB free"
    echo "  Recommended: At least 5 GB for build artifacts"
    ((WARNING_COUNT++))
fi

################################################################################
# Print summary
################################################################################
echo ""
echo "============================================================================"
echo "  Environment Check Summary"
echo "============================================================================"
echo ""

if [ $ERROR_COUNT -eq 0 ]; then
    echo -e "  ${GREEN}[OK]${NC} All required tools are installed!"
    echo ""
    if [ $WARNING_COUNT -gt 0 ]; then
        echo -e "  ${YELLOW}[INFO]${NC} $WARNING_COUNT optional warning(s) - see above"
        echo ""
    fi
    echo "  You can now build the project:"
    echo "    ./build_all_linux.sh"
    echo ""
    echo "  Or build specific parts:"
    echo "    cmake -S . -B build -DBUILD_PART1_FUNDAMENTALS=ON"
    echo "    cmake --build build -j\$(nproc)"
else
    echo -e "  ${RED}[ERROR]${NC} $ERROR_COUNT required tool(s) missing!"
    echo -e "  ${YELLOW}[WARNING]${NC} $WARNING_COUNT optional warning(s)"
    echo ""
    echo "  Please install the missing tools and run this script again."
fi

echo "============================================================================"
echo ""

################################################################################
# Optional: Offer to install dependencies
################################################################################
if [ $ERROR_COUNT -gt 0 ] || [ $WARNING_COUNT -gt 0 ]; then
    echo ""
    read -p "Would you like to attempt automatic installation of missing dependencies? (y/N) " -n 1 -r
    echo ""

    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo ""
        echo "Installing dependencies..."

        case $DISTRO in
            ubuntu|debian)
                sudo apt update
                sudo apt install -y cmake build-essential git python3 \
                    libgl1-mesa-dev libglu1-mesa-dev \
                    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
                ;;
            fedora|rhel|centos)
                sudo dnf groupinstall -y 'Development Tools'
                sudo dnf install -y cmake gcc-c++ git python3 \
                    mesa-libGL-devel mesa-libGLU-devel \
                    libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
                ;;
            arch|manjaro)
                sudo pacman -S --needed base-devel cmake git python mesa glu \
                    libx11 libxrandr libxinerama libxcursor libxi
                ;;
            opensuse*)
                sudo zypper install -t pattern devel_C_C++
                sudo zypper install cmake git python3 Mesa-libGL-devel Mesa-libGLU-devel \
                    libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
                ;;
            *)
                echo "Automatic installation not supported for your distribution."
                echo "Please install dependencies manually."
                ;;
        esac

        echo ""
        echo "Installation complete! Please run this script again to verify."
    fi
fi

exit $ERROR_COUNT
