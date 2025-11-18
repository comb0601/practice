/*
 * Lesson 56: Vector Operations
 * Example 04: Cross Product Applications
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

    Vector3 Cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
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
    std::cout << "=== Cross Product Applications ===" << std::endl << std::endl;

    // 1. Surface normal from triangle
    std::cout << "1. TRIANGLE NORMAL:" << std::endl;
    Vector3 a(0, 0, 0);
    Vector3 b(1, 0, 0);
    Vector3 c(0, 1, 0);

    Vector3 edge1 = b - a;
    Vector3 edge2 = c - a;
    Vector3 normal = edge1.Cross(edge2).Normalized();

    std::cout << "  Triangle vertices:" << std::endl;
    std::cout << "    a = "; a.Print(); std::cout << std::endl;
    std::cout << "    b = "; b.Print(); std::cout << std::endl;
    std::cout << "    c = "; c.Print(); std::cout << std::endl;
    std::cout << "  Normal = "; normal.Print(); std::cout << std::endl << std::endl;

    // 2. Building coordinate system
    std::cout << "2. BUILD COORDINATE SYSTEM:" << std::endl;
    Vector3 forward(1, 1, 0);
    forward = forward.Normalized();
    Vector3 worldUp(0, 1, 0);

    Vector3 right = forward.Cross(worldUp).Normalized();
    Vector3 up = right.Cross(forward);

    std::cout << "  Forward: "; forward.Print(); std::cout << std::endl;
    std::cout << "  Right:   "; right.Print(); std::cout << std::endl;
    std::cout << "  Up:      "; up.Print(); std::cout << std::endl << std::endl;

    // 3. Area calculation
    std::cout << "3. TRIANGLE AREA:" << std::endl;
    Vector3 p1(0, 0, 0);
    Vector3 p2(4, 0, 0);
    Vector3 p3(0, 3, 0);

    Vector3 e1 = p2 - p1;
    Vector3 e2 = p3 - p1;
    float area = e1.Cross(e2).Length() / 2.0f;

    std::cout << "  Triangle: "; p1.Print(); std::cout << ", ";
    p2.Print(); std::cout << ", "; p3.Print(); std::cout << std::endl;
    std::cout << "  Area = " << area << std::endl;

    return 0;
}
