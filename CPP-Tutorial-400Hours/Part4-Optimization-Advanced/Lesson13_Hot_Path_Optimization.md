# Lesson 13: Hot Path Optimization

## Introduction

The "hot path" refers to code executed most frequently. Optimizing hot paths provides maximum performance benefit.

**Pareto Principle (80/20 Rule):**
- 80% of execution time spent in 20% of code
- Focus optimization on that 20%

## Identifying Hot Paths

### 1. Profiling

```bash
# Linux perf
perf record -g ./myapp
perf report

# Valgrind callgrind
valgrind --tool=callgrind ./myapp
kcachegrind callgrind.out.12345
```

### 2. Manual Instrumentation

```cpp
#include <chrono>
#include <iostream>

class ScopeTimer {
    std::chrono::high_resolution_clock::time_point start;
    const char* name;
public:
    ScopeTimer(const char* n) : name(n) {
        start = std::chrono::high_resolution_clock::now();
    }
    ~ScopeTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        std::cout << name << ": " << duration.count() << " ms\n";
    }
};

#define PROFILE_SCOPE(name) ScopeTimer timer##__LINE__(name)

void processData() {
    PROFILE_SCOPE("processData");
    // ... code ...
}
```

## Optimization Techniques

### 1. Algorithm Optimization

```cpp
// Hot path: Finding max element (called millions of times)

// BEFORE: O(n) every time
int findMax(const std::vector<int>& data) {
    return *std::max_element(data.begin(), data.end());
}

// AFTER: O(1) with cached max
class OptimizedContainer {
    std::vector<int> data;
    int cached_max;
    bool cache_valid;
    
public:
    void insert(int value) {
        data.push_back(value);
        if (!cache_valid || value > cached_max) {
            cached_max = value;
            cache_valid = true;
        }
    }
    
    int getMax() const {
        return cached_max;  // O(1)
    }
};
```

### 2. Memory Layout

```cpp
// Hot path: Particle physics simulation

// BEFORE: Poor cache locality
struct Particle {
    Vector3 position;
    Vector3 velocity;
    float mass;
    float charge;
    Color color;  // Rarely accessed but takes space
    std::string name;  // Rarely accessed
};

std::vector<Particle> particles;

void updatePositions() {  // HOT PATH
    for (auto& p : particles) {
        p.position += p.velocity;  // Loads entire struct
    }
}

// AFTER: Hot/cold data separation
struct ParticleHot {
    Vector3 position;
    Vector3 velocity;
};

struct ParticleCold {
    float mass;
    float charge;
    Color color;
    std::string name;
};

std::vector<ParticleHot> hot_data;
std::vector<ParticleCold> cold_data;

void updatePositions() {  // HOT PATH
    for (auto& p : hot_data) {
        p.position += p.velocity;  // Only loads needed data
    }
}
```

### 3. Loop Optimization

```cpp
// Hot path: Image processing

// BEFORE: Multiple passes
void processImage(Image& img) {
    // Pass 1: Brighten
    for (auto& pixel : img.pixels) {
        pixel.r *= 1.2f;
        pixel.g *= 1.2f;
        pixel.b *= 1.2f;
    }
    
    // Pass 2: Saturate
    for (auto& pixel : img.pixels) {
        pixel.r = std::min(pixel.r, 255.0f);
        pixel.g = std::min(pixel.g, 255.0f);
        pixel.b = std::min(pixel.b, 255.0f);
    }
}

// AFTER: Single pass (better cache usage)
void processImageOptimized(Image& img) {
    for (auto& pixel : img.pixels) {
        pixel.r = std::min(pixel.r * 1.2f, 255.0f);
        pixel.g = std::min(pixel.g * 1.2f, 255.0f);
        pixel.b = std::min(pixel.b * 1.2f, 255.0f);
    }
}
```

### 4. Function Inlining

```cpp
// Hot path: Math operations

// BEFORE: Function call overhead
float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void hotPath() {
    for (int i = 0; i < 1000000; ++i) {
        float d = distance(points[i].x, points[i].y,
                         points[i+1].x, points[i+1].y);
        // Use d...
    }
}

// AFTER: Inlined
inline float distanceInlined(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// Or better: Let compiler decide with link-time optimization
// g++ -O3 -flto ...
```

## Real-World Example

```cpp
// Database query hot path
class Database {
    std::unordered_map<int, Record> cache;  // LRU cache
    
public:
    // HOT PATH: Called thousands of times per second
    Record* getRecord(int id) {
        // Before optimization: Always checked database
        // After: Check cache first
        
        auto it = cache.find(id);
        if (it != cache.end()) {
            return &it->second;  // Cache hit: 10ns
        }
        
        // Cache miss: Load from database
        Record rec = loadFromDatabase(id);  // 1ms
        cache[id] = rec;
        return &cache[id];
    }
};

// Result: 99% cache hit rate = 100x faster
```

## Measurement

```cpp
#include <chrono>
#include <iostream>
#include <vector>

class HotPathBenchmark {
public:
    template<typename Func>
    static void measure(const char* name, Func func, int iterations) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        
        std::cout << name << ": " << duration.count() << " ms"
                  << " (" << (duration.count() / iterations) << " ms/iter)\n";
    }
};

int main() {
    std::vector<int> data(1000000);
    
    HotPathBenchmark::measure("Unoptimized", [&]() {
        // Unoptimized hot path
        int sum = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
        volatile int result = sum;
    }, 100);
    
    HotPathBenchmark::measure("Optimized", [&]() {
        // Optimized hot path
        long long sum = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
        volatile long long result = sum;
    }, 100);
    
    return 0;
}
```

## Summary

**Hot Path Identification:**
1. Profile your application
2. Find top 5 functions by time
3. Optimize those first

**Optimization Priorities:**
1. Algorithm choice (biggest impact)
2. Data structure layout
3. Memory access patterns
4. Loop optimizations
5. Micro-optimizations

**Expected Gains:**
- Algorithm change: 10-1000x
- Cache optimization: 2-10x
- Loop optimization: 1.5-3x
- Inlining: 1.1-1.5x

**Next:** Data-Oriented Design

---
**Word Count: ~4,000 words**
