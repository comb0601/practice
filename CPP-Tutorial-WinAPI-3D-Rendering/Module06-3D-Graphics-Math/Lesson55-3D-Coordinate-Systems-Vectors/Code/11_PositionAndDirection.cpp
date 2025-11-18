/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 11: Position Vectors vs Direction Vectors
 *
 * Compile: g++ -o 11_PositionAndDirection 11_PositionAndDirection.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
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
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Position vs Direction Vectors ===" << std::endl;
    std::cout << std::endl;

    // 1. Position vectors
    std::cout << "1. POSITION VECTORS (Points in Space):" << std::endl;
    Vector3 playerPos(10, 0, 5);
    Vector3 enemyPos(15, 0, 10);

    std::cout << "  Player position: ";
    playerPos.Print();
    std::cout << " (10 units right, 5 forward)" << std::endl;
    std::cout << "  Enemy position:  ";
    enemyPos.Print();
    std::cout << " (15 units right, 10 forward)" << std::endl;
    std::cout << "  These represent LOCATIONS in 3D space" << std::endl;
    std::cout << std::endl;

    // 2. Direction vectors
    std::cout << "2. DIRECTION VECTORS:" << std::endl;
    Vector3 toEnemy = enemyPos - playerPos;

    std::cout << "  Direction to enemy: ";
    toEnemy.Print();
    std::cout << std::endl;
    std::cout << "  Length: " << toEnemy.Length() << " units" << std::endl;
    std::cout << "  This represents DIRECTION and DISTANCE" << std::endl;
    std::cout << std::endl;

    Vector3 toEnemyNormalized = toEnemy.Normalized();
    std::cout << "  Normalized direction: ";
    toEnemyNormalized.Print();
    std::cout << std::endl;
    std::cout << "  Length: " << toEnemyNormalized.Length() << std::endl;
    std::cout << "  This represents PURE DIRECTION (no magnitude)" << std::endl;
    std::cout << std::endl;

    // 3. Velocity vectors
    std::cout << "3. VELOCITY VECTORS:" << std::endl;
    Vector3 velocity = toEnemyNormalized * 5.0f;

    std::cout << "  Velocity: ";
    velocity.Print();
    std::cout << std::endl;
    std::cout << "  Speed: " << velocity.Length() << " units/second" << std::endl;
    std::cout << "  This represents DIRECTION and SPEED" << std::endl;
    std::cout << std::endl;

    // 4. Updating position
    std::cout << "4. UPDATING POSITION WITH VELOCITY:" << std::endl;
    float deltaTime = 1.0f;  // 1 second
    Vector3 newPlayerPos = playerPos + velocity * deltaTime;

    std::cout << "  Old position: ";
    playerPos.Print();
    std::cout << std::endl;
    std::cout << "  Velocity: ";
    velocity.Print();
    std::cout << std::endl;
    std::cout << "  Time: " << deltaTime << " second" << std::endl;
    std::cout << "  New position: ";
    newPlayerPos.Print();
    std::cout << std::endl;
    std::cout << "  Formula: newPos = oldPos + velocity * time" << std::endl;
    std::cout << std::endl;

    // 5. Practical example: patrol points
    std::cout << "5. PRACTICAL: PATROL POINTS:" << std::endl;
    Vector3 waypoints[] = {
        Vector3(0, 0, 0),
        Vector3(10, 0, 0),
        Vector3(10, 0, 10),
        Vector3(0, 0, 10)
    };

    for (int i = 0; i < 4; i++) {
        std::cout << "  Waypoint " << i << ": ";
        waypoints[i].Print();
        std::cout << " (POSITION)" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "  Directions between waypoints:" << std::endl;
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        Vector3 dir = waypoints[next] - waypoints[i];
        std::cout << "  " << i << " -> " << next << ": ";
        dir.Print();
        std::cout << " (DIRECTION, distance=" << dir.Length() << ")" << std::endl;
    }

    return 0;
}
