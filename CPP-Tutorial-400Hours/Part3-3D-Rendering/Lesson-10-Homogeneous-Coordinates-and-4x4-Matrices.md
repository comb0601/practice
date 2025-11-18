# Lesson 10: Homogeneous Coordinates and 4x4 Matrices

## Overview
Homogeneous coordinates are the secret sauce that makes 3D graphics transformations work seamlessly. This lesson explains why we use 4x4 matrices for 3D transformations, how homogeneous coordinates enable translation through multiplication, and the mathematical elegance behind projective geometry.

## Table of Contents
1. [Why Homogeneous Coordinates?](#why-homogeneous-coordinates)
2. [Mathematical Foundation](#mathematical-foundation)
3. [4x4 Matrix Structure](#4x4-matrix-structure)
4. [Perspective Division](#perspective-division)
5. [Practical Applications](#practical-applications)

## Why Homogeneous Coordinates?

### The Translation Problem

```cpp
// Problem: Can't represent translation as 3x3 matrix multiplication
Vector3 translate3x3(const Vector3& v) {
    // THIS DOESN'T WORK for translation!
    Matrix3 m; // Can only do rotation/scale
    return m * v; // Missing translation component
}

// Solution: Use 4x4 matrices with homogeneous coordinates
Vector4 translate4x4(const Vector3& v) {
    Matrix4 m = Matrix4::translation(5, 0, 0);
    Vector4 hom(v.x, v.y, v.z, 1.0f); // w=1 for points
    Vector4 result = m * hom;
    return result; // Now translation works!
}
```

### Unified Transformation Framework

```cpp
// With homogeneous coordinates, ALL affine transformations
// can be represented as matrix multiplication:

class Homogeneous {
public:
    // Translation
    static Matrix4 translation(float x, float y, float z) {
        return Matrix4(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        );
    }
    
    // Rotation (example: Z-axis)
    static Matrix4 rotationZ(float angle) {
        float c = cos(angle), s = sin(angle);
        return Matrix4(
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        );
    }
    
    // Scale
    static Matrix4 scale(float sx, float sy, float sz) {
        return Matrix4(
            sx, 0,  0,  0,
            0,  sy, 0,  0,
            0,  0,  sz, 0,
            0,  0,  0,  1
        );
    }
    
    // ALL can be combined through simple multiplication!
};
```

## Mathematical Foundation

### Homogeneous Coordinate System

```cpp
// 3D point (x, y, z) becomes 4D (x, y, z, w)
struct Vector4 {
    float x, y, z, w;
    
    // Constructor for points
    Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}
    
    // Constructor for directions
    static Vector4 direction(const Vector3& v) {
        return Vector4(v.x, v.y, v.z, 0.0f);
    }
    
    // Convert back to 3D
    Vector3 toVector3() const {
        if (abs(w) < 1e-6f) {
            return Vector3(x, y, z);
        }
        return Vector3(x/w, y/w, z/w); // Perspective division
    }
};
```

### Point vs Direction

```cpp
// KEY INSIGHT: w determines behavior
class HomogeneousTypes {
public:
    // Point: w = 1 (affected by translation)
    Vector4 point(float x, float y, float z) {
        return Vector4(x, y, z, 1.0f);
    }
    
    // Direction/Vector: w = 0 (NOT affected by translation)
    Vector4 direction(float x, float y, float z) {
        return Vector4(x, y, z, 0.0f);
    }
    
    void demonstrate() {
        Matrix4 T = Matrix4::translation(10, 0, 0);
        
        Vector4 p = point(0, 0, 0);
        Vector4 d = direction(1, 0, 0);
        
        Vector4 p2 = T * p; // p2 = (10, 0, 0, 1) - translated!
        Vector4 d2 = T * d; // d2 = (1, 0, 0, 0) - unchanged!
    }
};
```

### Mathematical Properties

```cpp
// Homogeneous coordinates have special properties:
class HomogeneousProperties {
public:
    // 1. Scale invariance: (x,y,z,w) = (kx,ky,kz,kw) for k≠0
    bool areEqual(const Vector4& a, const Vector4& b) {
        Vector3 a3 = a.toVector3(); // Performs division
        Vector3 b3 = b.toVector3();
        return a3.equals(b3);
    }
    
    // 2. Points at infinity
    Vector4 pointAtInfinity() {
        return Vector4(1, 0, 0, 0); // w=0, defines direction
    }
    
    // 3. Plane representation
    struct Plane {
        float a, b, c, d; // ax + by + cz + d = 0
        
        // As homogeneous vector
        Vector4 toHomogeneous() {
            return Vector4(a, b, c, d);
        }
        
        // Distance to point
        float distanceTo(const Vector4& point) {
            return (a*point.x + b*point.y + c*point.z + d*point.w);
        }
    };
};
```

## 4x4 Matrix Structure

### Matrix Layout

```cpp
class Matrix4 {
private:
    // Row-major layout:
    //
    // [ m00  m01  m02  m03 ]   [ Xx  Yx  Zx  Tx ]
    // [ m10  m11  m12  m13 ] = [ Xy  Yy  Zy  Ty ]
    // [ m20  m21  m22  m23 ]   [ Xz  Yz  Zz  Tz ]
    // [ m30  m31  m32  m33 ]   [  0   0   0   1 ]
    //
    // Where:
    //   X, Y, Z = basis vectors (rotation/scale)
    //   T = translation vector
    //   Bottom row = perspective (usually 0,0,0,1 for affine)
    
    float m[4][4];
    
public:
    // Extract components
    Vector3 getRight() const {
        return Vector3(m[0][0], m[1][0], m[2][0]);
    }
    
    Vector3 getUp() const {
        return Vector3(m[0][1], m[1][1], m[2][1]);
    }
    
    Vector3 getForward() const {
        return Vector3(m[0][2], m[1][2], m[2][2]);
    }
    
    Vector3 getTranslation() const {
        return Vector3(m[0][3], m[1][3], m[2][3]);
    }
    
    // Set components
    void setRight(const Vector3& v) {
        m[0][0] = v.x; m[1][0] = v.y; m[2][0] = v.z;
    }
    
    void setUp(const Vector3& v) {
        m[0][1] = v.x; m[1][1] = v.y; m[2][1] = v.z;
    }
    
    void setForward(const Vector3& v) {
        m[0][2] = v.x; m[1][2] = v.y; m[2][2] = v.z;
    }
    
    void setTranslation(const Vector3& v) {
        m[0][3] = v.x; m[1][3] = v.y; m[2][3] = v.z;
    }
};
```

### Affine Transformation Matrix

```cpp
// Standard affine transformation structure
class AffineMatrix {
public:
    // [ R  T ]  where R = 3x3 rotation/scale
    // [ 0  1 ]        T = 3x1 translation
    
    static Matrix4 createAffine(const Matrix3& rotation,
                               const Vector3& translation) {
        Matrix4 result;
        
        // Copy rotation/scale
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = rotation.m[i][j];
            }
        }
        
        // Set translation
        result.m[0][3] = translation.x;
        result.m[1][3] = translation.y;
        result.m[2][3] = translation.z;
        
        // Bottom row
        result.m[3][0] = 0;
        result.m[3][1] = 0;
        result.m[3][2] = 0;
        result.m[3][3] = 1;
        
        return result;
    }
    
    static bool isAffine(const Matrix4& m) {
        return abs(m.m[3][0]) < 1e-6f &&
               abs(m.m[3][1]) < 1e-6f &&
               abs(m.m[3][2]) < 1e-6f &&
               abs(m.m[3][3] - 1.0f) < 1e-6f;
    }
};
```

## Perspective Division

### The w Component

```cpp
// Perspective projection creates non-1 w values
class PerspectiveDivision {
public:
    // After projection, w ≠ 1
    Vector4 project(const Vector3& viewSpace, float fov, float aspect) {
        float f = 1.0f / tan(fov * 0.5f);
        
        Vector4 clip;
        clip.x = viewSpace.x * f / aspect;
        clip.y = viewSpace.y * f;
        clip.z = viewSpace.z; // Simplified
        clip.w = -viewSpace.z; // w varies with depth!
        
        return clip;
    }
    
    // Perspective divide: Convert clip space to NDC
    Vector3 toNDC(const Vector4& clip) {
        if (abs(clip.w) < 1e-6f) {
            return Vector3(clip.x, clip.y, clip.z);
        }
        
        return Vector3(
            clip.x / clip.w,  // Perspective division!
            clip.y / clip.w,
            clip.z / clip.w
        );
    }
    
    // This creates the perspective effect:
    // Objects further away (larger w) become smaller
    void demonstratePerspective() {
        Vector3 near(1, 1, -1);   // Close to camera
        Vector3 far(1, 1, -10);   // Far from camera
        
        Vector4 nearClip = project(near, PI/2, 1.0f);
        Vector4 farClip = project(far, PI/2, 1.0f);
        
        Vector3 nearNDC = toNDC(nearClip);
        Vector3 farNDC = toNDC(farClip);
        
        // nearNDC.x is LARGER than farNDC.x
        // Objects further away appear smaller!
    }
};
```

### Interpolation in Homogeneous Space

```cpp
// Perspective-correct interpolation
class PerspectiveInterpolation {
public:
    // WRONG: Linear interpolation in screen space
    float linearInterp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    // CORRECT: Interpolate 1/w, then divide
    float perspectiveCorrectInterp(float a, float b,
                                  float wa, float wb,
                                  float t) {
        // Interpolate in homogeneous space
        float inv_w = linearInterp(1.0f/wa, 1.0f/wb, t);
        float value = linearInterp(a/wa, b/wb, t);
        
        // Perspective divide
        return value / inv_w;
    }
    
    // Used for texture coordinate interpolation
    Vector2 interpolateTexCoord(const Vector2& uv0, float w0,
                               const Vector2& uv1, float w1,
                               float t) {
        float u = perspectiveCorrectInterp(uv0.x, uv1.x, w0, w1, t);
        float v = perspectiveCorrectInterp(uv0.y, uv1.y, w0, w1, t);
        return Vector2(u, v);
    }
};
```

## Practical Applications

### Complete Transform Chain

```cpp
class CompleteTransform {
public:
    Matrix4 model, view, projection;
    
    // Full transformation
    Vector4 transformVertex(const Vector3& vertex) {
        // Start with point (w=1)
        Vector4 v(vertex.x, vertex.y, vertex.z, 1.0f);
        
        // Model space → World space
        v = model * v;
        
        // World space → View space
        v = view * v;
        
        // View space → Clip space
        v = projection * v;
        
        // Now v.w may not be 1!
        return v;
    }
    
    // Convert to screen coordinates
    Vector2 toScreen(const Vector4& clip, int width, int height) {
        // Perspective division
        Vector3 ndc(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
        
        // NDC to screen
        float x = (ndc.x + 1.0f) * 0.5f * width;
        float y = (1.0f - ndc.y) * 0.5f * height;
        
        return Vector2(x, y);
    }
};
```

### Ray from Screen

```cpp
// Generate ray from screen point using inverse transform
class ScreenRay {
public:
    struct Ray {
        Vector3 origin;
        Vector3 direction;
    };
    
    Ray generateRay(int screenX, int screenY,
                   const Matrix4& invProjection,
                   const Matrix4& invView,
                   int screenWidth, int screenHeight) {
        // Screen to NDC
        float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
        float ndcY = 1.0f - (2.0f * screenY) / screenHeight;
        
        // NDC to clip (near plane, w=1)
        Vector4 clipNear(ndcX, ndcY, -1.0f, 1.0f);
        Vector4 clipFar(ndcX, ndcY, 1.0f, 1.0f);
        
        // Clip to view
        Vector4 viewNear = invProjection * clipNear;
        Vector4 viewFar = invProjection * clipFar;
        
        // Perspective divide
        viewNear = viewNear / viewNear.w;
        viewFar = viewFar / viewFar.w;
        
        // View to world
        Vector4 worldNear = invView * viewNear;
        Vector4 worldFar = invView * viewFar;
        
        Ray ray;
        ray.origin = Vector3(worldNear.x, worldNear.y, worldNear.z);
        Vector3 farPoint(worldFar.x, worldFar.y, worldFar.z);
        ray.direction = (farPoint - ray.origin).normalized();
        
        return ray;
    }
};
```

## Key Takeaways

1. **Homogeneous coordinates** add w component to enable projective transformations
2. **w=1** for points (affected by translation)
3. **w=0** for directions (not affected by translation)
4. **4x4 matrices** unify all affine transformations
5. **Perspective division** (x/w, y/w, z/w) creates perspective effect
6. **Bottom row** [0 0 0 1] indicates affine transformation
7. **Non-1 w values** occur after perspective projection
8. **Perspective-correct interpolation** requires special handling
9. **Scale invariance**: (x,y,z,w) ≡ (kx,ky,kz,kw)
10. **Matrix structure**: [Rotation|Translation; 0|1]

## Exercises

1. Implement perspective-correct texture mapping
2. Create a homogeneous coordinate visualizer
3. Write functions to test affine vs projective matrices
4. Implement depth interpolation for z-buffer
5. Create a complete vertex shader in C++ using homogeneous coordinates

---

**Lesson 10 Complete** - Next: Lesson 11 - Quaternions Introduction
