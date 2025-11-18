#pragma once
#include <cmath>
#include <iostream>

namespace Math3D {

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}

    // Copy constructor and assignment
    Vector3(const Vector3& other) = default;
    Vector3& operator=(const Vector3& other) = default;

    // Array access
    float& operator[](int index) {
        return (&x)[index];
    }
    const float& operator[](int index) const {
        return (&x)[index];
    }

    // Arithmetic operators
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        float inv = 1.0f / scalar;
        return Vector3(x * inv, y * inv, z * inv);
    }

    // Compound assignment operators
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    // Unary operators
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // Comparison operators
    bool operator==(const Vector3& other) const {
        const float epsilon = 0.0001f;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }

    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    // Vector operations
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            return Vector3(x * invLen, y * invLen, z * invLen);
        }
        return *this;
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            x *= invLen;
            y *= invLen;
            z *= invLen;
        }
    }

    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float Distance(const Vector3& other) const {
        return (*this - other).Length();
    }

    float DistanceSquared(const Vector3& other) const {
        return (*this - other).LengthSquared();
    }

    // Linear interpolation
    Vector3 Lerp(const Vector3& other, float t) const {
        return *this + (other - *this) * t;
    }

    // Reflection
    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }

    // Projection
    Vector3 Project(const Vector3& other) const {
        float dot = Dot(other);
        float len = other.LengthSquared();
        return other * (dot / len);
    }

    // Static utility functions
    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 UnitX() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 UnitY() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 UnitZ() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
    static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, -1.0f); }
    static Vector3 Back() { return Vector3(0.0f, 0.0f, 1.0f); }

    static float Dot(const Vector3& a, const Vector3& b) {
        return a.Dot(b);
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b) {
        return a.Cross(b);
    }

    static float Distance(const Vector3& a, const Vector3& b) {
        return a.Distance(b);
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a.Lerp(b, t);
    }

    // Friend functions for scalar * vector
    friend Vector3 operator*(float scalar, const Vector3& vec) {
        return vec * scalar;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
};

} // namespace Math3D
