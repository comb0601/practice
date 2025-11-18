# Lesson 01: Introduction to 3D Math

## Overview
This lesson introduces the fundamental concepts of 3D mathematics used in computer graphics.

## Topics Covered
- 3D coordinate systems
- Points and positions in 3D space
- Vectors and their basic properties
- Vector addition and scalar multiplication
- Unit vectors and normalization
- Distance calculations

## Building and Running

### Using CMake (All platforms)
```bash
cd Code-Examples/Part3-3D-Rendering
mkdir build
cd build
cmake ..
cmake --build .
./bin/Lessons01-20/Lesson01_3DMathIntro
```

### Manual Compilation
```bash
# Linux/Mac
g++ main.cpp -o lesson01 -std=c++17 -I../../Common

# Windows (Visual Studio Command Prompt)
cl main.cpp /std:c++17 /I../../Common /Fe:lesson01.exe
```

## Expected Output
The program demonstrates:
- Creating and displaying 3D points
- Calculating distances between points
- Vector operations (addition, scaling)
- Normalizing vectors to unit length
- A practical example of object movement

## Key Concepts

### 3D Coordinate System
- X-axis: Left/Right (Red)
- Y-axis: Up/Down (Green)
- Z-axis: Forward/Back (Blue)

### Vector vs Point
- **Point**: A location in space (position)
- **Vector**: A direction and magnitude (displacement, velocity, force)

### Vector Length
The magnitude of a vector v = (x, y, z) is:
```
length = √(x² + y² + z²)
```

### Normalization
Converting a vector to unit length (magnitude = 1):
```
normalized = v / |v|
```

## Next Steps
- Lesson 02: Vector Basics - Deeper dive into vector operations
- Lesson 03: Vector Mathematics - Dot and cross products
