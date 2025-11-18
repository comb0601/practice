# Lesson 08: Arrays and Multidimensional Arrays

**Duration: 3 hours**

## Table of Contents
1. Introduction to Arrays
2. Array Declaration and Initialization
3. Accessing Array Elements
4. Array Operations
5. Multidimensional Arrays
6. Arrays and Functions
7. Common Array Algorithms
8. Practical Exercises

## 1. Introduction to Arrays

Arrays are collections of elements of the same type stored in contiguous memory locations.

### Why Use Arrays?

```cpp
#include <iostream>

int main() {
    // Without arrays - need separate variables
    int score1 = 85;
    int score2 = 90;
    int score3 = 78;
    int score4 = 92;
    int score5 = 88;

    // With arrays - organized collection
    int scores[5] = {85, 90, 78, 92, 88};

    // Easy to process with loops
    for (int i = 0; i < 5; i++) {
        std::cout << "Score " << (i+1) << ": " << scores[i] << std::endl;
    }

    return 0;
}
```

## 2. Array Declaration and Initialization

### Basic Declaration

```cpp
#include <iostream>

int main() {
    // Syntax: type arrayName[size];
    int numbers[5];  // Uninitialized (contains garbage)

    // Initialize with values
    int ages[5] = {25, 30, 35, 40, 45};

    // Partial initialization (rest are zero)
    int values[10] = {1, 2, 3};  // {1,2,3,0,0,0,0,0,0,0}

    // Size inferred from initializer
    int temps[] = {20, 22, 25, 23, 21};  // Size is 5

    // Uniform initialization (C++11)
    int data[5] {10, 20, 30, 40, 50};

    return 0;
}
```

### Different Data Types

```cpp
#include <iostream>

int main() {
    int integers[5] = {1, 2, 3, 4, 5};
    double decimals[3] = {1.5, 2.7, 3.9};
    char letters[5] = {'A', 'B', 'C', 'D', 'E'};
    bool flags[3] = {true, false, true};

    // Character array (C-style string)
    char name[10] = "John";  // Automatically null-terminated

    // Display
    for (int i = 0; i < 5; i++) {
        std::cout << integers[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 3. Accessing Array Elements

### Index-Based Access

```cpp
#include <iostream>

int main() {
    int numbers[5] = {10, 20, 30, 40, 50};

    // Access using index (0-based)
    std::cout << "First element: " << numbers[0] << std::endl;
    std::cout << "Last element: " << numbers[4] << std::endl;

    // Modify elements
    numbers[2] = 35;
    std::cout << "Modified third element: " << numbers[2] << std::endl;

    // Loop through array
    for (int i = 0; i < 5; i++) {
        std::cout << "Element " << i << ": " << numbers[i] << std::endl;
    }

    return 0;
}
```

### Array Size

```cpp
#include <iostream>

int main() {
    int scores[] = {85, 90, 78, 92, 88, 95, 76};

    // Calculate size
    int size = sizeof(scores) / sizeof(scores[0]);

    std::cout << "Array size: " << size << std::endl;

    // Use in loops
    for (int i = 0; i < size; i++) {
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 4. Array Operations

### Finding Minimum and Maximum

```cpp
#include <iostream>
#include <climits>

int main() {
    int numbers[] = {45, 23, 78, 12, 90, 34, 67};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    int min = INT_MAX;
    int max = INT_MIN;

    for (int i = 0; i < size; i++) {
        if (numbers[i] < min) min = numbers[i];
        if (numbers[i] > max) max = numbers[i];
    }

    std::cout << "Minimum: " << min << std::endl;
    std::cout << "Maximum: " << max << std::endl;

    return 0;
}
```

### Sum and Average

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double grades[] = {85.5, 90.0, 78.5, 92.0, 88.5};
    int size = sizeof(grades) / sizeof(grades[0]);

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += grades[i];
    }

    double average = sum / size;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average: " << average << std::endl;

    return 0;
}
```

### Linear Search

```cpp
#include <iostream>

int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Found at index i
        }
    }
    return -1;  // Not found
}

int main() {
    int numbers[] = {10, 25, 30, 45, 50, 60};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    int target = 45;

    int index = linearSearch(numbers, size, target);

    if (index != -1) {
        std::cout << "Found " << target << " at index " << index << std::endl;
    } else {
        std::cout << target << " not found" << std::endl;
    }

    return 0;
}
```

### Bubble Sort

```cpp
#include <iostream>

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

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original array: ";
    printArray(numbers, size);

    bubbleSort(numbers, size);

    std::cout << "Sorted array: ";
    printArray(numbers, size);

    return 0;
}
```

## 5. Multidimensional Arrays

### 2D Arrays (Matrices)

```cpp
#include <iostream>

