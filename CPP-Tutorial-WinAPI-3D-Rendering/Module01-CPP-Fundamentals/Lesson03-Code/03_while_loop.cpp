/*
 * Program: While Loop Demo
 * Description: Demonstrates while and do-while loops with various examples
 * Compilation: g++ 03_while_loop.cpp -o 03_while_loop
 * Execution: ./03_while_loop
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== While Loop Demo ===" << endl << endl;

    // Basic while loop - counting
    cout << "1. Basic While Loop (Count to 5):" << endl;
    int count = 1;
    while (count <= 5) {
        cout << "Count: " << count << endl;
        count++;
    }
    cout << endl;

    // While loop - sum calculation
    cout << "2. Sum of Numbers (1 to 10):" << endl;
    int i = 1;
    int sum = 0;
    while (i <= 10) {
        sum += i;
        i++;
    }
    cout << "Sum of 1 to 10: " << sum << endl << endl;

    // While loop with user input
    cout << "3. Input Validation:" << endl;
    int number;
    cout << "Enter a positive number: ";
    cin >> number;

    while (number <= 0) {
        cout << "Invalid! Please enter a positive number: ";
        cin >> number;
    }
    cout << "You entered: " << number << endl << endl;

    // Factorial calculation
    cout << "4. Factorial Calculator:" << endl;
    int n;
    cout << "Enter a number: ";
    cin >> n;

    int factorial = 1;
    int temp = n;
    while (temp > 0) {
        factorial *= temp;
        temp--;
    }
    cout << n << "! = " << factorial << endl << endl;

    // Do-while loop (executes at least once)
    cout << "5. Do-While Loop Demo:" << endl;
    int choice;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Say Hello" << endl;
        cout << "2. Say Goodbye" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Hello, World!" << endl;
                break;
            case 2:
                cout << "Goodbye!" << endl;
                break;
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 3);

    // Digit sum using while
    cout << "\n6. Sum of Digits:" << endl;
    int num;
    cout << "Enter a number: ";
    cin >> num;

    int digitSum = 0;
    int original = num;
    while (num > 0) {
        digitSum += num % 10;
        num /= 10;
    }
    cout << "Sum of digits of " << original << " = " << digitSum << endl;

    // Reverse a number
    cout << "\n7. Reverse a Number:" << endl;
    cout << "Enter a number: ";
    cin >> num;

    int reversed = 0;
    original = num;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    cout << "Reversed number: " << reversed << endl;

    return 0;
}
