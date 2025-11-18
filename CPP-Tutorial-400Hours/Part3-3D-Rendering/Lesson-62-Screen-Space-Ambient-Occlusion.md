# Lesson 62: Screen Space Ambient Occlusion (SSAO)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to SSAO
2. SSAO Theory and Algorithm
3. Sample Kernel Generation
4. Noise Texture Creation
5. SSAO Shader Implementation
6. Blur Pass
7. Integration with Deferred Rendering
8. Exercises and Projects

---

## 1. Introduction to SSAO

### What is Ambient Occlusion?

Ambient Occlusion (AO) is a shading method that approximates how exposed each point in a scene is to ambient lighting. Areas that are occluded (hidden) receive less ambient light, creating subtle shadows in crevices, corners, and where objects meet.

### Screen Space Ambient Occlusion (SSAO)

SSAO is a real-time approximation of ambient occlusion calculated in screen space (using depth and normal buffers).

```cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>
#include <vector>
#include <random>

class SSAOManager
{
public:
    // SSAO framebuffer
    GLuint ssaoFBO;
    GLuint ssaoColorBuffer;
    GLuint ssaoBlurFBO;
    GLuint ssaoColorBufferBlur;

    // Noise texture
    GLuint noiseTexture;

    // Sample kernel
    std::vector<glm::vec3> ssaoKernel;

    // Shader programs
    GLuint ssaoShader;
    GLuint ssaoBlurShader;

    // Screen dimensions
    int width, height;

    SSAOManager(int w, int h) : width(w), height(h)
    {
        generateSampleKernel();
        generateNoiseTexture();
        setupSSAOBuffers();
        createShaders();
    }

    void generateSampleKernel()
    {
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        ssaoKernel.clear();

        for(unsigned int i = 0; i < 64; ++i)
        {
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            sample = glm::normalize(sample);
            sample *= randomFloats(generator);

            // Scale samples so they're more aligned to center of kernel
            float scale = (float)i / 64.0f;
            scale = 0.1f + (scale * scale) * 0.9f; // Lerp
            sample *= scale;

            ssaoKernel.push_back(sample);
        }
    }

    void generateNoiseTexture()
    {
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        std::vector<glm::vec3> ssaoNoise;
        for(unsigned int i = 0; i < 16; i++)
        {
            glm::vec3 noise(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f
            );
            ssaoNoise.push_back(noise);
        }

        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0,
                     GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void setupSSAOBuffers()
    {
        // SSAO framebuffer
        glGenFramebuffers(1, &ssaoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

        // SSAO color buffer
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                     GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, ssaoColorBuffer, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Framebuffer not complete!" << std::endl;

        // Blur framebuffer
        glGenFramebuffers(1, &ssaoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                     GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void createShaders()
    {
        // Will be implemented in following sections
    }

    ~SSAOManager()
    {
        glDeleteFramebuffers(1, &ssaoFBO);
        glDeleteFramebuffers(1, &ssaoBlurFBO);
        glDeleteTextures(1, &ssaoColorBuffer);
        glDeleteTextures(1, &ssaoColorBufferBlur);
        glDeleteTextures(1, &noiseTexture);
        glDeleteProgram(ssaoShader);
        glDeleteProgram(ssaoBlurShader);
    }
};
```

---

## 2. SSAO Theory and Algorithm

### The SSAO Algorithm

1. **For each pixel:**
   - Read position and normal from G-Buffer
   - Generate sample points in a hemisphere around the pixel
   - For each sample:
     - Project to screen space
     - Compare depth with G-Buffer depth
     - If sample is occluded, increase occlusion factor
   - Output occlusion value

### Mathematical Foundation

