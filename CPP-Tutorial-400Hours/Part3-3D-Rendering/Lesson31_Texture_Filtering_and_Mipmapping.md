# Lesson 31: Texture Filtering and Mipmapping

## Advanced Filtering Techniques

### Anisotropic Filtering
Improves texture quality when viewed at steep angles.

```cpp
// Check anisotropy support
float maxAnisotropy;
glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
std::cout << "Max anisotropy: " << maxAnisotropy << std::endl;

// Enable anisotropic filtering
glBindTexture(GL_TEXTURE_2D, textureID);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
```

### Custom Mipmap Levels
Manually specify each mipmap level for complete control.

```cpp
// Load and specify each mipmap level
for (int level = 0; level < mipLevels; level++) {
    int mipWidth = baseWidth >> level;
    int mipHeight = baseHeight >> level;
    
    unsigned char* mipData = loadMipmapLevel(filename, level);
    
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, 
                 mipWidth, mipHeight, 0, 
                 GL_RGB, GL_UNSIGNED_BYTE, mipData);
    
    free(mipData);
}

// Disable automatic mipmap generation
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
```

### LOD (Level of Detail) Bias
Adjust which mipmap level is used.

```cpp
// Make textures sharper or blurrier
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);  // Sharper
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.5f);   // Blurrier
```

```glsl
// In fragment shader - manual LOD selection
vec4 color = textureLod(texture1, TexCoord, lodLevel);
```

## Complete Example

```cpp
class AdvancedTexture {
public:
    GLuint ID;
    
    void setupFiltering() {
        glBindTexture(GL_TEXTURE_2D, ID);
        
        // Trilinear filtering with anisotropy
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Max anisotropy
        float maxAniso;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
        
        // LOD bias for detail
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.2f);
    }
};
```

See Lesson 30 for basic texturing and Lesson 32 for multiple textures.
