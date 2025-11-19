#pragma once
#include "Vector3.h"
#include <cmath>
#include <cstring>
#include <iostream>

namespace Math3D {

class Matrix4 {
public:
    // Column-major order (like OpenGL)
    float m[16];

    // Constructors
    Matrix4() {
        Identity();
    }

    Matrix4(float diagonal) {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = diagonal;
    }

    Matrix4(const float* values) {
        std::memcpy(m, values, sizeof(m));
    }

    // Element access
    float& operator[](int index) {
        return m[index];
    }

    const float& operator[](int index) const {
        return m[index];
    }

    float& operator()(int row, int col) {
        return m[col * 4 + row];
    }

    const float& operator()(int row, int col) const {
        return m[col * 4 + row];
    }

    // Matrix operations
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                float sum = 0.0f;
                for (int i = 0; i < 4; i++) {
                    sum += (*this)(row, i) * other(i, col);
                }
                result(row, col) = sum;
            }
        }
        return result;
    }

    Matrix4& operator*=(const Matrix4& other) {
        *this = *this * other;
        return *this;
    }

    Vector3 operator*(const Vector3& vec) const {
        float w = m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15];
        if (w == 0.0f) w = 1.0f;

        return Vector3(
            (m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12]) / w,
            (m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13]) / w,
            (m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14]) / w
        );
    }

    // Transform vector (ignores translation)
    Vector3 TransformVector(const Vector3& vec) const {
        return Vector3(
            m[0] * vec.x + m[4] * vec.y + m[8] * vec.z,
            m[1] * vec.x + m[5] * vec.y + m[9] * vec.z,
            m[2] * vec.x + m[6] * vec.y + m[10] * vec.z
        );
    }

    // Identity matrix
    void Identity() {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Matrix4 Identity() {
        return Matrix4(1.0f);
    }

    // Translation
    static Matrix4 Translation(const Vector3& translation) {
        Matrix4 result;
        result.m[12] = translation.x;
        result.m[13] = translation.y;
        result.m[14] = translation.z;
        return result;
    }

    static Matrix4 Translation(float x, float y, float z) {
        return Translation(Vector3(x, y, z));
    }

    // Scaling
    static Matrix4 Scale(const Vector3& scale) {
        Matrix4 result;
        result.m[0] = scale.x;
        result.m[5] = scale.y;
        result.m[10] = scale.z;
        return result;
    }

    static Matrix4 Scale(float x, float y, float z) {
        return Scale(Vector3(x, y, z));
    }

    static Matrix4 Scale(float uniform) {
        return Scale(Vector3(uniform, uniform, uniform));
    }

    // Rotation around X axis
    static Matrix4 RotationX(float angle) {
        Matrix4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[5] = c;
        result.m[6] = s;
        result.m[9] = -s;
        result.m[10] = c;
        return result;
    }

    // Rotation around Y axis
    static Matrix4 RotationY(float angle) {
        Matrix4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[0] = c;
        result.m[2] = -s;
        result.m[8] = s;
        result.m[10] = c;
        return result;
    }

    // Rotation around Z axis
    static Matrix4 RotationZ(float angle) {
        Matrix4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[0] = c;
        result.m[1] = s;
        result.m[4] = -s;
        result.m[5] = c;
        return result;
    }

    // Rotation around arbitrary axis
    static Matrix4 RotationAxis(const Vector3& axis, float angle) {
        Matrix4 result;
        Vector3 a = axis.Normalized();
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1.0f - c;

        result.m[0] = t * a.x * a.x + c;
        result.m[1] = t * a.x * a.y + s * a.z;
        result.m[2] = t * a.x * a.z - s * a.y;
        result.m[3] = 0.0f;

        result.m[4] = t * a.x * a.y - s * a.z;
        result.m[5] = t * a.y * a.y + c;
        result.m[6] = t * a.y * a.z + s * a.x;
        result.m[7] = 0.0f;

        result.m[8] = t * a.x * a.z + s * a.y;
        result.m[9] = t * a.y * a.z - s * a.x;
        result.m[10] = t * a.z * a.z + c;
        result.m[11] = 0.0f;

        result.m[12] = 0.0f;
        result.m[13] = 0.0f;
        result.m[14] = 0.0f;
        result.m[15] = 1.0f;

        return result;
    }

    // Look At matrix
    static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 zaxis = (eye - target).Normalized();
        Vector3 xaxis = up.Cross(zaxis).Normalized();
        Vector3 yaxis = zaxis.Cross(xaxis);

        Matrix4 result;
        result.m[0] = xaxis.x;
        result.m[4] = xaxis.y;
        result.m[8] = xaxis.z;
        result.m[12] = -xaxis.Dot(eye);

        result.m[1] = yaxis.x;
        result.m[5] = yaxis.y;
        result.m[9] = yaxis.z;
        result.m[13] = -yaxis.Dot(eye);

        result.m[2] = zaxis.x;
        result.m[6] = zaxis.y;
        result.m[10] = zaxis.z;
        result.m[14] = -zaxis.Dot(eye);

        result.m[3] = 0.0f;
        result.m[7] = 0.0f;
        result.m[11] = 0.0f;
        result.m[15] = 1.0f;

        return result;
    }

    // Perspective projection
    static Matrix4 Perspective(float fovY, float aspect, float near, float far) {
        Matrix4 result;
        std::memset(result.m, 0, sizeof(result.m));

        float tanHalfFovy = std::tan(fovY / 2.0f);

        result.m[0] = 1.0f / (aspect * tanHalfFovy);
        result.m[5] = 1.0f / tanHalfFovy;
        result.m[10] = -(far + near) / (far - near);
        result.m[11] = -1.0f;
        result.m[14] = -(2.0f * far * near) / (far - near);

        return result;
    }

    // Orthographic projection
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far) {
        Matrix4 result;
        std::memset(result.m, 0, sizeof(result.m));

        result.m[0] = 2.0f / (right - left);
        result.m[5] = 2.0f / (top - bottom);
        result.m[10] = -2.0f / (far - near);
        result.m[12] = -(right + left) / (right - left);
        result.m[13] = -(top + bottom) / (top - bottom);
        result.m[14] = -(far + near) / (far - near);
        result.m[15] = 1.0f;

        return result;
    }

    // Transpose
    Matrix4 Transpose() const {
        Matrix4 result;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result(row, col) = (*this)(col, row);
            }
        }
        return result;
    }

    // Inverse (simplified for common cases)
    Matrix4 Inverse() const {
        Matrix4 result;
        float* inv = result.m;
        const float* m = this->m;

        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
                 m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
                 m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
                 m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
                  m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
                 m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
                 m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
                 m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
                  m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
                 m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
                 m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
                  m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
                  m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
                 m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
                 m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
                  m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
                  m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0.0f) {
            return Matrix4::Identity();
        }

        det = 1.0f / det;

        for (int i = 0; i < 16; i++) {
            inv[i] *= det;
        }

        return result;
    }

    // Extract translation
    Vector3 GetTranslation() const {
        return Vector3(m[12], m[13], m[14]);
    }

    // Extract scale (approximation)
    Vector3 GetScale() const {
        Vector3 xaxis(m[0], m[1], m[2]);
        Vector3 yaxis(m[4], m[5], m[6]);
        Vector3 zaxis(m[8], m[9], m[10]);
        return Vector3(xaxis.Length(), yaxis.Length(), zaxis.Length());
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& mat) {
        os << "Matrix4:\n";
        for (int row = 0; row < 4; row++) {
            os << "[";
            for (int col = 0; col < 4; col++) {
                os << mat(row, col);
                if (col < 3) os << ", ";
            }
            os << "]\n";
        }
        return os;
    }
};

} // namespace Math3D
