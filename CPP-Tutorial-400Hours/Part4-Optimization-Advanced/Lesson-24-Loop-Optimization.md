# Lesson 24: Loop Optimization

**Duration**: 10 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Loop Optimization Fundamentals
2. Loop Unrolling
3. Loop Fusion and Fission
4. Strength Reduction
5. Loop Invariant Code Motion
6. Loop Interchange and Tiling
7. Vectorization and SIMD
8. Cache-Friendly Loop Patterns
9. Avoiding Loop Overhead
10. Exercises and Best Practices

---

## 1. Loop Optimization Fundamentals

Loops are the most executed parts of programs. Small improvements can yield massive performance gains.

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Poor: Multiple inefficiencies
void processData_poor(vector<int>& data) {
    for (int i = 0; i < data.size(); i++) {  // size() called each iteration
        data[i] = data[i] * 2 + sqrt(data[i]);  // Expensive sqrt each time
    }
}

// Better: Cache size, reduce expensive operations
void processData_better(vector<int>& data) {
    size_t size = data.size();  // Cache size
    for (size_t i = 0; i < size; i++) {
        int val = data[i];  // Load once
        data[i] = val * 2 + sqrt(val);
    }
}

// Best: Minimize calculations, use faster alternatives
void processData_best(vector<int>& data) {
    size_t size = data.size();
    for (size_t i = 0; i < size; i++) {
        int val = data[i];
        // Approximate sqrt or precompute if possible
        data[i] = val * 2 + static_cast<int>(sqrt(static_cast<double>(val)));
    }
}

void benchmark() {
    const int SIZE = 1000000;
    vector<int> data(SIZE);

    for (int i = 0; i < SIZE; i++) {
        data[i] = i % 1000 + 1;
    }

    auto test = [](const string& name, auto func, vector<int> data) {
        auto start = high_resolution_clock::now();
        func(data);
        auto end = high_resolution_clock::now();
        cout << name << ": " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    };

    test("Poor  ", processData_poor, data);
    test("Better", processData_better, data);
    test("Best  ", processData_best, data);
}

int main() {
    cout << "=== LOOP OPTIMIZATION FUNDAMENTALS ===\n\n";
    benchmark();
    return 0;
}
```

---

## 2. Loop Unrolling

Reduce loop overhead by processing multiple iterations at once.

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Original loop
void sum_normal(const vector<int>& data, int& result) {
    result = 0;
    for (size_t i = 0; i < data.size(); i++) {
        result += data[i];
    }
}

// Manually unrolled (4x)
void sum_unrolled4(const vector<int>& data, int& result) {
    result = 0;
    size_t size = data.size();
    size_t i = 0;

    // Process 4 elements at a time
    for (; i + 3 < size; i += 4) {
        result += data[i];
        result += data[i+1];
        result += data[i+2];
        result += data[i+3];
    }

    // Handle remaining elements
    for (; i < size; i++) {
        result += data[i];
    }
}

// Unrolled with accumulators (reduces dependencies)
void sum_unrolled_accum(const vector<int>& data, int& result) {
    size_t size = data.size();
    size_t i = 0;

    // Multiple accumulators for better pipelining
    int sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;

    for (; i + 3 < size; i += 4) {
        sum0 += data[i];
        sum1 += data[i+1];
        sum2 += data[i+2];
        sum3 += data[i+3];
    }

    result = sum0 + sum1 + sum2 + sum3;

    // Remaining elements
    for (; i < size; i++) {
        result += data[i];
    }
}

void benchmarkUnrolling() {
    const int SIZE = 10000000;
    vector<int> data(SIZE);
    for (int i = 0; i < SIZE; i++) data[i] = i;

    int result;

    auto test = [&](const string& name, auto func) {
        auto start = chrono::high_resolution_clock::now();
        func(data, result);
        auto end = chrono::high_resolution_clock::now();
        cout << name << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
             << " ms (sum=" << result << ")\n";
    };

    cout << "=== LOOP UNROLLING ===\n\n";
    test("Normal       ", sum_normal);
    test("Unrolled 4x  ", sum_unrolled4);
    test("Unroll+Accum ", sum_unrolled_accum);
}

int main() {
    benchmarkUnrolling();

    cout << "\nBenefits:\n";
    cout << "  • Reduces loop overhead (fewer comparisons/jumps)\n";
    cout << "  • Better instruction pipelining\n";
    cout << "  • Enables compiler optimizations\n";
    cout << "  • Typical speedup: 1.2-2x\n";

    return 0;
}
```

