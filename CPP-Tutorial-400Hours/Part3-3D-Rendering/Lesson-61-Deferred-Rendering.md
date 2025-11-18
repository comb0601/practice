# Lesson 61: Deferred Rendering

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Deferred Rendering
2. G-Buffer Architecture
3. Geometry Pass Implementation
4. Lighting Pass Implementation
5. Complete Deferred Renderer
6. Forward vs Deferred Comparison
7. Optimization Techniques
8. Exercises and Projects

---

## 1. Introduction to Deferred Rendering

### What is Deferred Rendering?

Deferred rendering is a rendering technique where geometry and lighting calculations are separated into distinct passes. Instead of calculating lighting for every fragment during geometry rendering, we first render all geometric information to multiple render targets (G-Buffer), then perform all lighting calculations in a second pass.

### Why Use Deferred Rendering?

**Traditional Forward Rendering Issues:**
```cpp
// Forward rendering: O(Lights * Fragments)
for each object:
    for each light:
        calculate lighting
        // Problem: Many fragments are overdrawn
        // Lighting calculated multiple times unnecessarily
```

**Deferred Rendering Benefits:**
```cpp
// Deferred rendering: O(Lights * Visible Pixels)
// Pass 1: Render geometry to G-Buffer
for each object:
    write position, normal, albedo to G-Buffer

// Pass 2: Calculate lighting
for each light:
    for each screen pixel:
        read G-Buffer data
        calculate lighting once per visible pixel
```

### Basic Deferred Rendering Structure

```cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

class DeferredRenderer
{
public:
    // G-Buffer textures
    GLuint gPosition;
    GLuint gNormal;
    GLuint gAlbedoSpec;
    GLuint gDepth;
    GLuint gBuffer;

    // Screen dimensions
    int width, height;

    DeferredRenderer(int w, int h) : width(w), height(h)
    {
        setupGBuffer();
    }

    void setupGBuffer()
    {
        // Create framebuffer
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        // Position color buffer (RGB32F for high precision)
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, gPosition, 0);

        // Normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                              GL_TEXTURE_2D, gNormal, 0);

        // Albedo + Specular color buffer
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                              GL_TEXTURE_2D, gAlbedoSpec, 0);

        // Tell OpenGL which color attachments we'll use
        GLuint attachments[3] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };
        glDrawBuffers(3, attachments);

        // Create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &gDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                             width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, gDepth);

        // Check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERROR: G-Buffer framebuffer is not complete!"
                      << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bindForGeometryPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void bindForLightingPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind G-Buffer textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    }

    ~DeferredRenderer()
    {
        glDeleteFramebuffers(1, &gBuffer);
        glDeleteTextures(1, &gPosition);
        glDeleteTextures(1, &gNormal);
        glDeleteTextures(1, &gAlbedoSpec);
        glDeleteRenderbuffers(1, &gDepth);
    }
};
```

---

## 2. G-Buffer Architecture

### Understanding the G-Buffer

The G-Buffer (Geometry Buffer) stores geometric information for every visible pixel. Common G-Buffer layouts include:

```cpp
struct GBufferLayout
{
    // RT0: Position (RGB32F)
    // - R: World space X
    // - G: World space Y
    // - B: World space Z

    // RT1: Normal (RGB16F)
    // - R: Normal X
    // - G: Normal Y
    // - B: Normal Z

    // RT2: Albedo + Specular (RGBA8)
    // - RGB: Diffuse color
    // - A: Specular intensity

    // Depth: Depth buffer
};
```

### Optimized G-Buffer Implementation

