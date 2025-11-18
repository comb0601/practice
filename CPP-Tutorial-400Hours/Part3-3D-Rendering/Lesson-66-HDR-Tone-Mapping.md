# Lesson 66: HDR and Tone Mapping

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to HDR
2. HDR Framebuffer Setup
3. Tone Mapping Operators
4. Exposure Control
5. Gamma Correction
6. Complete HDR Pipeline
7. Optimization Techniques
8. Exercises and Projects

---

## 1. Introduction to HDR

### What is High Dynamic Range (HDR)?

HDR rendering uses floating-point framebuffers to represent a wider range of color values than traditional [0,1] range, allowing for more realistic lighting.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class HDRFramebuffer
{
public:
    GLuint FBO;
    GLuint colorBuffer;
    GLuint depthBuffer;
    int width, height;

    HDRFramebuffer(int w, int h) : width(w), height(h)
    {
        createHDRFramebuffer();
    }

    void createHDRFramebuffer()
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create HDR color buffer (RGB16F or RGB32F)
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, colorBuffer, 0);

        // Create depth buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                             width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, depthBuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "HDR Framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~HDRFramebuffer()
    {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &colorBuffer);
        glDeleteRenderbuffers(1, &depthBuffer);
    }
};
```

---

## 2-3. Tone Mapping Operators

### Multiple Tone Mapping Algorithms

```cpp
class ToneMappingOperators
{
public:
    enum class Operator
    {
        REINHARD,
        REINHARD_EXTENDED,
        ACES,
        UNCHARTED2,
        EXPOSURE,
        LINEAR
    };

    static const char* getTonemappingShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D hdrBuffer;
            uniform int tonemapOperator;
            uniform float exposure;
            uniform float gamma;

            // Reinhard tone mapping
            vec3 Reinhard(vec3 color)
            {
                return color / (color + vec3(1.0));
            }

            // Reinhard extended
            vec3 ReinhardExtended(vec3 color, float maxWhite)
            {
                vec3 numerator = color * (1.0 + (color / vec3(maxWhite * maxWhite)));
                return numerator / (1.0 + color);
            }

            // ACES Filmic
            vec3 ACESFilm(vec3 x)
            {
                float a = 2.51;
                float b = 0.03;
                float c = 2.43;
                float d = 0.59;
                float e = 0.14;
                return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
            }

            // Uncharted 2
            vec3 Uncharted2Tonemap(vec3 x)
            {
                float A = 0.15;
                float B = 0.50;
                float C = 0.10;
                float D = 0.20;
                float E = 0.02;
                float F = 0.30;
                return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
            }

            vec3 Uncharted2(vec3 color)
            {
                float exposureBias = 2.0;
                vec3 curr = Uncharted2Tonemap(exposureBias * color);

                vec3 W = vec3(11.2);
                vec3 whiteScale = vec3(1.0) / Uncharted2Tonemap(W);
                return curr * whiteScale;
            }

            // Exposure tone mapping
            vec3 ExposureTonemap(vec3 color, float exposure)
            {
                return vec3(1.0) - exp(-color * exposure);
            }

            void main()
            {
                vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

                // Apply exposure
                hdrColor *= exposure;

                vec3 mapped;

                // Apply tone mapping
                if(tonemapOperator == 0)
                    mapped = Reinhard(hdrColor);
                else if(tonemapOperator == 1)
                    mapped = ReinhardExtended(hdrColor, 4.0);
                else if(tonemapOperator == 2)
                    mapped = ACESFilm(hdrColor);
                else if(tonemapOperator == 3)
                    mapped = Uncharted2(hdrColor);
                else if(tonemapOperator == 4)
                    mapped = ExposureTonemap(hdrColor, 1.0);
                else
                    mapped = hdrColor;

                // Gamma correction
                mapped = pow(mapped, vec3(1.0 / gamma));

                FragColor = vec4(mapped, 1.0);
            }
        )";
    }
};

class ToneMappingManager
{
private:
    GLuint shader;
    GLuint quadVAO, quadVBO;
    ToneMappingOperators::Operator currentOperator;
    float exposure;
    float gamma;

public:
    ToneMappingManager()
        : currentOperator(ToneMappingOperators::Operator::ACES),
          exposure(1.0f), gamma(2.2f)
    {
        createShader();
        setupQuad();
    }

