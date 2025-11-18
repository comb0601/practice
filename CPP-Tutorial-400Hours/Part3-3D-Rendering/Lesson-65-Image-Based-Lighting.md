# Lesson 65: Image Based Lighting (IBL)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to IBL
2. Environment Maps and Cubemaps
3. Irradiance Map Generation
4. Specular IBL and Prefiltering
5. BRDF Integration Map
6. Complete IBL Implementation
7. Performance Optimization
8. Exercises and Projects

---

## 1. Introduction to IBL

### What is Image Based Lighting?

IBL uses environment maps (typically HDR) to provide realistic ambient and reflective lighting for PBR materials.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

class IBLManager
{
public:
    // IBL textures
    GLuint envCubemap;           // Original environment map
    GLuint irradianceMap;        // Diffuse irradiance
    GLuint prefilterMap;         // Specular prefiltered
    GLuint brdfLUT;              // BRDF integration lookup table

    int envMapSize;
    int irradianceSize;
    int prefilterSize;
    int brdfSize;

    IBLManager(int envSize = 512, int irrSize = 32,
               int prefilSize = 128, int brdfS = 512)
        : envMapSize(envSize), irradianceSize(irrSize),
          prefilterSize(prefilSize), brdfSize(brdfS)
    {
        std::cout << "=== IBL Manager Initialized ===" << std::endl;
        std::cout << "Environment Map: " << envMapSize << "x" << envMapSize << std::endl;
        std::cout << "Irradiance Map: " << irradianceSize << "x" << irradianceSize << std::endl;
        std::cout << "Prefilter Map: " << prefilterSize << "x" << prefilterSize << std::endl;
        std::cout << "BRDF LUT: " << brdfSize << "x" << brdfSize << std::endl;
    }

    void createFromHDR(const std::string& hdrPath)
    {
        // Load HDR image
        // Convert equirectangular to cubemap
        // Generate irradiance map
        // Generate prefilter map
        // Generate BRDF LUT
    }

    void bindForRendering(GLuint shaderProgram)
    {
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glUniform1i(glGetUniformLocation(shaderProgram, "irradianceMap"), 10);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glUniform1i(glGetUniformLocation(shaderProgram, "prefilterMap"), 11);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        glUniform1i(glGetUniformLocation(shaderProgram, "brdfLUT"), 12);
    }

    ~IBLManager()
    {
        glDeleteTextures(1, &envCubemap);
        glDeleteTextures(1, &irradianceMap);
        glDeleteTextures(1, &prefilterMap);
        glDeleteTextures(1, &brdfLUT);
    }
};
```

---

## 2. Environment Maps and Cubemaps

### Equirectangular to Cubemap Conversion

```cpp
class CubemapConverter
{
public:
    static GLuint createCubemap(int size)
    {
        GLuint cubemap;
        glGenTextures(1, &cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                        size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return cubemap;
    }

    static const char* getEquirectToCubemapShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec3 localPos;

            uniform sampler2D equirectangularMap;

            const vec2 invAtan = vec2(0.1591, 0.3183);

            vec2 SampleSphericalMap(vec3 v)
            {
                vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
                uv *= invAtan;
                uv += 0.5;
                return uv;
            }

            void main()
            {
                vec2 uv = SampleSphericalMap(normalize(localPos));
                vec3 color = texture(equirectangularMap, uv).rgb;

                FragColor = vec4(color, 1.0);
            }
        )";
    }

    static GLuint convertEquirectangularToCubemap(GLuint hdrTexture, int size)
    {
        // Create cubemap
        GLuint envCubemap = createCubemap(size);

        // Setup framebuffer
        GLuint captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, captureRBO);

        // Projection and view matrices for cubemap faces
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),
                                                       1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // Render cubemap faces (shader and rendering code needed)
        // ...

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);

        return envCubemap;
    }
};
```

---

## 3. Irradiance Map Generation

### Convolution for Diffuse IBL

```cpp
class IrradianceGenerator
{
public:
    static const char* getIrradianceConvolutionShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec3 localPos;

            uniform samplerCube environmentMap;

            const float PI = 3.14159265359;

