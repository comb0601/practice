/* Lesson 71: Threading Basics (C++11)
 * Compile: cl /std:c++17 Lesson71_Threads.cpp */
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void printNumbers() {
    for (int i = 1; i <= 5; i++) {
        cout << "Thread: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void printMessage(string msg, int count) {
    for (int i = 0; i < count; i++) {
        cout << msg << " " << i << endl;
    }
}

int main() {
    cout << "=== Threading Demo ===" << endl;

    thread t1(printNumbers);
    thread t2(printMessage, "Hello", 3);

    t1.join();
    t2.join();

    cout << "All threads completed" << endl;
    return 0;
}
