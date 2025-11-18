# Lesson 16: Arrays Basics

**Duration**: 5 hours
**Difficulty**: Beginner to Intermediate

## Table of Contents
1. Introduction to Arrays
2. Array Declaration and Initialization
3. Accessing Array Elements
4. Array Size and Bounds
5. Arrays and Loops
6. Arrays and Functions
7. Common Array Operations
8. Exercises and Projects

---

## 1. Introduction to Arrays

### What is an Array?

An array is a collection of elements of the same type stored in contiguous memory locations. Arrays provide a way to store multiple values under a single variable name.

```cpp
#include <iostream>

int main()
{
    // Instead of declaring multiple variables:
    // int score1, score2, score3, score4, score5;

    // Use an array:
    int scores[5];  // Array of 5 integers

    // Assign values
    scores[0] = 85;
    scores[1] = 92;
    scores[2] = 78;
    scores[3] = 95;
    scores[4] = 88;

    // Access and display
    std::cout << "Scores:\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "Score " << i + 1 << ": " << scores[i] << "\n";
    }

    return 0;
}
```

**Output**:
```
Scores:
Score 1: 85
Score 2: 92
Score 3: 78
Score 4: 95
Score 5: 88
```

### Array Memory Layout:

```cpp
#include <iostream>

int main()
{
    int numbers[5] = {10, 20, 30, 40, 50};

    std::cout << "Array elements and their addresses:\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "numbers[" << i << "] = " << numbers[i]
                  << " at address " << &numbers[i] << "\n";
    }

    std::cout << "\nArray size: " << sizeof(numbers) << " bytes\n";
    std::cout << "Element size: " << sizeof(numbers[0]) << " bytes\n";
    std::cout << "Number of elements: " << sizeof(numbers) / sizeof(numbers[0]) << "\n";

    return 0;
}
```

---

## 2. Array Declaration and Initialization

### Declaration Syntax:

```cpp
#include <iostream>

int main()
{
    // Declaration with size
    int numbers[5];

    // Declaration with initialization
    int primes[5] = {2, 3, 5, 7, 11};

    // Partial initialization (rest are zero)
    int values[10] = {1, 2, 3};  // Last 7 elements are 0

    // Size inferred from initializer
    int fibonacci[] = {1, 1, 2, 3, 5, 8, 13};

    // Display arrays
    std::cout << "Primes: ";
    for (int i = 0; i < 5; i++)
        std::cout << primes[i] << " ";
    std::cout << "\n";

    std::cout << "Values: ";
    for (int i = 0; i < 10; i++)
        std::cout << values[i] << " ";
    std::cout << "\n";

    std::cout << "Fibonacci: ";
    for (int i = 0; i < 7; i++)
        std::cout << fibonacci[i] << " ";
    std::cout << "\n";

    return 0;
}
```

### Different Data Types:

```cpp
#include <iostream>
#include <string>

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

    // String array (C++ string objects)
    std::string names[3] = {"Alice", "Bob", "Charlie"};

    // Display
    std::cout << "Ages: ";
    for (int i = 0; i < 3; i++)
        std::cout << ages[i] << " ";
    std::cout << "\n";

    std::cout << "Prices: ";
    for (int i = 0; i < 4; i++)
        std::cout << "$" << prices[i] << " ";
    std::cout << "\n";

    std::cout << "Grades: ";
    for (int i = 0; i < 5; i++)
        std::cout << grades[i] << " ";
    std::cout << "\n";

    std::cout << "Names: ";
    for (int i = 0; i < 3; i++)
        std::cout << names[i] << " ";
    std::cout << "\n";

    return 0;
}
```

### Modern C++ Initialization:

```cpp
#include <iostream>

int main()
{
    // Uniform initialization (C++11)
    int numbers{10, 20, 30, 40, 50};

    // Zero initialization
    int zeros[100] = {};  // All elements are 0

    // Array of all same value
    int fives[5];
    for (int& val : fives)
        val = 5;

    std::cout << "Numbers: ";
    for (int num : numbers)
        std::cout << num << " ";
    std::cout << "\n";

    std::cout << "First 10 zeros: ";
    for (int i = 0; i < 10; i++)
        std::cout << zeros[i] << " ";
    std::cout << "\n";

    std::cout << "Fives: ";
    for (int val : fives)
        std::cout << val << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 3. Accessing Array Elements

### Index-Based Access:

```cpp
#include <iostream>

