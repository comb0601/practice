/*
 * Program: Break and Continue Demo
 * Description: Demonstrates break and continue statements in loops
 * Compilation: g++ 09_break_continue.cpp -o 09_break_continue
 * Execution: ./09_break_continue
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Break and Continue Demo ===" << endl << endl;

    // Break statement - exit loop early
    cout << "1. Break Statement (Find first number divisible by 7):" << endl;
    for (int i = 1; i <= 50; i++) {
        if (i % 7 == 0) {
            cout << "Found: " << i << endl;
            break;  // Exit the loop
        }
    }
    cout << endl;

    // Continue statement - skip current iteration
    cout << "2. Continue Statement (Print odd numbers 1-20):" << endl;
    for (int i = 1; i <= 20; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        cout << i << " ";
    }
    cout << endl << endl;

    // Break in while loop
    cout << "3. Break in While Loop (Sum until sum > 100):" << endl;
    int sum = 0;
    int num = 1;
    while (true) {  // Infinite loop
        sum += num;
        cout << "Added " << num << ", sum = " << sum << endl;

        if (sum > 100) {
            cout << "Sum exceeded 100, breaking..." << endl;
            break;
        }
        num++;
    }
    cout << endl;

    // Continue in while loop
    cout << "4. Continue in While Loop (Skip multiples of 3):" << endl;
    int i = 0;
    while (i < 20) {
        i++;
        if (i % 3 == 0) {
            continue;  // Skip multiples of 3
        }
        cout << i << " ";
    }
    cout << endl << endl;

    // Nested loops with break
    cout << "5. Break in Nested Loops (Find first pair that multiplies to 24):" << endl;
    bool found = false;
    for (int i = 1; i <= 10 && !found; i++) {
        for (int j = 1; j <= 10; j++) {
            if (i * j == 24) {
                cout << "Found: " << i << " * " << j << " = 24" << endl;
                found = true;
                break;  // Break inner loop
            }
        }
    }
    cout << endl;

    // Continue in nested loops
    cout << "6. Continue in Nested Loops (Skip when sum is even):" << endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            if ((i + j) % 2 == 0) {
                continue;  // Skip when sum is even
            }
            cout << "(" << i << "," << j << ") ";
        }
        cout << endl;
    }
    cout << endl;

    // Search for a number in range
    cout << "7. Search with Break:" << endl;
    int target = 42;
    found = false;

    for (int i = 1; i <= 100; i++) {
        if (i == target) {
            cout << "Target " << target << " found at position " << i << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Target not found" << endl;
    }
    cout << endl;

    // Sum only positive numbers using continue
    cout << "8. Sum Only Positive Numbers:" << endl;
    int numbers[] = {5, -3, 8, -1, 12, -7, 15, -9, 20};
    int arraySize = sizeof(numbers) / sizeof(numbers[0]);
    int positiveSum = 0;

    for (int i = 0; i < arraySize; i++) {
        if (numbers[i] < 0) {
            continue;  // Skip negative numbers
        }
        positiveSum += numbers[i];
    }

    cout << "Array: ";
    for (int i = 0; i < arraySize; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    cout << "Sum of positive numbers: " << positiveSum << endl;

    return 0;
}
