/*
 * Lesson 24: Multithreading Basics - multiple_threads
 * Compile: g++ -std=c++17 -pthread 04_multiple_threads.cpp -o 04_multiple_threads
 */
#include <iostream>
#include <thread>
#include <vector>
void worker(int id) {
    std::cout << "Thread " << id << " working\n";
}
int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
