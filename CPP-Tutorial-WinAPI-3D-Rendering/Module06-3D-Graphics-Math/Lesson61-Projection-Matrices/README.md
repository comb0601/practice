# Lesson 61: Projection Matrices

## Perspective Projection
```cpp
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
```

## Orthographic Projection
```cpp
Matrix4x4 Orthographic(float left, float right, float bottom, 
                       float top, float near, float far);
```

## Code Examples
1-15: Perspective matrix, orthographic matrix, FOV calculations, NDC, viewport, aspect ratio
