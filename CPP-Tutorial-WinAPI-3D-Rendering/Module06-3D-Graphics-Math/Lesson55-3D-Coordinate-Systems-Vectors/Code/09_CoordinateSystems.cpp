/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 09: Right-Handed vs Left-Handed Coordinate Systems
 *
 * Compile: g++ -o 09_CoordinateSystems 09_CoordinateSystems.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 CrossProduct(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    void Print() const {
        std::cout << "(" << std::setw(2) << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Coordinate System Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // Right-handed system
    std::cout << "RIGHT-HANDED COORDINATE SYSTEM (OpenGL, Mathematics):" << std::endl;
    std::cout << "  X-axis: Right" << std::endl;
    std::cout << "  Y-axis: Up" << std::endl;
    std::cout << "  Z-axis: Toward viewer (out of screen)" << std::endl;
    std::cout << std::endl;
    std::cout << "       Y (Up)" << std::endl;
    std::cout << "       |" << std::endl;
    std::cout << "       |" << std::endl;
    std::cout << "       +------- X (Right)" << std::endl;
    std::cout << "      /" << std::endl;
    std::cout << "     /" << std::endl;
    std::cout << "    Z (Toward you)" << std::endl;
    std::cout << std::endl;

    Vector3 xAxis(1, 0, 0);
    Vector3 yAxis(0, 1, 0);
    Vector3 zAxis(0, 0, 1);

    std::cout << "  X × Y = Z verification:" << std::endl;
    Vector3 crossXY = xAxis.CrossProduct(yAxis);
    std::cout << "  ";
    xAxis.Print();
    std::cout << " × ";
    yAxis.Print();
    std::cout << " = ";
    crossXY.Print();
    std::cout << " ✓" << std::endl;
    std::cout << std::endl;

    // Left-handed system
    std::cout << "LEFT-HANDED COORDINATE SYSTEM (DirectX, Unity default):" << std::endl;
    std::cout << "  X-axis: Right" << std::endl;
    std::cout << "  Y-axis: Up" << std::endl;
    std::cout << "  Z-axis: Away from viewer (into screen)" << std::endl;
    std::cout << std::endl;
    std::cout << "       Y (Up)" << std::endl;
    std::cout << "       |" << std::endl;
    std::cout << "       |" << std::endl;
    std::cout << "       +------- X (Right)" << std::endl;
    std::cout << "        \\" << std::endl;
    std::cout << "         \\" << std::endl;
    std::cout << "          Z (Away from you)" << std::endl;
    std::cout << std::endl;

    // Conversion
    std::cout << "CONVERTING BETWEEN SYSTEMS:" << std::endl;
    Vector3 rightHanded(3, 4, 5);
    Vector3 leftHanded(rightHanded.x, rightHanded.y, -rightHanded.z);

    std::cout << "  Right-handed: ";
    rightHanded.Print();
    std::cout << std::endl;
    std::cout << "  Left-handed:  ";
    leftHanded.Print();
    std::cout << " (negate Z)" << std::endl;
    std::cout << std::endl;

    std::cout << "APPLICATIONS:" << std::endl;
    std::cout << "  - OpenGL: Right-handed" << std::endl;
    std::cout << "  - DirectX: Left-handed" << std::endl;
    std::cout << "  - Unity: Left-handed (default)" << std::endl;
    std::cout << "  - Unreal: Left-handed (Z-up variant)" << std::endl;
    std::cout << "  - Mathematics/Physics: Usually right-handed" << std::endl;

    return 0;
}
