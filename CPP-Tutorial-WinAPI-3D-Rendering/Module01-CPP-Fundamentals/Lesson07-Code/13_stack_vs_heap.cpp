/*
 * Program: Stack vs Heap Comparison
 * Description: Detailed comparison of stack and heap memory
 * Compilation: g++ -std=c++17 13_stack_vs_heap.cpp -o 13_stack_vs_heap
 * Execution: ./13_stack_vs_heap
 */

#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

void stackAllocation() {
    int arr[10000];  // Stack allocation
    for (int i = 0; i < 10000; i++) {
        arr[i] = i;
    }
}

void heapAllocation() {
    int* arr = new int[10000];  // Heap allocation
    for (int i = 0; i < 10000; i++) {
        arr[i] = i;
    }
    delete[] arr;
}

int main() {
    cout << "=== Stack vs Heap ===" << endl << endl;

    cout << "STACK MEMORY:" << endl;
    cout << "- Fast allocation (just move stack pointer)" << endl;
    cout << "- Limited size (typically 1-8 MB)" << endl;
    cout << "- Automatic lifetime (scope-based)" << endl;
    cout << "- LIFO (Last In, First Out)" << endl;
    cout << "- No fragmentation" << endl;
    cout << "- Local variables, function parameters" << endl << endl;

    cout << "HEAP MEMORY:" << endl;
    cout << "- Slower allocation (search for free block)" << endl;
    cout << "- Large size (limited by system RAM)" << endl;
    cout << "- Manual lifetime (new/delete)" << endl;
    cout << "- Can persist beyond function scope" << endl;
    cout << "- Can fragment over time" << endl;
    cout << "- Dynamic allocations (new/malloc)" << endl << endl;

    // Speed test
    cout << "SPEED TEST (1000 iterations):" << endl;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        stackAllocation();
    }
    auto end = high_resolution_clock::now();
    auto stackTime = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        heapAllocation();
    }
    end = high_resolution_clock::now();
    auto heapTime = duration_cast<microseconds>(end - start).count();

    cout << "Stack: " << stackTime << " microseconds" << endl;
    cout << "Heap:  " << heapTime << " microseconds" << endl;
    cout << "Heap is ~" << (heapTime / stackTime) << "x slower" << endl << endl;

    cout << "WHEN TO USE:" << endl;
    cout << "\nStack:" << endl;
    cout << "- Small, fixed-size objects" << endl;
    cout << "- Short lifetime (function scope)" << endl;
    cout << "- Performance critical code" << endl;
    cout << "- Known size at compile time" << endl;

    cout << "\nHeap:" << endl;
    cout << "- Large objects" << endl;
    cout << "- Variable/unknown size" << endl;
    cout << "- Long lifetime (beyond function)" << endl;
    cout << "- Shared between functions" << endl;
    cout << "- Size determined at runtime" << endl;

    return 0;
}
