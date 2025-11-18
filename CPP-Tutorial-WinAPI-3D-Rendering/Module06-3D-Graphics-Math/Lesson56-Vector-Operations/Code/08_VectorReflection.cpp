/* Example 08: Vector Reflection */
#include <iostream>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Vector Reflection ===" << std::endl << std::endl;
    Vector3 incoming(1, -1, 0);
    Vector3 normal(0, 1, 0);
    Vector3 reflected = incoming.Reflect(normal);
    
    std::cout << "Incoming:  "; incoming.Print(); std::cout << std::endl;
    std::cout << "Normal:    "; normal.Print(); std::cout << std::endl;
    std::cout << "Reflected: "; reflected.Print(); std::cout << std::endl;
    return 0;
}
