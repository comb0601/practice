/*
 * Program: Array Merge and Split
 * Description: Merging multiple arrays and splitting arrays
 * Compilation: g++ 13_array_merge_split.cpp -o 13_array_merge_split
 * Execution: ./13_array_merge_split
 */

#include <iostream>
using namespace std;

void printArray(int arr[], int size, const char* name) {
    cout << name << ": ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Merge two arrays
void mergeArrays(int arr1[], int size1, int arr2[], int size2, int result[]) {
    // Copy first array
    for (int i = 0; i < size1; i++) {
        result[i] = arr1[i];
    }

    // Copy second array
    for (int i = 0; i < size2; i++) {
        result[size1 + i] = arr2[i];
    }
}

// Merge two sorted arrays into sorted result
void mergeSorted(int arr1[], int size1, int arr2[], int size2, int result[]) {
    int i = 0, j = 0, k = 0;

    // Merge while both arrays have elements
    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    // Copy remaining elements from arr1
    while (i < size1) {
        result[k++] = arr1[i++];
    }

    // Copy remaining elements from arr2
    while (j < size2) {
        result[k++] = arr2[j++];
    }
}

// Split array into two halves
void splitArray(int arr[], int size, int left[], int right[], int& leftSize, int& rightSize) {
    leftSize = size / 2;
    rightSize = size - leftSize;

    for (int i = 0; i < leftSize; i++) {
        left[i] = arr[i];
    }

    for (int i = 0; i < rightSize; i++) {
        right[i] = arr[leftSize + i];
    }
}

// Split array based on a pivot value
void splitByValue(int arr[], int size, int pivot, int smaller[], int larger[],
                  int& smallerSize, int& largerSize) {
    smallerSize = 0;
    largerSize = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i] < pivot) {
            smaller[smallerSize++] = arr[i];
        } else {
            larger[largerSize++] = arr[i];
        }
    }
}

// Interleave two arrays
void interleaveArrays(int arr1[], int size1, int arr2[], int size2, int result[]) {
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {
        result[k++] = arr1[i++];
        result[k++] = arr2[j++];
    }

    while (i < size1) {
        result[k++] = arr1[i++];
    }

    while (j < size2) {
        result[k++] = arr2[j++];
    }
}

int main() {
    cout << "=== Array Merge and Split Operations ===" << endl << endl;

    // Simple merge
    cout << "1. Simple Array Merge:" << endl;
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6, 7};
    int merged[10];

    printArray(arr1, 3, "Array 1");
    printArray(arr2, 4, "Array 2");

    mergeArrays(arr1, 3, arr2, 4, merged);
    printArray(merged, 7, "Merged ");
    cout << endl;

    // Merge sorted arrays
    cout << "2. Merge Sorted Arrays:" << endl;
    int sorted1[] = {1, 3, 5, 7};
    int sorted2[] = {2, 4, 6, 8};
    int sortedMerged[10];

    printArray(sorted1, 4, "Sorted 1");
    printArray(sorted2, 4, "Sorted 2");

    mergeSorted(sorted1, 4, sorted2, 4, sortedMerged);
    printArray(sortedMerged, 8, "Merged  ");
    cout << endl;

    // Split array
    cout << "3. Split Array into Two Halves:" << endl;
    int original[] = {10, 20, 30, 40, 50, 60, 70};
    int left[10], right[10];
    int leftSize, rightSize;

    printArray(original, 7, "Original");

    splitArray(original, 7, left, right, leftSize, rightSize);
    printArray(left, leftSize, "Left    ");
    printArray(right, rightSize, "Right   ");
    cout << endl;

    // Split by value
    cout << "4. Split by Pivot Value (pivot = 25):" << endl;
    int data[] = {10, 35, 15, 40, 5, 30, 20};
    int smaller[10], larger[10];
    int smallerSize, largerSize;

    printArray(data, 7, "Original");

    splitByValue(data, 7, 25, smaller, larger, smallerSize, largerSize);
    printArray(smaller, smallerSize, "< 25    ");
    printArray(larger, largerSize, ">= 25   ");
    cout << endl;

    // Merge three arrays
    cout << "5. Merge Three Arrays:" << endl;
    int a[] = {1, 2};
    int b[] = {3, 4};
    int c[] = {5, 6};
    int temp[10], final[10];

    printArray(a, 2, "A");
    printArray(b, 2, "B");
    printArray(c, 2, "C");

    mergeArrays(a, 2, b, 2, temp);
    mergeArrays(temp, 4, c, 2, final);
    printArray(final, 6, "Merged");
    cout << endl;

    // Interleave arrays
    cout << "6. Interleave Two Arrays:" << endl;
    int x[] = {1, 3, 5, 7};
    int y[] = {2, 4, 6};
    int interleaved[10];

    printArray(x, 4, "X         ");
    printArray(y, 3, "Y         ");

    interleaveArrays(x, 4, y, 3, interleaved);
    printArray(interleaved, 7, "Interleaved");
    cout << endl;

    // Even-odd split
    cout << "7. Split into Even and Odd:" << endl;
    int numbers[] = {12, 7, 23, 8, 15, 4, 19};
    int even[10], odd[10];
    int evenSize = 0, oddSize = 0;

    printArray(numbers, 7, "Original");

    for (int i = 0; i < 7; i++) {
        if (numbers[i] % 2 == 0) {
            even[evenSize++] = numbers[i];
        } else {
            odd[oddSize++] = numbers[i];
        }
    }

    printArray(even, evenSize, "Even    ");
    printArray(odd, oddSize, "Odd     ");

    return 0;
}
