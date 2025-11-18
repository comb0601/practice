/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 05: Vector Magnitude (Length) Calculations
 *
 * This program demonstrates:
 * - Calculating vector magnitude/length
 * - LengthSquared() optimization
 * - Distance between points
 * - Practical applications of magnitude
 *
 * Compile: g++ -o 05_VectorMagnitude 05_VectorMagnitude.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Calculate magnitude using the 3D Pythagorean theorem
    // Formula: ||v|| = sqrt(x² + y² + z²)
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Calculate squared magnitude (faster, no square root)
    // Useful for comparisons where actual length doesn't matter
    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    // Calculate distance to another point
    float DistanceTo(const Vector3& other) const {
        float dx = other.x - x;
        float dy = other.y - y;
        float dz = other.z - z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // Calculate squared distance (faster)
    float DistanceSquaredTo(const Vector3& other) const {
        float dx = other.x - x;
        float dy = other.y - y;
        float dz = other.z - z;
        return dx * dx + dy * dy + dz * dz;
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    void Print() const {
        std::cout << "(" << std::setw(6) << std::setprecision(2) << std::fixed
                  << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Vector Magnitude Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. BASIC MAGNITUDE CALCULATIONS
    std::cout << "1. BASIC MAGNITUDE CALCULATIONS:" << std::endl;

    Vector3 v1(3, 4, 0);
    std::cout << "  v1 = ";
    v1.Print();
    std::cout << std::endl;
    std::cout << "  |v1| = sqrt(3² + 4² + 0²) = sqrt(9 + 16) = sqrt(25) = " << v1.Length() << std::endl;
    std::cout << std::endl;

    Vector3 v2(1, 2, 2);
    std::cout << "  v2 = ";
    v2.Print();
    std::cout << std::endl;
    std::cout << "  |v2| = sqrt(1² + 2² + 2²) = sqrt(1 + 4 + 4) = sqrt(9) = " << v2.Length() << std::endl;
    std::cout << std::endl;

    Vector3 v3(1, 1, 1);
    std::cout << "  v3 = ";
    v3.Print();
    std::cout << std::endl;
    std::cout << "  |v3| = sqrt(1² + 1² + 1²) = sqrt(3) = " << v3.Length() << std::endl;
    std::cout << std::endl;

    // 2. AXIS-ALIGNED VECTORS
    std::cout << "2. AXIS-ALIGNED VECTORS:" << std::endl;

    Vector3 xAxis(1, 0, 0);
    Vector3 yAxis(0, 1, 0);
    Vector3 zAxis(0, 0, 1);

    std::cout << "  X-axis (1,0,0): length = " << xAxis.Length() << std::endl;
    std::cout << "  Y-axis (0,1,0): length = " << yAxis.Length() << std::endl;
    std::cout << "  Z-axis (0,0,1): length = " << zAxis.Length() << std::endl;
    std::cout << "  These are UNIT VECTORS (length = 1)" << std::endl;
    std::cout << std::endl;

    // 3. ZERO VECTOR
    std::cout << "3. ZERO VECTOR:" << std::endl;
    Vector3 zero(0, 0, 0);
    std::cout << "  Zero vector (0,0,0): length = " << zero.Length() << std::endl;
    std::cout << "  This is the only vector with length 0" << std::endl;
    std::cout << std::endl;

    // 4. LENGTH vs LENGTH SQUARED
    std::cout << "4. LENGTH vs LENGTH SQUARED:" << std::endl;
    Vector3 v4(5, 12, 0);

    std::cout << "  v4 = ";
    v4.Print();
    std::cout << std::endl;
    std::cout << "  Length():        " << v4.Length() << std::endl;
    std::cout << "  LengthSquared(): " << v4.LengthSquared() << std::endl;
    std::cout << "  Verify: 13² = " << (13.0f * 13.0f) << std::endl;
    std::cout << std::endl;

    std::cout << "  WHY USE LENGTHSQUARED?" << std::endl;
    std::cout << "  - Faster (no square root computation)" << std::endl;
    std::cout << "  - Useful for comparisons (if |v1| < |v2|, then |v1|² < |v2|²)" << std::endl;
    std::cout << "  - Avoids floating-point precision issues" << std::endl;
    std::cout << std::endl;

    // 5. DISTANCE BETWEEN POINTS
    std::cout << "5. DISTANCE BETWEEN POINTS:" << std::endl;

    Vector3 p1(0, 0, 0);
    Vector3 p2(3, 4, 0);

    std::cout << "  Point 1: ";
    p1.Print();
    std::cout << std::endl;
    std::cout << "  Point 2: ";
    p2.Print();
    std::cout << std::endl;

    float dist = p1.DistanceTo(p2);
    std::cout << "  Distance: " << dist << std::endl;

    // Alternative: distance as length of difference
    Vector3 diff = p2 - p1;
    std::cout << "  Verification: |p2 - p1| = " << diff.Length() << std::endl;
    std::cout << std::endl;

    // 6. 3D DISTANCE EXAMPLE
    std::cout << "6. 3D DISTANCE EXAMPLE:" << std::endl;

    Vector3 corner1(0, 0, 0);
    Vector3 corner2(1, 1, 1);

    std::cout << "  Corner of unit cube: ";
    corner1.Print();
    std::cout << std::endl;
    std::cout << "  Opposite corner:     ";
    corner2.Print();
    std::cout << std::endl;
    std::cout << "  Diagonal distance: " << corner1.DistanceTo(corner2) << std::endl;
    std::cout << "  (This is sqrt(3) ≈ 1.732)" << std::endl;
    std::cout << std::endl;

    // 7. COMPARING DISTANCES EFFICIENTLY
    std::cout << "7. COMPARING DISTANCES (using squared length):" << std::endl;

    Vector3 target(10, 0, 0);
    Vector3 enemy1(5, 0, 0);
    Vector3 enemy2(8, 6, 0);
    Vector3 enemy3(15, 0, 0);

    std::cout << "  Target: ";
    target.Print();
    std::cout << std::endl;

    float dist1Sq = target.DistanceSquaredTo(enemy1);
    float dist2Sq = target.DistanceSquaredTo(enemy2);
    float dist3Sq = target.DistanceSquaredTo(enemy3);

    std::cout << "  Enemy 1: ";
    enemy1.Print();
    std::cout << " -> Distance² = " << dist1Sq << std::endl;

    std::cout << "  Enemy 2: ";
    enemy2.Print();
    std::cout << " -> Distance² = " << dist2Sq << std::endl;

    std::cout << "  Enemy 3: ";
    enemy3.Print();
    std::cout << " -> Distance² = " << dist3Sq << std::endl;

    std::cout << "  Closest enemy: ";
    if (dist1Sq < dist2Sq && dist1Sq < dist3Sq) {
        std::cout << "Enemy 1";
    } else if (dist2Sq < dist3Sq) {
        std::cout << "Enemy 2";
    } else {
        std::cout << "Enemy 3";
    }
    std::cout << std::endl;
    std::cout << "  (Found without calculating square roots!)" << std::endl;
    std::cout << std::endl;

    // 8. PRACTICAL: SPHERE COLLISION (SIMPLE)
    std::cout << "8. PRACTICAL: SPHERE COLLISION DETECTION:" << std::endl;

    Vector3 sphere1Center(0, 0, 0);
    float sphere1Radius = 5.0f;

    Vector3 sphere2Center(8, 0, 0);
    float sphere2Radius = 2.0f;

    std::cout << "  Sphere 1: center = ";
    sphere1Center.Print();
    std::cout << ", radius = " << sphere1Radius << std::endl;

    std::cout << "  Sphere 2: center = ";
    sphere2Center.Print();
    std::cout << ", radius = " << sphere2Radius << std::endl;

    float centerDistance = sphere1Center.DistanceTo(sphere2Center);
    float minDistance = sphere1Radius + sphere2Radius;

    std::cout << "  Distance between centers: " << centerDistance << std::endl;
    std::cout << "  Minimum distance for collision: " << minDistance << std::endl;
    std::cout << "  Collision: " << (centerDistance < minDistance ? "YES" : "NO") << std::endl;
    std::cout << std::endl;

    // 9. MAGNITUDE PROPERTIES
    std::cout << "9. MAGNITUDE PROPERTIES:" << std::endl;

    Vector3 v5(2, 3, 6);
    Vector3 v5neg = Vector3(-v5.x, -v5.y, -v5.z);

    std::cout << "  v5  = ";
    v5.Print();
    std::cout << " -> |v5| = " << v5.Length() << std::endl;

    std::cout << "  -v5 = ";
    v5neg.Print();
    std::cout << " -> |-v5| = " << v5neg.Length() << std::endl;

    std::cout << "  Property: |v| = |-v| (negation doesn't change magnitude)" << std::endl;
    std::cout << std::endl;

    Vector3 v6(1, 1, 1);
    float scale = 5.0f;
    Vector3 v6scaled(v6.x * scale, v6.y * scale, v6.z * scale);

    std::cout << "  v6       = ";
    v6.Print();
    std::cout << " -> |v6| = " << v6.Length() << std::endl;

    std::cout << "  v6 * 5   = ";
    v6scaled.Print();
    std::cout << " -> |v6*5| = " << v6scaled.Length() << std::endl;

    std::cout << "  Property: |k*v| = |k| * |v| (scaling multiplies magnitude)" << std::endl;

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 *
 * 1. MAGNITUDE (LENGTH):
 *    Formula: ||v|| = sqrt(x² + y² + z²)
 *    - Always non-negative
 *    - Only zero vector has magnitude 0
 *    - Represents distance from origin
 *
 * 2. LENGTHSQUARED OPTIMIZATION:
 *    - Avoids expensive sqrt() operation
 *    - Perfect for comparisons
 *    - if ||v1|| < ||v2||, then ||v1||² < ||v2||²
 *    - Common in game loops and physics
 *
 * 3. DISTANCE CALCULATION:
 *    Distance = ||point2 - point1||
 *    - Symmetric: dist(A,B) = dist(B,A)
 *    - Triangle inequality: dist(A,C) ≤ dist(A,B) + dist(B,C)
 *
 * 4. MAGNITUDE PROPERTIES:
 *    - ||v|| ≥ 0
 *    - ||v|| = 0 ⟺ v = 0
 *    - ||k*v|| = |k| * ||v||
 *    - ||v|| = ||-v||
 *
 * 5. PRACTICAL APPLICATIONS:
 *    - Collision detection (sphere-sphere)
 *    - Finding closest object
 *    - Speed calculations
 *    - Normalization (next lesson)
 */
