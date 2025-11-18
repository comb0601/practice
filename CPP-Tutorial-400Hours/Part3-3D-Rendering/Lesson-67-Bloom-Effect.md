# Lesson 67: Bloom Effect

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Bloom
2. Bright Pass Extraction
3. Gaussian Blur Implementation
4. Two-Pass Blur Optimization
5. Bloom Composition
6. Advanced Bloom Techniques
7. Performance Optimization
8. Exercises and Projects

---

## 1. Introduction to Bloom

### What is Bloom?

Bloom simulates the light bleeding effect that occurs in cameras and eyes when viewing bright light sources. It makes bright areas glow.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class BloomEffect
{
public:
    struct BloomSettings
    {
        float threshold;      // Brightness threshold
        float intensity;      // Bloom strength
        int blurPasses;       // Number of blur iterations
        float radius;         // Blur radius

        BloomSettings()
            : threshold(1.0f), intensity(0.5f),
              blurPasses(5), radius(1.0f) {}
    };

private:
    GLuint brightFBO, brightTexture;
    GLuint pingpongFBO[2], pingpongTextures[2];
    GLuint brightPassShader, blurShader, composeShader;
    int width, height;
    BloomSettings settings;

public:
    BloomEffect(int w, int h) : width(w), height(h)
    {
        createFramebuffers();
        createShaders();
    }

    void createFramebuffers()
    {
        // Bright pass framebuffer
        glGenFramebuffers(1, &brightFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, brightFBO);

        glGenTextures(1, &brightTexture);
        glBindTexture(GL_TEXTURE_2D, brightTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, brightTexture, 0);

        // Ping-pong framebuffers for blur
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongTextures);

        for(int i = 0; i < 2; ++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                        GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_TEXTURE_2D, pingpongTextures[i], 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void createShaders()
    {
        // Create bright pass, blur, and compose shaders
    }

    void process(GLuint hdrTexture, GLuint& outputTexture)
    {
        // 1. Extract bright pixels
        extractBrightPixels(hdrTexture);

        // 2. Blur bright pixels
        blur();

        // 3. Compose with original (handled externally)
        outputTexture = pingpongTextures[0];
    }

    void extractBrightPixels(GLuint hdrTexture);
    void blur();

    ~BloomEffect()
    {
        glDeleteFramebuffers(1, &brightFBO);
        glDeleteFramebuffers(2, pingpongFBO);
        glDeleteTextures(1, &brightTexture);
        glDeleteTextures(2, pingpongTextures);
        glDeleteProgram(brightPassShader);
        glDeleteProgram(blurShader);
        glDeleteProgram(composeShader);
    }
};
```

---

## 2. Bright Pass Extraction

### Extract Bright Pixels Shader

```cpp
class BrightPassShader
{
public:
    static const char* getFragmentShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D hdrBuffer;
            uniform float threshold;

            void main()
            {
                vec3 color = texture(hdrBuffer, TexCoords).rgb;

                // Calculate luminance
                float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));

                // Extract bright pixels
                if(luminance > threshold)
                {
                    FragColor = vec4(color, 1.0);
                }
                else
                {
                    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
                }
            }
        )";
    }

    // Alternative: Smooth threshold
    static const char* getSmoothBrightPassShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D hdrBuffer;
            uniform float threshold;
            uniform float softThreshold;

            void main()
            {
                vec3 color = texture(hdrBuffer, TexCoords).rgb;
                float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));

                // Smooth threshold
                float knee = threshold * softThreshold;
                float soft = luminance - threshold + knee;
                soft = clamp(soft, 0.0, 2.0 * knee);
                soft = soft * soft / (4.0 * knee + 0.00001);

                float contribution = max(soft, luminance - threshold);
                contribution /= max(luminance, 0.00001);

                FragColor = vec4(color * contribution, 1.0);
            }
        )";
    }
};

