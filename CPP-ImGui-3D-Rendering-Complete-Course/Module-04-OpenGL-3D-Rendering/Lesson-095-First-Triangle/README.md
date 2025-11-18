# Lesson 95: Drawing Your First Triangle
**Duration: 2.5 hours**
**Module 4: OpenGL & 3D Rendering**

## Learning Objectives
By the end of this lesson, you will:
- Understand the complete OpenGL rendering pipeline
- Create and compile vertex and fragment shaders
- Set up Vertex Buffer Objects (VBOs)
- Set up Vertex Array Objects (VAOs)
- Render your first triangle to the screen
- Understand vertex attributes and data layout
- Debug common rendering issues

## 1. The OpenGL Graphics Pipeline

```
Vertex Data
    ↓
Vertex Shader (programmable)
    ↓
Shape Assembly
    ↓
Geometry Shader (programmable, optional)
    ↓
Rasterization
    ↓
Fragment Shader (programmable)
    ↓
Tests and Blending
    ↓
Framebuffer
```

### What We'll Program Today

**Vertex Shader**: Processes each vertex
**Fragment Shader**: Determines each pixel's color

## 2. Complete Working Code

### main.cpp - Full Triangle Rendering

```cpp
// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // Orange color
}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "My First Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Build and compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders into a program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders (we don't need them anymore after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Triangle vertex data (in NDC - Normalized Device Coordinates)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Bottom left
         0.5f, -0.5f, 0.0f,  // Bottom right
         0.0f,  0.5f, 0.0f   // Top center
    };

    // Create VAO and VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first, then bind and set VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO (optional, good practice)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark teal background
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
```

## 3. Understanding Vertex Data

### NDC (Normalized Device Coordinates)

```cpp
// NDC space: -1.0 to +1.0 in all axes
//
//      +Y (1.0)
//         |
//         |
// -X -----+------ +X (1.0)
// (-1.0)  |
//         |
//      -Y (-1.0)

float vertices[] = {
    // x      y     z
    -0.5f, -0.5f, 0.0f,  // Bottom left
     0.5f, -0.5f, 0.0f,  // Bottom right
     0.0f,  0.5f, 0.0f   // Top center
};
```

### Vertex Data Layout

```
vertices array in memory:
┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
│ -0.5    │ -0.5    │  0.0    │  0.5    │ -0.5    │  0.0    │  0.0    │  0.5    │  0.0    │
└─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
  Vertex 0 (x,y,z)             Vertex 1 (x,y,z)             Vertex 2 (x,y,z)
```

## 4. Vertex Buffer Object (VBO)

### Creating and Binding VBO

```cpp
unsigned int VBO;
glGenBuffers(1, &VBO);  // Generate buffer ID

glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind to GL_ARRAY_BUFFER target

// Copy data to GPU
glBufferData(GL_ARRAY_BUFFER,      // Target
             sizeof(vertices),      // Size in bytes
             vertices,              // Pointer to data
             GL_STATIC_DRAW);       // Usage hint
```

### Usage Hints

```cpp
GL_STATIC_DRAW   // Data won't change (or rarely)
GL_DYNAMIC_DRAW  // Data changes occasionally
GL_STREAM_DRAW   // Data changes every frame
```

## 5. Vertex Array Object (VAO)

### Purpose of VAO

VAO stores the configuration of vertex attributes. Without VAO, you'd have to reconfigure attributes every time you draw!

```cpp
// Create VAO
unsigned int VAO;
glGenVertexArrays(1, &VAO);

// Bind VAO to capture subsequent vertex attribute calls
glBindVertexArray(VAO);

// Now any vertex attribute configuration is stored in VAO
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Later, just bind VAO to use the same configuration
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);  // All configuration already set!
```

## 6. Configuring Vertex Attributes

### glVertexAttribPointer Breakdown