```cpp
#include <memory>

class OptimizedGBuffer
{
public:
    struct GBufferData
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 albedo;
        float specular;
        float metallic;
        float roughness;
    };

private:
    GLuint fbo;
    GLuint textures[4];  // position, normal, albedo, depth
    int width, height;

    // Shader for geometry pass
    GLuint geometryShader;

public:
    OptimizedGBuffer(int w, int h) : width(w), height(h)
    {
        setupBuffers();
        createGeometryShader();
    }

    void setupBuffers()
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(4, textures);

        // Position + Depth texture (RGB32F + D32F)
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, textures[0], 0);

        // Normal texture (RGB16F)
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                              GL_TEXTURE_2D, textures[1], 0);

        // Albedo + Material properties (RGBA16F)
        // RGB: Albedo, A: Packed material data
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                              GL_TEXTURE_2D, textures[2], 0);

        // Depth texture
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_TEXTURE_2D, textures[3], 0);

        // Specify color attachments
        GLuint attachments[3] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };
        glDrawBuffers(3, attachments);

        // Verify framebuffer
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "G-Buffer framebuffer not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void createGeometryShader()
    {
        // Vertex shader for geometry pass
        const char* vertexSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoords;

            out vec3 FragPos;
            out vec3 Normal;
            out vec2 TexCoords;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                vec4 worldPos = model * vec4(aPos, 1.0);
                FragPos = worldPos.xyz;

                mat3 normalMatrix = transpose(inverse(mat3(model)));
                Normal = normalize(normalMatrix * aNormal);

                TexCoords = aTexCoords;

                gl_Position = projection * view * worldPos;
            }
        )";

        // Fragment shader for geometry pass
        const char* fragmentSource = R"(
            #version 330 core
            layout (location = 0) out vec3 gPosition;
            layout (location = 1) out vec3 gNormal;
            layout (location = 2) out vec4 gAlbedoSpec;

            in vec3 FragPos;
            in vec3 Normal;
            in vec2 TexCoords;

            uniform vec3 albedo;
            uniform float specular;
            uniform float metallic;
            uniform float roughness;

            void main()
            {
                // Store fragment position
                gPosition = FragPos;

                // Store normalized normal
                gNormal = normalize(Normal);

                // Store albedo in RGB, pack material properties in A
                gAlbedoSpec.rgb = albedo;

                // Pack specular, metallic, roughness into alpha
                // This is a simple packing scheme
                gAlbedoSpec.a = specular;
            }
        )";

        // Compile and link shaders (simplified - add error checking)
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, nullptr);
        glCompileShader(fragment);

        geometryShader = glCreateProgram();
        glAttachShader(geometryShader, vertex);
        glAttachShader(geometryShader, fragment);
        glLinkProgram(geometryShader);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void beginGeometryPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(geometryShader);
    }

    void endGeometryPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getPositionTexture() const { return textures[0]; }
    GLuint getNormalTexture() const { return textures[1]; }
    GLuint getAlbedoTexture() const { return textures[2]; }
    GLuint getDepthTexture() const { return textures[3]; }
    GLuint getGeometryShader() const { return geometryShader; }

    ~OptimizedGBuffer()
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(4, textures);
        glDeleteProgram(geometryShader);
    }
};
```

---

## 3. Geometry Pass Implementation

### Complete Geometry Pass