    void createShader()
    {
        // Compile shader using ToneMappingOperators::getTonemappingShader()
        // ... (shader compilation code)
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
                            (void*)(2*sizeof(float)));
        glBindVertexArray(0);
    }

    void render(GLuint hdrTexture)
    {
        glUseProgram(shader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glUniform1i(glGetUniformLocation(shader, "hdrBuffer"), 0);

        glUniform1i(glGetUniformLocation(shader, "tonemapOperator"),
                   static_cast<int>(currentOperator));
        glUniform1f(glGetUniformLocation(shader, "exposure"), exposure);
        glUniform1f(glGetUniformLocation(shader, "gamma"), gamma);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void setOperator(ToneMappingOperators::Operator op) { currentOperator = op; }
    void setExposure(float exp) { exposure = exp; }
    void setGamma(float g) { gamma = g; }

    ~ToneMappingManager()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(shader);
    }
};
```

---

## 4-6. Exposure and Complete Pipeline

### Auto-Exposure System

```cpp
class AutoExposure
{
private:
    GLuint luminanceFBO;
    GLuint luminanceTextures[2];
    GLuint avgLuminanceShader;
    int mipLevels;

public:
    AutoExposure(int width, int height)
    {
        // Calculate mip levels
        mipLevels = 1 + floor(log2(std::max(width, height)));
        createLuminanceBuffer(width, height);
    }

    void createLuminanceBuffer(int width, int height)
    {
        glGenFramebuffers(1, &luminanceFBO);
        glGenTextures(2, luminanceTextures);

        for(int i = 0; i < 2; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, luminanceTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0,
                        GL_RED, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                          GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    float calculateAverageLuminance(GLuint hdrTexture)
    {
        // Render scene luminance to texture
        // Generate mipmaps
        // Read 1x1 pixel at highest mip level
        // Return average luminance

        return 1.0f; // Placeholder
    }

    float calculateExposure(float avgLuminance, float targetLuminance = 0.5f)
    {
        // Simple exposure calculation
        return targetLuminance / (avgLuminance + 0.001f);
    }

    ~AutoExposure()
    {
        glDeleteFramebuffers(1, &luminanceFBO);
        glDeleteTextures(2, luminanceTextures);
        glDeleteProgram(avgLuminanceShader);
    }
};

// Complete HDR rendering pipeline
class HDRPipeline
{
private:
    HDRFramebuffer* hdrFB;
    ToneMappingManager* toneMapper;
    AutoExposure* autoExposure;

    int width, height;
    bool useAutoExposure;

public:
    HDRPipeline(int w, int h)
        : width(w), height(h), useAutoExposure(false)
    {
        hdrFB = new HDRFramebuffer(width, height);
        toneMapper = new ToneMappingManager();
        autoExposure = new AutoExposure(width, height);
    }

    void beginScene()
    {
        hdrFB->bind();
    }

    void endScene()
    {
        hdrFB->unbind();
    }

    void present()
    {
        // Calculate exposure if auto-exposure enabled
        if(useAutoExposure)
        {
            float avgLum = autoExposure->calculateAverageLuminance(
                hdrFB->colorBuffer);
            float exposure = autoExposure->calculateExposure(avgLum);
            toneMapper->setExposure(exposure);
        }

        // Render to screen with tone mapping
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        toneMapper->render(hdrFB->colorBuffer);
    }

    void setTonemapOperator(ToneMappingOperators::Operator op)
    {
        toneMapper->setOperator(op);
    }

    void setExposure(float exp)
    {
        toneMapper->setExposure(exp);
        useAutoExposure = false;
    }

    void enableAutoExposure(bool enable)
    {
        useAutoExposure = enable;
    }

    ~HDRPipeline()
    {
        delete hdrFB;
        delete toneMapper;
        delete autoExposure;
    }
};
```

---

## Key Takeaways

1. **HDR uses floating-point framebuffers**
2. **Tone mapping converts HDR to LDR**
3. **Multiple tone mapping operators available**
4. **Exposure control critical for brightness**
5. **Gamma correction essential for display**
6. **Auto-exposure creates dynamic adaptation**
7. **ACES widely used in production**
8. **HDR enables realistic lighting**

---

## Summary Checklist

- [ ] Create HDR framebuffer with float textures
- [ ] Implement multiple tone mapping operators
- [ ] Add exposure control
- [ ] Implement gamma correction
- [ ] Create auto-exposure system
- [ ] Integrate with existing renderer
- [ ] Test with high contrast scenes
- [ ] Optimize for performance

---

## Next Lesson Preview

In **Lesson 67**, we'll explore:
- Bloom effect implementation
- Bright pass extraction
- Gaussian blur
- Bloom composition
- HDR bloom
- Optimization techniques

**HDR and Tone Mapping mastered!**
