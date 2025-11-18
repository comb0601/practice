# Lesson 16: Arrays - Declaration and Initialization

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Declare and initialize arrays in C++
- Access and modify array elements
- Understand array bounds and common pitfalls
- Work with arrays in functions
- Use different array initialization techniques
- Implement basic array algorithms

## 1. Array Basics

### 1.1 What is an Array?

An array is a collection of elements of the same type stored in contiguous memory locations.

```cpp
#include <iostream>
using namespace std;

int main() {
    // Declare an array of 5 integers
    int numbers[5];

    // Assign values
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    numbers[3] = 40;
    numbers[4] = 50;

    // Access and print values
    for (int i = 0; i < 5; i++) {
        cout << "numbers[" << i << "] = " << numbers[i] << endl;
    }

    return 0;
}
```

### 1.2 Array Declaration and Initialization

```cpp
#include <iostream>
using namespace std;

int main() {
    // Different ways to declare and initialize arrays

    // Declaration with size
    int arr1[5];  // Uninitialized (contains garbage values)

    // Declaration with initialization
    int arr2[5] = {1, 2, 3, 4, 5};

    // Partial initialization (rest filled with 0)
    int arr3[5] = {1, 2};  // {1, 2, 0, 0, 0}

    // Initialize all to zero
    int arr4[5] = {0};  // {0, 0, 0, 0, 0}

    // Size inferred from initializer
    int arr5[] = {10, 20, 30, 40};  // Size is 4

    // Print arr2
    cout << "arr2: ";
    for (int i = 0; i < 5; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    // Print arr3 (notice the zeros)
    cout << "arr3: ";
    for (int i = 0; i < 5; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;

    return 0;
}
```

### 1.3 Array Element Access

```cpp
#include <iostream>
using namespace std;

int main() {
    int scores[5] = {85, 92, 78, 95, 88};

    // Access elements using index (0-based)
    cout << "First score: " << scores[0] << endl;
    cout << "Last score: " << scores[4] << endl;

    // Modify elements
    scores[2] = 80;  // Change third element
    cout << "Updated third score: " << scores[2] << endl;

    // Calculate average
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += scores[i];
    }
    double average = sum / 5.0;
    cout << "Average score: " << average << endl;

    return 0;
}
```

## 2. Working with Arrays

### 2.1 Reading Array Input

```cpp
#include <iostream>
using namespace std;

int main() {
    const int SIZE = 5;
    int numbers[SIZE];

    cout << "Enter " << SIZE << " numbers:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << "Number " << (i + 1) << ": ";
        cin >> numbers[i];
    }

    cout << "\nYou entered:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    return 0;
}
```

### 2.2 Different Data Types in Arrays

```cpp
#include <iostream>
using namespace std;

int main() {
    // Integer array
    int integers[] = {1, 2, 3, 4, 5};

    // Double array
    double temperatures[] = {98.6, 72.5, 85.3, 91.2};

    // Character array
    char grades[] = {'A', 'B', 'C', 'D', 'F'};

    // Boolean array
    bool flags[] = {true, false, true, true, false};

    cout << "Temperatures: ";
    for (int i = 0; i < 4; i++) {
        cout << temperatures[i] << " ";
    }
    cout << endl;

    cout << "Grades: ";
    for (int i = 0; i < 5; i++) {
        cout << grades[i] << " ";
    }
    cout << endl;

    return 0;
}
```

### 2.3 Constant Arrays

```cpp
#include <iostream>
using namespace std;

int main() {
    // Constant array - cannot be modified
    const int DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char* MONTH_NAMES[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    cout << "Days in each month:" << endl;
    for (int i = 0; i < 12; i++) {
        cout << MONTH_NAMES[i] << ": " << DAYS_IN_MONTH[i] << " days" << endl;
    }

    // DAYS_IN_MONTH[0] = 30;  // ERROR: cannot modify const array

    return 0;
}
```

## 3. Arrays and Functions

### 3.1 Passing Arrays to Functions

```cpp
#include <iostream>
using namespace std;

// Function to print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function to find sum
int arraySum(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Function to find average
double arrayAverage(int arr[], int size) {
    return static_cast<double>(arraySum(arr, size)) / size;
}

int main() {
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    cout << "Array: ";
    printArray(numbers, size);

    cout << "Sum: " << arraySum(numbers, size) << endl;
    cout << "Average: " << arrayAverage(numbers, size) << endl;

    return 0;
}
```

### 3.2 Modifying Arrays in Functions

```cpp
#include <iostream>
using namespace std;

// Function to double all elements
void doubleArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

// Function to fill array with value
void fillArray(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;

    cout << "Original: ";
    printArray(numbers, size);

    doubleArray(numbers, size);
    cout << "Doubled: ";
    printArray(numbers, size);

    fillArray(numbers, size, 10);
    cout << "Filled with 10: ";
    printArray(numbers, size);

    return 0;
}
```

