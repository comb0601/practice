#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>
#include <iostream>

namespace Math3D {

class Quaternion {
public:
    float x, y, z, w;

    // Constructors
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        Vector3 normalizedAxis = axis.Normalized();

        x = normalizedAxis.x * s;
        y = normalizedAxis.y * s;
        z = normalizedAxis.z * s;
        w = std::cos(halfAngle);
    }

    // From Euler angles (in radians)
    static Quaternion FromEuler(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    // To Euler angles (in radians)
    Vector3 ToEuler() const {
        Vector3 angles;

        // Roll (x-axis rotation)
        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        float sinp = 2.0f * (w * y - z * x);
        if (std::abs(sinp) >= 1.0f)
            angles.y = std::copysign(3.14159265f / 2.0f, sinp); // use 90 degrees if out of range
        else
            angles.y = std::asin(sinp);

        // Yaw (z-axis rotation)
        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    // Quaternion operations
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Rotate a vector by this quaternion
    Vector3 operator*(const Vector3& vec) const {
        Vector3 qvec(x, y, z);
        Vector3 uv = qvec.Cross(vec);
        Vector3 uuv = qvec.Cross(uv);

        return vec + ((uv * w) + uuv) * 2.0f;
    }

    // Length and normalization
    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    Quaternion Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            return Quaternion(x * invLen, y * invLen, z * invLen, w * invLen);
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
            w *= invLen;
        }
    }

    // Conjugate (inverse rotation for unit quaternions)
    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    // Inverse
    Quaternion Inverse() const {
        float lenSq = LengthSquared();
        if (lenSq > 0.0f) {
            Quaternion conj = Conjugate();
            float invLenSq = 1.0f / lenSq;
            return Quaternion(
                conj.x * invLenSq,
                conj.y * invLenSq,
                conj.z * invLenSq,
                conj.w * invLenSq
            );
        }
        return *this;
    }

    // Dot product
    float Dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Spherical linear interpolation
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion qa = a.Normalized();
        Quaternion qb = b.Normalized();

        float dot = qa.Dot(qb);

        // If the dot product is negative, slerp won't take the shorter path
        if (dot < 0.0f) {
            qb = Quaternion(-qb.x, -qb.y, -qb.z, -qb.w);
            dot = -dot;
        }

        const float DOT_THRESHOLD = 0.9995f;
        if (dot > DOT_THRESHOLD) {
            // If the inputs are too close for comfort, linearly interpolate
            Quaternion result = Quaternion(
                qa.x + t * (qb.x - qa.x),
                qa.y + t * (qb.y - qa.y),
                qa.z + t * (qb.z - qa.z),
                qa.w + t * (qb.w - qa.w)
            );
            return result.Normalized();
        }

        float theta_0 = std::acos(dot);
        float theta = theta_0 * t;
        float sin_theta = std::sin(theta);
        float sin_theta_0 = std::sin(theta_0);

        float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        return Quaternion(
            qa.x * s0 + qb.x * s1,
            qa.y * s0 + qb.y * s1,
            qa.z * s0 + qb.z * s1,
            qa.w * s0 + qb.w * s1
        );
    }

    // Convert to rotation matrix
    Matrix4 ToMatrix() const {
        Matrix4 mat;

        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        mat.m[0] = 1.0f - 2.0f * (yy + zz);
        mat.m[1] = 2.0f * (xy + wz);
        mat.m[2] = 2.0f * (xz - wy);
        mat.m[3] = 0.0f;

        mat.m[4] = 2.0f * (xy - wz);
        mat.m[5] = 1.0f - 2.0f * (xx + zz);
        mat.m[6] = 2.0f * (yz + wx);
        mat.m[7] = 0.0f;

        mat.m[8] = 2.0f * (xz + wy);
        mat.m[9] = 2.0f * (yz - wx);
        mat.m[10] = 1.0f - 2.0f * (xx + yy);
        mat.m[11] = 0.0f;

        mat.m[12] = 0.0f;
        mat.m[13] = 0.0f;
        mat.m[14] = 0.0f;
        mat.m[15] = 1.0f;

        return mat;
    }

    // Static utility functions
    static Quaternion Identity() {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Quaternion RotationX(float angle) {
        float halfAngle = angle * 0.5f;
        return Quaternion(std::sin(halfAngle), 0.0f, 0.0f, std::cos(halfAngle));
    }

    static Quaternion RotationY(float angle) {
        float halfAngle = angle * 0.5f;
        return Quaternion(0.0f, std::sin(halfAngle), 0.0f, std::cos(halfAngle));
    }

    static Quaternion RotationZ(float angle) {
        float halfAngle = angle * 0.5f;
        return Quaternion(0.0f, 0.0f, std::sin(halfAngle), std::cos(halfAngle));
    }

    // Look rotation (creates rotation from direction)
    static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3::Up()) {
        Vector3 f = forward.Normalized();
        Vector3 r = up.Cross(f).Normalized();
        Vector3 u = f.Cross(r);

        Matrix4 mat;
        mat.m[0] = r.x; mat.m[4] = r.y; mat.m[8] = r.z;
        mat.m[1] = u.x; mat.m[5] = u.y; mat.m[9] = u.z;
        mat.m[2] = f.x; mat.m[6] = f.y; mat.m[10] = f.z;

        // Convert matrix to quaternion
        float trace = mat.m[0] + mat.m[5] + mat.m[10];
        Quaternion q;

        if (trace > 0.0f) {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (mat.m[6] - mat.m[9]) * s;
            q.y = (mat.m[8] - mat.m[2]) * s;
            q.z = (mat.m[1] - mat.m[4]) * s;
        } else if (mat.m[0] > mat.m[5] && mat.m[0] > mat.m[10]) {
            float s = 2.0f * std::sqrt(1.0f + mat.m[0] - mat.m[5] - mat.m[10]);
            q.w = (mat.m[6] - mat.m[9]) / s;
            q.x = 0.25f * s;
            q.y = (mat.m[4] + mat.m[1]) / s;
            q.z = (mat.m[8] + mat.m[2]) / s;
        } else if (mat.m[5] > mat.m[10]) {
            float s = 2.0f * std::sqrt(1.0f + mat.m[5] - mat.m[0] - mat.m[10]);
            q.w = (mat.m[8] - mat.m[2]) / s;
            q.x = (mat.m[4] + mat.m[1]) / s;
            q.y = 0.25f * s;
            q.z = (mat.m[9] + mat.m[6]) / s;
        } else {
            float s = 2.0f * std::sqrt(1.0f + mat.m[10] - mat.m[0] - mat.m[5]);
            q.w = (mat.m[1] - mat.m[4]) / s;
            q.x = (mat.m[8] + mat.m[2]) / s;
            q.y = (mat.m[9] + mat.m[6]) / s;
            q.z = 0.25f * s;
        }

        return q.Normalized();
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        os << "Quaternion(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
        return os;
    }
};

} // namespace Math3D
