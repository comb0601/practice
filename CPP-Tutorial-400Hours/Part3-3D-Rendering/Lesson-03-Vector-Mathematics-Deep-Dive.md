# Lesson 03: Vector Mathematics Deep Dive

## Overview
Building on the fundamentals from Lesson 02, this lesson explores advanced vector mathematics that forms the backbone of sophisticated 3D graphics techniques. We'll cover vector projections, reflections, advanced interpolation methods, and specialized operations used in lighting, physics, and rendering. Every concept includes complete C++ implementations and practical examples.

## Table of Contents
1. [Vector Projection](#vector-projection)
2. [Vector Rejection and Decomposition](#vector-rejection-and-decomposition)
3. [Vector Reflection](#vector-reflection)
4. [Vector Refraction](#vector-refraction)
5. [Advanced Interpolation](#advanced-interpolation)
6. [Angle Calculations](#angle-calculations)
7. [Orthonormal Bases](#orthonormal-bases)
8. [Gram-Schmidt Orthogonalization](#gram-schmidt-orthogonalization)
9. [Complete Advanced Vector Library](#complete-advanced-vector-library)
10. [Real-World Applications](#real-world-applications)

## Vector Projection

### What is Projection?

Projection is the process of finding the component of one vector in the direction of another. Imagine shining a light perpendicular to a surface and seeing the shadow of a vector on that surface - that's projection.

### Mathematical Definition

The projection of vector **a** onto vector **b** is:

```
proj_b(a) = ((a · b) / (b · b)) * b
```

For a unit vector **b̂**, this simplifies to:

```
proj_b̂(a) = (a · b̂) * b̂
```

### Geometric Interpretation

```
a
 \
  \
   \
    \________ proj_b(a)
         →
         b
```

The projection is the part of **a** that points in the direction of **b**.

### Applications

- **Lighting**: Calculate how much light hits a surface
- **Physics**: Separate motion into components
- **Camera**: Determine what's in front of the camera
- **Collision**: Find closest point on a line

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    float x, y, z;

    // ... (basic operations from Lesson 02) ...

    // Project this vector onto another vector
    Vector3Advanced projectOnto(const Vector3Advanced& target) const {
        float targetLengthSq = target.magnitudeSquared();

        if (targetLengthSq < 1e-8f) {
            throw std::runtime_error("Cannot project onto zero vector");
        }

        float dotProduct = this->dot(target);
        return target * (dotProduct / targetLengthSq);
    }

    // Project onto a unit vector (faster - no division by length)
    Vector3Advanced projectOntoNormalized(const Vector3Advanced& unitTarget) const {
        float dotProduct = this->dot(unitTarget);
        return unitTarget * dotProduct;
    }

    // Get scalar projection (the magnitude of the projection)
    float scalarProjectionOnto(const Vector3Advanced& target) const {
        float targetLength = target.magnitude();

        if (targetLength < 1e-8f) {
            throw std::runtime_error("Cannot project onto zero vector");
        }

        return this->dot(target) / targetLength;
    }
};
```

## Vector Rejection and Decomposition

### What is Rejection?

Rejection is the component of a vector perpendicular to another vector. Together, projection and rejection decompose a vector into parallel and perpendicular components.

### Mathematical Definition

```
rej_b(a) = a - proj_b(a)
```

The rejection is what's "left over" after removing the projection.

### Decomposition

Any vector **a** can be decomposed relative to **b**:

```
a = proj_b(a) + rej_b(a)
   parallel    perpendicular
```

### Visual Representation

```
      rej_b(a)
         ↑
         |
a       /
 \     /
  \   /
   \ /
    +------→ proj_b(a)

    Direction of b
```

### Applications

- **Sliding**: Make objects slide along surfaces
- **Physics**: Separate forces into components
- **Character Movement**: Walk along slopes
- **Collision Response**: Bounce off surfaces

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Get the rejection (perpendicular component)
    Vector3Advanced rejectFrom(const Vector3Advanced& target) const {
        return *this - projectOnto(target);
    }

    // Decompose into parallel and perpendicular components
    void decompose(const Vector3Advanced& target,
                   Vector3Advanced& outParallel,
                   Vector3Advanced& outPerpendicular) const {
        outParallel = projectOnto(target);
        outPerpendicular = *this - outParallel;
    }

    // Get the component along a direction (scalar)
    float componentAlong(const Vector3Advanced& direction) const {
        Vector3Advanced normalized = direction.normalized();
        return this->dot(normalized);
    }
};
```

## Vector Reflection

### What is Reflection?

Reflection calculates how a vector bounces off a surface. This is essential for mirror surfaces, light bouncing, and physics collisions.

### Mathematical Definition

Given incident vector **i** and surface normal **n** (unit vector):

```
r = i - 2(i · n)n
```

### Geometric Interpretation

```
    n (normal)
    ↑
    |
    |
i   |   r (reflected)
 \  |  /
  \ | /
   \|/
====+=====
  Surface
```

The angle of incidence equals the angle of reflection.

### Properties

- The incident and reflected vectors make equal angles with the normal
- The normal must point toward the incident vector (or adjust the formula)
- Used in ray tracing, light simulation, and collision response

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Reflect this vector across a surface with given normal
    // Normal should be unit length and point toward the incident vector
    Vector3Advanced reflect(const Vector3Advanced& normal) const {
        return *this - normal * (2.0f * this->dot(normal));
    }

    // Reflect with non-normalized normal (less efficient)
    Vector3Advanced reflectNonNormalized(const Vector3Advanced& normal) const {
        Vector3Advanced unitNormal = normal.normalized();
        return reflect(unitNormal);
    }

    // Check if vector is pointing toward or away from normal
    bool isPointingToward(const Vector3Advanced& normal) const {
        return this->dot(normal) < 0;
    }
};
```

## Vector Refraction

### What is Refraction?

Refraction occurs when a vector (like a light ray) passes through the boundary between two media with different densities, causing it to bend.

### Snell's Law

```
n₁ sin(θ₁) = n₂ sin(θ₂)

where:
n₁ = refractive index of first medium
n₂ = refractive index of second medium
θ₁ = angle of incidence
θ₂ = angle of refraction
```

### Mathematical Formula

Given incident vector **i**, normal **n**, and relative refractive index η = n₁/n₂:

```
k = 1 - η²(1 - (n · i)²)

if k < 0: total internal reflection
else: t = ηi - (η(n · i) + √k)n
```

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Refract vector through surface
    // normal: surface normal (unit vector)
    // eta: ratio of refractive indices (n1/n2)
    // returns refracted vector, or zero vector if total internal reflection
    Vector3Advanced refract(const Vector3Advanced& normal, float eta) const {
        float cosI = -this->dot(normal);
        float sinT2 = eta * eta * (1.0f - cosI * cosI);

        // Total internal reflection
        if (sinT2 > 1.0f) {
            return Vector3Advanced(0, 0, 0);
        }

        float cosT = std::sqrt(1.0f - sinT2);
        return (*this * eta) + (normal * (eta * cosI - cosT));
    }

    // Check if refraction results in total internal reflection
    bool isTotalInternalReflection(const Vector3Advanced& normal, float eta) const {
        float cosI = -this->dot(normal);
        float sinT2 = eta * eta * (1.0f - cosI * cosI);
        return sinT2 > 1.0f;
    }
};
```

### Refractive Index Examples

Common materials:
- Air: 1.0
- Water: 1.33
- Glass: 1.5 - 1.9
- Diamond: 2.42

## Advanced Interpolation

### Linear Interpolation (Lerp)

We covered basic lerp in Lesson 02. Here's a more detailed look:

```
lerp(a, b, t) = a + t(b - a) = (1-t)a + tb

where t ∈ [0, 1]
```

### Spherical Linear Interpolation (Slerp)

Slerp interpolates along the shortest arc on a sphere, maintaining constant velocity. It's crucial for smooth rotations.

```
slerp(a, b, t) = (sin((1-t)θ) / sin(θ))a + (sin(tθ) / sin(θ))b

where θ = arccos(a · b)
```

### Normalized Linear Interpolation (Nlerp)

Nlerp is a faster approximation of slerp:

```
nlerp(a, b, t) = normalize(lerp(a, b, t))
```

### Comparison

- **Lerp**: Fast, constant speed, doesn't maintain length
- **Nlerp**: Fast, maintains length, varying speed
- **Slerp**: Accurate, constant angular velocity, expensive

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Linear interpolation (from Lesson 02, included for completeness)
    static Vector3Advanced lerp(const Vector3Advanced& a,
                                 const Vector3Advanced& b,
                                 float t) {
        return a + (b - a) * t;
    }

    // Spherical linear interpolation
    static Vector3Advanced slerp(const Vector3Advanced& a,
                                  const Vector3Advanced& b,
                                  float t) {
        // Get the angle between vectors
        float dot = a.dot(b);

        // Clamp dot product to avoid numerical errors in acos
        dot = std::max(-1.0f, std::min(1.0f, dot));

        float theta = std::acos(dot);

        // If angle is very small, use lerp instead
        if (theta < 1e-6f) {
            return lerp(a, b, t);
        }

        float sinTheta = std::sin(theta);
        float wa = std::sin((1.0f - t) * theta) / sinTheta;
        float wb = std::sin(t * theta) / sinTheta;

        return a * wa + b * wb;
    }

    // Normalized lerp (faster approximation of slerp)
    static Vector3Advanced nlerp(const Vector3Advanced& a,
                                  const Vector3Advanced& b,
                                  float t) {
        return lerp(a, b, t).normalized();
    }

    // Cubic Hermite interpolation (smooth interpolation with velocity)
    static Vector3Advanced hermite(const Vector3Advanced& p0,
                                    const Vector3Advanced& p1,
                                    const Vector3Advanced& v0,
                                    const Vector3Advanced& v1,
                                    float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        float h00 = 2 * t3 - 3 * t2 + 1;
        float h10 = t3 - 2 * t2 + t;
        float h01 = -2 * t3 + 3 * t2;
        float h11 = t3 - t2;

        return p0 * h00 + v0 * h10 + p1 * h01 + v1 * h11;
    }

    // Catmull-Rom spline (smooth path through points)
    static Vector3Advanced catmullRom(const Vector3Advanced& p0,
                                       const Vector3Advanced& p1,
                                       const Vector3Advanced& p2,
                                       const Vector3Advanced& p3,
                                       float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        return p0 * (-0.5f * t3 + t2 - 0.5f * t) +
               p1 * (1.5f * t3 - 2.5f * t2 + 1.0f) +
               p2 * (-1.5f * t3 + 2.0f * t2 + 0.5f * t) +
               p3 * (0.5f * t3 - 0.5f * t2);
    }
};
```

## Angle Calculations

### Angle Between Vectors

The angle θ between two vectors can be found using the dot product:

```
cos(θ) = (a · b) / (|a| |b|)
θ = arccos((a · b) / (|a| |b|))
```

### Signed Angle

For a signed angle in a plane with normal **n**:

```
angle = atan2((a × b) · n, a · b)
```

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Angle between vectors in radians
    float angleTo(const Vector3Advanced& other) const {
        float lengthProduct = this->magnitude() * other.magnitude();

        if (lengthProduct < 1e-8f) {
            return 0.0f;
        }

        float cosAngle = this->dot(other) / lengthProduct;
        cosAngle = std::max(-1.0f, std::min(1.0f, cosAngle));  // Clamp for numerical safety

        return std::acos(cosAngle);
    }

    // Angle between vectors in degrees
    float angleToInDegrees(const Vector3Advanced& other) const {
        return angleTo(other) * 180.0f / 3.14159265358979323846f;
    }

    // Signed angle in a plane defined by normal
    float signedAngleTo(const Vector3Advanced& other,
                        const Vector3Advanced& normal) const {
        float unsignedAngle = angleTo(other);
        Vector3Advanced cross = this->cross(other);
        float sign = (cross.dot(normal) < 0) ? -1.0f : 1.0f;
        return unsignedAngle * sign;
    }

    // Fast angle check without computing actual angle
    bool isAcuteAngle(const Vector3Advanced& other) const {
        return this->dot(other) > 0;
    }

    bool isObtuseAngle(const Vector3Advanced& other) const {
        return this->dot(other) < 0;
    }

    bool isRightAngle(const Vector3Advanced& other, float epsilon = 1e-5f) const {
        return std::abs(this->dot(other)) < epsilon;
    }
};
```

## Orthonormal Bases

### What is an Orthonormal Basis?

An orthonormal basis is a set of mutually perpendicular unit vectors that define a coordinate system.

### Properties

1. **Orthogonal**: Each vector is perpendicular to the others (dot product = 0)
2. **Normalized**: Each vector has unit length (magnitude = 1)

### Standard Basis

The standard basis in 3D:
- **e₁** = (1, 0, 0) - X-axis
- **e₂** = (0, 1, 0) - Y-axis
- **e₃** = (0, 0, 1) - Z-axis

### Custom Bases

You can create custom coordinate systems for:
- Camera orientation (forward, right, up)
- Object local space
- Texture mapping coordinates
- Physics constraints

### C++ Implementation

```cpp
struct OrthonormalBasis {
    Vector3Advanced x, y, z;

    // Construct from three vectors (will be orthonormalized)
    OrthonormalBasis(const Vector3Advanced& v1,
                     const Vector3Advanced& v2,
                     const Vector3Advanced& v3) {
        // Use Gram-Schmidt to create orthonormal basis
        x = v1.normalized();
        y = (v2 - v2.projectOnto(x)).normalized();
        z = x.cross(y);  // Already normalized if x and y are
    }

    // Construct from one vector (create arbitrary perpendicular vectors)
    static OrthonormalBasis fromOneVector(const Vector3Advanced& forward) {
        Vector3Advanced f = forward.normalized();

        // Find a vector not parallel to forward
        Vector3Advanced temp = (std::abs(f.x) < 0.9f) ?
                               Vector3Advanced(1, 0, 0) :
                               Vector3Advanced(0, 1, 0);

        Vector3Advanced right = temp.cross(f).normalized();
        Vector3Advanced up = f.cross(right);

        OrthonormalBasis basis;
        basis.x = right;
        basis.y = up;
        basis.z = f;
        return basis;
    }

    // Check if basis is valid
    bool isValid(float epsilon = 1e-4f) const {
        // Check normalization
        if (!x.isNormalized(epsilon) ||
            !y.isNormalized(epsilon) ||
            !z.isNormalized(epsilon)) {
            return false;
        }

        // Check orthogonality
        if (std::abs(x.dot(y)) > epsilon ||
            std::abs(y.dot(z)) > epsilon ||
            std::abs(z.dot(x)) > epsilon) {
            return false;
        }

        return true;
    }

    // Transform vector from world space to this basis
    Vector3Advanced worldToLocal(const Vector3Advanced& worldVec) const {
        return Vector3Advanced(
            worldVec.dot(x),
            worldVec.dot(y),
            worldVec.dot(z)
        );
    }

    // Transform vector from this basis to world space
    Vector3Advanced localToWorld(const Vector3Advanced& localVec) const {
        return x * localVec.x + y * localVec.y + z * localVec.z;
    }
};
```

## Gram-Schmidt Orthogonalization

### What is Gram-Schmidt?

Gram-Schmidt is an algorithm that takes a set of linearly independent vectors and produces an orthonormal basis.

### Algorithm

Given vectors **v₁**, **v₂**, **v₃**:

```
1. u₁ = v₁ / |v₁|
2. u₂ = (v₂ - (v₂ · u₁)u₁) / |v₂ - (v₂ · u₁)u₁|
3. u₃ = v₃ - (v₃ · u₁)u₁ - (v₃ · u₂)u₂
   u₃ = u₃ / |u₃|
```

### Applications

- Create camera coordinate systems
- Orthogonalize rotation matrices
- Texture coordinate generation
- Physics constraint systems

### C++ Implementation

```cpp
class Vector3Advanced {
public:
    // ... previous methods ...

    // Gram-Schmidt orthogonalization for 3 vectors
    static void gramSchmidt(const Vector3Advanced& v1,
                           const Vector3Advanced& v2,
                           const Vector3Advanced& v3,
                           Vector3Advanced& out1,
                           Vector3Advanced& out2,
                           Vector3Advanced& out3) {
        // First vector - just normalize
        out1 = v1.normalized();

        // Second vector - remove component along first, then normalize
        out2 = v2 - v2.projectOnto(out1);
        out2.normalize();

        // Third vector - remove components along first two, then normalize
        out3 = v3 - v3.projectOnto(out1) - v3.projectOnto(out2);
        out3.normalize();
    }

    // Gram-Schmidt for 2 vectors (generate third automatically)
    static void gramSchmidt2D(const Vector3Advanced& v1,
                             const Vector3Advanced& v2,
                             Vector3Advanced& out1,
                             Vector3Advanced& out2,
                             Vector3Advanced& out3) {
        out1 = v1.normalized();
        out2 = v2 - v2.projectOnto(out1);
        out2.normalize();
        out3 = out1.cross(out2);  // Automatically orthogonal and normalized
    }
};
```

## Complete Advanced Vector Library

Here's a comprehensive example program demonstrating all advanced operations:

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// Complete Vector3Advanced class (combining all previous code)
class Vector3Advanced {
public:
    float x, y, z;

    // Constructors
    Vector3Advanced() : x(0), y(0), z(0) {}
    Vector3Advanced(float x, float y, float z) : x(x), y(y), z(z) {}

    // Basic operations (from Lesson 02)
    Vector3Advanced operator+(const Vector3Advanced& o) const { return Vector3Advanced(x+o.x, y+o.y, z+o.z); }
    Vector3Advanced operator-(const Vector3Advanced& o) const { return Vector3Advanced(x-o.x, y-o.y, z-o.z); }
    Vector3Advanced operator*(float s) const { return Vector3Advanced(x*s, y*s, z*s); }
    Vector3Advanced operator/(float s) const { return Vector3Advanced(x/s, y/s, z/s); }

    float dot(const Vector3Advanced& o) const { return x*o.x + y*o.y + z*o.z; }
    Vector3Advanced cross(const Vector3Advanced& o) const {
        return Vector3Advanced(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x);
    }

    float magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    float magnitudeSquared() const { return x*x + y*y + z*z; }

    Vector3Advanced normalized() const {
        float mag = magnitude();
        if (mag < 1e-8f) throw std::runtime_error("Cannot normalize zero vector");
        return *this / mag;
    }

    void normalize() {
        float mag = magnitude();
        if (mag < 1e-8f) throw std::runtime_error("Cannot normalize zero vector");
        *this = *this / mag;
    }

    bool isNormalized(float eps = 1e-5f) const {
        return std::abs(magnitudeSquared() - 1.0f) < eps;
    }

    // Projection and rejection
    Vector3Advanced projectOnto(const Vector3Advanced& target) const {
        float targetLenSq = target.magnitudeSquared();
        if (targetLenSq < 1e-8f) throw std::runtime_error("Cannot project onto zero vector");
        return target * (this->dot(target) / targetLenSq);
    }

    Vector3Advanced rejectFrom(const Vector3Advanced& target) const {
        return *this - projectOnto(target);
    }

    // Reflection and refraction
    Vector3Advanced reflect(const Vector3Advanced& normal) const {
        return *this - normal * (2.0f * this->dot(normal));
    }

    Vector3Advanced refract(const Vector3Advanced& normal, float eta) const {
        float cosI = -this->dot(normal);
        float sinT2 = eta * eta * (1.0f - cosI * cosI);
        if (sinT2 > 1.0f) return Vector3Advanced(0, 0, 0);
        float cosT = std::sqrt(1.0f - sinT2);
        return (*this * eta) + (normal * (eta * cosI - cosT));
    }

    // Angles
    float angleTo(const Vector3Advanced& other) const {
        float lenProd = magnitude() * other.magnitude();
        if (lenProd < 1e-8f) return 0.0f;
        float cosAngle = std::max(-1.0f, std::min(1.0f, dot(other) / lenProd));
        return std::acos(cosAngle);
    }

    float angleToInDegrees(const Vector3Advanced& other) const {
        return angleTo(other) * 180.0f / 3.14159265358979323846f;
    }

    // Interpolation
    static Vector3Advanced lerp(const Vector3Advanced& a, const Vector3Advanced& b, float t) {
        return a + (b - a) * t;
    }

    static Vector3Advanced slerp(const Vector3Advanced& a, const Vector3Advanced& b, float t) {
        float dot = a.dot(b);
        dot = std::max(-1.0f, std::min(1.0f, dot));
        float theta = std::acos(dot);
        if (theta < 1e-6f) return lerp(a, b, t);
        float sinTheta = std::sin(theta);
        float wa = std::sin((1.0f - t) * theta) / sinTheta;
        float wb = std::sin(t * theta) / sinTheta;
        return a * wa + b * wb;
    }

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vector3Advanced& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
        return os;
    }
};

// Demonstration programs
void demonstrateProjection() {
    std::cout << "=== Vector Projection ===" << std::endl << std::endl;

    Vector3Advanced velocity(10, 5, 0);
    Vector3Advanced forward(1, 0, 0);

    Vector3Advanced forwardSpeed = velocity.projectOnto(forward);
    Vector3Advanced sidewaysSpeed = velocity.rejectFrom(forward);

    std::cout << "Velocity: " << velocity << std::endl;
    std::cout << "Forward direction: " << forward << std::endl;
    std::cout << "Forward component: " << forwardSpeed << std::endl;
    std::cout << "Sideways component: " << sidewaysSpeed << std::endl;
    std::cout << "Verification: " << forwardSpeed + sidewaysSpeed << std::endl << std::endl;
}

void demonstrateReflection() {
    std::cout << "=== Vector Reflection ===" << std::endl << std::endl;

    Vector3Advanced incoming(-1, -1, 0);
    incoming.normalize();
    Vector3Advanced normal(0, 1, 0);

    Vector3Advanced reflected = incoming.reflect(normal);

    std::cout << "Incoming: " << incoming << std::endl;
    std::cout << "Surface normal: " << normal << std::endl;
    std::cout << "Reflected: " << reflected << std::endl;
    std::cout << "Angle in = " << incoming.angleToInDegrees(-normal) << " degrees" << std::endl;
    std::cout << "Angle out = " << reflected.angleToInDegrees(normal) << " degrees" << std::endl << std::endl;
}

void demonstrateInterpolation() {
    std::cout << "=== Advanced Interpolation ===" << std::endl << std::endl;

    Vector3Advanced start(1, 0, 0);
    Vector3Advanced end(0, 1, 0);

    std::cout << "Lerp vs Slerp (90 degree rotation):" << std::endl;
    std::cout << std::fixed << std::setprecision(4);

    for (float t = 0; t <= 1.0f; t += 0.25f) {
        Vector3Advanced lerped = Vector3Advanced::lerp(start, end, t);
        Vector3Advanced slerped = Vector3Advanced::slerp(start, end, t);

        std::cout << "t=" << t << ": ";
        std::cout << "Lerp=" << lerped << " (len=" << lerped.magnitude() << "), ";
        std::cout << "Slerp=" << slerped << " (len=" << slerped.magnitude() << ")" << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateOrthonormalization() {
    std::cout << "=== Gram-Schmidt Orthonormalization ===" << std::endl << std::endl;

    Vector3Advanced v1(1, 1, 0);
    Vector3Advanced v2(0, 1, 1);
    Vector3Advanced v3(1, 0, 1);

    std::cout << "Original vectors:" << std::endl;
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "v3: " << v3 << std::endl << std::endl;

    Vector3Advanced u1, u2, u3;
    Vector3Advanced::gramSchmidt(v1, v2, v3, u1, u2, u3);

    std::cout << "Orthonormal basis:" << std::endl;
    std::cout << "u1: " << u1 << " (length=" << u1.magnitude() << ")" << std::endl;
    std::cout << "u2: " << u2 << " (length=" << u2.magnitude() << ")" << std::endl;
    std::cout << "u3: " << u3 << " (length=" << u3.magnitude() << ")" << std::endl << std::endl;

    std::cout << "Orthogonality check:" << std::endl;
    std::cout << "u1 · u2 = " << u1.dot(u2) << std::endl;
    std::cout << "u2 · u3 = " << u2.dot(u3) << std::endl;
    std::cout << "u3 · u1 = " << u3.dot(u1) << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    try {
        demonstrateProjection();
        demonstrateReflection();
        demonstrateInterpolation();
        demonstrateOrthonormalization();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## Real-World Applications

### Lighting Calculation

```cpp
// Calculate diffuse lighting using projection
float calculateDiffuseLighting(const Vector3Advanced& lightDir,
                                const Vector3Advanced& surfaceNormal) {
    // Light contribution is projection of light onto normal
    float intensity = lightDir.dot(surfaceNormal);
    return std::max(0.0f, intensity);  // Clamp to positive
}
```

### Sliding on Slopes

```cpp
// Make character slide along slope instead of penetrating
Vector3Advanced slideAlongSurface(const Vector3Advanced& velocity,
                                   const Vector3Advanced& surfaceNormal) {
    // Remove component of velocity into surface
    return velocity.rejectFrom(surfaceNormal);
}
```

### Camera Orientation

```cpp
// Create camera basis from forward vector
OrthonormalBasis createCameraBasis(const Vector3Advanced& forward) {
    Vector3Advanced f = forward.normalized();
    Vector3Advanced worldUp(0, 1, 0);

    Vector3Advanced right = worldUp.cross(f).normalized();
    Vector3Advanced up = f.cross(right);

    OrthonormalBasis basis;
    basis.x = right;
    basis.y = up;
    basis.z = f;
    return basis;
}
```

## Key Takeaways

1. **Projection** extracts the component of a vector in a specific direction
2. **Rejection** gives the perpendicular component
3. **Reflection** bounces vectors off surfaces (angle in = angle out)
4. **Refraction** bends vectors passing through media boundaries
5. **Slerp** provides constant angular velocity interpolation
6. **Orthonormal bases** define custom coordinate systems
7. **Gram-Schmidt** creates orthonormal bases from arbitrary vectors
8. **Advanced operations** enable realistic lighting, physics, and rendering

## Exercises

1. Implement a `projectOntoPlane` function that projects a vector onto a plane
2. Create a function that calculates specular reflection for Phong lighting
3. Write a `rotateAround` function that rotates a vector around an arbitrary axis
4. Implement Bezier curve interpolation using vectors
5. Create a function to calculate the closest point on a line segment to a point

## What's Next?

In **Lesson 04**, we'll explore dot and cross products in extreme detail:
- Geometric interpretations
- Physical applications
- Triple products
- Applications in graphics and physics
- Optimization techniques

---

**Lesson 03 Complete** - Words: ~5,500 | Advanced Operations: 15+ | Next: Lesson 04 - Dot Product and Cross Product
