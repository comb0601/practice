# Lesson 05: Arrays and C-Style Strings
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Arrays](#introduction-to-arrays)
2. [Declaring and Initializing Arrays](#declaring-and-initializing-arrays)
3. [Accessing Array Elements](#accessing-array-elements)
4. [Iterating Through Arrays](#iterating-through-arrays)
5. [Multidimensional Arrays](#multidimensional-arrays)
6. [Arrays and Functions](#arrays-and-functions)
7. [C-Style Strings](#c-style-strings)
8. [String Manipulation](#string-manipulation)
9. [Common Array Algorithms](#common-array-algorithms)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to Arrays

An array is a collection of elements of the same type stored in contiguous memory locations.

### Why Use Arrays?

Instead of:
```cpp
int score1 = 85;
int score2 = 92;
int score3 = 78;
int score4 = 95;
int score5 = 88;
```

Use:
```cpp
int scores[5] = {85, 92, 78, 95, 88};
```

### Array Characteristics:
1. **Fixed size**: Size determined at declaration
2. **Same type**: All elements have same data type
3. **Contiguous memory**: Elements stored sequentially
4. **Zero-indexed**: First element at index 0
5. **Fast access**: Direct access by index (O(1))

### Memory Layout

```cpp
int numbers[5] = {10, 20, 30, 40, 50};

// Memory visualization:
// Index:     0    1    2    3    4
// Value:    10   20   30   40   50
// Address: 1000 1004 1008 1012 1016  (assuming 4-byte integers)
```

## Declaring and Initializing Arrays

### Basic Declaration

```cpp
// Declare array of 5 integers (uninitialized)
int numbers[5];

// Declare and initialize
int scores[5] = {85, 92, 78, 95, 88};

// Partial initialization (rest are zero)
int values[5] = {10, 20};  // {10, 20, 0, 0, 0}

// Initialize all to zero
int zeros[5] = {0};  // {0, 0, 0, 0, 0}
int zeros2[5] = {};  // Also all zeros
```

### Size Inference

```cpp
// Compiler infers size from initializer
int numbers[] = {1, 2, 3, 4, 5};  // Size is 5

// This is equivalent to:
int numbers[5] = {1, 2, 3, 4, 5};
```

### Different Data Types

```cpp
#include <iostream>

int main()
{
    // Integer array
    int ages[3] = {25, 30, 35};

    // Double array
    double prices[4] = {19.99, 29.99, 39.99, 49.99};

    // Character array
    char grades[5] = {'A', 'B', 'C', 'D', 'F'};

    // Boolean array
    bool flags[3] = {true, false, true};

    // Display
    std::cout << "First age: " << ages[0] << "\n";
    std::cout << "First price: " << prices[0] << "\n";
    std::cout << "First grade: " << grades[0] << "\n";
    std::cout << "First flag: " << flags[0] << "\n";

    return 0;
}
```

## Accessing Array Elements

### Index Access

```cpp
#include <iostream>

int main()
{
    int numbers[5] = {10, 20, 30, 40, 50};

    // Access individual elements (0-indexed)
    std::cout << "First element: " << numbers[0] << "\n";   // 10
    std::cout << "Third element: " << numbers[2] << "\n";   // 30
    std::cout << "Last element: " << numbers[4] << "\n";    // 50

    // Modify elements
    numbers[0] = 100;
    numbers[2] = 300;

    std::cout << "Modified first: " << numbers[0] << "\n";  // 100
    std::cout << "Modified third: " << numbers[2] << "\n";  // 300

    return 0;
}
```

### Bounds Checking (Manual)

```cpp
#include <iostream>

int main()
{
    const int SIZE = 5;
    int numbers[SIZE] = {10, 20, 30, 40, 50};

    int index;
    std::cout << "Enter index (0-4): ";
    std::cin >> index;

    // Check bounds before accessing
    if (index >= 0 && index < SIZE)
    {
        std::cout << "Value at index " << index << ": " << numbers[index] << "\n";
    }
    else
    {
        std::cout << "Invalid index!\n";
    }

    return 0;
}
```

**Warning**: C++ does NOT automatically check array bounds!

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int x = arr[10];  // UNDEFINED BEHAVIOR! No error at compile time
arr[10] = 100;    // UNDEFINED BEHAVIOR! May crash or corrupt memory
```

## Iterating Through Arrays

### For Loop (Traditional)

```cpp
#include <iostream>

int main()
{
    const int SIZE = 5;
    int numbers[SIZE] = {10, 20, 30, 40, 50};

    // Print all elements
    std::cout << "Array elements:\n";
    for (int i = 0; i < SIZE; i++)
    {
        std::cout << "numbers[" << i << "] = " << numbers[i] << "\n";
    }

    // Calculate sum
    int sum = 0;
    for (int i = 0; i < SIZE; i++)
    {
        sum += numbers[i];
    }
    std::cout << "Sum: " << sum << "\n";

    // Find maximum
    int max = numbers[0];
    for (int i = 1; i < SIZE; i++)
    {
        if (numbers[i] > max)
        {
            max = numbers[i];
        }
    }
    std::cout << "Maximum: " << max << "\n";

    return 0;
}
```

### Range-Based For Loop (C++11)

```cpp
#include <iostream>

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};

    // Read-only access
    std::cout << "Elements:\n";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Modify elements (use reference)
    for (int& num : numbers)
    {
        num *= 2;  // Double each element
    }

    std::cout << "After doubling:\n";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Common Patterns

```cpp
#include <iostream>

int main()
{
    const int SIZE = 10;
    int numbers[SIZE];

    // Fill with sequential values
    for (int i = 0; i < SIZE; i++)
    {
        numbers[i] = i + 1;  // 1, 2, 3, ..., 10
    }

    // Print in reverse
    for (int i = SIZE - 1; i >= 0; i--)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    // Process even indices only
    for (int i = 0; i < SIZE; i += 2)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
```

## Multidimensional Arrays

### 2D Arrays (Matrices)

```cpp
#include <iostream>

int main()
{
    // 2D array: 3 rows, 4 columns
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Access individual elements
    std::cout << "Element at [0][0]: " << matrix[0][0] << "\n";  // 1
    std::cout << "Element at [1][2]: " << matrix[1][2] << "\n";  // 7
    std::cout << "Element at [2][3]: " << matrix[2][3] << "\n";  // 12

    // Print entire matrix
    std::cout << "\nMatrix:\n";
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            std::cout << matrix[row][col] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Memory Layout of 2D Arrays

```cpp
int matrix[3][4];

// Stored in row-major order:
// [0][0] [0][1] [0][2] [0][3] [1][0] [1][1] [1][2] [1][3] [2][0] ...
```

### Initializing 2D Arrays

```cpp
// Full initialization
int a[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
};

// Partial initialization
int b[2][3] = {
    {1, 2},      // Third element is 0
    {4}          // Second and third are 0
};

// Linear initialization (not recommended)
int c[2][3] = {1, 2, 3, 4, 5, 6};

// All zeros
int d[2][3] = {0};
```

### 3D Arrays

```cpp
#include <iostream>

int main()
{
    // 3D array: 2 blocks, 3 rows, 4 columns
    int cube[2][3][4] = {
        {  // Block 0
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}
        },
        {  // Block 1
            {13, 14, 15, 16},
            {17, 18, 19, 20},
            {21, 22, 23, 24}
        }
    };

    // Access element
    std::cout << "cube[1][2][3] = " << cube[1][2][3] << "\n";  // 24

    // Iterate through 3D array
    for (int block = 0; block < 2; block++)
    {
        std::cout << "Block " << block << ":\n";
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                std::cout << cube[block][row][col] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}
```

## Arrays and Functions

### Passing Arrays to Functions

Arrays are **passed by reference** (actually as pointers).

```cpp
#include <iostream>

// Array passed as pointer (size needed separately)
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

// Modify array elements
void doubleValues(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] *= 2;
    }
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    std::cout << "Original: ";
    printArray(numbers, size);

    doubleValues(numbers, size);

    std::cout << "Doubled: ";
    printArray(numbers, size);

    return 0;
}
```

### Const Arrays (Read-Only)

```cpp
#include <iostream>

// Cannot modify array
void printArray(const int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
        // arr[i] = 0;  // ERROR: cannot modify const array
    }
    std::cout << "\n";
}

int sumArray(const int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    printArray(numbers, size);
    std::cout << "Sum: " << sumArray(numbers, size) << "\n";

    return 0;
}
```

### Returning Arrays (Using Static)

```cpp
#include <iostream>

// Return pointer to static array
int* createSequence(int n)
{
    static int sequence[100];  // Static array persists after function returns

    for (int i = 0; i < n && i < 100; i++)
    {
        sequence[i] = i + 1;
    }

    return sequence;
}

int main()
{
    int* seq = createSequence(10);

    for (int i = 0; i < 10; i++)
    {
        std::cout << seq[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Passing 2D Arrays

```cpp
#include <iostream>

// Must specify column size
void print2DArray(int arr[][4], int rows)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void fill2DArray(int arr[][4], int rows, int value)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            arr[i][j] = value;
        }
    }
}

int main()
{
    int matrix[3][4];

    fill2DArray(matrix, 3, 7);
    print2DArray(matrix, 3);

    return 0;
}
```

## C-Style Strings

C-style strings are arrays of characters terminated by '\0' (null character).

### String Basics

```cpp
#include <iostream>

int main()
{
    // Character array with null terminator
    char name[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

    // String literal (automatic null terminator)
    char greeting[] = "Hello";  // Size is 6 (5 chars + '\0')

    // Explicit size
    char message[20] = "Welcome";  // Rest filled with '\0'

    std::cout << name << "\n";
    std::cout << greeting << "\n";
    std::cout << message << "\n";

    return 0;
}
```

### String Memory Layout

```cpp
char str[] = "Hello";

// Memory: 'H' 'e' 'l' 'l' 'o' '\0'
// Index:   0   1   2   3   4   5
```

### Reading Strings

```cpp
#include <iostream>

int main()
{
    char name[50];

    // Read single word (stops at whitespace)
    std::cout << "Enter your first name: ";
    std::cin >> name;
    std::cout << "Hello, " << name << "!\n";

    // Read entire line (including spaces)
    std::cin.ignore();  // Clear newline from buffer
    std::cout << "Enter full name: ";
    std::cin.getline(name, 50);
    std::cout << "Hello, " << name << "!\n";

    return 0;
}
```

### String Library Functions

```cpp
#include <iostream>
#include <cstring>  // For C string functions

int main()
{
    char str1[50] = "Hello";
    char str2[50] = "World";
    char str3[50];

    // String length
    int len = strlen(str1);
    std::cout << "Length of \"" << str1 << "\": " << len << "\n";

    // String copy
    strcpy(str3, str1);
    std::cout << "Copied: " << str3 << "\n";

    // String concatenation
    strcat(str1, " ");
    strcat(str1, str2);
    std::cout << "Concatenated: " << str1 << "\n";

    // String comparison
    if (strcmp(str2, "World") == 0)
    {
        std::cout << "Strings are equal\n";
    }

    return 0;
}
```

**Common String Functions:**
- `strlen(str)` - Get string length
- `strcpy(dest, src)` - Copy string
- `strcat(dest, src)` - Concatenate strings
- `strcmp(str1, str2)` - Compare strings (returns 0 if equal)
- `strchr(str, ch)` - Find character in string
- `strstr(str, substr)` - Find substring

## String Manipulation

### Manual String Operations

```cpp
#include <iostream>

// Get string length manually
int getLength(const char str[])
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// Copy string manually
void copyString(char dest[], const char src[])
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // Don't forget null terminator!
}

// Concatenate strings manually
void concatString(char dest[], const char src[])
{
    int destLen = getLength(dest);
    int i = 0;

    while (src[i] != '\0')
    {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}

// Compare strings manually
int compareStrings(const char str1[], const char str2[])
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] < str2[i])
            return -1;
        if (str1[i] > str2[i])
            return 1;
        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0')
        return 0;
    return (str1[i] == '\0') ? -1 : 1;
}

int main()
{
    char str1[100] = "Hello";
    char str2[100] = "World";
    char str3[100];

    std::cout << "Length: " << getLength(str1) << "\n";

    copyString(str3, str1);
    std::cout << "Copy: " << str3 << "\n";

    concatString(str1, " ");
    concatString(str1, str2);
    std::cout << "Concat: " << str1 << "\n";

    return 0;
}
```

### Character Classification

```cpp
#include <iostream>
#include <cctype>

int main()
{
    char ch = 'A';

    // Character tests
    std::cout << "isalpha('A'): " << isalpha(ch) << "\n";   // Letter
    std::cout << "isdigit('A'): " << isdigit(ch) << "\n";   // Digit
    std::cout << "isalnum('A'): " << isalnum(ch) << "\n";   // Letter or digit
    std::cout << "isupper('A'): " << isupper(ch) << "\n";   // Uppercase
    std::cout << "islower('A'): " << islower(ch) << "\n";   // Lowercase
    std::cout << "isspace(' '): " << isspace(' ') << "\n";  // Whitespace

    // Character conversion
    std::cout << "toupper('a'): " << (char)toupper('a') << "\n";  // A
    std::cout << "tolower('A'): " << (char)tolower('A') << "\n";  // a

    return 0;
}
```

### String Utilities

```cpp
#include <iostream>
#include <cctype>

// Convert string to uppercase
void toUpper(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

// Convert string to lowercase
void toLower(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// Reverse string
void reverse(char str[])
{
    int length = 0;
    while (str[length] != '\0')
        length++;

    int left = 0;
    int right = length - 1;

    while (left < right)
    {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

// Check if palindrome
bool isPalindrome(const char str[])
{
    int length = 0;
    while (str[length] != '\0')
        length++;

    int left = 0;
    int right = length - 1;

    while (left < right)
    {
        if (str[left] != str[right])
            return false;
        left++;
        right--;
    }

    return true;
}

int main()
{
    char text[100] = "Hello World";

    toUpper(text);
    std::cout << "Uppercase: " << text << "\n";

    toLower(text);
    std::cout << "Lowercase: " << text << "\n";

    char word[] = "racecar";
    std::cout << word << " is palindrome: " << (isPalindrome(word) ? "Yes" : "No") << "\n";

    reverse(word);
    std::cout << "Reversed: " << word << "\n";

    return 0;
}
```

## Common Array Algorithms

### Linear Search

```cpp
#include <iostream>

int linearSearch(const int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;  // Return index where found
        }
    }
    return -1;  // Not found
}

int main()
{
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    int target = 22;
    int index = linearSearch(numbers, size, target);

    if (index != -1)
    {
        std::cout << target << " found at index " << index << "\n";
    }
    else
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

### Binary Search (Requires Sorted Array)

```cpp
#include <iostream>

int binarySearch(const int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
        {
            return mid;  // Found
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;  // Search right half
        }
        else
        {
            right = mid - 1;  // Search left half
        }
    }

    return -1;  // Not found
}

int main()
{
    int numbers[] = {11, 12, 22, 25, 34, 64, 90};  // Must be sorted!
    int size = 7;

    int target = 25;
    int index = binarySearch(numbers, size, target);

    if (index != -1)
    {
        std::cout << target << " found at index " << index << "\n";
    }
    else
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

### Bubble Sort

```cpp
#include <iostream>

void bubbleSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        // Flag to optimize (stop if no swaps)
        bool swapped = false;

        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        // If no swaps, array is sorted
        if (!swapped)
            break;
    }
}

void printArray(const int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    std::cout << "Before sorting: ";
    printArray(numbers, size);

    bubbleSort(numbers, size);

    std::cout << "After sorting: ";
    printArray(numbers, size);

    return 0;
}
```

### Selection Sort

```cpp
#include <iostream>

void selectionSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        // Find minimum in remaining unsorted array
        int minIndex = i;
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        // Swap if needed
        if (minIndex != i)
        {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

int main()
{
    int numbers[] = {64, 25, 12, 22, 11};
    int size = 5;

    std::cout << "Before: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    selectionSort(numbers, size);

    std::cout << "After: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

### Insertion Sort

```cpp
#include <iostream>

void insertionSort(int arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int main()
{
    int numbers[] = {12, 11, 13, 5, 6};
    int size = 5;

    std::cout << "Before: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    insertionSort(numbers, size);

    std::cout << "After: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

### Reverse Array

```cpp
#include <iostream>

void reverseArray(int arr[], int size)
{
    int left = 0;
    int right = size - 1;

    while (left < right)
    {
        // Swap elements
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;

        left++;
        right--;
    }
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = 10;

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    reverseArray(numbers, size);

    std::cout << "Reversed: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

## Complete Working Examples

### Example 1: Student Grade Manager

```cpp
#include <iostream>
#include <iomanip>

const int MAX_STUDENTS = 50;

void inputGrades(int grades[], int& count);
void displayGrades(const int grades[], int count);
double calculateAverage(const int grades[], int count);
int findHighest(const int grades[], int count);
int findLowest(const int grades[], int count);
void sortGrades(int grades[], int count);
int countAboveAverage(const int grades[], int count, double avg);

int main()
{
    int grades[MAX_STUDENTS];
    int studentCount = 0;

    std::cout << "=== Student Grade Manager ===\n\n";

    inputGrades(grades, studentCount);

    std::cout << "\n--- Grade Report ---\n";
    displayGrades(grades, studentCount);

    double avg = calculateAverage(grades, studentCount);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nAverage: " << avg << "\n";
    std::cout << "Highest: " << findHighest(grades, studentCount) << "\n";
    std::cout << "Lowest: " << findLowest(grades, studentCount) << "\n";
    std::cout << "Above average: " << countAboveAverage(grades, studentCount, avg) << "\n";

    sortGrades(grades, studentCount);
    std::cout << "\n--- Sorted Grades ---\n";
    displayGrades(grades, studentCount);

    return 0;
}

void inputGrades(int grades[], int& count)
{
    std::cout << "How many students? ";
    std::cin >> count;

    if (count > MAX_STUDENTS)
    {
        count = MAX_STUDENTS;
        std::cout << "Maximum " << MAX_STUDENTS << " students allowed.\n";
    }

    for (int i = 0; i < count; i++)
    {
        do
        {
            std::cout << "Enter grade for student " << (i + 1) << " (0-100): ";
            std::cin >> grades[i];

            if (grades[i] < 0 || grades[i] > 100)
            {
                std::cout << "Invalid grade! Try again.\n";
            }
        } while (grades[i] < 0 || grades[i] > 100);
    }
}

void displayGrades(const int grades[], int count)
{
    for (int i = 0; i < count; i++)
    {
        std::cout << "Student " << (i + 1) << ": " << grades[i] << "\n";
    }
}

double calculateAverage(const int grades[], int count)
{
    if (count == 0)
        return 0.0;

    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += grades[i];
    }

    return static_cast<double>(sum) / count;
}

int findHighest(const int grades[], int count)
{
    if (count == 0)
        return 0;

    int highest = grades[0];
    for (int i = 1; i < count; i++)
    {
        if (grades[i] > highest)
        {
            highest = grades[i];
        }
    }
    return highest;
}

int findLowest(const int grades[], int count)
{
    if (count == 0)
        return 0;

    int lowest = grades[0];
    for (int i = 1; i < count; i++)
    {
        if (grades[i] < lowest)
        {
            lowest = grades[i];
        }
    }
    return lowest;
}

void sortGrades(int grades[], int count)
{
    // Bubble sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (grades[j] > grades[j + 1])
            {
                int temp = grades[j];
                grades[j] = grades[j + 1];
                grades[j + 1] = temp;
            }
        }
    }
}

int countAboveAverage(const int grades[], int count, double avg)
{
    int countAbove = 0;
    for (int i = 0; i < count; i++)
    {
        if (grades[i] > avg)
        {
            countAbove++;
        }
    }
    return countAbove;
}
```

### Example 2: Matrix Operations

```cpp
#include <iostream>

const int MAX_SIZE = 10;

void inputMatrix(int matrix[][MAX_SIZE], int& rows, int& cols);
void displayMatrix(const int matrix[][MAX_SIZE], int rows, int cols);
void addMatrices(const int a[][MAX_SIZE], const int b[][MAX_SIZE],
                int result[][MAX_SIZE], int rows, int cols);
void multiplyMatrices(const int a[][MAX_SIZE], int aRows, int aCols,
                     const int b[][MAX_SIZE], int bRows, int bCols,
                     int result[][MAX_SIZE]);
void transposeMatrix(const int matrix[][MAX_SIZE], int result[][MAX_SIZE],
                    int rows, int cols);
int sumDiagonal(const int matrix[][MAX_SIZE], int size);

int main()
{
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    std::cout << "=== Matrix Operations ===\n\n";

    std::cout << "Enter first matrix:\n";
    inputMatrix(matrix1, rows, cols);

    std::cout << "\nEnter second matrix (same size):\n";
    int rows2, cols2;
    inputMatrix(matrix2, rows2, cols2);

    if (rows != rows2 || cols != cols2)
    {
        std::cout << "Matrices must have same dimensions!\n";
        return 1;
    }

    std::cout << "\nMatrix 1:\n";
    displayMatrix(matrix1, rows, cols);

    std::cout << "\nMatrix 2:\n";
    displayMatrix(matrix2, rows, cols);

    // Addition
    addMatrices(matrix1, matrix2, result, rows, cols);
    std::cout << "\nSum:\n";
    displayMatrix(result, rows, cols);

    // Transpose
    int transposed[MAX_SIZE][MAX_SIZE];
    transposeMatrix(matrix1, transposed, rows, cols);
    std::cout << "\nTranspose of Matrix 1:\n";
    displayMatrix(transposed, cols, rows);

    // Diagonal sum (if square)
    if (rows == cols)
    {
        std::cout << "\nDiagonal sum of Matrix 1: "
                  << sumDiagonal(matrix1, rows) << "\n";
    }

    return 0;
}

void inputMatrix(int matrix[][MAX_SIZE], int& rows, int& cols)
{
    std::cout << "Enter rows and columns: ";
    std::cin >> rows >> cols;

    std::cout << "Enter elements:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << "Element [" << i << "][" << j << "]: ";
            std::cin >> matrix[i][j];
        }
    }
}

void displayMatrix(const int matrix[][MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void addMatrices(const int a[][MAX_SIZE], const int b[][MAX_SIZE],
                int result[][MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void transposeMatrix(const int matrix[][MAX_SIZE], int result[][MAX_SIZE],
                    int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = matrix[i][j];
        }
    }
}

int sumDiagonal(const int matrix[][MAX_SIZE], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += matrix[i][i];
    }
    return sum;
}
```

### Example 3: Text Analyzer

```cpp
#include <iostream>
#include <cctype>
#include <cstring>

const int MAX_LENGTH = 1000;

int countWords(const char text[]);
int countSentences(const char text[]);
int countVowels(const char text[]);
int countConsonants(const char text[]);
void countLetterFrequency(const char text[], int freq[]);
void displayFrequency(const int freq[]);

int main()
{
    char text[MAX_LENGTH];

    std::cout << "=== Text Analyzer ===\n\n";
    std::cout << "Enter text (max 999 chars):\n";
    std::cin.getline(text, MAX_LENGTH);

    std::cout << "\n--- Analysis Results ---\n";
    std::cout << "Characters: " << strlen(text) << "\n";
    std::cout << "Words: " << countWords(text) << "\n";
    std::cout << "Sentences: " << countSentences(text) << "\n";
    std::cout << "Vowels: " << countVowels(text) << "\n";
    std::cout << "Consonants: " << countConsonants(text) << "\n";

    int frequency[26] = {0};
    countLetterFrequency(text, frequency);
    std::cout << "\n--- Letter Frequency ---\n";
    displayFrequency(frequency);

    return 0;
}

int countWords(const char text[])
{
    int count = 0;
    bool inWord = false;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            if (!inWord)
            {
                count++;
                inWord = true;
            }
        }
        else
        {
            inWord = false;
        }
    }

    return count;
}

int countSentences(const char text[])
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}

int countVowels(const char text[])
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char ch = tolower(text[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
        {
            count++;
        }
    }

    return count;
}

int countConsonants(const char text[])
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char ch = tolower(text[i]);
        if (isalpha(ch))
        {
            if (ch != 'a' && ch != 'e' && ch != 'i' && ch != 'o' && ch != 'u')
            {
                count++;
            }
        }
    }

    return count;
}

void countLetterFrequency(const char text[], int freq[])
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        char ch = tolower(text[i]);
        if (ch >= 'a' && ch <= 'z')
        {
            freq[ch - 'a']++;
        }
    }
}

