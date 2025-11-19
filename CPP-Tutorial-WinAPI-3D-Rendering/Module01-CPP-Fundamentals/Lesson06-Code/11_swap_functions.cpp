/*
 * Program: Swap Functions
 * Description: Different ways to swap values - by value, pointer, reference
 * Compilation: g++ -std=c++17 11_swap_functions.cpp -o 11_swap_functions
 * Execution: ./11_swap_functions
 */

#include <iostream>
using namespace std;

// Wrong: Pass by value - doesn't work
void swapByValue(int a, int b) {
    cout << "  Inside swapByValue:" << endl;
    cout << "    Before: a = " << a << ", b = " << b << endl;

    int temp = a;
    a = b;
    b = temp;

    cout << "    After: a = " << a << ", b = " << b << endl;
    cout << "  (Changes only local copies!)" << endl;
}

// Correct: Pass by pointer
void swapByPointer(int* a, int* b) {
    cout << "  Inside swapByPointer:" << endl;
    cout << "    Before: *a = " << *a << ", *b = " << *b << endl;

    int temp = *a;
    *a = *b;
    *b = temp;

    cout << "    After: *a = " << *a << ", *b = " << *b << endl;
}

// Correct: Pass by reference (modern C++)
void swapByReference(int& a, int& b) {
    cout << "  Inside swapByReference:" << endl;
    cout << "    Before: a = " << a << ", b = " << b << endl;

    int temp = a;
    a = b;
    b = temp;

    cout << "    After: a = " << a << ", b = " << b << endl;
}

// Swap using XOR (no temp variable)
void swapByXOR(int& a, int& b) {
    if (&a != &b) {  // Check they're not the same variable
        a = a ^ b;
        b = a ^ b;
        a = a ^ b;
    }
}

// Swap using arithmetic (no temp variable)
void swapByArithmetic(int& a, int& b) {
    if (&a != &b) {
        a = a + b;
        b = a - b;
        a = a - b;
    }
}

// Generic swap for any type
template<typename T>
void genericSwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Swap array elements
void swapArrayElements(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Swap pointers themselves
void swapPointers(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    cout << "=== Swap Functions ===" << endl << endl;

    // Pass by value - DOESN'T WORK
    cout << "1. Swap by Value (DOESN'T WORK):" << endl;
    int x1 = 10, y1 = 20;
    cout << "Before: x1 = " << x1 << ", y1 = " << y1 << endl;
    swapByValue(x1, y1);
    cout << "After: x1 = " << x1 << ", y1 = " << y1 << endl;
    cout << "No change! Values not swapped!" << endl << endl;

    // Pass by pointer - WORKS
    cout << "2. Swap by Pointer (WORKS):" << endl;
    int x2 = 10, y2 = 20;
    cout << "Before: x2 = " << x2 << ", y2 = " << y2 << endl;
    swapByPointer(&x2, &y2);
    cout << "After: x2 = " << x2 << ", y2 = " << y2 << endl;
    cout << "Success! Values swapped!" << endl << endl;

    // Pass by reference - WORKS (preferred)
    cout << "3. Swap by Reference (WORKS - PREFERRED):" << endl;
    int x3 = 10, y3 = 20;
    cout << "Before: x3 = " << x3 << ", y3 = " << y3 << endl;
    swapByReference(x3, y3);
    cout << "After: x3 = " << x3 << ", y3 = " << y3 << endl;
    cout << "Success! Values swapped!" << endl << endl;

    // XOR swap
    cout << "4. Swap Using XOR (no temp variable):" << endl;
    int x4 = 15, y4 = 25;
    cout << "Before: x4 = " << x4 << ", y4 = " << y4 << endl;
    swapByXOR(x4, y4);
    cout << "After: x4 = " << x4 << ", y4 = " << y4 << endl << endl;

    // Arithmetic swap
    cout << "5. Swap Using Arithmetic (no temp variable):" << endl;
    int x5 = 30, y5 = 40;
    cout << "Before: x5 = " << x5 << ", y5 = " << y5 << endl;
    swapByArithmetic(x5, y5);
    cout << "After: x5 = " << x5 << ", y5 = " << y5 << endl << endl;

    // Generic swap
    cout << "6. Generic Swap (works with any type):" << endl;
    double d1 = 3.14, d2 = 2.71;
    cout << "Before: d1 = " << d1 << ", d2 = " << d2 << endl;
    genericSwap(d1, d2);
    cout << "After: d1 = " << d1 << ", d2 = " << d2 << endl;

    char c1 = 'A', c2 = 'Z';
    cout << "Before: c1 = " << c1 << ", c2 = " << c2 << endl;
    genericSwap(c1, c2);
    cout << "After: c1 = " << c1 << ", c2 = " << c2 << endl << endl;

    // Swap array elements
    cout << "7. Swap Array Elements:" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    cout << "Before: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl;

    swapArrayElements(arr, 0, 4);  // Swap first and last
    swapArrayElements(arr, 1, 3);  // Swap second and fourth

    cout << "After:  ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl << endl;

    // Swap pointers
    cout << "8. Swap Pointers Themselves:" << endl;
    int a = 100, b = 200;
    int* p1 = &a;
    int* p2 = &b;

    cout << "Before:" << endl;
    cout << "  p1 points to: " << *p1 << endl;
    cout << "  p2 points to: " << *p2 << endl;

    swapPointers(&p1, &p2);

    cout << "After swapping pointers:" << endl;
    cout << "  p1 points to: " << *p1 << endl;
    cout << "  p2 points to: " << *p2 << endl << endl;

    // Using std::swap
    cout << "9. Using std::swap (C++ Standard Library):" << endl;
    int x9 = 50, y9 = 60;
    cout << "Before: x9 = " << x9 << ", y9 = " << y9 << endl;
    swap(x9, y9);  // STL swap
    cout << "After: x9 = " << x9 << ", y9 = " << y9 << endl;

    return 0;
}
