/*
 * Lesson 22: Pointer Arithmetic - Moving Through Memory
 * Compile: cl Lesson22_PointerArithmetic.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pointer Arithmetic Demo ===" << endl << endl;

    // ARRAY AND POINTER
    int arr[] = {10, 20, 30, 40, 50};
    int *ptr = arr;  // Points to first element

    cout << "Array elements using pointer arithmetic:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "*(ptr + " << i << ") = " << *(ptr + i) << endl;
    }
    cout << endl;

    // INCREMENT/DECREMENT
    cout << "Pointer increment:" << endl;
    ptr = arr;
    cout << "*ptr = " << *ptr << endl;
    ptr++;
    cout << "After ptr++: *ptr = " << *ptr << endl;
    ptr--;
    cout << "After ptr--: *ptr = " << *ptr << endl << endl;

    // POINTER SUBTRACTION
    int *start = &arr[0];
    int *end = &arr[4];
    cout << "Distance between pointers: " << (end - start) << endl << endl;

    // TRAVERSING ARRAY
    cout << "Traversing with pointer:" << endl;
    for (ptr = arr; ptr < arr + 5; ptr++) {
        cout << *ptr << " ";
    }
    cout << endl << endl;

    // COMPARISON
    int *p1 = &arr[2];
    int *p2 = &arr[4];
    cout << "p1 < p2: " << (p1 < p2) << endl;
    cout << "p1 == p2: " << (p1 == p2) << endl << endl;

    // POINTER AND ADDRESS
    cout << "Addresses:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "arr[" << i << "] at " << (arr + i) << " = " << *(arr + i) << endl;
    }

    return 0;
}