void displayFrequency(const int freq[])
{
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > 0)
        {
            std::cout << (char)('A' + i) << ": " << freq[i] << "\n";
        }
    }
}
```

### Example 4: Tic-Tac-Toe Game

```cpp
#include <iostream>

const int SIZE = 3;

void initializeBoard(char board[][SIZE]);
void displayBoard(const char board[][SIZE]);
bool makeMove(char board[][SIZE], int row, int col, char player);
bool checkWin(const char board[][SIZE], char player);
bool checkDraw(const char board[][SIZE]);
bool isBoardFull(const char board[][SIZE]);

int main()
{
    char board[SIZE][SIZE];
    char currentPlayer = 'X';
    int row, col;

    initializeBoard(board);

    std::cout << "=== Tic-Tac-Toe ===\n\n";

    while (true)
    {
        displayBoard(board);

        std::cout << "\nPlayer " << currentPlayer << "'s turn\n";
        std::cout << "Enter row (0-2): ";
        std::cin >> row;
        std::cout << "Enter column (0-2): ";
        std::cin >> col;

        if (!makeMove(board, row, col, currentPlayer))
        {
            std::cout << "Invalid move! Try again.\n";
            continue;
        }

        if (checkWin(board, currentPlayer))
        {
            displayBoard(board);
            std::cout << "\n*** Player " << currentPlayer << " wins! ***\n";
            break;
        }

        if (checkDraw(board))
        {
            displayBoard(board);
            std::cout << "\n*** It's a draw! ***\n";
            break;
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;
}

void initializeBoard(char board[][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void displayBoard(const char board[][SIZE])
{
    std::cout << "\n";
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            std::cout << " " << board[i][j];
            if (j < SIZE - 1)
                std::cout << " |";
        }
        std::cout << "\n";
        if (i < SIZE - 1)
            std::cout << "---|---|---\n";
    }
    std::cout << "\n";
}

bool makeMove(char board[][SIZE], int row, int col, char player)
{
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE)
        return false;

    if (board[row][col] != ' ')
        return false;

    board[row][col] = player;
    return true;
}

bool checkWin(const char board[][SIZE], char player)
{
    // Check rows
    for (int i = 0; i < SIZE; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
    }

    // Check columns
    for (int j = 0; j < SIZE; j++)
    {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
            return true;
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;

    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

bool checkDraw(const char board[][SIZE])
{
    return isBoardFull(board) && !checkWin(board, 'X') && !checkWin(board, 'O');
}

bool isBoardFull(const char board[][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}
```

### Example 5: Inventory Management System

```cpp
#include <iostream>
#include <cstring>
#include <iomanip>

const int MAX_ITEMS = 100;
const int NAME_LENGTH = 50;

struct Item
{
    char name[NAME_LENGTH];
    int quantity;
    double price;
};

void addItem(Item inventory[], int& count);
void displayInventory(const Item inventory[], int count);
void searchItem(const Item inventory[], int count);
void updateQuantity(Item inventory[], int count);
double calculateTotalValue(const Item inventory[], int count);
void sortByName(Item inventory[], int count);

int main()
{
    Item inventory[MAX_ITEMS];
    int itemCount = 0;
    int choice;

    std::cout << "=== Inventory Management System ===\n";

    do
    {
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Add Item\n";
        std::cout << "2. Display All Items\n";
        std::cout << "3. Search Item\n";
        std::cout << "4. Update Quantity\n";
        std::cout << "5. Show Total Value\n";
        std::cout << "6. Sort by Name\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                addItem(inventory, itemCount);
                break;
            case 2:
                displayInventory(inventory, itemCount);
                break;
            case 3:
                searchItem(inventory, itemCount);
                break;
            case 4:
                updateQuantity(inventory, itemCount);
                break;
            case 5:
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "\nTotal Inventory Value: $"
                          << calculateTotalValue(inventory, itemCount) << "\n";
                break;
            case 6:
                sortByName(inventory, itemCount);
                std::cout << "Inventory sorted by name.\n";
                break;
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}

void addItem(Item inventory[], int& count)
{
    if (count >= MAX_ITEMS)
    {
        std::cout << "Inventory full!\n";
        return;
    }

    std::cin.ignore();
    std::cout << "Enter item name: ";
    std::cin.getline(inventory[count].name, NAME_LENGTH);

    std::cout << "Enter quantity: ";
    std::cin >> inventory[count].quantity;

    std::cout << "Enter price: $";
    std::cin >> inventory[count].price;

    count++;
    std::cout << "Item added successfully!\n";
}

void displayInventory(const Item inventory[], int count)
{
    if (count == 0)
    {
        std::cout << "\nInventory is empty.\n";
        return;
    }

    std::cout << "\n--- Inventory ---\n";
    std::cout << std::left << std::setw(30) << "Name"
              << std::setw(10) << "Quantity"
              << std::setw(10) << "Price"
              << std::setw(10) << "Value" << "\n";
    std::cout << std::string(60, '-') << "\n";

    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < count; i++)
    {
        double value = inventory[i].quantity * inventory[i].price;
        std::cout << std::left << std::setw(30) << inventory[i].name
                  << std::setw(10) << inventory[i].quantity
                  << "$" << std::setw(9) << inventory[i].price
                  << "$" << value << "\n";
    }
}

void searchItem(const Item inventory[], int count)
{
    char searchName[NAME_LENGTH];

    std::cin.ignore();
    std::cout << "Enter item name to search: ";
    std::cin.getline(searchName, NAME_LENGTH);

    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(inventory[i].name, searchName) == 0)
        {
            std::cout << "\nItem found:\n";
            std::cout << "Name: " << inventory[i].name << "\n";
            std::cout << "Quantity: " << inventory[i].quantity << "\n";
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Price: $" << inventory[i].price << "\n";
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Item not found.\n";
    }
}

void updateQuantity(Item inventory[], int count)
{
    char searchName[NAME_LENGTH];

    std::cin.ignore();
    std::cout << "Enter item name: ";
    std::cin.getline(searchName, NAME_LENGTH);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(inventory[i].name, searchName) == 0)
        {
            std::cout << "Current quantity: " << inventory[i].quantity << "\n";
            std::cout << "Enter new quantity: ";
            std::cin >> inventory[i].quantity;
            std::cout << "Quantity updated!\n";
            return;
        }
    }

    std::cout << "Item not found.\n";
}

double calculateTotalValue(const Item inventory[], int count)
{
    double total = 0.0;
    for (int i = 0; i < count; i++)
    {
        total += inventory[i].quantity * inventory[i].price;
    }
    return total;
}

void sortByName(Item inventory[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(inventory[j].name, inventory[j + 1].name) > 0)
            {
                Item temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
}
```

## Common Mistakes

### 1. Array Out of Bounds

```cpp
// WRONG - accessing beyond array bounds
int arr[5] = {1, 2, 3, 4, 5};
int x = arr[5];   // ERROR: index 5 doesn't exist (valid: 0-4)
arr[10] = 100;    // ERROR: undefined behavior

// CORRECT - check bounds
const int SIZE = 5;
int arr[SIZE] = {1, 2, 3, 4, 5};
int index = 3;
if (index >= 0 && index < SIZE)
{
    int x = arr[index];  // Safe
}
```

### 2. Forgetting Null Terminator

```cpp
// WRONG - no null terminator
char str[5] = {'H', 'e', 'l', 'l', 'o'};
std::cout << str;  // Undefined behavior - no '\0'

// CORRECT
char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
// OR
char str[] = "Hello";  // Automatic null terminator
```

### 3. Array Size Mismatch

```cpp
// WRONG - destination too small
char dest[5];
char src[] = "Hello World";  // 12 characters + '\0'
strcpy(dest, src);  // Buffer overflow!

// CORRECT - ensure destination is large enough
char dest[20];
char src[] = "Hello World";
strcpy(dest, src);  // Safe
```

### 4. Not Passing Array Size

```cpp
// WRONG - function doesn't know array size
void printArray(int arr[])
{
    // How many elements? Unknown!
    for (int i = 0; i < ???; i++)  // Don't know when to stop
    {
        std::cout << arr[i] << " ";
    }
}

// CORRECT - pass size separately
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
}
```

### 5. Comparing Arrays with ==

```cpp
// WRONG - compares pointers, not contents
int arr1[] = {1, 2, 3};
int arr2[] = {1, 2, 3};
if (arr1 == arr2)  // Always false - different addresses
{
    // Never executes
}

// CORRECT - compare element by element
bool arraysEqual(const int a[], const int b[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}
```

### 6. Returning Local Array

```cpp
// WRONG - returning pointer to local array
int* createArray()
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    return arr;  // ERROR: arr is destroyed when function returns
}

// CORRECT - use static or dynamic allocation
int* createArray()
{
    static int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    return arr;  // OK: static persists
}
// (We'll learn about dynamic allocation in later lessons)
```

## Exercises

### Exercise 1: Array Statistics Extended
Write a program that:
1. Accepts an array of integers from user
2. Calculates and displays:
   - Mean, median, mode
   - Standard deviation
   - Range (max - min)
   - Sorted array (ascending and descending)
3. Find the kth largest/smallest element

### Exercise 2: String Cipher
Implement a Caesar cipher program that:
1. Encrypts a string by shifting each letter by n positions
2. Decrypts an encrypted string
3. Handles both uppercase and lowercase
4. Preserves spaces and punctuation
5. Allows user to specify shift amount

Example: "Hello" with shift 3 → "Khoor"

### Exercise 3: 2D Game Board
Create a simple dungeon crawler:
1. 10x10 grid representing a dungeon
2. Player can move (W/A/S/D)
3. Random placement of:
   - Treasures (T)
   - Enemies (E)
   - Walls (#)
4. Display board after each move
5. Track score and health

### Exercise 4: Contact List
Build a contact management system:
1. Store up to 50 contacts (name, phone, email)
2. Functions:
   - Add contact
   - Search by name
   - Delete contact
   - Sort alphabetically
   - Display all
3. Use C-style strings for all text data

### Exercise 5: Matrix Calculator
Implement comprehensive matrix operations:
1. Matrix addition and subtraction
2. Matrix multiplication
3. Transpose
4. Determinant (2x2 and 3x3)
5. Identity matrix generation
6. Scalar multiplication
7. Check if matrix is symmetric

## Summary

You learned:
- ✅ Array declaration, initialization, and access
- ✅ Iterating through arrays with loops
- ✅ Multidimensional arrays (2D, 3D)
- ✅ Passing arrays to functions
- ✅ C-style strings and null terminators
- ✅ String manipulation functions
- ✅ Common array algorithms (searching, sorting)
- ✅ Real-world applications with complete examples

## Next Lesson Preview

In Lesson 06, we'll explore:
- Pointers: memory addresses and indirection
- Pointer arithmetic and array relationship
- References and their differences from pointers
- Pointer to pointer (double pointers)
- Const pointers and pointer to const
- Common pointer pitfalls and best practices

---

**Congratulations on completing Lesson 05!**

**Practice time**: Spend 4+ hours implementing the exercises and creating your own array-based programs. Try building games, data analysis tools, or text processing utilities to solidify your understanding.
