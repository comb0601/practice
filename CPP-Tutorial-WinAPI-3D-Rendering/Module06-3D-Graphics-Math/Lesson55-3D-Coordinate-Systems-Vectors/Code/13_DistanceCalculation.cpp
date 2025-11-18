/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 13: Distance Calculations and Applications
 *
 * Compile: g++ -o 13_DistanceCalculation 13_DistanceCalculation.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    float DistanceTo(const Vector3& v) const {
        return (*this - v).Length();
    }

    float DistanceSquaredTo(const Vector3& v) const {
        return (*this - v).LengthSquared();
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "(" << std::setw(5) << x << ", "
                  << std::setw(5) << y << ", "
                  << std::setw(5) << z << ")";
    }
};

int main() {
    std::cout << "=== Distance Calculation Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. Basic distance
    std::cout << "1. BASIC DISTANCE CALCULATIONS:" << std::endl;
    Vector3 p1(0, 0, 0);
    Vector3 p2(3, 4, 0);

    std::cout << "  Point 1: ";
    p1.Print();
    std::cout << std::endl;
    std::cout << "  Point 2: ";
    p2.Print();
    std::cout << std::endl;
    std::cout << "  Distance: " << p1.DistanceTo(p2) << std::endl;
    std::cout << "  (3-4-5 right triangle: sqrt(3² + 4²) = 5)" << std::endl;
    std::cout << std::endl;

    // 2. 3D distance
    std::cout << "2. 3D DISTANCE:" << std::endl;
    Vector3 p3(1, 2, 3);
    Vector3 p4(4, 6, 8);

    std::cout << "  Point 1: ";
    p3.Print();
    std::cout << std::endl;
    std::cout << "  Point 2: ";
    p4.Print();
    std::cout << std::endl;

    Vector3 diff = p4 - p3;
    std::cout << "  Difference: ";
    diff.Print();
    std::cout << std::endl;
    std::cout << "  Distance: sqrt(" << diff.x << "² + " << diff.y << "² + " << diff.z << "²)" << std::endl;
    std::cout << "          = sqrt(" << diff.LengthSquared() << ") = " << diff.Length() << std::endl;
    std::cout << std::endl;

    // 3. Finding closest point
    std::cout << "3. FINDING CLOSEST POINT:" << std::endl;
    Vector3 player(5, 0, 5);
    Vector3 enemies[] = {
        Vector3(10, 0, 5),
        Vector3(3, 0, 8),
        Vector3(5, 0, 12),
        Vector3(8, 0, 2)
    };

    std::cout << "  Player: ";
    player.Print();
    std::cout << std::endl << std::endl;

    int closestIndex = 0;
    float closestDistSq = player.DistanceSquaredTo(enemies[0]);

    for (int i = 0; i < 4; i++) {
        float distSq = player.DistanceSquaredTo(enemies[i]);
        std::cout << "  Enemy " << i << ": ";
        enemies[i].Print();
        std::cout << " -> Distance²: " << distSq << std::endl;

        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closestIndex = i;
        }
    }

    std::cout << "  Closest: Enemy " << closestIndex
              << " (distance: " << std::sqrt(closestDistSq) << ")" << std::endl;
    std::cout << std::endl;

    // 4. Range checking
    std::cout << "4. RANGE CHECKING (Sphere):" << std::endl;
    Vector3 center(0, 0, 0);
    float radius = 10.0f;

    Vector3 points[] = {
        Vector3(5, 0, 0),
        Vector3(8, 6, 0),
        Vector3(15, 0, 0),
        Vector3(0, 9, 4)
    };

    std::cout << "  Sphere center: ";
    center.Print();
    std::cout << ", radius: " << radius << std::endl << std::endl;

    for (int i = 0; i < 4; i++) {
        float dist = center.DistanceTo(points[i]);
        std::cout << "  Point ";
        points[i].Print();
        std::cout << " -> Distance: " << std::setw(6) << dist;
        std::cout << (dist <= radius ? " [INSIDE]" : " [OUTSIDE]") << std::endl;
    }

    return 0;
}
