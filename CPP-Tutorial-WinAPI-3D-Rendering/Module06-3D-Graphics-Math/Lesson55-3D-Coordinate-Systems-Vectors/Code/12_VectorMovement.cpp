/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 12: Simulating Object Movement with Vectors
 *
 * Compile: g++ -o 12_VectorMovement 12_VectorMovement.cpp -std=c++11 -Wall
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

    Vector3 operator+=(const Vector3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "(" << std::setw(6) << x << ", "
                  << std::setw(6) << y << ", "
                  << std::setw(6) << z << ")";
    }
};

int main() {
    std::cout << "=== Vector Movement Simulations ===" << std::endl;
    std::cout << std::endl;

    // 1. Constant velocity movement
    std::cout << "1. CONSTANT VELOCITY MOVEMENT:" << std::endl;
    Vector3 position(0, 0, 0);
    Vector3 velocity(2, 1, 0);
    float deltaTime = 0.5f;

    std::cout << "  Initial position: ";
    position.Print();
    std::cout << std::endl;
    std::cout << "  Velocity: ";
    velocity.Print();
    std::cout << " (units/second)" << std::endl;
    std::cout << "  Delta time: " << deltaTime << " seconds per frame" << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 5; i++) {
        position += velocity * deltaTime;
        std::cout << "  Frame " << (i+1) << ": ";
        position.Print();
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 2. Accelerated movement (gravity)
    std::cout << "2. PROJECTILE MOTION (with gravity):" << std::endl;
    Vector3 projectilePos(0, 10, 0);
    Vector3 projectileVel(5, 10, 0);
    Vector3 gravity(0, -9.8f, 0);
    deltaTime = 0.1f;

    std::cout << "  Initial position: ";
    projectilePos.Print();
    std::cout << std::endl;
    std::cout << "  Initial velocity: ";
    projectileVel.Print();
    std::cout << std::endl;
    std::cout << "  Gravity: ";
    gravity.Print();
    std::cout << " (m/s²)" << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        projectileVel += gravity * deltaTime;
        projectilePos += projectileVel * deltaTime;

        std::cout << "  t=" << std::setw(3) << ((i+1) * deltaTime) << "s: ";
        projectilePos.Print();
        std::cout << " (vel: ";
        projectileVel.Print();
        std::cout << ")" << std::endl;

        if (projectilePos.y < 0) {
            std::cout << "  Hit ground!" << std::endl;
            break;
        }
    }
    std::cout << std::endl;

    // 3. Circular motion
    std::cout << "3. CIRCULAR MOTION:" << std::endl;
    float radius = 5.0f;
    float angularSpeed = 1.0f;  // radians per second

    std::cout << "  Radius: " << radius << " units" << std::endl;
    std::cout << "  Angular speed: " << angularSpeed << " rad/s" << std::endl;
    std::cout << std::endl;

    for (int i = 0; i <= 8; i++) {
        float angle = angularSpeed * i * 0.5f;
        Vector3 circlePos(
            radius * std::cos(angle),
            0,
            radius * std::sin(angle)
        );

        std::cout << "  Angle " << std::setw(4) << (angle * 180 / 3.14159f) << "°: ";
        circlePos.Print();
        std::cout << " (distance from origin: " << circlePos.Length() << ")" << std::endl;
    }

    return 0;
}
