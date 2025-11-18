/*
 * Lesson 24: Multithreading Basics - thread_id
 * Compile: g++ -std=c++17 -pthread 05_thread_id.cpp -o 05_thread_id
 */
#include <iostream>
#include <thread>
void printThreadId() {
    std::cout << "Thread ID: " << std::this_thread::get_id() << "\n";
}
int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n";
    std::thread t(printThreadId);
    t.join();
    return 0;
}
