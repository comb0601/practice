# Lesson 18: Small String Optimization (SSO)

## Introduction
Small String Optimization stores short strings directly in the string object, avoiding heap allocation.

**std::string typically:**
- Short strings (<15-23 chars): Stored inline
- Long strings: Heap allocated

## How SSO Works
```cpp
// Conceptual implementation
class String {
    union {
        struct {
            char* data;
            size_t capacity;
        } heap;
        
        char buffer[16];  // Small buffer for SSO
    } storage;
    
    size_t size;
    bool isSmall;
};

// Short string: "Hello" → stored in buffer (no allocation)
// Long string: "Very long string..." → heap allocated
```

## Custom SSO Implementation
```cpp
#include <cstring>
#include <algorithm>

class SmallString {
    static constexpr size_t SSO_SIZE = 15;
    
    union {
        char* heap;
        char buffer[SSO_SIZE + 1];
    };
    
    size_t length;
    bool isHeap;
    
public:
    SmallString() : length(0), isHeap(false) {
        buffer[0] = '\0';
    }
    
    SmallString(const char* str) {
        length = std::strlen(str);
        
        if (length <= SSO_SIZE) {
            isHeap = false;
            std::memcpy(buffer, str, length + 1);
        } else {
            isHeap = true;
            heap = new char[length + 1];
            std::memcpy(heap, str, length + 1);
        }
    }
    
    ~SmallString() {
        if (isHeap) {
            delete[] heap;
        }
    }
    
    const char* c_str() const {
        return isHeap ? heap : buffer;
    }
    
    size_t size() const { return length; }
};
```

## Performance Comparison
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

void testSSO() {
    const int COUNT = 1000000;
    
    // Short strings (SSO)
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> shortStrings;
    for (int i = 0; i < COUNT; ++i) {
        shortStrings.push_back("Short");  // No heap allocation
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    
    // Long strings (heap allocated)
    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> longStrings;
    for (int i = 0; i < COUNT; ++i) {
        longStrings.push_back("This is a very long string that won't fit in SSO");
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    
    auto t1 = std::chrono::duration<double, std::milli>(end1 - start1);
    auto t2 = std::chrono::duration<double, std::milli>(end2 - start2);
    
    std::cout << "Short strings (SSO): " << t1.count() << " ms\n";
    std::cout << "Long strings:        " << t2.count() << " ms\n";
    std::cout << "SSO speedup:         " << (t2.count() / t1.count()) << "x\n";
}

int main() {
    testSSO();
    return 0;
}
```

**Result:** SSO is 3-5x faster for short strings

## SSO Guidelines
1. Keep strings under 15 characters when possible
2. Use `string_view` for temporary string references
3. Avoid unnecessary string copies
4. Profile to verify SSO is being used

## Summary
- std::string uses SSO automatically
- Short strings (<15-23 chars): No allocation
- 3-5x faster than heap allocation
- Design APIs to favor short strings

**Next:** Copy Elision and RVO