```cpp
glVertexAttribPointer(
    0,                 // Attribute location (matches shader: layout(location = 0))
    3,                 // Number of components (3 for vec3: x, y, z)
    GL_FLOAT,          // Data type
    GL_FALSE,          // Normalize? (convert to 0-1 range)
    3 * sizeof(float), // Stride: bytes between consecutive vertices
    (void*)0           // Offset: where position data begins
);

glEnableVertexAttribArray(0);  // Enable attribute at location 0
```

### Visual Representation

```
Vertex data in buffer:
┌────────┬────────┬────────┐ ← 3 floats (12 bytes) = stride
│   X    │   Y    │   Z    │
└────────┴────────┴────────┘
    ↑
 offset = 0
```

### Example with Multiple Attributes

```cpp
struct Vertex {
    float position[3];  // 12 bytes
    float color[3];     // 12 bytes
    float texCoords[2]; // 8 bytes
};  // Total: 32 bytes

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
glEnableVertexAttribArray(0);

// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

// TexCoord attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(2);
```

## 7. Shaders in Detail

### Vertex Shader

```glsl
#version 330 core

// Input: vertex attribute at location 0
layout (location = 0) in vec3 aPos;

void main() {
    // gl_Position is a built-in output variable
    // Must be a vec4 (x, y, z, w)
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    // Shorthand:
    // gl_Position = vec4(aPos, 1.0);
}
```

### Fragment Shader

```glsl
#version 330 core

// Output: final fragment color
out vec4 FragColor;

void main() {
    // RGBA color (Red, Green, Blue, Alpha)
    // Values: 0.0 to 1.0
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // Orange
}
```

### Shader Compilation

```cpp
// Create shader object
unsigned int shader = glCreateShader(GL_VERTEX_SHADER);

// Attach source code
glShaderSource(shader, 1, &shaderSource, NULL);

// Compile
glCompileShader(shader);

// Check for errors
int success;
char infoLog[512];
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
}
```

### Shader Program Linking

```cpp
// Create program
unsigned int program = glCreateProgram();

// Attach shaders
glAttachShader(program, vertexShader);
glAttachShader(program, fragmentShader);

// Link
glLinkProgram(program);

// Check for errors
glGetProgramiv(program, GL_LINK_STATUS, &success);
if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "Linking failed:\n" << infoLog << std::endl;
}

// Use program
glUseProgram(program);

// Clean up individual shaders
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

## 8. Drawing Commands

### glDrawArrays

```cpp
glDrawArrays(
    GL_TRIANGLES,  // Primitive type
    0,             // Starting index
    3              // Number of vertices
);
```

### Primitive Types

```cpp
GL_POINTS         // Individual points
GL_LINES          // Pairs of vertices make lines
GL_LINE_STRIP     // Connected line segments
GL_LINE_LOOP      // Connected + closes to first vertex
GL_TRIANGLES      // Every 3 vertices make a triangle
GL_TRIANGLE_STRIP // Connected triangles
GL_TRIANGLE_FAN   // Triangles share first vertex
```

### Example: Multiple Triangles

```cpp
float vertices[] = {
    // Triangle 1
    -0.9f, -0.5f, 0.0f,
    -0.0f, -0.5f, 0.0f,
    -0.45f, 0.5f, 0.0f,

    // Triangle 2
     0.0f, -0.5f, 0.0f,
     0.9f, -0.5f, 0.0f,
     0.45f, 0.5f, 0.0f
};

// Draw both triangles
glDrawArrays(GL_TRIANGLES, 0, 6);  // 6 vertices = 2 triangles
```

## 9. Colored Triangle

### Vertex Data with Colors

```cpp
float vertices[] = {
    // Positions        // Colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Blue
};

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

