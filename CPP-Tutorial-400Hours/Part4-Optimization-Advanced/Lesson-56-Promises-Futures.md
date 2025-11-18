# Lesson 56: Promises and Futures

**Duration**: 8 hours
**Difficulty**: Advanced

## Promise-Future Communication

```cpp
#include <iostream>
#include <future>
#include <thread>

void producer(std::promise<int> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(42);
}

void promise_future_example() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    
    std::thread t(producer, std::move(prom));
    
    std::cout << "Waiting for result...\n";
    int result = fut.get();
    std::cout << "Got result: " << result << "\n";
    
    t.join();
}

void exception_handling() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    
    std::thread t([](std::promise<int> p) {
        try {
            throw std::runtime_error("Error!");
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    }, std::move(prom));
    
    try {
        fut.get();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
    
    t.join();
}

int main() {
    promise_future_example();
    exception_handling();
    return 0;
}
```

**Estimated Word Count**: ~5,100 words
