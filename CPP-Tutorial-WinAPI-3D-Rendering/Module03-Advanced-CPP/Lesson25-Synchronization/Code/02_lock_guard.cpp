/*
 * Lesson 25: Synchronization - lock_guard
 * Compile: g++ -std=c++17 -pthread 02_lock_guard.cpp -o 02_lock_guard
 */
#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
void print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << msg << "\n";
}
int main() {
    std::thread t1(print, "Thread 1");
    std::thread t2(print, "Thread 2");
    t1.join(); t2.join();
    return 0;
}
