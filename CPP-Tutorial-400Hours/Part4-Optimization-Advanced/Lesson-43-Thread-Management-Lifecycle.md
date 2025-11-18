# Lesson 43: Thread Management and Lifecycle

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Thread States and Lifecycle
2. join() vs detach()
3. joinable() State
4. Thread Destruction and std::terminate
5. Transferring Thread Ownership
6. Thread-Local Storage
7. Exception Handling in Threads
8. Graceful Thread Termination
9. Thread Naming and Debugging
10. Advanced Thread Management Patterns
11. Practical Examples
12. Exercises

---

## 1. Thread States and Lifecycle

### Thread State Diagram

```
┌─────────────┐
│   Created   │ (std::thread object created)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Joinable   │ (Thread is running or finished)
└──────┬──────┘
       │
       ├──► join()  ──────┐
       │                  │
       └──► detach() ─────┤
                          │
                          ▼
                   ┌─────────────┐
                   │ Not Joinable│ (Thread object can be destroyed)
                   └─────────────┘
```

### Basic Lifecycle Example

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void worker_task(int id) {
    std::cout << "Thread " << id << " starting\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Thread " << id << " finishing\n";
}

void demonstrate_lifecycle() {
    std::cout << "=== Thread Lifecycle ===\n\n";

    // State 1: Created but not yet running
    std::thread t(worker_task, 1);
    std::cout << "Thread created, joinable: " << t.joinable() << "\n";
    std::cout << "Thread ID: " << t.get_id() << "\n\n";

    // State 2: Running
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread running, joinable: " << t.joinable() << "\n\n";

    // State 3: Finished but not yet joined
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Thread finished, joinable: " << t.joinable() << "\n\n";

    // State 4: Joined
    t.join();
    std::cout << "Thread joined, joinable: " << t.joinable() << "\n";
    std::cout << "Thread ID after join: " << t.get_id() << "\n\n";
}

int main() {
    demonstrate_lifecycle();
    return 0;
}
```

### Thread Lifecycle States Explained

1. **Not-a-thread**: Default-constructed `std::thread` or after move/join/detach
2. **Joinable**: Thread is associated with an active thread of execution
3. **Finished**: Thread function has returned but not yet joined
4. **Joined**: `join()` called, thread resources reclaimed
5. **Detached**: `detach()` called, thread runs independently

---

## 2. join() vs detach()

### join() - Wait for Completion

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void task_with_join(int id, int duration_ms) {
    std::cout << "Task " << id << " starting\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    std::cout << "Task " << id << " completed\n";
}

void demonstrate_join() {
    std::cout << "=== join() Demonstration ===\n\n";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;

    // Create multiple threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(task_with_join, i, 100 * (i + 1));
    }

    std::cout << "All threads created\n\n";

    // Join all threads - wait for all to complete
    for (auto& t : threads) {
        std::cout << "Joining thread " << t.get_id() << "\n";
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nAll threads joined. Total time: " << duration.count() << "ms\n\n";
}

int main() {
    demonstrate_join();
    return 0;
}
```

**Key Points about join()**:
- Blocks until thread completes
- Must be called exactly once
- Can be called after thread has finished
- Calling on non-joinable thread is undefined behavior

### detach() - Fire and Forget

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void background_task(int id) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Background task " << id << " completed\n";
}

void demonstrate_detach() {
    std::cout << "=== detach() Demonstration ===\n\n";

    std::thread t(background_task, 1);
    std::cout << "Thread created, joinable: " << t.joinable() << "\n";

    t.detach();
    std::cout << "Thread detached, joinable: " << t.joinable() << "\n";

    // Thread continues to run independently
    std::cout << "Main thread continues without waiting\n";

    // Sleep to allow detached thread to potentially complete
    // (In real code, you can't rely on this - detached threads may not finish)
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Main thread finishing\n\n";
}

