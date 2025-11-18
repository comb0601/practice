# Lesson 80: Particle Systems

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Particle Systems
2. CPU-Based Particle System
3. GPU-Based Particles with Compute Shaders
4. Particle Emitters and Affectors
5. Billboard Rendering
6. Advanced Particle Effects
7. Performance Optimization
8. Exercises and Projects

---

## 1. Introduction to Particle Systems

### What are Particle Systems?

Particle systems simulate effects like fire, smoke, rain, explosions, and magic using many small sprites or meshes.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <iostream>

class ParticleSystem
{
public:
    struct Particle
    {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;
        float size;
        float life;
        float maxLife;

        Particle()
            : position(0.0f), velocity(0.0f),
              color(1.0f), size(1.0f),
              life(0.0f), maxLife(1.0f) {}
    };

private:
    std::vector<Particle> particles;
    int maxParticles;
    GLuint VAO, VBO;
    GLuint shaderProgram;

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randomFloat;

public:
    ParticleSystem(int maxCount = 1000)
        : maxParticles(maxCount), randomFloat(0.0f, 1.0f)
    {
        particles.reserve(maxParticles);
        setupBuffers();
        createShaders();
    }

    void setupBuffers()
    {
        // Particle billboard vertices
        float quadVertices[] = {
            -0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  1.0f, 0.0f,

            -0.5f,  0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                            4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                            4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }

    void createShaders()
    {
        const char* vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 aTexCoords;

            out vec2 TexCoords;
            out vec4 ParticleColor;

            uniform mat4 projection;
            uniform mat4 view;
            uniform vec3 position;
            uniform float size;
            uniform vec4 color;

            void main()
            {
                // Billboard: face camera
                vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
                vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

                vec3 vertexPosition = position
                    + cameraRight * aPos.x * size
                    + cameraUp * aPos.y * size;

                gl_Position = projection * view * vec4(vertexPosition, 1.0);
                TexCoords = aTexCoords;
                ParticleColor = color;
            }
        )";

        const char* fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoords;
            in vec4 ParticleColor;

            uniform sampler2D particleTexture;

            void main()
            {
                vec4 texColor = texture(particleTexture, TexCoords);
                FragColor = texColor * ParticleColor;
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

    void emit(const glm::vec3& position, int count = 1)
    {
        for(int i = 0; i < count && particles.size() < maxParticles; ++i)
        {
            Particle p;
            p.position = position;

            // Random velocity
            p.velocity = glm::vec3(
                randomFloat(generator) * 2.0f - 1.0f,
                randomFloat(generator) * 2.0f,
                randomFloat(generator) * 2.0f - 1.0f
            );

            // Random color
            p.color = glm::vec4(
                randomFloat(generator),
                randomFloat(generator),
                randomFloat(generator),
                1.0f
            );

            p.size = randomFloat(generator) * 0.5f + 0.2f;
            p.life = 0.0f;
            p.maxLife = randomFloat(generator) * 2.0f + 1.0f;

            particles.push_back(p);
        }
    }

    void update(float deltaTime)
    {
        for(auto it = particles.begin(); it != particles.end(); )
        {
            it->life += deltaTime;

            if(it->life > it->maxLife)
            {
                it = particles.erase(it);
                continue;
            }

            // Update physics
            it->velocity.y -= 9.8f * deltaTime; // Gravity
            it->position += it->velocity * deltaTime;

            // Fade out
            float lifeRatio = it->life / it->maxLife;
            it->color.a = 1.0f - lifeRatio;

            ++it;
        }
    }

    void render(const glm::mat4& view, const glm::mat4& projection,
               GLuint texture)
    {
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "particleTexture"), 0);

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glBindVertexArray(VAO);

        for(const auto& particle : particles)
        {
            glUniform3fv(glGetUniformLocation(shaderProgram, "position"),
                        1, &particle.position[0]);
            glUniform1f(glGetUniformLocation(shaderProgram, "size"),
                       particle.size);
            glUniform4fv(glGetUniformLocation(shaderProgram, "color"),
                        1, &particle.color[0]);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    int getParticleCount() const { return particles.size(); }

    ~ParticleSystem()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};
```

---

## 2-3. GPU-Based Particles

### Compute Shader Particle System

```cpp
class GPUParticleSystem
{
public:
    struct GPUParticle
    {
        glm::vec4 position;  // xyz = pos, w = size
        glm::vec4 velocity;  // xyz = vel, w = life
        glm::vec4 color;
    };

private:
    GLuint particleSSBO;
    GLuint computeShader;
    GLuint renderShader;
    GLuint VAO, VBO;
    int particleCount;

public:
    GPUParticleSystem(int count = 100000) : particleCount(count)
    {
        createComputeShader();
        createRenderShader();
        setupParticleBuffer();
        setupRenderBuffers();
    }

    void createComputeShader()
    {
        const char* computeSrc = R"(
            #version 430 core
            layout (local_size_x = 256) in;

            struct Particle
            {
                vec4 position;  // xyz = pos, w = size
                vec4 velocity;  // xyz = vel, w = life
                vec4 color;
            };

            layout(std430, binding = 0) buffer Particles
            {
                Particle particles[];
            };

            uniform float deltaTime;
            uniform vec3 emitterPos;

            void main()
            {
                uint id = gl_GlobalInvocationID.x;
                if(id >= particles.length())
                    return;

                Particle p = particles[id];

                // Update life
                p.velocity.w += deltaTime;

                // Reset dead particles
                if(p.velocity.w > 5.0)
                {
                    p.position.xyz = emitterPos;
                    p.velocity.xyz = vec3(
                        (fract(sin(float(id) * 43758.5453)) - 0.5) * 2.0,
                        (fract(sin(float(id) * 12.9898)) - 0.5) * 4.0 + 2.0,
                        (fract(sin(float(id) * 78.233)) - 0.5) * 2.0
                    );
                    p.velocity.w = 0.0;
                }

                // Update physics
                p.velocity.y -= 9.8 * deltaTime;
                p.position.xyz += p.velocity.xyz * deltaTime;

                // Update color (fade out)
                p.color.a = 1.0 - (p.velocity.w / 5.0);

                particles[id] = p;
            }
        )";

        // Compile compute shader
        GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &computeSrc, nullptr);
        glCompileShader(compute);

        computeShader = glCreateProgram();
        glAttachShader(computeShader, compute);
        glLinkProgram(computeShader);

        glDeleteShader(compute);
    }

