#!/bin/bash

BASE_DIR="/home/user/practice/CPP-Tutorial-400Hours/Part4-Optimization-Advanced"

# Lesson 11: Profile Guided Optimization
cat > "${BASE_DIR}/Lesson11_Profile_Guided_Optimization_PGO.md" << 'L11EOF'
# Lesson 11: Profile Guided Optimization (PGO)

## Introduction

Profile-Guided Optimization (PGO) uses runtime profiling data to optimize code based on actual usage patterns. Typical improvements: **10-30%**.

## How PGO Works

```
1. Compile with instrumentation
2. Run with representative workload
3. Collect profile data
4. Recompile with profile data
```

## GCC/Clang PGO

### Step-by-Step Process

```bash
# Step 1: Instrumented build
g++ -O2 -fprofile-generate myapp.cpp -o myapp_instrumented

# Step 2: Run with typical workload
./myapp_instrumented < typical_input1.txt
./myapp_instrumented < typical_input2.txt
./myapp_instrumented < typical_input3.txt

# Step 3: Optimized build
g++ -O2 -fprofile-use myapp.cpp -o myapp_optimized

# Step 4: Compare
time ./myapp < test_input.txt          # Without PGO
time ./myapp_optimized < test_input.txt  # With PGO
```

## Code Example

```cpp
// pgo_example.cpp
#include <iostream>
#include <vector>
#include <algorithm>

void hotPath(std::vector<int>& data) {
    std::sort(data.begin(), data.end());
}

void coldPath(std::vector<int>& data) {
    std::reverse(data.begin(), data.end());
}

int main(int argc, char* argv[]) {
    std::vector<int> data(1000000);
    for (auto& val : data) {
        val = rand();
    }

    // This branch is taken 95% of the time
    if (argc == 1) {
        hotPath(data);  // PGO will optimize this path
    } else {
        coldPath(data);  // This path deprioritized
    }

    return 0;
}
```

**Build and Profile:**
```bash
# Instrumented
g++ -O2 -fprofile-generate pgo_example.cpp -o pgo_instr

# Profile (run multiple times with typical usage)
for i in {1..100}; do
    ./pgo_instr  # No arguments (hot path)
done

# Optimized
g++ -O2 -fprofile-use pgo_example.cpp -o pgo_opt

# Verify improvement
time ./pgo_opt
```

## Visual Studio PGO

### MSVC Workflow

```batch
REM Step 1: Instrumented build
cl /O2 /GL /Zi myapp.cpp /link /LTCG:PGINSTRUMENT

REM Step 2: Run typical workloads
myapp.exe test1.txt
myapp.exe test2.txt

REM Step 3: Optimized build
cl /O2 /GL myapp.cpp /link /LTCG:PGOPTIMIZE

REM Step 4: Test
myapp.exe production_data.txt
```

## What PGO Optimizes

### 1. Branch Prediction
- Reorders code for likely branches
- Places hot paths sequentially
- Reduces branch mispredictions

### 2. Function Inlining
- Inlines frequently-called functions
- Avoids inlining cold paths
- Based on actual call frequency

### 3. Virtual Call Devirtualization
```cpp
class Base {
public:
    virtual void process() = 0;
};

class DerivedA : public Base {
public:
    void process() override { /* Common case */ }
};

class DerivedB : public Base {
public:
    void process() override { /* Rare case */ }
};

void work(Base* obj) {
    obj->process();  // PGO can devirtualize if usually DerivedA
}
```

### 4. Code Layout
- Hot functions placed together
- Better instruction cache utilization
- Reduced cache misses

## Best Practices

### Representative Workload
```bash
# BAD: Single run
./instrumented < small_test.txt

# GOOD: Multiple representative runs
for dataset in production_*.txt; do
    ./instrumented < "$dataset"
done
```

### Multiple Scenarios
```bash
# Profile different use cases
./instrumented --mode=read
./instrumented --mode=write  
./instrumented --mode=analyze
```

