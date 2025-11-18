# Lesson 68: Shadow Mapping Advanced (CSM, PCF)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Advanced Shadow Mapping
2. Cascaded Shadow Maps (CSM)
3. Percentage Closer Filtering (PCF)
4. Variance Shadow Maps (VSM)
5. Shadow Artifacts and Solutions
6. Performance Optimization
7. Complete Shadow System
8. Exercises and Projects

---

## 1. Introduction to Advanced Shadow Mapping

### Shadow Mapping Challenges

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

class AdvancedShadowSystem
{
public:
    enum class ShadowTechnique
    {
        BASIC,
        PCF,
        VSM,
        CSM
    };

    struct ShadowConfig
    {
        int shadowMapSize;
        float shadowBias;
        int pcfSamples;
        bool useCSM;
        int cascadeCount;

        ShadowConfig()
            : shadowMapSize(2048), shadowBias(0.005f),
              pcfSamples(16), useCSM(true), cascadeCount(4) {}
    };

    static void explainChallenges()
    {
        std::cout << "=== Shadow Mapping Challenges ===" << std::endl;
        std::cout << "1. Shadow acne (z-fighting)" << std::endl;
        std::cout << "2. Peter panning (detached shadows)" << std::endl;
        std::cout << "3. Perspective aliasing" << std::endl;
        std::cout << "4. Limited resolution" << std::endl;
        std::cout << "5. Hard shadow edges" << std::endl;
        std::cout << "6. Large scene coverage" << std::endl;
    }
};
```

---

## 2. Cascaded Shadow Maps (CSM)

### CSM Implementation

```cpp
class CascadedShadowMaps
{
public:
    struct Cascade
    {
        glm::mat4 viewProjMatrix;
        float splitDepth;
        float farPlane;
    };

private:
    static const int MAX_CASCADES = 4;
    int cascadeCount;
    std::vector<Cascade> cascades;

    GLuint shadowMapFBO;
    GLuint shadowMapArray;  // Texture array for cascades
    int shadowMapSize;

public:
    CascadedShadowMaps(int cascades = 4, int mapSize = 2048)
        : cascadeCount(cascades), shadowMapSize(mapSize)
    {
        createShadowMapArray();
        std::cout << "CSM initialized with " << cascades << " cascades" << std::endl;
    }

    void createShadowMapArray()
    {
        glGenFramebuffers(1, &shadowMapFBO);

        glGenTextures(1, &shadowMapArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapArray);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F,
                     shadowMapSize, shadowMapSize, cascadeCount,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE,
                       GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapArray, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Shadow map framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::vector<float> calculateCascadeSplits(float nearPlane, float farPlane,
                                             float lambda = 0.5f)
    {
        // Practical split scheme (mix of uniform and logarithmic)
        std::vector<float> splits;
        splits.push_back(nearPlane);

        for(int i = 1; i < cascadeCount; ++i)
        {
            float p = (float)i / cascadeCount;

            // Logarithmic split
            float logSplit = nearPlane * pow(farPlane / nearPlane, p);

            // Uniform split
            float uniformSplit = nearPlane + (farPlane - nearPlane) * p;

            // Blend
            float split = lambda * logSplit + (1.0f - lambda) * uniformSplit;
            splits.push_back(split);
        }

        splits.push_back(farPlane);
        return splits;
    }

    void updateCascades(const glm::mat4& viewMatrix, const glm::mat4& projMatrix,
                       const glm::vec3& lightDir, float nearPlane, float farPlane)
    {
        cascades.clear();

        std::vector<float> splits = calculateCascadeSplits(nearPlane, farPlane);

        for(int i = 0; i < cascadeCount; ++i)
        {
            Cascade cascade;
            cascade.farPlane = splits[i + 1];

            // Calculate frustum corners for this cascade
            std::vector<glm::vec4> frustumCorners = getFrustumCornersWorldSpace(
                viewMatrix, projMatrix, splits[i], splits[i + 1]);

            // Calculate centroid
            glm::vec3 center(0.0f);
            for(const auto& corner : frustumCorners)
                center += glm::vec3(corner);
            center /= frustumCorners.size();

            // Calculate light view matrix
            glm::mat4 lightView = glm::lookAt(center - lightDir,
                                             center,
                                             glm::vec3(0.0f, 1.0f, 0.0f));

            // Calculate bounds in light space
            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::lowest();

            for(const auto& corner : frustumCorners)
            {
                glm::vec4 lightSpaceCorner = lightView * corner;
                minX = std::min(minX, lightSpaceCorner.x);
                maxX = std::max(maxX, lightSpaceCorner.x);
                minY = std::min(minY, lightSpaceCorner.y);
                maxY = std::max(maxY, lightSpaceCorner.y);
                minZ = std::min(minZ, lightSpaceCorner.z);
                maxZ = std::max(maxZ, lightSpaceCorner.z);
            }

            // Create orthographic projection
            glm::mat4 lightProj = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

            cascade.viewProjMatrix = lightProj * lightView;
            cascade.splitDepth = (splits[i + 1] - nearPlane) / (farPlane - nearPlane);

            cascades.push_back(cascade);
        }
    }

    std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& view,
                                                       const glm::mat4& proj,
                                                       float nearPlane, float farPlane)
    {
        glm::mat4 invViewProj = glm::inverse(proj * view);

        std::vector<glm::vec4> corners;
        for(int x = 0; x < 2; ++x)
        {
            for(int y = 0; y < 2; ++y)
            {
                for(int z = 0; z < 2; ++z)
                {
                    glm::vec4 pt = invViewProj * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f
                    );
                    corners.push_back(pt / pt.w);
                }
            }
        }

