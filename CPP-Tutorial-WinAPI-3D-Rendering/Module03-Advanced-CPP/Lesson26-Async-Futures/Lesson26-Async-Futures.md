# Lesson 26: Async & Futures

## std::async
Launch async tasks and get results via futures.

```cpp
#include <future>

int compute() { return 42; }

auto fut = std::async(std::launch::async, compute);
int result = fut.get();  // Block until ready
```

## std::future
Handle to async result.

## std::promise
Set value for future.

```cpp
std::promise<int> prom;
std::future<int> fut = prom.get_future();
prom.set_value(42);
int val = fut.get();
```

## Launch Policies
- **std::launch::async**: Run in new thread
- **std::launch::deferred**: Lazy evaluation
- **std::launch::async | std::launch::deferred**: Implementation choice
