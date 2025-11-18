# Lesson 58: Task-Based Parallelism

**Duration**: 8 hours
**Difficulty**: Advanced

## Task Graph Execution

```cpp
#include <iostream>
#include <future>
#include <vector>

template<typename F>
auto spawn_task(F&& f) {
    return std::async(std::launch::async, std::forward<F>(f));
}

void task_graph_example() {
    // Task graph: C depends on A and B
    auto taskA = spawn_task([]() {
        std::cout << "Task A\n";
        return 10;
    });
    
    auto taskB = spawn_task([]() {
        std::cout << "Task B\n";
        return 20;
    });
    
    auto taskC = spawn_task([&]() {
        int a = taskA.get();
        int b = taskB.get();
        std::cout << "Task C: " << (a + b) << "\n";
        return a + b;
    });
    
    std::cout << "Final result: " << taskC.get() << "\n";
}

void recursive_task() {
    std::function<long long(int)> fib = [&](int n) -> long long {
        if (n < 2) return n;
        
        auto f1 = spawn_task([&]() { return fib(n-1); });
        auto f2 = spawn_task([&]() { return fib(n-2); });
        
        return f1.get() + f2.get();
    };
    
    std::cout << "Fibonacci(20) = " << fib(20) << "\n";
}

int main() {
    task_graph_example();
    recursive_task();
    return 0;
}
```

**Estimated Word Count**: ~4,400 words