        return corners;
    }

    void renderShadowMap(int cascadeIndex)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 shadowMapArray, 0, cascadeIndex);

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Render scene from light's perspective using cascades[cascadeIndex].viewProjMatrix
    }

    const std::vector<Cascade>& getCascades() const { return cascades; }
    GLuint getShadowMapArray() const { return shadowMapArray; }

    ~CascadedShadowMaps()
    {
        glDeleteFramebuffers(1, &shadowMapFBO);
        glDeleteTextures(1, &shadowMapArray);
    }
};
```

---

## 3. Percentage Closer Filtering (PCF)

### PCF Implementation

```cpp
class PCFShadowFiltering
{
public:
    static const char* getPCFShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 FragPos;
            in vec3 Normal;
            in vec4 FragPosLightSpace;

            uniform sampler2D shadowMap;
            uniform vec3 lightPos;
            uniform vec3 viewPos;

            float ShadowCalculation(vec4 fragPosLightSpace, float bias)
            {
                // Perspective divide
                vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
                projCoords = projCoords * 0.5 + 0.5;

                if(projCoords.z > 1.0)
                    return 0.0;

                float currentDepth = projCoords.z;

                // PCF filtering
                float shadow = 0.0;
                vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

                for(int x = -1; x <= 1; ++x)
                {
                    for(int y = -1; y <= 1; ++y)
                    {
                        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
                    }
                }

                shadow /= 9.0;
                return shadow;
            }

            void main()
            {
                vec3 color = vec3(0.3);
                vec3 normal = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);

                // Diffuse
                float diff = max(dot(lightDir, normal), 0.0);

                // Shadow
                float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
                float shadow = ShadowCalculation(FragPosLightSpace, bias);

