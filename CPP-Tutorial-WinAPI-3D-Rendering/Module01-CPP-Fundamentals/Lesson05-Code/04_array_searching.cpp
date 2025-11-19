/*
 * Program: Array Searching
 * Description: Linear search and binary search algorithms
 * Compilation: g++ 04_array_searching.cpp -o 04_array_searching
 * Execution: ./04_array_searching
 */

#include <iostream>
using namespace std;

int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Return index where found
        }
    }
    return -1;  // Not found
}

int binarySearch(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;  // Found
        }
        else if (arr[mid] < target) {
            left = mid + 1;  // Search right half
        }
        else {
            right = mid - 1;  // Search left half
        }
    }

    return -1;  // Not found
}

int main() {
    cout << "=== Array Searching Algorithms ===" << endl << endl;

    // Linear Search
    cout << "1. Linear Search:" << endl;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    cout << "Array: ";
    for (int i = 0; i < size1; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;

    int target1 = 22;
    int result1 = linearSearch(arr1, size1, target1);

    if (result1 != -1) {
        cout << "Found " << target1 << " at index " << result1 << endl;
    } else {
        cout << target1 << " not found" << endl;
    }

    target1 = 100;
    result1 = linearSearch(arr1, size1, target1);
    if (result1 != -1) {
        cout << "Found " << target1 << " at index " << result1 << endl;
    } else {
        cout << target1 << " not found" << endl;
    }
    cout << endl;

    // Binary Search (requires sorted array)
    cout << "2. Binary Search:" << endl;
    int arr2[] = {11, 12, 22, 25, 34, 64, 90};  // Sorted array
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    cout << "Sorted array: ";
    for (int i = 0; i < size2; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    int target2 = 25;
    int result2 = binarySearch(arr2, size2, target2);

    if (result2 != -1) {
        cout << "Found " << target2 << " at index " << result2 << endl;
    } else {
        cout << target2 << " not found" << endl;
    }

    target2 = 50;
    result2 = binarySearch(arr2, size2, target2);
    if (result2 != -1) {
        cout << "Found " << target2 << " at index " << result2 << endl;
    } else {
        cout << target2 << " not found" << endl;
    }
    cout << endl;

    // Find all occurrences
    cout << "3. Find All Occurrences:" << endl;
    int arr3[] = {5, 2, 8, 2, 9, 2, 3};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);

    cout << "Array: ";
    for (int i = 0; i < size3; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;

    int target3 = 2;
    cout << "Indices where " << target3 << " appears: ";
    bool found = false;
    for (int i = 0; i < size3; i++) {
        if (arr3[i] == target3) {
            cout << i << " ";
            found = true;
        }
    }
    if (!found) {
        cout << "Not found";
    }
    cout << endl << endl;

    // Interactive search
    cout << "4. Interactive Search:" << endl;
    int searchValue;
    cout << "Enter a value to search for in {11, 12, 22, 25, 34, 64, 90}: ";
    cin >> searchValue;

    int result = binarySearch(arr2, size2, searchValue);
    if (result != -1) {
        cout << "Found " << searchValue << " at index " << result << endl;
    } else {
        cout << searchValue << " not found in array" << endl;
    }

    return 0;
}
