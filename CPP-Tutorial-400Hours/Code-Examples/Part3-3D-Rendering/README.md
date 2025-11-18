# C++ 3D Rendering Course - Complete Code Examples

## 📋 Overview

This directory contains **100 complete, executable lessons** covering comprehensive 3D graphics programming in C++. Each lesson includes full source code, shaders, build files, and documentation.

## 🎯 Course Structure

### Part 1: 3D Mathematics (Lessons 01-20)
Complete 3D math library implementation covering all essential concepts:

- **Lessons 01-05**: Vector basics and operations
- **Lessons 06-10**: Matrix mathematics and transformations
- **Lessons 11-15**: Quaternions, cameras, and projections
- **Lessons 16-20**: Advanced math (frustum culling, ray tracing, bounding volumes)

**Path**: `Lessons01-20_3DMath/`

### Part 2: OpenGL Fundamentals (Lessons 21-40)
Complete OpenGL rendering pipeline from basics to advanced:

- **Lessons 21-25**: Window creation, VBO/VAO, first triangle
- **Lessons 26-30**: Shaders, texturing, transformations
- **Lessons 31-35**: Camera, lighting models, materials
- **Lessons 36-40**: Multiple lights, model loading, blending

**Path**: `Lessons21-40_OpenGL/`

### Part 3: DirectX 11 (Lessons 41-60)
DirectX 11 rendering with HLSL shaders (Windows only):

- **Lessons 41-45**: D3D11 setup, device/context, vertex buffers
- **Lessons 46-50**: Index buffers, HLSL shaders, textures
- **Lessons 51-55**: States, DirectXMath, camera
- **Lessons 56-60**: Lighting, normal mapping, shadows, DX12 intro

**Path**: `Lessons41-60_DirectX/`

### Part 4: Advanced Rendering (Lessons 61-80)
Modern rendering techniques:

- **Lessons 61-65**: Deferred rendering, SSAO, PBR
- **Lessons 66-70**: HDR, bloom, cubemaps, reflections
- **Lessons 71-75**: Geometry/tessellation/compute shaders, instancing, LOD
- **Lessons 76-80**: Culling, post-processing, anti-aliasing, particles

**Path**: `Lessons61-80_Advanced/`

### Part 5: Modern Rendering (Lessons 81-100)
Cutting-edge techniques and engine architecture:

- **Lessons 81-85**: Procedural textures, water, terrain, vegetation
- **Lessons 86-90**: Volumetric effects, ray tracing, path tracing
- **Lessons 91-95**: Vulkan, API comparison, optimization, multithreading
- **Lessons 96-100**: Complete engine, game renderer, material editor, best practices

**Path**: `Lessons81-100_Modern/`

## 🚀 Quick Start

### Prerequisites

- **CMake** 3.15 or higher
- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenGL** 3.3 compatible GPU
- **Git** (for dependency download)

### Windows
```cmd
Visual Studio 2019 or later with C++ Desktop Development workload
```

### Linux
```bash
sudo apt-get install build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

### macOS
```bash
brew install cmake
xcode-select --install
```

## 📦 Installation

### 1. Clone the Repository
```bash
cd CPP-Tutorial-400Hours/Code-Examples/Part3-3D-Rendering
```

### 2. Setup Dependencies
```bash
chmod +x setup_dependencies.sh
./setup_dependencies.sh
```

This downloads:
- **GLFW** - Window and input management
- **GLAD** - OpenGL function loader
- **GLM** - Mathematics library
- **STB Image** - Image loading
- **Assimp** - Model loading (optional)

### 3. Generate GLAD Files

Visit [https://glad.dav1d.de/](https://glad.dav1d.de/) and configure:
- Language: **C/C++**
- Specification: **OpenGL**
- API gl: **Version 3.3** (or 4.6 for advanced features)
- Profile: **Core**
- Options: **Generate a loader**

Download and extract to `External/glad/`

### 4. Build All Lessons
```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

### Build Specific Sections
```bash
# Math lessons only
cmake --build . --target Lesson01_3DMathIntro

# OpenGL lessons
cmake --build . --target Lesson21_OpenGLSetup
cmake --build . --target Lesson25_FirstTriangle

# Advanced lessons
cmake --build . --target Lesson61_DeferredRendering
```

## 🎮 Running Lessons

