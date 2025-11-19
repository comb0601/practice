/*
 * Program: Matrix Operations
 * Description: Matrix addition, multiplication, transpose
 * Compilation: g++ 09_matrix_operations.cpp -o 09_matrix_operations
 * Execution: ./09_matrix_operations
 */

#include <iostream>
using namespace std;

const int MAX_SIZE = 10;

void printMatrix(int matrix[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

void addMatrices(int a[][MAX_SIZE], int b[][MAX_SIZE], int result[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subtractMatrices(int a[][MAX_SIZE], int b[][MAX_SIZE], int result[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void multiplyMatrices(int a[][MAX_SIZE], int b[][MAX_SIZE], int result[][MAX_SIZE],
                      int rows1, int cols1, int cols2) {
    // Initialize result matrix to 0
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
        }
    }

    // Multiply
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transposeMatrix(int matrix[][MAX_SIZE], int result[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

void scalarMultiply(int matrix[][MAX_SIZE], int result[][MAX_SIZE], int rows, int cols, int scalar) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }
}

int main() {
    cout << "=== Matrix Operations ===" << endl << endl;

    int A[MAX_SIZE][MAX_SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int B[MAX_SIZE][MAX_SIZE] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int result[MAX_SIZE][MAX_SIZE];

    // Matrix Addition
    cout << "1. Matrix Addition:" << endl;
    cout << "Matrix A:" << endl;
    printMatrix(A, 3, 3);
    cout << "\nMatrix B:" << endl;
    printMatrix(B, 3, 3);
    cout << "\nA + B =" << endl;
    addMatrices(A, B, result, 3, 3);
    printMatrix(result, 3, 3);
    cout << endl;

    // Matrix Subtraction
    cout << "2. Matrix Subtraction:" << endl;
    cout << "A - B =" << endl;
    subtractMatrices(A, B, result, 3, 3);
    printMatrix(result, 3, 3);
    cout << endl;

    // Scalar Multiplication
    cout << "3. Scalar Multiplication:" << endl;
    cout << "A * 2 =" << endl;
    scalarMultiply(A, result, 3, 3, 2);
    printMatrix(result, 3, 3);
    cout << endl;

    // Matrix Transpose
    cout << "4. Matrix Transpose:" << endl;
    cout << "Transpose of A:" << endl;
    transposeMatrix(A, result, 3, 3);
    printMatrix(result, 3, 3);
    cout << endl;

    // Matrix Multiplication
    cout << "5. Matrix Multiplication:" << endl;
    int C[MAX_SIZE][MAX_SIZE] = {
        {1, 2},
        {3, 4},
        {5, 6}
    };

    int D[MAX_SIZE][MAX_SIZE] = {
        {7, 8, 9},
        {10, 11, 12}
    };

    cout << "Matrix C (3x2):" << endl;
    printMatrix(C, 3, 2);
    cout << "\nMatrix D (2x3):" << endl;
    printMatrix(D, 2, 3);
    cout << "\nC * D (3x3) =" << endl;
    multiplyMatrices(C, D, result, 3, 2, 3);
    printMatrix(result, 3, 3);
    cout << endl;

    // Identity Matrix
    cout << "6. Identity Matrix (4x4):" << endl;
    int identity[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            identity[i][j] = (i == j) ? 1 : 0;
        }
    }
    printMatrix(identity, 4, 4);

    return 0;
}
