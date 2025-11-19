/*
 * Lesson 26: References - Aliases for Variables
 * Compile: cl Lesson26_References.cpp
 */

#include <iostream>
using namespace std;

void modifyByReference(int &ref) {
    ref = 100;
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    cout << "=== References Demo ===" << endl << endl;

    // BASIC REFERENCE
    int num = 42;
    int &ref = num;  // ref is an alias for num

    cout << "num: " << num << endl;
    cout << "ref: " << ref << endl;
    cout << "Address of num: " << &num << endl;
    cout << "Address of ref: " << &ref << endl << endl;

    // MODIFYING VIA REFERENCE
    ref = 100;
    cout << "After ref = 100:" << endl;
    cout << "num: " << num << endl << endl;

    // PASS BY REFERENCE
    int value = 50;
    cout << "Before: " << value << endl;
    modifyByReference(value);
    cout << "After: " << value << endl << endl;

    // SWAP
    int x = 10, y = 20;
    cout << "Before swap: x=" << x << ", y=" << y << endl;
    swap(x, y);
    cout << "After swap: x=" << x << ", y=" << y << endl << endl;

    // CONST REFERENCE
    const int &constRef = num;
    cout << "Const reference: " << constRef << endl;
    // constRef = 200; // Error: cannot modify

    return 0;
}
