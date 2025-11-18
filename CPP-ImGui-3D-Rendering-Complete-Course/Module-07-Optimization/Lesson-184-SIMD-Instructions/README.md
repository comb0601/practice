# Lesson 184: SIMD Instructions (SSE/AVX)
**Duration: 3 hours**
**Module 7: Optimization and Performance**

## Learning Objectives
By the end of this lesson, you will:
- Understand what SIMD is and why it's crucial for graphics
- Use SSE intrinsics for 4x speedups
- Implement AVX for 8x speedups
- Optimize vector and matrix operations
- Write SIMD-optimized math libraries
- Understand alignment requirements
- Measure and verify performance gains

## 1. What is SIMD?

**SIMD** = Single Instruction, Multiple Data

### Traditional (Scalar) Processing

```cpp
// Process one value at a time
float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};
float result[4];

for (int i = 0; i < 4; i++) {
    result[i] = a[i] + b[i];  // 4 separate additions
}
// 4 CPU cycles (approximately)
```

### SIMD Processing

```cpp
// Process 4 values simultaneously
__m128 va = _mm_load_ps(a);      // Load 4 floats
__m128 vb = _mm_load_ps(b);      // Load 4 floats
__m128 vresult = _mm_add_ps(va, vb);  // Add all 4 at once!
_mm_store_ps(result, vresult);   // Store 4 floats
// 1 CPU cycle (approximately)
```

### Visual Representation

```
Scalar:
┌─────┐    ┌─────┐    ┌─────┐
│  +  │ →  │  +  │ →  │  +  │ →  │  +  │  (Sequential)
└─────┘    └─────┘    └─────┘    └─────┘

SIMD:
┌─────┬─────┬─────┬─────┐
│  +  │  +  │  +  │  +  │  (Parallel)
└─────┴─────┴─────┴─────┘
```

## 2. SSE (Streaming SIMD Extensions)

### SSE Data Types

```cpp
#include <immintrin.h>  // Includes all SIMD intrinsics

// SSE types - 128-bit registers (4 floats)
__m128  // 4 x 32-bit floats
__m128d // 2 x 64-bit doubles
__m128i // 4 x 32-bit integers (or 8x16-bit, 16x8-bit)

// AVX types - 256-bit registers (8 floats)
__m256  // 8 x 32-bit floats
__m256d // 4 x 64-bit doubles
__m256i // 8 x 32-bit integers
```

### Basic SSE Operations

```cpp
#include <immintrin.h>
#include <iostream>

void sse_example() {
    // Create arrays (must be 16-byte aligned for _mm_load_ps)
    alignas(16) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    alignas(16) float result[4];

    // Load data into SIMD registers
    __m128 va = _mm_load_ps(a);  // Load 4 floats
    __m128 vb = _mm_load_ps(b);

    // Perform operations
    __m128 sum = _mm_add_ps(va, vb);      // Add
    __m128 diff = _mm_sub_ps(va, vb);     // Subtract
    __m128 prod = _mm_mul_ps(va, vb);     // Multiply
    __m128 quot = _mm_div_ps(va, vb);     // Divide

    // Store result
    _mm_store_ps(result, sum);

    // Print results
    std::cout << "Sum: ";
    for (int i = 0; i < 4; i++) {
        std::cout << result[i] << " ";  // 6 8 10 12
    }
    std::cout << std::endl;
}
```

## 3. Vector Math with SIMD

### 3D Vector Addition

```cpp
#include <immintrin.h>
#include <iostream>

struct Vector3 {
    float x, y, z;
};

// Scalar version
Vector3 add_scalar(const Vector3& a, const Vector3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// SIMD version
Vector3 add_simd(const Vector3& a, const Vector3& b) {
    // Load vectors (x, y, z, 0)
    __m128 va = _mm_setr_ps(a.x, a.y, a.z, 0.0f);
    __m128 vb = _mm_setr_ps(b.x, b.y, b.z, 0.0f);

    // Add all components at once
    __m128 result = _mm_add_ps(va, vb);

    // Extract results
    alignas(16) float temp[4];
    _mm_store_ps(temp, result);

    return {temp[0], temp[1], temp[2]};
}

// Even better: Process multiple vectors at once
void add_many_vectors_simd(const Vector3* a, const Vector3* b, Vector3* result, int count) {
    for (int i = 0; i < count; i++) {
        __m128 va = _mm_setr_ps(a[i].x, a[i].y, a[i].z, 0.0f);
        __m128 vb = _mm_setr_ps(b[i].x, b[i].y, b[i].z, 0.0f);
        __m128 vr = _mm_add_ps(va, vb);

        alignas(16) float temp[4];
        _mm_store_ps(temp, vr);
        result[i] = {temp[0], temp[1], temp[2]};
    }
}
```

