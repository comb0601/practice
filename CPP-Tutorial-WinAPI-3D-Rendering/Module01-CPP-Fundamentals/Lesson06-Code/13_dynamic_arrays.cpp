/*
 * Program: Dynamic Arrays (Pointers)
 * Description: Creating and managing dynamic arrays using pointers
 * Compilation: g++ -std=c++17 13_dynamic_arrays.cpp -o 13_dynamic_arrays
 * Execution: ./13_dynamic_arrays
 */

#include <iostream>
using namespace std;

void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int* createArray(int size, int initialValue) {
    int* arr = new int[size];
    for (int i = 0; i < size; i++) {
        arr[i] = initialValue;
    }
    return arr;
}

int* resizeArray(int* oldArr, int oldSize, int newSize) {
    int* newArr = new int[newSize];

    // Copy old data
    int copySize = (oldSize < newSize) ? oldSize : newSize;
    for (int i = 0; i < copySize; i++) {
        newArr[i] = oldArr[i];
    }

    // Initialize new elements if growing
    for (int i = oldSize; i < newSize; i++) {
        newArr[i] = 0;
    }

    delete[] oldArr;  // Free old memory
    return newArr;
}

int main() {
    cout << "=== Dynamic Arrays with Pointers ===" << endl << endl;

    // Create dynamic array
    cout << "1. Creating Dynamic Array:" << endl;
    int size;
    cout << "Enter array size: ";
    cin >> size;

    int* arr = new int[size];

    cout << "Enter " << size << " numbers:" << endl;
    for (int i = 0; i < size; i++) {
        cout << "  [" << i << "]: ";
        cin >> arr[i];
    }

    cout << "Your array: ";
    printArray(arr, size);
    cout << endl;

    // Array operations
    cout << "2. Array Operations:" << endl;
    int sum = 0;
    int max = arr[0];
    int min = arr[0];

    for (int i = 0; i < size; i++) {
        sum += arr[i];
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }

    cout << "Sum: " << sum << endl;
    cout << "Average: " << (double)sum / size << endl;
    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl << endl;

    // Modify array
    cout << "3. Modifying Array:" << endl;
    cout << "Before: ";
    printArray(arr, size);

    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }

    cout << "After doubling: ";
    printArray(arr, size);
    cout << endl;

    // Clean up
    delete[] arr;
    arr = nullptr;

    // Create array with function
    cout << "4. Creating Array with Function:" << endl;
    int* arr2 = createArray(5, 7);
    cout << "Array filled with 7: ";
    printArray(arr2, 5);
    cout << endl;

    // Resize array
    cout << "5. Resizing Array:" << endl;
    cout << "Original (size 5): ";
    printArray(arr2, 5);

    arr2 = resizeArray(arr2, 5, 8);
    cout << "After resize to 8: ";
    printArray(arr2, 8);

    arr2 = resizeArray(arr2, 8, 3);
    cout << "After resize to 3: ";
    printArray(arr2, 3);
    cout << endl;

    delete[] arr2;

    // 2D dynamic array
    cout << "6. 2D Dynamic Array:" << endl;
    int rows = 3, cols = 4;
    int** matrix = new int*[rows];

    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // Fill matrix
    int value = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value++;
        }
    }

    // Print matrix
    cout << "Matrix (" << rows << "x" << cols << "):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Clean up 2D array
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    // Dynamic array of different sizes
    cout << "7. Jagged Array (Different Row Sizes):" << endl;
    int numRows = 3;
    int** jagged = new int*[numRows];
    int rowSizes[] = {2, 4, 3};

    for (int i = 0; i < numRows; i++) {
        jagged[i] = new int[rowSizes[i]];
        for (int j = 0; j < rowSizes[i]; j++) {
            jagged[i][j] = i * 10 + j;
        }
    }

    cout << "Jagged array:" << endl;
    for (int i = 0; i < numRows; i++) {
        cout << "Row " << i << " (size " << rowSizes[i] << "): ";
        for (int j = 0; j < rowSizes[i]; j++) {
            cout << jagged[i][j] << " ";
        }
        cout << endl;
    }

    // Clean up jagged array
    for (int i = 0; i < numRows; i++) {
        delete[] jagged[i];
    }
    delete[] jagged;

    return 0;
}
