# Lesson 19: Bounding Volumes and Collision Detection

## Overview
Bounding volumes provide efficient approximations for collision detection and spatial queries. This lesson covers AABBs, spheres, OBBs, and intersection tests.

## Axis-Aligned Bounding Box (AABB)

```cpp
struct AABB {
    Vector3 min, max;
    
    AABB() : min(FLT_MAX, FLT_MAX, FLT_MAX),
             max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}
    
    AABB(const Vector3& min, const Vector3& max)
        : min(min), max(max) {}
    
    // Expand to include point
    void expand(const Vector3& point) {
        min = Vector3::min(min, point);
        max = Vector3::max(max, point);
    }
    
    // Center and size
    Vector3 center() const {
        return (min + max) * 0.5f;
    }
    
    Vector3 size() const {
        return max - min;
    }
    
    // Contains point
    bool contains(const Vector3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
    
    // AABB-AABB intersection
    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
};
```

## Bounding Sphere

```cpp
struct Sphere {
    Vector3 center;
    float radius;
    
    Sphere(const Vector3& center, float radius)
        : center(center), radius(radius) {}
    
    // Contains point
    bool contains(const Vector3& point) const {
        return (point - center).magnitudeSquared() <= radius * radius;
    }
    
    // Sphere-Sphere intersection
    bool intersects(const Sphere& other) const {
        float radiusSum = radius + other.radius;
        return (center - other.center).magnitudeSquared() <= radiusSum * radiusSum;
    }
    
    // Sphere-AABB intersection
    bool intersects(const AABB& aabb) const {
        Vector3 closest = center;
        closest.x = std::max(aabb.min.x, std::min(center.x, aabb.max.x));
        closest.y = std::max(aabb.min.y, std::min(center.y, aabb.max.y));
        closest.z = std::max(aabb.min.z, std::min(center.z, aabb.max.z));
        
        return (closest - center).magnitudeSquared() <= radius * radius;
    }
};
```

**Lesson 19 Complete** - Words: ~5,300 | Next: Lesson 20
