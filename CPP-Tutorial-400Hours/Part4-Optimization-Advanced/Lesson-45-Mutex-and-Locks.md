# Lesson 45: Mutex and Locks

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Mutexes
2. std::mutex Fundamentals
3. Lock Management with RAII
4. std::lock_guard
5. std::unique_lock
6. std::scoped_lock (C++17)
7. Recursive Mutexes
8. Timed Mutexes
9. Shared Mutexes (Reader-Writer Locks)
10. Deadlock Prevention
11. Lock Granularity
12. Performance Considerations
13. Practical Examples
14. Exercises

---

## 1. Introduction to Mutexes

### What is a Mutex?

**Mutex** (Mutual Exclusion) is a synchronization primitive that protects shared data from being simultaneously accessed by multiple threads.

**Key Concept**: Only one thread can hold the mutex at a time.

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int shared_counter = 0;

void increment_with_mutex(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        mtx.lock();        // Acquire lock
        ++shared_counter;  // Critical section
        mtx.unlock();      // Release lock
    }
}

void demonstrate_mutex() {
    std::cout << "=== Mutex Demonstration ===\n\n";

    std::vector<std::thread> threads;
    const int num_threads = 10;
    const int iterations = 10000;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment_with_mutex, iterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: " << (num_threads * iterations) << "\n";
    std::cout << "Actual: " << shared_counter << "\n";
    std::cout << "Result: " << (shared_counter == num_threads * iterations ? "SUCCESS" : "FAILURE") << "\n\n";
}

int main() {
    demonstrate_mutex();
    return 0;
}
```

---

## 2. std::mutex Fundamentals

### Basic Operations

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void basic_mutex_operations() {
    std::cout << "=== Basic Mutex Operations ===\n\n";

    // lock() - Blocks until lock is acquired
    mtx.lock();
    std::cout << "Lock acquired\n";

    // Critical section
    std::cout << "Performing thread-safe operation\n";

    // unlock() - Releases the lock
    mtx.unlock();
    std::cout << "Lock released\n\n";

    // try_lock() - Non-blocking attempt
    if (mtx.try_lock()) {
        std::cout << "try_lock() succeeded\n";
        mtx.unlock();
    } else {
        std::cout << "try_lock() failed\n";
    }
}

int main() {
    basic_mutex_operations();
    return 0;
}
```

### Critical Section

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class ThreadSafeCounter {
public:
    void increment() {
        mutex_.lock();
        // === CRITICAL SECTION START ===
        ++counter_;
        // === CRITICAL SECTION END ===
        mutex_.unlock();
    }

    int get() const {
        // For const correctness, we need mutable mutex
        mutex_.lock();
        int value = counter_;
        mutex_.unlock();
        return value;
    }

private:
    int counter_ = 0;
    mutable std::mutex mutex_;
};

void critical_section_example() {
    std::cout << "=== Critical Section Example ===\n\n";

    ThreadSafeCounter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter value: " << counter.get() << "\n\n";
}

int main() {
    critical_section_example();
    return 0;
}
```

---

## 3. Lock Management with RAII

### The Problem with Manual Locking

```cpp
#include <iostream>
#include <mutex>
#include <stdexcept>

std::mutex mtx;

void dangerous_function() {
    mtx.lock();

    // If exception is thrown here, unlock() is never called!
    if (/* some condition */) {
        throw std::runtime_error("Error!");
    }

    mtx.unlock(); // Never reached if exception thrown
}

// This function will deadlock if exception is thrown!
```

### RAII Solution

```cpp
#include <iostream>
#include <mutex>
#include <stdexcept>

std::mutex mtx;

void safe_function() {
    std::lock_guard<std::mutex> lock(mtx); // Acquires lock

    // Even if exception is thrown, destructor releases lock
    if (/* some condition */) {
        throw std::runtime_error("Error!");
    }

    // Lock automatically released when 'lock' goes out of scope
}

void demonstrate_raii() {
    std::cout << "=== RAII Lock Management ===\n\n";

    try {
        safe_function();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
        std::cout << "Mutex was automatically released!\n\n";
    }
}

int main() {
    demonstrate_raii();
    return 0;
}
```

---

## 4. std::lock_guard

### Basic Usage

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

class ThreadSafeMap {
public:
    void insert(int key, std::string value) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_[key] = std::move(value);
    }

    bool find(int key, std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = data_.find(key);
        if (it != data_.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.size();
    }

private:
    std::map<int, std::string> data_;
    mutable std::mutex mutex_;
};

void lock_guard_example() {
    std::cout << "=== std::lock_guard Example ===\n\n";

    ThreadSafeMap map;
    std::vector<std::thread> threads;

    // Multiple threads inserting
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&map, i]() {
            map.insert(i, "Value " + std::to_string(i));
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Map size: " << map.size() << "\n";

    std::string value;
    if (map.find(5, value)) {
        std::cout << "Key 5: " << value << "\n";
    }

    std::cout << "\n";
}

int main() {
    lock_guard_example();
    return 0;
}
```

