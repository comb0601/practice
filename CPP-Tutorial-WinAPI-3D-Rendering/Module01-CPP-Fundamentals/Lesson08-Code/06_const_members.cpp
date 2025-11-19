/*
 * Program: Const Member Functions
 * Description: Using const with member functions
 * Compilation: g++ -std=c++17 06_const_members.cpp -o 06_const_members
 * Execution: ./06_const_members
 */

#include <iostream>
using namespace std;

class Point {
private:
    int x, y;

public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    // Const member functions - promise not to modify object
    int getX() const { return x; }
    int getY() const { return y; }

    void display() const {
        cout << "(" << x << ", " << y << ")" << endl;
        // x = 0;  // ERROR - cannot modify in const function
    }

    // Non-const functions
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
};

void printPoint(const Point& p) {  // const reference parameter
    // p.setX(0);  // ERROR - cannot call non-const method on const object
    p.display();   // OK - display is const
    cout << "X: " << p.getX() << ", Y: " << p.getY() << endl;
}

int main() {
    cout << "=== Const Member Functions ===" << endl << endl;

    // Regular object
    cout << "1. Regular Object:" << endl;
    Point p1(10, 20);
    p1.display();      // OK
    p1.setX(15);       // OK
    p1.display();
    cout << endl;

    // Const object
    cout << "2. Const Object:" << endl;
    const Point p2(30, 40);
    p2.display();      // OK - const function
    // p2.setX(35);    // ERROR - cannot call non-const on const object
    cout << "X: " << p2.getX() << endl;  // OK - const function
    cout << endl;

    // Function with const reference
    cout << "3. Const Reference Parameter:" << endl;
    printPoint(p1);

    return 0;
}
