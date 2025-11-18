# Lesson 63: Collision Detection

## AABB (Axis-Aligned Bounding Box)
```cpp
bool AABBIntersect(Vector3 min1, Vector3 max1, Vector3 min2, Vector3 max2);
```

## Sphere Collision
```cpp
bool SphereIntersect(Vector3 center1, float r1, Vector3 center2, float r2);
```

## Ray Intersection
- Ray-Sphere
- Ray-Box
- Ray-Plane

## Code Examples
1-15: AABB test, sphere test, ray-sphere, ray-box, broad phase, narrow phase, spatial hashing
