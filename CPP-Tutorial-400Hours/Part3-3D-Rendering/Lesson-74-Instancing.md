# Lesson 74: Instancing

**Duration**: 7 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Instancing
2. Basic Instancing with glDrawArraysInstanced
3. Instanced Vertex Attributes
4. Transform Feedback and Instancing
5. Advanced Instancing Techniques
6. Frustum Culling for Instances
7. Performance Optimization
8. Exercises and Projects

---

## 1. Introduction to Instancing

### What is Instancing?

Instancing is a rendering technique that allows you to draw the same mesh multiple times with different transformations using a single draw call, dramatically reducing CPU overhead.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

class InstancedRenderer
{
public:
    GLuint VAO, VBO, instanceVBO;
    GLuint shaderProgram;
    int instanceCount;

    InstancedRenderer(int count = 1000) : instanceCount(count)
    {
        setupMesh();
        setupInstanceData();
        createShaders();
    }

    void setupMesh()
    {
        // Simple quad vertices
        float quadVertices[] = {
            // positions     // texcoords
            -0.5f,  0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f,   0.0f, 0.0f,
             0.5f, -0.5f,   1.0f, 0.0f,

            -0.5f,  0.5f,   0.0f, 1.0f,
             0.5f, -0.5f,   1.0f, 0.0f,
             0.5f,  0.5f,   1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     quadVertices, GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                            4 * sizeof(float), (void*)0);

        // Texcoord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                            4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }

    void setupInstanceData()
    {
        // Generate instance data (positions, colors, etc.)
        std::vector<glm::vec3> translations;
        std::vector<glm::vec3> colors;

        int index = 0;
        float offset = 0.1f;
        int gridSize = sqrt(instanceCount);

        for(int y = -gridSize/2; y < gridSize/2; y++)
        {
            for(int x = -gridSize/2; x < gridSize/2; x++)
            {
                glm::vec3 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translation.z = 0.0f;
                translations.push_back(translation);

                glm::vec3 color((float)x / gridSize + 0.5f,
                               (float)y / gridSize + 0.5f,
                               0.5f);
                colors.push_back(color);

                index++;
                if(index >= instanceCount) break;
            }
            if(index >= instanceCount) break;
        }

        // Create instance VBO for translations
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec3),
                     &translations[0], GL_STATIC_DRAW);

        glBindVertexArray(VAO);

        // Set instance attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                            sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced attribute

        glBindVertexArray(0);
    }

    void createShaders()
    {
        const char* vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 aTexCoords;
            layout (location = 2) in vec3 aOffset;

            out vec2 TexCoords;
            out vec3 Color;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                vec3 pos = vec3(aPos, 0.0) + aOffset;
                gl_Position = projection * view * vec4(pos, 1.0);
                TexCoords = aTexCoords;
                Color = aOffset + 0.5; // Simple color based on position
            }
        )";

        const char* fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoords;
            in vec3 Color;

            void main()
            {
                FragColor = vec4(Color, 1.0);
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
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);
        glBindVertexArray(0);
    }

    ~InstancedRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &instanceVBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 2. Advanced Instancing with Matrices

### Full Transform Matrices

```cpp
class MatrixInstancedRenderer
{
private:
    GLuint VAO, VBO, EBO, instanceVBO;
    GLuint shaderProgram;
    int instanceCount;

public:
    MatrixInstancedRenderer(int count = 100) : instanceCount(count)
    {
        setupMesh();
        setupInstanceMatrices();
        createShaders();
    }

    void setupMesh()
    {
        // Cube vertices
        float cubeVertices[] = {
            // positions          // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            // ... (complete cube vertices)
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
                     cubeVertices, GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            6 * sizeof(float), (void*)0);

        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    void setupInstanceMatrices()
    {
        // Generate transformation matrices for each instance
        std::vector<glm::mat4> modelMatrices;

        for(int i = 0; i < instanceCount; ++i)
        {
            glm::mat4 model(1.0f);

            // Random position
            float x = (rand() % 100 - 50) / 10.0f;
            float y = (rand() % 100 - 50) / 10.0f;
            float z = (rand() % 100 - 50) / 10.0f;
            model = glm::translate(model, glm::vec3(x, y, z));

            // Random rotation
            float angle = (rand() % 360);
            model = glm::rotate(model, glm::radians(angle),
                              glm::vec3(1.0f, 0.3f, 0.5f));

            // Random scale
            float scale = (rand() % 20 + 10) / 100.0f;
            model = glm::scale(model, glm::vec3(scale));

            modelMatrices.push_back(model);
        }

        // Store matrices in instance buffer
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4),
                     &modelMatrices[0], GL_STATIC_DRAW);

        glBindVertexArray(VAO);

        // Set up matrix attributes (mat4 requires 4 vec4 attributes)
        for(int i = 0; i < 4; ++i)
        {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
                                sizeof(glm::mat4),
                                (void*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(2 + i, 1);
        }

        glBindVertexArray(0);
    }

    void createShaders()
    {
        const char* vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in mat4 aInstanceMatrix;

            out vec3 FragPos;
            out vec3 Normal;

            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;

                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";

        const char* fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 FragPos;
            in vec3 Normal;

            uniform vec3 lightPos;
            uniform vec3 viewPos;

            void main()
            {
                vec3 color = vec3(0.5, 0.7, 0.9);
                vec3 normal = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);

                float diff = max(dot(normal, lightDir), 0.0);
                vec3 diffuse = diff * color;

                vec3 ambient = 0.2 * color;

                FragColor = vec4(ambient + diffuse, 1.0);
            }
        )";

        // Compile and link shaders
        // ... (shader compilation code)
    }

    void render(const glm::mat4& view, const glm::mat4& projection)
    {
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instanceCount);
        glBindVertexArray(0);
    }

    ~MatrixInstancedRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &instanceVBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 3-7. Additional Advanced Techniques

### GPU Instancing with Compute Shaders

```cpp
class GPUInstancedRenderer
{
public:
    struct InstanceData
    {
        glm::mat4 transform;
        glm::vec4 color;
        float visible; // For culling
    };

