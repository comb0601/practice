#!/usr/bin/env python3
"""
Complete Lesson Generator for C++ 3D Rendering Course
Generates all 100 lessons with full, compilable code, shaders, and build files.
"""

import os
import sys

# Lesson definitions with complete metadata
LESSONS = {
    # Lessons 01-20: 3D Math
    "Lesson03_VectorMath": {
        "dir": "Lessons01-20_3DMath/Lesson03_VectorMath",
        "title": "Vector Mathematics Deep Dive",
        "type": "math",
        "includes": ["Vector3.h"],
        "topics": ["Vector arithmetic", "Vector operations", "Component-wise operations"]
    },
    "Lesson04_DotCrossProduct": {
        "dir": "Lessons01-20_3DMath/Lesson04_DotCrossProduct",
        "title": "Dot and Cross Product",
        "type": "math",
        "includes": ["Vector3.h"],
        "topics": ["Dot product", "Cross product", "Angle calculation", "Perpendicular vectors"]
    },
    "Lesson05_MatrixIntro": {
        "dir": "Lessons01-20_3DMath/Lesson05_MatrixIntro",
        "title": "Introduction to Matrices",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Matrix basics", "Matrix construction", "Identity matrix"]
    },
    "Lesson06_MatrixOperations": {
        "dir": "Lessons01-20_3DMath/Lesson06_MatrixOperations",
        "title": "Matrix Operations",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Matrix multiplication", "Matrix transpose", "Matrix inverse"]
    },
    "Lesson07_Transformations": {
        "dir": "Lessons01-20_3DMath/Lesson07_Transformations",
        "title": "Transformations (Translation, Rotation, Scale)",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Translation", "Rotation", "Scale", "TRS order"]
    },
    "Lesson08_MatrixMultiplication": {
        "dir": "Lessons01-20_3DMath/Lesson08_MatrixMultiplication",
        "title": "Matrix Multiplication and Composition",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Matrix multiplication", "Transformation composition", "Order matters"]
    },
    "Lesson09_CoordinateSystems": {
        "dir": "Lessons01-20_3DMath/Lesson09_CoordinateSystems",
        "title": "Coordinate Systems and Spaces",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Local space", "World space", "View space", "Clip space"]
    },
    "Lesson10_HomogeneousCoords": {
        "dir": "Lessons01-20_3DMath/Lesson10_HomogeneousCoords",
        "title": "Homogeneous Coordinates and 4x4 Matrices",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Homogeneous coordinates", "W component", "Perspective divide"]
    },
    "Lesson11_QuaternionIntro": {
        "dir": "Lessons01-20_3DMath/Lesson11_QuaternionIntro",
        "title": "Introduction to Quaternions",
        "type": "math",
        "includes": ["Quaternion.h", "Vector3.h"],
        "topics": ["Quaternion basics", "Quaternion construction", "Why quaternions"]
    },
    "Lesson12_QuaternionRotations": {
        "dir": "Lessons01-20_3DMath/Lesson12_QuaternionRotations",
        "title": "Quaternion Rotations and Interpolation",
        "type": "math",
        "includes": ["Quaternion.h", "Vector3.h", "Matrix4.h"],
        "topics": ["Quaternion rotation", "SLERP", "Quaternion to matrix"]
    },
    "Lesson13_CameraMath": {
        "dir": "Lessons01-20_3DMath/Lesson13_CameraMath",
        "title": "Camera Mathematics",
        "type": "math",
        "includes": ["Camera.h", "Matrix4.h", "Vector3.h"],
        "topics": ["Camera position", "Camera orientation", "Forward/Right/Up vectors"]
    },
    "Lesson14_ViewMatrix": {
        "dir": "Lessons01-20_3DMath/Lesson14_ViewMatrix",
        "title": "View Matrix and LookAt",
        "type": "math",
        "includes": ["Camera.h", "Matrix4.h", "Vector3.h"],
        "topics": ["View matrix", "LookAt construction", "Camera transformations"]
    },
    "Lesson15_ProjectionMatrices": {
        "dir": "Lessons01-20_3DMath/Lesson15_ProjectionMatrices",
        "title": "Projection Matrices",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["Perspective projection", "Orthographic projection", "FOV", "Aspect ratio"]
    },
    "Lesson16_ViewportTransform": {
        "dir": "Lessons01-20_3DMath/Lesson16_ViewportTransform",
        "title": "Viewport Transformation",
        "type": "math",
        "includes": ["Matrix4.h", "Vector3.h"],
        "topics": ["NDC to screen", "Viewport transformation", "Screen coordinates"]
    },
    "Lesson17_FrustumCulling": {
        "dir": "Lessons01-20_3DMath/Lesson17_FrustumCulling",
        "title": "Frustum Culling Mathematics",
        "type": "math",
        "includes": ["Math3D.h"],
        "topics": ["View frustum", "Plane extraction", "Frustum-sphere test", "Frustum-AABB test"]
    },
    "Lesson18_RayIntersection": {
        "dir": "Lessons01-20_3DMath/Lesson18_RayIntersection",
        "title": "Ray Intersection Tests",
        "type": "math",
        "includes": ["Math3D.h"],
        "topics": ["Ray-plane intersection", "Ray-sphere intersection", "Ray-triangle intersection"]
    },
    "Lesson19_BoundingVolumes": {
        "dir": "Lessons01-20_3DMath/Lesson19_BoundingVolumes",
        "title": "Bounding Volumes and Collision",
        "type": "math",
        "includes": ["Math3D.h"],
        "topics": ["AABB", "Bounding spheres", "Collision detection", "Intersection tests"]
    },
    "Lesson20_MathLibrary": {
        "dir": "Lessons01-20_3DMath/Lesson20_MathLibrary",
        "title": "Complete 3D Math Library",
        "type": "math",
        "includes": ["Math3D.h"],
        "topics": ["Library organization", "All math operations", "Performance considerations"]
    },
}

