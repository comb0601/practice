# Lesson 18: Array Manipulation

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Sorting Algorithms
2. Searching Algorithms
3. Array Transformations
4. Filtering and Mapping
5. Array Merging and Splitting
6. Array Rotation and Reversal
7. Advanced Array Operations
8. Exercises and Projects

---

## 1. Sorting Algorithms

### Bubble Sort:

```cpp
#include <iostream>

void bubbleSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
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

### Selection Sort:

```cpp
#include <iostream>

void selectionSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        // Swap
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

int main()
{
    int numbers[] = {64, 25, 12, 22, 11};
    int size = 5;

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    selectionSort(numbers, size);

    std::cout << "Sorted: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

### Insertion Sort:

```cpp
#include <iostream>

void insertionSort(int arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;

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

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    insertionSort(numbers, size);

    std::cout << "Sorted: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 2. Searching Algorithms

### Linear Search:

```cpp
#include <iostream>

int linearSearch(int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int countOccurrences(int arr[], int size, int target)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            count++;
        }
    }
    return count;
}

int main()
{
    int numbers[] = {10, 23, 45, 23, 67, 23, 90};
    int size = 7;

    int target = 23;
    int index = linearSearch(numbers, size, target);

    if (index != -1)
    {
        std::cout << "Found " << target << " at index " << index << "\n";
    }
    else
    {
        std::cout << target << " not found\n";
    }

    int count = countOccurrences(numbers, size, target);
    std::cout << target << " appears " << count << " times\n";

    return 0;
}
```

### Binary Search:

```cpp
#include <iostream>

