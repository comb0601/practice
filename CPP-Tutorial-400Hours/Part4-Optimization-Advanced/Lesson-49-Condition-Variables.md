# Lesson 49: Condition Variables

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Condition Variables
2. std::condition_variable Basics
3. wait, wait_for, wait_until
4. notify_one vs notify_all
5. Spurious Wakeups
6. Predicate-Based Waiting
7. Lost Wakeup Problem
8. Producer-Consumer with CV
9. Thread Pool with CV
10. Best Practices
11. Common Pitfalls
12. Exercises

---

## 1. Introduction to Condition Variables

### What is a Condition Variable?

A **condition variable** allows threads to wait for a specific condition to become true. It's used for thread synchronization and communication.

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
    std::cout << "Worker waiting...\n";
    cv.wait(lock, []{ return ready; });  // Wait for condition
    std::cout << "Worker proceeding!\n";
}

void signal() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    std::cout << "Signaling...\n";
    cv.notify_one();
}

void basic_cv_example() {
    std::cout << "=== Basic Condition Variable ===\n\n";

    std::thread t1(worker);
    std::thread t2(signal);

    t1.join();
    t2.join();
    std::cout << "\n";
}

int main() {
    basic_cv_example();
    return 0;
}
```

**Key Components**:
1. **Mutex**: Protects shared data
2. **Condition Variable**: Waits for notification
3. **Predicate**: Condition to check

---

## 2. std::condition_variable Basics

### Basic Operations

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;
bool finished = false;

void producer() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(i);
            std::cout << "Produced: " << i << "\n";
        }

        cv.notify_one();  // Wake up one waiting thread
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();  // Wake up all waiting threads
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, []{ return !data_queue.empty() || finished; });

        if (data_queue.empty() && finished) {
            break;
        }

        if (!data_queue.empty()) {
            int value = data_queue.front();
            data_queue.pop();
            lock.unlock();

            std::cout << "Consumer " << id << " consumed: " << value << "\n";
        }
    }
}

void producer_consumer_basic() {
    std::cout << "=== Producer-Consumer with CV ===\n\n";

    std::thread prod(producer);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);

    prod.join();
    cons1.join();
    cons2.join();
    std::cout << "\n";
}

int main() {
    producer_consumer_basic();
    return 0;
}
```

---

## 3. wait, wait_for, wait_until

### Different Wait Variants

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
bool condition_met = false;

void wait_example() {
    std::cout << "=== wait() ===\n";
    std::unique_lock<std::mutex> lock(mtx);

    // Blocks indefinitely until notified AND predicate is true
    cv.wait(lock, []{ return condition_met; });

    std::cout << "Condition met!\n\n";
}

void wait_for_example() {
    std::cout << "=== wait_for() ===\n";
    std::unique_lock<std::mutex> lock(mtx);

    // Wait for maximum of 2 seconds
    if (cv.wait_for(lock, std::chrono::seconds(2), []{ return condition_met; })) {
        std::cout << "Condition met within timeout!\n";
    } else {
        std::cout << "Timeout occurred!\n";
    }
    std::cout << "\n";
}

void wait_until_example() {
    std::cout << "=== wait_until() ===\n";
    std::unique_lock<std::mutex> lock(mtx);

    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(2);

    // Wait until specific time point
    if (cv.wait_until(lock, deadline, []{ return condition_met; })) {
        std::cout << "Condition met before deadline!\n";
    } else {
        std::cout << "Deadline reached!\n";
    }
    std::cout << "\n";
}

void demonstrate_wait_variants() {
    std::thread t1([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        wait_for_example();
    });

    std::thread t2([]() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        wait_for_example();
    });

    t1.join();
    t2.join();
}

