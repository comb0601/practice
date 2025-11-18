# Lesson 71: Multithreading Basics

**Duration**: 6 hours
**Difficulty**: Advanced

C++11 introduced native multithreading support via `<thread>`.

## Creating Threads:

```cpp
#include <iostream>
#include <thread>

void worker() {
    std::cout << "Worker thread\n";
}

int main()
{
    std::thread t(worker);
    t.join();  // Wait for thread to finish

    std::cout << "Main thread\n";
    return 0;
}
```

## Thread with Parameters:

```cpp
#include <iostream>
#include <thread>

void printNumbers(int start, int end) {
    for (int i = start; i <= end; i++) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::thread t1(printNumbers, 1, 5);
    std::thread t2(printNumbers, 6, 10);

    t1.join();
    t2.join();

    return 0;
}
```

## Lambda Threads:

```cpp
#include <iostream>
#include <thread>

int main()
{
    int x = 10;

    std::thread t([&x]() {
        x += 5;
        std::cout << "Thread: x = " << x << "\n";
    });

    t.join();
    std::cout << "Main: x = " << x << "\n";

    return 0;
}
```

## Multiple Threads:

```cpp
#include <iostream>
#include <thread>
#include <vector>

void task(int id) {
    std::cout << "Task " << id << "\n";
}

int main()
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(task, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```

## detach vs join:

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void work() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Work done\n";
}

int main()
{
    std::thread t1(work);
    t1.join();  // Wait for completion

    std::thread t2(work);
    t2.detach();  // Run independently

    // Note: detached thread may not finish before main exits
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
```

**Key Takeaways:**
- std::thread for creating threads
- join() waits for thread completion
- detach() runs thread independently
- Pass parameters by value or reference
- Use lambdas for inline thread code
- Hardware concurrency: std::thread::hardware_concurrency()
