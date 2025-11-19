/* Lesson 73: Mutex Examples
 * Compile: cl /std:c++17 Lesson73_Mutex.cpp */
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex printMutex;

void safePrint(string msg, int id) {
    lock_guard<mutex> lock(printMutex);
    cout << "Thread " << id << ": " << msg << endl;
}

int main() {
    thread threads[5];

    for (int i = 0; i < 5; i++) {
        threads[i] = thread(safePrint, "Hello", i);
    }

    for (int i = 0; i < 5; i++) {
        threads[i].join();
    }

    return 0;
}