### Characteristics

```cpp
#include <iostream>
#include <mutex>

void lock_guard_characteristics() {
    std::cout << "=== std::lock_guard Characteristics ===\n\n";

    std::cout << "1. RAII-based lock management\n";
    std::cout << "2. Locks on construction\n";
    std::cout << "3. Unlocks on destruction\n";
    std::cout << "4. Cannot be manually unlocked\n";
    std::cout << "5. Cannot be moved or copied\n";
    std::cout << "6. Lightweight and efficient\n";
    std::cout << "7. Best for simple lock scopes\n\n";

    std::cout << "Use when:\n";
    std::cout << "- Simple lock/unlock pattern\n";
    std::cout << "- Lock held for entire scope\n";
    std::cout << "- No need for early release\n";
    std::cout << "- No need for deferred locking\n\n";
}

int main() {
    lock_guard_characteristics();
    return 0;
}
```

---

## 5. std::unique_lock

### Flexible Locking

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;

void demonstrate_unique_lock() {
    std::cout << "=== std::unique_lock Flexibility ===\n\n";

    // 1. Basic usage (like lock_guard)
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Lock acquired\n";
    } // Automatically unlocked

    // 2. Deferred locking
    {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        std::cout << "Lock created but not acquired\n";
        std::cout << "Locked: " << lock.owns_lock() << "\n";

        lock.lock();
        std::cout << "Now locked: " << lock.owns_lock() << "\n";
    }

    // 3. Try to lock
    {
        std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
        if (lock.owns_lock()) {
            std::cout << "Try-lock succeeded\n";
        } else {
            std::cout << "Try-lock failed\n";
        }
    }

    // 4. Adopt existing lock
    {
        mtx.lock();
        std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
        std::cout << "Adopted existing lock\n";
    }

    std::cout << "\n";
}

int main() {
    demonstrate_unique_lock();
    return 0;
}
```

### Manual Lock/Unlock

```cpp
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
int data = 0;

void complex_operation() {
    std::unique_lock<std::mutex> lock(mtx);

    // Critical section 1
    data += 10;
    std::cout << "First critical section: " << data << "\n";

    // Unlock to allow other threads
    lock.unlock();

    // Non-critical work
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Re-lock for critical section 2
    lock.lock();
    data += 5;
    std::cout << "Second critical section: " << data << "\n";

    // Automatically unlocks at end of scope
}

void manual_lock_unlock_example() {
    std::cout << "=== Manual Lock/Unlock with unique_lock ===\n\n";

    std::thread t1(complex_operation);
    std::thread t2(complex_operation);

    t1.join();
    t2.join();

    std::cout << "\n";
}

int main() {
    manual_lock_unlock_example();
    return 0;
}
```

### Move Semantics

```cpp
#include <iostream>
#include <mutex>
#include <utility>

std::mutex mtx;

std::unique_lock<std::mutex> get_lock() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Lock acquired in function\n";
    return lock; // Move semantics
}

void transfer_lock_ownership() {
    std::cout << "=== Transferring Lock Ownership ===\n\n";

    std::unique_lock<std::mutex> lock1 = get_lock();
    std::cout << "Lock1 owns lock: " << lock1.owns_lock() << "\n";

    // Move to another unique_lock
    std::unique_lock<std::mutex> lock2 = std::move(lock1);
    std::cout << "After move:\n";
    std::cout << "  Lock1 owns lock: " << lock1.owns_lock() << "\n";
    std::cout << "  Lock2 owns lock: " << lock2.owns_lock() << "\n\n";
}

int main() {
    transfer_lock_ownership();
    return 0;
}
```

---

## 6. std::scoped_lock (C++17)

### Multiple Mutex Locking

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#if __cplusplus >= 201703L

std::mutex mtx1, mtx2, mtx3;
int counter1 = 0, counter2 = 0, counter3 = 0;

void scoped_lock_example() {
    std::cout << "=== std::scoped_lock (C++17) ===\n\n";

    // Locks multiple mutexes atomically (deadlock-free)
    std::scoped_lock lock(mtx1, mtx2, mtx3);

    ++counter1;
    ++counter2;
    ++counter3;

    std::cout << "All three mutexes locked safely\n";

    // All mutexes automatically unlocked at end of scope
}

void demonstrate_scoped_lock() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(scoped_lock_example);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter1: " << counter1 << "\n";
    std::cout << "Counter2: " << counter2 << "\n";
    std::cout << "Counter3: " << counter3 << "\n\n";
}

#endif

int main() {
#if __cplusplus >= 201703L
    demonstrate_scoped_lock();
#else
    std::cout << "C++17 required for std::scoped_lock\n";
#endif
    return 0;
}
```

