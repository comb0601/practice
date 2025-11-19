/*
 * Program: Delegating Constructors
 * Description: Constructors calling other constructors (C++11)
 * Compilation: g++ -std=c++17 07_delegating_constructors.cpp -o 07_delegating_constructors
 * Execution: ./07_delegating_constructors
 */

#include <iostream>
#include <string>
using namespace std;

class Rectangle {
private:
    int width, height;
    string color;

public:
    // Main constructor
    Rectangle(int w, int h, string c) : width(w), height(h), color(c) {
        cout << "Main constructor: " << width << "x" << height << " " << color << endl;
    }

    // Delegating constructor - calls main constructor
    Rectangle(int size) : Rectangle(size, size, "white") {
        cout << "Square delegating constructor" << endl;
    }

    // Delegating constructor - default color
    Rectangle(int w, int h) : Rectangle(w, h, "white") {
        cout << "Default color delegating constructor" << endl;
    }

    void display() const {
        cout << color << " rectangle: " << width << "x" << height << endl;
    }
};

int main() {
    cout << "=== Delegating Constructors ===" << endl << endl;

    cout << "Creating rect1 (full parameters):" << endl;
    Rectangle rect1(10, 5, "red");
    rect1.display();

    cout << "\nCreating rect2 (square):" << endl;
    Rectangle rect2(7);
    rect2.display();

    cout << "\nCreating rect3 (default color):" << endl;
    Rectangle rect3(15, 8);
    rect3.display();

    return 0;
}
