# Lesson 56: Vector Operations

## Course Information
- **Module**: Module 6 - 3D Graphics Mathematics
- **Lesson**: 56 of 64
- **Estimated Completion Time**: 4-5 hours
- **Difficulty**: Intermediate to Advanced

## Table of Contents
1. [Introduction](#introduction)
2. [Dot Product (Scalar Product)](#dot-product)
3. [Cross Product (Vector Product)](#cross-product)
4. [Vector Projection](#projection)
5. [Angle Between Vectors](#angles)
6. [Distance Formulas](#distances)
7. [Reflection and Bouncing](#reflection)
8. [Practical Applications](#applications)
9. [Code Examples Overview](#code-examples)
10. [Exercises](#exercises)

---

## 1. Introduction {#introduction}

Vector operations are essential mathematical tools for 3D graphics programming. In this lesson, we'll explore advanced vector operations that enable:
- Lighting calculations (dot product)
- Surface normals (cross product)
- Angle calculations
- Projection and rejection
- Reflection physics
- Collision response

### Key Operations Covered

1. **Dot Product**: Measures alignment between vectors
2. **Cross Product**: Creates perpendicular vector
3. **Projection**: Component of one vector along another
4. **Angle Calculation**: Finding angles between vectors
5. **Reflection**: Bouncing vectors off surfaces

---

## 2. Dot Product (Scalar Product) {#dot-product}

### Definition

The **dot product** of two vectors is a scalar (single number) that measures how much two vectors point in the same direction.

**Formula:**
```
v · w = vₓwₓ + vᵧwᵧ + vᵤwᵤ
```

**Alternative Formula:**
```
v · w = ||v|| ||w|| cos(θ)
```
where θ is the angle between the vectors.

### Properties

1. **Commutative**: v · w = w · v
2. **Distributive**: v · (w + u) = v · w + v · u
3. **Scalar Multiplication**: (kv) · w = k(v · w)
4. **Self Dot**: v · v = ||v||²

### Geometric Interpretation

```
v · w > 0  →  Vectors point in similar direction (angle < 90°)
v · w = 0  →  Vectors are perpendicular (angle = 90°)
v · w < 0  →  Vectors point in opposite directions (angle > 90°)
```

### Implementation

```cpp
float DotProduct(const Vector3& v, const Vector3& w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}
```

### Applications

**1. Lighting Calculations:**
```cpp
float intensity = max(0, normal.Dot(lightDirection));
```

**2. Testing If Facing:**
```cpp
Vector3 toTarget = target - position;
bool isFacing = forward.Dot(toTarget) > 0;
```

**3. Speed in Direction:**
```cpp
float speedForward = velocity.Dot(forward);
```

---

## 3. Cross Product (Vector Product) {#cross-product}

### Definition

The **cross product** of two vectors produces a third vector perpendicular to both input vectors.

**Formula:**
```
v × w = (vᵧwᵤ - vᵤwᵧ, vᵤwₓ - vₓwᵤ, vₓwᵧ - vᵧwₓ)
```

**Magnitude:**
```
||v × w|| = ||v|| ||w|| sin(θ)
```

### Properties

1. **Anti-commutative**: v × w = -(w × v)
2. **NOT Associative**: (v × w) × u ≠ v × (w × u)
3. **Distributive**: v × (w + u) = v × w + v × u
4. **Self Cross**: v × v = 0

### Right-Hand Rule

Point your right hand's fingers along v, curl them toward w. Your thumb points in the direction of v × w.

### Implementation

```cpp
Vector3 CrossProduct(const Vector3& v, const Vector3& w) {
    return Vector3(
        v.y * w.z - v.z * w.y,
        v.z * w.x - v.x * w.z,
        v.x * w.y - v.y * w.x
    );
}
```

### Applications

**1. Surface Normals:**
```cpp
// Triangle with vertices a, b, c
Vector3 edge1 = b - a;
Vector3 edge2 = c - a;
Vector3 normal = edge1.Cross(edge2).Normalized();
```

**2. Coordinate System Construction:**
```cpp
Vector3 forward = /* given */;
Vector3 up = Vector3(0, 1, 0);
Vector3 right = forward.Cross(up).Normalized();
Vector3 trueUp = right.Cross(forward).Normalized();
```

**3. Torque and Angular Velocity:**
```cpp
Vector3 torque = leverArm.Cross(force);
```

---

## 4. Vector Projection {#projection}

### Definition

**Projection** of vector v onto vector w is the component of v in the direction of w.

**Formula:**
```
proj_w(v) = ((v · w) / ||w||²) * w
```

**For unit vector w:**
```
proj_w(v) = (v · w) * w
```

### Rejection (Perpendicular Component)

The **rejection** is the component of v perpendicular to w:
```
rej_w(v) = v - proj_w(v)
```

### Visualization

```
v = proj_w(v) + rej_w(v)

      v
     /|
    / |
   /  | rej_w(v)
  /   |
 /____|_____> w
 proj_w(v)
```

### Applications

**1. Distance from Point to Line:**
```cpp
float distance = rejection.Length();
```

**2. Sliding Along Surface:**
```cpp
Vector3 movement = /* player input */;
Vector3 slideMovement = movement - movement.Dot(normal) * normal;
```

**3. Shadow Projection:**
```cpp
Vector3 shadow = position - position.Dot(groundNormal) * groundNormal;
```

---

## 5. Angle Between Vectors {#angles}

### Formula

```
cos(θ) = (v · w) / (||v|| ||w||)
θ = arccos((v · w) / (||v|| ||w||))
```

### Safe Implementation

```cpp
float AngleBetween(const Vector3& v, const Vector3& w) {
    float dot = v.Dot(w);
    float lengths = v.Length() * w.Length();

    if (lengths == 0) return 0;

    float cosAngle = dot / lengths;
    // Clamp to avoid numerical errors
    cosAngle = Clamp(cosAngle, -1.0f, 1.0f);

    return std::acos(cosAngle);
}
```

### Signed Angle (2D)

For vectors in a plane with normal n:
```cpp
float SignedAngle(const Vector3& v, const Vector3& w, const Vector3& n) {
    float angle = AngleBetween(v, w);
    Vector3 cross = v.Cross(w);
    if (cross.Dot(n) < 0) angle = -angle;
    return angle;
}
```

---

## 6. Distance Formulas {#distances}

### Point to Point
```cpp
float distance = (p2 - p1).Length();
```

### Point to Line
```cpp
// Line defined by point p and direction d
Vector3 toPoint = point - p;
Vector3 rejection = toPoint - toPoint.Dot(d) * d;
float distance = rejection.Length();
```

### Point to Plane
```cpp
// Plane defined by point p and normal n
float distance = abs((point - p).Dot(n));
```

### Point to Line Segment
```cpp
Vector3 ab = b - a;
float t = Clamp((point - a).Dot(ab) / ab.LengthSquared(), 0.0f, 1.0f);
Vector3 closest = a + ab * t;
float distance = (point - closest).Length();
```

---

## 7. Reflection and Bouncing {#reflection}

### Reflection Formula

Reflect vector v across surface with normal n:
```
r = v - 2(v · n)n
```

**For unit normal:**
```cpp
Vector3 Reflect(const Vector3& v, const Vector3& n) {
    return v - n * (2.0f * v.Dot(n));
}
```

### Applications

**1. Mirror Reflection:**
```cpp
Vector3 reflected = Reflect(incoming, surfaceNormal);
```

**2. Bounce Physics:**
```cpp
Vector3 velocity = /* current velocity */;
Vector3 normal = /* collision normal */;
Vector3 newVelocity = Reflect(velocity, normal) * restitution;
```

**3. Light Reflection:**
```cpp
Vector3 lightReflect = Reflect(-lightDir, normal);
float spec = pow(max(0, viewDir.Dot(lightReflect)), shininess);
```

---

## 8. Practical Applications {#applications}

### Lighting (Lambertian)

```cpp
float diffuse = max(0.0f, normal.Dot(lightDirection));
Color finalColor = diffuse * lightColor * surfaceColor;
```

### Backface Culling

```cpp
Vector3 toCamera = cameraPos - trianglePos;
bool isFrontFacing = triangleNormal.Dot(toCamera) > 0;
```

### Field of View Check

```cpp
Vector3 toTarget = (target - camera).Normalized();
Vector3 forward = camera.Forward();
float dot = forward.Dot(toTarget);
bool inFOV = (dot > cos(fovAngle / 2));
```

### Orbit Camera

```cpp
// Rotate camera around target
Vector3 offset = cameraPos - targetPos;
// Rotate offset using angle...
cameraPos = targetPos + rotatedOffset;
```

---

## 9. Code Examples Overview {#code-examples}

The `Code/` folder contains 15 complete examples:

1. **01_DotProduct.cpp** - Basic dot product calculations
2. **02_DotProductApplications.cpp** - Lighting, angles, projection length
3. **03_CrossProduct.cpp** - Basic cross product calculations
4. **04_CrossProductApplications.cpp** - Surface normals, coordinate systems
5. **05_VectorProjection.cpp** - Projection and rejection
6. **06_AngleCalculations.cpp** - Angles between vectors
7. **07_SignedAngles.cpp** - 2D signed angles
8. **08_VectorReflection.cpp** - Reflection across surfaces
9. **09_DistanceFormulas.cpp** - Point to line, plane, segment
10. **10_LightingCalculations.cpp** - Diffuse and specular lighting
11. **11_BackfaceCulling.cpp** - Face orientation testing
12. **12_ProjectionRejection.cpp** - Advanced projection/rejection
13. **13_TripleProducts.cpp** - Scalar and vector triple products
14. **14_GramSchmidt.cpp** - Orthogonalization
15. **15_PracticalApplications.cpp** - Combined real-world examples

---

## 10. Exercises {#exercises}

### Exercise 1: Dot Product
```cpp
Vector3 v1(1, 0, 0);
Vector3 v2(1, 1, 0);
// Calculate v1 · v2
// Expected: 1
```

### Exercise 2: Cross Product
```cpp
Vector3 v1(1, 0, 0);
Vector3 v2(0, 1, 0);
// Calculate v1 × v2
// Expected: (0, 0, 1) in right-handed system
```

### Exercise 3: Angle Calculation
```cpp
Vector3 v1(1, 0, 0);
Vector3 v2(0, 1, 0);
// Calculate angle between them
// Expected: 90° (π/2 radians)
```

### Exercise 4: Surface Normal
```cpp
// Triangle vertices
Vector3 a(0, 0, 0);
Vector3 b(1, 0, 0);
Vector3 c(0, 1, 0);
// Calculate surface normal
```

### Exercise 5: Reflection
```cpp
Vector3 incoming(1, -1, 0);  // 45° downward
Vector3 normal(0, 1, 0);      // Floor normal
// Calculate reflected vector
// Expected: (1, 1, 0) - 45° upward
```

---

## Summary

In this lesson, you learned:

1. **Dot Product**
   - Measures vector alignment
   - Returns scalar value
   - Applications: lighting, angles, projections

2. **Cross Product**
   - Creates perpendicular vector
   - Right-hand rule for direction
   - Applications: normals, coordinate systems

3. **Projection**
   - Component along another vector
   - Rejection is perpendicular component
   - Applications: sliding, shadows

4. **Angles**
   - Calculate from dot product
   - Handle numerical precision
   - Signed angles in 2D

5. **Reflection**
   - Bounce vectors off surfaces
   - Essential for physics and rendering

### Next Steps

In **Lesson 57: Matrices & Matrix Operations**, we'll explore:
- Matrix representation
- Matrix multiplication
- Identity, transpose, inverse
- Matrix-vector multiplication

---

**End of Lesson 56**