```cpp
#include <string>
#include <fstream>
#include <sstream>

class GeometryPass
{
private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Material
    {
        glm::vec3 albedo;
        float specular;
        float metallic;
        float roughness;
    };

    GeometryPass()
    {
        setupMesh();
        createShaders();
    }

    void setupMesh()
    {
        // Create a simple cube for demonstration
        float cubeVertices[] = {
            // positions          // normals           // texcoords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            // ... (add more faces)
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
                     cubeVertices, GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)0);

        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)(3 * sizeof(float)));

        // Texture coordinate attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
    }

    void createShaders()
    {
        // Already shown in OptimizedGBuffer class
        // Use the same vertex and fragment shaders
    }

    void render(const glm::mat4& model, const glm::mat4& view,
               const glm::mat4& projection, const Material& mat)
    {
        glUseProgram(shaderProgram);

        // Set matrices
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // Set material properties
        GLint albedoLoc = glGetUniformLocation(shaderProgram, "albedo");
        GLint specLoc = glGetUniformLocation(shaderProgram, "specular");
        GLint metalLoc = glGetUniformLocation(shaderProgram, "metallic");
        GLint roughLoc = glGetUniformLocation(shaderProgram, "roughness");

        glUniform3fv(albedoLoc, 1, &mat.albedo[0]);
        glUniform1f(specLoc, mat.specular);
        glUniform1f(metalLoc, mat.metallic);
        glUniform1f(roughLoc, mat.roughness);

        // Render mesh
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  // 36 vertices for a cube
        glBindVertexArray(0);
    }

    ~GeometryPass()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 4. Lighting Pass Implementation

### Deferred Lighting Shader

```cpp
class LightingPass
{
private:
    GLuint shaderProgram;
    GLuint quadVAO, quadVBO;

public:
    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float radius;
    };

    LightingPass()
    {
        setupQuad();
        createLightingShader();
    }

    void setupQuad()
    {
        // Full-screen quad for lighting pass
        float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void createLightingShader()
    {
        // Vertex shader
        const char* vertexSource = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 aTexCoords;

            out vec2 TexCoords;

            void main()
            {
                TexCoords = aTexCoords;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";

        // Fragment shader
        const char* fragmentSource = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoords;

            uniform sampler2D gPosition;
            uniform sampler2D gNormal;
            uniform sampler2D gAlbedoSpec;

            struct Light {
                vec3 Position;
                vec3 Color;
                float Intensity;
                float Radius;
            };

            const int MAX_LIGHTS = 32;
            uniform Light lights[MAX_LIGHTS];
            uniform int numLights;
            uniform vec3 viewPos;

            void main()
            {
                // Retrieve data from G-Buffer
                vec3 FragPos = texture(gPosition, TexCoords).rgb;
                vec3 Normal = texture(gNormal, TexCoords).rgb;
                vec4 AlbedoSpec = texture(gAlbedoSpec, TexCoords);
                vec3 Albedo = AlbedoSpec.rgb;
                float Specular = AlbedoSpec.a;

                // Calculate lighting
                vec3 lighting = Albedo * 0.1; // Ambient
                vec3 viewDir = normalize(viewPos - FragPos);

                for(int i = 0; i < numLights; ++i)
                {
                    // Diffuse
                    vec3 lightDir = normalize(lights[i].Position - FragPos);
                    float diff = max(dot(Normal, lightDir), 0.0);
                    vec3 diffuse = lights[i].Color * diff * Albedo;

                    // Specular
                    vec3 halfwayDir = normalize(lightDir + viewDir);
                    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
                    vec3 specular = lights[i].Color * spec * Specular;

                    // Attenuation
                    float distance = length(lights[i].Position - FragPos);
                    float attenuation = 1.0 / (1.0 + 0.09 * distance +
                                              0.032 * distance * distance);

                    diffuse *= attenuation * lights[i].Intensity;
                    specular *= attenuation * lights[i].Intensity;

                    lighting += diffuse + specular;
                }

                FragColor = vec4(lighting, 1.0);
            }
        )";

        // Compile and link (simplified)
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, nullptr);
        glCompileShader(fragment);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void render(const std::vector<PointLight>& lights,
               const glm::vec3& viewPos)
    {
        glUseProgram(shaderProgram);

        // Set G-Buffer textures
        glUniform1i(glGetUniformLocation(shaderProgram, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(shaderProgram, "gAlbedoSpec"), 2);

        // Set lights
        glUniform1i(glGetUniformLocation(shaderProgram, "numLights"),
                   lights.size());

        for(size_t i = 0; i < lights.size(); ++i)
        {
            std::string base = "lights[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(shaderProgram,
                        (base + ".Position").c_str()),
                        1, &lights[i].position[0]);
            glUniform3fv(glGetUniformLocation(shaderProgram,
                        (base + ".Color").c_str()),
                        1, &lights[i].color[0]);
            glUniform1f(glGetUniformLocation(shaderProgram,
                       (base + ".Intensity").c_str()),
                       lights[i].intensity);
            glUniform1f(glGetUniformLocation(shaderProgram,
                       (base + ".Radius").c_str()),
                       lights[i].radius);
        }

        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"),
                    1, &viewPos[0]);

        // Render quad
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    ~LightingPass()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 5. Complete Deferred Renderer

### Full Implementation

