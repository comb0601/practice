# Lesson 24: Vertex Buffers and Vertex Arrays (VBO/VAO)

## Table of Contents
1. [Understanding GPU Memory](#understanding-gpu-memory)
2. [Vertex Buffer Objects (VBO)](#vertex-buffer-objects-vbo)
3. [Vertex Array Objects (VAO)](#vertex-array-objects-vao)
4. [Element Buffer Objects (EBO)](#element-buffer-objects-ebo)
5. [Vertex Attribute Configuration](#vertex-attribute-configuration)
6. [Buffer Usage Patterns](#buffer-usage-patterns)
7. [Complete Buffer Management](#complete-buffer-management)
8. [Advanced Buffer Techniques](#advanced-buffer-techniques)
9. [Performance Optimization](#performance-optimization)
10. [Best Practices](#best-practices)

## Understanding GPU Memory

### CPU vs GPU Memory

**CPU Memory (RAM)**:
- General-purpose storage
- Accessible by CPU
- Slower for GPU to access

**GPU Memory (VRAM)**:
- Specialized for graphics
- Fast access by GPU
- Limited capacity

### Why Buffer Objects?

**Problem with Immediate Mode** (Legacy OpenGL):
```cpp
// BAD: Sends data every frame
glBegin(GL_TRIANGLES);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
glEnd();  // Terrible performance!
```

**Solution with Buffer Objects**:
```cpp
// GOOD: Send once, draw many times
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// Later...
glDrawArrays(GL_TRIANGLES, 0, 3);  // Fast!
```

## Vertex Buffer Objects (VBO)

### What is a VBO?

A Vertex Buffer Object is a memory buffer stored on the GPU that holds vertex data (positions, colors, normals, texture coordinates, etc.).

### Creating and Using VBOs

```cpp
// 1. Generate buffer ID
GLuint VBO;
glGenBuffers(1, &VBO);

// 2. Bind buffer (make it active)
glBindBuffer(GL_ARRAY_BUFFER, VBO);

// 3. Copy data to GPU
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 4. Configure vertex attributes (covered later)
// 5. Draw
glDrawArrays(GL_TRIANGLES, 0, 3);

// 6. Cleanup
glDeleteBuffers(1, &VBO);
```

### Buffer Types

**GL_ARRAY_BUFFER**: Vertex attributes (positions, colors, etc.)
```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

**GL_ELEMENT_ARRAY_BUFFER**: Vertex indices
```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
```

**GL_UNIFORM_BUFFER**: Uniform data shared across shaders
```cpp
glBindBuffer(GL_UNIFORM_BUFFER, UBO);
```

**GL_TEXTURE_BUFFER**: Texture data
```cpp
glBindBuffer(GL_TEXTURE_BUFFER, TBO);
```

### Complete VBO Example

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Setup window and OpenGL context (omitted for brevity)

    // Vertex data: position (x, y, z) for each vertex
    float vertices[] = {
        // Triangle 1
        -0.8f, -0.5f, 0.0f,  // Bottom-left
        -0.2f, -0.5f, 0.0f,  // Bottom-right
        -0.5f,  0.5f, 0.0f,  // Top

        // Triangle 2
         0.2f, -0.5f, 0.0f,  // Bottom-left
         0.8f, -0.5f, 0.0f,  // Bottom-right
         0.5f,  0.5f, 0.0f   // Top
    };

    // Generate and bind VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure vertex attribute (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw both triangles (6 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteBuffers(1, &VBO);

    return 0;
}
```

## Vertex Array Objects (VAO)

### What is a VAO?

A Vertex Array Object stores all vertex attribute configurations. It remembers:
- Vertex attribute formats
- Which VBOs are used
- Which EBO is bound
- Enabled vertex attributes

**Analogy**: A VAO is like a "configuration preset" for vertex data.

### Why Use VAOs?

**Without VAO** (repetitive):
```cpp
// Setup for object 1
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glDrawArrays(GL_TRIANGLES, 0, 3);

// Setup for object 2
glBindBuffer(GL_ARRAY_BUFFER, VBO2);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glDrawArrays(GL_TRIANGLES, 0, 6);
```

**With VAO** (efficient):
```cpp
// Setup once
glBindVertexArray(VAO1);
// Configure attributes...
glBindVertexArray(VAO2);
// Configure attributes...

// Render
glBindVertexArray(VAO1);
glDrawArrays(GL_TRIANGLES, 0, 3);

glBindVertexArray(VAO2);
glDrawArrays(GL_TRIANGLES, 0, 6);
```

### Creating and Using VAOs

```cpp
// 1. Generate VAO
GLuint VAO;
glGenVertexArrays(1, &VAO);

// 2. Bind VAO (all subsequent configurations stored in it)
glBindVertexArray(VAO);

// 3. Create and configure VBO
GLuint VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 4. Configure vertex attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 5. Unbind VAO (optional, good practice)
glBindVertexArray(0);

// 6. Draw later
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);

// 7. Cleanup
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
```

### Complete VAO Example

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialize GLFW and create window (omitted)

    // Triangle vertices
    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Square vertices
    float squareVertices[] = {
        -0.3f, -0.3f, 0.0f,
         0.3f, -0.3f, 0.0f,
         0.3f,  0.3f, 0.0f,
        -0.3f,  0.3f, 0.0f
    };

    // Create VAO and VBO for triangle
    GLuint triangleVAO, triangleVBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO and VBO for square
    GLuint squareVAO, squareVBO;
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);

    glBindVertexArray(squareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glBindVertexArray(triangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw square
        glBindVertexArray(squareVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);

    return 0;
}
```

## Element Buffer Objects (EBO)

### What is an EBO?

An Element Buffer Object (also called Index Buffer Object - IBO) stores indices that reference vertices in a VBO. This eliminates duplicate vertices.

### Why Use EBOs?

**Without EBO** (duplicate vertices):
```cpp
float vertices[] = {
    // First triangle
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f,  // Bottom-right
     0.5f,  0.5f, 0.0f,  // Top-right

    // Second triangle (2 vertices duplicated!)
    -0.5f, -0.5f, 0.0f,  // Bottom-left (duplicate)
     0.5f,  0.5f, 0.0f,  // Top-right (duplicate)
    -0.5f,  0.5f, 0.0f   // Top-left
};
// 6 vertices = 72 bytes
```

**With EBO** (indexed):
```cpp
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // 0: Bottom-left
     0.5f, -0.5f, 0.0f,  // 1: Bottom-right
     0.5f,  0.5f, 0.0f,  // 2: Top-right
    -0.5f,  0.5f, 0.0f   // 3: Top-left
};
// 4 vertices = 48 bytes

unsigned int indices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};
// 6 indices = 24 bytes
// Total: 72 bytes (same), but with complex meshes: huge savings!
```

### Creating and Using EBOs

```cpp
// Vertex data
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // 0
     0.5f, -0.5f, 0.0f,  // 1
     0.5f,  0.5f, 0.0f,  // 2
    -0.5f,  0.5f, 0.0f   // 3
};

// Index data
unsigned int indices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};

// Create VAO, VBO, EBO
GLuint VAO, VBO, EBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

// Bind VAO
glBindVertexArray(VAO);

// Setup VBO
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Setup EBO
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// Configure vertex attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Unbind VAO
glBindVertexArray(0);

// Draw with indices
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// Cleanup
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
```

### Complete EBO Example

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Setup (omitted for brevity)

    // Cube vertices (8 unique vertices instead of 36)
    float vertices[] = {
        // Positions
        -0.5f, -0.5f, -0.5f,  // 0
         0.5f, -0.5f, -0.5f,  // 1
         0.5f,  0.5f, -0.5f,  // 2
        -0.5f,  0.5f, -0.5f,  // 3
        -0.5f, -0.5f,  0.5f,  // 4
         0.5f, -0.5f,  0.5f,  // 5
         0.5f,  0.5f,  0.5f,  // 6
        -0.5f,  0.5f,  0.5f   // 7
    };

    // Cube indices (12 triangles = 36 indices)
    unsigned int indices[] = {
        // Front face
        4, 5, 6,  4, 6, 7,
        // Back face
        1, 0, 3,  1, 3, 2,
        // Left face
        0, 4, 7,  0, 7, 3,
        // Right face
        5, 1, 2,  5, 2, 6,
        // Top face
        7, 6, 2,  7, 2, 3,
        // Bottom face
        0, 1, 5,  0, 5, 4
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}
```

## Vertex Attribute Configuration

### Understanding glVertexAttribPointer

```cpp
glVertexAttribPointer(
    GLuint index,          // Attribute location (0, 1, 2, ...)
    GLint size,            // Number of components (1, 2, 3, or 4)
    GLenum type,           // Data type (GL_FLOAT, GL_INT, etc.)
    GLboolean normalized,  // Normalize fixed-point data?
    GLsizei stride,        // Bytes between consecutive attributes
    const void* pointer    // Offset of first component
);
```

### Interleaved Attributes

```cpp
// Vertex data: position (3) + color (3)
float vertices[] = {
    // Positions        // Colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Blue
};

GLuint VAO, VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Position attribute (location = 0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Color attribute (location = 1)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

### Multiple Attributes Example

```cpp
// Vertex structure
struct Vertex {
    float position[3];   // x, y, z
    float color[4];      // r, g, b, a
    float texCoord[2];   // u, v
    float normal[3];     // nx, ny, nz
};

Vertex vertices[] = {
    // Position            Color                 TexCoord    Normal
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}}
};

GLuint VAO, VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

GLsizei stride = sizeof(Vertex);

// Position (location = 0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));
glEnableVertexAttribArray(0);

// Color (location = 1)
glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, color));
glEnableVertexAttribArray(1);

// TexCoord (location = 2)
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoord));
glEnableVertexAttribArray(2);

