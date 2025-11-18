# Lesson 32: Multiple Textures and Texture Units

## Texture Units
OpenGL supports multiple texture units (typically 16-32) for binding multiple textures simultaneously.

### Binding Multiple Textures

```cpp
// Activate texture unit 0 and bind diffuse map
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseMap);

// Activate texture unit 1 and bind specular map
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularMap);

// Activate texture unit 2 and bind normal map
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, normalMap);

// Set shader uniforms to texture units
shader.use();
shader.setInt("material.diffuse", 0);   // GL_TEXTURE0
shader.setInt("material.specular", 1);  // GL_TEXTURE1
shader.setInt("material.normal", 2);    // GL_TEXTURE2
```

### Material System with Multiple Textures

```glsl
// Fragment Shader
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D emission;
};

uniform Material material;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    vec3 diffuse = texture(material.diffuse, TexCoord).rgb;
    vec3 specular = texture(material.specular, TexCoord).rgb;
    vec3 normal = texture(material.normal, TexCoord).rgb;
    vec3 emission = texture(material.emission, TexCoord).rgb;
    
    // Use all textures in lighting calculation
    FragColor = vec4(diffuse + emission, 1.0);
}
```

### Texture Blending

```glsl
// Mix two textures
vec4 texture1 = texture(diffuseMap1, TexCoord);
vec4 texture2 = texture(diffuseMap2, TexCoord);
vec4 blended = mix(texture1, texture2, 0.5);  // 50/50 blend

// Multiply blend
vec4 multiplied = texture1 * texture2;

// Additive blend
vec4 added = texture1 + texture2;
```

## Complete Example

```cpp
class MultiTexturedObject {
public:
    GLuint diffuseMap, specularMap, normalMap;
    
    void setupTextures() {
        diffuseMap = loadTexture("diffuse.png");
        specularMap = loadTexture("specular.png");
        normalMap = loadTexture("normal.png");
    }
    
    void draw(Shader& shader) {
        shader.use();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        shader.setInt("material.diffuse", 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        shader.setInt("material.specular", 1);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        shader.setInt("material.normal", 2);
        
        // Draw geometry...
    }
};
```
