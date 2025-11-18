# Lesson 52: Work Stealing

**Duration**: 8 hours
**Difficulty**: Expert

## Complete Work-Stealing Deque

```cpp
#include <iostream>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <optional>

template<typename T>
class WorkStealingQueue {
public:
    void push_bottom(T item) {
        std::lock_guard<std::mutex> lock(mutex_);
        deque_.push_back(std::move(item));
    }
    
    std::optional<T> pop_bottom() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (deque_.empty()) return std::nullopt;
        T item = std::move(deque_.back());
        deque_.pop_back();
        return item;
    }
    
    std::optional<T> steal() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (deque_.empty()) return std::nullopt;
        T item = std::move(deque_.front());
        deque_.pop_front();
        return item;
    }
    
private:
    std::deque<T> deque_;
    std::mutex mutex_;
};

class WorkStealingThreadPool {
public:
    WorkStealingThreadPool(size_t num_threads) : stop_(false) {
        queues_.resize(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this, i]() { worker_thread(i); });
        }
    }
    
    ~WorkStealingThreadPool() {
        stop_ = true;
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }
    
    template<typename F>
    void submit(F&& task) {
        size_t index = counter_++ % queues_.size();
        queues_[index].push_bottom(std::forward<F>(task));
    }
    
private:
    void worker_thread(size_t index) {
        while (!stop_) {
            if (auto task = queues_[index].pop_bottom()) {
                (*task)();
            } else {
                // Try stealing from other queues
                bool stolen = false;
                for (size_t i = 0; i < queues_.size(); ++i) {
                    if (i != index) {
                        if (auto task = queues_[i].steal()) {
                            (*task)();
                            stolen = true;
                            break;
                        }
                    }
                }
                if (!stolen) {
                    std::this_thread::yield();
                }
            }
        }
    }
    
    std::vector<WorkStealingQueue<std::function<void()>>> queues_;
    std::vector<std::thread> workers_;
    std::atomic<size_t> counter_{0};
    std::atomic<bool> stop_;
};

int main() {
    WorkStealingThreadPool pool(4);
    for (int i = 0; i < 100; ++i) {
        pool.submit([i]() {
            std::cout << "Task " << i << "\n";
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
```

**Key Points**:
- Owner thread pushes/pops from bottom
- Other threads steal from top
- Reduces contention
- Better cache locality
- Self-balancing workload

**Estimated Word Count**: ~4,800 words
