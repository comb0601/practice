/*
 * Lesson 10: Pattern Printing - Creative Nested Loops
 *
 * Demonstrates various pattern printing techniques
 *
 * Compile: cl Lesson10_Patterns.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pattern Printing Demo ===" << endl << endl;

    // PATTERN 1: INCREASING STARS
    cout << "Pattern 1: Increasing Stars" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 2: DECREASING STARS
    cout << "Pattern 2: Decreasing Stars" << endl;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 3: CENTERED PYRAMID
    cout << "Pattern 3: Centered Pyramid" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5 - i; j++) cout << " ";
        for (int j = 1; j <= 2 * i - 1; j++) cout << "*";
        cout << endl;
    }
    cout << endl;

    // PATTERN 4: INVERTED PYRAMID
    cout << "Pattern 4: Inverted Pyramid" << endl;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= 5 - i; j++) cout << " ";
        for (int j = 1; j <= 2 * i - 1; j++) cout << "*";
        cout << endl;
    }
    cout << endl;

    // PATTERN 5: DIAMOND
    cout << "Pattern 5: Diamond" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5 - i; j++) cout << " ";
        for (int j = 1; j <= 2 * i - 1; j++) cout << "*";
        cout << endl;
    }
    for (int i = 4; i >= 1; i--) {
        for (int j = 1; j <= 5 - i; j++) cout << " ";
        for (int j = 1; j <= 2 * i - 1; j++) cout << "*";
        cout << endl;
    }
    cout << endl;

    // PATTERN 6: NUMBER TRIANGLE
    cout << "Pattern 6: Number Triangle" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 7: ALPHABET PATTERN
    cout << "Pattern 7: Alphabet Pattern" << endl;
    char ch = 'A';
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << ch << " ";
            ch++;
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 8: HOLLOW SQUARE
    cout << "Pattern 8: Hollow Square" << endl;
    int n = 5;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == 1 || i == n || j == 1 || j == n) {
                cout << "* ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 9: FLOYD'S TRIANGLE
    cout << "Pattern 9: Floyd's Triangle" << endl;
    int num = 1;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << num++ << " ";
        }
        cout << endl;
    }
    cout << endl;

    // PATTERN 10: PASCAL'S TRIANGLE
    cout << "Pattern 10: Pascal's Triangle" << endl;
    for (int i = 0; i < 5; i++) {
        int val = 1;
        for (int j = 1; j < 5 - i; j++) cout << " ";
        for (int j = 0; j <= i; j++) {
            cout << val << " ";
            val = val * (i - j) / (j + 1);
        }
        cout << endl;
    }

    return 0;
}