---

## 3. Loop Fusion and Fission

**Loop Fusion**: Combine multiple loops into one (better cache locality)
**Loop Fission**: Split one loop into multiple (reduce register pressure)

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Before fusion: Two separate loops
void process_separate(vector<int>& a, vector<int>& b, vector<int>& c) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] = b[i] + 10;
    }

    for (size_t i = 0; i < a.size(); i++) {
        c[i] = a[i] * 2;
    }
}

// After fusion: Combined into one loop
void process_fused(vector<int>& a, vector<int>& b, vector<int>& c) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] = b[i] + 10;
        c[i] = a[i] * 2;  // Uses just-computed value (in cache!)
    }
}

// Example: Loop fission (splitting)
void complexOperation_original(vector<int>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        // Too many operations, register pressure
        int temp1 = data[i] * 2;
        int temp2 = temp1 + 5;
        int temp3 = temp2 * temp2;
        int temp4 = temp3 - data[i];
        data[i] = temp4 % 100;
    }
}

// Split into multiple loops (if needed for register pressure)
void complexOperation_split(vector<int>& data, vector<int>& temp) {
    // First pass
    for (size_t i = 0; i < data.size(); i++) {
        temp[i] = data[i] * 2 + 5;
    }

    // Second pass
    for (size_t i = 0; i < data.size(); i++) {
        int val = temp[i] * temp[i] - data[i];
        data[i] = val % 100;
    }
}