void BloomEffect::extractBrightPixels(GLuint hdrTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, brightFBO);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(brightPassShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glUniform1i(glGetUniformLocation(brightPassShader, "hdrBuffer"), 0);
    glUniform1f(glGetUniformLocation(brightPassShader, "threshold"),
               settings.threshold);

    // Render full-screen quad
    // ...

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
```

---

## 3-4. Gaussian Blur Implementation

### Optimized Two-Pass Gaussian Blur

```cpp
class GaussianBlur
{
public:
    static std::vector<float> generateGaussianWeights(int radius)
    {
        std::vector<float> weights;
        float sigma = radius / 3.0f;
        float sum = 0.0f;

        for(int i = -radius; i <= radius; ++i)
        {
            float weight = exp(-(i * i) / (2.0f * sigma * sigma));
            weights.push_back(weight);
            sum += weight;
        }

        // Normalize
        for(auto& w : weights)
            w /= sum;

        return weights;
    }

    static const char* getBlurShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D image;
            uniform bool horizontal;
            uniform float weight[5];

            void main()
            {
                vec2 texOffset = 1.0 / textureSize(image, 0);
                vec3 result = texture(image, TexCoords).rgb * weight[0];

                if(horizontal)
                {
                    for(int i = 1; i < 5; ++i)
                    {
                        result += texture(image, TexCoords + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                        result += texture(image, TexCoords - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
                    }
                }
                else
                {
                    for(int i = 1; i < 5; ++i)
                    {
                        result += texture(image, TexCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
                        result += texture(image, TexCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
                    }
                }

                FragColor = vec4(result, 1.0);
            }
        )";
    }

    // Advanced: Dual-filter blur (faster for large radii)
    static const char* getDualFilterDownsampleShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D image;
            uniform vec2 texelSize;

            void main()
            {
                vec4 color = vec4(0.0);
                vec2 offset = texelSize;

                // 13-tap downsampling
                color += texture(image, TexCoords) * 4.0;
                color += texture(image, TexCoords + vec2(-offset.x, -offset.y)) * 1.0;
                color += texture(image, TexCoords + vec2(-offset.x,  offset.y)) * 1.0;
                color += texture(image, TexCoords + vec2( offset.x, -offset.y)) * 1.0;
                color += texture(image, TexCoords + vec2( offset.x,  offset.y)) * 1.0;

                FragColor = color / 8.0;
            }
        )";
    }

    static const char* getDualFilterUpsampleShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D image;
            uniform vec2 texelSize;
            uniform float radius;

            void main()
            {
                vec4 color = vec4(0.0);
                vec2 offset = texelSize * radius;

                // 9-tap tent filter upsampling
                color += texture(image, TexCoords + vec2(-offset.x, -offset.y)) * 1.0;
                color += texture(image, TexCoords + vec2(-offset.x,  0.0)) * 2.0;
                color += texture(image, TexCoords + vec2(-offset.x,  offset.y)) * 1.0;

                color += texture(image, TexCoords + vec2(0.0, -offset.y)) * 2.0;
                color += texture(image, TexCoords) * 4.0;
                color += texture(image, TexCoords + vec2(0.0,  offset.y)) * 2.0;

                color += texture(image, TexCoords + vec2( offset.x, -offset.y)) * 1.0;
                color += texture(image, TexCoords + vec2( offset.x,  0.0)) * 2.0;
                color += texture(image, TexCoords + vec2( offset.x,  offset.y)) * 1.0;

                FragColor = color / 16.0;
            }
        )";
    }
};

void BloomEffect::blur()
{
    glUseProgram(blurShader);

    // Generate Gaussian weights
    std::vector<float> weights = GaussianBlur::generateGaussianWeights(4);

    // Upload weights to shader
    for(size_t i = 0; i < weights.size() && i < 5; ++i)
    {
        std::string name = "weight[" + std::to_string(i) + "]";
        glUniform1f(glGetUniformLocation(blurShader, name.c_str()), weights[i]);
    }

    bool horizontal = true;
    GLuint sourceTexture = brightTexture;

    for(int i = 0; i < settings.blurPasses * 2; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);

        glUniform1i(glGetUniformLocation(blurShader, "horizontal"), horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sourceTexture);
        glUniform1i(glGetUniformLocation(blurShader, "image"), 0);

        // Render quad
        // ...

        // Ping-pong
        sourceTexture = pingpongTextures[horizontal];
        horizontal = !horizontal;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
```

---

## 5-6. Bloom Composition and Advanced Techniques

### Compositing Bloom with Scene

```cpp
class BloomComposer
{
public:
    static const char* getComposeShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D scene;
            uniform sampler2D bloomBlur;
            uniform float bloomIntensity;
            uniform float exposure;

            void main()
            {
                vec3 hdrColor = texture(scene, TexCoords).rgb;
                vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

                // Additive blending
                vec3 result = hdrColor + bloomColor * bloomIntensity;

                // Tone mapping
                result = vec3(1.0) - exp(-result * exposure);

                // Gamma correction
                result = pow(result, vec3(1.0 / 2.2));

                FragColor = vec4(result, 1.0);
            }
        )";
    }

    // Screen blend mode
    static const char* getScreenBlendShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D scene;
            uniform sampler2D bloomBlur;
            uniform float bloomIntensity;

            void main()
            {
                vec3 hdrColor = texture(scene, TexCoords).rgb;
                vec3 bloomColor = texture(bloomBlur, TexCoords).rgb * bloomIntensity;

                // Screen blend mode
                vec3 result = vec3(1.0) - (vec3(1.0) - hdrColor) * (vec3(1.0) - bloomColor);

                FragColor = vec4(result, 1.0);
            }
        )";
    }
};

