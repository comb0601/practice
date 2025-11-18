# Lesson 94: SIMD & Vectorization
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to SIMD](#introduction)
2. [SIMD Instruction Sets](#simd-instruction-sets)
3. [SSE Intrinsics](#sse-intrinsics)
4. [AVX Intrinsics](#avx-intrinsics)
5. [Vectorizing Loops](#vectorizing-loops)
6. [Data Parallel Operations](#data-parallel-operations)
7. [SIMD for Graphics](#simd-for-graphics)
8. [Compiler Auto-Vectorization](#compiler-vectorization)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction to SIMD

**SIMD**: Single Instruction, Multiple Data

Process multiple data elements with one instruction:

```
Scalar (No SIMD):  1 operation per instruction
a = b + c;    // 1 addition

SIMD (SSE):  4 operations per instruction
[a0,a1,a2,a3] = [b0,b1,b2,b3] + [c0,c1,c2,c3];  // 4 additions

Result: 4x speedup!
```

### SIMD Benefits

**4-8x Speedup** for data-parallel operations:
- Vector math (graphics)
- Image processing
- Audio processing
- Physics simulations
- Machine learning

## SIMD Instruction Sets

### Evolution of x86 SIMD

| ISA | Year | Register Size | Float Operations | Int Operations |
|-----|------|--------------|------------------|----------------|
| **MMX** | 1996 | 64-bit | 0 | 8×8-bit |
| **SSE** | 1999 | 128-bit | 4×float | 4×int32 |
| **SSE2** | 2001 | 128-bit | 2×double | 2×int64 |
| **AVX** | 2011 | 256-bit | 8×float | 8×int32 |
| **AVX2** | 2013 | 256-bit | 8×float | 8×int32 (improved) |
| **AVX-512** | 2016 | 512-bit | 16×float | 16×int32 |

### Register Widths

```
SSE:      128 bits  [████████████████]
AVX:      256 bits  [████████████████████████████████]
AVX-512:  512 bits  [████████████████████████████████████████████████████████████████]

Example with floats (4 bytes each):
SSE:      4 floats
AVX:      8 floats
AVX-512:  16 floats
```

## SSE Intrinsics

### Including SSE

```cpp
#include <xmmintrin.h>  // SSE
#include <emmintrin.h>  // SSE2
#include <pmmintrin.h>  // SSE3
#include <smmintrin.h>  // SSE4.1
```

### SSE Data Types

```cpp
__m128  // 4 floats
__m128d // 2 doubles
__m128i // 4 int32, 8 int16, 16 int8
```

### Basic SSE Operations

**Load/Store:**
```cpp
float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};

// Load from memory (aligned)
__m128 vec = _mm_load_ps(a);

// Load from memory (unaligned)
__m128 vec2 = _mm_loadu_ps(a);

// Store to memory
_mm_store_ps(a, vec);
```

**Arithmetic:**
```cpp
__m128 a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);  // Set values
__m128 b = _mm_set_ps(8.0f, 7.0f, 6.0f, 5.0f);

__m128 add = _mm_add_ps(a, b);  // [9, 9, 9, 9]
__m128 sub = _mm_sub_ps(a, b);  // [-3, -3, -3, -3]
__m128 mul = _mm_mul_ps(a, b);  // [40, 21, 12, 5]
__m128 div = _mm_div_ps(a, b);  // [0.5, 0.43, 0.33, 0.2]
```

**Math Functions:**
```cpp
__m128 sqrt_val = _mm_sqrt_ps(a);    // Square root
__m128 min_val = _mm_min_ps(a, b);   // Minimum
__m128 max_val = _mm_max_ps(a, b);   // Maximum
```

### Example: Vector Addition (4x Speedup)

**Scalar Version:**
```cpp
void vectorAddScalar(float* a, float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}
// Processes 1 element per iteration
```

**SSE Version:**
```cpp
void vectorAddSSE(float* a, float* b, float* c, int n) {
    for (int i = 0; i < n; i += 4) {
        __m128 va = _mm_load_ps(&a[i]);
        __m128 vb = _mm_load_ps(&b[i]);
        __m128 vc = _mm_add_ps(va, vb);
        _mm_store_ps(&c[i], vc);
    }
}
// Processes 4 elements per iteration = 4x faster!
```

## AVX Intrinsics

### AVX: 256-bit SIMD

```cpp
#include <immintrin.h>  // AVX

__m256  // 8 floats
__m256d // 4 doubles
__m256i // 8 int32
```

### AVX Operations

```cpp
float a[8] = {1,2,3,4,5,6,7,8};
float b[8] = {8,7,6,5,4,3,2,1};

// Load 8 floats
__m256 va = _mm256_load_ps(a);
__m256 vb = _mm256_load_ps(b);

// Add 8 floats in parallel
__m256 vc = _mm256_add_ps(va, vb);  // [9,9,9,9,9,9,9,9]

// Store result
float c[8];
_mm256_store_ps(c, vc);
```

### Example: Dot Product

**Scalar:**
```cpp
float dotScalar(float* a, float* b, int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
```

**AVX:**
```cpp
float dotAVX(float* a, float* b, int n) {
    __m256 sum = _mm256_setzero_ps();

    for (int i = 0; i < n; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 prod = _mm256_mul_ps(va, vb);
        sum = _mm256_add_ps(sum, prod);
    }

    // Horizontal sum
    alignas(32) float temp[8];
    _mm256_store_ps(temp, sum);
    return temp[0] + temp[1] + temp[2] + temp[3] +
           temp[4] + temp[5] + temp[6] + temp[7];
}
// 8x faster than scalar!
```

## Vectorizing Loops

### Requirements for Vectorization

1. **No dependencies between iterations**
```cpp
// CAN vectorize
for (int i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];  // No dependency
}

// CANNOT vectorize
for (int i = 1; i < n; ++i) {
    a[i] = a[i-1] + b[i];  // Depends on previous iteration
}
```

2. **Aligned memory access (preferred)**
```cpp
alignas(16) float data[1000];  // SSE: 16-byte alignment
alignas(32) float data[1000];  // AVX: 32-byte alignment
```

3. **Known loop count at compile time (helps)**

### Auto-Vectorization Hints

```cpp
// Tell compiler to vectorize
#pragma omp simd
for (int i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
}

// Microsoft specific
#pragma loop(ivdep)  // Ignore vector dependencies
for (int i = 0; i < n; ++i) {
    c[i] = a[i] * 2.0f;
}
```

## Data Parallel Operations

### Image Processing Example

**Brighten Image (Scalar):**
```cpp
void brightenScalar(unsigned char* pixels, int count, int amount) {
    for (int i = 0; i < count; ++i) {
        pixels[i] = min(pixels[i] + amount, 255);
    }
}
```

**Brighten Image (SSE):**
```cpp
void brightenSSE(unsigned char* pixels, int count, int amount) {
    __m128i amt = _mm_set1_epi8(amount);

    for (int i = 0; i < count; i += 16) {
        __m128i px = _mm_loadu_si128((__m128i*)&pixels[i]);
        px = _mm_adds_epu8(px, amt);  // Saturating add (clamps at 255)
        _mm_storeu_si128((__m128i*)&pixels[i], px);
    }
}
// Processes 16 pixels at once = 16x faster!
```

### Audio Processing Example

**Apply Gain (Scalar):**
```cpp
void applyGainScalar(float* samples, int count, float gain) {
    for (int i = 0; i < count; ++i) {
        samples[i] *= gain;
    }
}
```

**Apply Gain (AVX):**
```cpp
void applyGainAVX(float* samples, int count, float gain) {
    __m256 vgain = _mm256_set1_ps(gain);

    for (int i = 0; i < count; i += 8) {
        __m256 samp = _mm256_load_ps(&samples[i]);
        samp = _mm256_mul_ps(samp, vgain);
        _mm256_store_ps(&samples[i], samp);
    }
}
```

## SIMD for Graphics

### 3D Vector Operations

**Vector3 Structure (SIMD-friendly):**
```cpp
struct alignas(16) Vector3 {
    float x, y, z, w;  // w is padding for SSE alignment

    Vector3 operator+(const Vector3& other) const {
        Vector3 result;
        __m128 a = _mm_load_ps(&x);
        __m128 b = _mm_load_ps(&other.x);
        __m128 c = _mm_add_ps(a, b);
        _mm_store_ps(&result.x, c);
        return result;
    }

    Vector3 operator*(float scalar) const {
        Vector3 result;
        __m128 v = _mm_load_ps(&x);
        __m128 s = _mm_set1_ps(scalar);
        __m128 r = _mm_mul_ps(v, s);
        _mm_store_ps(&result.x, r);
        return result;
    }

    float dot(const Vector3& other) const {
        __m128 a = _mm_load_ps(&x);
        __m128 b = _mm_load_ps(&other.x);
        __m128 mul = _mm_mul_ps(a, b);
        __m128 sum = _mm_hadd_ps(mul, mul);
        sum = _mm_hadd_ps(sum, sum);
        return _mm_cvtss_f32(sum);
    }
};
```

### Matrix-Vector Multiplication

```cpp
// 4x4 matrix * 4D vector using SSE
__m128 matrixVectorMultiply(float* matrix, __m128 vec) {
    __m128 row0 = _mm_load_ps(&matrix[0]);
    __m128 row1 = _mm_load_ps(&matrix[4]);
    __m128 row2 = _mm_load_ps(&matrix[8]);
    __m128 row3 = _mm_load_ps(&matrix[12]);

    __m128 x = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(0,0,0,0));
    __m128 y = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1,1,1,1));
    __m128 z = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2,2,2,2));
    __m128 w = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3,3,3,3));

    __m128 result = _mm_mul_ps(row0, x);
    result = _mm_add_ps(result, _mm_mul_ps(row1, y));
    result = _mm_add_ps(result, _mm_mul_ps(row2, z));
    result = _mm_add_ps(result, _mm_mul_ps(row3, w));

    return result;
}
```

## Compiler Auto-Vectorization

### Compiler Flags

**Visual Studio:**
```
/arch:AVX   - Enable AVX
/arch:AVX2  - Enable AVX2
/O2         - Enables auto-vectorization
```

**GCC/Clang:**
```bash
-mavx       # Enable AVX
-mavx2      # Enable AVX2
-O3         # Aggressive optimization + vectorization
-ftree-vectorize  # Explicit vectorization
```

### Checking Vectorization

**Visual Studio:**
```cpp
// Check generated assembly:
// View → Other Windows → Disassembly
// Look for instructions like: vaddps, vmulps (AVX)
```

**GCC:**
```bash
g++ -O3 -mavx2 -ftree-vectorizer-verbose=2 file.cpp
# Prints vectorization report
```

### Best Practices

1. **Use simple loops** - Complex loops harder to vectorize
2. **Align data** - 16/32-byte alignment
3. **Avoid branches** - Use min/max instead
4. **Minimize dependencies** - Independent iterations
5. **Use restrict keyword** - No pointer aliasing

```cpp
void example(float* __restrict a, float* __restrict b, float* __restrict c, int n) {
    // __restrict tells compiler pointers don't overlap
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];  // Easier to vectorize
    }
}
```

## Summary

**Key Takeaways**:
1. **SIMD gives 4-16x speedup** for data-parallel code
2. **SSE: 4 floats, AVX: 8 floats, AVX-512: 16 floats**
3. **Use intrinsics** for explicit control
4. **Let compiler auto-vectorize** simple loops
5. **Align data** for best performance
6. **Best for**: Graphics, image/audio processing, physics

**When to Use SIMD**:
- ✓ Large arrays of data
- ✓ Same operation on many elements
- ✓ Performance-critical code
- ✗ Branchy code
- ✗ Dependent iterations
- ✗ Small data sets

---

**Congratulations on completing Lesson 94!**
