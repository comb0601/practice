/*
 * Lesson 9: Nested Loops - Loops within Loops
 *
 * Demonstrates various nested loop patterns and applications
 *
 * Compile: cl Lesson09_NestedLoops.cpp
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== Nested Loops Demo ===" << endl << endl;

    // BASIC NESTED LOOP
    cout << "=== BASIC NESTED LOOP ===" << endl;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 4; j++) {
            cout << "(" << i << "," << j << ") ";
        }
        cout << endl;
    }
    cout << endl;

    // MULTIPLICATION TABLE
    cout << "=== MULTIPLICATION TABLE ===" << endl;
    cout << "    ";
    for (int i = 1; i <= 10; i++) {
        cout << setw(4) << i;
    }
    cout << endl << "   " << string(44, '-') << endl;

    for (int i = 1; i <= 10; i++) {
        cout << setw(2) << i << " |";
        for (int j = 1; j <= 10; j++) {
            cout << setw(4) << (i * j);
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 1: SQUARE
    cout << "=== SQUARE PATTERN ===" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 2: RIGHT TRIANGLE
    cout << "=== RIGHT TRIANGLE ===" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 3: INVERTED TRIANGLE
    cout << "=== INVERTED TRIANGLE ===" << endl;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 4: PYRAMID
    cout << "=== PYRAMID ===" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5 - i; j++) {
            cout << "  ";
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // NUMBER PATTERN
    cout << "=== NUMBER PATTERN ===" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // MATRIX OPERATIONS
    cout << "=== MATRIX SUM ===" << endl;
    int matrix1[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    int matrix2[3][3] = {{9,8,7}, {6,5,4}, {3,2,1}};
    int result[3][3];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    cout << "Result matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << result[i][j];
        }
        cout << endl;
    }

    return 0;
}