// Normal (location = 3)
glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));
glEnableVertexAttribArray(3);
```

## Buffer Usage Patterns

### Usage Hints

**GL_STATIC_DRAW**: Data set once, used many times
```cpp
// Static geometry (terrain, buildings)
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
```

**GL_DYNAMIC_DRAW**: Data modified repeatedly, used many times
```cpp
// Animated characters, particles
glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
```

**GL_STREAM_DRAW**: Data modified once per frame, used few times
```cpp
// Temporary effects, debug visualization
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
```

### Updating Buffer Data

**Complete Update**:
```cpp
// Replace entire buffer
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(newData), newData, GL_DYNAMIC_DRAW);
```

**Partial Update**:
```cpp
// Update only part of buffer
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferSubData(GL_ARRAY_BUFFER, offset, size, newData);
```

**Mapping Buffer**:
```cpp
// Map buffer to CPU memory
glBindBuffer(GL_ARRAY_BUFFER, VBO);
float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

// Modify data
for (int i = 0; i < vertexCount * 3; i++) {
    ptr[i] = newValues[i];
}

// Unmap buffer
glUnmapBuffer(GL_ARRAY_BUFFER);
```

## Complete Buffer Management

### Buffer Manager Class

```cpp
#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>

class BufferManager {
public:
    struct Vertex {
        float position[3];
        float color[4];
        float texCoord[2];
        float normal[3];
    };

