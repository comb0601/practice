/*
 * Program: Override and Final
 * Description: C++11 override and final keywords
 * Compilation: g++ -std=c++17 07_override_final.cpp -o 07_override_final
 * Execution: ./07_override_final
 */

#include <iostream>
using namespace std;

class Base {
public:
    virtual void func1() {
        cout << "Base::func1()" << endl;
    }

    virtual void func2() final {
        cout << "Base::func2() - final" << endl;
    }

    virtual ~Base() {}
};

class Derived : public Base {
public:
    void func1() override {  // override - catches errors
        cout << "Derived::func1()" << endl;
    }

    // void func2() override {  // ERROR - cannot override final
    //     cout << "Derived::func2()" << endl;
    // }
};

class FinalClass final {  // Cannot be inherited
public:
    void display() {
        cout << "Final class" << endl;
    }
};

// class CannotDerive : public FinalClass {  // ERROR
// };

int main() {
    cout << "=== Override and Final ===" << endl << endl;

    Base* ptr = new Derived();
    ptr->func1();
    ptr->func2();

    delete ptr;

    FinalClass fc;
    fc.display();

    return 0;
}
