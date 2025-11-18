/* Example 05: Vector Projection */
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float LengthSquared() const { return Dot(*this); }
    Vector3 ProjectOnto(const Vector3& v) const {
        float dotProduct = Dot(v);
        float lenSq = v.LengthSquared();
        return (lenSq > 0) ? v * (dotProduct / lenSq) : Vector3(0,0,0);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Vector Projection ===" << std::endl << std::endl;
    Vector3 v(3, 4, 0);
    Vector3 w(1, 0, 0);
    Vector3 proj = v.ProjectOnto(w);
    Vector3 rej = v - proj;
    std::cout << "v = "; v.Print(); std::cout << std::endl;
    std::cout << "w = "; w.Print(); std::cout << std::endl;
    std::cout << "Projection of v onto w = "; proj.Print(); std::cout << std::endl;
    std::cout << "Rejection = "; rej.Print(); std::cout << std::endl;
    return 0;
}
