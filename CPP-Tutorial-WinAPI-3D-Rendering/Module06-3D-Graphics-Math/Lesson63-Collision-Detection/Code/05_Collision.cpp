#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float Length() { return sqrt(x*x+y*y+z*z); }
    Vector3 operator-(Vector3 v) { return Vector3(x-v.x, y-v.y, z-v.z); }
};
bool SphereSphere(Vector3 c1, float r1, Vector3 c2, float r2) {
    return (c2-c1).Length() < (r1+r2);
}
int main() {
    bool hit = SphereSphere(Vector3(0,0,0), 5, Vector3(8,0,0), 2);
    std::cout << "Collision Example 05: " << (hit?"HIT":"MISS") << std::endl;
    return 0;
}
