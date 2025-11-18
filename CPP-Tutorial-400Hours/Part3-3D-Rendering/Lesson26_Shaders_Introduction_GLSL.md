# Lesson 26: Shaders Introduction (GLSL)

## Table of Contents
1. [What is GLSL?](#what-is-glsl)
2. [GLSL Basics](#glsl-basics)
3. [Data Types](#data-types)
4. [Variables and Qualifiers](#variables-and-qualifiers)
5. [Operators and Functions](#operators-and-functions)
6. [Built-in Variables](#built-in-variables)
7. [Input and Output](#input-and-output)
8. [Complete Shader Examples](#complete-shader-examples)
9. [Shader Compilation in C++](#shader-compilation-in-cpp)
10. [Best Practices](#best-practices)

## What is GLSL?

**GLSL (OpenGL Shading Language)** is a high-level programming language for writing shaders that run on the GPU. It's based on C but tailored for graphics programming.

### Key Characteristics

- **C-like Syntax**: Familiar to C/C++ programmers
- **Strongly Typed**: Explicit type declarations required
- **Vector-Oriented**: Built-in vector and matrix types
- **GPU-Optimized**: Parallel execution across thousands of cores
- **Version-Specific**: Different versions for different OpenGL versions

### Shader Types

**Vertex Shader**: Processes each vertex (position transformations)
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
```

**Fragment Shader**: Determines pixel colors
```glsl
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
```

**Geometry Shader**: Generates/modifies primitives (optional)
**Tessellation Shaders**: Subdivides geometry (optional)
**Compute Shader**: General-purpose GPU computing (OpenGL 4.3+)

## GLSL Basics

### Shader Structure

```glsl
#version 330 core                    // Version directive (required)

// Input variables
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Output variables
out vec3 vertexColor;

// Uniform variables
uniform mat4 transform;
uniform float time;

// Main function (required)
void main() {
    // Shader code here
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = aColor;
}
```

### Version Directive

```glsl
#version 330 core              // OpenGL 3.3 core profile
#version 410 core              // OpenGL 4.1 core profile
#version 450 core              // OpenGL 4.5 core profile

#version 330 compatibility     // OpenGL 3.3 compatibility profile
```

Always use **core profile** for modern OpenGL.

### Comments

```glsl
// Single-line comment

/*
 * Multi-line comment
 * Second line
 */

/* Inline comment */ vec3 color;
```

## Data Types

### Scalar Types

```glsl
bool myBool = true;
int myInt = 42;
uint myUint = 100u;           // Unsigned integer
float myFloat = 3.14;
double myDouble = 3.14159265; // Higher precision (OpenGL 4.0+)
```

### Vector Types

```glsl
// Boolean vectors
bvec2 bv2 = bvec2(true, false);
bvec3 bv3 = bvec3(true, false, true);
bvec4 bv4 = bvec4(true, false, true, false);

// Integer vectors
ivec2 iv2 = ivec2(1, 2);
ivec3 iv3 = ivec3(1, 2, 3);
ivec4 iv4 = ivec4(1, 2, 3, 4);

// Unsigned integer vectors
uvec2 uv2 = uvec2(1u, 2u);
uvec3 uv3 = uvec3(1u, 2u, 3u);
uvec4 uv4 = uvec4(1u, 2u, 3u, 4u);

// Float vectors (most common)
vec2 v2 = vec2(1.0, 2.0);
vec3 v3 = vec3(1.0, 2.0, 3.0);
vec4 v4 = vec4(1.0, 2.0, 3.0, 4.0);

// Double vectors
dvec2 dv2 = dvec2(1.0, 2.0);
dvec3 dv3 = dvec3(1.0, 2.0, 3.0);
dvec4 dv4 = dvec4(1.0, 2.0, 3.0, 4.0);
```

### Matrix Types

```glsl
// 2x2 matrix
mat2 m2 = mat2(
    1.0, 2.0,
    3.0, 4.0
);

// 3x3 matrix
mat3 m3 = mat3(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
);

// 4x4 matrix (most common for transformations)
mat4 m4 = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
);

// Non-square matrices
mat2x3 m23;  // 2 columns, 3 rows
mat3x2 m32;  // 3 columns, 2 rows
mat2x4 m24;  // 2 columns, 4 rows
mat4x2 m42;  // 4 columns, 2 rows
mat3x4 m34;  // 3 columns, 4 rows
mat4x3 m43;  // 4 columns, 3 rows
```

### Sampler Types

```glsl
sampler1D s1D;          // 1D texture
sampler2D s2D;          // 2D texture (most common)
sampler3D s3D;          // 3D texture
samplerCube sCube;      // Cubemap texture

sampler2DShadow s2DS;   // Shadow map
samplerCubeShadow sCS;  // Cubemap shadow

isampler2D is2D;        // Integer texture
usampler2D us2D;        // Unsigned integer texture
```

### Structs

```glsl
struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Usage
Light light;
light.position = vec3(0.0, 10.0, 0.0);
light.color = vec3(1.0, 1.0, 1.0);
light.intensity = 1.0;

Material mat = Material(
    vec3(0.2, 0.2, 0.2),
    vec3(0.8, 0.8, 0.8),
    vec3(1.0, 1.0, 1.0),
    32.0
);
```

### Arrays

```glsl
// Fixed-size arrays
float values[5];
vec3 positions[10];
mat4 transforms[4];

// Initialization
float values[5] = float[](1.0, 2.0, 3.0, 4.0, 5.0);
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

// Access
float val = values[2];          // 3.0
vec3 color = colors[1];         // (0, 1, 0)

// Length
int len = values.length();      // 5
```

## Variables and Qualifiers

### Storage Qualifiers

**const**: Compile-time constant
```glsl
const float PI = 3.14159265;
const vec3 UP = vec3(0.0, 1.0, 0.0);
```

**in**: Input from previous stage
```glsl
// Vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Fragment shader
in vec3 vertexColor;  // From vertex shader
```

**out**: Output to next stage
```glsl
// Vertex shader
out vec3 vertexColor;  // To fragment shader

// Fragment shader
out vec4 FragColor;    // To framebuffer
```

**uniform**: Constant across all shader invocations
```glsl
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform sampler2D texture1;
```

**buffer**: Shader storage buffer (OpenGL 4.3+)
```glsl
layout(std430, binding = 0) buffer MyBuffer {
    vec4 data[];
};
```

### Interpolation Qualifiers

**smooth**: Linear interpolation (default)
```glsl
smooth out vec3 color;
```

**flat**: No interpolation (use first vertex value)
```glsl
flat out int objectID;
```

**noperspective**: Linear in screen space
```glsl
noperspective out vec2 texCoord;
```

### Precision Qualifiers (GLSL ES)

```glsl
highp float f;     // High precision
mediump float f;   // Medium precision
lowp float f;      // Low precision

precision mediump float;  // Default precision
```

## Operators and Functions

### Arithmetic Operators

```glsl
float a = 10.0;
float b = 3.0;

float add = a + b;      // 13.0
float sub = a - b;      // 7.0
float mul = a * b;      // 30.0
float div = a / b;      // 3.333...
float mod = mod(a, b);  // 1.0 (use mod function, not %)

// Vector operations
vec3 v1 = vec3(1.0, 2.0, 3.0);
vec3 v2 = vec3(4.0, 5.0, 6.0);

vec3 sum = v1 + v2;     // (5, 7, 9)
vec3 diff = v1 - v2;    // (-3, -3, -3)
vec3 scale = v1 * 2.0;  // (2, 4, 6)

// Matrix multiplication
mat4 m1, m2;
mat4 result = m1 * m2;
vec4 transformed = m1 * vec4(1.0, 2.0, 3.0, 1.0);
```

### Comparison Operators

```glsl
bool b1 = (5.0 > 3.0);   // true
bool b2 = (5.0 < 3.0);   // false
bool b3 = (5.0 >= 5.0);  // true
bool b4 = (5.0 <= 3.0);  // false
bool b5 = (5.0 == 5.0);  // true
bool b6 = (5.0 != 3.0);  // true
```

### Logical Operators

```glsl
bool a = true;
bool b = false;

bool andResult = a && b;  // false
bool orResult = a || b;   // true
bool notResult = !a;      // false
```

### Swizzling

```glsl
vec4 v = vec4(1.0, 2.0, 3.0, 4.0);

// Access components
float x = v.x;  // 1.0 (or v.r, v.s)
float y = v.y;  // 2.0 (or v.g, v.t)
float z = v.z;  // 3.0 (or v.b, v.p)
float w = v.w;  // 4.0 (or v.a, v.q)

// Swizzling
vec2 xy = v.xy;      // (1, 2)
vec3 bgr = v.bgr;    // (3, 2, 1)
vec4 wzyx = v.wzyx;  // (4, 3, 2, 1)

// Duplication
vec3 xxx = v.xxx;    // (1, 1, 1)

// Assignment
v.xy = vec2(10.0, 20.0);  // v = (10, 20, 3, 4)
v.rgba = vec4(0.5);       // v = (0.5, 0.5, 0.5, 0.5)
```

### Built-in Math Functions

```glsl
// Trigonometric
float s = sin(radians(90.0));   // 1.0
float c = cos(radians(0.0));    // 1.0
float t = tan(radians(45.0));   // 1.0
float as = asin(1.0);            // 1.5708 (90 degrees)
float ac = acos(1.0);            // 0.0
float at = atan(1.0);            // 0.7854 (45 degrees)

// Exponential
float p = pow(2.0, 3.0);         // 8.0
float e = exp(1.0);              // 2.718...
float l = log(10.0);             // 2.303...
float sq = sqrt(16.0);           // 4.0
float isq = inversesqrt(16.0);   // 0.25

// Common
float ab = abs(-5.0);            // 5.0
float sg = sign(-5.0);           // -1.0
float fl = floor(3.7);           // 3.0
float ce = ceil(3.2);            // 4.0
float fr = fract(3.7);           // 0.7
float md = mod(7.0, 3.0);        // 1.0
float mn = min(3.0, 5.0);        // 3.0
float mx = max(3.0, 5.0);        // 5.0
float cl = clamp(7.0, 0.0, 5.0); // 5.0

// Interpolation
float m = mix(0.0, 10.0, 0.5);   // 5.0 (linear interpolation)
float ss = smoothstep(0.0, 1.0, 0.5); // Smooth interpolation
float st = step(0.5, 0.3);       // 0.0 (threshold function)
```

### Vector Functions

```glsl
vec3 v1 = vec3(1.0, 2.0, 3.0);
vec3 v2 = vec3(4.0, 5.0, 6.0);

// Length and distance
float len = length(v1);          // sqrt(1^2 + 2^2 + 3^2) = 3.742
float dist = distance(v1, v2);   // 5.196

// Normalization
vec3 normalized = normalize(v1); // (0.267, 0.535, 0.802)

// Dot and cross product
float dotProd = dot(v1, v2);     // 1*4 + 2*5 + 3*6 = 32
vec3 crossProd = cross(v1, v2);  // (-3, 6, -3)

// Reflection and refraction
vec3 I = normalize(vec3(1.0, -1.0, 0.0));
vec3 N = vec3(0.0, 1.0, 0.0);
vec3 R = reflect(I, N);
vec3 refrVec = refract(I, N, 1.0/1.33);  // Air to water

// Face forward
vec3 Nref = faceforward(N, I, N);
```

### Matrix Functions

```glsl
mat4 m = mat4(1.0);  // Identity matrix

// Transpose
mat4 transposed = transpose(m);

// Determinant
float det = determinant(m);

// Inverse
mat4 inv = inverse(m);

// Matrix-vector multiplication
vec4 v = vec4(1.0, 2.0, 3.0, 1.0);
vec4 transformed = m * v;
```

## Built-in Variables

### Vertex Shader

```glsl
// Output (required)
out gl_PerVertex {
    vec4 gl_Position;       // Vertex position in clip space (REQUIRED)
    float gl_PointSize;     // Point size (for GL_POINTS)
    float gl_ClipDistance[]; // User-defined clip distances
};

// Input (built-in)
in int gl_VertexID;         // Current vertex index
in int gl_InstanceID;       // Current instance index (for instancing)
```

### Fragment Shader

```glsl
// Input (built-in)
in vec4 gl_FragCoord;       // Fragment position in window space
in bool gl_FrontFacing;     // Is front-facing triangle?
in vec2 gl_PointCoord;      // Point sprite coordinates

// Output (required)
out vec4 FragColor;         // Fragment color (user-defined name)

// Or:
layout(location = 0) out vec4 outColor;

// Depth output (optional)
out float gl_FragDepth;     // Custom depth value
```

## Input and Output

### Vertex Shader Input

```glsl
#version 330 core

// Vertex attributes (from VBO)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;

// Corresponding C++ code:
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ...); // Position
// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ...); // Normal
// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, ...); // TexCoord
// glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, ...); // Color
```

### Communication Between Shaders

```glsl
// ========== Vertex Shader ==========
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;      // To fragment shader
out vec2 TexCoord;         // To fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}

// ========== Fragment Shader ==========
#version 330 core
in vec3 vertexColor;       // From vertex shader (interpolated)
in vec2 TexCoord;          // From vertex shader (interpolated)

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
```

### Multiple Render Targets

```glsl
// Fragment shader
#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

void main() {
    gPosition = vec4(worldPos, 1.0);
    gNormal = vec4(normalize(normal), 1.0);
    gAlbedo = vec4(color, 1.0);
}
```

## Complete Shader Examples

### Example 1: Colored Vertices

```glsl
// ========== Vertex Shader ==========
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = aColor;
}

// ========== Fragment Shader ==========
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
```

### Example 2: Time-Based Animation

```glsl
// ========== Vertex Shader ==========
#version 330 core
layout (location = 0) in vec3 aPos;

uniform float time;

void main() {
    vec3 pos = aPos;
    pos.y += sin(time + aPos.x * 5.0) * 0.1;
    gl_Position = vec4(pos, 1.0);
}

// ========== Fragment Shader ==========
#version 330 core
out vec4 FragColor;

uniform float time;

void main() {
    vec3 color = vec3(
        0.5 + 0.5 * sin(time),
        0.5 + 0.5 * cos(time),
        0.5
    );
    FragColor = vec4(color, 1.0);
}
```

### Example 3: Texture Sampling

```glsl
// ========== Vertex Shader ==========
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

// ========== Fragment Shader ==========
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 color1 = texture(texture1, TexCoord);
    vec4 color2 = texture(texture2, TexCoord);
    FragColor = mix(color1, color2, 0.5);  // Blend 50/50
}
```

### Example 4: Lighting

```glsl
// ========== Vertex Shader ==========
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

// ========== Fragment Shader ==========
#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

## Shader Compilation in C++

### Shader Class

```cpp
#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        // Read shader files
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Compile shaders
        GLuint vertex = compile(GL_VERTEX_SHADER, vShaderCode, "VERTEX");
        GLuint fragment = compile(GL_FRAGMENT_SHADER, fShaderCode, "FRAGMENT");

        // Link program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setMat4(const std::string& name, const float* value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
    }

private:
    std::string readFile(const char* path) {
        std::ifstream file(path);
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

    void checkCompileErrors(GLuint shader, const std::string& type) {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
            }
        }
    }
};

// Usage
int main() {
    Shader shader("vertex.glsl", "fragment.glsl");

    while (!glfwWindowShouldClose(window)) {
        shader.use();
        shader.setFloat("time", glfwGetTime());
        shader.setVec3("lightPos", 1.0f, 2.0f, 3.0f);

        // Render...
    }
}
```

## Best Practices

### 1. Use Meaningful Names

```glsl
// BAD
in vec3 a;
uniform float f;

// GOOD
in vec3 aPosition;
uniform float time;
```

### 2. Optimize Performance

```glsl
// BAD: Expensive operations in fragment shader
void main() {
    for (int i = 0; i < 100; i++) {
        // Complex calculation per pixel
    }
}

// GOOD: Move to vertex shader when possible
// Vertex shader:
out float precomputedValue;
void main() {
    precomputedValue = expensiveCalculation();
}
```

### 3. Use const for Constants

```glsl
const float PI = 3.14159265;
const int MAX_LIGHTS = 8;
const vec3 UP = vec3(0.0, 1.0, 0.0);
```

### 4. Normalize Vectors

```glsl
// Always normalize before using in calculations
vec3 normal = normalize(aNormal);
vec3 lightDir = normalize(lightPos - FragPos);
```

### 5. Check Shader Compilation

Always check for compilation and linking errors in your C++ code.

## Summary

In this lesson, you learned:

1. **GLSL Basics**: Syntax, structure, and version directives
2. **Data Types**: Scalars, vectors, matrices, samplers, structs, and arrays
3. **Qualifiers**: in, out, uniform, const, and interpolation qualifiers
4. **Operators**: Arithmetic, comparison, logical, and swizzling
5. **Functions**: Built-in math, vector, and matrix functions
6. **Built-in Variables**: gl_Position, gl_FragCoord, gl_FragColor
7. **I/O**: Communication between CPU and shaders, between shader stages
8. **Examples**: Complete working shaders for various effects
9. **C++ Integration**: Shader class for compilation and uniform setting
10. **Best Practices**: Performance, naming, and optimization tips

### What's Next?

In **Lesson 27**, we'll dive deep into:
- Vertex shader transformations
- Model, view, and projection matrices
- Normal transformations
- Advanced vertex manipulations
- Vertex shader optimization

### Practice Exercises

1. **Color Gradient**: Create a shader that generates a color gradient
2. **Time Animation**: Animate vertex positions using sine waves
3. **Multi-Output**: Use multiple output variables
4. **Custom Functions**: Write helper functions in GLSL
5. **Shader Library**: Extend the Shader class with more features

Congratulations! You now understand GLSL and can write your own shaders!