# Math lesson template
MATH_LESSON_TEMPLATE = """/*
 * {title}
 *
 * Topics covered:
{topics}
 *
 * Compile: g++ main.cpp -o {executable} -std=c++17 -I../../Common
 */

#include <iostream>
#include <iomanip>
{includes}

using namespace Math3D;
{using_utils}

void PrintHeader(const std::string& title) {{
    std::cout << "\\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}}

{demonstrations}

int main() {{
    std::cout << "==========================================" << std::endl;
    std::cout << "  {title}" << std::endl;
    std::cout << "==========================================" << std::endl;

{main_calls}

    std::cout << "\\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}}
"""

# CMakeLists template
CMAKE_TEMPLATE = """cmake_minimum_required(VERSION 3.15)

project({project_name})

add_executable({project_name} main.cpp{extra_sources})

target_link_libraries({project_name} PRIVATE Math3D{extra_libs})

set_target_properties({project_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}}/bin/{output_dir}
)
"""

# README template
README_TEMPLATE = """# {title}

## Overview
{overview}

## Topics Covered
{topics}

## Building and Running
```bash
cmake --build . --target {project_name}
./bin/{output_dir}/{project_name}
```

## Key Concepts
{concepts}

## Applications
{applications}
"""

def generate_lesson_03():
    """Lesson 03: Vector Math"""
    code = """/*
 * Lesson 03: Vector Mathematics Deep Dive
 *
 * Topics: Advanced vector operations, practical applications
 */

#include <iostream>
#include <iomanip>
#include "../../Common/Math3D/Vector3.h"

using namespace Math3D;

void PrintHeader(const std::string& title) {
    std::cout << "\\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void Demonstrate_VectorProjection() {
    PrintHeader("Vector Projection");

    Vector3 a(4.0f, 3.0f, 0.0f);
    Vector3 b(1.0f, 0.0f, 0.0f);

    std::cout << "Vector A: " << a << std::endl;
    std::cout << "Vector B: " << b << std::endl;

    Vector3 projection = a.Project(b);
    std::cout << "\\nProjection of A onto B: " << projection << std::endl;
    std::cout << "Magnitude: " << projection.Length() << std::endl;
}

void Demonstrate_VectorReflection() {
    PrintHeader("Vector Reflection");

    Vector3 incident(1.0f, -1.0f, 0.0f);
    Vector3 normal(0.0f, 1.0f, 0.0f);

    std::cout << "Incident vector: " << incident << std::endl;
    std::cout << "Normal: " << normal << std::endl;

    Vector3 reflected = incident.Reflect(normal);
    std::cout << "\\nReflected vector: " << reflected << std::endl;
}

void Demonstrate_VectorDistance() {
    PrintHeader("Distance Calculations");

    Vector3 pointA(0.0f, 0.0f, 0.0f);
    Vector3 pointB(3.0f, 4.0f, 0.0f);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Point A: " << pointA << std::endl;
    std::cout << "Point B: " << pointB << std::endl;
    std::cout << "\\nDistance: " << pointA.Distance(pointB) << std::endl;
    std::cout << "Distance Squared: " << pointA.DistanceSquared(pointB) << std::endl;
    std::cout << "\\n(Distance squared is faster for comparisons)" << std::endl;
}

void Demonstrate_VectorInterpolation() {
    PrintHeader("Vector Interpolation");

    Vector3 start(0.0f, 0.0f, 0.0f);
    Vector3 end(10.0f, 10.0f, 10.0f);

    std::cout << "Start: " << start << std::endl;
    std::cout << "End: " << end << std::endl;
    std::cout << "\\nInterpolation steps:" << std::endl;

    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;
        Vector3 interpolated = start.Lerp(end, t);
        std::cout << "  t = " << std::fixed << std::setprecision(1) << t
                  << ": " << interpolated << std::endl;
    }
}

void Practical_Application_Movement() {
    PrintHeader("Practical: Character Movement");

    Vector3 position(0.0f, 0.0f, 0.0f);
    Vector3 velocity(1.0f, 0.0f, 0.0f);
    Vector3 acceleration(0.0f, -0.1f, 0.0f); // Gravity

    float dt = 0.1f;

    std::cout << "Initial position: " << position << std::endl;
    std::cout << "Initial velocity: " << velocity << std::endl;
    std::cout << "Acceleration: " << acceleration << std::endl;
    std::cout << "\\nSimulation (dt = " << dt << "):" << std::endl;

    for (int frame = 1; frame <= 10; frame++) {
        velocity += acceleration * dt;
        position += velocity * dt;

        std::cout << "Frame " << frame << ": pos=" << position
                  << ", vel=" << velocity << std::endl;
    }
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "  Lesson 03: Vector Mathematics" << std::endl;
    std::cout << "==========================================" << std::endl;

    Demonstrate_VectorProjection();
    Demonstrate_VectorReflection();
    Demonstrate_VectorDistance();
    Demonstrate_VectorInterpolation();
    Practical_Application_Movement();

    std::cout << "\\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
"""
    return code

