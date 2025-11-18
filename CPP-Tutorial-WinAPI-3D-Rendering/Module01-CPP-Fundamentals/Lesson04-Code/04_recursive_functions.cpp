/*
 * Program: Recursive Functions Demo
 * Description: Demonstrates recursion with various examples
 * Compilation: g++ 04_recursive_functions.cpp -o 04_recursive_functions
 * Execution: ./04_recursive_functions
 */

#include <iostream>
using namespace std;

// Factorial using recursion
long long factorial(int n) {
    if (n <= 1) {
        return 1;  // Base case
    }
    return n * factorial(n - 1);  // Recursive case
}

// Fibonacci using recursion
int fibonacci(int n) {
    if (n <= 1) {
        return n;  // Base case
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // Recursive case
}

// Sum of numbers from 1 to n
int sum(int n) {
    if (n == 0) {
        return 0;
    }
    return n + sum(n - 1);
}

// Power function (x^n)
double power(double x, int n) {
    if (n == 0) {
        return 1;
    }
    if (n < 0) {
        return 1.0 / power(x, -n);
    }
    return x * power(x, n - 1);
}

// Reverse a number
int reverseNumber(int num, int reversed = 0) {
    if (num == 0) {
        return reversed;
    }
    return reverseNumber(num / 10, reversed * 10 + num % 10);
}

// Sum of digits
int sumOfDigits(int num) {
    if (num == 0) {
        return 0;
    }
    return (num % 10) + sumOfDigits(num / 10);
}

// GCD using Euclidean algorithm (recursive)
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Print numbers from n to 1
void printDescending(int n) {
    if (n == 0) {
        return;
    }
    cout << n << " ";
    printDescending(n - 1);
}

// Print numbers from 1 to n
void printAscending(int n) {
    if (n == 0) {
        return;
    }
    printAscending(n - 1);
    cout << n << " ";
}

// Binary search (recursive)
int binarySearch(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;  // Not found
    }
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) {
        return mid;
    }
    if (arr[mid] > target) {
        return binarySearch(arr, left, mid - 1, target);
    }
    return binarySearch(arr, mid + 1, right, target);
}

int main() {
    cout << "=== Recursive Functions Demo ===" << endl << endl;

    // Factorial
    cout << "1. Factorial:" << endl;
    cout << "5! = " << factorial(5) << endl;
    cout << "10! = " << factorial(10) << endl << endl;

    // Fibonacci
    cout << "2. Fibonacci Sequence (first 10):" << endl;
    for (int i = 0; i < 10; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << endl << endl;

    // Sum
    cout << "3. Sum of 1 to n:" << endl;
    cout << "Sum of 1 to 10: " << sum(10) << endl;
    cout << "Sum of 1 to 100: " << sum(100) << endl << endl;

    // Power
    cout << "4. Power Function:" << endl;
    cout << "2^5 = " << power(2, 5) << endl;
    cout << "3^4 = " << power(3, 4) << endl;
    cout << "2^-3 = " << power(2, -3) << endl << endl;

    // Reverse number
    cout << "5. Reverse Number:" << endl;
    cout << "Reverse of 12345: " << reverseNumber(12345) << endl << endl;

    // Sum of digits
    cout << "6. Sum of Digits:" << endl;
    cout << "Sum of digits of 12345: " << sumOfDigits(12345) << endl << endl;

    // GCD
    cout << "7. GCD (Greatest Common Divisor):" << endl;
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "GCD(100, 50) = " << gcd(100, 50) << endl << endl;

    // Print sequences
    cout << "8. Print Descending (10 to 1): ";
    printDescending(10);
    cout << endl;

    cout << "9. Print Ascending (1 to 10): ";
    printAscending(10);
    cout << endl << endl;

    // Binary search
    cout << "10. Binary Search:" << endl;
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 67, 78};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target = 23;
    int index = binarySearch(arr, 0, size - 1, target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }

    return 0;
}
