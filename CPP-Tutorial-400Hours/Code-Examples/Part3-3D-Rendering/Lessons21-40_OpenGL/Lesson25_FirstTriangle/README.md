# Lesson 25: Drawing Your First Triangle

## Overview
Complete OpenGL triangle rendering with vertex buffers, shaders, and the full rendering pipeline.

## Features
- Vertex Buffer Object (VBO) creation
- Vertex Array Object (VAO) setup
- GLSL shader compilation
- Colored triangle with per-vertex colors
- Complete OpenGL 3.3 Core rendering pipeline

## Building
```bash
cmake --build . --target Lesson25_FirstTriangle
cd bin/Lessons21-40
./Lesson25_FirstTriangle
```

## Shaders
- `vertex.glsl`: Vertex shader with position and color attributes
- `fragment.glsl`: Fragment shader with color interpolation

## Expected Output
- Window with a gradient-colored triangle:
  - Top vertex: Red
  - Bottom left: Green
  - Bottom right: Blue
- Console output showing resource creation

## Key Concepts
- **VBO**: Stores vertex data in GPU memory
- **VAO**: Stores vertex attribute configuration
- **Vertex Shader**: Transforms vertex positions
- **Fragment Shader**: Determines pixel colors
- **Interpolation**: Colors smoothly interpolated between vertices

## Technical Details
```cpp
// Vertex layout: [x, y, z, r, g, b]
float vertices[] = {
     0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top (red)
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left (green)
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // bottom right (blue)
};
```
