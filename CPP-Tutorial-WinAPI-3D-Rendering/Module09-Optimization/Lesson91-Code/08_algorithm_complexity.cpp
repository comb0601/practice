/*
 * Lesson 91 - Example 08: Algorithm Complexity Comparison
 *
 * Demonstrates how algorithm complexity (Big O notation) affects
 * performance. Compares O(n²) vs O(n) algorithms empirically.
 */

#include <iostream>
#include <vector>
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMicroseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count();
    }
};

// O(n²) - Nested loops
// Checks all pairs of elements
bool hasDuplicatesSlow(const std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[i] == arr[j]) {
                return true;
            }
        }
    }
    return false;
}

// O(n) - Single pass with hash set
// Uses extra memory for speed
bool hasDuplicatesFast(const std::vector<int>& arr) {
    std::vector<bool> seen(1000, false);  // Assuming values 0-999

    for (int value : arr) {
        if (seen[value]) {
            return true;
        }
        seen[value] = true;
    }

    return false;
}

void compareComplexity(int n) {
    // Create test data (no duplicates)
    std::vector<int> data;
    for (int i = 0; i < n; ++i) {
        data.push_back(i);
    }

    Timer timer;

    // Benchmark O(n²) version
    timer.reset();
    bool result1 = hasDuplicatesSlow(data);
    double timeSlow = timer.elapsedMicroseconds();

    // Benchmark O(n) version
    timer.reset();
    bool result2 = hasDuplicatesFast(data);
    double timeFast = timer.elapsedMicroseconds();

    std::cout << "n = " << n << ":\n";
    std::cout << "  O(n²) time: " << timeSlow << " us\n";
    std::cout << "  O(n) time:  " << timeFast << " us\n";
    std::cout << "  Speedup:    " << (timeSlow / timeFast) << "x\n";
    std::cout << "  Result match: " << (result1 == result2 ? "YES" : "NO") << "\n\n";
}

int main() {
    std::cout << "=== Algorithm Complexity Comparison ===\n\n";
    std::cout << "Comparing O(n²) vs O(n) duplicate detection:\n\n";

    // Test with increasing sizes
    std::vector<int> sizes = {100, 200, 400, 800, 1600};

    for (int size : sizes) {
        compareComplexity(size);
    }

    std::cout << "========== OBSERVATIONS ==========\n\n";
    std::cout << "O(n²) algorithm:\n";
    std::cout << "  - Time quadruples when n doubles\n";
    std::cout << "  - Example: n=400 takes ~4x longer than n=200\n\n";

    std::cout << "O(n) algorithm:\n";
    std::cout << "  - Time doubles when n doubles\n";
    std::cout << "  - Linear scaling with input size\n\n";

    std::cout << "Speedup grows with n:\n";
    std::cout << "  - For small n: Modest improvement\n";
    std::cout << "  - For large n: Massive improvement\n\n";

    std::cout << "Key Lesson:\n";
    std::cout << "  Algorithm choice matters more than micro-optimizations!\n";
    std::cout << "  O(n²) → O(n) is better than any low-level optimization.\n";

    return 0;
}
