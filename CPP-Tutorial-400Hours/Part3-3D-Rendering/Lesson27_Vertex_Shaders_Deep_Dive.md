# Lesson 27: Vertex Shaders Deep Dive

## Table of Contents
1. [Vertex Shader Role](#vertex-shader-role)
2. [Coordinate Spaces and Transformations](#coordinate-spaces-and-transformations)
3. [Model-View-Projection (MVP) Matrix](#model-view-projection-mvp-matrix)
4. [Normal Transformations](#normal-transformations)
5. [Vertex Attributes](#vertex-attributes)
6. [Per-Vertex Calculations](#per-vertex-calculations)
7. [Advanced Techniques](#advanced-techniques)
8. [Complete Examples](#complete-examples)
9. [Optimization Tips](#optimization-tips)
10. [Best Practices](#best-practices)

## Vertex Shader Role

The vertex shader is the first programmable stage in the graphics pipeline. It processes each vertex independently and in parallel.

### Primary Responsibilities

1. **Transform vertex positions** from model space to clip space
2. **Calculate per-vertex lighting** (optional)
3. **Pass data to fragment shader** (colors, texture coordinates, normals)
4. **Perform vertex animations** (skinning, morphing, displacement)

### Basic Vertex Shader

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

## Coordinate Spaces and Transformations

### The Five Coordinate Spaces

**1. Model/Local Space**: Vertex positions relative to model origin
```
Vertices defined in modeling software
```

**2. World Space**: Vertex positions in the game world
```
Model matrix transforms: Local → World
```

**3. View/Camera Space**: Vertex positions relative to camera
```
View matrix transforms: World → View
```

**4. Clip Space**: Vertex positions in normalized device coordinates
```
Projection matrix transforms: View → Clip
```

**5. Screen Space**: Final pixel positions
```
Viewport transform: Clip → Screen (automatic)
```

### Transformation Pipeline

```glsl
// Complete transformation chain
vec4 localPos = vec4(aPos, 1.0);
vec4 worldPos = model * localPos;
vec4 viewPos = view * worldPos;
vec4 clipPos = projection * viewPos;

// Or combined:
gl_Position = projection * view * model * vec4(aPos, 1.0);
```

### Visual Representation

```
Local Space           World Space          View Space          Clip Space
   (-1,1)               (5,2)               (-3,1)            (-0.5,0.3)
      ●                   ●                   ●                   ●
     / \                 / \                 / \                 / \
    /   \               /   \               /   \               /   \
   ●     ●             ●     ●             ●     ●             ●     ●
 Model Matrix    +   View Matrix   +  Projection Matrix  =  gl_Position
```

## Model-View-Projection (MVP) Matrix

### Model Matrix

Transforms from local space to world space. Includes:
- Translation: Position in world
- Rotation: Orientation
- Scale: Size

```cpp
// C++ code using GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 model = glm::mat4(1.0f);

// Translation
model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f));

// Rotation (45 degrees around Y-axis)
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// Scale
model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

// Send to shader
shader.setMat4("model", glm::value_ptr(model));
```

```glsl
// Vertex shader
uniform mat4 model;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    // ...
}
```

### View Matrix

Transforms from world space to camera space.

```cpp
// C++ code
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

shader.setMat4("view", glm::value_ptr(view));
```

```glsl
// Vertex shader
uniform mat4 view;

void main() {
    vec4 viewPos = view * worldPos;
    // ...
}
```

### Projection Matrix

Transforms from view space to clip space.

**Perspective Projection** (for 3D scenes):
```cpp
// C++ code
float fov = glm::radians(45.0f);
float aspect = 800.0f / 600.0f;
float nearPlane = 0.1f;
float farPlane = 100.0f;

glm::mat4 projection = glm::perspective(fov, aspect, nearPlane, farPlane);

shader.setMat4("projection", glm::value_ptr(projection));
```

**Orthographic Projection** (for 2D or isometric views):
```cpp
// C++ code
glm::mat4 projection = glm::ortho(
    -10.0f, 10.0f,  // Left, right
    -10.0f, 10.0f,  // Bottom, top
    0.1f, 100.0f    // Near, far
);
```

```glsl
// Vertex shader
uniform mat4 projection;

void main() {
    gl_Position = projection * viewPos;
}
```

### Complete MVP Example

```glsl
// ========== Vertex Shader ==========
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

```cpp
// ========== C++ Code ==========
// Model matrix
glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

// View matrix
glm::mat4 view = glm::mat4(1.0f);
view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

// Projection matrix
glm::mat4 projection = glm::perspective(
    glm::radians(45.0f),
    800.0f / 600.0f,
    0.1f,
    100.0f
);

// Send to shader
shader.use();
shader.setMat4("model", glm::value_ptr(model));
shader.setMat4("view", glm::value_ptr(view));
shader.setMat4("projection", glm::value_ptr(projection));
```

## Normal Transformations

Normals must be transformed differently than positions to maintain correctness under non-uniform scaling.

### Why Normal Transformation is Different

```
Regular transformation:    WRONG for normals
vertex' = M * vertex       normal' ≠ M * normal

Correct normal transform:
normal' = transpose(inverse(M)) * normal
```

### Normal Matrix Calculation

```cpp
// C++ code
glm::mat4 model = getModelMatrix();
glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
```

```glsl
// Vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;  // transpose(inverse(model))

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;  // Correct normal transformation
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

### Optimization: Calculate Normal Matrix in Shader

For uniform scaling (not recommended for non-uniform):
```glsl
// Vertex shader (less efficient but works for uniform scaling)
mat3 normalMatrix = mat3(transpose(inverse(model)));
Normal = normalMatrix * aNormal;

// Better: Pass from CPU
uniform mat3 normalMatrix;
Normal = normalMatrix * aNormal;
```

## Vertex Attributes

### Common Vertex Attributes

```glsl
#version 330 core

// Position (always required)
layout (location = 0) in vec3 aPos;

// Normal (for lighting)
layout (location = 1) in vec3 aNormal;

// Texture coordinates
layout (location = 2) in vec2 aTexCoord;

// Vertex color
layout (location = 3) in vec4 aColor;

// Tangent and bitangent (for normal mapping)
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

// Bone IDs and weights (for skeletal animation)
layout (location = 6) in ivec4 aBoneIDs;
layout (location = 7) in vec4 aWeights;

void main() {
    // Use attributes...
}
```

### Corresponding C++ Vertex Structure

```cpp
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec4 color;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

// Setup vertex attributes
GLsizei stride = sizeof(Vertex);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoord));
glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, color));
glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, tangent));
glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, bitangent));

for (int i = 0; i < 6; i++) {
    glEnableVertexAttribArray(i);
}
```

## Per-Vertex Calculations

### Per-Vertex Lighting (Gouraud Shading)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 lightingColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = normalize(normalMatrix * aNormal);

    gl_Position = projection * view * vec4(FragPos, 1.0);

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

    lightingColor = ambient + diffuse + specular;
}
```

### Texture Coordinate Manipulation

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 mvp;
uniform float time;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);

    // Scroll texture
    TexCoord = aTexCoord + vec2(time * 0.1, 0.0);

    // Tile texture
    // TexCoord = aTexCoord * 5.0;

    // Rotate texture
    // float angle = time;
    // mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    // TexCoord = rot * (aTexCoord - 0.5) + 0.5;
}
```

## Advanced Techniques

### Vertex Displacement

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    vec3 pos = aPos;

    // Sine wave displacement
    float displacement = sin(aPos.x * 10.0 + time) * 0.1;
    pos += aNormal * displacement;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
```

### Vertex Animation (Wave Effect)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvp;
uniform float time;

void main() {
    vec3 pos = aPos;

    // Create wave
    float frequency = 5.0;
    float amplitude = 0.2;
    float wave = sin(pos.x * frequency + time) * amplitude;
    pos.y += wave;

    gl_Position = mvp * vec4(pos, 1.0);
}
```

### Billboarding

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 billboardPos;
uniform vec2 billboardSize;

void main() {
    // Extract camera right and up vectors from view matrix
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

    // Calculate billboard position
    vec3 vertexPos = billboardPos
                   + cameraRight * aPos.x * billboardSize.x
                   + cameraUp * aPos.y * billboardSize.y;

    gl_Position = projection * view * vec4(vertexPos, 1.0);
    TexCoord = aTexCoord;
}
```

### Instanced Rendering

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 aInstanceMatrix;  // Per-instance matrix

out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

## Complete Examples

### Example 1: Rotating Cube

```glsl
// ========== Vertex Shader ==========
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

// ========== Fragment Shader ==========
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
```

```cpp
// ========== C++ Code ==========
// In render loop
float time = glfwGetTime();

glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));

glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
);

glm::mat4 projection = glm::perspective(
    glm::radians(45.0f),
    (float)SCR_WIDTH / SCR_HEIGHT,
    0.1f,
    100.0f
);

shader.use();
shader.setMat4("model", glm::value_ptr(model));
shader.setMat4("view", glm::value_ptr(view));
shader.setMat4("projection", glm::value_ptr(projection));

glBindVertexArray(cubeVAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
```

### Example 2: Multiple Objects

```cpp
// Different transformations for different objects
std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f)
};

shader.use();
shader.setMat4("view", glm::value_ptr(view));
shader.setMat4("projection", glm::value_ptr(projection));

for (unsigned int i = 0; i < cubePositions.size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i + glfwGetTime() * 50.0f;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    shader.setMat4("model", glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
```

### Example 3: Camera System

```cpp
// Camera class (simplified)
class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw, pitch;

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(int direction, float deltaTime) {
        float velocity = 2.5f * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= glm::normalize(glm::cross(front, up)) * velocity;
        if (direction == RIGHT)
            position += glm::normalize(glm::cross(front, up)) * velocity;
    }

    void processMouse(float xoffset, float yoffset) {
        xoffset *= 0.1f;
        yoffset *= 0.1f;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateCameraVectors();
    }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }
};
```

## Optimization Tips

### 1. Minimize Calculations

```glsl
// BAD: Recalculate every vertex
mat3 normalMatrix = mat3(transpose(inverse(model)));

// GOOD: Calculate on CPU, pass as uniform
uniform mat3 normalMatrix;
```

### 2. Use Built-in Functions

```glsl
// SLOW
float len = sqrt(x*x + y*y + z*z);

// FAST
float len = length(vec3(x, y, z));
```

### 3. Avoid Branching

```glsl
// AVOID if possible
if (condition) {
    // ...
} else {
    // ...
}

// PREFER
float factor = step(threshold, value);
result = mix(value1, value2, factor);
```

### 4. Precalculate Uniform Matrices

```cpp
// Calculate once per frame, not per vertex
glm::mat4 mvp = projection * view * model;
shader.setMat4("mvp", glm::value_ptr(mvp));
```

```glsl
// Vertex shader
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);  // Single matrix multiplication
}
```

## Best Practices

### 1. Use Meaningful Variable Names

```glsl
// GOOD
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// AVOID
uniform mat4 m, v, p;
```

### 2. Pass Only Required Data

```glsl
// If fragment shader needs world position
out vec3 FragPos;
FragPos = vec3(model * vec4(aPos, 1.0));

// If fragment shader doesn't need it, don't pass it
```

### 3. Normalize Vectors

```glsl
// Always normalize normals after transformation
vec3 Normal = normalize(normalMatrix * aNormal);
```

### 4. Use Correct Data Types

```glsl
// Position: vec4 with w=1.0
gl_Position = mvp * vec4(aPos, 1.0);

// Direction: vec4 with w=0.0
vec4 direction = mvp * vec4(someDir, 0.0);
```

## Summary

In this lesson, you learned:

1. **Vertex Shader Role**: Transform positions, calculate lighting, pass data
2. **Coordinate Spaces**: Model, world, view, clip, and screen spaces
3. **MVP Matrices**: Model, view, and projection transformations
4. **Normal Transformations**: Correct normal matrix calculation
5. **Vertex Attributes**: Position, normal, texture coordinates, colors
6. **Per-Vertex Calculations**: Gouraud shading, texture manipulation
7. **Advanced Techniques**: Displacement, animation, billboarding, instancing
8. **Complete Examples**: Rotating cubes, multiple objects, camera system
9. **Optimization**: Minimize calculations, use built-ins, avoid branching
10. **Best Practices**: Naming, data passing, normalization, types

### What's Next?

In **Lesson 28**, we'll explore:
- Fragment shader operations
- Per-pixel lighting (Phong shading)
- Texture sampling techniques
- Advanced fragment effects
- Fragment shader optimization

### Practice Exercises

1. **Orbiting Camera**: Implement a camera that orbits around an object
2. **Wave Animation**: Create an animated water surface
3. **Multiple Lights**: Calculate lighting for multiple light sources
4. **Skeletal Animation**: Implement basic bone-based animation
5. **Procedural Geometry**: Generate geometry in vertex shader

Congratulations! You now have a deep understanding of vertex shaders and transformations!
