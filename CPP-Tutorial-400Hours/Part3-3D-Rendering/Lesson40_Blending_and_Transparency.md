# Lesson 40: Blending and Transparency

## Enable Blending

```cpp
// Enable blending
glEnable(GL_BLEND);

// Set blend function (alpha blending)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

## Blend Functions

```cpp
// Alpha blending (transparency)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Additive blending (lights, particles)
glBlendFunc(GL_ONE, GL_ONE);

// Multiplicative blending
glBlendFunc(GL_DST_COLOR, GL_ZERO);

// Screen blending
glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
```

## Rendering Transparent Objects

```cpp
// 1. Draw opaque objects first
renderOpaqueObjects();

// 2. Sort transparent objects by distance (back to front)
std::sort(transparentObjects.begin(), transparentObjects.end(),
    [&camera](const Object& a, const Object& b) {
        float distA = glm::length(camera.Position - a.position);
        float distB = glm::length(camera.Position - b.position);
        return distA > distB;  // Farther objects first
    });

// 3. Disable depth writing for transparent objects
glDepthMask(GL_FALSE);

// 4. Draw transparent objects
for (auto& obj : transparentObjects) {
    obj.Draw(shader);
}

// 5. Re-enable depth writing
glDepthMask(GL_TRUE);
```

## Fragment Shader

```glsl
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    
    // Discard fully transparent fragments
    if (texColor.a < 0.1)
        discard;
    
    FragColor = texColor;
}
```

## Blend Equation

```cpp
// Default: result = src*srcFactor + dst*dstFactor
glBlendEquation(GL_FUNC_ADD);

// Subtract: result = src*srcFactor - dst*dstFactor
glBlendEquation(GL_FUNC_SUBTRACT);

// Reverse subtract: result = dst*dstFactor - src*srcFactor
glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);

// Min/Max
glBlendEquation(GL_MIN);
glBlendEquation(GL_MAX);
```

## Separate RGB and Alpha Blending

```cpp
glBlendFuncSeparate(
    GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,  // RGB
    GL_ONE, GL_ZERO                         // Alpha
);
```
