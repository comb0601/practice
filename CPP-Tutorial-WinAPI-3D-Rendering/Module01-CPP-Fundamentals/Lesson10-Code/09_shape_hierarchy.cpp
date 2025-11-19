/*
 * Program: Shape Hierarchy
 * Description: Complete shape hierarchy with polymorphism
 * Compilation: g++ -std=c++17 09_shape_hierarchy.cpp -o 09_shape_hierarchy
 * Execution: ./09_shape_hierarchy
 */

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void display() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;
    static constexpr double PI = 3.14159265359;

public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return PI * radius * radius;
    }

    double perimeter() const override {
        return 2 * PI * radius;
    }

    void display() const override {
        cout << "Circle (r=" << radius << "): "
             << "Area=" << area() << ", Perimeter=" << perimeter() << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void display() const override {
        cout << "Rectangle (" << width << "x" << height << "): "
             << "Area=" << area() << ", Perimeter=" << perimeter() << endl;
    }
};

class Triangle : public Shape {
private:
    double a, b, c;

public:
    Triangle(double s1, double s2, double s3) : a(s1), b(s2), c(s3) {}

    double area() const override {
        double s = (a + b + c) / 2;
        return sqrt(s * (s-a) * (s-b) * (s-c));
    }

    double perimeter() const override {
        return a + b + c;
    }

    void display() const override {
        cout << "Triangle (" << a << "," << b << "," << c << "): "
             << "Area=" << area() << ", Perimeter=" << perimeter() << endl;
    }
};

int main() {
    cout << "=== Shape Hierarchy ===" << endl << endl;

    vector<Shape*> shapes;
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Rectangle(4.0, 6.0));
    shapes.push_back(new Triangle(3.0, 4.0, 5.0));

    double totalArea = 0;
    for (auto shape : shapes) {
        shape->display();
        totalArea += shape->area();
    }

    cout << "\nTotal area: " << totalArea << endl;

    for (auto shape : shapes) {
        delete shape;
    }

    return 0;
}
