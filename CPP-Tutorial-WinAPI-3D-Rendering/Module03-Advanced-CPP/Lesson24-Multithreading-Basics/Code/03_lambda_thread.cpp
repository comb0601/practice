/*
 * Lesson 24: Multithreading Basics - lambda_thread
 * Compile: g++ -std=c++17 -pthread 03_lambda_thread.cpp -o 03_lambda_thread
 */
#include <iostream>
#include <thread>
int main() {
    int x = 10;
    std::thread t([x]() {
        std::cout << "Lambda thread, x = " << x << "\n";
    });
    t.join();
    return 0;
}
