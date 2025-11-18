/*
 * Program: For Loop Demo
 * Description: Demonstrates for loops with various examples
 * Compilation: g++ 04_for_loop.cpp -o 04_for_loop
 * Execution: ./04_for_loop
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== For Loop Demo ===" << endl << endl;

    // Basic for loop
    cout << "1. Basic For Loop (1 to 10):" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << i << " ";
    }
    cout << endl << endl;

    // For loop with decrement
    cout << "2. Countdown (10 to 1):" << endl;
    for (int i = 10; i >= 1; i--) {
        cout << i << " ";
    }
    cout << "Liftoff!" << endl << endl;

    // For loop with step
    cout << "3. Even Numbers (0 to 20):" << endl;
    for (int i = 0; i <= 20; i += 2) {
        cout << i << " ";
    }
    cout << endl << endl;

    // Multiplication table
    cout << "4. Multiplication Table:" << endl;
    int table;
    cout << "Enter a number: ";
    cin >> table;

    for (int i = 1; i <= 10; i++) {
        cout << table << " x " << i << " = " << (table * i) << endl;
    }
    cout << endl;

    // Sum of numbers
    cout << "5. Sum of First N Numbers:" << endl;
    int n;
    cout << "Enter N: ";
    cin >> n;

    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    cout << "Sum of 1 to " << n << " = " << sum << endl << endl;

    // Fibonacci series
    cout << "6. Fibonacci Series:" << endl;
    cout << "Enter number of terms: ";
    cin >> n;

    int first = 0, second = 1, next;
    cout << "Fibonacci Series: ";

    for (int i = 0; i < n; i++) {
        if (i <= 1) {
            next = i;
        } else {
            next = first + second;
            first = second;
            second = next;
        }
        cout << next << " ";
    }
    cout << endl << endl;

    // Prime number checker
    cout << "7. Prime Number Checker:" << endl;
    int num;
    cout << "Enter a number: ";
    cin >> num;

    bool isPrime = true;
    if (num <= 1) {
        isPrime = false;
    } else {
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
    }

    if (isPrime) {
        cout << num << " is a PRIME number" << endl;
    } else {
        cout << num << " is NOT a prime number" << endl;
    }
    cout << endl;

    // Power calculation
    cout << "8. Power Calculator:" << endl;
    int base, exponent;
    cout << "Enter base: ";
    cin >> base;
    cout << "Enter exponent: ";
    cin >> exponent;

    long long result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    cout << base << "^" << exponent << " = " << result << endl;

    return 0;
}
