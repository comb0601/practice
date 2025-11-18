# Lesson 24: Multithreading Basics

## Introduction to Multithreading
Modern C++ (C++11+) provides built-in support for multithreading through the `<thread>` header. This eliminates the need for platform-specific threading libraries.

## std::thread
The `std::thread` class represents a single thread of execution.

### Creating Threads
```cpp
#include <thread>

void function() {
    // Thread work
}

int main() {
    std::thread t(function);  // Create and start thread
    t.join();  // Wait for completion
}
```

### Thread Functions
- **join()**: Wait for thread to finish
- **detach()**: Let thread run independently
- **joinable()**: Check if thread can be joined
- **get_id()**: Get thread ID
- **hardware_concurrency()**: Get number of hardware threads

### Passing Arguments
```cpp
void func(int x, const std::string& s) {
    // Use x and s
}

std::thread t(func, 42, "hello");
```

### Lambda with Thread
```cpp
std::thread t([](int x) {
    std::cout << "Value: " << x << "\n";
}, 10);
```

## Thread Safety Issues
- **Data races**: Multiple threads accessing shared data
- **Deadlocks**: Threads waiting for each other
- **Race conditions**: Outcome depends on timing

## Best Practices
1. Always join or detach threads
2. Use RAII for thread management
3. Minimize shared state
4. Protect shared data with mutexes
5. Avoid detached threads when possible

See code examples for complete implementations.
