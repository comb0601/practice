/*
 * Lesson 91 - Example 14: Loop Optimization Techniques
 *
 * Demonstrates various loop optimization techniques:
 * - Loop hoisting (moving invariant code outside)
 * - Strength reduction (replacing expensive operations)
 * - Loop unrolling
 * - Loop fusion
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

// ===== Loop Hoisting =====

// SLOW: Calculates constant inside loop
double loopHoistingSlow(const std::vector<double>& data) {
    double result = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        double factor = std::sqrt(2.0) * 3.14159;  // Constant! But calculated every iteration
        result += data[i] * factor;
    }

    return result;
}

// FAST: Move constant outside loop
double loopHoistingFast(const std::vector<double>& data) {
    const double factor = std::sqrt(2.0) * 3.14159;  // Calculate once
    double result = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        result += data[i] * factor;
    }

    return result;
}

// ===== Strength Reduction =====

// SLOW: Uses expensive operations (division, modulo)
long long strengthReductionSlow(int n) {
    long long sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += i / 2;  // Division in every iteration
    }

    return sum;
}

// FAST: Replace division with multiplication
long long strengthReductionFast(int n) {
    long long sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += i * 0.5;  // Multiplication is faster than division
    }

    return sum;
}

// Even FASTER: Use bit shift for powers of 2
long long strengthReductionFastest(int n) {
    long long sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += i >> 1;  // Right shift = divide by 2
    }

    return sum;
}

// ===== Loop Unrolling =====

// Normal loop
int loopNormal(const std::vector<int>& data) {
    int sum = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }

    return sum;
}

// Unrolled loop (process 4 elements per iteration)
int loopUnrolled(const std::vector<int>& data) {
    int sum = 0;
    size_t i = 0;

    // Main loop: process 4 elements at a time
    for (; i + 3 < data.size(); i += 4) {
        sum += data[i];
        sum += data[i + 1];
        sum += data[i + 2];
        sum += data[i + 3];
    }

    // Handle remaining elements
    for (; i < data.size(); ++i) {
        sum += data[i];
    }

    return sum;
}

// ===== Loop Fusion =====

// SLOW: Two separate loops
void loopFusionSlow(std::vector<int>& a, std::vector<int>& b) {
    // First loop
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] = a[i] * 2;
    }

    // Second loop
    for (size_t i = 0; i < b.size(); ++i) {
        b[i] = b[i] + 10;
    }
}

// FAST: Fuse loops if possible
void loopFusionFast(std::vector<int>& a, std::vector<int>& b) {
    // Combined loop (if both have same size)
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] = a[i] * 2;
        b[i] = b[i] + 10;
    }
}

int main() {
    std::cout << "=== Loop Optimization Techniques ===\n\n";

    Timer timer;

    // ===== Test 1: Loop Hoisting =====
    std::cout << "Test 1: Loop Hoisting\n";

    std::vector<double> dataDouble(10000000, 1.5);

    timer.reset();
    double result1Slow = loopHoistingSlow(dataDouble);
    double time1Slow = timer.elapsedMicroseconds();

    timer.reset();
    double result1Fast = loopHoistingFast(dataDouble);
    double time1Fast = timer.elapsedMicroseconds();

    std::cout << "  Without hoisting: " << time1Slow << " us\n";
    std::cout << "  With hoisting:    " << time1Fast << " us\n";
    std::cout << "  Speedup:          " << (time1Slow / time1Fast) << "x\n\n";

    // ===== Test 2: Strength Reduction =====
    std::cout << "Test 2: Strength Reduction\n";

    const int N = 100000000;

    timer.reset();
    long long result2Slow = strengthReductionSlow(N);
    double time2Slow = timer.elapsedMicroseconds();

    timer.reset();
    long long result2Fast = strengthReductionFast(N);
    double time2Fast = timer.elapsedMicroseconds();

    timer.reset();
    long long result2Fastest = strengthReductionFastest(N);
    double time2Fastest = timer.elapsedMicroseconds();

    std::cout << "  Division:         " << time2Slow << " us\n";
    std::cout << "  Multiplication:   " << time2Fast << " us\n";
    std::cout << "  Bit shift:        " << time2Fastest << " us\n";
    std::cout << "  Speedup (mul):    " << (time2Slow / time2Fast) << "x\n";
    std::cout << "  Speedup (shift):  " << (time2Slow / time2Fastest) << "x\n\n";

    // ===== Test 3: Loop Unrolling =====
    std::cout << "Test 3: Loop Unrolling\n";

    std::vector<int> dataInt(10000000);
    for (size_t i = 0; i < dataInt.size(); ++i) {
        dataInt[i] = i % 100;
    }

    timer.reset();
    int result3Normal = loopNormal(dataInt);
    double time3Normal = timer.elapsedMicroseconds();

    timer.reset();
    int result3Unrolled = loopUnrolled(dataInt);
    double time3Unrolled = timer.elapsedMicroseconds();

    std::cout << "  Normal loop:      " << time3Normal << " us\n";
    std::cout << "  Unrolled loop:    " << time3Unrolled << " us\n";
    std::cout << "  Speedup:          " << (time3Normal / time3Unrolled) << "x\n";
    std::cout << "  Results match:    " << (result3Normal == result3Unrolled ? "YES" : "NO") << "\n\n";

    // ===== Test 4: Loop Fusion =====
    std::cout << "Test 4: Loop Fusion\n";

    std::vector<int> a(10000000, 5);
    std::vector<int> b(10000000, 3);
    std::vector<int> a2 = a;
    std::vector<int> b2 = b;

    timer.reset();
    loopFusionSlow(a, b);
    double time4Slow = timer.elapsedMicroseconds();

    timer.reset();
    loopFusionFast(a2, b2);
    double time4Fast = timer.elapsedMicroseconds();

    std::cout << "  Separate loops:   " << time4Slow << " us\n";
    std::cout << "  Fused loop:       " << time4Fast << " us\n";
    std::cout << "  Speedup:          " << (time4Slow / time4Fast) << "x\n\n";

    // ===== Summary =====
    std::cout << "========== KEY LESSONS ==========\n\n";

    std::cout << "1. Loop Hoisting:\n";
    std::cout << "   - Move loop-invariant code outside the loop\n";
    std::cout << "   - Especially for expensive operations (sqrt, sin, etc.)\n\n";

    std::cout << "2. Strength Reduction:\n";
    std::cout << "   - Replace expensive operations with cheaper ones\n";
    std::cout << "   - Division → Multiplication (if possible)\n";
    std::cout << "   - Multiply/Divide by 2 → Bit shifts\n\n";

    std::cout << "3. Loop Unrolling:\n";
    std::cout << "   - Process multiple elements per iteration\n";
    std::cout << "   - Reduces loop overhead\n";
    std::cout << "   - Better instruction pipelining\n";
    std::cout << "   - Trade-off: Increased code size\n\n";

    std::cout << "4. Loop Fusion:\n";
    std::cout << "   - Combine multiple loops into one\n";
    std::cout << "   - Better cache locality\n";
    std::cout << "   - Fewer loop overhead costs\n\n";

    std::cout << "Note: Modern compilers can do many of these optimizations\n";
    std::cout << "automatically in Release mode. Always profile to verify!\n";

    return 0;
}
