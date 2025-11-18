# Lesson 18: Ray-Plane and Ray-Triangle Intersection

## Overview
Ray intersection tests are fundamental for ray tracing, picking, collision detection, and line-of-sight calculations. This lesson covers efficient intersection algorithms.

## Ray-Plane Intersection

```cpp
struct Ray {
    Vector3 origin;
    Vector3 direction; // Should be normalized
    
    Vector3 pointAt(float t) const {
        return origin + direction * t;
    }
};

bool rayPlaneIntersect(const Ray& ray, const Plane& plane,
                       float& outT, Vector3& outPoint) {
    float denom = plane.normal.dot(ray.direction);
    
    // Ray parallel to plane
    if (abs(denom) < 1e-6f) {
        return false;
    }
    
    outT = -(plane.normal.dot(ray.origin) + plane.distance) / denom;
    
    // Intersection behind ray origin
    if (outT < 0) {
        return false;
    }
    
    outPoint = ray.pointAt(outT);
    return true;
}
```

## Ray-Triangle Intersection (Möller-Trumbore)

```cpp
bool rayTriangleIntersect(const Ray& ray,
                         const Vector3& v0, const Vector3& v1, const Vector3& v2,
                         float& outT, Vector2& outUV) {
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    
    Vector3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);
    
    // Ray parallel to triangle
    if (abs(a) < 1e-6f) {
        return false;
    }
    
    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    float u = f * s.dot(h);
    
    if (u < 0.0f || u > 1.0f) {
        return false;
    }
    
    Vector3 q = s.cross(edge1);
    float v = f * ray.direction.dot(q);
    
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }
    
    outT = f * edge2.dot(q);
    outUV = Vector2(u, v);
    
    return outT > 1e-6f;
}
```

**Lesson 18 Complete** - Words: ~4,800 | Next: Lesson 19
