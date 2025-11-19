/*
 * Program: Destructor Basics
 * Description: Understanding destructors and resource cleanup
 * Compilation: g++ -std=c++17 05_destructor_basics.cpp -o 05_destructor_basics
 * Execution: ./05_destructor_basics
 */

#include <iostream>
using namespace std;

class Resource {
private:
    int* data;
    int size;
    int id;
    static int count;

public:
    Resource(int s) : size(s) {
        id = ++count;
        data = new int[size];
        cout << "Constructor: Resource #" << id << " allocated " << size << " ints" << endl;
    }

    ~Resource() {
        cout << "Destructor: Resource #" << id << " released" << endl;
        delete[] data;
    }

    void fill(int value) {
        for (int i = 0; i < size; i++) {
            data[i] = value;
        }
    }
};

int Resource::count = 0;

void function() {
    cout << "\nInside function:" << endl;
    Resource local(5);
    local.fill(10);
    cout << "Function ending..." << endl;
    // Destructor called automatically
}

int main() {
    cout << "=== Destructor Basics ===" << endl << endl;

    cout << "Creating r1:" << endl;
    Resource r1(10);

    cout << "\nCalling function:" << endl;
    function();

    cout << "\nBack in main" << endl;

    {
        cout << "\nEntering inner scope:" << endl;
        Resource r2(3);
        cout << "Leaving inner scope..." << endl;
    }

    cout << "\nProgram ending..." << endl;
    return 0;
}