    BufferManager() : VAO(0), VBO(0), EBO(0), indexCount(0) {}

    ~BufferManager() {
        cleanup();
    }

    void setupMesh(const std::vector<Vertex>& vertices,
                   const std::vector<unsigned int>& indices) {
        this->indexCount = indices.size();

        // Generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind VAO
        glBindVertexArray(VAO);

        // Setup VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(Vertex),
                     vertices.data(),
                     GL_STATIC_DRAW);

        // Setup EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);

        // Configure vertex attributes
        GLsizei stride = sizeof(Vertex);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                             (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                             (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // TexCoord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                             (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        // Normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride,
                             (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        // Unbind VAO
        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void drawInstanced(unsigned int instanceCount) const {
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
        glBindVertexArray(0);
    }

    void updateVertices(const std::vector<Vertex>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                       vertices.size() * sizeof(Vertex),
                       vertices.data());
    }

    GLuint getVAO() const { return VAO; }
    GLuint getVBO() const { return VBO; }
    GLuint getEBO() const { return EBO; }

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;

    void cleanup() {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }
        if (VBO != 0) {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
            EBO = 0;
        }
    }
};

// Usage example
int main() {
    // Create mesh
    std::vector<BufferManager::Vertex> vertices = {
        // Position            Color                 TexCoord    Normal
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    BufferManager bufferMgr;
    bufferMgr.setupMesh(vertices, indices);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        bufferMgr.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
```

## Advanced Buffer Techniques

### Persistent Mapped Buffers (OpenGL 4.4+)

```cpp
// Create persistent buffer
GLuint buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);

GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
glBufferStorage(GL_ARRAY_BUFFER, bufferSize, nullptr, flags);

// Map buffer permanently
void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize, flags);

// Update data anytime
memcpy(ptr, newData, dataSize);

// No need to unmap!
```

### Buffer Orphaning

```cpp
// Technique to avoid GPU stalls
void updateBuffer(GLuint VBO, const void* data, size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Orphan old buffer
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    // Fill with new data
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}
```

### Multi-Draw Indirect

```cpp
struct DrawCommand {
    unsigned int vertexCount;
    unsigned int instanceCount;
    unsigned int firstVertex;
    unsigned int baseInstance;
};

DrawCommand commands[] = {
    {36, 100, 0, 0},    // Draw cube 100 times
    {18, 50, 36, 100}   // Draw pyramid 50 times
};

GLuint indirectBuffer;
glGenBuffers(1, &indirectBuffer);
glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(commands), commands, GL_STATIC_DRAW);

