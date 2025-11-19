/*
 * Program: Static Members
 * Description: Static member variables and functions
 * Compilation: g++ -std=c++17 05_static_members.cpp -o 05_static_members
 * Execution: ./05_static_members
 */

#include <iostream>
using namespace std;

class Counter {
private:
    static int objectCount;  // Shared by all objects
    int id;

public:
    Counter() {
        id = ++objectCount;
        cout << "Object " << id << " created" << endl;
    }

    ~Counter() {
        cout << "Object " << id << " destroyed" << endl;
        objectCount--;
    }

    static int getCount() {  // Static method
        return objectCount;
    }

    int getId() {
        return id;
    }
};

// Initialize static member
int Counter::objectCount = 0;

class Math {
public:
    static double PI;
    static double E;

    static double square(double x) {
        return x * x;
    }

    static double cube(double x) {
        return x * x * x;
    }
};

// Initialize static members
double Math::PI = 3.14159265359;
double Math::E = 2.71828182846;

int main() {
    cout << "=== Static Members ===" << endl << endl;

    // Static counter
    cout << "1. Static Counter:" << endl;
    cout << "Count: " << Counter::getCount() << endl;

    {
        Counter c1;
        cout << "Count: " << Counter::getCount() << endl;

        Counter c2;
        cout << "Count: " << Counter::getCount() << endl;

        Counter c3;
        cout << "Count: " << Counter::getCount() << endl;
    }

    cout << "After scope ends: " << Counter::getCount() << endl;
    cout << endl;

    // Static methods
    cout << "2. Static Methods:" << endl;
    cout << "PI = " << Math::PI << endl;
    cout << "E = " << Math::E << endl;
    cout << "square(5) = " << Math::square(5) << endl;
    cout << "cube(3) = " << Math::cube(3) << endl;

    return 0;
}
