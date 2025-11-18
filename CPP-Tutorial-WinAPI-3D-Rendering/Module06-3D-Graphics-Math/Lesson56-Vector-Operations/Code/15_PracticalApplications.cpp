/* Example 15: Practical Applications */
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator+(const Vector3& v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(Dot(*this)); }
    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? *this * (1.0f/len) : Vector3(0,0,0);
    }
    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Practical Applications ===" << std::endl << std::endl;
    
    // 1. Camera look-at
    std::cout << "1. CAMERA LOOK-AT:" << std::endl;
    Vector3 cameraPos(0, 5, -10);
    Vector3 targetPos(0, 0, 0);
    Vector3 worldUp(0, 1, 0);
    
    Vector3 forward = (targetPos - cameraPos).Normalized();
    Vector3 right = forward.Cross(worldUp).Normalized();
    Vector3 up = right.Cross(forward);
    
    std::cout << "  Forward: "; forward.Print(); std::cout << std::endl;
    std::cout << "  Right:   "; right.Print(); std::cout << std::endl;
    std::cout << "  Up:      "; up.Print(); std::cout << std::endl << std::endl;
    
    // 2. Field of view check
    std::cout << "2. FIELD OF VIEW CHECK:" << std::endl;
    Vector3 enemyPos(3, 0, 2);
    Vector3 toEnemy = (enemyPos - cameraPos).Normalized();
    float fovAngle = 90.0f * 3.14159f / 180.0f;  // 90 degrees in radians
    bool inFOV = forward.Dot(toEnemy) > std::cos(fovAngle / 2.0f);
    
    std::cout << "  Enemy: "; enemyPos.Print();
    std::cout << (inFOV ? " [IN FOV]" : " [OUT OF FOV]") << std::endl;
    
    return 0;
}
