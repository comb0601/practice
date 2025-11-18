# Lesson 25: Synchronization

## Introduction
When multiple threads access shared data, synchronization is essential to prevent data races and ensure correctness.

## std::mutex
Mutual exclusion lock for protecting shared data.

```cpp
#include <mutex>

std::mutex mtx;
int shared_data = 0;

void increment() {
    mtx.lock();
    ++shared_data;
    mtx.unlock();
}
```

## RAII Lock Guards
- **std::lock_guard**: Simple RAII mutex wrapper
- **std::unique_lock**: More flexible lock
- **std::scoped_lock**: Lock multiple mutexes (C++17)

```cpp
void safe_increment() {
    std::lock_guard<std::mutex> lock(mtx);
    ++shared_data;
}  // Automatically unlocked
```

## Condition Variables
For thread synchronization and waiting.

```cpp
std::condition_variable cv;
std::mutex mtx;
bool ready = false;

void wait_for_signal() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });
}

void send_signal() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
}
```

## std::atomic
Lock-free atomic operations for simple types.

```cpp
#include <atomic>

std::atomic<int> counter{0};
counter++;  // Atomic increment
```

## Deadlock Prevention
1. Lock ordering
2. std::scoped_lock for multiple mutexes
3. Timeout with try_lock
4. Avoid nested locks

See code examples for implementations.
