/*
 * Lesson 24: Pointers with Functions
 * Compile: cl Lesson24_PointersFunctions.cpp
 */

#include <iostream>
using namespace std;

// Pass by pointer
void modifyValue(int *ptr) {
    *ptr = 100;
}

// Swap using pointers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Return pointer
int* findMax(int *arr, int size) {
    int *max = arr;
    for (int i = 1; i < size; i++) {
        if (*(arr + i) > *max) {
            max = arr + i;
        }
    }
    return max;
}

// Function pointer
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

int main() {
    cout << "=== Pointers with Functions Demo ===" << endl << endl;

    // PASS BY POINTER
    cout << "=== PASS BY POINTER ===" << endl;
    int num = 42;
    cout << "Before: " << num << endl;
    modifyValue(&num);
    cout << "After: " << num << endl << endl;

    // SWAP
    cout << "=== SWAP ===" << endl;
    int x = 10, y = 20;
    cout << "Before: x=" << x << ", y=" << y << endl;
    swap(&x, &y);
    cout << "After: x=" << x << ", y=" << y << endl << endl;

    // RETURN POINTER
    cout << "=== RETURN POINTER ===" << endl;
    int arr[] = {15, 42, 8, 23, 16};
    int *maxPtr = findMax(arr, 5);
    cout << "Maximum value: " << *maxPtr << endl;
    cout << "At index: " << (maxPtr - arr) << endl << endl;

    // FUNCTION POINTER
    cout << "=== FUNCTION POINTER ===" << endl;
    int (*operation)(int, int);

    operation = add;
    cout << "5 + 3 = " << operation(5, 3) << endl;

    operation = subtract;
    cout << "5 - 3 = " << operation(5, 3) << endl << endl;

    // ARRAY OF FUNCTION POINTERS
    int (*ops[])(int, int) = {add, subtract};
    cout << "Using array of function pointers:" << endl;
    cout << "10 + 5 = " << ops[0](10, 5) << endl;
    cout << "10 - 5 = " << ops[1](10, 5) << endl;

    return 0;
}
