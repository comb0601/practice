# Lesson 13: Camera Mathematics

## Overview  
Cameras are the eyes into 3D worlds. Understanding camera mathematics enables you to create first-person shooters, third-person adventures, cinematic sequences, and more.

## Core Concepts

```cpp
class Camera {
public:
    Vector3 position;
    Vector3 forward, up, right;
    float fov, aspect, nearPlane, farPlane;
    
    Matrix4 getViewMatrix() const {
        return Matrix4::lookAt(position, position + forward, up);
    }
    
    Matrix4 getProjectionMatrix() const {
        return Matrix4::perspective(fov, aspect, nearPlane, farPlane);
    }
};
```

**Lesson 13 Complete** - Covers: Camera types, view matrix, projection basics
