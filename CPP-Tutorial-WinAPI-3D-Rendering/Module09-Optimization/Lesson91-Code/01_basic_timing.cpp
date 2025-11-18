/*
 * Lesson 91 - Example 01: Basic Timing with std::chrono
 *
 * Demonstrates basic performance measurement using C++11 chrono library.
 * Shows how to time simple operations and report results.
 */

#include <iostream>
#include <chrono>
#include <vector>

// Simple function to measure
void doWork(int iterations) {
    volatile int sum = 0;  // volatile prevents optimization
    for (int i = 0; i < iterations; ++i) {
        sum += i;
    }
}

int main() {
    std::cout << "=== Basic Timing Example ===\n\n";

    const int ITERATIONS = 100000000;

    // Method 1: Using high_resolution_clock
    auto start = std::chrono::high_resolution_clock::now();

    doWork(ITERATIONS);

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration in different units
    auto durationNano = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Time for " << ITERATIONS << " iterations:\n";
    std::cout << "  Nanoseconds:  " << durationNano.count() << " ns\n";
    std::cout << "  Microseconds: " << durationMicro.count() << " us\n";
    std::cout << "  Milliseconds: " << durationMilli.count() << " ms\n";

    // Calculate operations per second
    double seconds = durationMilli.count() / 1000.0;
    double opsPerSecond = ITERATIONS / seconds;

    std::cout << "\nPerformance:\n";
    std::cout << "  " << opsPerSecond << " operations/second\n";
    std::cout << "  " << (seconds / ITERATIONS * 1000000000) << " ns per operation\n";

    return 0;
}
