/*
 * Lesson 24: Multithreading Basics - thread_with_args
 * Compile: g++ -std=c++17 -pthread 02_thread_with_args.cpp -o 02_thread_with_args
 */
#include <iostream>
#include <thread>
void printSum(int a, int b) {
    std::cout << a << " + " << b << " = " << (a+b) << "\n";
}
int main() {
    std::thread t(printSum, 5, 3);
    t.join();
    return 0;
}
