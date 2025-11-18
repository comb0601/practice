/*
 * Lesson 09: SIMD (Single Instruction Multiple Data)
 * Demonstrates SSE and AVX vectorization for performance
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstring>

#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
};

// ========== Scalar (No SIMD) Implementations ==========

void add_arrays_scalar(const float* a, const float* b, float* result, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] + b[i];
    }
}

float dot_product_scalar(const float* a, const float* b, size_t n) {
    float sum = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

void multiply_scalar(const float* a, float scalar, float* result, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] * scalar;
    }
}

// ========== SSE Implementations (128-bit, 4 floats) ==========

void add_arrays_sse(const float* a, const float* b, float* result, size_t n) {
    size_t i = 0;

    // Process 4 floats at a time
    for (; i + 4 <= n; i += 4) {
        __m128 va = _mm_loadu_ps(&a[i]);
        __m128 vb = _mm_loadu_ps(&b[i]);
        __m128 vresult = _mm_add_ps(va, vb);
        _mm_storeu_ps(&result[i], vresult);
    }

    // Handle remainder
    for (; i < n; ++i) {
        result[i] = a[i] + b[i];
    }
}

float dot_product_sse(const float* a, const float* b, size_t n) {
    __m128 vsum = _mm_setzero_ps();
    size_t i = 0;

    // Process 4 floats at a time
    for (; i + 4 <= n; i += 4) {
        __m128 va = _mm_loadu_ps(&a[i]);
        __m128 vb = _mm_loadu_ps(&b[i]);
        __m128 vmul = _mm_mul_ps(va, vb);
        vsum = _mm_add_ps(vsum, vmul);
    }

    // Horizontal add to get final sum
    alignas(16) float temp[4];
    _mm_store_ps(temp, vsum);
    float sum = temp[0] + temp[1] + temp[2] + temp[3];

    // Handle remainder
    for (; i < n; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

void multiply_scalar_sse(const float* a, float scalar, float* result, size_t n) {
    __m128 vscalar = _mm_set1_ps(scalar);
    size_t i = 0;

    for (; i + 4 <= n; i += 4) {
        __m128 va = _mm_loadu_ps(&a[i]);
        __m128 vresult = _mm_mul_ps(va, vscalar);
        _mm_storeu_ps(&result[i], vresult);
    }

    for (; i < n; ++i) {
        result[i] = a[i] * scalar;
    }
}

// ========== AVX Implementations (256-bit, 8 floats) ==========

#if defined(__AVX__) || defined(__AVX2__)

void add_arrays_avx(const float* a, const float* b, float* result, size_t n) {
    size_t i = 0;

    // Process 8 floats at a time
    for (; i + 8 <= n; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vresult = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&result[i], vresult);
    }

    // Handle remainder
    for (; i < n; ++i) {
        result[i] = a[i] + b[i];
    }
}

float dot_product_avx(const float* a, const float* b, size_t n) {
    __m256 vsum = _mm256_setzero_ps();
    size_t i = 0;

    for (; i + 8 <= n; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vmul = _mm256_mul_ps(va, vb);
        vsum = _mm256_add_ps(vsum, vmul);
    }

    // Horizontal add
    alignas(32) float temp[8];
    _mm256_store_ps(temp, vsum);
    float sum = 0.0f;
    for (int j = 0; j < 8; ++j) {
        sum += temp[j];
    }

    // Handle remainder
    for (; i < n; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

void multiply_scalar_avx(const float* a, float scalar, float* result, size_t n) {
    __m256 vscalar = _mm256_set1_ps(scalar);
    size_t i = 0;

    for (; i + 8 <= n; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vresult = _mm256_mul_ps(va, vscalar);
        _mm256_storeu_ps(&result[i], vresult);
    }

    for (; i < n; ++i) {
        result[i] = a[i] * scalar;
    }
}

#endif

// ========== Benchmarking Functions ==========

void benchmark_add_arrays(const std::vector<float>& a, const std::vector<float>& b, size_t iterations) {
    std::cout << "\n=== Array Addition Benchmark ===\n";

    const size_t n = a.size();
    std::vector<float> result(n);

    // Scalar
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            add_arrays_scalar(a.data(), b.data(), result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "Scalar: " << std::fixed << std::setprecision(2) << ms << " ms\n";
    }

    // SSE
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            add_arrays_sse(a.data(), b.data(), result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "SSE:    " << std::fixed << std::setprecision(2) << ms << " ms";
        std::cout << " (4x parallelism)\n";
    }

#if defined(__AVX__) || defined(__AVX2__)
    // AVX
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            add_arrays_avx(a.data(), b.data(), result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "AVX:    " << std::fixed << std::setprecision(2) << ms << " ms";
        std::cout << " (8x parallelism)\n";
    }
#endif
}

void benchmark_dot_product(const std::vector<float>& a, const std::vector<float>& b, size_t iterations) {
    std::cout << "\n=== Dot Product Benchmark ===\n";

    const size_t n = a.size();
    float result;

    // Scalar
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            result = dot_product_scalar(a.data(), b.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "Scalar: " << std::fixed << std::setprecision(2) << ms << " ms";
        std::cout << " (result: " << result << ")\n";
    }

    // SSE
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            result = dot_product_sse(a.data(), b.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "SSE:    " << std::fixed << std::setprecision(2) << ms << " ms";
        std::cout << " (result: " << result << ")\n";
    }

#if defined(__AVX__) || defined(__AVX2__)
    // AVX
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            result = dot_product_avx(a.data(), b.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "AVX:    " << std::fixed << std::setprecision(2) << ms << " ms";
        std::cout << " (result: " << result << ")\n";
    }
#endif
}

void benchmark_scalar_multiply(const std::vector<float>& a, size_t iterations) {
    std::cout << "\n=== Scalar Multiplication Benchmark ===\n";

    const size_t n = a.size();
    std::vector<float> result(n);
    const float scalar = 2.5f;

    // Scalar
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            multiply_scalar(a.data(), scalar, result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "Scalar: " << std::fixed << std::setprecision(2) << ms << " ms\n";
    }

    // SSE
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            multiply_scalar_sse(a.data(), scalar, result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "SSE:    " << std::fixed << std::setprecision(2) << ms << " ms\n";
    }

#if defined(__AVX__) || defined(__AVX2__)
    // AVX
    {
        Timer t;
        for (size_t i = 0; i < iterations; ++i) {
            multiply_scalar_avx(a.data(), scalar, result.data(), n);
        }
        double ms = t.elapsed_ms();
        std::cout << "AVX:    " << std::fixed << std::setprecision(2) << ms << " ms\n";
    }
#endif
}

// ========== CPU Feature Detection ==========

void print_cpu_features() {
    std::cout << "\n=== CPU SIMD Support ===\n";

#ifdef __SSE__
    std::cout << "SSE:   YES\n";
#else
    std::cout << "SSE:   NO\n";
#endif

#ifdef __SSE2__
    std::cout << "SSE2:  YES\n";
#else
    std::cout << "SSE2:  NO\n";
#endif

#ifdef __SSE3__
    std::cout << "SSE3:  YES\n";
#else
    std::cout << "SSE3:  NO\n";
#endif

#ifdef __SSE4_1__
    std::cout << "SSE4.1: YES\n";
#else
    std::cout << "SSE4.1: NO\n";
#endif

#ifdef __AVX__
    std::cout << "AVX:   YES\n";
#else
    std::cout << "AVX:   NO\n";
#endif

#ifdef __AVX2__
    std::cout << "AVX2:  YES\n";
#else
    std::cout << "AVX2:  NO\n";
#endif

#ifdef __AVX512F__
    std::cout << "AVX512: YES\n";
#else
    std::cout << "AVX512: NO\n";
#endif
}

int main() {
    std::cout << "SIMD Optimization with SSE and AVX\n";
    std::cout << "===================================\n";

    print_cpu_features();

    // Create test data
    const size_t array_size = 1000000;  // 1 million floats
    const size_t iterations = 1000;

    std::cout << "\nArray size: " << array_size << " floats ("
              << (array_size * sizeof(float) / 1024 / 1024) << " MB)\n";
    std::cout << "Iterations: " << iterations << "\n";

    std::vector<float> a(array_size);
    std::vector<float> b(array_size);

    // Initialize with random data
    std::random_device rd;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_real_distribution<float> dis(-100.0f, 100.0f);

    for (size_t i = 0; i < array_size; ++i) {
        a[i] = dis(gen);
        b[i] = dis(gen);
    }

    // Run benchmarks
    benchmark_add_arrays(a, b, iterations);
    benchmark_dot_product(a, b, iterations);
    benchmark_scalar_multiply(a, iterations);

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Key Takeaways:\n";
    std::cout << "- SSE provides ~4x speedup (4 floats per instruction)\n";
    std::cout << "- AVX provides ~8x speedup (8 floats per instruction)\n";
    std::cout << "- SIMD is crucial for performance-critical code\n";
    std::cout << "- Handle remainder elements after SIMD loop\n";
    std::cout << "- Ensure data alignment for best performance\n";
    std::cout << std::string(60, '=') << "\n";

    return 0;
}