```cpp
#include <memory>
#include <chrono>

class CompleteDeferredRenderer
{
private:
    std::unique_ptr<OptimizedGBuffer> gBuffer;
    std::unique_ptr<GeometryPass> geometryPass;
    std::unique_ptr<LightingPass> lightingPass;

    int width, height;

    // Camera
    glm::vec3 cameraPos;
    glm::mat4 view;
    glm::mat4 projection;

    // Scene data
    std::vector<GeometryPass::Material> materials;
    std::vector<glm::mat4> modelMatrices;
    std::vector<LightingPass::PointLight> lights;

public:
    CompleteDeferredRenderer(int w, int h)
        : width(w), height(h),
          cameraPos(0.0f, 0.0f, 3.0f)
    {
        gBuffer = std::make_unique<OptimizedGBuffer>(width, height);
        geometryPass = std::make_unique<GeometryPass>();
        lightingPass = std::make_unique<LightingPass>();

        setupScene();
        updateProjection();
    }

    void setupScene()
    {
        // Create some materials
        GeometryPass::Material mat1;
        mat1.albedo = glm::vec3(1.0f, 0.0f, 0.0f);
        mat1.specular = 0.5f;
        mat1.metallic = 0.0f;
        mat1.roughness = 0.5f;
        materials.push_back(mat1);

        GeometryPass::Material mat2;
        mat2.albedo = glm::vec3(0.0f, 1.0f, 0.0f);
        mat2.specular = 0.8f;
        mat2.metallic = 0.0f;
        mat2.roughness = 0.3f;
        materials.push_back(mat2);

        // Create model matrices
        glm::mat4 model1 = glm::translate(glm::mat4(1.0f),
                                         glm::vec3(-2.0f, 0.0f, 0.0f));
        modelMatrices.push_back(model1);

        glm::mat4 model2 = glm::translate(glm::mat4(1.0f),
                                         glm::vec3(2.0f, 0.0f, 0.0f));
        modelMatrices.push_back(model2);

        // Create lights
        LightingPass::PointLight light1;
        light1.position = glm::vec3(0.0f, 2.0f, 2.0f);
        light1.color = glm::vec3(1.0f, 1.0f, 1.0f);
        light1.intensity = 1.0f;
        light1.radius = 10.0f;
        lights.push_back(light1);

        LightingPass::PointLight light2;
        light2.position = glm::vec3(-3.0f, 1.0f, 0.0f);
        light2.color = glm::vec3(1.0f, 0.0f, 0.0f);
        light2.intensity = 0.5f;
        light2.radius = 5.0f;
        lights.push_back(light2);
    }

    void updateProjection()
    {
        projection = glm::perspective(glm::radians(45.0f),
                                     (float)width / (float)height,
                                     0.1f, 100.0f);
    }

    void updateCamera(const glm::vec3& pos, const glm::vec3& target)
    {
        cameraPos = pos;
        view = glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void render()
    {
        // === GEOMETRY PASS ===
        gBuffer->beginGeometryPass();

        for(size_t i = 0; i < modelMatrices.size(); ++i)
        {
            geometryPass->render(modelMatrices[i], view, projection,
                               materials[i % materials.size()]);
        }

        gBuffer->endGeometryPass();

        // === LIGHTING PASS ===
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind G-Buffer textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());

        lightingPass->render(lights, cameraPos);
    }

    void addLight(const LightingPass::PointLight& light)
    {
        lights.push_back(light);
    }

    void addObject(const glm::mat4& model,
                  const GeometryPass::Material& material)
    {
        modelMatrices.push_back(model);
        materials.push_back(material);
    }

    // Performance monitoring
    struct PerformanceStats
    {
        float geometryPassTime;
        float lightingPassTime;
        float totalFrameTime;
        int drawCalls;
    };

    PerformanceStats getPerformanceStats()
    {
        PerformanceStats stats;

        auto start = std::chrono::high_resolution_clock::now();

        // Geometry pass
        gBuffer->beginGeometryPass();
        auto geoStart = std::chrono::high_resolution_clock::now();

        for(size_t i = 0; i < modelMatrices.size(); ++i)
        {
            geometryPass->render(modelMatrices[i], view, projection,
                               materials[i % materials.size()]);
        }

        auto geoEnd = std::chrono::high_resolution_clock::now();
        gBuffer->endGeometryPass();

        // Lighting pass
        auto lightStart = std::chrono::high_resolution_clock::now();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());

        lightingPass->render(lights, cameraPos);
        auto lightEnd = std::chrono::high_resolution_clock::now();

        auto end = std::chrono::high_resolution_clock::now();

        stats.geometryPassTime = std::chrono::duration<float,
                                std::milli>(geoEnd - geoStart).count();
        stats.lightingPassTime = std::chrono::duration<float,
                                std::milli>(lightEnd - lightStart).count();
        stats.totalFrameTime = std::chrono::duration<float,
                              std::milli>(end - start).count();
        stats.drawCalls = modelMatrices.size() + 1; // +1 for lighting quad

        return stats;
    }
};
```

