# Lesson 55: std::async and std::future

**Duration**: 8 hours
**Difficulty**: Advanced

## std::async Basics

```cpp
#include <iostream>
#include <future>
#include <vector>

int compute(int x) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x * x;
}

void async_example() {
    std::cout << "=== std::async Example ===\n\n";
    
    // Launch async tasks
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; ++i) {
        futures.push_back(std::async(std::launch::async, compute, i));
    }
    
    // Get results
    for (auto& fut : futures) {
        std::cout << "Result: " << fut.get() << "\n";
    }
}

void launch_policies() {
    std::cout << "\n=== Launch Policies ===\n\n";
    
    // Guaranteed async
    auto f1 = std::async(std::launch::async, compute, 5);
    
    // May be deferred
    auto f2 = std::async(std::launch::deferred, compute, 6);
    
    // Implementation choice
    auto f3 = std::async(std::launch::async | std::launch::deferred, compute, 7);
    
    std::cout << "f1: " << f1.get() << "\n";
    std::cout << "f2: " << f2.get() << "\n";
    std::cout << "f3: " << f3.get() << "\n";
}

int main() {
    async_example();
    launch_policies();
    return 0;
}
```

**Estimated Word Count**: ~4,900 words