```cpp
class SSAOTheory
{
public:
    // SSAO parameters
    struct SSAOParams
    {
        float radius;        // Sampling radius
        float bias;          // Depth bias to prevent acne
        int kernelSize;      // Number of samples
        float power;         // Occlusion strength

        SSAOParams()
            : radius(0.5f), bias(0.025f),
              kernelSize(64), power(2.0f)
        {}
    };

    static float calculateOcclusion(
        const glm::vec3& fragPos,
        const glm::vec3& normal,
        const std::vector<glm::vec3>& kernel,
        const SSAOParams& params)
    {
        float occlusion = 0.0f;

        for(const auto& sample : kernel)
        {
            // Transform sample to world space
            glm::vec3 samplePos = fragPos + sample * params.radius;

            // Check if sample is occluded
            // (This is conceptual - actual implementation in shader)
            float sampleDepth = getSampleDepth(samplePos);
            float fragDepth = getFragmentDepth(fragPos);

            float rangeCheck = smoothstep(0.0f, 1.0f,
                params.radius / abs(fragPos.z - sampleDepth));

            occlusion += (sampleDepth >= samplePos.z + params.bias ? 1.0 : 0.0)
                        * rangeCheck;
        }

        occlusion = 1.0 - (occlusion / kernel.size());
        return pow(occlusion, params.power);
    }

private:
    static float getSampleDepth(const glm::vec3& pos) { return 0.0f; }
    static float getFragmentDepth(const glm::vec3& pos) { return 0.0f; }

    static float smoothstep(float edge0, float edge1, float x)
    {
        float t = glm::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
};
```

### Hemisphere Sample Distribution

```cpp
class HemisphereSampler
{
public:
    static std::vector<glm::vec3> generateHemisphereSamples(
        int numSamples, float minRadius = 0.1f)
    {
        std::vector<glm::vec3> samples;
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for(int i = 0; i < numSamples; ++i)
        {
            // Generate random point in hemisphere
            float theta = randomFloats(generator) * 2.0f * M_PI;
            float phi = acos(2.0f * randomFloats(generator) - 1.0f);

            float x = sin(phi) * cos(theta);
            float y = sin(phi) * sin(theta);
            float z = cos(phi);

            glm::vec3 sample(x, y, abs(z)); // Ensure positive Z
            sample = glm::normalize(sample);

            // Random length
            float length = randomFloats(generator);

            // Distribute samples more towards the center
            float scale = (float)i / (float)numSamples;
            scale = minRadius + (scale * scale) * (1.0f - minRadius);

            sample *= scale;
            samples.push_back(sample);
        }

        return samples;
    }

    static void visualizeSamples(const std::vector<glm::vec3>& samples)
    {
        std::cout << "=== SSAO Sample Kernel ===" << std::endl;
        std::cout << "Total samples: " << samples.size() << std::endl;

        // Calculate distribution statistics
        int innerSamples = 0;
        int outerSamples = 0;

        for(const auto& sample : samples)
        {
            float length = glm::length(sample);
            if(length < 0.5f)
                innerSamples++;
            else
                outerSamples++;
        }

        std::cout << "Inner samples (< 0.5): " << innerSamples << std::endl;
        std::cout << "Outer samples (>= 0.5): " << outerSamples << std::endl;

        // Show first few samples
        std::cout << "\nFirst 5 samples:" << std::endl;
        for(int i = 0; i < std::min(5, (int)samples.size()); ++i)
        {
            std::cout << "  [" << i << "] ("
                     << samples[i].x << ", "
                     << samples[i].y << ", "
                     << samples[i].z << ") - length: "
                     << glm::length(samples[i]) << std::endl;
        }
    }
};
```

---

## 3. Sample Kernel Generation

### Optimized Kernel Generator

```cpp
class OptimizedKernelGenerator
{
public:
    struct KernelConfig
    {
        int numSamples;
        float minDistance;
        float maxDistance;
        float distributionPower; // Higher = more samples near center

        KernelConfig()
            : numSamples(64), minDistance(0.01f),
              maxDistance(1.0f), distributionPower(2.0f)
        {}
    };

    static std::vector<glm::vec3> generateKernel(const KernelConfig& config)
    {
        std::vector<glm::vec3> kernel;
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for(int i = 0; i < config.numSamples; ++i)
        {
            // Generate random point in unit hemisphere
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            sample = glm::normalize(sample);

            // Vary length with distribution
            float scale = (float)i / (float)config.numSamples;

            // Apply power function for distribution
            scale = config.minDistance +
                   pow(scale, config.distributionPower) *
                   (config.maxDistance - config.minDistance);

            sample *= scale;
            kernel.push_back(sample);
        }

        return kernel;
    }

    // Generate kernel with Poisson disk sampling for better distribution
    static std::vector<glm::vec3> generatePoissonKernel(int numSamples)
    {
        std::vector<glm::vec3> kernel;
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        const int maxAttempts = 30;
        const float minDist = 0.05f;

        while(kernel.size() < numSamples)
        {
            bool validSample = false;
            glm::vec3 sample;

            for(int attempt = 0; attempt < maxAttempts; ++attempt)
            {
                // Generate candidate
                sample = glm::vec3(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator)
                );

                sample = glm::normalize(sample);
                sample *= randomFloats(generator);

                // Check distance to all existing samples
                bool tooClose = false;
                for(const auto& existingSample : kernel)
                {
                    if(glm::distance(sample, existingSample) < minDist)
                    {
                        tooClose = true;
                        break;
                    }
                }

                if(!tooClose)
                {
                    validSample = true;
                    break;
                }
            }

            if(validSample)
                kernel.push_back(sample);
            else
                break; // Can't find more valid samples
        }

        return kernel;
    }

    // Create TBN-oriented kernel
    static std::vector<glm::vec3> createTBNKernel(
        const glm::vec3& normal,
        const std::vector<glm::vec3>& kernel)
    {
        // Create TBN matrix
        glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        if(abs(glm::dot(normal, tangent)) > 0.9f)
            tangent = glm::vec3(0.0f, 1.0f, 0.0f);

        tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));
        glm::vec3 bitangent = glm::cross(normal, tangent);

        glm::mat3 TBN(tangent, bitangent, normal);

        // Transform kernel
        std::vector<glm::vec3> transformedKernel;
        for(const auto& sample : kernel)
        {
            transformedKernel.push_back(TBN * sample);
        }

        return transformedKernel;
    }
};
```

