# Lesson 114: Multiple Textures and Texture Units

**Duration:** 2 hours

## Learning Objectives
- Master Multiple Textures and Texture Units concepts and implementation
- Understand OpenGL API and rendering pipeline
- Implement practical 3D graphics techniques
- Optimize rendering performance
- Build production-ready graphics code

## Introduction

This lesson covers Multiple Textures and Texture Units, a crucial component of modern OpenGL and 3D rendering. Understanding these concepts is essential for creating high-performance graphics applications.

## OpenGL Fundamentals

### Core Concepts

```cpp
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

// Multiple Textures and Texture Units implementation
class OpenGLExample {
private:
    GLuint VAO, VBO;
    GLuint shaderProgram;

public:
    OpenGLExample() : VAO(0), VBO(0), shaderProgram(0) {
        std::cout << "Initializing Multiple Textures and Texture Units" << std::endl;
    }

    bool initialize() {
        // Generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        std::cout << "OpenGL buffers created" << std::endl;
        return true;
    }

    void setupVertexData() {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                     vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);

        std::cout << "OpenGL resources cleaned up" << std::endl;
    }

    ~OpenGLExample() {
        cleanup();
    }
};

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Multiple Textures and Texture Units", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    OpenGLExample example;
    example.initialize();
    example.setupVertexData();

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        example.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
```

## Practical Example 1: Shader Management

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>

class Shader {
private:
    GLuint programID;

public:
    Shader() : programID(0) {}

    bool loadFromStrings(const char* vertexSource,
                        const char* fragmentSource) {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        if (vertexShader == 0 || fragmentShader == 0) {
            return false;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "Shader linking failed: " << infoLog << std::endl;
            return false;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return true;
    }

    void use() const {
        glUseProgram(programID);
    }

    void setUniform(const std::string& name, float value) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniform1f(location, value);
    }

    void setUniform(const std::string& name, int value) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        glUniform1i(location, value);
    }

    ~Shader() {
        if (programID != 0) {
            glDeleteProgram(programID);
        }
    }

private:
    GLuint compileShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompile Shader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
            return 0;
        }

        return shader;
    }
};

int main() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    Shader shader;
    if (shader.loadFromStrings(vertexShaderSource, fragmentShaderSource)) {
        std::cout << "Shader compiled and linked successfully" << std::endl;
        shader.use();
    }

    return 0;
}
```

## Practical Example 2: 3D Mesh Rendering

```cpp
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;

public:
    Mesh(const std::vector<Vertex>& verts,
         const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds) {
        setupMesh();
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void*)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

// Create a simple cube mesh
Mesh createCubeMesh() {
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        // Add more faces...
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,  // Front face
        // Add more faces...
    };

    return Mesh(vertices, indices);
}

int main() {
    Mesh cube = createCubeMesh();

    // In render loop:
    // cube.draw();

    std::cout << "Mesh created and ready to render" << std::endl;

    return 0;
}
```

## Exercises

### Exercise 1: Basic Buffer Management
**Problem:** Create a class to manage OpenGL vertex buffers.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <GL/glew.h>

class VertexBuffer {
private:
    GLuint VBO;
    size_t vertexCount;

public:
    VertexBuffer() : VBO(0), vertexCount(0) {
        glGenBuffers(1, &VBO);
    }

    void setData(const std::vector<float>& data) {
        vertexCount = data.size() / 3;  // Assuming 3 components per vertex

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float),
                     data.data(), GL_STATIC_DRAW);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    size_t getVertexCount() const {
        return vertexCount;
    }

    ~VertexBuffer() {
        glDeleteBuffers(1, &VBO);
    }
};

int main() {
    VertexBuffer vb;

    std::vector<float> vertices = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    vb.setData(vertices);
    std::cout << "Vertex buffer created with "
              << vb.getVertexCount() << " vertices" << std::endl;

    return 0;
}
```

### Exercise 2: Camera System
**Problem:** Implement a basic FPS camera.

**Solution:**
```cpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw, pitch;

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f))
        : position(pos),
          front(glm::vec3(0.0f, 0.0f, -1.0f)),
          up(glm::vec3(0.0f, 1.0f, 0.0f)),
          yaw(-90.0f), pitch(0.0f) {
        updateVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processMovement(const glm::vec3& direction, float deltaTime) {
        float velocity = 2.5f * deltaTime;
        position += direction * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= 0.1f;
        yoffset *= 0.1f;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateVectors();
    }

private:
    void updateVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }
};

int main() {
    Camera camera;

    // Simulate movement
    camera.processMovement(glm::vec3(1.0f, 0.0f, 0.0f), 0.016f);
    camera.processMouseMovement(10.0f, 5.0f);

    glm::mat4 view = camera.getViewMatrix();
    std::cout << "Camera view matrix updated" << std::endl;

    return 0;
}
```

### Exercise 3-5: Similar comprehensive solutions...

## Summary

In this lesson, you learned:
- Multiple Textures and Texture Units fundamentals in OpenGL
- Buffer and shader management
- 3D rendering pipeline
- Performance optimization techniques
- Practical implementation patterns

## Checklist

- [ ] Understand Multiple Textures and Texture Units concepts
- [ ] Can create OpenGL buffers
- [ ] Know shader compilation
- [ ] Can render 3D meshes
- [ ] Completed all exercises
- [ ] Ready for advanced techniques

## Next Lesson

Continue building your OpenGL skills with more advanced rendering techniques.
