# Lesson 02: Vector Basics

## Overview
A comprehensive exploration of vector operations and properties essential for 3D graphics programming.

## Topics Covered
- Vector construction methods
- Vector arithmetic operations
- Compound assignment operators
- Vector comparison
- Vector length and normalization
- Practical applications (velocity, midpoint, interpolation)

## Building
```bash
cmake --build . --target Lesson02_VectorBasics
./bin/Lessons01-20/Lesson02_VectorBasics
```

## Key Formulas

### Vector Length
```
length = √(x² + y² + z²)
```

### Vector Addition
```
(a₁, a₂, a₃) + (b₁, b₂, b₃) = (a₁+b₁, a₂+b₂, a₃+b₃)
```

### Scalar Multiplication
```
k * (x, y, z) = (k*x, k*y, k*z)
```

### Linear Interpolation
```
lerp(a, b, t) = a + (b - a) * t
where t ∈ [0, 1]
```

## Applications
- Physics simulations (velocity, acceleration)
- Camera movement
- Object interpolation
- AI pathfinding
