/*
 * Lesson 17: 2D Arrays - Matrices
 * Compile: cl Lesson17_2DArrays.cpp
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== 2D Arrays Demo ===" << endl << endl;

    // 2D ARRAY DECLARATION
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // DISPLAY MATRIX
    cout << "Matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << setw(3) << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;

    // MATRIX ADDITION
    int matrix2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };
    int result[3][3];

    cout << "Matrix Addition:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = matrix[i][j] + matrix2[i][j];
            cout << setw(3) << result[i][j];
        }
        cout << endl;
    }
    cout << endl;

    // MATRIX MULTIPLICATION
    int m1[2][3] = {{1,2,3}, {4,5,6}};
    int m2[3][2] = {{7,8}, {9,10}, {11,12}};
    int product[2][2] = {0};

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 3; k++) {
                product[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    cout << "Matrix Multiplication Result:" << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            cout << setw(4) << product[i][j];
        }
        cout << endl;
    }
    cout << endl;

    // TRANSPOSE
    int transpose[3][3];
    cout << "Transpose:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            transpose[i][j] = matrix[j][i];
            cout << setw(3) << transpose[i][j];
        }
        cout << endl;
    }
    cout << endl;

    // DIAGONAL SUM
    int diagSum = 0;
    for (int i = 0; i < 3; i++) {
        diagSum += matrix[i][i];
    }
    cout << "Diagonal sum: " << diagSum << endl;

    return 0;
}
