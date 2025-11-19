/*
 * Lesson 61 - Projection Matrices
 * Perspective and orthographic projection
 *
 * Compilation: g++ -std=c++11 -o output filename.cpp -lm
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>

#define PI 3.14159265358979323846f

class Vector4 {
public:
    float x, y, z, w;
    Vector4(float x=0, float y=0, float z=0, float w=1) : x(x), y(y), z(z), w(w) {}
    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
    }
};

class Matrix4x4 {
public:
    float m[16];

    Matrix4x4() {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    float& operator()(int row, int col) {
        return m[row*4 + col];
    }

    Vector4 Transform(const Vector4& v) const {
        return Vector4(
            m[0]*v.x + m[1]*v.y + m[2]*v.z + m[3]*v.w,
            m[4]*v.x + m[5]*v.y + m[6]*v.z + m[7]*v.w,
            m[8]*v.x + m[9]*v.y + m[10]*v.z + m[11]*v.w,
            m[12]*v.x + m[13]*v.y + m[14]*v.z + m[15]*v.w
        );
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(4);
        for (int row = 0; row < 4; row++) {
            std::cout << "[ ";
            for (int col = 0; col < 4; col++)
                std::cout << std::setw(9) << m[row*4+col] << " ";
            std::cout << "]\n";
        }
    }

    static Matrix4x4 PerspectiveFOV(float fov, float aspect, float nearZ, float farZ) {
        float tanHalfFov = tan(fov / 2.0f);
        Matrix4x4 result;
        memset(result.m, 0, sizeof(result.m));

        result(0, 0) = 1.0f / (aspect * tanHalfFov);
        result(1, 1) = 1.0f / tanHalfFov;
        result(2, 2) = farZ / (farZ - nearZ);
        result(2, 3) = -(farZ * nearZ) / (farZ - nearZ);
        result(3, 2) = 1.0f;
        result(3, 3) = 0.0f;

        return result;
    }

    static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
        Matrix4x4 result;
        memset(result.m, 0, sizeof(result.m));

        result(0, 0) = 2.0f / (right - left);
        result(1, 1) = 2.0f / (top - bottom);
        result(2, 2) = 1.0f / (farZ - nearZ);
        result(0, 3) = -(right + left) / (right - left);
        result(1, 3) = -(top + bottom) / (top - bottom);
        result(2, 3) = -nearZ / (farZ - nearZ);
        result(3, 3) = 1.0f;

        return result;
    }
};

int main() {
    std::cout << "=== Lesson 61: Projection Matrices ===\n\n";

    // Perspective projection
    float fov = PI / 3.0f;  // 60 degrees
    float aspect = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    std::cout << "Perspective Projection Matrix:\n";
    std::cout << "FOV: 60°, Aspect: 16:9, Near: 0.1, Far: 1000\n";
    Matrix4x4 perspective = Matrix4x4::PerspectiveFOV(fov, aspect, nearPlane, farPlane);
    perspective.Print();

    // Test point
    Vector4 point(1, 1, 10, 1);  // Point at z=10
    std::cout << "\nOriginal point: "; point.Print(); std::cout << "\n";

    Vector4 projected = perspective.Transform(point);
    std::cout << "After projection: "; projected.Print(); std::cout << "\n";

    // Perspective divide
    Vector4 ndc(projected.x/projected.w, projected.y/projected.w, projected.z/projected.w, 1);
    std::cout << "After perspective divide (NDC): "; ndc.Print(); std::cout << "\n";

    // Orthographic projection
    std::cout << "\n\nOrthographic Projection Matrix:\n";
    std::cout << "Left: -10, Right: 10, Bottom: -10, Top: 10, Near: 0.1, Far: 100\n";
    Matrix4x4 ortho = Matrix4x4::Orthographic(-10, 10, -10, 10, 0.1f, 100);
    ortho.Print();

    Vector4 orthoProjected = ortho.Transform(point);
    std::cout << "\nPoint after orthographic projection: ";
    orthoProjected.Print();
    std::cout << "\n";

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
