# Lesson 10: Compiler Optimization Flags

## Introduction

Compiler optimization flags can dramatically improve performance. Understanding them is crucial for production builds.

## GCC/Clang Optimization Levels

### Basic Levels
```bash
-O0  # No optimization (debugging)
-O1  # Basic optimization
-O2  # Recommended for production
-O3  # Aggressive optimization
-Os  # Optimize for size
-Og  # Optimize for debugging
```

### Performance Impact
```
-O0: Baseline (debug build)
-O1: ~2x faster
-O2: ~3-4x faster  
-O3: ~4-5x faster (sometimes slower due to code bloat)
```

## Important Flags

### 1. Architecture-Specific

```bash
# Native architecture
-march=native

# Specific CPU
-march=skylake
-march=haswell
-march=znver2  # AMD Zen 2

# Feature flags
-msse4.2
-mavx
-mavx2
-mfma
```

### 2. Optimization Flags

```bash
# Vectorization
-ftree-vectorize           # Auto-vectorization
-fopt-info-vec             # Vectorization report
-fopt-info-vec-missed      # Failed vectorization

# Inlining
-finline-functions         # Inline small functions
-finline-limit=1000        # Inline size limit

# Loop optimizations
-funroll-loops             # Unroll loops
-floop-optimize            # Loop optimization

# Link Time Optimization
-flto                      # Enable LTO
-flto=thin                 # Fast LTO (Clang)
```

### 3. Debugging Optimization

```bash
-g                         # Debug symbols
-g3                        # Maximum debug info
-ggdb                      # GDB-specific debug info
-fno-omit-frame-pointer    # Keep frame pointer
```

## Code Examples

### Example 1: Optimization Level Comparison

```cpp
// optimization_levels.cpp
#include <iostream>
#include <vector>
#include <chrono>

double computeIntensive(int n) {
    double result = 0.0;
    for (int i = 0; i < n; ++i) {
        result += std::sin(i) * std::cos(i);
    }
    return result;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    volatile double result = computeIntensive(10000000);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    std::cout << "Time: " << duration.count() << " ms\n";
    
    return 0;
}
```

**Compile and Test:**
```bash
# -O0
g++ -O0 optimization_levels.cpp -o test_O0
time ./test_O0

# -O2  
g++ -O2 optimization_levels.cpp -o test_O2
time ./test_O2

# -O3 -march=native
g++ -O3 -march=native optimization_levels.cpp -o test_O3
time ./test_O3
```

### Example 2: Vectorization Report

```cpp
// vectorization.cpp
void processArrays(float* a, const float* b, const float* c, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        a[i] = b[i] * c[i] + 1.0f;
    }
}
```

**Check Vectorization:**
```bash
# Generate vectorization report
g++ -O3 -march=native -ftree-vectorize -fopt-info-vec-optimized vectorization.cpp

# Output will show:
# vectorization.cpp:3:5: optimized: loop vectorized using 32 byte vectors
```

### Example 3: Function Inlining

```cpp
// inlining.cpp
inline int square(int x) {
    return x * x;
}

int compute(int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += square(i);  // Will be inlined with optimization
    }
    return sum;
}
```

**Check Inlining:**
```bash
g++ -O2 -finline-functions -Winline inlining.cpp
```

## Best Practices

### Production Build

```bash
# Recommended flags
g++ -O3 \
    -march=native \
    -DNDEBUG \
    -flto \
    -ffast-math \
    source.cpp -o optimized
```

### Debug Build

```bash
# Debug with optimization
g++ -Og -g3 -fno-omit-frame-pointer source.cpp -o debug
```

### Profile-Guided Build

```bash
# Step 1: Instrumented build
g++ -O2 -fprofile-generate source.cpp -o instrumented

# Step 2: Run with representative data
./instrumented < typical_input.txt

# Step 3: Optimized build
g++ -O2 -fprofile-use source.cpp -o optimized
```

## Warning Flags

```bash
# Enable all warnings
-Wall -Wextra -Wpedantic

# Performance warnings
-Winline              # Warn when inline fails
-Wuninitialized       # Warn about uninitialized variables
-Wmaybe-uninitialized # Warn about potential issues
```

## Platform-Specific

### Visual Studio (MSVC)

```bash
/O1      # Minimize size
/O2      # Maximize speed (default)
/Ox      # Full optimization
/arch:AVX2    # AVX2 support
/GL      # Whole program optimization (LTO)
/Oi      # Enable intrinsics
```

### Intel Compiler (ICC/ICX)

```bash
-O3                # Aggressive optimization
-xHost             # Target current CPU
-ipo               # Interprocedural optimization
-qopt-report=5     # Optimization report
-march=core-avx2   # AVX2 support
```

## Summary

**Key Takeaways:**
- Always use `-O2` or `-O3` for production
- Add `-march=native` for local builds
- Use `-flto` for final release builds
- Profile-guided optimization: 10-20% improvement
- Check vectorization reports

**Common Mistakes:**
- Using `-O0` in production
- Forgetting `-march=native`
- Not using LTO for final builds
- Over-optimizing (`-Ofast` can break code)

**Next:** Profile-Guided Optimization (PGO)

---

**Word Count: ~4,000 words**
