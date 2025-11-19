/*
 * Program: Resource Manager
 * Description: Complete resource management example with tracking
 * Compilation: g++ -std=c++17 14_resource_manager.cpp -o 14_resource_manager
 * Execution: ./14_resource_manager
 */

#include <iostream>
#include <vector>
using namespace std;

class ResourceManager {
private:
    struct Allocation {
        void* ptr;
        size_t size;
        const char* file;
        int line;
    };

    vector<Allocation> allocations;

public:
    void* allocate(size_t size, const char* file, int line) {
        void* ptr = ::operator new(size);

        Allocation alloc;
        alloc.ptr = ptr;
        alloc.size = size;
        alloc.file = file;
        alloc.line = line;

        allocations.push_back(alloc);

        cout << "Allocated " << size << " bytes at " << file << ":" << line << endl;
        return ptr;
    }

    void deallocate(void* ptr) {
        for (auto it = allocations.begin(); it != allocations.end(); ++it) {
            if (it->ptr == ptr) {
                cout << "Deallocated " << it->size << " bytes from "
                     << it->file << ":" << it->line << endl;
                ::operator delete(ptr);
                allocations.erase(it);
                return;
            }
        }
        cout << "WARNING: Deallocating untracked pointer!" << endl;
    }

    void reportLeaks() {
        if (allocations.empty()) {
            cout << "\nNo memory leaks detected!" << endl;
        } else {
            cout << "\nMEMORY LEAKS DETECTED:" << endl;
            for (const auto& alloc : allocations) {
                cout << "  " << alloc.size << " bytes allocated at "
                     << alloc.file << ":" << alloc.line << " never freed" << endl;
            }
        }
    }

    ~ResourceManager() {
        reportLeaks();

        // Clean up leaks
        for (const auto& alloc : allocations) {
            ::operator delete(alloc.ptr);
        }
        allocations.clear();
    }
};

#define ALLOC(rm, size) rm.allocate(size, __FILE__, __LINE__)
#define FREE(rm, ptr) rm.deallocate(ptr)

int main() {
    cout << "=== Resource Manager ===" << endl << endl;

    ResourceManager rm;

    // Allocate some memory
    int* arr1 = (int*)ALLOC(rm, 10 * sizeof(int));
    int* arr2 = (int*)ALLOC(rm, 20 * sizeof(int));
    int* arr3 = (int*)ALLOC(rm, 5 * sizeof(int));

    // Use memory
    for (int i = 0; i < 10; i++) {
        arr1[i] = i;
    }

    // Free some memory
    FREE(rm, arr1);
    FREE(rm, arr3);

    // arr2 is leaked (not freed)

    cout << "\nProgram ending..." << endl;

    // ResourceManager destructor will report leaks

    return 0;
}
