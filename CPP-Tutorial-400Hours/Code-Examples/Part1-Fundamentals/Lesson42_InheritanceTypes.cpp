/*
 * Lesson 42: Inheritance Types - public, protected, private
 * Compile: cl Lesson42_InheritanceTypes.cpp
 */

#include <iostream>
using namespace std;

class Base {
public:
    int publicVar;
protected:
    int protectedVar;
private:
    int privateVar;
public:
    Base() : publicVar(1), protectedVar(2), privateVar(3) {}
};

class PublicDerived : public Base {
public:
    void access() {
        publicVar = 10;      // OK
        protectedVar = 20;   // OK
        // privateVar = 30;  // Error
    }
};

class ProtectedDerived : protected Base {
public:
    void access() {
        publicVar = 10;      // OK (now protected)
        protectedVar = 20;   // OK
    }
};

class PrivateDerived : private Base {
public:
    void access() {
        publicVar = 10;      // OK (now private)
        protectedVar = 20;   // OK (now private)
    }
};

int main() {
    cout << "=== Inheritance Types Demo ===" << endl << endl;

    PublicDerived pd;
    pd.publicVar = 100;  // OK
    cout << "Public inheritance: publicVar accessible" << endl;

    ProtectedDerived prd;
    // prd.publicVar = 100;  // Error: now protected

    PrivateDerived priv;
    // priv.publicVar = 100;  // Error: now private

    return 0;
}
