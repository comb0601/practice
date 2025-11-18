/*
 * Program: Pattern Printing
 * Description: Prints various advanced patterns using nested loops
 * Compilation: g++ 06_pattern_printing.cpp -o 06_pattern_printing
 * Execution: ./06_pattern_printing
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pattern Printing ===" << endl << endl;

    int n = 5;  // Pattern size

    // Pattern 1: Pyramid
    cout << "1. Pyramid Pattern:" << endl;
    for (int i = 1; i <= n; i++) {
        // Print spaces
        for (int j = 1; j <= n - i; j++) {
            cout << " ";
        }
        // Print stars
        for (int j = 1; j <= 2 * i - 1; j++) {
            cout << "*";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 2: Inverted Pyramid
    cout << "2. Inverted Pyramid:" << endl;
    for (int i = n; i >= 1; i--) {
        // Print spaces
        for (int j = 1; j <= n - i; j++) {
            cout << " ";
        }
        // Print stars
        for (int j = 1; j <= 2 * i - 1; j++) {
            cout << "*";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 3: Diamond
    cout << "3. Diamond Pattern:" << endl;
    // Upper half
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - i; j++) {
            cout << " ";
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            cout << "*";
        }
        cout << endl;
    }
    // Lower half
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= n - i; j++) {
            cout << " ";
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            cout << "*";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 4: Hollow Square
    cout << "4. Hollow Square:" << endl;
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

    // Pattern 5: Number Pyramid
    cout << "5. Number Pyramid:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - i; j++) {
            cout << " ";
        }
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 6: Pascal's Triangle
    cout << "6. Pascal's Triangle:" << endl;
    for (int i = 0; i < n; i++) {
        // Print spaces
        for (int j = 0; j < n - i - 1; j++) {
            cout << " ";
        }

        int number = 1;
        for (int j = 0; j <= i; j++) {
            cout << number << " ";
            number = number * (i - j) / (j + 1);
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 7: Alphabet Triangle
    cout << "7. Alphabet Triangle:" << endl;
    for (int i = 0; i < n; i++) {
        char ch = 'A';
        for (int j = 0; j <= i; j++) {
            cout << ch << " ";
            ch++;
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 8: Butterfly
    cout << "8. Butterfly Pattern:" << endl;
    // Upper half
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << "*";
        }
        for (int j = 1; j <= 2 * (n - i); j++) {
            cout << " ";
        }
        for (int j = 1; j <= i; j++) {
            cout << "*";
        }
        cout << endl;
    }
    // Lower half
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            cout << "*";
        }
        for (int j = 1; j <= 2 * (n - i); j++) {
            cout << " ";
        }
        for (int j = 1; j <= i; j++) {
            cout << "*";
        }
        cout << endl;
    }

    return 0;
}
