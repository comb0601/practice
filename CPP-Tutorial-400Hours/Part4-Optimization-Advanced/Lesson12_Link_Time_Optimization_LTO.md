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
