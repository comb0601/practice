# Lesson 28: Fragment Shaders Deep Dive

## Table of Contents
1. [Fragment Shader Role](#fragment-shader-role)
2. [Fragment Processing](#fragment-processing)
3. [Texture Sampling](#texture-sampling)
4. [Lighting Calculations](#lighting-calculations)
5. [Color Operations](#color-operations)
6. [Discarding Fragments](#discarding-fragments)
7. [Advanced Effects](#advanced-effects)
8. [Built-in Variables](#built-in-variables)
9. [Optimization Techniques](#optimization-techniques)
10. [Complete Examples](#complete-examples)

## Fragment Shader Role

The fragment shader determines the final color of each pixel. It runs once for every fragment (potential pixel) that passes through the rasterizer.

### Key Responsibilities

1. **Calculate final pixel color** from interpolated vertex data
2. **Sample textures** and combine texture colors
3. **Perform lighting calculations** (Phong, PBR, etc.)
4. **Apply effects** (fog, bloom, color grading, etc.)
5. **Discard fragments** for transparency/clipping

### Basic Fragment Shader

```glsl
#version 330 core
in vec3 vertexColor;   // From vertex shader (interpolated)
out vec4 FragColor;    // Final color

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
```

## Fragment Processing

### Input from Vertex Shader

```glsl
#version 330 core

// Interpolated inputs from vertex shader
in vec3 FragPos;       // Fragment position in world space
in vec3 Normal;        // Interpolated normal
in vec2 TexCoord;      // Interpolated texture coordinates
in vec3 vertexColor;   // Interpolated color

out vec4 FragColor;

void main() {
    // Use interpolated values
    FragColor = vec4(vertexColor, 1.0);
}
```

### Understanding Interpolation

```
Vertex Shader:           Fragment Shader:
                         (Interpolation happens automatically)

v1: color = (1,0,0)      ●─────●─────●  Each fragment gets
v2: color = (0,1,0)       \    |    /   interpolated values
v3: color = (0,0,1)        \   |   /    based on distance
                            \  |  /     to vertices
                             \ | /
                              \|/
                               ●
                         Fragment: color ≈ (0.33, 0.33, 0.33)
```

### Multiple Outputs

```glsl
#version 330 core

// Multiple render targets (MRT)
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

void main() {
    gPosition = vec4(FragPos, 1.0);
    gNormal = vec4(normalize(Normal), 1.0);
    gAlbedoSpec.rgb = texture(albedoMap, TexCoord).rgb;
    gAlbedoSpec.a = texture(specularMap, TexCoord).r;
}
```

## Texture Sampling

### Basic Texture Sampling

```glsl
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
```

### Multiple Textures

```glsl
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

void main() {
    vec3 diffuse = texture(diffuseMap, TexCoord).rgb;
    vec3 specular = texture(specularMap, TexCoord).rgb;
    vec3 normal = texture(normalMap, TexCoord).rgb;

    // Use textures...
    FragColor = vec4(diffuse, 1.0);
}
```

### Texture Blending

```glsl
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendFactor;

void main() {
    vec4 color1 = texture(texture1, TexCoord);
    vec4 color2 = texture(texture2, TexCoord);

    // Linear blend
    FragColor = mix(color1, color2, blendFactor);

    // Multiply blend
    // FragColor = color1 * color2;

    // Add blend
    // FragColor = color1 + color2;

    // Screen blend
    // FragColor = vec4(1.0) - (vec4(1.0) - color1) * (vec4(1.0) - color2);
}
```

### Texture Tiling and Scrolling

```glsl
uniform sampler2D texture1;
uniform float time;

void main() {
    // Tiling
    vec2 tiledCoord = TexCoord * 5.0;

    // Scrolling
    vec2 scrollingCoord = TexCoord + vec2(time * 0.1, 0.0);

    // Rotating
    float angle = time;
    mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    vec2 rotatedCoord = rot * (TexCoord - 0.5) + 0.5;

    FragColor = texture(texture1, scrollingCoord);
}
```

## Lighting Calculations

### Phong Lighting Model

```glsl
#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

void main() {
    vec3 objectColor = texture(diffuseMap, TexCoord).rgb;
    vec3 specular = texture(specularMap, TexCoord).rgb;

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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specularLight = specularStrength * spec * lightColor * specular;

    vec3 result = (ambient + diffuse + specularLight) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

### Blinn-Phong Lighting (Improved Specular)

```glsl
void main() {
    // ... (ambient and diffuse same as above)

    // Blinn-Phong specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specularLight = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specularLight) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

### Attenuation (Distance-based Light Falloff)

```glsl
void main() {
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 ambient = ambientStrength * lightColor * attenuation;
    vec3 diffuse = diff * lightColor * attenuation;
    vec3 specular = spec * lightColor * attenuation;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

### Directional Light

```glsl
uniform vec3 lightDirection;  // Direction TO light (normalized)

void main() {
    vec3 lightDir = normalize(-lightDirection);  // Direction FROM light
    float diff = max(dot(norm, lightDir), 0.0);

    // No attenuation for directional lights
    vec3 diffuse = diff * lightColor;

    // ... rest of lighting calculation
}
```

### Spot Light

```glsl
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float cutOff;      // Inner cone
uniform float outerCutOff; // Outer cone

void main() {
    vec3 lightDirection = normalize(lightPos - FragPos);

    // Calculate angle between light direction and fragment direction
    float theta = dot(lightDirection, normalize(-lightDir));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    // Apply spotlight intensity
    vec3 diffuse = diff * lightColor * intensity;
    vec3 specular = spec * lightColor * intensity;

    // ... rest of lighting
}
```

## Color Operations

### Grayscale Conversion

```glsl
void main() {
    vec4 color = texture(texture1, TexCoord);

    // Method 1: Average
    float gray = (color.r + color.g + color.b) / 3.0;

    // Method 2: Weighted (perceptually accurate)
    float gray = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;

    FragColor = vec4(vec3(gray), 1.0);
}
```

### Sepia Tone

```glsl
void main() {
    vec4 color = texture(texture1, TexCoord);

    float r = color.r * 0.393 + color.g * 0.769 + color.b * 0.189;
    float g = color.r * 0.349 + color.g * 0.686 + color.b * 0.168;
    float b = color.r * 0.272 + color.g * 0.534 + color.b * 0.131;

    FragColor = vec4(r, g, b, 1.0);
}
```

### Color Inversion

```glsl
void main() {
    vec4 color = texture(texture1, TexCoord);
    FragColor = vec4(1.0 - color.rgb, 1.0);
}
```

### Brightness and Contrast

```glsl
uniform float brightness;  // -1.0 to 1.0
uniform float contrast;    // 0.0 to 2.0

void main() {
    vec4 color = texture(texture1, TexCoord);

    // Apply brightness
    color.rgb += brightness;

    // Apply contrast
    color.rgb = (color.rgb - 0.5) * contrast + 0.5;

    FragColor = color;
}
```

### Gamma Correction

```glsl
uniform float gamma;  // Usually 2.2

void main() {
    vec4 color = texture(texture1, TexCoord);

    // Apply gamma correction
    color.rgb = pow(color.rgb, vec3(1.0 / gamma));

    FragColor = color;
}
```

## Discarding Fragments

### Alpha Testing

```glsl
uniform sampler2D texture1;

void main() {
    vec4 color = texture(texture1, TexCoord);

    // Discard if alpha below threshold
    if (color.a < 0.1)
        discard;

    FragColor = color;
}
```

### Clip Planes

```glsl
in vec3 FragPos;

uniform vec4 clipPlane;  // (A, B, C, D) where Ax + By + Cz + D = 0

void main() {
    // Discard if on wrong side of plane
    if (dot(vec4(FragPos, 1.0), clipPlane) < 0.0)
        discard;

    FragColor = vec4(1.0);
}
```

### Procedural Patterns

```glsl
void main() {
    // Checkerboard pattern
    float scale = 10.0;
    vec2 scaled = TexCoord * scale;
    float pattern = mod(floor(scaled.x) + floor(scaled.y), 2.0);

    if (pattern < 0.5)
        discard;

    FragColor = vec4(1.0);
}
```

## Advanced Effects

### Fog

```glsl
uniform vec3 fogColor;
uniform float fogDensity;

in vec3 FragPos;
in vec3 viewPos;

void main() {
    vec4 color = texture(texture1, TexCoord);

    // Linear fog
    float distance = length(viewPos - FragPos);
    float fogStart = 10.0;
    float fogEnd = 50.0;
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);

    // Exponential fog (more realistic)
    // float fogFactor = exp(-fogDensity * distance);

    // Exponential squared fog
    // float fogFactor = exp(-pow(fogDensity * distance, 2.0));

    FragColor = mix(vec4(fogColor, 1.0), color, fogFactor);
}
```

### Rim Lighting

```glsl
uniform vec3 rimColor;
uniform float rimPower;

void main() {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = pow(rim, rimPower);

    vec3 color = texture(diffuseMap, TexCoord).rgb;
    vec3 rimLighting = rim * rimColor;

    FragColor = vec4(color + rimLighting, 1.0);
}
```

### Fresnel Effect

```glsl
void main() {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    float fresnel = pow(1.0 - max(dot(viewDir, norm), 0.0), 3.0);

    // Mix between reflection and refraction based on fresnel
    vec3 reflection = texture(skybox, reflect(-viewDir, norm)).rgb;
    vec3 refraction = texture(skybox, refract(-viewDir, norm, 1.0/1.52)).rgb;

    FragColor = vec4(mix(refraction, reflection, fresnel), 1.0);
}
```

### Procedural Noise

```glsl
// Simple pseudo-random function
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    float noise = random(TexCoord);

    // Add noise to color
    vec3 color = texture(texture1, TexCoord).rgb;
    color += vec3(noise * 0.1);

    FragColor = vec4(color, 1.0);
}
```

### Cel Shading (Toon Shading)

```glsl
void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float NdotL = max(dot(norm, lightDir), 0.0);

    // Quantize lighting
    float intensity;
    if (NdotL > 0.95)
        intensity = 1.0;
    else if (NdotL > 0.5)
        intensity = 0.6;
    else if (NdotL > 0.25)
        intensity = 0.4;
    else
        intensity = 0.2;

    vec3 color = texture(diffuseMap, TexCoord).rgb;
    FragColor = vec4(color * intensity, 1.0);

    // Add outline (based on normal)
    vec3 viewDir = normalize(viewPos - FragPos);
    float edge = max(dot(viewDir, norm), 0.0);
    if (edge < 0.3) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);  // Black outline
    }
}
```

## Built-in Variables

### gl_FragCoord

```glsl
void main() {
    // Fragment position in window space
    vec2 pixelPos = gl_FragCoord.xy;

    // Normalized device coordinates
    vec2 ndc = (pixelPos / resolution) * 2.0 - 1.0;

    // Create pattern based on position
    float pattern = sin(pixelPos.x * 0.1) * sin(pixelPos.y * 0.1);

    FragColor = vec4(vec3(pattern), 1.0);
}
```

### gl_FrontFacing

```glsl
void main() {
    vec3 color;

    if (gl_FrontFacing) {
        color = vec3(1.0, 0.0, 0.0);  // Front face: red
    } else {
        color = vec3(0.0, 0.0, 1.0);  // Back face: blue
    }

    FragColor = vec4(color, 1.0);
}
```

### gl_FragDepth

```glsl
void main() {
    vec4 color = texture(texture1, TexCoord);

    // Custom depth value
    gl_FragDepth = gl_FragCoord.z + 0.1;

    // Logarithmic depth (for large scenes)
    // float C = 1.0;
    // float FC = 1.0 / log(farPlane * C + 1.0);
    // gl_FragDepth = log(gl_FragCoord.w * C + 1.0) * FC;

    FragColor = color;
}
```

## Optimization Techniques

### 1. Early Exit

```glsl
void main() {
    // Check alpha first
    vec4 color = texture(texture1, TexCoord);
    if (color.a < 0.1)
        discard;  // Exit early, skip expensive calculations

    // Expensive lighting calculations
    vec3 lighting = calculateLighting();

    FragColor = vec4(lighting, 1.0);
}
```

### 2. Avoid Branching

```glsl
// BAD (branching)
if (useTexture) {
    color = texture(texture1, TexCoord);
} else {
    color = uniformColor;
}

// GOOD (no branching)
float useTexFloat = float(useTexture);  // Convert bool to float
color = mix(uniformColor, texture(texture1, TexCoord), useTexFloat);
```

### 3. Precalculate Constants

```glsl
// BAD (calculate every fragment)
void main() {
    float invGamma = 1.0 / 2.2;
    color.rgb = pow(color.rgb, vec3(invGamma));
}

// GOOD (pass as uniform)
uniform float invGamma;

void main() {
    color.rgb = pow(color.rgb, vec3(invGamma));
}
```

### 4. Use Lower Precision (Mobile/GLSL ES)

```glsl
#ifdef GL_ES
precision mediump float;
#endif

// Use appropriate precision
lowp vec4 color;      // 8-bit per channel
mediump float value;  // 16-bit
highp vec3 position;  // 32-bit
```

### 5. Minimize Texture Fetches

```glsl
// BAD (multiple fetches)
float r = texture(tex, uv).r;
float g = texture(tex, uv).g;
float b = texture(tex, uv).b;

// GOOD (single fetch)
vec3 rgb = texture(tex, uv).rgb;
float r = rgb.r;
float g = rgb.g;
float b = rgb.b;
```

## Complete Examples

### Example 1: Complete Phong Lighting

```glsl
#version 330 core

// Inputs
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Outputs
out vec4 FragColor;

// Uniforms
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

struct Material {
    float shininess;
};

uniform Material material;

void main() {
    // Sample textures
    vec3 objectColor = texture(diffuseMap, TexCoord).rgb;
    vec3 specularColor = texture(specularMap, TexCoord).rgb;

    // Ambient
    vec3 ambient = 0.1 * lightColor * objectColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor * specularColor;

    // Attenuation
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Final color
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```

### Example 2: Multi-Light Scene

```glsl
#version 330 core

#define MAX_LIGHTS 4

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D diffuseMap;

vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * light.color * light.intensity;

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    return (diffuse + specular) * attenuation;
}

void main() {
    vec3 objectColor = texture(diffuseMap, TexCoord).rgb;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.1) * objectColor;  // Ambient

    // Calculate lighting for each light
    for (int i = 0; i < numLights; i++) {
        result += calculateLight(lights[i], norm, FragPos, viewDir) * objectColor;
    }

    FragColor = vec4(result, 1.0);
}
```

## Summary

In this lesson, you learned:

1. **Fragment Shader Role**: Determining final pixel colors
2. **Interpolation**: How vertex data is interpolated to fragments
3. **Texture Sampling**: Basic and advanced texture techniques
4. **Lighting**: Phong, Blinn-Phong, attenuation, multiple light types
5. **Color Operations**: Grayscale, sepia, inversion, brightness, contrast
6. **Discarding**: Alpha testing, clip planes, procedural patterns
7. **Advanced Effects**: Fog, rim lighting, Fresnel, cel shading
8. **Built-in Variables**: gl_FragCoord, gl_FrontFacing, gl_FragDepth
9. **Optimization**: Early exit, avoiding branching, precalculation
10. **Complete Examples**: Phong lighting, multi-light scenes

### What's Next?

In **Lesson 29**, we'll explore:
- Shader programs in detail
- Uniform variables and uniform buffers
- Shader compilation and linking
- Error handling and debugging
- Shader hot-reloading

### Practice Exercises

1. **Custom Lighting**: Implement your own lighting model
2. **Post-Processing**: Create grayscale, blur, or edge detection
3. **Procedural Textures**: Generate textures in fragment shader
4. **Multi-Pass Rendering**: Implement a multi-pass effect
5. **Performance Testing**: Compare different optimization techniques

Congratulations! You now have a deep understanding of fragment shaders and per-pixel operations!
