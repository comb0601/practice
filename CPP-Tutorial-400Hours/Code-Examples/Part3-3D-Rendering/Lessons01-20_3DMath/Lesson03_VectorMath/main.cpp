/*
 * Lesson 03: Vector Mathematics Deep Dive
 *
 * Topics: Advanced vector operations, practical applications
 */

#include <iostream>
#include <iomanip>
#include "../../Common/Math3D/Vector3.h"

using namespace Math3D;

void PrintHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
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
    std::cout << "\nProjection of A onto B: " << projection << std::endl;
    std::cout << "Magnitude: " << projection.Length() << std::endl;
}

void Demonstrate_VectorReflection() {
    PrintHeader("Vector Reflection");

    Vector3 incident(1.0f, -1.0f, 0.0f);
    Vector3 normal(0.0f, 1.0f, 0.0f);

    std::cout << "Incident vector: " << incident << std::endl;
    std::cout << "Normal: " << normal << std::endl;

    Vector3 reflected = incident.Reflect(normal);
    std::cout << "\nReflected vector: " << reflected << std::endl;
}

void Demonstrate_VectorDistance() {
    PrintHeader("Distance Calculations");

    Vector3 pointA(0.0f, 0.0f, 0.0f);
    Vector3 pointB(3.0f, 4.0f, 0.0f);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Point A: " << pointA << std::endl;
    std::cout << "Point B: " << pointB << std::endl;
    std::cout << "\nDistance: " << pointA.Distance(pointB) << std::endl;
    std::cout << "Distance Squared: " << pointA.DistanceSquared(pointB) << std::endl;
    std::cout << "\n(Distance squared is faster for comparisons)" << std::endl;
}

void Demonstrate_VectorInterpolation() {
    PrintHeader("Vector Interpolation");

    Vector3 start(0.0f, 0.0f, 0.0f);
    Vector3 end(10.0f, 10.0f, 10.0f);

    std::cout << "Start: " << start << std::endl;
    std::cout << "End: " << end << std::endl;
    std::cout << "\nInterpolation steps:" << std::endl;

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
    std::cout << "\nSimulation (dt = " << dt << "):" << std::endl;

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

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
