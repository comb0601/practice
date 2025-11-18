/*
 * Program: Arithmetic Operations
 * Description: Demonstrates all arithmetic operators and operations
 * Compilation: g++ 09_arithmetic_operations.cpp -o 09_arithmetic_operations
 * Execution: ./09_arithmetic_operations
 */

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    cout << "=== Arithmetic Operations ===" << endl << endl;

    // Basic arithmetic operators
    int a = 17, b = 5;
    cout << "Given: a = " << a << ", b = " << b << endl << endl;

    cout << "1. Basic Operators:" << endl;
    cout << "Addition (a + b): " << (a + b) << endl;
    cout << "Subtraction (a - b): " << (a - b) << endl;
    cout << "Multiplication (a * b): " << (a * b) << endl;
    cout << "Division (a / b): " << (a / b) << endl;
    cout << "Modulus (a % b): " << (a % b) << endl << endl;

    // Increment and decrement
    cout << "2. Increment and Decrement:" << endl;
    int x = 10;
    cout << "Initial x: " << x << endl;
    cout << "x++: " << x++ << " (post-increment, x is now " << x << ")" << endl;
    cout << "++x: " << ++x << " (pre-increment)" << endl;
    cout << "x--: " << x-- << " (post-decrement, x is now " << x << ")" << endl;
    cout << "--x: " << --x << " (pre-decrement)" << endl << endl;

    // Compound assignment operators
    cout << "3. Compound Assignment:" << endl;
    int num = 10;
    cout << "Initial num: " << num << endl;

    num += 5;  // num = num + 5
    cout << "After num += 5: " << num << endl;

    num -= 3;  // num = num - 3
    cout << "After num -= 3: " << num << endl;

    num *= 2;  // num = num * 2
    cout << "After num *= 2: " << num << endl;

    num /= 4;  // num = num / 4
    cout << "After num /= 4: " << num << endl;

    num %= 5;  // num = num % 5
    cout << "After num %= 5: " << num << endl << endl;

    // Math library functions
    cout << "4. Math Library Functions:" << endl;
    double value = 16.5;
    cout << "Value: " << value << endl;
    cout << "sqrt(" << value << "): " << sqrt(value) << endl;
    cout << "pow(" << value << ", 2): " << pow(value, 2) << endl;
    cout << "abs(-" << value << "): " << abs(-value) << endl;
    cout << "ceil(" << value << "): " << ceil(value) << endl;
    cout << "floor(" << value << "): " << floor(value) << endl;
    cout << "round(" << value << "): " << round(value) << endl << endl;

    // Order of operations
    cout << "5. Order of Operations:" << endl;
    int result1 = 2 + 3 * 4;  // Multiplication first
    int result2 = (2 + 3) * 4;  // Parentheses first
    cout << "2 + 3 * 4 = " << result1 << " (multiplication first)" << endl;
    cout << "(2 + 3) * 4 = " << result2 << " (parentheses first)" << endl << endl;

    // Mixed-type arithmetic
    cout << "6. Mixed-Type Arithmetic:" << endl;
    int intNum = 7;
    double doubleNum = 2.5;
    double mixedResult = intNum / doubleNum;
    cout << intNum << " / " << doubleNum << " = " << mixedResult << endl;

    return 0;
}
