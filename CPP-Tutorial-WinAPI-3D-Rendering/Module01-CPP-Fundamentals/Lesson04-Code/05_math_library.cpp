/*
 * Program: Math Library Functions
 * Description: Custom math library with various mathematical functions
 * Compilation: g++ 05_math_library.cpp -o 05_math_library
 * Execution: ./05_math_library
 */

#include <iostream>
#include <cmath>
using namespace std;

// Absolute value
int absolute(int x) {
    return (x < 0) ? -x : x;
}

// Maximum of two numbers
int maximum(int a, int b) {
    return (a > b) ? a : b;
}

// Minimum of two numbers
int minimum(int a, int b) {
    return (a < b) ? a : b;
}

// Check if prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Calculate GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Calculate LCM
int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

// Check if perfect number
bool isPerfect(int n) {
    int sum = 0;
    for (int i = 1; i < n; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum == n;
}

// Check if Armstrong number
bool isArmstrong(int n) {
    int original = n, sum = 0, digits = 0;

    // Count digits
    int temp = n;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }

    // Calculate sum of digits raised to power of number of digits
    temp = n;
    while (temp > 0) {
        int digit = temp % 10;
        sum += pow(digit, digits);
        temp /= 10;
    }

    return sum == original;
}

// Calculate square root (Newton's method)
double squareRoot(double n, double epsilon = 0.0001) {
    if (n < 0) return -1;
    if (n == 0) return 0;

    double guess = n / 2.0;
    while (absolute(guess * guess - n) > epsilon) {
        guess = (guess + n / guess) / 2.0;
    }
    return guess;
}

// Calculate factorial
long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Calculate nCr (combinations)
long long nCr(int n, int r) {
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    return factorial(n) / (factorial(r) * factorial(n - r));
}

// Calculate nPr (permutations)
long long nPr(int n, int r) {
    if (r > n) return 0;
    return factorial(n) / factorial(n - r);
}

int main() {
    cout << "=== Math Library Functions ===" << endl << endl;

    // Absolute value
    cout << "1. Absolute Value:" << endl;
    cout << "|−15| = " << absolute(-15) << endl << endl;

    // Max and Min
    cout << "2. Maximum and Minimum:" << endl;
    cout << "Max(10, 25) = " << maximum(10, 25) << endl;
    cout << "Min(10, 25) = " << minimum(10, 25) << endl << endl;

    // Prime check
    cout << "3. Prime Number Check:" << endl;
    cout << "Is 17 prime? " << (isPrime(17) ? "Yes" : "No") << endl;
    cout << "Is 20 prime? " << (isPrime(20) ? "Yes" : "No") << endl << endl;

    // GCD and LCM
    cout << "4. GCD and LCM:" << endl;
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "LCM(48, 18) = " << lcm(48, 18) << endl << endl;

    // Perfect number
    cout << "5. Perfect Number Check:" << endl;
    cout << "Is 6 perfect? " << (isPerfect(6) ? "Yes" : "No") << endl;
    cout << "Is 28 perfect? " << (isPerfect(28) ? "Yes" : "No") << endl << endl;

    // Armstrong number
    cout << "6. Armstrong Number Check:" << endl;
    cout << "Is 153 Armstrong? " << (isArmstrong(153) ? "Yes" : "No") << endl;
    cout << "Is 9474 Armstrong? " << (isArmstrong(9474) ? "Yes" : "No") << endl << endl;

    // Square root
    cout << "7. Square Root (Newton's method):" << endl;
    cout << "√25 = " << squareRoot(25) << endl;
    cout << "√50 = " << squareRoot(50) << endl << endl;

    // Factorial
    cout << "8. Factorial:" << endl;
    cout << "5! = " << factorial(5) << endl;
    cout << "10! = " << factorial(10) << endl << endl;

    // Combinations and Permutations
    cout << "9. Combinations and Permutations:" << endl;
    cout << "5C2 (combinations) = " << nCr(5, 2) << endl;
    cout << "5P2 (permutations) = " << nPr(5, 2) << endl;

    return 0;
}
