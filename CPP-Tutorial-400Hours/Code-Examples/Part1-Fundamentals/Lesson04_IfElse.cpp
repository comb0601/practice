/*
 * Lesson 4: If-Else Statements - Conditional Logic
 *
 * This program demonstrates:
 * - Simple if statement
 * - if-else statement
 * - if-else if-else ladder
 * - Nested if statements
 * - Practical examples
 *
 * Compile: cl Lesson04_IfElse.cpp
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== C++ If-Else Statements Demo ===" << endl << endl;

    // SIMPLE IF STATEMENT
    cout << "=== SIMPLE IF STATEMENT ===" << endl;
    int number = 10;

    if (number > 0) {
        cout << number << " is positive" << endl;
    }

    if (number % 2 == 0) {
        cout << number << " is even" << endl;
    }

    if (number >= 10) {
        cout << number << " is greater than or equal to 10" << endl;
    }
    cout << endl;

    // IF-ELSE STATEMENT
    cout << "=== IF-ELSE STATEMENT ===" << endl;
    int age = 20;

    if (age >= 18) {
        cout << "Age " << age << ": You are an adult" << endl;
    } else {
        cout << "Age " << age << ": You are a minor" << endl;
    }

    int score = 75;
    if (score >= 60) {
        cout << "Score " << score << ": Passed!" << endl;
    } else {
        cout << "Score " << score << ": Failed!" << endl;
    }
    cout << endl;

    // IF-ELSE IF-ELSE LADDER
    cout << "=== IF-ELSE IF-ELSE LADDER ===" << endl;
    int marks = 85;

    cout << "Marks: " << marks << endl;
    if (marks >= 90) {
        cout << "Grade: A (Excellent!)" << endl;
    } else if (marks >= 80) {
        cout << "Grade: B (Very Good)" << endl;
    } else if (marks >= 70) {
        cout << "Grade: C (Good)" << endl;
    } else if (marks >= 60) {
        cout << "Grade: D (Satisfactory)" << endl;
    } else {
        cout << "Grade: F (Fail)" << endl;
    }
    cout << endl;

    // NESTED IF STATEMENTS
    cout << "=== NESTED IF STATEMENTS ===" << endl;
    int num = 24;

    cout << "Number: " << num << endl;
    if (num > 0) {
        cout << "The number is positive" << endl;

        if (num % 2 == 0) {
            cout << "The number is even" << endl;

            if (num % 4 == 0) {
                cout << "The number is divisible by 4" << endl;
            }
        } else {
            cout << "The number is odd" << endl;
        }
    } else if (num < 0) {
        cout << "The number is negative" << endl;
    } else {
        cout << "The number is zero" << endl;
    }
    cout << endl;

    // MULTIPLE CONDITIONS (LOGICAL OPERATORS)
    cout << "=== MULTIPLE CONDITIONS ===" << endl;
    int temperature = 25;
    bool isSunny = true;

    cout << "Temperature: " << temperature << "°C, Sunny: " << (isSunny ? "Yes" : "No") << endl;

    // Using AND (&&)
    if (temperature > 20 && temperature < 30 && isSunny) {
        cout << "Perfect weather for outdoor activities!" << endl;
    }

    // Using OR (||)
    if (temperature < 0 || temperature > 40) {
        cout << "Extreme temperature!" << endl;
    } else {
        cout << "Temperature is in normal range" << endl;
    }

    // Using NOT (!)
    if (!isSunny) {
        cout << "Take an umbrella!" << endl;
    } else {
        cout << "No need for an umbrella" << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE 1: AGE CATEGORY
    cout << "=== EXAMPLE 1: AGE CATEGORY ===" << endl;
    int personAge = 35;

    cout << "Age: " << personAge << endl;
    if (personAge < 0) {
        cout << "Invalid age!" << endl;
    } else if (personAge <= 12) {
        cout << "Category: Child" << endl;
    } else if (personAge <= 19) {
        cout << "Category: Teenager" << endl;
    } else if (personAge <= 59) {
        cout << "Category: Adult" << endl;
    } else {
        cout << "Category: Senior" << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE 2: LOGIN VALIDATION
    cout << "=== EXAMPLE 2: LOGIN VALIDATION ===" << endl;
    string username = "admin";
    string password = "pass123";

    cout << "Username: " << username << endl;
    cout << "Password: " << string(password.length(), '*') << endl;

    if (username == "admin" && password == "pass123") {
        cout << "Login successful! Welcome, " << username << endl;
    } else {
        cout << "Login failed! Invalid credentials." << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE 3: LEAP YEAR CHECK
    cout << "=== EXAMPLE 3: LEAP YEAR CHECK ===" << endl;
    int year = 2024;

    cout << "Year: " << year << endl;
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        cout << year << " is a leap year" << endl;
    } else {
        cout << year << " is not a leap year" << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE 4: TICKET PRICING
    cout << "=== EXAMPLE 4: TICKET PRICING ===" << endl;
    int customerAge = 25;
    bool isStudent = false;
    bool isWeekend = true;

    double basePrice = 10.0;
    double finalPrice = basePrice;

    cout << "Customer Age: " << customerAge << endl;
    cout << "Student: " << (isStudent ? "Yes" : "No") << endl;
    cout << "Weekend: " << (isWeekend ? "Yes" : "No") << endl;
    cout << "Base Price: $" << basePrice << endl;

    if (customerAge < 5) {
        finalPrice = 0;  // Free for children under 5
        cout << "Discount: Free (child under 5)" << endl;
    } else if (customerAge <= 12) {
        finalPrice = basePrice * 0.5;  // 50% off for children
        cout << "Discount: 50% (child)" << endl;
    } else if (customerAge >= 65) {
        finalPrice = basePrice * 0.7;  // 30% off for seniors
        cout << "Discount: 30% (senior)" << endl;
    } else if (isStudent) {
        finalPrice = basePrice * 0.8;  // 20% off for students
        cout << "Discount: 20% (student)" << endl;
    }

    if (isWeekend) {
        finalPrice *= 1.2;  // 20% surcharge on weekends
        cout << "Weekend surcharge: +20%" << endl;
    }

    cout << "Final Price: $" << finalPrice << endl;
    cout << endl;

    // PRACTICAL EXAMPLE 5: NUMBER CLASSIFIER
    cout << "=== EXAMPLE 5: NUMBER CLASSIFIER ===" << endl;
    int testNum = 42;

    cout << "Number: " << testNum << endl;
    cout << "Properties:" << endl;

    if (testNum == 0) {
        cout << "  - Zero" << endl;
    } else {
        if (testNum > 0) {
            cout << "  - Positive" << endl;
        } else {
            cout << "  - Negative" << endl;
        }

        if (testNum % 2 == 0) {
            cout << "  - Even" << endl;
        } else {
            cout << "  - Odd" << endl;
        }

        if (testNum % 3 == 0) {
            cout << "  - Divisible by 3" << endl;
        }

        if (testNum % 5 == 0) {
            cout << "  - Divisible by 5" << endl;
        }

        if (testNum >= 10 && testNum <= 99) {
            cout << "  - Two-digit number" << endl;
        }
    }

    return 0;
}

/*
 * Expected Output:
 * === C++ If-Else Statements Demo ===
 *
 * === SIMPLE IF STATEMENT ===
 * 10 is positive
 * 10 is even
 * 10 is greater than or equal to 10
 *
 * === IF-ELSE STATEMENT ===
 * Age 20: You are an adult
 * Score 75: Passed!
 *
 * === IF-ELSE IF-ELSE LADDER ===
 * Marks: 85
 * Grade: B (Very Good)
 *
 * ... (continues with all examples)
 */
