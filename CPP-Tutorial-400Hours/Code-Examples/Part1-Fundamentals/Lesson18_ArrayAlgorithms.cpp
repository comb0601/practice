/*
 * Lesson 18: Array Algorithms - Sorting, Searching, etc.
 * Compile: cl Lesson18_ArrayAlgorithms.cpp
 */

#include <iostream>
#include <algorithm>
using namespace std;

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int binarySearch(int arr[], int size, int target) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    cout << "=== Array Algorithms Demo ===" << endl << endl;

    // BUBBLE SORT
    cout << "=== BUBBLE SORT ===" << endl;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int size1 = 7;
    cout << "Before: "; printArray(arr1, size1);
    bubbleSort(arr1, size1);
    cout << "After: "; printArray(arr1, size1);
    cout << endl;

    // SELECTION SORT
    cout << "=== SELECTION SORT ===" << endl;
    int arr2[] = {64, 25, 12, 22, 11};
    int size2 = 5;
    cout << "Before: "; printArray(arr2, size2);
    selectionSort(arr2, size2);
    cout << "After: "; printArray(arr2, size2);
    cout << endl;

    // LINEAR SEARCH
    cout << "=== LINEAR SEARCH ===" << endl;
    int arr3[] = {10, 20, 30, 40, 50};
    int target = 30;
    int result = linearSearch(arr3, 5, target);
    cout << "Searching for " << target << ": ";
    cout << (result != -1 ? "Found at index " + to_string(result) : "Not found") << endl;
    cout << endl;

    // BINARY SEARCH
    cout << "=== BINARY SEARCH ===" << endl;
    int arr4[] = {10, 20, 30, 40, 50, 60, 70};
    result = binarySearch(arr4, 7, 40);
    cout << "Searching for 40: ";
    cout << (result != -1 ? "Found at index " + to_string(result) : "Not found") << endl;
    cout << endl;

    // REVERSE ARRAY
    cout << "=== REVERSE ARRAY ===" << endl;
    int arr5[] = {1, 2, 3, 4, 5};
    cout << "Before: "; printArray(arr5, 5);
    reverse(arr5, arr5 + 5);
    cout << "After: "; printArray(arr5, 5);
    cout << endl;

    // MIN AND MAX
    cout << "=== MIN AND MAX ===" << endl;
    int arr6[] = {15, 8, 23, 4, 42, 16};
    int minVal = *min_element(arr6, arr6 + 6);
    int maxVal = *max_element(arr6, arr6 + 6);
    cout << "Array: "; printArray(arr6, 6);
    cout << "Min: " << minVal << ", Max: " << maxVal << endl;

    return 0;
}