### Dot Product

```cpp
// Scalar dot product
float dot_scalar(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// SIMD dot product (SSE4.1+)
float dot_simd(const Vector3& a, const Vector3& b) {
    __m128 va = _mm_setr_ps(a.x, a.y, a.z, 0.0f);
    __m128 vb = _mm_setr_ps(b.x, b.y, b.z, 0.0f);

    // Multiply
    __m128 mul = _mm_mul_ps(va, vb);

    // Horizontal add (sum all components)
    __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sums = _mm_add_ps(mul, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);

    return _mm_cvtss_f32(sums);
}

// Alternative using SSE3 horizontal add
float dot_simd_sse3(const Vector3& a, const Vector3& b) {
    __m128 va = _mm_setr_ps(a.x, a.y, a.z, 0.0f);
    __m128 vb = _mm_setr_ps(b.x, b.y, b.z, 0.0f);

    __m128 mul = _mm_mul_ps(va, vb);
    __m128 hadd1 = _mm_hadd_ps(mul, mul);
    __m128 hadd2 = _mm_hadd_ps(hadd1, hadd1);

    return _mm_cvtss_f32(hadd2);
}
```

### Vector Length (Magnitude)

```cpp
#include <cmath>

// Scalar version
float length_scalar(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// SIMD version
float length_simd(const Vector3& v) {
    __m128 vec = _mm_setr_ps(v.x, v.y, v.z, 0.0f);

    // Square all components
    __m128 squared = _mm_mul_ps(vec, vec);

    // Sum components
    __m128 shuf = _mm_shuffle_ps(squared, squared, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sums = _mm_add_ps(squared, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);

    // Square root
    __m128 result = _mm_sqrt_ss(sums);

    return _mm_cvtss_f32(result);
}

// Fast inverse square root (for normalization)
float inv_sqrt_simd(float x) {
    __m128 vec = _mm_set_ss(x);
    __m128 result = _mm_rsqrt_ss(vec);  // Fast reciprocal square root
    return _mm_cvtss_f32(result);
    // Note: Less accurate than 1.0f / sqrt(x), but much faster!
}
```

## 4. Matrix Operations with SIMD

### 4x4 Matrix Multiplication

```cpp
struct Matrix4x4 {
    float m[16];  // Column-major order

    // Constructor
    Matrix4x4() {
        for (int i = 0; i < 16; i++) m[i] = 0.0f;
    }
};

// Scalar matrix multiplication
Matrix4x4 multiply_scalar(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += a.m[row + i * 4] * b.m[i + col * 4];
            }
            result.m[row + col * 4] = sum;
        }
    }

    return result;
}

// SIMD matrix multiplication (much faster!)
Matrix4x4 multiply_simd(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result;

    // Load columns of matrix B
    __m128 b0 = _mm_load_ps(&b.m[0]);
    __m128 b1 = _mm_load_ps(&b.m[4]);
    __m128 b2 = _mm_load_ps(&b.m[8]);
    __m128 b3 = _mm_load_ps(&b.m[12]);

    // Process each column of result
    for (int i = 0; i < 4; i++) {
        // Load row elements from A
        __m128 a0 = _mm_set1_ps(a.m[i + 0]);
        __m128 a1 = _mm_set1_ps(a.m[i + 4]);
        __m128 a2 = _mm_set1_ps(a.m[i + 8]);
        __m128 a3 = _mm_set1_ps(a.m[i + 12]);

        // Multiply and accumulate
        __m128 r = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1)),
            _mm_add_ps(_mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3))
        );

        _mm_store_ps(&result.m[i], r);
    }

    return result;
}
```