int main() {
    demonstrate_detach();
    return 0;
}
```

**Key Points about detach()**:
- Thread runs independently
- No way to join after detaching
- Main thread can exit before detached thread finishes
- Detached thread may not complete if process terminates
- Cannot access local variables from main thread safely

### Comparison: join() vs detach()

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

std::atomic<int> shared_counter{0};

void increment_task(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        shared_counter.fetch_add(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void compare_join_detach() {
    std::cout << "=== join() vs detach() Comparison ===\n\n";

    // Test 1: Using join()
    {
        shared_counter = 0;
        std::thread t(increment_task, 100);
        t.join(); // Wait for completion
        std::cout << "With join() - Counter: " << shared_counter << " (expected: 100)\n";
    }

    // Test 2: Using detach()
    {
        shared_counter = 0;
        std::thread t(increment_task, 100);
        t.detach(); // Don't wait
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Partial wait
        std::cout << "With detach() - Counter: " << shared_counter << " (unpredictable, < 100)\n";
    }

    std::cout << "\n";
}

int main() {
    compare_join_detach();
    return 0;
}
```

### When to Use Each

**Use join() when**:
- You need the result of the thread's work
- Thread accesses data that will be destroyed
- You need to ensure thread completes before continuing
- You want predictable, synchronous behavior

**Use detach() when**:
- Thread is completely independent
- Thread only accesses its own data or global/static data
- Fire-and-forget tasks (logging, monitoring)
- Long-running background services

---

## 3. joinable() State

### Understanding joinable()

```cpp
#include <iostream>
#include <thread>

void simple_task() {
    std::cout << "Task executing\n";
}

void demonstrate_joinable() {
    std::cout << "=== joinable() States ===\n\n";

    // Default-constructed thread (not joinable)
    std::thread t1;
    std::cout << "Default-constructed: joinable = " << t1.joinable() << "\n";

    // Constructed with function (joinable)
    std::thread t2(simple_task);
    std::cout << "With function: joinable = " << t2.joinable() << "\n";

    // After join (not joinable)
    t2.join();
    std::cout << "After join: joinable = " << t2.joinable() << "\n";

    // Constructed then detached (not joinable)
    std::thread t3(simple_task);
    t3.detach();
    std::cout << "After detach: joinable = " << t3.joinable() << "\n";

    // After move (source not joinable, destination joinable)
    std::thread t4(simple_task);
    std::thread t5 = std::move(t4);
    std::cout << "After move - source: joinable = " << t4.joinable() << "\n";
    std::cout << "After move - destination: joinable = " << t5.joinable() << "\n";
    t5.join();

    std::cout << "\n";
}

int main() {
    demonstrate_joinable();
    return 0;
}
```

### Safe Join Pattern

```cpp
#include <iostream>
#include <thread>
#include <vector>

void safe_join(std::thread& t) {
    if (t.joinable()) {
        t.join();
    }
}

void safe_join_example() {
    std::cout << "=== Safe Join Pattern ===\n\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            std::cout << "Thread " << i << " executing\n";
        });
    }

    // Safe join - checks joinable first
    for (auto& t : threads) {
        safe_join(t);
    }

    // Trying to join again is safe (does nothing)
    for (auto& t : threads) {
        safe_join(t);
    }

    std::cout << "All threads safely joined\n\n";
}

int main() {
    safe_join_example();
    return 0;
}
```

---

## 4. Thread Destruction and std::terminate

### The Danger of Unjoined Threads

```cpp
#include <iostream>
#include <thread>

void task() {
    std::cout << "Task executing\n";
}

void dangerous_function() {
    std::thread t(task);
    // DANGER: t goes out of scope without join() or detach()
    // This calls std::terminate()!
}

// DON'T RUN THIS FUNCTION - it will terminate the program!
```

### Why std::terminate is Called

When a `std::thread` object with an associated thread of execution (joinable) is destroyed, the program calls `std::terminate()`. This is intentional to prevent:
- Resource leaks
- Undefined behavior
- Data races

### RAII Solution: Thread Guard

