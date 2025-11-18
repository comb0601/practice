/*
 * Lesson 91 - Example 09: Cache Warm-Up Effects
 *
 * Demonstrates the importance of warming up the CPU cache before benchmarking.
 * Shows difference between cold and warm cache performance.
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

// Function to benchmark
long long sumArray(const std::vector<int>& arr) {
    long long sum = 0;
    for (int value : arr) {
        sum += value;
    }
    return sum;
}

// Pollute the cache with other data
void polluteCache() {
    const int POLLUTION_SIZE = 10000000;
    std::vector<int> pollution(POLLUTION_SIZE);

    volatile long long dummy = 0;
    for (int value : pollution) {
        dummy += value;
    }
}

int main() {
    std::cout << "=== Cache Warm-Up Effects Example ===\n\n";

    const int ARRAY_SIZE = 1000000;
    std::vector<int> data(ARRAY_SIZE);

    // Initialize data
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = i % 100;
    }

    Timer timer;

    // Test 1: Cold cache (first run)
    std::cout << "--- Cold Cache Test ---\n";
    std::cout << "Data is not in CPU cache yet.\n\n";

    polluteCache();  // Make sure data is not in cache

    timer.reset();
    long long result1 = sumArray(data);
    double coldTime = timer.elapsedMicroseconds();

    std::cout << "Cold cache time: " << coldTime << " us\n";
    std::cout << "Result: " << result1 << "\n\n";

    // Test 2: Warm cache (immediately after)
    std::cout << "--- Warm Cache Test ---\n";
    std::cout << "Data is now in CPU cache.\n\n";

    timer.reset();
    long long result2 = sumArray(data);
    double warmTime = timer.elapsedMicroseconds();

    std::cout << "Warm cache time: " << warmTime << " us\n";
    std::cout << "Result: " << result2 << "\n\n";

    // Test 3: Multiple warm runs
    std::cout << "--- Multiple Warm Runs ---\n";
    std::cout << "Running 10 times with warm cache:\n\n";

    std::vector<double> warmTimes;
    for (int i = 0; i < 10; ++i) {
        timer.reset();
        long long result = sumArray(data);
        double time = timer.elapsedMicroseconds();
        warmTimes.push_back(time);
        std::cout << "  Run " << (i + 1) << ": " << time << " us\n";
    }

    // Calculate average warm time
    double sumWarm = 0;
    for (double t : warmTimes) {
        sumWarm += t;
    }
    double avgWarmTime = sumWarm / warmTimes.size();

    std::cout << "\n  Average warm time: " << avgWarmTime << " us\n\n";

    // Analysis
    std::cout << "========== ANALYSIS ==========\n\n";

    double speedup = coldTime / avgWarmTime;
    std::cout << "Cache speedup: " << speedup << "x faster\n";
    std::cout << "Cold vs Warm difference: " << (coldTime - avgWarmTime) << " us\n";
    std::cout << "Percent improvement: " << ((coldTime - avgWarmTime) / coldTime * 100) << "%\n\n";

    std::cout << "========== BEST PRACTICES ==========\n\n";
    std::cout << "1. Always warm up cache before benchmarking:\n";
    std::cout << "   - Run function several times before measuring\n";
    std::cout << "   - Discard first few measurements\n\n";

    std::cout << "2. Understand your use case:\n";
    std::cout << "   - If data is accessed repeatedly: warm cache is realistic\n";
    std::cout << "   - If data is accessed once: cold cache is realistic\n\n";

    std::cout << "3. Report both scenarios when relevant:\n";
    std::cout << "   - Cold cache = worst case\n";
    std::cout << "   - Warm cache = typical case\n\n";

    std::cout << "In this example:\n";
    std::cout << "   Cold: " << coldTime << " us (first access)\n";
    std::cout << "   Warm: " << avgWarmTime << " us (subsequent accesses)\n";

    return 0;
}
