#include <iostream>
#include <cmath>
class Quaternion {
public:
    float w, x, y, z;
    Quaternion(float w=1, float x=0, float y=0, float z=0) : w(w),x(x),y(y),z(z) {}
    static Quaternion FromAxisAngle(float ax, float ay, float az, float angle) {
        float s = sin(angle/2);
        return Quaternion(cos(angle/2), ax*s, ay*s, az*s);
    }
    void Print() { std::cout << "(" << w <<","<< x <<","<< y <<","<< z << ")" << std::endl; }
};
int main() {
    Quaternion q = Quaternion::FromAxisAngle(0,0,1, 3.14159f/2);
    std::cout << "Quaternion Example 05: "; q.Print();
    return 0;
}