int binarySearch(int arr[], int left, int right, int target)
{
    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return mid;

        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

int main()
{
    int numbers[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int size = 10;

    int target = 23;
    int result = binarySearch(numbers, 0, size - 1, target);

    if (result != -1)
    {
        std::cout << "Found " << target << " at index " << result << "\n";
    }
    else
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

---

## 3. Array Transformations

### Mapping (Transform Each Element):

```cpp
#include <iostream>

void mapArray(int arr[], int size, int (*func)(int))
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = func(arr[i]);
    }
}

int square(int x)
{
    return x * x;
}

int doubleValue(int x)
{
    return x * 2;
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    mapArray(numbers, size, square);

    std::cout << "After squaring: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 4. Filtering and Mapping

### Filtering Arrays:

```cpp
#include <iostream>

int filterArray(int source[], int dest[], int size, bool (*predicate)(int))
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (predicate(source[i]))
        {
            dest[count++] = source[i];
        }
    }
    return count;
}

bool isEven(int x)
{
    return x % 2 == 0;
}

bool isPositive(int x)
{
    return x > 0;
}

int main()
{
    int numbers[] = {-5, 12, -3, 8, 15, -7, 20, 4};
    int size = 8;
    int filtered[100];

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    int evenCount = filterArray(numbers, filtered, size, isEven);
    std::cout << "Even numbers: ";
    for (int i = 0; i < evenCount; i++)
        std::cout << filtered[i] << " ";
    std::cout << "\n";

    int positiveCount = filterArray(numbers, filtered, size, isPositive);
    std::cout << "Positive numbers: ";
    for (int i = 0; i < positiveCount; i++)
        std::cout << filtered[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 5. Array Merging and Splitting

### Merging Arrays:

```cpp
#include <iostream>

void mergeArrays(int arr1[], int size1, int arr2[], int size2, int result[])
{
    for (int i = 0; i < size1; i++)
    {
        result[i] = arr1[i];
    }

    for (int i = 0; i < size2; i++)
    {
        result[size1 + i] = arr2[i];
    }
}

void mergeSortedArrays(int arr1[], int size1, int arr2[], int size2, int result[])
{
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2)
    {
        if (arr1[i] <= arr2[j])
        {
            result[k++] = arr1[i++];
        }
        else
        {
            result[k++] = arr2[j++];
        }
    }

    while (i < size1)
    {
        result[k++] = arr1[i++];
    }

    while (j < size2)
    {
        result[k++] = arr2[j++];
    }
}

int main()
{
    int arr1[] = {1, 3, 5, 7};
    int arr2[] = {2, 4, 6, 8};
    int result[8];

    mergeSortedArrays(arr1, 4, arr2, 4, result);

    std::cout << "Merged sorted array: ";
    for (int i = 0; i < 8; i++)
    {
        std::cout << result[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 6. Array Rotation and Reversal

### Array Rotation:

```cpp
#include <iostream>

void rotateLeft(int arr[], int size, int positions)
{
    positions = positions % size;

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
    int size = 10;

    std::cout << "Original: ";
    printArray(numbers, size);

    int copy1[10], copy2[10];
    for (int i = 0; i < size; i++)
    {
        copy1[i] = copy2[i] = numbers[i];
    }

    rotateLeft(copy1, size, 3);
    std::cout << "Rotate left 3: ";
    printArray(copy1, size);

    rotateRight(copy2, size, 2);
    std::cout << "Rotate right 2: ";
    printArray(copy2, size);

    return 0;
}
```

---

## 7. Advanced Array Operations

### Remove Duplicates:

```cpp
#include <iostream>

int removeDuplicates(int arr[], int size)
{
    if (size == 0 || size == 1)
        return size;

    int temp[100];
    int j = 0;

    for (int i = 0; i < size - 1; i++)
    {
        if (arr[i] != arr[i + 1])
        {
            temp[j++] = arr[i];
        }
    }
    temp[j++] = arr[size - 1];

    for (int i = 0; i < j; i++)
    {
        arr[i] = temp[i];
    }

    return j;
}

int main()
{
    int numbers[] = {1, 1, 2, 2, 3, 4, 4, 5};
    int size = 8;

    std::cout << "Original: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    int newSize = removeDuplicates(numbers, size);

    std::cout << "After removing duplicates: ";
    for (int i = 0; i < newSize; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Practice Project: Array Toolkit

```cpp
#include <iostream>
#include <algorithm>

class ArrayToolkit
{
public:
    static void printArray(int arr[], int size, const char* label = "Array")
    {
        std::cout << label << ": ";
        for (int i = 0; i < size; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
    }

    static void bubbleSort(int arr[], int size)
    {
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = 0; j < size - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    static int binarySearch(int arr[], int size, int target)
    {
        int left = 0, right = size - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (arr[mid] == target)
                return mid;

            if (arr[mid] < target)
                left = mid + 1;
            else
                right = mid - 1;
        }

        return -1;
    }

    static void reverse(int arr[], int size)
    {
        for (int i = 0; i < size / 2; i++)
        {
            std::swap(arr[i], arr[size - 1 - i]);
        }
    }

    static int findMax(int arr[], int size)
    {
        int max = arr[0];
        for (int i = 1; i < size; i++)
        {
            if (arr[i] > max)
                max = arr[i];
        }
        return max;
    }

    static int findMin(int arr[], int size)
    {
        int min = arr[0];
        for (int i = 1; i < size; i++)
        {
            if (arr[i] < min)
                min = arr[i];
        }
        return min;
    }

    static double average(int arr[], int size)
    {
        int sum = 0;
        for (int i = 0; i < size; i++)
        {
            sum += arr[i];
        }
        return static_cast<double>(sum) / size;
    }
};

int main()
{
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    ArrayToolkit::printArray(numbers, size, "Original");

    std::cout << "Max: " << ArrayToolkit::findMax(numbers, size) << "\n";
    std::cout << "Min: " << ArrayToolkit::findMin(numbers, size) << "\n";
    std::cout << "Average: " << ArrayToolkit::average(numbers, size) << "\n\n";

    ArrayToolkit::bubbleSort(numbers, size);
    ArrayToolkit::printArray(numbers, size, "Sorted");

    int target = 22;
    int index = ArrayToolkit::binarySearch(numbers, size, target);
    if (index != -1)
    {
        std::cout << "Found " << target << " at index " << index << "\n";
    }

    ArrayToolkit::reverse(numbers, size);
    ArrayToolkit::printArray(numbers, size, "Reversed");

    return 0;
}
```

---

## Key Takeaways

1. Sorting algorithms organize array elements
2. Searching algorithms find specific values
3. Binary search requires sorted array
4. Transform arrays using mapping functions
5. Filter arrays based on conditions
6. Merge and split arrays as needed
7. Rotation changes element positions
8. Understanding algorithms improves programming skills

---

## Summary Checklist

Before moving to Lesson 19, ensure you can:
- [ ] Implement sorting algorithms
- [ ] Use searching algorithms effectively
- [ ] Transform array elements
- [ ] Filter arrays based on criteria
- [ ] Merge and split arrays
- [ ] Rotate and reverse arrays
- [ ] Remove duplicates from arrays
- [ ] Understand algorithm complexity

---

## Next Lesson Preview

In **Lesson 19**, we'll explore:
- C-style strings (char arrays)
- String manipulation with char arrays
- String functions from cstring
- Character arrays vs std::string
- Null-terminated strings

**Array manipulation mastered!**
