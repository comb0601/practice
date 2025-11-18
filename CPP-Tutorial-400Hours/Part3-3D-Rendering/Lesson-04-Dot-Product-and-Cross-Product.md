# Lesson 04: Dot Product and Cross Product

## Overview
The dot product and cross product are two of the most important operations in 3D graphics mathematics. These operations appear everywhere: lighting calculations, collision detection, determining angles, finding perpendicular vectors, and much more. This lesson provides an exhaustive exploration of both products, their geometric meanings, mathematical properties, and practical applications with complete C++ implementations.

## Table of Contents
1. [Dot Product Fundamentals](#dot-product-fundamentals)
2. [Geometric Interpretation of Dot Product](#geometric-interpretation-of-dot-product)
3. [Dot Product Applications](#dot-product-applications)
4. [Cross Product Fundamentals](#cross-product-fundamentals)
5. [Geometric Interpretation of Cross Product](#geometric-interpretation-of-cross-product)
6. [Cross Product Applications](#cross-product-applications)
7. [Triple Products](#triple-products)
8. [Complete Implementation](#complete-implementation)
9. [Performance Optimization](#performance-optimization)
10. [Real-World Examples](#real-world-examples)

## Dot Product Fundamentals

### Mathematical Definition

The dot product (also called scalar product or inner product) of two vectors **a** and **b** is:

```
a · b = ax*bx + ay*by + az*bz
```

The result is a **scalar** (single number), not a vector.

### Alternative Formula

```
a · b = |a| |b| cos(θ)

where:
|a| = magnitude of vector a
|b| = magnitude of vector b
θ = angle between the vectors
```

This formula reveals the geometric meaning of the dot product.

### Properties

1. **Commutative**: a · b = b · a
2. **Distributive**: a · (b + c) = a · b + a · c
3. **Scalar multiplication**: (ka) · b = k(a · b) = a · (kb)
4. **Self dot product**: a · a = |a|²
5. **Zero vector**: a · 0 = 0

### C++ Basic Implementation

```cpp
class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Dot product
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Alternative syntax
    float operator*(const Vector3& other) const {
        return dot(other);
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        float mag = magnitude();
        return (mag > 1e-8f) ? Vector3(x/mag, y/mag, z/mag) : Vector3(0,0,0);
    }
};
```

## Geometric Interpretation of Dot Product

### Projection Length

The dot product gives the length of the projection of one vector onto another:

```
projection_length = a · b̂
where b̂ is the unit vector in direction of b
```

### Angle Detection

The sign of the dot product reveals the angle relationship:

```
a · b > 0  →  angle < 90° (acute)
a · b = 0  →  angle = 90° (perpendicular/orthogonal)
a · b < 0  →  angle > 90° (obtuse)
```

Visual representation:

```
Acute angle (a·b > 0):     Right angle (a·b = 0):    Obtuse angle (a·b < 0):
      a                          a                         a
       \                         |                        /
        \                        |                       /
         \  θ<90°               |  θ=90°               /  θ>90°
          \                     |                     /
           → b                  → b                  → b
```

### Similarity Measurement

For normalized vectors:
```
a · b = cos(θ)

Values range from -1 to 1:
 1.0: vectors point in same direction
 0.0: vectors are perpendicular
-1.0: vectors point in opposite directions
```

## Dot Product Applications

### 1. Calculate Angle Between Vectors

```cpp
float angleBetween(const Vector3& a, const Vector3& b) {
    float dotProduct = a.dot(b);
    float magnitudes = a.magnitude() * b.magnitude();

    if (magnitudes < 1e-8f) {
        return 0.0f;  // Handle zero vectors
    }

    float cosTheta = dotProduct / magnitudes;
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));  // Clamp for numerical safety

    return std::acos(cosTheta);  // Returns radians
}

float angleInDegrees(const Vector3& a, const Vector3& b) {
    return angleBetween(a, b) * 180.0f / 3.14159265358979323846f;
}
```

### 2. Test if Vectors are Perpendicular

```cpp
bool arePerpendicular(const Vector3& a, const Vector3& b, float epsilon = 1e-5f) {
    return std::abs(a.dot(b)) < epsilon;
}

bool areParallel(const Vector3& a, const Vector3& b, float epsilon = 1e-5f) {
    Vector3 aNorm = a.normalized();
    Vector3 bNorm = b.normalized();
    float dotProd = std::abs(aNorm.dot(bNorm));
    return std::abs(dotProd - 1.0f) < epsilon;
}
```

### 3. Project Vector onto Another

```cpp
Vector3 projectOnto(const Vector3& vector, const Vector3& target) {
    float targetLengthSquared = target.dot(target);

    if (targetLengthSquared < 1e-8f) {
        throw std::runtime_error("Cannot project onto zero vector");
    }

    float projectionLength = vector.dot(target) / targetLengthSquared;
    return target * projectionLength;
}

// For unit vectors (faster)
Vector3 projectOntoNormalized(const Vector3& vector, const Vector3& unitTarget) {
    float projectionLength = vector.dot(unitTarget);
    return unitTarget * projectionLength;
}
```

### 4. Lighting Calculations (Lambert's Law)

```cpp
// Calculate diffuse lighting intensity
float calculateDiffuseLighting(const Vector3& lightDirection,
                                const Vector3& surfaceNormal) {
    // Both should be normalized
    float intensity = lightDirection.dot(surfaceNormal);

    // Clamp to [0, 1] - negative means light is behind surface
    return std::max(0.0f, intensity);
}

// Complete diffuse lighting with color
Vector3 calculateDiffuseColor(const Vector3& lightDir,
                               const Vector3& surfaceNormal,
                               const Vector3& lightColor,
                               const Vector3& surfaceColor) {
    float intensity = std::max(0.0f, lightDir.dot(surfaceNormal));

    return Vector3(
        surfaceColor.x * lightColor.x * intensity,
        surfaceColor.y * lightColor.y * intensity,
        surfaceColor.z * lightColor.z * intensity
    );
}
```

### 5. Back-Face Culling

```cpp
// Determine if a polygon faces the camera
bool isFacingCamera(const Vector3& polygonNormal,
                    const Vector3& viewDirection) {
    // If dot product is negative, polygon faces camera
    return polygonNormal.dot(viewDirection) < 0;
}

// Alternative: check against camera position
bool isFacingPoint(const Vector3& polygonNormal,
                   const Vector3& polygonPosition,
                   const Vector3& viewPoint) {
    Vector3 toView = (viewPoint - polygonPosition).normalized();
    return polygonNormal.dot(toView) > 0;
}
```

### 6. Distance to Plane

```cpp
// Calculate signed distance from point to plane
float distanceToPlane(const Vector3& point,
                      const Vector3& planeNormal,
                      const Vector3& planePoint) {
    Vector3 pointToPlane = point - planePoint;
    return pointToPlane.dot(planeNormal);
}

// Classify point relative to plane
enum PlaneClassification {
    FRONT,   // Positive side
    BACK,    // Negative side
    ON_PLANE // On the plane
};

PlaneClassification classifyPoint(const Vector3& point,
                                   const Vector3& planeNormal,
                                   const Vector3& planePoint,
                                   float epsilon = 1e-5f) {
    float distance = distanceToPlane(point, planeNormal, planePoint);

    if (distance > epsilon) return FRONT;
    if (distance < -epsilon) return BACK;
    return ON_PLANE;
}
```

## Cross Product Fundamentals

### Mathematical Definition

The cross product (also called vector product) of two vectors **a** and **b** is:

```
a × b = ⟨ay*bz - az*by,
         az*bx - ax*bz,
         ax*by - ay*bx⟩
```

The result is a **vector**, not a scalar.

### Alternative Formula

```
|a × b| = |a| |b| sin(θ)

Direction: perpendicular to both a and b (right-hand rule)
```

### Properties

1. **Anti-commutative**: a × b = -(b × a)
2. **Not associative**: a × (b × c) ≠ (a × b) × c
3. **Distributive**: a × (b + c) = a × b + a × c
4. **Scalar multiplication**: (ka) × b = k(a × b) = a × (kb)
5. **Parallel vectors**: a × a = 0
6. **Perpendicular result**: (a × b) · a = 0 and (a × b) · b = 0

### Right-Hand Rule

Point your right hand's fingers in the direction of **a**, curl them toward **b**, and your thumb points in the direction of **a × b**.

```
        a × b (thumb)
           ↑
           |
    a ----+---→ b
   (fingers)  (curl)
```

### C++ Implementation

```cpp
class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Cross product
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Alternative: operator overload
    Vector3 operator^(const Vector3& other) const {
        return cross(other);
    }
};
```

## Geometric Interpretation of Cross Product

### Magnitude as Area

The magnitude of the cross product equals the area of the parallelogram formed by the two vectors:

```
Area = |a × b| = |a| |b| sin(θ)

Visual:
    b
   /|
  / |
 /  | height = |b|sin(θ)
/__θ|
a

Area = base * height = |a| * |b|sin(θ)
```

### Triangle Area

Half the parallelogram area gives the triangle area:

```
Triangle area = |a × b| / 2
```

### Direction and Orientation

The cross product vector points perpendicular to the plane containing **a** and **b**. The direction follows the right-hand rule.

## Cross Product Applications

### 1. Calculate Surface Normal

```cpp
// Calculate normal for a triangle with vertices A, B, C
Vector3 calculateTriangleNormal(const Vector3& A,
                                const Vector3& B,
                                const Vector3& C) {
    Vector3 AB = B - A;
    Vector3 AC = C - A;

    Vector3 normal = AB.cross(AC);
    return normal.normalized();
}

// Calculate normal for a quad (assumes coplanar vertices)
Vector3 calculateQuadNormal(const Vector3& A,
                            const Vector3& B,
                            const Vector3& C,
                            const Vector3& D) {
    // Use diagonals for better accuracy
    Vector3 diagonal1 = C - A;
    Vector3 diagonal2 = D - B;

    Vector3 normal = diagonal1.cross(diagonal2);
    return normal.normalized();
}
```

### 2. Test if Vectors are Parallel

```cpp
bool areVectorsParallel(const Vector3& a, const Vector3& b, float epsilon = 1e-5f) {
    Vector3 cross = a.cross(b);
    return cross.magnitude() < epsilon;
}

// Check if three points are collinear
bool arePointsCollinear(const Vector3& A,
                        const Vector3& B,
                        const Vector3& C,
                        float epsilon = 1e-5f) {
    Vector3 AB = B - A;
    Vector3 AC = C - A;
    return areVectorsParallel(AB, AC, epsilon);
}
```

### 3. Calculate Triangle Area

```cpp
float calculateTriangleArea(const Vector3& A,
                            const Vector3& B,
                            const Vector3& C) {
    Vector3 AB = B - A;
    Vector3 AC = C - A;
    Vector3 cross = AB.cross(AC);

    return cross.magnitude() * 0.5f;
}

// Calculate area of a polygon (assumes planar and convex)
float calculatePolygonArea(const std::vector<Vector3>& vertices) {
    if (vertices.size() < 3) return 0.0f;

    float totalArea = 0.0f;
    Vector3 reference = vertices[0];

    for (size_t i = 1; i < vertices.size() - 1; i++) {
        totalArea += calculateTriangleArea(reference, vertices[i], vertices[i + 1]);
    }

    return totalArea;
}
```

### 4. Create Perpendicular Vector

```cpp
// Find a vector perpendicular to the given vector
Vector3 findPerpendicular(const Vector3& v) {
    // Choose a vector that's unlikely to be parallel
    Vector3 candidate = (std::abs(v.x) < 0.9f) ?
                        Vector3(1, 0, 0) :
                        Vector3(0, 1, 0);

    return v.cross(candidate).normalized();
}

// Create orthonormal basis from one vector
void createOrthonormalBasis(const Vector3& normal,
                            Vector3& outTangent,
                            Vector3& outBitangent) {
    outTangent = findPerpendicular(normal);
    outBitangent = normal.cross(outTangent);
    // All three are now mutually perpendicular and normalized
}
```

### 5. Determine Winding Order

```cpp
// Check if triangle vertices are in counter-clockwise order
// when viewed from the direction of the normal
bool isCounterClockwise(const Vector3& A,
                        const Vector3& B,
                        const Vector3& C,
                        const Vector3& viewDirection) {
    Vector3 AB = B - A;
    Vector3 AC = C - A;
    Vector3 normal = AB.cross(AC);

    return normal.dot(viewDirection) > 0;
}

// Check if point P is to the left of line AB (2D test in 3D)
bool isLeftOfLine(const Vector3& A,
                  const Vector3& B,
                  const Vector3& P,
                  const Vector3& planeNormal) {
    Vector3 AB = B - A;
    Vector3 AP = P - A;
    Vector3 cross = AB.cross(AP);

    return cross.dot(planeNormal) > 0;
}
```

### 6. Torque and Rotation

```cpp
// Calculate torque = radius × force
Vector3 calculateTorque(const Vector3& radius, const Vector3& force) {
    return radius.cross(force);
}

// Calculate angular velocity from linear velocity
// omega = r × v / |r|²
Vector3 calculateAngularVelocity(const Vector3& radius,
                                 const Vector3& linearVelocity) {
    float radiusSquared = radius.magnitudeSquared();

    if (radiusSquared < 1e-8f) {
        return Vector3(0, 0, 0);
    }

    return radius.cross(linearVelocity) / radiusSquared;
}
```

## Triple Products

### Scalar Triple Product

The scalar triple product combines dot and cross products:

```
a · (b × c)

Geometric meaning: Signed volume of parallelepiped
```

### Properties

1. Cyclic permutation: a · (b × c) = b · (c × a) = c · (a × b)
2. Anti-cyclic: a · (b × c) = -a · (c × b)
3. Coplanar test: If result is 0, vectors are coplanar

### C++ Implementation

```cpp
float scalarTripleProduct(const Vector3& a, const Vector3& b, const Vector3& c) {
    return a.dot(b.cross(c));
}

// Test if three vectors are coplanar
bool areCoplanar(const Vector3& a, const Vector3& b, const Vector3& c,
                 float epsilon = 1e-5f) {
    return std::abs(scalarTripleProduct(a, b, c)) < epsilon;
}

// Calculate signed volume of tetrahedron
float tetrahedronVolume(const Vector3& A, const Vector3& B,
                        const Vector3& C, const Vector3& D) {
    Vector3 AB = B - A;
    Vector3 AC = C - A;
    Vector3 AD = D - A;

    return std::abs(scalarTripleProduct(AB, AC, AD)) / 6.0f;
}
```

### Vector Triple Product

```
a × (b × c)

Identity: a × (b × c) = b(a·c) - c(a·b)
```

### C++ Implementation

```cpp
Vector3 vectorTripleProduct(const Vector3& a, const Vector3& b, const Vector3& c) {
    return b * a.dot(c) - c * a.dot(b);
}

// Alternative (less efficient but more obvious)
Vector3 vectorTripleProductDirect(const Vector3& a, const Vector3& b, const Vector3& c) {
    return a.cross(b.cross(c));
}
```

## Complete Implementation

Here's a comprehensive Vector3 class with all dot and cross product functionality:

```cpp
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <stdexcept>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Basic arithmetic
    Vector3 operator+(const Vector3& o) const { return Vector3(x+o.x, y+o.y, z+o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    Vector3 operator/(float s) const { return Vector3(x/s, y/s, z/s); }

    // Dot product
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude operations
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3 normalized() const {
        float mag = magnitude();
        if (mag < 1e-8f) return Vector3(0, 0, 0);
        return *this / mag;
    }

    // Angle operations
    float angleTo(const Vector3& other) const {
        float mag = magnitude() * other.magnitude();
        if (mag < 1e-8f) return 0.0f;

        float cosAngle = dot(other) / mag;
        cosAngle = std::max(-1.0f, std::min(1.0f, cosAngle));
        return std::acos(cosAngle);
    }

    float angleToInDegrees(const Vector3& other) const {
        return angleTo(other) * 180.0f / 3.14159265358979323846f;
    }

    // Projection
    Vector3 projectOnto(const Vector3& target) const {
        float targetMagSq = target.magnitudeSquared();
        if (targetMagSq < 1e-8f) return Vector3(0, 0, 0);
        return target * (dot(target) / targetMagSq);
    }

    Vector3 rejectFrom(const Vector3& target) const {
        return *this - projectOnto(target);
    }

    // Tests
    bool isPerpendicular(const Vector3& other, float eps = 1e-5f) const {
        return std::abs(dot(other)) < eps;
    }

    bool isParallel(const Vector3& other, float eps = 1e-5f) const {
        return cross(other).magnitude() < eps;
    }

    // Triple products
    float scalarTriple(const Vector3& b, const Vector3& c) const {
        return dot(b.cross(c));
    }

    Vector3 vectorTriple(const Vector3& b, const Vector3& c) const {
        return b * dot(c) - c * dot(b);
    }

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
        return os;
    }
};

// Scalar * Vector
Vector3 operator*(float s, const Vector3& v) {
    return v * s;
}

// ========================================
// DEMONSTRATION PROGRAMS
// ========================================

void demonstrateDotProduct() {
    std::cout << "=== DOT PRODUCT DEMONSTRATIONS ===" << std::endl << std::endl;

    // Angle calculation
    std::cout << "--- Angle Calculation ---" << std::endl;
    Vector3 v1(1, 0, 0);
    Vector3 v2(1, 1, 0);
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "Angle: " << v1.angleToInDegrees(v2) << " degrees" << std::endl;
    std::cout << "Dot product: " << v1.dot(v2) << std::endl << std::endl;

    // Perpendicularity test
    std::cout << "--- Perpendicularity Test ---" << std::endl;
    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    Vector3 c(1, 1, 0);
    std::cout << "a: " << a << ", b: " << b << std::endl;
    std::cout << "Are perpendicular? " << (a.isPerpendicular(b) ? "Yes" : "No") << std::endl;
    std::cout << "a: " << a << ", c: " << c << std::endl;
    std::cout << "Are perpendicular? " << (a.isPerpendicular(c) ? "Yes" : "No") << std::endl << std::endl;

    // Lighting simulation
    std::cout << "--- Lighting Simulation ---" << std::endl;
    Vector3 lightDir(0, -1, 0);  // Light from above
    Vector3 normal1(0, 1, 0);    // Surface facing up
    Vector3 normal2(1, 0, 0);    // Surface facing right
    Vector3 normal3(0, -1, 0);   // Surface facing down

    std::cout << "Light direction: " << lightDir << std::endl;
    std::cout << "Surface facing up:    intensity = "
              << std::max(0.0f, -lightDir.dot(normal1)) << std::endl;
    std::cout << "Surface facing right: intensity = "
              << std::max(0.0f, -lightDir.dot(normal2)) << std::endl;
    std::cout << "Surface facing down:  intensity = "
              << std::max(0.0f, -lightDir.dot(normal3)) << std::endl;
}

void demonstrateCrossProduct() {
    std::cout << "\n=== CROSS PRODUCT DEMONSTRATIONS ===" << std::endl << std::endl;

    // Normal calculation
    std::cout << "--- Surface Normal Calculation ---" << std::endl;
    Vector3 A(0, 0, 0);
    Vector3 B(1, 0, 0);
    Vector3 C(0, 1, 0);

    Vector3 AB = B - A;
    Vector3 AC = C - A;
    Vector3 normal = AB.cross(AC).normalized();

    std::cout << "Triangle: " << A << ", " << B << ", " << C << std::endl;
    std::cout << "Normal: " << normal << std::endl << std::endl;

    // Area calculation
    std::cout << "--- Triangle Area ---" << std::endl;
    float area = AB.cross(AC).magnitude() * 0.5f;
    std::cout << "Area: " << area << std::endl << std::endl;

    // Perpendicular vector
    std::cout << "--- Creating Perpendicular Vectors ---" << std::endl;
    Vector3 v(1, 1, 0);
    Vector3 up(0, 0, 1);
    Vector3 perp = v.cross(up).normalized();
    std::cout << "Vector: " << v << std::endl;
    std::cout << "Perpendicular: " << perp << std::endl;
    std::cout << "Verify dot product = " << v.dot(perp) << " (should be ~0)" << std::endl;
}

void demonstrateTripleProducts() {
    std::cout << "\n=== TRIPLE PRODUCTS ===" << std::endl << std::endl;

    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    Vector3 c(0, 0, 1);

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "c: " << c << std::endl << std::endl;

    float scalarTriple = a.scalarTriple(b, c);
    std::cout << "Scalar triple product a·(b×c): " << scalarTriple << std::endl;
    std::cout << "Volume of parallelepiped: " << std::abs(scalarTriple) << std::endl << std::endl;

    Vector3 vectorTriple = a.vectorTriple(b, c);
    std::cout << "Vector triple product a×(b×c): " << vectorTriple << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    demonstrateDotProduct();
    demonstrateCrossProduct();
    demonstrateTripleProducts();

    return 0;
}
```

## Performance Optimization

### Dot Product Optimization

```cpp
// Standard
float dotProduct = a.x * b.x + a.y * b.y + a.z * b.z;

// With FMA (Fused Multiply-Add) if available
float dotProduct = std::fma(a.x, b.x, std::fma(a.y, b.y, a.z * b.z));

// SIMD (example with SSE)
#ifdef USE_SSE
__m128 va = _mm_set_ps(0, a.z, a.y, a.x);
__m128 vb = _mm_set_ps(0, b.z, b.y, b.x);
__m128 product = _mm_mul_ps(va, vb);
__m128 sum = _mm_hadd_ps(product, product);
sum = _mm_hadd_ps(sum, sum);
float result = _mm_cvtss_f32(sum);
#endif
```

### Cross Product Optimization

```cpp
// Standard implementation is already optimal for scalar code
// SIMD version is complex due to shuffling required

// Inline for small functions
inline Vector3 Vector3::cross(const Vector3& o) const {
    return Vector3(
        y * o.z - z * o.y,
        z * o.x - x * o.z,
        x * o.y - y * o.x
    );
}
```

## Real-World Examples

### Example 1: Point Inside Triangle Test

```cpp
bool isPointInTriangle(const Vector3& P,
                       const Vector3& A,
                       const Vector3& B,
                       const Vector3& C) {
    Vector3 normal = (B - A).cross(C - A);

    // Check each edge
    Vector3 edge1 = B - A;
    Vector3 cross1 = edge1.cross(P - A);
    if (normal.dot(cross1) < 0) return false;

    Vector3 edge2 = C - B;
    Vector3 cross2 = edge2.cross(P - B);
    if (normal.dot(cross2) < 0) return false;

    Vector3 edge3 = A - C;
    Vector3 cross3 = edge3.cross(P - C);
    if (normal.dot(cross3) < 0) return false;

    return true;
}
```

### Example 2: Reflect Vector Across Plane

```cpp
Vector3 reflectAcrossPlane(const Vector3& incident, const Vector3& normal) {
    // incident should point toward the surface
    return incident - normal * (2.0f * incident.dot(normal));
}
```

### Example 3: Calculate Centroid

```cpp
Vector3 calculateCentroid(const std::vector<Vector3>& points) {
    Vector3 sum(0, 0, 0);
    for (const auto& point : points) {
        sum = sum + point;
    }
    return sum / static_cast<float>(points.size());
}
```

## Key Takeaways

1. **Dot product returns a scalar**, useful for angles and projections
2. **Cross product returns a vector**, perpendicular to both inputs
3. **Dot product is commutative**, cross product is anti-commutative
4. **Dot product measures similarity**, cross product measures "perpendicularity"
5. **Use dot product for**: angles, lighting, projections, distance to planes
6. **Use cross product for**: normals, areas, perpendicular vectors, winding order
7. **Triple products** combine both operations for volume and coplanarity tests
8. **Normalize vectors** before angle calculations for numerical stability
9. **Cross product magnitude** equals parallelogram area
10. **Right-hand rule** determines cross product direction

## Exercises

1. Implement a function to find the closest point on a line segment to a given point
2. Create a function that rotates a vector around an arbitrary axis using cross products
3. Write a function to determine if four points form a planar quadrilateral
4. Implement specular lighting using dot product (Phong reflection model)
5. Create a function to calculate the dihedral angle between two planes

## What's Next?

In **Lesson 05**, we begin our journey into matrices:
- Matrix representation and notation
- Matrix types (identity, zero, diagonal)
- Basic matrix operations
- Matrix storage in memory
- Preparing for transformations

---

**Lesson 04 Complete** - Words: ~6,000 | Code Examples: 20+ functions | Next: Lesson 05 - Matrices Introduction and Basics
