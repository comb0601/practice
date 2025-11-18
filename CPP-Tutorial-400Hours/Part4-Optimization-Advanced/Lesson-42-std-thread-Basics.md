# Lesson 42: std::thread Basics

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to std::thread
2. Creating Threads
3. Thread Function Signatures
4. Passing Arguments to Threads
5. Thread Ownership and Move Semantics
6. Thread Identifiers
7. Hardware Concurrency
8. Thread Sleep and Yield
9. Exception Safety with Threads
10. Common Patterns and Idioms
11. Practical Examples
12. Exercises

---

## 1. Introduction to std::thread

C++11 introduced the `<thread>` header, providing native threading support in the C++ standard library. Prior to C++11, threading required platform-specific libraries (pthreads on UNIX, Windows threads on Windows).

### Why std::thread?

**Benefits**:
- **Portable**: Write once, run on any platform
- **Type-safe**: Compile-time type checking
- **Modern C++**: Integrates with lambdas, move semantics, and RAII
- **Standard**: No external dependencies

**Basic Structure**:
```cpp
#include <thread>

std::thread t(function_to_execute);
t.join(); // Wait for completion
```

### Your First Thread

```cpp
#include <iostream>
#include <thread>

void hello_from_thread() {
    std::cout << "Hello from thread!\n";
}

int main() {
    std::cout << "Hello from main!\n";

    // Create and launch a thread
    std::thread t(hello_from_thread);

    // Wait for the thread to finish
    t.join();

    std::cout << "Thread has finished!\n";
    return 0;
}
```

**Output** (order may vary):
```
Hello from main!
Hello from thread!
Thread has finished!
```

---

## 2. Creating Threads

### Different Ways to Create Threads

#### 1. Function Pointer

```cpp
#include <iostream>
#include <thread>

void print_message() {
    std::cout << "Thread function executed\n";
}

int main() {
    std::thread t(print_message);
    t.join();
    return 0;
}
```

#### 2. Lambda Expression

```cpp
#include <iostream>
#include <thread>

int main() {
    std::thread t([]() {
        std::cout << "Lambda thread executed\n";
    });
    t.join();
    return 0;
}
```

#### 3. Function Object (Functor)

```cpp
#include <iostream>
#include <thread>

class ThreadTask {
public:
    void operator()() const {
        std::cout << "Functor thread executed\n";
    }
};

int main() {
    ThreadTask task;
    std::thread t(task);
    t.join();
    return 0;
}
```

#### 4. Member Function

```cpp
#include <iostream>
#include <thread>
#include <string>

class Worker {
public:
    void do_work(const std::string& message) {
        std::cout << "Worker: " << message << "\n";
    }
};

int main() {
    Worker worker;
    std::thread t(&Worker::do_work, &worker, "Hello from member function");
    t.join();
    return 0;
}
```

#### 5. Static Member Function

```cpp
#include <iostream>
#include <thread>

class Utility {
public:
    static void static_task() {
        std::cout << "Static member function executed\n";
    }
};

int main() {
    std::thread t(&Utility::static_task);
    t.join();
    return 0;
}
```

### Comprehensive Example: All Creation Methods

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <string>

// 1. Regular function
void regular_function(int id) {
    std::cout << "Regular function, thread " << id << "\n";
}

// 2. Function object
class Functor {
public:
    void operator()(int id) const {
        std::cout << "Functor, thread " << id << "\n";
    }
};

// 3. Class with member function
class Worker {
public:
    void work(int id) {
        std::cout << "Member function, thread " << id << "\n";
    }

    static void static_work(int id) {
        std::cout << "Static member function, thread " << id << "\n";
    }
};

