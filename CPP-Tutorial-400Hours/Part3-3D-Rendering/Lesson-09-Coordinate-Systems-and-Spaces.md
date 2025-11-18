# Lesson 09: Coordinate Systems and Spaces

## Overview
Understanding coordinate systems and spatial transformations is fundamental to 3D graphics. This lesson explores different coordinate spaces, transformations between them, and how they're used in the graphics pipeline.

## Table of Contents
1. [Coordinate System Basics](#coordinate-system-basics)
2. [Common Coordinate Spaces](#common-coordinate-spaces)
3. [Space Transformations](#space-transformations)
4. [The Graphics Pipeline](#the-graphics-pipeline)
5. [Practical Implementations](#practical-implementations)

## Coordinate System Basics

### Right-Hand vs Left-Hand Systems

```cpp
enum class CoordinateSystem {
    RIGHT_HAND,  // OpenGL, Maya, Blender
    LEFT_HAND    // DirectX, Unity
};

// Right-hand rule: 
// - Thumb = X (right)
// - Index = Y (up)
// - Middle = Z (toward you)

// Left-hand rule:
// - Thumb = X (right)
// - Index = Y (up)
// - Middle = Z (away from you)
```

### Coordinate Space Properties

```cpp
struct CoordinateSpace {
    Vector3 origin;
    Vector3 xAxis;  // Right
    Vector3 yAxis;  // Up
    Vector3 zAxis;  // Forward
    
    CoordinateSpace() 
        : origin(0, 0, 0)
        , xAxis(1, 0, 0)
        , yAxis(0, 1, 0)
        , zAxis(0, 0, 1)
    {}
    
    Matrix4 getTransformToWorld() const {
        Matrix4 m;
        m.setColumn(0, Vector4(xAxis, 0));
        m.setColumn(1, Vector4(yAxis, 0));
        m.setColumn(2, Vector4(zAxis, 0));
        m.setColumn(3, Vector4(origin, 1));
        return m;
    }
};
```

## Common Coordinate Spaces

### 1. Model/Object Space

Local coordinates relative to object's center:

```cpp
struct ModelSpace {
    // Vertices defined in model's own coordinate system
    std::vector<Vector3> vertices;
    Transform transform;  // Stores position, rotation, scale
    
    // Convert to world space
    Matrix4 getModelMatrix() const {
        return transform.toMatrix();
    }
};
```

### 2. World Space

Global coordinate system for entire scene:

```cpp
class WorldSpace {
public:
    // Transform from model space to world space
    static Vector3 modelToWorld(const Vector3& modelPoint, 
                                const Matrix4& modelMatrix) {
        return modelMatrix.transformPoint(modelPoint);
    }
    
    // All objects share this common reference frame
};
```

### 3. View/Camera/Eye Space

Coordinates relative to camera:

```cpp
class ViewSpace {
    Matrix4 viewMatrix;
    
public:
    Matrix4 createViewMatrix(const Vector3& position,
                            const Vector3& target,
                            const Vector3& up) {
        Vector3 forward = (target - position).normalized();
        Vector3 right = up.cross(forward).normalized();
        Vector3 cameraUp = forward.cross(right);
        
        Matrix4 view;
        // Camera basis
        view.setRow(0, Vector4(right.x, right.y, right.z, 0));
        view.setRow(1, Vector4(cameraUp.x, cameraUp.y, cameraUp.z, 0));
        view.setRow(2, Vector4(forward.x, forward.y, forward.z, 0));
        view.setRow(3, Vector4(0, 0, 0, 1));
        
        // Translation
        Matrix4 translation;
        translation.m[0][3] = -position.x;
        translation.m[1][3] = -position.y;
        translation.m[2][3] = -position.z;
        
        return view * translation;
    }
};
```

### 4. Clip Space

After projection, before perspective divide:

```cpp
// Homogeneous clip coordinates
struct ClipSpace {
    Vector4 position;  // (x, y, z, w)
    
    // Frustum culling in clip space
    bool isInside() const {
        return (position.x >= -position.w && position.x <= position.w) &&
               (position.y >= -position.w && position.y <= position.w) &&
               (position.z >= -position.w && position.z <= position.w);
    }
};
```

### 5. NDC (Normalized Device Coordinates)

After perspective divide:

```cpp
struct NDC {
    Vector3 position;  // Range: [-1, 1] for all axes
    
    static NDC fromClipSpace(const Vector4& clip) {
        NDC ndc;
        if (std::abs(clip.w) > 1e-6f) {
            ndc.position.x = clip.x / clip.w;
            ndc.position.y = clip.y / clip.w;
            ndc.position.z = clip.z / clip.w;
        }
        return ndc;
    }
};
```

### 6. Screen/Window Space

Final pixel coordinates:

```cpp
struct ScreenSpace {
    int x, y;
    float depth;
    
    static ScreenSpace fromNDC(const NDC& ndc, 
                               int screenWidth, 
                               int screenHeight) {
        ScreenSpace screen;
        screen.x = static_cast<int>((ndc.position.x + 1.0f) * 0.5f * screenWidth);
        screen.y = static_cast<int>((1.0f - ndc.position.y) * 0.5f * screenHeight);
        screen.depth = ndc.position.z;
        return screen;
    }
};
```

## Space Transformations

### Complete Transformation Pipeline

```cpp
class TransformPipeline {
public:
    Matrix4 modelMatrix;
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    
    // Model → World
    Vector3 modelToWorld(const Vector3& modelPos) const {
        return modelMatrix.transformPoint(modelPos);
    }
    
    // World → View
    Vector3 worldToView(const Vector3& worldPos) const {
        return viewMatrix.transformPoint(worldPos);
    }
    
    // View → Clip
    Vector4 viewToClip(const Vector3& viewPos) const {
        return projectionMatrix * Vector4(viewPos, 1.0f);
    }
    
    // Complete: Model → Screen
    ScreenSpace modelToScreen(const Vector3& modelPos,
                             int screenWidth,
                             int screenHeight) const {
        // Model → World
        Vector3 world = modelToWorld(modelPos);
        
        // World → View
        Vector3 view = worldToView(world);
        
        // View → Clip
        Vector4 clip = viewToClip(view);
        
        // Clip → NDC
        NDC ndc = NDC::fromClipSpace(clip);
        
        // NDC → Screen
        return ScreenSpace::fromNDC(ndc, screenWidth, screenHeight);
    }
    
    // MVP matrix (Model-View-Projection)
    Matrix4 getMVP() const {
        return projectionMatrix * viewMatrix * modelMatrix;
    }
};
```

### Inverse Transformations

```cpp
class InverseTransforms {
public:
    // Screen → World (for mouse picking)
    Vector3 screenToWorld(int screenX, int screenY,
                         float depth,
                         const Matrix4& invMVP,
                         int screenWidth,
                         int screenHeight) {
        // Screen → NDC
        float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
        float ndcY = 1.0f - (2.0f * screenY) / screenHeight;
        float ndcZ = depth;
        
        // NDC → Clip (reverse perspective divide)
        Vector4 clip(ndcX, ndcY, ndcZ, 1.0f);
        
        // Clip → World
        Vector4 world = invMVP * clip;
        
        // Homogeneous divide
        if (std::abs(world.w) > 1e-6f) {
            return Vector3(world.x / world.w, 
                          world.y / world.w, 
                          world.z / world.w);
        }
        
        return Vector3(world.x, world.y, world.z);
    }
};
```

## The Graphics Pipeline

### Vertex Transformation Sequence

```cpp
struct Vertex {
    Vector3 position;     // Model space
    Vector3 normal;       // Model space
    Vector2 texCoord;
    
    // Transform through pipeline
    struct TransformedVertex {
        Vector3 worldPos;
        Vector3 worldNormal;
        Vector3 viewPos;
        Vector4 clipPos;
        Vector2 texCoord;
    };
    
    TransformedVertex transform(const TransformPipeline& pipeline) const {
        TransformedVertex tv;
        
        // Position: Model → World → View → Clip
        tv.worldPos = pipeline.modelToWorld(position);
        tv.viewPos = pipeline.worldToView(tv.worldPos);
        tv.clipPos = pipeline.viewToClip(tv.viewPos);
        
        // Normal: Transform by normal matrix (inverse transpose)
        Matrix4 normalMatrix = pipeline.modelMatrix.inverse().transpose();
        tv.worldNormal = normalMatrix.transformVector(normal).normalized();
        
        // Texture coordinates pass through
        tv.texCoord = texCoord;
        
        return tv;
    }
};
```

### Complete Pipeline Implementation

```cpp
class RenderPipeline {
    TransformPipeline transform;
    int screenWidth, screenHeight;
    
public:
    void renderTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
        // 1. Vertex transformation
        auto tv0 = v0.transform(transform);
        auto tv1 = v1.transform(transform);
        auto tv2 = v2.transform(transform);
        
        // 2. Clipping (simplified)
        if (!isTriangleVisible(tv0.clipPos, tv1.clipPos, tv2.clipPos)) {
            return;
        }
        
        // 3. Perspective divide (Clip → NDC)
        NDC ndc0 = NDC::fromClipSpace(tv0.clipPos);
        NDC ndc1 = NDC::fromClipSpace(tv1.clipPos);
        NDC ndc2 = NDC::fromClipSpace(tv2.clipPos);
        
        // 4. Viewport transformation (NDC → Screen)
        ScreenSpace s0 = ScreenSpace::fromNDC(ndc0, screenWidth, screenHeight);
        ScreenSpace s1 = ScreenSpace::fromNDC(ndc1, screenWidth, screenHeight);
        ScreenSpace s2 = ScreenSpace::fromNDC(ndc2, screenWidth, screenHeight);
        
        // 5. Rasterization
        rasterizeTriangle(s0, s1, s2, tv0, tv1, tv2);
    }
    
private:
    bool isTriangleVisible(const Vector4& c0, const Vector4& c1, const Vector4& c2) {
        ClipSpace clip0{c0}, clip1{c1}, clip2{c2};
        return clip0.isInside() || clip1.isInside() || clip2.isInside();
    }
};
```

## Practical Implementations

### Mouse Picking (Screen to World)

```cpp
class MousePicker {
public:
    Vector3 getRayDirection(int mouseX, int mouseY,
                           const Matrix4& invProjection,
                           const Matrix4& invView,
                           int screenWidth,
                           int screenHeight) {
        // Screen → NDC
        float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
        float ndcY = 1.0f - (2.0f * mouseY) / screenHeight;
        
        // NDC → Clip (near plane)
        Vector4 clipNear(ndcX, ndcY, -1.0f, 1.0f);
        
        // Clip → View
        Vector4 viewNear = invProjection * clipNear;
        viewNear /= viewNear.w;
        
        // View → World
        Vector4 worldNear = invView * viewNear;
        
        // Ray direction
        return Vector3(worldNear.x, worldNear.y, worldNear.z).normalized();
    }
};
```

### Shadow Mapping Spaces

```cpp
class ShadowMap {
    Matrix4 lightViewMatrix;
    Matrix4 lightProjectionMatrix;
    
public:
    // Transform world position to light's clip space
    Vector4 worldToLightClip(const Vector3& worldPos) const {
        Vector3 lightView = lightViewMatrix.transformPoint(worldPos);
        return lightProjectionMatrix * Vector4(lightView, 1.0f);
    }
    
    // Sample shadow map
    bool isInShadow(const Vector3& worldPos) const {
        Vector4 lightClip = worldToLightClip(worldPos);
        
        // Perspective divide
        NDC lightNDC = NDC::fromClipSpace(lightClip);
        
        // Convert to texture coordinates [0, 1]
        float u = lightNDC.position.x * 0.5f + 0.5f;
        float v = lightNDC.position.y * 0.5f + 0.5f;
        
        // Compare depth (simplified)
        float shadowDepth = sampleShadowMap(u, v);
        return lightNDC.position.z > shadowDepth + 0.001f;
    }
};
```

## Key Takeaways

1. **Multiple coordinate spaces** exist in the graphics pipeline
2. **Model space** is local to each object
3. **World space** is the global reference frame
4. **View space** is relative to the camera
5. **Clip space** uses homogeneous coordinates
6. **NDC** ranges from -1 to 1
7. **Screen space** is final pixel coordinates
8. **Transformations chain** through matrix multiplication
9. **Inverse transformations** enable picking and UI
10. **Normal matrices** require special transformation

## Exercises

1. Implement a complete mouse picking system
2. Create a coordinate space visualizer/debugger
3. Build a shadow mapping system with proper space transforms
4. Implement billboarding in different spaces
5. Create a reflection/refraction system using space transformations

---

**Lesson 09 Complete** - Next: Lesson 10 - Homogeneous Coordinates and 4x4 Matrices
