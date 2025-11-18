/*
 * Program: Prime Number Programs
 * Description: Various programs related to prime numbers
 * Compilation: g++ 13_prime_numbers.cpp -o 13_prime_numbers
 * Execution: ./13_prime_numbers
 */

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    cout << "=== Prime Number Programs ===" << endl << endl;

    int choice;

    cout << "Select an option:" << endl;
    cout << "1. Check if a number is prime" << endl;
    cout << "2. Print all primes in a range" << endl;
    cout << "3. Print first N prime numbers" << endl;
    cout << "4. Count primes in a range" << endl;
    cout << "5. Sum of all primes up to N" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cout << endl;

    if (choice == 1) {
        // Check if number is prime
        int num;
        cout << "Enter a number: ";
        cin >> num;

        if (num <= 1) {
            cout << num << " is NOT a prime number" << endl;
        } else {
            bool isPrime = true;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) {
                    isPrime = false;
                    cout << num << " is divisible by " << i << endl;
                    break;
                }
            }

            if (isPrime) {
                cout << num << " is a PRIME number" << endl;
            } else {
                cout << num << " is NOT a prime number" << endl;
            }
        }
    }
    else if (choice == 2) {
        // Print all primes in range
        int start, end;
        cout << "Enter start of range: ";
        cin >> start;
        cout << "Enter end of range: ";
        cin >> end;

        cout << "\nPrime numbers between " << start << " and " << end << ":" << endl;

        for (int num = start; num <= end; num++) {
            if (num <= 1) continue;

            bool isPrime = true;
            for (int i = 2; i <= sqrt(num); i++) {
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
    }
    else if (choice == 3) {
        // Print first N primes
        int n;
        cout << "How many prime numbers to print? ";
        cin >> n;

        cout << "\nFirst " << n << " prime numbers:" << endl;

        int count = 0;
        int num = 2;

        while (count < n) {
            bool isPrime = true;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if (isPrime) {
                cout << num << " ";
                count++;
            }
            num++;
        }
        cout << endl;
    }
    else if (choice == 4) {
        // Count primes in range
        int start, end;
        cout << "Enter start of range: ";
        cin >> start;
        cout << "Enter end of range: ";
        cin >> end;

        int count = 0;

        for (int num = start; num <= end; num++) {
            if (num <= 1) continue;

            bool isPrime = true;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if (isPrime) {
                count++;
            }
        }

        cout << "\nNumber of primes between " << start << " and " << end << ": " << count << endl;
    }
    else if (choice == 5) {
        // Sum of primes up to N
        int n;
        cout << "Enter N: ";
        cin >> n;

        int sum = 0;
        int count = 0;

        cout << "\nPrime numbers up to " << n << ":" << endl;

        for (int num = 2; num <= n; num++) {
            bool isPrime = true;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if (isPrime) {
                cout << num << " ";
                sum += num;
                count++;
            }
        }

        cout << endl << endl;
        cout << "Count of primes: " << count << endl;
        cout << "Sum of primes: " << sum << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
