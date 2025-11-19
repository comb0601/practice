/*
 * Lesson 33: Member Functions - Methods
 * Compile: cl Lesson33_MemberFunctions.cpp
 */

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Calculator {
public:
    // Simple member functions
    int add(int a, int b) {
        return a + b;
    }

    int subtract(int a, int b) {
        return a - b;
    }

    double power(double base, int exp) {
        return pow(base, exp);
    }

    // Member function with default parameter
    double divide(double a, double b, bool showResult = true) {
        if (b == 0) {
            if (showResult) cout << "Error: Division by zero!" << endl;
            return 0;
        }
        double result = a / b;
        if (showResult) cout << a << " / " << b << " = " << result << endl;
        return result;
    }
};

class Circle {
private:
    double radius;
    static constexpr double PI = 3.14159;

public:
    Circle(double r) : radius(r) {}

    // Inline member function
    double getRadius() const { return radius; }

    void setRadius(double r) {
        if (r > 0) radius = r;
    }

    // Const member function
    double area() const {
        return PI * radius * radius;
    }

    double circumference() const {
        return 2 * PI * radius;
    }

    void display() const {
        cout << "Circle (r=" << radius << ")" << endl;
        cout << "Area: " << area() << endl;
        cout << "Circumference: " << circumference() << endl;
    }
};

class Box {
private:
    double length, width, height;

public:
    Box(double l, double w, double h) : length(l), width(w), height(h) {}

    // Member function returning object
    Box scale(double factor) {
        return Box(length * factor, width * factor, height * factor);
    }

    double volume() const {
        return length * width * height;
    }

    void display() const {
        cout << "Box: " << length << "x" << width << "x" << height;
        cout << " (Volume: " << volume() << ")" << endl;
    }
};

int main() {
    cout << "=== Member Functions Demo ===" << endl << endl;

    // CALCULATOR
    cout << "=== CALCULATOR ===" << endl;
    Calculator calc;
    cout << "10 + 5 = " << calc.add(10, 5) << endl;
    cout << "10 - 5 = " << calc.subtract(10, 5) << endl;
    cout << "2^8 = " << calc.power(2, 8) << endl;
    calc.divide(10, 2);
    cout << endl;

    // CIRCLE
    cout << "=== CIRCLE ===" << endl;
    Circle c(5.0);
    c.display();
    cout << "\nAfter setting radius to 10:" << endl;
    c.setRadius(10.0);
    c.display();
    cout << endl;

    // BOX
    cout << "=== BOX ===" << endl;
    Box box1(2, 3, 4);
    box1.display();

    Box box2 = box1.scale(2);
    cout << "Scaled box:" << endl;
    box2.display();

    return 0;
}