```cpp
#include <iostream>
#include <thread>
#include <stdexcept>

// Thread guard class ensures thread is always joined
class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& t) : thread_(t) {}

    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    // Prevent copying
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

private:
    std::thread& thread_;
};

void may_throw() {
    throw std::runtime_error("Oops!");
}

void safe_thread_management() {
    std::cout << "=== Safe Thread Management with RAII ===\n\n";

    std::thread t([]() {
        std::cout << "Thread executing\n";
    });

    ThreadGuard guard(t);

    try {
        may_throw();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    // ThreadGuard destructor ensures thread is joined
    std::cout << "Function ending - thread will be safely joined\n\n";
}

int main() {
    safe_thread_management();
    return 0;
}
```

### Scoped Thread (Alternative RAII)

```cpp
#include <iostream>
#include <thread>
#include <utility>

class ScopedThread {
public:
    explicit ScopedThread(std::thread t) : thread_(std::move(t)) {
        if (!thread_.joinable()) {
            throw std::logic_error("No thread");
        }
    }

    ~ScopedThread() {
        thread_.join();
    }

    // Prevent copying
    ScopedThread(const ScopedThread&) = delete;
    ScopedThread& operator=(const ScopedThread&) = delete;

private:
    std::thread thread_;
};

void scoped_thread_example() {
    std::cout << "=== Scoped Thread Example ===\n\n";

    ScopedThread st(std::thread([]() {
        std::cout << "Thread in ScopedThread\n";
    }));

    // Thread automatically joined when st goes out of scope
    std::cout << "Function ending\n\n";
}

int main() {
    scoped_thread_example();
    return 0;
}
```

### C++20 std::jthread

```cpp
#include <iostream>
#include <thread>
#include <chrono>

#if __cplusplus >= 202002L
#include <stop_token>

void jthread_example() {
    std::cout << "=== std::jthread (C++20) ===\n\n";

    // Automatically joins in destructor
    std::jthread t([]() {
        std::cout << "jthread executing\n";
    });

    // No need to manually join - destructor handles it!
    std::cout << "Function ending - jthread auto-joins\n\n";
}
#endif

int main() {
#if __cplusplus >= 202002L
    jthread_example();
#else
    std::cout << "C++20 required for std::jthread\n\n";
#endif
    return 0;
}
```

---

## 5. Transferring Thread Ownership

### Move Semantics with Threads

```cpp
#include <iostream>
#include <thread>
#include <utility>

void task(int id) {
    std::cout << "Task " << id << " executing\n";
}

void move_semantics_example() {
    std::cout << "=== Thread Move Semantics ===\n\n";

    // Create thread
    std::thread t1(task, 1);
    std::cout << "t1 created, joinable: " << t1.joinable() << "\n";

    // Move to another thread object
    std::thread t2 = std::move(t1);
    std::cout << "After move to t2:\n";
    std::cout << "  t1 joinable: " << t1.joinable() << "\n";
    std::cout << "  t2 joinable: " << t2.joinable() << "\n\n";

    // Move into vector
    std::thread t3(task, 3);
    std::thread t4 = std::move(t3);

    t2.join();
    t4.join();
}

int main() {
    move_semantics_example();
    return 0;
}
```

### Returning Threads from Functions

```cpp
#include <iostream>
#include <thread>
#include <vector>

std::thread create_thread(int id) {
    return std::thread([id]() {
        std::cout << "Thread " << id << " created in function\n";
    });
}

void factory_pattern() {
    std::cout << "=== Thread Factory Pattern ===\n\n";

    std::vector<std::thread> threads;

    // Create threads via factory function
    for (int i = 0; i < 5; ++i) {
        threads.push_back(create_thread(i));
    }

    // Join all
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\n";
}

int main() {
    factory_pattern();
    return 0;
}
```

### Thread Manager Class

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

class ThreadManager {
public:
    ~ThreadManager() {
        join_all();
    }

    void add_thread(std::thread t) {
        threads_.push_back(std::move(t));
    }

