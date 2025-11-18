# Lesson 93: Algorithmic Optimization
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to Algorithmic Optimization](#introduction)
2. [Big O Notation Review](#big-o-notation)
3. [Common Algorithm Complexities](#common-complexities)
4. [Sorting Algorithm Optimization](#sorting)
5. [Search Algorithm Optimization](#searching)
6. [Hash Tables for O(1) Lookup](#hash-tables)
7. [Dynamic Programming](#dynamic-programming)
8. [Space-Time Trade-offs](#space-time-tradeoffs)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction

**Most Important Optimization Rule**: Choose the right algorithm!

Going from O(n²) to O(n log n) provides much bigger gains than any micro-optimization.

### Example Impact
```
n = 10,000
O(n²):      100,000,000 operations   (2 minutes)
O(n log n):      130,000 operations   (0.002 seconds)

Speedup: 770x! No amount of cache optimization achieves this.
```

## Big O Notation Review

### Time Complexity Classes

| Notation | Name | Example | n=1000 ops |
|----------|------|---------|------------|
| O(1) | Constant | Array access | 1 |
| O(log n) | Logarithmic | Binary search | 10 |
| O(n) | Linear | Linear search | 1,000 |
| O(n log n) | Linearithmic | Merge sort | 10,000 |
| O(n²) | Quadratic | Bubble sort | 1,000,000 |
| O(n³) | Cubic | Matrix multiply | 1,000,000,000 |
| O(2ⁿ) | Exponential | Brute force | NEVER FINISHES |

### Rules
- **Drop constants**: O(2n) = O(n)
- **Drop lower terms**: O(n² + n) = O(n²)
- **Multiply nested loops**: O(n) * O(m) = O(n*m)

## Common Algorithm Complexities

### O(1) - Constant Time
```cpp
int getValue(int* array, int index) {
    return array[index];  // Direct access
}
```

### O(log n) - Logarithmic
```cpp
int binarySearch(int* arr, int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
// Each iteration cuts search space in half
```

### O(n) - Linear
```cpp
int findMax(int* arr, int n) {
    int max = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}
```

### O(n log n) - Linearithmic
```cpp
void mergeSort(int* arr, int n);  // Optimal comparison sort
void quickSort(int* arr, int n);  // Average case
```

### O(n²) - Quadratic
```cpp
// Bubble sort - AVOID for large data!
void bubbleSort(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
```

## Sorting Algorithm Optimization

### Bubble Sort: O(n²) - BAD
```cpp
void bubbleSort(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
// Time: O(n²) - Very slow for large arrays
```

### Quick Sort: O(n log n) average - GOOD
```cpp
void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}
// Time: O(n log n) average - Fast
```

### STL Sort: O(n log n) - BEST
```cpp
#include <algorithm>

std::sort(arr, arr + n);  // Optimized introsort (quick+heap+insertion)
// - Fastest in practice
// - Handles worst cases
// - Use this unless you have a specific reason not to
```

### Performance Comparison (n=100,000)
```
Bubble Sort:  ~5000 ms    (O(n²))
Quick Sort:   ~15 ms      (O(n log n))
std::sort:    ~10 ms      (Highly optimized)

Speedup: 500x!
```

## Search Algorithm Optimization

### Linear Search: O(n) - Unsorted Data
```cpp
int linearSearch(int* arr, int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) return i;
    }
    return -1;
}
// Must check every element in worst case
```

### Binary Search: O(log n) - Sorted Data
```cpp
int binarySearch(int* arr, int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
// Only works on sorted data, but much faster
```

### When to Sort for Searching

**Single search**: O(n) linear search
**Multiple searches**: O(n log n) sort + O(log n) × k searches

**Break-even point**:
```
If you search more than ~10 times, sorting first is faster!

Linear: k * n
Binary: n log n + k * log n

When k > log n, binary search wins
```

## Hash Tables for O(1) Lookup

### The Power of Hash Tables

```cpp
// Linear search: O(n)
bool contains_slow(std::vector<int>& vec, int value) {
    for (int v : vec) {
        if (v == value) return true;
    }
    return false;
}

// Hash table: O(1) average
bool contains_fast(std::unordered_set<int>& set, int value) {
    return set.find(value) != set.end();
}

// For n=1,000,000 lookups:
// Linear:     ~500ms
// Hash table: ~5ms
// Speedup: 100x!
```

### Hash Table Applications

**1. Duplicate Detection**
```cpp
// O(n²) - BAD
bool hasDuplicatesSlow(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[i] == arr[j]) return true;
        }
    }
    return false;
}

// O(n) - GOOD
bool hasDuplicatesFast(std::vector<int>& arr) {
    std::unordered_set<int> seen;
    for (int val : arr) {
        if (seen.count(val)) return true;
        seen.insert(val);
    }
    return false;
}
```

**2. Frequency Counting**
```cpp
std::unordered_map<int, int> countFrequencies(std::vector<int>& arr) {
    std::unordered_map<int, int> freq;
    for (int val : arr) {
        freq[val]++;  // O(1) access
    }
    return freq;
}
// Total: O(n) time
```

**3. Two Sum Problem**
```cpp
// O(n²) - BAD
std::pair<int,int> twoSumSlow(std::vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[i] + arr[j] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// O(n) - GOOD
std::pair<int,int> twoSumFast(std::vector<int>& arr, int target) {
    std::unordered_map<int, int> seen;
    for (size_t i = 0; i < arr.size(); ++i) {
        int complement = target - arr[i];
        if (seen.count(complement)) {
            return {seen[complement], (int)i};
        }
        seen[arr[i]] = i;
    }
    return {-1, -1};
}
```

## Dynamic Programming

### Fibonacci: Recursion vs DP

**Naive Recursion: O(2ⁿ) - TERRIBLE**
```cpp
int fibSlow(int n) {
    if (n <= 1) return n;
    return fibSlow(n - 1) + fibSlow(n - 2);  // Exponential!
}
// fib(40) takes ~2 seconds
// fib(50) takes ~2 minutes
```

**Memoization: O(n) - GOOD**
```cpp
std::unordered_map<int, long long> memo;

long long fibFast(int n) {
    if (n <= 1) return n;
    if (memo.count(n)) return memo[n];

    memo[n] = fibFast(n - 1) + fibFast(n - 2);
    return memo[n];
}
// fib(50) takes < 1 millisecond!
```

**Iterative: O(n), O(1) space - BEST**
```cpp
long long fibBest(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

### Performance Comparison (n=40)
```
Recursive:   2000 ms      (O(2ⁿ))
Memoized:    0.05 ms      (O(n))
Iterative:   0.001 ms     (O(n), O(1) space)

Speedup: 2,000,000x!
```

## Space-Time Trade-offs

### Precomputation

```cpp
// Compute on demand: Slow for multiple calls
double distance(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Precompute and cache: Fast for repeated access
class DistanceCache {
    std::unordered_map<std::pair<int,int>, double> cache;
public:
    double getDistance(int i, int j, Point* points) {
        auto key = std::make_pair(i, j);
        if (!cache.count(key)) {
            cache[key] = distance(points[i], points[j]);
        }
        return cache[key];
    }
};
```

### Lookup Tables

```cpp
// Compute sine for every degree
const double SINE_TABLE[360] = { /* precomputed */ };

double fastSin(int degrees) {
    return SINE_TABLE[degrees % 360];  // O(1) vs O(n) for std::sin
}
```

## Summary

**Key Principles**:
1. **Algorithm choice >> micro-optimizations**
2. **O(n²) → O(n log n) = Huge win**
3. **Use hash tables for lookups**: O(1) vs O(n)
4. **Cache expensive computations**: Dynamic programming
5. **Trade space for time when appropriate**

**Algorithm Selection Guide**:
- **Sorting**: Use `std::sort` (O(n log n))
- **Searching**: Use `binary_search` on sorted data (O(log n))
- **Lookups**: Use `unordered_map/unordered_set` (O(1))
- **Avoid**: Bubble sort, linear search on sorted data, naive recursion

---

**Congratulations on completing Lesson 93!**
