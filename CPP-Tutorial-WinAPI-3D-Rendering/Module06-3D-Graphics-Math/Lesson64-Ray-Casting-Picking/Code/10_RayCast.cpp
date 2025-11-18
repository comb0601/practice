#include <iostream>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    void Print() { std::cout << "(" << x << "," << y << "," << z << ")"; }
};
class Ray {
public:
    Vector3 origin, direction;
    Ray(Vector3 o, Vector3 d) : origin(o), direction(d) {}
    Vector3 At(float t) { 
        return Vector3(origin.x + direction.x*t,
                      origin.y + direction.y*t,
                      origin.z + direction.z*t);
    }
};
int main() {
    Ray ray(Vector3(0,0,0), Vector3(1,0,0));
    std::cout << "RayCast Example 10: ";
    ray.At(5).Print();
    std::cout << std::endl;
    return 0;
}
