/*
 * Lesson 25: Synchronization - unique_lock
 * Compile: g++ -std=c++17 -pthread 03_unique_lock.cpp -o 03_unique_lock
 */
#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
void task() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Working...\n";
    lock.unlock();  // Manual unlock
    // Do work without lock
    lock.lock();  // Re-lock
    std::cout << "Done\n";
}
int main() {
    std::thread t1(task);
    std::thread t2(task);
    t1.join(); t2.join();
    return 0;
}
