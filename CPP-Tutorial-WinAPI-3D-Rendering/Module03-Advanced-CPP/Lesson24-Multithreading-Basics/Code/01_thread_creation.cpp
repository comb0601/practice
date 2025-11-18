/*
 * Lesson 24: Multithreading Basics - thread_creation
 * Compile: g++ -std=c++17 -pthread 01_thread_creation.cpp -o 01_thread_creation
 */
#include <iostream>
#include <thread>
void threadFunc() {
    std::cout << "Hello from thread!\n";
}
int main() {
    std::thread t(threadFunc);
    t.join();
    std::cout << "Thread completed\n";
    return 0;
}
