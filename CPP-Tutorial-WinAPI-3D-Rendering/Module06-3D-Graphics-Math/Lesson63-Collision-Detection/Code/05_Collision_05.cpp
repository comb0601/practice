/*
 * Lesson 63 - Collision Detection
 * 3D collision detection algorithms
 *
 * Compilation: g++ -std=c++11 -o output filename.cpp -lm
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }

    float Dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
    float Length() const { return sqrt(x*x + y*y + z*z); }
    float LengthSquared() const { return x*x + y*y + z*z; }

    Vector3 Normalized() const {
        float len = Length();
        return len > 0 ? *this * (1.0f/len) : Vector3();
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

struct Sphere {
    Vector3 center;
    float radius;

    Sphere(const Vector3& c, float r) : center(c), radius(r) {}
};

struct AABB {  // Axis-Aligned Bounding Box
    Vector3 min, max;

    AABB(const Vector3& min, const Vector3& max) : min(min), max(max) {}
};

struct Plane {
    Vector3 normal;
    float distance;

    Plane(const Vector3& n, float d) : normal(n.Normalized()), distance(d) {}
};

// Sphere-Sphere collision
bool SphereSphereCollision(const Sphere& a, const Sphere& b) {
    float distSq = (b.center - a.center).LengthSquared();
    float radiusSum = a.radius + b.radius;
    return distSq <= radiusSum * radiusSum;
}

// AABB-AABB collision
bool AABBAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// Sphere-AABB collision
bool SphereAABBCollision(const Sphere& s, const AABB& aabb) {
    // Find closest point on AABB to sphere center
    Vector3 closest(
        std::max(aabb.min.x, std::min(s.center.x, aabb.max.x)),
        std::max(aabb.min.y, std::min(s.center.y, aabb.max.y)),
        std::max(aabb.min.z, std::min(s.center.z, aabb.max.z))
    );

    float distSq = (closest - s.center).LengthSquared();
    return distSq <= s.radius * s.radius;
}

// Point-Plane distance
float PointPlaneDistance(const Vector3& point, const Plane& plane) {
    return plane.normal.Dot(point) - plane.distance;
}

int main() {
    std::cout << "=== Lesson 63: Collision Detection ===\n\n";

    // Sphere-Sphere test
    Sphere s1(Vector3(0, 0, 0), 5.0f);
    Sphere s2(Vector3(8, 0, 0), 4.0f);
    Sphere s3(Vector3(20, 0, 0), 3.0f);

    std::cout << "Sphere-Sphere Collision:\n";
    std::cout << "  s1 vs s2: " << (SphereSphereCollision(s1, s2) ? "COLLISION" : "No collision") << "\n";
    std::cout << "  s1 vs s3: " << (SphereSphereCollision(s1, s3) ? "COLLISION" : "No collision") << "\n";

    // AABB-AABB test
    AABB box1(Vector3(-5, -5, -5), Vector3(5, 5, 5));
    AABB box2(Vector3(3, 3, 3), Vector3(10, 10, 10));
    AABB box3(Vector3(20, 20, 20), Vector3(30, 30, 30));

    std::cout << "\nAABB-AABB Collision:\n";
    std::cout << "  box1 vs box2: " << (AABBAABBCollision(box1, box2) ? "COLLISION" : "No collision") << "\n";
    std::cout << "  box1 vs box3: " << (AABBAABBCollision(box1, box3) ? "COLLISION" : "No collision") << "\n";

    // Sphere-AABB test
    std::cout << "\nSphere-AABB Collision:\n";
    std::cout << "  s1 vs box2: " << (SphereAABBCollision(s1, box2) ? "COLLISION" : "No collision") << "\n";
    std::cout << "  s3 vs box1: " << (SphereAABBCollision(s3, box1) ? "COLLISION" : "No collision") << "\n";

    // Point-Plane distance
    Plane ground(Vector3(0, 1, 0), 0);
    Vector3 point1(0, 5, 0);
    Vector3 point2(0, -3, 0);

    std::cout << "\nPoint-Plane Distance:\n";
    std::cout << "  Point (0,5,0) to ground: " << PointPlaneDistance(point1, ground) << "\n";
    std::cout << "  Point (0,-3,0) to ground: " << PointPlaneDistance(point2, ground) << "\n";

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
