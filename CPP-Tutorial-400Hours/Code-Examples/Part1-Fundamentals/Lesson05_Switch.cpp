/*
 * Lesson 5: Switch Statement - Multi-way Branching
 *
 * This program demonstrates:
 * - Basic switch statement syntax
 * - Switch with break statements
 * - Fall-through behavior
 * - Switch with multiple cases
 * - Practical applications of switch
 * - Comparison with if-else
 *
 * Compile: cl Lesson05_Switch.cpp
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== C++ Switch Statement Demo ===" << endl << endl;

    // BASIC SWITCH STATEMENT
    cout << "=== BASIC SWITCH STATEMENT ===" << endl;
    int day = 3;

    cout << "Day " << day << ": ";
    switch (day) {
        case 1:
            cout << "Monday" << endl;
            break;
        case 2:
            cout << "Tuesday" << endl;
            break;
        case 3:
            cout << "Wednesday" << endl;
            break;
        case 4:
            cout << "Thursday" << endl;
            break;
        case 5:
            cout << "Friday" << endl;
            break;
        case 6:
            cout << "Saturday" << endl;
            break;
        case 7:
            cout << "Sunday" << endl;
            break;
        default:
            cout << "Invalid day!" << endl;
    }
    cout << endl;

    // SWITCH WITH CHARACTER
    cout << "=== SWITCH WITH CHARACTER ===" << endl;
    char grade = 'B';

    cout << "Grade: " << grade << endl;
    switch (grade) {
        case 'A':
            cout << "Excellent! Score: 90-100" << endl;
            break;
        case 'B':
            cout << "Very Good! Score: 80-89" << endl;
            break;
        case 'C':
            cout << "Good! Score: 70-79" << endl;
            break;
        case 'D':
            cout << "Satisfactory. Score: 60-69" << endl;
            break;
        case 'F':
            cout << "Failed. Score: Below 60" << endl;
            break;
        default:
            cout << "Invalid grade!" << endl;
    }
    cout << endl;

    // MULTIPLE CASES WITH SAME ACTION
    cout << "=== MULTIPLE CASES ===" << endl;
    char vowel = 'e';

    cout << "Character: " << vowel << endl;
    switch (vowel) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            cout << vowel << " is a vowel" << endl;
            break;
        default:
            cout << vowel << " is a consonant" << endl;
    }
    cout << endl;

    // FALL-THROUGH BEHAVIOR (without break)
    cout << "=== FALL-THROUGH BEHAVIOR ===" << endl;
    int month = 2;

    cout << "Month " << month << " has ";
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            cout << "31 days" << endl;
            break;
        case 4: case 6: case 9: case 11:
            cout << "30 days" << endl;
            break;
        case 2:
            cout << "28 or 29 days" << endl;
            break;
        default:
            cout << "Invalid month!" << endl;
    }
    cout << endl;

    // MENU-DRIVEN PROGRAM
    cout << "=== MENU-DRIVEN PROGRAM ===" << endl;
    cout << "Calculator Menu:" << endl;
    cout << "1. Addition" << endl;
    cout << "2. Subtraction" << endl;
    cout << "3. Multiplication" << endl;
    cout << "4. Division" << endl;
    cout << "5. Exit" << endl;

    int choice = 1;
    double num1 = 10, num2 = 5;

    cout << "\nSelected option: " << choice << endl;
    cout << "Numbers: " << num1 << " and " << num2 << endl;
    cout << "Result: ";

    switch (choice) {
        case 1:
            cout << num1 << " + " << num2 << " = " << (num1 + num2) << endl;
            break;
        case 2:
            cout << num1 << " - " << num2 << " = " << (num1 - num2) << endl;
            break;
        case 3:
            cout << num1 << " * " << num2 << " = " << (num1 * num2) << endl;
            break;
        case 4:
            if (num2 != 0) {
                cout << num1 << " / " << num2 << " = " << (num1 / num2) << endl;
            } else {
                cout << "Error: Division by zero!" << endl;
            }
            break;
        case 5:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
    cout << endl;

    // NESTED SWITCH
    cout << "=== NESTED SWITCH ===" << endl;
    int category = 1;
    int subcategory = 2;

    cout << "Category: " << category << ", Subcategory: " << subcategory << endl;

    switch (category) {
        case 1:
            cout << "Electronics - ";
            switch (subcategory) {
                case 1:
                    cout << "Computers" << endl;
                    break;
                case 2:
                    cout << "Phones" << endl;
                    break;
                case 3:
                    cout << "Cameras" << endl;
                    break;
                default:
                    cout << "Unknown" << endl;
            }
            break;
        case 2:
            cout << "Clothing - ";
            switch (subcategory) {
                case 1:
                    cout << "Men's" << endl;
                    break;
                case 2:
                    cout << "Women's" << endl;
                    break;
                case 3:
                    cout << "Kids" << endl;
                    break;
                default:
                    cout << "Unknown" << endl;
            }
            break;
        default:
            cout << "Unknown category" << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE: TRAFFIC LIGHT
    cout << "=== TRAFFIC LIGHT SYSTEM ===" << endl;
    char light = 'G';

    cout << "Traffic Light: " << light << endl;
    cout << "Action: ";

    switch (light) {
        case 'R':
        case 'r':
            cout << "STOP - Red light" << endl;
            break;
        case 'Y':
        case 'y':
            cout << "CAUTION - Yellow light, prepare to stop" << endl;
            break;
        case 'G':
        case 'g':
            cout << "GO - Green light, proceed" << endl;
            break;
        default:
            cout << "Invalid light signal!" << endl;
    }
    cout << endl;

    // PRACTICAL EXAMPLE: SEASON
    cout << "=== SEASON IDENTIFIER ===" << endl;
    int seasonMonth = 7;

    cout << "Month: " << seasonMonth << endl;
    cout << "Season: ";

    switch (seasonMonth) {
        case 12:
        case 1:
        case 2:
            cout << "Winter" << endl;
            cout << "Temperature: Cold (0-10°C)" << endl;
            cout << "Activities: Skiing, Ice skating" << endl;
            break;
        case 3:
        case 4:
        case 5:
            cout << "Spring" << endl;
            cout << "Temperature: Mild (10-20°C)" << endl;
            cout << "Activities: Gardening, Hiking" << endl;
            break;
        case 6:
        case 7:
        case 8:
            cout << "Summer" << endl;
            cout << "Temperature: Hot (20-35°C)" << endl;
            cout << "Activities: Swimming, Beach" << endl;
            break;
        case 9:
        case 10:
        case 11:
            cout << "Fall/Autumn" << endl;
            cout << "Temperature: Cool (10-20°C)" << endl;
            cout << "Activities: Leaf watching, Harvest" << endl;
            break;
        default:
            cout << "Invalid month!" << endl;
    }
    cout << endl;

    // COMPARISON: SWITCH vs IF-ELSE
    cout << "=== SWITCH vs IF-ELSE ===" << endl;
    int value = 2;

    // Using switch
    cout << "Using switch (value = " << value << "): ";
    switch (value) {
        case 1: cout << "One"; break;
        case 2: cout << "Two"; break;
        case 3: cout << "Three"; break;
        default: cout << "Other";
    }
    cout << endl;

    // Using if-else
    cout << "Using if-else (value = " << value << "): ";
    if (value == 1) {
        cout << "One";
    } else if (value == 2) {
        cout << "Two";
    } else if (value == 3) {
        cout << "Three";
    } else {
        cout << "Other";
    }
    cout << endl << endl;

    cout << "Note: Switch is cleaner and more efficient for multiple equality checks!" << endl;

    return 0;
}

/*
 * Expected Output:
 * === C++ Switch Statement Demo ===
 *
 * === BASIC SWITCH STATEMENT ===
 * Day 3: Wednesday
 *
 * === SWITCH WITH CHARACTER ===
 * Grade: B
 * Very Good! Score: 80-89
 *
 * === MULTIPLE CASES ===
 * Character: e
 * e is a vowel
 *
 * === FALL-THROUGH BEHAVIOR ===
 * Month 2 has 28 or 29 days
 *
 * ... (continues with all examples)
 */
