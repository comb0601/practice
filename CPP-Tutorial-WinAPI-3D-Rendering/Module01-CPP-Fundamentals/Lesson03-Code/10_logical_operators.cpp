/*
 * Program: Logical Operators Demo
 * Description: Demonstrates AND, OR, NOT logical operators
 * Compilation: g++ 10_logical_operators.cpp -o 10_logical_operators
 * Execution: ./10_logical_operators
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Logical Operators Demo ===" << endl << endl;

    // AND operator (&&)
    cout << "1. AND Operator (&&):" << endl;
    int age;
    cout << "Enter your age: ";
    cin >> age;

    if (age >= 18 && age <= 65) {
        cout << "You are in the working age group (18-65)" << endl;
    } else if (age < 18) {
        cout << "You are under 18" << endl;
    } else {
        cout << "You are over 65 (retirement age)" << endl;
    }
    cout << endl;

    // OR operator (||)
    cout << "2. OR Operator (||):" << endl;
    char grade;
    cout << "Enter your grade (A, B, C, D, F): ";
    cin >> grade;

    if (grade == 'A' || grade == 'B') {
        cout << "Excellent or Good! You passed with honors." << endl;
    } else if (grade == 'C' || grade == 'D') {
        cout << "You passed, but there's room for improvement." << endl;
    } else {
        cout << "You failed. Better luck next time." << endl;
    }
    cout << endl;

    // NOT operator (!)
    cout << "3. NOT Operator (!):" << endl;
    bool hasLicense;
    cout << "Do you have a driver's license? (1 = Yes, 0 = No): ";
    cin >> hasLicense;

    if (!hasLicense) {
        cout << "You cannot drive. Get a license first!" << endl;
    } else {
        cout << "You can drive!" << endl;
    }
    cout << endl;

    // Combining multiple operators
    cout << "4. Combined Logical Operators:" << endl;
    int temperature;
    bool isRaining;

    cout << "Enter temperature (Celsius): ";
    cin >> temperature;
    cout << "Is it raining? (1 = Yes, 0 = No): ";
    cin >> isRaining;

    if (temperature >= 20 && temperature <= 30 && !isRaining) {
        cout << "Perfect weather for a picnic!" << endl;
    } else if (temperature < 0 || temperature > 40) {
        cout << "Extreme weather! Stay indoors." << endl;
    } else if (isRaining) {
        cout << "It's raining. Bring an umbrella!" << endl;
    } else {
        cout << "Weather is okay, but not ideal for picnic." << endl;
    }
    cout << endl;

    // Login system
    cout << "5. Login System:" << endl;
    string username, password;
    const string CORRECT_USER = "admin";
    const string CORRECT_PASS = "password123";

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (username == CORRECT_USER && password == CORRECT_PASS) {
        cout << "Login successful! Welcome, " << username << "!" << endl;
    } else if (username != CORRECT_USER && password != CORRECT_PASS) {
        cout << "Login failed! Both username and password are incorrect." << endl;
    } else if (username != CORRECT_USER) {
        cout << "Login failed! Invalid username." << endl;
    } else {
        cout << "Login failed! Invalid password." << endl;
    }
    cout << endl;

    // Range checking
    cout << "6. Range Checking:" << endl;
    int score;
    cout << "Enter test score (0-100): ";
    cin >> score;

    if (score < 0 || score > 100) {
        cout << "Invalid score! Must be between 0 and 100." << endl;
    } else if (score >= 90 && score <= 100) {
        cout << "Grade A" << endl;
    } else if (score >= 80 && score < 90) {
        cout << "Grade B" << endl;
    } else if (score >= 70 && score < 80) {
        cout << "Grade C" << endl;
    } else {
        cout << "Grade D or F" << endl;
    }
    cout << endl;

    // Truth table
    cout << "7. Truth Table for Logical Operators:" << endl;
    cout << "A\tB\tA&&B\tA||B\t!A\t!B" << endl;
    cout << "-------------------------------------" << endl;

    bool values[] = {false, true};
    for (bool a : values) {
        for (bool b : values) {
            cout << a << "\t" << b << "\t";
            cout << (a && b) << "\t";
            cout << (a || b) << "\t";
            cout << !a << "\t";
            cout << !b << endl;
        }
    }

    return 0;
}
