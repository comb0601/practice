# Lesson 74: Condition Variables

**Duration**: 5 hours
**Difficulty**: Advanced

Condition variables enable threads to wait for specific conditions.

## Basic Condition Variable:

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // Wait until ready
    std::cout << "Worker proceeding\n";
}

void setter() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // Wake up one waiting thread
}

int main()
{
    std::thread t1(worker);
    std::thread t2(setter);

    t1.join();
    t2.join();

    return 0;
}
```

## Producer-Consumer Pattern:

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> buffer;
const int maxSize = 10;

void producer() {
    for (int i = 0; i < 20; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return buffer.size() < maxSize; });

        buffer.push(i);
        std::cout << "Produced: " << i << "\n";

        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer() {
    for (int i = 0; i < 20; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !buffer.empty(); });

        int value = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << value << "\n";

        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main()
{
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}
```

**Key Takeaways:**
- Condition variables for thread coordination
- wait() releases mutex while waiting
- notify_one() wakes one thread
- notify_all() wakes all threads
- Always use predicate with wait()
- Producer-consumer is common pattern
