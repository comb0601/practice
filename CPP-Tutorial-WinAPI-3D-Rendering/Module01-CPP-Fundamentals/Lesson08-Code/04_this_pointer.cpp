/*
 * Program: This Pointer
 * Description: Understanding and using the 'this' pointer
 * Compilation: g++ -std=c++17 04_this_pointer.cpp -o 04_this_pointer
 * Execution: ./04_this_pointer
 */

#include <iostream>
using namespace std;

class Box {
private:
    double width, height, depth;

public:
    // Using 'this' to resolve name conflict
    void setDimensions(double width, double height, double depth) {
        this->width = width;    // this->width is member, width is parameter
        this->height = height;
        this->depth = depth;
    }

    // Method chaining using 'this'
    Box& setWidth(double width) {
        this->width = width;
        return *this;  // Return reference to current object
    }

    Box& setHeight(double height) {
        this->height = height;
        return *this;
    }

    Box& setDepth(double depth) {
        this->depth = depth;
        return *this;
    }

    double getVolume() {
        return width * height * depth;
    }

    void display() {
        cout << "Box: " << width << "x" << height << "x" << depth << endl;
        cout << "Volume: " << getVolume() << endl;
    }

    // Comparing with another object
    bool isLargerThan(const Box& other) {
        return this->getVolume() > other.getVolume();
    }
};

int main() {
    cout << "=== This Pointer ===" << endl << endl;

    // Basic usage
    cout << "1. Basic Usage:" << endl;
    Box box1;
    box1.setDimensions(10, 5, 3);
    box1.display();
    cout << endl;

    // Method chaining
    cout << "2. Method Chaining:" << endl;
    Box box2;
    box2.setWidth(8).setHeight(4).setDepth(2);  // Chained calls!
    box2.display();
    cout << endl;

    // Comparison
    cout << "3. Object Comparison:" << endl;
    cout << "box1 is larger than box2: "
         << (box1.isLargerThan(box2) ? "Yes" : "No") << endl;

    return 0;
}
