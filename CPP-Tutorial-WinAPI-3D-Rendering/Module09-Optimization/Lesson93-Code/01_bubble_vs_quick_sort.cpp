/*
 * Lesson 93 - Example 01: Bubble Sort vs Quick Sort
 *
 * Demonstrates the massive difference between O(n²) and O(n log n) algorithms.
 * Shows why algorithm choice is the most important optimization.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
public:
    void reset() { start = std::chrono::high_resolution_clock::now(); }
    double elapsedMs() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    }
};

// O(n²) - Bubble Sort
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// O(n log n) - Quick Sort Helper
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortRecursive(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRecursive(arr, low, pi - 1);
        quickSortRecursive(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    quickSortRecursive(arr, 0, arr.size() - 1);
}

// O(n log n) - STL Sort (optimized introsort)
void stlSort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
}

int main() {
    std::cout << "=== Sorting Algorithm Comparison ===\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    // Test different sizes
    std::vector<int> sizes = {1000, 5000, 10000};

    for (int size : sizes) {
        std::cout << "Array size: " << size << "\n";

        // Generate random data
        std::vector<int> original(size);
        std::uniform_int_distribution<> dis(0, 10000);
        for (int& val : original) {
            val = dis(gen);
        }

        Timer timer;

        // Test Bubble Sort (O(n²))
        std::vector<int> arr1 = original;
        timer.reset();
        bubbleSort(arr1);
        double timeBubble = timer.elapsedMs();

        // Test Quick Sort (O(n log n))
        std::vector<int> arr2 = original;
        timer.reset();
        quickSort(arr2);
        double timeQuick = timer.elapsedMs();

        // Test STL Sort (O(n log n), highly optimized)
        std::vector<int> arr3 = original;
        timer.reset();
        stlSort(arr3);
        double timeSTL = timer.elapsedMs();

        // Verify all sorted correctly
        bool allCorrect = (arr1 == arr2) && (arr2 == arr3);

        std::cout << "  Bubble Sort: " << timeBubble << " ms\n";
        std::cout << "  Quick Sort:  " << timeQuick << " ms\n";
        std::cout << "  STL Sort:    " << timeSTL << " ms\n";
        std::cout << "  Speedup (Quick vs Bubble): " << (timeBubble / timeQuick) << "x\n";
        std::cout << "  Speedup (STL vs Bubble):   " << (timeBubble / timeSTL) << "x\n";
        std::cout << "  Results match: " << (allCorrect ? "YES" : "NO") << "\n\n";
    }

    std::cout << "========== KEY LESSON ==========\n\n";
    std::cout << "Algorithm complexity matters MORE than any micro-optimization!\n\n";
    std::cout << "O(n²) vs O(n log n):\n";
    std::cout << "  n=1,000:   Bubble takes 10-50x longer\n";
    std::cout << "  n=10,000:  Bubble takes 100-500x longer!\n\n";
    std::cout << "For n=100,000, Bubble Sort would take HOURS,\n";
    std::cout << "while Quick Sort takes milliseconds.\n\n";
    std::cout << "Always use std::sort() unless you have a very specific reason not to!\n";

    return 0;
}
