#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    void Print() { std::cout << "(" << x << "," << y << "," << z << ")"; }
};
class Camera {
    Vector3 position, target;
public:
    Camera(Vector3 pos, Vector3 tgt) : position(pos), target(tgt) {}
    void Print() {
        std::cout << "Camera pos="; position.Print();
        std::cout << " target="; target.Print(); std::cout << std::endl;
    }
};
int main() {
    Camera cam(Vector3(0,5,-10), Vector3(0,0,0));
    std::cout << "Camera Example 12" << std::endl;
    cam.Print();
    return 0;
}