int main() {
    std::cout << "=== Different Thread Creation Methods ===\n\n";

    // Method 1: Regular function
    std::thread t1(regular_function, 1);

    // Method 2: Lambda
    std::thread t2([](int id) {
        std::cout << "Lambda, thread " << id << "\n";
    }, 2);

    // Method 3: Functor
    Functor functor;
    std::thread t3(functor, 3);

    // Method 4: Member function
    Worker worker;
    std::thread t4(&Worker::work, &worker, 4);

    // Method 5: Static member function
    std::thread t5(&Worker::static_work, 5);

    // Wait for all threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
```

---

## 3. Thread Function Signatures

### Return Values

**Important**: Thread functions **cannot return values directly**. Use:
- Pass-by-reference parameters
- Shared variables with synchronization
- std::promise/std::future (covered in later lessons)

```cpp
#include <iostream>
#include <thread>
#include <vector>

// WRONG: Cannot get return value this way
int wrong_approach() {
    return 42; // This value is discarded
}

// CORRECT: Use reference parameter
void correct_approach(int& result) {
    result = 42;
}

// CORRECT: Use shared variable
int shared_result = 0;
void shared_variable_approach() {
    shared_result = 42;
}

int main() {
    std::cout << "=== Thread Return Values ===\n\n";

    // Approach 1: Reference parameter
    int result1 = 0;
    std::thread t1(correct_approach, std::ref(result1));
    t1.join();
    std::cout << "Result via reference: " << result1 << "\n";

    // Approach 2: Shared variable
    std::thread t2(shared_variable_approach);
    t2.join();
    std::cout << "Result via shared variable: " << shared_result << "\n";

    return 0;
}
```

### Accepting Parameters

```cpp
#include <iostream>
#include <thread>
#include <string>
#include <vector>

// Various parameter types
void thread_with_params(int id, const std::string& message, double value) {
    std::cout << "Thread " << id << ": " << message << ", value=" << value << "\n";
}

void thread_with_vector(const std::vector<int>& data) {
    std::cout << "Vector size: " << data.size() << "\n";
    std::cout << "Elements: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Thread Parameters ===\n\n";

    // Basic types
    std::thread t1(thread_with_params, 1, "Hello", 3.14);
    t1.join();

    // Complex types
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::thread t2(thread_with_vector, std::ref(data));
    t2.join();

    return 0;
}
```

---

## 4. Passing Arguments to Threads

### By Value

Arguments are **copied** by default:

```cpp
#include <iostream>
#include <thread>
#include <string>

void print_by_value(std::string message) {
    message += " (modified in thread)";
    std::cout << "Inside thread: " << message << "\n";
}

int main() {
    std::string msg = "Original message";

    std::thread t(print_by_value, msg);
    t.join();

    std::cout << "After thread: " << msg << "\n"; // Still "Original message"

    return 0;
}
```

**Output**:
```
Inside thread: Original message (modified in thread)
After thread: Original message
```

### By Reference

Use `std::ref()` to pass by reference:

```cpp
#include <iostream>
#include <thread>
#include <string>

void modify_by_reference(std::string& message) {
    message += " (modified in thread)";
    std::cout << "Inside thread: " << message << "\n";
}

int main() {
    std::string msg = "Original message";

    std::thread t(modify_by_reference, std::ref(msg));
    t.join();

    std::cout << "After thread: " << msg << "\n"; // Modified!

    return 0;
}
```

**Output**:
```
Inside thread: Original message (modified in thread)
After thread: Original message (modified in thread)
```

### By Move

Efficient for large objects:

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <memory>

void process_large_data(std::vector<int> data) {
    std::cout << "Processing " << data.size() << " elements\n";
}

void process_unique_ptr(std::unique_ptr<int> ptr) {
    std::cout << "Value: " << *ptr << "\n";
}

int main() {
    std::cout << "=== Moving Arguments ===\n\n";

    // Move large vector
    std::vector<int> large_data(1000000, 42);
    std::thread t1(process_large_data, std::move(large_data));
    t1.join();
    // large_data is now empty

    // Move unique_ptr
    std::unique_ptr<int> ptr = std::make_unique<int>(100);
    std::thread t2(process_unique_ptr, std::move(ptr));
    t2.join();
    // ptr is now nullptr

    return 0;
}
```

### Parameter Pitfalls

```cpp
#include <iostream>
#include <thread>
#include <string>

void dangerous_function(const std::string& str) {
    std::cout << str << "\n";
}

void safe_function(std::string str) {
    std::cout << str << "\n";
}

int main() {
    std::cout << "=== Parameter Safety ===\n\n";

    // DANGEROUS: Pointer to local variable
    {
        char buffer[] = "Hello";
        // BAD: buffer might be destroyed before thread accesses it
        // std::thread t(dangerous_function, buffer);

        // GOOD: Explicitly construct string
        std::thread t(dangerous_function, std::string(buffer));
        t.join();
    }

    // SAFE: Pass by value
    {
        std::string msg = "Safe message";
        std::thread t(safe_function, msg);
        t.join();
    }

    return 0;
}
```

### Comprehensive Argument Passing Example

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <memory>

// By value
void process_by_value(int x, std::string s) {
    std::cout << "By value: x=" << x << ", s=" << s << "\n";
}

// By reference
void process_by_reference(int& x, std::string& s) {
    x *= 2;
    s += " modified";
    std::cout << "By reference: x=" << x << ", s=" << s << "\n";
}

// By const reference
void process_by_const_ref(const std::vector<int>& vec) {
    std::cout << "By const ref: size=" << vec.size() << "\n";
}

// By move
void process_by_move(std::unique_ptr<int> ptr) {
    std::cout << "By move: value=" << *ptr << "\n";
}

int main() {
    std::cout << "=== Comprehensive Argument Passing ===\n\n";

    // By value
    {
        int x = 10;
        std::string s = "hello";
        std::thread t(process_by_value, x, s);
        t.join();
        std::cout << "After thread: x=" << x << ", s=" << s << "\n\n";
    }

    // By reference
    {
        int x = 10;
        std::string s = "hello";
        std::thread t(process_by_reference, std::ref(x), std::ref(s));
        t.join();
        std::cout << "After thread: x=" << x << ", s=" << s << "\n\n";
    }

    // By const reference
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::thread t(process_by_const_ref, std::cref(vec));
        t.join();
        std::cout << "\n";
    }

    // By move
    {
        auto ptr = std::make_unique<int>(42);
        std::thread t(process_by_move, std::move(ptr));
        t.join();
        std::cout << "ptr is now: " << (ptr ? "valid" : "null") << "\n\n";
    }

    return 0;
}
```

---

## 5. Thread Ownership and Move Semantics

### Thread Ownership

`std::thread` is **not copyable** but **movable**:

```cpp
#include <iostream>
#include <thread>