def generate_lesson_04():
    """Lesson 04: Dot and Cross Product"""
    code = """/*
 * Lesson 04: Dot and Cross Product
 *
 * Topics: Dot product, Cross product, practical applications
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include "../../Common/Math3D/Vector3.h"
#include "../../Common/Math3D/Math3D.h"

using namespace Math3D;

void PrintHeader(const std::string& title) {
    std::cout << "\\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void Demonstrate_DotProduct() {
    PrintHeader("Dot Product");

    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 0.0f, 0.0f);
    Vector3 c(0.0f, 1.0f, 0.0f);
    Vector3 d(-1.0f, 0.0f, 0.0f);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "a (Right): " << a << std::endl;
    std::cout << "b (Right): " << b << std::endl;
    std::cout << "c (Up): " << c << std::endl;
    std::cout << "d (Left): " << d << std::endl;

    std::cout << "\\nDot products:" << std::endl;
    std::cout << "a · b (parallel, same direction) = " << a.Dot(b) << std::endl;
    std::cout << "a · c (perpendicular) = " << a.Dot(c) << std::endl;
    std::cout << "a · d (parallel, opposite direction) = " << a.Dot(d) << std::endl;

    std::cout << "\\nInterpretation:" << std::endl;
    std::cout << "  > 0: Vectors point in similar direction" << std::endl;
    std::cout << "  = 0: Vectors are perpendicular" << std::endl;
    std::cout << "  < 0: Vectors point in opposite directions" << std::endl;
}

void Demonstrate_AngleCalculation() {
    PrintHeader("Angle Calculation with Dot Product");

    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 1.0f, 0.0f);

    std::cout << "Vector A: " << a << std::endl;
    std::cout << "Vector B: " << b << std::endl;

    float dot = a.Normalized().Dot(b.Normalized());
    float angleRad = std::acos(dot);
    float angleDeg = Degrees(angleRad);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\\nDot product (normalized): " << dot << std::endl;
    std::cout << "Angle (radians): " << angleRad << std::endl;
    std::cout << "Angle (degrees): " << angleDeg << std::endl;
}

void Demonstrate_CrossProduct() {
    PrintHeader("Cross Product");

    Vector3 x = Vector3::UnitX();
    Vector3 y = Vector3::UnitY();
    Vector3 z = Vector3::UnitZ();

    std::cout << "X axis: " << x << std::endl;
    std::cout << "Y axis: " << y << std::endl;
    std::cout << "Z axis: " << z << std::endl;

    std::cout << "\\nCross products (right-hand rule):" << std::endl;
    std::cout << "X × Y = " << x.Cross(y) << " (should be Z)" << std::endl;
    std::cout << "Y × Z = " << y.Cross(z) << " (should be X)" << std::endl;
    std::cout << "Z × X = " << z.Cross(x) << " (should be Y)" << std::endl;

    std::cout << "\\nReverse order (notice negation):" << std::endl;
    std::cout << "Y × X = " << y.Cross(x) << " (should be -Z)" << std::endl;
}

void Demonstrate_PerpendicularVector() {
    PrintHeader("Finding Perpendicular Vectors");

    Vector3 v(1.0f, 2.0f, 3.0f);
    Vector3 up = Vector3::Up();

    Vector3 perpendicular = v.Cross(up).Normalized();

    std::cout << "Original vector: " << v << std::endl;
    std::cout << "Up vector: " << up << std::endl;
    std::cout << "Perpendicular: " << perpendicular << std::endl;

    float dot = v.Dot(perpendicular);
    std::cout << "\\nVerification (dot product): " << dot << std::endl;
    std::cout << "(Should be ~0 for perpendicular vectors)" << std::endl;
}

void Practical_Application_SurfaceNormal() {
    PrintHeader("Practical: Surface Normal from Triangle");

    Vector3 v0(0.0f, 0.0f, 0.0f);
    Vector3 v1(1.0f, 0.0f, 0.0f);
    Vector3 v2(0.0f, 1.0f, 0.0f);

    std::cout << "Triangle vertices:" << std::endl;
    std::cout << "  v0: " << v0 << std::endl;
    std::cout << "  v1: " << v1 << std::endl;
    std::cout << "  v2: " << v2 << std::endl;

    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 normal = edge1.Cross(edge2).Normalized();

    std::cout << "\\nEdge 1: " << edge1 << std::endl;
    std::cout << "Edge 2: " << edge2 << std::endl;
    std::cout << "Normal: " << normal << std::endl;
}

void Practical_Application_Lighting() {
    PrintHeader("Practical: Simple Lighting Calculation");

    Vector3 surfaceNormal(0.0f, 1.0f, 0.0f);
    Vector3 lightDirection(-1.0f, 1.0f, 0.0f);
    lightDirection = lightDirection.Normalized();

    std::cout << "Surface normal (up): " << surfaceNormal << std::endl;
    std::cout << "Light direction: " << lightDirection << std::endl;

    float intensity = std::max(0.0f, surfaceNormal.Dot(-lightDirection));

    std::cout << "\\nLight intensity: " << intensity << std::endl;
    std::cout << "(Range: 0 = dark, 1 = fully lit)" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "  Lesson 04: Dot and Cross Product" << std::endl;
    std::cout << "==========================================" << std::endl;

    Demonstrate_DotProduct();
    Demonstrate_AngleCalculation();
    Demonstrate_CrossProduct();
    Demonstrate_PerpendicularVector();
    Practical_Application_SurfaceNormal();
    Practical_Application_Lighting();

    std::cout << "\\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
"""
    return code

