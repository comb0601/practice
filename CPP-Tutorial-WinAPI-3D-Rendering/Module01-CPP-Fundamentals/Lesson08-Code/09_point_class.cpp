/*
 * Program: Point Class
 * Description: 2D point with distance and operations
 * Compilation: g++ -std=c++17 09_point_class.cpp -o 09_point_class
 * Execution: ./09_point_class
 */

#include <iostream>
#include <cmath>
using namespace std;

class Point {
private:
    double x, y;

public:
    Point() : x(0), y(0) {}
    Point(double xVal, double yVal) : x(xVal), y(yVal) {}

    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double xVal) { x = xVal; }
    void setY(double yVal) { y = yVal; }

    double distanceFromOrigin() const {
        return sqrt(x * x + y * y);
    }

    double distanceTo(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    void translate(double dx, double dy) {
        x += dx;
        y += dy;
    }

    void display() const {
        cout << "(" << x << ", " << y << ")";
    }
};

int main() {
    cout << "=== Point Class ===" << endl << endl;

    Point p1(3, 4);
    Point p2(6, 8);

    cout << "Point 1: ";
    p1.display();
    cout << endl;

    cout << "Point 2: ";
    p2.display();
    cout << endl;

    cout << "Distance from origin: " << p1.distanceFromOrigin() << endl;
    cout << "Distance between points: " << p1.distanceTo(p2) << endl;

    p1.translate(1, 1);
    cout << "After translate(1,1): ";
    p1.display();
    cout << endl;

    return 0;
}