---

## 4. Noise Texture Creation

### Random Rotation Noise

```cpp
class NoiseTextureGenerator
{
public:
    static GLuint createRotationNoise(int size = 4)
    {
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        std::vector<glm::vec3> noise;
        for(int i = 0; i < size * size; ++i)
        {
            // Random rotation around Z-axis
            glm::vec3 rotation(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f
            );
            noise.push_back(rotation);
        }

        GLuint noiseTexture;
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size, size, 0,
                     GL_RGB, GL_FLOAT, &noise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return noiseTexture;
    }

    static GLuint createBlueNoise(int size = 64)
    {
        // Simplified blue noise generation
        // For production, load pre-generated blue noise texture
        std::vector<float> noise(size * size);
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for(int i = 0; i < size * size; ++i)
        {
            noise[i] = randomFloats(generator);
        }

        GLuint noiseTexture;
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size, size, 0,
                     GL_RED, GL_FLOAT, noise.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return noiseTexture;
    }

    static void visualizeNoise(GLuint texture, int size)
    {
        std::vector<glm::vec3> data(size * size);
        glBindTexture(GL_TEXTURE_2D, texture);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data.data());

        std::cout << "=== Noise Texture Visualization ===" << std::endl;
        std::cout << "Size: " << size << "x" << size << std::endl;

        // Show corner samples
        std::cout << "\nCorner samples:" << std::endl;
        std::cout << "Top-left: (" << data[0].x << ", "
                 << data[0].y << ", " << data[0].z << ")" << std::endl;
        std::cout << "Top-right: (" << data[size-1].x << ", "
                 << data[size-1].y << ", " << data[size-1].z << ")" << std::endl;
        std::cout << "Bottom-left: (" << data[(size-1)*size].x << ", "
                 << data[(size-1)*size].y << ", "
                 << data[(size-1)*size].z << ")" << std::endl;
        std::cout << "Bottom-right: (" << data[size*size-1].x << ", "
                 << data[size*size-1].y << ", "
                 << data[size*size-1].z << ")" << std::endl;
    }
};
```

---

## 5. SSAO Shader Implementation

### Complete SSAO Shader