### Math Lessons (01-20)
```bash
./bin/Lessons01-20/Lesson01_3DMathIntro
./bin/Lessons01-20/Lesson04_DotCrossProduct
```

### OpenGL Lessons (21-40)
```bash
./bin/Lessons21-40/Lesson21_OpenGLSetup
./bin/Lessons21-40/Lesson25_FirstTriangle
./bin/Lessons21-40/Lesson35_PhongLighting
```

### DirectX Lessons (41-60) [Windows Only]
```bash
./bin/Lessons41-60/Lesson41_DirectXIntro.exe
./bin/Lessons41-60/Lesson47_HLSLShaders.exe
```

### Advanced Lessons (61-100)
```bash
./bin/Lessons61-80_Advanced/Lesson61_DeferredRendering
./bin/Lessons81-100_Modern/Lesson88_RayTracing
```

## 📁 Directory Structure

```
Part3-3D-Rendering/
├── CMakeLists.txt                    # Master build file
├── README.md                         # This file
├── setup_dependencies.sh             # Dependency installer
│
├── Common/                           # Shared libraries
│   ├── Math3D/                       # Complete 3D math library
│   │   ├── Vector3.h                 # 3D vector class
│   │   ├── Matrix4.h                 # 4x4 matrix class
│   │   ├── Quaternion.h              # Quaternion class
│   │   └── Math3D.h                  # Complete library header
│   │
│   └── Utils/                        # Utility libraries
│       ├── ShaderLoader.h            # Shader compilation
│       ├── TextureLoader.h           # Texture loading
│       └── Camera.h                  # FPS camera
│
├── External/                         # Third-party dependencies
│   ├── glfw/                         # Window library
│   ├── glad/                         # OpenGL loader
│   ├── glm/                          # Math library
│   ├── stb/                          # Image loading
│   └── assimp/                       # Model loading
│
├── Lessons01-20_3DMath/              # Math lessons
│   ├── Lesson01_3DMathIntro/
│   │   ├── main.cpp
│   │   ├── CMakeLists.txt
│   │   └── README.md
│   └── ... (20 lessons)
│
├── Lessons21-40_OpenGL/              # OpenGL lessons
│   ├── Lesson21_OpenGLSetup/
│   │   ├── main.cpp
│   │   ├── CMakeLists.txt
│   │   └── README.md
│   ├── Lesson25_FirstTriangle/
│   │   ├── main.cpp
│   │   ├── vertex.glsl               # Vertex shader
│   │   ├── fragment.glsl             # Fragment shader
│   │   ├── CMakeLists.txt
│   │   └── README.md
│   └── ... (20 lessons with shaders)
│
├── Lessons41-60_DirectX/             # DirectX lessons
│   ├── Lesson41_DirectXIntro/
│   │   ├── main.cpp
│   │   ├── VertexShader.hlsl         # HLSL vertex shader
│   │   ├── PixelShader.hlsl          # HLSL pixel shader
│   │   ├── CMakeLists.txt
│   │   └── README.md
│   └── ... (20 lessons with HLSL)
│
├── Lessons61-80_Advanced/            # Advanced rendering
│   ├── Lesson61_DeferredRendering/
│   ├── Lesson64_PBRImplementation/
│   └── ... (20 advanced lessons)
│
└── Lessons81-100_Modern/             # Modern techniques
    ├── Lesson88_RayTracing/
    ├── Lesson91_Vulkan/
    ├── Lesson96_3DEngine/
    └── ... (20 modern lessons)
```

## 🔧 Build Configuration

### CMake Options
```bash
# Build only OpenGL examples
cmake -DBUILD_OPENGL_EXAMPLES=ON -DBUILD_DIRECTX_EXAMPLES=OFF ..

# Build only DirectX examples (Windows)
cmake -DBUILD_OPENGL_EXAMPLES=OFF -DBUILD_DIRECTX_EXAMPLES=ON ..

# Build shared libraries
cmake -DBUILD_SHARED_LIBS=ON ..

# Set build type
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Compiler-Specific Builds

#### GCC/Clang
```bash
mkdir build && cd build
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
make -j$(nproc)
```

#### MSVC
```cmd
mkdir build && cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

#### MinGW (Windows)
```cmd
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -j%NUMBER_OF_PROCESSORS%
```

## 📚 Lesson Details

### Example Lesson Structure

