/*
 * Program: Custom Allocator
 * Description: Simple custom memory allocator
 * Compilation: g++ -std=c++17 12_custom_allocator.cpp -o 12_custom_allocator
 * Execution: ./12_custom_allocator
 */

#include <iostream>
using namespace std;

class Allocator {
private:
    static size_t totalAllocated;
    static size_t allocationCount;

public:
    static void* allocate(size_t size) {
        void* ptr = ::operator new(size);
        totalAllocated += size;
        allocationCount++;
        cout << "Allocated " << size << " bytes at " << ptr << endl;
        return ptr;
    }

    static void deallocate(void* ptr, size_t size) {
        ::operator delete(ptr);
        totalAllocated -= size;
        allocationCount--;
        cout << "Deallocated " << size << " bytes at " << ptr << endl;
    }

    static void printStats() {
        cout << "\nMemory Statistics:" << endl;
        cout << "  Total allocated: " << totalAllocated << " bytes" << endl;
        cout << "  Active allocations: " << allocationCount << endl;
    }
};

size_t Allocator::totalAllocated = 0;
size_t Allocator::allocationCount = 0;

int main() {
    cout << "=== Custom Allocator ===" << endl << endl;

    int* arr = (int*)Allocator::allocate(10 * sizeof(int));

    for (int i = 0; i < 10; i++) {
        arr[i] = i * 10;
    }

    Allocator::printStats();

    Allocator::deallocate(arr, 10 * sizeof(int));

    Allocator::printStats();

    return 0;
}