int main()
{
    int numbers[5] = {10, 20, 30, 40, 50};

    // Access individual elements
    std::cout << "First element: " << numbers[0] << "\n";
    std::cout << "Third element: " << numbers[2] << "\n";
    std::cout << "Last element: " << numbers[4] << "\n\n";

    // Modify elements
    numbers[0] = 100;
    numbers[2] = 300;

    std::cout << "After modification:\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "numbers[" << i << "] = " << numbers[i] << "\n";
    }

    // Use element in expression
    int sum = numbers[0] + numbers[1];
    std::cout << "\nSum of first two: " << sum << "\n";

    // Increment element
    numbers[3]++;
    std::cout << "Fourth element incremented: " << numbers[3] << "\n";

    return 0;
}
```

### Using Variables as Indices:

```cpp
#include <iostream>

int main()
{
    int scores[5] = {85, 92, 78, 95, 88};

    // Access using variable
    int index = 2;
    std::cout << "Score at index " << index << ": " << scores[index] << "\n\n";

    // Find and display maximum score
    int maxIndex = 0;
    for (int i = 1; i < 5; i++)
    {
        if (scores[i] > scores[maxIndex])
        {
            maxIndex = i;
        }
    }

    std::cout << "Highest score: " << scores[maxIndex]
              << " at position " << maxIndex + 1 << "\n";

    // Interactive access
    std::cout << "\nEnter an index (0-4): ";
    int userIndex;
    std::cin >> userIndex;

    if (userIndex >= 0 && userIndex < 5)
    {
        std::cout << "Score at index " << userIndex << ": "
                  << scores[userIndex] << "\n";
    }
    else
    {
        std::cout << "Invalid index!\n";
    }

    return 0;
}
```

---

## 4. Array Size and Bounds

### Getting Array Size:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};

    // Calculate size
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Array size: " << size << " elements\n";
    std::cout << "Total bytes: " << sizeof(numbers) << "\n";
    std::cout << "Bytes per element: " << sizeof(numbers[0]) << "\n\n";

    // Use size in loop
    for (int i = 0; i < size; i++)
    {
        std::cout << "numbers[" << i << "] = " << numbers[i] << "\n";
    }

    return 0;
}
```

### Bounds Checking:

```cpp
#include <iostream>

void safeAccess(int arr[], int size, int index)
{
    if (index >= 0 && index < size)
    {
        std::cout << "arr[" << index << "] = " << arr[index] << "\n";
    }
    else
    {
        std::cout << "Error: Index " << index << " is out of bounds!\n";
    }
}

int main()
{
    int data[5] = {10, 20, 30, 40, 50};
    int size = sizeof(data) / sizeof(data[0]);

    // Safe access
    safeAccess(data, size, 2);   // Valid
    safeAccess(data, size, 10);  // Out of bounds
    safeAccess(data, size, -1);  // Out of bounds

    return 0;
}
```

### Using Constants for Size:

```cpp
#include <iostream>

const int MAX_STUDENTS = 100;

int main()
{
    int scores[MAX_STUDENTS];
    int actualStudents = 5;

    // Initialize some students
    scores[0] = 85;
    scores[1] = 92;
    scores[2] = 78;
    scores[3] = 95;
    scores[4] = 88;

    std::cout << "Scores for " << actualStudents << " students:\n";
    for (int i = 0; i < actualStudents; i++)
    {
        std::cout << "Student " << (i + 1) << ": " << scores[i] << "\n";
    }

    return 0;
}
```

---

## 5. Arrays and Loops

### For Loop with Arrays:

```cpp
#include <iostream>

int main()
{
    const int SIZE = 10;
    int squares[SIZE];

    // Fill array
    for (int i = 0; i < SIZE; i++)
    {
        squares[i] = (i + 1) * (i + 1);
    }

    // Display array
    std::cout << "Squares of 1-10:\n";
    for (int i = 0; i < SIZE; i++)
    {
        std::cout << (i + 1) << "² = " << squares[i] << "\n";
    }

    return 0;
}
```

### Range-Based For Loop:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};

    // Read-only access
    std::cout << "Elements: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Modifiable access (reference)
    for (int& num : numbers)
    {
        num *= 2;  // Double each element
    }

    std::cout << "After doubling: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Searching in Arrays:

