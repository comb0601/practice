/*
 * Program: Searching Functions
 * Description: Implementation of linear and binary search algorithms
 * Compilation: g++ 14_searching_functions.cpp -o 14_searching_functions
 * Execution: ./14_searching_functions
 */

#include <iostream>
using namespace std;

// Linear Search
int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Found at index i
        }
    }
    return -1;  // Not found
}

// Binary Search (iterative) - array must be sorted
int binarySearch(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;  // Found
        }
        if (arr[mid] < target) {
            left = mid + 1;  // Search right half
        } else {
            right = mid - 1;  // Search left half
        }
    }
    return -1;  // Not found
}

// Binary Search (recursive)
int binarySearchRecursive(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;  // Not found
    }

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        return mid;  // Found
    }
    if (arr[mid] < target) {
        return binarySearchRecursive(arr, mid + 1, right, target);
    }
    return binarySearchRecursive(arr, left, mid - 1, target);
}

// Find first occurrence
int findFirst(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Find last occurrence
int findLast(int arr[], int size, int target) {
    int lastIndex = -1;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            lastIndex = i;
        }
    }
    return lastIndex;
}

// Count occurrences
int countOccurrences(int arr[], int size, int target) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            count++;
        }
    }
    return count;
}

// Find maximum
int findMaximum(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Find minimum
int findMinimum(int arr[], int size) {
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "=== Searching Functions ===" << endl << endl;

    int unsorted[] = {64, 34, 25, 12, 22, 11, 90, 22};
    int unsortedSize = sizeof(unsorted) / sizeof(unsorted[0]);

    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 67, 78};
    int sortedSize = sizeof(sorted) / sizeof(sorted[0]);

    // Linear Search
    cout << "1. Linear Search (Unsorted Array):" << endl;
    cout << "Array: ";
    printArray(unsorted, unsortedSize);

    int target = 22;
    int index = linearSearch(unsorted, unsortedSize, target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }
    cout << endl;

    // Binary Search (Iterative)
    cout << "2. Binary Search - Iterative (Sorted Array):" << endl;
    cout << "Array: ";
    printArray(sorted, sortedSize);

    target = 23;
    index = binarySearch(sorted, sortedSize, target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }
    cout << endl;

    // Binary Search (Recursive)
    cout << "3. Binary Search - Recursive:" << endl;
    target = 56;
    index = binarySearchRecursive(sorted, 0, sortedSize - 1, target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }
    cout << endl;

    // Find first and last occurrence
    cout << "4. First and Last Occurrence:" << endl;
    cout << "Array: ";
    printArray(unsorted, unsortedSize);

    target = 22;
    cout << "First occurrence of " << target << ": " << findFirst(unsorted, unsortedSize, target) << endl;
    cout << "Last occurrence of " << target << ": " << findLast(unsorted, unsortedSize, target) << endl;
    cout << "Total occurrences: " << countOccurrences(unsorted, unsortedSize, target) << endl;
    cout << endl;

    // Find max and min
    cout << "5. Maximum and Minimum:" << endl;
    cout << "Array: ";
    printArray(unsorted, unsortedSize);
    cout << "Maximum: " << findMaximum(unsorted, unsortedSize) << endl;
    cout << "Minimum: " << findMinimum(unsorted, unsortedSize) << endl;
    cout << endl;

    // Performance comparison
    cout << "=== Algorithm Characteristics ===" << endl << endl;

    cout << "Linear Search:" << endl;
    cout << "  - Time Complexity: O(n)" << endl;
    cout << "  - Works on any array (sorted or unsorted)" << endl;
    cout << "  - Simple implementation" << endl << endl;

    cout << "Binary Search:" << endl;
    cout << "  - Time Complexity: O(log n)" << endl;
    cout << "  - Requires sorted array" << endl;
    cout << "  - Much faster for large arrays" << endl;
    cout << "  - Can be implemented iteratively or recursively" << endl;

    return 0;
}
