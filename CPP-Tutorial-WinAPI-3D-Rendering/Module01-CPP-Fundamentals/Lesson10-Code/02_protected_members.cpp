/*
 * Program: Protected Members
 * Description: Public, protected, and private inheritance
 * Compilation: g++ -std=c++17 02_protected_members.cpp -o 02_protected_members
 * Execution: ./02_protected_members
 */

#include <iostream>
using namespace std;

class Base {
private:
    int privateVar;

protected:
    int protectedVar;

public:
    int publicVar;

    Base() : privateVar(1), protectedVar(2), publicVar(3) {}

    void displayBase() {
        cout << "Private: " << privateVar << endl;
        cout << "Protected: " << protectedVar << endl;
        cout << "Public: " << publicVar << endl;
    }
};

class Derived : public Base {
public:
    void displayDerived() {
        // cout << privateVar << endl;    // ERROR - not accessible
        cout << "Protected (accessible): " << protectedVar << endl;
        cout << "Public (accessible): " << publicVar << endl;
    }

    void modifyProtected() {
        protectedVar = 20;  // OK - can modify protected members
    }
};

int main() {
    cout << "=== Protected Members ===" << endl << endl;

    Derived d;
    d.publicVar = 100;  // OK
    // d.protectedVar = 200;  // ERROR - not accessible outside class
    // d.privateVar = 300;    // ERROR

    d.displayBase();
    cout << endl;

    d.displayDerived();
    cout << endl;

    d.modifyProtected();
    d.displayDerived();

    return 0;
}
