/* Example 09: Distance Formulas */
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

float PointToPlaneDistance(const Vector3& point, const Vector3& planePoint, const Vector3& normal) {
    return std::abs((point - planePoint).Dot(normal));
}

int main() {
    std::cout << "=== Distance Formulas ===" << std::endl << std::endl;
    
    // Point to plane
    Vector3 point(0, 5, 0);
    Vector3 planePoint(0, 0, 0);
    Vector3 planeNormal(0, 1, 0);
    float dist = PointToPlaneDistance(point, planePoint, planeNormal);
    
    std::cout << "Point: "; point.Print(); std::cout << std::endl;
    std::cout << "Plane: point="; planePoint.Print();
    std::cout << ", normal="; planeNormal.Print(); std::cout << std::endl;
    std::cout << "Distance = " << dist << std::endl;
    return 0;
}
