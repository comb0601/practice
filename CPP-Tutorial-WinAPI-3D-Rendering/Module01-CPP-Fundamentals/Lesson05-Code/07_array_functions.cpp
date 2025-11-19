/*
 * Program: Arrays and Functions
 * Description: Passing arrays to functions, returning arrays (via pointers)
 * Compilation: g++ 07_array_functions.cpp -o 07_array_functions
 * Execution: ./07_array_functions
 */

#include <iostream>
using namespace std;

// Function to print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function to find sum of array elements
int arraySum(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Function to find average
double arrayAverage(int arr[], int size) {
    return static_cast<double>(arraySum(arr, size)) / size;
}

// Function to find maximum element
int findMax(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Function to find minimum element
int findMin(int arr[], int size) {
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Function to reverse array in place
void reverseArray(int arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

// Function to fill array with a value
void fillArray(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

// Function to copy array
void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

// Function to multiply all elements by a factor
void scaleArray(int arr[], int size, int factor) {
    for (int i = 0; i < size; i++) {
        arr[i] *= factor;
    }
}

// Function to count occurrences of a value
int countOccurrences(int arr[], int size, int value) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            count++;
        }
    }
    return count;
}

int main() {
    cout << "=== Arrays and Functions ===" << endl << endl;

    int numbers[] = {5, 2, 8, 1, 9, 3, 7};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    // Print array
    cout << "1. Original Array:" << endl;
    printArray(numbers, size);
    cout << endl;

    // Sum and average
    cout << "2. Sum and Average:" << endl;
    cout << "Sum: " << arraySum(numbers, size) << endl;
    cout << "Average: " << arrayAverage(numbers, size) << endl << endl;

    // Max and min
    cout << "3. Maximum and Minimum:" << endl;
    cout << "Maximum: " << findMax(numbers, size) << endl;
    cout << "Minimum: " << findMin(numbers, size) << endl << endl;

    // Reverse array
    cout << "4. Reverse Array:" << endl;
    cout << "Before: ";
    printArray(numbers, size);
    reverseArray(numbers, size);
    cout << "After:  ";
    printArray(numbers, size);
    reverseArray(numbers, size);  // Reverse back
    cout << endl;

    // Fill array
    cout << "5. Fill Array:" << endl;
    int temp[5];
    fillArray(temp, 5, 7);
    cout << "Array filled with 7: ";
    printArray(temp, 5);
    cout << endl;

    // Copy array
    cout << "6. Copy Array:" << endl;
    int copy[7];
    copyArray(numbers, copy, size);
    cout << "Original: ";
    printArray(numbers, size);
    cout << "Copy:     ";
    printArray(copy, size);
    cout << endl;

    // Scale array
    cout << "7. Scale Array:" << endl;
    int scale[] = {1, 2, 3, 4, 5};
    int scaleSize = 5;
    cout << "Before scaling by 3: ";
    printArray(scale, scaleSize);
    scaleArray(scale, scaleSize, 3);
    cout << "After scaling by 3:  ";
    printArray(scale, scaleSize);
    cout << endl;

    // Count occurrences
    cout << "8. Count Occurrences:" << endl;
    int data[] = {5, 2, 8, 2, 9, 2, 7};
    int dataSize = 7;
    cout << "Array: ";
    printArray(data, dataSize);
    cout << "Value 2 appears " << countOccurrences(data, dataSize, 2) << " times" << endl;

    return 0;
}
