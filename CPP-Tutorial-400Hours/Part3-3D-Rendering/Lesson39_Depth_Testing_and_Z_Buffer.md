# Lesson 39: Depth Testing and Z-Buffer

## Enable Depth Testing

```cpp
// Enable depth testing
glEnable(GL_DEPTH_TEST);

// Set depth function
glDepthFunc(GL_LESS);  // Default: pass if fragment is closer

// In render loop - clear depth buffer
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

## Depth Functions

```cpp
glDepthFunc(GL_ALWAYS);   // Always pass
glDepthFunc(GL_NEVER);    // Never pass
glDepthFunc(GL_LESS);     // Pass if less (closer)
glDepthFunc(GL_LEQUAL);   // Pass if less or equal
glDepthFunc(GL_GREATER);  // Pass if greater (farther)
glDepthFunc(GL_GEQUAL);   // Pass if greater or equal
glDepthFunc(GL_EQUAL);    // Pass if equal
glDepthFunc(GL_NOTEQUAL); // Pass if not equal
```

## Depth Buffer Visualization

```glsl
// Fragment shader - visualize depth
void main() {
    float depth = gl_FragCoord.z;
    FragColor = vec4(vec3(depth), 1.0);
}
```

## Depth Mask

```cpp
// Disable depth writing (still tests depth)
glDepthMask(GL_FALSE);

// Re-enable depth writing
glDepthMask(GL_TRUE);
```

## Custom Depth Values

```glsl
// Fragment shader - set custom depth
void main() {
    gl_FragDepth = customDepthValue;
    FragColor = vec4(color, 1.0);
}
```
