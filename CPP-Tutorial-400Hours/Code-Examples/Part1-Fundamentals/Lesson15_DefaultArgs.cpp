/*
 * Lesson 15: Default Arguments - Optional Parameters
 *
 * Demonstrates functions with default parameter values
 *
 * Compile: cl Lesson15_DefaultArgs.cpp
 */

#include <iostream>
#include <string>
using namespace std;

// Function with default arguments
void greet(string name = "User", string greeting = "Hello") {
    cout << greeting << ", " << name << "!" << endl;
}

// Rectangle area with default height
double rectangleArea(double width, double height = 1.0) {
    return width * height;
}

// Power function with default exponent
double power(double base, int exp = 2) {
    double result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Display info with defaults
void displayInfo(string title, int value = 0, bool verbose = false) {
    cout << title << ": " << value;
    if (verbose) {
        cout << " (detailed mode)";
    }
    cout << endl;
}

// Print array with default separator
void printArray(int arr[], int size, string sep = ", ") {
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << sep;
    }
    cout << endl;
}

int main() {
    cout << "=== Default Arguments Demo ===" << endl << endl;

    // GREET FUNCTION
    cout << "=== GREET WITH DEFAULTS ===" << endl;
    greet();                          // Uses both defaults
    greet("Alice");                   // Uses default greeting
    greet("Bob", "Hi");              // No defaults used
    cout << endl;

    // RECTANGLE AREA
    cout << "=== RECTANGLE AREA ===" << endl;
    cout << "Area(width=5): " << rectangleArea(5) << endl;
    cout << "Area(5x3): " << rectangleArea(5, 3) << endl << endl;

    // POWER FUNCTION
    cout << "=== POWER FUNCTION ===" << endl;
    cout << "power(5) = " << power(5) << " (5^2)" << endl;
    cout << "power(5, 3) = " << power(5, 3) << " (5^3)" << endl << endl;

    // DISPLAY INFO
    cout << "=== DISPLAY INFO ===" << endl;
    displayInfo("Score");
    displayInfo("Level", 10);
    displayInfo("Experience", 1500, true);
    cout << endl;

    // PRINT ARRAY
    cout << "=== PRINT ARRAY ===" << endl;
    int numbers[] = {1, 2, 3, 4, 5};
    cout << "Default separator: ";
    printArray(numbers, 5);
    cout << "Custom separator: ";
    printArray(numbers, 5, " | ");

    return 0;
}
