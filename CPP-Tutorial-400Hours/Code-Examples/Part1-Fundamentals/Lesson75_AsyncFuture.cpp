/* Lesson 75: async and future
 * Compile: cl /std:c++17 Lesson75_AsyncFuture.cpp */
#include <iostream>
#include <future>
#include <chrono>
using namespace std;

int calculate(int x) {
    this_thread::sleep_for(chrono::seconds(1));
    return x * x;
}

int main() {
    cout << "Starting async task..." << endl;

    future<int> result = async(launch::async, calculate, 10);

    cout << "Doing other work..." << endl;

    int value = result.get();
    cout << "Result: " << value << endl;

    return 0;
}
