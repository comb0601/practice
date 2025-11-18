/*
 * Lesson 94 - Example 01: SSE Vector Addition
 *
 * Demonstrates 4x speedup using SSE intrinsics for float array addition.
 * Compares scalar vs SIMD performance.
 */

#include <iostream>
#include <chrono>
#include <xmmintrin.h>  // SSE
#include <vector>

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

// Scalar version: Process 1 element at a time
void vectorAddScalar(const float* a, const float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

// SSE version: Process 4 elements at a time
void vectorAddSSE(const float* a, const float* b, float* c, int n) {
    int i = 0;

    // Process 4 floats at a time
    for (; i <= n - 4; i += 4) {
        __m128 va = _mm_load_ps(&a[i]);  // Load 4 floats
        __m128 vb = _mm_load_ps(&b[i]);  // Load 4 floats
        __m128 vc = _mm_add_ps(va, vb);  // Add 4 pairs
        _mm_store_ps(&c[i], vc);         // Store 4 results
    }

    // Handle remaining elements (if n not multiple of 4)
    for (; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    std::cout << "=== SSE Vector Addition Example ===\n\n";

    const int SIZE = 10000000;  // 10 million floats
    const int ITERATIONS = 100;

    // Allocate aligned memory (16-byte alignment for SSE)
    alignas(16) float* a = new float[SIZE];
    alignas(16) float* b = new float[SIZE];
    alignas(16) float* c_scalar = new float[SIZE];
    alignas(16) float* c_sse = new float[SIZE];

    // Initialize data
    for (int i = 0; i < SIZE; ++i) {
        a[i] = i * 0.5f;
        b[i] = i * 1.5f;
    }

    Timer timer;

    // Benchmark scalar version
    timer.reset();
    for (int iter = 0; iter < ITERATIONS; ++iter) {
        vectorAddScalar(a, b, c_scalar, SIZE);
    }
    double timeScalar = timer.elapsedMs();

    // Benchmark SSE version
    timer.reset();
    for (int iter = 0; iter < ITERATIONS; ++iter) {
        vectorAddSSE(a, b, c_sse, SIZE);
    }
    double timeSSE = timer.elapsedMs();

    // Verify results match
    bool resultsMatch = true;
    for (int i = 0; i < SIZE && resultsMatch; ++i) {
        if (std::abs(c_scalar[i] - c_sse[i]) > 0.0001f) {
            resultsMatch = false;
        }
    }

    // Results
    std::cout << "Array size: " << SIZE << " floats (" << (SIZE * 4 / 1024 / 1024) << " MB)\n";
    std::cout << "Iterations: " << ITERATIONS << "\n\n";

    std::cout << "Performance:\n";
    std::cout << "  Scalar (1 at a time):  " << timeScalar << " ms\n";
    std::cout << "  SSE (4 at a time):     " << timeSSE << " ms\n";
    std::cout << "  Speedup:               " << (timeScalar / timeSSE) << "x\n\n";

    std::cout << "Results match: " << (resultsMatch ? "YES" : "NO") << "\n\n";

    std::cout << "========== HOW SSE WORKS ==========\n\n";
    std::cout << "Scalar:\n";
    std::cout << "  c[0] = a[0] + b[0]  // 1 addition\n";
    std::cout << "  c[1] = a[1] + b[1]  // 1 addition\n";
    std::cout << "  c[2] = a[2] + b[2]  // 1 addition\n";
    std::cout << "  c[3] = a[3] + b[3]  // 1 addition\n";
    std::cout << "  Total: 4 instructions\n\n";

    std::cout << "SSE:\n";
    std::cout << "  Load 4 floats: va = [a[0], a[1], a[2], a[3]]\n";
    std::cout << "  Load 4 floats: vb = [b[0], b[1], b[2], b[3]]\n";
    std::cout << "  Add 4 pairs:   vc = va + vb  // ALL 4 AT ONCE!\n";
    std::cout << "  Store 4 results\n";
    std::cout << "  Total: 1 addition instruction (for 4 operations)\n\n";

    std::cout << "Result: " << (timeScalar / timeSSE) << "x faster!\n\n";

    std::cout << "SIMD IS BEST FOR:\n";
    std::cout << "  ✓ Large arrays (millions of elements)\n";
    std::cout << "  ✓ Same operation on all elements\n";
    std::cout << "  ✓ Vector math, image/audio processing\n";
    std::cout << "  ✓ When data is aligned (16/32-byte)\n";

    delete[] a;
    delete[] b;
    delete[] c_scalar;
    delete[] c_sse;

    return 0;
}
