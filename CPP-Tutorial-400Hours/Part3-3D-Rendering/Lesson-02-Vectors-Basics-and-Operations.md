# Lesson 02: Vectors - Basics and Operations

## Overview
Vectors are the fundamental building blocks of 3D graphics mathematics. Unlike points, which represent positions in space, vectors represent direction and magnitude. Understanding vectors deeply is essential for everything from moving objects to calculating lighting, from detecting collisions to implementing physics. This lesson provides a comprehensive exploration of vector mathematics with complete C++ implementations.

## Table of Contents
1. [What Are Vectors?](#what-are-vectors)
2. [Vector Representation](#vector-representation)
3. [Basic Vector Operations](#basic-vector-operations)
4. [Vector Properties](#vector-properties)
5. [Geometric Interpretations](#geometric-interpretations)
6. [Complete Vector3 Implementation](#complete-vector3-implementation)
7. [Practical Applications](#practical-applications)
8. [Performance Optimization](#performance-optimization)

## What Are Vectors?

### Mathematical Definition

In mathematics, a vector is a quantity that has both **magnitude** (size/length) and **direction**. Unlike scalars (single numbers), vectors capture directional information.

### Vectors vs Points

This distinction is crucial:

**Point** (Position):
- Represents a location in space
- Has coordinates: P = (x, y, z)
- Example: "The player is at position (10, 5, 0)"

**Vector** (Direction + Magnitude):
- Represents direction and distance
- Has components: V = ⟨x, y, z⟩
- Example: "The player moves ⟨2, 0, 0⟩ per second (2 units right)"

### Visual Representation

```
Point:                Vector:
    •                    →
Position in space    Arrow showing direction and length

Point at (3,2):      Vector ⟨3,2⟩ from origin:
    |                    |
  2 •                  2 |→
    |                    |
    +----+---           +----+
         3                   3
```

## Vector Representation

### Cartesian Coordinates

In 3D space, vectors have three components:
- **x-component**: Amount in the X direction
- **y-component**: Amount in the Y direction
- **z-component**: Amount in the Z direction

```
v = ⟨vx, vy, vz⟩
```

### Column Vector Notation

In linear algebra, vectors are often written as column matrices:

```
     [vx]
v =  [vy]
     [vz]
```

### Magnitude and Direction

Any vector can be described by:
1. **Magnitude** (length): ||v|| = √(vx² + vy² + vz²)
2. **Direction**: Unit vector pointing in the same direction

```
v = ||v|| * (v/||v||)
    magnitude * direction
```

## Basic Vector Operations

### Addition

Vector addition combines two vectors by adding corresponding components.

**Mathematical Definition**:
```
u + v = ⟨ux + vx, uy + vy, uz + vz⟩
```

**Geometric Interpretation**:
Place the tail of the second vector at the head of the first. The sum is the vector from the first tail to the second head (triangle rule).

```
u = ⟨2, 1, 0⟩
v = ⟨1, 2, 0⟩
u + v = ⟨3, 3, 0⟩

Visual (2D for clarity):
    v
    |→
    +--→ u
   /
  +---→
  u+v
```

**Properties**:
- Commutative: u + v = v + u
- Associative: (u + v) + w = u + (v + w)
- Identity: v + 0 = v

### Subtraction

Vector subtraction finds the vector from one point to another.

**Mathematical Definition**:
```
u - v = ⟨ux - vx, uy - vy, uz - vz⟩
```

**Geometric Interpretation**:
The difference u - v points from the tip of v to the tip of u when both start at the origin.

```
Given points A and B:
Vector from A to B = B - A
```

### Scalar Multiplication

Multiplying a vector by a scalar changes its magnitude but not its direction (unless negative, which reverses it).

**Mathematical Definition**:
```
k * v = ⟨k*vx, k*vy, k*vz⟩
```

**Effects**:
- k > 1: Stretches the vector
- 0 < k < 1: Shrinks the vector
- k = 0: Zero vector
- k < 0: Reverses direction and scales

```
v = ⟨1, 1, 0⟩
2v = ⟨2, 2, 0⟩    (twice as long)
0.5v = ⟨0.5, 0.5, 0⟩  (half as long)
-v = ⟨-1, -1, 0⟩  (opposite direction)
```

### Scalar Division

Dividing by a scalar is equivalent to multiplying by its reciprocal.

**Mathematical Definition**:
```
v / k = v * (1/k) = ⟨vx/k, vy/k, vz/k⟩
```

**Important**: Always check for division by zero!

## Vector Properties

### Magnitude (Length)

The magnitude is the length of the vector.

**Formula**:
```
||v|| = √(vx² + vy² + vz²)
```

**Example**:
```
v = ⟨3, 4, 0⟩
||v|| = √(9 + 16 + 0) = √25 = 5
```

### Unit Vectors

A unit vector has magnitude 1. Unit vectors represent pure direction.

**Normalization**:
To convert any vector to a unit vector (normalize it):
```
û = v / ||v||
```

**Standard Basis Vectors**:
- i = ⟨1, 0, 0⟩ (X-axis)
- j = ⟨0, 1, 0⟩ (Y-axis)
- k = ⟨0, 0, 1⟩ (Z-axis)

Any vector can be written as a combination:
```
v = vx*i + vy*j + vz*k
```

### Zero Vector

The zero vector ⟨0, 0, 0⟩ has:
- Magnitude: 0
- No defined direction
- Special properties in operations

## Geometric Interpretations

### Displacement

Vectors represent movement from one point to another:
```
Position_new = Position_old + Velocity * Time
```

### Velocity and Acceleration

In physics simulations:
- **Velocity**: Rate of position change (vector)
- **Acceleration**: Rate of velocity change (vector)

```
velocity = ⟨vx, vy, vz⟩ units per second
position += velocity * deltaTime
```

### Forces

Forces in physics are vectors:
- **Gravity**: ⟨0, -9.8, 0⟩ m/s²
- **Wind**: ⟨wx, wy, wz⟩
- **Net Force**: Sum of all forces

## Complete Vector3 Implementation

### Enhanced Vector3 Class

```cpp
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    // ========================================
    // CONSTRUCTORS
    // ========================================

    // Default constructor - zero vector
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

    // Parameterized constructor
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Copy constructor
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

    // ========================================
    // ASSIGNMENT
    // ========================================

    Vector3& operator=(const Vector3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    // ========================================
    // ARITHMETIC OPERATORS
    // ========================================

    // Vector addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Unary negation
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3 operator/(float scalar) const {
        if (std::abs(scalar) < 1e-8f) {
            throw std::runtime_error("Vector3: Division by zero or near-zero scalar");
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // ========================================
    // COMPOUND ASSIGNMENT OPERATORS
    // ========================================

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) {
        if (std::abs(scalar) < 1e-8f) {
            throw std::runtime_error("Vector3: Division by zero or near-zero scalar");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // ========================================
    // MAGNITUDE OPERATIONS
    // ========================================

    // Calculate magnitude (length)
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Calculate squared magnitude (faster, no square root)
    float magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    // Normalize - return unit vector in same direction
    Vector3 normalized() const {
        float mag = magnitude();
        if (mag < 1e-8f) {
            throw std::runtime_error("Vector3: Cannot normalize zero vector");
        }
        return *this / mag;
    }

    // Normalize in-place
    void normalize() {
        float mag = magnitude();
        if (mag < 1e-8f) {
            throw std::runtime_error("Vector3: Cannot normalize zero vector");
        }
        *this /= mag;
    }

    // Safe normalize - returns zero vector if magnitude is too small
    Vector3 safeNormalized() const {
        float mag = magnitude();
        if (mag < 1e-8f) {
            return Vector3(0, 0, 0);
        }
        return *this / mag;
    }

    // Check if vector is normalized (unit length)
    bool isNormalized(float epsilon = 1e-5f) const {
        return std::abs(magnitudeSquared() - 1.0f) < epsilon;
    }

    // Check if vector is zero
    bool isZero(float epsilon = 1e-8f) const {
        return std::abs(x) < epsilon &&
               std::abs(y) < epsilon &&
               std::abs(z) < epsilon;
    }

    // ========================================
    // VECTOR PRODUCTS (Basic)
    // ========================================

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

    // ========================================
    // UTILITY FUNCTIONS
    // ========================================

    // Component-wise minimum
    Vector3 min(const Vector3& other) const {
        return Vector3(
            std::min(x, other.x),
            std::min(y, other.y),
            std::min(z, other.z)
        );
    }

    // Component-wise maximum
    Vector3 max(const Vector3& other) const {
        return Vector3(
            std::max(x, other.x),
            std::max(y, other.y),
            std::max(z, other.z)
        );
    }

    // Component-wise absolute value
    Vector3 abs() const {
        return Vector3(std::abs(x), std::abs(y), std::abs(z));
    }

    // Clamp components between min and max
    Vector3 clamp(float minVal, float maxVal) const {
        return Vector3(
            std::max(minVal, std::min(maxVal, x)),
            std::max(minVal, std::min(maxVal, y)),
            std::max(minVal, std::min(maxVal, z))
        );
    }

    // ========================================
    // COMPARISON
    // ========================================

    // Equality with epsilon
    bool equals(const Vector3& other, float epsilon = 1e-5f) const {
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }

    // Exact equality
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Inequality
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    // ========================================
    // ARRAY ACCESS
    // ========================================

    // Index operator (0=x, 1=y, 2=z)
    float& operator[](int index) {
        switch(index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Vector3: Index out of range");
        }
    }

    const float& operator[](int index) const {
        switch(index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Vector3: Index out of range");
        }
    }

    // ========================================
    // OUTPUT
    // ========================================

    void print() const {
        std::cout << *this;
    }

    void println() const {
        std::cout << *this << std::endl;
    }

    // ========================================
    // STATIC HELPER FUNCTIONS
    // ========================================

    // Distance between two vectors (treating as points)
    static float distance(const Vector3& a, const Vector3& b) {
        return (b - a).magnitude();
    }

    // Squared distance (faster)
    static float distanceSquared(const Vector3& a, const Vector3& b) {
        return (b - a).magnitudeSquared();
    }

    // Linear interpolation
    static Vector3 lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
    }

    // Clamp interpolation parameter
    static Vector3 lerpClamped(const Vector3& a, const Vector3& b, float t) {
        t = std::max(0.0f, std::min(1.0f, t));
        return lerp(a, b, t);
    }

    // Component-wise multiplication
    static Vector3 scale(const Vector3& a, const Vector3& b) {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    // ========================================
    // STANDARD VECTORS
    // ========================================

    static Vector3 zero()    { return Vector3(0, 0, 0); }
    static Vector3 one()     { return Vector3(1, 1, 1); }
    static Vector3 right()   { return Vector3(1, 0, 0); }
    static Vector3 left()    { return Vector3(-1, 0, 0); }
    static Vector3 up()      { return Vector3(0, 1, 0); }
    static Vector3 down()    { return Vector3(0, -1, 0); }
    static Vector3 forward() { return Vector3(0, 0, 1); }  // Right-hand: toward viewer
    static Vector3 back()    { return Vector3(0, 0, -1); }

    // Unit basis vectors
    static Vector3 unitX()   { return Vector3(1, 0, 0); }
    static Vector3 unitY()   { return Vector3(0, 1, 0); }
    static Vector3 unitZ()   { return Vector3(0, 0, 1); }
};

// ========================================
// NON-MEMBER OPERATORS
// ========================================

// Scalar * Vector (commutative multiplication)
inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
}

// Stream output
inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
    os << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
    return os;
}
```

### Comprehensive Example Programs

```cpp
// ========================================
// EXAMPLE 1: Basic Vector Operations
// ========================================

void demonstrateBasicOperations() {
    std::cout << "=== Basic Vector Operations ===" << std::endl << std::endl;

    // Creating vectors
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    Vector3 v3(3, 4, 0);

    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;
    std::cout << "v3 = " << v3 << std::endl << std::endl;

    // Addition
    std::cout << "--- Addition ---" << std::endl;
    Vector3 sum = v1 + v2;
    std::cout << "v1 + v2 = " << sum << std::endl << std::endl;

    // Subtraction
    std::cout << "--- Subtraction ---" << std::endl;
    Vector3 diff = v2 - v1;
    std::cout << "v2 - v1 = " << diff << std::endl << std::endl;

    // Scalar multiplication
    std::cout << "--- Scalar Multiplication ---" << std::endl;
    Vector3 scaled = v3 * 2.0f;
    std::cout << "v3 * 2 = " << scaled << std::endl;
    Vector3 halfScale = v3 / 2.0f;
    std::cout << "v3 / 2 = " << halfScale << std::endl << std::endl;

    // Magnitude
    std::cout << "--- Magnitude ---" << std::endl;
    std::cout << "|v1| = " << v1.magnitude() << std::endl;
    std::cout << "|v3| = " << v3.magnitude() << " (3-4-5 triangle)" << std::endl << std::endl;

    // Normalization
    std::cout << "--- Normalization ---" << std::endl;
    Vector3 normalized = v3.normalized();
    std::cout << "v3 normalized = " << normalized << std::endl;
    std::cout << "Magnitude of normalized v3 = " << normalized.magnitude() << std::endl;
}

// ========================================
// EXAMPLE 2: Vector Math Applications
// ========================================

void demonstrateVectorMath() {
    std::cout << "\n=== Vector Math Applications ===" << std::endl << std::endl;

    // Distance calculation
    std::cout << "--- Distance Calculation ---" << std::endl;
    Vector3 pointA(0, 0, 0);
    Vector3 pointB(3, 4, 0);

    float dist = Vector3::distance(pointA, pointB);
    std::cout << "Distance from " << pointA << " to " << pointB << " = " << dist << std::endl << std::endl;

    // Interpolation
    std::cout << "--- Linear Interpolation ---" << std::endl;
    Vector3 start(0, 0, 0);
    Vector3 end(10, 10, 0);

    std::cout << "Start: " << start << std::endl;
    std::cout << "End: " << end << std::endl;
    std::cout << "25% along: " << Vector3::lerp(start, end, 0.25f) << std::endl;
    std::cout << "50% along: " << Vector3::lerp(start, end, 0.5f) << std::endl;
    std::cout << "75% along: " << Vector3::lerp(start, end, 0.75f) << std::endl << std::endl;

    // Component operations
    std::cout << "--- Component Operations ---" << std::endl;
    Vector3 a(1, 5, 3);
    Vector3 b(4, 2, 6);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "min(a, b) = " << a.min(b) << std::endl;
    std::cout << "max(a, b) = " << a.max(b) << std::endl;
}

// ========================================
// EXAMPLE 3: Physics Simulation
// ========================================

class Particle {
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;

    Particle(const Vector3& pos, float m = 1.0f)
        : position(pos), mass(m) {}

    void applyForce(const Vector3& force) {
        // F = ma, so a = F/m
        acceleration += force / mass;
    }

    void update(float deltaTime) {
        // Update velocity
        velocity += acceleration * deltaTime;

        // Update position
        position += velocity * deltaTime;

        // Reset acceleration (forces must be reapplied each frame)
        acceleration = Vector3::zero();
    }

    void print() const {
        std::cout << "Position: " << position
                  << ", Velocity: " << velocity << std::endl;
    }
};

void demonstratePhysics() {
    std::cout << "\n=== Physics Simulation ===" << std::endl << std::endl;

    Particle particle(Vector3(0, 10, 0), 1.0f);
    Vector3 gravity(0, -9.8f, 0);  // m/s^2

    float deltaTime = 0.1f;  // 100ms per frame

    std::cout << "Simulating falling particle (gravity = " << gravity << ")" << std::endl;
    std::cout << "Starting position: " << particle.position << std::endl << std::endl;

    for (int frame = 0; frame < 10; frame++) {
        particle.applyForce(gravity * particle.mass);
        particle.update(deltaTime);

        std::cout << "Frame " << frame + 1 << ": ";
        particle.print();

        // Stop if hit ground
        if (particle.position.y <= 0) {
            std::cout << "Hit ground!" << std::endl;
            break;
        }
    }
}

// ========================================
// EXAMPLE 4: Velocity and Movement
// ========================================

void demonstrateMovement() {
    std::cout << "\n=== Movement System ===" << std::endl << std::endl;

    Vector3 playerPosition(0, 0, 0);
    Vector3 targetPosition(10, 0, 5);

    std::cout << "Player at: " << playerPosition << std::endl;
    std::cout << "Target at: " << targetPosition << std::endl << std::endl;

    // Calculate direction to target
    Vector3 toTarget = targetPosition - playerPosition;
    float distance = toTarget.magnitude();
    Vector3 direction = toTarget.normalized();

    std::cout << "Direction to target: " << direction << std::endl;
    std::cout << "Distance to target: " << distance << std::endl << std::endl;

    // Move toward target
    float speed = 2.0f;  // units per second
    float deltaTime = 0.016f;  // ~60 FPS

    std::cout << "Moving toward target at " << speed << " units/second:" << std::endl;

    for (int step = 0; step < 10; step++) {
        Vector3 velocity = direction * speed;
        playerPosition += velocity * deltaTime;

        float currentDist = Vector3::distance(playerPosition, targetPosition);
        std::cout << "Step " << step + 1 << ": Position " << playerPosition
                  << ", Distance remaining: " << currentDist << std::endl;

        if (currentDist < 0.1f) {
            std::cout << "Reached target!" << std::endl;
            break;
        }
    }
}

// ========================================
// MAIN PROGRAM
// ========================================

int main() {
    std::cout << std::fixed << std::setprecision(4);

    try {
        demonstrateBasicOperations();
        demonstrateVectorMath();
        demonstratePhysics();
        demonstrateMovement();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## Practical Applications

### Game Movement

```cpp
// Player movement system
class Player {
    Vector3 position;
    Vector3 forward;  // Which way player faces
    float speed;

    void moveForward(float deltaTime) {
        position += forward * speed * deltaTime;
    }

    void moveRight(float deltaTime) {
        Vector3 right = forward.cross(Vector3::up());
        position += right * speed * deltaTime;
    }
};
```

### Projectile Motion

```cpp
// Calculate projectile trajectory
class Projectile {
    Vector3 position;
    Vector3 velocity;

    void update(float deltaTime, const Vector3& gravity) {
        velocity += gravity * deltaTime;  // Apply gravity
        position += velocity * deltaTime;  // Update position
    }
};
```

### Path Following

```cpp
// Make an object follow a path
Vector3 followPath(const Vector3& current,
                   const Vector3& target,
                   float speed,
                   float deltaTime) {
    Vector3 toTarget = target - current;
    float distance = toTarget.magnitude();

    if (distance < speed * deltaTime) {
        return target;  // Reached target
    }

    Vector3 direction = toTarget / distance;
    return current + direction * speed * deltaTime;
}
```

## Performance Optimization

### Avoid Unnecessary Square Roots

```cpp
// SLOW - uses square root
if (vectorA.magnitude() < vectorB.magnitude()) {
    // A is shorter
}

// FAST - compares squared magnitudes (no square root)
if (vectorA.magnitudeSquared() < vectorB.magnitudeSquared()) {
    // A is shorter
}
```

### Reuse Normalized Vectors

```cpp
// BAD - normalizes multiple times
Vector3 direction = velocity.normalized();
// ... later ...
Vector3 dir2 = velocity.normalized();  // Redundant calculation!

// GOOD - normalize once, reuse
Vector3 direction = velocity.normalized();
// ... use direction multiple times ...
```

### Inline Functions

Small, frequently-called functions benefit from inlining:

```cpp
inline float Vector3::magnitudeSquared() const {
    return x * x + y * y + z * z;
}
```

## Key Takeaways

1. **Vectors represent direction and magnitude**, not just positions
2. **Vector addition** follows the triangle rule geometrically
3. **Normalization** creates unit vectors (magnitude = 1) for pure direction
4. **Magnitude squared** is faster than magnitude for comparisons
5. **Linear interpolation** (lerp) is fundamental for animation and movement
6. **Zero vector** requires special handling in normalization
7. **Component-wise operations** enable flexible vector manipulation
8. **Physics simulations** rely heavily on vector arithmetic
9. **Performance matters**: avoid square roots when possible
10. **Epsilon comparisons** are essential for floating-point equality

## Common Pitfalls

### Normalizing the Zero Vector

```cpp
Vector3 v = Vector3::zero();
v.normalize();  // ERROR! Division by zero!

// Solution: Check before normalizing
if (!v.isZero()) {
    v.normalize();
}
// Or use safe normalize
Vector3 normalized = v.safeNormalized();
```

### Comparing Floats with ==

```cpp
// BAD
if (v1 == v2) { ... }

// GOOD
if (v1.equals(v2, 0.0001f)) { ... }
```

### Confusing Points and Vectors

```cpp
// BAD - adding two positions doesn't make sense
Vector3 averagePosition = position1 + position2;

// GOOD - take the midpoint
Vector3 midpoint = (position1 + position2) / 2.0f;
```

## Exercises

1. **Implement a `reflect` function** that reflects a vector across a normal vector
2. **Create a `projectOnto` function** that projects one vector onto another
3. **Write a function to calculate the angle** between two vectors in degrees
4. **Implement a simple bouncing ball** simulation using vectors
5. **Create a waypoint follower** that smoothly moves through a series of points

## What's Next?

In **Lesson 03**, we'll dive deeper into advanced vector mathematics:
- Vector projections and rejections
- Orthogonal and parallel components
- Vector reflection and refraction
- Triple products
- Gram-Schmidt orthogonalization
- Vector spaces and bases

These advanced concepts build on the foundation you've learned here and are essential for lighting calculations, physics, and advanced transformations.

## Conclusion

Vectors are the workhorses of 3D graphics. You now have a solid understanding of vector representation, operations, and applications, along with a complete, production-ready Vector3 class. Practice using vectors in different contexts to build intuition for their geometric meaning.

Remember: every time you move an object, calculate lighting, detect collisions, or implement physics, you're using vector mathematics. Mastering vectors is mastering 3D graphics.

---

**Lesson 02 Complete** - Words: ~5,200 | Code Lines: ~500+ | Next: Lesson 03 - Vector Mathematics Deep Dive

