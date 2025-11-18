/*
 * Lesson 11: Functions - Code Reusability
 *
 * Demonstrates function declaration, definition, and calling
 *
 * Compile: cl Lesson11_Functions.cpp
 */

#include <iostream>
#include <cmath>
using namespace std;

// Function declarations (prototypes)
void greet();
void greetUser(string name);
int add(int a, int b);
double calculateCircleArea(double radius);
bool isEven(int num);
int findMax(int a, int b, int c);
void printArray(int arr[], int size);
double calculatePower(double base, int exp);

int main() {
    cout << "=== Functions Demo ===" << endl << endl;

    // VOID FUNCTION (NO RETURN VALUE)
    cout << "=== VOID FUNCTION ===" << endl;
    greet();
    greetUser("Alice");
    cout << endl;

    // FUNCTION WITH RETURN VALUE
    cout << "=== FUNCTION WITH RETURN ===" << endl;
    int sum = add(10, 20);
    cout << "Sum: " << sum << endl;

    double area = calculateCircleArea(5.0);
    cout << "Circle area (r=5): " << area << endl << endl;

    // BOOLEAN FUNCTION
    cout << "=== BOOLEAN FUNCTION ===" << endl;
    cout << "Is 10 even? " << (isEven(10) ? "Yes" : "No") << endl;
    cout << "Is 15 even? " << (isEven(15) ? "Yes" : "No") << endl << endl;

    // FUNCTION WITH MULTIPLE PARAMETERS
    cout << "=== MULTIPLE PARAMETERS ===" << endl;
    int maximum = findMax(25, 100, 50);
    cout << "Maximum of 25, 100, 50: " << maximum << endl << endl;

    // FUNCTION WITH ARRAY PARAMETER
    cout << "=== ARRAY PARAMETER ===" << endl;
    int numbers[] = {1, 2, 3, 4, 5};
    printArray(numbers, 5);
    cout << endl;

    // MATHEMATICAL FUNCTIONS
    cout << "=== MATHEMATICAL FUNCTIONS ===" << endl;
    cout << "2^8 = " << calculatePower(2, 8) << endl;
    cout << "sqrt(16) = " << sqrt(16) << endl;
    cout << "abs(-15) = " << abs(-15) << endl;

    return 0;
}

// Function definitions
void greet() {
    cout << "Hello, World!" << endl;
}

void greetUser(string name) {
    cout << "Hello, " << name << "!" << endl;
}

int add(int a, int b) {
    return a + b;
}

double calculateCircleArea(double radius) {
    const double PI = 3.14159;
    return PI * radius * radius;
}

bool isEven(int num) {
    return num % 2 == 0;
}

int findMax(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

void printArray(int arr[], int size) {
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

double calculatePower(double base, int exp) {
    double result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
