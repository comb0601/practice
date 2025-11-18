/*
 * Lesson 16: Custom Memory Allocators
 * Demonstrates high-performance allocation strategies
 */

#include "allocators.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <random>

class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
};

void print_header(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

// Test structure
struct TestObject {
    int id;
    double value;
    char data[48];  // Padding to make it 64 bytes total

    TestObject(int i = 0) : id(i), value(i * 1.5) {
        for (int j = 0; j < 48; ++j) data[j] = static_cast<char>(j);
    }
};

void demo_stack_allocator() {
    print_header("Stack Allocator Demo");

    StackAllocator<1024 * 1024> allocator;  // 1 MB stack

    std::cout << "Initial state:\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n\n";

    // Allocate some objects
    int* numbers = static_cast<int*>(allocator.allocate(100 * sizeof(int)));
    for (int i = 0; i < 100; ++i) {
        numbers[i] = i;
    }

    std::cout << "After allocating 100 ints:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n\n";

    // Allocate more
    double* values = static_cast<double*>(allocator.allocate(50 * sizeof(double)));
    for (int i = 0; i < 50; ++i) {
        values[i] = i * 1.5;
    }

    std::cout << "After allocating 50 doubles:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n\n";

    // Reset and reuse
    allocator.reset();

    std::cout << "After reset:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n";
}

void demo_pool_allocator() {
    print_header("Pool Allocator Demo");

    PoolAllocator<TestObject> pool;

    std::cout << "Allocating and deallocating TestObjects...\n\n";

    std::vector<TestObject*> objects;

    // Allocate 1000 objects
    for (int i = 0; i < 1000; ++i) {
        TestObject* obj = pool.allocate();
        new (obj) TestObject(i);  // Placement new
        objects.push_back(obj);
    }

    std::cout << "After 1000 allocations:\n";
    std::cout << "  Total allocations: " << pool.allocation_count() << "\n";
    std::cout << "  Total deallocations: " << pool.deallocation_count() << "\n\n";

    // Deallocate half
    for (size_t i = 0; i < objects.size() / 2; ++i) {
        objects[i]->~TestObject();  // Call destructor
        pool.deallocate(objects[i]);
    }

    std::cout << "After 500 deallocations:\n";
    std::cout << "  Total allocations: " << pool.allocation_count() << "\n";
    std::cout << "  Total deallocations: " << pool.deallocation_count() << "\n\n";

    // Deallocate rest
    for (size_t i = objects.size() / 2; i < objects.size(); ++i) {
        objects[i]->~TestObject();
        pool.deallocate(objects[i]);
    }

    std::cout << "After all deallocations:\n";
    std::cout << "  Total allocations: " << pool.allocation_count() << "\n";
    std::cout << "  Total deallocations: " << pool.deallocation_count() << "\n";
}

void benchmark_allocators() {
    print_header("Allocator Performance Benchmark");

    const int iterations = 100000;

    std::cout << "Allocating " << iterations << " TestObjects...\n\n";

    // Standard allocator (new/delete)
    {
        Timer t;
        std::vector<TestObject*> objects;

        for (int i = 0; i < iterations; ++i) {
            objects.push_back(new TestObject(i));
        }

        for (auto* obj : objects) {
            delete obj;
        }

        std::cout << "Standard new/delete: " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }

    // Pool allocator
    {
        PoolAllocator<TestObject> pool;
        Timer t;
        std::vector<TestObject*> objects;

        for (int i = 0; i < iterations; ++i) {
            TestObject* obj = pool.allocate();
            new (obj) TestObject(i);
            objects.push_back(obj);
        }

        for (auto* obj : objects) {
            obj->~TestObject();
            pool.deallocate(obj);
        }

        std::cout << "Pool allocator:      " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }

    // Stack allocator (no individual deallocation)
    {
        StackAllocator<64 * 1024 * 1024> stack;  // 64 MB
        Timer t;

        for (int i = 0; i < iterations; ++i) {
            TestObject* obj = static_cast<TestObject*>(
                stack.allocate(sizeof(TestObject), alignof(TestObject))
            );
            new (obj) TestObject(i);
        }

        // Reset all at once
        stack.reset();

        std::cout << "Stack allocator:     " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }
}

void demo_monotonic_allocator() {
    print_header("Monotonic Allocator Demo");

    MonotonicAllocator allocator(1024 * 1024);  // 1 MB

    std::cout << "Allocating various sizes with alignment...\n\n";

    // Allocate with different alignments
    void* p1 = allocator.allocate(100, 16);
    std::cout << "Allocated 100 bytes (16-byte aligned)\n";
    std::cout << "  Address: " << p1 << "\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n\n";

    void* p2 = allocator.allocate(256, 32);
    std::cout << "Allocated 256 bytes (32-byte aligned)\n";
    std::cout << "  Address: " << p2 << "\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n\n";

    void* p3 = allocator.allocate(1024, 64);
    std::cout << "Allocated 1024 bytes (64-byte aligned)\n";
    std::cout << "  Address: " << p3 << "\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n\n";

    std::cout << "Total used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "Available: " << allocator.bytes_available() << " bytes\n";
}

void demo_freelist_allocator() {
    print_header("Free List Allocator Demo");

    FreeListAllocator allocator(1024 * 1024);  // 1 MB

    std::cout << "Allocating and deallocating with free list...\n\n";

    // Allocate some blocks
    void* p1 = allocator.allocate(1000);
    void* p2 = allocator.allocate(2000);
    void* p3 = allocator.allocate(500);

    std::cout << "After 3 allocations:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n\n";

    // Deallocate middle block
    allocator.deallocate(p2);

    std::cout << "After deallocating middle block:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n\n";

    // Allocate again (should reuse freed block if possible)
    void* p4 = allocator.allocate(1500);

    std::cout << "After reallocating:\n";
    std::cout << "  Used: " << allocator.bytes_used() << " bytes\n";
    std::cout << "  Available: " << allocator.bytes_available() << " bytes\n";

    // Clean up
    allocator.deallocate(p1);
    allocator.deallocate(p3);
    allocator.deallocate(p4);
}

int main() {
    std::cout << "Custom Memory Allocators\n";
    std::cout << "========================\n";
    std::cout << "\nDemonstrating various allocation strategies:\n";
    std::cout << "1. Stack (Linear/Arena) Allocator\n";
    std::cout << "2. Pool (Fixed-Size) Allocator\n";
    std::cout << "3. Monotonic (Bump) Allocator\n";
    std::cout << "4. Free List Allocator\n";

    demo_stack_allocator();
    demo_pool_allocator();
    demo_monotonic_allocator();
    demo_freelist_allocator();
    benchmark_allocators();

    print_header("Summary");
    std::cout << "Allocator Trade-offs:\n\n";
    std::cout << "Stack Allocator:\n";
    std::cout << "  + Extremely fast allocation (just pointer bump)\n";
    std::cout << "  + Perfect for temporary allocations\n";
    std::cout << "  - No individual deallocation\n";
    std::cout << "  - Fixed size buffer\n\n";

    std::cout << "Pool Allocator:\n";
    std::cout << "  + Fast allocation/deallocation\n";
    std::cout << "  + No fragmentation\n";
    std::cout << "  + Cache-friendly\n";
    std::cout << "  - Fixed object size\n";
    std::cout << "  - Memory overhead for large objects\n\n";

    std::cout << "Monotonic Allocator:\n";
    std::cout << "  + Very fast allocation\n";
    std::cout << "  + Supports any size/alignment\n";
    std::cout << "  - No individual deallocation\n";
    std::cout << "  - Must reset entire allocator\n\n";

    std::cout << "Free List Allocator:\n";
    std::cout << "  + Supports any size\n";
    std::cout << "  + Individual deallocation\n";
    std::cout << "  + Memory reuse\n";
    std::cout << "  - Can fragment\n";
    std::cout << "  - Slower than specialized allocators\n";

    std::cout << std::string(60, '=') << "\n";

    return 0;
}
