/*
 * Program: Basic If-Else Statement
 * Description: Demonstrates basic if, if-else, and if-else-if statements
 * Compilation: g++ 01_if_else_basic.cpp -o 01_if_else_basic
 * Execution: ./01_if_else_basic
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== If-Else Statement Demo ===" << endl << endl;

    // Simple if statement
    cout << "1. Simple If Statement:" << endl;
    int age;
    cout << "Enter your age: ";
    cin >> age;

    if (age >= 18) {
        cout << "You are an adult." << endl;
    }
    cout << endl;

    // If-else statement
    cout << "2. If-Else Statement:" << endl;
    int number;
    cout << "Enter a number: ";
    cin >> number;

    if (number % 2 == 0) {
        cout << number << " is EVEN" << endl;
    } else {
        cout << number << " is ODD" << endl;
    }
    cout << endl;

    // If-else-if ladder
    cout << "3. If-Else-If Ladder:" << endl;
    int score;
    cout << "Enter your test score (0-100): ";
    cin >> score;

    if (score >= 90) {
        cout << "Grade: A (Excellent!)" << endl;
    } else if (score >= 80) {
        cout << "Grade: B (Good!)" << endl;
    } else if (score >= 70) {
        cout << "Grade: C (Average)" << endl;
    } else if (score >= 60) {
        cout << "Grade: D (Below Average)" << endl;
    } else {
        cout << "Grade: F (Fail)" << endl;
    }
    cout << endl;

    // Nested if statements
    cout << "4. Nested If Statements:" << endl;
    int year;
    cout << "Enter a year: ";
    cin >> year;

    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                cout << year << " is a LEAP year" << endl;
            } else {
                cout << year << " is NOT a leap year" << endl;
            }
        } else {
            cout << year << " is a LEAP year" << endl;
        }
    } else {
        cout << year << " is NOT a leap year" << endl;
    }

    // Logical operators
    cout << "\n5. Logical Operators:" << endl;
    int temp;
    cout << "Enter temperature in Celsius: ";
    cin >> temp;

    if (temp < 0) {
        cout << "It's freezing!" << endl;
    } else if (temp >= 0 && temp < 15) {
        cout << "It's cold." << endl;
    } else if (temp >= 15 && temp < 25) {
        cout << "It's comfortable." << endl;
    } else if (temp >= 25 && temp < 35) {
        cout << "It's warm." << endl;
    } else {
        cout << "It's hot!" << endl;
    }

    return 0;
}