int main() {
    demonstrate_wait_variants();
    return 0;
}
```

---

## 4. notify_one vs notify_all

### Choosing the Right Notification

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

std::mutex mtx;
std::condition_variable cv;
int work_item = 0;
bool done = false;

void worker_notify_one(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return work_item > 0 || done; });

        if (done) break;

        if (work_item > 0) {
            --work_item;
            std::cout << "Worker " << id << " processing item\n";
        }
    }
}

void notify_one_example() {
    std::cout << "=== notify_one() Example ===\n\n";

    std::vector<std::thread> workers;
    for (int i = 0; i < 3; ++i) {
        workers.emplace_back(worker_notify_one, i);
    }

    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            ++work_item;
        }
        cv.notify_one();  // Wake up ONE worker
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();  // Wake up ALL workers to finish

    for (auto& t : workers) {
        t.join();
    }

    std::cout << "\nnotify_one():\n";
    std::cout << "- Wakes up one waiting thread\n";
    std::cout << "- More efficient when only one can proceed\n";
    std::cout << "- Use for work distribution\n\n";

    std::cout << "notify_all():\n";
    std::cout << "- Wakes up all waiting threads\n";
    std::cout << "- Use when all threads should check condition\n";
    std::cout << "- Use for broadcast events\n\n";
}

int main() {
    notify_one_example();
    return 0;
}
```

---

## 5. Spurious Wakeups

### Handling Spurious Wakeups

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void spurious_wakeup_wrong() {
    std::unique_lock<std::mutex> lock(mtx);

    // WRONG: Doesn't check condition after wakeup
    cv.wait(lock);

    // ready might still be false!
    if (ready) {
        std::cout << "Spurious wakeup didn't occur\n";
    } else {
        std::cout << "Spurious wakeup occurred!\n";
    }
}

void spurious_wakeup_correct() {
    std::unique_lock<std::mutex> lock(mtx);

    // CORRECT: Predicate automatically handles spurious wakeups
    cv.wait(lock, []{ return ready; });

    std::cout << "Condition is guaranteed to be true\n";
}

// Equivalent to predicate version
void spurious_wakeup_manual() {
    std::unique_lock<std::mutex> lock(mtx);

    // Manual loop to handle spurious wakeups
    while (!ready) {
        cv.wait(lock);
    }

    std::cout << "Condition is guaranteed to be true\n";
}

void explain_spurious_wakeups() {
    std::cout << "=== Spurious Wakeups ===\n\n";

    std::cout << "What are spurious wakeups?\n";
    std::cout << "- Thread wakes up without notification\n";
    std::cout << "- OS/implementation detail\n";
    std::cout << "- Condition may not be true\n\n";

    std::cout << "How to handle:\n";
    std::cout << "1. Always use predicate with wait()\n";
    std::cout << "2. Or manually loop checking condition\n";
    std::cout << "3. NEVER assume condition is true after wait()\n\n";
}

int main() {
    explain_spurious_wakeups();
    return 0;
}
```

---

## 6. Thread Pool with Condition Variables

### Complete Thread Pool Implementation

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads) : stop_(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);

                        // Wait for task or stop signal
                        condition_.wait(lock, [this]() {
                            return stop_ || !tasks_.empty();
                        });

                        if (stop_ && tasks_.empty()) {
                            return;
                        }

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();

        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template<typename F>
    void enqueue(F&& task) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace(std::forward<F>(task));
        }
        condition_.notify_one();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};

void thread_pool_example() {
    std::cout << "=== Thread Pool with Condition Variables ===\n\n";

    ThreadPool pool(4);

    for (int i = 0; i < 20; ++i) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " executing on thread "
                      << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "\nAll tasks completed\n";
}

int main() {
    thread_pool_example();
    return 0;
}
```

---

## 7. Blocking Queue Implementation

### Thread-Safe Bounded Queue

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

template<typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(size_t capacity) : capacity_(capacity) {}

    void push(T item) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Wait until queue is not full
        not_full_.wait(lock, [this]() {
            return queue_.size() < capacity_;
        });

        queue_.push(std::move(item));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);

        // Wait until queue is not empty
        not_empty_.wait(lock, [this]() {
            return !queue_.empty();
        });

        T item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();

        return item;
    }

    bool try_pop(T& item, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!not_empty_.wait_for(lock, timeout, [this]() {
            return !queue_.empty();
        })) {
            return false;  // Timeout
        }

        item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();

        return true;
    }

private:
    std::queue<T> queue_;
    size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

