# Lesson 01: Performance Optimization Introduction

## Overview
This lesson introduces fundamental performance optimization concepts through practical examples and benchmarks.

## Topics Covered
1. **Cache Locality** - Memory access patterns and their impact on performance
2. **Algorithm Complexity** - Comparing O(n²) vs O(n log n) algorithms
3. **Memory Allocation** - Pre-allocation vs dynamic allocation
4. **Branch Prediction** - Effects of predictable vs unpredictable branches

## Building the Project

### Using CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./perf_intro
```

### Using Visual Studio
1. Open `Lesson01_PerfIntro.vcxproj` in Visual Studio 2019+
2. Build the solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

### Using Command Line (Windows)
```cmd
cl /EHsc /O2 /std:c++17 main.cpp /Fe:perf_intro.exe
perf_intro.exe
```

### Using Command Line (Linux/Mac)
```bash
g++ -std=c++17 -O2 -o perf_intro main.cpp
./perf_intro
```

## Expected Output
The program will run four benchmarks and display timing results:

```
Performance Optimization Introduction
======================================

============================================================
Cache Locality Benchmark
============================================================
Inefficient (column-major) took: XXXXX microseconds
Efficient (row-major)    took: XXXXX microseconds
Both sums equal: YES

============================================================
Algorithm Complexity Benchmark
============================================================
Bubble Sort (O(n²))     took: XXXXX microseconds
std::sort (O(n log n)) took: XXXXX microseconds
Both sorted correctly: YES

...
```

## Key Performance Insights

### 1. Cache Locality (Typical Results)
- **Column-major access**: ~8-15ms (cache misses)
- **Row-major access**: ~1-3ms (cache hits)
- **Speedup**: 3-10x faster with proper access patterns

### 2. Algorithm Complexity (Typical Results)
- **Bubble Sort O(n²)**: ~200-500ms for 10,000 elements
- **std::sort O(n log n)**: ~1-3ms for 10,000 elements
- **Speedup**: 100-200x faster with better algorithm

### 3. Memory Allocation (Typical Results)
- **Without reserve()**: ~15-30ms (multiple reallocations)
- **With reserve()**: ~3-8ms (single allocation)
- **Speedup**: 3-5x faster with pre-allocation

### 4. Branch Prediction (Typical Results)
- **Sorted data**: ~1-2ms (predictable branches)
- **Random data**: ~3-6ms (unpredictable branches)
- **Speedup**: 2-3x faster with predictable branches

## Experiments to Try

1. **Cache Locality**
   - Change matrix dimensions (100x100, 1000x1000, 5000x5000)
   - Observe how cache effects scale with data size

2. **Algorithm Complexity**
   - Try different array sizes (1,000, 10,000, 100,000)
   - Plot execution time vs array size

3. **Memory Allocation**
   - Test with different iteration counts
   - Monitor memory usage during execution

4. **Branch Prediction**
   - Try different data patterns (sorted, reverse-sorted, random)
   - Measure the impact on execution time

## Performance Tools

### Windows
- **Visual Studio Profiler**: Debug → Performance Profiler
- **PerfView**: Download from Microsoft GitHub
- **Windows Performance Analyzer**: Part of Windows SDK

### Linux
- **perf**: `perf stat ./perf_intro`
- **valgrind**: `valgrind --tool=cachegrind ./perf_intro`
- **gprof**: Compile with `-pg` flag

### Cross-Platform
- **Google Benchmark**: For detailed microbenchmarks
- **Tracy Profiler**: Real-time profiling
- **Intel VTune**: Advanced performance analysis

## Next Steps
- Lesson 02: Complete Benchmarking Framework
- Lesson 03: Visual Studio Profiler Deep Dive
- Lesson 04: Intel VTune Profiling

## Additional Resources
- "Computer Systems: A Programmer's Perspective" by Bryant & O'Hallaron
- "Optimizing Software in C++" by Agner Fog
- CppCon talks on performance optimization
- Chandler Carruth's talks on performance

## Notes
- Results may vary based on CPU, compiler, and system load
- Run multiple times for consistent measurements
- Disable other applications for accurate benchmarks
- Use Release/optimized builds for realistic results
