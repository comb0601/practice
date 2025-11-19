/*
 * Lesson 14: Recursion - Functions Calling Themselves
 *
 * Demonstrates recursive functions and their applications
 *
 * Compile: cl Lesson14_Recursion.cpp
 */

#include <iostream>
using namespace std;

// Factorial
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Fibonacci
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Sum of n numbers
int sum(int n) {
    if (n == 0) return 0;
    return n + sum(n - 1);
}

// Power
int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

// GCD (Greatest Common Divisor)
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Binary search (recursive)
int binarySearch(int arr[], int left, int right, int target) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binarySearch(arr, left, mid - 1, target);
    return binarySearch(arr, mid + 1, right, target);
}

// Print numbers (recursive)
void printNumbers(int n) {
    if (n == 0) return;
    printNumbers(n - 1);
    cout << n << " ";
}

// Reverse print
void reversePrint(int n) {
    if (n == 0) return;
    cout << n << " ";
    reversePrint(n - 1);
}

int main() {
    cout << "=== Recursion Demo ===" << endl << endl;

    // FACTORIAL
    cout << "=== FACTORIAL ===" << endl;
    cout << "5! = " << factorial(5) << endl;
    cout << "10! = " << factorial(10) << endl << endl;

    // FIBONACCI
    cout << "=== FIBONACCI ===" << endl;
    cout << "First 10 Fibonacci numbers:" << endl;
    for (int i = 0; i < 10; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << endl << endl;

    // SUM
    cout << "=== SUM OF N NUMBERS ===" << endl;
    cout << "Sum(1 to 10) = " << sum(10) << endl;
    cout << "Sum(1 to 100) = " << sum(100) << endl << endl;

    // POWER
    cout << "=== POWER ===" << endl;
    cout << "2^10 = " << power(2, 10) << endl;
    cout << "5^3 = " << power(5, 3) << endl << endl;

    // GCD
    cout << "=== GCD ===" << endl;
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "GCD(100, 35) = " << gcd(100, 35) << endl << endl;

    // BINARY SEARCH
    cout << "=== BINARY SEARCH ===" << endl;
    int arr[] = {10, 20, 30, 40, 50, 60, 70};
    int target = 40;
    int result = binarySearch(arr, 0, 6, target);
    cout << "Searching for " << target << ": ";
    if (result != -1) {
        cout << "Found at index " << result << endl;
    } else {
        cout << "Not found" << endl;
    }
    cout << endl;

    // PRINT NUMBERS
    cout << "=== RECURSIVE PRINT ===" << endl;
    cout << "Forward: ";
    printNumbers(10);
    cout << endl;
    cout << "Reverse: ";
    reversePrint(10);
    cout << endl;

    return 0;
}
