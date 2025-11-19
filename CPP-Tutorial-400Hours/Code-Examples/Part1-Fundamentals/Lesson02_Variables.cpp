/*
 * Lesson 2: Variables - Storing and Using Data
 *
 * Variables are containers for storing data values. This program demonstrates:
 * - Variable declaration
 * - Variable initialization
 * - Variable assignment
 * - Using variables in expressions
 * - Displaying variable values
 *
 * Compile: cl Lesson02_Variables.cpp
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== C++ Variables Demo ===" << endl << endl;

    // 1. Variable Declaration (declaring without initializing)
    int age;
    age = 25;  // Assignment after declaration
    cout << "Age: " << age << endl;

    // 2. Variable Declaration with Initialization
    int score = 100;
    cout << "Score: " << score << endl;

    // 3. Multiple variables of same type
    int x = 10, y = 20, z = 30;
    cout << "x = " << x << ", y = " << y << ", z = " << z << endl;

    // 4. Different data types
    double price = 19.99;
    char grade = 'A';
    bool isPassed = true;
    string name = "John Doe";

    cout << endl << "=== Different Data Types ===" << endl;
    cout << "Name: " << name << endl;
    cout << "Price: $" << price << endl;
    cout << "Grade: " << grade << endl;
    cout << "Passed: " << (isPassed ? "Yes" : "No") << endl;

    // 5. Variable modification
    cout << endl << "=== Modifying Variables ===" << endl;
    int count = 5;
    cout << "Initial count: " << count << endl;

    count = 10;
    cout << "After assignment: " << count << endl;

    count = count + 5;
    cout << "After addition: " << count << endl;

    // 6. Using variables in calculations
    cout << endl << "=== Calculations with Variables ===" << endl;
    int length = 10;
    int width = 5;
    int area = length * width;
    int perimeter = 2 * (length + width);

    cout << "Rectangle Length: " << length << endl;
    cout << "Rectangle Width: " << width << endl;
    cout << "Area: " << area << endl;
    cout << "Perimeter: " << perimeter << endl;

    // 7. Variable naming rules
    cout << endl << "=== Variable Naming ===" << endl;
    int studentAge = 20;           // camelCase (common in C++)
    int student_grade = 85;        // snake_case
    int MAX_SCORE = 100;           // UPPER_CASE for constants

    cout << "studentAge: " << studentAge << endl;
    cout << "student_grade: " << student_grade << endl;
    cout << "MAX_SCORE: " << MAX_SCORE << endl;

    return 0;
}

/*
 * Expected Output:
 * === C++ Variables Demo ===
 *
 * Age: 25
 * Score: 100
 * x = 10, y = 20, z = 30
 *
 * === Different Data Types ===
 * Name: John Doe
 * Price: $19.99
 * Grade: A
 * Passed: Yes
 *
 * === Modifying Variables ===
 * Initial count: 5
 * After assignment: 10
 * After addition: 15
 *
 * === Calculations with Variables ===
 * Rectangle Length: 10
 * Rectangle Width: 5
 * Area: 50
 * Perimeter: 30
 *
 * === Variable Naming ===
 * studentAge: 20
 * student_grade: 85
 * MAX_SCORE: 100
 */