void worker() {
    std::cout << "Worker thread\n";
}

int main() {
    std::thread t1(worker);

    // ILLEGAL: Cannot copy threads
    // std::thread t2 = t1; // Compilation error!

    // LEGAL: Can move threads
    std::thread t2 = std::move(t1);

    // t1 is now empty (not associated with any thread)
    // t2 now owns the thread

    if (t1.joinable()) {
        std::cout << "t1 is joinable\n";
    } else {
        std::cout << "t1 is NOT joinable\n"; // This will execute
    }

    if (t2.joinable()) {
        std::cout << "t2 is joinable\n"; // This will execute
        t2.join();
    }

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
        std::cout << "Thread " << id << " executing\n";
    });
}

int main() {
    std::vector<std::thread> threads;

    // Create multiple threads
    for (int i = 0; i < 5; ++i) {
        threads.push_back(create_thread(i));
    }

    // Wait for all
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```

### Storing Threads in Containers

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

void task(int id) {
    std::cout << "Task " << id << " running\n";
}

int main() {
    std::cout << "=== Threads in Containers ===\n\n";

    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(task, i);
    }

    std::cout << "Created " << threads.size() << " threads\n";

    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "All threads completed\n";

    return 0;
}
```

---

## 6. Thread Identifiers

### Getting Thread IDs

```cpp
#include <iostream>
#include <thread>
#include <sstream>

void print_thread_id(int task_id) {
    std::ostringstream oss;
    oss << "Task " << task_id << " running on thread " << std::this_thread::get_id() << "\n";
    std::cout << oss.str();
}

int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n\n";

    std::thread t1(print_thread_id, 1);
    std::thread t2(print_thread_id, 2);
    std::thread t3(print_thread_id, 3);

    std::cout << "t1 thread ID: " << t1.get_id() << "\n";
    std::cout << "t2 thread ID: " << t2.get_id() << "\n";
    std::cout << "t3 thread ID: " << t3.get_id() << "\n\n";

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
```

### Using Thread IDs as Map Keys

```cpp
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <mutex>

std::mutex map_mutex;
std::map<std::thread::id, int> thread_results;

void worker(int id, int value) {
    // Simulate work
    int result = value * value;

    // Store result with thread ID as key
    std::lock_guard<std::mutex> lock(map_mutex);
    thread_results[std::this_thread::get_id()] = result;
}

int main() {
    std::cout << "=== Thread IDs as Map Keys ===\n\n";

    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, i, i + 1);
    }

    // Wait for all
    for (auto& t : threads) {
        t.join();
    }

    // Display results
    std::cout << "Results by thread ID:\n";
    for (const auto& [tid, result] : thread_results) {
        std::cout << "Thread " << tid << ": " << result << "\n";
    }

    return 0;
}
```

---

## 7. Hardware Concurrency

### Detecting Available Cores

```cpp
#include <iostream>
#include <thread>
#include <vector>

void cpu_bound_task(int id) {
    volatile long long sum = 0;
    for (long long i = 0; i < 100000000; ++i) {
        sum += i;
    }
    std::cout << "Task " << id << " completed\n";
}

int main() {
    unsigned int num_cores = std::thread::hardware_concurrency();

    std::cout << "=== Hardware Concurrency ===\n";
    std::cout << "Available hardware threads: " << num_cores << "\n\n";

    if (num_cores == 0) {
        std::cout << "Cannot determine hardware concurrency\n";
        num_cores = 2; // Fallback
    }

    std::cout << "Creating " << num_cores << " threads...\n\n";

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < num_cores; ++i) {
        threads.emplace_back(cpu_bound_task, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nAll threads completed\n";

    return 0;
}
```

### Optimal Thread Count

```cpp
#include <iostream>
#include <thread>
#include <algorithm>

size_t optimal_thread_count(size_t min_per_thread, size_t max_threads, size_t data_size) {
    const unsigned int hardware_threads = std::thread::hardware_concurrency();

    // Maximum threads based on hardware
    const size_t max_by_hardware = hardware_threads != 0 ? hardware_threads : 2;

    // Maximum threads based on data size
    const size_t max_by_data = (data_size + min_per_thread - 1) / min_per_thread;

    // Take minimum of all constraints
    return std::min({max_by_hardware, max_by_data, max_threads});
}

int main() {
    std::cout << "=== Optimal Thread Count ===\n\n";

    size_t data_sizes[] = {100, 1000, 10000, 100000, 1000000};

    for (size_t size : data_sizes) {
        size_t threads = optimal_thread_count(1000, 16, size);
        std::cout << "Data size: " << size << " → Optimal threads: " << threads << "\n";
    }

    return 0;
}
```

---

## 8. Thread Sleep and Yield

### std::this_thread::sleep_for

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void sleep_demo() {
    std::cout << "Starting task...\n";

    // Sleep for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Slept for 1 second\n";

    // Sleep for 500 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Slept for 500ms\n";

    // Sleep for 100 microseconds
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    std::cout << "Slept for 100us\n";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    sleep_demo();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total time: " << duration.count() << "ms\n";

    return 0;
}
```

### std::this_thread::sleep_until

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void sleep_until_demo() {
    auto now = std::chrono::system_clock::now();
    auto wake_up = now + std::chrono::seconds(2);

    std::cout << "Going to sleep...\n";
    std::this_thread::sleep_until(wake_up);
    std::cout << "Woke up!\n";
}

int main() {
    sleep_until_demo();
    return 0;
}
```

### std::this_thread::yield

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<bool> ready{false};

void spinner(int id) {
    while (!ready.load()) {
        // Give up time slice to other threads
        std::this_thread::yield();
    }
    std::cout << "Thread " << id << " proceeding\n";
}

void yielding_example() {
    std::cout << "=== Thread Yield Example ===\n\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(spinner, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Setting ready flag...\n";
    ready.store(true);

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    yielding_example();
    return 0;
}
```

---

## 9. Exception Safety with Threads

### The Problem

```cpp
#include <iostream>
#include <thread>
#include <stdexcept>

void may_throw() {
    throw std::runtime_error("Thread exception");
}

void dangerous_code() {
    std::thread t(may_throw);

    // If exception thrown here, t.join() is never called!
    // This will call std::terminate()

    t.join();
}

// DON'T RUN THIS - it will terminate the program
```

### RAII Solution: Thread Guard

```cpp
#include <iostream>
#include <thread>
#include <stdexcept>

class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& t) : thread_(t) {}

    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

