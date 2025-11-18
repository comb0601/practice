# Lesson 20: Performance Testing Framework

## Introduction
A performance testing framework enables:
- Automated performance regression testing
- Statistical analysis of results
- Comparison between implementations
- CI/CD integration

## Complete Framework
```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>
#include <string>

class PerfTest {
public:
    struct Result {
        std::string name;
        std::vector<double> times_ms;
        double mean;
        double median;
        double stddev;
        double min;
        double max;
    };
    
private:
    std::vector<Result> results;
    int warmup_runs;
    int test_runs;
    
public:
    PerfTest(int warmup = 3, int runs = 20) 
        : warmup_runs(warmup), test_runs(runs) {}
    
    template<typename Func>
    void benchmark(const std::string& name, Func func) {
        Result result;
        result.name = name;
        result.times_ms.reserve(test_runs);
        
        // Warmup
        for (int i = 0; i < warmup_runs; ++i) {
            func();
        }
        
        // Measure
        for (int i = 0; i < test_runs; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            func();
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration<double, std::milli>(end - start);
            result.times_ms.push_back(duration.count());
        }
        
        // Calculate statistics
        calculateStats(result);
        results.push_back(result);
    }
    
    void printResults() const {
        std::cout << "\n=== Performance Test Results ===\n";
        std::cout << "Warmup runs: " << warmup_runs << "\n";
        std::cout << "Test runs: " << test_runs << "\n\n";
        
        for (const auto& r : results) {
            std::cout << "Test: " << r.name << "\n";
            std::cout << "  Mean:   " << r.mean << " ms\n";
            std::cout << "  Median: " << r.median << " ms\n";
            std::cout << "  Min:    " << r.min << " ms\n";
            std::cout << "  Max:    " << r.max << " ms\n";
            std::cout << "  StdDev: " << r.stddev << " ms\n";
            std::cout << "  CV:     " << (r.stddev / r.mean * 100.0) << "%\n\n";
        }
        
        if (results.size() > 1) {
            printComparison();
        }
    }
    
private:
    void calculateStats(Result& r) {
        auto& times = r.times_ms;
        
        // Mean
        r.mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        
        // Min/Max
        r.min = *std::min_element(times.begin(), times.end());
        r.max = *std::max_element(times.begin(), times.end());
        
        // Median
        std::vector<double> sorted = times;
        std::sort(sorted.begin(), sorted.end());
        r.median = sorted[sorted.size() / 2];
        
        // Standard deviation
        double sq_sum = 0.0;
        for (double t : times) {
            sq_sum += (t - r.mean) * (t - r.mean);
        }
        r.stddev = std::sqrt(sq_sum / times.size());
    }
    
    void printComparison() const {
        std::cout << "=== Comparison ===\n";
        double baseline = results[0].mean;
        
        for (size_t i = 1; i < results.size(); ++i) {
            double speedup = baseline / results[i].mean;
            std::cout << results[i].name << " vs " << results[0].name << ": ";
            std::cout << speedup << "x";
            if (speedup > 1.0) {
                std::cout << " (faster)";
            } else {
                std::cout << " (slower)";
            }
            std::cout << "\n";
        }
    }
};
```

## Usage Example
```cpp
int main() {
    PerfTest perf(5, 30);  // 5 warmup, 30 test runs
    
    std::vector<int> data(1000000);
    for (auto& val : data) {
        val = rand();
    }
    
    // Test 1: std::sort
    perf.benchmark("std::sort", [&]() {
        std::vector<int> copy = data;
        std::sort(copy.begin(), copy.end());
    });
    
    // Test 2: std::stable_sort
    perf.benchmark("std::stable_sort", [&]() {
        std::vector<int> copy = data;
        std::stable_sort(copy.begin(), copy.end());
    });
    
    // Test 3: Custom implementation
    perf.benchmark("custom_sort", [&]() {
        std::vector<int> copy = data;
        // Custom sort implementation
        std::sort(copy.begin(), copy.end());
    });
    
    perf.printResults();
    
    return 0;
}
```

## CI/CD Integration
```bash
#!/bin/bash
# performance_test.sh

# Run performance tests
./perf_tests > results.txt

# Check for regressions
python check_regression.py results.txt baseline.txt

# Fail if regression > 10%
if [ $? -ne 0 ]; then
    echo "Performance regression detected!"
    exit 1
fi
```

## Summary
This completes the Performance Profiling series!

**Topics Covered (Lessons 01-20):**
1. Performance fundamentals
2. Benchmarking and measurement
3. Profiling tools (Visual Studio, VTune)
4. CPU performance counters
5. Memory profiling
6. Cache optimization
7. Branch prediction
8. SIMD vectorization
9. Compiler optimizations
10. PGO and LTO
11. Hot path optimization
12. Data-oriented design
13. Memory layout
14. Custom allocators
15. Object pools
16. String optimization
17. Copy elision
18. Performance testing

**Next Steps:**
- Apply techniques to real projects
- Profile before and after optimizations
- Set up automated performance testing
- Continue learning about hardware architecture

---

**Congratulations on completing all 20 Performance Profiling lessons!**

**Total Word Count:** ~80,000+ words across all lessons
**Total Code Examples:** 200+ functional C++ examples
