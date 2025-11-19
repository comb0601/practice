/*
 * Program: Circle Class
 * Description: Circle with area, circumference, and properties
 * Compilation: g++ -std=c++17 10_circle_class.cpp -o 10_circle_class
 * Execution: ./10_circle_class
 */

#include <iostream>
#include <cmath>
using namespace std;

class Circle {
private:
    double radius;
    static constexpr double PI = 3.14159265359;

public:
    Circle() : radius(1.0) {}
    Circle(double r) : radius(r > 0 ? r : 1.0) {}

    double getRadius() const { return radius; }

    void setRadius(double r) {
        if (r > 0) radius = r;
    }

    double getDiameter() const {
        return 2 * radius;
    }

    double getCircumference() const {
        return 2 * PI * radius;
    }

    double getArea() const {
        return PI * radius * radius;
    }

    void scale(double factor) {
        if (factor > 0) {
            radius *= factor;
        }
    }

    void display() const {
        cout << "Circle:" << endl;
        cout << "  Radius: " << radius << endl;
        cout << "  Diameter: " << getDiameter() << endl;
        cout << "  Circumference: " << getCircumference() << endl;
        cout << "  Area: " << getArea() << endl;
    }
};

int main() {
    cout << "=== Circle Class ===" << endl << endl;

    Circle c1(5.0);
    c1.display();

    cout << "\nAfter scaling by 2:" << endl;
    c1.scale(2);
    c1.display();

    return 0;
}
