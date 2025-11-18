# Lesson 58: Multithreading Basics with std::thread

**Duration:** 2.5 hours

## Learning Objectives
- Understand thread creation and management
- Learn thread synchronization with mutexes
- Master std::lock_guard and std::unique_lock
- Prevent data races and deadlocks
- Use condition variables for thread communication

## Introduction

Modern C++ provides robust multithreading support through the standard library. Understanding how to safely create and manage concurrent operations is essential for high-performance applications, including graphics and game engines.

## Creating Threads

### Basic Thread Creation

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void printMessage(const std::string& msg, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << msg << " " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Create thread with function
    std::thread t1(printMessage, "Thread 1", 5);

    // Create thread with lambda
    std::thread t2([]() {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 2 " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Wait for threads to complete
    t1.join();
    t2.join();

    std::cout << "All threads completed" << std::endl;

    return 0;
}
```

## Thread Synchronization with Mutex

### Protecting Shared Data

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int sharedCounter = 0;

void incrementCounter(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++sharedCounter;
    }
}

int main() {
    const int numThreads = 4;
    const int iterations = 10000;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(incrementCounter, iterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter value: " << sharedCounter << std::endl;
    std::cout << "Expected: " << (numThreads * iterations) << std::endl;

    return 0;
}
```

## Practical Example: Thread Pool

```cpp
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        for (auto& worker : workers) {
            worker.join();
        }
    }

    template <typename F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }
};

int main() {
    ThreadPool pool(4);

    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " executing on thread "
                      << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
```

## Exercises

### Exercise 1: Parallel Sum

**Solution:**
```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>

void partialSum(const std::vector<int>& data,
                size_t start, size_t end, long long& result) {
    result = std::accumulate(data.begin() + start,
                            data.begin() + end, 0LL);
}

long long parallelSum(const std::vector<int>& data, int numThreads) {
    std::vector<std::thread> threads;
    std::vector<long long> partialResults(numThreads);

    size_t chunkSize = data.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? data.size() : start + chunkSize;

        threads.emplace_back(partialSum, std::cref(data),
                           start, end, std::ref(partialResults[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    return std::accumulate(partialResults.begin(), partialResults.end(), 0LL);
}

int main() {
    std::vector<int> data(10000000);
    std::iota(data.begin(), data.end(), 1);

    long long sum = parallelSum(data, 4);
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
```

## Summary

- std::thread for creating threads
- Mutex for protecting shared data
- lock_guard for RAII-style locking
- condition_variable for thread communication
- Thread pools for efficient task management

## Checklist

- [ ] Can create and join threads
- [ ] Understand mutex protection
- [ ] Know lock_guard vs unique_lock
- [ ] Can use condition variables
- [ ] Completed exercises

## Next Lesson

In Lesson 59, we'll explore **Async and Futures** for easier concurrent programming.