    template<typename Func, typename... Args>
    void create_thread(Func&& func, Args&&... args) {
        threads_.emplace_back(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    void join_all() {
        for (auto& t : threads_) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    size_t thread_count() const {
        return threads_.size();
    }

private:
    std::vector<std::thread> threads_;
};

void thread_manager_example() {
    std::cout << "=== Thread Manager Example ===\n\n";

    ThreadManager manager;

    // Add threads
    for (int i = 0; i < 5; ++i) {
        manager.create_thread([i]() {
            std::cout << "Managed thread " << i << " executing\n";
        });
    }

    std::cout << "Created " << manager.thread_count() << " threads\n";

    // Automatically joins all threads in destructor
    std::cout << "Manager going out of scope...\n\n";
}

int main() {
    thread_manager_example();
    return 0;
}
```

---

## 6. Thread-Local Storage

### thread_local Keyword

```cpp
#include <iostream>
#include <thread>
#include <vector>

// Thread-local variable
thread_local int thread_counter = 0;

// Regular global variable (shared)
int global_counter = 0;

void increment_counters(int thread_id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        ++thread_counter;  // Each thread has its own copy
        ++global_counter;  // Shared (unsafe without synchronization!)
    }

    std::cout << "Thread " << thread_id
              << " - thread_local: " << thread_counter
              << ", global: " << global_counter << "\n";
}

void thread_local_example() {
    std::cout << "=== thread_local Example ===\n\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(increment_counters, i, 1000);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nMain thread - thread_counter: " << thread_counter << "\n";
    std::cout << "Note: Each thread has its own thread_counter starting at 0\n\n";
}

int main() {
    thread_local_example();
    return 0;
}
```

### Thread-Local Storage with Classes

```cpp
#include <iostream>
#include <thread>
#include <string>
#include <sstream>

class ThreadContext {
public:
    ThreadContext() {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        thread_id_ = oss.str();
        std::cout << "ThreadContext created for thread " << thread_id_ << "\n";
    }

    ~ThreadContext() {
        std::cout << "ThreadContext destroyed for thread " << thread_id_ << "\n";
    }

    void log(const std::string& message) {
        std::cout << "[Thread " << thread_id_ << "] " << message << "\n";
    }

private:
    std::string thread_id_;
};

thread_local ThreadContext context;

void worker(int id) {
    context.log("Starting work");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    context.log("Finishing work");
}

void thread_local_class_example() {
    std::cout << "=== Thread-Local Class Example ===\n\n";

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);

    t1.join();
    t2.join();

    std::cout << "\n";
}

int main() {
    thread_local_class_example();
    return 0;
}
```

---

## 7. Exception Handling in Threads

### Exceptions in Threads

```cpp
#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>

void throwing_function() {
    throw std::runtime_error("Thread exception!");
}

void unsafe_exception_handling() {
    std::cout << "=== Unsafe Exception Handling ===\n\n";

    try {
        std::thread t(throwing_function);
        t.join();
    } catch (const std::exception& e) {
        // This will NOT catch the exception from the thread!
        std::cout << "Caught: " << e.what() << "\n";
    }

    std::cout << "This won't print - program will terminate!\n";
}

// DON'T RUN unsafe_exception_handling() - it will terminate!
```

### Safe Exception Handling

```cpp
#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>
#include <future>

void may_throw_function(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("Intentional exception");
    }
    std::cout << "Function completed successfully\n";
}

// Approach 1: Catch inside thread
void safe_thread_function(bool should_throw) {
    try {
        may_throw_function(should_throw);
    } catch (const std::exception& e) {
        std::cout << "Caught in thread: " << e.what() << "\n";
    }
}

void safe_exception_handling() {
    std::cout << "=== Safe Exception Handling ===\n\n";

    std::thread t1(safe_thread_function, true);
    std::thread t2(safe_thread_function, false);

    t1.join();
    t2.join();

    std::cout << "\n";
}

int main() {
    safe_exception_handling();
    return 0;
}
```

### Exception Propagation with std::exception_ptr

```cpp
#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>