            void main()
            {
                // The sample direction equals the hemisphere's orientation
                vec3 normal = normalize(localPos);

                vec3 irradiance = vec3(0.0);

                vec3 up    = vec3(0.0, 1.0, 0.0);
                vec3 right = normalize(cross(up, normal));
                up         = normalize(cross(normal, right));

                float sampleDelta = 0.025;
                float nrSamples = 0.0;

                for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
                {
                    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
                    {
                        // Spherical to cartesian (in tangent space)
                        vec3 tangentSample = vec3(sin(theta) * cos(phi),
                                                 sin(theta) * sin(phi),
                                                 cos(theta));

                        // Tangent space to world
                        vec3 sampleVec = tangentSample.x * right +
                                       tangentSample.y * up +
                                       tangentSample.z * normal;

                        irradiance += texture(environmentMap, sampleVec).rgb *
                                    cos(theta) * sin(theta);
                        nrSamples++;
                    }
                }

                irradiance = PI * irradiance * (1.0 / float(nrSamples));

                FragColor = vec4(irradiance, 1.0);
            }
        )";
    }

    static GLuint generateIrradianceMap(GLuint envCubemap, int size = 32)
    {
        GLuint irradianceMap = CubemapConverter::createCubemap(size);

        // Setup framebuffer for rendering
        GLuint captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);

        // Render each cubemap face
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),
                                                       1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        glViewport(0, 0, size, size);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        for(unsigned int i = 0; i < 6; ++i)
        {
            // Set view matrix and render
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                  irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Render cube with irradiance shader
            // ...
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);

        return irradianceMap;
    }
};
```

---

## 4. Specular IBL and Prefiltering

### Prefiltered Environment Map

```cpp
class PrefilterGenerator
{
public:
    static const char* getPrefilterShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec3 localPos;

            uniform samplerCube environmentMap;
            uniform float roughness;

            const float PI = 3.14159265359;

            float DistributionGGX(vec3 N, vec3 H, float roughness)
            {
                float a = roughness * roughness;
                float a2 = a * a;
                float NdotH = max(dot(N, H), 0.0);
                float NdotH2 = NdotH * NdotH;

                float nom = a2;
                float denom = (NdotH2 * (a2 - 1.0) + 1.0);
                denom = PI * denom * denom;

                return nom / denom;
            }

            float RadicalInverse_VdC(uint bits)
            {
                bits = (bits << 16u) | (bits >> 16u);
                bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                return float(bits) * 2.3283064365386963e-10;
            }

            vec2 Hammersley(uint i, uint N)
            {
                return vec2(float(i)/float(N), RadicalInverse_VdC(i));
            }

            vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
            {
                float a = roughness * roughness;

                float phi = 2.0 * PI * Xi.x;
                float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
                float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

                // Spherical to cartesian
                vec3 H;
                H.x = cos(phi) * sinTheta;
                H.y = sin(phi) * sinTheta;
                H.z = cosTheta;

                // Tangent to world space
                vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                vec3 tangent = normalize(cross(up, N));
                vec3 bitangent = cross(N, tangent);

                vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
                return normalize(sampleVec);
            }

