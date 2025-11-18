/*
 * Program: Sorting Functions
 * Description: Implementation of common sorting algorithms
 * Compilation: g++ 13_sorting_functions.cpp -o 13_sorting_functions
 * Execution: ./13_sorting_functions
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

// Bubble Sort
void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Selection Sort
void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

// Insertion Sort
void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Copy array for testing
void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

int main() {
    cout << "=== Sorting Functions ===" << endl << endl;

    int original[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(original) / sizeof(original[0]);

    cout << "Original array: ";
    printArray(original, size);
    cout << endl;

    // Bubble Sort
    cout << "1. Bubble Sort:" << endl;
    int arr1[size];
    copyArray(original, arr1, size);
    cout << "Before: ";
    printArray(arr1, size);
    bubbleSort(arr1, size);
    cout << "After:  ";
    printArray(arr1, size);
    cout << endl;

    // Selection Sort
    cout << "2. Selection Sort:" << endl;
    int arr2[size];
    copyArray(original, arr2, size);
    cout << "Before: ";
    printArray(arr2, size);
    selectionSort(arr2, size);
    cout << "After:  ";
    printArray(arr2, size);
    cout << endl;

    // Insertion Sort
    cout << "3. Insertion Sort:" << endl;
    int arr3[size];
    copyArray(original, arr3, size);
    cout << "Before: ";
    printArray(arr3, size);
    insertionSort(arr3, size);
    cout << "After:  ";
    printArray(arr3, size);
    cout << endl;

    // Algorithm characteristics
    cout << "=== Algorithm Characteristics ===" << endl << endl;

    cout << "Bubble Sort:" << endl;
    cout << "  - Time Complexity: O(n²)" << endl;
    cout << "  - Space Complexity: O(1)" << endl;
    cout << "  - Stable: Yes" << endl << endl;

    cout << "Selection Sort:" << endl;
    cout << "  - Time Complexity: O(n²)" << endl;
    cout << "  - Space Complexity: O(1)" << endl;
    cout << "  - Stable: No" << endl << endl;

    cout << "Insertion Sort:" << endl;
    cout << "  - Time Complexity: O(n²)" << endl;
    cout << "  - Space Complexity: O(1)" << endl;
    cout << "  - Stable: Yes" << endl;
    cout << "  - Good for small or nearly sorted arrays" << endl;

    return 0;
}
