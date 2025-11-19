/* Lesson 69: Move Semantics (C++11)
 * Compile: cl /std:c++17 Lesson69_MoveSemantics.cpp */
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class Buffer {
private:
    int* data;
    size_t size;
public:
    Buffer(size_t s) : size(s) {
        data = new int[size];
        cout << "Constructor" << endl;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Move constructor" << endl;
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "Move assignment" << endl;
        }
        return *this;
    }

    ~Buffer() {
        delete[] data;
        cout << "Destructor" << endl;
    }
};

int main() {
    Buffer b1(100);
    Buffer b2(move(b1));  // Move constructor
    return 0;
}
