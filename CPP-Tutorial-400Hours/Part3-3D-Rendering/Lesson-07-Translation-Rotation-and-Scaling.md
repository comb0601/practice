# Lesson 07: Translation, Rotation, and Scaling

## Overview
Translation, rotation, and scaling are the three fundamental transformations in 3D graphics. This lesson explores each transformation in depth, covering mathematical theory, implementation details, edge cases, and practical applications. You'll learn to create robust transformation systems that handle all scenarios correctly.

## Table of Contents
1. [Translation in Depth](#translation-in-depth)
2. [Rotation Systems](#rotation-systems)
3. [Scaling Transformations](#scaling-transformations)
4. [Combining Transformations](#combining-transformations)
5. [Transformation Invariants](#transformation-invariants)
6. [Complete Implementation](#complete-implementation)
7. [Real-World Examples](#real-world-examples)
8. [Common Pitfalls](#common-pitfalls)

## Translation in Depth

### Mathematical Foundation

Translation moves every point by a constant displacement vector:

```
P' = P + T

where T = (tx, ty, tz) is the translation vector
```

### Translation Matrix

```
T(tx, ty, tz) = [1  0  0  tx]
                [0  1  0  ty]
                [0  0  1  tz]
                [0  0  0  1 ]
```

### Properties

1. **Commutative**: T1 + T2 = T2 + T1
2. **Associative**: (T1 + T2) + T3 = T1 + (T2 + T3)
3. **Identity**: T(0,0,0) = I
4. **Inverse**: T^(-1)(tx,ty,tz) = T(-tx,-ty,-tz)

### Implementation

```cpp
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& o) const { return Vector3(x+o.x, y+o.y, z+o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    Vector3 operator-() const { return Vector3(-x, -y, -z); }

    float magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    Vector3 normalized() const {
        float mag = magnitude();
        return (mag > 1e-8f) ? (*this / mag) : Vector3(0,0,0);
    }

    float dot(const Vector3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vector3 cross(const Vector3& o) const {
        return Vector3(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
        return os;
    }
};

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

    // Create translation matrix
    static Matrix4 translation(const Vector3& t) {
        return translation(t.x, t.y, t.z);
    }

    static Matrix4 translation(float tx, float ty, float tz) {
        Matrix4 result;
        result.m[0][3] = tx;
        result.m[1][3] = ty;
        result.m[2][3] = tz;
        return result;
    }

    // Apply translation to existing matrix
    void translate(const Vector3& t) {
        m[0][3] += t.x;
        m[1][3] += t.y;
        m[2][3] += t.z;
    }

    // Extract translation component
    Vector3 getTranslation() const {
        return Vector3(m[0][3], m[1][3], m[2][3]);
    }

    // Set translation component (preserving rotation/scale)
    void setTranslation(const Vector3& t) {
        m[0][3] = t.x;
        m[1][3] = t.y;
        m[2][3] = t.z;
    }

    // Matrix multiplication
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

    // Transform point
    Vector3 transformPoint(const Vector3& p) const {
        float w = m[3][0]*p.x + m[3][1]*p.y + m[3][2]*p.z + m[3][3];
        if (std::abs(w) < 1e-8f) w = 1.0f;

        return Vector3(
            (m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3]) / w,
            (m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3]) / w,
            (m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3]) / w
        );
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

### Translation Applications

```cpp
// Orbit camera around target
Vector3 orbitCamera(const Vector3& target, float radius, float angle) {
    Vector3 offset(radius * std::cos(angle), 0, radius * std::sin(angle));
    return target + offset;
}

// Follow target with offset
Vector3 followTarget(const Vector3& target, const Vector3& offset) {
    return target + offset;
}

// Smooth movement toward target
Vector3 moveToward(const Vector3& current, const Vector3& target,
                   float maxDistance) {
    Vector3 direction = target - current;
    float distance = direction.magnitude();

    if (distance <= maxDistance) {
        return target;
    }

    return current + direction.normalized() * maxDistance;
}
```

## Rotation Systems

### Euler Angles

Rotations represented as three angles (pitch, yaw, roll):

```cpp
class Matrix4 {
public:
    // Rotation around X-axis (pitch)
    static Matrix4 rotationX(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[1][1] = c;  result.m[1][2] = -s;
        result.m[2][1] = s;  result.m[2][2] = c;

        return result;
    }

    // Rotation around Y-axis (yaw)
    static Matrix4 rotationY(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[0][0] = c;   result.m[0][2] = s;
        result.m[2][0] = -s;  result.m[2][2] = c;

        return result;
    }

    // Rotation around Z-axis (roll)
    static Matrix4 rotationZ(float angleRadians) {
        Matrix4 result;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        result.m[0][0] = c;  result.m[0][1] = -s;
        result.m[1][0] = s;  result.m[1][1] = c;

        return result;
    }

    // Combined Euler rotation (ZYX order)
    static Matrix4 rotationEuler(float pitch, float yaw, float roll) {
        return rotationZ(roll) * rotationY(yaw) * rotationX(pitch);
    }

    // Alternative: XYZ order
    static Matrix4 rotationEulerXYZ(float angleX, float angleY, float angleZ) {
        return rotationZ(angleZ) * rotationY(angleY) * rotationX(angleX);
    }
};
```

### Axis-Angle Rotation

Rodrigues' rotation formula:

```cpp
class Matrix4 {
public:
    // Rotate around arbitrary axis
    static Matrix4 rotationAxis(const Vector3& axis, float angleRadians) {
        Vector3 a = axis.normalized();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        float t = 1.0f - c;

        Matrix4 result;

        // Row 0
        result.m[0][0] = t * a.x * a.x + c;
        result.m[0][1] = t * a.x * a.y - s * a.z;
        result.m[0][2] = t * a.x * a.z + s * a.y;

        // Row 1
        result.m[1][0] = t * a.x * a.y + s * a.z;
        result.m[1][1] = t * a.y * a.y + c;
        result.m[1][2] = t * a.y * a.z - s * a.x;

        // Row 2
        result.m[2][0] = t * a.x * a.z - s * a.y;
        result.m[2][1] = t * a.y * a.z + s * a.x;
        result.m[2][2] = t * a.z * a.z + c;

        return result;
    }

    // Rotate to align one vector with another
    static Matrix4 rotationFromTo(const Vector3& from, const Vector3& to) {
        Vector3 f = from.normalized();
        Vector3 t = to.normalized();

        float dot = f.dot(t);

        // Parallel vectors
        if (dot > 0.99999f) {
            return Matrix4();  // Identity
        }

        // Anti-parallel vectors
        if (dot < -0.99999f) {
            Vector3 axis = Vector3(1, 0, 0);
            if (std::abs(f.x) > 0.9f) {
                axis = Vector3(0, 1, 0);
            }
            axis = axis.cross(f).normalized();
            return rotationAxis(axis, 3.14159265f);
        }

        // General case
        Vector3 axis = f.cross(t);
        float angle = std::acos(dot);
        return rotationAxis(axis, angle);
    }

    // Look at rotation (align Z-axis toward target)
    static Matrix4 lookAt(const Vector3& eye, const Vector3& target,
                          const Vector3& up) {
        Vector3 forward = (target - eye).normalized();
        Vector3 right = up.cross(forward).normalized();
        Vector3 realUp = forward.cross(right);

        Matrix4 result;

        result.m[0][0] = right.x;
        result.m[0][1] = right.y;
        result.m[0][2] = right.z;

        result.m[1][0] = realUp.x;
        result.m[1][1] = realUp.y;
        result.m[1][2] = realUp.z;

        result.m[2][0] = forward.x;
        result.m[2][1] = forward.y;
        result.m[2][2] = forward.z;

        return result;
    }
};
```

### Extracting Euler Angles

```cpp
Vector3 Matrix4::toEulerAngles() const {
    float pitch, yaw, roll;

    // Extract rotation (assuming no scale/shear)
    if (std::abs(m[2][0]) < 0.99999f) {
        yaw = std::asin(-m[2][0]);
        pitch = std::atan2(m[2][1], m[2][2]);
        roll = std::atan2(m[1][0], m[0][0]);
    } else {
        // Gimbal lock case
        pitch = std::atan2(-m[1][2], m[1][1]);
        yaw = (m[2][0] < 0) ? 1.5707963f : -1.5707963f;
        roll = 0;
    }

    return Vector3(pitch, yaw, roll);
}
```

### Gimbal Lock

```cpp
// Demonstrate gimbal lock problem
void demonstrateGimbalLock() {
    std::cout << "=== Gimbal Lock Demo ===" << std::endl;

    // Rotate 90 degrees around Y
    Matrix4 rotY90 = Matrix4::rotationY(3.14159f / 2.0f);

    std::cout << "After 90° Y rotation:" << std::endl;
    Vector3 xAxis = rotY90.transformPoint(Vector3(1, 0, 0));
    Vector3 zAxis = rotY90.transformPoint(Vector3(0, 0, 1));

    std::cout << "X-axis becomes: " << xAxis << std::endl;
    std::cout << "Z-axis becomes: " << zAxis << std::endl;
    std::cout << "X and Z rotations now affect same axis! (Gimbal Lock)" << std::endl;
}
```

## Scaling Transformations

### Uniform Scaling

All axes scaled equally:

```cpp
class Matrix4 {
public:
    static Matrix4 scale(float s) {
        return scale(s, s, s);
    }

    static Matrix4 scale(float sx, float sy, float sz) {
        Matrix4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }

    static Matrix4 scale(const Vector3& s) {
        return scale(s.x, s.y, s.z);
    }
};
```

### Non-Uniform Scaling

Different scale per axis:

```cpp
// Extract scale from matrix
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

// Remove scale from matrix (normalize basis vectors)
Matrix4 Matrix4::removeScale() const {
    Vector3 scale = getScale();

    Matrix4 result = *this;

    if (scale.x > 1e-6f) {
        result.m[0][0] /= scale.x;
        result.m[1][0] /= scale.x;
        result.m[2][0] /= scale.x;
    }

    if (scale.y > 1e-6f) {
        result.m[0][1] /= scale.y;
        result.m[1][1] /= scale.y;
        result.m[2][1] /= scale.y;
    }

    if (scale.z > 1e-6f) {
        result.m[0][2] /= scale.z;
        result.m[1][2] /= scale.z;
        result.m[2][2] /= scale.z;
    }

    return result;
}
```

### Negative Scale (Reflection)

```cpp
// Reflect across plane
class Matrix4 {
public:
    // Reflect across XY plane (flip Z)
    static Matrix4 reflectionXY() {
        return scale(1, 1, -1);
    }

    // Reflect across XZ plane (flip Y)
    static Matrix4 reflectionXZ() {
        return scale(1, -1, 1);
    }

    // Reflect across YZ plane (flip X)
    static Matrix4 reflectionYZ() {
        return scale(-1, 1, 1);
    }

    // Reflect across arbitrary plane
    static Matrix4 reflectionPlane(const Vector3& normal) {
        Vector3 n = normal.normalized();

        Matrix4 result;

        result.m[0][0] = 1 - 2 * n.x * n.x;
        result.m[0][1] = -2 * n.x * n.y;
        result.m[0][2] = -2 * n.x * n.z;

        result.m[1][0] = -2 * n.y * n.x;
        result.m[1][1] = 1 - 2 * n.y * n.y;
        result.m[1][2] = -2 * n.y * n.z;

        result.m[2][0] = -2 * n.z * n.x;
        result.m[2][1] = -2 * n.z * n.y;
        result.m[2][2] = 1 - 2 * n.z * n.z;

        return result;
    }

    // Check if matrix contains reflection
    bool hasReflection() const {
        // Determinant < 0 indicates reflection
        return determinant() < 0;
    }

    float determinant() const {
        // Simplified for 3x3 upper-left (rotation/scale part)
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }
};
```

## Combining Transformations

### Standard TRS Order

```cpp
class Transform {
public:
    Vector3 translation;
    Vector3 rotation;  // Euler angles
    Vector3 scale;

    Transform()
        : translation(0, 0, 0)
        , rotation(0, 0, 0)
        , scale(1, 1, 1)
    {}

    // Build transformation matrix: T * R * S
    Matrix4 toMatrix() const {
        Matrix4 S = Matrix4::scale(scale);
        Matrix4 R = Matrix4::rotationEuler(rotation.x, rotation.y, rotation.z);
        Matrix4 T = Matrix4::translation(translation);

        // Order: Scale first, then rotate, then translate
        return T * R * S;
    }

    // Apply transformation to point
    Vector3 transformPoint(const Vector3& point) const {
        // Apply in order: scale, rotate, translate
        Vector3 p = point;
        p = Vector3(p.x * scale.x, p.y * scale.y, p.z * scale.z);  // Scale
        p = Matrix4::rotationEuler(rotation.x, rotation.y, rotation.z).transformPoint(p);  // Rotate
        p = p + translation;  // Translate

        return p;
    }

    // Inverse transformation
    Transform inverse() const {
        Transform inv;

        // Inverse scale
        inv.scale = Vector3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

        // Inverse rotation (negate angles)
        inv.rotation = -rotation;

        // Inverse translation
        Matrix4 invRot = Matrix4::rotationEuler(-rotation.x, -rotation.y, -rotation.z);
        Vector3 scaledTrans(-translation.x / scale.x,
                            -translation.y / scale.y,
                            -translation.z / scale.z);
        inv.translation = invRot.transformPoint(scaledTrans);

        return inv;
    }
};
```

### Pivot Point Transformations

```cpp
// Rotate around arbitrary pivot point
Matrix4 rotateAroundPivot(const Vector3& pivot, const Vector3& axis,
                          float angle) {
    // 1. Translate pivot to origin
    Matrix4 T1 = Matrix4::translation(-pivot);

    // 2. Rotate
    Matrix4 R = Matrix4::rotationAxis(axis, angle);

    // 3. Translate back
    Matrix4 T2 = Matrix4::translation(pivot);

    // Combine: T2 * R * T1
    return T2 * R * T1;
}

// Scale around pivot
Matrix4 scaleAroundPivot(const Vector3& pivot, const Vector3& scale) {
    Matrix4 T1 = Matrix4::translation(-pivot);
    Matrix4 S = Matrix4::scale(scale);
    Matrix4 T2 = Matrix4::translation(pivot);

    return T2 * S * T1;
}
```

## Transformation Invariants

### What Remains Unchanged

```cpp
// Properties preserved by different transformations
struct TransformationProperties {
    // Translation preserves:
    // - Distances
    // - Angles
    // - Shapes
    // - Orientations

    // Rotation preserves:
    // - Distances
    // - Angles
    // - Shapes
    // Changes: orientations

    // Uniform scale preserves:
    // - Angles
    // - Shape proportions
    // Changes: distances, sizes

    // Non-uniform scale preserves:
    // - Collinearity (points on line stay on line)
    // Changes: angles, shapes, distances
};
```

## Complete Implementation

```cpp
// Full demonstration program
#include <iostream>
#include <iomanip>

void demonstrateTransformations() {
    std::cout << "=== Complete Transformation Demo ===" << std::endl << std::endl;

    Vector3 point(1, 0, 0);
    std::cout << "Original point: " << point << std::endl << std::endl;

    // Translation
    std::cout << "--- Translation ---" << std::endl;
    Matrix4 T = Matrix4::translation(5, 2, 0);
    Vector3 translated = T.transformPoint(point);
    std::cout << "After translate(5,2,0): " << translated << std::endl << std::endl;

    // Rotation
    std::cout << "--- Rotation ---" << std::endl;
    Matrix4 R = Matrix4::rotationZ(3.14159f / 2.0f);  // 90 degrees
    Vector3 rotated = R.transformPoint(point);
    std::cout << "After 90° Z rotation: " << rotated << std::endl << std::endl;

    // Scaling
    std::cout << "--- Scaling ---" << std::endl;
    Matrix4 S = Matrix4::scale(2, 3, 1);
    Vector3 scaled = S.transformPoint(point);
    std::cout << "After scale(2,3,1): " << scaled << std::endl << std::endl;

    // Combined TRS
    std::cout << "--- Combined TRS ---" << std::endl;
    Matrix4 TRS = T * R * S;
    Vector3 transformed = TRS.transformPoint(point);
    std::cout << "After T*R*S: " << transformed << std::endl << std::endl;

    // Extraction
    std::cout << "--- Extraction ---" << std::endl;
    std::cout << "Translation from TRS: " << TRS.getTranslation() << std::endl;
    std::cout << "Scale from TRS: " << TRS.getScale() << std::endl;
}

void demonstratePivotRotation() {
    std::cout << "\n=== Pivot Rotation Demo ===" << std::endl << std::endl;

    Vector3 point(5, 0, 0);
    Vector3 pivot(3, 0, 0);

    std::cout << "Point: " << point << std::endl;
    std::cout << "Pivot: " << pivot << std::endl << std::endl;

    Matrix4 rotateAroundPivot = rotateAroundPivot(pivot, Vector3(0, 0, 1),
                                                  3.14159f / 2.0f);

    Vector3 result = rotateAroundPivot.transformPoint(point);
    std::cout << "After 90° rotation around pivot: " << result << std::endl;
    std::cout << "Distance from pivot: " << (result - pivot).magnitude() << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    demonstrateTransformations();
    demonstratePivotRotation();

    return 0;
}
```

## Real-World Examples

### Character Controller

```cpp
class CharacterController {
    Vector3 position;
    float yaw;  // Rotation around Y
    float speed;

public:
    CharacterController() : position(0,0,0), yaw(0), speed(5.0f) {}

    void moveForward(float deltaTime) {
        Vector3 forward(std::sin(yaw), 0, std::cos(yaw));
        position = position + forward * speed * deltaTime;
    }

    void rotate(float deltaYaw) {
        yaw += deltaYaw;
        // Keep in [0, 2π]
        while (yaw > 2 * 3.14159f) yaw -= 2 * 3.14159f;
        while (yaw < 0) yaw += 2 * 3.14159f;
    }

    Matrix4 getTransform() const {
        return Matrix4::translation(position) * Matrix4::rotationY(yaw);
    }
};
```

### Orbital Camera

```cpp
class OrbitalCamera {
    Vector3 target;
    float distance;
    float azimuth;  // Horizontal angle
    float elevation;  // Vertical angle

public:
    Vector3 getPosition() const {
        float x = distance * std::cos(elevation) * std::sin(azimuth);
        float y = distance * std::sin(elevation);
        float z = distance * std::cos(elevation) * std::cos(azimuth);

        return target + Vector3(x, y, z);
    }

    void orbit(float deltaAzimuth, float deltaElevation) {
        azimuth += deltaAzimuth;
        elevation += deltaElevation;

        // Clamp elevation
        elevation = std::max(-1.5f, std::min(1.5f, elevation));
    }
};
```

## Common Pitfalls

### 1. Transformation Order

```cpp
// WRONG: Translate then rotate - object orbits around origin
Matrix4 wrong = Matrix4::rotationY(angle) * Matrix4::translation(5, 0, 0);

// CORRECT: Rotate then translate - object rotates in place, then moves
Matrix4 correct = Matrix4::translation(5, 0, 0) * Matrix4::rotationY(angle);
```

### 2. Forgetting Pivot Points

```cpp
// WRONG: Rotate around origin instead of center
Matrix4 wrong = Matrix4::rotationZ(angle);

// CORRECT: Rotate around object center
Vector3 center = getObjectCenter();
Matrix4 correct = Matrix4::translation(center) *
                  Matrix4::rotationZ(angle) *
                  Matrix4::translation(-center);
```

### 3. Gimbal Lock

```cpp
// Solution: Use quaternions (covered in Lessons 11-12)
// Or: Avoid 90° rotations in Euler angles
```

## Key Takeaways

1. **Translation** moves objects without changing orientation
2. **Rotation** changes orientation around a point/axis
3. **Scaling** changes size uniformly or non-uniformly
4. **Standard order**: Scale → Rotate → Translate (T*R*S)
5. **Pivot transformations**: Translate to origin, transform, translate back
6. **Euler angles** suffer from gimbal lock
7. **Axis-angle** rotation works for any arbitrary axis
8. **Negative scale** creates reflections
9. **Transformation order matters**: AB ≠ BA
10. **Extract components** from combined matrices when needed

## Exercises

1. Implement a smooth interpolation between two transforms
2. Create a billboard system (sprite always faces camera)
3. Write a function to decompose any matrix into TRS components
4. Implement a spring-based camera follow system
5. Create a hierarchical transform system (parent-child relationships)

## What's Next?

In **Lesson 08**, we'll master matrix multiplication and composition:
- Advanced multiplication techniques
- Matrix stacks and hierarchies
- Optimization strategies
- Complex transformation chains
- Practical composition patterns

---

**Lesson 07 Complete** - Words: ~4,500 | Complete TRS System | Next: Lesson 08 - Matrix Multiplication and Composition