void blocking_queue_example() {
    std::cout << "=== Blocking Queue ===\n\n";

    BlockingQueue<int> queue(10);

    std::thread producer([&]() {
        for (int i = 0; i < 20; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread consumer([&]() {
        for (int i = 0; i < 20; ++i) {
            int value = queue.pop();
            std::cout << "Consumed: " << value << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    producer.join();
    consumer.join();

    std::cout << "\n";
}

int main() {
    blocking_queue_example();
    return 0;
}
```

---

## 8. Best Practices

### Guidelines for Condition Variables

```cpp
#include <iostream>
#include <mutex>
#include <condition_variable>

void best_practices() {
    std::cout << "=== Condition Variable Best Practices ===\n\n";

    std::cout << "1. ALWAYS use a mutex:\n";
    std::cout << "   - CV must be used with a mutex\n";
    std::cout << "   - Protects shared state\n\n";

    std::cout << "2. ALWAYS use a predicate:\n";
    std::cout << "   - Handles spurious wakeups\n";
    std::cout << "   - Ensures condition is true\n\n";

    std::cout << "3. ALWAYS use unique_lock:\n";
    std::cout << "   - wait() needs to unlock/relock\n";
    std::cout << "   - lock_guard won't work\n\n";

    std::cout << "4. Notify OUTSIDE the lock:\n";
    std::cout << "   - More efficient\n";
    std::cout << "   - Reduces contention\n\n";

    std::cout << "5. Choose correct notification:\n";
    std::cout << "   - notify_one() for single waiter\n";
    std::cout << "   - notify_all() for multiple waiters\n\n";

    std::cout << "6. Avoid lost wakeups:\n";
    std::cout << "   - Set condition before notify\n";
    std::cout << "   - Check condition before wait\n\n";
}

int main() {
    best_practices();
    return 0;
}
```

---

## 9. Common Pitfalls

### Mistakes to Avoid

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// WRONG: Notifying while holding lock
void wrong_notify() {
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
    cv.notify_one();  // Less efficient
}

// CORRECT: Notify after releasing lock
void correct_notify() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }  // Lock released here
    cv.notify_one();  // More efficient
}

// WRONG: No predicate (spurious wakeups)
void wrong_wait() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);  // Spurious wakeup possible!
}

// CORRECT: Use predicate
void correct_wait() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // Safe
}

void common_pitfalls() {
    std::cout << "=== Common Pitfalls ===\n\n";

    std::cout << "Pitfall 1: No predicate\n";
    std::cout << "- Vulnerable to spurious wakeups\n";
    std::cout << "- Always use predicate form\n\n";

    std::cout << "Pitfall 2: Notify under lock\n";
    std::cout << "- Less efficient\n";
    std::cout << "- Notify after releasing lock\n\n";

    std::cout << "Pitfall 3: Lost wakeups\n";
    std::cout << "- Set flag before notify\n";
    std::cout << "- Check flag before wait\n\n";

    std::cout << "Pitfall 4: Using lock_guard\n";
    std::cout << "- wait() needs unique_lock\n";
    std::cout << "- Must be able to unlock/relock\n\n";
}

int main() {
    common_pitfalls();
    return 0;
}
```

---

## 10. Exercises

### Exercise 1: Semaphore
Implement a semaphore using condition variables.

### Exercise 2: Barrier
Create a barrier synchronization primitive.

### Exercise 3: Monitor
Implement a monitor pattern with CV.

### Exercise 4: Priority Queue
Build a priority blocking queue.

---

## Key Takeaways

1. Condition variables enable thread waiting and signaling
2. Always use with mutex and predicate
3. `wait()` blocks, `wait_for()` and `wait_until()` have timeouts
4. `notify_one()` wakes one thread, `notify_all()` wakes all
5. Spurious wakeups require predicate-based waiting
6. Use `unique_lock`, not `lock_guard`
7. Notify outside the lock for better performance
8. Avoid lost wakeups by proper synchronization
9. Condition variables are building blocks for higher-level primitives
10. Thread pools, queues, and barriers all use CVs

---

## Summary Checklist

Before moving to Lesson 50, ensure you can:
- [ ] Use condition variables for thread synchronization
- [ ] Implement wait with predicates
- [ ] Choose between notify_one and notify_all
- [ ] Handle spurious wakeups correctly
- [ ] Use wait_for and wait_until with timeouts
- [ ] Build blocking queues
- [ ] Create thread pools with CVs
- [ ] Avoid common pitfalls
- [ ] Follow best practices
- [ ] Design synchronization primitives

**Next Lesson**: Semaphores and Barriers - advanced synchronization primitives.

---

**Estimated Word Count**: ~4,500 words
