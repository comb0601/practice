/*
 * Lesson 6: While Loop - Repetition Control
 *
 * This program demonstrates:
 * - Basic while loop
 * - While loop with counter
 * - While loop with sentinel value
 * - Do-while loop
 * - Infinite loops and break
 * - Practical applications
 *
 * Compile: cl Lesson06_WhileLoop.cpp
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== C++ While Loop Demo ===" << endl << endl;

    // BASIC WHILE LOOP
    cout << "=== BASIC WHILE LOOP ===" << endl;
    int count = 1;

    cout << "Counting from 1 to 5:" << endl;
    while (count <= 5) {
        cout << count << " ";
        count++;
    }
    cout << endl << endl;

    // WHILE LOOP WITH DECREMENT
    cout << "=== COUNTDOWN ===" << endl;
    int countdown = 10;

    cout << "Countdown from 10:" << endl;
    while (countdown > 0) {
        cout << countdown << "... ";
        countdown--;
    }
    cout << "Blast off!" << endl << endl;

    // SUM OF NUMBERS
    cout << "=== SUM OF NUMBERS ===" << endl;
    int num = 1;
    int sum = 0;

    while (num <= 10) {
        sum += num;
        num++;
    }
    cout << "Sum of numbers 1 to 10: " << sum << endl << endl;

    // FACTORIAL CALCULATION
    cout << "=== FACTORIAL ===" << endl;
    int n = 5;
    int factorial = 1;
    int i = 1;

    while (i <= n) {
        factorial *= i;
        i++;
    }
    cout << n << "! = " << factorial << endl << endl;

    // MULTIPLICATION TABLE
    cout << "=== MULTIPLICATION TABLE ===" << endl;
    int table = 7;
    int multiplier = 1;

    cout << "Multiplication table for " << table << ":" << endl;
    while (multiplier <= 10) {
        cout << table << " x " << multiplier << " = " << (table * multiplier) << endl;
        multiplier++;
    }
    cout << endl;

    // POWER CALCULATION
    cout << "=== POWER CALCULATION ===" << endl;
    int base = 2;
    int exponent = 8;
    int result = 1;
    int exp_counter = 0;

    while (exp_counter < exponent) {
        result *= base;
        exp_counter++;
    }
    cout << base << "^" << exponent << " = " << result << endl << endl;

    // DIGIT COUNT
    cout << "=== DIGIT COUNT ===" << endl;
    int number = 12345;
    int temp = number;
    int digitCount = 0;

    while (temp > 0) {
        temp /= 10;
        digitCount++;
    }
    cout << "Number " << number << " has " << digitCount << " digits" << endl << endl;

    // REVERSE NUMBER
    cout << "=== REVERSE NUMBER ===" << endl;
    int original = 12345;
    int reversed = 0;
    temp = original;

    while (temp > 0) {
        int digit = temp % 10;
        reversed = reversed * 10 + digit;
        temp /= 10;
    }
    cout << "Original: " << original << endl;
    cout << "Reversed: " << reversed << endl << endl;

    // DO-WHILE LOOP
    cout << "=== DO-WHILE LOOP ===" << endl;
    int value = 1;

    cout << "Do-while executes at least once:" << endl;
    do {
        cout << "Value: " << value << endl;
        value++;
    } while (value <= 5);
    cout << endl;

    // DO-WHILE vs WHILE
    cout << "=== DO-WHILE vs WHILE ===" << endl;
    int test = 10;

    cout << "While loop (condition false from start):" << endl;
    while (test < 10) {
        cout << "This won't print" << endl;
    }
    cout << "While loop didn't execute" << endl << endl;

    test = 10;
    cout << "Do-while loop (condition false from start):" << endl;
    do {
        cout << "This prints once! (value: " << test << ")" << endl;
    } while (test < 10);
    cout << endl;

    // USER INPUT VALIDATION
    cout << "=== INPUT VALIDATION ===" << endl;
    int userInput = 0;
    int attempts = 0;

    // Simulate input validation (in real program, use cin)
    cout << "Enter a number between 1 and 10:" << endl;
    while (userInput < 1 || userInput > 10) {
        attempts++;
        userInput = 5;  // Simulated input
        cout << "Attempt " << attempts << ": Valid input received: " << userInput << endl;
        break;  // Break for demo purposes
    }
    cout << endl;

    // FIBONACCI SEQUENCE
    cout << "=== FIBONACCI SEQUENCE ===" << endl;
    int fib1 = 0, fib2 = 1;
    int fibCount = 0;
    int maxFib = 10;

    cout << "First " << maxFib << " Fibonacci numbers:" << endl;
    while (fibCount < maxFib) {
        if (fibCount == 0) {
            cout << fib1 << " ";
        } else if (fibCount == 1) {
            cout << fib2 << " ";
        } else {
            int nextFib = fib1 + fib2;
            cout << nextFib << " ";
            fib1 = fib2;
            fib2 = nextFib;
        }
        fibCount++;
    }
    cout << endl << endl;

    // PRIME NUMBER CHECK
    cout << "=== PRIME NUMBER CHECK ===" << endl;
    int primeTest = 17;
    bool isPrime = true;
    int divisor = 2;

    cout << "Checking if " << primeTest << " is prime:" << endl;

    if (primeTest <= 1) {
        isPrime = false;
    } else {
        while (divisor * divisor <= primeTest) {
            if (primeTest % divisor == 0) {
                isPrime = false;
                break;
            }
            divisor++;
        }
    }

    if (isPrime) {
        cout << primeTest << " is a prime number" << endl;
    } else {
        cout << primeTest << " is not a prime number" << endl;
    }
    cout << endl;

    // GCD CALCULATION
    cout << "=== GCD (EUCLIDEAN ALGORITHM) ===" << endl;
    int a = 48, b = 18;
    int gcd_a = a, gcd_b = b;

    cout << "Finding GCD of " << a << " and " << b << ":" << endl;

    while (gcd_b != 0) {
        int temp = gcd_b;
        gcd_b = gcd_a % gcd_b;
        gcd_a = temp;
    }

    cout << "GCD(" << a << ", " << b << ") = " << gcd_a << endl << endl;

    // MENU SYSTEM
    cout << "=== MENU SYSTEM ===" << endl;
    int menuChoice = 0;
    bool exitMenu = false;

    cout << "Simple Menu System:" << endl;
    while (!exitMenu) {
        cout << "\n1. Option 1" << endl;
        cout << "2. Option 2" << endl;
        cout << "3. Option 3" << endl;
        cout << "4. Exit" << endl;

        menuChoice = 4;  // Simulated choice

        switch (menuChoice) {
            case 1:
                cout << "You selected Option 1" << endl;
                break;
            case 2:
                cout << "You selected Option 2" << endl;
                break;
            case 3:
                cout << "You selected Option 3" << endl;
                break;
            case 4:
                cout << "Exiting menu..." << endl;
                exitMenu = true;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}

/*
 * Expected Output:
 * === C++ While Loop Demo ===
 *
 * === BASIC WHILE LOOP ===
 * Counting from 1 to 5:
 * 1 2 3 4 5
 *
 * === COUNTDOWN ===
 * Countdown from 10:
 * 10... 9... 8... 7... 6... 5... 4... 3... 2... 1... Blast off!
 *
 * === SUM OF NUMBERS ===
 * Sum of numbers 1 to 10: 55
 *
 * === FACTORIAL ===
 * 5! = 120
 *
 * ... (continues with all examples)
 */
