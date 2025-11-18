/* Example 07: Signed Angles */
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis) {
    float angle = std::acos(from.Dot(to) / (from.Length() * to.Length()));
    if (from.Cross(to).Dot(axis) < 0) angle = -angle;
    return angle;
}

int main() {
    std::cout << "=== Signed Angles ===" << std::endl << std::endl;
    Vector3 from(1, 0, 0);
    Vector3 to1(0, 1, 0);
    Vector3 to2(0, -1, 0);
    Vector3 zAxis(0, 0, 1);
    
    float angle1 = SignedAngle(from, to1, zAxis) * 180.0f / 3.14159f;
    float angle2 = SignedAngle(from, to2, zAxis) * 180.0f / 3.14159f;
    
    std::cout << "From "; from.Print(); std::cout << " to "; to1.Print();
    std::cout << " = " << angle1 << "°" << std::endl;
    std::cout << "From "; from.Print(); std::cout << " to "; to2.Print();
    std::cout << " = " << angle2 << "°" << std::endl;
    return 0;
}
