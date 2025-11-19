/*
 * Program: Upcasting and Downcasting
 * Description: Converting between base and derived class pointers
 * Compilation: g++ -std=c++17 12_upcasting_downcasting.cpp -o 12_upcasting_downcasting
 * Execution: ./12_upcasting_downcasting
 */

#include <iostream>
using namespace std;

class Base {
public:
    virtual void display() {
        cout << "Base class" << endl;
    }

    virtual ~Base() {}
};

class Derived : public Base {
public:
    void display() override {
        cout << "Derived class" << endl;
    }

    void derivedOnly() {
        cout << "Derived-specific method" << endl;
    }
};

int main() {
    cout << "=== Upcasting and Downcasting ===" << endl << endl;

    // Upcasting (safe, implicit)
    cout << "1. Upcasting:" << endl;
    Derived derived;
    Base* basePtr = &derived;  // Upcast
    basePtr->display();  // Calls Derived::display() (polymorphism)
    cout << endl;

    // Downcasting (requires explicit cast)
    cout << "2. Downcasting (dynamic_cast):" << endl;
    Base* bp = new Derived();

    Derived* dp = dynamic_cast<Derived*>(bp);
    if (dp) {
        cout << "Successful downcast" << endl;
        dp->derivedOnly();
    } else {
        cout << "Failed downcast" << endl;
    }
    cout << endl;

    // Failed downcast
    cout << "3. Failed Downcast:" << endl;
    Base* bp2 = new Base();
    Derived* dp2 = dynamic_cast<Derived*>(bp2);

    if (dp2) {
        cout << "Successful downcast" << endl;
    } else {
        cout << "Failed downcast (bp2 is not actually Derived)" << endl;
    }

    delete bp;
    delete bp2;

    return 0;
}
