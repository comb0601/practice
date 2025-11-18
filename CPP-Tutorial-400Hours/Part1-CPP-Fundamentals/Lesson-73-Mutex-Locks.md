# Lesson 73: Mutex and Locks

**Duration**: 6 hours
**Difficulty**: Advanced

Mutexes (mutual exclusion) protect shared resources from concurrent access.

## Basic Mutex:

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int counter = 0;

void increment() {
    for (int i = 0; i < 100000; i++) {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << counter << "\n";

    return 0;
}
```

## lock_guard (RAII):

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int counter = 0;

void increment() {
    for (int i = 0; i < 100000; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
    }  // Automatically unlocked
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << counter << "\n";

    return 0;
}
```

## unique_lock:

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void task() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Task running\n";

    lock.unlock();  // Manual unlock
    // Do non-critical work

    lock.lock();    // Re-lock
    std::cout << "Critical section\n";
}

int main()
{
    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    return 0;
}
```

## scoped_lock (C++17):

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;

void transfer() {
    std::scoped_lock lock(mtx1, mtx2);  // Locks both, deadlock-free
    // Critical section
}

int main()
{
    std::thread t1(transfer);
    std::thread t2(transfer);

    t1.join();
    t2.join();

    return 0;
}
```

## Deadlock Example and Prevention:

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;

void deadlockExample() {
    // Thread 1
    std::thread t1([]() {
        std::lock_guard<std::mutex> lock1(mtx1);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock2(mtx2);  // Deadlock!
    });

    // Thread 2
    std::thread t2([]() {
        std::lock_guard<std::mutex> lock2(mtx2);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock1(mtx1);  // Deadlock!
    });

    t1.join();
    t2.join();
}

void deadlockPrevention() {
    // Use std::lock or scoped_lock
    std::thread t1([]() {
        std::scoped_lock lock(mtx1, mtx2);  // Locks both atomically
    });

    std::thread t2([]() {
        std::scoped_lock lock(mtx1, mtx2);  // Same order
    });

    t1.join();
    t2.join();
}

int main()
{
    deadlockPrevention();  // Safe
    return 0;
}
```

**Key Takeaways:**
- std::mutex for mutual exclusion
- lock_guard for RAII-style locking
- unique_lock for flexible locking
- scoped_lock for multiple mutexes (C++17)
- Always unlock in reverse order
- Avoid deadlocks with consistent lock ordering
