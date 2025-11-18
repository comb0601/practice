# Lesson 55: 3D Coordinate Systems & Vectors

## Course Information
- **Module**: Module 6 - 3D Graphics Mathematics
- **Lesson**: 55 of 64
- **Estimated Completion Time**: 4-5 hours
- **Difficulty**: Intermediate

## Table of Contents
1. [Introduction to 3D Graphics Mathematics](#introduction)
2. [Coordinate Systems](#coordinate-systems)
3. [Right-Handed vs Left-Handed Systems](#handedness)
4. [3D Vectors - Mathematical Foundation](#vectors)
5. [Vector Representation](#vector-representation)
6. [Vector Class Implementation](#vector-class)
7. [Practical Applications](#applications)
8. [Code Examples Overview](#code-examples)
9. [Exercises](#exercises)
10. [Additional Resources](#resources)

---

## 1. Introduction to 3D Graphics Mathematics {#introduction}

### What is 3D Graphics Mathematics?

3D graphics mathematics is the foundation of all computer graphics rendering. Whether you're developing games, CAD software, simulations, or visualization tools, understanding the mathematical principles behind 3D transformations is essential.

**Key Areas We'll Cover in This Module:**
- Coordinate systems and vectors (this lesson)
- Vector operations and calculations
- Matrices and transformations
- Quaternions for rotation
- Camera systems
- Projection transformations
- Collision detection
- Ray casting and picking

### Why Vectors Matter

Vectors are the fundamental building blocks of 3D graphics. They represent:
- **Positions** in 3D space (points)
- **Directions** (normalized vectors)
- **Velocities** (direction + magnitude)
- **Forces** in physics simulations
- **Colors** (RGB as a 3D vector)
- **Normals** for lighting calculations

---

## 2. Coordinate Systems {#coordinate-systems}

### The Cartesian Coordinate System

The Cartesian coordinate system uses three perpendicular axes to define positions in 3D space:
- **X-axis**: Typically represents left/right (width)
- **Y-axis**: Typically represents up/down (height)
- **Z-axis**: Typically represents forward/backward (depth)

```
        Y (Up)
        |
        |
        |
        +---------- X (Right)
       /
      /
     Z (Forward)
```

### Origin Point

The **origin** is the point (0, 0, 0) where all three axes intersect. All positions in 3D space are measured relative to the origin.

### Coordinate Notation

A point in 3D space is written as:
- **P = (x, y, z)**
- **Example**: P = (3.5, 2.0, -1.5)
  - 3.5 units along the X-axis
  - 2.0 units along the Y-axis
  - -1.5 units along the Z-axis (negative direction)

### Axis Properties

Each axis has:
1. **Direction**: Positive and negative directions
2. **Unit**: The measurement scale (usually arbitrary units or real-world units like meters)
3. **Orthogonality**: Each axis is perpendicular (90 degrees) to the other two

---

## 3. Right-Handed vs Left-Handed Systems {#handedness}

### Understanding Handedness

The arrangement of axes in 3D space follows one of two conventions:
- **Right-Handed Coordinate System** (used by OpenGL, Maya, Blender)
- **Left-Handed Coordinate System** (used by DirectX, Unity by default)

### Right-Handed System

**Rule**: Point your right thumb along the +X axis, your index finger along +Y axis. Your middle finger (perpendicular to both) points along +Z axis.

```
       Y
       |
       |
       +------- X
      /
     /
    Z (pointing toward you)
```

**Characteristics:**
- +X: Right
- +Y: Up
- +Z: Toward viewer (out of screen)
- Rotation: Counter-clockwise is positive when looking down an axis toward origin

**Mathematical Basis:**
```
Cross Product: X × Y = Z
               Y × Z = X
               Z × X = Y
```

### Left-Handed System

**Rule**: Point your left thumb along the +X axis, your index finger along +Y axis. Your middle finger (perpendicular to both) points along +Z axis.

```
       Y
       |
       |
       +------- X
        \
         \
          Z (pointing away from you)
```

**Characteristics:**
- +X: Right
- +Y: Up
- +Z: Away from viewer (into screen)
- Rotation: Clockwise is positive when looking down an axis toward origin

**Mathematical Basis:**
```
Cross Product: X × Y = -Z (opposite of right-handed)
               Y × Z = -X
               Z × X = -Y
```

### Choosing a System

**Right-Handed (Our Choice for This Tutorial):**
- More common in mathematics and physics
- Used by OpenGL
- Intuitive for cross products
- Standard in most 3D mathematics textbooks

**Conversion Between Systems:**

If you need to convert from right-handed to left-handed (or vice versa):
1. Negate the Z-coordinate: `(x, y, z) → (x, y, -z)`
2. Or, negate the Z-axis direction in your transformations

### Visual Memory Aid

```
Right-Handed:          Left-Handed:
    Y                      Y
    |                      |
    |                      |
    +--- X                 +--- X
   /                        \
  Z (thumb up)               Z (thumb down)
```

**Important**: Once you choose a coordinate system, be consistent throughout your entire project!

---

## 4. 3D Vectors - Mathematical Foundation {#vectors}

### What is a Vector?

A **vector** is a mathematical entity that has both:
1. **Magnitude** (length or size)
2. **Direction** (orientation in space)

**Notation:**
- Bold lowercase: **v**
- Arrow notation: **v⃗**
- Component form: **v** = (vₓ, vᵧ, vᵤ) or **v** = <vₓ, vᵧ, vᵤ>

### Scalar vs Vector

**Scalar**: A single number (magnitude only)
- Examples: temperature (72°F), speed (60 mph), mass (5 kg)

**Vector**: Multiple components (magnitude AND direction)
- Examples: velocity (60 mph northeast), force (10 N downward), position (x=3, y=4, z=5)

### Vector Components

A 3D vector has three components:

```
v = (vₓ, vᵧ, vᵤ)

where:
  vₓ = component along X-axis
  vᵧ = component along Y-axis
  vᵤ = component along Z-axis
```

**Example:**
```
v = (3, 4, 5)

Visual representation:
       (3,4,5)
      *
     /|
    / |
   /  | 5 (z)
  /   |
 /    |
*---------
(0,0,0)
```

### Vector Magnitude (Length)

The **magnitude** or **length** of a vector is calculated using the 3D Pythagorean theorem:

```
||v|| = √(vₓ² + vᵧ² + vᵤ²)
```

**Example:**
```
v = (3, 4, 5)
||v|| = √(3² + 4² + 5²)
     = √(9 + 16 + 25)
     = √50
     ≈ 7.071
```

### Position Vector vs Direction Vector

**Position Vector**: Represents a point in space relative to the origin
- Example: P = (5, 3, 2) means "the point at x=5, y=3, z=2"

**Direction Vector**: Represents a direction and distance
- Example: v = (1, 0, 0) means "one unit in the +X direction"
- Often normalized to unit length for pure direction

### Zero Vector

The **zero vector** has all components equal to zero:
```
0⃗ = (0, 0, 0)
```

Properties:
- Magnitude: ||0⃗|| = 0
- No defined direction
- Identity element for vector addition

### Unit Vectors

A **unit vector** has magnitude 1:
```
||û|| = 1
```

**Standard basis vectors** (axes directions):
```
x̂ = (1, 0, 0)  →  X-axis direction
ŷ = (0, 1, 0)  →  Y-axis direction
ẑ = (0, 0, 1)  →  Z-axis direction
```

**Any vector can be expressed as a combination of basis vectors:**
```
v = (3, 4, 5) = 3x̂ + 4ŷ + 5ẑ
              = 3(1,0,0) + 4(0,1,0) + 5(0,0,1)
```

---

## 5. Vector Representation {#vector-representation}

### Mathematical Representations

**1. Component Form:**
```
v = (vₓ, vᵧ, vᵤ)
```

**2. Column Vector (Matrix Form):**
```
    ⎡vₓ⎤
v = ⎢vᵧ⎥
    ⎣vᵤ⎦
```

**3. Row Vector:**
```
v = [vₓ  vᵧ  vᵤ]
```

**4. Basis Vector Form:**
```
v = vₓx̂ + vᵧŷ + vᵤẑ
```

### Geometric Interpretation

A vector can be visualized as:
1. **Arrow**: From origin to point (position vector)
2. **Directed Line Segment**: From one point to another
3. **Displacement**: How to get from point A to point B

```
Vector from A to B:
    B(4,5,6)
    *
   /|
  / |
 /  |
*---+  A(1,2,3)

v = B - A = (4-1, 5-2, 6-3) = (3, 3, 3)
```

### Vector Equality

Two vectors are equal if and only if all corresponding components are equal:

```
u = (uₓ, uᵧ, uᵤ)
v = (vₓ, vᵧ, vᵤ)

u == v  ⟺  uₓ == vₓ AND uᵧ == vᵧ AND uᵤ == vᵤ
```

**Important**: Vectors are equal even if they start at different points, as long as they have the same direction and magnitude.

### Free Vectors vs Bound Vectors

**Free Vectors**: Can be moved anywhere in space
- Used for directions, velocities, forces
- Only magnitude and direction matter

**Bound Vectors**: Tied to a specific point
- Used for positions relative to origin
- Starting point matters

---

## 6. Vector Class Implementation {#vector-class}

### Design Considerations

A well-designed Vector3 class should:
1. Store three float/double components (x, y, z)
2. Provide constructors for initialization
3. Implement arithmetic operators (+, -, *, /)
4. Implement comparison operators (==, !=)
5. Provide methods for common operations (length, normalize, etc.)
6. Support both member and non-member functions
7. Be efficient (inline functions, pass by reference)
8. Be const-correct

### Basic Structure

```cpp
class Vector3 {
public:
    // Data members
    float x, y, z;

    // Constructors
    Vector3();                          // Default: (0,0,0)
    Vector3(float x, float y, float z); // Parameterized
    Vector3(const Vector3& other);      // Copy constructor

    // Operators
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    // Assignment
    Vector3& operator=(const Vector3& other);
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    // Comparison
    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    // Methods
    float Length() const;              // Magnitude
    float LengthSquared() const;       // Magnitude² (faster)
    void Normalize();                  // Make unit length
    Vector3 Normalized() const;        // Return normalized copy

    // Utility
    void Print() const;
    void Set(float x, float y, float z);
};
```

### Implementation Details

**Why float vs double?**
- **float**: 4 bytes, faster, sufficient precision for graphics (typically ±10⁻⁶)
- **double**: 8 bytes, slower, higher precision (typically ±10⁻¹⁵)
- Modern GPUs work with floats, so we use float for consistency

**Why inline functions?**
- Vector operations are called frequently
- Inlining eliminates function call overhead
- Allows compiler optimizations

**Why const correctness?**
- Methods that don't modify the object should be marked const
- Allows use with const references
- Helps compiler optimize
- Prevents accidental modifications

### Memory Layout

```cpp
class Vector3 {
    float x;  // Offset 0 bytes
    float y;  // Offset 4 bytes
    float z;  // Offset 8 bytes
};          // Total: 12 bytes (on most platforms)
```

The compact memory layout means:
- Efficient cache usage
- Easy to pass to GPU
- Compatible with SIMD operations
- Natural array-like access possible

### Usage Examples

```cpp
// Construction
Vector3 v1;                    // (0, 0, 0)
Vector3 v2(1.0f, 2.0f, 3.0f);  // (1, 2, 3)
Vector3 v3(v2);                // Copy of v2

// Arithmetic
Vector3 sum = v1 + v2;         // Vector addition
Vector3 diff = v2 - v1;        // Vector subtraction
Vector3 scaled = v2 * 5.0f;    // Scalar multiplication
Vector3 div = v2 / 2.0f;       // Scalar division

// In-place operations
v1 += v2;                      // v1 = v1 + v2
v1 *= 2.0f;                    // v1 = v1 * 2

// Queries
float len = v2.Length();       // Calculate magnitude
bool equal = (v1 == v2);       // Comparison

// Modification
v1.Normalize();                // Make v1 unit length
Vector3 unit = v2.Normalized(); // Get normalized copy (v2 unchanged)
```

---

## 7. Practical Applications {#applications}

### Game Development

**Character Movement:**
```cpp
Vector3 position(0, 0, 0);     // Character position
Vector3 velocity(1, 0, 0);     // Moving right at 1 unit/second
float deltaTime = 0.016f;       // 16ms frame time (60 FPS)

// Update position
position += velocity * deltaTime;
```

**Direction to Target:**
```cpp
Vector3 playerPos(10, 0, 5);
Vector3 enemyPos(15, 0, 10);

// Calculate direction vector
Vector3 direction = enemyPos - playerPos;  // (5, 0, 5)
direction.Normalize();                     // Unit direction

// Move toward target
Vector3 velocity = direction * speed;
```

### Physics Simulation

**Force and Acceleration:**
```cpp
Vector3 force(0, -9.8f, 0);    // Gravity force
float mass = 2.0f;              // Object mass

Vector3 acceleration = force / mass;  // F = ma, so a = F/m
```

**Projectile Motion:**
```cpp
Vector3 position(0, 10, 0);     // Starting position
Vector3 velocity(5, 10, 0);     // Initial velocity (up and right)
Vector3 gravity(0, -9.8f, 0);   // Gravity acceleration

// Each frame
velocity += gravity * deltaTime;
position += velocity * deltaTime;
```

### 3D Graphics

**Surface Normals:**
```cpp
// Normal vector perpendicular to surface
Vector3 normal(0, 1, 0);  // Points straight up
normal.Normalize();        // Ensure unit length
```

**Camera Direction:**
```cpp
Vector3 cameraPos(0, 5, -10);
Vector3 targetPos(0, 0, 0);

Vector3 forward = (targetPos - cameraPos).Normalized();
// Camera looks toward target
```

### Collision Detection

**Distance Between Points:**
```cpp
Vector3 point1(0, 0, 0);
Vector3 point2(3, 4, 0);

Vector3 diff = point2 - point1;
float distance = diff.Length();  // 5.0 units
```

**Simple Sphere Collision:**
```cpp
Vector3 sphere1Center(0, 0, 0);
float sphere1Radius = 2.0f;

Vector3 sphere2Center(3, 0, 0);
float sphere2Radius = 1.5f;

Vector3 between = sphere2Center - sphere1Center;
float distance = between.Length();
float minDistance = sphere1Radius + sphere2Radius;

bool colliding = (distance < minDistance);
```

---

## 8. Code Examples Overview {#code-examples}

The `Code/` folder contains 15 complete, executable C++ programs demonstrating vector concepts:

### Basic Vector Class Implementation
1. **01_BasicVector3Class.cpp** - Simple Vector3 class with constructors
2. **02_VectorConstructors.cpp** - All constructor types and initialization
3. **03_VectorArithmetic.cpp** - Addition, subtraction, scalar operations
4. **04_VectorOperators.cpp** - All operator overloads

### Vector Properties
5. **05_VectorMagnitude.cpp** - Length and LengthSquared calculations
6. **06_VectorNormalization.cpp** - Creating unit vectors
7. **07_ZeroAndUnitVectors.cpp** - Special vectors and basis vectors
8. **08_VectorComparison.cpp** - Equality and floating-point comparison

### Coordinate Systems
9. **09_CoordinateSystems.cpp** - Right-handed vs left-handed systems
10. **10_AxisVectors.cpp** - Working with coordinate axes

### Practical Applications
11. **11_PositionAndDirection.cpp** - Position vs direction vectors
12. **12_VectorMovement.cpp** - Simulating object movement
13. **13_DistanceCalculation.cpp** - Distance between points
14. **14_VectorScaling.cpp** - Scaling and interpolation
15. **15_VectorVisualization.cpp** - Printing and debugging vectors

### Compiling and Running

```bash
# Compile any example
g++ -o program 01_BasicVector3Class.cpp -std=c++11 -Wall

# Run
./program
```

Each program includes:
- Complete, self-contained code
- Detailed comments explaining concepts
- Console output for visualization
- Test cases demonstrating functionality

---

## 9. Exercises {#exercises}

### Exercise 1: Vector Basics
Create vectors and perform basic operations:
```cpp
// Create vectors
Vector3 v1(3, 4, 0);
Vector3 v2(1, 2, 5);

// Calculate:
// a) v1 + v2
// b) v1 - v2
// c) Length of v1
// d) Length of v2
// e) Normalized v1
```

**Expected Results:**
```
a) (4, 6, 5)
b) (2, 2, -5)
c) 5.0
d) √30 ≈ 5.477
e) (0.6, 0.8, 0.0)
```

### Exercise 2: Distance Calculation
Calculate distances between points:
```cpp
Vector3 p1(0, 0, 0);
Vector3 p2(1, 0, 0);
Vector3 p3(0, 1, 0);
Vector3 p4(1, 1, 1);

// Calculate distance from p1 to each other point
```

**Expected Results:**
```
p1 to p2: 1.0
p1 to p3: 1.0
p1 to p4: √3 ≈ 1.732
```

### Exercise 3: Movement Simulation
Simulate object moving with velocity:
```cpp
Vector3 position(0, 0, 0);
Vector3 velocity(2, 1, 0);  // units per second
float deltaTime = 1.0f/60.0f; // 60 FPS

// Update position for 120 frames (2 seconds)
// Print position every 30 frames
```

**Expected Result:**
After 2 seconds, position should be (4, 2, 0).

### Exercise 4: Direction to Target
Calculate direction from start to target:
```cpp
Vector3 start(5, 3, 2);
Vector3 target(8, 7, 2);

// Calculate:
// a) Direction vector (unnormalized)
// b) Distance to target
// c) Normalized direction
```

**Expected Results:**
```
a) (3, 4, 0)
b) 5.0
c) (0.6, 0.8, 0.0)
```

### Exercise 5: Coordinate System
Work with different coordinate systems:
```cpp
// Right-handed system
Vector3 xAxis(1, 0, 0);
Vector3 yAxis(0, 1, 0);

// Calculate what zAxis should be
// Verify it forms a right-handed system
```

### Exercise 6: Vector Scaling
Create vectors at different scales:
```cpp
Vector3 direction(1, 1, 0);
direction.Normalize();

// Create vectors with lengths: 1, 5, 10, 50, 100
// All pointing in the same direction
```

### Exercise 7: Boundary Check
Check if point is within bounds:
```cpp
Vector3 point(5, 3, 2);
Vector3 minBounds(0, 0, 0);
Vector3 maxBounds(10, 10, 10);

// Write function to check if point is inside box
bool IsInsideBounds(const Vector3& point,
                   const Vector3& min,
                   const Vector3& max);
```

### Exercise 8: Midpoint Calculation
Find midpoint between two positions:
```cpp
Vector3 a(0, 0, 0);
Vector3 b(10, 10, 10);

// Calculate midpoint
Vector3 midpoint = (a + b) / 2.0f;
```

### Exercise 9: Circle on XZ Plane
Create points on a circle:
```cpp
float radius = 5.0f;
int numPoints = 8;

// Create 8 points evenly distributed on a circle
// in the XZ plane (Y = 0)
```

### Exercise 10: Vector Interpolation
Interpolate between two vectors:
```cpp
Vector3 start(0, 0, 0);
Vector3 end(10, 10, 10);

// Calculate positions at t = 0.0, 0.25, 0.5, 0.75, 1.0
// Linear interpolation: result = start + t * (end - start)
```

---

## 10. Additional Resources {#resources}

### Recommended Books
1. **"3D Math Primer for Graphics and Game Development"** by Fletcher Dunn
   - Comprehensive coverage of 3D math concepts
   - Practical examples and clear explanations

2. **"Mathematics for 3D Game Programming and Computer Graphics"** by Eric Lengyel
   - Deep dive into mathematical foundations
   - Advanced topics and optimizations

3. **"Essential Mathematics for Games and Interactive Applications"** by James M. Van Verth
   - Practical focus on game development
   - Excellent code examples

### Online Resources

**Interactive Visualizations:**
- [3Blue1Brown - Essence of Linear Algebra](https://www.3blue1brown.com/topics/linear-algebra)
- [GeoGebra 3D Calculator](https://www.geogebra.org/3d)

**Tutorials and Articles:**
- [Scratchapixel - Geometry](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry)
- [Khan Academy - Linear Algebra](https://www.khanacademy.org/math/linear-algebra)

**Reference:**
- [GLM Library Documentation](https://glm.g-truc.net/) - Excellent C++ vector math library
- [DirectXMath Programming Reference](https://docs.microsoft.com/en-us/windows/win32/dxmath/directxmath-portal)

### Video Tutorials
- **"Linear Algebra"** by MIT OpenCourseWare
- **"Game Math"** series on YouTube
- **"Essential Mathematics for Aspiring Game Developers"** by Jim Van Verth (GDC talks)

### Practice Platforms
- [LeetCode](https://leetcode.com/) - Programming problems including geometry
- [Project Euler](https://projecteuler.net/) - Mathematical problems
- [HackerRank](https://www.hackerrank.com/) - Math and algorithms

### Graphics APIs Documentation
- **OpenGL**: https://www.opengl.org/documentation/
- **DirectX**: https://docs.microsoft.com/en-us/windows/win32/directx
- **Vulkan**: https://www.vulkan.org/learn

---

## Summary

In this lesson, you learned:

1. **Coordinate Systems**
   - Cartesian 3D coordinate system with X, Y, Z axes
   - Origin point and axis properties
   - Coordinate notation and conventions

2. **Handedness**
   - Right-handed coordinate systems (OpenGL standard)
   - Left-handed coordinate systems (DirectX standard)
   - How to choose and stay consistent

3. **Vectors**
   - Definition: magnitude + direction
   - Component representation
   - Magnitude calculation
   - Position vs direction vectors
   - Unit vectors and basis vectors

4. **Vector Class**
   - Design principles
   - Implementation structure
   - Operators and methods
   - Memory layout and efficiency

5. **Applications**
   - Game development (movement, targeting)
   - Physics simulation (forces, motion)
   - Graphics (normals, camera)
   - Collision detection (distances)

### Next Steps

In **Lesson 56: Vector Operations**, we'll explore:
- Dot product (scalar product)
- Cross product (vector product)
- Vector projections
- Angle calculations
- Distance formulas
- Advanced vector mathematics

### Key Takeaways

1. Vectors are fundamental to 3D graphics
2. Choose a coordinate system and stick with it
3. Normalize vectors when you need pure direction
4. Use efficient implementations (inline, const correctness)
5. Understand both the math and the code

---

**End of Lesson 55**

Continue to Lesson 56 to learn about vector operations and advanced calculations!