void benchmarkFusion() {
    const int SIZE = 1000000;
    vector<int> a(SIZE), b(SIZE), c(SIZE);

    for (int i = 0; i < SIZE; i++) {
        b[i] = i;
    }

    auto start = chrono::high_resolution_clock::now();
    process_separate(a, b, c);
    auto end = chrono::high_resolution_clock::now();
    cout << "Separate loops: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    process_fused(a, b, c);
    end = chrono::high_resolution_clock::now();
    cout << "Fused loop:     " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

int main() {
    cout << "=== LOOP FUSION AND FISSION ===\n\n";
    benchmarkFusion();

    cout << "\nLoop Fusion Benefits:\n";
    cout << "  • Better cache locality\n";
    cout << "  • Fewer loop iterations overall\n";
    cout << "  • Reduced memory traffic\n\n";

    cout << "Loop Fission Use Cases:\n";
    cout << "  • Reduce register pressure\n";
    cout << "  • Enable better vectorization\n";
    cout << "  • Improve instruction scheduling\n";

    return 0;
}
```

---

## 4. Strength Reduction

Replace expensive operations with cheaper equivalents.

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Poor: Expensive operations in loop
void compute_poor(vector<int>& result, int n) {
    for (int i = 0; i < n; i++) {
        result[i] = i * i * i;  // Multiplication each iteration
    }
}

// Better: Strength reduction (multiplication → addition)
void compute_better(vector<int>& result, int n) {
    int cube = 0;
    int square = 0;
    int linear = 1;

    for (int i = 0; i < n; i++) {
        result[i] = cube;

        // Update using: (i+1)³ = i³ + 3i² + 3i + 1
        cube += square + linear;
        square += linear + linear;
        linear += 6;
    }
}

// Example: Division → Multiplication by reciprocal
void normalize_poor(vector<float>& data, float divisor) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] / divisor;  // Division each time
    }
}

void normalize_better(vector<float>& data, float divisor) {
    float multiplier = 1.0f / divisor;  // Compute once
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] * multiplier;  // Multiplication (faster!)
    }
}

// Example: Modulo → Bitwise AND (when power of 2)
void process_poor(vector<int>& data, int mod) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] % mod;  // Slow modulo
    }
}

void process_better(vector<int>& data, int mod) {
    // Only works if mod is power of 2!
    int mask = mod - 1;
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] & mask;  // Fast bitwise AND
    }
}

void benchmarkStrengthReduction() {
    const int SIZE = 1000000;
    vector<int> result(SIZE);

    auto start = chrono::high_resolution_clock::now();
    compute_poor(result, SIZE);
    auto end = chrono::high_resolution_clock::now();
    cout << "Multiplication: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    compute_better(result, SIZE);
    end = chrono::high_resolution_clock::now();
    cout << "Addition:       " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

int main() {
    cout << "=== STRENGTH REDUCTION ===\n\n";
    benchmarkStrengthReduction();

    cout << "\nCommon Strength Reductions:\n";
    cout << "  • Multiplication → Addition\n";
    cout << "  • Division → Multiplication by reciprocal\n";
    cout << "  • Modulo → Bitwise AND (power of 2)\n";
    cout << "  • Exponentiation → Repeated multiplication\n";

    return 0;
}
```

---

## 5. Loop Invariant Code Motion

Move calculations that don't change out of the loop.

```cpp
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Poor: Recalculating invariants
void compute_poor(vector<double>& result, int n, double x, double y) {
    for (int i = 0; i < n; i++) {
        double factor = sqrt(x*x + y*y);  // Computed every iteration!
        double scale = sin(factor);       // Also invariant!
        result[i] = i * scale;
    }
}

// Better: Move invariants out
void compute_better(vector<double>& result, int n, double x, double y) {
    double factor = sqrt(x*x + y*y);  // Compute once!
    double scale = sin(factor);        // Compute once!

    for (int i = 0; i < n; i++) {
        result[i] = i * scale;
    }
}

// Example: Array size lookup
void process_poor(vector<int>& a, vector<int>& b) {
    for (size_t i = 0; i < a.size(); i++) {  // size() called each iteration
        b[i] = a[i] * a.size();  // size() again!
    }
}

void process_better(vector<int>& a, vector<int>& b) {
    size_t size = a.size();  // Cache it
    for (size_t i = 0; i < size; i++) {
        b[i] = a[i] * size;
    }
}

// Complex example: Nested loop invariants
void matrixMultiply_poor(const vector<vector<int>>& A,
                         const vector<vector<int>>& B,
                         vector<vector<int>>& C) {
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < B[0].size(); j++) {
            C[i][j] = 0;
            for (size_t k = 0; k < B.size(); k++) {
                // A.size(), B[0].size(), B.size() called repeatedly!
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrixMultiply_better(const vector<vector<int>>& A,
                           const vector<vector<int>>& B,
                           vector<vector<int>>& C) {
    size_t m = A.size();
    size_t n = B[0].size();
    size_t p = B.size();

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            int sum = 0;  // Local variable
            for (size_t k = 0; k < p; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    cout << "=== LOOP INVARIANT CODE MOTION ===\n\n";

    const int SIZE = 100000;
    vector<double> result(SIZE);

    auto start = chrono::high_resolution_clock::now();
    compute_poor(result, SIZE, 3.0, 4.0);
    auto end = chrono::high_resolution_clock::now();
    cout << "With invariants in loop: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    compute_better(result, SIZE, 3.0, 4.0);
    end = chrono::high_resolution_clock::now();
    cout << "Invariants hoisted:      "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    cout << "\nRule: If it doesn't change in the loop, move it out!\n";

    return 0;
}
```

---

## 6. Loop Interchange and Tiling

Optimize memory access patterns for better cache performance.

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Poor: Row-major access of column-major data (cache misses!)
void transpose_poor(vector<vector<int>>& matrix,
                    vector<vector<int>>& result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j][i] = matrix[i][j];  // Non-contiguous write
        }
    }
}

// Better: Interchange loops for better cache locality
void transpose_better(vector<vector<int>>& matrix,
                      vector<vector<int>>& result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix[j][i];  // Contiguous write
        }
    }
}