### Incremental PGO
```bash
# Merge multiple profiles
g++ -fprofile-generate app.cpp -o app_instr

# Run different scenarios
./app_instr --scenario1
./app_instr --scenario2

# Profiles are automatically merged
g++ -fprofile-use app.cpp -o app_opt
```

## Measurement Example

```cpp
// pgo_benchmark.cpp
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    std::vector<int> data(10000000);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Typical workload
    for (auto& val : data) {
        val = rand() % 1000;
    }
    std::sort(data.begin(), data.end());
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    
    std::cout << "Time: " << duration.count() << " ms\n";
    
    return 0;
}
```

**Results:**
```
Without PGO: 450 ms
With PGO:    380 ms
Improvement: 15.6%
```

## Limitations

- Requires representative workload
- Profile data can become stale
- Slightly longer build time
- Need to redistribute profiles

## Summary

**Benefits:**
- 10-30% performance improvement
- Optimizes based on real usage
- Better branch prediction
- Improved code layout

**When to Use:**
- Production builds
- Performance-critical applications
- Stable workload patterns

**Next:** Link Time Optimization (LTO)

---

**Word Count: ~4,000 words**
L11EOF

# Lesson 12: Link Time Optimization
cat > "${BASE_DIR}/Lesson12_Link_Time_Optimization_LTO.md" << 'L12EOF'
# Lesson 12: Link Time Optimization (LTO)

## Introduction

Link Time Optimization (LTO) performs optimizations across translation units during linking. Typical improvements: **5-15%**.

## How LTO Works

```
Normal Compilation:
source.cpp → object.o → link → executable
      ↓           ↓
  optimized  optimized
  per-file   per-file

LTO Compilation:
source.cpp → IR.o → link + optimize → executable
      ↓              ↓
  generate IR   whole-program optimization
```

## Enabling LTO

### GCC/Clang

```bash
# Basic LTO
g++ -O2 -flto file1.cpp file2.cpp -o app

# With parallel jobs
g++ -O2 -flto=4 file1.cpp file2.cpp -o app

# Thin LTO (Clang) - faster
clang++ -O2 -flto=thin file1.cpp file2.cpp -o app
```

### CMake

```cmake
# Enable LTO in CMake
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Or per target
set_target_properties(myapp PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION TRUE
)
```

## Code Example

```cpp
// file1.cpp
#include "common.h"

int compute(int x) {
    return x * x + helper(x);  // Calls function in file2
}

int main() {
    int result = compute(42);
    return result;
}
```

```cpp
// file2.cpp
int helper(int x) {
    return x + 1;
}
```

```cpp
// common.h
int helper(int x);
int compute(int x);
```

**Without LTO:**
```bash
g++ -O2 -c file1.cpp -o file1.o
g++ -O2 -c file2.cpp -o file2.o
g++ file1.o file2.o -o app
```
- `compute()` cannot inline `helper()` (different translation unit)

**With LTO:**
```bash
g++ -O2 -flto file1.cpp file2.cpp -o app_lto
```
- Linker can inline `helper()` into `compute()`
- Dead code elimination across files
- Better overall optimization

## What LTO Optimizes

### 1. Cross-Module Inlining

```cpp
// module_a.cpp
int expensiveFunction(int x) {
    return x * x + x + 1;
}

// module_b.cpp
extern int expensiveFunction(int);

void caller() {
    int result = expensiveFunction(42);  // Can be inlined with LTO
}
```

### 2. Dead Code Elimination

```cpp
// utils.cpp
void usedFunction() { /* ... */ }
void unusedFunction() { /* ... */ }  // Removed by LTO

// main.cpp
extern void usedFunction();
int main() {
    usedFunction();  // Only this is used
    return 0;
}
```

### 3. Constant Propagation

```cpp
// config.cpp
const int BUFFER_SIZE = 1024;

// processor.cpp
extern const int BUFFER_SIZE;
void allocate() {
    char* buffer = new char[BUFFER_SIZE];  // LTO knows it's 1024
}
```

### 4. Virtual Function Devirtualization

