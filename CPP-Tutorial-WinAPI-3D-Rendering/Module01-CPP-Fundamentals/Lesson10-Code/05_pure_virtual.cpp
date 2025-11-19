/*
 * Program: Pure Virtual Functions
 * Description: Abstract classes and pure virtual functions
 * Compilation: g++ -std=c++17 05_pure_virtual.cpp -o 05_pure_virtual
 * Execution: ./05_pure_virtual
 */

#include <iostream>
using namespace std;

class AbstractShape {
public:
    virtual double area() = 0;  // Pure virtual
    virtual double perimeter() = 0;  // Pure virtual
    virtual void display() = 0;

    virtual ~AbstractShape() {}
};

class Triangle : public AbstractShape {
private:
    double a, b, c;  // sides

public:
    Triangle(double s1, double s2, double s3) : a(s1), b(s2), c(s3) {}

    double area() override {
        double s = (a + b + c) / 2;
        return sqrt(s * (s-a) * (s-b) * (s-c));
    }

    double perimeter() override {
        return a + b + c;
    }

    void display() override {
        cout << "Triangle: sides " << a << ", " << b << ", " << c << endl;
    }
};

class Square : public AbstractShape {
private:
    double side;

public:
    Square(double s) : side(s) {}

    double area() override {
        return side * side;
    }

    double perimeter() override {
        return 4 * side;
    }

    void display() override {
        cout << "Square: side " << side << endl;
    }
};

int main() {
    cout << "=== Pure Virtual Functions ===" << endl << endl;

    // AbstractShape s;  // ERROR - cannot instantiate abstract class

    Triangle t(3, 4, 5);
    Square s(5);

    AbstractShape* shapes[] = {&t, &s};

    for (int i = 0; i < 2; i++) {
        shapes[i]->display();
        cout << "Area: " << shapes[i]->area() << endl;
        cout << "Perimeter: " << shapes[i]->perimeter() << endl;
        cout << endl;
    }

    return 0;
}
