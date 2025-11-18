/*
 * Program: Number Patterns and Sequences
 * Description: Generates various number patterns and sequences
 * Compilation: g++ 15_number_patterns.cpp -o 15_number_patterns
 * Execution: ./15_number_patterns
 */

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    cout << "=== Number Patterns and Sequences ===" << endl << endl;

    int n = 5;  // Default size for patterns

    // Pattern 1: Natural numbers triangle
    cout << "1. Natural Numbers Triangle:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 2: Repeated number triangle
    cout << "2. Repeated Number Triangle:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 3: Descending numbers
    cout << "3. Descending Numbers Triangle:" << endl;
    for (int i = n; i >= 1; i--) {
        for (int j = i; j >= 1; j--) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 4: Floyd's triangle
    cout << "4. Floyd's Triangle:" << endl;
    int num = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << num << " ";
            num++;
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 5: Multiplication table pattern
    cout << "5. Multiplication Pattern:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << (i * j) << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 6: Sum pattern
    cout << "6. Sum Pattern (cumulative):" << endl;
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
        cout << "Sum of 1 to " << i << " = " << sum << endl;
    }
    cout << endl;

    // Pattern 7: Powers of 2
    cout << "7. Powers of 2:" << endl;
    for (int i = 0; i < 10; i++) {
        int power = pow(2, i);
        cout << "2^" << i << " = " << power << endl;
    }
    cout << endl;

    // Pattern 8: Perfect squares
    cout << "8. Perfect Squares (1-10):" << endl;
    for (int i = 1; i <= 10; i++) {
        int square = i * i;
        cout << i << "² = " << square << endl;
    }
    cout << endl;

    // Pattern 9: Palindrome number pyramid
    cout << "9. Palindrome Number Pyramid:" << endl;
    for (int i = 1; i <= n; i++) {
        // Spaces
        for (int j = 1; j <= n - i; j++) {
            cout << "  ";
        }
        // Ascending
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        // Descending
        for (int j = i - 1; j >= 1; j--) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Pattern 10: Binary pattern (0s and 1s)
    cout << "10. Binary Pattern:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            cout << ((i + j) % 2) << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Sequence demonstrations
    cout << "=== Number Sequences ===" << endl << endl;

    // Arithmetic sequence
    cout << "11. Arithmetic Sequence (start=5, diff=3):" << endl;
    int start = 5, diff = 3;
    for (int i = 0; i < 10; i++) {
        cout << (start + i * diff) << " ";
    }
    cout << endl << endl;

    // Geometric sequence
    cout << "12. Geometric Sequence (start=2, ratio=3):" << endl;
    int gStart = 2, ratio = 3;
    int term = gStart;
    for (int i = 0; i < 8; i++) {
        cout << term << " ";
        term *= ratio;
    }
    cout << endl << endl;

    // Triangular numbers
    cout << "13. Triangular Numbers:" << endl;
    for (int i = 1; i <= 10; i++) {
        int triangular = (i * (i + 1)) / 2;
        cout << "T(" << i << ") = " << triangular << " ";
    }
    cout << endl << endl;

    // Factorial sequence
    cout << "14. Factorial Sequence:" << endl;
    long long factorial = 1;
    for (int i = 1; i <= 10; i++) {
        factorial *= i;
        cout << i << "! = " << factorial << endl;
    }
    cout << endl;

    // Collatz sequence
    cout << "15. Collatz Sequence (starting from 13):" << endl;
    int collatz = 13;
    cout << collatz << " ";
    while (collatz != 1) {
        if (collatz % 2 == 0) {
            collatz = collatz / 2;
        } else {
            collatz = 3 * collatz + 1;
        }
        cout << collatz << " ";
    }
    cout << endl;

    return 0;
}
