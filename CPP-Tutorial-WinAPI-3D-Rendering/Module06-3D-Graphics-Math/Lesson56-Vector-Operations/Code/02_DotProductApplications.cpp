/*
 * Lesson 56: Vector Operations  
 * Example 02: Dot Product Applications
 */

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float Length() const {
        return std::sqrt(Dot(*this));
    }

    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? *this * (1.0f / len) : Vector3(0,0,0);
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Dot Product Applications ===" << std::endl << std::endl;

    // 1. Check if facing target
    std::cout << "1. FACING CHECK:" << std::endl;
    Vector3 forward(0, 0, 1);
    Vector3 toTarget1(1, 0, 1);
    Vector3 toTarget2(-1, 0, -1);

    std::cout << "  Forward: "; forward.Print(); std::cout << std::endl;
    std::cout << "  ToTarget1: "; toTarget1.Print();
    std::cout << " -> Dot: " << forward.Dot(toTarget1.Normalized());
    std::cout << (forward.Dot(toTarget1) > 0 ? " [FACING]" : " [NOT FACING]") << std::endl;

    std::cout << "  ToTarget2: "; toTarget2.Print();
    std::cout << " -> Dot: " << forward.Dot(toTarget2.Normalized());
    std::cout << (forward.Dot(toTarget2) > 0 ? " [FACING]" : " [NOT FACING]") << std::endl << std::endl;

    // 2. Simple lighting
    std::cout << "2. LIGHTING INTENSITY:" << std::endl;
    Vector3 normal(0, 1, 0);  // Surface pointing up
    Vector3 lightDir(1, 1, 0); // Light from above-right
    lightDir = lightDir.Normalized();

    float intensity = std::max(0.0f, normal.Dot(lightDir));
    std::cout << "  Surface normal: "; normal.Print(); std::cout << std::endl;
    std::cout << "  Light direction: "; lightDir.Print(); std::cout << std::endl;
    std::cout << "  Lighting intensity: " << intensity << std::endl << std::endl;

    // 3. Projection length
    std::cout << "3. PROJECTION LENGTH:" << std::endl;
    Vector3 velocity(3, 4, 0);
    Vector3 direction(1, 0, 0);

    float speedInDirection = velocity.Dot(direction);
    std::cout << "  Velocity: "; velocity.Print(); std::cout << std::endl;
    std::cout << "  Direction: "; direction.Print(); std::cout << std::endl;
    std::cout << "  Speed in that direction: " << speedInDirection << std::endl;

    return 0;
}
