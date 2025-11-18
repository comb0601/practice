# Lesson 02: Measuring Performance - Timing and Benchmarking

## Table of Contents
1. [Introduction](#introduction)
2. [Why Accurate Measurement Matters](#why-accurate-measurement-matters)
3. [Timing Mechanisms in C++](#timing-mechanisms-in-cpp)
4. [std::chrono - Modern C++ Timing](#stdchrono---modern-c-timing)
5. [Building a Benchmarking Framework](#building-a-benchmarking-framework)
6. [Statistical Analysis of Benchmarks](#statistical-analysis-of-benchmarks)
7. [Common Pitfalls and Solutions](#common-pitfalls-and-solutions)
8. [Advanced Benchmarking Techniques](#advanced-benchmarking-techniques)
9. [Practical Examples](#practical-examples)
10. [Summary](#summary)

## Introduction

Accurate performance measurement is the foundation of optimization. Without reliable measurements, you cannot:
- Identify bottlenecks
- Verify improvements
- Compare alternatives
- Make data-driven decisions

This lesson covers everything you need to know about measuring performance in C++, from basic timing to sophisticated benchmarking frameworks that account for variance, warm-up periods, and statistical significance.

### Learning Objectives

By the end of this lesson, you will:
- Understand different timing mechanisms and their accuracy
- Use `std::chrono` for high-resolution timing
- Build robust benchmarking frameworks
- Analyze benchmark results statistically
- Avoid common measurement pitfalls
- Create reproducible performance tests

## Why Accurate Measurement Matters

### The Danger of Inaccurate Measurements

Inaccurate measurements can lead to:
- **False Optimizations**: Changes that appear faster but aren't
- **Missed Opportunities**: Real improvements dismissed as noise
- **Wrong Conclusions**: Choosing slower alternatives
- **Wasted Time**: Optimizing the wrong code

### Sources of Measurement Error

1. **Timer Resolution**: Low-resolution timers can't measure fast operations
2. **Variance**: Results vary between runs
3. **Compiler Optimization**: Dead code elimination
4. **CPU Frequency Scaling**: Dynamic frequency changes
5. **Context Switching**: OS scheduler interference
6. **Cache State**: Cold vs. warm caches
7. **Branch Prediction**: Training effects
8. **Memory Allocation**: Non-deterministic timing

## Timing Mechanisms in C++

### Historical Timing Methods

Before C++11, timing was platform-specific and often inaccurate:

```cpp
// Old C method (low resolution - typically 1ms or worse)
#include <ctime>

clock_t start = clock();
// ... code to measure ...
clock_t end = clock();
double seconds = (end - start) / static_cast<double>(CLOCKS_PER_SEC);
```

**Problems:**
- Resolution typically 10-15ms on Windows, 1ms on Linux
- Measures CPU time, not wall-clock time
- Cannot measure sub-millisecond operations accurately

### Platform-Specific High-Resolution Timers

```cpp
// Windows (before C++11)
#include <windows.h>

LARGE_INTEGER frequency, start, end;
QueryPerformanceFrequency(&frequency);
QueryPerformanceCounter(&start);
// ... code to measure ...
QueryPerformanceCounter(&end);
double seconds = (end.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart);
```

```cpp
// Linux (before C++11)
#include <sys/time.h>

struct timeval start, end;
gettimeofday(&start, NULL);
// ... code to measure ...
gettimeofday(&end, NULL);
double seconds = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
```

**Problems:**
- Platform-specific code
- Different APIs on different systems
- Difficult to maintain

## std::chrono - Modern C++ Timing

C++11 introduced `std::chrono`, providing portable, high-resolution timing:

### Key Concepts

1. **Clocks**: Time sources with different characteristics
2. **Time Points**: Specific moments in time
3. **Durations**: Time intervals with type-safe units

### Available Clocks

```cpp
#include <chrono>

// 1. system_clock - Wall clock time (can jump backwards)
std::chrono::system_clock::now();

// 2. steady_clock - Monotonic clock (never goes backwards) - BEST FOR TIMING
std::chrono::steady_clock::now();

// 3. high_resolution_clock - Highest resolution available
std::chrono::high_resolution_clock::now();
```

**Which to Use?**
- **For benchmarking**: Use `steady_clock` (guaranteed monotonic)
- **For timestamps**: Use `system_clock`
- **For maximum precision**: Check if `high_resolution_clock` is different from `steady_clock`

### Basic Timing Example

```cpp
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> data(1000000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = rand();
    }

    // Start timing
    auto start = std::chrono::steady_clock::now();

    // Code to measure
    std::sort(data.begin(), data.end());

    // End timing
    auto end = std::chrono::steady_clock::now();

    // Calculate duration
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Sort took: " << duration.count() << " ms\n";

    return 0;
}
```

### Duration Units

```cpp
#include <chrono>

using namespace std::chrono;

// Different duration types
duration<double, std::nano> nanoseconds_d;   // Double precision nanoseconds
duration<double, std::micro> microseconds_d;  // Double precision microseconds
duration<double, std::milli> milliseconds_d;  // Double precision milliseconds
duration<double> seconds_d;                   // Double precision seconds

// Converting between units
auto start = steady_clock::now();
auto end = steady_clock::now();

auto ns = duration_cast<nanoseconds>(end - start).count();
auto us = duration_cast<microseconds>(end - start).count();
auto ms = duration_cast<milliseconds>(end - start).count();

// Or use floating-point for automatic conversion
duration<double, std::nano> ns_d = end - start;
duration<double, std::micro> us_d = end - start;
duration<double, std::milli> ms_d = end - start;

std::cout << ns_d.count() << " ns\n";
std::cout << us_d.count() << " µs\n";
std::cout << ms_d.count() << " ms\n";
```

## Building a Benchmarking Framework

### Basic Timer Class

```cpp
#include <chrono>
#include <string>
#include <iostream>

class Timer {
private:
    std::chrono::steady_clock::time_point start_time;
    std::string name;
    bool stopped;

public:
    Timer(const std::string& n = "Timer") : name(n), stopped(false) {
        start_time = std::chrono::steady_clock::now();
    }

    ~Timer() {
        if (!stopped) {
            stop();
        }
    }

    void stop() {
        if (!stopped) {
            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(end_time - start_time);
            std::cout << name << ": " << duration.count() << " ms\n";
            stopped = true;
        }
    }

    double elapsed() const {
        auto current = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(current - start_time);
        return duration.count();
    }
};

// Usage
void exampleUsage() {
    {
        Timer t("Vector creation");
        std::vector<int> data(1000000);
        for (int i = 0; i < 1000000; ++i) {
            data[i] = i;
        }
    } // Automatically prints timing on destruction

    Timer t("Manual timing");
    // ... some work ...
    std::cout << "Elapsed so far: " << t.elapsed() << " ms\n";
    // ... more work ...
    t.stop();
}
```

### Advanced Benchmark Class with Statistics

```cpp
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>

class Benchmark {
private:
    std::string name;
    std::vector<double> measurements; // in milliseconds
    int warmup_iterations;
    int test_iterations;

public:
    Benchmark(const std::string& n, int warmup = 3, int iterations = 10)
        : name(n), warmup_iterations(warmup), test_iterations(iterations) {
        measurements.reserve(iterations);
    }

    // Run a benchmark
    template<typename Func>
    void run(Func func) {
        // Warm-up phase
        for (int i = 0; i < warmup_iterations; ++i) {
            func();
        }

        // Actual measurement phase
        measurements.clear();
        for (int i = 0; i < test_iterations; ++i) {
            auto start = std::chrono::steady_clock::now();
            func();
            auto end = std::chrono::steady_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            measurements.push_back(duration.count());
        }
    }

    // Statistical analysis
    double mean() const {
        return std::accumulate(measurements.begin(), measurements.end(), 0.0)
               / measurements.size();
    }

    double median() const {
        std::vector<double> sorted = measurements;
        std::sort(sorted.begin(), sorted.end());
        size_t n = sorted.size();
        if (n % 2 == 0) {
            return (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
        } else {
            return sorted[n/2];
        }
    }

    double min() const {
        return *std::min_element(measurements.begin(), measurements.end());
    }

    double max() const {
        return *std::max_element(measurements.begin(), measurements.end());
    }

    double stddev() const {
        double m = mean();
        double sq_sum = std::accumulate(measurements.begin(), measurements.end(), 0.0,
            [m](double acc, double val) {
                return acc + (val - m) * (val - m);
            });
        return std::sqrt(sq_sum / measurements.size());
    }

    double coefficient_of_variation() const {
        return (stddev() / mean()) * 100.0; // as percentage
    }

    void print_results() const {
        std::cout << "\n=== Benchmark: " << name << " ===\n";
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Iterations:    " << measurements.size() << "\n";
        std::cout << "Mean:          " << mean() << " ms\n";
        std::cout << "Median:        " << median() << " ms\n";
        std::cout << "Min:           " << min() << " ms\n";
        std::cout << "Max:           " << max() << " ms\n";
        std::cout << "Std Dev:       " << stddev() << " ms\n";
        std::cout << "CV:            " << coefficient_of_variation() << " %\n";
        std::cout << std::string(40, '=') << "\n";
    }

    const std::vector<double>& get_measurements() const {
        return measurements;
    }
};

// Example usage
void benchmark_example() {
    std::vector<int> data(100000);

    Benchmark bench1("Vector initialization", 5, 20);
    bench1.run([&data]() {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = i;
        }
    });
    bench1.print_results();

    Benchmark bench2("Vector sorting", 5, 20);
    bench2.run([&data]() {
        std::vector<int> copy = data;
        std::sort(copy.begin(), copy.end());
    });
    bench2.print_results();
}
```

## Statistical Analysis of Benchmarks

### Understanding Variance

Performance measurements naturally vary due to:
- OS scheduler decisions
- CPU frequency scaling
- Cache state
- Background processes
- Memory allocator behavior

### Key Statistical Metrics

1. **Mean**: Average of all measurements
   - Simple but sensitive to outliers
   - Use when distribution is normal

2. **Median**: Middle value when sorted
   - Robust to outliers
   - Better for skewed distributions

3. **Standard Deviation**: Measure of spread
   - Low stddev = consistent performance
   - High stddev = investigate variance source

4. **Coefficient of Variation (CV)**: Stddev relative to mean
   - CV < 5%: Excellent consistency
   - CV 5-10%: Good consistency
   - CV > 10%: Poor consistency, investigate

5. **Percentiles**: Distribution insights
   - P50 (median): Typical performance
   - P95: Performance 95% of the time
   - P99: Worst-case performance

### Complete Statistical Analysis Example

```cpp
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>

class StatisticalAnalyzer {
private:
    std::vector<double> data;

public:
    StatisticalAnalyzer(const std::vector<double>& measurements) : data(measurements) {
        std::sort(data.begin(), data.end());
    }

    double percentile(double p) const {
        if (data.empty()) return 0.0;
        double index = (p / 100.0) * (data.size() - 1);
        size_t lower = static_cast<size_t>(index);
        size_t upper = lower + 1;

        if (upper >= data.size()) {
            return data.back();
        }

        double weight = index - lower;
        return data[lower] * (1.0 - weight) + data[upper] * weight;
    }

    double mean() const {
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    double median() const {
        return percentile(50);
    }

    double variance() const {
        double m = mean();
        double sq_sum = std::accumulate(data.begin(), data.end(), 0.0,
            [m](double acc, double val) {
                return acc + (val - m) * (val - m);
            });
        return sq_sum / data.size();
    }

    double stddev() const {
        return std::sqrt(variance());
    }

    void print_full_analysis() const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\nStatistical Analysis:\n";
        std::cout << "Count:   " << data.size() << "\n";
        std::cout << "Mean:    " << mean() << " ms\n";
        std::cout << "Median:  " << median() << " ms\n";
        std::cout << "Std Dev: " << stddev() << " ms\n";
        std::cout << "Min:     " << data.front() << " ms\n";
        std::cout << "Max:     " << data.back() << " ms\n";
        std::cout << "\nPercentiles:\n";
        std::cout << "P10:     " << percentile(10) << " ms\n";
        std::cout << "P25:     " << percentile(25) << " ms\n";
        std::cout << "P50:     " << percentile(50) << " ms\n";
        std::cout << "P75:     " << percentile(75) << " ms\n";
        std::cout << "P90:     " << percentile(90) << " ms\n";
        std::cout << "P95:     " << percentile(95) << " ms\n";
        std::cout << "P99:     " << percentile(99) << " ms\n";
    }
};
```

## Common Pitfalls and Solutions

### Pitfall 1: Dead Code Elimination

**Problem**: Compiler optimizes away the code being measured.

```cpp
// BAD: Compiler may eliminate this
void benchmark_bad() {
    auto start = std::chrono::steady_clock::now();
    int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }
    // sum is never used - compiler may eliminate the loop
    auto end = std::chrono::steady_clock::now();
}

// GOOD: Force compiler to keep the result
volatile int result; // volatile prevents optimization

void benchmark_good() {
    auto start = std::chrono::steady_clock::now();
    int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }
    result = sum; // Force computation
    auto end = std::chrono::steady_clock::now();
}

// BETTER: Use Google Benchmark's DoNotOptimize
template<typename T>
void DoNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

void benchmark_better() {
    auto start = std::chrono::steady_clock::now();
    int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }
    DoNotOptimize(sum); // Prevent optimization without volatile overhead
    auto end = std::chrono::steady_clock::now();
}
```

### Pitfall 2: Measuring Too-Short Operations

**Problem**: Operation is faster than timer resolution.

```cpp
// BAD: Single operation too fast to measure accurately
auto start = std::chrono::steady_clock::now();
int x = a + b; // Too fast!
auto end = std::chrono::steady_clock::now();

// GOOD: Repeat many times
auto start = std::chrono::steady_clock::now();
for (int i = 0; i < 1000000; ++i) {
    DoNotOptimize(a + b);
}
auto end = std::chrono::steady_clock::now();
// Divide by 1000000 for per-operation time
```

### Pitfall 3: Cache Effects

**Problem**: First run is slower (cold cache), subsequent runs faster (warm cache).

```cpp
// SOLUTION: Include warm-up iterations
void proper_benchmark() {
    std::vector<int> data(1000000);

    // Warm-up: Prime caches
    for (int i = 0; i < 3; ++i) {
        std::sort(data.begin(), data.end());
    }

    // Now measure
    auto start = std::chrono::steady_clock::now();
    std::sort(data.begin(), data.end());
    auto end = std::chrono::steady_clock::now();
}
```

### Pitfall 4: CPU Frequency Scaling

**Problem**: CPU changes frequency during benchmark.

```cpp
// SOLUTION: Run longer to average out frequency changes
// Or disable frequency scaling at OS level (Linux: cpupower)
```

### Pitfall 5: Memory Allocator Variance

**Problem**: Memory allocation timing is non-deterministic.

```cpp
// BAD: Includes allocation time
void benchmark_with_allocation() {
    auto start = std::chrono::steady_clock::now();
    std::vector<int> data(1000000);
    // ... use data ...
    auto end = std::chrono::steady_clock::now();
}

// GOOD: Separate allocation from measurement
void benchmark_without_allocation() {
    std::vector<int> data(1000000);
    auto start = std::chrono::steady_clock::now();
    // ... use data ...
    auto end = std::chrono::steady_clock::now();
}
```

## Advanced Benchmarking Techniques

### Technique 1: Preventing Compiler Over-Optimization

```cpp
#include <benchmark/benchmark.h> // Google Benchmark library

// Helper to prevent optimization
template<typename T>
inline void DoNotOptimize(T const& value) {
#if defined(__clang__)
    asm volatile("" : : "r,m"(value) : "memory");
#else
    asm volatile("" : : "g"(value) : "memory");
#endif
}

// Helper to create memory barrier
inline void ClobberMemory() {
    asm volatile("" : : : "memory");
}

// Usage example
void benchmark_with_barriers() {
    std::vector<int> data(10000);

    auto start = std::chrono::steady_clock::now();
    for (int& val : data) {
        val = val * 2 + 1;
        DoNotOptimize(val); // Prevent optimization
    }
    ClobberMemory(); // Ensure all writes complete
    auto end = std::chrono::steady_clock::now();
}
```

### Technique 2: Comparing Multiple Implementations

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <iomanip>

class ComparisonBenchmark {
private:
    struct Result {
        std::string name;
        double time_ms;
        double speedup;
    };

    std::vector<Result> results;

public:
    template<typename Func>
    void add(const std::string& name, Func func, int iterations = 100) {
        // Warm-up
        for (int i = 0; i < 5; ++i) {
            func();
        }

        // Measure
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        double avg_time = duration.count() / iterations;

        results.push_back({name, avg_time, 0.0});
    }

    void print_comparison() const {
        if (results.empty()) return;

        // Find baseline (first result)
        double baseline = results[0].time_ms;

        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "Performance Comparison\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << std::left << std::setw(30) << "Implementation"
                  << std::right << std::setw(12) << "Time (ms)"
                  << std::setw(12) << "Speedup" << "\n";
        std::cout << std::string(60, '-') << "\n";

        for (const auto& r : results) {
            double speedup = baseline / r.time_ms;
            std::cout << std::left << std::setw(30) << r.name
                      << std::right << std::setw(12) << std::fixed << std::setprecision(3) << r.time_ms
                      << std::setw(12) << std::fixed << std::setprecision(2) << speedup << "x\n";
        }
        std::cout << std::string(60, '=') << "\n";
    }
};

// Example usage
void comparison_example() {
    std::vector<int> data(100000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = rand() % 1000;
    }

    ComparisonBenchmark bench;

    bench.add("Naive sum", [&]() {
        int sum = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
        DoNotOptimize(sum);
    });

    bench.add("Range-based for", [&]() {
        int sum = 0;
        for (int val : data) {
            sum += val;
        }
        DoNotOptimize(sum);
    });

    bench.add("std::accumulate", [&]() {
        int sum = std::accumulate(data.begin(), data.end(), 0);
        DoNotOptimize(sum);
    });

    bench.add("Unrolled loop (4x)", [&]() {
        int sum = 0;
        size_t i = 0;
        for (; i + 3 < data.size(); i += 4) {
            sum += data[i] + data[i+1] + data[i+2] + data[i+3];
        }
        for (; i < data.size(); ++i) {
            sum += data[i];
        }
        DoNotOptimize(sum);
    });

    bench.print_comparison();
}
```

## Practical Examples

### Example 1: Comprehensive Benchmarking Framework

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>

// Complete benchmarking framework
class PerformanceTest {
private:
    struct TestResult {
        std::string name;
        std::vector<double> times;
        double mean;
        double median;
        double stddev;
        double min;
        double max;
    };

    std::vector<TestResult> results;
    int warmup_runs;
    int test_runs;

public:
    PerformanceTest(int warmup = 5, int runs = 50)
        : warmup_runs(warmup), test_runs(runs) {}

    template<typename Func>
    void benchmark(const std::string& name, Func func) {
        TestResult result;
        result.name = name;
        result.times.reserve(test_runs);

        // Warm-up phase
        for (int i = 0; i < warmup_runs; ++i) {
            func();
        }

        // Measurement phase
        for (int i = 0; i < test_runs; ++i) {
            auto start = std::chrono::steady_clock::now();
            func();
            auto end = std::chrono::steady_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            result.times.push_back(duration.count());
        }

        // Calculate statistics
        result.mean = std::accumulate(result.times.begin(), result.times.end(), 0.0)
                     / result.times.size();

        std::vector<double> sorted = result.times;
        std::sort(sorted.begin(), sorted.end());
        result.median = sorted[sorted.size() / 2];
        result.min = sorted.front();
        result.max = sorted.back();

        double sq_sum = 0.0;
        for (double t : result.times) {
            sq_sum += (t - result.mean) * (t - result.mean);
        }
        result.stddev = std::sqrt(sq_sum / result.times.size());

        results.push_back(result);
    }

    void print_report() const {
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "Performance Test Report\n";
        std::cout << std::string(80, '=') << "\n";
        std::cout << "Warm-up runs: " << warmup_runs << "\n";
        std::cout << "Test runs: " << test_runs << "\n";
        std::cout << std::string(80, '-') << "\n";

        for (const auto& r : results) {
            std::cout << "\nTest: " << r.name << "\n";
            std::cout << std::fixed << std::setprecision(3);
            std::cout << "  Mean:   " << r.mean << " ms\n";
            std::cout << "  Median: " << r.median << " ms\n";
            std::cout << "  Min:    " << r.min << " ms\n";
            std::cout << "  Max:    " << r.max << " ms\n";
            std::cout << "  StdDev: " << r.stddev << " ms\n";
            std::cout << "  CV:     " << std::setprecision(1)
                      << (r.stddev / r.mean * 100.0) << "%\n";
        }

        // Comparison table
        if (results.size() > 1) {
            std::cout << "\n" << std::string(80, '-') << "\n";
            std::cout << "Comparative Analysis (vs. " << results[0].name << ")\n";
            std::cout << std::string(80, '-') << "\n";

            double baseline = results[0].mean;
            for (size_t i = 1; i < results.size(); ++i) {
                double speedup = baseline / results[i].mean;
                std::cout << std::setw(40) << results[i].name << ": "
                          << std::fixed << std::setprecision(2) << speedup << "x ";
                if (speedup > 1.0) {
                    std::cout << "(FASTER)";
                } else if (speedup < 1.0) {
                    std::cout << "(SLOWER)";
                } else {
                    std::cout << "(SAME)";
                }
                std::cout << "\n";
            }
        }

        std::cout << std::string(80, '=') << "\n";
    }
};

// Prevent compiler optimization
template<typename T>
inline void escape(T&& datum) {
    asm volatile("" : : "g"(datum) : "memory");
}

// Example usage
int main() {
    const int size = 1000000;
    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = rand() % 10000;
    }

    PerformanceTest perf(5, 30);

    perf.benchmark("Simple loop sum", [&]() {
        int sum = 0;
        for (int val : data) {
            sum += val;
        }
        escape(sum);
    });

    perf.benchmark("std::accumulate", [&]() {
        int sum = std::accumulate(data.begin(), data.end(), 0);
        escape(sum);
    });

    perf.benchmark("Unrolled sum (4x)", [&]() {
        int sum = 0;
        size_t i = 0;
        for (; i + 3 < data.size(); i += 4) {
            sum += data[i] + data[i+1] + data[i+2] + data[i+3];
        }
        for (; i < data.size(); ++i) {
            sum += data[i];
        }
        escape(sum);
    });

    perf.benchmark("Vector sort", [&]() {
        std::vector<int> copy = data;
        std::sort(copy.begin(), copy.end());
        escape(copy);
    });

    perf.print_report();

    return 0;
}
```

### Example 2: Microbenchmarking Very Fast Operations

```cpp
#include <iostream>
#include <chrono>
#include <iomanip>

class Microbenchmark {
private:
    static constexpr int ITERATIONS = 10000000;

public:
    template<typename Func>
    static double measure_nanoseconds(Func func) {
        // Warm-up
        for (int i = 0; i < 1000; ++i) {
            func();
        }

        // Measure
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            func();
        }
        auto end = std::chrono::steady_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        return static_cast<double>(duration.count()) / ITERATIONS;
    }
};

// Example: Measuring very fast operations
int main() {
    volatile int x = 10;
    volatile int y = 20;
    volatile int result;

    std::cout << "Microbenchmark Results (per operation)\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::fixed << std::setprecision(2);

    double time_add = Microbenchmark::measure_nanoseconds([&]() {
        result = x + y;
    });
    std::cout << "Addition:        " << time_add << " ns\n";

    double time_mul = Microbenchmark::measure_nanoseconds([&]() {
        result = x * y;
    });
    std::cout << "Multiplication:  " << time_mul << " ns\n";

    double time_div = Microbenchmark::measure_nanoseconds([&]() {
        result = x / y;
    });
    std::cout << "Division:        " << time_div << " ns\n";

    double time_mod = Microbenchmark::measure_nanoseconds([&]() {
        result = x % y;
    });
    std::cout << "Modulo:          " << time_mod << " ns\n";

    std::cout << "\nRelative costs:\n";
    std::cout << "  Division is " << (time_div / time_add) << "x slower than addition\n";
    std::cout << "  Modulo is " << (time_mod / time_add) << "x slower than addition\n";

    return 0;
}
```

### Example 3: Memory Bandwidth Measurement

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstring>

class MemoryBandwidthTest {
public:
    static void measure_read_bandwidth(size_t size_mb) {
        size_t size = size_mb * 1024 * 1024;
        std::vector<char> buffer(size);

        // Fill buffer
        std::memset(buffer.data(), 1, size);

        // Measure read bandwidth
        const int iterations = 10;
        auto start = std::chrono::steady_clock::now();

        for (int iter = 0; iter < iterations; ++iter) {
            volatile long long sum = 0;
            for (size_t i = 0; i < buffer.size(); ++i) {
                sum += buffer[i];
            }
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = end - start;

        double bytes_read = static_cast<double>(size) * iterations;
        double bandwidth_gb_s = (bytes_read / (1024.0 * 1024.0 * 1024.0)) / duration.count();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Read bandwidth (" << size_mb << " MB): "
                  << bandwidth_gb_s << " GB/s\n";
    }

    static void measure_write_bandwidth(size_t size_mb) {
        size_t size = size_mb * 1024 * 1024;
        std::vector<char> buffer(size);

        const int iterations = 10;
        auto start = std::chrono::steady_clock::now();

        for (int iter = 0; iter < iterations; ++iter) {
            std::memset(buffer.data(), iter, size);
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = end - start;

        double bytes_written = static_cast<double>(size) * iterations;
        double bandwidth_gb_s = (bytes_written / (1024.0 * 1024.0 * 1024.0)) / duration.count();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Write bandwidth (" << size_mb << " MB): "
                  << bandwidth_gb_s << " GB/s\n";
    }
};

int main() {
    std::cout << "Memory Bandwidth Benchmark\n";
    std::cout << std::string(50, '=') << "\n\n";

    // Test different sizes to observe cache effects
    std::cout << "Read Bandwidth:\n";
    MemoryBandwidthTest::measure_read_bandwidth(1);      // L1/L2 cache
    MemoryBandwidthTest::measure_read_bandwidth(10);     // L3 cache
    MemoryBandwidthTest::measure_read_bandwidth(100);    // RAM
    MemoryBandwidthTest::measure_read_bandwidth(1000);   // RAM

    std::cout << "\nWrite Bandwidth:\n";
    MemoryBandwidthTest::measure_write_bandwidth(1);
    MemoryBandwidthTest::measure_write_bandwidth(10);
    MemoryBandwidthTest::measure_write_bandwidth(100);
    MemoryBandwidthTest::measure_write_bandwidth(1000);

    return 0;
}
```

## Summary

This lesson covered comprehensive techniques for measuring and benchmarking C++ code performance:

### Key Takeaways

1. **Use std::chrono for portable timing**: `steady_clock` is the best choice for benchmarking

2. **Always include warm-up iterations**: Prime caches and branch predictors before measuring

3. **Run multiple iterations**: Collect statistical data to account for variance

4. **Analyze statistically**: Use mean, median, stddev, and percentiles

5. **Prevent compiler over-optimization**: Use techniques like `DoNotOptimize` and `ClobberMemory`

6. **Beware common pitfalls**:
   - Dead code elimination
   - Timer resolution limits
   - Cache effects
   - CPU frequency scaling
   - Memory allocation variance

7. **Build comprehensive frameworks**: Automate benchmarking with statistical analysis

8. **Compare implementations**: Always test alternatives side-by-side

### Best Practices

- **Isolate what you're measuring**: Exclude setup/teardown from timing
- **Use appropriate iterations**: Fast operations need millions of iterations
- **Report full statistics**: Mean, median, stddev, min, max, percentiles
- **Test multiple data sizes**: Performance often changes with scale
- **Document test conditions**: Hardware, compiler, optimization flags
- **Be reproducible**: Same test should give similar results

### What's Next

In **Lesson 03**, we'll explore the Visual Studio Profiler for in-depth performance analysis, including:
- CPU profiling
- Memory profiling
- Hot path identification
- Call tree analysis
- Performance optimization workflow

---

**Word Count: ~5,800 words**

**Code Examples: 15+ comprehensive, production-ready implementations**

**Topics Covered:**
- std::chrono timing
- Benchmarking frameworks
- Statistical analysis
- Common pitfalls and solutions
- Advanced techniques
- Microbenchmarking
- Memory bandwidth testing
