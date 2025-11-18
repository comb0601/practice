/*
 * Lesson 21: Pointers - Memory Addresses
 * Compile: cl Lesson21_Pointers.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pointers Demo ===" << endl << endl;

    // BASIC POINTER
    int num = 42;
    int *ptr = &num;  // ptr stores address of num

    cout << "Value of num: " << num << endl;
    cout << "Address of num: " << &num << endl;
    cout << "Value of ptr: " << ptr << endl;
    cout << "Value pointed to by ptr: " << *ptr << endl << endl;

    // MODIFYING THROUGH POINTER
    *ptr = 100;
    cout << "After *ptr = 100:" << endl;
    cout << "num = " << num << endl << endl;

    // POINTER TO DIFFERENT TYPES
    double pi = 3.14159;
    double *dptr = &pi;
    cout << "Double value: " << *dptr << endl << endl;

    char ch = 'A';
    char *cptr = &ch;
    cout << "Char value: " << *cptr << endl << endl;

    // NULL POINTER
    int *nullPtr = nullptr;
    cout << "Null pointer value: " << nullPtr << endl;
    if (nullPtr == nullptr) {
        cout << "Pointer is null" << endl;
    }
    cout << endl;

    // POINTER SIZE
    cout << "Size of pointer: " << sizeof(ptr) << " bytes" << endl;
    cout << "Size of int: " << sizeof(int) << " bytes" << endl << endl;

    // POINTER TO POINTER
    int value = 25;
    int *p1 = &value;
    int **p2 = &p1;  // Pointer to pointer

    cout << "Value: " << value << endl;
    cout << "Via p1: " << *p1 << endl;
    cout << "Via p2: " << **p2 << endl << endl;

    // VOID POINTER
    void *vptr;
    int x = 10;
    vptr = &x;
    cout << "Void pointer to int: " << *(static_cast<int*>(vptr)) << endl;

    return 0;
}
