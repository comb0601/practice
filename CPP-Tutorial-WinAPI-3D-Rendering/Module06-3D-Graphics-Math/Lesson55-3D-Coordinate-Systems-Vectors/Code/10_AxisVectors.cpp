/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 10: Working with Coordinate Axes
 *
 * Compile: g++ -o 10_AxisVectors 10_AxisVectors.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    static Vector3 UnitX() { return Vector3(1, 0, 0); }
    static Vector3 UnitY() { return Vector3(0, 1, 0); }
    static Vector3 UnitZ() { return Vector3(0, 0, 1); }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    float DotProduct(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Working with Coordinate Axes ===" << std::endl;
    std::cout << std::endl;

    // 1. Basis vectors
    std::cout << "1. STANDARD BASIS VECTORS:" << std::endl;
    Vector3 xAxis = Vector3::UnitX();
    Vector3 yAxis = Vector3::UnitY();
    Vector3 zAxis = Vector3::UnitZ();

    std::cout << "  X-axis: ";
    xAxis.Print();
    std::cout << std::endl;
    std::cout << "  Y-axis: ";
    yAxis.Print();
    std::cout << std::endl;
    std::cout << "  Z-axis: ";
    zAxis.Print();
    std::cout << std::endl << std::endl;

    // 2. Projecting onto axes
    std::cout << "2. PROJECTING VECTOR ONTO AXES:" << std::endl;
    Vector3 v(3, 4, 5);

    std::cout << "  Vector v = ";
    v.Print();
    std::cout << std::endl;
    std::cout << "  X-component: " << v.DotProduct(xAxis) << std::endl;
    std::cout << "  Y-component: " << v.DotProduct(yAxis) << std::endl;
    std::cout << "  Z-component: " << v.DotProduct(zAxis) << std::endl;
    std::cout << std::endl;

    // 3. Reconstructing vector
    std::cout << "3. RECONSTRUCTING VECTOR FROM COMPONENTS:" << std::endl;
    Vector3 reconstructed = xAxis * v.x + yAxis * v.y + zAxis * v.z;
    std::cout << "  Original:      ";
    v.Print();
    std::cout << std::endl;
    std::cout << "  Reconstructed: ";
    reconstructed.Print();
    std::cout << std::endl;
    std::cout << "  = " << v.x << "*X + " << v.y << "*Y + " << v.z << "*Z" << std::endl;
    std::cout << std::endl;

    // 4. Axis-aligned movements
    std::cout << "4. AXIS-ALIGNED MOVEMENTS:" << std::endl;
    Vector3 position(0, 0, 0);

    std::cout << "  Start: ";
    position.Print();
    std::cout << std::endl;

    position = position + xAxis * 5.0f;
    std::cout << "  Move right 5:   ";
    position.Print();
    std::cout << std::endl;

    position = position + yAxis * 3.0f;
    std::cout << "  Move up 3:      ";
    position.Print();
    std::cout << std::endl;

    position = position + zAxis * 2.0f;
    std::cout << "  Move forward 2: ";
    position.Print();
    std::cout << std::endl;
    std::cout << std::endl;

    // 5. Orthogonality check
    std::cout << "5. AXES ARE ORTHOGONAL (PERPENDICULAR):" << std::endl;
    std::cout << "  X·Y = " << xAxis.DotProduct(yAxis) << " (should be 0)" << std::endl;
    std::cout << "  Y·Z = " << yAxis.DotProduct(zAxis) << " (should be 0)" << std::endl;
    std::cout << "  Z·X = " << zAxis.DotProduct(xAxis) << " (should be 0)" << std::endl;
    std::cout << "  Dot product of perpendicular vectors = 0" << std::endl;

    return 0;
}