### Updated Vertex Shader

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;  // Output to fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;  // Pass color to fragment shader
}
```

### Updated Fragment Shader

```glsl
#version 330 core
in vec3 vertexColor;   // Input from vertex shader
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
```

## 10. Common Issues and Solutions

### Issue 1: Nothing Renders

**Checklist:**
- [ ] VAO bound before drawing?
- [ ] Shader program active (`glUseProgram`)?
- [ ] Vertices in visible range (-1 to 1)?
- [ ] Background color different from triangle color?
- [ ] Depth test disabled for 2D? (`glDisable(GL_DEPTH_TEST)`)

### Issue 2: Shader Compilation Fails

```cpp
// Always check shader compilation!
int success;
char infoLog[512];
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR: " << infoLog << std::endl;
}
```

### Issue 3: Black Triangle

**Possible causes:**
- Fragment shader not outputting color
- Shader program not linked correctly
- Color values outside 0-1 range

### Issue 4: Triangle Upside Down

OpenGL's Y-axis points up, which might feel inverted. This is normal!

```cpp
// If needed, flip Y-axis in vertex shader
gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
```

## 11. Exercises

### Exercise 1: Different Shapes
Modify vertices to create:
- A square (2 triangles)
- A hexagon (6 triangles)
- A star (5 triangles)

### Exercise 2: Color Changes
- Make triangle green
- Make triangle semi-transparent (alpha < 1.0)
- Create a gradient triangle using vertex colors

### Exercise 3: Multiple Triangles
- Render 3 separate triangles
- Use different colors for each
- Position them in a pattern

### Exercise 4: Animation
- Move triangle horizontally using time
- Rotate triangle
- Pulse size larger/smaller

Hint for animation:
```cpp
float time = glfwGetTime();
float offset = sin(time) * 0.5f;  // Oscillate -0.5 to +0.5
```

## 12. Exercise Solutions

### Solution 1: Square

```cpp
float vertices[] = {
    // Triangle 1
    -0.5f, -0.5f, 0.0f,  // Bottom left
     0.5f, -0.5f, 0.0f,  // Bottom right
     0.5f,  0.5f, 0.0f,  // Top right

    // Triangle 2
     0.5f,  0.5f, 0.0f,  // Top right
    -0.5f,  0.5f, 0.0f,  // Top left
    -0.5f, -0.5f, 0.0f   // Bottom left
};

glDrawArrays(GL_TRIANGLES, 0, 6);
```

### Solution 2: Animated Triangle

```cpp
// In vertex shader, add uniform for time offset
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform float xOffset;

void main() {
    gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
}
)";

// In main loop
float time = glfwGetTime();
float offset = sin(time) * 0.5f;

int offsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
glUseProgram(shaderProgram);
glUniform1f(offsetLocation, offset);

glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

## Summary

### Key Concepts

✓ **VAO** stores vertex attribute configuration
✓ **VBO** stores actual vertex data on GPU
✓ **Vertex Shader** processes each vertex
✓ **Fragment Shader** determines pixel colors
✓ **Shaders** must be compiled and linked
✓ **glDrawArrays** executes the rendering

### Rendering Pipeline Summary

```
1. Create and bind VAO
2. Create and fill VBO with vertex data
3. Configure vertex attributes
4. Compile vertex and fragment shaders
5. Link shaders into program
6. In render loop:
   - Clear screen
   - Use shader program
   - Bind VAO
   - Draw with glDrawArrays
```

## Checklist

- [ ] Can render a triangle
- [ ] Understand VBO and VAO
- [ ] Can write simple shaders
- [ ] Know how to configure vertex attributes
- [ ] Can compile and link shaders
- [ ] Understand glDrawArrays
- [ ] Can debug rendering issues
- [ ] Completed exercises

---

**Time to complete**: 2.5 hours
**Previous lesson**: [Lesson 94 - VAOs (Vertex Array Objects)](../Lesson-094-VAOs/README.md)
**Next lesson**: [Lesson 96 - Element Buffer Objects (EBOs)](../Lesson-096-EBOs/README.md)

**Congratulations!** You've rendered your first OpenGL triangle - the "Hello World" of 3D graphics!
