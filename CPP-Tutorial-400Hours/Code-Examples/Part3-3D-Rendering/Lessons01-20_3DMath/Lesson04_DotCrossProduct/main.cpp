/*
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
    std::cout << "\n" << std::string(60, '=') << std::endl;
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

    std::cout << "\nDot products:" << std::endl;
    std::cout << "a · b (parallel, same direction) = " << a.Dot(b) << std::endl;
    std::cout << "a · c (perpendicular) = " << a.Dot(c) << std::endl;
    std::cout << "a · d (parallel, opposite direction) = " << a.Dot(d) << std::endl;

    std::cout << "\nInterpretation:" << std::endl;
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
    std::cout << "\nDot product (normalized): " << dot << std::endl;
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

    std::cout << "\nCross products (right-hand rule):" << std::endl;
    std::cout << "X × Y = " << x.Cross(y) << " (should be Z)" << std::endl;
    std::cout << "Y × Z = " << y.Cross(z) << " (should be X)" << std::endl;
    std::cout << "Z × X = " << z.Cross(x) << " (should be Y)" << std::endl;

    std::cout << "\nReverse order (notice negation):" << std::endl;
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
    std::cout << "\nVerification (dot product): " << dot << std::endl;
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

    std::cout << "\nEdge 1: " << edge1 << std::endl;
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

    std::cout << "\nLight intensity: " << intensity << std::endl;
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

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
