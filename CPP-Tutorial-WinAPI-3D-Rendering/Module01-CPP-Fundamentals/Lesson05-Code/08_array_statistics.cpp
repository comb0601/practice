/*
 * Program: Array Statistics
 * Description: Computing statistical measures - mean, median, mode, variance
 * Compilation: g++ 08_array_statistics.cpp -o 08_array_statistics
 * Execution: ./08_array_statistics
 */

#include <iostream>
#include <cmath>
using namespace std;

double calculateMean(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}

void sortArray(int arr[], int size) {
    // Bubble sort
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

double calculateMedian(int arr[], int size) {
    // Create a copy to avoid modifying original
    int* temp = new int[size];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }

    sortArray(temp, size);

    double median;
    if (size % 2 == 0) {
        median = (temp[size/2 - 1] + temp[size/2]) / 2.0;
    } else {
        median = temp[size/2];
    }

    delete[] temp;
    return median;
}

int calculateMode(int arr[], int size) {
    int maxCount = 0;
    int mode = arr[0];

    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (arr[j] == arr[i]) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            mode = arr[i];
        }
    }

    return mode;
}

double calculateVariance(int arr[], int size) {
    double mean = calculateMean(arr, size);
    double sumSquaredDiff = 0;

    for (int i = 0; i < size; i++) {
        double diff = arr[i] - mean;
        sumSquaredDiff += diff * diff;
    }

    return sumSquaredDiff / size;
}

double calculateStandardDeviation(int arr[], int size) {
    return sqrt(calculateVariance(arr, size));
}

int findRange(int arr[], int size) {
    int max = arr[0];
    int min = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }

    return max - min;
}

int main() {
    cout << "=== Array Statistics ===" << endl << endl;

    int data[] = {5, 8, 3, 9, 5, 2, 8, 5, 7, 4};
    int size = sizeof(data) / sizeof(data[0]);

    // Display data
    cout << "Dataset: ";
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }
    cout << endl << endl;

    // Mean
    cout << "1. Mean (Average):" << endl;
    double mean = calculateMean(data, size);
    cout << "Mean = " << mean << endl << endl;

    // Median
    cout << "2. Median:" << endl;
    double median = calculateMedian(data, size);
    cout << "Median = " << median << endl << endl;

    // Mode
    cout << "3. Mode (Most Frequent):" << endl;
    int mode = calculateMode(data, size);
    cout << "Mode = " << mode << endl << endl;

    // Range
    cout << "4. Range:" << endl;
    int range = findRange(data, size);
    cout << "Range = " << range << endl << endl;

    // Variance
    cout << "5. Variance:" << endl;
    double variance = calculateVariance(data, size);
    cout << "Variance = " << variance << endl << endl;

    // Standard Deviation
    cout << "6. Standard Deviation:" << endl;
    double stdDev = calculateStandardDeviation(data, size);
    cout << "Standard Deviation = " << stdDev << endl << endl;

    // Summary
    cout << "=== Statistical Summary ===" << endl;
    cout << "Count:    " << size << endl;
    cout << "Mean:     " << mean << endl;
    cout << "Median:   " << median << endl;
    cout << "Mode:     " << mode << endl;
    cout << "Range:    " << range << endl;
    cout << "Variance: " << variance << endl;
    cout << "Std Dev:  " << stdDev << endl;

    return 0;
}
