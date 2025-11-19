/*
 * Lesson 57 - Matrices & Matrix Operations
 * Complete implementation with detailed examples
 *
 * Compilation:
 * cl /EHsc filename.cpp
 * g++ -std=c++11 filename.cpp -o output
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>

class Vector3 {
public:
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class Matrix4x4 {
public:
    float m[16];  // Row-major: m[row*4 + col]

    Matrix4x4() {
        Identity();
    }

    Matrix4x4(const float* values) {
        memcpy(m, values, 16 * sizeof(float));
    }

    void Identity() {
        for (int i = 0; i < 16; i++)
            m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    void Zero() {
        memset(m, 0, 16 * sizeof(float));
    }

    float& operator()(int row, int col) {
        return m[row * 4 + col];
    }

    const float& operator()(int row, int col) const {
        return m[row * 4 + col];
    }

    Matrix4x4 operator*(const Matrix4x4& b) const {
        Matrix4x4 result;
        result.Zero();
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                for (int k = 0; k < 4; k++) {
                    result(row, col) += (*this)(row, k) * b(k, col);
                }
            }
        }
        return result;
    }

    Matrix4x4 Transpose() const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result(i, j) = (*this)(j, i);
        return result;
    }

    float Determinant() const {
        // Simplified determinant for 4x4 matrix
        return m[0]*m[5]*m[10]*m[15] - m[0]*m[5]*m[11]*m[14] +
               m[0]*m[6]*m[11]*m[13] - m[0]*m[6]*m[9]*m[15];
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        for (int row = 0; row < 4; row++) {
            std::cout << "[ ";
            for (int col = 0; col < 4; col++) {
                std::cout << std::setw(8) << (*this)(row, col) << " ";
            }
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

    static Matrix4x4 RotationZ(float angle) {
        Matrix4x4 result;
        float c = cos(angle);
        float s = sin(angle);
        result(0, 0) = c;  result(0, 1) = -s;
        result(1, 0) = s;  result(1, 1) = c;
        return result;
    }
};

int main() {
    std::cout << "=== Lesson 57: Matrix Operations ===\n\n";

    // Create identity matrix
    Matrix4x4 identity;
    std::cout << "Identity Matrix:\n";
    identity.Print();

    // Create translation matrix
    Matrix4x4 trans = Matrix4x4::Translation(5, 10, 15);
    std::cout << "\nTranslation(5, 10, 15):\n";
    trans.Print();

    // Create scale matrix
    Matrix4x4 scale = Matrix4x4::Scale(2, 3, 4);
    std::cout << "\nScale(2, 3, 4):\n";
    scale.Print();

    // Matrix multiplication
    Matrix4x4 combined = trans * scale;
    std::cout << "\nTranslation × Scale:\n";
    combined.Print();

    // Transpose
    Matrix4x4 transposed = combined.Transpose();
    std::cout << "\nTransposed:\n";
    transposed.Print();

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
