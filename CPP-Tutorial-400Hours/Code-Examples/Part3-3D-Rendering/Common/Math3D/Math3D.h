#pragma once

// Complete 3D Math Library for C++ 3D Rendering Course
// This library provides all essential 3D math functionality needed for graphics programming

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include <algorithm>

namespace Math3D {

// Constants
constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 6.28318530717958647692f;
constexpr float HALF_PI = 1.57079632679489661923f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;
constexpr float EPSILON = 0.00001f;

// Utility functions
inline float Clamp(float value, float min, float max) {
    return std::max(min, std::min(value, max));
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float Radians(float degrees) {
    return degrees * DEG_TO_RAD;
}

inline float Degrees(float radians) {
    return radians * RAD_TO_DEG;
}

// Smooth step interpolation
inline float SmoothStep(float edge0, float edge1, float x) {
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Ray structure for intersection tests
struct Ray {
    Vector3 origin;
    Vector3 direction;

    Ray() : origin(), direction(Vector3::Forward()) {}
    Ray(const Vector3& origin, const Vector3& direction)
        : origin(origin), direction(direction.Normalized()) {}

    Vector3 GetPoint(float t) const {
        return origin + direction * t;
    }
};

// Plane structure
struct Plane {
    Vector3 normal;
    float distance;

    Plane() : normal(Vector3::Up()), distance(0.0f) {}
    Plane(const Vector3& normal, float distance)
        : normal(normal.Normalized()), distance(distance) {}
    Plane(const Vector3& normal, const Vector3& point)
        : normal(normal.Normalized()), distance(normal.Dot(point)) {}

    float DistanceToPoint(const Vector3& point) const {
        return normal.Dot(point) - distance;
    }

    bool RayIntersection(const Ray& ray, float& t) const {
        float denom = normal.Dot(ray.direction);
        if (std::abs(denom) > EPSILON) {
            t = (distance - normal.Dot(ray.origin)) / denom;
            return t >= 0.0f;
        }
        return false;
    }
};

// AABB (Axis-Aligned Bounding Box)
struct AABB {
    Vector3 min;
    Vector3 max;

    AABB() : min(Vector3::Zero()), max(Vector3::Zero()) {}
    AABB(const Vector3& min, const Vector3& max) : min(min), max(max) {}

    Vector3 GetCenter() const {
        return (min + max) * 0.5f;
    }

    Vector3 GetExtents() const {
        return (max - min) * 0.5f;
    }

    bool Contains(const Vector3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    bool RayIntersection(const Ray& ray, float& tMin, float& tMax) const {
        Vector3 invDir(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
        Vector3 t0 = (min - ray.origin) * invDir;
        Vector3 t1 = (max - ray.origin) * invDir;

        Vector3 tmin(std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z));
        Vector3 tmax(std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z));

        tMin = std::max(std::max(tmin.x, tmin.y), tmin.z);
        tMax = std::min(std::min(tmax.x, tmax.y), tmax.z);

        return tMax >= tMin && tMax >= 0.0f;
    }
};

// Sphere structure
struct Sphere {
    Vector3 center;
    float radius;

    Sphere() : center(), radius(1.0f) {}
    Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}

    bool Contains(const Vector3& point) const {
        return center.DistanceSquared(point) <= radius * radius;
    }

    bool Intersects(const Sphere& other) const {
        float radiusSum = radius + other.radius;
        return center.DistanceSquared(other.center) <= radiusSum * radiusSum;
    }

    bool RayIntersection(const Ray& ray, float& t) const {
        Vector3 oc = ray.origin - center;
        float a = ray.direction.Dot(ray.direction);
        float b = 2.0f * oc.Dot(ray.direction);
        float c = oc.Dot(oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f) {
            return false;
        }

        float sqrtD = std::sqrt(discriminant);
        float t0 = (-b - sqrtD) / (2.0f * a);
        float t1 = (-b + sqrtD) / (2.0f * a);

        if (t0 > 0.0f) {
            t = t0;
            return true;
        } else if (t1 > 0.0f) {
            t = t1;
            return true;
        }

        return false;
    }
};

// Frustum for view frustum culling
struct Frustum {
    Plane planes[6]; // Left, Right, Bottom, Top, Near, Far

    void ExtractFromMatrix(const Matrix4& viewProj) {
        // Left plane
        planes[0].normal.x = viewProj.m[3] + viewProj.m[0];
        planes[0].normal.y = viewProj.m[7] + viewProj.m[4];
        planes[0].normal.z = viewProj.m[11] + viewProj.m[8];
        planes[0].distance = viewProj.m[15] + viewProj.m[12];

        // Right plane
        planes[1].normal.x = viewProj.m[3] - viewProj.m[0];
        planes[1].normal.y = viewProj.m[7] - viewProj.m[4];
        planes[1].normal.z = viewProj.m[11] - viewProj.m[8];
        planes[1].distance = viewProj.m[15] - viewProj.m[12];

        // Bottom plane
        planes[2].normal.x = viewProj.m[3] + viewProj.m[1];
        planes[2].normal.y = viewProj.m[7] + viewProj.m[5];
        planes[2].normal.z = viewProj.m[11] + viewProj.m[9];
        planes[2].distance = viewProj.m[15] + viewProj.m[13];

        // Top plane
        planes[3].normal.x = viewProj.m[3] - viewProj.m[1];
        planes[3].normal.y = viewProj.m[7] - viewProj.m[5];
        planes[3].normal.z = viewProj.m[11] - viewProj.m[9];
        planes[3].distance = viewProj.m[15] - viewProj.m[13];

        // Near plane
        planes[4].normal.x = viewProj.m[3] + viewProj.m[2];
        planes[4].normal.y = viewProj.m[7] + viewProj.m[6];
        planes[4].normal.z = viewProj.m[11] + viewProj.m[10];
        planes[4].distance = viewProj.m[15] + viewProj.m[14];

        // Far plane
        planes[5].normal.x = viewProj.m[3] - viewProj.m[2];
        planes[5].normal.y = viewProj.m[7] - viewProj.m[6];
        planes[5].normal.z = viewProj.m[11] - viewProj.m[10];
        planes[5].distance = viewProj.m[15] - viewProj.m[14];

        // Normalize all planes
        for (int i = 0; i < 6; i++) {
            float length = planes[i].normal.Length();
            planes[i].normal = planes[i].normal / length;
            planes[i].distance /= length;
        }
    }

    bool ContainsPoint(const Vector3& point) const {
        for (int i = 0; i < 6; i++) {
            if (planes[i].DistanceToPoint(point) < 0.0f) {
                return false;
            }
        }
        return true;
    }

    bool ContainsSphere(const Sphere& sphere) const {
        for (int i = 0; i < 6; i++) {
            if (planes[i].DistanceToPoint(sphere.center) < -sphere.radius) {
                return false;
            }
        }
        return true;
    }

    bool ContainsAABB(const AABB& aabb) const {
        Vector3 center = aabb.GetCenter();
        Vector3 extents = aabb.GetExtents();

        for (int i = 0; i < 6; i++) {
            const Plane& plane = planes[i];
            float r = std::abs(extents.x * plane.normal.x) +
                     std::abs(extents.y * plane.normal.y) +
                     std::abs(extents.z * plane.normal.z);
            float d = plane.DistanceToPoint(center);
            if (d < -r) {
                return false;
            }
        }
        return true;
    }
};

// Triangle intersection
inline bool RayTriangleIntersection(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& t, Vector3& barycentric) {
    const float EPSILON = 0.0000001f;
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 h = ray.direction.Cross(edge2);
    float a = edge1.Dot(h);

    if (a > -EPSILON && a < EPSILON) {
        return false; // Ray is parallel to triangle
    }

    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    barycentric.x = f * s.Dot(h);

    if (barycentric.x < 0.0f || barycentric.x > 1.0f) {
        return false;
    }

    Vector3 q = s.Cross(edge1);
    barycentric.y = f * ray.direction.Dot(q);

    if (barycentric.y < 0.0f || barycentric.x + barycentric.y > 1.0f) {
        return false;
    }

    t = f * edge2.Dot(q);
    barycentric.z = 1.0f - barycentric.x - barycentric.y;

    return t > EPSILON;
}

} // namespace Math3D
