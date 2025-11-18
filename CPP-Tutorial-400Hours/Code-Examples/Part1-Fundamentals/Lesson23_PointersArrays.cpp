/*
 * Lesson 23: Pointers and Arrays - Relationship
 * Compile: cl Lesson23_PointersArrays.cpp
 */

#include <iostream>
using namespace std;

void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int sumArray(int *arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *(arr + i);
    }
    return sum;
}

int main() {
    cout << "=== Pointers and Arrays Demo ===" << endl << endl;

    // ARRAY NAME AS POINTER
    int arr[] = {1, 2, 3, 4, 5};
    cout << "arr: " << arr << endl;
    cout << "&arr[0]: " << &arr[0] << endl;
    cout << "arr == &arr[0]: " << (arr == &arr[0]) << endl << endl;

    // ACCESSING VIA POINTER
    cout << "arr[2] = " << arr[2] << endl;
    cout << "*(arr + 2) = " << *(arr + 2) << endl << endl;

    // PASSING ARRAY TO FUNCTION
    cout << "Array via function: ";
    printArray(arr, 5);
    cout << "Sum: " << sumArray(arr, 5) << endl << endl;

    // POINTER TO ARRAY
    int (*ptr)[5] = &arr;
    cout << "Via pointer to array: ";
    for (int i = 0; i < 5; i++) {
        cout << (*ptr)[i] << " ";
    }
    cout << endl << endl;

    // ARRAY OF POINTERS
    int a = 10, b = 20, c = 30;
    int *ptrArr[] = {&a, &b, &c};
    cout << "Array of pointers:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "*ptrArr[" << i << "] = " << *ptrArr[i] << endl;
    }
    cout << endl;

    // 2D ARRAY AND POINTER
    int matrix[2][3] = {{1,2,3}, {4,5,6}};
    cout << "2D array via pointer:" << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            cout << *(*(matrix + i) + j) << " ";
        }
        cout << endl;
    }

    return 0;
}