```cpp
#include <iostream>

int linearSearch(int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;  // Found at index i
        }
    }
    return -1;  // Not found
}

int main()
{
    int numbers[] = {23, 45, 12, 67, 89, 34, 56};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    int target = 67;
    int index = linearSearch(numbers, size, target);

    if (index != -1)
    {
        std::cout << "Found " << target << " at index " << index << "\n";
    }
    else
    {
        std::cout << target << " not found in array\n";
    }

    return 0;
}
```

---

## 6. Arrays and Functions

### Passing Arrays to Functions:

```cpp
#include <iostream>

// Array decays to pointer when passed to function
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

void modifyArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] *= 2;
    }
}

double calculateAverage(int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    modifyArray(numbers, size);
    std::cout << "After doubling: ";
    printArray(numbers, size);

    double avg = calculateAverage(numbers, size);
    std::cout << "Average: " << avg << "\n";

    return 0;
}
```

### Returning Arrays (Using Pointers):

```cpp
#include <iostream>

// Note: Don't return pointer to local array!
// This example shows the concept, but is unsafe
int* createArray(int size)
{
    static int arr[100];  // Static to avoid deallocation
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }
    return arr;
}

// Better: Modify array passed by caller
void fillArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }
}

int main()
{
    int myArray[10];
    fillArray(myArray, 10);

    std::cout << "Filled array: ";
    for (int i = 0; i < 10; i++)
    {
        std::cout << myArray[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 7. Common Array Operations

### Finding Min/Max:

```cpp
#include <iostream>

