/*
 * Program: Factorial and Fibonacci
 * Description: Calculates factorial and generates Fibonacci series
 * Compilation: g++ 14_factorial_fibonacci.cpp -o 14_factorial_fibonacci
 * Execution: ./14_factorial_fibonacci
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Factorial and Fibonacci Programs ===" << endl << endl;

    int choice;

    cout << "Select an option:" << endl;
    cout << "1. Calculate Factorial" << endl;
    cout << "2. Generate Fibonacci Series" << endl;
    cout << "3. Check if number is in Fibonacci sequence" << endl;
    cout << "4. Factorial table (1 to N)" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cout << endl;

    if (choice == 1) {
        // Calculate factorial
        int n;
        cout << "Enter a number: ";
        cin >> n;

        if (n < 0) {
            cout << "Factorial is not defined for negative numbers!" << endl;
        } else {
            long long factorial = 1;

            cout << n << "! = ";
            for (int i = n; i >= 1; i--) {
                factorial *= i;
                cout << i;
                if (i > 1) cout << " × ";
            }

            cout << " = " << factorial << endl;
        }
    }
    else if (choice == 2) {
        // Fibonacci series
        int n;
        cout << "How many Fibonacci numbers to generate? ";
        cin >> n;

        if (n <= 0) {
            cout << "Please enter a positive number!" << endl;
        } else {
            cout << "\nFibonacci Series (" << n << " terms):" << endl;

            long long first = 0, second = 1, next;

            for (int i = 0; i < n; i++) {
                if (i == 0) {
                    next = 0;
                } else if (i == 1) {
                    next = 1;
                } else {
                    next = first + second;
                    first = second;
                    second = next;
                }

                cout << next;
                if (i < n - 1) cout << ", ";
            }
            cout << endl;
        }
    }
    else if (choice == 3) {
        // Check if number is in Fibonacci sequence
        long long num;
        cout << "Enter a number: ";
        cin >> num;

        if (num < 0) {
            cout << num << " is NOT in the Fibonacci sequence" << endl;
        } else {
            long long a = 0, b = 1, next = 0;
            bool found = false;

            if (num == 0 || num == 1) {
                found = true;
            } else {
                while (next < num) {
                    next = a + b;
                    a = b;
                    b = next;

                    if (next == num) {
                        found = true;
                        break;
                    }
                }
            }

            if (found) {
                cout << num << " IS in the Fibonacci sequence" << endl;
            } else {
                cout << num << " is NOT in the Fibonacci sequence" << endl;
            }
        }
    }
    else if (choice == 4) {
        // Factorial table
        int n;
        cout << "Generate factorial table from 1 to: ";
        cin >> n;

        if (n < 1 || n > 20) {
            cout << "Please enter a number between 1 and 20!" << endl;
        } else {
            cout << "\nFactorial Table:" << endl;
            cout << "N\tN!" << endl;
            cout << "-------------------" << endl;

            long long factorial = 1;
            for (int i = 1; i <= n; i++) {
                factorial *= i;
                cout << i << "\t" << factorial << endl;
            }
        }
    }
    else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    // Additional calculations
    cout << "\n=== Additional Information ===" << endl;

    if (choice == 1) {
        int n;
        cout << "Enter another number for factorial: ";
        cin >> n;

        if (n >= 0) {
            // Using while loop
            long long factorial = 1;
            int temp = n;

            while (temp > 0) {
                factorial *= temp;
                temp--;
            }

            cout << n << "! = " << factorial << endl;

            // Calculate number of digits
            int digits = 0;
            long long tempFactorial = factorial;
            while (tempFactorial > 0) {
                digits++;
                tempFactorial /= 10;
            }
            cout << "Number of digits: " << digits << endl;
        }
    }
    else if (choice == 2) {
        // Show golden ratio approximation
        cout << "Golden Ratio approximation (ratio of consecutive Fibonacci numbers):" << endl;

        long long a = 0, b = 1, next;
        for (int i = 0; i < 10; i++) {
            next = a + b;
            if (b != 0) {
                double ratio = static_cast<double>(next) / b;
                cout << "F(" << (i + 2) << ")/F(" << (i + 1) << ") = " << next << "/" << b << " = " << ratio << endl;
            }
            a = b;
            b = next;
        }
        cout << "Golden Ratio ≈ 1.618033988..." << endl;
    }

    return 0;
}