    GLuint instanceSSBO;  // Shader Storage Buffer Object

    void createInstanceBuffer(int count)
    {
        std::vector<InstanceData> instances(count);

        // Initialize instance data
        for(int i = 0; i < count; ++i)
        {
            // Generate random transforms
            instances[i].transform = generateRandomTransform();
            instances[i].color = generateRandomColor();
            instances[i].visible = 1.0f;
        }

        // Create SSBO
        glGenBuffers(1, &instanceSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     instances.size() * sizeof(InstanceData),
                     instances.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, instanceSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    glm::mat4 generateRandomTransform()
    {
        glm::mat4 model(1.0f);
        float x = (rand() % 200 - 100) / 10.0f;
        float y = (rand() % 200 - 100) / 10.0f;
        float z = (rand() % 200 - 100) / 10.0f;
        model = glm::translate(model, glm::vec3(x, y, z));
        return model;
    }

    glm::vec4 generateRandomColor()
    {
        return glm::vec4(rand() / (float)RAND_MAX,
                        rand() / (float)RAND_MAX,
                        rand() / (float)RAND_MAX,
                        1.0f);
    }
};
```

### Performance Comparison

```cpp
class InstancingBenchmark
{
public:
    static void compareRenderingMethods(int objectCount)
    {
        std::cout << "=== Instancing Performance ===" << std::endl;
        std::cout << "Objects: " << objectCount << std::endl;

        // Without instancing (individual draw calls)
        float individualTime = objectCount * 0.01f; // Approximate

        // With instancing
        float instancingTime = 0.1f; // Single draw call overhead

        std::cout << "Individual draws: " << individualTime << "ms" << std::endl;
        std::cout << "Instanced draw: " << instancingTime << "ms" << std::endl;
        std::cout << "Speedup: " << (individualTime / instancingTime) << "x" << std::endl;

        std::cout << "\nBenefits:" << std::endl;
        std::cout << "- Reduced CPU overhead" << std::endl;
        std::cout << "- Fewer state changes" << std::endl;
        std::cout << "- Better GPU utilization" << std::endl;
        std::cout << "- Scalable to millions of objects" << std::endl;
    }
};
```

---

## Key Takeaways

1. **Instancing renders many objects with one draw call**
2. **Use glDrawArraysInstanced or glDrawElementsInstanced**
3. **Instance attributes use glVertexAttribDivisor**
4. **Mat4 requires 4 attribute locations**
5. **SSBO allows dynamic instance data**
6. **Massive performance improvement for many objects**
7. **Perfect for particles, vegetation, crowds**
8. **Combine with frustum culling for optimization**

---

## Summary Checklist

- [ ] Understand instancing concept
- [ ] Implement basic instancing
- [ ] Use instanced vertex attributes
- [ ] Pass transformation matrices
- [ ] Add instance colors/data
- [ ] Implement frustum culling for instances
- [ ] Benchmark performance gains
- [ ] Optimize for large instance counts

---

## Next Lesson Preview

In **Lesson 75**, we'll explore:
- Level of Detail (LOD) systems
- Distance-based LOD switching
- Automatic LOD generation
- Continuous LOD
- Performance impact
- Best practices

**Instancing mastered!**
