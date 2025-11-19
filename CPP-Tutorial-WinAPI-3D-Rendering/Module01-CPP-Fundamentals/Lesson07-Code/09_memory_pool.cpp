/*
 * Program: Memory Pool
 * Description: Simple memory pool implementation for efficient allocation
 * Compilation: g++ -std=c++17 09_memory_pool.cpp -o 09_memory_pool
 * Execution: ./09_memory_pool
 */

#include <iostream>
using namespace std;

class MemoryPool {
private:
    struct Block {
        Block* next;
    };

    Block* freeList;
    char* pool;
    size_t blockSize;
    size_t poolSize;

public:
    MemoryPool(size_t blockSize, size_t numBlocks)
        : blockSize(blockSize), poolSize(blockSize * numBlocks) {

        pool = new char[poolSize];
        freeList = nullptr;

        // Initialize free list
        for (size_t i = 0; i < numBlocks; i++) {
            Block* block = (Block*)(pool + i * blockSize);
            block->next = freeList;
            freeList = block;
        }

        cout << "MemoryPool created: " << numBlocks << " blocks of "
             << blockSize << " bytes" << endl;
    }

    ~MemoryPool() {
        delete[] pool;
        cout << "MemoryPool destroyed" << endl;
    }

    void* allocate() {
        if (freeList == nullptr) {
            cout << "Pool exhausted!" << endl;
            return nullptr;
        }

        Block* block = freeList;
        freeList = freeList->next;
        return block;
    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) return;

        Block* block = (Block*)ptr;
        block->next = freeList;
        freeList = block;
    }
};

struct Object {
    int data[10];

    Object() {
        cout << "Object constructed" << endl;
    }

    ~Object() {
        cout << "Object destructed" << endl;
    }
};

int main() {
    cout << "=== Memory Pool ===" << endl << endl;

    // Create pool for 5 Objects
    MemoryPool pool(sizeof(Object), 5);
    cout << endl;

    // Allocate objects
    cout << "Allocating 3 objects:" << endl;
    void* obj1 = pool.allocate();
    void* obj2 = pool.allocate();
    void* obj3 = pool.allocate();
    cout << endl;

    // Deallocate
    cout << "Deallocating object 2:" << endl;
    pool.deallocate(obj2);
    cout << endl;

    // Reallocate
    cout << "Reallocating:" << endl;
    void* obj4 = pool.allocate();  // Reuses obj2's memory
    cout << endl;

    // Cleanup
    pool.deallocate(obj1);
    pool.deallocate(obj3);
    pool.deallocate(obj4);

    return 0;
}
