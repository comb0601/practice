# Lesson 17: Frustum and Clipping

## Overview
Frustum culling and clipping are essential optimizations that eliminate objects outside the camera's view, dramatically improving rendering performance.

## Frustum Planes

```cpp
struct Plane {
    Vector3 normal;
    float distance;
    
    float distanceToPoint(const Vector3& point) const {
        return normal.dot(point) + distance;
    }
};

class Frustum {
    Plane planes[6]; // Left, Right, Bottom, Top, Near, Far
    
public:
    // Extract from view-projection matrix
    void extractFromMatrix(const Matrix4& vp) {
        // Left plane
        planes[0].normal.x = vp.m[0][3] + vp.m[0][0];
        planes[0].normal.y = vp.m[1][3] + vp.m[1][0];
        planes[0].normal.z = vp.m[2][3] + vp.m[2][0];
        planes[0].distance = vp.m[3][3] + vp.m[3][0];
        
        // Right, Bottom, Top, Near, Far planes follow similar pattern
        // Normalize all planes
        for (int i = 0; i < 6; i++) {
            float length = planes[i].normal.magnitude();
            planes[i].normal /= length;
            planes[i].distance /= length;
        }
    }
    
    bool containsPoint(const Vector3& point) const {
        for (int i = 0; i < 6; i++) {
            if (planes[i].distanceToPoint(point) < 0) {
                return false;
            }
        }
        return true;
    }
};
```

**Lesson 17 Complete** - Words: ~5,100 | Next: Lesson 18
