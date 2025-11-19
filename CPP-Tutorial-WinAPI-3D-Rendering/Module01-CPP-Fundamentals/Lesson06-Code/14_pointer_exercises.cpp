/*
 * Program: Pointer Exercises
 * Description: Practice problems using pointers
 * Compilation: g++ -std=c++17 14_pointer_exercises.cpp -o 14_pointer_exercises
 * Execution: ./14_pointer_exercises
 */

#include <iostream>
using namespace std;

// 1. Find maximum using pointers
int* findMax(int* arr, int size) {
    int* maxPtr = arr;
    for (int i = 1; i < size; i++) {
        if (*(arr + i) > *maxPtr) {
            maxPtr = arr + i;
        }
    }
    return maxPtr;
}

// 2. Reverse array using pointers
void reverseArray(int* arr, int size) {
    int* left = arr;
    int* right = arr + size - 1;

    while (left < right) {
        int temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

// 3. Remove duplicates
int removeDuplicates(int* arr, int size) {
    if (size == 0) return 0;

    int* slow = arr;
    for (int* fast = arr + 1; fast < arr + size; fast++) {
        bool isDuplicate = false;
        for (int* check = arr; check <= slow; check++) {
            if (*fast == *check) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            slow++;
            *slow = *fast;
        }
    }

    return slow - arr + 1;
}

// 4. Merge two sorted arrays
int* mergeSortedArrays(int* arr1, int size1, int* arr2, int size2, int& resultSize) {
    resultSize = size1 + size2;
    int* result = new int[resultSize];

    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    while (i < size1) result[k++] = arr1[i++];
    while (j < size2) result[k++] = arr2[j++];

    return result;
}

// 5. Rotate array
void rotateArray(int* arr, int size, int k) {
    k = k % size;
    if (k == 0) return;

    int* temp = new int[k];

    // Save first k elements
    for (int i = 0; i < k; i++) {
        temp[i] = arr[i];
    }

    // Shift remaining elements
    for (int i = 0; i < size - k; i++) {
        arr[i] = arr[i + k];
    }

    // Copy back saved elements
    for (int i = 0; i < k; i++) {
        arr[size - k + i] = temp[i];
    }

    delete[] temp;
}

// 6. Find pair with sum
bool findPairWithSum(int* arr, int size, int target, int& idx1, int& idx2) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] + arr[j] == target) {
                idx1 = i;
                idx2 = j;
                return true;
            }
        }
    }
    return false;
}

// 7. Copy array (deep copy)
int* copyArray(int* arr, int size) {
    int* copy = new int[size];
    for (int i = 0; i < size; i++) {
        copy[i] = arr[i];
    }
    return copy;
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "=== Pointer Exercises ===" << endl << endl;

    // Exercise 1: Find maximum
    cout << "1. Find Maximum Using Pointers:" << endl;
    int arr1[] = {5, 2, 9, 1, 7, 6};
    int size1 = 6;

    cout << "Array: ";
    printArray(arr1, size1);

    int* maxPtr = findMax(arr1, size1);
    cout << "Maximum: " << *maxPtr << " at index " << (maxPtr - arr1) << endl << endl;

    // Exercise 2: Reverse array
    cout << "2. Reverse Array:" << endl;
    int arr2[] = {1, 2, 3, 4, 5};
    int size2 = 5;

    cout << "Before: ";
    printArray(arr2, size2);

    reverseArray(arr2, size2);

    cout << "After:  ";
    printArray(arr2, size2);
    cout << endl;

    // Exercise 3: Remove duplicates
    cout << "3. Remove Duplicates:" << endl;
    int arr3[] = {1, 2, 2, 3, 4, 4, 5};
    int size3 = 7;

    cout << "Before: ";
    printArray(arr3, size3);

    int newSize = removeDuplicates(arr3, size3);

    cout << "After (unique elements): ";
    printArray(arr3, newSize);
    cout << endl;

    // Exercise 4: Merge sorted arrays
    cout << "4. Merge Two Sorted Arrays:" << endl;
    int sorted1[] = {1, 3, 5, 7};
    int sorted2[] = {2, 4, 6, 8};
    int resultSize;

    cout << "Array 1: ";
    printArray(sorted1, 4);
    cout << "Array 2: ";
    printArray(sorted2, 4);

    int* merged = mergeSortedArrays(sorted1, 4, sorted2, 4, resultSize);

    cout << "Merged:  ";
    printArray(merged, resultSize);
    cout << endl;

    delete[] merged;

    // Exercise 5: Rotate array
    cout << "5. Rotate Array:" << endl;
    int arr5[] = {1, 2, 3, 4, 5, 6, 7};
    int size5 = 7;

    cout << "Original: ";
    printArray(arr5, size5);

    rotateArray(arr5, size5, 3);

    cout << "Rotated by 3: ";
    printArray(arr5, size5);
    cout << endl;

    // Exercise 6: Find pair with sum
    cout << "6. Find Pair with Sum:" << endl;
    int arr6[] = {2, 7, 11, 15};
    int size6 = 4;
    int target = 9;

    cout << "Array: ";
    printArray(arr6, size6);
    cout << "Target sum: " << target << endl;

    int idx1, idx2;
    if (findPairWithSum(arr6, size6, target, idx1, idx2)) {
        cout << "Found pair: arr[" << idx1 << "] + arr[" << idx2 << "] = "
             << arr6[idx1] << " + " << arr6[idx2] << " = " << target << endl;
    } else {
        cout << "No pair found" << endl;
    }
    cout << endl;

    // Exercise 7: Copy array
    cout << "7. Deep Copy Array:" << endl;
    int arr7[] = {10, 20, 30};
    int size7 = 3;

    cout << "Original: ";
    printArray(arr7, size7);

    int* copy = copyArray(arr7, size7);

    cout << "Copy:     ";
    printArray(copy, size7);

    // Modify original
    arr7[0] = 99;

    cout << "After modifying original[0] = 99:" << endl;
    cout << "Original: ";
    printArray(arr7, size7);
    cout << "Copy:     ";
    printArray(copy, size7);
    cout << "Copy is independent!" << endl;

    delete[] copy;

    return 0;
}