                vec3 lighting = (1.0 - shadow) * diff * color;
                FragColor = vec4(lighting, 1.0);
            }
        )";
    }

    // Poisson disk sampling for better PCF
    static const char* getPoissonPCFShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 FragPos;
            in vec3 Normal;
            in vec4 FragPosLightSpace;

            uniform sampler2D shadowMap;

            const vec2 poissonDisk[16] = vec2[](
                vec2(-0.94201624, -0.39906216),
                vec2(0.94558609, -0.76890725),
                vec2(-0.094184101, -0.92938870),
                vec2(0.34495938, 0.29387760),
                vec2(-0.91588581, 0.45771432),
                vec2(-0.81544232, -0.87912464),
                vec2(-0.38277543, 0.27676845),
                vec2(0.97484398, 0.75648379),
                vec2(0.44323325, -0.97511554),
                vec2(0.53742981, -0.47373420),
                vec2(-0.26496911, -0.41893023),
                vec2(0.79197514, 0.19090188),
                vec2(-0.24188840, 0.99706507),
                vec2(-0.81409955, 0.91437590),
                vec2(0.19984126, 0.78641367),
                vec2(0.14383161, -0.14100790)
            );

            float ShadowCalculationPoisson(vec4 fragPosLightSpace, float bias)
            {
                vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
                projCoords = projCoords * 0.5 + 0.5;

                if(projCoords.z > 1.0)
                    return 0.0;

                float currentDepth = projCoords.z;
                float shadow = 0.0;

                vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

                for(int i = 0; i < 16; ++i)
                {
                    float pcfDepth = texture(shadowMap,
                        projCoords.xy + poissonDisk[i] * texelSize * 2.0).r;
                    shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
                }

                shadow /= 16.0;
                return shadow;
            }

            void main()
            {
                // ... (similar to previous shader)
            }
        )";
    }

    // Adaptive PCF based on distance
    static const char* getAdaptivePCFShader()
    {
        return R"(
            #version 330 core

            float ShadowCalculationAdaptive(vec4 fragPosLightSpace, float bias,
                                           float distance)
            {
                vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
                projCoords = projCoords * 0.5 + 0.5;

                if(projCoords.z > 1.0)
                    return 0.0;

                float currentDepth = projCoords.z;

                // Adaptive kernel size based on distance
                int kernelSize = int(mix(1.0, 4.0, distance / 100.0));

                float shadow = 0.0;
                vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
                int samples = 0;

                for(int x = -kernelSize; x <= kernelSize; ++x)
                {
                    for(int y = -kernelSize; y <= kernelSize; ++y)
                    {
                        float pcfDepth = texture(shadowMap,
                            projCoords.xy + vec2(x, y) * texelSize).r;
                        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
                        samples++;
                    }
                }

                shadow /= float(samples);
                return shadow;
            }
        )";
    }
};
```

---

## 4. Variance Shadow Maps (VSM)

### VSM Implementation

```cpp
class VarianceShadowMaps
{
public:
    static const char* getVSMFragmentShader()
    {
        return R"(
            #version 330 core
            out vec2 moments;

            void main()
            {
                float depth = gl_FragCoord.z;

                // Store first and second moments
                moments.x = depth;
                moments.y = depth * depth;
            }
        )";
    }

    static const char* getVSMSampleShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 FragPos;
            in vec3 Normal;
            in vec4 FragPosLightSpace;

            uniform sampler2D shadowMap;  // Contains moments

            float linstep(float min, float max, float v)
            {
                return clamp((v - min) / (max - min), 0.0, 1.0);
            }

            float ChebyshevUpperBound(vec2 moments, float t, float minVariance)
            {
                // One-tailed Chebyshev
                float p = (t <= moments.x) ? 1.0 : 0.0;

                float variance = moments.y - (moments.x * moments.x);
                variance = max(variance, minVariance);

                float d = t - moments.x;
                float p_max = variance / (variance + d * d);

                return max(p, p_max);
            }

            float ShadowCalculationVSM(vec4 fragPosLightSpace)
            {
                vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
                projCoords = projCoords * 0.5 + 0.5;

                if(projCoords.z > 1.0)
                    return 0.0;

                vec2 moments = texture(shadowMap, projCoords.xy).xy;
                float depth = projCoords.z;

                float lit = ChebyshevUpperBound(moments, depth, 0.00001);

                // Reduce light bleeding
                return linstep(0.2, 1.0, lit);
            }

            void main()
            {
                // ... (use ShadowCalculationVSM)
            }
        )";
    }

    static GLuint createVSMFramebuffer(int width, int height)
    {
        GLuint vsmFBO;
        glGenFramebuffers(1, &vsmFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO);

        GLuint vsmTexture;
        glGenTextures(1, &vsmTexture);
        glBindTexture(GL_TEXTURE_2D, vsmTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0,
                     GL_RG, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, vsmTexture, 0);

        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                             width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, depthBuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "VSM framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return vsmFBO;
    }
};
```

---

## 5-8. Artifacts, Optimization, and Complete System

### Shadow Artifacts Solutions

```cpp
class ShadowArtifactFixes
{
public:
    static void explainArtifacts()
    {
        std::cout << "=== Shadow Artifacts and Solutions ===" << std::endl;

        std::cout << "\n1. Shadow Acne:" << std::endl;
        std::cout << "   Problem: Z-fighting on surfaces" << std::endl;
        std::cout << "   Solution: Add depth bias" << std::endl;
        std::cout << "   Code: bias = max(0.05 * (1.0 - dot(N, L)), 0.005)" << std::endl;

        std::cout << "\n2. Peter Panning:" << std::endl;
        std::cout << "   Problem: Shadows detached from objects" << std::endl;
        std::cout << "   Solution: Reduce bias, use front-face culling" << std::endl;

        std::cout << "\n3. Perspective Aliasing:" << std::endl;
        std::cout << "   Problem: Blocky shadows near camera" << std::endl;
        std::cout << "   Solution: Use Cascaded Shadow Maps" << std::endl;

        std::cout << "\n4. Edge Flickering:" << std::endl;
        std::cout << "   Problem: Shadows flicker at edges" << std::endl;
        std::cout << "   Solution: Stabilize shadow matrix, snap to texels" << std::endl;

        std::cout << "\n5. Light Bleeding (VSM):" << std::endl;
        std::cout << "   Problem: Shadows leak light" << std::endl;
        std::cout << "   Solution: Use linstep function to reduce bleeding" << std::endl;
    }

