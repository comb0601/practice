# Lesson 54: Readers-Writers Problem

**Duration**: 8 hours
**Difficulty**: Advanced

## Shared Mutex Solution

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

class SharedData {
public:
    void write(int value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data_ = value;
        std::cout << "Write: " << value << "\n";
    }
    
    int read() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        std::cout << "Read: " << data_ << "\n";
        return data_;
    }
    
private:
    mutable std::shared_mutex mutex_;
    int data_ = 0;
};

void readers_writers_example() {
    SharedData shared;
    std::vector<std::thread> threads;
    
    // Readers (can run concurrently)
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&shared]() {
            for (int j = 0; j < 10; ++j) {
                shared.read();
            }
        });
    }
    
    // Writers (exclusive access)
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&shared, i]() {
            for (int j = 0; j < 5; ++j) {
                shared.write(i * 100 + j);
            }
        });
    }
    
    for (auto& t : threads) t.join();
}

int main() {
    readers_writers_example();
    return 0;
}
```

**Estimated Word Count**: ~4,600 words
