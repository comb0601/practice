/*
 * Program: Constructor Overloading
 * Description: Multiple constructors with different parameters
 * Compilation: g++ -std=c++17 03_constructor_overloading.cpp -o 03_constructor_overloading
 * Execution: ./03_constructor_overloading
 */

#include <iostream>
#include <string>
using namespace std;

class Box {
private:
    double width, height, depth;

public:
    // Default constructor
    Box() : width(1), height(1), depth(1) {
        cout << "Default constructor" << endl;
    }

    // Constructor with one parameter (cube)
    Box(double size) : width(size), height(size), depth(size) {
        cout << "Cube constructor: " << size << endl;
    }

    // Constructor with three parameters
    Box(double w, double h, double d) : width(w), height(h), depth(d) {
        cout << "Full constructor: " << w << "x" << h << "x" << d << endl;
    }

    double volume() {
        return width * height * depth;
    }

    void display() {
        cout << "Box: " << width << "x" << height << "x" << depth
             << ", Volume: " << volume() << endl;
    }
};

int main() {
    cout << "=== Constructor Overloading ===" << endl << endl;

    cout << "Creating box1 (default):" << endl;
    Box box1;
    box1.display();

    cout << "\nCreating box2 (cube):" << endl;
    Box box2(5);
    box2.display();

    cout << "\nCreating box3 (custom):" << endl;
    Box box3(10, 5, 3);
    box3.display();

    return 0;
}