```cpp
class SSAOShader
{
public:
    static const char* getVertexShader()
    {
        return R"(
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
    }

    static const char* getFragmentShader()
    {
        return R"(
            #version 330 core
            out float FragColor;

            in vec2 TexCoords;

            uniform sampler2D gPosition;
            uniform sampler2D gNormal;
            uniform sampler2D texNoise;

            uniform vec3 samples[64];
            uniform mat4 projection;
            uniform mat4 view;

            // SSAO parameters
            uniform float radius;
            uniform float bias;
            uniform int kernelSize;
            uniform float power;

            // Tile noise over screen based on screen dimensions / noise size
            const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0);

            void main()
            {
                // Get input for SSAO algorithm
                vec3 fragPos = texture(gPosition, TexCoords).xyz;
                vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
                vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

                // Create TBN change-of-basis matrix: from tangent-space to view-space
                vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
                vec3 bitangent = cross(normal, tangent);
                mat3 TBN = mat3(tangent, bitangent, normal);

                // Iterate over the sample kernel and calculate occlusion factor
                float occlusion = 0.0;
                for(int i = 0; i < kernelSize; ++i)
                {
                    // Get sample position
                    vec3 samplePos = TBN * samples[i]; // From tangent to view-space
                    samplePos = fragPos + samplePos * radius;

                    // Project sample position (to sample texture)
                    vec4 offset = vec4(samplePos, 1.0);
                    offset = projection * offset;    // from view to clip-space
                    offset.xyz /= offset.w;          // perspective divide
                    offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

                    // Get sample depth
                    float sampleDepth = texture(gPosition, offset.xy).z;

                    // Range check & accumulate
                    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
                    occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
                }

                occlusion = 1.0 - (occlusion / kernelSize);
                FragColor = pow(occlusion, power);
            }
        )";
    }

    static GLuint createProgram()
    {
        const char* vertexSrc = getVertexShader();
        const char* fragmentSrc = getFragmentShader();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSrc, nullptr);
        glCompileShader(vertex);

        // Check for errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "Vertex Shader Error: " << infoLog << std::endl;
        }

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSrc, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "Fragment Shader Error: " << infoLog << std::endl;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Program Linking Error: " << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }
};
```

### SSAO Pass Implementation

```cpp
class SSAOPass
{
private:
    GLuint quadVAO, quadVBO;
    GLuint ssaoShader;
    GLuint ssaoFBO;
    GLuint ssaoTexture;

    std::vector<glm::vec3> kernel;
    GLuint noiseTexture;

    int width, height;

    struct SSAOConfig
    {
        float radius = 0.5f;
        float bias = 0.025f;
        int kernelSize = 64;
        float power = 1.5f;
    } config;

public:
    SSAOPass(int w, int h) : width(w), height(h)
    {
        setupQuad();
        kernel = OptimizedKernelGenerator::generateKernel(
            OptimizedKernelGenerator::KernelConfig());
        noiseTexture = NoiseTextureGenerator::createRotationNoise(4);
        ssaoShader = SSAOShader::createProgram();
        setupFramebuffer();
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void setupFramebuffer()
    {
        glGenFramebuffers(1, &ssaoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

        glGenTextures(1, &ssaoTexture);
        glBindTexture(GL_TEXTURE_2D, ssaoTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                     GL_RED, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, ssaoTexture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "SSAO framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void render(GLuint gPosition, GLuint gNormal,
               const glm::mat4& projection, const glm::mat4& view)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ssaoShader);

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);

        // Set uniforms
        glUniform1i(glGetUniformLocation(ssaoShader, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(ssaoShader, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(ssaoShader, "texNoise"), 2);

        // Send kernel samples
        for(int i = 0; i < kernel.size(); ++i)
        {
            std::string name = "samples[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(ssaoShader, name.c_str()),
                        1, &kernel[i][0]);
        }

        glUniformMatrix4fv(glGetUniformLocation(ssaoShader, "projection"),
                          1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(ssaoShader, "view"),
                          1, GL_FALSE, &view[0][0]);

        glUniform1f(glGetUniformLocation(ssaoShader, "radius"), config.radius);
        glUniform1f(glGetUniformLocation(ssaoShader, "bias"), config.bias);
        glUniform1i(glGetUniformLocation(ssaoShader, "kernelSize"),
                   config.kernelSize);
        glUniform1f(glGetUniformLocation(ssaoShader, "power"), config.power);

        // Render quad
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getSSAOTexture() const { return ssaoTexture; }

    void setConfig(const SSAOConfig& newConfig)
    {
        config = newConfig;
    }

    ~SSAOPass()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteFramebuffers(1, &ssaoFBO);
        glDeleteTextures(1, &ssaoTexture);
        glDeleteTextures(1, &noiseTexture);
        glDeleteProgram(ssaoShader);
    }
};
```

---

## 6. Blur Pass

### Bilateral Blur for SSAO

