/**
 * Template Binary Search
 * Generic binary search and related algorithms
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

// Binary search on sorted array
template <typename T>
int binarySearch(const T* arr, size_t size, const T& target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;  // Not found
}

// Recursive binary search
template <typename T>
int binarySearchRecursive(const T* arr, int left, int right, const T& target) {
    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        return mid;
    } else if (arr[mid] < target) {
        return binarySearchRecursive(arr, mid + 1, right, target);
    } else {
        return binarySearchRecursive(arr, left, mid - 1, target);
    }
}

// Binary search on vector
template <typename T>
int binarySearchVector(const vector<T>& vec, const T& target) {
    int left = 0;
    int right = vec.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (vec[mid] == target) {
            return mid;
        } else if (vec[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

// Find first occurrence
template <typename T>
int findFirst(const T* arr, size_t size, const T& target) {
    int left = 0;
    int right = size - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = mid;
            right = mid - 1;  // Continue searching left
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

// Find last occurrence
template <typename T>
int findLast(const T* arr, size_t size, const T& target) {
    int left = 0;
    int right = size - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = mid;
            left = mid + 1;  // Continue searching right
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

// Count occurrences
template <typename T>
int countOccurrences(const T* arr, size_t size, const T& target) {
    int first = findFirst(arr, size, target);
    if (first == -1) {
        return 0;
    }
    int last = findLast(arr, size, target);
    return last - first + 1;
}

// Find insertion position
template <typename T>
int findInsertPosition(const T* arr, size_t size, const T& target) {
    int left = 0;
    int right = size - 1;
    int result = size;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] >= target) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return result;
}

// Find closest value
template <typename T>
int findClosest(const T* arr, size_t size, const T& target) {
    if (size == 0) return -1;

    int left = 0;
    int right = size - 1;

    if (target <= arr[left]) return left;
    if (target >= arr[right]) return right;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // Compare neighbors
    if (left > 0 && abs(arr[left - 1] - target) < abs(arr[left] - target)) {
        return left - 1;
    }
    return left;
}

// Print array
template <typename T>
void printArray(const T* arr, size_t size) {
    cout << "[";
    for (size_t i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    cout << "=== Template Binary Search ===" << endl << endl;

    // Basic binary search
    cout << "Basic Binary Search:" << endl;
    int arr1[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 67, 78};
    size_t size1 = sizeof(arr1) / sizeof(arr1[0]);
    printArray(arr1, size1);

    int target = 23;
    int index = binarySearch(arr1, size1, target);
    cout << "Searching for " << target << ": "
         << (index != -1 ? "Found at index " + to_string(index) : "Not found") << endl;

    target = 50;
    index = binarySearch(arr1, size1, target);
    cout << "Searching for " << target << ": "
         << (index != -1 ? "Found at index " + to_string(index) : "Not found") << endl;
    cout << endl;

    // Recursive binary search
    cout << "Recursive Binary Search:" << endl;
    target = 45;
    index = binarySearchRecursive(arr1, 0, size1 - 1, target);
    cout << "Searching for " << target << ": "
         << (index != -1 ? "Found at index " + to_string(index) : "Not found") << endl;
    cout << endl;

    // String binary search
    cout << "String Binary Search:" << endl;
    string arr2[] = {"apple", "banana", "cherry", "date", "grape", "mango", "orange"};
    size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
    printArray(arr2, size2);

    string strTarget = "grape";
    int strIndex = binarySearch(arr2, size2, strTarget);
    cout << "Searching for \"" << strTarget << "\": "
         << (strIndex != -1 ? "Found at index " + to_string(strIndex) : "Not found") << endl;
    cout << endl;

    // Find first and last occurrence
    cout << "Find First and Last Occurrence:" << endl;
    int arr3[] = {1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5, 6};
    size_t size3 = sizeof(arr3) / sizeof(arr3[0]);
    printArray(arr3, size3);

    target = 5;
    int first = findFirst(arr3, size3, target);
    int last = findLast(arr3, size3, target);
    cout << "First occurrence of " << target << ": " << first << endl;
    cout << "Last occurrence of " << target << ": " << last << endl;
    cout << "Count of " << target << ": " << countOccurrences(arr3, size3, target) << endl;
    cout << endl;

    // Find insertion position
    cout << "Find Insertion Position:" << endl;
    int arr4[] = {1, 3, 5, 7, 9, 11, 13};
    size_t size4 = sizeof(arr4) / sizeof(arr4[0]);
    printArray(arr4, size4);

    target = 6;
    int pos = findInsertPosition(arr4, size4, target);
    cout << "Insert position for " << target << ": " << pos << endl;

    target = 0;
    pos = findInsertPosition(arr4, size4, target);
    cout << "Insert position for " << target << ": " << pos << endl;
    cout << endl;

    // Find closest value
    cout << "Find Closest Value:" << endl;
    int arr5[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    size_t size5 = sizeof(arr5) / sizeof(arr5[0]);
    printArray(arr5, size5);

    target = 37;
    int closestIdx = findClosest(arr5, size5, target);
    cout << "Closest value to " << target << ": " << arr5[closestIdx]
         << " at index " << closestIdx << endl;

    target = 85;
    closestIdx = findClosest(arr5, size5, target);
    cout << "Closest value to " << target << ": " << arr5[closestIdx]
         << " at index " << closestIdx << endl;

    return 0;
}
