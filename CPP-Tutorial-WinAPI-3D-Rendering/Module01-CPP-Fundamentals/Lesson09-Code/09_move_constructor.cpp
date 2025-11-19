/*
 * Program: Move Constructor
 * Description: Move semantics and move constructor (C++11)
 * Compilation: g++ -std=c++17 09_move_constructor.cpp -o 09_move_constructor
 * Execution: ./09_move_constructor
 */

#include <iostream>
#include <utility>
using namespace std;

class Buffer {
private:
    int* data;
    int size;

public:
    Buffer(int s) : size(s) {
        data = new int[size];
        cout << "Constructor: allocated " << size << " ints" << endl;
    }

    ~Buffer() {
        if (data) {
            cout << "Destructor: deallocating" << endl;
            delete[] data;
        }
    }

    // Copy constructor
    Buffer(const Buffer& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Copy constructor: deep copy" << endl;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Move constructor: transferred ownership" << endl;
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
            cout << "Move assignment: transferred ownership" << endl;
        }
        return *this;
    }

    void fill(int value) {
        for (int i = 0; i < size; i++) {
            data[i] = value;
        }
    }
};

Buffer createBuffer() {
    Buffer temp(100);
    temp.fill(42);
    return temp;  // Move constructor used
}

int main() {
    cout << "=== Move Constructor ===" << endl << endl;

    cout << "Creating b1:" << endl;
    Buffer b1(50);

    cout << "\nCopy construction:" << endl;
    Buffer b2 = b1;  // Copy

    cout << "\nMove construction:" << endl;
    Buffer b3 = move(b1);  // Move (b1 is now empty)

    cout << "\nReturning from function:" << endl;
    Buffer b4 = createBuffer();  // Move optimization

    cout << "\nExiting:" << endl;
    return 0;
}