    void createRenderShader()
    {
        const char* vertexSrc = R"(
            #version 430 core

            struct Particle
            {
                vec4 position;
                vec4 velocity;
                vec4 color;
            };

            layout(std430, binding = 0) buffer Particles
            {
                Particle particles[];
            };

            out vec4 ParticleColor;

            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                Particle p = particles[gl_VertexID];

                vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
                vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

                vec3 vertexPosition = p.position.xyz
                    + cameraRight * p.position.w * 0.5
                    + cameraUp * p.position.w * 0.5;

                gl_Position = projection * view * vec4(vertexPosition, 1.0);
                gl_PointSize = p.position.w * 10.0;
                ParticleColor = p.color;
            }
        )";

        const char* fragmentSrc = R"(
            #version 430 core
            out vec4 FragColor;
            in vec4 ParticleColor;

            void main()
            {
                vec2 coord = gl_PointCoord - vec2(0.5);
                float dist = length(coord);

                if(dist > 0.5)
                    discard;

                FragColor = ParticleColor * (1.0 - dist * 2.0);
            }
        )";

        // Compile render shader
        // ... (shader compilation code)
    }

    void setupParticleBuffer()
    {
        std::vector<GPUParticle> particles(particleCount);

        // Initialize particles
        std::default_random_engine gen;
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        for(int i = 0; i < particleCount; ++i)
        {
            particles[i].position = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);
            particles[i].velocity = glm::vec4(dist(gen), dist(gen) + 2.0f,
                                             dist(gen), 0.0f);
            particles[i].color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }

        // Create SSBO
        glGenBuffers(1, &particleSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     particles.size() * sizeof(GPUParticle),
                     particles.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void setupRenderBuffers()
    {
        glGenVertexArrays(1, &VAO);
    }

    void update(float deltaTime, const glm::vec3& emitterPos)
    {
        glUseProgram(computeShader);

        glUniform1f(glGetUniformLocation(computeShader, "deltaTime"), deltaTime);
        glUniform3fv(glGetUniformLocation(computeShader, "emitterPos"),
                    1, &emitterPos[0]);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);

        glDispatchCompute((particleCount + 255) / 256, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    void render(const glm::mat4& view, const glm::mat4& projection)
    {
        glUseProgram(renderShader);

        glUniformMatrix4fv(glGetUniformLocation(renderShader, "view"),
                          1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(renderShader, "projection"),
                          1, GL_FALSE, &projection[0][0]);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
        glEnable(GL_PROGRAM_POINT_SIZE);

        glBindVertexArray(VAO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);

        glDrawArrays(GL_POINTS, 0, particleCount);

        glBindVertexArray(0);

        glDisable(GL_PROGRAM_POINT_SIZE);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    ~GPUParticleSystem()
    {
        glDeleteBuffers(1, &particleSSBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(computeShader);
        glDeleteProgram(renderShader);
    }
};
```

---

## 4-8. Advanced Techniques

### Performance Tips

```cpp
class ParticleOptimization
{
public:
    static void printOptimizationTips()
    {
        std::cout << "=== Particle System Optimization ===" << std::endl;
        std::cout << "1. Use GPU compute shaders for 100k+ particles" << std::endl;
        std::cout << "2. Sort particles back-to-front for transparency" << std::endl;
        std::cout << "3. Use texture atlases to reduce state changes" << std::endl;
        std::cout << "4. Implement particle pooling/reuse" << std::endl;
        std::cout << "5. Use instanced rendering for billboards" << std::endl;
        std::cout << "6. Consider soft particles for better blending" << std::endl;
        std::cout << "7. Use LOD for distant particle systems" << std::endl;
        std::cout << "8. Profile and optimize hot paths" << std::endl;
    }
};
```

---

## Key Takeaways

1. **Particle systems simulate natural effects**
2. **CPU particles good for <10k particles**
3. **GPU compute shaders handle millions**
4. **Billboarding faces particles to camera**
5. **Blending critical for transparency**
6. **Sort particles for correct rendering**
7. **Emitters control particle spawning**
8. **Performance scales with particle count**

---

## Summary Checklist

- [ ] Implement CPU particle system
- [ ] Add GPU compute shader particles
- [ ] Implement billboard rendering
- [ ] Add particle emitters
- [ ] Handle transparency and blending
- [ ] Optimize for large particle counts
- [ ] Create various particle effects
- [ ] Profile and benchmark

---

## Course Completion

**Congratulations! You've completed the Advanced Rendering Techniques course!**

You've mastered:
- Deferred Rendering
- SSAO
- PBR Theory and Implementation
- IBL
- HDR and Tone Mapping
- Bloom
- Advanced Shadow Mapping
- Cubemaps and Skyboxes
- Environment Mapping
- Geometry/Tessellation/Compute Shaders
- Instancing
- LOD, Culling, and Optimization
- Post-Processing
- Anti-Aliasing
- Particle Systems

**You now have the skills to create professional-quality 3D rendering systems!**
