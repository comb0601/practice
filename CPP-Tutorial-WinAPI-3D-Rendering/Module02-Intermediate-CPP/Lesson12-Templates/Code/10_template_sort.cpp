/**
 * Template Sort Functions
 * Generic sorting algorithms using templates
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Bubble sort template
template <typename T>
void bubbleSort(T* arr, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Selection sort template
template <typename T>
void selectionSort(T* arr, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            T temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

// Insertion sort template
template <typename T>
void insertionSort(T* arr, size_t size) {
    for (size_t i = 1; i < size; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Quick sort helper
template <typename T>
int partition(T* arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    T temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

// Quick sort template
template <typename T>
void quickSort(T* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

template <typename T>
void quickSort(T* arr, size_t size) {
    if (size > 1) {
        quickSort(arr, 0, size - 1);
    }
}

// Vector sort template
template <typename T>
void sortVector(vector<T>& vec) {
    sort(vec.begin(), vec.end());
}

// Custom comparison function template
template <typename T>
void sortDescending(T* arr, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {  // Note: reversed comparison
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Print array template
template <typename T>
void printArray(const T* arr, size_t size) {
    cout << "[";
    for (size_t i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print vector template
template <typename T>
void printVector(const vector<T>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Check if sorted
template <typename T>
bool isSorted(const T* arr, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "=== Template Sort Functions ===" << endl << endl;

    // Bubble sort
    cout << "Bubble Sort:" << endl;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    size_t size1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Before: ";
    printArray(arr1, size1);
    bubbleSort(arr1, size1);
    cout << "After:  ";
    printArray(arr1, size1);
    cout << "Is sorted: " << (isSorted(arr1, size1) ? "Yes" : "No") << endl;
    cout << endl;

    // Selection sort
    cout << "Selection Sort:" << endl;
    double arr2[] = {3.14, 1.41, 2.71, 0.99, 5.67};
    size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
    cout << "Before: ";
    printArray(arr2, size2);
    selectionSort(arr2, size2);
    cout << "After:  ";
    printArray(arr2, size2);
    cout << endl;

    // Insertion sort
    cout << "Insertion Sort:" << endl;
    string arr3[] = {"zebra", "apple", "mango", "banana", "cherry"};
    size_t size3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "Before: ";
    printArray(arr3, size3);
    insertionSort(arr3, size3);
    cout << "After:  ";
    printArray(arr3, size3);
    cout << endl;

    // Quick sort
    cout << "Quick Sort:" << endl;
    int arr4[] = {10, 7, 8, 9, 1, 5};
    size_t size4 = sizeof(arr4) / sizeof(arr4[0]);
    cout << "Before: ";
    printArray(arr4, size4);
    quickSort(arr4, size4);
    cout << "After:  ";
    printArray(arr4, size4);
    cout << endl;

    // Descending sort
    cout << "Descending Sort:" << endl;
    int arr5[] = {5, 2, 8, 1, 9, 3, 7};
    size_t size5 = sizeof(arr5) / sizeof(arr5[0]);
    cout << "Before: ";
    printArray(arr5, size5);
    sortDescending(arr5, size5);
    cout << "After:  ";
    printArray(arr5, size5);
    cout << endl;

    // Vector sort
    cout << "Vector Sort:" << endl;
    vector<int> vec = {64, 34, 25, 12, 22, 11, 90};
    cout << "Before: ";
    printVector(vec);
    sortVector(vec);
    cout << "After:  ";
    printVector(vec);
    cout << endl;

    // Character array sort
    cout << "Character Array Sort:" << endl;
    char arr6[] = {'z', 'a', 'm', 'b', 'x', 'c'};
    size_t size6 = sizeof(arr6) / sizeof(arr6[0]);
    cout << "Before: ";
    printArray(arr6, size6);
    bubbleSort(arr6, size6);
    cout << "After:  ";
    printArray(arr6, size6);

    return 0;
}
