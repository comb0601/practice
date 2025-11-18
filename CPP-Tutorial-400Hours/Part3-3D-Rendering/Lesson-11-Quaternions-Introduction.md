# Lesson 11: Quaternions - Introduction

## Overview
Quaternions are a powerful mathematical tool for representing rotations in 3D space. They solve many problems inherent in Euler angles and rotation matrices, including gimbal lock, efficient interpolation, and compact storage. This lesson introduces quaternion mathematics with complete C++ implementations.

## Table of Contents
1. [What Are Quaternions?](#what-are-quaternions)
2. [Quaternion Mathematics](#quaternion-mathematics)
3. [Quaternion Operations](#quaternion-operations)
4. [Rotation Representation](#rotation-representation)
5. [Complete Implementation](#complete-implementation)

## What Are Quaternions?

### Mathematical Definition

```cpp
// A quaternion is a 4-component number: q = w + xi + yj + zk
// where i² = j² = k² = ijk = -1
struct Quaternion {
    float w; // Real/scalar part
    float x, y, z; // Imaginary/vector part
    
    Quaternion(float w = 1, float x = 0, float y = 0, float z = 0)
        : w(w), x(x), y(y), z(z) {}
    
    // Alternative: scalar + vector form
    Quaternion(float scalar, const Vector3& vector)
        : w(scalar), x(vector.x), y(vector.y), z(vector.z) {}
};
```

### Why Quaternions?

```cpp
// Advantages over Euler angles:
class QuaternionAdvantages {
public:
    // 1. NO GIMBAL LOCK
    // 2. Smooth interpolation (slerp)
    // 3. Efficient composition
    // 4. Compact storage (4 floats vs 9 for matrix)
    // 5. Numerically stable
    
    // Disadvantages:
    // 1. Less intuitive
    // 2. Requires normalization
    // 3. Not directly visualizable
};
```

## Quaternion Mathematics

### Basic Operations

```cpp
class Quaternion {
public:
    float w, x, y, z;
    
    Quaternion(float w=1, float x=0, float y=0, float z=0)
        : w(w), x(x), y(y), z(z) {}
    
    // Addition
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }
    
    // Scalar multiplication
    Quaternion operator*(float s) const {
        return Quaternion(w*s, x*s, y*s, z*s);
    }
    
    // Quaternion multiplication (Hamilton product)
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z, // Scalar part
            w*q.x + x*q.w + y*q.z - z*q.y, // i component
            w*q.y - x*q.z + y*q.w + z*q.x, // j component
            w*q.z + x*q.y - y*q.x + z*q.w  // k component
        );
    }
    
    // Magnitude
    float magnitude() const {
        return sqrt(w*w + x*x + y*y + z*z);
    }
    
    // Normalize
    Quaternion normalized() const {
        float mag = magnitude();
        if (mag < 1e-8f) return Quaternion();
        return (*this) * (1.0f / mag);
    }
    
    void normalize() {
        *this = normalized();
    }
    
    // Conjugate
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }
    
    // Inverse
    Quaternion inverse() const {
        float magSq = w*w + x*x + y*y + z*z;
        if (magSq < 1e-8f) return Quaternion();
        return conjugate() * (1.0f / magSq);
    }
    
    // Dot product
    float dot(const Quaternion& q) const {
        return w*q.w + x*q.x + y*q.y + z*q.z;
    }
};
```

### Unit Quaternions

```cpp
// Unit quaternions (magnitude = 1) represent rotations
class UnitQuaternion {
public:
    // All rotation quaternions MUST be unit quaternions
    static bool isUnit(const Quaternion& q, float epsilon = 1e-5f) {
        return abs(q.magnitude() - 1.0f) < epsilon;
    }
    
    // Identity rotation
    static Quaternion identity() {
        return Quaternion(1, 0, 0, 0);
    }
    
    // Verify unit quaternion
    static Quaternion ensureUnit(const Quaternion& q) {
        if (!isUnit(q)) {
            std::cerr << "Warning: Non-unit quaternion, normalizing\n";
            return q.normalized();
        }
        return q;
    }
};
```

## Quaternion Operations

### Creating Rotations

```cpp
class Quaternion {
public:
    // From axis-angle representation
    static Quaternion fromAxisAngle(const Vector3& axis, float angleRadians) {
        float halfAngle = angleRadians * 0.5f;
        float s = sin(halfAngle);
        Vector3 a = axis.normalized();
        
        return Quaternion(
            cos(halfAngle),
            a.x * s,
            a.y * s,
            a.z * s
        );
    }
    
    // To axis-angle
    void toAxisAngle(Vector3& outAxis, float& outAngle) const {
        Quaternion q = normalized();
        
        outAngle = 2.0f * acos(q.w);
        float s = sqrt(1.0f - q.w * q.w);
        
        if (s < 1e-6f) {
            outAxis = Vector3(1, 0, 0); // Any axis works for zero rotation
        } else {
            outAxis = Vector3(q.x / s, q.y / s, q.z / s);
        }
    }
    
    // From Euler angles (ZYX order)
    static Quaternion fromEuler(float pitch, float yaw, float roll) {
        float cy = cos(yaw * 0.5f);
        float sy = sin(yaw * 0.5f);
        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);
        float cr = cos(roll * 0.5f);
        float sr = sin(roll * 0.5f);
        
        return Quaternion(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        );
    }
    
    // To Euler angles
    Vector3 toEuler() const {
        Vector3 euler;
        
        // Pitch (x-axis rotation)
        float sinp = 2.0f * (w * y - z * x);
        if (abs(sinp) >= 1)
            euler.x = copysign(PI / 2, sinp); // Gimbal lock
        else
            euler.x = asin(sinp);
        
        // Yaw (y-axis rotation)
        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        euler.y = atan2(siny_cosp, cosy_cosp);
        
        // Roll (z-axis rotation)
        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        euler.z = atan2(sinr_cosp, cosr_cosp);
        
        return euler;
    }
    
    // From rotation matrix
    static Quaternion fromMatrix(const Matrix3& m) {
        Quaternion q;
        float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
        
        if (trace > 0) {
            float s = 0.5f / sqrt(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m.m[2][1] - m.m[1][2]) * s;
            q.y = (m.m[0][2] - m.m[2][0]) * s;
            q.z = (m.m[1][0] - m.m[0][1]) * s;
        } else if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
            float s = 2.0f * sqrt(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
            q.w = (m.m[2][1] - m.m[1][2]) / s;
            q.x = 0.25f * s;
            q.y = (m.m[0][1] + m.m[1][0]) / s;
            q.z = (m.m[0][2] + m.m[2][0]) / s;
        } else if (m.m[1][1] > m.m[2][2]) {
            float s = 2.0f * sqrt(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
            q.w = (m.m[0][2] - m.m[2][0]) / s;
            q.x = (m.m[0][1] + m.m[1][0]) / s;
            q.y = 0.25f * s;
            q.z = (m.m[1][2] + m.m[2][1]) / s;
        } else {
            float s = 2.0f * sqrt(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
            q.w = (m.m[1][0] - m.m[0][1]) / s;
            q.x = (m.m[0][2] + m.m[2][0]) / s;
            q.y = (m.m[1][2] + m.m[2][1]) / s;
            q.z = 0.25f * s;
        }
        
        return q.normalized();
    }
    
    // To rotation matrix
    Matrix3 toMatrix() const {
        Quaternion q = normalized();
        
        Matrix3 m;
        
        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;
        
        m.m[0][0] = 1.0f - 2.0f * (yy + zz);
        m.m[0][1] = 2.0f * (xy - wz);
        m.m[0][2] = 2.0f * (xz + wy);
        
        m.m[1][0] = 2.0f * (xy + wz);
        m.m[1][1] = 1.0f - 2.0f * (xx + zz);
        m.m[1][2] = 2.0f * (yz - wx);
        
        m.m[2][0] = 2.0f * (xz - wy);
        m.m[2][1] = 2.0f * (yz + wx);
        m.m[2][2] = 1.0f - 2.0f * (xx + yy);
        
        return m;
    }
};
```

## Rotation Representation

### Rotating Vectors

```cpp
class Quaternion {
public:
    // Rotate vector by quaternion
    Vector3 rotate(const Vector3& v) const {
        Quaternion q = normalized();
        
        // q * v * q^-1 (for unit quaternions, q^-1 = q*)
        Quaternion vq(0, v.x, v.y, v.z);
        Quaternion result = q * vq * q.conjugate();
        
        return Vector3(result.x, result.y, result.z);
    }
    
    // Optimized rotation (fewer multiplications)
    Vector3 rotateFast(const Vector3& v) const {
        Vector3 qvec(x, y, z);
        Vector3 uv = qvec.cross(v);
        Vector3 uuv = qvec.cross(uv);
        
        return v + ((uv * w) + uuv) * 2.0f;
    }
};
```

## Complete Implementation

```cpp
#include <iostream>
#include <cmath>

class Quaternion {
public:
    float w, x, y, z;
    
    Quaternion(float w=1, float x=0, float y=0, float z=0)
        : w(w), x(x), y(y), z(z) {}
    
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        );
    }
    
    float magnitude() const {
        return sqrt(w*w + x*x + y*y + z*z);
    }
    
    Quaternion normalized() const {
        float mag = magnitude();
        return (mag > 1e-8f) ? (*this * (1.0f/mag)) : Quaternion();
    }
    
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }
    
    Vector3 rotate(const Vector3& v) const {
        Vector3 qvec(x, y, z);
        Vector3 uv = qvec.cross(v);
        Vector3 uuv = qvec.cross(uv);
        return v + ((uv * w) + uuv) * 2.0f;
    }
    
    static Quaternion fromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = sin(halfAngle);
        Vector3 a = axis.normalized();
        return Quaternion(cos(halfAngle), a.x*s, a.y*s, a.z*s);
    }
    
    void print() const {
        std::cout << "Quat(" << w << ", " << x << ", "
                  << y << ", " << z << ")\n";
    }
};

int main() {
    // Create 90-degree rotation around Z-axis
    Quaternion q = Quaternion::fromAxisAngle(Vector3(0,0,1), PI/2);
    q.print();
    
    // Rotate vector
    Vector3 v(1, 0, 0);
    Vector3 rotated = q.rotate(v);
    std::cout << "Rotated: " << rotated << "\n";
    
    return 0;
}
```

## Key Takeaways

1. **Quaternions** are 4D complex numbers: w + xi + yj + zk
2. **Unit quaternions** (magnitude=1) represent rotations
3. **No gimbal lock** unlike Euler angles
4. **Efficient composition** through multiplication
5. **Compact storage**: 4 floats vs 9 for matrix
6. **Hamilton product** is non-commutative: qp ≠ pq
7. **Conjugate** of unit quaternion is its inverse
8. **axis-angle** conversion is straightforward
9. **Vector rotation**: v' = q * v * q*
10. **Always normalize** after operations to maintain unit length

---

**Lesson 11 Complete** - Next: Lesson 12 - Quaternions Rotations and Interpolation
