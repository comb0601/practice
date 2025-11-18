/*
 * Lesson 25: Synchronization - condition_variable
 * Compile: g++ -std=c++17 -pthread 04_condition_variable.cpp -o 04_condition_variable
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{return ready;});
    std::cout << "Worker proceeding\n";
}
int main() {
    std::thread t(worker);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    t.join();
    return 0;
}
