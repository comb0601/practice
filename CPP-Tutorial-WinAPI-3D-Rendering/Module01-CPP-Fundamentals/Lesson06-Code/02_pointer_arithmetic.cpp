/*
 * Program: Pointer Arithmetic
 * Description: Pointer arithmetic operations - increment, decrement, addition, subtraction
 * Compilation: g++ -std=c++17 02_pointer_arithmetic.cpp -o 02_pointer_arithmetic
 * Execution: ./02_pointer_arithmetic
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pointer Arithmetic ===" << endl << endl;

    // Array for demonstration
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;  // Points to first element

    // Basic pointer arithmetic
    cout << "1. Array and Pointer:" << endl;
    cout << "Array: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << "ptr points to first element: " << *ptr << endl;
    cout << "Address: " << ptr << endl << endl;

    // Pointer increment
    cout << "2. Pointer Increment (++):" << endl;
    cout << "Initial: *ptr = " << *ptr << " at " << ptr << endl;

    ptr++;
    cout << "After ptr++: *ptr = " << *ptr << " at " << ptr << endl;

    ptr++;
    cout << "After ptr++: *ptr = " << *ptr << " at " << ptr << endl << endl;

    // Reset pointer
    ptr = arr;

    // Pointer addition
    cout << "3. Pointer Addition:" << endl;
    cout << "ptr points to: " << *ptr << endl;
    cout << "*(ptr + 0) = " << *(ptr + 0) << endl;
    cout << "*(ptr + 1) = " << *(ptr + 1) << endl;
    cout << "*(ptr + 2) = " << *(ptr + 2) << endl;
    cout << "*(ptr + 3) = " << *(ptr + 3) << endl;
    cout << "*(ptr + 4) = " << *(ptr + 4) << endl << endl;

    // Pointer decrement
    cout << "4. Pointer Decrement (--):" << endl;
    ptr = &arr[4];  // Point to last element
    cout << "ptr points to last element: " << *ptr << endl;

    ptr--;
    cout << "After ptr--: " << *ptr << endl;

    ptr--;
    cout << "After ptr--: " << *ptr << endl << endl;

    // Pointer subtraction
    ptr = arr;
    cout << "5. Pointer Subtraction:" << endl;
    int* ptr1 = &arr[0];
    int* ptr2 = &arr[4];

    cout << "ptr1 points to arr[0]: " << *ptr1 << " at " << ptr1 << endl;
    cout << "ptr2 points to arr[4]: " << *ptr2 << " at " << ptr2 << endl;
    cout << "ptr2 - ptr1 = " << (ptr2 - ptr1) << " elements apart" << endl << endl;

    // Array traversal using pointer
    cout << "6. Array Traversal Using Pointer Arithmetic:" << endl;
    ptr = arr;
    cout << "Forward traversal: ";
    for (int i = 0; i < 5; i++) {
        cout << *(ptr + i) << " ";
    }
    cout << endl;

    cout << "Using pointer increment: ";
    ptr = arr;
    for (int i = 0; i < 5; i++, ptr++) {
        cout << *ptr << " ";
    }
    cout << endl << endl;

    // Comparison with array indexing
    cout << "7. Pointer vs Array Indexing:" << endl;
    ptr = arr;
    cout << "arr[2] = " << arr[2] << endl;
    cout << "*(arr + 2) = " << *(arr + 2) << endl;
    cout << "ptr[2] = " << ptr[2] << endl;
    cout << "*(ptr + 2) = " << *(ptr + 2) << endl;
    cout << "All access the same element!" << endl << endl;

    // Address calculations
    cout << "8. Address Differences:" << endl;
    cout << "Address of arr[0]: " << &arr[0] << endl;
    cout << "Address of arr[1]: " << &arr[1] << endl;
    cout << "Difference: " << ((char*)&arr[1] - (char*)&arr[0]) << " bytes" << endl;
    cout << "Size of int: " << sizeof(int) << " bytes" << endl;

    return 0;
}
