/*
 * Lesson 58 - Transformation Matrices
 * Translation, Rotation, Scaling, and Combined Transformations
 *
 * Compilation:
 * cl /EHsc filename.cpp
 * g++ -std=c++11 filename.cpp -o output -lm
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>

#define PI 3.14159265358979323846f

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "(" << std::setw(7) << x << ", "
                  << std::setw(7) << y << ", "
                  << std::setw(7) << z << ")";
    }
};

class Matrix4x4 {
public:
    float m[16];

    Matrix4x4() { Identity(); }

    void Identity() {
        for (int i = 0; i < 16; i++)
            m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    float& operator()(int row, int col) {
        return m[row * 4 + col];
    }

    const float& operator()(int row, int col) const {
        return m[row * 4 + col];
    }

    Matrix4x4 operator*(const Matrix4x4& b) const {
        Matrix4x4 result;
        memset(result.m, 0, 16 * sizeof(float));
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                for (int k = 0; k < 4; k++)
                    result(row, col) += (*this)(row, k) * b(k, col);
        return result;
    }

    Vector3 Transform(const Vector3& v) const {
        float x = m[0]*v.x + m[1]*v.y + m[2]*v.z + m[3];
        float y = m[4]*v.x + m[5]*v.y + m[6]*v.z + m[7];
        float z = m[8]*v.x + m[9]*v.y + m[10]*v.z + m[11];
        float w = m[12]*v.x + m[13]*v.y + m[14]*v.z + m[15];
        return Vector3(x/w, y/w, z/w);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        for (int row = 0; row < 4; row++) {
            std::cout << "[ ";
            for (int col = 0; col < 4; col++)
                std::cout << std::setw(8) << (*this)(row, col) << " ";
            std::cout << "]\n";
        }
    }

    static Matrix4x4 Translation(float x, float y, float z) {
        Matrix4x4 result;
        result(0, 3) = x;
        result(1, 3) = y;
        result(2, 3) = z;
        return result;
    }

    static Matrix4x4 Scale(float x, float y, float z) {
        Matrix4x4 result;
        result(0, 0) = x;
        result(1, 1) = y;
        result(2, 2) = z;
        return result;
    }

    static Matrix4x4 RotationX(float angle) {
        Matrix4x4 result;
        float c = cos(angle), s = sin(angle);
        result(1, 1) = c;  result(1, 2) = -s;
        result(2, 1) = s;  result(2, 2) = c;
        return result;
    }

    static Matrix4x4 RotationY(float angle) {
        Matrix4x4 result;
        float c = cos(angle), s = sin(angle);
        result(0, 0) = c;   result(0, 2) = s;
        result(2, 0) = -s;  result(2, 2) = c;
        return result;
    }

    static Matrix4x4 RotationZ(float angle) {
        Matrix4x4 result;
        float c = cos(angle), s = sin(angle);
        result(0, 0) = c;  result(0, 1) = -s;
        result(1, 0) = s;  result(1, 1) = c;
        return result;
    }
};

int main() {
    std::cout << "=== Lesson 58: Transformation Matrices ===\n\n";

    Vector3 point(1, 0, 0);
    std::cout << "Original point: "; point.Print(); std::cout << "\n\n";

    // Translation
    Matrix4x4 trans = Matrix4x4::Translation(5, 10, 0);
    std::cout << "Translation Matrix (5, 10, 0):\n";
    trans.Print();
    Vector3 translated = trans.Transform(point);
    std::cout << "Translated: "; translated.Print(); std::cout << "\n\n";

    // Rotation
    Matrix4x4 rot = Matrix4x4::RotationZ(PI/4);  // 45 degrees
    std::cout << "Rotation Matrix (45° around Z):\n";
    rot.Print();
    Vector3 rotated = rot.Transform(point);
    std::cout << "Rotated: "; rotated.Print(); std::cout << "\n\n";

    // Scaling
    Matrix4x4 scale = Matrix4x4::Scale(2, 3, 1);
    std::cout << "Scale Matrix (2, 3, 1):\n";
    scale.Print();
    Vector3 scaled = scale.Transform(point);
    std::cout << "Scaled: "; scaled.Print(); std::cout << "\n\n";

    // Combined transformation: Scale, then Rotate, then Translate
    Matrix4x4 combined = trans * rot * scale;
    std::cout << "Combined Matrix (T × R × S):\n";
    combined.Print();
    Vector3 final = combined.Transform(point);
    std::cout << "Final transformed: "; final.Print(); std::cout << "\n\n";

    std::cout << "=== Program Complete ===\n";
    return 0;
}