## 4. Common Array Operations

### 4.1 Finding Minimum and Maximum

```cpp
#include <iostream>
using namespace std;

int findMin(int arr[], int size) {
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int findMax(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main() {
    int numbers[] = {45, 23, 78, 12, 90, 34, 67};
    int size = 7;

    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    cout << "Minimum: " << findMin(numbers, size) << endl;
    cout << "Maximum: " << findMax(numbers, size) << endl;

    return 0;
}
```

### 4.2 Searching in Arrays

```cpp
#include <iostream>
using namespace std;

// Linear search
int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Return index if found
        }
    }
    return -1;  // Not found
}

// Count occurrences
int countOccurrences(int arr[], int size, int value) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            count++;
        }
    }
    return count;
}

int main() {
    int numbers[] = {5, 12, 5, 8, 5, 20, 5, 15};
    int size = 8;

    int target = 8;
    int index = linearSearch(numbers, size, target);

    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }

    int count = countOccurrences(numbers, size, 5);
    cout << "Number 5 appears " << count << " times" << endl;

    return 0;
}
```

### 4.3 Copying Arrays

```cpp
#include <iostream>
using namespace std;

void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int original[] = {10, 20, 30, 40, 50};
    int copy[5];
    int size = 5;

    copyArray(original, copy, size);

    cout << "Original: ";
    printArray(original, size);

    cout << "Copy: ";
    printArray(copy, size);

    // Modify copy
    copy[2] = 999;

    cout << "\nAfter modifying copy:" << endl;
    cout << "Original: ";
    printArray(original, size);

    cout << "Copy: ";
    printArray(copy, size);

    return 0;
}
```

## 5. Practical Examples

### Example 1: Student Grades Manager

```cpp
#include <iostream>
using namespace std;

const int MAX_STUDENTS = 10;

void inputGrades(int grades[], int count) {
    for (int i = 0; i < count; i++) {
        cout << "Enter grade for student " << (i + 1) << ": ";
        cin >> grades[i];
    }
}

char getLetterGrade(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

void printGradeReport(int grades[], int count) {
    cout << "\n=== Grade Report ===" << endl;
    for (int i = 0; i < count; i++) {
        cout << "Student " << (i + 1) << ": "
             << grades[i] << " (" << getLetterGrade(grades[i]) << ")" << endl;
    }
}

int main() {
    int grades[MAX_STUDENTS];
    int studentCount;

    cout << "How many students? ";
    cin >> studentCount;

    if (studentCount > MAX_STUDENTS) {
        studentCount = MAX_STUDENTS;
    }

    inputGrades(grades, studentCount);
    printGradeReport(grades, studentCount);

    return 0;
}
```

### Example 2: Temperature Tracker

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

const int DAYS = 7;

void inputTemperatures(double temps[], int days) {
    for (int i = 0; i < days; i++) {
        cout << "Temperature for day " << (i + 1) << ": ";
        cin >> temps[i];
    }
}

double getAverage(double temps[], int days) {
    double sum = 0;
    for (int i = 0; i < days; i++) {
        sum += temps[i];
    }
    return sum / days;
}

void printReport(double temps[], int days) {
    cout << fixed << setprecision(1);

    cout << "\n=== Temperature Report ===" << endl;
    for (int i = 0; i < days; i++) {
        cout << "Day " << (i + 1) << ": " << temps[i] << "°F" << endl;
    }

    double avg = getAverage(temps, days);
    cout << "\nAverage: " << avg << "°F" << endl;

    cout << "\nDays above average:" << endl;
    for (int i = 0; i < days; i++) {
        if (temps[i] > avg) {
            cout << "  Day " << (i + 1) << ": " << temps[i] << "°F" << endl;
        }
    }
}

int main() {
    double temperatures[DAYS];

    cout << "Enter temperatures for " << DAYS << " days:" << endl;
    inputTemperatures(temperatures, DAYS);
    printReport(temperatures, DAYS);

    return 0;
}
```

### Example 3: Histogram Generator

```cpp
#include <iostream>
using namespace std;

void printHistogram(int data[], int size) {
    cout << "\nHistogram:" << endl;

    for (int i = 0; i < size; i++) {
        cout << i + 1 << ": ";
        for (int j = 0; j < data[i]; j++) {
            cout << "*";
        }
        cout << " (" << data[i] << ")" << endl;
    }
}

int main() {
    int values[] = {5, 8, 3, 10, 7, 4, 9};
    int size = 7;

    cout << "Data values:" << endl;
    for (int i = 0; i < size; i++) {
        cout << values[i] << " ";
    }
    cout << endl;

    printHistogram(values, size);

    return 0;
}
```

## Exercises

### Exercise 1: Array Statistics
Write a program that reads 10 integers into an array and computes the sum, average, minimum, and maximum values.

### Exercise 2: Reverse Array
Create a function that reverses an array in place (modifies the original array).

### Exercise 3: Remove Duplicates
Write a function that removes duplicate values from an array and returns the new size.

### Exercise 4: Merge Arrays
Implement a function that merges two sorted arrays into a third sorted array.

### Exercise 5: Rotate Array
Create a function that rotates an array to the right by k positions.

## Exercise Solutions

### Solution 1: Array Statistics

```cpp
#include <iostream>
using namespace std;

