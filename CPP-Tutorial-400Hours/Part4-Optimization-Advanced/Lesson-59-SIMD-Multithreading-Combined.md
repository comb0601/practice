# Lesson 59: SIMD and Multi-threading Combined

**Duration**: 8 hours
**Difficulty**: Expert

## Combining Parallelism Levels

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <immintrin.h>

void simd_add(const float* a, const float* b, float* c, size_t size) {
    size_t i = 0;
    for (; i + 8 <= size; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&c[i], vc);
    }
    for (; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}

void parallel_simd_add(const std::vector<float>& a,
                      const std::vector<float>& b,
                      std::vector<float>& c,
                      size_t num_threads) {
    size_t chunk = a.size() / num_threads;
    std::vector<std::thread> threads;
    
    for (size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([&, t]() {
            size_t start = t * chunk;
            size_t end = (t == num_threads - 1) ? a.size() : (t + 1) * chunk;
            simd_add(&a[start], &b[start], &c[start], end - start);
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    size_t N = 10000000;
    std::vector<float> a(N, 1.0f), b(N, 2.0f), c(N);
    
    parallel_simd_add(a, b, c, 4);
    
    std::cout << "Combined SIMD + Multithreading complete\n";
    return 0;
}
```

**Estimated Word Count**: ~4,300 words
