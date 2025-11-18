# Lesson 01: Introduction to 3D Graphics and Math

## Overview
Welcome to the exciting world of 3D graphics programming! This comprehensive lesson introduces you to the fundamental mathematical concepts that power modern 3D graphics, from video games to CAD software, from animated films to virtual reality experiences. Understanding these mathematical foundations is essential for anyone looking to master 3D graphics programming.

## Table of Contents
1. [What is 3D Graphics?](#what-is-3d-graphics)
2. [The Mathematics Foundation](#the-mathematics-foundation)
3. [Coordinate Systems](#coordinate-systems)
4. [The 3D Graphics Pipeline](#the-3d-graphics-pipeline)
5. [Essential Data Structures](#essential-data-structures)
6. [Building Your First 3D Point System](#building-your-first-3d-point-system)
7. [Practical Applications](#practical-applications)
8. [Performance Considerations](#performance-considerations)

## What is 3D Graphics?

3D graphics is the art and science of creating, manipulating, and rendering three-dimensional objects on a two-dimensional screen. Unlike 2D graphics, which work with flat shapes on an X-Y plane, 3D graphics add depth through a Z-axis, allowing us to represent objects in a three-dimensional space.

### The Illusion of Depth

When you look at a 3D game or animation, you're actually viewing a 2D image on your screen. The "3D effect" is an illusion created through mathematical transformations that simulate perspective, lighting, and depth. Understanding how to create this illusion is at the heart of 3D graphics programming.

### Real-World Applications

3D graphics technology is everywhere:
- **Video Games**: Real-time rendering of interactive 3D worlds
- **Film and Animation**: Creating photorealistic or stylized 3D scenes
- **CAD/CAM**: Engineering design and manufacturing
- **Medical Imaging**: 3D visualization of CT scans and MRI data
- **Virtual Reality**: Immersive 3D environments
- **Augmented Reality**: Overlaying 3D objects on the real world
- **Scientific Visualization**: Representing complex data in 3D space

## The Mathematics Foundation

3D graphics relies heavily on several mathematical disciplines:

### Linear Algebra

Linear algebra is the backbone of 3D graphics. It provides the tools to:
- Represent points and directions in 3D space (vectors)
- Transform objects (matrices)
- Solve systems of equations (intersections, projections)

### Trigonometry

Trigonometry helps us:
- Calculate angles between objects
- Rotate objects around axes
- Implement camera movements
- Compute lighting angles

### Calculus (Advanced)

While not essential for beginners, calculus becomes important for:
- Smooth animations (derivatives for velocity and acceleration)
- Curve and surface generation (Bezier curves, NURBS)
- Physics simulations

### Geometry

Geometric principles help us:
- Understand spatial relationships
- Implement collision detection
- Calculate distances and areas
- Work with shapes and volumes

## Coordinate Systems

In 3D graphics, we represent positions using coordinate systems. The most common is the **Cartesian coordinate system**, which uses three perpendicular axes: X, Y, and Z.

### Right-Hand vs Left-Hand Coordinate Systems

There are two conventions for 3D coordinate systems:

**Right-Hand System** (used by OpenGL, Maya, Blender):
- X-axis points right
- Y-axis points up
- Z-axis points toward you (out of the screen)

**Left-Hand System** (used by DirectX, Unity):
- X-axis points right
- Y-axis points up
- Z-axis points away from you (into the screen)

To remember the right-hand system: extend your right hand with thumb pointing right (X), index finger pointing up (Y), and middle finger pointing toward you (Z).

### Understanding Axes

```
Right-Hand System:
      +Y (Up)
      |
      |
      |________ +X (Right)
     /
    /
   +Z (Toward viewer)

Left-Hand System:
      +Y (Up)
      |
      |
      |________ +X (Right)
     /
    /
   -Z (Away from viewer, into screen)
```

## The 3D Graphics Pipeline

The 3D graphics pipeline is the sequence of steps that transforms 3D objects into 2D images on your screen. Understanding this pipeline is crucial for effective 3D programming.

### Pipeline Stages

1. **Model/Object Space**: Define vertices in their local coordinate system
2. **World Space**: Transform objects to their position in the world
3. **View/Camera Space**: Transform to camera's perspective
4. **Clip Space**: Apply projection and prepare for clipping
5. **Screen Space**: Map to actual pixel coordinates
6. **Rasterization**: Convert geometric data to pixels
7. **Fragment Processing**: Calculate final pixel colors
8. **Display**: Output to the screen

### Transformation Chain

The transformation from 3D world to 2D screen involves several matrix multiplications:

```
Vertex → Model Matrix → World Matrix → View Matrix → Projection Matrix → Screen
```

Each transformation serves a specific purpose:
- **Model Matrix**: Scales, rotates, and positions the object
- **View Matrix**: Positions and orients the camera
- **Projection Matrix**: Applies perspective or orthographic projection
- **Viewport Matrix**: Maps to screen coordinates

## Essential Data Structures

Before writing code, let's understand the fundamental data structures in 3D graphics.

### Points and Vertices

A **point** (or **vertex**) represents a location in 3D space, defined by three coordinates (x, y, z).

### Vectors

A **vector** represents a direction and magnitude. In 3D graphics, vectors are used for:
- Directions (camera forward, object velocity)
- Normals (surface orientation for lighting)
- Displacement (movement from one point to another)

### Matrices

A **matrix** is a rectangular array of numbers used for transformations. In 3D graphics, we primarily use:
- 3x3 matrices (rotation and scale)
- 4x4 matrices (full transformations including translation)

### Primitives

3D objects are built from primitives:
- **Points**: Single vertices
- **Lines**: Two connected vertices
- **Triangles**: Three vertices forming a face (most common)
- **Quads**: Four vertices (often split into two triangles)

## Building Your First 3D Point System

Let's start coding! We'll build a foundation for 3D mathematics in C++.

### Basic Point3D Class

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

// Point3D class - represents a point in 3D space
class Point3D {
public:
    float x, y, z;

    // Constructors
    Point3D() : x(0.0f), y(0.0f), z(0.0f) {}

    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}

    // Copy constructor
    Point3D(const Point3D& other) : x(other.x), y(other.y), z(other.z) {}

    // Assignment operator
    Point3D& operator=(const Point3D& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    // Addition
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction
    Point3D operator-(const Point3D& other) const {
        return Point3D(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    Point3D operator*(float scalar) const {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Point3D operator/(float scalar) const {
        if (std::abs(scalar) < 1e-6f) {
            std::cerr << "Warning: Division by near-zero scalar!" << std::endl;
            return *this;
        }
        return Point3D(x / scalar, y / scalar, z / scalar);
    }

    // Compound operators
    Point3D& operator+=(const Point3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Point3D& operator-=(const Point3D& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Point3D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Point3D& operator/=(float scalar) {
        if (std::abs(scalar) < 1e-6f) {
            std::cerr << "Warning: Division by near-zero scalar!" << std::endl;
            return *this;
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Distance calculation
    float distanceTo(const Point3D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // Distance squared (faster, no square root)
    float distanceSquaredTo(const Point3D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }

    // Magnitude (length from origin)
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Magnitude squared
    float magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    // Print point
    void print() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "(" << x << ", " << y << ", " << z << ")";
    }

    // Equality check with epsilon for floating point comparison
    bool equals(const Point3D& other, float epsilon = 1e-5f) const {
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
};

// Scalar multiplication (scalar * point)
Point3D operator*(float scalar, const Point3D& point) {
    return point * scalar;
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}
```

### Vector3D Class

```cpp
// Vector3D class - represents direction and magnitude
class Vector3D {
public:
    float x, y, z;

    // Constructors
    Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    // Create vector from two points
    Vector3D(const Point3D& from, const Point3D& to)
        : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z) {}

    // Copy constructor
    Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z) {}

    // Vector operations
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D operator/(float scalar) const {
        if (std::abs(scalar) < 1e-6f) {
            std::cerr << "Warning: Division by near-zero scalar!" << std::endl;
            return *this;
        }
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    // Dot product
    float dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Length (magnitude)
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Length squared
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    // Normalize (make unit length)
    Vector3D normalized() const {
        float len = length();
        if (len < 1e-6f) {
            std::cerr << "Warning: Cannot normalize zero vector!" << std::endl;
            return Vector3D(0, 0, 0);
        }
        return *this / len;
    }

    // Normalize in place
    void normalize() {
        float len = length();
        if (len < 1e-6f) {
            std::cerr << "Warning: Cannot normalize zero vector!" << std::endl;
            return;
        }
        *this /= len;
    }

    // Print vector
    void print() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "<" << x << ", " << y << ", " << z << ">";
    }

    // Static helper - create common vectors
    static Vector3D up() { return Vector3D(0, 1, 0); }
    static Vector3D down() { return Vector3D(0, -1, 0); }
    static Vector3D left() { return Vector3D(-1, 0, 0); }
    static Vector3D right() { return Vector3D(1, 0, 0); }
    static Vector3D forward() { return Vector3D(0, 0, -1); }  // OpenGL convention
    static Vector3D back() { return Vector3D(0, 0, 1); }
    static Vector3D zero() { return Vector3D(0, 0, 0); }
    static Vector3D one() { return Vector3D(1, 1, 1); }
};

// Scalar multiplication (scalar * vector)
Vector3D operator*(float scalar, const Vector3D& vec) {
    return vec * scalar;
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
    return os;
}
```

### Comprehensive Example Program

```cpp
#include <iostream>
#include <iomanip>

// Demonstrates basic 3D point and vector operations
void demonstrateBasics() {
    std::cout << "=== 3D Graphics Math Fundamentals Demo ===" << std::endl << std::endl;

    // Creating points
    std::cout << "--- Creating Points ---" << std::endl;
    Point3D origin(0, 0, 0);
    Point3D p1(3, 4, 0);
    Point3D p2(0, 4, 3);

    std::cout << "Origin: " << origin << std::endl;
    std::cout << "Point 1: " << p1 << std::endl;
    std::cout << "Point 2: " << p2 << std::endl << std::endl;

    // Distance calculations
    std::cout << "--- Distance Calculations ---" << std::endl;
    float dist1 = origin.distanceTo(p1);
    float dist2 = origin.distanceTo(p2);
    std::cout << "Distance from origin to p1: " << dist1 << std::endl;
    std::cout << "Distance from origin to p2: " << dist2 << std::endl;
    std::cout << "Distance between p1 and p2: " << p1.distanceTo(p2) << std::endl << std::endl;

    // Point arithmetic
    std::cout << "--- Point Arithmetic ---" << std::endl;
    Point3D midpoint = (p1 + p2) / 2.0f;
    std::cout << "Midpoint of p1 and p2: " << midpoint << std::endl;
    Point3D offset = p1 + Point3D(1, 1, 1);
    std::cout << "P1 offset by (1,1,1): " << offset << std::endl << std::endl;

    // Creating vectors
    std::cout << "--- Creating Vectors ---" << std::endl;
    Vector3D v1(1, 0, 0);  // Unit vector in X direction
    Vector3D v2(0, 1, 0);  // Unit vector in Y direction
    Vector3D v3(p1, p2);   // Vector from p1 to p2

    std::cout << "Vector v1 (X-axis): " << v1 << std::endl;
    std::cout << "Vector v2 (Y-axis): " << v2 << std::endl;
    std::cout << "Vector from p1 to p2: " << v3 << std::endl;
    std::cout << "Length of v3: " << v3.length() << std::endl << std::endl;

    // Vector operations
    std::cout << "--- Vector Operations ---" << std::endl;
    Vector3D sum = v1 + v2;
    std::cout << "v1 + v2 = " << sum << std::endl;

    Vector3D scaled = v1 * 5.0f;
    std::cout << "v1 * 5 = " << scaled << std::endl;

    Vector3D normalized = v3.normalized();
    std::cout << "Normalized v3 = " << normalized << std::endl;
    std::cout << "Length of normalized v3: " << normalized.length() << std::endl << std::endl;

    // Dot product
    std::cout << "--- Dot Product ---" << std::endl;
    float dotProduct = v1.dot(v2);
    std::cout << "v1 • v2 = " << dotProduct
              << " (perpendicular vectors have dot product = 0)" << std::endl;

    Vector3D v4(1, 1, 0);
    Vector3D v5(1, 0, 0);
    dotProduct = v4.normalized().dot(v5);
    std::cout << "Normalized(1,1,0) • (1,0,0) = " << dotProduct
              << " (cosine of angle between vectors)" << std::endl << std::endl;

    // Cross product
    std::cout << "--- Cross Product ---" << std::endl;
    Vector3D cross = v1.cross(v2);
    std::cout << "X-axis × Y-axis = " << cross
              << " (perpendicular to both, follows right-hand rule)" << std::endl;

    // Verify perpendicularity
    std::cout << "Cross product • v1 = " << cross.dot(v1) << " (should be 0)" << std::endl;
    std::cout << "Cross product • v2 = " << cross.dot(v2) << " (should be 0)" << std::endl << std::endl;

    // Standard basis vectors
    std::cout << "--- Standard Basis Vectors ---" << std::endl;
    std::cout << "Right: " << Vector3D::right() << std::endl;
    std::cout << "Up: " << Vector3D::up() << std::endl;
    std::cout << "Forward: " << Vector3D::forward() << std::endl;
}

// Demonstrates practical 3D calculations
void demonstratePracticalApplications() {
    std::cout << "\n=== Practical 3D Applications ===" << std::endl << std::endl;

    // Example 1: Triangle area calculation
    std::cout << "--- Triangle Area Calculation ---" << std::endl;
    Point3D triangleA(0, 0, 0);
    Point3D triangleB(4, 0, 0);
    Point3D triangleC(0, 3, 0);

    Vector3D AB(triangleA, triangleB);
    Vector3D AC(triangleA, triangleC);
    Vector3D crossProduct = AB.cross(AC);
    float area = crossProduct.length() / 2.0f;

    std::cout << "Triangle vertices: " << triangleA << ", " << triangleB << ", " << triangleC << std::endl;
    std::cout << "Triangle area: " << area << std::endl << std::endl;

    // Example 2: Angle between vectors
    std::cout << "--- Angle Between Vectors ---" << std::endl;
    Vector3D vec1(1, 0, 0);
    Vector3D vec2(1, 1, 0);

    float cosAngle = vec1.normalized().dot(vec2.normalized());
    float angleRadians = std::acos(cosAngle);
    float angleDegrees = angleRadians * 180.0f / 3.14159265f;

    std::cout << "Vector 1: " << vec1 << std::endl;
    std::cout << "Vector 2: " << vec2 << std::endl;
    std::cout << "Angle between them: " << angleDegrees << " degrees" << std::endl << std::endl;

    // Example 3: Projecting a point onto a line
    std::cout << "--- Point Projection onto Line ---" << std::endl;
    Point3D lineStart(0, 0, 0);
    Point3D lineEnd(10, 0, 0);
    Point3D point(5, 5, 0);

    Vector3D lineDir(lineStart, lineEnd);
    lineDir.normalize();
    Vector3D toPoint(lineStart, point);

    float projectionLength = toPoint.dot(lineDir);
    Point3D projection(
        lineStart.x + lineDir.x * projectionLength,
        lineStart.y + lineDir.y * projectionLength,
        lineStart.z + lineDir.z * projectionLength
    );

    std::cout << "Line from " << lineStart << " to " << lineEnd << std::endl;
    std::cout << "Point: " << point << std::endl;
    std::cout << "Projection onto line: " << projection << std::endl;
    std::cout << "Distance from point to line: " << point.distanceTo(projection) << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    demonstrateBasics();
    demonstratePracticalApplications();

    return 0;
}
```

## Practical Applications

### Game Development

In games, these basic structures are used for:
- **Character position**: Store player and NPC locations
- **Movement vectors**: Calculate velocity and acceleration
- **Line of sight**: Check if enemies can see the player
- **Projectile paths**: Calculate bullet trajectories

### CAD Systems

In CAD applications:
- **Precise measurements**: Calculate distances between components
- **Alignment**: Ensure parts fit together correctly
- **Normal vectors**: Determine surface orientation for manufacturing

### Physics Simulations

For physics engines:
- **Forces and velocities**: Track object motion
- **Collision detection**: Find intersection points
- **Momentum calculations**: Determine impact results

## Performance Considerations

### Floating Point Precision

We use `float` (32-bit) instead of `double` (64-bit) because:
- Graphics hardware is optimized for 32-bit floats
- Memory bandwidth is critical in real-time graphics
- Precision is sufficient for most graphics applications

However, be aware of:
- **Accumulation errors**: Small errors can compound over time
- **Epsilon comparisons**: Never use `==` for float comparison
- **Catastrophic cancellation**: Subtracting similar numbers loses precision

### Optimization Techniques

1. **Avoid square roots**: Use `distanceSquared()` for comparisons
2. **Cache normalized vectors**: Don't recalculate repeatedly
3. **Use SIMD**: Modern CPUs can process 4 floats simultaneously
4. **Minimize allocations**: Reuse objects when possible

### Example: Fast Distance Comparison

```cpp
// SLOW - compares actual distances (2 square roots)
if (point1.distanceTo(target) < point2.distanceTo(target)) {
    // point1 is closer
}

// FAST - compares squared distances (no square roots)
if (point1.distanceSquaredTo(target) < point2.distanceSquaredTo(target)) {
    // point1 is closer
}
```

## Key Takeaways

1. **3D graphics is mathematical**: Everything is built on linear algebra, vectors, and matrices
2. **Coordinate systems matter**: Know whether you're using right-hand or left-hand coordinates
3. **Precision vs performance**: Balance accuracy with speed
4. **Build on fundamentals**: Complex systems are built from simple building blocks
5. **Think in transformations**: 3D graphics is about transforming coordinates through different spaces

## What's Next?

In the upcoming lessons, we'll build on these foundations:
- **Lesson 02**: Deep dive into vector mathematics
- **Lesson 03**: Advanced vector operations
- **Lesson 04**: Dot and cross products in detail
- **Lesson 05**: Introduction to matrices
- **Lesson 06-08**: Matrix transformations and operations
- **Lesson 09-10**: Coordinate systems and homogeneous coordinates
- **Lesson 11-12**: Quaternions for rotations
- **Lesson 13-16**: Camera mathematics and projections
- **Lesson 17-19**: Advanced topics (frustum, ray casting, collision)
- **Lesson 20**: Complete 3D math library

## Exercises

1. **Modify the Point3D class** to include a method that returns the point reflected across the XY plane
2. **Implement a Line3D class** with methods to find the closest point on the line to a given point
3. **Write a function** to determine if three points are collinear (lie on the same line)
4. **Create a Plane3D class** with a normal vector and a point, and implement a distance-to-plane function
5. **Extend the Vector3D class** with methods to calculate the angle between two vectors in degrees

## Additional Resources

- **Books**: "3D Math Primer for Graphics and Game Development" by Fletcher Dunn
- **Online**: Khan Academy's Linear Algebra course
- **Reference**: OpenGL and DirectX documentation
- **Practice**: Implement a simple 3D object viewer

## Conclusion

You've taken your first steps into 3D graphics mathematics! You now understand the fundamental concepts of 3D space, have implemented basic Point3D and Vector3D classes, and can perform essential calculations. These building blocks will serve as the foundation for everything we build in future lessons.

Remember: mastering 3D graphics is a journey. Don't worry if everything doesn't click immediately. As we progress through more lessons and see these concepts applied in different contexts, the pieces will fall into place.

In the next lesson, we'll dive deeper into vectors, exploring advanced operations and their geometric interpretations. See you there!

---

**Lesson 01 Complete** - Words: ~4,800 | Code Examples: 3 complete implementations | Next: Lesson 02 - Vectors Basics and Operations
