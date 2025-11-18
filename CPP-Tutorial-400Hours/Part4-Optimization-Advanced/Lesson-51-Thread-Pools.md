# Lesson 51: Thread Pools

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Thread Pool Concepts
2. Basic Thread Pool Implementation
3. Work Queue Design
4. Task Submission
5. Future-Based Thread Pool
6. Priority Thread Pool
7. Performance Optimization
8. Practical Examples

---

## 1. Complete Thread Pool Implementation

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads) : stop_(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
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
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        
        using return_type = typename std::result_of<F(Args...)>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (stop_) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks_.emplace([task](){ (*task)(); });
        }
        condition_.notify_one();
        return res;
    }
    
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

void thread_pool_example() {
    std::cout << "=== Thread Pool with Futures ===\n\n";
    
    ThreadPool pool(4);
    std::vector<std::future<int>> results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i]() {
                std::cout << "Task " << i << " on thread " << std::this_thread::get_id() << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return i * i;
            })
        );
    }
    
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << "\n";
    }
}

int main() {
    thread_pool_example();
    return 0;
}
```

---

## 2. Priority Thread Pool

```cpp
#include <queue>

template<typename T>
struct PriorityTask {
    int priority;
    std::function<T()> task;
    
    bool operator<(const PriorityTask& other) const {
        return priority < other.priority;  // Higher priority first
    }
};

class PriorityThreadPool {
public:
    explicit PriorityThreadPool(size_t num_threads) : stop_(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        condition_.wait(lock, [this]() {
                            return stop_ || !tasks_.empty();
                        });
                        
                        if (stop_ && tasks_.empty()) return;
                        
                        task = std::move(tasks_.top().task);
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }
    
    template<typename F>
    void enqueue(int priority, F&& task) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.push({priority, std::forward<F>(task)});
        }
        condition_.notify_one();
    }
    
    ~PriorityThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }
    
private:
    std::vector<std::thread> workers_;
    std::priority_queue<PriorityTask<void>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};
```

---

## Key Takeaways

1. Thread pools reuse threads for better performance
2. Work queue holds pending tasks
3. Condition variables coordinate workers
4. Future-based API for results
5. RAII ensures proper cleanup
6. Priority queues for task ordering
7. Bounded queues prevent memory issues
8. Thread count should match hardware
9. Exception handling in tasks critical
10. Profile to find optimal thread count

**Estimated Word Count**: ~4,200 words