## 5. AVX - Advanced Vector Extensions

### 8-way Parallel Operations

```cpp
#include <immintrin.h>

// AVX processes 8 floats at once (256-bit)
void avx_example() {
    alignas(32) float a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    alignas(32) float b[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    alignas(32) float result[8];

    // Load 8 floats
    __m256 va = _mm256_load_ps(a);
    __m256 vb = _mm256_load_ps(b);

    // Add all 8 at once
    __m256 sum = _mm256_add_ps(va, vb);

    // Store result
    _mm256_store_ps(result, sum);

    for (int i = 0; i < 8; i++) {
        std::cout << result[i] << " ";  // 9 9 9 9 9 9 9 9
    }
    std::cout << std::endl;
}

// Process 1000 floats with AVX
void process_array_avx(float* data, int count) {
    int i = 0;

    // Process 8 at a time
    for (; i <= count - 8; i += 8) {
        __m256 vec = _mm256_load_ps(&data[i]);

        // Do something (example: square all values)
        vec = _mm256_mul_ps(vec, vec);

        _mm256_store_ps(&data[i], vec);
    }

    // Handle remaining elements (< 8)
    for (; i < count; i++) {
        data[i] = data[i] * data[i];
    }
}
```

## 6. Practical Example: Particle System

### Optimizing Particle Updates

```cpp
struct Particle {
    float x, y, z;      // Position
    float vx, vy, vz;   // Velocity
};

// Scalar version
void update_particles_scalar(Particle* particles, int count, float dt) {
    for (int i = 0; i < count; i++) {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
        particles[i].z += particles[i].vz * dt;
    }
}

// SIMD version - Structure of Arrays (SoA) layout
struct ParticlesSoA {
    float* x;   // All x positions
    float* y;   // All y positions
    float* z;   // All z positions
    float* vx;  // All x velocities
    float* vy;  // All y velocities
    float* vz;  // All z velocities
    int count;
};

void update_particles_simd(ParticlesSoA& particles, float dt) {
    __m128 dt_vec = _mm_set1_ps(dt);  // Broadcast dt to all 4 slots

    int i = 0;
    // Process 4 particles at once
    for (; i <= particles.count - 4; i += 4) {
        // Load positions and velocities
        __m128 x = _mm_load_ps(&particles.x[i]);
        __m128 y = _mm_load_ps(&particles.y[i]);
        __m128 z = _mm_load_ps(&particles.z[i]);

        __m128 vx = _mm_load_ps(&particles.vx[i]);
        __m128 vy = _mm_load_ps(&particles.vy[i]);
        __m128 vz = _mm_load_ps(&particles.vz[i]);

        // Update: position += velocity * dt
        x = _mm_add_ps(x, _mm_mul_ps(vx, dt_vec));
        y = _mm_add_ps(y, _mm_mul_ps(vy, dt_vec));
        z = _mm_add_ps(z, _mm_mul_ps(vz, dt_vec));

        // Store updated positions
        _mm_store_ps(&particles.x[i], x);
        _mm_store_ps(&particles.y[i], y);
        _mm_store_ps(&particles.z[i], z);
    }

    // Handle remaining particles
    for (; i < particles.count; i++) {
        particles.x[i] += particles.vx[i] * dt;
        particles.y[i] += particles.vy[i] * dt;
        particles.z[i] += particles.vz[i] * dt;
    }
}
```

## 7. Alignment Requirements

### Why Alignment Matters

```cpp
// Unaligned data - SLOW or crashes with _mm_load_ps
float data[4] = {1, 2, 3, 4};

// Aligned data - FAST
alignas(16) float aligned_data[4] = {1, 2, 3, 4};

// Or dynamically allocate aligned memory
float* aligned_ptr = (float*)_mm_malloc(16 * sizeof(float), 16);
// ... use aligned_ptr ...
_mm_free(aligned_ptr);
```

### Load Functions

