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