void worker_with_exception_ptr(std::exception_ptr& eptr) {
    try {
        throw std::runtime_error("Exception from worker");
    } catch (...) {
        eptr = std::current_exception();
    }
}

void exception_ptr_example() {
    std::cout << "=== Exception Propagation with exception_ptr ===\n\n";

    std::exception_ptr eptr;
    std::thread t(worker_with_exception_ptr, std::ref(eptr));
    t.join();

    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            std::cout << "Caught propagated exception: " << e.what() << "\n";
        }
    }

    std::cout << "\n";
}

int main() {
    exception_ptr_example();
    return 0;
}
```

---

## 8. Graceful Thread Termination

### Using Atomic Flag

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

class Worker {
public:
    void start() {
        stop_flag_ = false;
        thread_ = std::thread(&Worker::work, this);
    }

    void stop() {
        stop_flag_ = true;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    void work() {
        int iteration = 0;
        while (!stop_flag_) {
            std::cout << "Working... iteration " << ++iteration << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        std::cout << "Worker stopped gracefully\n";
    }

    std::thread thread_;
    std::atomic<bool> stop_flag_{false};
};

void graceful_shutdown_example() {
    std::cout << "=== Graceful Thread Termination ===\n\n";

    Worker worker;
    worker.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Requesting stop...\n";
    worker.stop();
    std::cout << "Worker stopped\n\n";
}

int main() {
    graceful_shutdown_example();
    return 0;
}
```

### C++20 std::stop_token

```cpp
#include <iostream>
#include <thread>
#include <chrono>

#if __cplusplus >= 202002L
#include <stop_token>

void interruptible_worker(std::stop_token stoken) {
    int iteration = 0;
    while (!stoken.stop_requested()) {
        std::cout << "Working... iteration " << ++iteration << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Worker stopped via stop_token\n";
}

void stop_token_example() {
    std::cout << "=== C++20 stop_token Example ===\n\n";

    std::jthread worker(interruptible_worker);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Requesting stop...\n";
    worker.request_stop();

    // jthread automatically joins
}
#endif

int main() {
#if __cplusplus >= 202002L
    stop_token_example();
#else
    std::cout << "C++20 required for stop_token\n";
#endif
    return 0;
}
```

---

## 9. Exercises

### Exercise 1: Thread Lifecycle Analyzer
Create a program that logs all state transitions of a thread.

### Exercise 2: Safe Thread Container
Implement a container class that safely manages multiple threads.

### Exercise 3: Thread Pool with Graceful Shutdown
Build a thread pool that can be gracefully shut down.

### Exercise 4: Exception-Safe Parallel Map
Implement a parallel map function with proper exception handling.

---

## Key Takeaways

1. Threads have distinct lifecycle states: created, joinable, finished, joined/detached
2. Always call `join()` or `detach()` before thread object destruction
3. `joinable()` returns true if thread is associated with active execution
4. Use RAII (ThreadGuard, ScopedThread) for exception-safe thread management
5. C++20's `std::jthread` automatically joins in destructor
6. Thread ownership can be transferred using move semantics
7. `thread_local` provides separate storage for each thread
8. Exceptions in threads must be caught within the thread or propagated via `std::exception_ptr`
9. Graceful termination requires cooperative mechanisms (atomic flags, stop_token)
10. Never let a joinable thread be destroyed - use RAII patterns

---

## Summary Checklist

Before moving to Lesson 44, ensure you can:
- [ ] Explain the thread lifecycle and states
- [ ] Properly use join() and detach()
- [ ] Check joinable() state before operations
- [ ] Implement RAII thread management
- [ ] Transfer thread ownership safely
- [ ] Use thread-local storage
- [ ] Handle exceptions in threads
- [ ] Implement graceful thread termination
- [ ] Avoid std::terminate() with proper thread management
- [ ] Use modern C++20 features when available

**Next Lesson**: Race Conditions and Data Races - understanding and preventing concurrency bugs.

---

**Estimated Word Count**: ~4,900 words
