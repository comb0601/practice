# Lesson 72: Thread Synchronization

**Duration**: 6 hours
**Difficulty**: Advanced

Synchronization prevents race conditions when multiple threads access shared data.

## Race Condition Example:

```cpp
#include <iostream>
#include <thread>
#include <vector>

int counter = 0;  // Shared variable

void increment() {
    for (int i = 0; i < 100000; i++) {
        counter++;  // Race condition!
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << counter << "\n";  // Not 1000000!

    return 0;
}
```

## Using Atomic:

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> counter(0);  // Thread-safe

void increment() {
    for (int i = 0; i < 100000; i++) {
        counter++;  // Atomic operation
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << counter << "\n";  // Exactly 1000000

    return 0;
}
```

## Atomic Operations:

```cpp
#include <iostream>
#include <atomic>

int main()
{
    std::atomic<int> x(0);

    x.store(10);           // Atomic write
    int y = x.load();      // Atomic read
    x.fetch_add(5);        // Atomic increment by 5
    x.fetch_sub(2);        // Atomic decrement by 2

    std::cout << "x = " << x << "\n";

    // Compare and exchange
    int expected = 13;
    bool success = x.compare_exchange_strong(expected, 20);
    std::cout << "CAS: " << (success ? "success" : "failed") << "\n";

    return 0;
}
```

**Key Takeaways:**
- Race conditions occur with unsynchronized shared data
- std::atomic for lock-free atomic operations
- Use atomic for simple counters and flags
- Atomic operations have memory ordering guarantees
- Prefer atomic over mutex for simple operations
