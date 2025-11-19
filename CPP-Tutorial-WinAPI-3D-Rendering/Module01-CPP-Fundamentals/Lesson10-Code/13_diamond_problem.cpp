/*
 * Program: Diamond Problem
 * Description: Diamond problem and virtual inheritance
 * Compilation: g++ -std=c++17 13_diamond_problem.cpp -o 13_diamond_problem
 * Execution: ./13_diamond_problem
 */

#include <iostream>
using namespace std;

// Without virtual inheritance - diamond problem
class A {
public:
    int value;
    A() : value(10) {
        cout << "A constructor" << endl;
    }
};

class B : virtual public A {
public:
    B() {
        cout << "B constructor" << endl;
    }
};

class C : virtual public A {
public:
    C() {
        cout << "C constructor" << endl;
    }
};

class D : public B, public C {
public:
    D() {
        cout << "D constructor" << endl;
    }
};

int main() {
    cout << "=== Diamond Problem (Virtual Inheritance) ===" << endl << endl;

    D d;
    cout << "Value from A: " << d.value << endl;  // No ambiguity with virtual inheritance
    // Without virtual, would need: d.B::value or d.C::value

    return 0;
}