// Draw all in one call
glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, 2, 0);
```

## Performance Optimization

### 1. Minimize Buffer Binds

```cpp
// BAD
for (auto& obj : objects) {
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
}

// GOOD: Use single large buffer
GLuint sharedVBO;
// Pack all vertex data into single buffer
// Use glDrawArrays with different offsets
for (auto& obj : objects) {
    glDrawArrays(GL_TRIANGLES, obj.offset, obj.vertexCount);
}
```

### 2. Use Appropriate Storage

```cpp
// For static data
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

// For dynamic data
glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

// For streaming data (updated every frame)
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
```

### 3. Batch Similar Geometry

```cpp
// Combine multiple objects with same vertex format
std::vector<Vertex> batchedVertices;
std::vector<unsigned int> batchedIndices;

for (auto& mesh : meshes) {
    unsigned int baseVertex = batchedVertices.size();

    batchedVertices.insert(batchedVertices.end(),
                          mesh.vertices.begin(),
                          mesh.vertices.end());

    for (auto index : mesh.indices) {
        batchedIndices.push_back(index + baseVertex);
    }
}

// Single draw call for all meshes
glDrawElements(GL_TRIANGLES, batchedIndices.size(), GL_UNSIGNED_INT, 0);
```

## Best Practices

### 1. Always Use VAOs

```cpp
// REQUIRED in OpenGL Core Profile
GLuint VAO;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);
// Configure attributes...
```

### 2. Cleanup Resources

```cpp
// Use RAII pattern
class Mesh {
    GLuint VAO, VBO, EBO;
public:
    Mesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};
```

### 3. Validate Buffer Setup

```cpp
void validateBufferSetup(GLuint VAO) {
    glBindVertexArray(VAO);

    // Check enabled attributes
    for (int i = 0; i < 16; i++) {
        GLint enabled;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);

        if (enabled) {
            GLint size, stride;
            GLenum type;
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*)&type);
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);

            std::cout << "Attribute " << i << ": size=" << size
                      << ", type=" << type << ", stride=" << stride << std::endl;
        }
    }
}
```

### 4. Use Indexed Drawing

```cpp
// Always use indices for complex meshes
// Saves memory and improves cache coherency
glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
```

### 5. Interleave Attributes

```cpp
// GOOD: Better cache performance
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

// AVOID: Separate arrays (worse cache performance)
float positions[1000];
float normals[1000];
float texCoords[1000];
```

## Summary

In this lesson, you learned:

1. **GPU Memory**: Difference between CPU and GPU memory, importance of buffer objects
2. **VBOs**: Creating, binding, and uploading vertex data to GPU
3. **VAOs**: Storing vertex attribute configurations for easy switching
4. **EBOs**: Eliminating duplicate vertices with indexed drawing
5. **Vertex Attributes**: Configuring position, color, texture coordinates, and normals
6. **Buffer Usage**: Static, dynamic, and streaming buffer patterns
7. **Buffer Management**: Complete buffer manager class implementation
8. **Advanced Techniques**: Persistent mapping, buffer orphaning, multi-draw indirect
9. **Optimization**: Minimizing binds, batching geometry, using appropriate storage
10. **Best Practices**: VAO usage, resource cleanup, validation, and interleaving

### What's Next?

In **Lesson 25**, we'll put everything together to:
- Draw your first triangle with complete shader setup
- Combine VBO, VAO, and shaders
- Create colored triangles
- Implement error checking
- Build a complete rendering pipeline

### Practice Exercises

1. **Multiple Shapes**: Create VAOs for triangle, square, pentagon, and hexagon
2. **Dynamic Updates**: Implement animated vertices with buffer updates
3. **Memory Profiling**: Measure memory savings with EBO vs without
4. **Attribute Layouts**: Create vertex format with 5+ attributes
5. **Buffer Manager**: Extend the BufferManager class with more features

### Additional Resources

- **OpenGL Buffer Objects**: https://www.khronos.org/opengl/wiki/Buffer_Object
- **Vertex Specification**: https://www.khronos.org/opengl/wiki/Vertex_Specification
- **OpenGL Performance**: https://www.khronos.org/opengl/wiki/Performance
- **LearnOpenGL - Hello Triangle**: https://learnopengl.com/Getting-started/Hello-Triangle

Congratulations! You now understand how to efficiently manage vertex data on the GPU. This is the foundation for all OpenGL rendering!
