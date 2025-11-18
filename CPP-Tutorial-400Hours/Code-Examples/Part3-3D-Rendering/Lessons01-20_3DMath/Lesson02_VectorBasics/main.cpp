/*
 * Lesson 02: Vector Basics
 *
 * This lesson provides a deep dive into vector operations:
 * - Vector construction and initialization
 * - Vector arithmetic (add, subtract, multiply, divide)
 * - Vector comparison
 * - Component-wise operations
 *
 * Compile with: g++ main.cpp -o lesson02 -std=c++17 -I../../Common
 */

#include <iostream>
#include <iomanip>
#include "../../Common/Math3D/Vector3.h"

using namespace Math3D;

void PrintHeader(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void Lesson_VectorConstruction() {
    PrintHeader("Vector Construction");

    // Different ways to create vectors
    Vector3 v1;                          // Default: (0, 0, 0)
    Vector3 v2(1.0f, 2.0f, 3.0f);       // Individual components
    Vector3 v3(5.0f);                    // All components same value
    Vector3 v4 = v2;                     // Copy construction

    std::cout << "Default vector: " << v1 << std::endl;
    std::cout << "Component vector: " << v2 << std::endl;
    std::cout << "Uniform vector: " << v3 << std::endl;
    std::cout << "Copied vector: " << v4 << std::endl;

    // Accessing components
    std::cout << "\nAccessing components of " << v2 << ":" << std::endl;
    std::cout << "  x: " << v2.x << std::endl;
    std::cout << "  y: " << v2.y << std::endl;
    std::cout << "  z: " << v2.z << std::endl;
    std::cout << "  [0]: " << v2[0] << std::endl;
    std::cout << "  [1]: " << v2[1] << std::endl;
    std::cout << "  [2]: " << v2[2] << std::endl;
}

void Lesson_VectorArithmetic() {
    PrintHeader("Vector Arithmetic");

    Vector3 a(2.0f, 3.0f, 4.0f);
    Vector3 b(1.0f, 5.0f, 2.0f);

    std::cout << "Vector A: " << a << std::endl;
    std::cout << "Vector B: " << b << std::endl;

    // Addition
    Vector3 sum = a + b;
    std::cout << "\nA + B = " << sum << std::endl;

    // Subtraction
    Vector3 diff = a - b;
    std::cout << "A - B = " << diff << std::endl;

    // Scalar multiplication
    Vector3 scaled = a * 2.0f;
    std::cout << "A * 2 = " << scaled << std::endl;

    // Scalar division
    Vector3 divided = a / 2.0f;
    std::cout << "A / 2 = " << divided << std::endl;

    // Negation
    Vector3 negated = -a;
    std::cout << "-A = " << negated << std::endl;
}

void Lesson_CompoundOperators() {
    PrintHeader("Compound Assignment Operators");

    Vector3 v(1.0f, 2.0f, 3.0f);
    std::cout << "Initial vector: " << v << std::endl;

    v += Vector3(1.0f, 1.0f, 1.0f);
    std::cout << "After += (1,1,1): " << v << std::endl;

    v -= Vector3(0.5f, 0.5f, 0.5f);
    std::cout << "After -= (0.5,0.5,0.5): " << v << std::endl;

    v *= 2.0f;
    std::cout << "After *= 2: " << v << std::endl;

    v /= 2.0f;
    std::cout << "After /= 2: " << v << std::endl;
}

void Lesson_VectorComparison() {
    PrintHeader("Vector Comparison");

    Vector3 v1(1.0f, 2.0f, 3.0f);
    Vector3 v2(1.0f, 2.0f, 3.0f);
    Vector3 v3(1.0f, 2.0f, 3.001f);

    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "v3: " << v3 << std::endl;

    std::cout << "\nv1 == v2: " << (v1 == v2 ? "true" : "false") << std::endl;
    std::cout << "v1 == v3: " << (v1 == v3 ? "true" : "false") << std::endl;
    std::cout << "v1 != v3: " << (v1 != v3 ? "true" : "false") << std::endl;
}

void Lesson_VectorProperties() {
    PrintHeader("Vector Properties");

    Vector3 v(3.0f, 4.0f, 0.0f);

    std::cout << "Vector: " << v << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Length: " << v.Length() << std::endl;
    std::cout << "Length Squared: " << v.LengthSquared() << std::endl;

    Vector3 normalized = v.Normalized();
    std::cout << "Normalized: " << normalized << std::endl;
    std::cout << "Normalized Length: " << normalized.Length() << std::endl;

    // Show that normalization doesn't change direction
    std::cout << "\nScaling normalized vector:" << std::endl;
    std::cout << "normalized * 5 = " << (normalized * 5.0f) << std::endl;
    std::cout << "Length of (normalized * 5) = " << (normalized * 5.0f).Length() << std::endl;
}

void Lesson_PracticalApplications() {
    PrintHeader("Practical Applications");

    // Application 1: Calculating velocity from two positions
    std::cout << "Application 1: Velocity Calculation\n";
    Vector3 startPos(0.0f, 0.0f, 0.0f);
    Vector3 endPos(10.0f, 5.0f, 0.0f);
    float time = 2.0f;

    Vector3 displacement = endPos - startPos;
    Vector3 velocity = displacement / time;

    std::cout << "Start: " << startPos << std::endl;
    std::cout << "End: " << endPos << std::endl;
    std::cout << "Time: " << time << " seconds" << std::endl;
    std::cout << "Velocity: " << velocity << " units/sec" << std::endl;

    // Application 2: Finding midpoint
    std::cout << "\nApplication 2: Midpoint Calculation\n";
    Vector3 pointA(1.0f, 2.0f, 3.0f);
    Vector3 pointB(5.0f, 6.0f, 7.0f);
    Vector3 midpoint = (pointA + pointB) / 2.0f;

    std::cout << "Point A: " << pointA << std::endl;
    std::cout << "Point B: " << pointB << std::endl;
    std::cout << "Midpoint: " << midpoint << std::endl;

    // Application 3: Interpolation (Lerp)
    std::cout << "\nApplication 3: Linear Interpolation\n";
    Vector3 start(0.0f, 0.0f, 0.0f);
    Vector3 end(10.0f, 10.0f, 10.0f);

    std::cout << "Start: " << start << std::endl;
    std::cout << "End: " << end << std::endl;
    std::cout << "25% interpolation: " << start.Lerp(end, 0.25f) << std::endl;
    std::cout << "50% interpolation: " << start.Lerp(end, 0.50f) << std::endl;
    std::cout << "75% interpolation: " << start.Lerp(end, 0.75f) << std::endl;
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "  Lesson 02: Vector Basics\n";
    std::cout << "===========================================\n";

    Lesson_VectorConstruction();
    Lesson_VectorArithmetic();
    Lesson_CompoundOperators();
    Lesson_VectorComparison();
    Lesson_VectorProperties();
    Lesson_PracticalApplications();

    std::cout << "\n===========================================\n";
    std::cout << "  Lesson Complete!\n";
    std::cout << "===========================================\n";

    return 0;
}
