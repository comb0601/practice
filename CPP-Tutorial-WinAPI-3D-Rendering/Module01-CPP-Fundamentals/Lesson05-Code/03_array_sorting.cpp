/*
 * Program: Array Sorting
 * Description: Implementing bubble sort and selection sort algorithms
 * Compilation: g++ 03_array_sorting.cpp -o 03_array_sorting
 * Execution: ./03_array_sorting
 */

#include <iostream>
using namespace std;

void printArray(int arr[], int size, const char* message) {
    cout << message;
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

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

void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap arr[i] and arr[minIndex]
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

int main() {
    cout << "=== Array Sorting Algorithms ===" << endl << endl;

    // Bubble Sort
    cout << "1. Bubble Sort:" << endl;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    printArray(arr1, size1, "Original array: ");
    bubbleSort(arr1, size1);
    printArray(arr1, size1, "Sorted array:   ");
    cout << endl;

    // Selection Sort
    cout << "2. Selection Sort:" << endl;
    int arr2[] = {64, 25, 12, 22, 11};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    printArray(arr2, size2, "Original array: ");
    selectionSort(arr2, size2);
    printArray(arr2, size2, "Sorted array:   ");
    cout << endl;

    // Descending order (modified bubble sort)
    cout << "3. Bubble Sort (Descending):" << endl;
    int arr3[] = {5, 2, 8, 1, 9, 3};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);

    printArray(arr3, size3, "Original array: ");

    // Sort in descending order
    for (int i = 0; i < size3 - 1; i++) {
        for (int j = 0; j < size3 - i - 1; j++) {
            if (arr3[j] < arr3[j + 1]) {  // Changed comparison
                int temp = arr3[j];
                arr3[j] = arr3[j + 1];
                arr3[j + 1] = temp;
            }
        }
    }

    printArray(arr3, size3, "Sorted (desc):  ");
    cout << endl;

    // Interactive sorting
    cout << "4. Interactive Sorting:" << endl;
    int userArray[5];
    cout << "Enter 5 numbers to sort: ";
    for (int i = 0; i < 5; i++) {
        cin >> userArray[i];
    }

    printArray(userArray, 5, "Before sorting: ");
    bubbleSort(userArray, 5);
    printArray(userArray, 5, "After sorting:  ");

    return 0;
}
