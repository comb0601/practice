/*
 * Lesson 28: Pointer vs Reference - Comparison
 * Compile: cl Lesson28_PointerVsReference.cpp
 */

#include <iostream>
using namespace std;

void modifyViaPointer(int *ptr) {
    if (ptr != nullptr) {
        *ptr = 100;
    }
}

void modifyViaReference(int &ref) {
    ref = 200;
}

int main() {
    cout << "=== Pointer vs Reference Demo ===" << endl << endl;

    // INITIALIZATION
    cout << "=== INITIALIZATION ===" << endl;
    int num = 42;

    int *ptr = &num;  // Pointer needs address
    int &ref = num;   // Reference is direct alias

    cout << "Via pointer: " << *ptr << endl;
    cout << "Via reference: " << ref << endl << endl;

    // REASSIGNMENT
    cout << "=== REASSIGNMENT ===" << endl;
    int other = 100;

    ptr = &other;  // Pointer can be reassigned
    cout << "Pointer after reassignment: " << *ptr << endl;

    // ref = other; // This assigns value, not reference!
    cout << "Reference still refers to original: " << ref << endl << endl;

    // NULL CHECK
    cout << "=== NULL CHECK ===" << endl;
    int *nullPtr = nullptr;
    if (nullPtr == nullptr) {
        cout << "Pointer can be null" << endl;
    }
    // Reference cannot be null
    cout << endl;

    // FUNCTION CALLS
    cout << "=== FUNCTION CALLS ===" << endl;
    int val1 = 10, val2 = 20;

    modifyViaPointer(&val1);
    modifyViaReference(val2);

    cout << "After pointer function: " << val1 << endl;
    cout << "After reference function: " << val2 << endl << endl;

    // COMPARISON
    cout << "=== COMPARISON ===" << endl;
    cout << "Pointer: Can be null, reassignable, needs dereferencing" << endl;
    cout << "Reference: Cannot be null, not reassignable, direct access" << endl;

    return 0;
}
