/*
 * Lesson 01: Introduction to 3D Math
 *
 * This lesson covers the basics of 3D mathematics including:
 * - Points in 3D space
 * - Distance calculations
 * - Basic 3D coordinate system understanding
 * - Simple vector operations
 *
 * Compile with: g++ main.cpp -o lesson01 -std=c++17 -I../../Common
 */

#include <iostream>
#include <cmath>
#include "../../Common/Math3D/Vector3.h"

using namespace Math3D;

void PrintSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

void Lesson_Points3D() {
    PrintSeparator("3D Points");

    // Creating points in 3D space
    Vector3 origin(0.0f, 0.0f, 0.0f);
    Vector3 pointA(1.0f, 2.0f, 3.0f);
    Vector3 pointB(4.0f, 5.0f, 6.0f);

    std::cout << "Origin: " << origin << std::endl;
    std::cout << "Point A: " << pointA << std::endl;
    std::cout << "Point B: " << pointB << std::endl;

    // Calculate distances
    float distanceAB = pointA.Distance(pointB);
    float distanceFromOriginA = pointA.Distance(origin);

    std::cout << "\nDistance from A to B: " << distanceAB << std::endl;
    std::cout << "Distance from Origin to A: " << distanceFromOriginA << std::endl;
}

void Lesson_BasicVectors() {
    PrintSeparator("Basic Vectors");

    // Vectors represent direction and magnitude
    Vector3 velocity(10.0f, 0.0f, 0.0f);  // Moving along X axis
    Vector3 gravity(0.0f, -9.8f, 0.0f);    // Downward force

    std::cout << "Velocity: " << velocity << std::endl;
    std::cout << "Gravity: " << gravity << std::endl;

    // Vector length (magnitude)
    std::cout << "\nVelocity magnitude: " << velocity.Length() << std::endl;
    std::cout << "Gravity magnitude: " << gravity.Length() << std::endl;
}

void Lesson_VectorAddition() {
    PrintSeparator("Vector Addition");

    // Vector addition combines forces/movements
    Vector3 windForce(5.0f, 0.0f, 2.0f);
    Vector3 currentForce(0.0f, 0.0f, 3.0f);

    Vector3 totalForce = windForce + currentForce;

    std::cout << "Wind Force: " << windForce << std::endl;
    std::cout << "Current Force: " << currentForce << std::endl;
    std::cout << "Total Force: " << totalForce << std::endl;
}

void Lesson_VectorScaling() {
    PrintSeparator("Vector Scaling");

    // Scaling changes magnitude without changing direction
    Vector3 direction(1.0f, 0.0f, 0.0f);

    std::cout << "Original direction: " << direction << std::endl;
    std::cout << "Scaled by 2: " << direction * 2.0f << std::endl;
    std::cout << "Scaled by 5: " << direction * 5.0f << std::endl;
    std::cout << "Scaled by 0.5: " << direction * 0.5f << std::endl;
}

void Lesson_UnitVectors() {
    PrintSeparator("Unit Vectors (Normalized)");

    // Unit vectors have length 1 and represent pure direction
    Vector3 arbitrary(3.0f, 4.0f, 0.0f);
    Vector3 normalized = arbitrary.Normalized();

    std::cout << "Original vector: " << arbitrary << std::endl;
    std::cout << "Original length: " << arbitrary.Length() << std::endl;
    std::cout << "\nNormalized vector: " << normalized << std::endl;
    std::cout << "Normalized length: " << normalized.Length() << std::endl;

    // Standard unit vectors
    std::cout << "\nStandard Unit Vectors:" << std::endl;
    std::cout << "X-axis (Right): " << Vector3::UnitX() << std::endl;
    std::cout << "Y-axis (Up): " << Vector3::UnitY() << std::endl;
    std::cout << "Z-axis (Forward): " << Vector3::UnitZ() << std::endl;
}

void Lesson_PracticalExample() {
    PrintSeparator("Practical Example: Moving Object");

    // Simulate a moving object
    Vector3 position(0.0f, 0.0f, 0.0f);
    Vector3 velocity(1.0f, 0.5f, 0.0f);
    float deltaTime = 0.016f; // ~60 FPS

    std::cout << "Initial position: " << position << std::endl;
    std::cout << "Velocity: " << velocity << " units/second" << std::endl;
    std::cout << "\nSimulating 10 frames:\n";

    for (int i = 0; i < 10; i++) {
        position = position + velocity * deltaTime;
        std::cout << "Frame " << i + 1 << ": " << position << std::endl;
    }
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "  Lesson 01: Introduction to 3D Math\n";
    std::cout << "===========================================\n";

    Lesson_Points3D();
    Lesson_BasicVectors();
    Lesson_VectorAddition();
    Lesson_VectorScaling();
    Lesson_UnitVectors();
    Lesson_PracticalExample();

    std::cout << "\n===========================================\n";
    std::cout << "  Lesson Complete!\n";
    std::cout << "===========================================\n";

    return 0;
}
