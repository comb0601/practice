/*
 * Program: Function Pointers
 * Description: Pointers to functions and callbacks
 * Compilation: g++ -std=c++17 09_function_pointers.cpp -o 09_function_pointers
 * Execution: ./09_function_pointers
 */

#include <iostream>
using namespace std;

// Simple functions for demonstration
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b != 0) return a / b;
    return 0;
}

// Function that takes function pointer as parameter
int calculate(int x, int y, int (*operation)(int, int)) {
    return operation(x, y);
}

// Array processing with callback
void processArray(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);
    }
}

void printValue(int x) {
    cout << x << " ";
}

void printSquare(int x) {
    cout << (x * x) << " ";
}

// Comparison function type for sorting
bool ascending(int a, int b) {
    return a < b;
}

bool descending(int a, int b) {
    return a > b;
}

void bubbleSort(int arr[], int size, bool (*compare)(int, int)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compare(arr[j + 1], arr[j])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    cout << "=== Function Pointers ===" << endl << endl;

    // Basic function pointer
    cout << "1. Basic Function Pointer:" << endl;
    int (*funcPtr)(int, int) = add;  // Pointer to function

    int result = funcPtr(5, 3);
    cout << "funcPtr(5, 3) = " << result << endl;

    // Alternative syntax
    result = (*funcPtr)(10, 2);
    cout << "(*funcPtr)(10, 2) = " << result << endl << endl;

    // Changing function pointer
    cout << "2. Changing Function Pointer:" << endl;
    cout << "Using add: " << funcPtr(10, 5) << endl;

    funcPtr = subtract;
    cout << "Using subtract: " << funcPtr(10, 5) << endl;

    funcPtr = multiply;
    cout << "Using multiply: " << funcPtr(10, 5) << endl;

    funcPtr = divide;
    cout << "Using divide: " << funcPtr(10, 5) << endl << endl;

    // Passing function pointer to function
    cout << "3. Function Pointer as Parameter:" << endl;
    cout << "calculate(8, 4, add) = " << calculate(8, 4, add) << endl;
    cout << "calculate(8, 4, subtract) = " << calculate(8, 4, subtract) << endl;
    cout << "calculate(8, 4, multiply) = " << calculate(8, 4, multiply) << endl;
    cout << "calculate(8, 4, divide) = " << calculate(8, 4, divide) << endl << endl;

    // Array of function pointers
    cout << "4. Array of Function Pointers:" << endl;
    int (*operations[4])(int, int) = {add, subtract, multiply, divide};
    const char* names[] = {"add", "subtract", "multiply", "divide"};

    int a = 20, b = 5;
    for (int i = 0; i < 4; i++) {
        cout << names[i] << "(" << a << ", " << b << ") = "
             << operations[i](a, b) << endl;
    }
    cout << endl;

    // Callback functions
    cout << "5. Callback Functions:" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;

    cout << "Print values: ";
    processArray(arr, size, printValue);
    cout << endl;

    cout << "Print squares: ";
    processArray(arr, size, printSquare);
    cout << endl << endl;

    // Custom sorting with comparison function
    cout << "6. Custom Sorting:" << endl;
    int numbers[] = {5, 2, 8, 1, 9};
    int numSize = 5;

    cout << "Original: ";
    for (int i = 0; i < numSize; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    bubbleSort(numbers, numSize, ascending);
    cout << "Ascending: ";
    for (int i = 0; i < numSize; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    bubbleSort(numbers, numSize, descending);
    cout << "Descending: ";
    for (int i = 0; i < numSize; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    // Calculator using function pointers
    cout << "7. Simple Calculator:" << endl;
    char choice;
    int x, y;

    cout << "Enter two numbers: ";
    cin >> x >> y;

    cout << "Choose operation (+, -, *, /): ";
    cin >> choice;

    int (*selectedOp)(int, int) = nullptr;

    switch (choice) {
        case '+': selectedOp = add; break;
        case '-': selectedOp = subtract; break;
        case '*': selectedOp = multiply; break;
        case '/': selectedOp = divide; break;
        default:
            cout << "Invalid operation!" << endl;
            return 1;
    }

    if (selectedOp != nullptr) {
        cout << "Result: " << selectedOp(x, y) << endl;
    }

    return 0;
}
