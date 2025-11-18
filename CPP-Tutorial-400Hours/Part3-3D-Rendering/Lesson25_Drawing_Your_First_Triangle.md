# Lesson 25: Drawing Your First Triangle

## Table of Contents
1. [Overview](#overview)
2. [Prerequisites](#prerequisites)
3. [Complete Triangle Rendering Pipeline](#complete-triangle-rendering-pipeline)
4. [Step-by-Step Implementation](#step-by-step-implementation)
5. [Shader Code](#shader-code)
6. [Colored Triangle](#colored-triangle)
7. [Multiple Triangles](#multiple-triangles)
8. [Error Handling and Debugging](#error-handling-and-debugging)
9. [Common Issues and Solutions](#common-issues-and-solutions)
10. [Next Steps](#next-steps)

## Overview

Drawing a triangle is the "Hello World" of graphics programming. This lesson combines everything from previous lessons:
- Window creation with GLFW
- OpenGL context setup
- Vertex buffer objects (VBO)
- Vertex array objects (VAO)
- Shader compilation and linking
- Rendering pipeline

### What We'll Build

A complete application that renders a colorful triangle on screen with:
- Modern OpenGL 3.3+ core profile
- Proper resource management
- Error checking
- Clean, reusable code structure

## Prerequisites

Ensure you have:
- GLFW 3.3+
- GLAD (OpenGL 3.3 core profile)
- GLM (OpenGL Mathematics)
- C++17 compiler
- Basic understanding of previous lessons

## Complete Triangle Rendering Pipeline

```cpp
/*
 * complete_triangle.cpp
 * A complete, working example of rendering a triangle in modern OpenGL
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Framebuffer size callback
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Compile shader helper function
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }

    return shader;
}

// Create shader program
GLuint createShaderProgram(const char* vertSource, const char* fragSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSource);

    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Delete shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // ==============================
    // 1. Initialize GLFW
    // ==============================
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

    // ==============================
    // 2. Create Window
    // ==============================
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "My First Triangle",
        nullptr,
        nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // ==============================
    // 3. Load OpenGL Functions
    // ==============================
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // ==============================
    // 4. Create Shader Program
    // ==============================
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        glfwTerminate();
        return -1;
    }

    // ==============================
    // 5. Set Up Vertex Data
    // ==============================
    float vertices[] = {
        // Positions         // Colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom-left (red)
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right (green)
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top (blue)
    };

    // ==============================
    // 6. Create VAO, VBO
    // ==============================
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure vertex attributes

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO (optional, good practice)
    glBindVertexArray(0);

    // ==============================
    // 7. Render Loop
    // ==============================
    std::cout << "\nRendering triangle..." << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ==============================
    // 8. Cleanup
    // ==============================
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "\nProgram exited successfully" << std::endl;
    return 0;
}
```

## Step-by-Step Implementation

### Step 1: Initialize GLFW and Create Window

```cpp
// Initialize GLFW
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// Create window
GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
glfwMakeContextCurrent(window);
```

**Why**: Creates the rendering context and window where our triangle will appear.

### Step 2: Load OpenGL Functions with GLAD

```cpp
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
}
```

**Why**: GLAD loads all OpenGL function pointers so we can call OpenGL functions.

### Step 3: Create and Compile Shaders

```cpp
// Vertex shader
GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
glCompileShader(vertexShader);

// Fragment shader
GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
glCompileShader(fragmentShader);

// Link into program
GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

**Why**: Shaders tell the GPU how to process vertices and color pixels.

### Step 4: Define Vertex Data

```cpp
float vertices[] = {
    // Positions         // Colors
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Green
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Blue
};
```

**Why**: Defines the shape (positions) and appearance (colors) of our triangle.

### Step 5: Create VBO and VAO

```cpp
GLuint VAO, VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

**Why**: Transfers vertex data to GPU memory for fast rendering.

### Step 6: Configure Vertex Attributes

```cpp
// Position (location = 0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Color (location = 1)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

**Why**: Tells OpenGL how to interpret the vertex data.

### Step 7: Render Loop

```cpp
while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

**Why**: Continuously renders the triangle until the window is closed.

### Step 8: Cleanup

```cpp
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);
glfwTerminate();
```

**Why**: Frees GPU resources and terminates GLFW.

## Shader Code

### Vertex Shader Explained

```glsl
#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;    // Position from VBO
layout (location = 1) in vec3 aColor;  // Color from VBO

// Output to fragment shader
out vec3 vertexColor;

void main() {
    // Set vertex position (required)
    gl_Position = vec4(aPos, 1.0);

    // Pass color to fragment shader
    vertexColor = aColor;
}
```

**Explanation**:
- `layout (location = 0)`: Matches `glVertexAttribPointer(0, ...)`
- `gl_Position`: Required output, vertex position in clip space
- `out vertexColor`: Passed to fragment shader (interpolated)

### Fragment Shader Explained

```glsl
#version 330 core

// Input from vertex shader (interpolated)
in vec3 vertexColor;

// Output color for this fragment
out vec4 FragColor;

void main() {
    // Set fragment color (RGBA)
    FragColor = vec4(vertexColor, 1.0);
}
```

**Explanation**:
- `in vertexColor`: Receives interpolated color from vertex shader
- `FragColor`: Final color written to framebuffer
- `vec4(..., 1.0)`: RGB + Alpha (1.0 = fully opaque)

## Colored Triangle

### Rainbow Gradient Triangle

```cpp
float vertices[] = {
    // Positions         // Colors (RGB)
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Green
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Blue
};
```

The GPU automatically interpolates colors between vertices!

### Single Color Triangle

Vertex shader:
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
```

Fragment shader:
```glsl
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // Orange
}
```

### Uniform Color Triangle

Vertex shader (same as single color).

Fragment shader:
```glsl
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;  // Set from C++ code

void main() {
    FragColor = ourColor;
}
```

C++ code:
```cpp
// Get uniform location
GLint colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

// Set color (must be done after glUseProgram)
glUseProgram(shaderProgram);
glUniform4f(colorLoc, 1.0f, 0.5f, 0.2f, 1.0f);
```

### Animated Color Triangle

```cpp
// In render loop
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
GLint colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

glUseProgram(shaderProgram);
glUniform4f(colorLoc, 0.0f, greenValue, 0.0f, 1.0f);
```

## Multiple Triangles

### Two Separate Triangles

```cpp
float vertices[] = {
    // First triangle
    -0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.2f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

    // Second triangle
     0.2f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
     0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f
};

// Draw both triangles
glDrawArrays(GL_TRIANGLES, 0, 6);  // 6 vertices = 2 triangles
```

### Triangle Strip (Connected Triangles)

```cpp
float vertices[] = {
    -0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.4f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.4f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
     0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f
};

// Draw as triangle strip (5 vertices = 3 triangles)
glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
```

### Multiple VAOs (Different Shapes)

```cpp
// Triangle VAO
GLuint triangleVAO, triangleVBO;
glGenVertexArrays(1, &triangleVAO);
glGenBuffers(1, &triangleVBO);
// ... setup triangle ...

// Square VAO
GLuint squareVAO, squareVBO;
glGenVertexArrays(1, &squareVAO);
glGenBuffers(1, &squareVBO);
// ... setup square ...

// Render loop
while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Draw triangle
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw square
    glBindVertexArray(squareVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

## Error Handling and Debugging

### Comprehensive Error Checking

```cpp
#include <iostream>
#include <string>

void checkGLError(const std::string& location) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::string errorString;
        switch (error) {
            case GL_INVALID_ENUM:      errorString = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     errorString = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: errorString = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:     errorString = "GL_OUT_OF_MEMORY"; break;
            default:                   errorString = "UNKNOWN_ERROR"; break;
        }
        std::cerr << "OpenGL Error at " << location << ": " << errorString << std::endl;
    }
}

// Usage
glDrawArrays(GL_TRIANGLES, 0, 3);
checkGLError("glDrawArrays");
```

### Shader Compilation Error Checking

```cpp
GLuint compileShaderWithErrorCheck(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check compilation
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
```

### Debug Output Callback (OpenGL 4.3+)

```cpp
void GLAPIENTRY messageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;  // Ignore notifications

    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
              << " type = 0x" << type
              << ", severity = 0x" << severity
              << ", message = " << message << std::endl;
}

// Enable debug output
glEnable(GL_DEBUG_OUTPUT);
glDebugMessageCallback(messageCallback, 0);
```

## Common Issues and Solutions

### Issue 1: Black Screen

**Possible Causes**:
1. Shaders not compiled/linked correctly
2. VAO not bound before drawing
3. Vertices outside clip space [-1, 1]
4. Depth testing enabled without clearing depth buffer

**Solutions**:
```cpp
// Check shader compilation
GLint success;
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) { /* handle error */ }

// Always bind VAO before drawing
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);

// Ensure vertices in range [-1, 1]
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // OK
     1.5f,  0.5f, 0.0f,  // OUTSIDE! Won't be visible
};

// If using depth testing
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

### Issue 2: Triangle Not Visible

**Check**:
```cpp
// Winding order (counter-clockwise for front-facing)
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // 1
     0.5f, -0.5f, 0.0f,  // 2
     0.0f,  0.5f, 0.0f   // 3 (CCW: correct)
};

// Face culling disabled (for testing)
glDisable(GL_CULL_FACE);

// Clear color different from triangle color
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
```

### Issue 3: Colors Not Appearing

**Check**:
```cpp
// Attribute enabled
glEnableVertexAttribArray(1);  // Color attribute

// Correct stride and offset
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                     6 * sizeof(float),           // Stride: 6 floats
                     (void*)(3 * sizeof(float))); // Offset: skip position

// Shader receives color
// Vertex shader:
layout (location = 1) in vec3 aColor;  // Must match attribute location
out vec3 vertexColor;
vertexColor = aColor;  // Pass to fragment shader

// Fragment shader:
in vec3 vertexColor;  // Must match vertex shader output
FragColor = vec4(vertexColor, 1.0);
```

### Issue 4: Crash or Segfault

**Check**:
```cpp
// GLAD initialized after context creation
glfwMakeContextCurrent(window);
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);  // AFTER context

// Valid shader program
if (shaderProgram == 0) {
    std::cerr << "Failed to create shader program!" << std::endl;
    return -1;
}

// Buffers created before use
glGenBuffers(1, &VBO);  // Create first
glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Then bind
```

## Next Steps

### Enhancements to Try

1. **Different Shapes**: Square, pentagon, hexagon
2. **Wireframe Mode**: `glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);`
3. **Point Sprites**: `glDrawArrays(GL_POINTS, 0, 3);` and `glPointSize(10.0f);`
4. **Animation**: Rotate or move triangle using uniforms
5. **Multiple Colors**: Different shaders for different triangles

### Example: Wireframe Toggle

```cpp
bool wireframe = false;

// In input processing
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    wireframe = !wireframe;
}

// In render loop
if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
} else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
```

### Example: Rotating Triangle

```cpp
// Vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float rotation;  // Rotation angle

void main() {
    // Simple 2D rotation
    float s = sin(rotation);
    float c = cos(rotation);
    mat2 rotMat = mat2(c, -s, s, c);

    vec2 rotatedPos = rotMat * aPos.xy;
    gl_Position = vec4(rotatedPos, aPos.z, 1.0);
    vertexColor = aColor;
}
```

```cpp
// C++ code
float rotation = 0.0f;

// In render loop
rotation += 0.01f;
GLint rotLoc = glGetUniformLocation(shaderProgram, "rotation");
glUniform1f(rotLoc, rotation);
```

## Summary

Congratulations! You've successfully rendered your first triangle in modern OpenGL. You learned:

1. **Complete Pipeline**: From window creation to rendering
2. **Shader Compilation**: Creating and linking vertex and fragment shaders
3. **Vertex Data**: Setting up positions and colors
4. **Buffer Management**: VBO and VAO creation and configuration
5. **Rendering**: Drawing triangles with `glDrawArrays`
6. **Error Handling**: Debugging shaders and OpenGL errors
7. **Common Issues**: Solutions to frequent problems
8. **Next Steps**: Enhancements and animations

### What's Next?

In **Lesson 26**, we'll dive deep into:
- GLSL shader language syntax
- Data types and operators
- Built-in variables and functions
- Shader communication (in/out variables)
- Uniforms and attributes in detail

### Practice Exercises

1. **Three Triangles**: Draw three triangles with different colors
2. **Pentagon**: Create a pentagon using `GL_TRIANGLE_FAN`
3. **Color Cycling**: Animate colors using time-based uniforms
4. **User Input**: Change triangle color with keyboard input
5. **Multiple Shapes**: Draw triangle, square, and hexagon simultaneously

Congratulations on rendering your first triangle! This is a major milestone in graphics programming!
