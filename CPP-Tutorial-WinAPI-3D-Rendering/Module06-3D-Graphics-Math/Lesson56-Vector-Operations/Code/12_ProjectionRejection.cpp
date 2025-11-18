/* Example 12: Projection and Rejection */
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(Dot(*this)); }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Projection and Rejection ===" << std::endl << std::endl;
    
    Vector3 movement(3, 4, 0);
    Vector3 wallNormal(1, 0, 0);  // Wall blocks X movement
    
    // Project onto normal (perpendicular to wall)
    Vector3 proj = wallNormal * movement.Dot(wallNormal);
    
    // Reject from normal (parallel to wall)
    Vector3 slide = movement - proj;
    
    std::cout << "Movement:   "; movement.Print(); std::cout << std::endl;
    std::cout << "Wall normal:"; wallNormal.Print(); std::cout << std::endl;
    std::cout << "Blocked:    "; proj.Print(); std::cout << std::endl;
    std::cout << "Slide:      "; slide.Print();
    std::cout << " (movement along wall)" << std::endl;
    return 0;
}
