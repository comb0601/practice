/* Example 06: Angle Calculations */
#include <iostream>
#include <cmath>
#include <algorithm>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

float AngleBetween(const Vector3& v, const Vector3& w) {
    float dot = v.Dot(w);
    float lengths = v.Length() * w.Length();
    if (lengths == 0) return 0;
    float cosAngle = std::max(-1.0f, std::min(1.0f, dot / lengths));
    return std::acos(cosAngle);
}

int main() {
    std::cout << "=== Angle Calculations ===" << std::endl << std::endl;
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    float angleRad = AngleBetween(v1, v2);
    float angleDeg = angleRad * 180.0f / 3.14159f;
    std::cout << "v1 = "; v1.Print(); std::cout << std::endl;
    std::cout << "v2 = "; v2.Print(); std::cout << std::endl;
    std::cout << "Angle = " << angleDeg << " degrees" << std::endl;
    return 0;
}
