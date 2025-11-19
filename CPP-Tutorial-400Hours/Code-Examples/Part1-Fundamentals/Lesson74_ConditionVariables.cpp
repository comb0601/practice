/* Lesson 74: Condition Variables
 * Compile: cl /std:c++17 Lesson74_ConditionVariables.cpp */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void worker() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });
    cout << "Worker thread processing" << endl;
}

int main() {
    thread t(worker);

    this_thread::sleep_for(chrono::seconds(1));

    {
        lock_guard<mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();

    t.join();
    return 0;
}
