/*
 * Lesson 7: For Loop - Controlled Iteration
 *
 * This program demonstrates:
 * - Basic for loop syntax
 * - For loop variations
 * - Nested for loops
 * - Range-based for loop (C++11)
 * - Practical applications
 *
 * Compile: cl /std:c++17 Lesson07_ForLoop.cpp
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    cout << "=== C++ For Loop Demo ===" << endl << endl;

    // BASIC FOR LOOP
    cout << "=== BASIC FOR LOOP ===" << endl;
    cout << "Numbers 1 to 10:" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << i << " ";
    }
    cout << endl << endl;

    // FOR LOOP WITH DIFFERENT INCREMENTS
    cout << "=== DIFFERENT INCREMENTS ===" << endl;
    cout << "Even numbers (0 to 20):" << endl;
    for (int i = 0; i <= 20; i += 2) {
        cout << i << " ";
    }
    cout << endl;

    cout << "Count by 5s (0 to 50):" << endl;
    for (int i = 0; i <= 50; i += 5) {
        cout << i << " ";
    }
    cout << endl << endl;

    // DECREMENTING FOR LOOP
    cout << "=== DECREMENTING FOR LOOP ===" << endl;
    cout << "Countdown from 10:" << endl;
    for (int i = 10; i >= 1; i--) {
        cout << i << " ";
    }
    cout << "Liftoff!" << endl << endl;

    // SUM CALCULATION
    cout << "=== SUM CALCULATION ===" << endl;
    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        sum += i;
    }
    cout << "Sum of 1 to 100: " << sum << endl << endl;

    // MULTIPLICATION TABLE
    cout << "=== MULTIPLICATION TABLE ===" << endl;
    int tableNum = 5;
    cout << "Multiplication table for " << tableNum << ":" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << tableNum << " x " << i << " = " << (tableNum * i) << endl;
    }
    cout << endl;

    // FACTORIAL
    cout << "=== FACTORIAL ===" << endl;
    int n = 6;
    long long factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial *= i;
    }
    cout << n << "! = " << factorial << endl << endl;

    // POWER CALCULATION
    cout << "=== POWER CALCULATION ===" << endl;
    int base = 2;
    long long result = 1;
    cout << "Powers of " << base << ":" << endl;
    for (int exp = 0; exp <= 10; exp++) {
        result = 1;
        for (int i = 0; i < exp; i++) {
            result *= base;
        }
        cout << base << "^" << exp << " = " << result << endl;
    }
    cout << endl;

    // NESTED FOR LOOPS - PATTERN
    cout << "=== NESTED FOR LOOPS ===" << endl;
    cout << "Right triangle pattern:" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    cout << endl;

    // MULTIPLICATION TABLE GRID
    cout << "=== MULTIPLICATION TABLE GRID ===" << endl;
    cout << "   ";
    for (int i = 1; i <= 5; i++) {
        cout << "  " << i << " ";
    }
    cout << endl;
    cout << "   " << string(20, '-') << endl;

    for (int i = 1; i <= 5; i++) {
        cout << i << " |";
        for (int j = 1; j <= 5; j++) {
            cout << " " << (i * j < 10 ? " " : "") << (i * j) << " ";
        }
        cout << endl;
    }
    cout << endl;

    // PRIME NUMBERS
    cout << "=== PRIME NUMBERS ===" << endl;
    cout << "Prime numbers between 1 and 50:" << endl;
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
    cout << endl << endl;

    // FIBONACCI SEQUENCE
    cout << "=== FIBONACCI SEQUENCE ===" << endl;
    int fib1 = 0, fib2 = 1;
    cout << "First 15 Fibonacci numbers:" << endl;
    for (int i = 0; i < 15; i++) {
        if (i == 0) {
            cout << fib1 << " ";
        } else if (i == 1) {
            cout << fib2 << " ";
        } else {
            int nextFib = fib1 + fib2;
            cout << nextFib << " ";
            fib1 = fib2;
            fib2 = nextFib;
        }
    }
    cout << endl << endl;

    // ARRAY PROCESSING
    cout << "=== ARRAY PROCESSING ===" << endl;
    int numbers[] = {10, 20, 30, 40, 50};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    cout << "Array elements:" << endl;
    for (int i = 0; i < size; i++) {
        cout << "numbers[" << i << "] = " << numbers[i] << endl;
    }
    cout << endl;

    // RANGE-BASED FOR LOOP (C++11)
    cout << "=== RANGE-BASED FOR LOOP ===" << endl;
    int values[] = {5, 10, 15, 20, 25};

    cout << "Using range-based for:" << endl;
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl << endl;

    // VECTOR ITERATION
    cout << "=== VECTOR ITERATION ===" << endl;
    vector<string> fruits = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};

    cout << "Fruits list:" << endl;
    for (size_t i = 0; i < fruits.size(); i++) {
        cout << i + 1 << ". " << fruits[i] << endl;
    }

    cout << "\nUsing range-based for with vector:" << endl;
    for (const string& fruit : fruits) {
        cout << "- " << fruit << endl;
    }
    cout << endl;

    // MULTIPLE VARIABLES
    cout << "=== MULTIPLE VARIABLES ===" << endl;
    cout << "Forward and backward counting:" << endl;
    for (int i = 0, j = 10; i <= 10; i++, j--) {
        cout << "i=" << i << ", j=" << j << endl;
    }
    cout << endl;

    // INFINITE LOOP WITH BREAK
    cout << "=== LOOP WITH BREAK ===" << endl;
    cout << "Breaking at 5:" << endl;
    for (int i = 1; ; i++) {  // Infinite loop
        if (i > 5) break;
        cout << i << " ";
    }
    cout << endl << endl;

    // CONTINUE STATEMENT
    cout << "=== CONTINUE STATEMENT ===" << endl;
    cout << "Numbers 1-10, skipping multiples of 3:" << endl;
    for (int i = 1; i <= 10; i++) {
        if (i % 3 == 0) continue;
        cout << i << " ";
    }
    cout << endl << endl;

    // NESTED LOOPS - MATRIX
    cout << "=== MATRIX EXAMPLE ===" << endl;
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "3x3 Matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // PRACTICAL: SUM OF EVEN AND ODD
    cout << "=== SUM OF EVEN AND ODD ===" << endl;
    int evenSum = 0, oddSum = 0;
    for (int i = 1; i <= 20; i++) {
        if (i % 2 == 0) {
            evenSum += i;
        } else {
            oddSum += i;
        }
    }
    cout << "Sum of even numbers (1-20): " << evenSum << endl;
    cout << "Sum of odd numbers (1-20): " << oddSum << endl;

    return 0;
}

/*
 * Expected Output:
 * === C++ For Loop Demo ===
 *
 * === BASIC FOR LOOP ===
 * Numbers 1 to 10:
 * 1 2 3 4 5 6 7 8 9 10
 *
 * === DIFFERENT INCREMENTS ===
 * Even numbers (0 to 20):
 * 0 2 4 6 8 10 12 14 16 18 20
 * Count by 5s (0 to 50):
 * 0 5 10 15 20 25 30 35 40 45 50
 *
 * ... (continues with all examples)
 */
