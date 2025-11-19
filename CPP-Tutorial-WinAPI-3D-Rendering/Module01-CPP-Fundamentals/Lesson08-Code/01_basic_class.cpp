/*
 * Program: Basic Class
 * Description: Introduction to classes - basic syntax and usage
 * Compilation: g++ -std=c++17 01_basic_class.cpp -o 01_basic_class
 * Execution: ./01_basic_class
 */

#include <iostream>
using namespace std;

// Simple class definition
class Rectangle {
public:
    int width;
    int height;

    int area() {
        return width * height;
    }

    int perimeter() {
        return 2 * (width + height);
    }

    void display() {
        cout << "Rectangle: " << width << "x" << height << endl;
        cout << "Area: " << area() << endl;
        cout << "Perimeter: " << perimeter() << endl;
    }
};

class Circle {
public:
    double radius;

    double area() {
        return 3.14159 * radius * radius;
    }

    double circumference() {
        return 2 * 3.14159 * radius;
    }

    void display() {
        cout << "Circle with radius: " << radius << endl;
        cout << "Area: " << area() << endl;
        cout << "Circumference: " << circumference() << endl;
    }
};

int main() {
    cout << "=== Basic Class ===" << endl << endl;

    // Create object
    cout << "1. Creating Objects:" << endl;
    Rectangle rect;
    rect.width = 10;
    rect.height = 5;

    cout << "Created rectangle" << endl;
    rect.display();
    cout << endl;

    // Multiple objects
    cout << "2. Multiple Objects:" << endl;
    Rectangle rect1, rect2;

    rect1.width = 8;
    rect1.height = 4;

    rect2.width = 12;
    rect2.height = 6;

    cout << "Rectangle 1:" << endl;
    rect1.display();

    cout << "\nRectangle 2:" << endl;
    rect2.display();
    cout << endl;

    // Circle
    cout << "3. Circle Class:" << endl;
    Circle circle;
    circle.radius = 5.0;
    circle.display();
    cout << endl;

    // Array of objects
    cout << "4. Array of Objects:" << endl;
    Circle circles[3];
    circles[0].radius = 1.0;
    circles[1].radius = 2.0;
    circles[2].radius = 3.0;

    for (int i = 0; i < 3; i++) {
        cout << "Circle " << i + 1 << ": radius = " << circles[i].radius
             << ", area = " << circles[i].area() << endl;
    }

    return 0;
}