```cpp
class SSAOBlurPass
{
private:
    GLuint blurShader;
    GLuint blurFBO;
    GLuint blurTexture;
    GLuint quadVAO, quadVBO;
    int width, height;

public:
    SSAOBlurPass(int w, int h) : width(w), height(h)
    {
        setupQuad();
        createBlurShader();
        setupFramebuffer();
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void createBlurShader()
    {
        const char* vertexSrc = R"(
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

        const char* fragmentSrc = R"(
            #version 330 core
            out float FragColor;

            in vec2 TexCoords;

            uniform sampler2D ssaoInput;
            uniform vec2 texelSize;

            void main()
            {
                vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
                float result = 0.0;
                float totalWeight = 0.0;

                // Simple box blur
                for (int x = -2; x < 2; ++x)
                {
                    for (int y = -2; y < 2; ++y)
                    {
                        vec2 offset = vec2(float(x), float(y)) * texelSize;
                        float sample = texture(ssaoInput, TexCoords + offset).r;

                        // Simple weighting
                        float weight = 1.0;
                        result += sample * weight;
                        totalWeight += weight;
                    }
                }

                FragColor = result / totalWeight;
            }
        )";

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSrc, nullptr);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSrc, nullptr);
        glCompileShader(fragment);

        blurShader = glCreateProgram();
        glAttachShader(blurShader, vertex);
        glAttachShader(blurShader, fragment);
        glLinkProgram(blurShader);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void setupFramebuffer()
    {
        glGenFramebuffers(1, &blurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

        glGenTextures(1, &blurTexture);
        glBindTexture(GL_TEXTURE_2D, blurTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                     GL_RED, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, blurTexture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Blur framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void render(GLuint ssaoTexture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(blurShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoTexture);
        glUniform1i(glGetUniformLocation(blurShader, "ssaoInput"), 0);

        glm::vec2 texelSize(1.0f / width, 1.0f / height);
        glUniform2fv(glGetUniformLocation(blurShader, "texelSize"),
                    1, &texelSize[0]);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getBlurredTexture() const { return blurTexture; }

    ~SSAOBlurPass()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteFramebuffers(1, &blurFBO);
        glDeleteTextures(1, &blurTexture);
        glDeleteProgram(blurShader);
    }
};
```

---

## 7. Integration with Deferred Rendering

### Complete SSAO Pipeline

```cpp
class DeferredSSAORenderer
{
private:
    // G-Buffer
    GLuint gBuffer;
    GLuint gPosition, gNormal, gAlbedo;
    GLuint gDepth;

    // SSAO components
    std::unique_ptr<SSAOPass> ssaoPass;
    std::unique_ptr<SSAOBlurPass> blurPass;

    // Lighting
    GLuint lightingShader;
    GLuint quadVAO, quadVBO;

    int width, height;

public:
    DeferredSSAORenderer(int w, int h) : width(w), height(h)
    {
        setupGBuffer();
        ssaoPass = std::make_unique<SSAOPass>(width, height);
        blurPass = std::make_unique<SSAOBlurPass>(width, height);
        setupLighting();
    }

    void setupGBuffer()
    {
        // Setup G-Buffer (simplified - use from Lesson 61)
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        // Position, normal, albedo textures...
        // (Implementation from previous lesson)

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void setupLighting()
    {
        // Setup lighting quad and shader
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void*)(2 * sizeof(float)));
        glBindVertexArray(0);

        createLightingShader();
    }

    void createLightingShader()
    {
        const char* fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoords;

            uniform sampler2D gPosition;
            uniform sampler2D gNormal;
            uniform sampler2D gAlbedo;
            uniform sampler2D ssaoTexture;

            uniform vec3 lightPos;
            uniform vec3 lightColor;
            uniform vec3 viewPos;

            void main()
            {
                vec3 FragPos = texture(gPosition, TexCoords).rgb;
                vec3 Normal = texture(gNormal, TexCoords).rgb;
                vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
                float AO = texture(ssaoTexture, TexCoords).r;

                // Ambient with AO
                vec3 ambient = vec3(0.3) * Albedo * AO;

                // Diffuse
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(Normal, lightDir), 0.0);
                vec3 diffuse = diff * lightColor * Albedo;

                // Specular
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 halfwayDir = normalize(lightDir + viewDir);
                float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
                vec3 specular = spec * lightColor;

                vec3 lighting = ambient + diffuse + specular;
                FragColor = vec4(lighting, 1.0);
            }
        )";

        // Compile and link shader (simplified)
        // ...
    }

    void render(const glm::mat4& view, const glm::mat4& projection)
    {
        // 1. Geometry pass (render to G-Buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ... render scene geometry ...
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. SSAO pass
        ssaoPass->render(gPosition, gNormal, projection, view);

        // 3. Blur SSAO
        blurPass->render(ssaoPass->getSSAOTexture());

        // 4. Lighting pass with SSAO
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(lightingShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, blurPass->getBlurredTexture());

        // Set uniforms and render quad
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    ~DeferredSSAORenderer()
    {
        glDeleteFramebuffers(1, &gBuffer);
        glDeleteTextures(1, &gPosition);
        glDeleteTextures(1, &gNormal);
        glDeleteTextures(1, &gAlbedo);
        glDeleteRenderbuffers(1, &gDepth);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(lightingShader);
    }
};
```

