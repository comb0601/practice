# Lesson 69: Cubemap and Skybox

**Duration**: 7 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Cubemaps
2. Cubemap Creation and Loading
3. Skybox Implementation
4. Cubemap Sampling Techniques
5. Dynamic Cubemaps
6. Performance Optimization
7. Advanced Skybox Effects
8. Exercises and Projects

---

## 1. Introduction to Cubemaps

### What is a Cubemap?

A cubemap is a texture composed of 6 square textures that form the faces of a cube, used for environment mapping and skyboxes.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>

class Cubemap
{
public:
    GLuint textureID;
    int size;

    enum Face
    {
        POSITIVE_X = 0,  // Right
        NEGATIVE_X,      // Left
        POSITIVE_Y,      // Top
        NEGATIVE_Y,      // Bottom
        POSITIVE_Z,      // Front
        NEGATIVE_Z       // Back
    };

    Cubemap(int cubeSize = 512) : size(cubeSize)
    {
        createCubemap();
    }

    void createCubemap()
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // Allocate storage for all 6 faces
        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                        size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void loadFace(Face face, const std::string& path)
    {
        // Load image data (using stb_image or similar)
        // unsigned char* data = loadImage(path, &width, &height, &channels);

        // glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB,
        //             width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // freeImage(data);
    }

    void bind(int unit = 0)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }

    static void explainCubemapLayout()
    {
        std::cout << "=== Cubemap Face Layout ===" << std::endl;
        std::cout << "Face 0 (GL_TEXTURE_CUBE_MAP_POSITIVE_X): +X (Right)" << std::endl;
        std::cout << "Face 1 (GL_TEXTURE_CUBE_MAP_NEGATIVE_X): -X (Left)" << std::endl;
        std::cout << "Face 2 (GL_TEXTURE_CUBE_MAP_POSITIVE_Y): +Y (Top)" << std::endl;
        std::cout << "Face 3 (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y): -Y (Bottom)" << std::endl;
        std::cout << "Face 4 (GL_TEXTURE_CUBE_MAP_POSITIVE_Z): +Z (Front)" << std::endl;
        std::cout << "Face 5 (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z): -Z (Back)" << std::endl;
    }

    ~Cubemap()
    {
        glDeleteTextures(1, &textureID);
    }
};
```

---

## 2. Cubemap Creation and Loading

### Loading Cubemap from Files

```cpp
class CubemapLoader
{
public:
    struct CubemapPaths
    {
        std::string right;   // +X
        std::string left;    // -X
        std::string top;     // +Y
        std::string bottom;  // -Y
        std::string front;   // +Z
        std::string back;    // -Z

        // Constructor for common naming pattern
        CubemapPaths(const std::string& dir, const std::string& ext = ".jpg")
        {
            right  = dir + "/right" + ext;
            left   = dir + "/left" + ext;
            top    = dir + "/top" + ext;
            bottom = dir + "/bottom" + ext;
            front  = dir + "/front" + ext;
            back   = dir + "/back" + ext;
        }
    };

    static GLuint loadCubemap(const CubemapPaths& paths)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        std::vector<std::string> faces = {
            paths.right, paths.left,
            paths.top, paths.bottom,
            paths.front, paths.back
        };

        for(unsigned int i = 0; i < faces.size(); ++i)
        {
            // Load image (pseudo-code - use stb_image or similar)
            // int width, height, channels;
            // unsigned char* data = loadImage(faces[i].c_str(),
            //                                &width, &height, &channels);

            // if(data)
            // {
            //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            //                 0, GL_RGB, width, height, 0,
            //                 GL_RGB, GL_UNSIGNED_BYTE, data);
            //     freeImage(data);
            // }
            // else
            // {
            //     std::cerr << "Failed to load cubemap face: "
            //              << faces[i] << std::endl;
            // }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    // Load from single equirectangular HDR image
    static GLuint loadFromHDR(const std::string& hdrPath, int size = 512)
    {
        // 1. Load HDR equirectangular map
        // 2. Create cubemap
        // 3. Render equirectangular to cubemap faces
        // (Implementation would use conversion shader from Lesson 65)

        return 0; // Placeholder
    }
};
```

---

## 3. Skybox Implementation

### Complete Skybox Renderer

```cpp
class Skybox
{
private:
    GLuint cubemapTexture;
    GLuint skyboxVAO, skyboxVBO;
    GLuint shaderProgram;

public:
    Skybox(GLuint cubemap) : cubemapTexture(cubemap)
    {
        createSkyboxGeometry();
        createShaders();
    }

