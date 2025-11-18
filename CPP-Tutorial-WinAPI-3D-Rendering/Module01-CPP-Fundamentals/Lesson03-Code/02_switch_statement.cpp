/*
 * Program: Switch Statement Demo
 * Description: Demonstrates switch-case statement with various examples
 * Compilation: g++ 02_switch_statement.cpp -o 02_switch_statement
 * Execution: ./02_switch_statement
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Switch Statement Demo ===" << endl << endl;

    // Basic switch statement
    cout << "1. Day of Week:" << endl;
    int day;
    cout << "Enter day number (1-7): ";
    cin >> day;

    switch (day) {
        case 1:
            cout << "Monday - Start of the week!" << endl;
            break;
        case 2:
            cout << "Tuesday" << endl;
            break;
        case 3:
            cout << "Wednesday - Midweek!" << endl;
            break;
        case 4:
            cout << "Thursday" << endl;
            break;
        case 5:
            cout << "Friday - Almost weekend!" << endl;
            break;
        case 6:
            cout << "Saturday - Weekend!" << endl;
            break;
        case 7:
            cout << "Sunday - Weekend!" << endl;
            break;
        default:
            cout << "Invalid day number!" << endl;
    }
    cout << endl;

    // Switch with multiple cases (fall-through)
    cout << "2. Month Days Calculator:" << endl;
    int month;
    cout << "Enter month number (1-12): ";
    cin >> month;

    int days;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            days = 31;
            break;
        case 4: case 6: case 9: case 11:
            days = 30;
            break;
        case 2:
            days = 28;  // Not accounting for leap years
            cout << "February has " << days << " days (29 in leap years)" << endl;
            break;
        default:
            cout << "Invalid month!" << endl;
            return 1;
    }

    if (month != 2) {
        cout << "This month has " << days << " days" << endl;
    }
    cout << endl;

    // Calculator using switch
    cout << "3. Simple Calculator:" << endl;
    double num1, num2;
    char operation;

    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter operator (+, -, *, /): ";
    cin >> operation;
    cout << "Enter second number: ";
    cin >> num2;

    double result;
    switch (operation) {
        case '+':
            result = num1 + num2;
            cout << num1 << " + " << num2 << " = " << result << endl;
            break;
        case '-':
            result = num1 - num2;
            cout << num1 << " - " << num2 << " = " << result << endl;
            break;
        case '*':
            result = num1 * num2;
            cout << num1 << " * " << num2 << " = " << result << endl;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                cout << num1 << " / " << num2 << " = " << result << endl;
            } else {
                cout << "Error: Division by zero!" << endl;
            }
            break;
        default:
            cout << "Invalid operator!" << endl;
    }
    cout << endl;

    // Grade categorization
    cout << "4. Grade Category:" << endl;
    char grade;
    cout << "Enter your grade (A, B, C, D, F): ";
    cin >> grade;

    switch (grade) {
        case 'A':
        case 'a':
            cout << "Excellent! (90-100)" << endl;
            break;
        case 'B':
        case 'b':
            cout << "Good! (80-89)" << endl;
            break;
        case 'C':
        case 'c':
            cout << "Average (70-79)" << endl;
            break;
        case 'D':
        case 'd':
            cout << "Below Average (60-69)" << endl;
            break;
        case 'F':
        case 'f':
            cout << "Fail (0-59)" << endl;
            break;
        default:
            cout << "Invalid grade!" << endl;
    }

    return 0;
}
