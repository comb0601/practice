# Lesson 91: CPU Profiling & Performance Analysis
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to Performance Analysis](#introduction-to-performance-analysis)
2. [Why Performance Matters](#why-performance-matters)
3. [Performance Measurement Techniques](#performance-measurement-techniques)
4. [High-Precision Timing](#high-precision-timing)
5. [CPU Profiling Tools](#cpu-profiling-tools)
6. [Identifying Bottlenecks](#identifying-bottlenecks)
7. [Benchmarking Best Practices](#benchmarking-best-practices)
8. [Statistical Analysis of Performance](#statistical-analysis-of-performance)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction to Performance Analysis

### What is Performance Analysis?

Performance analysis is the process of measuring, analyzing, and optimizing the execution time and resource usage of software. It helps you:

- **Identify bottlenecks**: Find the parts of your code that are slow
- **Measure improvements**: Quantify the impact of optimizations
- **Make informed decisions**: Choose the right optimization strategies
- **Meet performance requirements**: Ensure your software runs fast enough

### The Performance Optimization Cycle

```
1. Measure (Profile) → 2. Analyze (Find Bottlenecks) → 3. Optimize → 4. Verify (Measure Again)
        ↑                                                                      |
        |______________________________________________________________________|
```

**Golden Rule**: Never optimize without measuring first!

### Common Performance Metrics

```cpp
// 1. Execution Time - How long does it take?
double executionTimeSeconds = 2.5;

// 2. Throughput - How many operations per second?
double operationsPerSecond = 1000000;

// 3. Latency - How long does one operation take?
double latencyMicroseconds = 1.0;

// 4. CPU Usage - Percentage of CPU time used
double cpuUsagePercent = 45.5;

// 5. Memory Usage - Bytes allocated
size_t memoryBytes = 1024 * 1024 * 100; // 100 MB
```

## Why Performance Matters

### Real-World Impact

**Video Games (60 FPS)**
- Frame budget: 16.67 milliseconds
- If your code takes 20ms per frame, you get stuttering and poor user experience

**Web Servers**
- 100ms delay = 1% loss in sales (Amazon study)
- 1 second delay = 7% reduction in conversions

**Scientific Computing**
- Simulation that takes 1 week vs 1 day
- Difference can be critical for research deadlines

### Performance Requirements by Domain

| Domain | Target Latency | Target FPS | Notes |
|--------|---------------|-----------|-------|
| AAA Games | < 16.67ms | 60 FPS | VR requires 90+ FPS |
| Web APIs | < 100ms | N/A | User perceivable delay |
| Real-time Systems | < 1ms | N/A | Safety-critical |
| Batch Processing | N/A | N/A | Maximize throughput |
| Mobile Apps | < 16.67ms | 60 FPS | Battery life critical |

## Performance Measurement Techniques

### 1. Wall-Clock Time

Measures the actual elapsed time from the user's perspective.

```cpp
#include <chrono>

// Start timer
auto start = std::chrono::high_resolution_clock::now();

// Code to measure
doWork();

// End timer
auto end = std::chrono::high_resolution_clock::now();

// Calculate duration
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
std::cout << "Time: " << duration.count() << " microseconds\n";
```

### 2. CPU Time

Measures actual CPU time spent (excludes time waiting for I/O, other processes).

```cpp
#include <ctime>

clock_t start = clock();

// Code to measure
doWork();

clock_t end = clock();

double cpuTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
std::cout << "CPU Time: " << cpuTime << " seconds\n";
```

### 3. Instruction Counting

Count the number of instructions or operations executed.

```cpp
unsigned long long instructionCount = 0;

for (int i = 0; i < n; ++i) {
    instructionCount++; // Count iterations
    // Do work
}

std::cout << "Instructions: " << instructionCount << "\n";
```

## High-Precision Timing

### Windows QueryPerformanceCounter

The most accurate timing method on Windows:

```cpp
#include <windows.h>
#include <iostream>

class HighPrecisionTimer {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER startTime;

public:
    HighPrecisionTimer() {
        QueryPerformanceFrequency(&frequency);
    }

    void start() {
        QueryPerformanceCounter(&startTime);
    }

    double getElapsedMilliseconds() {
        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart = endTime.QuadPart - startTime.QuadPart;

        // Convert to milliseconds
        return (elapsed.QuadPart * 1000.0) / frequency.QuadPart;
    }

    double getElapsedMicroseconds() {
        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart = endTime.QuadPart - startTime.QuadPart;

        // Convert to microseconds
        return (elapsed.QuadPart * 1000000.0) / frequency.QuadPart;
    }
};
```

**Usage Example:**

```cpp
HighPrecisionTimer timer;
timer.start();

// Code to measure
for (int i = 0; i < 1000000; ++i) {
    // Some work
}

std::cout << "Elapsed: " << timer.getElapsedMicroseconds() << " μs\n";
```

### C++11 std::chrono (Cross-Platform)

```cpp
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;

public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    double getElapsedMilliseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
        return duration.count() / 1000.0;
    }

    double getElapsedMicroseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
        return duration.count();
    }
};
```

### RAII Timer Pattern

Automatically measure function execution time:

```cpp
class ScopedTimer {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    ScopedTimer(const std::string& name) : name(name) {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        std::cout << name << " took " << duration.count() << " μs\n";
    }
};

// Usage
void myFunction() {
    ScopedTimer timer("myFunction");
    // Function code here
    // Timer automatically reports when function exits
}
```

## CPU Profiling Tools

### 1. Visual Studio Profiler

**Performance Profiler (Visual Studio 2019+)**

```
Menu: Debug → Performance Profiler (Alt+F2)

Tools available:
- CPU Usage: See which functions take the most time
- Memory Usage: Track allocations and leaks
- GPU Usage: DirectX performance
- .NET Object Allocation: Managed code profiling
```

**How to Use:**

1. **Start Profiling**: Debug → Performance Profiler
2. **Select "CPU Usage"**
3. **Click "Start"**
4. **Run your application** (perform the slow operation)
5. **Stop profiling**
6. **Analyze results**:
   - Hot Path: Most expensive call stack
   - Top Functions: Functions with highest execution time
   - Call Tree: Hierarchical view of function calls

**Reading the Results:**

```
Function Name          | Total CPU % | Self CPU % | Module
--------------------- | ----------- | ---------- | ------
ProcessFrame          | 85.2%       | 2.1%       | Game.exe
└─ RenderScene        | 80.1%       | 5.3%       | Game.exe
   └─ DrawMesh        | 74.8%       | 74.8%      | Game.exe  ← BOTTLENECK!
```

**Self CPU %**: Time spent in the function itself
**Total CPU %**: Time spent in function + all its children

### 2. Intel VTune Profiler

Professional-grade profiler with advanced features:

**Features:**
- Hardware event sampling (cache misses, branch mispredictions)
- Microarchitecture analysis
- Threading analysis
- Memory access patterns

**Hotspot Analysis:**
```
Top Hotspots:
1. DrawMesh::Render()         45.2% (1250ms)
2. PhysicsEngine::Update()    22.1% (610ms)
3. AIManager::Think()         15.3% (425ms)
```

### 3. Very Sleepy (Free, Open Source)

Simple and effective CPU profiler for Windows:

**Download**: http://www.codersnotes.com/sleepy/

**How to Use:**
1. Launch Very Sleepy
2. Click "Launch and profile"
3. Select your executable
4. Run your program
5. Stop profiling
6. View results sorted by execution time

### 4. Built-In Instrumentation

Add your own profiling code:

```cpp
struct ProfileResult {
    std::string name;
    double milliseconds;
};

class Profiler {
private:
    static std::vector<ProfileResult> results;

public:
    static void recordResult(const std::string& name, double ms) {
        results.push_back({name, ms});
    }

    static void printResults() {
        std::cout << "\n=== Profiling Results ===\n";

        // Sort by time (descending)
        std::sort(results.begin(), results.end(),
            [](const ProfileResult& a, const ProfileResult& b) {
                return a.milliseconds > b.milliseconds;
            });

        for (const auto& result : results) {
            std::cout << result.name << ": " << result.milliseconds << " ms\n";
        }
    }

    static void clear() {
        results.clear();
    }
};

std::vector<ProfileResult> Profiler::results;

// RAII wrapper for profiling
class ScopedProfiler {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    ScopedProfiler(const std::string& name) : name(name) {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ScopedProfiler() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        Profiler::recordResult(name, duration.count() / 1000.0);
    }
};

// Usage
void render() {
    ScopedProfiler prof("Render");
    // Rendering code
}

void physics() {
    ScopedProfiler prof("Physics");
    // Physics code
}

int main() {
    for (int frame = 0; frame < 100; ++frame) {
        render();
        physics();
    }

    Profiler::printResults();
    return 0;
}
```

## Identifying Bottlenecks

### What is a Bottleneck?

A bottleneck is the part of your program that limits overall performance. Like water flowing through pipes of different sizes, the narrowest pipe determines the flow rate.

**Analogy:**
```
Fast (10ms) → SLOW (100ms) → Fast (5ms) → Fast (2ms)
              ↑
          BOTTLENECK

Total time: 117ms (dominated by the 100ms section)
```

### The 90/10 Rule (Pareto Principle)

**90% of execution time is spent in 10% of the code.**

**Implication**: Focus your optimization efforts on the 10% that matters!

### How to Find Bottlenecks

**Step 1: Profile the entire program**

```cpp
void gameLoop() {
    ScopedProfiler prof("GameLoop");

    {
        ScopedProfiler prof2("Input");
        processInput();          // 0.5ms
    }

    {
        ScopedProfiler prof2("Update");
        updateGame();            // 2.0ms
    }

    {
        ScopedProfiler prof2("Render");
        render();                // 45.0ms ← BOTTLENECK!
    }
}
```

**Step 2: Profile the bottleneck in detail**

```cpp
void render() {
    ScopedProfiler prof("Render");

    {
        ScopedProfiler prof2("ClearScreen");
        clearScreen();           // 0.1ms
    }

    {
        ScopedProfiler prof2("DrawMeshes");
        drawAllMeshes();         // 40.0ms ← BOTTLENECK!
    }

    {
        ScopedProfiler prof2("Present");
        present();               // 4.9ms
    }
}
```

**Step 3: Profile the nested bottleneck**

```cpp
void drawAllMeshes() {
    ScopedProfiler prof("DrawAllMeshes");

    for (int i = 0; i < meshes.size(); ++i) {
        // Each mesh takes 0.04ms
        // 1000 meshes * 0.04ms = 40ms
        drawMesh(meshes[i]);     ← Too many draw calls!
    }
}
```

**Diagnosis**: We're making too many draw calls. Solution: batching or instancing (covered in Lesson 96).

### Common Bottleneck Patterns

**1. Nested Loops (O(n²) complexity)**

```cpp
// SLOW: O(n²)
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
        // This runs n² times
    }
}

// For n = 10,000: 100,000,000 iterations!
```

**2. Frequent Memory Allocations**

```cpp
// SLOW: Allocates in every iteration
for (int i = 0; i < 1000000; ++i) {
    std::vector<int> temp;  // Allocation!
    temp.push_back(i);
}

// FAST: Allocate once, reuse
std::vector<int> temp;
temp.reserve(1);
for (int i = 0; i < 1000000; ++i) {
    temp.clear();
    temp.push_back(i);
}
```

**3. Cache Misses**

```cpp
// SLOW: Non-contiguous memory access
struct Node {
    int data;
    Node* next;  // Pointer to random memory location
};

// FAST: Contiguous memory access
std::vector<int> data;  // All elements in a row
```

**4. Redundant Computations**

```cpp
// SLOW: Recalculates every frame
for (int frame = 0; frame < 1000; ++frame) {
    float value = sqrt(2.0f) * sin(3.14159f / 4.0f);  // Always the same!
    // Use value...
}

// FAST: Calculate once
const float value = sqrt(2.0f) * sin(3.14159f / 4.0f);
for (int frame = 0; frame < 1000; ++frame) {
    // Use value...
}
```

## Benchmarking Best Practices

### 1. Warm Up the Cache

```cpp
// Run once before measuring to warm up CPU cache
for (int i = 0; i < 1000; ++i) {
    processData();
}

// Now measure
Timer timer;
timer.start();
for (int i = 0; i < 1000; ++i) {
    processData();
}
double elapsed = timer.getElapsedMilliseconds();
```

### 2. Multiple Iterations

```cpp
const int ITERATIONS = 1000;

Timer timer;
timer.start();

for (int i = 0; i < ITERATIONS; ++i) {
    processData();
}

double elapsed = timer.getElapsedMilliseconds();
double averageTime = elapsed / ITERATIONS;

std::cout << "Average time: " << averageTime << " ms\n";
```

### 3. Statistical Analysis

```cpp
std::vector<double> samples;

// Collect multiple samples
for (int run = 0; run < 100; ++run) {
    Timer timer;
    timer.start();

    processData();

    samples.push_back(timer.getElapsedMicroseconds());
}

// Calculate statistics
double sum = 0;
for (double sample : samples) {
    sum += sample;
}
double mean = sum / samples.size();

// Calculate standard deviation
double variance = 0;
for (double sample : samples) {
    variance += (sample - mean) * (sample - mean);
}
variance /= samples.size();
double stddev = sqrt(variance);

// Sort to find median
std::sort(samples.begin(), samples.end());
double median = samples[samples.size() / 2];

std::cout << "Mean: " << mean << " μs\n";
std::cout << "Median: " << median << " μs\n";
std::cout << "Std Dev: " << stddev << " μs\n";
std::cout << "Min: " << samples.front() << " μs\n";
std::cout << "Max: " << samples.back() << " μs\n";
```

### 4. Compiler Optimization Levels

Always benchmark with optimizations enabled:

**Visual Studio:**
```
Debug: /Od (No optimization) - Very slow!
Release: /O2 (Maximize speed) - Use this for benchmarking
```

**GCC/Clang:**
```
Debug: -O0 (No optimization)
Release: -O2 or -O3 (Full optimization)
```

**Comparison:**

```cpp
// Same code, different compiler settings
// Debug build:   1000.0 ms
// Release build:   10.5 ms  ← 95x faster!
```

### 5. Prevent Compiler Over-Optimization

```cpp
// Problem: Compiler might optimize away unused results
int result = compute();  // Compiler: "result is never used, skip compute()!"

// Solution 1: Use volatile
volatile int result = compute();

// Solution 2: Use result in a way compiler can't optimize
int result = compute();
if (result == 12345678) {  // Unlikely condition
    std::cout << "Unexpected value\n";
}

// Solution 3: Google Benchmark's DoNotOptimize
benchmark::DoNotOptimize(compute());
```

## Statistical Analysis of Performance

### Why Statistics Matter

Performance measurements have variability due to:
- OS scheduling
- CPU frequency scaling
- Cache state
- Background processes

**Single measurement**: Unreliable
**Multiple measurements + statistics**: Reliable

### Key Statistical Measures

**1. Mean (Average)**
```cpp
double mean = sum / count;
```
Good for: Overall average performance
Bad for: Hides outliers

**2. Median (Middle Value)**
```cpp
std::sort(samples.begin(), samples.end());
double median = samples[samples.size() / 2];
```
Good for: Typical performance (not affected by outliers)
Bad for: Doesn't show variability

**3. Standard Deviation (Variability)**
```cpp
double stddev = sqrt(variance);
```
Good for: Measuring consistency
Bad for: Hard to interpret alone

**4. Percentiles**
```cpp
std::sort(samples.begin(), samples.end());
double p95 = samples[(int)(samples.size() * 0.95)];  // 95th percentile
double p99 = samples[(int)(samples.size() * 0.99)];  // 99th percentile
```
Good for: Understanding worst-case performance
Example: "95% of requests complete in < 50ms"

### Comparing Two Implementations

```cpp
// Test Implementation A
std::vector<double> samplesA = benchmarkImplementationA();
double meanA = calculateMean(samplesA);

// Test Implementation B
std::vector<double> samplesB = benchmarkImplementationB();
double meanB = calculateMean(samplesB);

// Calculate speedup
double speedup = meanA / meanB;

std::cout << "Implementation B is " << speedup << "x faster\n";

// Example output:
// Implementation B is 2.5x faster
```

## Complete Examples

### Example 1: Simple Benchmark Comparison

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsed() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count();
    }
};

// Slow implementation
int sumArraySlow(const std::vector<int>& arr) {
    int sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// Fast implementation (same algorithm, but compiler can optimize better)
int sumArrayFast(const std::vector<int>& arr) {
    int sum = 0;
    for (int value : arr) {  // Range-based for loop
        sum += value;
    }
    return sum;
}

int main() {
    const int SIZE = 10000000;
    std::vector<int> data(SIZE);

    // Fill with data
    for (int i = 0; i < SIZE; ++i) {
        data[i] = i % 100;
    }

    Timer timer;

    // Benchmark slow version
    timer.reset();
    int result1 = sumArraySlow(data);
    double timeSlow = timer.elapsed();

    // Benchmark fast version
    timer.reset();
    int result2 = sumArrayFast(data);
    double timeFast = timer.elapsed();

    std::cout << "Slow version: " << timeSlow << " μs\n";
    std::cout << "Fast version: " << timeFast << " μs\n";
    std::cout << "Speedup: " << (timeSlow / timeFast) << "x\n";

    return 0;
}
```

### Example 2: Statistical Benchmarking

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>

// [Timer class from previous example]

struct BenchmarkStats {
    double mean;
    double median;
    double stddev;
    double min;
    double max;
    double p95;
    double p99;
};

BenchmarkStats analyzeSamples(std::vector<double> samples) {
    BenchmarkStats stats;

    // Sort for percentiles
    std::sort(samples.begin(), samples.end());

    // Min and Max
    stats.min = samples.front();
    stats.max = samples.back();

    // Median
    stats.median = samples[samples.size() / 2];

    // Mean
    double sum = 0;
    for (double s : samples) sum += s;
    stats.mean = sum / samples.size();

    // Standard deviation
    double variance = 0;
    for (double s : samples) {
        variance += (s - stats.mean) * (s - stats.mean);
    }
    stats.stddev = sqrt(variance / samples.size());

    // Percentiles
    stats.p95 = samples[(size_t)(samples.size() * 0.95)];
    stats.p99 = samples[(size_t)(samples.size() * 0.99)];

    return stats;
}

void printStats(const std::string& name, const BenchmarkStats& stats) {
    std::cout << "\n" << name << ":\n";
    std::cout << "  Mean:   " << stats.mean << " μs\n";
    std::cout << "  Median: " << stats.median << " μs\n";
    std::cout << "  StdDev: " << stats.stddev << " μs\n";
    std::cout << "  Min:    " << stats.min << " μs\n";
    std::cout << "  Max:    " << stats.max << " μs\n";
    std::cout << "  95th:   " << stats.p95 << " μs\n";
    std::cout << "  99th:   " << stats.p99 << " μs\n";
}

int main() {
    const int RUNS = 1000;
    std::vector<int> data(100000, 42);

    std::vector<double> samples;
    Timer timer;

    // Collect samples
    for (int i = 0; i < RUNS; ++i) {
        timer.reset();

        // Code to benchmark
        int sum = 0;
        for (int val : data) {
            sum += val;
        }

        samples.push_back(timer.elapsed());
    }

    // Analyze
    BenchmarkStats stats = analyzeSamples(samples);
    printStats("Array Sum", stats);

    return 0;
}
```

## Exercises

### Exercise 1: Timer Class
Create a cross-platform `Timer` class that supports:
- Starting/stopping
- Pausing/resuming
- Getting elapsed time in different units (μs, ms, s)

### Exercise 2: Function Profiler
Build a profiling system that:
- Tracks function call counts
- Measures total and average execution time
- Reports top 10 slowest functions

### Exercise 3: Benchmark Framework
Create a simple benchmark framework that:
- Runs a function N times
- Collects statistical data
- Compares two implementations
- Reports speedup factor

### Exercise 4: Cache Timing
Write a program that demonstrates cache effects by:
- Accessing array elements sequentially vs randomly
- Measuring the time difference
- Showing how cache improves sequential access

### Exercise 5: Optimization Impact
Take a slow algorithm (e.g., bubble sort on large array) and:
- Benchmark the original
- Optimize it (use quicksort)
- Measure the speedup
- Calculate the time saved

## Summary

In this lesson, you learned:
- ✅ Why performance analysis is critical
- ✅ How to measure execution time accurately
- ✅ Using QueryPerformanceCounter and std::chrono
- ✅ CPU profiling tools (Visual Studio, VTune, Very Sleepy)
- ✅ How to identify bottlenecks
- ✅ Benchmarking best practices
- ✅ Statistical analysis of performance data
- ✅ Comparing implementations quantitatively

## Next Lesson Preview

In Lesson 92, we'll explore:
- Memory optimization techniques
- Cache-friendly data structures
- Data-oriented design
- Memory alignment and struct packing
- Reducing cache misses

## Key Takeaways

1. **Measure first, optimize second** - Never guess where the bottleneck is
2. **Use proper tools** - Profilers reveal what manual inspection cannot
3. **Focus on hotspots** - 90% of time is spent in 10% of code
4. **Statistics matter** - Single measurements are unreliable
5. **Release mode** - Always benchmark with optimizations enabled
6. **Quantify improvements** - "2.5x faster" is better than "feels faster"

---

**Congratulations on completing Lesson 91!**

**Practice time**: Spend 2-3 hours profiling your own code or sample applications to identify bottlenecks and practice using the timing techniques.
