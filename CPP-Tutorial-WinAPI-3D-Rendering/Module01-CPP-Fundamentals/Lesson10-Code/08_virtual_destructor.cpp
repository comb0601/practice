/*
 * Program: Virtual Destructor
 * Description: Importance of virtual destructors in polymorphism
 * Compilation: g++ -std=c++17 08_virtual_destructor.cpp -o 08_virtual_destructor
 * Execution: ./08_virtual_destructor
 */

#include <iostream>
using namespace std;

class Base {
public:
    Base() {
        cout << "Base constructor" << endl;
    }

    virtual ~Base() {  // Virtual destructor!
        cout << "Base destructor" << endl;
    }
};

class Derived : public Base {
private:
    int* data;

public:
    Derived() : data(new int[100]) {
        cout << "Derived constructor (allocated memory)" << endl;
    }

    ~Derived() {
        cout << "Derived destructor (freeing memory)" << endl;
        delete[] data;
    }
};

int main() {
    cout << "=== Virtual Destructor ===" << endl << endl;

    cout << "With virtual destructor:" << endl;
    Base* ptr = new Derived();
    delete ptr;  // Both destructors called!

    cout << "\nDirect object:" << endl;
    {
        Derived d;
    }

    return 0;
}
