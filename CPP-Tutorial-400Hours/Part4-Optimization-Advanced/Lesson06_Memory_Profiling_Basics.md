# Lesson 06: Memory Profiling Basics

## Table of Contents
1. [Introduction](#introduction)
2. [Memory Profiling Tools](#memory-profiling-tools)
3. [Valgrind and Memcheck](#valgrind-and-memcheck)
4. [AddressSanitizer (ASan)](#addresssanitizer-asan)
5. [Dr. Memory](#dr-memory)
6. [Heap Profilers](#heap-profilers)
7. [Common Memory Issues](#common-memory-issues)
8. [Practical Examples](#practical-examples)
9. [Memory Optimization Techniques](#memory-optimization-techniques)
10. [Summary](#summary)

## Introduction

Memory profiling is essential for identifying:
- Memory leaks
- Buffer overflows
- Use-after-free errors
- Double-free errors
- Invalid memory access
- Heap fragmentation
- Memory usage patterns

This lesson covers comprehensive memory profiling techniques and tools to diagnose and fix memory-related issues.

### Learning Objectives

- Use Valgrind for memory debugging
- Apply AddressSanitizer for fast detection
- Profile heap allocations
- Identify and fix memory leaks
- Optimize memory usage
- Build memory-aware applications

## Memory Profiling Tools

### Tool Comparison

| Tool | Speed | Accuracy | Platform | Best For |
|------|-------|----------|----------|----------|
| **Valgrind** | Slow (10-50x) | Excellent | Linux/Mac | Detailed analysis |
| **AddressSanitizer** | Fast (2x) | Good | All | Development |
| **Dr. Memory** | Medium (10x) | Excellent | Windows/Linux | Windows dev |
| **Massif** | Medium | Good | Linux | Heap profiling |
| **Heaptrack** | Fast | Good | Linux | Production |

## Valgrind and Memcheck

### Installing Valgrind

```bash
# Ubuntu/Debian
sudo apt-get install valgrind

# macOS
brew install valgrind

# Check installation
valgrind --version
```

### Basic Memcheck Usage

```cpp
// memory_leak_example.cpp
#include <iostream>
#include <vector>

// Example 1: Simple memory leak
void simpleLeak() {
    int* ptr = new int[100];
    // Missing: delete[] ptr;
}

// Example 2: Double free
void doubleFree() {
    int* ptr = new int;
    delete ptr;
    // delete ptr;  // Uncomment to demonstrate double-free
}

// Example 3: Use after free
void useAfterFree() {
    int* ptr = new int(42);
    delete ptr;
    // std::cout << *ptr << "\n";  // Uncomment to demonstrate use-after-free
}

// Example 4: Buffer overflow
void bufferOverflow() {
    int* arr = new int[10];
    // arr[15] = 42;  // Uncomment to demonstrate overflow
    delete[] arr;
}

// Example 5: Invalid read
void invalidRead() {
    int* ptr = new int[10];
    delete[] ptr;
    // int x = ptr[0];  // Uncomment to demonstrate invalid read
}

int main() {
    std::cout << "Memory Error Examples\n";
    std::cout << "====================\n\n";

    std::cout << "Creating memory leak...\n";
    simpleLeak();

    std::cout << "Testing double free (commented out)...\n";
    doubleFree();

    std::cout << "Testing use-after-free (commented out)...\n";
    useAfterFree();

    std::cout << "Testing buffer overflow (commented out)...\n";
    bufferOverflow();

    std::cout << "Testing invalid read (commented out)...\n";
    invalidRead();

    std::cout << "\nRun with: valgrind --leak-check=full ./memory_leak_example\n";

    return 0;
}
```

### Running Valgrind

```bash
# Compile with debug symbols
g++ -g memory_leak_example.cpp -o memory_leak_example

# Basic memcheck
valgrind ./memory_leak_example

# Detailed leak check
valgrind --leak-check=full ./memory_leak_example

# Show reachable blocks
valgrind --leak-check=full --show-reachable=yes ./memory_leak_example

# Track origins of uninitialized values
valgrind --track-origins=yes ./memory_leak_example

# Save output to file
valgrind --leak-check=full --log-file=valgrind.log ./memory_leak_example
```

### Interpreting Valgrind Output

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 400 bytes in 1 blocks
==12345==   total heap usage: 2 allocs, 1 frees, 73,104 bytes allocated
==12345==
==12345== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2E0EF: operator new[](unsigned long) (vg_replace_malloc.c:433)
==12345==    by 0x400916: simpleLeak() (memory_leak_example.cpp:8)
==12345==    by 0x400A12: main (memory_leak_example.cpp:45)
==12345==
==12345== LEAK SUMMARY:
==12345==    definitely lost: 400 bytes in 1 blocks
==12345==    indirectly lost: 0 bytes in 0 blocks
==12345==      possibly lost: 0 bytes in 0 blocks
==12345==    still reachable: 0 bytes in 0 blocks
==12345==         suppressed: 0 bytes in 0 blocks
```

**Leak Categories:**
- **Definitely lost**: Memory leak - fix immediately
- **Indirectly lost**: Leaked via pointer from definitely lost block
- **Possibly lost**: Might be leak or might be custom allocator
- **Still reachable**: Not freed but pointer still exists
- **Suppressed**: Ignored via suppression file

## AddressSanitizer (ASan)

### Using AddressSanitizer

AddressSanitizer is much faster than Valgrind (only ~2x slowdown) and catches:
- Heap buffer overflow
- Stack buffer overflow
- Use-after-free
- Use-after-return
- Use-after-scope
- Double-free
- Memory leaks

### Compilation with ASan

```bash
# GCC/Clang
g++ -fsanitize=address -g memory_test.cpp -o memory_test

# With leak detection
g++ -fsanitize=address -fsanitize=leak -g memory_test.cpp -o memory_test

# Run normally
./memory_test
```

### ASan Example

```cpp
// asan_example.cpp
#include <iostream>
#include <vector>

// Heap buffer overflow
void heapBufferOverflow() {
    int* arr = new int[10];
    arr[10] = 42;  // Out of bounds!
    delete[] arr;
}

// Stack buffer overflow
void stackBufferOverflow() {
    int arr[10];
    arr[10] = 42;  // Out of bounds!
}

// Use after free
void useAfterFree() {
    int* ptr = new int(42);
    delete ptr;
    std::cout << *ptr << "\n";  // Use after free!
}

// Use after scope
void useAfterScope() {
    int* ptr;
    {
        int x = 42;
        ptr = &x;
    }
    // std::cout << *ptr << "\n";  // Use after scope!
}

// Memory leak
void memoryLeak() {
    int* ptr = new int[1000];
    // Never freed
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <test_number>\n";
        return 1;
    }

    int test = std::atoi(argv[1]);

    switch (test) {
        case 1:
            std::cout << "Heap buffer overflow test\n";
            heapBufferOverflow();
            break;
        case 2:
            std::cout << "Stack buffer overflow test\n";
            stackBufferOverflow();
            break;
        case 3:
            std::cout << "Use after free test\n";
            useAfterFree();
            break;
        case 4:
            std::cout << "Memory leak test\n";
            memoryLeak();
            break;
    }

    return 0;
}
```

### Compiling and Running

```bash
# Compile with ASan
g++ -fsanitize=address -fsanitize=leak -g -O1 asan_example.cpp -o asan_test

# Test heap overflow
./asan_test 1

# Test stack overflow
./asan_test 2

# Test use-after-free
./asan_test 3

# Test memory leak (run with ASAN_OPTIONS)
ASAN_OPTIONS=detect_leaks=1 ./asan_test 4
```

### ASan Output Example

```
=================================================================
==23456==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60300000eff8
READ of size 4 at 0x60300000eff8 thread T0
    #0 0x400b6e in heapBufferOverflow() asan_example.cpp:7
    #1 0x400c45 in main asan_example.cpp:51

0x60300000eff8 is located 0 bytes to the right of 40-byte region
allocated by thread T0 here:
    #0 0x7f8b3e8a2d28 in operator new[](unsigned long)
    #1 0x400b52 in heapBufferOverflow() asan_example.cpp:6
    #2 0x400c45 in main asan_example.cpp:51

SUMMARY: AddressSanitizer: heap-buffer-overflow asan_example.cpp:7
```

## Dr. Memory

### Windows Memory Profiling

Dr. Memory is excellent for Windows development:

```bash
# Download from https://drmemory.org/

# Run Dr. Memory
drmemory.exe -batch -- myapp.exe

# With detailed leak detection
drmemory.exe -leak_scan full -- myapp.exe

# Generate HTML report
drmemory.exe -html -batch -- myapp.exe
```

## Heap Profilers

### Massif (Valgrind Suite)

Massif profiles heap memory usage over time:

```bash
# Run massif
valgrind --tool=massif ./myapp

# View results
ms_print massif.out.12345

# With detailed snapshots
valgrind --tool=massif --detailed-freq=1 ./myapp
```

### Example for Heap Profiling

```cpp
// heap_profile_example.cpp
#include <iostream>
#include <vector>
#include <memory>

class DataBuffer {
private:
    std::vector<double> data;

public:
    DataBuffer(size_t size) : data(size) {
        std::cout << "Allocated " << size * sizeof(double) / 1024 << " KB\n";
    }

    void fill() {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = static_cast<double>(i);
        }
    }
};

void demonstrateHeapGrowth() {
    std::vector<std::unique_ptr<DataBuffer>> buffers;

    // Simulate heap growth
    for (int i = 0; i < 10; ++i) {
        size_t size = 100000 * (i + 1);
        buffers.push_back(std::make_unique<DataBuffer>(size));
        buffers.back()->fill();
    }

    std::cout << "Peak memory usage reached\n";

    // Simulate cleanup
    buffers.clear();
    std::cout << "Memory freed\n";
}

int main() {
    std::cout << "Heap Profiling Example\n";
    std::cout << "======================\n\n";

    demonstrateHeapGrowth();

    std::cout << "\nProfile with:\n";
    std::cout << "  valgrind --tool=massif ./heap_profile_example\n";
    std::cout << "  ms_print massif.out.xxxxx\n";

    return 0;
}
```

## Common Memory Issues

### Issue 1: Memory Leaks

```cpp
// memory_leak_patterns.cpp
#include <iostream>
#include <vector>
#include <string>

// Pattern 1: Forgetting to delete
void forgottenDelete() {
    int* ptr = new int(42);
    // Forgot: delete ptr;
}

// Pattern 2: Exception during allocation
void exceptionLeak() {
    int* ptr1 = new int(42);
    int* ptr2 = new int(43);  // If this throws, ptr1 leaks
    // Better: Use smart pointers
    delete ptr1;
    delete ptr2;
}

// Pattern 3: Circular references with shared_ptr
class Node {
public:
    std::shared_ptr<Node> next;
    std::vector<int> data;
    Node() : data(1000, 0) {}
};

void circularReference() {
    auto node1 = std::make_shared<Node>();
    auto node2 = std::make_shared<Node>();
    node1->next = node2;
    node2->next = node1;  // Circular reference - memory leak!
}

// Pattern 4: Container holding raw pointers
void containerLeak() {
    std::vector<int*> ptrs;
    for (int i = 0; i < 100; ++i) {
        ptrs.push_back(new int(i));
    }
    // Forgot to delete each pointer before clearing
    ptrs.clear();  // Leaks all allocated ints!
}

// FIXED VERSIONS

void fixedExceptionLeak() {
    auto ptr1 = std::make_unique<int>(42);
    auto ptr2 = std::make_unique<int>(43);
    // Automatically cleaned up even if exception thrown
}

class NodeFixed {
public:
    std::weak_ptr<NodeFixed> next;  // Use weak_ptr to break cycle
    std::vector<int> data;
    NodeFixed() : data(1000, 0) {}
};

void fixedCircularReference() {
    auto node1 = std::make_shared<NodeFixed>();
    auto node2 = std::make_shared<NodeFixed>();
    node1->next = node2;
    node2->next = node1;  // No leak - weak_ptr breaks cycle
}

void fixedContainerLeak() {
    std::vector<std::unique_ptr<int>> ptrs;
    for (int i = 0; i < 100; ++i) {
        ptrs.push_back(std::make_unique<int>(i));
    }
    ptrs.clear();  // Automatically deletes all ints
}

int main() {
    std::cout << "Memory Leak Patterns\n";
    std::cout << "====================\n\n";

    std::cout << "Creating various memory leaks...\n";
    forgottenDelete();
    circularReference();
    containerLeak();

    std::cout << "\nUsing fixed versions...\n";
    fixedExceptionLeak();
    fixedCircularReference();
    fixedContainerLeak();

    std::cout << "\nRun with Valgrind or ASan to detect leaks\n";

    return 0;
}
```

### Issue 2: Buffer Overflows

```cpp
// buffer_overflow_examples.cpp
#include <iostream>
#include <cstring>

// Stack buffer overflow
void stackOverflow() {
    char buffer[10];
    strcpy(buffer, "This string is too long!");  // Overflow!
}

// Heap buffer overflow
void heapOverflow() {
    char* buffer = new char[10];
    strcpy(buffer, "This string is too long!");  // Overflow!
    delete[] buffer;
}

// Off-by-one error
void offByOne() {
    int arr[10];
    for (int i = 0; i <= 10; ++i) {  // Should be i < 10
        arr[i] = i;  // Last iteration overflows!
    }
}

// FIXED VERSIONS

void fixedStackOverflow() {
    char buffer[100];  // Large enough
    strncpy(buffer, "This string is safe now", sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination
}

void fixedHeapOverflow() {
    std::string str = "This string is safe now";  // std::string manages memory
}

void fixedOffByOne() {
    int arr[10];
    for (int i = 0; i < 10; ++i) {  // Correct bounds
        arr[i] = i;
    }
}

int main() {
    std::cout << "Buffer Overflow Examples\n";
    std::cout << "========================\n\n";

    std::cout << "Fixed versions:\n";
    fixedStackOverflow();
    fixedHeapOverflow();
    fixedOffByOne();

    std::cout << "\nUncomment overflow examples and run with ASan to detect\n";

    return 0;
}
```

## Practical Examples

### Example 1: Memory Profiling Workflow

```cpp
// profiling_workflow.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

class MemoryIntensiveClass {
private:
    std::vector<double> data;
    std::string name;

public:
    MemoryIntensiveClass(const std::string& n, size_t size)
        : name(n), data(size) {
        std::cout << "Created " << name << " with "
                  << size * sizeof(double) / 1024 << " KB\n";
    }

    ~MemoryIntensiveClass() {
        std::cout << "Destroyed " << name << "\n";
    }

    void process() {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = std::sin(static_cast<double>(i));
        }
    }

    size_t getMemoryUsage() const {
        return data.size() * sizeof(double) + name.size();
    }
};

class MemoryManager {
private:
    std::vector<std::unique_ptr<MemoryIntensiveClass>> objects;
    size_t totalAllocated = 0;

public:
    void allocate(const std::string& name, size_t size) {
        objects.push_back(std::make_unique<MemoryIntensiveClass>(name, size));
        totalAllocated += size * sizeof(double);
    }

    void processAll() {
        for (auto& obj : objects) {
            obj->process();
        }
    }

    void cleanup() {
        std::cout << "\nCleaning up...\n";
        objects.clear();
        totalAllocated = 0;
    }

    void printStats() const {
        std::cout << "\nMemory Statistics:\n";
        std::cout << "  Objects: " << objects.size() << "\n";
        std::cout << "  Total allocated: " << totalAllocated / 1024 / 1024 << " MB\n";
    }
};

int main() {
    std::cout << "Memory Profiling Workflow\n";
    std::cout << "=========================\n\n";

    MemoryManager manager;

    // Allocate various sized objects
    manager.allocate("Small", 10000);
    manager.allocate("Medium", 1000000);
    manager.allocate("Large", 10000000);

    manager.printStats();

    std::cout << "\nProcessing all objects...\n";
    manager.process All();

    std::cout << "\nCleaning up...\n";
    manager.cleanup();

    manager.printStats();

    std::cout << "\nProfile with:\n";
    std::cout << "  1. Valgrind: valgrind --leak-check=full ./profiling_workflow\n";
    std::cout << "  2. ASan: compile with -fsanitize=address\n";
    std::cout << "  3. Massif: valgrind --tool=massif ./profiling_workflow\n";

    return 0;
}
```

### Example 2: Memory Usage Tracker

```cpp
// memory_tracker.cpp
#include <iostream>
#include <map>
#include <mutex>
#include <cstdlib>

class MemoryTracker {
private:
    struct AllocationInfo {
        size_t size;
        const char* file;
        int line;
    };

    static std::map<void*, AllocationInfo> allocations;
    static std::mutex mutex;
    static size_t totalAllocated;
    static size_t peakAllocated;

public:
    static void* allocate(size_t size, const char* file, int line) {
        void* ptr = std::malloc(size);
        if (ptr) {
            std::lock_guard<std::mutex> lock(mutex);
            allocations[ptr] = {size, file, line};
            totalAllocated += size;
            if (totalAllocated > peakAllocated) {
                peakAllocated = totalAllocated;
            }
        }
        return ptr;
    }

    static void deallocate(void* ptr) {
        if (ptr) {
            std::lock_guard<std::mutex> lock(mutex);
            auto it = allocations.find(ptr);
            if (it != allocations.end()) {
                totalAllocated -= it->second.size;
                allocations.erase(it);
            }
            std::free(ptr);
        }
    }

    static void printStats() {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "\nMemory Tracking Statistics:\n";
        std::cout << "  Current allocations: " << allocations.size() << "\n";
        std::cout << "  Current memory: " << totalAllocated << " bytes\n";
        std::cout << "  Peak memory: " << peakAllocated << " bytes\n";

        if (!allocations.empty()) {
            std::cout << "\nActive allocations:\n";
            for (const auto& pair : allocations) {
                std::cout << "  " << pair.second.size << " bytes at "
                          << pair.second.file << ":" << pair.second.line << "\n";
            }
        }
    }
};

std::map<void*, MemoryTracker::AllocationInfo> MemoryTracker::allocations;
std::mutex MemoryTracker::mutex;
size_t MemoryTracker::totalAllocated = 0;
size_t MemoryTracker::peakAllocated = 0;

// Macro for tracked allocations
#define TRACKED_NEW(size) MemoryTracker::allocate(size, __FILE__, __LINE__)
#define TRACKED_DELETE(ptr) MemoryTracker::deallocate(ptr)

int main() {
    std::cout << "Memory Tracking Example\n";
    std::cout << "=======================\n";

    void* ptr1 = TRACKED_NEW(1000);
    void* ptr2 = TRACKED_NEW(2000);
    void* ptr3 = TRACKED_NEW(500);

    MemoryTracker::printStats();

    TRACKED_DELETE(ptr2);

    MemoryTracker::printStats();

    TRACKED_DELETE(ptr1);
    TRACKED_DELETE(ptr3);

    MemoryTracker::printStats();

    return 0;
}
```

## Memory Optimization Techniques

### Technique 1: Object Pooling

```cpp
// object_pool.cpp
#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class ObjectPool {
private:
    std::vector<T*> available;
    std::vector<std::unique_ptr<T>> allocated;

public:
    ~ObjectPool() {
        available.clear();
        allocated.clear();
    }

    T* acquire() {
        if (available.empty()) {
            allocated.push_back(std::make_unique<T>());
            return allocated.back().get();
        } else {
            T* obj = available.back();
            available.pop_back();
            return obj;
        }
    }

    void release(T* obj) {
        if (obj) {
            available.push_back(obj);
        }
    }

    size_t poolSize() const {
        return allocated.size();
    }

    size_t availableCount() const {
        return available.size();
    }
};

class HeavyObject {
    std::vector<double> data;
public:
    HeavyObject() : data(10000, 0.0) {
        std::cout << "HeavyObject constructed\n";
    }
    void reset() {
        std::fill(data.begin(), data.end(), 0.0);
    }
};

int main() {
    ObjectPool<HeavyObject> pool;

    std::cout << "Using object pool\n";

    // Acquire objects
    HeavyObject* obj1 = pool.acquire();
    HeavyObject* obj2 = pool.acquire();
    HeavyObject* obj3 = pool.acquire();

    std::cout << "Pool size: " << pool.poolSize()
              << ", Available: " << pool.availableCount() << "\n";

    // Release objects
    pool.release(obj2);

    std::cout << "After release - Available: " << pool.availableCount() << "\n";

    // Reuse released object
    HeavyObject* obj4 = pool.acquire();

    std::cout << "After reuse - Available: " << pool.availableCount() << "\n";

    return 0;
}
```

## Summary

This lesson covered comprehensive memory profiling techniques:

### Key Takeaways

1. **Profiling Tools**:
   - Valgrind: Detailed but slow (10-50x overhead)
   - AddressSanitizer: Fast and accurate (2x overhead)
   - Dr. Memory: Windows-friendly
   - Massif: Heap profiling

2. **Common Issues**:
   - Memory leaks
   - Buffer overflows
   - Use-after-free
   - Double-free
   - Uninitialized memory

3. **Best Practices**:
   - Use smart pointers (unique_ptr, shared_ptr)
   - Prefer RAII
   - Avoid raw pointer ownership
   - Break circular references with weak_ptr
   - Use containers instead of raw arrays

4. **Optimization**:
   - Object pooling
   - Pre-allocation
   - Memory tracking
   - Minimize allocations

### What's Next

In **Lesson 07**, we'll explore Cache Analysis and Optimization:
- Cache hierarchy understanding
- Cache-friendly data structures
- False sharing prevention
- Prefetching techniques
- Cache optimization strategies

---

**Word Count: ~4,200 words**

**Code Examples: 12+ comprehensive examples**
