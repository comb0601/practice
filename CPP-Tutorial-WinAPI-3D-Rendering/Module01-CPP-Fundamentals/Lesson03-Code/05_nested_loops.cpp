/*
 * Program: Nested Loops Demo
 * Description: Demonstrates nested loops for various applications
 * Compilation: g++ 05_nested_loops.cpp -o 05_nested_loops
 * Execution: ./05_nested_loops
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Nested Loops Demo ===" << endl << endl;

    // Rectangle pattern
    cout << "1. Rectangle Pattern (5x8):" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // Right-angled triangle
    cout << "2. Right-Angled Triangle:" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // Inverted triangle
    cout << "3. Inverted Triangle:" << endl;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // Number triangle
    cout << "4. Number Triangle:" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Multiplication table (1-5)
    cout << "5. Multiplication Table (1-5):" << endl;
    cout << "    ";
    for (int i = 1; i <= 5; i++) {
        cout << i << "   ";
    }
    cout << endl << "   -------------------" << endl;

    for (int i = 1; i <= 5; i++) {
        cout << i << " | ";
        for (int j = 1; j <= 5; j++) {
            cout << (i * j) << "   ";
            if (i * j < 10) cout << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Chess board pattern
    cout << "6. Chess Board Pattern (8x8):" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0) {
                cout << "■ ";
            } else {
                cout << "□ ";
            }
        }
        cout << endl;
    }
    cout << endl;

    // Floyd's triangle
    cout << "7. Floyd's Triangle:" << endl;
    int number = 1;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << number << " ";
            number++;
        }
        cout << endl;
    }
    cout << endl;

    // Prime numbers in range
    cout << "8. Prime Numbers (1-50):" << endl;
    for (int num = 2; num <= 50; num++) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            cout << num << " ";
        }
    }
    cout << endl;

    return 0;
}
