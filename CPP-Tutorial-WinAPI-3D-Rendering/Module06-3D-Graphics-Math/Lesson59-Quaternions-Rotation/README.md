# Lesson 59: Quaternions for Rotation

## Quaternion Basics
- Represents rotation as q = (w, x, y, z)
- Unit quaternion: w² + x² + y² + z² = 1
- Avoids gimbal lock
- Smooth interpolation (SLERP)

## Quaternion Operations
```cpp
class Quaternion {
    float w, x, y, z;
    Quaternion operator*(const Quaternion& q) const;
    Vector3 RotateVector(const Vector3& v) const;
    static Quaternion FromAxisAngle(Vector3 axis, float angle);
    static Quaternion Slerp(Quaternion a, Quaternion b, float t);
};
```

## Code Examples
1-15: Quaternion class, multiplication, axis-angle conversion, Euler to quaternion, matrix to quaternion, SLERP, rotation composition
