/*
 * Program: Pointers and Arrays
 * Description: Relationship between pointers and arrays
 * Compilation: g++ -std=c++17 03_pointers_arrays.cpp -o 03_pointers_arrays
 * Execution: ./03_pointers_arrays
 */

#include <iostream>
using namespace std;

void printArrayUsingPointer(int* arr, int size) {
    cout << "Array elements: ";
    for (int i = 0; i < size; i++) {
        cout << *(arr + i) << " ";
    }
    cout << endl;
}

int sumArrayUsingPointer(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *(arr + i);
    }
    return sum;
}

void reverseArrayUsingPointers(int* arr, int size) {
    int* left = arr;
    int* right = arr + size - 1;

    while (left < right) {
        // Swap
        int temp = *left;
        *left = *right;
        *right = temp;

        left++;
        right--;
    }
}

int main() {
    cout << "=== Pointers and Arrays ===" << endl << endl;

    int numbers[] = {10, 20, 30, 40, 50};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    // Array name as pointer
    cout << "1. Array Name as Pointer:" << endl;
    cout << "numbers = " << numbers << endl;
    cout << "&numbers[0] = " << &numbers[0] << endl;
    cout << "They are the same!" << endl << endl;

    // Accessing array elements
    cout << "2. Accessing Elements:" << endl;
    cout << "Using array notation: numbers[0] = " << numbers[0] << endl;
    cout << "Using pointer notation: *numbers = " << *numbers << endl;
    cout << "Both access first element!" << endl << endl;

    // Array indexing equivalence
    cout << "3. Array Indexing Equivalence:" << endl;
    for (int i = 0; i < size; i++) {
        cout << "numbers[" << i << "] = " << numbers[i];
        cout << " = *(numbers + " << i << ") = " << *(numbers + i) << endl;
    }
    cout << endl;

    // Pointer to array
    cout << "4. Pointer to Array:" << endl;
    int* ptr = numbers;

    cout << "Using pointer like array:" << endl;
    for (int i = 0; i < size; i++) {
        cout << "ptr[" << i << "] = " << ptr[i] << endl;
    }
    cout << endl;

    // Passing array to function
    cout << "5. Passing Array to Function:" << endl;
    printArrayUsingPointer(numbers, size);
    cout << endl;

    // Sum using pointer
    cout << "6. Sum Using Pointer:" << endl;
    int total = sumArrayUsingPointer(numbers, size);
    cout << "Sum of array: " << total << endl << endl;

    // Reverse array
    cout << "7. Reverse Array Using Pointers:" << endl;
    cout << "Before: ";
    printArrayUsingPointer(numbers, size);

    reverseArrayUsingPointers(numbers, size);

    cout << "After:  ";
    printArrayUsingPointer(numbers, size);
    cout << endl;

    // Pointer navigation
    cout << "8. Pointer Navigation:" << endl;
    reverseArrayUsingPointers(numbers, size);  // Reverse back

    ptr = numbers;
    cout << "Start: *ptr = " << *ptr << endl;

    ptr += 2;
    cout << "After ptr += 2: *ptr = " << *ptr << endl;

    ptr -= 1;
    cout << "After ptr -= 1: *ptr = " << *ptr << endl << endl;

    // Array of pointers
    cout << "9. Array of Pointers:" << endl;
    int a = 10, b = 20, c = 30;
    int* ptrArray[3] = {&a, &b, &c};

    cout << "Array of pointers:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "ptrArray[" << i << "] points to: " << *ptrArray[i] << endl;
    }
    cout << endl;

    // 2D array with pointers
    cout << "10. 2D Array Access:" << endl;
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

    cout << "Matrix using pointers:" << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            cout << *(*(matrix + i) + j) << " ";
        }
        cout << endl;
    }

    return 0;
}