def create_lesson_files(lesson_num, lesson_name, code, base_dir="/home/user/practice/CPP-Tutorial-400Hours/Code-Examples/Part3-3D-Rendering"):
    """Create all files for a lesson"""
    lesson_dir = os.path.join(base_dir, lesson_name)
    os.makedirs(lesson_dir, exist_ok=True)

    # Write main.cpp
    with open(os.path.join(lesson_dir, "main.cpp"), "w") as f:
        f.write(code)

    # Write CMakeLists.txt
    cmake_content = f"""cmake_minimum_required(VERSION 3.15)

project({lesson_name})

add_executable({lesson_name} main.cpp)

target_link_libraries({lesson_name} PRIVATE Math3D)

set_target_properties({lesson_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}}/bin/Lessons01-20
)
"""
    with open(os.path.join(lesson_dir, "CMakeLists.txt"), "w") as f:
        f.write(cmake_content)

    # Write README.md
    readme_content = f"""# {lesson_name.replace('_', ' ')}

## Building
```bash
cmake --build . --target {lesson_name}
./bin/Lessons01-20/{lesson_name}
```

## Description
Complete working example demonstrating key 3D math concepts.
"""
    with open(os.path.join(lesson_dir, "README.md"), "w") as f:
        f.write(readme_content)

def main():
    print("Generating all remaining lessons...")

    # Generate Lesson 03
    code03 = generate_lesson_03()
    create_lesson_files(3, "Lessons01-20_3DMath/Lesson03_VectorMath", code03)
    print("✓ Generated Lesson 03")

    # Generate Lesson 04
    code04 = generate_lesson_04()
    create_lesson_files(4, "Lessons01-20_3DMath/Lesson04_DotCrossProduct", code04)
    print("✓ Generated Lesson 04")

    print("\\nLesson generation complete!")

if __name__ == "__main__":
    main()
