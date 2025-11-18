/*
 * Lesson 91 - Example 04: Benchmark Comparison
 *
 * Demonstrates comparing two implementations to measure speedup.
 * Shows unoptimized vs optimized code with quantitative results.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

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

// SLOW: Square root calculated in every iteration
double slowVersion(const std::vector<int>& data) {
    double result = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        // Expensive operation inside loop
        double factor = std::sqrt(static_cast<double>(i + 1));
        result += data[i] * factor;
    }

    return result;
}

// FAST: Pre-calculate square roots
double fastVersion(const std::vector<int>& data) {
    // Pre-calculate factors
    std::vector<double> factors(data.size());
    for (size_t i = 0; i < factors.size(); ++i) {
        factors[i] = std::sqrt(static_cast<double>(i + 1));
    }

    // Now just multiply
    double result = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        result += data[i] * factors[i];
    }

    return result;
}

void benchmarkComparison(const std::vector<int>& data) {
    Timer timer;

    // Warm up
    slowVersion(data);
    fastVersion(data);

    // Benchmark slow version
    timer.reset();
    double result1 = slowVersion(data);
    double timeSlow = timer.elapsedMicroseconds();

    // Benchmark fast version
    timer.reset();
    double result2 = fastVersion(data);
    double timeFast = timer.elapsedMicroseconds();

    // Verify results are the same
    double difference = std::abs(result1 - result2);
    bool resultsMatch = (difference < 0.001);

    // Report results
    std::cout << "Data size: " << data.size() << " elements\n";
    std::cout << "\nResults:\n";
    std::cout << "  Slow: " << result1 << "\n";
    std::cout << "  Fast: " << result2 << "\n";
    std::cout << "  Match: " << (resultsMatch ? "YES" : "NO") << "\n";

    std::cout << "\nPerformance:\n";
    std::cout << "  Slow version: " << timeSlow << " us\n";
    std::cout << "  Fast version: " << timeFast << " us\n";

    double speedup = timeSlow / timeFast;
    double percentFaster = ((timeSlow - timeFast) / timeSlow) * 100.0;

    std::cout << "\nImprovement:\n";
    std::cout << "  Speedup: " << speedup << "x faster\n";
    std::cout << "  Percent: " << percentFaster << "% faster\n";
    std::cout << "  Time saved: " << (timeSlow - timeFast) << " us\n";
}

int main() {
    std::cout << "=== Benchmark Comparison Example ===\n\n";

    // Test with different sizes
    std::vector<int> sizes = { 10000, 50000, 100000 };

    for (int size : sizes) {
        std::vector<int> data(size);

        // Fill with sample data
        for (int i = 0; i < size; ++i) {
            data[i] = i % 100;
        }

        std::cout << "--- Test " << size << " ---\n";
        benchmarkComparison(data);
        std::cout << "\n";
    }

    std::cout << "Key Insight: Pre-calculating expensive operations\n";
    std::cout << "outside loops can dramatically improve performance!\n";

    return 0;
}