// Loop tiling (blocking) for large matrices
void matmul_tiled(const vector<vector<int>>& A,
                  const vector<vector<int>>& B,
                  vector<vector<int>>& C, int n) {
    const int TILE_SIZE = 64;  // Cache-friendly tile

    for (int i = 0; i < n; i += TILE_SIZE) {
        for (int j = 0; j < n; j += TILE_SIZE) {
            for (int k = 0; k < n; k += TILE_SIZE) {
                // Process tile
                int imax = min(i + TILE_SIZE, n);
                int jmax = min(j + TILE_SIZE, n);
                int kmax = min(k + TILE_SIZE, n);

                for (int ii = i; ii < imax; ii++) {
                    for (int jj = j; jj < jmax; jj++) {
                        int sum = C[ii][jj];
                        for (int kk = k; kk < kmax; kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] = sum;
                    }
                }
            }
        }
    }
}

void benchmarkInterchange() {
    const int N = 512;
    vector<vector<int>> matrix(N, vector<int>(N));
    vector<vector<int>> result(N, vector<int>(N));

    // Initialize
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = i * N + j;

    auto start = chrono::high_resolution_clock::now();
    transpose_poor(matrix, result, N);
    auto end = chrono::high_resolution_clock::now();
    cout << "Poor order:  " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    transpose_better(matrix, result, N);
    end = chrono::high_resolution_clock::now();
    cout << "Better order: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

int main() {
    cout << "=== LOOP INTERCHANGE AND TILING ===\n\n";
    benchmarkInterchange();

    cout << "\nKey Principles:\n";
    cout << "  • Access memory in contiguous order\n";
    cout << "  • Use loop tiling for large data sets\n";
    cout << "  • Match loop order to data layout\n";
    cout << "  • Tile size should fit in cache\n";

    return 0;
}
```

---

## 7. Vectorization and SIMD

Enable compiler vectorization for parallel processing.

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>  // SSE/AVX intrinsics

using namespace std;

// Scalar version
void add_scalar(const float* a, const float* b, float* c, int n) {
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

// Vectorized version (compiler can auto-vectorize this!)
void add_vectorizable(const float* __restrict a,
                      const float* __restrict b,
                      float* __restrict c, int n) {
    // __restrict tells compiler pointers don't overlap
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

// Manual SIMD (AVX - 8 floats at once)
void add_simd_avx(const float* a, const float* b, float* c, int n) {
    int i = 0;

    // Process 8 elements at a time
    for (; i + 7 < n; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&c[i], vc);
    }

    // Remaining elements
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void benchmarkVectorization() {
    const int SIZE = 10000000;
    vector<float> a(SIZE), b(SIZE), c(SIZE);

    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    auto test = [&](const string& name, auto func) {
        auto start = chrono::high_resolution_clock::now();
        func(a.data(), b.data(), c.data(), SIZE);
        auto end = chrono::high_resolution_clock::now();
        cout << name << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    };

    cout << "=== VECTORIZATION ===\n\n";
    test("Scalar      ", add_scalar);
    test("Vectorizable", add_vectorizable);
    test("SIMD (AVX)  ", add_simd_avx);
}

int main() {
    benchmarkVectorization();

    cout << "\nVectorization Tips:\n";
    cout << "  • Use restrict or __restrict for pointer parameters\n";
    cout << "  • Align data to 16/32-byte boundaries\n";
    cout << "  • Avoid complex control flow in loops\n";
    cout << "  • Use -O3 -march=native for best auto-vectorization\n";
    cout << "  • Check compiler reports (e.g., /Qvec-report)\n";

    return 0;
}
```

---

## 8. Cache-Friendly Loop Patterns

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Cache-unfriendly: stride access
void sum_stride_poor(vector<int>& data, int stride) {
    long long sum = 0;
    for (size_t i = 0; i < data.size(); i += stride) {
        sum += data[i];  // Large strides = cache misses
    }
}

// Cache-friendly: sequential access
void sum_sequential(vector<int>& data) {
    long long sum = 0;
    for (size_t i = 0; i < data.size(); i++) {
        sum += data[i];  // Sequential = cache hits
    }
}

// Matrix example: row vs column major
void sumRows(vector<vector<int>>& matrix) {
    for (size_t i = 0; i < matrix.size(); i++) {
        long long sum = 0;
        for (size_t j = 0; j < matrix[i].size(); j++) {
            sum += matrix[i][j];  // Cache-friendly (row-major)
        }
    }
}

void sumCols_poor(vector<vector<int>>& matrix) {
    for (size_t j = 0; j < matrix[0].size(); j++) {
        long long sum = 0;
        for (size_t i = 0; i < matrix.size(); i++) {
            sum += matrix[i][j];  // Cache-unfriendly!
        }
    }
}

int main() {
    cout << "=== CACHE-FRIENDLY PATTERNS ===\n\n";

    cout << "Best Practices:\n";
    cout << "  • Access memory sequentially when possible\n";
    cout << "  • Use structure-of-arrays instead of array-of-structures\n";
    cout << "  • Keep working set < cache size\n";
    cout << "  • Prefetch data when predictable\n";
    cout << "  • Align data structures\n";

    return 0;
}
```

---

## 9. Avoiding Loop Overhead

```cpp
#include <iostream>
#include <vector>

using namespace std;

// Iterator overhead
void process_iterator(vector<int>& data) {
    for (auto it = data.begin(); it != data.end(); ++it) {
        *it += 1;
    }
}

// Range-based for (cleaner, same performance)
void process_range(vector<int>& data) {
    for (int& val : data) {
        val += 1;
    }
}

// Index-based (explicit control)
void process_index(vector<int>& data) {
    size_t size = data.size();
    for (size_t i = 0; i < size; i++) {
        data[i] += 1;
    }
}

// Algorithm (best for simple operations)
void process_algorithm(vector<int>& data) {
    transform(data.begin(), data.end(), data.begin(),
              [](int x) { return x + 1; });
}

int main() {
    cout << "=== REDUCING LOOP OVERHEAD ===\n\n";

    cout << "Tips:\n";
    cout << "  • Use range-based for when possible (clean & fast)\n";
    cout << "  • Cache end iterators and sizes\n";
    cout << "  • Prefer ++i over i++ in loops\n";
    cout << "  • Consider STL algorithms for simple operations\n";

    return 0;
}
```

---

## 10. Exercises and Best Practices

### Loop Optimization Checklist

```cpp
/*
LOOP OPTIMIZATION CHECKLIST:

1. Move invariant code out of loops
   ❌ for (i = 0; i < n; i++) { x = sqrt(a*a + b*b); ... }
   ✅ x = sqrt(a*a + b*b); for (i = 0; i < n; i++) { ... }

2. Cache frequently-used values
   ❌ for (i = 0; i < arr.size(); i++) { ... }
   ✅ size_t size = arr.size(); for (i = 0; i < size; i++) { ... }

3. Use strength reduction
   ❌ for (i = 0; i < n; i++) { x = i * i * i; ... }
   ✅ Use incremental calculation or table lookup

4. Consider loop unrolling (manually or via compiler)
   ✅ Process multiple elements per iteration

5. Optimize memory access patterns
   ✅ Sequential > Random access
   ✅ Row-major for C++ arrays

6. Enable compiler optimizations
   ✅ Use -O3 -march=native (GCC/Clang)
   ✅ Use /O2 /arch:AVX2 (MSVC)

7. Use restrict/const for pointers
   ✅ Helps compiler optimize

8. Align data when using SIMD
   ✅ alignas(32) for AVX

9. Profile before optimizing
   ✅ Find actual bottlenecks first!

10. Let compiler help
    ✅ Check vectorization reports
    ✅ Use optimization pragmas when needed
*/
```

---

## Key Takeaways

1. ✅ **Move invariants out** of loops
2. ✅ **Cache sizes and end conditions**
3. ✅ **Use strength reduction** (cheaper operations)
4. ✅ **Unroll loops** for better throughput
5. ✅ **Optimize memory access** patterns
6. ✅ **Enable vectorization** when possible
7. ✅ **Fuse loops** for better cache locality
8. ✅ **Profile first**, optimize second

---

## Summary Checklist

Before moving to Lesson 25, ensure you can:
- [ ] Identify loop invariant code
- [ ] Apply loop unrolling techniques
- [ ] Use loop fusion for cache optimization
- [ ] Implement strength reduction
- [ ] Optimize memory access patterns
- [ ] Enable compiler vectorization
- [ ] Measure loop performance improvements

**Congratulations on mastering Loop Optimization!**

Next lesson: Function Inlining Strategies
