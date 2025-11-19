/*
 * Program: Placement New
 * Description: Using placement new to construct objects at specific memory locations
 * Compilation: g++ -std=c++17 08_placement_new.cpp -o 08_placement_new
 * Execution: ./08_placement_new
 */

#include <iostream>
#include <new>  // For placement new
using namespace std;

class Point {
private:
    int x, y;
public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) {
        cout << "Point(" << x << ", " << y << ") constructed" << endl;
    }

    ~Point() {
        cout << "Point(" << x << ", " << y << ") destructed" << endl;
    }

    void display() {
        cout << "Point(" << x << ", " << y << ")" << endl;
    }
};

int main() {
    cout << "=== Placement New ===" << endl << endl;

    // Regular new
    cout << "1. Regular New:" << endl;
    Point* p1 = new Point(10, 20);
    p1->display();
    delete p1;
    cout << endl;

    // Placement new - construct at pre-allocated memory
    cout << "2. Placement New:" << endl;
    char buffer[sizeof(Point)];  // Pre-allocated memory
    cout << "Buffer allocated at: " << (void*)buffer << endl;

    // Construct object in buffer
    Point* p2 = new (buffer) Point(30, 40);
    cout << "Point constructed at: " << (void*)p2 << endl;
    p2->display();

    // Must manually call destructor!
    p2->~Point();
    // Don't delete p2! Memory not from heap
    cout << endl;

    // Array with placement new
    cout << "3. Array with Placement New:" << endl;
    const int SIZE = 3;
    char arrayBuffer[SIZE * sizeof(Point)];

    Point* arr = (Point*)arrayBuffer;

    for (int i = 0; i < SIZE; i++) {
        new (&arr[i]) Point(i * 10, i * 20);
    }

    for (int i = 0; i < SIZE; i++) {
        arr[i].display();
    }

    // Manually destruct in reverse order
    for (int i = SIZE - 1; i >= 0; i--) {
        arr[i].~Point();
    }

    return 0;
}