```cpp
// _mm_load_ps: Requires 16-byte alignment (FAST)
__m128 vec = _mm_load_ps(aligned_data);

// _mm_loadu_ps: Works with unaligned data (SLOWER)
__m128 vec2 = _mm_loadu_ps(unaligned_data);

// Same for AVX (32-byte alignment)
__m256 avx_aligned = _mm256_load_ps(aligned_32);    // Aligned
__m256 avx_unaligned = _mm256_loadu_ps(unaligned);  // Unaligned
```

## 8. Performance Benchmarking

### Complete Benchmark Code

```cpp
#include <immintrin.h>
#include <chrono>
#include <iostream>
#include <vector>

// Benchmark helper
template<typename Func>
double benchmark(Func f, int iterations = 1000000) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {
        f();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    return diff.count();
}

void run_benchmarks() {
    const int SIZE = 1000;
    alignas(16) float a[SIZE], b[SIZE], result[SIZE];

    // Initialize arrays
    for (int i = 0; i < SIZE; i++) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(i * 2);
    }

    // Scalar version
    auto scalar_add = [&]() {
        for (int i = 0; i < SIZE; i++) {
            result[i] = a[i] + b[i];
        }
    };

    // SIMD version
    auto simd_add = [&]() {
        for (int i = 0; i < SIZE; i += 4) {
            __m128 va = _mm_load_ps(&a[i]);
            __m128 vb = _mm_load_ps(&b[i]);
            __m128 vr = _mm_add_ps(va, vb);
            _mm_store_ps(&result[i], vr);
        }
    };

    double scalar_time = benchmark(scalar_add);
    double simd_time = benchmark(simd_add);

    std::cout << "Scalar time: " << scalar_time << "s" << std::endl;
    std::cout << "SIMD time:   " << simd_time << "s" << std::endl;
    std::cout << "Speedup:     " << (scalar_time / simd_time) << "x" << std::endl;
}

int main() {
    run_benchmarks();
    return 0;
}
```

### Expected Output

```
Scalar time: 0.523s
SIMD time:   0.142s
Speedup:     3.68x
```

## 9. Real-World SIMD Math Library

### Complete Vec3 Class with SIMD

```cpp
#include <immintrin.h>
#include <cmath>

class alignas(16) Vec3 {
public:
    union {
        struct { float x, y, z, w; };  // w is padding for alignment
        __m128 simd;
    };

    Vec3() : x(0), y(0), z(0), w(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z), w(0) {}

    // Addition
    Vec3 operator+(const Vec3& other) const {
        Vec3 result;
        result.simd = _mm_add_ps(simd, other.simd);
        return result;
    }

    // Subtraction
    Vec3 operator-(const Vec3& other) const {
        Vec3 result;
        result.simd = _mm_sub_ps(simd, other.simd);
        return result;
    }

    // Scalar multiplication
    Vec3 operator*(float scalar) const {
        Vec3 result;
        __m128 s = _mm_set1_ps(scalar);
        result.simd = _mm_mul_ps(simd, s);
        return result;
    }

    // Dot product
    float dot(const Vec3& other) const {
        __m128 mul = _mm_mul_ps(simd, other.simd);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
    }

    // Length
    float length() const {
        float dot_product = dot(*this);
        return std::sqrt(dot_product);
    }

    // Normalize
    Vec3 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return *this * (1.0f / len);
        }
        return *this;
    }

    // Fast normalize using rsqrt
    Vec3 normalized_fast() const {
        float dot_product = dot(*this);
        __m128 len = _mm_set_ss(dot_product);
        __m128 inv_len = _mm_rsqrt_ss(len);  // Fast reciprocal sqrt

        Vec3 result;
        __m128 scale = _mm_shuffle_ps(inv_len, inv_len, 0);
        result.simd = _mm_mul_ps(simd, scale);
        return result;
    }
};
```

## 10. Compiler Intrinsics Reference

### SSE Operations

