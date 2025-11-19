/*
 * Lesson 11: Operator Overloading - new and delete Operators
 *
 * This example demonstrates overloading new and delete operators
 * for custom memory management and tracking.
 *
 * Compilation: g++ -std=c++17 13_new_delete_operators.cpp -o new_delete
 * Execution: ./new_delete
 */

#include <iostream>
#include <cstdlib>
#include <new>

// Global memory tracking
size_t totalAllocated = 0;
size_t totalDeallocated = 0;
size_t allocationCount = 0;

// Example 1: Class with custom new/delete
class TrackedObject {
private:
    int id;
    static int nextId;

public:
    TrackedObject() : id(nextId++) {
        std::cout << "Constructor: TrackedObject #" << id << std::endl;
    }

    ~TrackedObject() {
        std::cout << "Destructor: TrackedObject #" << id << std::endl;
    }

    // Overload new operator
    void* operator new(size_t size) {
        std::cout << "Custom new: Allocating " << size << " bytes" << std::endl;
        void* ptr = std::malloc(size);
        if (!ptr) {
            throw std::bad_alloc();
        }
        totalAllocated += size;
        allocationCount++;
        return ptr;
    }

    // Overload delete operator
    void operator delete(void* ptr) noexcept {
        std::cout << "Custom delete: Freeing memory" << std::endl;
        totalDeallocated += sizeof(TrackedObject);
        std::free(ptr);
    }

    // Overload array new
    void* operator new[](size_t size) {
        std::cout << "Custom new[]: Allocating " << size << " bytes" << std::endl;
        void* ptr = std::malloc(size);
        if (!ptr) {
            throw std::bad_alloc();
        }
        totalAllocated += size;
        allocationCount++;
        return ptr;
    }

    // Overload array delete
    void operator delete[](void* ptr) noexcept {
        std::cout << "Custom delete[]: Freeing memory" << std::endl;
        std::free(ptr);
    }

    void display() const {
        std::cout << "TrackedObject #" << id;
    }
};

int TrackedObject::nextId = 1;

// Example 2: Placement new
class PlacementObject {
private:
    int value;

public:
    PlacementObject(int v = 0) : value(v) {
        std::cout << "PlacementObject(" << value << ") constructed" << std::endl;
    }

    ~PlacementObject() {
        std::cout << "PlacementObject(" << value << ") destroyed" << std::endl;
    }

    void display() const {
        std::cout << "Value: " << value;
    }
};

// Example 3: Memory pool allocator
class PoolAllocated {
private:
    int data;
    static const size_t POOL_SIZE = 10;
    static char memoryPool[POOL_SIZE * sizeof(PoolAllocated)];
    static bool used[POOL_SIZE];

public:
    PoolAllocated(int d = 0) : data(d) {
        std::cout << "PoolAllocated(" << data << ") constructed" << std::endl;
    }

    ~PoolAllocated() {
        std::cout << "PoolAllocated(" << data << ") destroyed" << std::endl;
    }

    void* operator new(size_t size) {
        std::cout << "Pool new: ";
        for (size_t i = 0; i < POOL_SIZE; ++i) {
            if (!used[i]) {
                used[i] = true;
                std::cout << "Allocated from pool slot " << i << std::endl;
                return &memoryPool[i * sizeof(PoolAllocated)];
            }
        }
        std::cout << "Pool exhausted, using standard allocation" << std::endl;
        return ::operator new(size);
    }

    void operator delete(void* ptr) noexcept {
        std::cout << "Pool delete: ";
        // Check if pointer is in pool
        if (ptr >= memoryPool && ptr < memoryPool + sizeof(memoryPool)) {
            size_t index = (static_cast<char*>(ptr) - memoryPool) / sizeof(PoolAllocated);
            used[index] = false;
            std::cout << "Freed pool slot " << index << std::endl;
        } else {
            std::cout << "Using standard deallocation" << std::endl;
            ::operator delete(ptr);
        }
    }

    void display() const {
        std::cout << "Data: " << data;
    }
};

char PoolAllocated::memoryPool[PoolAllocated::POOL_SIZE * sizeof(PoolAllocated)];
bool PoolAllocated::used[PoolAllocated::POOL_SIZE] = {false};

void showMemoryStats() {
    std::cout << "\n=== Memory Statistics ===" << std::endl;
    std::cout << "Total allocated: " << totalAllocated << " bytes" << std::endl;
    std::cout << "Total deallocated: " << totalDeallocated << " bytes" << std::endl;
    std::cout << "Allocation count: " << allocationCount << std::endl;
    std::cout << "Currently in use: " << (totalAllocated - totalDeallocated) << " bytes" << std::endl;
}

int main() {
    std::cout << "=== new/delete Operator Overloading Demo ===" << std::endl << std::endl;

    // Example 1: Custom new/delete
    std::cout << "1. Custom new/delete:" << std::endl;
    TrackedObject* obj1 = new TrackedObject();
    obj1->display();
    std::cout << std::endl;
    delete obj1;

    std::cout << "\n2. Array new/delete:" << std::endl;
    TrackedObject* arr = new TrackedObject[3];
    delete[] arr;

    showMemoryStats();

    // Example 2: Placement new
    std::cout << "\n3. Placement new:" << std::endl;
    char buffer[sizeof(PlacementObject)];

    // Construct object in pre-allocated buffer
    PlacementObject* obj2 = new (buffer) PlacementObject(42);
    obj2->display();
    std::cout << std::endl;

    // Must manually call destructor with placement new
    obj2->~PlacementObject();

    // Example 3: Memory pool
    std::cout << "\n4. Memory Pool Allocator:" << std::endl;
    PoolAllocated* p1 = new PoolAllocated(1);
    PoolAllocated* p2 = new PoolAllocated(2);
    PoolAllocated* p3 = new PoolAllocated(3);

    p1->display(); std::cout << std::endl;
    p2->display(); std::cout << std::endl;
    p3->display(); std::cout << std::endl;

    delete p2;  // Free slot 1

    PoolAllocated* p4 = new PoolAllocated(4);  // Reuse slot 1
    p4->display(); std::cout << std::endl;

    delete p1;
    delete p3;
    delete p4;

    // Example 4: nothrow new
    std::cout << "\n5. nothrow new:" << std::endl;
    TrackedObject* obj3 = new (std::nothrow) TrackedObject();
    if (obj3) {
        std::cout << "Allocation successful" << std::endl;
        obj3->display();
        std::cout << std::endl;
        delete obj3;
    } else {
        std::cout << "Allocation failed" << std::endl;
    }

    showMemoryStats();

    return 0;
}
