/*
 * Program: Default Constructor
 * Description: Understanding default constructors
 * Compilation: g++ -std=c++17 01_default_constructor.cpp -o 01_default_constructor
 * Execution: ./01_default_constructor
 */

#include <iostream>
using namespace std;

class Box {
private:
    int width, height, depth;

public:
    // Default constructor
    Box() {
        width = 1;
        height = 1;
        depth = 1;
        cout << "Default constructor called" << endl;
    }

    void display() {
        cout << "Box: " << width << "x" << height << "x" << depth << endl;
    }

    int getVolume() {
        return width * height * depth;
    }
};

class Point {
public:
    int x, y;

    // Compiler-generated default constructor used if not defined
    Point() : x(0), y(0) {
        cout << "Point default constructor" << endl;
    }

    void display() {
        cout << "Point(" << x << ", " << y << ")" << endl;
    }
};

int main() {
    cout << "=== Default Constructor ===" << endl << endl;

    // Default constructor called
    cout << "Creating box1:" << endl;
    Box box1;
    box1.display();
    cout << "Volume: " << box1.getVolume() << endl << endl;

    // Array of objects - default constructor called for each
    cout << "Creating array of 3 boxes:" << endl;
    Box boxes[3];

    cout << "\nDisplaying boxes:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "Box " << i << ": ";
        boxes[i].display();
    }

    cout << "\nCreating point:" << endl;
    Point p;
    p.display();

    return 0;
}
