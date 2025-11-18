# Lesson 16: Custom Memory Allocators

## Introduction
Custom allocators can provide:
- Faster allocation/deallocation
- Better cache locality
- Reduced fragmentation
- Specialized memory strategies

## Types of Custom Allocators

### 1. Stack Allocator (Fast, Fixed Size)
```cpp
class StackAllocator {
    char* buffer;
    size_t size;
    size_t offset;
    
public:
    StackAllocator(size_t sz) : size(sz), offset(0) {
        buffer = new char[size];
    }
    
    ~StackAllocator() {
        delete[] buffer;
    }
    
    void* allocate(size_t bytes) {
        if (offset + bytes > size) return nullptr;
        void* ptr = buffer + offset;
        offset += bytes;
        return ptr;
    }
    
    void reset() {
        offset = 0;  // Instant "deallocation"
    }
};

// Usage: Perfect for frame-based allocations
StackAllocator frameAllocator(1024 * 1024);  // 1 MB

void processFrame() {
    int* temp = (int*)frameAllocator.allocate(1000 * sizeof(int));
    // Use temp...
    frameAllocator.reset();  // Free everything
}
```

### 2. Pool Allocator
```cpp
template<typename T, size_t BlockSize = 4096>
class PoolAllocator {
    union Node {
        T element;
        Node* next;
    };
    
    Node* freeList;
    std::vector<char*> blocks;
    
public:
    PoolAllocator() : freeList(nullptr) {
        allocateBlock();
    }
    
    ~PoolAllocator() {
        for (auto block : blocks) {
            delete[] block;
        }
    }
    
    T* allocate() {
        if (!freeList) {
            allocateBlock();
        }
        
        Node* node = freeList;
        freeList = node->next;
        return reinterpret_cast<T*>(node);
    }
    
    void deallocate(T* ptr) {
        Node* node = reinterpret_cast<Node*>(ptr);
        node->next = freeList;
        freeList = node;
    }
    
private:
    void allocateBlock() {
        char* block = new char[BlockSize];
        blocks.push_back(block);
        
        size_t count = BlockSize / sizeof(Node);
        for (size_t i = 0; i < count; ++i) {
            Node* node = reinterpret_cast<Node*>(block + i * sizeof(Node));
            node->next = freeList;
            freeList = node;
        }
    }
};
```

### 3. STL-Compatible Allocator
```cpp
template<typename T>
class CustomAllocator {
public:
    using value_type = T;
    
    CustomAllocator() = default;
    
    template<typename U>
    CustomAllocator(const CustomAllocator<U>&) {}
    
    T* allocate(size_t n) {
        // Custom allocation logic
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    
    void deallocate(T* p, size_t n) {
        ::operator delete(p);
    }
};

// Usage with STL
std::vector<int, CustomAllocator<int>> vec;
```

## Performance Comparison
```cpp
#include <iostream>
#include <chrono>

int main() {
    const int ITERATIONS = 1000000;
    
    // Standard allocator
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        int* ptr = new int;
        delete ptr;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    
    // Pool allocator
    PoolAllocator<int> pool;
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        int* ptr = pool.allocate();
        pool.deallocate(ptr);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    
    auto t1 = std::chrono::duration<double, std::milli>(end1 - start1);
    auto t2 = std::chrono::duration<double, std::milli>(end2 - start2);
    
    std::cout << "Standard: " << t1.count() << " ms\n";
    std::cout << "Pool:     " << t2.count() << " ms\n";
    std::cout << "Speedup:  " << (t1.count() / t2.count()) << "x\n";
    
    return 0;
}
```
**Typical result:** Pool allocator is 10-100x faster

## Summary
- Stack allocators: Fastest, linear allocation
- Pool allocators: Fast fixed-size allocations
- Custom allocators can be 10-100x faster than malloc/new

**Next:** Object Pool Pattern
