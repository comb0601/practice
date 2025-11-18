/*
 * Lesson 25: Dynamic Memory - new and delete
 * Compile: cl Lesson25_DynamicMemory.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Dynamic Memory Demo ===" << endl << endl;

    // SINGLE VARIABLE
    cout << "=== SINGLE VARIABLE ===" << endl;
    int *ptr = new int;
    *ptr = 42;
    cout << "Value: " << *ptr << endl;
    delete ptr;
    cout << "Memory freed" << endl << endl;

    // WITH INITIALIZATION
    int *num = new int(100);
    cout << "Initialized value: " << *num << endl;
    delete num;
    cout << endl;

    // DYNAMIC ARRAY
    cout << "=== DYNAMIC ARRAY ===" << endl;
    int size = 5;
    int *arr = new int[size];

    cout << "Enter " << size << " numbers:" << endl;
    for (int i = 0; i < size; i++) {
        arr[i] = (i + 1) * 10;  // Simulated input
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Array values: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
    cout << "Array memory freed" << endl << endl;

    // 2D DYNAMIC ARRAY
    cout << "=== 2D DYNAMIC ARRAY ===" << endl;
    int rows = 3, cols = 4;
    int **matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // Fill matrix
    int val = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = val++;
        }
    }

    // Display matrix
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Free matrix
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << "Matrix memory freed" << endl << endl;

    // MEMORY LEAK WARNING
    cout << "=== MEMORY MANAGEMENT ===" << endl;
    int *leak = new int(50);
    cout << "Always remember to delete!" << endl;
    delete leak;

    return 0;
}
