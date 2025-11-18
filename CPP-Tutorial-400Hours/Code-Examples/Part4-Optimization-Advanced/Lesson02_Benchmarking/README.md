# Lesson 02: Complete Benchmarking Framework

## Overview
A professional-grade benchmarking framework for C++ with statistical analysis, setup/teardown support, and performance comparison capabilities.

## Features
- High-resolution timing
- Statistical analysis (min, max, mean, median, std dev)
- Warmup iterations
- Setup/teardown support
- Benchmark suites for comparisons
- Compiler optimization barriers
- Scaling analysis

## Building

### CMake
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
./benchmarking_framework
```

### Visual Studio
Open `Lesson02_Benchmarking.vcxproj` and build in Release mode.

### Manual Build
```bash
g++ -std=c++17 -O3 -o benchmark main.cpp
```

## Framework Usage

### Basic Benchmark
```cpp
Benchmark bench("My Benchmark", 1000, 50);  // 1000 iterations, 50 warmup
auto stats = bench.run([]() {
    // Code to benchmark
});
stats.print("My Benchmark");
```

### Benchmark Suite
```cpp
BenchmarkSuite suite("Comparison", 1000, 50);

suite.add("Method A", []() {
    // Implementation A
});

suite.add("Method B", []() {
    // Implementation B
});

suite.print_results();  // Shows comparison
```

### With Setup/Teardown
```cpp
bench.run_with_setup(
    []() { /* benchmark code */ },
    []() { /* setup */ },
    []() { /* teardown */ }
);
```

## Preventing Compiler Optimizations

Always use `do_not_optimize()` to prevent the compiler from eliminating your benchmark code:

```cpp
int result = expensive_function();
do_not_optimize(result);  // Prevents optimization
```

## Expected Results

The framework will show comparisons like:

```
Benchmark Suite: Vector Summation Comparison
Index Loop: 2.50x slower
Range-based For: baseline
std::accumulate: 1.05x faster
```

## Key Insights
- `std::accumulate` is typically fastest
- Reserving string capacity improves performance by 2-3x
- Object pooling reduces allocation overhead by 10-50x
- Native math functions (`std::sqrt`) are heavily optimized

## Best Practices
1. Always run warmup iterations
2. Use Release builds for accurate results
3. Run multiple iterations for statistical significance
4. Prevent compiler optimizations with `do_not_optimize()`
5. Measure scaling behavior with different input sizes
6. Close other applications during benchmarking

## Next Steps
- Lesson 03: Visual Studio Profiler
- Lesson 04: Intel VTune Profiler
- Lesson 05: CPU Performance Counters
