# Lesson 15: Memory Layout and Alignment

## Introduction
Memory alignment affects performance significantly. Misaligned data causes:
- Slower access (multiple cache lines)
- Potential crashes on some architectures
- Wasted memory bandwidth

## Alignment Basics
```cpp
struct Unaligned {
    char c;      // 1 byte
    int i;       // 4 bytes (padded to 4-byte boundary)
    char c2;     // 1 byte
    double d;    // 8 bytes (padded to 8-byte boundary)
};
// Size: 24 bytes (with padding)

struct alignas(64) Aligned {
    char c;
    int i;
    char c2;
    double d;
};
// Size: 64 bytes (aligned to cache line)
```

## Code Examples
```cpp
#include <iostream>
#include <chrono>
#include <vector>

// Misaligned structure
struct Misaligned {
    char c1;
    int i1;
    char c2;
    int i2;
};  // 16 bytes with padding

// Optimally packed structure
struct Packed {
    int i1;
    int i2;
    char c1;
    char c2;
};  // 12 bytes

// Cache-line aligned
struct alignas(64) CacheAligned {
    int value;
    char padding[60];
};

int main() {
    std::cout << "Misaligned size: " << sizeof(Misaligned) << "\n";
    std::cout << "Packed size: " << sizeof(Packed) << "\n";
    std::cout << "CacheAligned size: " << sizeof(CacheAligned) << "\n";
    
    // Performance test
    const int COUNT = 10000000;
    std::vector<Packed> packed(COUNT);
    
    auto start = std::chrono::high_resolution_clock::now();
    for (auto& p : packed) {
        p.i1 = p.i2 + 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    std::cout << "Time: " << duration.count() << " ms\n";
    
    return 0;
}
```

## Summary
- Use `alignas()` for cache-line alignment
- Pack structures to minimize padding
- Align hot data to cache lines (64 bytes)

**Next:** Custom Memory Allocators
