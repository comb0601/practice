# Lesson 92: Memory Optimization & Cache Efficiency
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to Memory Optimization](#introduction-to-memory-optimization)
2. [Understanding CPU Cache](#understanding-cpu-cache)
3. [Cache-Friendly Data Structures](#cache-friendly-data-structures)
4. [Data-Oriented Design](#data-oriented-design)
5. [Memory Alignment](#memory-alignment)
6. [Struct Packing and Padding](#struct-packing-and-padding)
7. [Reducing Cache Misses](#reducing-cache-misses)
8. [Memory Access Patterns](#memory-access-patterns)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction to Memory Optimization

### The Memory Hierarchy

Modern computers have a memory hierarchy with different speeds and sizes:

```
CPU Registers      <1 cycle    ~1 KB      FASTEST, SMALLEST
L1 Cache           ~4 cycles   32-64 KB   ↑
L2 Cache           ~12 cycles  256-512 KB ↑
L3 Cache           ~40 cycles  8-16 MB    ↑
RAM                ~200 cycles 8-64 GB    ↑
SSD                ~100,000 cycles        ↑
HDD                ~10,000,000 cycles     SLOWEST, LARGEST
```

**Key Insight**: Accessing RAM is ~50x slower than L1 cache!

### Why Cache Matters

```cpp
// Example: Cache vs RAM speed difference
// L1 cache access: 4 cycles = 1 nanosecond @ 4GHz
// RAM access:      200 cycles = 50 nanoseconds @ 4GHz

// If you access 1 million integers:
// All in cache:  1 million ns = 1 millisecond
// All from RAM:  50 million ns = 50 milliseconds (50x slower!)
```

### Cache Lines

The CPU loads data in chunks called **cache lines**:
- Typical size: **64 bytes**
- When you access one byte, CPU loads 64 bytes into cache
- Accessing nearby data is essentially free!

```cpp
struct Data {
    int value;        // 4 bytes
    char padding[60]; // 60 bytes padding
};  // Total: 64 bytes = 1 cache line

Data arr[100];  // 100 cache lines

// Accessing arr[0].value loads entire cache line
// Accessing arr[0].padding is free (already in cache)
// Accessing arr[1].value requires new cache line
```

## Understanding CPU Cache

### Cache Organization

**L1 Cache** (Per Core):
- Smallest: ~32 KB
- Fastest: ~4 cycles
- Separated: L1i (instructions) + L1d (data)

**L2 Cache** (Per Core):
- Medium: ~256 KB
- Fast: ~12 cycles
- Unified (instructions + data)

**L3 Cache** (Shared):
- Largest: ~8 MB
- Slower: ~40 cycles
- Shared between all cores

### Cache Miss Types

**1. Cold Miss (Compulsory)**
```cpp
int arr[1000];
// First access to arr[0] is always a cache miss
int x = arr[0];  // COLD MISS - data not in cache yet
```

**2. Capacity Miss**
```cpp
int huge_array[10000000];
// Array too large to fit in cache
// Later accesses evict earlier data
```

**3. Conflict Miss**
```cpp
// Two different addresses map to same cache location
// One evicts the other
```

### Temporal vs Spatial Locality

**Temporal Locality**: Accessing the same data repeatedly
```cpp
for (int i = 0; i < 1000; ++i) {
    sum += same_variable;  // Same data, reused
}
```

**Spatial Locality**: Accessing nearby data
```cpp
for (int i = 0; i < 1000; ++i) {
    sum += array[i];  // Sequential access
}
```

## Cache-Friendly Data Structures

### Array vs Linked List

**Array (Cache-Friendly)**
```cpp
std::vector<int> arr = {1, 2, 3, 4, 5};
// Memory: [1][2][3][4][5] - contiguous
// Access: One cache miss loads multiple elements
```

**Linked List (Cache-Hostile)**
```cpp
std::list<int> lst = {1, 2, 3, 4, 5};
// Memory: [1]→[2]→[3]→[4]→[5] - scattered
// Access: Each element is a cache miss
```

**Performance Comparison:**
```
Array:       1 cache miss for 16 integers (64 bytes / 4 bytes)
Linked List: 16 cache misses (one per node + pointer)

Result: Array is ~16x faster for sequential access!
```

### Structure of Arrays (SoA) vs Array of Structures (AoS)

**Array of Structures (AoS) - Common but not optimal**
```cpp
struct Particle {
    float x, y, z;     // 12 bytes
    float vx, vy, vz;  // 12 bytes
    float age;         // 4 bytes
    float lifetime;    // 4 bytes
};  // 32 bytes per particle

Particle particles[1000];

// Update positions only
for (int i = 0; i < 1000; ++i) {
    particles[i].x += particles[i].vx;  // Loads entire 32-byte struct
    particles[i].y += particles[i].vy;  // Wastes 20 bytes per particle
    particles[i].z += particles[i].vz;
}
```

**Structure of Arrays (SoA) - Optimal for cache**
```cpp
struct Particles {
    float x[1000], y[1000], z[1000];
    float vx[1000], vy[1000], vz[1000];
    float age[1000];
    float lifetime[1000];
};

Particles particles;

// Update positions only - accesses only needed data
for (int i = 0; i < 1000; ++i) {
    particles.x[i] += particles.vx[i];  // Only loads position/velocity
    particles.y[i] += particles.vy[i];  // No wasted cache space
    particles.z[i] += particles.vz[i];
}

// Result: 2-4x faster due to better cache utilization!
```

## Data-Oriented Design

### Principles

1. **Separate hot and cold data**: Frequently accessed data together
2. **Structure of Arrays**: Group same fields together
3. **Minimize pointer chasing**: Use indices instead of pointers
4. **Batch processing**: Process many items at once

### Example: Game Entity System

**Object-Oriented (Cache-Hostile)**
```cpp
class Entity {
    Vector3 position;     // Hot - updated every frame
    Vector3 velocity;     // Hot - updated every frame
    Mesh* mesh;           // Hot - rendered every frame
    std::string name;     // Cold - rarely accessed
    int id;               // Cold - rarely accessed
    Texture* texture;     // Cold - rarely changed
    // More cold data...
};

std::vector<Entity*> entities;  // Pointer indirection!

// Update loop - cache misses everywhere
for (Entity* e : entities) {
    e->position += e->velocity;  // Loads entire Entity + follows pointer
}
```

**Data-Oriented (Cache-Friendly)**
```cpp
struct EntitySystem {
    // Hot data - accessed every frame
    std::vector<Vector3> positions;
    std::vector<Vector3> velocities;
    std::vector<MeshID> meshes;

    // Cold data - separate structure
    std::vector<std::string> names;
    std::vector<int> ids;
};

EntitySystem entities;

// Update loop - excellent cache usage
for (size_t i = 0; i < entities.positions.size(); ++i) {
    entities.positions[i] += entities.velocities[i];
    // Only loads hot data, sequential access
}

// Result: 5-10x faster!
```

## Memory Alignment

### What is Alignment?

Memory alignment means data starts at addresses that are multiples of its size:

```cpp
// 4-byte int should start at address divisible by 4
int x;  // Address: 0x1000 ✓ (aligned)
        // Address: 0x1001 ✗ (misaligned)

// Misaligned access is slower or crashes on some CPUs!
```

### Natural Alignment

```cpp
char    c;  // 1-byte alignment
short   s;  // 2-byte alignment
int     i;  // 4-byte alignment
float   f;  // 4-byte alignment
double  d;  // 8-byte alignment
void*   p;  // 4-byte (32-bit) or 8-byte (64-bit)
```

### Cache Line Alignment (64 bytes)

```cpp
// Align to cache line for performance-critical data
struct alignas(64) CacheLineAligned {
    int data[16];  // 64 bytes = 1 cache line
};

// This ensures entire struct fits in one cache line
// Avoids split loads across two cache lines
```

### SIMD Alignment (16/32 bytes)

```cpp
// SSE requires 16-byte alignment
alignas(16) float vec4[4];  // 16 bytes

// AVX requires 32-byte alignment
alignas(32) float vec8[8];  // 32 bytes

// Misaligned SIMD access is MUCH slower or crashes
```

## Struct Packing and Padding

### Automatic Padding

The compiler adds padding to align struct members:

```cpp
struct Example {
    char  a;   // 1 byte
    // 3 bytes padding
    int   b;   // 4 bytes
    char  c;   // 1 byte
    // 7 bytes padding
    double d;  // 8 bytes
};  // Total: 24 bytes (not 14!)

// Memory layout:
// [a][pad][pad][pad][b b b b][c][pad pad pad pad pad pad pad][d d d d d d d d]
```

### Optimal Ordering

**Bad (13 bytes wasted)**:
```cpp
struct Bad {
    char  a;   // 1 byte
    // 7 bytes padding
    double d;  // 8 bytes
    char  b;   // 1 byte
    // 3 bytes padding
    int   c;   // 4 bytes
};  // Total: 24 bytes
```

**Good (only 3 bytes wasted)**:
```cpp
struct Good {
    double d;  // 8 bytes
    int    c;  // 4 bytes
    char   a;  // 1 byte
    char   b;  // 1 byte
    // 2 bytes padding
};  // Total: 16 bytes
```

**Best Practice**: Order members from largest to smallest.

### Tight Packing (Be Careful!)

```cpp
#pragma pack(push, 1)  // No padding
struct Packed {
    char  a;   // 1 byte
    int   b;   // 4 bytes
    char  c;   // 1 byte
};  // Total: 6 bytes
#pragma pack(pop)

// WARNING: Misaligned access to 'b' is slower!
// Only use when memory size matters more than speed
```

## Reducing Cache Misses

### 1. Sequential Access

```cpp
// GOOD: Sequential access
for (int i = 0; i < n; ++i) {
    sum += array[i];  // Cache-friendly
}

// BAD: Random access
for (int i = 0; i < n; ++i) {
    sum += array[random_indices[i]];  // Cache-hostile
}
```

### 2. Loop Tiling (Blocking)

```cpp
// BAD: Large stride, poor cache reuse
for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
        C[i][j] = A[i][j] + B[i][j];
    }
}

// GOOD: Process in tiles that fit in cache
const int TILE = 64;
for (int ii = 0; ii < N; ii += TILE) {
    for (int jj = 0; jj < N; jj += TILE) {
        for (int i = ii; i < min(ii + TILE, N); ++i) {
            for (int j = jj; j < min(jj + TILE, N); ++j) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
    }
}
```

### 3. Prefetching

```cpp
// Manual prefetch hint
for (int i = 0; i < n; ++i) {
    _mm_prefetch(&array[i + 8], _MM_HINT_T0);  // Prefetch 8 elements ahead
    process(array[i]);
}
```

### 4. False Sharing Prevention

```cpp
// BAD: False sharing in multithreading
struct Counter {
    int count;  // 4 bytes
};  // Threads modifying different counters still share cache line!

// GOOD: Pad to separate cache lines
struct Counter {
    alignas(64) int count;  // 64-byte aligned
};  // Each counter on different cache line
```

## Memory Access Patterns

### Row-Major vs Column-Major

C++ uses **row-major** order for 2D arrays:

```cpp
int matrix[3][4];  // 3 rows, 4 columns

// Memory layout (row-major):
// [0,0][0,1][0,2][0,3][1,0][1,1][1,2][1,3][2,0][2,1][2,2][2,3]

// GOOD: Access by rows (sequential)
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 4; ++j) {
        sum += matrix[i][j];  // Cache-friendly
    }
}

// BAD: Access by columns (strided)
for (int j = 0; j < 4; ++j) {
    for (int i = 0; i < 3; ++i) {
        sum += matrix[i][j];  // Cache-hostile
    }
}
```

### Pointer Chasing

```cpp
// BAD: Pointer chasing
struct Node {
    int data;
    Node* next;
};

Node* current = head;
while (current) {
    process(current->data);  // Each access is a cache miss!
    current = current->next;
}

// GOOD: Use array/vector
std::vector<int> data;
for (int value : data) {
    process(value);  // Sequential, cache-friendly
}
```

## Complete Examples

[See code files for complete working examples]

## Exercises

### Exercise 1: AoS to SoA Conversion
Convert an Array of Structures particle system to Structure of Arrays and measure the performance improvement.

### Exercise 2: Struct Size Optimization
Given a poorly ordered struct, reorder members to minimize padding and measure memory savings.

### Exercise 3: Cache Tiling
Implement matrix multiplication with and without tiling. Measure performance difference.

### Exercise 4: False Sharing
Create a multithreaded counter with false sharing, then fix it with alignment. Measure speedup.

### Exercise 5: Memory Access Pattern
Compare row-major vs column-major matrix traversal and measure cache miss rates.

## Summary

In this lesson, you learned:
- ✅ CPU cache hierarchy and importance
- ✅ Cache lines and cache misses
- ✅ Cache-friendly data structures (arrays vs lists)
- ✅ Data-oriented design principles
- ✅ Structure of Arrays (SoA) pattern
- ✅ Memory alignment for performance
- ✅ Struct packing and padding optimization
- ✅ Techniques to reduce cache misses
- ✅ Sequential vs random access patterns

## Next Lesson Preview

In Lesson 93, we'll explore:
- Algorithmic optimization
- Big O notation in practice
- Replacing O(n²) with O(n log n) algorithms
- Hash tables for O(1) lookup
- Real-world optimization examples

## Key Takeaways

1. **Cache is king** - RAM access is 50x slower than L1 cache
2. **Sequential access** - Always prefer contiguous memory
3. **Data-oriented design** - Structure data for access patterns
4. **SoA over AoS** - Better cache utilization for batch processing
5. **Alignment matters** - For both correctness and performance
6. **Measure, don't guess** - Profile to verify improvements

---

**Congratulations on completing Lesson 92!**

**Practice time**: Spend 2-3 hours converting your existing code to use cache-friendly patterns and measuring the improvements.
