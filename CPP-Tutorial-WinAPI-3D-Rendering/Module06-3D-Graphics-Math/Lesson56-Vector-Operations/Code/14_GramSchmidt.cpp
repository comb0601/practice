/* Example 14: Gram-Schmidt Orthogonalization */
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
    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? *this * (1.0f/len) : Vector3(0,0,0);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Gram-Schmidt Orthogonalization ===" << std::endl << std::endl;
    
    // Start with non-orthogonal vectors
    Vector3 v1(1, 1, 0);
    Vector3 v2(1, 2, 0);
    
    // Orthogonalize
    Vector3 u1 = v1.Normalized();
    Vector3 u2 = (v2 - u1 * v2.Dot(u1)).Normalized();
    
    std::cout << "Original vectors:" << std::endl;
    std::cout << "  v1 = "; v1.Print(); std::cout << std::endl;
    std::cout << "  v2 = "; v2.Print(); std::cout << std::endl;
    std::cout << "  v1 · v2 = " << v1.Dot(v2) << " (not zero)" << std::endl << std::endl;
    
    std::cout << "Orthonormal vectors:" << std::endl;
    std::cout << "  u1 = "; u1.Print(); std::cout << std::endl;
    std::cout << "  u2 = "; u2.Print(); std::cout << std::endl;
    std::cout << "  u1 · u2 = " << u1.Dot(u2) << " (should be ~0)" << std::endl;
    return 0;
}