    // Stabilized shadow matrix for CSM
    static glm::mat4 stabilizeShadowMatrix(const glm::mat4& lightViewProj,
                                          float texelSize)
    {
        glm::mat4 stabilized = lightViewProj;

        // Snap to texel grid
        glm::vec4 origin = stabilized * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        origin *= 1.0f / origin.w;

        glm::vec2 roundedOrigin = glm::round(glm::vec2(origin) / texelSize) * texelSize;
        glm::vec2 roundOffset = roundedOrigin - glm::vec2(origin);

        stabilized[3][0] += roundOffset.x;
        stabilized[3][1] += roundOffset.y;

        return stabilized;
    }
};

// Complete shadow system with all techniques
class CompleteShadowSystem
{
private:
    CascadedShadowMaps* csm;
    AdvancedShadowSystem::ShadowConfig config;
    AdvancedShadowSystem::ShadowTechnique technique;

public:
    CompleteShadowSystem(const AdvancedShadowSystem::ShadowConfig& cfg)
        : config(cfg)
    {
        if(config.useCSM)
        {
            csm = new CascadedShadowMaps(config.cascadeCount,
                                        config.shadowMapSize);
            technique = AdvancedShadowSystem::ShadowTechnique::CSM;
        }

        std::cout << "Shadow system initialized" << std::endl;
    }

    void updateAndRender(const glm::mat4& viewMatrix,
                        const glm::mat4& projMatrix,
                        const glm::vec3& lightDir,
                        float nearPlane, float farPlane)
    {
        if(config.useCSM)
        {
            csm->updateCascades(viewMatrix, projMatrix, lightDir,
                              nearPlane, farPlane);

            for(int i = 0; i < config.cascadeCount; ++i)
            {
                csm->renderShadowMap(i);
                // Render scene here
            }
        }
    }

    ~CompleteShadowSystem()
    {
        if(csm) delete csm;
    }
};
```

---

## Key Takeaways

1. **CSM solves perspective aliasing**
2. **PCF creates soft shadow edges**
3. **VSM enables efficient filtering**
4. **Shadow bias prevents acne**
5. **Poisson disk improves PCF quality**
6. **Cascade splits critical for quality**
7. **Stabilization reduces flickering**
8. **Multiple techniques solve different problems**

---

## Summary Checklist

- [ ] Understand shadow mapping limitations
- [ ] Implement Cascaded Shadow Maps
- [ ] Add PCF filtering
- [ ] Implement VSM (optional)
- [ ] Fix shadow artifacts
- [ ] Optimize cascade splits
- [ ] Add shadow stabilization
- [ ] Test with complex scenes

---

## Next Lesson Preview

In **Lesson 69**, we'll explore:
- Cubemap creation and rendering
- Skybox implementation
- Environment mapping
- Reflection probes
- Cubemap optimization
- Dynamic cubemaps

**Advanced Shadow Mapping mastered!**
