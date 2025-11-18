/*
 * Lesson 91 - Example 10: Compiler Optimization Levels
 *
 * Demonstrates the importance of compiler optimizations.
 * This program shows how to detect and report optimization level.
 *
 * COMPILE WITH DIFFERENT FLAGS TO SEE DIFFERENCES:
 *
 * Visual Studio:
 *   Debug:   /Od (no optimization)
 *   Release: /O2 (maximize speed)
 *
 * GCC/Clang:
 *   Debug:   g++ -O0 file.cpp
 *   Release: g++ -O2 file.cpp   (or -O3)
 */

#include <iostream>
#include <chrono>
#include <vector>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMilliseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    }
};

// Function that benefits from optimization
double complexCalculation(int n) {
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        // Compiler can optimize this loop significantly
        result += i * 1.5;
        result -= i * 0.5;  // This simplifies to: result += i
    }

    return result;
}

// Function with removable dead code
int deadCodeExample(int n) {
    int result = 0;

    for (int i = 0; i < n; ++i) {
        int unused = i * 2;  // Dead code - never used
        result += i;
    }

    return result;
}

// Function with constant folding opportunity
double constantFolding() {
    // Compiler can compute this at compile-time
    double result = 0.0;

    for (int i = 0; i < 100; ++i) {
        result += 3.14159 * 2.0;  // Constant expression
    }

    return result;
}

void detectOptimizationLevel() {
    std::cout << "=== Compiler Optimization Detection ===\n\n";

    #ifdef _DEBUG
        std::cout << "Build Mode: DEBUG\n";
    #else
        std::cout << "Build Mode: RELEASE\n";
    #endif

    #ifdef NDEBUG
        std::cout << "NDEBUG defined: YES (asserts disabled)\n";
    #else
        std::cout << "NDEBUG defined: NO (asserts enabled)\n";
    #endif

    #ifdef _MSC_VER
        std::cout << "Compiler: Microsoft Visual C++\n";
        #ifdef _MSVC_TRADITIONAL
            std::cout << "Version: " << _MSC_VER << "\n";
        #endif
    #endif

    #ifdef __GNUC__
        std::cout << "Compiler: GCC/G++\n";
        std::cout << "Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "\n";
    #endif

    #ifdef __clang__
        std::cout << "Compiler: Clang\n";
    #endif

    // Detect optimization flags (GCC/Clang)
    #ifdef __OPTIMIZE__
        std::cout << "Optimization: ENABLED\n";
    #else
        std::cout << "Optimization: DISABLED\n";
    #endif

    std::cout << "\n";
}

int main() {
    detectOptimizationLevel();

    std::cout << "=== Benchmark Results ===\n\n";

    const int ITERATIONS = 100000000;
    Timer timer;

    // Test 1: Complex calculation
    std::cout << "Test 1: Complex calculation\n";
    timer.reset();
    double result1 = complexCalculation(ITERATIONS);
    double time1 = timer.elapsedMilliseconds();
    std::cout << "  Time: " << time1 << " ms\n";
    std::cout << "  Result: " << result1 << "\n\n";

    // Test 2: Dead code
    std::cout << "Test 2: Dead code elimination\n";
    timer.reset();
    int result2 = deadCodeExample(ITERATIONS);
    double time2 = timer.elapsedMilliseconds();
    std::cout << "  Time: " << time2 << " ms\n";
    std::cout << "  Result: " << result2 << "\n\n";

    // Test 3: Constant folding
    std::cout << "Test 3: Constant folding\n";
    timer.reset();
    double result3 = constantFolding();
    double time3 = timer.elapsedMilliseconds();
    std::cout << "  Time: " << time3 << " ms\n";
    std::cout << "  Result: " << result3 << "\n\n";

    // Test 4: Vector operations
    std::cout << "Test 4: Vector operations\n";
    timer.reset();
    std::vector<int> vec(10000000);
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i * 2;
    }
    long long sum = 0;
    for (int val : vec) {
        sum += val;
    }
    double time4 = timer.elapsedMilliseconds();
    std::cout << "  Time: " << time4 << " ms\n";
    std::cout << "  Sum: " << sum << "\n\n";

    std::cout << "========== EXPECTED RESULTS ==========\n\n";

    std::cout << "DEBUG BUILD (/O0, -O0):\n";
    std::cout << "  - Test 1: ~2000-5000 ms\n";
    std::cout << "  - Test 2: ~2000-5000 ms\n";
    std::cout << "  - Test 3: ~10-50 ms\n";
    std::cout << "  - Test 4: ~500-1000 ms\n\n";

    std::cout << "RELEASE BUILD (/O2, -O2):\n";
    std::cout << "  - Test 1: ~100-500 ms (10-50x faster!)\n";
    std::cout << "  - Test 2: ~100-500 ms (dead code removed)\n";
    std::cout << "  - Test 3: ~0-5 ms (constant computed at compile-time)\n";
    std::cout << "  - Test 4: ~50-200 ms (vectorization, loop unrolling)\n\n";

    std::cout << "KEY LESSONS:\n";
    std::cout << "  1. ALWAYS benchmark in Release mode with optimizations\n";
    std::cout << "  2. Debug builds can be 10-100x slower\n";
    std::cout << "  3. Compiler optimizations are extremely powerful:\n";
    std::cout << "     - Dead code elimination\n";
    std::cout << "     - Constant folding\n";
    std::cout << "     - Loop unrolling\n";
    std::cout << "     - Vectorization (SIMD)\n";
    std::cout << "     - Inlining\n";
    std::cout << "  4. Profile before optimizing manually - compiler might\n";
    std::cout << "     already have optimized what you think is slow!\n";

    return 0;
}
