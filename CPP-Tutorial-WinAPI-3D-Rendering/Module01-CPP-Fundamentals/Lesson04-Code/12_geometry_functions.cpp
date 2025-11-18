/*
 * Program: Geometry Functions
 * Description: Functions for calculating areas, perimeters, and volumes
 * Compilation: g++ 12_geometry_functions.cpp -o 12_geometry_functions
 * Execution: ./12_geometry_functions
 */

#include <iostream>
#include <cmath>
using namespace std;

const double PI = 3.14159265359;

// Circle
double circleArea(double radius) {
    return PI * radius * radius;
}

double circleCircumference(double radius) {
    return 2 * PI * radius;
}

// Rectangle
double rectangleArea(double length, double width) {
    return length * width;
}

double rectanglePerimeter(double length, double width) {
    return 2 * (length + width);
}

double rectangleDiagonal(double length, double width) {
    return sqrt(length * length + width * width);
}

// Triangle
double triangleArea(double base, double height) {
    return 0.5 * base * height;
}

double trianglePerimeter(double a, double b, double c) {
    return a + b + c;
}

// Sphere
double sphereVolume(double radius) {
    return (4.0 / 3.0) * PI * radius * radius * radius;
}

double sphereSurfaceArea(double radius) {
    return 4 * PI * radius * radius;
}

// Cube
double cubeVolume(double side) {
    return side * side * side;
}

double cubeSurfaceArea(double side) {
    return 6 * side * side;
}

// Cylinder
double cylinderVolume(double radius, double height) {
    return PI * radius * radius * height;
}

double cylinderSurfaceArea(double radius, double height) {
    return 2 * PI * radius * (radius + height);
}

// Cone
double coneVolume(double radius, double height) {
    return (1.0 / 3.0) * PI * radius * radius * height;
}

double coneSurfaceArea(double radius, double height) {
    double slantHeight = sqrt(radius * radius + height * height);
    return PI * radius * (radius + slantHeight);
}

int main() {
    cout << "=== Geometry Functions ===" << endl << endl;

    // Circle
    cout << "1. Circle (radius = 5):" << endl;
    cout << "   Area: " << circleArea(5) << endl;
    cout << "   Circumference: " << circleCircumference(5) << endl << endl;

    // Rectangle
    cout << "2. Rectangle (4 x 6):" << endl;
    cout << "   Area: " << rectangleArea(4, 6) << endl;
    cout << "   Perimeter: " << rectanglePerimeter(4, 6) << endl;
    cout << "   Diagonal: " << rectangleDiagonal(4, 6) << endl << endl;

    // Triangle
    cout << "3. Triangle (base=6, height=4, sides=3,4,5):" << endl;
    cout << "   Area: " << triangleArea(6, 4) << endl;
    cout << "   Perimeter: " << trianglePerimeter(3, 4, 5) << endl << endl;

    // Sphere
    cout << "4. Sphere (radius = 3):" << endl;
    cout << "   Volume: " << sphereVolume(3) << endl;
    cout << "   Surface Area: " << sphereSurfaceArea(3) << endl << endl;

    // Cube
    cout << "5. Cube (side = 4):" << endl;
    cout << "   Volume: " << cubeVolume(4) << endl;
    cout << "   Surface Area: " << cubeSurfaceArea(4) << endl << endl;

    // Cylinder
    cout << "6. Cylinder (radius=3, height=7):" << endl;
    cout << "   Volume: " << cylinderVolume(3, 7) << endl;
    cout << "   Surface Area: " << cylinderSurfaceArea(3, 7) << endl << endl;

    // Cone
    cout << "7. Cone (radius=4, height=6):" << endl;
    cout << "   Volume: " << coneVolume(4, 6) << endl;
    cout << "   Surface Area: " << coneSurfaceArea(4, 6) << endl;

    return 0;
}