            void main()
            {
                vec3 N = normalize(localPos);
                vec3 R = N;
                vec3 V = R;

                const uint SAMPLE_COUNT = 1024u;
                vec3 prefilteredColor = vec3(0.0);
                float totalWeight = 0.0;

                for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                {
                    vec2 Xi = Hammersley(i, SAMPLE_COUNT);
                    vec3 H = ImportanceSampleGGX(Xi, N, roughness);
                    vec3 L = normalize(2.0 * dot(V, H) * H - V);

                    float NdotL = max(dot(N, L), 0.0);
                    if(NdotL > 0.0)
                    {
                        prefilteredColor += texture(environmentMap, L).rgb * NdotL;
                        totalWeight += NdotL;
                    }
                }

                prefilteredColor = prefilteredColor / totalWeight;

                FragColor = vec4(prefilteredColor, 1.0);
            }
        )";
    }

    static GLuint generatePrefilterMap(GLuint envCubemap, int size = 128)
    {
        GLuint prefilterMap;
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);

        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                        size, size, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // Generate mipmap levels for different roughness values
        GLuint captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),
                                                       1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        unsigned int maxMipLevels = 5;
        for(unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            unsigned int mipWidth = size * std::pow(0.5, mip);
            unsigned int mipHeight = size * std::pow(0.5, mip);

            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                                mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);

            for(unsigned int i = 0; i < 6; ++i)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                      prefilterMap, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Render cube with prefilter shader (roughness set)
                // ...
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);

        return prefilterMap;
    }
};
```

---

## 5-8. Complete Implementation

### BRDF Integration Map

```cpp
class BRDFIntegration
{
public:
    static const char* getBRDFIntegrationShader()
    {
        return R"(
            #version 330 core
            out vec2 FragColor;
            in vec2 TexCoords;

            const float PI = 3.14159265359;

            float RadicalInverse_VdC(uint bits);
            vec2 Hammersley(uint i, uint N);
            vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
            float GeometrySchlickGGX(float NdotV, float roughness);
            float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

            vec2 IntegrateBRDF(float NdotV, float roughness)
            {
                vec3 V;
                V.x = sqrt(1.0 - NdotV*NdotV);
                V.y = 0.0;
                V.z = NdotV;

                float A = 0.0;
                float B = 0.0;

                vec3 N = vec3(0.0, 0.0, 1.0);

                const uint SAMPLE_COUNT = 1024u;
                for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                {
                    vec2 Xi = Hammersley(i, SAMPLE_COUNT);
                    vec3 H = ImportanceSampleGGX(Xi, N, roughness);
                    vec3 L = normalize(2.0 * dot(V, H) * H - V);

                    float NdotL = max(L.z, 0.0);
                    float NdotH = max(H.z, 0.0);
                    float VdotH = max(dot(V, H), 0.0);

                    if(NdotL > 0.0)
                    {
                        float G = GeometrySmith(N, V, L, roughness);
                        float G_Vis = (G * VdotH) / (NdotH * NdotV);
                        float Fc = pow(1.0 - VdotH, 5.0);

                        A += (1.0 - Fc) * G_Vis;
                        B += Fc * G_Vis;
                    }
                }

                A /= float(SAMPLE_COUNT);
                B /= float(SAMPLE_COUNT);

                return vec2(A, B);
            }

            void main()
            {
                vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
                FragColor = integratedBRDF;
            }

            // Implementation of helper functions
            // ...
        )";
    }

    static GLuint generateBRDFLUT(int size = 512)
    {
        GLuint brdfLUT;
        glGenTextures(1, &brdfLUT);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, size, size, 0,
                     GL_RG, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Render BRDF integration to texture
        GLuint captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, brdfLUT, 0);

        glViewport(0, 0, size, size);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render quad with BRDF integration shader
        // ...

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);

        return brdfLUT;
    }
};

// Complete IBL in PBR shader
class PBRWithIBL
{
public:
    static const char* getIBLFragmentShader()
    {
        return R"(
            // ... (previous PBR shader code)

            uniform samplerCube irradianceMap;
            uniform samplerCube prefilterMap;
            uniform sampler2D brdfLUT;

            void main()
            {
                // ... (calculate material properties and N, V)

                // ... (direct lighting loop)

                // IBL contribution
                vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

                vec3 kS = F;
                vec3 kD = 1.0 - kS;
                kD *= 1.0 - metallic;

                vec3 irradiance = texture(irradianceMap, N).rgb;
                vec3 diffuse = irradiance * albedo;

                vec3 R = reflect(-V, N);
                const float MAX_REFLECTION_LOD = 4.0;
                vec3 prefilteredColor = textureLod(prefilterMap, R,
                                                   roughness * MAX_REFLECTION_LOD).rgb;
                vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
                vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

                vec3 ambient = (kD * diffuse + specular) * ao;

                vec3 color = ambient + Lo;

                // HDR tonemapping and gamma correction
                color = color / (color + vec3(1.0));
                color = pow(color, vec3(1.0/2.2));

                FragColor = vec4(color, 1.0);
            }
        )";
    }
};
```

---

## Key Takeaways

1. **IBL provides realistic ambient lighting**
2. **Irradiance map for diffuse IBL**
3. **Prefiltered environment map for specular IBL**
4. **BRDF LUT for split-sum approximation**
5. **Importance sampling critical for quality**
6. **Mipmap levels represent roughness**
7. **Complete PBR requires IBL**
8. **Performance depends on map resolutions**

---

## Summary Checklist

- [ ] Understand IBL principles
- [ ] Convert equirectangular to cubemap
- [ ] Generate irradiance map
- [ ] Generate prefilter map with mipmaps
- [ ] Generate BRDF integration LUT
- [ ] Integrate IBL with PBR shader
- [ ] Optimize map resolutions
- [ ] Test with various environment maps

---

## Next Lesson Preview

In **Lesson 66**, we'll explore:
- HDR rendering pipeline
- Tone mapping operators
- Exposure control
- Bloom preparation
- Color grading
- Complete HDR workflow

**IBL mastered!**
