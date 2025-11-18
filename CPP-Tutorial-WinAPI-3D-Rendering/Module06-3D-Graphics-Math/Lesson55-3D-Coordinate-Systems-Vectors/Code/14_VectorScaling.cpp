/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 14: Vector Scaling and Interpolation
 *
 * Compile: g++ -o 14_VectorScaling 14_VectorScaling.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? Vector3(x/len, y/len, z/len) : Vector3(0,0,0);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(4)
                  << "(" << std::setw(7) << x << ", "
                  << std::setw(7) << y << ", "
                  << std::setw(7) << z << ")";
    }
};

// Linear interpolation: lerp(a, b, t) = a + t * (b - a)
Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
    return a + (b - a) * t;
}

int main() {
    std::cout << "=== Vector Scaling and Interpolation ===" << std::endl;
    std::cout << std::endl;

    // 1. Scaling basics
    std::cout << "1. BASIC SCALING:" << std::endl;
    Vector3 v(1, 2, 3);

    std::cout << "  Original: ";
    v.Print();
    std::cout << " (len=" << v.Length() << ")" << std::endl;

    std::cout << "  * 2:      ";
    (v * 2).Print();
    std::cout << " (len=" << (v * 2).Length() << ")" << std::endl;

    std::cout << "  * 0.5:    ";
    (v * 0.5f).Print();
    std::cout << " (len=" << (v * 0.5f).Length() << ")" << std::endl;

    std::cout << "  * 0:      ";
    (v * 0).Print();
    std::cout << " (len=" << (v * 0).Length() << ")" << std::endl;
    std::cout << std::endl;

    // 2. Scaling to specific length
    std::cout << "2. SCALING TO SPECIFIC LENGTH:" << std::endl;
    Vector3 direction(3, 4, 0);
    std::cout << "  Direction: ";
    direction.Print();
    std::cout << " (len=" << direction.Length() << ")" << std::endl;

    for (float targetLen : {1.0f, 5.0f, 10.0f, 100.0f}) {
        Vector3 scaled = direction.Normalized() * targetLen;
        std::cout << "  Length " << std::setw(3) << targetLen << ": ";
        scaled.Print();
        std::cout << " (len=" << scaled.Length() << ")" << std::endl;
    }
    std::cout << std::endl;

    // 3. Linear interpolation (LERP)
    std::cout << "3. LINEAR INTERPOLATION (LERP):" << std::endl;
    Vector3 start(0, 0, 0);
    Vector3 end(10, 10, 10);

    std::cout << "  Start: ";
    start.Print();
    std::cout << std::endl;
    std::cout << "  End:   ";
    end.Print();
    std::cout << std::endl << std::endl;

    for (float t = 0.0f; t <= 1.0f; t += 0.2f) {
        Vector3 result = Lerp(start, end, t);
        std::cout << "  t=" << std::fixed << std::setprecision(1) << t << ": ";
        result.Print();
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 4. Smooth camera movement
    std::cout << "4. SMOOTH CAMERA MOVEMENT (LERP):" << std::endl;
    Vector3 currentPos(0, 5, -10);
    Vector3 targetPos(10, 5, 0);
    float smoothSpeed = 0.3f;  // 30% per frame

    std::cout << "  Current: ";
    currentPos.Print();
    std::cout << std::endl;
    std::cout << "  Target:  ";
    targetPos.Print();
    std::cout << std::endl << std::endl;

    for (int i = 0; i < 10; i++) {
        currentPos = Lerp(currentPos, targetPos, smoothSpeed);
        std::cout << "  Frame " << std::setw(2) << (i+1) << ": ";
        currentPos.Print();
        std::cout << std::endl;
    }

    return 0;
}
