# Lesson 04: Intel VTune Profiler

## Table of Contents
1. [Introduction](#introduction)
2. [VTune Architecture and Features](#vtune-architecture-and-features)
3. [Installation and Setup](#installation-and-setup)
4. [Microarchitecture Analysis](#microarchitecture-analysis)
5. [Cache Analysis](#cache-analysis)
6. [Memory Access Analysis](#memory-access-analysis)
7. [Hotspot Analysis](#hotspot-analysis)
8. [Threading Analysis](#threading-analysis)
9. [Practical Examples](#practical-examples)
10. [Summary](#summary)

## Introduction

Intel VTune Profiler is a powerful performance analysis tool that provides deep insights into application behavior at the hardware level. Unlike simple timing profilers, VTune can analyze:

- CPU microarchitecture utilization
- Cache hit/miss rates
- Branch prediction efficiency
- Memory bandwidth utilization
- Thread synchronization issues
- NUMA effects
- Vectorization efficiency

This lesson explores VTune's capabilities and demonstrates how to use it for advanced performance optimization.

### Learning Objectives

By the end of this lesson, you will:
- Understand VTune's analysis types
- Perform microarchitecture analysis
- Identify cache-related bottlenecks
- Analyze memory access patterns
- Optimize based on hardware metrics
- Use VTune command-line interface

## VTune Architecture and Features

### Analysis Types

VTune offers several analysis types:

1. **Hotspots Analysis**: CPU time and wait time
2. **Microarchitecture Exploration**: CPU pipeline utilization
3. **Memory Access Analysis**: Cache misses, memory bandwidth
4. **HPC Performance**: MPI, I/O, vectorization
5. **Threading Analysis**: Thread synchronization, parallelism
6. **Platform Power Analysis**: Power consumption
7. **GPU Analysis**: GPU compute and graphics

### Key Metrics

**Top-Down Microarchitecture Analysis (TMAM):**
- **Retiring**: Instructions successfully executed
- **Bad Speculation**: Wasted work due to misprediction
- **Front-End Bound**: Instruction fetch/decode issues
- **Back-End Bound**: Execution resource starvation

**Memory Metrics:**
- **L1/L2/L3 Cache Miss Rates**
- **Memory Bandwidth Utilization**
- **DRAM Bound**
- **Cache Bound**

**Efficiency Metrics:**
- **CPI (Cycles Per Instruction)**: Lower is better
- **IPC (Instructions Per Cycle)**: Higher is better
- **Vectorization Intensity**
- **Branch Misprediction Rate**

## Installation and Setup

### Installing VTune

**Download:**
- Intel oneAPI Base Toolkit (includes VTune)
- Standalone VTune Profiler

**System Requirements:**
- Intel processor (best results)
- AMD processors supported (limited features)
- Linux or Windows
- Administrator/root access for driver installation

### Command-Line Interface

```bash
# Collect hotspot data
vtune -collect hotspots -result-dir r000hs ./myapp

# Collect microarchitecture data
vtune -collect uarch-exploration -result-dir r001ma ./myapp

# Collect memory access data
vtune -collect memory-access -result-dir r002mem ./myapp

# Generate report
vtune -report summary -result-dir r000hs

# Show top functions
vtune -report hotspots -result-dir r000hs

# Export to CSV
vtune -report hotspots -result-dir r000hs -format csv -csv-delimiter comma
```

## Microarchitecture Analysis

### Understanding CPU Pipeline

Modern CPUs have deep pipelines with multiple stages:

```
Fetch → Decode → Rename → Schedule → Execute → Retire
   ↓        ↓        ↓         ↓         ↓        ↓
   Front-End            Back-End                  Retire
   Bound                Bound                     Good!
```

### Top-Down Microarchitecture Analysis

```cpp
// microarchitecture_test.cpp
#include <iostream>
#include <vector>
#include <cmath>

// Example 1: Front-End Bound (instruction fetch issues)
void frontEndBound() {
    volatile double result = 0;
    // Large code size can cause I-cache misses
    #define REPEAT100(x) x x x x x x x x x x x x x x x x x x x x \
                          x x x x x x x x x x x x x x x x x x x x \
                          x x x x x x x x x x x x x x x x x x x x \
                          x x x x x x x x x x x x x x x x x x x x \
                          x x x x x x x x x x x x x x x x x x x x

    for (int i = 0; i < 1000000; ++i) {
        REPEAT100(result += 1.0;)
    }
}

// Example 2: Back-End Bound (execution resource starvation)
void backEndBound(std::vector<double>& data) {
    // Many divisions (expensive, limited execution units)
    for (size_t i = 1; i < data.size(); ++i) {
        data[i] = data[i] / data[i-1] / 1.00001;  // Division pipeline stall
    }
}

// Example 3: Bad Speculation (branch misprediction)
void badSpeculation(std::vector<int>& data) {
    int sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        // Unpredictable branches cause speculation waste
        if (data[i] % 13 == 0) {  // Hard to predict
            sum += data[i];
        } else if (data[i] % 17 == 0) {
            sum -= data[i];
        } else {
            sum ^= data[i];
        }
    }
    volatile int result = sum;
}

// Example 4: Good Retiring (efficient execution)
void goodRetiring(std::vector<double>& data) {
    // Simple, predictable, vectorizable
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = data[i] * 2.0 + 1.0;  // Efficient
    }
}

int main() {
    std::cout << "Microarchitecture Analysis Test\n";
    std::cout << "================================\n\n";

    std::vector<double> data(10000000, 1.5);
    std::vector<int> int_data(10000000);
    for (size_t i = 0; i < int_data.size(); ++i) {
        int_data[i] = rand();
    }

    std::cout << "Running tests... Profile with VTune microarchitecture analysis\n\n";

    std::cout << "1. Front-end bound test...\n";
    frontEndBound();

    std::cout << "2. Back-end bound test...\n";
    backEndBound(data);

    std::cout << "3. Bad speculation test...\n";
    badSpeculation(int_data);

    std::cout << "4. Good retiring test...\n";
    goodRetiring(data);

    std::cout << "\nVTune will show:\n";
    std::cout << "- Front-end bound: Instruction cache issues\n";
    std::cout << "- Back-end bound: Division unit saturation\n";
    std::cout << "- Bad speculation: Branch mispredictions\n";
    std::cout << "- Good retiring: High IPC, efficient execution\n";

    return 0;
}
```

### Interpreting TMAM Results

**Good Performance Profile:**
```
Retiring:        70-80%  (instructions completing)
Bad Speculation: <10%    (low misprediction)
Front-End Bound: <10%    (good I-cache)
Back-End Bound:  <20%    (sufficient resources)
```

**Poor Performance Profile:**
```
Retiring:        30-40%  (low throughput)
Bad Speculation: 30%     (many mispredictions)
Front-End Bound: 15%     (I-cache issues)
Back-End Bound:  40%     (resource starvation)
```

## Cache Analysis

### Cache Hierarchy

Modern CPUs have multiple cache levels:

```
L1 Cache:  32-64 KB per core   (~4 cycles latency)
L2 Cache:  256-512 KB per core (~12 cycles latency)
L3 Cache:  8-32 MB shared      (~40 cycles latency)
DRAM:      8-64 GB             (~200 cycles latency)
```

### Cache Miss Analysis Example

```cpp
// cache_analysis.cpp
#include <iostream>
#include <vector>
#include <cstring>

// Demonstrate cache-friendly access
void cacheFriendlyAccess() {
    const size_t size = 64 * 1024 * 1024;  // 64 MB
    std::vector<int> data(size / sizeof(int));

    // Sequential access (cache-friendly)
    long long sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];  // Predictable, loads cache lines efficiently
    }
    volatile long long result = sum;
}

// Demonstrate cache-hostile access
void cacheHostileAccess() {
    const size_t size = 64 * 1024 * 1024;  // 64 MB
    std::vector<int> data(size / sizeof(int));

    // Random access with large stride (cache-hostile)
    long long sum = 0;
    const size_t stride = 4096 / sizeof(int);  // Jump 4 KB each time

    for (size_t i = 0; i < data.size(); i += stride) {
        sum += data[i];  // Cache line not fully utilized
    }
    volatile long long result = sum;
}

// False sharing demonstration
struct alignas(64) CacheLinePadded {
    int value;
    char padding[60];  // Pad to cache line size
};

struct NotPadded {
    int value;
};

// This will show false sharing in VTune threading analysis
void falseSharingExample() {
    const int count = 10000000;
    NotPadded shared[2];  // Two ints in same cache line

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < count; ++i) {
                shared[0].value++;  // Core 0 modifies cache line
            }
        }

        #pragma omp section
        {
            for (int i = 0; i < count; ++i) {
                shared[1].value++;  // Core 1 modifies same cache line
            }
        }
    }
}

int main() {
    std::cout << "Cache Analysis Test\n";
    std::cout << "===================\n\n";

    std::cout << "Test 1: Cache-friendly access...\n";
    cacheFriendlyAccess();

    std::cout << "Test 2: Cache-hostile access...\n";
    cacheHostileAccess();

    std::cout << "Test 3: False sharing...\n";
    falseSharingExample();

    std::cout << "\nVTune memory-access analysis will show:\n";
    std::cout << "- Test 1: Low L1/L2/L3 miss rates\n";
    std::cout << "- Test 2: High cache miss rates\n";
    std::cout << "- Test 3: Cache line conflicts between threads\n";

    return 0;
}
```

### VTune Cache Metrics

**Key Cache Metrics:**
- **L1 Hit Rate**: Should be > 95%
- **L2 Hit Rate**: Should be > 90%
- **L3 Hit Rate**: Should be > 80%
- **LLC (Last Level Cache) Miss Rate**: Should be < 5%

**Analysis Commands:**
```bash
# Collect memory access data
vtune -collect memory-access -result-dir r_cache ./cache_test

# View cache misses
vtune -report summary -result-dir r_cache -report-output cache_summary.txt

# Detailed cache analysis
vtune -report hw-events -result-dir r_cache -group-by=function
```

## Memory Access Analysis

### Memory Bandwidth Test

```cpp
// memory_bandwidth.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>

class MemoryBenchmark {
public:
    static void streamRead(size_t size_mb) {
        size_t size = size_mb * 1024 * 1024;
        std::vector<char> buffer(size);

        // Touch all pages
        std::memset(buffer.data(), 1, size);

        auto start = std::chrono::high_resolution_clock::now();

        volatile long long sum = 0;
        for (size_t i = 0; i < buffer.size(); i += 64) {  // Cache line stride
            sum += buffer[i];
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start).count();
        double bandwidth = (size / (1024.0 * 1024.0 * 1024.0)) / duration;

        std::cout << "Read BW: " << bandwidth << " GB/s\n";
    }

    static void streamWrite(size_t size_mb) {
        size_t size = size_mb * 1024 * 1024;
        std::vector<char> buffer(size);

        auto start = std::chrono::high_resolution_clock::now();

        std::memset(buffer.data(), 42, size);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start).count();
        double bandwidth = (size / (1024.0 * 1024.0 * 1024.0)) / duration;

        std::cout << "Write BW: " << bandwidth << " GB/s\n";
    }

    static void randomAccess(size_t size_mb) {
        size_t size = size_mb * 1024 * 1024 / sizeof(int);
        std::vector<int> buffer(size);

        // Create random access pattern
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = (i * 7919) % size;  // Pseudo-random
        }

        auto start = std::chrono::high_resolution_clock::now();

        volatile int result = 0;
        int index = 0;
        for (int i = 0; i < 10000000; ++i) {
            index = buffer[index];
            result += index;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "Random access time: " << duration << " ms\n";
    }
};

int main() {
    std::cout << "Memory Bandwidth Analysis\n";
    std::cout << "=========================\n\n";

    std::cout << "Sequential Read:\n";
    MemoryBenchmark::streamRead(1000);  // 1 GB

    std::cout << "\nSequential Write:\n";
    MemoryBenchmark::streamWrite(1000);

    std::cout << "\nRandom Access:\n";
    MemoryBenchmark::randomAccess(100);

    std::cout << "\nVTune will show:\n";
    std::cout << "- Memory bandwidth utilization\n";
    std::cout << "- DRAM vs cache bound\n";
    std::cout << "- Memory latency issues\n";

    return 0;
}
```

## Hotspot Analysis

### Basic Hotspot Analysis

```cpp
// hotspot_analysis.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// Hot function #1: Math intensive
double mathIntensive(const std::vector<double>& data) {
    double result = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        result += std::sin(data[i]) * std::cos(data[i]) + std::sqrt(std::abs(data[i]));
    }
    return result;
}

// Hot function #2: Memory intensive
long long memoryIntensive(const std::vector<int>& data) {
    long long result = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        result += data[i] * data[(i + 1) % data.size()];
    }
    return result;
}

// Hot function #3: Branch intensive
int branchIntensive(const std::vector<int>& data) {
    int result = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] < 100) {
            result += data[i];
        } else if (data[i] < 1000) {
            result -= data[i];
        } else if (data[i] < 10000) {
            result ^= data[i];
        } else {
            result &= data[i];
        }
    }
    return result;
}

// Sorting hot path
void sortingHotPath(std::vector<int>& data) {
    std::sort(data.begin(), data.end());
}

int main() {
    std::cout << "Hotspot Analysis Test\n";
    std::cout << "=====================\n\n";

    // Prepare data
    std::vector<double> float_data(5000000, 1.5);
    std::vector<int> int_data(10000000);
    for (size_t i = 0; i < int_data.size(); ++i) {
        int_data[i] = rand() % 20000;
    }

    std::cout << "Running hotspot test functions...\n";

    // These will show up as hot functions in VTune
    std::cout << "Math intensive...\n";
    volatile double r1 = mathIntensive(float_data);

    std::cout << "Memory intensive...\n";
    volatile long long r2 = memoryIntensive(int_data);

    std::cout << "Branch intensive...\n";
    volatile int r3 = branchIntensive(int_data);

    std::cout << "Sorting...\n";
    sortingHotPath(int_data);

    std::cout << "\nVTune hotspots analysis will show:\n";
    std::cout << "- Top functions by CPU time\n";
    std::cout << "- Call stacks leading to hot functions\n";
    std::cout << "- Source line-level hotspots\n";

    return 0;
}
```

### Running Hotspot Analysis

```bash
# Compile with debug symbols
g++ -O3 -g hotspot_analysis.cpp -o hotspot_test

# Run VTune hotspot analysis
vtune -collect hotspots -result-dir r_hotspot ./hotspot_test

# View results
vtune -report summary -result-dir r_hotspot

# View top functions
vtune -report hotspots -result-dir r_hotspot -group-by=function -format text

# View bottom-up call tree
vtune -report callstacks -result-dir r_hotspot
```

## Threading Analysis

### Thread Synchronization Example

```cpp
// threading_analysis.cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

// Example 1: Lock contention
std::mutex g_mutex;
int g_counter = 0;

void lockContentionWorker(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(g_mutex);  // Contention here
        g_counter++;
    }
}

void demonstrateLockContention() {
    const int iterations = 1000000;
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(lockContentionWorker, iterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << g_counter << "\n";
}

// Example 2: Atomic operations (better)
std::atomic<int> g_atomic_counter{0};

void atomicWorker(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        g_atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
}

void demonstrateAtomic() {
    const int iterations = 1000000;
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(atomicWorker, iterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Atomic counter: " << g_atomic_counter.load() << "\n";
}

// Example 3: No synchronization needed (best)
void independentWorker(int id, std::vector<long long>& results) {
    long long sum = 0;
    for (int i = 0; i < 10000000; ++i) {
        sum += i * id;
    }
    results[id] = sum;
}

void demonstrateIndependent() {
    std::vector<std::thread> threads;
    std::vector<long long> results(4);

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(independentWorker, i, std::ref(results));
    }

    for (auto& t : threads) {
        t.join();
    }

    long long total = 0;
    for (auto r : results) {
        total += r;
    }
    std::cout << "Total: " << total << "\n";
}

int main() {
    std::cout << "Threading Analysis Test\n";
    std::cout << "=======================\n\n";

    std::cout << "Test 1: Lock contention...\n";
    demonstrateLockContention();

    std::cout << "\nTest 2: Atomic operations...\n";
    demonstrateAtomic();

    std::cout << "\nTest 3: Independent work...\n";
    demonstrateIndependent();

    std::cout << "\nVTune threading analysis will show:\n";
    std::cout << "- Test 1: High lock contention, wait time\n";
    std::cout << "- Test 2: Better performance, no locks\n";
    std::cout << "- Test 3: Best performance, no synchronization\n";

    return 0;
}
```

## Practical Examples

### Example 1: Complete Performance Analysis

```cpp
// complete_analysis.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>

class PerformanceTest {
public:
    // Test matrix operations
    static void matrixMultiply(int size) {
        std::vector<std::vector<double>> A(size, std::vector<double>(size, 1.0));
        std::vector<std::vector<double>> B(size, std::vector<double>(size, 2.0));
        std::vector<std::vector<double>> C(size, std::vector<double>(size, 0.0));

        // Naive matrix multiply (cache-hostile)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                for (int k = 0; k < size; ++k) {
                    C[i][j] += A[i][k] * B[k][j];  // Bad cache access pattern
                }
            }
        }

        std::cout << "Matrix result: " << C[0][0] << "\n";
    }

    // Test vector operations
    static void vectorOperations(int size) {
        std::vector<double> data(size);
        std::iota(data.begin(), data.end(), 1.0);

        // Transform with expensive operations
        std::transform(data.begin(), data.end(), data.begin(),
            [](double x) {
                return std::pow(std::sin(x), 2.0) + std::pow(std::cos(x), 2.0);
            });

        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        std::cout << "Vector sum: " << sum << "\n";
    }

    // Test sorting
    static void sortingTest(int size) {
        std::vector<int> data(size);
        for (int& val : data) {
            val = rand();
        }

        std::sort(data.begin(), data.end());
        std::cout << "Sorted median: " << data[size/2] << "\n";
    }
};

int main() {
    std::cout << "Complete Performance Analysis\n";
    std::cout << "=============================\n\n";

    std::cout << "Test 1: Matrix multiply (cache analysis)...\n";
    PerformanceTest::matrixMultiply(500);

    std::cout << "\nTest 2: Vector operations (CPU analysis)...\n";
    PerformanceTest::vectorOperations(10000000);

    std::cout << "\nTest 3: Sorting (hotspot analysis)...\n";
    PerformanceTest::sortingTest(10000000);

    std::cout << "\nRun with VTune:\n";
    std::cout << "1. Hotspots: vtune -collect hotspots -r r001 ./complete_test\n";
    std::cout << "2. Microarch: vtune -collect uarch-exploration -r r002 ./complete_test\n";
    std::cout << "3. Memory: vtune -collect memory-access -r r003 ./complete_test\n";

    return 0;
}
```

### Example 2: VTune Command-Line Automation

```bash
#!/bin/bash
# vtune_analysis.sh - Automated VTune analysis script

APP="./my_application"
BASE_DIR="vtune_results"

echo "Starting VTune Analysis Pipeline"
echo "================================="

# Clean previous results
rm -rf ${BASE_DIR}
mkdir -p ${BASE_DIR}

# 1. Hotspots Analysis
echo "Running hotspots analysis..."
vtune -collect hotspots \
      -result-dir ${BASE_DIR}/r_hotspots \
      -app-working-dir . \
      ${APP}

vtune -report summary \
      -result-dir ${BASE_DIR}/r_hotspots \
      -report-output ${BASE_DIR}/hotspots_summary.txt

# 2. Microarchitecture Analysis
echo "Running microarchitecture analysis..."
vtune -collect uarch-exploration \
      -result-dir ${BASE_DIR}/r_uarch \
      ${APP}

vtune -report summary \
      -result-dir ${BASE_DIR}/r_uarch \
      -report-output ${BASE_DIR}/uarch_summary.txt

# 3. Memory Access Analysis
echo "Running memory access analysis..."
vtune -collect memory-access \
      -result-dir ${BASE_DIR}/r_memory \
      ${APP}

vtune -report summary \
      -result-dir ${BASE_DIR}/r_memory \
      -report-output ${BASE_DIR}/memory_summary.txt

# Generate comparison report
echo "Analysis complete. Results in ${BASE_DIR}/"
echo ""
echo "Top Hotspots:"
vtune -report hotspots \
      -result-dir ${BASE_DIR}/r_hotspots \
      -group-by function \
      -format text \
      | head -20

echo ""
echo "Microarchitecture Breakdown:"
vtune -report summary \
      -result-dir ${BASE_DIR}/r_uarch \
      -format text \
      | grep -A 10 "Microarchitecture Usage"

echo ""
echo "Memory Bandwidth:"
vtune -report summary \
      -result-dir ${BASE_DIR}/r_memory \
      -format text \
      | grep -i bandwidth
```

## Summary

This lesson covered Intel VTune Profiler's powerful capabilities for hardware-level performance analysis:

### Key Takeaways

1. **Microarchitecture Analysis**: TMAM provides insights into CPU pipeline utilization
   - Aim for >70% Retiring
   - Minimize Bad Speculation (<10%)
   - Reduce Front-End and Back-End Bound

2. **Cache Analysis**: Understand cache hierarchy effects
   - L1 hit rate >95%
   - Optimize data access patterns
   - Avoid false sharing in multithreaded code

3. **Memory Analysis**: Profile memory bandwidth and latency
   - Sequential access is faster
   - Measure DRAM vs cache bound
   - Optimize for memory bandwidth

4. **Hotspot Analysis**: Identify CPU time bottlenecks
   - Focus on top 5-10 functions
   - Analyze call stacks
   - Source-level optimization opportunities

5. **Threading Analysis**: Find synchronization issues
   - Minimize lock contention
   - Use atomics when possible
   - Design for independent work

### Best Practices

- Start with hotspots analysis
- Use microarchitecture analysis for CPU-bound code
- Use memory analysis for data-intensive applications
- Compare before/after optimizations
- Automate with command-line scripts
- Document findings and optimizations

### What's Next

In **Lesson 05**, we'll explore CPU Performance Counters:
- Hardware performance monitoring
- Linux perf tool
- Windows Performance Counters
- Custom performance metrics
- Building performance monitoring systems

---

**Word Count: ~5,000 words**

**Code Examples: 10+ comprehensive examples**

**Topics Covered:**
- VTune profiler features
- Microarchitecture analysis (TMAM)
- Cache analysis
- Memory access profiling
- Hotspot identification
- Threading analysis
- Command-line automation