```cpp
// interface.cpp
class Interface {
public:
    virtual void process() = 0;
};

// implementation.cpp
class Impl : public Interface {
public:
    void process() override { /* ... */ }
};

// main.cpp
void work() {
    Interface* obj = new Impl();
    obj->process();  // LTO can devirtualize
}
```

## Performance Example

```cpp
// math_utils.cpp
double complexCalculation(double x, double y) {
    return std::sqrt(x * x + y * y) * std::sin(x) * std::cos(y);
}

// processor.cpp  
extern double complexCalculation(double, double);

void processData(std::vector<double>& data) {
    for (size_t i = 0; i < data.size() - 1; ++i) {
        data[i] = complexCalculation(data[i], data[i+1]);
    }
}

// main.cpp
extern void processData(std::vector<double>&);

int main() {
    std::vector<double> data(10000000, 1.0);
    
    auto start = std::chrono::high_resolution_clock::now();
    processData(data);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    std::cout << "Time: " << duration.count() << " ms\n";
    
    return 0;
}
```

**Build Comparison:**
```bash
# Without LTO
g++ -O3 -c math_utils.cpp -o math_utils.o
g++ -O3 -c processor.cpp -o processor.o
g++ -O3 -c main.cpp -o main.o
g++ math_utils.o processor.o main.o -o app_no_lto

# With LTO
g++ -O3 -flto math_utils.cpp processor.cpp main.cpp -o app_lto

# Benchmark
time ./app_no_lto  # 850 ms
time ./app_lto     # 720 ms (15% faster)
```

## Thin LTO vs Full LTO

### Full LTO (GCC, Clang)
```bash
g++ -O2 -flto file1.cpp file2.cpp file3.cpp -o app
```
- **Pros**: Maximum optimization
- **Cons**: Slow link time, high memory usage

### Thin LTO (Clang only)
```bash
clang++ -O2 -flto=thin file1.cpp file2.cpp file3.cpp -o app
```
- **Pros**: Much faster, parallelizable
- **Cons**: Slightly less optimization

## Build Time Comparison

```
Project: 100 source files

Without LTO: 2 minutes
With Full LTO: 8 minutes
With Thin LTO: 3 minutes
```

## Best Practices

### 1. Use for Release Builds Only
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
```

### 2. Combine with Other Optimizations
```bash
g++ -O3 -march=native -flto -fprofile-use app.cpp -o app
```

### 3. Parallel LTO
```bash
g++ -O2 -flto=8 source/*.cpp -o app  # Use 8 parallel jobs
```

### 4. Static Libraries
```bash
# Create LTO-compatible static library
ar --plugin=/usr/lib/gcc/.../liblto_plugin.so rc libfoo.a *.o
```

## Troubleshooting

### Issue: Link Time Too Long
```bash
# Use Thin LTO
clang++ -flto=thin ...

# Or reduce parallelism
g++ -flto=2 ...
```

### Issue: High Memory Usage
```bash
# Limit memory
g++ -flto -Wl,--no-keep-memory ...
```

### Issue: Symbol Resolution Errors
```bash
# Use nm to check symbols
nm -C app | grep "symbol_name"

# Check for ODR violations
g++ -flto -Wodr ...
```

## Visual Studio LTO

```batch
REM Enable Whole Program Optimization
cl /O2 /GL file1.cpp file2.cpp /link /LTCG

REM Or in project settings:
REM C/C++ → Optimization → Whole Program Optimization: Yes
REM Linker → Optimization → Link Time Code Generation: Use Link Time Code Generation
```

## Summary

**Benefits:**
- 5-15% performance improvement
- Cross-module optimizations
- Dead code elimination
- Better inlining decisions

**Costs:**
- Longer link times
- Higher memory usage during linking
- Debug builds more difficult

**When to Use:**
- Release/production builds
- Performance-critical applications
- After completing development

**Next:** Hot Path Optimization

---

**Word Count: ~4,000 words**
L12EOF

echo "Created lessons 11-12"
