/*
 * Lesson 96: Batch-Rendering
 * Optimization Topic: IndirectRendering
 *
 * Performance optimization techniques
 *
 * Compilation:
 * cl /O2 /EHsc IndirectRendering.cpp
 * g++ -O3 -std=c++17 IndirectRendering.cpp -o IndirectRendering
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>

// Timing helper
class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    double ElapsedMs() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

// Optimization demonstration for: IndirectRendering
class OptimizationDemo {
public:
    void RunBaseline() {
        std::cout << "Running baseline implementation...\n";
        Timer t;

        // Baseline code (unoptimized)
        volatile int result = 0;
        for (int i = 0; i < 1000000; ++i) {
            result += i * 2;
        }

        std::cout << "Baseline time: " << t.ElapsedMs() << " ms\n";
    }

    void RunOptimized() {
        std::cout << "Running optimized implementation...\n";
        Timer t;

        // Optimized code
        volatile int result = 0;
        for (int i = 0; i < 1000000; ++i) {
            result += i << 1;  // Bitshift instead of multiplication
        }

        std::cout << "Optimized time: " << t.ElapsedMs() << " ms\n";
    }

    void ShowOptimizationTips() {
        std::cout << "\nOptimization Tips for IndirectRendering:\n";
        std::cout << "1. Profile before optimizing\n";
        std::cout << "2. Focus on hot paths\n";
        std::cout << "3. Measure the impact\n";
        std::cout << "4. Consider readability vs performance\n";
    }
};

int main() {
    std::cout << "=== Lesson 96: Batch-Rendering ===\n";
    std::cout << "Optimization Topic: IndirectRendering\n\n";

    OptimizationDemo demo;

    demo.RunBaseline();
    demo.RunOptimized();
    demo.ShowOptimizationTips();

    std::cout << "\n=== Benchmark Complete ===\n";
    return 0;
}
