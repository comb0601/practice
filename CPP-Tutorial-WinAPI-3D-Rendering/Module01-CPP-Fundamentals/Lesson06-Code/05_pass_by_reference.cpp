/*
 * Program: Pass by Reference
 * Description: Passing arguments by reference to functions
 * Compilation: g++ -std=c++17 05_pass_by_reference.cpp -o 05_pass_by_reference
 * Execution: ./05_pass_by_reference
 */

#include <iostream>
using namespace std;

// Pass by value - makes a copy
void incrementByValue(int x) {
    x++;
    cout << "Inside incrementByValue: x = " << x << endl;
}

// Pass by reference - modifies original
void incrementByReference(int& x) {
    x++;
    cout << "Inside incrementByReference: x = " << x << endl;
}

// Pass by pointer - modifies original
void incrementByPointer(int* x) {
    (*x)++;
    cout << "Inside incrementByPointer: *x = " << *x << endl;
}

// Swap using references
void swapByReference(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Swap using pointers
void swapByPointer(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Return multiple values using references
void getDimensions(int& width, int& height) {
    width = 1920;
    height = 1080;
}

// Const reference - efficient for large objects
void printValue(const int& value) {
    cout << "Value: " << value << endl;
    // value++;  // ERROR - cannot modify const reference
}

// Calculate statistics using references
void calculateStats(int arr[], int size, int& min, int& max, double& avg) {
    min = arr[0];
    max = arr[0];
    int sum = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
        sum += arr[i];
    }

    avg = static_cast<double>(sum) / size;
}

int main() {
    cout << "=== Pass by Reference ===" << endl << endl;

    // Pass by value vs reference
    cout << "1. Pass by Value vs Reference:" << endl;
    int num1 = 10;
    cout << "Original num1 = " << num1 << endl;

    incrementByValue(num1);
    cout << "After incrementByValue: num1 = " << num1 << endl;

    incrementByReference(num1);
    cout << "After incrementByReference: num1 = " << num1 << endl << endl;

    // Pass by pointer
    cout << "2. Pass by Pointer:" << endl;
    int num2 = 20;
    cout << "Original num2 = " << num2 << endl;

    incrementByPointer(&num2);
    cout << "After incrementByPointer: num2 = " << num2 << endl << endl;

    // Swap using references
    cout << "3. Swap Using References:" << endl;
    int a = 5, b = 10;
    cout << "Before swap: a = " << a << ", b = " << b << endl;

    swapByReference(a, b);
    cout << "After swap: a = " << a << ", b = " << b << endl << endl;

    // Swap using pointers
    cout << "4. Swap Using Pointers:" << endl;
    int x = 15, y = 25;
    cout << "Before swap: x = " << x << ", y = " << y << endl;

    swapByPointer(&x, &y);
    cout << "After swap: x = " << x << ", y = " << y << endl << endl;

    // Return multiple values
    cout << "5. Return Multiple Values:" << endl;
    int width, height;
    getDimensions(width, height);
    cout << "Width: " << width << ", Height: " << height << endl << endl;

    // Const reference
    cout << "6. Const Reference (efficient for large objects):" << endl;
    int value = 100;
    printValue(value);
    cout << endl;

    // Calculate statistics
    cout << "7. Calculate Statistics:" << endl;
    int data[] = {5, 2, 8, 1, 9, 3, 7};
    int size = sizeof(data) / sizeof(data[0]);
    int minVal, maxVal;
    double avgVal;

    cout << "Data: ";
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }
    cout << endl;

    calculateStats(data, size, minVal, maxVal, avgVal);

    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;
    cout << "Average: " << avgVal << endl << endl;

    // Benefits of pass by reference
    cout << "8. Benefits of Pass by Reference:" << endl;
    cout << "- No copying (efficient for large objects)" << endl;
    cout << "- Can modify original variable" << endl;
    cout << "- Can return multiple values" << endl;
    cout << "- Cleaner syntax than pointers" << endl;
    cout << "- Use const& for read-only access" << endl;

    return 0;
}
