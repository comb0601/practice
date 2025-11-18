/*
 * Program: Array Functions
 * Description: Functions for array manipulation and operations
 * Compilation: g++ 09_array_functions.cpp -o 09_array_functions
 * Execution: ./09_array_functions
 */

#include <iostream>
using namespace std;

// Print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Find maximum element
int findMax(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Find minimum element
int findMin(int arr[], int size) {
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Calculate sum
int calculateSum(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Calculate average
double calculateAverage(int arr[], int size) {
    return static_cast<double>(calculateSum(arr, size)) / size;
}

// Reverse array
void reverseArray(int arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

// Search element (linear search)
int searchElement(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;  // Not found
}

// Count occurrences
int countOccurrences(int arr[], int size, int value) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            count++;
        }
    }
    return count;
}

// Copy array
void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

// Check if sorted
bool isSorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "=== Array Functions ===" << endl << endl;

    int numbers[] = {5, 2, 8, 1, 9, 3, 7};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    // Print array
    cout << "1. Original Array: ";
    printArray(numbers, size);
    cout << endl;

    // Max and Min
    cout << "2. Maximum and Minimum:" << endl;
    cout << "Maximum: " << findMax(numbers, size) << endl;
    cout << "Minimum: " << findMin(numbers, size) << endl << endl;

    // Sum and Average
    cout << "3. Sum and Average:" << endl;
    cout << "Sum: " << calculateSum(numbers, size) << endl;
    cout << "Average: " << calculateAverage(numbers, size) << endl << endl;

    // Search
    cout << "4. Search Element:" << endl;
    int target = 8;
    int index = searchElement(numbers, size, target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }
    cout << endl;

    // Count occurrences
    cout << "5. Count Occurrences:" << endl;
    cout << "Occurrences of 5: " << countOccurrences(numbers, size, 5) << endl << endl;

    // Reverse array
    cout << "6. Reverse Array:" << endl;
    cout << "Before: ";
    printArray(numbers, size);
    reverseArray(numbers, size);
    cout << "After: ";
    printArray(numbers, size);
    cout << endl;

    // Copy array
    cout << "7. Copy Array:" << endl;
    int copy[7];
    copyArray(numbers, copy, size);
    cout << "Copied array: ";
    printArray(copy, size);
    cout << endl;

    // Check if sorted
    cout << "8. Check if Sorted:" << endl;
    cout << "Is sorted? " << (isSorted(numbers, size) ? "Yes" : "No") << endl;

    return 0;
}
