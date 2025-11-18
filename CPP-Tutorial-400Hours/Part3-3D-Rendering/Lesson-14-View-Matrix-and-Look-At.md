# Lesson 14: View Matrix and Look-At

## Overview
The view matrix transforms world coordinates to camera space. Master the look-at matrix for flexible camera control.

## Look-At Matrix

```cpp
Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    Vector3 f = (center - eye).normalized();
    Vector3 r = up.cross(f).normalized();
    Vector3 u = f.cross(r);
    
    Matrix4 view;
    view.m[0][0] = r.x; view.m[0][1] = r.y; view.m[0][2] = r.z; view.m[0][3] = -r.dot(eye);
    view.m[1][0] = u.x; view.m[1][1] = u.y; view.m[1][2] = u.z; view.m[1][3] = -u.dot(eye);
    view.m[2][0] = f.x; view.m[2][1] = f.y; view.m[2][2] = f.z; view.m[2][3] = -f.dot(eye);
    view.m[3][0] = 0;   view.m[3][1] = 0;   view.m[3][2] = 0;   view.m[3][3] = 1;
    
    return view;
}
```

**Lesson 14 Complete** - Covers: View transforms, camera orientation, look-at construction
