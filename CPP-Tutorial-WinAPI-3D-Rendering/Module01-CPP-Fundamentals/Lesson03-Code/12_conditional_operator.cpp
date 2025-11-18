/*
 * Program: Conditional (Ternary) Operator Demo
 * Description: Demonstrates the ternary operator (? :)
 * Compilation: g++ 12_conditional_operator.cpp -o 12_conditional_operator
 * Execution: ./12_conditional_operator
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== Conditional (Ternary) Operator Demo ===" << endl << endl;

    // Basic ternary operator
    cout << "1. Basic Ternary Operator:" << endl;
    int age;
    cout << "Enter your age: ";
    cin >> age;

    string status = (age >= 18) ? "Adult" : "Minor";
    cout << "You are: " << status << endl << endl;

    // Find max using ternary
    cout << "2. Find Maximum of Two Numbers:" << endl;
    int a, b;
    cout << "Enter first number: ";
    cin >> a;
    cout << "Enter second number: ";
    cin >> b;

    int max = (a > b) ? a : b;
    cout << "Maximum: " << max << endl << endl;

    // Even or odd
    cout << "3. Even or Odd Check:" << endl;
    int number;
    cout << "Enter a number: ";
    cin >> number;

    string parity = (number % 2 == 0) ? "Even" : "Odd";
    cout << number << " is " << parity << endl << endl;

    // Absolute value
    cout << "4. Absolute Value:" << endl;
    int value;
    cout << "Enter a number: ";
    cin >> value;

    int absolute = (value >= 0) ? value : -value;
    cout << "Absolute value: " << absolute << endl << endl;

    // Nested ternary operators
    cout << "5. Nested Ternary (Find largest of three):" << endl;
    int x, y, z;
    cout << "Enter three numbers: ";
    cin >> x >> y >> z;

    int largest = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    cout << "Largest: " << largest << endl << endl;

    // Grade assignment
    cout << "6. Grade Assignment:" << endl;
    int score;
    cout << "Enter score (0-100): ";
    cin >> score;

    char grade = (score >= 90) ? 'A' :
                 (score >= 80) ? 'B' :
                 (score >= 70) ? 'C' :
                 (score >= 60) ? 'D' : 'F';

    cout << "Grade: " << grade << endl << endl;

    // Discount calculation
    cout << "7. Discount Calculation:" << endl;
    double price;
    bool isMember;
    cout << "Enter price: $";
    cin >> price;
    cout << "Are you a member? (1=Yes, 0=No): ";
    cin >> isMember;

    double discount = isMember ? 0.20 : 0.10;
    double finalPrice = price * (1 - discount);

    cout << "Discount: " << (discount * 100) << "%" << endl;
    cout << "Final price: $" << finalPrice << endl << endl;

    // Leap year check
    cout << "8. Leap Year Check:" << endl;
    int year;
    cout << "Enter year: ";
    cin >> year;

    bool isLeap = (year % 4 == 0) ? ((year % 100 == 0) ? ((year % 400 == 0) ? true : false) : true) : false;
    cout << year << " is " << (isLeap ? "a leap year" : "not a leap year") << endl << endl;

    // Sign of number
    cout << "9. Sign of Number:" << endl;
    int num;
    cout << "Enter a number: ";
    cin >> num;

    string sign = (num > 0) ? "Positive" : (num < 0) ? "Negative" : "Zero";
    cout << "The number is: " << sign << endl << endl;

    // Temperature status
    cout << "10. Temperature Status:" << endl;
    int temp;
    cout << "Enter temperature (Celsius): ";
    cin >> temp;

    string tempStatus = (temp < 0) ? "Freezing" :
                       (temp < 15) ? "Cold" :
                       (temp < 25) ? "Comfortable" :
                       (temp < 35) ? "Warm" : "Hot";

    cout << "Temperature status: " << tempStatus << endl;

    return 0;
}
