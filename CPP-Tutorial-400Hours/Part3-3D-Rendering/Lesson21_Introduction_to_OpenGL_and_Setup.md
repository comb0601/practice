# Lesson 21: Introduction to OpenGL and Setup

## Table of Contents
1. [What is OpenGL?](#what-is-opengl)
2. [OpenGL History and Evolution](#opengl-history-and-evolution)
3. [Modern OpenGL vs Legacy OpenGL](#modern-opengl-vs-legacy-opengl)
4. [OpenGL Architecture](#opengl-architecture)
5. [Setting Up OpenGL Development Environment](#setting-up-opengl-development-environment)
6. [Required Libraries and Tools](#required-libraries-and-tools)
7. [Complete Setup Example](#complete-setup-example)
8. [Your First OpenGL Program](#your-first-opengl-program)
9. [Common Setup Issues and Solutions](#common-setup-issues-and-solutions)
10. [Best Practices](#best-practices)

## What is OpenGL?

OpenGL (Open Graphics Library) is a cross-platform, language-independent API for rendering 2D and 3D vector graphics. It provides a powerful and flexible framework for creating interactive graphics applications, games, simulations, and scientific visualizations.

### Key Characteristics of OpenGL

**Cross-Platform Compatibility**: OpenGL runs on Windows, Linux, macOS, and mobile platforms, making it an excellent choice for portable graphics applications.

**Hardware Acceleration**: OpenGL leverages the GPU (Graphics Processing Unit) to perform massive parallel computations, achieving rendering speeds orders of magnitude faster than CPU-based rendering.

**Industry Standard**: Developed by the Khronos Group, OpenGL is an open standard supported by all major GPU manufacturers (NVIDIA, AMD, Intel).

**State Machine**: OpenGL operates as a large state machine. You set various states and options, and these remain active until you change them.

### What Can You Build with OpenGL?

- **3D Games**: Real-time rendering engines for games
- **CAD Applications**: Computer-aided design and modeling tools
- **Scientific Visualization**: Data visualization and simulation
- **Virtual Reality**: VR and AR applications
- **Video Processing**: Real-time video effects and filters
- **Medical Imaging**: 3D reconstruction and visualization

## OpenGL History and Evolution

### The Timeline

**1992 - OpenGL 1.0**: Released by Silicon Graphics, Inc. (SGI) as an open standard alternative to proprietary graphics APIs.

**1992-2004 - OpenGL 1.x - 2.x**: The "fixed-function pipeline" era. Graphics programming was done through predefined functions like `glBegin()`, `glVertex3f()`, and `glEnd()`.

**2008 - OpenGL 3.0**: Introduction of the deprecation model and the beginnings of modern OpenGL.

**2010 - OpenGL 3.3**: The version we'll focus on in this course. Marks the transition to programmable shaders and modern GPU programming.

**2014 - OpenGL 4.5**: Added advanced features like direct state access and improved debugging.

**2016 - Vulkan**: The successor to OpenGL, offering even lower-level hardware access and better multi-threading support.

### Why Learn OpenGL 3.3+?

Modern OpenGL (3.3 and above) provides:
- **Better Performance**: Direct control over GPU resources
- **Greater Flexibility**: Programmable shaders allow custom rendering effects
- **Industry Relevance**: Most game engines and graphics applications use modern OpenGL principles
- **Foundation for Vulkan**: Understanding modern OpenGL makes learning Vulkan much easier

## Modern OpenGL vs Legacy OpenGL

### Legacy OpenGL (Fixed-Function Pipeline)

The old way of OpenGL programming used immediate mode:

```cpp
// LEGACY - DO NOT USE
glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
glEnd();
```

**Problems with Legacy OpenGL**:
- **Slow**: Data sent to GPU every frame
- **Inflexible**: Limited to predefined rendering operations
- **Deprecated**: Not supported on modern hardware
- **CPU Bottleneck**: Too much CPU-GPU communication

### Modern OpenGL (Programmable Pipeline)

Modern OpenGL uses buffer objects and shaders:

```cpp
// MODERN - This is what we'll learn
GLuint VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Use custom shaders
glUseProgram(shaderProgram);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

**Advantages of Modern OpenGL**:
- **Fast**: Data stored on GPU, minimal CPU-GPU communication
- **Flexible**: Complete control through custom shaders
- **Scalable**: Handles complex scenes efficiently
- **Future-Proof**: Aligns with modern GPU architecture

## OpenGL Architecture

### The Graphics Pipeline

OpenGL processes graphics through a pipeline with several stages:

```
Vertex Data
    ↓
Vertex Shader (Programmable)
    ↓
Shape Assembly
    ↓
Geometry Shader (Programmable, Optional)
    ↓
Rasterization
    ↓
Fragment Shader (Programmable)
    ↓
Tests and Blending
    ↓
Framebuffer
```

### Key Components

**1. Vertex Shader**: Processes each vertex (position transformations, lighting calculations)

**2. Fragment Shader**: Determines the color of each pixel

**3. Vertex Buffer Objects (VBO)**: Store vertex data on the GPU

**4. Vertex Array Objects (VAO)**: Store vertex attribute configurations

**5. Element Buffer Objects (EBO)**: Store indices for indexed drawing

**6. Textures**: Images applied to 3D surfaces

**7. Framebuffers**: Render targets (screen or off-screen)

## Setting Up OpenGL Development Environment

### System Requirements

- **Operating System**: Windows 10/11, Ubuntu 20.04+, or macOS 10.14+
- **Compiler**: GCC 9+, Clang 10+, or MSVC 2019+
- **GPU**: Any GPU from the last 10 years (supports OpenGL 3.3+)
- **RAM**: 4GB minimum, 8GB recommended

### Installing Compilers

#### Windows (MSYS2/MinGW-w64)

```bash
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-make
```

#### Windows (Visual Studio)

1. Download Visual Studio Community 2022
2. Install "Desktop development with C++" workload
3. Ensure C++ CMake tools are selected

#### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
```

#### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake
```

## Required Libraries and Tools

### GLFW (Graphics Library Framework)

GLFW provides window creation, input handling, and OpenGL context management.

**Installation**:

```bash
# Ubuntu/Debian
sudo apt install libglfw3-dev

# macOS
brew install glfw

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-glfw

# Or build from source:
git clone https://github.com/glfw/glfw.git
cd glfw
cmake -S . -B build
cmake --build build
sudo cmake --install build
```

### GLAD (OpenGL Loader)

GLAD loads OpenGL function pointers. Generate it from https://glad.dav1d.de/:

1. Language: C/C++
2. Specification: OpenGL
3. API gl: Version 3.3 or higher
4. Profile: Core
5. Generate loader: Checked
6. Download the generated files

### GLM (OpenGL Mathematics)

GLM provides vector and matrix math for graphics.

```bash
# Ubuntu/Debian
sudo apt install libglm-dev

# macOS
brew install glm

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-glm

# Or download headers from: https://github.com/g-truc/glm
```

## Complete Setup Example

### Project Structure

```
OpenGLProject/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
└── lib/
    └── glad.c
```

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.15)
project(OpenGLBasics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add GLAD
add_library(glad lib/glad.c)
target_include_directories(glad PUBLIC include)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    glad
    glm::glm
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
```

### Build Script (build.sh for Linux/macOS)

```bash
#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./OpenGLBasics
```

### Build Script (build.bat for Windows)

```batch
@echo off

REM Create build directory
if not exist build mkdir build
cd build

REM Configure and build
cmake ..
cmake --build . --config Release

REM Run the application
Release\OpenGLBasics.exe
```

## Your First OpenGL Program

### Complete Working Example

```cpp
// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Window dimensions
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Callback function for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "OpenGL Tutorial - Lesson 21",
        nullptr,
        nullptr
    );

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL information
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        processInput(window);

        // Render commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark cyan background
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
```

### Understanding the Code

**GLFW Initialization**:
```cpp
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
```
Sets up GLFW and requests OpenGL 3.3 context.

**Window Creation**:
```cpp
GLFWwindow* window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
glfwMakeContextCurrent(window);
```
Creates a window and makes its OpenGL context current.

**GLAD Initialization**:
```cpp
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
```
Loads all OpenGL function pointers.

**Render Loop**:
```cpp
while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```
Continuously renders frames until the window is closed.

## Common Setup Issues and Solutions

### Issue 1: "Failed to initialize GLAD"

**Cause**: GLAD not finding OpenGL functions.

**Solution**:
- Ensure GLAD is initialized AFTER making the OpenGL context current
- Check that your GPU supports OpenGL 3.3+
- Update graphics drivers

### Issue 2: "Undefined reference to glXXX"

**Cause**: Missing OpenGL library linking.

**Solution**:
```cmake
# In CMakeLists.txt
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
```

### Issue 3: GLFW Window Creation Fails

**Cause**: Incompatible OpenGL version requested.

**Solution**:
```cpp
// Try requesting OpenGL 3.0 instead
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

// Check for errors
GLenum error = glGetError();
if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << error << std::endl;
}
```

### Issue 4: Black Screen

**Cause**: Not clearing the framebuffer or not swapping buffers.

**Solution**:
```cpp
glClear(GL_COLOR_BUFFER_BIT);  // Clear before rendering
glfwSwapBuffers(window);       // Present after rendering
```

### Issue 5: CMake Cannot Find GLFW

**Cause**: GLFW not installed or not in CMake search path.

**Solution**:
```bash
# Linux
sudo apt install libglfw3-dev

# Or specify path manually
cmake -DGLFW_DIR=/path/to/glfw/lib/cmake/glfw ..
```

## Best Practices

### 1. Always Check for Errors

```cpp
// OpenGL error checking function
void checkGLError(const char* location) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error at " << location << ": " << error << std::endl;
    }
}

// Use it after OpenGL calls
glClear(GL_COLOR_BUFFER_BIT);
checkGLError("glClear");
```

### 2. Use Modern OpenGL Only

Avoid deprecated functions like:
- `glBegin()` / `glEnd()`
- `glVertex3f()`
- `glMatrixMode()`
- `glLoadIdentity()`

Use modern equivalents:
- Vertex Buffer Objects (VBO)
- Vertex Array Objects (VAO)
- Shader programs
- GLM for mathematics

### 3. Organize Your Code

```cpp
// Separate concerns
class Shader { /* shader management */ };
class Mesh { /* geometry data */ };
class Renderer { /* rendering logic */ };

// Use RAII for resource management
class GLBuffer {
    GLuint id;
public:
    GLBuffer() { glGenBuffers(1, &id); }
    ~GLBuffer() { glDeleteBuffers(1, &id); }
    GLuint get() const { return id; }
};
```

### 4. Enable Debug Output (Debug Builds)

```cpp
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam) {
        std::cerr << "GL DEBUG: " << message << std::endl;
    }, nullptr);
#endif
```

### 5. Version Control for Shaders

Store shaders in separate files:
```
shaders/
├── vertex.glsl
└── fragment.glsl
```

Load them at runtime:
```cpp
std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
```

## Summary

In this lesson, you learned:

1. **What OpenGL is**: A cross-platform graphics API for 2D and 3D rendering
2. **OpenGL Evolution**: From fixed-function pipeline to modern programmable shaders
3. **Modern vs Legacy**: Why we use modern OpenGL (3.3+) with shaders and buffer objects
4. **OpenGL Architecture**: The graphics pipeline and key components
5. **Environment Setup**: Installing compilers, libraries (GLFW, GLAD, GLM)
6. **First OpenGL Program**: Creating a window and render loop
7. **Troubleshooting**: Common issues and their solutions
8. **Best Practices**: Error checking, code organization, and modern techniques

### What's Next?

In **Lesson 22**, we'll dive deeper into:
- Creating and configuring OpenGL contexts
- Understanding GLFW window management
- Handling window events and input
- Setting up callbacks for resize, keyboard, and mouse
- Creating multiple windows and contexts

### Practice Exercises

1. **Modify Background Color**: Change the clear color to different values and observe the results
2. **Add Window Resizing**: Implement proper viewport resizing when the window changes size
3. **Print GPU Info**: Display additional OpenGL information like maximum texture size
4. **Multiple Windows**: Try creating two GLFW windows simultaneously
5. **Error Checking**: Add comprehensive error checking to all OpenGL calls

### Additional Resources

- **Official OpenGL Documentation**: https://www.opengl.org/documentation/
- **LearnOpenGL**: https://learnopengl.com/
- **OpenGL SuperBible**: Comprehensive book on modern OpenGL
- **Khronos OpenGL Registry**: https://registry.khronos.org/OpenGL/
- **GLFW Documentation**: https://www.glfw.org/documentation.html

Congratulations on setting up your OpenGL development environment! You're now ready to start creating stunning 3D graphics applications.
