/*
 * Lesson 56: Vector Operations
 * Example 01: Dot Product Basics
 *
 * Compile: g++ -o 01_DotProduct 01_DotProduct.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? Vector3(x/len, y/len, z/len) : Vector3(0,0,0);
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Dot Product Demonstrations ===" << std::endl << std::endl;

    // 1. Basic calculation
    std::cout << "1. BASIC DOT PRODUCT:" << std::endl;
    Vector3 v1(3, 4, 0);
    Vector3 v2(1, 2, 0);
    float dot = v1.Dot(v2);

    std::cout << "  v1 = "; v1.Print(); std::cout << std::endl;
    std::cout << "  v2 = "; v2.Print(); std::cout << std::endl;
    std::cout << "  v1 · v2 = " << v1.x << "*" << v2.x << " + "
              << v1.y << "*" << v2.y << " + " << v1.z << "*" << v2.z << std::endl;
    std::cout << "         = " << dot << std::endl << std::endl;

    // 2. Perpendicular vectors
    std::cout << "2. PERPENDICULAR VECTORS (dot = 0):" << std::endl;
    Vector3 xAxis(1, 0, 0);
    Vector3 yAxis(0, 1, 0);
    std::cout << "  X-axis · Y-axis = " << xAxis.Dot(yAxis) << std::endl << std::endl;

    // 3. Parallel vectors
    std::cout << "3. PARALLEL VECTORS:" << std::endl;
    Vector3 v3(2, 0, 0);
    Vector3 v4(4, 0, 0);
    std::cout << "  v3 = "; v3.Print();
    std::cout << ", v4 = "; v4.Print();
    std::cout << ", dot = " << v3.Dot(v4) << std::endl;
    std::cout << "  (Positive = same direction)" << std::endl << std::endl;

    // 4. Opposite vectors
    std::cout << "4. OPPOSITE VECTORS:" << std::endl;
    Vector3 v5(1, 0, 0);
    Vector3 v6(-1, 0, 0);
    std::cout << "  v5 = "; v5.Print();
    std::cout << ", v6 = "; v6.Print();
    std::cout << ", dot = " << v5.Dot(v6) << std::endl;
    std::cout << "  (Negative = opposite direction)" << std::endl << std::endl;

    // 5. Self dot product = length squared
    std::cout << "5. SELF DOT PRODUCT:" << std::endl;
    Vector3 v7(3, 4, 0);
    std::cout << "  v = "; v7.Print(); std::cout << std::endl;
    std::cout << "  v · v = " << v7.Dot(v7) << std::endl;
    std::cout << "  |v|² = " << (v7.Length() * v7.Length()) << std::endl;
    std::cout << "  (Self dot = length squared)" << std::endl << std::endl;

    // 6. Angle interpretation
    std::cout << "6. ANGLE INTERPRETATION:" << std::endl;
    Vector3 a = Vector3(1, 1, 0).Normalized();
    Vector3 b = Vector3(1, 0, 0);
    float dotAB = a.Dot(b);
    float angle = std::acos(dotAB) * 180.0f / 3.14159f;

    std::cout << "  a = "; a.Print(); std::cout << " (normalized)" << std::endl;
    std::cout << "  b = "; b.Print(); std::cout << " (unit)" << std::endl;
    std::cout << "  cos(θ) = a · b = " << dotAB << std::endl;
    std::cout << "  θ = " << angle << " degrees" << std::endl;

    return 0;
}
