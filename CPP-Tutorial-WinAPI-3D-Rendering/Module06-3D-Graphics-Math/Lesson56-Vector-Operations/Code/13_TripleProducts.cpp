/* Example 13: Triple Products */
#include <iostream>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

float ScalarTripleProduct(const Vector3& a, const Vector3& b, const Vector3& c) {
    return a.Dot(b.Cross(c));
}

int main() {
    std::cout << "=== Triple Products ===" << std::endl << std::endl;
    
    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    Vector3 c(0, 0, 1);
    
    float scalar = ScalarTripleProduct(a, b, c);
    
    std::cout << "a = "; a.Print(); std::cout << std::endl;
    std::cout << "b = "; b.Print(); std::cout << std::endl;
    std::cout << "c = "; c.Print(); std::cout << std::endl;
    std::cout << "a · (b × c) = " << scalar << std::endl;
    std::cout << "(Volume of parallelepiped)" << std::endl;
    return 0;
}
