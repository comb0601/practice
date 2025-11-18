/*
 * Lesson 3: Operators - All C++ Operators
 *
 * This program demonstrates all types of operators in C++:
 * - Arithmetic operators
 * - Relational/Comparison operators
 * - Logical operators
 * - Bitwise operators
 * - Assignment operators
 * - Increment/Decrement operators
 * - Conditional (ternary) operator
 * - sizeof operator
 *
 * Compile: cl Lesson03_Operators.cpp
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== C++ Operators Demo ===" << endl << endl;

    // ARITHMETIC OPERATORS
    cout << "=== ARITHMETIC OPERATORS ===" << endl;
    int a = 10, b = 3;

    cout << "a = " << a << ", b = " << b << endl;
    cout << "Addition (a + b): " << (a + b) << endl;
    cout << "Subtraction (a - b): " << (a - b) << endl;
    cout << "Multiplication (a * b): " << (a * b) << endl;
    cout << "Division (a / b): " << (a / b) << " (integer division)" << endl;
    cout << "Modulus (a % b): " << (a % b) << " (remainder)" << endl;

    double x = 10.0, y = 3.0;
    cout << "Float division (10.0 / 3.0): " << (x / y) << endl << endl;

    // RELATIONAL/COMPARISON OPERATORS
    cout << "=== RELATIONAL OPERATORS ===" << endl;
    int num1 = 5, num2 = 10;

    cout << "num1 = " << num1 << ", num2 = " << num2 << endl;
    cout << boolalpha;  // Display true/false instead of 1/0
    cout << "num1 == num2: " << (num1 == num2) << " (equal to)" << endl;
    cout << "num1 != num2: " << (num1 != num2) << " (not equal to)" << endl;
    cout << "num1 > num2: " << (num1 > num2) << " (greater than)" << endl;
    cout << "num1 < num2: " << (num1 < num2) << " (less than)" << endl;
    cout << "num1 >= num2: " << (num1 >= num2) << " (greater than or equal)" << endl;
    cout << "num1 <= num2: " << (num1 <= num2) << " (less than or equal)" << endl << endl;

    // LOGICAL OPERATORS
    cout << "=== LOGICAL OPERATORS ===" << endl;
    bool p = true, q = false;

    cout << "p = " << p << ", q = " << q << endl;
    cout << "p && q: " << (p && q) << " (logical AND)" << endl;
    cout << "p || q: " << (p || q) << " (logical OR)" << endl;
    cout << "!p: " << (!p) << " (logical NOT)" << endl;
    cout << "!q: " << (!q) << endl << endl;

    // BITWISE OPERATORS
    cout << "=== BITWISE OPERATORS ===" << endl;
    unsigned int m = 12;  // Binary: 1100
    unsigned int n = 10;  // Binary: 1010

    cout << "m = " << m << " (binary: 1100)" << endl;
    cout << "n = " << n << " (binary: 1010)" << endl;
    cout << "m & n: " << (m & n) << " (bitwise AND)" << endl;
    cout << "m | n: " << (m | n) << " (bitwise OR)" << endl;
    cout << "m ^ n: " << (m ^ n) << " (bitwise XOR)" << endl;
    cout << "~m: " << (~m) << " (bitwise NOT)" << endl;
    cout << "m << 1: " << (m << 1) << " (left shift)" << endl;
    cout << "m >> 1: " << (m >> 1) << " (right shift)" << endl << endl;

    // ASSIGNMENT OPERATORS
    cout << "=== ASSIGNMENT OPERATORS ===" << endl;
    int c = 10;

    cout << "Initial value c = " << c << endl;
    c += 5;  // c = c + 5
    cout << "c += 5: " << c << endl;
    c -= 3;  // c = c - 3
    cout << "c -= 3: " << c << endl;
    c *= 2;  // c = c * 2
    cout << "c *= 2: " << c << endl;
    c /= 4;  // c = c / 4
    cout << "c /= 4: " << c << endl;
    c %= 5;  // c = c % 5
    cout << "c %= 5: " << c << endl << endl;

    // INCREMENT AND DECREMENT OPERATORS
    cout << "=== INCREMENT/DECREMENT OPERATORS ===" << endl;
    int count = 5;

    cout << "Initial count = " << count << endl;
    cout << "count++: " << count++ << " (post-increment, returns then increments)" << endl;
    cout << "After post-increment: " << count << endl;
    cout << "++count: " << ++count << " (pre-increment, increments then returns)" << endl;
    cout << "count--: " << count-- << " (post-decrement)" << endl;
    cout << "After post-decrement: " << count << endl;
    cout << "--count: " << --count << " (pre-decrement)" << endl << endl;

    // CONDITIONAL (TERNARY) OPERATOR
    cout << "=== CONDITIONAL OPERATOR ===" << endl;
    int age = 20;
    string status = (age >= 18) ? "Adult" : "Minor";

    cout << "Age: " << age << endl;
    cout << "Status: " << status << endl;

    int max = (a > b) ? a : b;
    cout << "Maximum of " << a << " and " << b << " is: " << max << endl << endl;

    // SIZEOF OPERATOR
    cout << "=== SIZEOF OPERATOR ===" << endl;
    cout << "sizeof(int): " << sizeof(int) << " bytes" << endl;
    cout << "sizeof(double): " << sizeof(double) << " bytes" << endl;
    cout << "sizeof(char): " << sizeof(char) << " byte" << endl;
    cout << "sizeof(bool): " << sizeof(bool) << " byte" << endl;

    int arr[10];
    cout << "sizeof(int[10]): " << sizeof(arr) << " bytes" << endl << endl;

    // COMMA OPERATOR
    cout << "=== COMMA OPERATOR ===" << endl;
    int result = (a = 1, b = 2, a + b);
    cout << "result = (a = 1, b = 2, a + b): " << result << endl;
    cout << "The comma operator evaluates left to right and returns the last value" << endl << endl;

    // OPERATOR PRECEDENCE EXAMPLE
    cout << "=== OPERATOR PRECEDENCE ===" << endl;
    int value = 2 + 3 * 4;  // Multiplication first, then addition
    cout << "2 + 3 * 4 = " << value << " (multiplication has higher precedence)" << endl;

    int value2 = (2 + 3) * 4;  // Parentheses first
    cout << "(2 + 3) * 4 = " << value2 << " (parentheses override precedence)" << endl << endl;

    // PRACTICAL EXAMPLES
    cout << "=== PRACTICAL EXAMPLES ===" << endl;

    // Check if number is even or odd
    int number = 17;
    cout << number << " is " << ((number % 2 == 0) ? "even" : "odd") << endl;

    // Calculate compound interest
    double principal = 1000;
    double rate = 0.05;  // 5%
    int years = 3;
    double amount = principal * (1 + rate) * (1 + rate) * (1 + rate);
    cout << fixed << setprecision(2);
    cout << "Investment: $" << principal << " at " << (rate * 100) << "% for "
         << years << " years" << endl;
    cout << "Final amount: $" << amount << endl;

    return 0;
}

/*
 * Expected Output:
 * === C++ Operators Demo ===
 *
 * === ARITHMETIC OPERATORS ===
 * a = 10, b = 3
 * Addition (a + b): 13
 * Subtraction (a - b): 7
 * Multiplication (a * b): 30
 * Division (a / b): 3 (integer division)
 * Modulus (a % b): 1 (remainder)
 * Float division (10.0 / 3.0): 3.33333
 *
 * ... (continues with all operator demonstrations)
 */
