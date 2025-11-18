# Lesson 19: Copy Elision and RVO

## Introduction
Copy Elision and Return Value Optimization (RVO) eliminate unnecessary copies.

## Named Return Value Optimization (NRVO)
```cpp
struct BigObject {
    int data[1000];
    
    BigObject() {
        std::cout << "Constructed\n";
    }
    
    BigObject(const BigObject&) {
        std::cout << "Copied\n";
    }
    
    BigObject(BigObject&&) {
        std::cout << "Moved\n";
    }
};

// Without NRVO (before C++17)
BigObject createObject_Bad() {
    BigObject obj;
    return obj;  // Copy or move
}

// With NRVO (C++17+)
BigObject createObject_Good() {
    BigObject obj;
    return obj;  // No copy, no move!
}

int main() {
    auto obj = createObject_Good();  // Only one construction
    return 0;
}
```

## Copy Elision Examples
```cpp
// Example 1: Prvalue elision (guaranteed since C++17)
BigObject obj = BigObject();  // Only one construction

// Example 2: Function parameter
void process(BigObject obj) {
    // ...
}

process(BigObject());  // Copy elided (since C++17)

// Example 3: Return temporary
BigObject create() {
    return BigObject();  // Copy elided
}
```

## When Elision Fails
```cpp
// Conditional return (NRVO may fail)
BigObject conditionalReturn(bool flag) {
    if (flag) {
        BigObject obj1;
        return obj1;  // Might not elide
    } else {
        BigObject obj2;
        return obj2;  // Might not elide
    }
}

// Solution: Use move
BigObject conditionalReturn_Fixed(bool flag) {
    if (flag) {
        BigObject obj1;
        return std::move(obj1);  // Explicit move
    } else {
        BigObject obj2;
        return std::move(obj2);  // Explicit move
    }
}
```

## Performance Measurement
```cpp
#include <iostream>
#include <chrono>
#include <vector>

struct LargeData {
    std::vector<int> data;
    
    LargeData() : data(10000, 0) {}
    
    LargeData(const LargeData& other) : data(other.data) {
        // Copy
    }
    
    LargeData(LargeData&& other) noexcept : data(std::move(other.data)) {
        // Move
    }
};

LargeData createWithRVO() {
    LargeData obj;
    return obj;  // RVO
}

int main() {
    const int ITERATIONS = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        LargeData obj = createWithRVO();  // RVO eliminates copies
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    std::cout << "Time: " << duration.count() << " ms\n";
    
    return 0;
}
```

## Best Practices
1. Return by value when possible (trust RVO)
2. Don't use std::move on return values (defeats RVO)
3. Use single return path for NRVO
4. Enable optimizations (-O2/-O3)

## Summary
- RVO eliminates copies when returning objects
- Guaranteed in C++17 for prvalues
- NRVO for named return values (compiler-dependent)
- Don't use std::move on return values

**Next:** Performance Testing Framework
