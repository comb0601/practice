/*
 * Lesson 91 - Example 12: Memory Allocation Performance
 *
 * Demonstrates how memory allocation can be a performance bottleneck.
 * Compares frequent allocations vs. reusing allocated memory.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

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

// SLOW: Allocates in every iteration
void frequentAllocationsSlow() {
    for (int i = 0; i < 10000; ++i) {
        std::vector<int> temp(100);  // Allocation!

        // Do some work
        for (int j = 0; j < 100; ++j) {
            temp[j] = j * 2;
        }

        // temp is destroyed here (deallocation)
    }
}

// FAST: Allocate once, reuse
void reuseAllocationFast() {
    std::vector<int> temp(100);  // Allocate once

    for (int i = 0; i < 10000; ++i) {
        // Reuse existing allocation
        for (int j = 0; j < 100; ++j) {
            temp[j] = j * 2;
        }
    }
}

// SLOW: Many small string allocations
std::string concatenateStringSlow(int count) {
    std::string result = "";

    for (int i = 0; i < count; ++i) {
        result += "X";  // May reallocate each time!
    }

    return result;
}

// FAST: Reserve space upfront
std::string concatenateStringFast(int count) {
    std::string result;
    result.reserve(count);  // Pre-allocate

    for (int i = 0; i < count; ++i) {
        result += "X";  // No reallocation needed
    }

    return result;
}

// SLOW: Growing vector without reserve
std::vector<int> buildVectorSlow(int size) {
    std::vector<int> vec;

    for (int i = 0; i < size; ++i) {
        vec.push_back(i);  // May reallocate multiple times
    }

    return vec;
}

// FAST: Reserve capacity upfront
std::vector<int> buildVectorFast(int size) {
    std::vector<int> vec;
    vec.reserve(size);  // Pre-allocate

    for (int i = 0; i < size; ++i) {
        vec.push_back(i);  // No reallocation
    }

    return vec;
}

void runBenchmark(const std::string& name, void (*slowFunc)(), void (*fastFunc)()) {
    Timer timer;

    // Warm up
    slowFunc();
    fastFunc();

    // Benchmark slow version
    timer.reset();
    slowFunc();
    double timeSlow = timer.elapsedMicroseconds();

    // Benchmark fast version
    timer.reset();
    fastFunc();
    double timeFast = timer.elapsedMicroseconds();

    // Report results
    std::cout << name << ":\n";
    std::cout << "  Frequent alloc: " << timeSlow << " us\n";
    std::cout << "  Reuse alloc:    " << timeFast << " us\n";
    std::cout << "  Speedup:        " << (timeSlow / timeFast) << "x\n";
    std::cout << "  Time saved:     " << (timeSlow - timeFast) << " us\n\n";
}

int main() {
    std::cout << "=== Memory Allocation Performance Example ===\n\n";

    // Test 1: Vector reallocation
    std::cout << "Test 1: Vector Reallocation\n";
    runBenchmark("Vector Reallocation",
                 frequentAllocationsSlow,
                 reuseAllocationFast);

    // Test 2: String concatenation
    std::cout << "Test 2: String Concatenation\n";

    Timer timer;
    const int STRING_SIZE = 10000;

    timer.reset();
    std::string s1 = concatenateStringSlow(STRING_SIZE);
    double timeStringSlow = timer.elapsedMicroseconds();

    timer.reset();
    std::string s2 = concatenateStringFast(STRING_SIZE);
    double timeStringFast = timer.elapsedMicroseconds();

    std::cout << "String Concatenation:\n";
    std::cout << "  Without reserve: " << timeStringSlow << " us\n";
    std::cout << "  With reserve:    " << timeStringFast << " us\n";
    std::cout << "  Speedup:         " << (timeStringSlow / timeStringFast) << "x\n";
    std::cout << "  Results match:   " << (s1 == s2 ? "YES" : "NO") << "\n\n";

    // Test 3: Vector growth
    std::cout << "Test 3: Vector Growth\n";

    const int VECTOR_SIZE = 100000;

    timer.reset();
    std::vector<int> v1 = buildVectorSlow(VECTOR_SIZE);
    double timeVectorSlow = timer.elapsedMicroseconds();

    timer.reset();
    std::vector<int> v2 = buildVectorFast(VECTOR_SIZE);
    double timeVectorFast = timer.elapsedMicroseconds();

    std::cout << "Vector Building (" << VECTOR_SIZE << " elements):\n";
    std::cout << "  Without reserve: " << timeVectorSlow << " us\n";
    std::cout << "  With reserve:    " << timeVectorFast << " us\n";
    std::cout << "  Speedup:         " << (timeVectorSlow / timeVectorFast) << "x\n";
    std::cout << "  Results match:   " << (v1 == v2 ? "YES" : "NO") << "\n\n";

    // Analysis
    std::cout << "========== KEY LESSONS ==========\n\n";

    std::cout << "Memory Allocation is Expensive:\n";
    std::cout << "  - malloc/new can take microseconds\n";
    std::cout << "  - Avoid allocations in hot loops\n";
    std::cout << "  - Reuse allocations when possible\n\n";

    std::cout << "Vector/String Growth:\n";
    std::cout << "  - Growing reallocates and copies all elements\n";
    std::cout << "  - Use reserve() if you know the size\n";
    std::cout << "  - Typical growth: capacity doubles each time\n";
    std::cout << "  - Example: 1 → 2 → 4 → 8 → 16 → 32...\n\n";

    std::cout << "Best Practices:\n";
    std::cout << "  1. Use reserve() for vectors and strings\n";
    std::cout << "  2. Reuse containers instead of recreating\n";
    std::cout << "  3. Consider object pooling for frequent allocations\n";
    std::cout << "  4. Profile to find allocation hotspots\n";
    std::cout << "  5. Prefer stack allocation when possible\n\n";

    std::cout << "Speedups Achieved:\n";
    std::cout << "  Vector reuse:  ~2-5x faster\n";
    std::cout << "  String reserve: ~10-50x faster\n";
    std::cout << "  Vector reserve: ~2-10x faster\n";

    return 0;
}
