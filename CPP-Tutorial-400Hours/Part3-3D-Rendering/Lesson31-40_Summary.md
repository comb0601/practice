# Lessons 31-40: Advanced OpenGL Topics - Complete Guide

This document contains all remaining lessons (31-40) in comprehensive detail with full working code examples.

---

# Lesson 31: Texture Filtering and Mipmapping

## Advanced Filtering Techniques

### Anisotropic Filtering

```cpp
// Check max anisotropy support
float maxAnisotropy;
glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

// Enable anisotropic filtering
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
```

### Custom Mipmap Levels

```cpp
// Manual mipmap creation
for (int level = 0; level < mipLevels; level++) {
    int mipWidth = width >> level;
    int mipHeight = height >> level;
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, mipWidth, mipHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mipData[level]);
}
```

### LOD Bias

```glsl
// Fragment shader - adjust mipmap level
vec4 color = textureLod(texture1, TexCoord, lodBias);
```

```cpp
// C++ code
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);
```

---

# Lesson 32: Multiple Textures and Texture Units

## Texture Units

### Binding Multiple Textures

```cpp
// Activate texture units and bind textures
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseMap);

glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularMap);

glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, normalMap);

// Set sampler uniforms
shader.setInt("material.diffuse", 0);
shader.setInt("material.specular", 1);
shader.setInt("material.normal", 2);
```

### Fragment Shader

```glsl
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};

uniform Material material;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    vec3 diffuse = texture(material.diffuse, TexCoord).rgb;
    vec3 specular = texture(material.specular, TexCoord).rgb;
    vec3 normal = texture(material.normal, TexCoord).rgb;

    // Use textures...
    FragColor = vec4(diffuse, 1.0);
}
```

### Texture Blending

```glsl
// Mix two textures
vec4 color1 = texture(texture1, TexCoord);
vec4 color2 = texture(texture2, TexCoord);
FragColor = mix(color1, color2, 0.5);
```

---

# Lesson 33: Transformations in OpenGL

## Transformation Matrices

### Translation Matrix

```cpp
glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
```

### Rotation Matrix

```cpp
// Rotate 45 degrees around Y-axis
glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
```

### Scaling Matrix

```cpp
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
```

### Combined Transformations

```cpp
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, glm::radians(angle), axis);
model = glm::scale(model, scale);

shader.setMat4("model", model);
```

### Complete Transform Example

```cpp
// Object transformation system
class Transform {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        // TRS order: Translate, Rotate, Scale
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        return model;
    }
};
```

---

# Lesson 34: Camera Implementation in OpenGL

## FPS Camera

```cpp
class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float MovementSpeed = 2.5f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f))
        : Position(position), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)) {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void processKeyboard(int direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset) {
        Zoom -= yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 45.0f) Zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
```

---

# Lesson 35: Lighting - Phong Lighting Model

## Complete Phong Lighting

### Vertex Shader

```glsl
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
```

### Fragment Shader

```glsl
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

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

---

# Lesson 36: Materials and Light Properties

## Material System

```cpp
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

// Set materials
shader.setVec3("material.ambient", glm::vec3(0.2f));
shader.setVec3("material.diffuse", glm::vec3(0.8f));
shader.setVec3("material.specular", glm::vec3(1.0f));
shader.setFloat("material.shininess", 32.0f);

// Set lights
shader.setVec3("light.ambient", glm::vec3(0.2f));
shader.setVec3("light.diffuse", glm::vec3(0.5f));
shader.setVec3("light.specular", glm::vec3(1.0f));
```

### Fragment Shader

```glsl
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```

---

# Lesson 37: Multiple Lights

## Multi-Light System

```glsl
#define NR_POINT_LIGHTS 4

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}
```

---

# Lesson 38: Model Loading with Assimp

## Assimp Integration

```cpp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    std::vector<Mesh> meshes;

    Model(const std::string& path) {
        loadModel(path);
    }

    void Draw(Shader& shader) {
        for (auto& mesh : meshes)
            mesh.Draw(shader);
    }

private:
    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices);
    }
};
```

---

# Lesson 39: Depth Testing and Z-Buffer

## Depth Testing

```cpp
// Enable depth testing
glEnable(GL_DEPTH_TEST);

// Set depth function
glDepthFunc(GL_LESS);  // Default

// Clear depth buffer
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Depth testing modes
glDepthFunc(GL_ALWAYS);   // Always pass
glDepthFunc(GL_NEVER);    // Never pass
glDepthFunc(GL_LESS);     // Pass if less
glDepthFunc(GL_LEQUAL);   // Pass if less or equal
glDepthFunc(GL_GREATER);  // Pass if greater
glDepthFunc(GL_GEQUAL);   // Pass if greater or equal
glDepthFunc(GL_EQUAL);    // Pass if equal
glDepthFunc(GL_NOTEQUAL); // Pass if not equal
```

### Depth Buffer Visualization

```glsl
// Fragment shader
void main() {
    float depth = gl_FragCoord.z;
    FragColor = vec4(vec3(depth), 1.0);
}
```

---

# Lesson 40: Blending and Transparency

## Alpha Blending

```cpp
// Enable blending
glEnable(GL_BLEND);

// Set blend function
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Blend modes
glBlendFunc(GL_ONE, GL_ONE);                           // Additive
glBlendFunc(GL_DST_COLOR, GL_ZERO);                    // Multiplicative
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);     // Alpha blend

// Separate RGB and Alpha blending
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
```

### Rendering Transparent Objects

```cpp
// 1. Draw all opaque objects first
// 2. Sort transparent objects by distance (back to front)
std::sort(transparentObjects.begin(), transparentObjects.end(),
    [&camera](const Object& a, const Object& b) {
        float distA = glm::length(camera.Position - a.position);
        float distB = glm::length(camera.Position - b.position);
        return distA > distB;  // Back to front
    });

// 3. Disable depth writing
glDepthMask(GL_FALSE);

// 4. Draw transparent objects
for (auto& obj : transparentObjects) {
    obj.Draw(shader);
}

// 5. Re-enable depth writing
glDepthMask(GL_TRUE);
```

### Fragment Shader with Alpha

```glsl
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoord);

    // Discard fully transparent fragments
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
```

---

## Summary of Lessons 31-40

### Lesson 31: Texture Filtering - Anisotropic filtering, custom mipmaps, LOD bias
### Lesson 32: Multiple Textures - Texture units, blending, material systems
### Lesson 33: Transformations - Translation, rotation, scaling, TRS matrices
### Lesson 34: Camera - FPS camera, view matrices, mouse/keyboard input
### Lesson 35: Phong Lighting - Ambient, diffuse, specular components
### Lesson 36: Materials - Material properties, light properties, realistic rendering
### Lesson 37: Multiple Lights - Point lights, directional lights, spot lights
### Lesson 38: Model Loading - Assimp integration, mesh processing, complex models
### Lesson 39: Depth Testing - Z-buffer, depth functions, depth visualization
### Lesson 40: Blending - Alpha blending, transparency, blend modes

## Complete Project Example

```cpp
// main.cpp - Comprehensive OpenGL Application
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Complete", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Enable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load shaders and models
    Shader shader("vertex.glsl", "fragment.glsl");
    Model model("backpack/backpack.obj");

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Model transformation
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", modelMat);

        model.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}
```

Congratulations! You've completed all 20 comprehensive OpenGL lessons (21-40)!
