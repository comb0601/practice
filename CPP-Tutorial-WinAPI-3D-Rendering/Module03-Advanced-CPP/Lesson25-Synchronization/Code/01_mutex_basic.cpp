/*
 * Lesson 25: Synchronization - mutex_basic
 * Compile: g++ -std=c++17 -pthread 01_mutex_basic.cpp -o 01_mutex_basic
 */
#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
int counter = 0;
void increment() {
    for (int i = 0; i < 1000; ++i) {
        mtx.lock();
        ++counter;
        mtx.unlock();
    }
}
int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << "\n";
    return 0;
}
