/*
 * Program: Multidimensional Arrays
 * Description: Working with 2D and 3D arrays - matrices, tables, grids
 * Compilation: g++ 02_multidimensional_arrays.cpp -o 02_multidimensional_arrays
 * Execution: ./02_multidimensional_arrays
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Multidimensional Arrays Demo ===" << endl << endl;

    // 2D Array - Matrix
    cout << "1. 2D Array (Matrix):" << endl;
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    cout << "3x4 Matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Accessing individual elements
    cout << "2. Accessing Elements:" << endl;
    cout << "Element at [0][0]: " << matrix[0][0] << endl;
    cout << "Element at [1][2]: " << matrix[1][2] << endl;
    cout << "Element at [2][3]: " << matrix[2][3] << endl << endl;

    // Modifying elements
    cout << "3. Modifying Elements:" << endl;
    matrix[1][1] = 99;
    cout << "After changing [1][1] to 99:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Row and column sums
    cout << "4. Row Sums:" << endl;
    for (int i = 0; i < 3; i++) {
        int rowSum = 0;
        for (int j = 0; j < 4; j++) {
            rowSum += matrix[i][j];
        }
        cout << "Row " << i << " sum: " << rowSum << endl;
    }
    cout << endl;

    cout << "5. Column Sums:" << endl;
    for (int j = 0; j < 4; j++) {
        int colSum = 0;
        for (int i = 0; i < 3; i++) {
            colSum += matrix[i][j];
        }
        cout << "Column " << j << " sum: " << colSum << endl;
    }
    cout << endl;

    // Identity matrix
    cout << "6. Identity Matrix (3x3):" << endl;
    int identity[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            identity[i][j] = (i == j) ? 1 : 0;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << identity[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 3D Array
    cout << "7. 3D Array:" << endl;
    int cube[2][2][2] = {
        {{1, 2}, {3, 4}},
        {{5, 6}, {7, 8}}
    };

    cout << "3D Array (2x2x2 cube):" << endl;
    for (int i = 0; i < 2; i++) {
        cout << "Layer " << i << ":" << endl;
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                cout << cube[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
