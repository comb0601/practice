#include <iostream>
#include <cmath>
using namespace std;
class Vector {
    double x, y;
public:
    Vector(double x = 0, double y = 0) : x(x), y(y) {}
    friend double dot(const Vector&, const Vector&);
    friend double magnitude(const Vector&);
    friend Vector normalize(const Vector&);
};
double dot(const Vector& v1, const Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
double magnitude(const Vector& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}
Vector normalize(const Vector& v) {
    double mag = magnitude(v);
    return Vector(v.x / mag, v.y / mag);
}
int main() {
    Vector v1(3, 4), v2(1, 0);
    cout << "Dot product: " << dot(v1, v2) << endl;
    cout << "Magnitude of v1: " << magnitude(v1) << endl;
    return 0;
}
