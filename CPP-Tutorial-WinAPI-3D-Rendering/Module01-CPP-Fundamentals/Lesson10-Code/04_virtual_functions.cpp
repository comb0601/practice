/*
 * Program: Virtual Functions
 * Description: Virtual functions and runtime polymorphism
 * Compilation: g++ -std=c++17 04_virtual_functions.cpp -o 04_virtual_functions
 * Execution: ./04_virtual_functions
 */

#include <iostream>
using namespace std;

class Shape {
protected:
    string name;

public:
    Shape(const string& n) : name(n) {}

    virtual double area() {
        cout << "Shape::area() called" << endl;
        return 0;
    }

    virtual void display() {
        cout << "Shape: " << name << endl;
    }

    virtual ~Shape() {
        cout << "Shape destructor" << endl;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : Shape("Circle"), radius(r) {}

    double area() override {
        return 3.14159 * radius * radius;
    }

    void display() override {
        cout << "Circle with radius: " << radius << endl;
    }

    ~Circle() {
        cout << "Circle destructor" << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : Shape("Rectangle"), width(w), height(h) {}

    double area() override {
        return width * height;
    }

    void display() override {
        cout << "Rectangle: " << width << "x" << height << endl;
    }

    ~Rectangle() {
        cout << "Rectangle destructor" << endl;
    }
};

int main() {
    cout << "=== Virtual Functions ===" << endl << endl;

    Circle c(5.0);
    Rectangle r(4.0, 6.0);

    // Polymorphism through base class pointers
    Shape* shapes[] = {&c, &r};

    for (int i = 0; i < 2; i++) {
        shapes[i]->display();
        cout << "Area: " << shapes[i]->area() << endl;
        cout << endl;
    }

    return 0;
}