private:
    std::thread& thread_;
};

void task() {
    std::cout << "Thread task executing\n";
}

void safe_code() {
    std::thread t(task);
    ThreadGuard guard(t);

    // Even if exception is thrown here, ~ThreadGuard() will join the thread
    // throw std::runtime_error("Exception after thread creation");

    std::cout << "Function completed normally\n";
}

int main() {
    std::cout << "=== Exception-Safe Thread Management ===\n\n";

    try {
        safe_code();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}
```

### Modern Approach: std::jthread (C++20)

```cpp
#include <iostream>
#include <thread>

#if __cplusplus >= 202002L
#include <stop_token>

void modern_approach() {
    std::cout << "=== C++20 std::jthread ===\n\n";

    // std::jthread automatically joins in destructor
    std::jthread t([]() {
        std::cout << "Thread executing\n";
    });

    // No need to manually join - destructor handles it
}
#endif

int main() {
#if __cplusplus >= 202002L
    modern_approach();
#else
    std::cout << "C++20 required for std::jthread\n";
#endif
    return 0;
}
```

---

## 10. Common Patterns and Idioms

### Pattern 1: Worker Pool

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class SimpleThreadPool {
public:
    SimpleThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this, i]() {
                std::cout << "Worker " << i << " started\n";
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        condition_.wait(lock, [this]() {
                            return stop_ || !tasks_.empty();
                        });

                        if (stop_ && tasks_.empty()) {
                            return;
                        }

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~SimpleThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        condition_.notify_all();

        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.push(std::move(task));
        }
        condition_.notify_one();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

int main() {
    std::cout << "=== Simple Thread Pool ===\n\n";

    SimpleThreadPool pool(4);

    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " executing on thread "
                      << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
```

---

## 11. Exercises

### Exercise 1: Thread Creation
Create a program that launches threads using all five creation methods.

### Exercise 2: Parameter Passing
Write a function that correctly passes various types of parameters to threads.

### Exercise 3: Thread Manager
Implement a ThreadManager class that manages thread lifecycle.

### Exercise 4: Performance Measurement
Measure the overhead of creating threads vs. reusing them.

---

## Key Takeaways

1. `std::thread` provides portable, type-safe threading in C++11+
2. Threads can be created from functions, lambdas, functors, and member functions
3. Arguments are copied by default; use `std::ref()` for references
4. Threads are movable but not copyable
5. Always join or detach threads before destruction
6. Use `std::thread::hardware_concurrency()` to detect available cores
7. `std::this_thread` provides utilities for the current thread
8. RAII (ThreadGuard) ensures exception-safe thread management
9. Thread IDs can be used for identification and as map keys
10. Modern C++ (C++20) provides `std::jthread` with automatic joining

---

## Summary Checklist

Before moving to Lesson 43, ensure you can:
- [ ] Create threads using different callable types
- [ ] Pass arguments by value, reference, and move
- [ ] Manage thread ownership with move semantics
- [ ] Use thread identifiers effectively
- [ ] Query hardware concurrency
- [ ] Use sleep_for, sleep_until, and yield
- [ ] Implement exception-safe thread management
- [ ] Store threads in containers
- [ ] Understand the difference between join and detach
- [ ] Create simple concurrent programs

**Next Lesson**: Thread Management and Lifecycle - deep dive into join, detach, and thread states.

---

**Estimated Word Count**: ~5,200 words