```cpp
// Arithmetic
_mm_add_ps(a, b)    // Add
_mm_sub_ps(a, b)    // Subtract
_mm_mul_ps(a, b)    // Multiply
_mm_div_ps(a, b)    // Divide
_mm_sqrt_ps(a)      // Square root
_mm_rsqrt_ps(a)     // Reciprocal square root (fast)

// Load/Store
_mm_load_ps(ptr)    // Load aligned
_mm_loadu_ps(ptr)   // Load unaligned
_mm_store_ps(ptr, a)// Store aligned
_mm_storeu_ps(ptr, a)// Store unaligned

// Set
_mm_set_ps(e3, e2, e1, e0)  // Set individual values
_mm_set1_ps(a)               // Broadcast value
_mm_setzero_ps()            // Set to zero

// Comparison
_mm_cmpeq_ps(a, b)  // Equal
_mm_cmplt_ps(a, b)  // Less than
_mm_cmpgt_ps(a, b)  // Greater than

// Min/Max
_mm_min_ps(a, b)    // Minimum
_mm_max_ps(a, b)    // Maximum

// Shuffle
_mm_shuffle_ps(a, b, mask)  // Shuffle elements
_mm_unpacklo_ps(a, b)       // Unpack low
_mm_unpackhi_ps(a, b)       // Unpack high
```

## 11. Common Pitfalls

### Pitfall 1: Ignoring Alignment

```cpp
// BAD - May crash!
float data[4] = {1, 2, 3, 4};
__m128 vec = _mm_load_ps(data);  // Crash if not aligned!

// GOOD
alignas(16) float data[4] = {1, 2, 3, 4};
__m128 vec = _mm_load_ps(data);  // Safe

// ALTERNATIVE
__m128 vec = _mm_loadu_ps(data);  // Works but slower
```

### Pitfall 2: Structure of Arrays vs Array of Structures

```cpp
// BAD for SIMD - Array of Structures (AoS)
struct Particle {
    float x, y, z, vx, vy, vz;
};
Particle particles[1000];  // Hard to vectorize

// GOOD for SIMD - Structure of Arrays (SoA)
struct Particles {
    float x[1000], y[1000], z[1000];
    float vx[1000], vy[1000], vz[1000];
};  // Easy to vectorize!
```

### Pitfall 3: Not Handling Remainders

```cpp
// BAD - What if count is not divisible by 4?
for (int i = 0; i < count; i += 4) {
    // Process 4 elements
    // Crashes if count = 10!
}

// GOOD
int i = 0;
for (; i <= count - 4; i += 4) {
    // SIMD processing
}
for (; i < count; i++) {
    // Scalar processing for remainder
}
```

## 12. Exercises

### Exercise 1: Vector Cross Product
Implement cross product using SIMD intrinsics.

### Exercise 2: Matrix Transpose
Write a 4x4 matrix transpose using SIMD.

### Exercise 3: Distance Calculations
Calculate distances between many points using SIMD.

### Exercise 4: Benchmark Comparison
Compare scalar vs SSE vs AVX for array operations.

## Summary

### Key Takeaways

✓ **SIMD provides massive speedups** (2-8x for appropriate workloads)
✓ **SSE processes 4 floats**, AVX processes 8 floats
✓ **Alignment is crucial** for best performance
✓ **Structure of Arrays** is better than Array of Structures for SIMD
✓ **Handle remainders** when array size isn't divisible
✓ **Use intrinsics** for explicit control, trust compiler autovectorization for simple loops

### When to Use SIMD

✓ Vector/matrix math
✓ Particle systems
✓ Physics simulations
✓ Image processing
✓ Audio processing
✓ Large array operations

## Checklist

- [ ] Understand SIMD concepts
- [ ] Can use SSE intrinsics
- [ ] Can use AVX intrinsics
- [ ] Understand alignment requirements
- [ ] Can optimize vector math
- [ ] Can optimize matrix operations
- [ ] Can benchmark SIMD code
- [ ] Completed exercises

---

**Time to complete**: 3 hours
**Previous lesson**: [Lesson 183 - Data-Oriented Design](../Lesson-183-Data-Oriented-Design/README.md)
**Next lesson**: [Lesson 185 - Multithreading for Game Engines](../Lesson-185-Multithreading/README.md)

**Congratulations!** You now have the tools to write high-performance SIMD code for graphics and game engines!
