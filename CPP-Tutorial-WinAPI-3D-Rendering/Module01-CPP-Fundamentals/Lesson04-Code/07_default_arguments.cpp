/*
 * Program: Default Arguments Demo
 * Description: Demonstrates default function parameters
 * Compilation: g++ 07_default_arguments.cpp -o 07_default_arguments
 * Execution: ./07_default_arguments
 */

#include <iostream>
using namespace std;

// Function with default arguments
void printInfo(string name, int age = 18, string city = "Unknown") {
    cout << "Name: " << name << ", Age: " << age << ", City: " << city << endl;
}

// Calculate power with default exponent
double power(double base, int exponent = 2) {
    double result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Calculate area with default height (for square)
double rectangleArea(double length, double width = 0) {
    if (width == 0) width = length;  // Square if width not provided
    return length * width;
}

// Display array with default separator
void displayArray(int arr[], int size, string separator = ", ") {
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << separator;
    }
    cout << endl;
}

// Calculate discount with default rate
double applyDiscount(double price, double discountRate = 0.10) {
    return price * (1 - discountRate);
}

int main() {
    cout << "=== Default Arguments Demo ===" << endl << endl;

    // Using default arguments
    cout << "1. Print Info with Defaults:" << endl;
    printInfo("Alice");  // Uses default age and city
    printInfo("Bob", 25);  // Uses default city
    printInfo("Charlie", 30, "New York");  // All arguments provided
    cout << endl;

    // Power with default exponent
    cout << "2. Power Function:" << endl;
    cout << "5^2 (default): " << power(5) << endl;
    cout << "5^3: " << power(5, 3) << endl;
    cout << "2^5: " << power(2, 5) << endl;
    cout << endl;

    // Rectangle area (square by default)
    cout << "3. Rectangle/Square Area:" << endl;
    cout << "Square 5x5: " << rectangleArea(5) << endl;
    cout << "Rectangle 4x6: " << rectangleArea(4, 6) << endl;
    cout << endl;

    // Display array with different separators
    cout << "4. Display Array:" << endl;
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;

    cout << "Default separator: ";
    displayArray(numbers, size);

    cout << "Custom separator ' | ': ";
    displayArray(numbers, size, " | ");

    cout << "Custom separator ' - ': ";
    displayArray(numbers, size, " - ");
    cout << endl;

    // Discount calculation
    cout << "5. Discount Calculation:" << endl;
    double price = 100.0;
    cout << "Original price: $" << price << endl;
    cout << "With default discount (10%): $" << applyDiscount(price) << endl;
    cout << "With 25% discount: $" << applyDiscount(price, 0.25) << endl;
    cout << "With 50% discount: $" << applyDiscount(price, 0.50) << endl;

    return 0;
}