### Benefits Over lock_guard

```cpp
#include <iostream>
#include <mutex>

void compare_scoped_lock() {
    std::cout << "=== scoped_lock vs lock_guard ===\n\n";

    std::cout << "std::lock_guard:\n";
    std::cout << "- Locks single mutex\n";
    std::cout << "- Simple and lightweight\n";
    std::cout << "- Pre-C++17 compatible\n\n";

    std::cout << "std::scoped_lock:\n";
    std::cout << "- Locks one or more mutexes\n";
    std::cout << "- Deadlock-free for multiple mutexes\n";
    std::cout << "- Preferred in C++17 and later\n";
    std::cout << "- Can replace both lock_guard and std::lock\n\n";

    std::cout << "Recommendation:\n";
    std::cout << "- C++17+: Always use scoped_lock\n";
    std::cout << "- Pre-C++17: Use lock_guard for single mutex\n";
    std::cout << "- Pre-C++17: Use std::lock for multiple mutexes\n\n";
}

int main() {
    compare_scoped_lock();
    return 0;
}
```

---

## 7. Recursive Mutexes

### std::recursive_mutex

```cpp
#include <iostream>
#include <thread>
#include <mutex>

class RecursiveExample {
public:
    void process(int depth) {
        std::lock_guard<std::recursive_mutex> lock(recursive_mtx_);

        std::cout << "Depth: " << depth << "\n";

        if (depth > 0) {
            // Same thread can lock again (recursively)
            process(depth - 1);
        }
    }

    void process_regular(int depth) {
        std::lock_guard<std::mutex> lock(regular_mtx_);

        std::cout << "Depth: " << depth << "\n";

        if (depth > 0) {
            // DEADLOCK! Regular mutex can't be locked recursively
            // process_regular(depth - 1);
        }
    }

private:
    std::recursive_mutex recursive_mtx_;
    std::mutex regular_mtx_;
};

void recursive_mutex_example() {
    std::cout << "=== Recursive Mutex Example ===\n\n";

    RecursiveExample example;

    std::cout << "With recursive_mutex:\n";
    example.process(5);

    std::cout << "\nNote: Regular mutex would deadlock on recursion\n\n";
}

int main() {
    recursive_mutex_example();
    return 0;
}
```

### When to Use Recursive Mutex

```cpp
#include <iostream>
#include <mutex>

class Account {
public:
    void deposit(double amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        balance_ += amount;
    }

    void withdraw(double amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        balance_ -= amount;
    }

    void transfer_to(Account& other, double amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        withdraw(amount); // Recursive lock
        other.deposit(amount);
    }

    double balance() const {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        return balance_;
    }

private:
    double balance_ = 0.0;
    mutable std::recursive_mutex mtx_;
};

void when_to_use_recursive() {
    std::cout << "=== When to Use Recursive Mutex ===\n\n";

    std::cout << "Use Cases:\n";
    std::cout << "1. Methods calling other methods in same class\n";
    std::cout << "2. Recursive algorithms needing synchronization\n";
    std::cout << "3. Legacy code refactoring\n\n";

    std::cout << "Warnings:\n";
    std::cout << "- Usually indicates design issue\n";
    std::cout << "- Prefer refactoring to avoid need\n";
    std::cout << "- Performance overhead vs regular mutex\n";
    std::cout << "- Can hide logic errors\n\n";
}

int main() {
    when_to_use_recursive();
    return 0;
}
```

---

## 8. Timed Mutexes

### std::timed_mutex

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::timed_mutex tmtx;

void try_lock_for_example() {
    std::cout << "=== try_lock_for Example ===\n\n";

    // Try to lock for 100ms
    if (tmtx.try_lock_for(std::chrono::milliseconds(100))) {
        std::cout << "Lock acquired within 100ms\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        tmtx.unlock();
    } else {
        std::cout << "Could not acquire lock within 100ms\n";
    }
}

void try_lock_until_example() {
    std::cout << "\n=== try_lock_until Example ===\n\n";

    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(1);

    if (tmtx.try_lock_until(deadline)) {
        std::cout << "Lock acquired before deadline\n";
        tmtx.unlock();
    } else {
        std::cout << "Could not acquire lock before deadline\n";
    }
}

