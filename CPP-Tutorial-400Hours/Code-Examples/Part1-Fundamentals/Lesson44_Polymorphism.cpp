/* Lesson 44: Polymorphism - Virtual Functions
 * Compile: cl Lesson44_Polymorphism.cpp */
#include <iostream>
using namespace std;

class Shape {
public:
    virtual double area() { return 0; }
    virtual void draw() { cout << "Drawing Shape" << endl; }
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() override { return 3.14159 * radius * radius; }
    void draw() override { cout << "Drawing Circle" << endl; }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() override { return width * height; }
    void draw() override { cout << "Drawing Rectangle" << endl; }
};

int main() {
    Shape *shapes[3];
    shapes[0] = new Circle(5);
    shapes[1] = new Rectangle(4, 6);
    shapes[2] = new Circle(3);

    for (int i = 0; i < 3; i++) {
        shapes[i]->draw();
        cout << "Area: " << shapes[i]->area() << endl << endl;
        delete shapes[i];
    }
    return 0;
}
