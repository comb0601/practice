# Lesson 53: Producer-Consumer Pattern

**Duration**: 8 hours
**Difficulty**: Advanced

## Multiple Producers, Multiple Consumers

```cpp
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

template<typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(size_t capacity) : capacity_(capacity) {}
    
    void produce(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });
        queue_.push(std::move(item));
        not_empty_.notify_one();
    }
    
    T consume() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { return !queue_.empty(); });
        T item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return item;
    }
    
private:
    std::queue<T> queue_;
    size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

void mpmc_example() {
    BoundedQueue<int> queue(10);
    std::vector<std::thread> producers, consumers;
    
    // Multiple producers
    for (int p = 0; p < 3; ++p) {
        producers.emplace_back([&queue, p]() {
            for (int i = 0; i < 20; ++i) {
                queue.produce(p * 100 + i);
                std::cout << "P" << p << " produced " << (p*100+i) << "\n";
            }
        });
    }
    
    // Multiple consumers
    for (int c = 0; c < 2; ++c) {
        consumers.emplace_back([&queue, c]() {
            for (int i = 0; i < 30; ++i) {
                int item = queue.consume();
                std::cout << "C" << c << " consumed " << item << "\n";
            }
        });
    }
    
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
}

int main() {
    mpmc_example();
    return 0;
}
```

**Estimated Word Count**: ~5,000 words