---

## 6. Forward vs Deferred Comparison

### Performance Analysis

```cpp
class RenderingComparison
{
public:
    struct BenchmarkResults
    {
        float forwardRenderTime;
        float deferredRenderTime;
        int objectCount;
        int lightCount;
        float speedupFactor;
    };

    static BenchmarkResults runBenchmark(int objects, int lights)
    {
        BenchmarkResults results;
        results.objectCount = objects;
        results.lightCount = lights;

        // Forward rendering complexity: O(Objects * Lights * Fragments)
        // Approximate timing
        results.forwardRenderTime = objects * lights * 0.1f; // ms

        // Deferred rendering complexity: O(Objects * Fragments + Lights * Pixels)
        // Approximate timing
        float geometryTime = objects * 0.05f; // Geometry pass
        float lightingTime = lights * 0.03f;   // Lighting pass
        results.deferredRenderTime = geometryTime + lightingTime;

        results.speedupFactor = results.forwardRenderTime /
                               results.deferredRenderTime;

        return results;
    }

    static void printComparison()
    {
        std::cout << "=== Forward vs Deferred Rendering ===" << std::endl;
        std::cout << std::endl;

        std::vector<int> objectCounts = {10, 50, 100, 500};
        std::vector<int> lightCounts = {4, 16, 32, 64};

        for(int objects : objectCounts)
        {
            for(int lights : lightCounts)
            {
                auto results = runBenchmark(objects, lights);

                std::cout << "Objects: " << objects
                         << ", Lights: " << lights << std::endl;
                std::cout << "  Forward: " << results.forwardRenderTime
                         << " ms" << std::endl;
                std::cout << "  Deferred: " << results.deferredRenderTime
                         << " ms" << std::endl;
                std::cout << "  Speedup: " << results.speedupFactor
                         << "x" << std::endl;
                std::cout << std::endl;
            }
        }

        std::cout << "=== Advantages ===" << std::endl;
        std::cout << "\nForward Rendering:" << std::endl;
        std::cout << "  + Simple implementation" << std::endl;
        std::cout << "  + Works with MSAA" << std::endl;
        std::cout << "  + Low memory usage" << std::endl;
        std::cout << "  + Supports transparency easily" << std::endl;
        std::cout << "  - Poor performance with many lights" << std::endl;
        std::cout << "  - Redundant lighting calculations" << std::endl;

        std::cout << "\nDeferred Rendering:" << std::endl;
        std::cout << "  + Excellent with many lights" << std::endl;
        std::cout << "  + Lighting calculated once per pixel" << std::endl;
        std::cout << "  + Easy to add lighting effects" << std::endl;
        std::cout << "  - High memory bandwidth" << std::endl;
        std::cout << "  - Difficult transparency" << std::endl;
        std::cout << "  - MSAA requires workarounds" << std::endl;
    }
};
```

---

## 7. Optimization Techniques

### Light Volume Optimization

