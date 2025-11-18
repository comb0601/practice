/* Example 10: Lighting Calculations */
#include <iostream>
#include <cmath>
#include <algorithm>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? *this * (1.0f/len) : Vector3(0,0,0);
    }
    Vector3 Reflect(const Vector3& n) const {
        return *this - n * (2.0f * Dot(n));
    }
    void Print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

int main() {
    std::cout << "=== Lighting Calculations ===" << std::endl << std::endl;
    
    Vector3 normal(0, 1, 0);
    Vector3 lightDir = Vector3(1, 1, 0).Normalized();
    Vector3 viewDir = Vector3(0, 1, -1).Normalized();
    
    // Diffuse (Lambertian)
    float diffuse = std::max(0.0f, normal.Dot(lightDir));
    
    // Specular (Phong)
    Vector3 reflected = lightDir.Reflect(normal);
    float specular = std::pow(std::max(0.0f, viewDir.Dot(reflected)), 32.0f);
    
    std::cout << "Normal: "; normal.Print(); std::cout << std::endl;
    std::cout << "Light:  "; lightDir.Print(); std::cout << std::endl;
    std::cout << "View:   "; viewDir.Print(); std::cout << std::endl;
    std::cout << "Diffuse intensity:  " << diffuse << std::endl;
    std::cout << "Specular intensity: " << specular << std::endl;
    return 0;
}