Each lesson contains:
- **main.cpp**: Complete, runnable implementation
- **shaders**: GLSL (.glsl) or HLSL (.hlsl) shader files
- **CMakeLists.txt**: Build configuration
- **README.md**: Lesson overview, build instructions, key concepts

### Key Features

✅ **Complete Code**: Every lesson is fully functional and compiles
✅ **Modern C++**: C++17 features and best practices
✅ **Cross-Platform**: Works on Windows, Linux, macOS (OpenGL lessons)
✅ **Well-Commented**: Extensive inline documentation
✅ **Progressive**: Each lesson builds on previous concepts
✅ **Practical**: Real-world rendering techniques
✅ **Optimized**: Performance-conscious implementations

## 🎓 Learning Path

### Beginner Path (No prior graphics experience)
1. Start with Lessons 01-05 (Vector math basics)
2. Continue to Lessons 06-10 (Matrices and transformations)
3. Learn OpenGL with Lessons 21-30
4. Practice with Lessons 31-40 (Lighting and materials)

### Intermediate Path (Some graphics knowledge)
1. Review Lessons 01-20 (Math foundation)
2. Jump to Lessons 25-40 (Complete OpenGL pipeline)
3. Explore Lessons 41-50 (DirectX basics)
4. Study Lessons 61-70 (Advanced techniques)

### Advanced Path (Experienced developers)
1. Skim Lessons 01-40 (Foundation review)
2. Focus on Lessons 61-80 (PBR, deferred rendering, etc.)
3. Master Lessons 81-100 (Ray tracing, Vulkan, engine architecture)

## 🎮 Controls

Most interactive lessons use these controls:

- **W/A/S/D**: Move camera forward/left/back/right
- **Q/E**: Move camera up/down
- **Mouse**: Look around
- **Scroll Wheel**: Zoom
- **ESC**: Exit application
- **F1**: Toggle wireframe (some lessons)
- **F2**: Toggle debug view (some lessons)

## 🐛 Troubleshooting

### GLAD Not Found
```bash
# Download from https://glad.dav1d.de/
# Extract to External/glad/
```

### OpenGL Version Error
```cpp
// Update CMakeLists.txt to request OpenGL 3.3
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
```

### DirectX Lessons Won't Build
- Ensure you're on Windows
- Install Windows SDK
- Use Visual Studio 2017 or later

### Shader Compilation Errors
- Check shader file paths in main.cpp
- Verify shaders are copied to build directory
- Check OpenGL/DirectX version compatibility

## 📖 Additional Resources

### Documentation
- **OpenGL**: [https://www.opengl.org/](https://www.opengl.org/)
- **DirectX**: [https://docs.microsoft.com/en-us/windows/win32/direct3d](https://docs.microsoft.com/en-us/windows/win32/direct3d)
- **GLFW**: [https://www.glfw.org/docs/latest/](https://www.glfw.org/docs/latest/)
- **Learn OpenGL**: [https://learnopengl.com/](https://learnopengl.com/)

### Tools
- **RenderDoc**: Graphics debugger [https://renderdoc.org/](https://renderdoc.org/)
- **NVIDIA Nsight**: GPU profiler
- **PIX**: DirectX debugger (Windows)

## 🏆 Course Completion

After completing all 100 lessons, you will have:

✅ Comprehensive understanding of 3D mathematics
✅ Mastery of OpenGL rendering pipeline
✅ DirectX 11 expertise (Windows)
✅ Advanced rendering technique implementation
✅ Modern graphics programming skills
✅ Complete 3D engine architecture knowledge
✅ Portfolio of 100 working graphics projects

## 📄 License

This course material is provided for educational purposes. Individual dependencies have their own licenses:
- GLFW: zlib/libpng license
- GLAD: MIT license
- GLM: MIT license
- STB: Public domain

## 👥 Support

For issues, questions, or contributions:
1. Check lesson README files
2. Review code comments
3. Consult the main course documentation

## 🎯 Next Steps

1. **Setup**: Run `./setup_dependencies.sh`
2. **Build**: `mkdir build && cd build && cmake .. && cmake --build .`
3. **Start**: Begin with `Lesson01_3DMathIntro`
4. **Learn**: Progress through all 100 lessons
5. **Create**: Build your own 3D applications!

---

**Happy Coding! 🚀**

Welcome to the complete C++ 3D Rendering course. You now have 100 fully functional, compilable lessons ready to explore. Start your journey into professional 3D graphics programming!