// Advanced: Lens dirt/flares
class LensDirtEffect
{
public:
    static const char* getLensDirtShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 TexCoords;

            uniform sampler2D scene;
            uniform sampler2D bloomBlur;
            uniform sampler2D lensDirt;
            uniform float bloomIntensity;
            uniform float dirtIntensity;

            void main()
            {
                vec3 hdrColor = texture(scene, TexCoords).rgb;
                vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
                vec3 dirt = texture(lensDirt, TexCoords).rgb;

                // Apply bloom with lens dirt modulation
                vec3 result = hdrColor + bloomColor * bloomIntensity * (1.0 + dirt * dirtIntensity);

                FragColor = vec4(result, 1.0);
            }
        )";
    }
};

// Complete bloom pipeline
class CompleteBloomPipeline
{
private:
    BloomEffect* bloom;
    GLuint composeShader;
    GLuint quadVAO, quadVBO;

public:
    CompleteBloomPipeline(int width, int height)
    {
        bloom = new BloomEffect(width, height);
        createComposeShader();
        setupQuad();
    }

    void createComposeShader()
    {
        // Compile shader from BloomComposer::getComposeShader()
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

    void render(GLuint sceneTexture, float intensity = 0.5f, float exposure = 1.0f)
    {
        GLuint bloomTexture;
        bloom->process(sceneTexture, bloomTexture);

        // Compose
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(composeShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sceneTexture);
        glUniform1i(glGetUniformLocation(composeShader, "scene"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bloomTexture);
        glUniform1i(glGetUniformLocation(composeShader, "bloomBlur"), 1);

        glUniform1f(glGetUniformLocation(composeShader, "bloomIntensity"), intensity);
        glUniform1f(glGetUniformLocation(composeShader, "exposure"), exposure);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    ~CompleteBloomPipeline()
    {
        delete bloom;
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(composeShader);
    }
};
```

---

## 7-8. Optimization and Projects

### Performance Optimization

```cpp
class BloomOptimization
{
public:
    // Use smaller resolution for bloom
    static void useDownscaledBloom()
    {
        std::cout << "=== Bloom Optimization Tips ===" << std::endl;
        std::cout << "1. Downscale bright pass to 1/2 or 1/4 resolution" << std::endl;
        std::cout << "2. Use dual-filter blur instead of Gaussian" << std::endl;
        std::cout << "3. Limit blur passes (3-5 is usually enough)" << std::endl;
        std::cout << "4. Use bilinear filtering for upsampling" << std::endl;
        std::cout << "5. Consider using compute shaders for blur" << std::endl;
    }

    // Mipmap-based bloom (Call of Duty style)
    static void mipmapBloom()
    {
        std::cout << "\n=== Mipmap Bloom ===" << std::endl;
        std::cout << "1. Extract bright pass" << std::endl;
        std::cout << "2. Generate mipmaps with downsample filter" << std::endl;
        std::cout << "3. Upsample and blend each level" << std::endl;
        std::cout << "4. Combine with original scene" << std::endl;
    }
};
```

---

## Key Takeaways

1. **Bloom simulates light bleeding from bright sources**
2. **Bright pass extraction uses luminance threshold**
3. **Gaussian blur creates glow effect**
4. **Two-pass blur optimization reduces performance cost**
5. **Bloom composed additively with scene**
6. **Dual-filter blur faster for large radii**
7. **Downscaling improves performance**
8. **Lens dirt adds realism**

---

## Summary Checklist

- [ ] Understand bloom principle
- [ ] Implement bright pass extraction
- [ ] Create Gaussian blur shader
- [ ] Optimize with two-pass blur
- [ ] Implement bloom composition
- [ ] Add threshold and intensity controls
- [ ] Test with HDR content
- [ ] Optimize for performance

---

## Next Lesson Preview

In **Lesson 68**, we'll explore:
- Advanced shadow mapping
- Cascaded Shadow Maps (CSM)
- Percentage Closer Filtering (PCF)
- Variance Shadow Maps
- Shadow artifacts and solutions
- Performance optimization

**Bloom Effect mastered!**
