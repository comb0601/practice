/*
 * Program: Array Rotation
 * Description: Rotating arrays left and right by n positions
 * Compilation: g++ 12_array_rotation.cpp -o 12_array_rotation
 * Execution: ./12_array_rotation
 */

#include <iostream>
using namespace std;

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void rotateLeft(int arr[], int size, int positions) {
    positions = positions % size;  // Handle positions > size

    for (int p = 0; p < positions; p++) {
        int first = arr[0];
        for (int i = 0; i < size - 1; i++) {
            arr[i] = arr[i + 1];
        }
        arr[size - 1] = first;
    }
}

void rotateRight(int arr[], int size, int positions) {
    positions = positions % size;  // Handle positions > size

    for (int p = 0; p < positions; p++) {
        int last = arr[size - 1];
        for (int i = size - 1; i > 0; i--) {
            arr[i] = arr[i - 1];
        }
        arr[0] = last;
    }
}

void reverseArray(int arr[], int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Efficient rotation using reversal algorithm
void rotateLeftEfficient(int arr[], int size, int positions) {
    positions = positions % size;

    // Step 1: Reverse first 'positions' elements
    reverseArray(arr, 0, positions - 1);

    // Step 2: Reverse remaining elements
    reverseArray(arr, positions, size - 1);

    // Step 3: Reverse entire array
    reverseArray(arr, 0, size - 1);
}

int main() {
    cout << "=== Array Rotation ===" << endl << endl;

    // Left rotation
    cout << "1. Rotate Left by 2 positions:" << endl;
    int arr1[] = {1, 2, 3, 4, 5, 6, 7};
    int size1 = 7;

    cout << "Original: ";
    printArray(arr1, size1);

    rotateLeft(arr1, size1, 2);
    cout << "Rotated:  ";
    printArray(arr1, size1);
    cout << endl;

    // Right rotation
    cout << "2. Rotate Right by 3 positions:" << endl;
    int arr2[] = {1, 2, 3, 4, 5, 6, 7};
    int size2 = 7;

    cout << "Original: ";
    printArray(arr2, size2);

    rotateRight(arr2, size2, 3);
    cout << "Rotated:  ";
    printArray(arr2, size2);
    cout << endl;

    // Rotate by more than size
    cout << "3. Rotate Left by 10 (size=7):" << endl;
    int arr3[] = {1, 2, 3, 4, 5, 6, 7};
    int size3 = 7;

    cout << "Original: ";
    printArray(arr3, size3);

    rotateLeft(arr3, size3, 10);  // Same as rotating by 3
    cout << "Rotated:  ";
    printArray(arr3, size3);
    cout << endl;

    // Efficient rotation
    cout << "4. Efficient Rotation (Reversal Algorithm):" << endl;
    int arr4[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size4 = 9;

    cout << "Original: ";
    printArray(arr4, size4);

    rotateLeftEfficient(arr4, size4, 4);
    cout << "Rotated left by 4: ";
    printArray(arr4, size4);
    cout << endl;

    // Complete rotation (size times)
    cout << "5. Complete Rotation (7 positions on size 7):" << endl;
    int arr5[] = {1, 2, 3, 4, 5, 6, 7};
    int size5 = 7;

    cout << "Original: ";
    printArray(arr5, size5);

    rotateLeft(arr5, size5, 7);  // Should return to original
    cout << "After rotating 7 times: ";
    printArray(arr5, size5);
    cout << endl;

    // Interactive rotation
    cout << "6. Interactive Rotation:" << endl;
    int arr6[] = {10, 20, 30, 40, 50};
    int size6 = 5;

    cout << "Array: ";
    printArray(arr6, size6);

    cout << "Enter number of positions to rotate left: ";
    int pos;
    cin >> pos;

    rotateLeft(arr6, size6, pos);
    cout << "Result: ";
    printArray(arr6, size6);
    cout << endl;

    // Demonstrate step-by-step rotation
    cout << "7. Step-by-Step Left Rotation:" << endl;
    int arr7[] = {1, 2, 3, 4, 5};
    int size7 = 5;

    cout << "Original:    ";
    printArray(arr7, size7);

    for (int i = 1; i <= 3; i++) {
        rotateLeft(arr7, size7, 1);
        cout << "After step " << i << ": ";
        printArray(arr7, size7);
    }

    return 0;
}