    void createSkyboxGeometry()
    {
        // Skybox cube vertices (positions only)
        float skyboxVertices[] = {
            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
                     skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            3 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    void createShaders()
    {
        const char* vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            out vec3 TexCoords;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                TexCoords = aPos;

                // Remove translation from view matrix
                mat4 rotView = mat4(mat3(view));
                vec4 pos = projection * rotView * vec4(aPos, 1.0);

                // Ensure skybox is always at far plane
                gl_Position = pos.xyww;
            }
        )";

        const char* fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 TexCoords;

            uniform samplerCube skybox;

            void main()
            {
                FragColor = texture(skybox, TexCoords);
            }
        )";

        // Compile shaders (simplified)
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSrc, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSrc, nullptr);
        glCompileShader(fragment);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void render(const glm::mat4& view, const glm::mat4& projection)
    {
        // Change depth function so skybox is only rendered where depth = 1.0
        glDepthFunc(GL_LEQUAL);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Restore depth function
        glDepthFunc(GL_LESS);
    }

    ~Skybox()
    {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 4-8. Advanced Techniques

### Cubemap Sampling and Advanced Effects

```cpp
// Advanced skybox with effects
class AdvancedSkybox
{
public:
    static const char* getAtmosphericSkyboxShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 TexCoords;

            uniform samplerCube skybox;
            uniform vec3 sunDirection;
            uniform float timeOfDay;  // 0-1
            uniform float exposure;

            vec3 atmosphericScattering(vec3 direction)
            {
                vec3 skyColor = texture(skybox, direction).rgb;

                // Simple atmospheric color tint based on time of day
                vec3 dayColor = vec3(0.5, 0.7, 1.0);
                vec3 sunsetColor = vec3(1.0, 0.5, 0.3);
                vec3 nightColor = vec3(0.1, 0.1, 0.3);

                vec3 tint;
                if(timeOfDay < 0.3)
                    tint = mix(nightColor, dayColor, timeOfDay / 0.3);
                else if(timeOfDay < 0.7)
                    tint = dayColor;
                else
                    tint = mix(dayColor, sunsetColor, (timeOfDay - 0.7) / 0.3);

                return skyColor * tint * exposure;
            }

            void main()
            {
                vec3 direction = normalize(TexCoords);
                vec3 color = atmosphericScattering(direction);

                FragColor = vec4(color, 1.0);
            }
        )";
    }

    // Dynamic skybox (rendered per frame)
    static const char* getDynamicSkyboxShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 TexCoords;

            uniform vec3 sunDirection;
            uniform float time;

            // Procedural sky
            vec3 proceduralSky(vec3 direction)
            {
                float sunDot = max(dot(direction, sunDirection), 0.0);

                // Sky gradient
                vec3 skyTop = vec3(0.2, 0.5, 1.0);
                vec3 skyHorizon = vec3(0.8, 0.9, 1.0);
                vec3 skyColor = mix(skyHorizon, skyTop, direction.y * 0.5 + 0.5);

                // Sun
                float sunIntensity = pow(sunDot, 512.0);
                vec3 sunColor = vec3(1.0, 0.9, 0.7) * sunIntensity * 10.0;

                // Simple clouds
                float clouds = sin(direction.x * 5.0 + time) *
                              sin(direction.z * 5.0 + time * 0.5);
                clouds = smoothstep(0.4, 0.6, clouds) * 0.3;

                return skyColor + sunColor + vec3(clouds);
            }

            void main()
            {
                vec3 direction = normalize(TexCoords);
                vec3 color = proceduralSky(direction);

                FragColor = vec4(color, 1.0);
            }
        )";
    }
};

// Dynamic cubemap rendering
class DynamicCubemap
{
private:
    GLuint cubemapFBO;
    GLuint cubemapTexture;
    GLuint depthBuffer;
    int size;

public:
    DynamicCubemap(int cubeSize = 256) : size(cubeSize)
    {
        createDynamicCubemap();
    }

    void createDynamicCubemap()
    {
        // Create cubemap texture
        glGenTextures(1, &cubemapTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                        size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Create framebuffer
        glGenFramebuffers(1, &cubemapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, cubemapFBO);

        // Create depth buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, depthBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void renderToCubemap(const glm::vec3& position)
    {
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),
                                                       1.0f, 0.1f, 100.0f);

        glm::mat4 captureViews[] = {
            glm::lookAt(position, position + glm::vec3( 1.0f,  0.0f,  0.0f),
                       glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(position, position + glm::vec3(-1.0f,  0.0f,  0.0f),
                       glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(position, position + glm::vec3( 0.0f,  1.0f,  0.0f),
                       glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(position, position + glm::vec3( 0.0f, -1.0f,  0.0f),
                       glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(position, position + glm::vec3( 0.0f,  0.0f,  1.0f),
                       glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(position, position + glm::vec3( 0.0f,  0.0f, -1.0f),
                       glm::vec3(0.0f, -1.0f,  0.0f))
        };

        glBindFramebuffer(GL_FRAMEBUFFER, cubemapFBO);
        glViewport(0, 0, size, size);

        for(unsigned int i = 0; i < 6; ++i)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                  cubemapTexture, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Render scene from this view
            // renderScene(captureViews[i], captureProjection);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getTexture() const { return cubemapTexture; }

    ~DynamicCubemap()
    {
        glDeleteFramebuffers(1, &cubemapFBO);
        glDeleteTextures(1, &cubemapTexture);
        glDeleteRenderbuffers(1, &depthBuffer);
    }
};

// Performance optimization tips
class CubemapOptimization
{
public:
    static void printOptimizationTips()
    {
        std::cout << "=== Cubemap Optimization Tips ===" << std::endl;
        std::cout << "1. Use appropriate resolution (512-1024 for skybox)" << std::endl;
        std::cout << "2. Use mipmaps for reflection sampling" << std::endl;
        std::cout << "3. Update dynamic cubemaps infrequently" << std::endl;
        std::cout << "4. Use lower resolution for dynamic cubemaps" << std::endl;
        std::cout << "5. Consider using cubemap arrays for multiple probes" << std::endl;
        std::cout << "6. Use compressed texture formats (DXT, BC6H for HDR)" << std::endl;
        std::cout << "7. Render skybox last with depth test LEQUAL" << std::endl;
    }
};
```

---

## Key Takeaways

1. **Cubemaps have 6 faces forming a cube**
2. **Skybox renders background environment**
3. **Use view matrix without translation**
4. **Render skybox with depth function LEQUAL**
5. **Dynamic cubemaps enable reflections**
6. **Appropriate resolution critical for performance**
7. **Cubemap sampling uses 3D direction vector**
8. **Can create procedural or image-based skyboxes**

---

## Summary Checklist

- [ ] Understand cubemap structure
- [ ] Create and load cubemaps
- [ ] Implement skybox rendering
- [ ] Handle view matrix for skybox
- [ ] Add dynamic cubemap support
- [ ] Optimize cubemap resolution
- [ ] Test with different environments
- [ ] Add atmospheric effects (optional)

---

## Next Lesson Preview

In **Lesson 70**, we'll explore:
- Environment mapping techniques
- Reflection and refraction
- Parallax-corrected reflections
- Reflection probes
- Screen-space reflections
- Real-time reflections

**Cubemap and Skybox mastered!**
