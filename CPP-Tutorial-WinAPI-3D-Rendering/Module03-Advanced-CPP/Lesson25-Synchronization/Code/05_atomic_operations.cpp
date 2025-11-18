/*
 * Lesson 25: Synchronization - atomic_operations
 * Compile: g++ -std=c++17 -pthread 05_atomic_operations.cpp -o 05_atomic_operations
 */
#include <iostream>
#include <thread>
#include <atomic>
std::atomic<int> counter{0};
void increment() {
    for (int i = 0; i < 1000; ++i) {
        counter++;  // Atomic
    }
}
int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << "\n";
    return 0;
}
