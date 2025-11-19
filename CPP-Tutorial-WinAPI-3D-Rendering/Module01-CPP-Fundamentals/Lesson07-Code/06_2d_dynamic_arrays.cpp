/*
 * Program: 2D Dynamic Arrays
 * Description: Creating and managing 2D dynamic arrays
 * Compilation: g++ -std=c++17 06_2d_dynamic_arrays.cpp -o 06_2d_dynamic_arrays
 * Execution: ./06_2d_dynamic_arrays
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== 2D Dynamic Arrays ===" << endl << endl;

    // Method 1: Array of pointers
    cout << "1. Method 1: Array of Pointers" << endl;
    int rows = 3, cols = 4;
    int** matrix = new int*[rows];

    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // Fill
    int val = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = val++;
        }
    }

    // Print
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    // Cleanup
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << endl;

    // Method 2: Single contiguous array
    cout << "2. Method 2: Contiguous Array (More Efficient)" << endl;
    int r = 3, c = 4;
    int* flat = new int[r * c];

    // Fill
    val = 1;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            flat[i * c + j] = val++;
        }
    }

    // Print
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << flat[i * c + j] << "\t";
        }
        cout << endl;
    }

    delete[] flat;
    cout << endl;

    // Method 3: Jagged array (different row sizes)
    cout << "3. Method 3: Jagged Array" << endl;
    int numRows = 4;
    int* rowSizes = new int[numRows]{2, 3, 1, 4};
    int** jagged = new int*[numRows];

    for (int i = 0; i < numRows; i++) {
        jagged[i] = new int[rowSizes[i]];
        for (int j = 0; j < rowSizes[i]; j++) {
            jagged[i][j] = i * 10 + j;
        }
    }

    for (int i = 0; i < numRows; i++) {
        cout << "Row " << i << ": ";
        for (int j = 0; j < rowSizes[i]; j++) {
            cout << jagged[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < numRows; i++) {
        delete[] jagged[i];
    }
    delete[] jagged;
    delete[] rowSizes;

    return 0;
}
