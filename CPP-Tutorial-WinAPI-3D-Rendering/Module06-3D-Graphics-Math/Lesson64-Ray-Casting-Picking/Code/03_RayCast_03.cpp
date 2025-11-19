/*
 * Lesson 64 - Ray Casting and Picking
 * Ray-sphere, ray-plane, ray-triangle intersection
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
    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

    float Length() const { return sqrt(x*x + y*y + z*z); }
    float LengthSquared() const { return x*x + y*y + z*z; }

    Vector3 Normalized() const {
        float len = Length();
        return len > 0 ? *this * (1.0f/len) : Vector3();
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

struct Ray {
    Vector3 origin;
    Vector3 direction;  // Must be normalized

    Ray(const Vector3& o, const Vector3& d) : origin(o), direction(d.Normalized()) {}

    Vector3 PointAt(float t) const {
        return origin + direction * t;
    }
};

struct Sphere {
    Vector3 center;
    float radius;

    Sphere(const Vector3& c, float r) : center(c), radius(r) {}
};

struct Plane {
    Vector3 normal;
    float distance;

    Plane(const Vector3& n, float d) : normal(n.Normalized()), distance(d) {}
};

// Ray-Sphere intersection
bool RaySphereIntersection(const Ray& ray, const Sphere& sphere, float& t) {
    Vector3 oc = ray.origin - sphere.center;
    float a = ray.direction.Dot(ray.direction);
    float b = 2.0f * oc.Dot(ray.direction);
    float c = oc.Dot(oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;  // No intersection
    }

    float sqrtDisc = sqrt(discriminant);
    float t0 = (-b - sqrtDisc) / (2.0f * a);
    float t1 = (-b + sqrtDisc) / (2.0f * a);

    // Return closest positive intersection
    if (t0 > 0) {
        t = t0;
        return true;
    } else if (t1 > 0) {
        t = t1;
        return true;
    }

    return false;
}

// Ray-Plane intersection
bool RayPlaneIntersection(const Ray& ray, const Plane& plane, float& t) {
    float denom = plane.normal.Dot(ray.direction);

    if (fabs(denom) < 1e-6) {
        return false;  // Ray parallel to plane
    }

    t = (plane.distance - plane.normal.Dot(ray.origin)) / denom;
    return t >= 0;
}

// Ray-Triangle intersection (Möller-Trumbore algorithm)
bool RayTriangleIntersection(const Ray& ray, const Vector3& v0, const Vector3& v1,
                             const Vector3& v2, float& t) {
    const float EPSILON = 1e-6f;

    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 h = ray.direction.Cross(edge2);
    float a = edge1.Dot(h);

    if (fabs(a) < EPSILON) {
        return false;  // Ray parallel to triangle
    }

    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    float u = f * s.Dot(h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vector3 q = s.Cross(edge1);
    float v = f * ray.direction.Dot(q);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    t = f * edge2.Dot(q);
    return t > EPSILON;
}

int main() {
    std::cout << "=== Lesson 64: Ray Casting and Picking ===\n\n";

    // Test ray
    Ray ray(Vector3(0, 0, -10), Vector3(0, 0, 1));
    std::cout << "Ray origin: "; ray.origin.Print(); std::cout << "\n";
    std::cout << "Ray direction: "; ray.direction.Print(); std::cout << "\n\n";

    // Ray-Sphere test
    Sphere sphere(Vector3(0, 0, 0), 3.0f);
    float t;

    if (RaySphereIntersection(ray, sphere, t)) {
        Vector3 hitPoint = ray.PointAt(t);
        std::cout << "Ray-Sphere Intersection:\n";
        std::cout << "  Hit at t = " << t << "\n";
        std::cout << "  Hit point: "; hitPoint.Print(); std::cout << "\n\n";
    }

    // Ray-Plane test
    Plane plane(Vector3(0, 0, 1), 5.0f);
    if (RayPlaneIntersection(ray, plane, t)) {
        Vector3 hitPoint = ray.PointAt(t);
        std::cout << "Ray-Plane Intersection:\n";
        std::cout << "  Hit at t = " << t << "\n";
        std::cout << "  Hit point: "; hitPoint.Print(); std::cout << "\n\n";
    }

    // Ray-Triangle test
    Vector3 v0(-5, -5, 0);
    Vector3 v1(5, -5, 0);
    Vector3 v2(0, 5, 0);

    if (RayTriangleIntersection(ray, v0, v1, v2, t)) {
        Vector3 hitPoint = ray.PointAt(t);
        std::cout << "Ray-Triangle Intersection:\n";
        std::cout << "  Hit at t = " << t << "\n";
        std::cout << "  Hit point: "; hitPoint.Print(); std::cout << "\n\n";
    } else {
        std::cout << "Ray-Triangle: No intersection\n\n";
    }

    std::cout << "=== Program Complete ===\n";
    return 0;
}