```cpp
class OptimizedDeferredLighting
{
private:
    GLuint sphereVAO, sphereVBO, sphereEBO;
    GLuint lightVolumeShader;
    std::vector<glm::vec3> sphereVertices;
    std::vector<unsigned int> sphereIndices;

public:
    OptimizedDeferredLighting()
    {
        generateSphere(16, 16);
        createLightVolumeShader();
    }

    void generateSphere(int rings, int sectors)
    {
        // Generate sphere mesh for light volumes
        sphereVertices.clear();
        sphereIndices.clear();

        float const R = 1.0f / (float)(rings - 1);
        float const S = 1.0f / (float)(sectors - 1);

        for(int r = 0; r < rings; ++r)
        {
            for(int s = 0; s < sectors; ++s)
            {
                float y = sin(-M_PI_2 + M_PI * r * R);
                float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
                float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

                sphereVertices.push_back(glm::vec3(x, y, z));
            }
        }

        for(int r = 0; r < rings - 1; ++r)
        {
            for(int s = 0; s < sectors - 1; ++s)
            {
                sphereIndices.push_back(r * sectors + s);
                sphereIndices.push_back((r + 1) * sectors + s);
                sphereIndices.push_back((r + 1) * sectors + (s + 1));

                sphereIndices.push_back(r * sectors + s);
                sphereIndices.push_back((r + 1) * sectors + (s + 1));
                sphereIndices.push_back(r * sectors + (s + 1));
            }
        }

        // Setup VAO/VBO/EBO
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);

        glBindVertexArray(sphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sphereVertices.size() * sizeof(glm::vec3),
                     sphereVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sphereIndices.size() * sizeof(unsigned int),
                     sphereIndices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            sizeof(glm::vec3), (void*)0);

        glBindVertexArray(0);
    }

    void createLightVolumeShader()
    {
        // Shader that renders light volumes instead of full-screen quad
        const char* vertexSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        const char* fragmentSource = R"(
            #version 330 core
            out vec4 FragColor;

            uniform sampler2D gPosition;
            uniform sampler2D gNormal;
            uniform sampler2D gAlbedoSpec;

            uniform vec3 lightPos;
            uniform vec3 lightColor;
            uniform float lightIntensity;
            uniform float lightRadius;
            uniform vec3 viewPos;

            uniform vec2 screenSize;

            void main()
            {
                // Calculate texture coordinates from screen position
                vec2 TexCoords = gl_FragCoord.xy / screenSize;

                // Retrieve G-Buffer data
                vec3 FragPos = texture(gPosition, TexCoords).rgb;
                vec3 Normal = texture(gNormal, TexCoords).rgb;
                vec4 AlbedoSpec = texture(gAlbedoSpec, TexCoords);
                vec3 Albedo = AlbedoSpec.rgb;
                float Specular = AlbedoSpec.a;

                // Calculate lighting for this light only
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(Normal, lightDir), 0.0);
                vec3 diffuse = lightColor * diff * Albedo;

                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
                vec3 specular = lightColor * spec * Specular;

                float distance = length(lightPos - FragPos);
                float attenuation = 1.0 / (1.0 + 0.09 * distance +
                                          0.032 * distance * distance);

                diffuse *= attenuation * lightIntensity;
                specular *= attenuation * lightIntensity;

                FragColor = vec4(diffuse + specular, 1.0);
            }
        )";

        // Compile and link shader
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, nullptr);
        glCompileShader(fragment);

        lightVolumeShader = glCreateProgram();
        glAttachShader(lightVolumeShader, vertex);
        glAttachShader(lightVolumeShader, fragment);
        glLinkProgram(lightVolumeShader);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void renderLightVolume(const LightingPass::PointLight& light,
                          const glm::mat4& view,
                          const glm::mat4& projection,
                          const glm::vec3& viewPos,
                          const glm::vec2& screenSize)
    {
        glUseProgram(lightVolumeShader);

        // Create model matrix (scale by light radius)
        glm::mat4 model = glm::translate(glm::mat4(1.0f), light.position);
        model = glm::scale(model, glm::vec3(light.radius));

        glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "model"),
                          1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glUniform3fv(glGetUniformLocation(lightVolumeShader, "lightPos"),
                    1, &light.position[0]);
        glUniform3fv(glGetUniformLocation(lightVolumeShader, "lightColor"),
                    1, &light.color[0]);
        glUniform1f(glGetUniformLocation(lightVolumeShader, "lightIntensity"),
                   light.intensity);
        glUniform1f(glGetUniformLocation(lightVolumeShader, "lightRadius"),
                   light.radius);
        glUniform3fv(glGetUniformLocation(lightVolumeShader, "viewPos"),
                    1, &viewPos[0]);
        glUniform2fv(glGetUniformLocation(lightVolumeShader, "screenSize"),
                    1, &screenSize[0]);

        // Set G-Buffer textures
        glUniform1i(glGetUniformLocation(lightVolumeShader, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(lightVolumeShader, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(lightVolumeShader, "gAlbedoSpec"), 2);

        // Enable additive blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        // Disable depth write, enable depth test
        glDepthMask(GL_FALSE);
        glEnable(GL_DEPTH_TEST);

        // Cull front faces (we're inside the sphere)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        // Render sphere
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(),
                      GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Restore state
        glCullFace(GL_BACK);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    ~OptimizedDeferredLighting()
    {
        glDeleteVertexArrays(1, &sphereVAO);
        glDeleteBuffers(1, &sphereVBO);
        glDeleteBuffers(1, &sphereEBO);
        glDeleteProgram(lightVolumeShader);
    }
};
```

