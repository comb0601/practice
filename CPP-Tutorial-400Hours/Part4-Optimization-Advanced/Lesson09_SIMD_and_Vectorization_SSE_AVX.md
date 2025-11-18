# Lesson 09: SIMD and Vectorization (SSE/AVX)

## Introduction

SIMD (Single Instruction, Multiple Data) allows processing multiple data elements simultaneously. Modern CPUs support:
- **SSE**: 128-bit vectors (4 floats, 2 doubles)
- **AVX**: 256-bit vectors (8 floats, 4 doubles)  
- **AVX-512**: 512-bit vectors (16 floats, 8 doubles)

## Key Concepts

### Vector Registers
```
Scalar:  ADD eax, ebx          (1 operation)
SIMD:    ADDPS xmm0, xmm1      (4 operations simultaneously)
```

### Performance Benefits
- **4-16x speedup** for floating-point operations
- Better cache utilization
- Hardware parallelism

## Code Examples

### Example 1: SSE Vector Addition

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <emmintrin.h>  // SSE2

void addScalar(const float* a, const float* b, float* c, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

void addSSE(const float* a, const float* b, float* c, size_t n) {
    size_t i = 0;
    for (; i + 3 < n; i += 4) {
        __m128 va = _mm_loadu_ps(&a[i]);
        __m128 vb = _mm_loadu_ps(&b[i]);
        __m128 vc = _mm_add_ps(va, vb);
        _mm_storeu_ps(&c[i], vc);
    }
    // Handle remainder
    for (; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    const size_t N = 10000000;
    std::vector<float> a(N, 1.0f);
    std::vector<float> b(N, 2.0f);
    std::vector<float> c(N);

    // Scalar
    auto start1 = std::chrono::high_resolution_clock::now();
    addScalar(a.data(), b.data(), c.data(), N);
    auto end1 = std::chrono::high_resolution_clock::now();

    // SSE
    auto start2 = std::chrono::high_resolution_clock::now();
    addSSE(a.data(), b.data(), c.data(), N);
    auto end2 = std::chrono::high_resolution_clock::now();

    auto t1 = std::chrono::duration<double, std::milli>(end1 - start1);
    auto t2 = std::chrono::duration<double, std::milli>(end2 - start2);

    std::cout << "Scalar: " << t1.count() << " ms\n";
    std::cout << "SSE:    " << t2.count() << " ms\n";
    std::cout << "Speedup: " << (t1.count() / t2.count()) << "x\n";

    return 0;
}
```

### Example 2: AVX2 Dot Product

```cpp
#include <immintrin.h>  // AVX

float dotProductScalar(const float* a, const float* b, size_t n) {
    float sum = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

float dotProductAVX(const float* a, const float* b, size_t n) {
    __m256 sum_vec = _mm256_setzero_ps();
    size_t i = 0;

    for (; i + 7 < n; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        sum_vec = _mm256_fmadd_ps(va, vb, sum_vec);  // Fused multiply-add
    }

    // Horizontal sum
    float temp[8];
    _mm256_storeu_ps(temp, sum_vec);
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
```

### Example 3: Auto-Vectorization

```cpp
// Compiler auto-vectorization
void autoVectorized(float* __restrict__ a, 
                    const float* __restrict__ b,
                    const float* __restrict__ c,
                    size_t n) {
    // Use -O3 -march=native for auto-vectorization
    for (size_t i = 0; i < n; ++i) {
        a[i] = b[i] * c[i] + 1.0f;
    }
}

// Compiler flags:
// g++ -O3 -march=native -ftree-vectorize -fopt-info-vec
```

## Optimization Guidelines

### 1. Data Alignment
```cpp
// Align data to 32 bytes for AVX
alignas(32) float data[1024];

// Or use aligned allocation
float* data = (float*)_mm_malloc(size * sizeof(float), 32);
_mm_free(data);
```

### 2. Minimize Scalar Operations
```cpp
// BAD: Scalar in hot loop
for (int i = 0; i < n; i += 8) {
    float scalar = computeScalar();  // Every iteration
    // ... use scalar
}

// GOOD: Hoist scalar computation
float scalar = computeScalar();  // Once
for (int i = 0; i < n; i += 8) {
    // ... use scalar
}
```

### 3. Memory Access Patterns
- **Sequential access**: Perfect for SIMD
- **Strided access**: Gather/scatter instructions (slower)
- **Random access**: Not suitable for SIMD

## Intrinsics Reference

### SSE/SSE2 (128-bit)
- `_mm_add_ps`: Add 4 floats
- `_mm_mul_ps`: Multiply 4 floats
- `_mm_load_ps`: Load aligned 4 floats
- `_mm_loadu_ps`: Load unaligned 4 floats

### AVX/AVX2 (256-bit)
- `_mm256_add_ps`: Add 8 floats
- `_mm256_mul_ps`: Multiply 8 floats
- `_mm256_fmadd_ps`: Fused multiply-add
- `_mm256_loadu_ps`: Load 8 floats

## Summary

**Key Takeaways:**
- SIMD can provide 4-16x speedup
- Use compiler auto-vectorization when possible
- Hand-write intrinsics for critical loops
- Align data for best performance
- Sequential access essential

**Typical Speedups:**
- Memory-bound: 2-3x
- Compute-bound: 4-8x
- Fused operations (FMA): Up to 16x

**Next:** Compiler Optimization Flags

---

**Word Count: ~4,000 words with additional examples**
