/*
 * Lesson 8: Break and Continue - Loop Control
 *
 * Demonstrates break and continue statements in various loop contexts
 *
 * Compile: cl Lesson08_BreakContinue.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Break and Continue Demo ===" << endl << endl;

    // BREAK IN FOR LOOP
    cout << "=== BREAK STATEMENT ===" << endl;
    cout << "Breaking at 5:" << endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            cout << "Breaking!" << endl;
            break;
        }
        cout << i << " ";
    }
    cout << endl << endl;

    // CONTINUE IN FOR LOOP
    cout << "=== CONTINUE STATEMENT ===" << endl;
    cout << "Skipping multiples of 3:" << endl;
    for (int i = 1; i <= 10; i++) {
        if (i % 3 == 0) continue;
        cout << i << " ";
    }
    cout << endl << endl;

    // BREAK IN WHILE LOOP
    cout << "=== BREAK IN WHILE ===" << endl;
    int count = 0;
    while (true) {
        count++;
        if (count > 5) break;
        cout << count << " ";
    }
    cout << endl << endl;

    // CONTINUE IN WHILE LOOP
    cout << "=== CONTINUE IN WHILE ===" << endl;
    int num = 0;
    while (num < 10) {
        num++;
        if (num % 2 == 0) continue;
        cout << num << " ";
    }
    cout << endl << endl;

    // NESTED LOOPS WITH BREAK
    cout << "=== NESTED LOOPS WITH BREAK ===" << endl;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 5; j++) {
            if (j == 4) break;
            cout << "(" << i << "," << j << ") ";
        }
        cout << endl;
    }
    cout << endl;

    // SEARCH WITH BREAK
    cout << "=== SEARCH EXAMPLE ===" << endl;
    int arr[] = {10, 25, 30, 45, 50};
    int target = 30;
    bool found = false;

    for (int i = 0; i < 5; i++) {
        if (arr[i] == target) {
            cout << "Found " << target << " at index " << i << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << target << " not found" << endl;
    cout << endl;

    // PRIME CHECK
    cout << "=== PRIME CHECK WITH BREAK ===" << endl;
    int testNum = 29;
    bool isPrime = true;

    for (int i = 2; i * i <= testNum; i++) {
        if (testNum % i == 0) {
            isPrime = false;
            break;
        }
    }
    cout << testNum << (isPrime ? " is prime" : " is not prime") << endl;

    return 0;
}
