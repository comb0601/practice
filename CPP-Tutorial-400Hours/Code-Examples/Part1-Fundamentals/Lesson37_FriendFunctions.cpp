/*
 * Lesson 37: Friend Functions - Accessing Private Members
 * Compile: cl Lesson37_FriendFunctions.cpp
 */

#include <iostream>
using namespace std;

class Box {
private:
    double length, width, height;

public:
    Box(double l, double w, double h) : length(l), width(w), height(h) {}

    friend double calculateVolume(const Box& b);
    friend void displayDimensions(const Box& b);
    friend class BoxAnalyzer;
};

double calculateVolume(const Box& b) {
    return b.length * b.width * b.height;
}

void displayDimensions(const Box& b) {
    cout << "Dimensions: " << b.length << " x " << b.width << " x " << b.height << endl;
}

class BoxAnalyzer {
public:
    static double surfaceArea(const Box& b) {
        return 2 * (b.length * b.width + b.width * b.height + b.height * b.length);
    }
};

class Point {
private:
    int x, y;

public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    friend double distance(const Point& p1, const Point& p2);

    void display() const {
        cout << "(" << x << ", " << y << ")";
    }
};

double distance(const Point& p1, const Point& p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    cout << "=== Friend Functions Demo ===" << endl << endl;

    // FRIEND FUNCTION
    Box box(3, 4, 5);
    displayDimensions(box);
    cout << "Volume: " << calculateVolume(box) << endl;
    cout << "Surface Area: " << BoxAnalyzer::surfaceArea(box) << endl << endl;

    // FRIEND FUNCTION WITH TWO OBJECTS
    Point p1(0, 0), p2(3, 4);
    cout << "Point 1: "; p1.display(); cout << endl;
    cout << "Point 2: "; p2.display(); cout << endl;
    cout << "Distance: " << distance(p1, p2) << endl;

    return 0;
}