int findMin(int arr[], int size)
{
    int min = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

int findMax(int arr[], int size)
{
    int max = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

int main()
{
    int numbers[] = {45, 23, 67, 12, 89, 34};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Minimum: " << findMin(numbers, size) << "\n";
    std::cout << "Maximum: " << findMax(numbers, size) << "\n";

    return 0;
}
```

### Reversing an Array:

```cpp
#include <iostream>

void reverseArray(int arr[], int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    reverseArray(numbers, size);

    std::cout << "Reversed: ";
    printArray(numbers, size);

    return 0;
}
```

### Copying Arrays:

```cpp
#include <iostream>

void copyArray(int source[], int dest[], int size)
{
    for (int i = 0; i < size; i++)
    {
        dest[i] = source[i];
    }
}

int main()
{
    int original[] = {10, 20, 30, 40, 50};
    int copy[5];
    int size = 5;

    copyArray(original, copy, size);

    // Modify copy
    copy[0] = 999;

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << original[i] << " ";
    std::cout << "\n";

    std::cout << "Copy: ";
    for (int i = 0; i < size; i++)
        std::cout << copy[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Array Statistics

```cpp
#include <iostream>
#include <cmath>

struct Statistics
{
    int min;
    int max;
    double average;
    double median;
    int sum;
};

Statistics calculateStats(int arr[], int size)
{
    Statistics stats;

    // Min and Max
    stats.min = stats.max = arr[0];
    stats.sum = arr[0];

    for (int i = 1; i < size; i++)
    {
        if (arr[i] < stats.min) stats.min = arr[i];
        if (arr[i] > stats.max) stats.max = arr[i];
        stats.sum += arr[i];
    }

    // Average
    stats.average = static_cast<double>(stats.sum) / size;

    // Median (need to sort first - simple bubble sort)
    int temp[100];
    for (int i = 0; i < size; i++)
        temp[i] = arr[i];

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (temp[j] > temp[j + 1])
            {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    if (size % 2 == 0)
        stats.median = (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
    else
        stats.median = temp[size / 2];

    return stats;
}

int main()
{
    int numbers[] = {45, 23, 67, 12, 89, 34, 56, 78, 90, 11};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Array: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n\n";

    Statistics stats = calculateStats(numbers, size);

    std::cout << "=== STATISTICS ===\n";
    std::cout << "Minimum: " << stats.min << "\n";
    std::cout << "Maximum: " << stats.max << "\n";
    std::cout << "Sum: " << stats.sum << "\n";
    std::cout << "Average: " << stats.average << "\n";
    std::cout << "Median: " << stats.median << "\n";

    return 0;
}
```

### Exercise 2: Array Manipulation

```cpp
#include <iostream>

void rotateLeft(int arr[], int size, int positions)
{
    positions = positions % size;  // Handle rotations > size

    for (int p = 0; p < positions; p++)
    {
        int first = arr[0];
        for (int i = 0; i < size - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        arr[size - 1] = first;
    }
}

void rotateRight(int arr[], int size, int positions)
{
    positions = positions % size;

    for (int p = 0; p < positions; p++)
    {
        int last = arr[size - 1];
        for (int i = size - 1; i > 0; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[0] = last;
    }
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Original: ";
    printArray(numbers, size);

    rotateLeft(numbers, size, 3);
    std::cout << "After rotating left 3: ";
    printArray(numbers, size);

    rotateRight(numbers, size, 5);
    std::cout << "After rotating right 5: ";
    printArray(numbers, size);

    return 0;
}
```

---

## Practice Project: Grade Book System

```cpp
#include <iostream>
#include <string>
#include <iomanip>

const int MAX_STUDENTS = 50;

class GradeBook
{
private:
    std::string studentNames[MAX_STUDENTS];
    int grades[MAX_STUDENTS];
    int studentCount;

public:
    GradeBook() : studentCount(0) {}

    void addStudent(const std::string& name, int grade)
    {
        if (studentCount < MAX_STUDENTS)
        {
            studentNames[studentCount] = name;
            grades[studentCount] = grade;
            studentCount++;
            std::cout << "Student added successfully!\n";
        }
        else
        {
            std::cout << "Grade book is full!\n";
        }
    }

    void displayAll()
    {
        std::cout << "\n=== GRADE BOOK ===\n";
        std::cout << std::left << std::setw(20) << "Name"
                  << std::setw(10) << "Grade"
                  << "Letter\n";
        std::cout << std::string(40, '-') << "\n";

        for (int i = 0; i < studentCount; i++)
        {
            std::cout << std::left << std::setw(20) << studentNames[i]
                     << std::setw(10) << grades[i]
                     << getLetterGrade(grades[i]) << "\n";
        }
    }

    double getAverage()
    {
        if (studentCount == 0)
            return 0.0;

        int sum = 0;
        for (int i = 0; i < studentCount; i++)
        {
            sum += grades[i];
        }
        return static_cast<double>(sum) / studentCount;
    }

    int getHighestGrade()
    {
        if (studentCount == 0)
            return 0;

        int max = grades[0];
        for (int i = 1; i < studentCount; i++)
        {
            if (grades[i] > max)
                max = grades[i];
        }
        return max;
    }

    int getLowestGrade()
    {
        if (studentCount == 0)
            return 0;

        int min = grades[0];
        for (int i = 1; i < studentCount; i++)
        {
            if (grades[i] < min)
                min = grades[i];
        }
        return min;
    }

    char getLetterGrade(int grade)
    {
        if (grade >= 90) return 'A';
        if (grade >= 80) return 'B';
        if (grade >= 70) return 'C';
        if (grade >= 60) return 'D';
        return 'F';
    }

    void displayStatistics()
    {
        std::cout << "\n=== STATISTICS ===\n";
        std::cout << "Total students: " << studentCount << "\n";
        std::cout << "Average grade: " << std::fixed << std::setprecision(2)
                  << getAverage() << "\n";
        std::cout << "Highest grade: " << getHighestGrade() << "\n";
        std::cout << "Lowest grade: " << getLowestGrade() << "\n";
    }
};

int main()
{
    GradeBook gradeBook;

    gradeBook.addStudent("Alice Johnson", 95);
    gradeBook.addStudent("Bob Smith", 87);
    gradeBook.addStudent("Charlie Brown", 92);
    gradeBook.addStudent("Diana Prince", 78);
    gradeBook.addStudent("Eve Wilson", 85);

    gradeBook.displayAll();
    gradeBook.displayStatistics();

    return 0;
}
```

---

## Key Takeaways

1. Arrays store multiple values of the same type
2. Array indices start at 0
3. Array size must be known at compile time
4. Arrays decay to pointers when passed to functions
5. Always pass size along with array to functions
6. Check bounds to avoid undefined behavior
7. Use const for array sizes
8. Range-based for loops are convenient for arrays

---

## Summary Checklist

Before moving to Lesson 17, ensure you can:
- [ ] Declare and initialize arrays
- [ ] Access array elements using indices
- [ ] Calculate array size
- [ ] Pass arrays to functions
- [ ] Perform common array operations
- [ ] Understand array bounds and safety
- [ ] Use loops with arrays
- [ ] Implement basic array algorithms

---

## Next Lesson Preview

In **Lesson 17**, we'll explore:
- Two-dimensional arrays
- Multidimensional array declaration
- Accessing 2D array elements
- Matrices and matrix operations
- Practical applications of multidimensional arrays

**Arrays mastered - multidimensional arrays next!**
