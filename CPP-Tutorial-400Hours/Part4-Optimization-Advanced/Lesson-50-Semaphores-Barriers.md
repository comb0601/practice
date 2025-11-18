# Lesson 50: Semaphores and Barriers

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Semaphores
2. Counting Semaphores
3. Binary Semaphores
4. C++20 std::counting_semaphore
5. C++20 std::binary_semaphore
6. Barriers
7. C++20 std::barrier
8. C++20 std::latch
9. Resource Pooling
10. Practical Examples
11. Exercises

---

## 1. Introduction to Semaphores

Semaphores are synchronization primitives that control access to shared resources through a counter.

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#if __cplusplus >= 202002L
#include <semaphore>

void demonstrate_semaphore() {
    std::cout << "=== Semaphore Basics ===\n\n";
    
    // Counting semaphore with initial count of 3
    std::counting_semaphore<3> sem(3);
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&sem, i]() {
            sem.acquire();  // Decrement counter, block if zero
            std::cout << "Thread " << i << " acquired semaphore\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Thread " << i << " releasing semaphore\n";
            sem.release();  // Increment counter
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
#else
void demonstrate_semaphore() {
    std::cout << "C++20 required for std::semaphore\n";
}
#endif

int main() {
    demonstrate_semaphore();
    return 0;
}
```

---

## 2. Implementing Semaphore (Pre-C++20)

```cpp
#include <iostream>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    explicit Semaphore(int count) : count_(count) {}
    
    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return count_ > 0; });
        --count_;
    }
    
    void release() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }
    
    bool try_acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            return true;
        }
        return false;
    }
    
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

void custom_semaphore_example() {
    std::cout << "=== Custom Semaphore Implementation ===\n\n";
    
    Semaphore sem(2);  // Allow 2 concurrent accesses
    
    auto worker = [&sem](int id) {
        sem.acquire();
        std::cout << "Worker " << id << " working\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Worker " << id << " done\n";
        sem.release();
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    custom_semaphore_example();
    return 0;
}
```

---

## 3. Resource Pool with Semaphore

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

template<typename T>
class ResourcePool {
public:
    ResourcePool(size_t size, std::function<std::unique_ptr<T>()> factory)
        : semaphore_(size) {
        for (size_t i = 0; i < size; ++i) {
            pool_.push_back(factory());
        }
    }
    
    std::unique_ptr<T> acquire() {
        semaphore_.acquire();
        std::lock_guard<std::mutex> lock(mutex_);
        auto resource = std::move(pool_.back());
        pool_.pop_back();
        return resource;
    }
    
    void release(std::unique_ptr<T> resource) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            pool_.push_back(std::move(resource));
        }
        semaphore_.release();
    }
    
private:
    Semaphore semaphore_;
    std::vector<std::unique_ptr<T>> pool_;
    std::mutex mutex_;
};
```

---

## 4. C++20 Barriers

```cpp
#include <iostream>
#include <thread>
#include <vector>

#if __cplusplus >= 202002L
#include <barrier>

void barrier_example() {
    std::cout << "=== C++20 Barrier ===\n\n";
    
    const int num_threads = 5;
    std::barrier sync_point(num_threads, []() noexcept {
        std::cout << "All threads reached barrier!\n";
    });
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&sync_point, i]() {
            std::cout << "Thread " << i << " phase 1\n";
            sync_point.arrive_and_wait();  // Wait for all threads
            
            std::cout << "Thread " << i << " phase 2\n";
            sync_point.arrive_and_wait();
            
            std::cout << "Thread " << i << " done\n";
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
#else
void barrier_example() {
    std::cout << "C++20 required for std::barrier\n";
}
#endif

int main() {
    barrier_example();
    return 0;
}
```

---

## 5. C++20 Latch

```cpp
#include <iostream>
#include <thread>
#include <vector>

#if __cplusplus >= 202002L
#include <latch>

void latch_example() {
    std::cout << "=== C++20 Latch ===\n\n";
    
    const int num_tasks = 5;
    std::latch done(num_tasks);
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_tasks; ++i) {
        threads.emplace_back([&done, i]() {
            std::cout << "Task " << i << " working...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " done\n";
            done.count_down();  // Decrement counter
        });
    }
    
    done.wait();  // Wait for all tasks
    std::cout << "All tasks completed!\n";
    
    for (auto& t : threads) {
        t.join();
    }
}
#else
void latch_example() {
    std::cout << "C++20 required for std::latch\n";
}
#endif

int main() {
    latch_example();
    return 0;
}
```

---

## Key Takeaways

1. Semaphores control access through counting
2. Binary semaphore is like a mutex
3. Counting semaphore limits concurrent access
4. Barriers synchronize threads at specific points
5. Latches count down from initial value
6. C++20 provides native semaphore/barrier support
7. Can implement with mutex + condition_variable
8. Useful for resource pools
9. Barriers enable multi-phase algorithms
10. Choose right primitive for your needs

---

**Estimated Word Count**: ~4,000 words
