# Lesson 29: Shader Programs and Uniforms

## Table of Contents
1. [Shader Programs Overview](#shader-programs-overview)
2. [Creating and Compiling Shaders](#creating-and-compiling-shaders)
3. [Linking Shader Programs](#linking-shader-programs)
4. [Uniform Variables](#uniform-variables)
5. [Uniform Buffers (UBO)](#uniform-buffers-ubo)
6. [Shader Storage Buffers (SSBO)](#shader-storage-buffers-ssbo)
7. [Complete Shader Class](#complete-shader-class)
8. [Hot Reloading Shaders](#hot-reloading-shaders)
9. [Debugging Shaders](#debugging-shaders)
10. [Best Practices](#best-practices)

## Shader Programs Overview

A shader program combines multiple shader stages into a complete rendering pipeline.

### Shader Pipeline

```
Vertex Shader → Tessellation Control → Tessellation Evaluation →
Geometry Shader → Fragment Shader
        ↓
   Shader Program
```

### Minimum Requirement

```
Vertex Shader + Fragment Shader = Valid Shader Program
```

## Creating and Compiling Shaders

### Manual Shader Compilation

```cpp
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Load shader source from file
std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Compile shader
GLuint compileShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check compilation status
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string shaderTypeName;
        switch (shaderType) {
            case GL_VERTEX_SHADER: shaderTypeName = "VERTEX"; break;
            case GL_FRAGMENT_SHADER: shaderTypeName = "FRAGMENT"; break;
            case GL_GEOMETRY_SHADER: shaderTypeName = "GEOMETRY"; break;
            default: shaderTypeName = "UNKNOWN"; break;
        }
        std::cerr << "ERROR::SHADER::" << shaderTypeName << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Usage
int main() {
    // Load sources
    std::string vertSource = loadShaderSource("vertex.glsl");
    std::string fragSource = loadShaderSource("fragment.glsl");

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertSource.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSource.c_str());

    // ...link program (see next section)
}
```

## Linking Shader Programs

### Creating Shader Program

```cpp
GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();

    // Attach shaders
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link program
    glLinkProgram(program);

    // Check linking status
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }

    // Validate program (optional, for debugging)
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Usage
GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
glUseProgram(shaderProgram);
```

### Using Multiple Shader Programs

```cpp
GLuint basicShader = createShaderProgram(/* ... */);
GLuint lightingShader = createShaderProgram(/* ... */);
GLuint skyboxShader = createShaderProgram(/* ... */);

// Render loop
glUseProgram(basicShader);
// Draw objects with basic shader

glUseProgram(lightingShader);
// Draw objects with lighting

glUseProgram(skyboxShader);
// Draw skybox
```

## Uniform Variables

Uniforms are global variables that remain constant for all shader invocations within a draw call.

### Basic Uniform Types

```cpp
// C++ code
glUseProgram(shaderProgram);

// Float
GLint loc = glGetUniformLocation(shaderProgram, "time");
glUniform1f(loc, 3.14f);

// Integer
loc = glGetUniformLocation(shaderProgram, "textureSlot");
glUniform1i(loc, 0);

// Boolean (as int)
loc = glGetUniformLocation(shaderProgram, "enableLighting");
glUniform1i(loc, 1);

// Vec2
loc = glGetUniformLocation(shaderProgram, "resolution");
glUniform2f(loc, 800.0f, 600.0f);

// Vec3
loc = glGetUniformLocation(shaderProgram, "lightPos");
glUniform3f(loc, 1.0f, 2.0f, 3.0f);

// Vec4
loc = glGetUniformLocation(shaderProgram, "color");
glUniform4f(loc, 1.0f, 0.5f, 0.2f, 1.0f);
```

```glsl
// GLSL shader
#version 330 core

uniform float time;
uniform int textureSlot;
uniform bool enableLighting;
uniform vec2 resolution;
uniform vec3 lightPos;
uniform vec4 color;

void main() {
    // Use uniforms...
}
```

### Array Uniforms

```cpp
// C++ code
float values[3] = {1.0f, 2.0f, 3.0f};
GLint loc = glGetUniformLocation(shaderProgram, "values");
glUniform1fv(loc, 3, values);

// Vector array
glm::vec3 positions[5] = {/* ... */};
GLint loc = glGetUniformLocation(shaderProgram, "positions");
glUniform3fv(loc, 5, glm::value_ptr(positions[0]));
```

```glsl
// GLSL shader
uniform float values[3];
uniform vec3 positions[5];

void main() {
    float sum = 0.0;
    for (int i = 0; i < 3; i++) {
        sum += values[i];
    }

    vec3 totalPos = vec3(0.0);
    for (int i = 0; i < 5; i++) {
        totalPos += positions[i];
    }
}
```

### Matrix Uniforms

```cpp
// C++ code
glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

GLint loc = glGetUniformLocation(shaderProgram, "model");
glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

// mat3
glm::mat3 normalMatrix = glm::mat3(1.0f);
loc = glGetUniformLocation(shaderProgram, "normalMatrix");
glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
```

```glsl
// GLSL shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = normalMatrix * aNormal;
}
```

### Struct Uniforms

```cpp
// C++ code
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

Material material = {
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    32.0f
};

// Set individual members
GLint loc = glGetUniformLocation(shaderProgram, "material.ambient");
glUniform3fv(loc, 1, glm::value_ptr(material.ambient));

loc = glGetUniformLocation(shaderProgram, "material.diffuse");
glUniform3fv(loc, 1, glm::value_ptr(material.diffuse));

loc = glGetUniformLocation(shaderProgram, "material.specular");
glUniform3fv(loc, 1, glm::value_ptr(material.specular));

loc = glGetUniformLocation(shaderProgram, "material.shininess");
glUniform1f(loc, material.shininess);
```

```glsl
// GLSL shader
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main() {
    vec3 ambient = material.ambient * lightColor;
    // ... use material properties
}
```

## Uniform Buffers (UBO)

Uniform Buffer Objects allow sharing uniform data across multiple shader programs efficiently.

### Benefits of UBOs

- **Share uniforms** across multiple shaders
- **Reduce API calls** (set once, use many times)
- **Better performance** for large uniform data
- **Hardware optimization**

### Creating a UBO

```cpp
// Define uniform block in shader
```glsl
// Common uniforms shared across all shaders
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};
```

```cpp
// C++ code
GLuint UBO;
glGenBuffers(1, &UBO);

glBindBuffer(GL_UNIFORM_BUFFER, UBO);
glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
glBindBuffer(GL_UNIFORM_BUFFER, 0);

// Bind to binding point 0
GLuint bindingPoint = 0;
glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);

// Set binding point in shader program
GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "Matrices");
glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);

// Update UBO data
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
glm::mat4 view = camera.getViewMatrix();

glBindBuffer(GL_UNIFORM_BUFFER, UBO);
glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

### Complete UBO Example

```cpp
// ubo_example.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class UBOManager {
public:
    GLuint UBO;
    GLuint bindingPoint;

    UBOManager(GLuint binding = 0) : bindingPoint(binding) {
        glGenBuffers(1, &UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    ~UBOManager() {
        glDeleteBuffers(1, &UBO);
    }

    void updateMatrices(const glm::mat4& projection, const glm::mat4& view) {
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void bindToShader(GLuint shaderProgram, const char* blockName) {
        GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, blockName);
        glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);
    }
};

// Usage
UBOManager uboManager(0);

// Bind to all shaders
uboManager.bindToShader(shader1, "Matrices");
uboManager.bindToShader(shader2, "Matrices");
uboManager.bindToShader(shader3, "Matrices");

// Update once per frame
uboManager.updateMatrices(projection, view);

// All shaders now have updated projection and view matrices!
```

### UBO Memory Layout (std140)

```glsl
layout (std140) uniform ExampleBlock {
    float value;          // Offset 0,   Size 4
    vec3 vector;          // Offset 16,  Size 12 (aligned to 16)
    mat4 matrix;          // Offset 32,  Size 64
    float array[3];       // Offset 96,  Size 48 (each element aligned to 16)
    bool boolean;         // Offset 144, Size 4
    int integer;          // Offset 148, Size 4
};
```

## Shader Storage Buffers (SSBO)

SSBOs provide larger, more flexible storage than UBOs (OpenGL 4.3+).

### Creating an SSBO

```cpp
// C++ code
std::vector<glm::vec4> data(1000);

GLuint SSBO;
glGenBuffers(1, &SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_DYNAMIC_DRAW);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
```

```glsl
// GLSL shader
#version 430 core

layout(std430, binding = 0) buffer DataBuffer {
    vec4 data[];
};

void main() {
    // Read from SSBO
    vec4 value = data[gl_VertexID];

    // Write to SSBO
    data[gl_VertexID] = vec4(1.0);
}
```

### SSBO vs UBO

| Feature | UBO | SSBO |
|---------|-----|------|
| Max Size | ~64KB | GPU memory limit |
| Read/Write | Read-only in shader | Read-write |
| OpenGL Version | 3.1+ | 4.3+ |
| Performance | Faster for small data | Better for large data |

## Complete Shader Class

```cpp
// Shader.h
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader {
public:
    GLuint ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode = loadFile(vertexPath);
        std::string fragmentCode = loadFile(fragmentPath);

        GLuint vertex = compile(GL_VERTEX_SHADER, vertexCode.c_str(), "VERTEX");
        GLuint fragment = compile(GL_FRAGMENT_SHADER, fragmentCode.c_str(), "FRAGMENT");

        ID = link(vertex, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
        std::string vertexCode = loadFile(vertexPath);
        std::string fragmentCode = loadFile(fragmentPath);
        std::string geometryCode = loadFile(geometryPath);

        GLuint vertex = compile(GL_VERTEX_SHADER, vertexCode.c_str(), "VERTEX");
        GLuint fragment = compile(GL_FRAGMENT_SHADER, fragmentCode.c_str(), "FRAGMENT");
        GLuint geometry = compile(GL_GEOMETRY_SHADER, geometryCode.c_str(), "GEOMETRY");

        ID = link(vertex, fragment, geometry);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteShader(geometry);
    }

    ~Shader() {
        glDeleteProgram(ID);
    }

    void use() const {
        glUseProgram(ID);
    }

    // Utility functions
    void setBool(const std::string& name, bool value) {
        glUniform1i(getUniformLocation(name), static_cast<int>(value));
    }

    void setInt(const std::string& name, int value) {
        glUniform1i(getUniformLocation(name), value);
    }

    void setFloat(const std::string& name, float value) {
        glUniform1f(getUniformLocation(name), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
    }

    void setVec2(const std::string& name, float x, float y) {
        glUniform2f(getUniformLocation(name), x, y);
    }

    void setVec3(const std::string& name, const glm::vec3& value) {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
    }

    void setVec3(const std::string& name, float x, float y, float z) {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    void setVec4(const std::string& name, const glm::vec4& value) {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
    }

    void setVec4(const std::string& name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    void setMat2(const std::string& name, const glm::mat2& mat) {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat3(const std::string& name, const glm::mat3& mat) {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    std::unordered_map<std::string, GLint> uniformCache;

    GLint getUniformLocation(const std::string& name) {
        if (uniformCache.find(name) != uniformCache.end()) {
            return uniformCache[name];
        }

        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        }
        uniformCache[name] = location;
        return location;
    }

    std::string loadFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLuint compile(GLenum type, const char* source, const std::string& typeName) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, typeName);
        return shader;
    }

    GLuint link(GLuint vertex, GLuint fragment, GLuint geometry = 0) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        if (geometry != 0) {
            glAttachShader(program, geometry);
        }
        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");
        return program;
    }

    void checkCompileErrors(GLuint shader, const std::string& type) {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n"
                          << "-------------------------------------------------------" << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n"
                          << infoLog << "\n"
                          << "-------------------------------------------------------" << std::endl;
            }
        }
    }
};

// Usage
int main() {
    Shader shader("vertex.glsl", "fragment.glsl");

    // In render loop
    shader.use();
    shader.setFloat("time", glfwGetTime());
    shader.setVec3("lightPos", glm::vec3(1.0f, 2.0f, 3.0f));
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw...
}
```

## Hot Reloading Shaders

```cpp
class HotReloadShader : public Shader {
public:
    HotReloadShader(const std::string& vertPath, const std::string& fragPath)
        : Shader(vertPath, fragPath), vertexPath(vertPath), fragmentPath(fragPath) {
        lastModTime = getLastModifiedTime();
    }

    void checkAndReload() {
        auto currentTime = getLastModifiedTime();
        if (currentTime != lastModTime) {
            std::cout << "Reloading shaders..." << std::endl;
            reload();
            lastModTime = currentTime;
        }
    }

private:
    std::string vertexPath, fragmentPath;
    std::time_t lastModTime;

    void reload() {
        GLuint oldID = ID;

        std::string vertexCode = loadFile(vertexPath);
        std::string fragmentCode = loadFile(fragmentPath);

        GLuint vertex = compile(GL_VERTEX_SHADER, vertexCode.c_str(), "VERTEX");
        GLuint fragment = compile(GL_FRAGMENT_SHADER, fragmentCode.c_str(), "FRAGMENT");

        GLuint newID = link(vertex, fragment);

        if (newID != 0) {
            glDeleteProgram(oldID);
            ID = newID;
            uniformCache.clear();
            std::cout << "Shaders reloaded successfully!" << std::endl;
        } else {
            std::cerr << "Shader reload failed, keeping old version" << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    std::time_t getLastModifiedTime() {
        struct stat vertStat, fragStat;
        stat(vertexPath.c_str(), &vertStat);
        stat(fragmentPath.c_str(), &fragStat);
        return std::max(vertStat.st_mtime, fragStat.st_mtime);
    }
};

// Usage
HotReloadShader shader("vertex.glsl", "fragment.glsl");

// In render loop
shader.checkAndReload();
shader.use();
// ...
```

## Debugging Shaders

### Print Debug Information

```cpp
void printActiveUniforms(GLuint program) {
    GLint numUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

    std::cout << "Active Uniforms: " << numUniforms << std::endl;

    for (GLint i = 0; i < numUniforms; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);

        std::cout << "  " << i << ": " << name
                  << " (type: " << type << ", size: " << size << ")" << std::endl;
    }
}

void printActiveAttributes(GLuint program) {
    GLint numAttributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    std::cout << "Active Attributes: " << numAttributes << std::endl;

    for (GLint i = 0; i < numAttributes; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveAttrib(program, i, sizeof(name), &length, &size, &type, name);

        std::cout << "  " << i << ": " << name
                  << " (type: " << type << ", size: " << size << ")" << std::endl;
    }
}
```

## Best Practices

### 1. Cache Uniform Locations

```cpp
// BAD (query every frame)
glUniform1f(glGetUniformLocation(program, "time"), time);

// GOOD (cache location)
static GLint timeLocation = glGetUniformLocation(program, "time");
glUniform1f(timeLocation, time);
```

### 2. Minimize Uniform Updates

```cpp
// Only update if changed
if (newValue != oldValue) {
    shader.setFloat("value", newValue);
    oldValue = newValue;
}
```

### 3. Use Uniform Buffers for Shared Data

```cpp
// Shared across all shaders
UBO matrices;
matrices.projection = ...;
matrices.view = ...;
```

### 4. Organize Shaders in Files

```
shaders/
├── basic/
│   ├── vertex.glsl
│   └── fragment.glsl
├── lighting/
│   ├── vertex.glsl
│   └── fragment.glsl
└── common/
    └── lighting.glsl (include file)
```

### 5. Handle Shader Errors Gracefully

```cpp
if (shader.ID == 0) {
    std::cerr << "Failed to create shader, using fallback" << std::endl;
    shader = fallbackShader;
}
```

## Summary

You learned:

1. **Shader Compilation**: Loading, compiling, and error checking
2. **Program Linking**: Combining shaders into programs
3. **Uniforms**: Setting basic, array, matrix, and struct uniforms
4. **UBOs**: Sharing uniforms across shaders efficiently
5. **SSBOs**: Large, writable shader storage buffers
6. **Shader Class**: Complete, production-ready shader manager
7. **Hot Reloading**: Reloading shaders without restarting
8. **Debugging**: Printing active uniforms and attributes
9. **Best Practices**: Caching, minimizing updates, organization

### What's Next?

**Lesson 30**: Texturing Basics - loading, sampling, and applying textures

### Practice Exercises

1. **Multi-Shader System**: Create shader manager for multiple shaders
2. **Uniform Inspector**: Build GUI for viewing/editing uniforms
3. **Shader Variants**: Implement shader permutations system
4. **Performance Profiling**: Measure uniform update costs
5. **Shader Library**: Create reusable shader component system
