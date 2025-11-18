/*
 * Lesson 92 - Example 01: Array vs Linked List Cache Performance
 *
 * Demonstrates the massive performance difference between cache-friendly
 * arrays and cache-hostile linked lists for sequential access.
 */

#include <iostream>
#include <vector>
#include <list>
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
public:
    void reset() { start = std::chrono::high_resolution_clock::now(); }
    double elapsedMs() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    }
};

int main() {
    std::cout << "=== Array vs Linked List Cache Performance ===\n\n";

    const int SIZE = 1000000;
    Timer timer;

    // Test 1: std::vector (contiguous array)
    std::vector<int> vec(SIZE);
    for (int i = 0; i < SIZE; ++i) vec[i] = i;

    timer.reset();
    long long sum1 = 0;
    for (int val : vec) sum1 += val;
    double timeVector = timer.elapsedMs();

    // Test 2: std::list (linked list)
    std::list<int> lst;
    for (int i = 0; i < SIZE; ++i) lst.push_back(i);

    timer.reset();
    long long sum2 = 0;
    for (int val : lst) sum2 += val;
    double timeList = timer.elapsedMs();

    std::cout << "Vector (array):   " << timeVector << " ms\n";
    std::cout << "List (linked):    " << timeList << " ms\n";
    std::cout << "Speedup:          " << (timeList / timeVector) << "x\n\n";

    std::cout << "Why?\n";
    std::cout << "- Vector: Contiguous memory, sequential cache access\n";
    std::cout << "- List:   Scattered memory, every node is a cache miss\n";
    std::cout << "- Result: Vector is 10-50x faster!\n";

    return 0;
}
