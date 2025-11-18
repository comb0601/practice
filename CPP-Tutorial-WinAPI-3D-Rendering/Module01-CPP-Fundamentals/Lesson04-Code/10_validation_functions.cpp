/*
 * Program: Validation Functions
 * Description: Input validation and checking functions
 * Compilation: g++ 10_validation_functions.cpp -o 10_validation_functions
 * Execution: ./10_validation_functions
 */

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Check if number is positive
bool isPositive(int n) {
    return n > 0;
}

// Check if number is in range
bool isInRange(int n, int min, int max) {
    return (n >= min && n <= max);
}

// Check if string is valid email (simple check)
bool isValidEmail(string email) {
    int atPos = email.find('@');
    int dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos && dotPos > atPos + 1);
}

// Check if string contains only digits
bool isNumeric(string str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return !str.empty();
}

// Check if string contains only letters
bool isAlphabetic(string str) {
    for (char c : str) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return !str.empty();
}

// Check if password is strong
bool isStrongPassword(string password) {
    if (password.length() < 8) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasLower && hasDigit;
}

// Validate age
bool isValidAge(int age) {
    return (age >= 0 && age <= 150);
}

// Validate phone number (simple: 10 digits)
bool isValidPhone(string phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main() {
    cout << "=== Validation Functions ===" << endl << endl;

    // Positive number check
    cout << "1. Positive Number Check:" << endl;
    cout << "Is 5 positive? " << (isPositive(5) ? "Yes" : "No") << endl;
    cout << "Is -3 positive? " << (isPositive(-3) ? "Yes" : "No") << endl << endl;

    // Range check
    cout << "2. Range Check (1-100):" << endl;
    cout << "Is 50 in range? " << (isInRange(50, 1, 100) ? "Yes" : "No") << endl;
    cout << "Is 150 in range? " << (isInRange(150, 1, 100) ? "Yes" : "No") << endl << endl;

    // Email validation
    cout << "3. Email Validation:" << endl;
    cout << "user@example.com valid? " << (isValidEmail("user@example.com") ? "Yes" : "No") << endl;
    cout << "invalid.email valid? " << (isValidEmail("invalid.email") ? "Yes" : "No") << endl << endl;

    // Numeric check
    cout << "4. Numeric String Check:" << endl;
    cout << "\"12345\" numeric? " << (isNumeric("12345") ? "Yes" : "No") << endl;
    cout << "\"123abc\" numeric? " << (isNumeric("123abc") ? "Yes" : "No") << endl << endl;

    // Alphabetic check
    cout << "5. Alphabetic String Check:" << endl;
    cout << "\"Hello\" alphabetic? " << (isAlphabetic("Hello") ? "Yes" : "No") << endl;
    cout << "\"Hello123\" alphabetic? " << (isAlphabetic("Hello123") ? "Yes" : "No") << endl << endl;

    // Strong password check
    cout << "6. Strong Password Check:" << endl;
    cout << "\"Pass123word\" strong? " << (isStrongPassword("Pass123word") ? "Yes" : "No") << endl;
    cout << "\"weak\" strong? " << (isStrongPassword("weak") ? "Yes" : "No") << endl << endl;

    // Age validation
    cout << "7. Age Validation:" << endl;
    cout << "Age 25 valid? " << (isValidAge(25) ? "Yes" : "No") << endl;
    cout << "Age 200 valid? " << (isValidAge(200) ? "Yes" : "No") << endl << endl;

    // Phone validation
    cout << "8. Phone Number Validation:" << endl;
    cout << "\"1234567890\" valid? " << (isValidPhone("1234567890") ? "Yes" : "No") << endl;
    cout << "\"12345\" valid? " << (isValidPhone("12345") ? "Yes" : "No") << endl;

    return 0;
}
