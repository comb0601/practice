/*
 * Program: Basic Arrays
 * Description: Introduction to arrays - declaration, initialization, and access
 * Compilation: g++ 01_basic_arrays.cpp -o 01_basic_arrays
 * Execution: ./01_basic_arrays
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Basic Arrays Demo ===" << endl << endl;

    // Array declaration and initialization
    cout << "1. Array Declaration and Initialization:" << endl;
    int numbers[5] = {10, 20, 30, 40, 50};

    cout << "Array elements: ";
    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    // Accessing and modifying elements
    cout << "2. Accessing and Modifying Elements:" << endl;
    cout << "First element: " << numbers[0] << endl;
    cout << "Last element: " << numbers[4] << endl;

    numbers[2] = 35;  // Modify third element
    cout << "After modifying index 2: ";
    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    // Partial initialization
    cout << "3. Partial Initialization:" << endl;
    int partial[5] = {1, 2};  // Rest initialized to 0
    cout << "Partially initialized: ";
    for (int i = 0; i < 5; i++) {
        cout << partial[i] << " ";
    }
    cout << endl << endl;

    // Array size calculation
    cout << "4. Array Size:" << endl;
    int size = sizeof(numbers) / sizeof(numbers[0]);
    cout << "Array size: " << size << " elements" << endl;
    cout << "Total bytes: " << sizeof(numbers) << endl << endl;

    // Taking input
    cout << "5. Input Array Elements:" << endl;
    int userArray[3];
    cout << "Enter 3 numbers: ";
    for (int i = 0; i < 3; i++) {
        cin >> userArray[i];
    }

    cout << "You entered: ";
    for (int i = 0; i < 3; i++) {
        cout << userArray[i] << " ";
    }
    cout << endl << endl;

    // Sum and average
    cout << "6. Sum and Average:" << endl;
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    double average = static_cast<double>(sum) / 5;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << average << endl << endl;

    // Finding maximum and minimum
    cout << "7. Maximum and Minimum:" << endl;
    int max = numbers[0], min = numbers[0];
    for (int i = 1; i < 5; i++) {
        if (numbers[i] > max) max = numbers[i];
        if (numbers[i] < min) min = numbers[i];
    }
    cout << "Maximum: " << max << endl;
    cout << "Minimum: " << min << endl;

    return 0;
}
