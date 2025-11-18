# Lesson 75: Async and Future

**Duration**: 5 hours
**Difficulty**: Advanced

High-level asynchronous programming with std::async and std::future.

## Basic Async:

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int calculate() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main()
{
    std::future<int> result = std::async(calculate);

    std::cout << "Waiting for result...\n";
    int value = result.get();  // Blocks until ready
    std::cout << "Result: " << value << "\n";

    return 0;
}
```

## async with Lambda:

```cpp
#include <iostream>
#include <future>

int main()
{
    auto future = std::async([]() {
        return 100 + 200;
    });

    std::cout << "Result: " << future.get() << "\n";

    return 0;
}
```

## Launch Policies:

```cpp
#include <iostream>
#include <future>

int main()
{
    // Launch async (separate thread)
    auto f1 = std::async(std::launch::async, []() {
        return 42;
    });

    // Launch deferred (lazy evaluation)
    auto f2 = std::async(std::launch::deferred, []() {
        return 100;
    });

    // Default (implementation chooses)
    auto f3 = std::async([]() {
        return 200;
    });

    std::cout << f1.get() << "\n";
    std::cout << f2.get() << "\n";
    std::cout << f3.get() << "\n";

    return 0;
}
```

## Promise and Future:

```cpp
#include <iostream>
#include <future>
#include <thread>

void task(std::promise<int> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(42);
}

int main()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(task, std::move(prom));

    std::cout << "Waiting...\n";
    int value = fut.get();
    std::cout << "Value: " << value << "\n";

    t.join();

    return 0;
}
```

**Key Takeaways:**
- std::async for easy async operations
- std::future to get async results
- get() blocks until ready
- Launch policies control execution
- std::promise for manual result setting
- Cleaner than manual thread management
