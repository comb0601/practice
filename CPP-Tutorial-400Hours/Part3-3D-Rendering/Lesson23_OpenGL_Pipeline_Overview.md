# Lesson 23: OpenGL Pipeline Overview

## Table of Contents
1. [What is the Graphics Pipeline?](#what-is-the-graphics-pipeline)
2. [Pipeline Stages in Detail](#pipeline-stages-in-detail)
3. [Vertex Processing Stage](#vertex-processing-stage)
4. [Primitive Assembly](#primitive-assembly)
5. [Rasterization Stage](#rasterization-stage)
6. [Fragment Processing](#fragment-processing)
7. [Per-Fragment Operations](#per-fragment-operations)
8. [Pipeline Data Flow](#pipeline-data-flow)
9. [Complete Pipeline Example](#complete-pipeline-example)
10. [Pipeline Optimization](#pipeline-optimization)

## What is the Graphics Pipeline?

The graphics pipeline is a sequence of stages that transform 3D geometry into 2D pixels on your screen. It's called a "pipeline" because data flows through it in stages, with each stage performing specific operations.

### Pipeline Characteristics

**Parallel Processing**: Modern GPUs process thousands of vertices and pixels simultaneously.

**Fixed-Function and Programmable**: Some stages are programmable (shaders), others are fixed-function (configured but not programmed).

**State-Based**: Pipeline behavior is controlled by OpenGL state settings.

**High Throughput**: Designed for processing millions of triangles per frame.

### Pipeline Overview Diagram

```
Application (CPU)
    ↓
Vertex Specification
    ↓
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PROGRAMMABLE: Vertex Shader
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    ↓
Tessellation Control Shader (Optional)
    ↓
Tessellation Primitive Generation
    ↓
Tessellation Evaluation Shader (Optional)
    ↓
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PROGRAMMABLE: Geometry Shader (Optional)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    ↓
Primitive Assembly
    ↓
Clipping
    ↓
Face Culling
    ↓
Viewport Transform
    ↓
Rasterization
    ↓
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PROGRAMMABLE: Fragment Shader
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    ↓
Per-Fragment Operations
    ↓
Framebuffer
    ↓
Display
```

## Pipeline Stages in Detail

### 1. Vertex Specification (Application)

**Purpose**: Send vertex data from CPU to GPU.

**Data Provided**:
- Vertex positions
- Vertex colors
- Texture coordinates
- Normals
- Custom attributes

**OpenGL Objects Used**:
- Vertex Buffer Objects (VBO)
- Vertex Array Objects (VAO)
- Element Buffer Objects (EBO)

**Example**:
```cpp
// Vertex data
float vertices[] = {
    // Positions        // Colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Blue
};

GLuint VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

### 2. Vertex Shader (Programmable)

**Purpose**: Transform vertex positions and pass data to next stage.

**Operations**:
- Transform positions (Model-View-Projection)
- Calculate per-vertex lighting
- Pass attributes to fragment shader

**Input**:
```glsl
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
```

**Output**:
```glsl
out vec3 vertexColor;
gl_Position = vec4(aPos, 1.0);
```

**Example Vertex Shader**:
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
```

### 3. Tessellation (Optional)

**Tessellation Control Shader**: Controls tessellation level.

**Tessellation Evaluation Shader**: Generates new vertices.

**Use Cases**:
- Terrain rendering
- Displacement mapping
- Level-of-detail systems

We'll cover tessellation in advanced lessons.

### 4. Geometry Shader (Optional)

**Purpose**: Generate new primitives from existing ones.

**Capabilities**:
- Emit new vertices
- Change primitive type
- Discard primitives

**Use Cases**:
- Particle systems
- Procedural geometry
- Normal visualization

**Example**:
```glsl
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
```

### 5. Primitive Assembly

**Purpose**: Group vertices into primitives (triangles, lines, points).

**Primitive Types**:
- `GL_POINTS`: Individual points
- `GL_LINES`: Pairs of vertices as lines
- `GL_TRIANGLES`: Triplets of vertices as triangles
- `GL_TRIANGLE_STRIP`: Connected triangles
- `GL_TRIANGLE_FAN`: Triangles sharing first vertex

**Example**:
```cpp
// Draw triangles
glDrawArrays(GL_TRIANGLES, 0, 3);

// Draw triangle strip
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

// Draw with indices
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

### 6. Clipping

**Purpose**: Discard geometry outside the view frustum.

**Clip Space**: Coordinates in range [-w, w] for x, y, z.

**Automatic**: OpenGL handles this automatically.

**Custom Clip Distances**:
```glsl
// Vertex shader
gl_ClipDistance[0] = dot(worldPosition, clipPlane);
```

```cpp
// Enable clip distance
glEnable(GL_CLIP_DISTANCE0);
```

### 7. Face Culling

**Purpose**: Discard back-facing triangles.

**Benefits**:
- Reduces fragment processing by ~50%
- Improves performance for closed meshes

**Configuration**:
```cpp
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);      // Cull back faces (default)
glFrontFace(GL_CCW);      // Counter-clockwise is front (default)
```

**Winding Order**:
```cpp
// Counter-clockwise (front-facing)
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f,  // Bottom-right
     0.0f,  0.5f, 0.0f   // Top
};

// Clockwise (back-facing)
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.0f,  0.5f, 0.0f,  // Top
     0.5f, -0.5f, 0.0f   // Bottom-right
};
```

### 8. Viewport Transform

**Purpose**: Convert from NDC (Normalized Device Coordinates) to window coordinates.

**NDC Range**: [-1, 1] for x, y, z.

**Window Coordinates**: [0, width] and [0, height].

**Configuration**:
```cpp
glViewport(0, 0, 800, 600);  // x, y, width, height
```

**Depth Range**:
```cpp
glDepthRange(0.0, 1.0);  // Near, far (default)
```

### 9. Rasterization

**Purpose**: Convert primitives into fragments (potential pixels).

**Process**:
1. Determine which pixels are covered by primitive
2. Interpolate vertex attributes
3. Generate fragments for each covered pixel

**Interpolation**:
```
Vertex 1 (red)    Vertex 2 (green)
       ●─────────────●
        \           /
         \         /
          \       /
           \     /
            \   /  ← Fragments interpolated
             \ /      between red and green
              ●
         Vertex 3 (blue)
```

**Rasterization Rules**:
```cpp
// Point size
glPointSize(5.0f);

// Line width
glLineWidth(2.0f);

// Polygon mode
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Filled (default)
```

## Fragment Processing

### Fragment Shader (Programmable)

**Purpose**: Determine the final color of each fragment.

**Operations**:
- Texture sampling
- Lighting calculations
- Color computations
- Discard fragments

**Example Fragment Shader**:
```glsl
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

uniform sampler2D texture1;
in vec2 TexCoord;

void main() {
    // Simple color output
    FragColor = vec4(vertexColor, 1.0);

    // With texture
    // FragColor = texture(texture1, TexCoord);

    // Discard transparent fragments
    // if (FragColor.a < 0.1)
    //     discard;
}
```

**Built-in Variables**:
```glsl
in vec4 gl_FragCoord;     // Window-space coordinates
in bool gl_FrontFacing;   // Is front-facing triangle?
out float gl_FragDepth;   // Custom depth value
```

## Per-Fragment Operations

### Depth Testing

**Purpose**: Determine if fragment is visible (not occluded).

**Configuration**:
```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);  // Pass if fragment depth < buffer depth
```

**Depth Functions**:
- `GL_ALWAYS`: Always pass
- `GL_NEVER`: Never pass
- `GL_LESS`: Pass if less than buffer (default)
- `GL_LEQUAL`: Pass if less than or equal
- `GL_GREATER`: Pass if greater than buffer
- `GL_GEQUAL`: Pass if greater than or equal
- `GL_EQUAL`: Pass if equal to buffer
- `GL_NOTEQUAL`: Pass if not equal to buffer

### Stencil Testing

**Purpose**: Mask rendering to specific screen regions.

**Configuration**:
```cpp
glEnable(GL_STENCIL_TEST);
glStencilFunc(GL_EQUAL, 1, 0xFF);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
```

**Use Cases**:
- Reflections
- Shadows
- Outlining objects
- Portals

### Blending

**Purpose**: Combine fragment color with framebuffer color.

**Configuration**:
```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

**Blend Equation**:
```
finalColor = srcColor * srcFactor + dstColor * dstFactor
```

**Common Blend Modes**:
```cpp
// Alpha blending (transparency)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Additive blending (lights, particles)
glBlendFunc(GL_ONE, GL_ONE);

// Multiplicative blending (shadows)
glBlendFunc(GL_DST_COLOR, GL_ZERO);
```

### Framebuffer Operations

**Color Masking**:
```cpp
// Disable writing to color channels
glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
// Enable writing to all channels
glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
```

**Depth Masking**:
```cpp
// Disable depth writing
glDepthMask(GL_FALSE);
// Enable depth writing
glDepthMask(GL_TRUE);
```

## Pipeline Data Flow

### Complete Data Flow Example

```cpp
// complete_pipeline.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Vertex shader source
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

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Pipeline Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ============================================
    // STAGE 1: Vertex Specification
    // ============================================
    float vertices[] = {
        // Positions        // Colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left (red)
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right (green)
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Top (blue)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ============================================
    // STAGE 2: Compile Shaders
    // ============================================

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }

    // Link shaders into program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ============================================
    // STAGE 3: Configure Pipeline
    // ============================================

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Set viewport
    glViewport(0, 0, 800, 600);

    // ============================================
    // RENDER LOOP
    // ============================================
    std::cout << "\nPipeline Stages Demonstration:" << std::endl;
    std::cout << "1. Vertex data sent to GPU (VBO)" << std::endl;
    std::cout << "2. Vertex shader transforms each vertex" << std::endl;
    std::cout << "3. Primitive assembly creates triangle" << std::endl;
    std::cout << "4. Rasterizer generates fragments" << std::endl;
    std::cout << "5. Fragment shader colors each pixel" << std::endl;
    std::cout << "6. Depth test and blending" << std::endl;
    std::cout << "7. Final image displayed\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Clear framebuffer
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Bind VAO
        glBindVertexArray(VAO);

        // Draw triangle (triggers pipeline)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
```

### Pipeline State Tracking

```cpp
// pipeline_state.cpp
#include <glad/glad.h>
#include <iostream>
#include <string>

class PipelineState {
public:
    static void printCurrentState() {
        std::cout << "\n=== Current Pipeline State ===" << std::endl;

        // Viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        std::cout << "Viewport: " << viewport[0] << ", " << viewport[1]
                  << ", " << viewport[2] << "x" << viewport[3] << std::endl;

        // Depth testing
        GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
        std::cout << "Depth Test: " << (depthTest ? "Enabled" : "Disabled") << std::endl;

        if (depthTest) {
            GLint depthFunc;
            glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
            std::cout << "  Depth Func: " << getDepthFuncName(depthFunc) << std::endl;
        }

        // Face culling
        GLboolean cullFace = glIsEnabled(GL_CULL_FACE);
        std::cout << "Face Culling: " << (cullFace ? "Enabled" : "Disabled") << std::endl;

        if (cullFace) {
            GLint cullFaceMode, frontFace;
            glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
            glGetIntegerv(GL_FRONT_FACE, &frontFace);
            std::cout << "  Cull Mode: " << getCullModeName(cullFaceMode) << std::endl;
            std::cout << "  Front Face: " << (frontFace == GL_CCW ? "CCW" : "CW") << std::endl;
        }

        // Blending
        GLboolean blend = glIsEnabled(GL_BLEND);
        std::cout << "Blending: " << (blend ? "Enabled" : "Disabled") << std::endl;

        if (blend) {
            GLint srcRGB, dstRGB, srcAlpha, dstAlpha;
            glGetIntegerv(GL_BLEND_SRC_RGB, &srcRGB);
            glGetIntegerv(GL_BLEND_DST_RGB, &dstRGB);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &srcAlpha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, &dstAlpha);
            std::cout << "  RGB: " << getBlendFactorName(srcRGB)
                      << " -> " << getBlendFactorName(dstRGB) << std::endl;
        }

        // Active program
        GLint program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
        std::cout << "Active Shader Program: " << program << std::endl;

        // Polygon mode
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        std::cout << "Polygon Mode: " << getPolygonModeName(polygonMode[0]) << std::endl;

        std::cout << "==============================\n" << std::endl;
    }

private:
    static std::string getDepthFuncName(GLenum func) {
        switch (func) {
            case GL_NEVER: return "GL_NEVER";
            case GL_LESS: return "GL_LESS";
            case GL_EQUAL: return "GL_EQUAL";
            case GL_LEQUAL: return "GL_LEQUAL";
            case GL_GREATER: return "GL_GREATER";
            case GL_NOTEQUAL: return "GL_NOTEQUAL";
            case GL_GEQUAL: return "GL_GEQUAL";
            case GL_ALWAYS: return "GL_ALWAYS";
            default: return "UNKNOWN";
        }
    }

    static std::string getCullModeName(GLenum mode) {
        switch (mode) {
            case GL_FRONT: return "GL_FRONT";
            case GL_BACK: return "GL_BACK";
            case GL_FRONT_AND_BACK: return "GL_FRONT_AND_BACK";
            default: return "UNKNOWN";
        }
    }

    static std::string getBlendFactorName(GLenum factor) {
        switch (factor) {
            case GL_ZERO: return "GL_ZERO";
            case GL_ONE: return "GL_ONE";
            case GL_SRC_ALPHA: return "GL_SRC_ALPHA";
            case GL_ONE_MINUS_SRC_ALPHA: return "GL_ONE_MINUS_SRC_ALPHA";
            default: return "UNKNOWN";
        }
    }

    static std::string getPolygonModeName(GLenum mode) {
        switch (mode) {
            case GL_POINT: return "GL_POINT";
            case GL_LINE: return "GL_LINE";
            case GL_FILL: return "GL_FILL";
            default: return "UNKNOWN";
        }
    }
};
```

## Pipeline Optimization

### 1. Minimize State Changes

```cpp
// BAD: Too many state changes
for (auto& object : objects) {
    glUseProgram(object.shader);
    glBindTexture(GL_TEXTURE_2D, object.texture);
    object.draw();
}

// GOOD: Batch by state
std::sort(objects.begin(), objects.end(), [](const Object& a, const Object& b) {
    if (a.shader != b.shader) return a.shader < b.shader;
    return a.texture < b.texture;
});

GLuint currentShader = 0, currentTexture = 0;
for (auto& object : objects) {
    if (object.shader != currentShader) {
        glUseProgram(object.shader);
        currentShader = object.shader;
    }
    if (object.texture != currentTexture) {
        glBindTexture(GL_TEXTURE_2D, object.texture);
        currentTexture = object.texture;
    }
    object.draw();
}
```

### 2. Use Instanced Rendering

```cpp
// Instead of drawing each object separately
for (int i = 0; i < 1000; i++) {
    setModelMatrix(modelMatrices[i]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Use instanced rendering
glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000);
```

### 3. Optimize Fragment Processing

```cpp
// Fragment shader optimizations

// BAD: Complex calculations in fragment shader
void main() {
    vec3 result = vec3(0.0);
    for (int i = 0; i < 100; i++) {
        result += complexCalculation(i);
    }
    FragColor = vec4(result, 1.0);
}

// GOOD: Move to vertex shader or precompute
void main() {
    vec3 result = precomputedValue;  // Passed from vertex shader
    FragColor = vec4(result, 1.0);
}
```

### 4. Early Fragment Testing

```cpp
// Enable early depth testing
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);

// Draw opaque objects front-to-back
std::sort(opaqueObjects.begin(), opaqueObjects.end(),
    [&](const Object& a, const Object& b) {
        return distance(camera, a) < distance(camera, b);
    });

for (auto& obj : opaqueObjects) {
    obj.draw();
}
```

### 5. Use Appropriate Primitive Types

```cpp
// For a quad:

// INEFFICIENT: 6 vertices
float vertices[] = {
    // Triangle 1
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    // Triangle 2
    -0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
glDrawArrays(GL_TRIANGLES, 0, 6);

// EFFICIENT: 4 vertices with indices
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

## Summary

In this lesson, you learned:

1. **Graphics Pipeline**: The sequence of stages that transform 3D data to 2D pixels
2. **Programmable Stages**: Vertex shader, geometry shader, and fragment shader
3. **Fixed-Function Stages**: Primitive assembly, rasterization, and tests
4. **Vertex Processing**: Transforming positions and passing attributes
5. **Rasterization**: Converting primitives to fragments
6. **Fragment Processing**: Determining final pixel colors
7. **Per-Fragment Operations**: Depth testing, stencil testing, and blending
8. **Data Flow**: How data moves through the entire pipeline
9. **Pipeline State**: Configuring and querying OpenGL state
10. **Optimization**: Techniques to maximize pipeline efficiency

### What's Next?

In **Lesson 24**, we'll dive deep into:
- Vertex Buffer Objects (VBO) in detail
- Vertex Array Objects (VAO) configuration
- Element Buffer Objects (EBO) for indexed drawing
- Vertex attribute layouts
- Buffer management and best practices

### Practice Exercises

1. **Pipeline Visualization**: Create a diagram showing data flow for your own 3D model
2. **State Tracker**: Implement a complete pipeline state tracker class
3. **Performance Testing**: Compare drawing with and without face culling
4. **Custom Geometry**: Create different primitives (points, lines, triangles)
5. **Pipeline Stages**: Add debug output showing each pipeline stage execution

### Additional Resources

- **OpenGL Pipeline Documentation**: https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview
- **GPU Architecture**: https://developer.nvidia.com/gpugems/gpugems2/part-iv-general-purpose-computation-gpus-primer
- **Pipeline Performance**: https://www.khronos.org/opengl/wiki/Performance
- **OpenGL SuperBible**: Chapter on Graphics Pipeline

Congratulations! You now understand how OpenGL transforms your 3D data into beautiful rendered images. This knowledge is fundamental for all future OpenGL programming!
