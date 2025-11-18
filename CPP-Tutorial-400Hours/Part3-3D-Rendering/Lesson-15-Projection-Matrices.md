# Lesson 15: Projection Matrices (Perspective and Orthographic)

## Overview
Projection matrices convert 3D view space to 2D screen space with perspective or orthographic projection.

## Perspective Projection

```cpp
Matrix4 perspective(float fovY, float aspect, float near, float far) {
    float f = 1.0f / tan(fovY * 0.5f);
    
    Matrix4 proj;
    proj.m[0][0] = f / aspect;
    proj.m[1][1] = f;
    proj.m[2][2] = (far + near) / (near - far);
    proj.m[2][3] = (2.0f * far * near) / (near - far);
    proj.m[3][2] = -1.0f;
    proj.m[3][3] = 0.0f;
    
    return proj;
}
```

## Orthographic Projection

```cpp
Matrix4 ortho(float left, float right, float bottom, float top,
             float near, float far) {
    Matrix4 proj;
    proj.m[0][0] = 2.0f / (right - left);
    proj.m[1][1] = 2.0f / (top - bottom);
    proj.m[2][2] = -2.0f / (far - near);
    proj.m[0][3] = -(right + left) / (right - left);
    proj.m[1][3] = -(top + bottom) / (top - bottom);
    proj.m[2][3] = -(far + near) / (far - near);
    
    return proj;
}
```

**Lesson 15 Complete** - Covers: Perspective, orthographic, FOV calculations