---

## 8. Exercises and Projects

### Exercise 1: Basic Deferred Renderer

Create a simple deferred renderer with:
- G-Buffer with position, normal, and albedo
- Support for 4 point lights
- Full-screen lighting pass

### Exercise 2: Debug Visualization

```cpp
class GBufferDebugViewer
{
private:
    GLuint debugShader;
    GLuint quadVAO, quadVBO;

public:
    enum class DebugMode
    {
        POSITION,
        NORMAL,
        ALBEDO,
        SPECULAR,
        DEPTH,
        FINAL
    };

    GBufferDebugViewer()
    {
        setupQuad();
        createDebugShader();
    }

    void setupQuad()
    {
        float quadVertices[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void createDebugShader()
    {
        const char* vertexSource = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 aTexCoords;
            out vec2 TexCoords;
            void main() {
                TexCoords = aTexCoords;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";

        const char* fragmentSource = R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D gPosition;
            uniform sampler2D gNormal;
            uniform sampler2D gAlbedoSpec;
            uniform sampler2D gDepth;
            uniform int debugMode;

            void main() {
                if(debugMode == 0) // Position
                    FragColor = vec4(texture(gPosition, TexCoords).rgb, 1.0);
                else if(debugMode == 1) // Normal
                    FragColor = vec4(texture(gNormal, TexCoords).rgb * 0.5 + 0.5, 1.0);
                else if(debugMode == 2) // Albedo
                    FragColor = vec4(texture(gAlbedoSpec, TexCoords).rgb, 1.0);
                else if(debugMode == 3) // Specular
                    FragColor = vec4(vec3(texture(gAlbedoSpec, TexCoords).a), 1.0);
                else if(debugMode == 4) // Depth
                    FragColor = vec4(vec3(texture(gDepth, TexCoords).r), 1.0);
            }
        )";

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, nullptr);
        glCompileShader(fragment);

        debugShader = glCreateProgram();
        glAttachShader(debugShader, vertex);
        glAttachShader(debugShader, fragment);
        glLinkProgram(debugShader);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void render(DebugMode mode)
    {
        glUseProgram(debugShader);

        glUniform1i(glGetUniformLocation(debugShader, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(debugShader, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(debugShader, "gAlbedoSpec"), 2);
        glUniform1i(glGetUniformLocation(debugShader, "gDepth"), 3);
        glUniform1i(glGetUniformLocation(debugShader, "debugMode"),
                   static_cast<int>(mode));

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    ~GBufferDebugViewer()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(debugShader);
    }
};
```

### Project: Complete Scene Renderer

Build a complete scene renderer with:
1. Deferred rendering pipeline
2. Support for 100+ lights
3. Light volume optimization
4. Debug visualization modes
5. Performance monitoring
6. Dynamic light addition/removal

---

## Key Takeaways

1. **Deferred rendering separates geometry and lighting**
2. **G-Buffer stores geometric information**
3. **Lighting calculated once per visible pixel**
4. **Excellent performance with many lights**
5. **Higher memory bandwidth requirements**
6. **Light volumes improve performance**
7. **Debug visualization aids development**
8. **Trade-offs vs forward rendering**

---

## Summary Checklist

Before moving to Lesson 62, ensure you can:
- [ ] Understand deferred rendering pipeline
- [ ] Create and manage G-Buffer
- [ ] Implement geometry pass
- [ ] Implement lighting pass
- [ ] Handle multiple lights efficiently
- [ ] Use light volumes for optimization
- [ ] Debug G-Buffer contents
- [ ] Compare forward vs deferred rendering
- [ ] Implement performance monitoring

---

## Next Lesson Preview

In **Lesson 62**, we'll explore:
- Screen Space Ambient Occlusion (SSAO)
- SSAO theory and algorithms
- Sample kernel generation
- Random rotation noise
- SSAO blur and optimization
- Integration with deferred rendering

**Deferred rendering mastered!**