int main() {
    const int SIZE = 10;
    int numbers[SIZE];
    int sum = 0, min, max;

    cout << "Enter 10 integers:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << "Number " << (i + 1) << ": ";
        cin >> numbers[i];
    }

    // Initialize min and max with first element
    min = max = numbers[0];
    sum = numbers[0];

    // Process remaining elements
    for (int i = 1; i < SIZE; i++) {
        sum += numbers[i];
        if (numbers[i] < min) min = numbers[i];
        if (numbers[i] > max) max = numbers[i];
    }

    double average = sum / static_cast<double>(SIZE);

    cout << "\nStatistics:" << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << average << endl;
    cout << "Minimum: " << min << endl;
    cout << "Maximum: " << max << endl;

    return 0;
}
```

### Solution 2: Reverse Array

```cpp
#include <iostream>
using namespace std;

void reverseArray(int arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = 10;

    cout << "Original: ";
    printArray(numbers, size);

    reverseArray(numbers, size);

    cout << "Reversed: ";
    printArray(numbers, size);

    return 0;
}
```

### Solution 3: Remove Duplicates

```cpp
#include <iostream>
using namespace std;

int removeDuplicates(int arr[], int size) {
    if (size == 0) return 0;

    int newSize = 1;  // Keep first element

    for (int i = 1; i < size; i++) {
        bool isDuplicate = false;

        // Check if current element is duplicate
        for (int j = 0; j < newSize; j++) {
            if (arr[i] == arr[j]) {
                isDuplicate = true;
                break;
            }
        }

        // If not duplicate, add to result
        if (!isDuplicate) {
            arr[newSize++] = arr[i];
        }
    }

    return newSize;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers[] = {5, 2, 8, 2, 9, 5, 3, 8, 1};
    int size = 9;

    cout << "Original array: ";
    printArray(numbers, size);

    int newSize = removeDuplicates(numbers, size);

    cout << "After removing duplicates: ";
    printArray(numbers, newSize);

    return 0;
}
```

### Solution 4: Merge Arrays

```cpp
#include <iostream>
using namespace std;

void mergeArrays(int arr1[], int size1, int arr2[], int size2, int result[]) {
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

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int arr1[] = {1, 3, 5, 7, 9};
    int arr2[] = {2, 4, 6, 8, 10};
    int size1 = 5, size2 = 5;
    int merged[10];

    mergeArrays(arr1, size1, arr2, size2, merged);

    cout << "Array 1: ";
    printArray(arr1, size1);

    cout << "Array 2: ";
    printArray(arr2, size2);

    cout << "Merged: ";
    printArray(merged, size1 + size2);

    return 0;
}
```

### Solution 5: Rotate Array

```cpp
#include <iostream>
using namespace std;

void rotateRight(int arr[], int size, int k) {
    k = k % size;  // Handle k > size

    // Create temporary array
    int temp[k];

    // Store last k elements
    for (int i = 0; i < k; i++) {
        temp[i] = arr[size - k + i];
    }

    // Shift remaining elements to right
    for (int i = size - 1; i >= k; i--) {
        arr[i] = arr[i - k];
    }

    // Copy temp elements to beginning
    for (int i = 0; i < k; i++) {
        arr[i] = temp[i];
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7};
    int size = 7;

    cout << "Original: ";
    printArray(numbers, size);

    rotateRight(numbers, size, 3);

    cout << "Rotated right by 3: ";
    printArray(numbers, size);

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Arrays** store multiple values of the same type in contiguous memory
- Array indices are **0-based** in C++
- Arrays can be **initialized** in multiple ways
- Arrays are **passed by reference** to functions
- Common array operations: searching, finding min/max, copying
- **Array bounds** are not checked automatically - be careful!
- Always pass **array size** to functions that work with arrays

## Checklist

- [ ] I understand what arrays are and how they work
- [ ] I can declare and initialize arrays
- [ ] I can access and modify array elements
- [ ] I understand array indexing (0-based)
- [ ] I can pass arrays to functions
- [ ] I completed all exercises and understand the solutions
- [ ] I understand common array operations
- [ ] I know the dangers of accessing out-of-bounds indices

## Additional Resources

- **Next Lesson:** Multi-Dimensional Arrays
- **Related Topics:** Pointers and Arrays, Dynamic Arrays, std::array (C++11)
- **Practice:** Implement more array algorithms like sorting and binary search

---

**Congratulations!** You've completed Lesson 16. You now have a solid understanding of arrays in C++, which are fundamental data structures used throughout programming.
