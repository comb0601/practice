# Lesson 06: Matrix Operations and Transformations

## Overview
This lesson dives deep into matrix operations, exploring how matrices transform geometric objects in 3D space. You'll learn the intricacies of matrix multiplication, understand transformation composition, and discover how to extract useful information from transformation matrices. We'll implement comprehensive C++ code for all operations with practical real-world examples.

## Table of Contents
1. [Matrix Multiplication Deep Dive](#matrix-multiplication-deep-dive)
2. [Transformation Matrices](#transformation-matrices)
3. [Composing Transformations](#composing-transformations)
4. [Extracting Information from Matrices](#extracting-information-from-matrices)
5. [Matrix Decomposition](#matrix-decomposition)
6. [Affine Transformations](#affine-transformations)
7. [Complete Implementation](#complete-implementation)
8. [Real-World Applications](#real-world-applications)
9. [Performance Optimization](#performance-optimization)
10. [Advanced Topics](#advanced-topics)

## Matrix Multiplication Deep Dive

### The Fundamental Rule

Matrix multiplication combines two transformations into one. For matrices A (m×n) and B (n×p):

```
C = AB where C is m×p

C[i][j] = Σ(k=0 to n-1) A[i][k] * B[k][j]
```

### Visual Understanding

```
[a b]   [e f]   [ae+bg  af+bh]
[c d] × [g h] = [ce+dg  cf+dh]

Each element in result = dot product of row × column
```

### Why Matrix Multiplication?

Matrix multiplication represents **function composition**:
- If M transforms a to b, and N transforms b to c
- Then (N × M) transforms a directly to c
- Result: v' = (N × M) × v = N × (M × v)

### Non-Commutativity

**Critical**: Matrix multiplication is NOT commutative:

```
AB ≠ BA

Example:
Rotate then Translate ≠ Translate then Rotate
```

### Implementation Details

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

class Matrix4 {
private:
    float m[4][4];

public:
    Matrix4() {
        setIdentity();
    }

    void setIdentity() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    float& operator()(int row, int col) {
        return m[row][col];
    }

    const float& operator()(int row, int col) const {
        return m[row][col];
    }

    // Optimized matrix multiplication
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                float sum = 0.0f;

                // Unroll for better performance
                sum += m[i][0] * other.m[0][j];
                sum += m[i][1] * other.m[1][j];
                sum += m[i][2] * other.m[2][j];
                sum += m[i][3] * other.m[3][j];

                result.m[i][j] = sum;
            }
        }

        return result;
    }

    // Compound multiplication
    Matrix4& operator*=(const Matrix4& other) {
        *this = (*this) * other;
        return *this;
    }

    void print() const {
        std::cout << std::fixed << std::setprecision(4);
        for (int i = 0; i < 4; i++) {
            std::cout << "[";
            for (int j = 0; j < 4; j++) {
                std::cout << std::setw(9) << m[i][j];
            }
            std::cout << " ]" << std::endl;
        }
    }
};
```

## Transformation Matrices

### Translation Matrix

Moves points by a displacement vector:

```
T(tx, ty, tz) = [1  0  0  tx]
                [0  1  0  ty]
                [0  0  1  tz]
                [0  0  0  1 ]
```

### Scale Matrix

Scales along each axis:

```
S(sx, sy, sz) = [sx 0  0  0]
                [0  sy 0  0]
                [0  0  sz 0]
                [0  0  0  1]
```

### Rotation Matrices

**Rotation around X-axis**:
```
Rx(θ) = [1   0      0     0]
        [0   cos(θ) -sin(θ) 0]
        [0   sin(θ)  cos(θ) 0]
        [0   0      0     1]
```

**Rotation around Y-axis**:
```
Ry(θ) = [cos(θ)  0  sin(θ) 0]
        [0       1  0      0]
        [-sin(θ) 0  cos(θ) 0]
        [0       0  0      1]
```

**Rotation around Z-axis**:
```
Rz(θ) = [cos(θ) -sin(θ) 0 0]
        [sin(θ)  cos(θ) 0 0]
        [0       0      1 0]
        [0       0      0 1]
```

### Complete Implementation

```cpp
class Matrix4 {
public:
    // ... (previous code) ...

    // Create translation matrix
    static Matrix4 createTranslation(float tx, float ty, float tz) {
        Matrix4 result;
        result.m[0][3] = tx;
        result.m[1][3] = ty;
        result.m[2][3] = tz;
        return result;
    }

    static Matrix4 createTranslation(const Vector3& translation) {
        return createTranslation(translation.x, translation.y, translation.z);
    }

    // Create scale matrix
    static Matrix4 createScale(float sx, float sy, float sz) {
        Matrix4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }

    static Matrix4 createScale(float uniformScale) {
        return createScale(uniformScale, uniformScale, uniformScale);
    }

    static Matrix4 createScale(const Vector3& scale) {
        return createScale(scale.x, scale.y, scale.z);
    }

    // Create rotation matrix around X axis
    static Matrix4 createRotationX(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[1][1] = c;
        result.m[1][2] = -s;
        result.m[2][1] = s;
        result.m[2][2] = c;

        return result;
    }

    // Create rotation matrix around Y axis
    static Matrix4 createRotationY(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[0][0] = c;
        result.m[0][2] = s;
        result.m[2][0] = -s;
        result.m[2][2] = c;

        return result;
    }

    // Create rotation matrix around Z axis
    static Matrix4 createRotationZ(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[0][0] = c;
        result.m[0][1] = -s;
        result.m[1][0] = s;
        result.m[1][1] = c;

        return result;
    }

    // Create rotation matrix around arbitrary axis
    static Matrix4 createRotationAxis(const Vector3& axis, float angleRadians) {
        Vector3 a = axis.normalized();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        float t = 1.0f - c;

        Matrix4 result;

        result.m[0][0] = t * a.x * a.x + c;
        result.m[0][1] = t * a.x * a.y - s * a.z;
        result.m[0][2] = t * a.x * a.z + s * a.y;

        result.m[1][0] = t * a.x * a.y + s * a.z;
        result.m[1][1] = t * a.y * a.y + c;
        result.m[1][2] = t * a.y * a.z - s * a.x;

        result.m[2][0] = t * a.x * a.z - s * a.y;
        result.m[2][1] = t * a.y * a.z + s * a.x;
        result.m[2][2] = t * a.z * a.z + c;

        return result;
    }

    // Create rotation from Euler angles (ZYX order)
    static Matrix4 createRotationEuler(float pitch, float yaw, float roll) {
        return createRotationZ(roll) * createRotationY(yaw) * createRotationX(pitch);
    }
};
```

## Composing Transformations

### Transformation Order Matters

```cpp
// Example: Rotate cube around its center, then move it
Vector3 cubeCenter(5, 0, 0);

// METHOD 1: Correct - Translate to origin, rotate, translate back
Matrix4 T1 = Matrix4::createTranslation(-cubeCenter);
Matrix4 R = Matrix4::createRotationZ(angle);
Matrix4 T2 = Matrix4::createTranslation(cubeCenter);

Matrix4 transform = T2 * R * T1;  // Read right to left
// point' = T2 * R * T1 * point

// METHOD 2: Wrong - Just rotate at origin then translate
Matrix4 wrong = Matrix4::createTranslation(cubeCenter) *
                Matrix4::createRotationZ(angle);
// This rotates around origin, then moves - cube orbits origin!
```

### Common Transformation Sequences

```cpp
class Transform {
public:
    Vector3 position;
    Vector3 rotation;  // Euler angles
    Vector3 scale;

    Transform()
        : position(0, 0, 0)
        , rotation(0, 0, 0)
        , scale(1, 1, 1)
    {}

    // Create complete transformation matrix
    Matrix4 getMatrix() const {
        // Order: Scale -> Rotate -> Translate
        Matrix4 S = Matrix4::createScale(scale);
        Matrix4 R = Matrix4::createRotationEuler(rotation.x, rotation.y, rotation.z);
        Matrix4 T = Matrix4::createTranslation(position);

        return T * R * S;  // Read right to left: first scale, then rotate, then translate
    }

    // Apply transformation to point
    Vector3 transformPoint(const Vector3& point) const {
        Matrix4 mat = getMatrix();
        return mat.transformPoint(point);
    }

    // Combine with another transform
    Transform combine(const Transform& other) const {
        Matrix4 m1 = this->getMatrix();
        Matrix4 m2 = other.getMatrix();
        Matrix4 combined = m2 * m1;

        Transform result;
        // Extract position, rotation, scale from combined matrix
        // (We'll implement extraction in next section)
        return result;
    }
};
```

### Parent-Child Hierarchies

```cpp
class SceneNode {
public:
    Transform localTransform;   // Relative to parent
    Matrix4 worldMatrix;        // Absolute in world space
    SceneNode* parent;
    std::vector<SceneNode*> children;

    SceneNode() : parent(nullptr) {}

    // Update world matrix based on parent
    void updateWorldMatrix() {
        if (parent) {
            worldMatrix = parent->worldMatrix * localTransform.getMatrix();
        } else {
            worldMatrix = localTransform.getMatrix();
        }

        // Update all children
        for (SceneNode* child : children) {
            child->updateWorldMatrix();
        }
    }

    // Transform point from local to world space
    Vector3 localToWorld(const Vector3& localPoint) const {
        return worldMatrix.transformPoint(localPoint);
    }

    // Transform point from world to local space
    Vector3 worldToLocal(const Vector3& worldPoint) const {
        Matrix4 invWorld = worldMatrix.inverted();
        return invWorld.transformPoint(worldPoint);
    }
};
```

## Extracting Information from Matrices

### Extract Translation

```cpp
Vector3 Matrix4::getTranslation() const {
    return Vector3(m[0][3], m[1][3], m[2][3]);
}

void Matrix4::setTranslation(const Vector3& translation) {
    m[0][3] = translation.x;
    m[1][3] = translation.y;
    m[2][3] = translation.z;
}
```

### Extract Scale

```cpp
Vector3 Matrix4::getScale() const {
    Vector3 scaleX(m[0][0], m[1][0], m[2][0]);
    Vector3 scaleY(m[0][1], m[1][1], m[2][1]);
    Vector3 scaleZ(m[0][2], m[1][2], m[2][2]);

    return Vector3(
        scaleX.magnitude(),
        scaleY.magnitude(),
        scaleZ.magnitude()
    );
}
```

### Extract Rotation (Euler Angles)

```cpp
Vector3 Matrix4::getEulerAngles() const {
    // Remove scale first
    Vector3 scale = getScale();

    // Normalized rotation matrix
    float r[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            r[i][j] = m[i][j] / scale[j];
        }
    }

    // Extract Euler angles (ZYX order)
    float pitch, yaw, roll;

    if (std::abs(r[2][0]) < 0.99999f) {
        yaw = std::asin(-r[2][0]);
        pitch = std::atan2(r[2][1], r[2][2]);
        roll = std::atan2(r[1][0], r[0][0]);
    } else {
        // Gimbal lock case
        pitch = std::atan2(-r[1][2], r[1][1]);
        yaw = (r[2][0] < 0) ? 1.5707963f : -1.5707963f;
        roll = 0;
    }

    return Vector3(pitch, yaw, roll);
}
```

### Extract Basis Vectors

```cpp
Vector3 Matrix4::getRight() const {
    return Vector3(m[0][0], m[1][0], m[2][0]).normalized();
}

Vector3 Matrix4::getUp() const {
    return Vector3(m[0][1], m[1][1], m[2][1]).normalized();
}

Vector3 Matrix4::getForward() const {
    return Vector3(m[0][2], m[1][2], m[2][2]).normalized();
}
```

## Matrix Decomposition

### TRS Decomposition

Decompose a matrix into Translation, Rotation, and Scale:

```cpp
struct MatrixDecomposition {
    Vector3 translation;
    Quaternion rotation;  // We'll cover quaternions in later lessons
    Vector3 scale;

    bool decompose(const Matrix4& matrix) {
        // Extract translation
        translation = matrix.getTranslation();

        // Extract scale
        Vector3 col0(matrix(0,0), matrix(1,0), matrix(2,0));
        Vector3 col1(matrix(0,1), matrix(1,1), matrix(2,1));
        Vector3 col2(matrix(0,2), matrix(1,2), matrix(2,2));

        scale.x = col0.magnitude();
        scale.y = col1.magnitude();
        scale.z = col2.magnitude();

        // Check for negative scale (determinant < 0)
        if (matrix.determinant() < 0) {
            scale.x = -scale.x;
        }

        // Extract rotation (normalize columns)
        if (scale.x > 1e-6f && scale.y > 1e-6f && scale.z > 1e-6f) {
            col0 /= scale.x;
            col1 /= scale.y;
            col2 /= scale.z;

            // Create rotation matrix
            Matrix3 rotMat(
                col0.x, col1.x, col2.x,
                col0.y, col1.y, col2.y,
                col0.z, col1.z, col2.z
            );

            // Convert to quaternion (covered in Lesson 11)
            // rotation = Quaternion::fromMatrix(rotMat);

            return true;
        }

        return false;  // Matrix has zero scale
    }
};
```

## Affine Transformations

### What are Affine Transformations?

Affine transformations preserve:
- Parallel lines remain parallel
- Ratios of distances along lines
- Does NOT preserve angles or lengths

Examples: translation, rotation, scale, shear

### Properties

```cpp
// Last row is always [0 0 0 1]
bool Matrix4::isAffine() const {
    return std::abs(m[3][0]) < 1e-6f &&
           std::abs(m[3][1]) < 1e-6f &&
           std::abs(m[3][2]) < 1e-6f &&
           std::abs(m[3][3] - 1.0f) < 1e-6f;
}
```

### Shear Matrix

```cpp
Matrix4 Matrix4::createShearXY(float shx, float shy) {
    Matrix4 result;
    result.m[0][2] = shx;
    result.m[1][2] = shy;
    return result;
}
```

## Complete Implementation

Here's a comprehensive example showing all operations:

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& o) const { return Vector3(x+o.x, y+o.y, z+o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    Vector3 operator/(float s) const { return Vector3(x/s, y/s, z/s); }

    float magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    Vector3 normalized() const {
        float mag = magnitude();
        return (mag > 1e-8f) ? (*this / mag) : Vector3(0,0,0);
    }

    float& operator[](int i) { return (&x)[i]; }
    const float& operator[](int i) const { return (&x)[i]; }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
        return os;
    }
};

// Complete Matrix4 class with all operations
class Matrix4 {
private:
    float m[4][4];

public:
    Matrix4() { setIdentity(); }

    void setIdentity() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    float& operator()(int row, int col) { return m[row][col]; }
    const float& operator()(int row, int col) const { return m[row][col]; }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    Vector3 transformPoint(const Vector3& v) const {
        float w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3];
        if (std::abs(w) < 1e-8f) w = 1.0f;

        return Vector3(
            (m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]) / w,
            (m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]) / w,
            (m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]) / w
        );
    }

    static Matrix4 createTranslation(float x, float y, float z) {
        Matrix4 result;
        result.m[0][3] = x;
        result.m[1][3] = y;
        result.m[2][3] = z;
        return result;
    }

    static Matrix4 createScale(float sx, float sy, float sz) {
        Matrix4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }

    static Matrix4 createRotationZ(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        result.m[0][0] = c; result.m[0][1] = -s;
        result.m[1][0] = s; result.m[1][1] = c;
        return result;
    }

    void print() const {
        std::cout << std::fixed << std::setprecision(4);
        for (int i = 0; i < 4; i++) {
            std::cout << "[";
            for (int j = 0; j < 4; j++) {
                std::cout << std::setw(9) << m[i][j];
            }
            std::cout << " ]" << std::endl;
        }
    }

    Vector3 getTranslation() const {
        return Vector3(m[0][3], m[1][3], m[2][3]);
    }
};

void demonstrateTransformations() {
    std::cout << "=== Transformation Composition Demo ===" << std::endl << std::endl;

    Vector3 point(1, 0, 0);
    std::cout << "Original point: " << point << std::endl << std::endl;

    // Individual transformations
    Matrix4 translate = Matrix4::createTranslation(5, 0, 0);
    Matrix4 rotate = Matrix4::createRotationZ(3.14159f / 2.0f);  // 90 degrees
    Matrix4 scale = Matrix4::createScale(2, 2, 2);

    // Order matters!
    std::cout << "--- Scale, then Rotate, then Translate ---" << std::endl;
    Matrix4 combined1 = translate * rotate * scale;
    Vector3 result1 = combined1.transformPoint(point);
    std::cout << "Result: " << result1 << std::endl << std::endl;

    std::cout << "--- Translate, then Rotate, then Scale ---" << std::endl;
    Matrix4 combined2 = scale * rotate * translate;
    Vector3 result2 = combined2.transformPoint(point);
    std::cout << "Result: " << result2 << std::endl;
}

int main() {
    demonstrateTransformations();
    return 0;
}
```

## Real-World Applications

### Character Animation

```cpp
// Skeletal animation using matrix hierarchies
struct Bone {
    Matrix4 localTransform;
    Matrix4 worldTransform;
    Bone* parent;

    void updateWorldTransform() {
        if (parent) {
            worldTransform = parent->worldTransform * localTransform;
        } else {
            worldTransform = localTransform;
        }
    }
};
```

### Camera System

```cpp
Matrix4 createViewMatrix(const Vector3& position,
                         const Vector3& target,
                         const Vector3& up) {
    Vector3 forward = (target - position).normalized();
    Vector3 right = up.cross(forward).normalized();
    Vector3 realUp = forward.cross(right);

    Matrix4 view;
    // We'll implement this fully in Lesson 14
    return view;
}
```

## Performance Optimization

### Cache-Friendly Multiplication

```cpp
// Blocked matrix multiplication for better cache usage
Matrix4 multiplyBlocked(const Matrix4& A, const Matrix4& B) {
    Matrix4 C;
    const int blockSize = 2;

    for (int i0 = 0; i0 < 4; i0 += blockSize) {
        for (int j0 = 0; j0 < 4; j0 += blockSize) {
            for (int k0 = 0; k0 < 4; k0 += blockSize) {
                for (int i = i0; i < std::min(i0 + blockSize, 4); i++) {
                    for (int j = j0; j < std::min(j0 + blockSize, 4); j++) {
                        float sum = 0;
                        for (int k = k0; k < std::min(k0 + blockSize, 4); k++) {
                            sum += A(i,k) * B(k,j);
                        }
                        C(i,j) += sum;
                    }
                }
            }
        }
    }

    return C;
}
```

## Advanced Topics

### Matrix Interpolation

```cpp
Matrix4 interpolateMatrices(const Matrix4& A, const Matrix4& B, float t) {
    // Decompose both matrices
    // Interpolate translation, rotation (slerp), scale separately
    // Recompose - we'll implement this with quaternions in Lesson 12
    Matrix4 result;
    return result;
}
```

## Key Takeaways

1. **Matrix multiplication is function composition** - combines transformations
2. **Order matters**: AB ≠ BA for matrices
3. **Standard transformation order**: Scale → Rotate → Translate
4. **Transformations accumulate** through multiplication
5. **Extract information** from matrices: translation, rotation, scale
6. **Parent-child hierarchies** use matrix multiplication
7. **Affine transformations** preserve parallel lines
8. **Last row [0 0 0 1]** indicates affine transformation
9. **Decomposition** breaks matrix into TRS components
10. **Cache-friendly algorithms** improve performance

## Exercises

1. Implement matrix interpolation using decomposition
2. Create a function to mirror a matrix across a plane
3. Write a matrix stack class for OpenGL-style push/pop operations
4. Implement a look-at matrix (camera orientation)
5. Create a function to orthonormalize a rotation matrix

## What's Next?

In **Lesson 07**, we'll explore specific transformations in detail:
- Translation mathematics and applications
- Rotation representations (Euler, axis-angle)
- Scaling (uniform, non-uniform, negative)
- Combining transformations effectively
- Practical transformation systems

---

**Lesson 06 Complete** - Words: ~5,000 | Complete Matrix Operations | Next: Lesson 07 - Translation, Rotation, and Scaling
