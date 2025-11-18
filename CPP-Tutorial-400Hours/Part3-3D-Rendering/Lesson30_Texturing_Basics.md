# Lesson 30: Texturing Basics

## Table of Contents
1. [What are Textures?](#what-are-textures)
2. [Loading Textures with stb_image](#loading-textures-with-stb_image)
3. [Texture Creation in OpenGL](#texture-creation-in-opengl)
4. [Texture Coordinates](#texture-coordinates)
5. [Texture Wrapping](#texture-wrapping)
6. [Texture Filtering](#texture-filtering)
7. [Mipmaps](#mipmaps)
8. [Complete Texture Class](#complete-texture-class)
9. [Practical Examples](#practical-examples)
10. [Best Practices](#best-practices)

## What are Textures?

Textures are 2D images applied to 3D surfaces to add detail without increasing geometry complexity.

### Common Texture Types

- **Diffuse Maps**: Base color/albedo
- **Specular Maps**: Shininess/reflectivity
- **Normal Maps**: Surface detail
- **Height/Displacement Maps**: Surface elevation
- **Ambient Occlusion Maps**: Shadow detail
- **Roughness/Metallic Maps**: PBR materials

### Texture Coordinates

Texture coordinates (UV coordinates) map 2D texture space to 3D geometry:

```
(0,1) ───────── (1,1)
  │               │
  │    Texture    │
  │               │
(0,0) ───────── (1,0)
```

## Loading Textures with stb_image

### Installing stb_image

Download `stb_image.h` from: https://github.com/nothings/stb

```cpp
// In ONE .cpp file:
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
```

### Loading an Image

```cpp
#include "stb_image.h"
#include <iostream>

unsigned char* loadImage(const char* filename, int* width, int* height, int* channels) {
    // Flip vertically (OpenGL expects (0,0) at bottom-left)
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filename, width, height, channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }

    std::cout << "Loaded texture: " << filename << std::endl;
    std::cout << "  Size: " << *width << "x" << *height << std::endl;
    std::cout << "  Channels: " << *channels << std::endl;

    return data;
}

// Usage
int width, height, channels;
unsigned char* imageData = loadImage("texture.png", &width, &height, &channels);

// Use imageData...

// Free memory when done
stbi_image_free(imageData);
```

## Texture Creation in OpenGL

### Basic Texture Creation

```cpp
GLuint createTexture(const char* filename) {
    // Load image
    int width, height, channels;
    unsigned char* data = loadImage(filename, &width, &height, &channels);

    if (!data) {
        return 0;
    }

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Determine format
    GLenum format;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;
    else {
        std::cerr << "Unsupported channel count: " << channels << std::endl;
        stbi_image_free(data);
        return 0;
    }

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free image memory
    stbi_image_free(data);

    return texture;
}

// Usage
GLuint texture = createTexture("wall.jpg");
glBindTexture(GL_TEXTURE_2D, texture);
// Draw...
glDeleteTextures(1, &texture);
```

## Texture Coordinates

### Defining Texture Coordinates

```cpp
float vertices[] = {
    // Positions        // Texture Coords
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // Top-left
};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
};

// Configure vertex attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

### Vertex Shader

```glsl
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
```

### Fragment Shader

```glsl
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
```

## Texture Wrapping

Controls what happens when texture coordinates exceed [0, 1] range.

### Wrapping Modes

```cpp
// GL_REPEAT (default) - Repeat texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// GL_MIRRORED_REPEAT - Mirror and repeat
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

// GL_CLAMP_TO_EDGE - Clamp to edge (stretch edge pixels)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

// GL_CLAMP_TO_BORDER - Clamp to border color
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};  // Yellow
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
```

### Visual Comparison

```
Coordinates (0,0) to (2,2):

GL_REPEAT:           GL_MIRRORED_REPEAT:    GL_CLAMP_TO_EDGE:
┌─────┬─────┐        ┌─────┬─────┐          ┌─────┬─────┐
│ Tex │ Tex │        │ Tex │ xeT │          │ Tex │▓▓▓▓▓│
├─────┼─────┤        ├─────┼─────┤          ├─────┼▓▓▓▓▓│
│ Tex │ Tex │        │ xeT │ Tex │          │▓▓▓▓▓│▓▓▓▓▓│
└─────┴─────┘        └─────┴─────┘          └─────┴─────┘
```

## Texture Filtering

Determines how pixels are sampled when magnified or minified.

### Filtering Modes

```cpp
// Magnification filter (texture magnified - appears larger)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Pixelated
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Smooth

// Minification filter (texture minified - appears smaller)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
```

### Filtering Comparison

```
GL_NEAREST:          GL_LINEAR:
┌───┬───┬───┐        ┌───────────┐
│ █ │ █ │   │        │  ░▒▓█▓▒░  │  Smooth gradient
├───┼───┼───┤        │  ░▒▓█▓▒░  │
│   │ █ │ █ │        │  ░▒▓█▓▒░  │
└───┴───┴───┘        └───────────┘
Sharp, pixelated     Smooth, blurred
```

## Mipmaps

Pre-calculated, progressively smaller versions of a texture for efficient rendering at different distances.

### Generating Mipmaps

```cpp
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);  // Automatically generate mipmap chain
```

### Mipmap Filtering Modes

```cpp
// Nearest mipmap, nearest texel
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

// Nearest mipmap, linear texel
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

// Linear mipmap, nearest texel
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

// Linear mipmap, linear texel (best quality, trilinear filtering)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
```

### Mipmap Levels

```
Level 0: 1024x1024  (original)
Level 1:  512x512
Level 2:  256x256
Level 3:  128x128
...
Level 10:   1x1
```

## Complete Texture Class

```cpp
// Texture.h
#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
public:
    GLuint ID;
    int width, height, channels;
    std::string path;

    Texture() : ID(0), width(0), height(0), channels(0) {}

    Texture(const std::string& filepath, bool generateMipmaps = true) {
        load(filepath, generateMipmaps);
    }

    ~Texture() {
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
    }

    bool load(const std::string& filepath, bool generateMipmaps = true) {
        path = filepath;

        // Load image
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "Failed to load texture: " << filepath << std::endl;
            std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
            return false;
        }

        // Determine format
        GLenum format, internalFormat;
        if (channels == 1) {
            format = internalFormat = GL_RED;
        } else if (channels == 3) {
            format = GL_RGB;
            internalFormat = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA;
        } else {
            std::cerr << "Unsupported channel count: " << channels << std::endl;
            stbi_image_free(data);
            return false;
        }

        // Create texture
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps
        if (generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Free image memory
        stbi_image_free(data);

        std::cout << "Loaded texture: " << filepath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;

        return true;
    }

    void bind(unsigned int unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setWrap(GLenum wrapS, GLenum wrapT) {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    }

    void setFilter(GLenum minFilter, GLenum magFilter) {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    void setBorderColor(float r, float g, float b, float a) {
        float borderColor[] = {r, g, b, a};
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
};

// Usage
int main() {
    Texture wallTexture("wall.jpg");
    Texture floorTexture("floor.png");

    // In render loop
    shader.use();
    wallTexture.bind(0);
    shader.setInt("texture1", 0);

    // Draw...
}
```

## Practical Examples

### Example 1: Textured Quad

```cpp
// Complete textured quad example
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = R"(
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
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
)";

int main() {
    // Initialize GLFW and create window
    // ... (omitted for brevity)

    // Compile shaders
    Shader shader(vertexShaderSource, fragmentShaderSource);

    // Vertex data
    float vertices[] = {
        // Positions        // Texture Coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Create buffers
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load texture
    Texture texture("wall.jpg");

    // Matrices
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        texture.bind(0);
        shader.setInt("texture1", 0);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
```

### Example 2: Textured Cube

```cpp
// Cube with texture coordinates
float cubeVertices[] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// Draw cube
glBindVertexArray(cubeVAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
```

## Best Practices

### 1. Texture Atlases

```cpp
// Combine multiple textures into one atlas
// Reduces texture binds and draw calls
```

### 2. Texture Compression

```cpp
// Use compressed texture formats for better performance
// DXT/BC compression, ETC2, ASTC
```

### 3. Power-of-Two Textures

```cpp
// Prefer 512x512, 1024x1024, 2048x2048
// Better for mipmaps and hardware compatibility
```

### 4. sRGB Textures

```cpp
// Use sRGB internal format for color textures
glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
```

### 5. Texture Pooling

```cpp
// Reuse texture objects
// Avoid creating/destroying textures every frame
```

## Summary

You learned:

1. **Textures**: 2D images applied to 3D surfaces
2. **stb_image**: Loading images from various formats
3. **Texture Creation**: Generating and uploading textures to GPU
4. **Texture Coordinates**: Mapping 2D textures to 3D geometry
5. **Wrapping**: Controlling texture behavior at boundaries
6. **Filtering**: Nearest vs linear, magnification vs minification
7. **Mipmaps**: Pre-calculated texture levels for efficiency
8. **Texture Class**: Complete, reusable texture manager
9. **Examples**: Textured quads and cubes
10. **Best Practices**: Atlases, compression, power-of-two

### What's Next?

**Lesson 31**: Texture Filtering and Mipmapping - advanced filtering techniques, anisotropic filtering

### Practice Exercises

1. **Multi-Texture Quad**: Load and display multiple textures
2. **Scrolling Texture**: Animate texture coordinates
3. **Texture Tiling**: Create repeating patterns
4. **Procedural Textures**: Generate textures in code
5. **Texture Inspector**: Build GUI for texture properties
