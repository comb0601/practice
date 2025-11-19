/*
 * Lesson 16: Arrays - Collection of Elements
 * Compile: cl Lesson16_Arrays.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Arrays Demo ===" << endl << endl;

    // ARRAY DECLARATION AND INITIALIZATION
    int numbers[5] = {10, 20, 30, 40, 50};
    int values[] = {1, 2, 3, 4, 5, 6};  // Size inferred
    int scores[10] = {0};  // All initialized to 0

    // ACCESSING ELEMENTS
    cout << "First element: " << numbers[0] << endl;
    cout << "Last element: " << numbers[4] << endl << endl;

    // ITERATING THROUGH ARRAY
    cout << "All elements: ";
    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    // MODIFYING ELEMENTS
    numbers[2] = 99;
    cout << "After modification: ";
    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    // ARRAY SUM
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    cout << "Sum: " << sum << endl << endl;

    // FINDING MAXIMUM
    int max = numbers[0];
    for (int i = 1; i < 5; i++) {
        if (numbers[i] > max) max = numbers[i];
    }
    cout << "Maximum: " << max << endl << endl;

    // LINEAR SEARCH
    int target = 40;
    bool found = false;
    for (int i = 0; i < 5; i++) {
        if (numbers[i] == target) {
            cout << target << " found at index " << i << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << target << " not found" << endl;
    cout << endl;

    // REVERSE ARRAY
    cout << "Original: ";
    for (int i = 0; i < 5; i++) cout << numbers[i] << " ";
    cout << endl;

    for (int i = 0; i < 5/2; i++) {
        int temp = numbers[i];
        numbers[i] = numbers[4-i];
        numbers[4-i] = temp;
    }

    cout << "Reversed: ";
    for (int i = 0; i < 5; i++) cout << numbers[i] << " ";
    cout << endl << endl;

    // CHARACTER ARRAY (STRING)
    char name[] = "C++";
    cout << "String: " << name << endl;
    cout << "Length: " << sizeof(name) - 1 << endl;

    return 0;
}