int main() {
    // Declare 2D array
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Access elements
    std::cout << "Element at [1][2]: " << matrix[1][2] << std::endl;

    // Loop through 2D array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << "	";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Matrix Operations

```cpp
#include <iostream>

const int ROWS = 3;
const int COLS = 3;

void printMatrix(int matrix[][COLS], int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << matrix[i][j] << "	";
        }
        std::cout << std::endl;
    }
}

void addMatrices(int a[][COLS], int b[][COLS], int result[][COLS], int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLS; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

int main() {
    int matrix1[ROWS][COLS] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int matrix2[ROWS][COLS] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int result[ROWS][COLS];

    std::cout << "Matrix 1:" << std::endl;
    printMatrix(matrix1, ROWS);

    std::cout << "
Matrix 2:" << std::endl;
    printMatrix(matrix2, ROWS);

    addMatrices(matrix1, matrix2, result, ROWS);

    std::cout << "
Sum:" << std::endl;
    printMatrix(result, ROWS);

    return 0;
}
```

### 3D Arrays

```cpp
#include <iostream>

int main() {
    // 3D array: layers x rows x columns
    int cube[2][3][4] = {
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}
        },
        {
            {13, 14, 15, 16},
            {17, 18, 19, 20},
            {21, 22, 23, 24}
        }
    };

    // Access element
    std::cout << "Element at [1][2][3]: " << cube[1][2][3] << std::endl;

    // Loop through 3D array
    for (int i = 0; i < 2; i++) {
        std::cout << "Layer " << i << ":" << std::endl;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                std::cout << cube[i][j][k] << "	";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## 6. Arrays and Functions

### Passing Arrays to Functions

```cpp
#include <iostream>

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void multiplyByTwo(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    multiplyByTwo(numbers, size);

    std::cout << "After multiplying by 2: ";
    printArray(numbers, size);

    return 0;
}
```

### Returning Arrays from Functions (Using Pointers)

```cpp
#include <iostream>

int* createArray(int size) {
    int* arr = new int[size];
    for (int i = 0; i < size; i++) {
        arr[i] = i * 10;
    }
    return arr;
}

int main() {
    int size = 5;
    int* numbers = createArray(size);

    for (int i = 0; i < size; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    delete[] numbers;  // Important: free memory

    return 0;
}
```

## 7. Common Array Algorithms

### Reverse Array

```cpp
#include <iostream>

void reverseArray(int arr[], int size) {
    int start = 0;
    int end = size - 1;

    while (start < end) {
        // Swap elements
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;

        start++;
        end--;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    reverseArray(numbers, size);

    std::cout << "Reversed: ";
    printArray(numbers, size);

    return 0;
}
```

### Rotate Array

```cpp
#include <iostream>

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

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    rotateLeft(numbers, size, 2);

    std::cout << "After rotating left by 2: ";
    printArray(numbers, size);

    return 0;
}
```

## 8. Practical Exercises

### Exercise 1: Grade Manager

```cpp
#include <iostream>
#include <iomanip>

int main() {
    const int MAX_STUDENTS = 5;
    double grades[MAX_STUDENTS];

    // Input grades
    std::cout << "Enter grades for " << MAX_STUDENTS << " students:" << std::endl;
    for (int i = 0; i < MAX_STUDENTS; i++) {
        std::cout << "Student " << (i + 1) << ": ";
        std::cin >> grades[i];
    }

    // Calculate statistics
    double sum = 0, max = grades[0], min = grades[0];
    for (int i = 0; i < MAX_STUDENTS; i++) {
        sum += grades[i];
        if (grades[i] > max) max = grades[i];
        if (grades[i] < min) min = grades[i];
    }

    double average = sum / MAX_STUDENTS;

    // Display results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "
=== Grade Statistics ===" << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Highest: " << max << std::endl;
    std::cout << "Lowest: " << min << std::endl;

    return 0;
}
```

## Complete Project: Student Record System

```cpp
#include <iostream>
#include <string>
#include <iomanip>

const int MAX_STUDENTS = 50;

int main() {
    std::string names[MAX_STUDENTS];
    int ages[MAX_STUDENTS];
    double gpa[MAX_STUDENTS];
    int studentCount = 0;
    int choice;

    do {
        std::cout << "
=== Student Record System ===" << std::endl;
        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. Display All Students" << std::endl;
        std::cout << "3. Find Top Student" << std::endl;
        std::cout << "4. Calculate Average GPA" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                if (studentCount < MAX_STUDENTS) {
                    std::cin.ignore();
                    std::cout << "Enter name: ";
                    std::getline(std::cin, names[studentCount]);
                    std::cout << "Enter age: ";
                    std::cin >> ages[studentCount];
                    std::cout << "Enter GPA: ";
                    std::cin >> gpa[studentCount];
                    studentCount++;
                    std::cout << "Student added!" << std::endl;
                } else {
                    std::cout << "Maximum students reached!" << std::endl;
                }
                break;

            case 2:
                std::cout << "
=== Student List ===" << std::endl;
                for (int i = 0; i < studentCount; i++) {
                    std::cout << (i + 1) << ". " << names[i]
                              << " (Age: " << ages[i]
                              << ", GPA: " << std::fixed
                              << std::setprecision(2) << gpa[i] << ")" << std::endl;
                }
                break;

            case 3:
                if (studentCount > 0) {
                    int topIndex = 0;
                    for (int i = 1; i < studentCount; i++) {
                        if (gpa[i] > gpa[topIndex]) {
                            topIndex = i;
                        }
                    }
                    std::cout << "Top student: " << names[topIndex]
                              << " (GPA: " << std::fixed << std::setprecision(2)
                              << gpa[topIndex] << ")" << std::endl;
                } else {
                    std::cout << "No students in system!" << std::endl;
                }
                break;

            case 4:
                if (studentCount > 0) {
                    double sum = 0;
                    for (int i = 0; i < studentCount; i++) {
                        sum += gpa[i];
                    }
                    std::cout << "Average GPA: " << std::fixed
                              << std::setprecision(2) << (sum / studentCount) << std::endl;
                } else {
                    std::cout << "No students in system!" << std::endl;
                }
                break;

            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 5);

    return 0;
}
```

## Summary

In this lesson, you learned:
- ✅ Array declaration and initialization
- ✅ Accessing and modifying array elements
- ✅ Common array operations (search, sort, min, max)
- ✅ Multidimensional arrays (2D and 3D)
- ✅ Passing arrays to functions
- ✅ Array algorithms (reverse, rotate)
- ✅ Practical applications

## Next Lesson

Lesson 09: Pointers and References

## Practice Problems

1. Write a program to find second largest element in array
2. Implement selection sort algorithm
3. Create a program to merge two sorted arrays
4. Write matrix multiplication function
5. Implement a simple tic-tac-toe game using 2D array
