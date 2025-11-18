/* Example 11: Backface Culling */
#include <iostream>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Backface Culling ===" << std::endl << std::endl;
    Vector3 v0(0, 0, 0), v1(1, 0, 0), v2(0, 1, 0);
    Vector3 cameraPos(0, 0, -1);
    
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 normal = edge1.Cross(edge2);
    Vector3 toCamera = cameraPos - v0;
    
    bool isFrontFacing = normal.Dot(toCamera) < 0;
    
    std::cout << "Triangle: "; v0.Print(); std::cout << ", ";
    v1.Print(); std::cout << ", "; v2.Print(); std::cout << std::endl;
    std::cout << "Camera: "; cameraPos.Print(); std::cout << std::endl;
    std::cout << "Front facing: " << (isFrontFacing ? "YES" : "NO") << std::endl;
    return 0;
}
