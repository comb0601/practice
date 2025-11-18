# Lesson 12: Quaternions - Rotations and Interpolation

## Overview
This lesson explores advanced quaternion operations focusing on smooth rotations, interpolation techniques (slerp, nlerp), and practical applications in animation, camera systems, and character controllers.

## Table of Contents
1. [Quaternion Interpolation](#quaternion-interpolation)
2. [Slerp vs Nlerp](#slerp-vs-nlerp)
3. [Rotation Composition](#rotation-composition)
4. [Practical Applications](#practical-applications)
5. [Complete Animation System](#complete-animation-system)

## Quaternion Interpolation

### Linear Interpolation (Lerp)

```cpp
class Quaternion {
public:
    // Simple linear interpolation
    static Quaternion lerp(const Quaternion& a, const Quaternion& b, float t) {
        // Ensure shortest path
        float dot = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
        Quaternion b2 = (dot < 0) ? Quaternion(-b.w, -b.x, -b.y, -b.z) : b;
        
        Quaternion result(
            a.w + t * (b2.w - a.w),
            a.x + t * (b2.x - a.x),
            a.y + t * (b2.y - a.y),
            a.z + t * (b2.z - a.z)
        );
        
        return result.normalized();
    }
};
```

### Spherical Linear Interpolation (Slerp)

```cpp
class Quaternion {
public:
    // Slerp - constant angular velocity interpolation
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion q1 = a.normalized();
        Quaternion q2 = b.normalized();
        
        // Compute dot product
        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
        
        // If dot < 0, negate q2 to take shorter path
        if (dot < 0.0f) {
            q2 = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
            dot = -dot;
        }
        
        // Clamp dot product
        dot = std::min(1.0f, std::max(-1.0f, dot));
        
        // If quaternions are very close, use lerp to avoid division by zero
        if (dot > 0.9995f) {
            return lerp(q1, q2, t);
        }
        
        // Calculate angle
        float theta = std::acos(dot);
        float sinTheta = std::sin(theta);
        
        // Calculate interpolation coefficients
        float wa = std::sin((1.0f - t) * theta) / sinTheta;
        float wb = std::sin(t * theta) / sinTheta;
        
        return Quaternion(
            wa * q1.w + wb * q2.w,
            wa * q1.x + wb * q2.x,
            wa * q1.y + wb * q2.y,
            wa * q1.z + wb * q2.z
        );
    }
};
```

## Slerp vs Nlerp

### Normalized Lerp (Nlerp)

```cpp
class Quaternion {
public:
    // Nlerp - faster approximation of slerp
    static Quaternion nlerp(const Quaternion& a, const Quaternion& b, float t) {
        return lerp(a, b, t).normalized();
    }
    
    // Comparison demo
    static void compareMethods() {
        Quaternion start = fromAxisAngle(Vector3(0, 1, 0), 0);
        Quaternion end = fromAxisAngle(Vector3(0, 1, 0), PI);
        
        std::cout << "Method comparison at t=0.5:\n";
        
        Quaternion lerp_result = lerp(start, end, 0.5f);
        std::cout << "Lerp:  " << lerp_result.magnitude() << " (not unit)\n";
        
        Quaternion nlerp_result = nlerp(start, end, 0.5f);
        std::cout << "Nlerp: " << nlerp_result.magnitude() << " (unit)\n";
        
        Quaternion slerp_result = slerp(start, end, 0.5f);
        std::cout << "Slerp: " << slerp_result.magnitude() << " (unit)\n";
    }
};
```

### When to Use Each

```cpp
// Performance vs Quality trade-off
class InterpolationChoice {
public:
    // Use SLERP when:
    // - Constant angular velocity is required
    // - High-quality rotations needed
    // - Angle between quaternions is large
    
    // Use NLERP when:
    // - Performance is critical
    // - Angle between quaternions is small
    // - Slight speed variations are acceptable
    
    // Use LERP when:
    // - Don't need unit quaternions (rare)
    // - Intermediate step before normalization
};
```

## Rotation Composition

### Combining Rotations

```cpp
class Quaternion {
public:
    // Combine two rotations: first apply q1, then q2
    static Quaternion combine(const Quaternion& q1, const Quaternion& q2) {
        return q2 * q1; // Note: right-to-left application
    }
    
    // Difference between rotations
    static Quaternion difference(const Quaternion& from, const Quaternion& to) {
        return to * from.inverse();
    }
    
    // Relative rotation
    Quaternion relativeTo(const Quaternion& reference) const {
        return reference.inverse() * (*this);
    }
};
```

### Rotation Chains

```cpp
class RotationChain {
    std::vector<Quaternion> rotations;
    
public:
    void addRotation(const Quaternion& q) {
        rotations.push_back(q);
    }
    
    Quaternion getComposed() const {
        if (rotations.empty()) return Quaternion();
        
        Quaternion result = rotations[0];
        for (size_t i = 1; i < rotations.size(); i++) {
            result = rotations[i] * result;
        }
        return result.normalized();
    }
};
```

## Practical Applications

### Smooth Camera Follow

```cpp
class SmoothCamera {
    Quaternion currentRotation;
    float smoothSpeed;
    
public:
    SmoothCamera() : smoothSpeed(0.1f) {
        currentRotation = Quaternion();
    }
    
    void update(const Quaternion& targetRotation, float deltaTime) {
        // Smooth interpolation
        float t = 1.0f - exp(-smoothSpeed * deltaTime);
        currentRotation = Quaternion::slerp(currentRotation, targetRotation, t);
        currentRotation.normalize();
    }
    
    Matrix4 getViewMatrix(const Vector3& position) const {
        Matrix3 rotation = currentRotation.toMatrix();
        // Combine rotation and translation into view matrix
        return createViewMatrix(rotation, position);
    }
};
```

### Character Controller

```cpp
class CharacterController {
    Quaternion orientation;
    Vector3 position;
    
public:
    void rotateToward(const Vector3& target, float turnSpeed, float deltaTime) {
        // Calculate desired direction
        Vector3 direction = (target - position).normalized();
        
        // Current forward vector
        Vector3 forward = orientation.rotate(Vector3(0, 0, 1));
        
        // Create rotation from current to target
        Quaternion targetRotation = Quaternion::fromTo(forward, direction);
        
        // Smooth rotation
        float t = std::min(1.0f, turnSpeed * deltaTime);
        orientation = Quaternion::slerp(orientation, targetRotation, t);
        orientation.normalize();
    }
    
    Quaternion fromTo(const Vector3& from, const Vector3& to) {
        Vector3 f = from.normalized();
        Vector3 t = to.normalized();
        
        float dot = f.dot(t);
        
        if (dot > 0.99999f) return Quaternion(); // Same direction
        if (dot < -0.99999f) {
            // Opposite directions - 180 degree rotation
            Vector3 axis = Vector3(1, 0, 0);
            if (abs(f.x) > 0.9f) axis = Vector3(0, 1, 0);
            return Quaternion::fromAxisAngle(axis, PI);
        }
        
        Vector3 axis = f.cross(t).normalized();
        float angle = acos(dot);
        return Quaternion::fromAxisAngle(axis, angle);
    }
};
```

## Complete Animation System

```cpp
struct KeyFrame {
    float time;
    Quaternion rotation;
    Vector3 position;
    Vector3 scale;
};

class Animation {
    std::vector<KeyFrame> keyframes;
    float duration;
    
public:
    void addKeyFrame(const KeyFrame& kf) {
        keyframes.push_back(kf);
        if (kf.time > duration) duration = kf.time;
    }
    
    Transform evaluate(float time) const {
        // Loop animation
        time = fmod(time, duration);
        
        // Find surrounding keyframes
        size_t i = 0;
        while (i < keyframes.size() - 1 && keyframes[i+1].time <= time) {
            i++;
        }
        
        if (i >= keyframes.size() - 1) {
            return transformFromKeyFrame(keyframes.back());
        }
        
        // Interpolate
        const KeyFrame& kf0 = keyframes[i];
        const KeyFrame& kf1 = keyframes[i+1];
        
        float t = (time - kf0.time) / (kf1.time - kf0.time);
        
        Transform result;
        result.rotation = Quaternion::slerp(kf0.rotation, kf1.rotation, t);
        result.position = Vector3::lerp(kf0.position, kf1.position, t);
        result.scale = Vector3::lerp(kf0.scale, kf1.scale, t);
        
        return result;
    }
};
```

## Key Takeaways

1. **Slerp** provides constant angular velocity
2. **Nlerp** is faster but non-constant velocity
3. **Always check dot product** for shortest path
4. **Normalize after interpolation** to maintain unit length
5. **Quaternion multiplication** combines rotations
6. **Order matters**: q2 * q1 applies q1 first
7. **Use slerp for animations**, nlerp for real-time
8. **Smooth camera** systems use quaternion interpolation
9. **Character rotation** benefits from slerp
10. **Animation systems** rely on keyframe interpolation

## Exercises

1. Implement squad (spherical and quadrangle) interpolation
2. Create a spline-based rotation path system
3. Build an IK solver using quaternions
4. Implement look-at rotation with constraints
5. Create smooth orbit camera with quaternions

---

**Lesson 12 Complete** - Next: Lesson 13 - Camera Mathematics