---

## 8. Exercises and Projects

### Exercise 1: Basic SSAO

Implement basic SSAO with:
- 32 sample kernel
- 4x4 noise texture
- Simple box blur
- Integration with deferred renderer

### Exercise 2: SSAO Parameter Tuning

```cpp
class SSAOParameterTuner
{
public:
    struct SSAOSettings
    {
        float radius = 0.5f;
        float bias = 0.025f;
        int kernelSize = 64;
        float power = 1.5f;
        int blurSize = 4;
    };

    static void tuneParameters()
    {
        std::cout << "=== SSAO Parameter Tuning Guide ===" << std::endl;
        std::cout << "\nRadius:" << std::endl;
        std::cout << "  Small (0.1-0.3): Fine detail occlusion" << std::endl;
        std::cout << "  Medium (0.4-0.7): Balanced occlusion" << std::endl;
        std::cout << "  Large (0.8-2.0): Broad occlusion" << std::endl;

        std::cout << "\nBias:" << std::endl;
        std::cout << "  Too low: Acne artifacts" << std::endl;
        std::cout << "  Too high: Lost detail" << std::endl;
        std::cout << "  Typical: 0.01-0.05" << std::endl;

        std::cout << "\nKernel Size:" << std::endl;
        std::cout << "  16: Fast, noisy" << std::endl;
        std::cout << "  32: Balanced" << std::endl;
        std::cout << "  64: High quality, slower" << std::endl;

        std::cout << "\nPower:" << std::endl;
        std::cout << "  1.0: Linear" << std::endl;
        std::cout << "  1.5-2.0: Enhanced contrast" << std::endl;
        std::cout << "  >2.0: Very strong AO" << std::endl;
    }

    static SSAOSettings getPreset(const std::string& preset)
    {
        SSAOSettings settings;

        if(preset == "performance")
        {
            settings.radius = 0.4f;
            settings.bias = 0.03f;
            settings.kernelSize = 16;
            settings.power = 1.5f;
            settings.blurSize = 2;
        }
        else if(preset == "balanced")
        {
            settings.radius = 0.5f;
            settings.bias = 0.025f;
            settings.kernelSize = 32;
            settings.power = 1.5f;
            settings.blurSize = 4;
        }
        else if(preset == "quality")
        {
            settings.radius = 0.6f;
            settings.bias = 0.02f;
            settings.kernelSize = 64;
            settings.power = 2.0f;
            settings.blurSize = 4;
        }

        return settings;
    }
};
```

### Project: Complete SSAO System

Build a complete SSAO system with:
1. Deferred rendering integration
2. Adjustable parameters via UI
3. Multiple quality presets
4. Performance monitoring
5. Debug visualization modes
6. Bilateral blur
7. Temporal filtering (optional)

---

## Key Takeaways

1. **SSAO approximates ambient occlusion in screen space**
2. **Sample kernel distributed in hemisphere**
3. **Random noise reduces banding artifacts**
4. **Blur pass smooths results**
5. **Integration with deferred rendering**
6. **Parameter tuning critical for quality**
7. **Performance scalable with kernel size**
8. **Depth-aware blur preserves edges**

---

## Summary Checklist

Before moving to Lesson 63, ensure you can:
- [ ] Understand SSAO algorithm
- [ ] Generate sample kernels
- [ ] Create noise textures
- [ ] Implement SSAO shader
- [ ] Add blur pass
- [ ] Integrate with deferred renderer
- [ ] Tune SSAO parameters
- [ ] Optimize for performance
- [ ] Debug SSAO artifacts

---

## Next Lesson Preview

In **Lesson 63**, we'll explore:
- Physically Based Rendering (PBR) theory
- Cook-Torrance BRDF
- Fresnel equations
- Microfacet theory
- Energy conservation
- Material properties

**SSAO mastered!**
