/*
 * Lesson 13: Function Overloading - Same Name, Different Parameters
 *
 * Demonstrates function overloading with different parameter types/counts
 *
 * Compile: cl Lesson13_Overloading.cpp
 */

#include <iostream>
#include <string>
using namespace std;

// Overloaded print functions
void print(int x) {
    cout << "Integer: " << x << endl;
}

void print(double x) {
    cout << "Double: " << x << endl;
}

void print(string x) {
    cout << "String: " << x << endl;
}

void print(int x, int y) {
    cout << "Two integers: " << x << ", " << y << endl;
}

// Overloaded add functions
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

int add(int a, int b, int c) {
    return a + b + c;
}

// Overloaded area functions
double area(double side) {  // Square
    return side * side;
}

double area(double length, double width) {  // Rectangle
    return length * width;
}

double area(double radius, bool isCircle) {  // Circle
    const double PI = 3.14159;
    return PI * radius * radius;
}

int main() {
    cout << "=== Function Overloading Demo ===" << endl << endl;

    // OVERLOADED PRINT
    cout << "=== OVERLOADED PRINT ===" << endl;
    print(42);
    print(3.14);
    print("Hello");
    print(10, 20);
    cout << endl;

    // OVERLOADED ADD
    cout << "=== OVERLOADED ADD ===" << endl;
    cout << "add(5, 3) = " << add(5, 3) << endl;
    cout << "add(2.5, 3.7) = " << add(2.5, 3.7) << endl;
    cout << "add(1, 2, 3) = " << add(1, 2, 3) << endl << endl;

    // OVERLOADED AREA
    cout << "=== OVERLOADED AREA ===" << endl;
    cout << "Square (side=5): " << area(5.0) << endl;
    cout << "Rectangle (4x6): " << area(4.0, 6.0) << endl;
    cout << "Circle (r=3): " << area(3.0, true) << endl;

    return 0;
}