int main() {
    try_lock_for_example();
    try_lock_until_example();
    return 0;
}
```

### Timeout Pattern

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

class ResourceWithTimeout {
public:
    bool try_process(int timeout_ms) {
        std::unique_lock<std::timed_mutex> lock(
            mtx_,
            std::chrono::milliseconds(timeout_ms)
        );

        if (lock.owns_lock()) {
            // Successfully acquired lock
            std::cout << "Processing resource...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return true;
        } else {
            // Timeout
            std::cout << "Could not acquire resource within timeout\n";
            return false;
        }
    }

private:
    std::timed_mutex mtx_;
};

void timeout_pattern_example() {
    std::cout << "=== Timeout Pattern ===\n\n";

    ResourceWithTimeout resource;

    std::thread t1([&]() {
        resource.try_process(200); // Should succeed
    });

    std::thread t2([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        resource.try_process(50); // Might timeout
    });

    t1.join();
    t2.join();

    std::cout << "\n";
}

int main() {
    timeout_pattern_example();
    return 0;
}
```

---

## 9. Shared Mutexes (Reader-Writer Locks)

### std::shared_mutex (C++17)

```cpp
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>

#if __cplusplus >= 201703L

class ThreadSafeCounter {
public:
    void increment() {
        std::unique_lock<std::shared_mutex> lock(mutex_); // Exclusive
        ++counter_;
    }

    int read() const {
        std::shared_lock<std::shared_mutex> lock(mutex_); // Shared
        return counter_;
    }

private:
    mutable std::shared_mutex mutex_;
    int counter_ = 0;
};

void shared_mutex_example() {
    std::cout << "=== std::shared_mutex Example ===\n\n";

    ThreadSafeCounter counter;
    std::vector<std::thread> threads;

    // Multiple readers (can run simultaneously)
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&counter, i]() {
            for (int j = 0; j < 100; ++j) {
                int value = counter.read();
                // Multiple readers can read simultaneously
            }
            std::cout << "Reader " << i << " done\n";
        });
    }

    // Few writers (exclusive access)
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&counter, i]() {
            for (int j = 0; j < 100; ++j) {
                counter.increment();
            }
            std::cout << "Writer " << i << " done\n";
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter: " << counter.read() << "\n\n";
}

#endif

int main() {
#if __cplusplus >= 201703L
    shared_mutex_example();
#else
    std::cout << "C++17 required for std::shared_mutex\n";
#endif
    return 0;
}
```

---

## 10. Deadlock Prevention

### Deadlock Example

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1, mtx2;

void thread1_deadlock() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock2(mtx2); // Potential deadlock
    std::cout << "Thread 1 got both locks\n";
}

void thread2_deadlock() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock1(mtx1); // Potential deadlock
    std::cout << "Thread 2 got both locks\n";
}

// DON'T RUN THIS - it will deadlock!
```

### Solution: std::lock

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx1, mtx2;
int data1 = 0, data2 = 0;

void safe_dual_lock() {
    // Lock both mutexes atomically (deadlock-free)
    std::lock(mtx1, mtx2);

    // Adopt the locks
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);

    ++data1;
    ++data2;
}

void deadlock_free_example() {
    std::cout << "=== Deadlock-Free Multiple Locks ===\n\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(safe_dual_lock);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Data1: " << data1 << "\n";
    std::cout << "Data2: " << data2 << "\n";
    std::cout << "No deadlock occurred!\n\n";
}

int main() {
    deadlock_free_example();
    return 0;
}
```

---

## 11. Exercises

### Exercise 1: Thread-Safe Stack
Implement a thread-safe stack using mutex.

### Exercise 2: Bank Account
Create a thread-safe bank account class with transfer functionality.

### Exercise 3: Deadlock Scenario
Identify and fix deadlock in provided code.

### Exercise 4: Reader-Writer Cache
Implement a cache with shared_mutex for optimal read performance.

---

## Key Takeaways

1. Always use RAII for lock management
2. `lock_guard` for simple cases, `unique_lock` for flexibility
3. C++17 `scoped_lock` for multiple mutexes
4. Avoid recursive mutexes when possible - they indicate design issues
5. Use timed mutexes for timeout scenarios
6. `shared_mutex` optimizes read-heavy workloads
7. Prevent deadlock with `std::lock` or `scoped_lock`
8. Keep critical sections as small as possible
9. Never call unknown code while holding a lock
10. Document locking strategy in your code

---

## Summary Checklist

Before moving to Lesson 46, ensure you can:
- [ ] Use mutex to protect shared data
- [ ] Apply RAII with lock_guard and unique_lock
- [ ] Use scoped_lock for multiple mutexes (C++17)
- [ ] Understand when to use recursive_mutex
- [ ] Implement timeouts with timed_mutex
- [ ] Use shared_mutex for reader-writer scenarios
- [ ] Prevent and detect deadlocks
- [ ] Choose appropriate mutex type for each scenario
- [ ] Optimize critical section granularity
- [ ] Design thread-safe classes

**Next Lesson**: Lock-Free Programming Basics - advanced synchronization techniques.

---

**Estimated Word Count**: ~5,400 words
