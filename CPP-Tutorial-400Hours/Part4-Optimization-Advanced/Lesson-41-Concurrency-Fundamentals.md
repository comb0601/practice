# Lesson 41: Concurrency Fundamentals

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Concurrency
2. Concurrency vs. Parallelism
3. Why Concurrency Matters
4. The Hardware Reality: Multi-Core Processors
5. Concurrency Models
6. Challenges in Concurrent Programming
7. Amdahl's Law and Scalability
8. Memory Models and Visibility
9. Practical Examples
10. Best Practices
11. Exercises

---

## 1. Introduction to Concurrency

Concurrency is one of the most powerful and challenging aspects of modern programming. At its core, **concurrency** is about dealing with multiple things at once, enabling programs to make progress on multiple tasks during overlapping time periods.

### What is Concurrency?

Concurrency is a property of systems where multiple computational tasks can be in progress at the same time. These tasks may not necessarily execute simultaneously (that's parallelism), but they can be interleaved in a way that allows progress on all tasks.

### Historical Context

Before multi-core processors became ubiquitous, concurrency was primarily used for:
- **I/O Operations**: While waiting for disk or network I/O, the CPU could work on other tasks
- **Responsiveness**: GUI applications could remain responsive while performing background work
- **Server Applications**: Handling multiple client connections simultaneously

With modern multi-core processors, concurrency has become essential for:
- **Performance**: Utilizing all available CPU cores
- **Throughput**: Processing more work in less time
- **Efficiency**: Better resource utilization

### Simple Concurrency Example

Let's start with a simple example to understand the concept:

```cpp
#include <iostream>
#include <thread>
#include <chrono>

// Sequential version - tasks run one after another
void sequential_example() {
    std::cout << "=== Sequential Execution ===\n";
    auto start = std::chrono::high_resolution_clock::now();

    // Task 1: Count to 5
    std::cout << "Task 1 starting...\n";
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 1: " << i << "\n";
    }

    // Task 2: Count to 5
    std::cout << "Task 2 starting...\n";
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 2: " << i << "\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total time: " << duration.count() << "ms\n\n";
}

// Concurrent version - tasks can make progress simultaneously
void task1() {
    std::cout << "Task 1 starting...\n";
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 1: " << i << "\n";
    }
}

void task2() {
    std::cout << "Task 2 starting...\n";
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 2: " << i << "\n";
    }
}

void concurrent_example() {
    std::cout << "=== Concurrent Execution ===\n";
    auto start = std::chrono::high_resolution_clock::now();

    // Launch both tasks concurrently
    std::thread t1(task1);
    std::thread t2(task2);

    // Wait for both to complete
    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total time: " << duration.count() << "ms\n\n";
}

int main() {
    sequential_example();
    concurrent_example();
    return 0;
}
```

**Output Analysis**:
- Sequential: Takes ~1000ms (500ms + 500ms)
- Concurrent: Takes ~500ms (both run in parallel)

---

## 2. Concurrency vs. Parallelism

This is one of the most misunderstood concepts in concurrent programming. Let's clarify:

### Concurrency

**Concurrency** is about **structure** - it's a way of structuring your program to deal with multiple things at once. It's about composition of independently executing processes.

**Key Characteristics**:
- Tasks can start, run, and complete in overlapping time periods
- May or may not run simultaneously
- Can be achieved on a single-core processor through time-slicing
- Focus: **Managing multiple tasks**

### Parallelism

**Parallelism** is about **execution** - it's about doing multiple things simultaneously. It's about simultaneous execution of multiple computations.

**Key Characteristics**:
- Tasks run at exactly the same time
- Requires multiple processing units (cores)
- Subset of concurrency
- Focus: **Performance through simultaneous execution**

### Visual Representation

```
Single-Core (Concurrency without Parallelism):
Time →
Core 1: [Task A][Task B][Task A][Task B][Task A][Task B]
         ↑ Context switching between tasks

Multi-Core (True Parallelism):
Time →
Core 1: [Task A...................]
Core 2: [Task B...................]
         ↑ Tasks running simultaneously
```

### Practical Example

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// CPU-intensive task (benefits from parallelism)
unsigned long long compute_intensive_task(int id, unsigned long long limit) {
    unsigned long long sum = 0;
    for (unsigned long long i = 0; i < limit; ++i) {
        sum += i * i;
    }
    return sum;
}

// I/O-intensive task (benefits from concurrency)
void io_intensive_task(int id) {
    std::cout << "Thread " << id << " starting I/O operation\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate I/O wait
    std::cout << "Thread " << id << " completed I/O operation\n";
}

void demonstrate_parallelism() {
    std::cout << "=== Parallelism Demo (CPU-Intensive) ===\n";
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " cores\n\n";

    const unsigned long long limit = 100000000;

    // Sequential execution
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long long result1 = compute_intensive_task(1, limit);
    unsigned long long result2 = compute_intensive_task(2, limit);
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Sequential execution time: " << sequential_time.count() << "ms\n";

    // Parallel execution
    start = std::chrono::high_resolution_clock::now();
    unsigned long long res1 = 0, res2 = 0;
    std::thread t1([&]() { res1 = compute_intensive_task(1, limit); });
    std::thread t2([&]() { res2 = compute_intensive_task(2, limit); });
    t1.join();
    t2.join();
    end = std::chrono::high_resolution_clock::now();
    auto parallel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Parallel execution time: " << parallel_time.count() << "ms\n";
    std::cout << "Speedup: " << (double)sequential_time.count() / parallel_time.count() << "x\n\n";
}

void demonstrate_concurrency() {
    std::cout << "=== Concurrency Demo (I/O-Intensive) ===\n";

    // Sequential I/O operations
    auto start = std::chrono::high_resolution_clock::now();
    io_intensive_task(1);
    io_intensive_task(2);
    io_intensive_task(3);
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Sequential I/O time: " << sequential_time.count() << "ms\n\n";

    // Concurrent I/O operations
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(io_intensive_task, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    end = std::chrono::high_resolution_clock::now();
    auto concurrent_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Concurrent I/O time: " << concurrent_time.count() << "ms\n";
    std::cout << "Time saved: " << sequential_time.count() - concurrent_time.count() << "ms\n\n";
}

int main() {
    demonstrate_parallelism();
    demonstrate_concurrency();
    return 0;
}
```

---

## 3. Why Concurrency Matters

### The Free Lunch is Over

For decades, software got faster automatically because CPU clock speeds doubled every 18-24 months (Moore's Law). Around 2004, this "free lunch" ended due to physical limitations (heat dissipation, power consumption).

**The Paradigm Shift**:
- **Before 2004**: Wait for faster CPUs
- **After 2004**: Use multiple cores

### Performance Benefits

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <chrono>
#include <algorithm>

// Calculate sum of large array
long long sequential_sum(const std::vector<int>& data) {
    return std::accumulate(data.begin(), data.end(), 0LL);
}

// Parallel sum using multiple threads
long long parallel_sum(const std::vector<int>& data, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(num_threads, 0);

    size_t chunk_size = data.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? data.size() : (i + 1) * chunk_size;

        threads.emplace_back([&data, &partial_sums, i, start, end]() {
            partial_sums[i] = std::accumulate(data.begin() + start, data.begin() + end, 0LL);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
}

void benchmark_performance() {
    const size_t SIZE = 100000000; // 100 million elements
    std::vector<int> data(SIZE);

    // Initialize with values
    for (size_t i = 0; i < SIZE; ++i) {
        data[i] = i % 100;
    }

    std::cout << "Array size: " << SIZE << " elements\n";
    std::cout << "Hardware cores: " << std::thread::hardware_concurrency() << "\n\n";

    // Sequential benchmark
    auto start = std::chrono::high_resolution_clock::now();
    long long seq_result = sequential_sum(data);
    auto end = std::chrono::high_resolution_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Sequential sum: " << seq_result << "\n";
    std::cout << "Sequential time: " << seq_time.count() << "ms\n\n";

    // Parallel benchmark with different thread counts
    for (int num_threads : {2, 4, 8}) {
        start = std::chrono::high_resolution_clock::now();
        long long par_result = parallel_sum(data, num_threads);
        end = std::chrono::high_resolution_clock::now();
        auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Parallel sum (" << num_threads << " threads): " << par_result << "\n";
        std::cout << "Parallel time: " << par_time.count() << "ms\n";
        std::cout << "Speedup: " << (double)seq_time.count() / par_time.count() << "x\n";
        std::cout << "Efficiency: " << ((double)seq_time.count() / par_time.count() / num_threads * 100) << "%\n\n";
    }
}

int main() {
    benchmark_performance();
    return 0;
}
```

### Real-World Applications

1. **Web Servers**: Handle thousands of concurrent connections
2. **Video Games**: Render, physics, AI, audio all running concurrently
3. **Video Encoding**: Process frames in parallel
4. **Machine Learning**: Train models using data parallelism
5. **Financial Trading**: Process market data and execute trades concurrently
6. **Database Systems**: Handle multiple queries simultaneously

---

## 4. The Hardware Reality: Multi-Core Processors

Understanding the hardware is crucial for effective concurrent programming.

### Modern CPU Architecture

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

void print_hardware_info() {
    std::cout << "=== Hardware Concurrency Information ===\n\n";

    // Number of concurrent threads supported
    unsigned int num_threads = std::thread::hardware_concurrency();

    std::cout << "Hardware concurrency: " << num_threads << " threads\n";
    std::cout << "This typically represents:\n";
    std::cout << "  - Number of physical cores × threads per core\n";
    std::cout << "  - With hyperthreading: 2 threads per physical core\n";
    std::cout << "  - Without hyperthreading: 1 thread per physical core\n\n";

    if (num_threads == 0) {
        std::cout << "Note: Value not computable or not well-defined\n";
    }

    // Demonstrate thread affinity
    std::cout << "Current thread ID: " << std::this_thread::get_id() << "\n\n";
}

// Demonstrate cache effects
void demonstrate_cache_coherence() {
    std::cout << "=== Cache Coherence Demo ===\n\n";

    const int SIZE = 10000000;
    std::vector<int> data(SIZE, 1);

    // False sharing demonstration
    struct AlignedCounter {
        alignas(64) int counter; // Cache line is typically 64 bytes
    };

    std::cout << "Understanding cache lines is crucial for performance\n";
    std::cout << "Size of int: " << sizeof(int) << " bytes\n";
    std::cout << "Size of aligned counter: " << sizeof(AlignedCounter) << " bytes\n";
    std::cout << "Typical cache line size: 64 bytes\n\n";
}

int main() {
    print_hardware_info();
    demonstrate_cache_coherence();
    return 0;
}
```

### Memory Hierarchy

```
CPU Registers (1 cycle)
    ↓
L1 Cache (3-4 cycles) - 32-64 KB per core
    ↓
L2 Cache (10-20 cycles) - 256-512 KB per core
    ↓
L3 Cache (40-75 cycles) - 8-32 MB shared
    ↓
Main Memory (100-300 cycles) - GBs
    ↓
SSD/Disk (10,000-1,000,000 cycles)
```

**Implications for Concurrent Programming**:
- Data shared between threads may need cache synchronization
- False sharing can severely degrade performance
- Locality matters even more in concurrent code

---

## 5. Concurrency Models

### Thread-Based Concurrency

The most common model in C++. Each thread represents an independent flow of execution.

```cpp
#include <iostream>
#include <thread>
#include <vector>

void worker(int id) {
    std::cout << "Worker " << id << " on thread " << std::this_thread::get_id() << "\n";
}

void thread_based_model() {
    std::cout << "=== Thread-Based Model ===\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    thread_based_model();
    return 0;
}
```

### Task-Based Concurrency

Higher-level abstraction focusing on tasks rather than threads.

```cpp
#include <iostream>
#include <future>
#include <vector>

int compute_task(int id, int value) {
    return value * value;
}

void task_based_model() {
    std::cout << "\n=== Task-Based Model ===\n";

    std::vector<std::future<int>> futures;

    for (int i = 0; i < 4; ++i) {
        futures.push_back(std::async(std::launch::async, compute_task, i, i + 1));
    }

    for (size_t i = 0; i < futures.size(); ++i) {
        std::cout << "Task " << i << " result: " << futures[i].get() << "\n";
    }
}

int main() {
    task_based_model();
    return 0;
}
```

### Data-Parallel Model

Apply the same operation to multiple pieces of data concurrently.

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

void data_parallel_model() {
    std::cout << "\n=== Data-Parallel Model ===\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> results(data.size());

    auto transform_chunk = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            results[i] = data[i] * data[i];
        }
    };

    const int num_threads = 4;
    std::vector<std::thread> threads;
    size_t chunk_size = data.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? data.size() : (i + 1) * chunk_size;
        threads.emplace_back(transform_chunk, start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Results: ";
    for (int r : results) {
        std::cout << r << " ";
    }
    std::cout << "\n";
}

int main() {
    data_parallel_model();
    return 0;
}
```

---

## 6. Challenges in Concurrent Programming

### Race Conditions

Occur when multiple threads access shared data and at least one modifies it.

```cpp
#include <iostream>
#include <thread>
#include <vector>

// UNSAFE: Race condition example
int unsafe_counter = 0;

void unsafe_increment() {
    for (int i = 0; i < 100000; ++i) {
        unsafe_counter++; // NOT thread-safe!
    }
}

void demonstrate_race_condition() {
    std::cout << "=== Race Condition Demo ===\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(unsafe_increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: " << 10 * 100000 << "\n";
    std::cout << "Actual: " << unsafe_counter << "\n";
    std::cout << "Data race detected!\n\n";
}

int main() {
    demonstrate_race_condition();
    return 0;
}
```

### Deadlock

Two or more threads wait for each other indefinitely.

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutex1, mutex2;

void thread1_deadlock() {
    std::lock_guard<std::mutex> lock1(mutex1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock2(mutex2); // May deadlock
    std::cout << "Thread 1 got both locks\n";
}

void thread2_deadlock() {
    std::lock_guard<std::mutex> lock2(mutex2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock1(mutex1); // May deadlock
    std::cout << "Thread 2 got both locks\n";
}

// Note: This example demonstrates the concept
// Don't run this in production - it will deadlock!
```

### Livelock

Threads continuously change state in response to each other without making progress.

### Starvation

A thread is perpetually denied access to resources.

---

## 7. Amdahl's Law and Scalability

### Amdahl's Law

Describes the theoretical speedup in execution time when using multiple processors.

**Formula**:
```
Speedup = 1 / ((1 - P) + P/N)

Where:
- P = Proportion of program that can be parallelized
- N = Number of processors
- (1 - P) = Sequential portion
```

### Practical Implementation

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

double amdahls_law(double parallel_fraction, int num_processors) {
    double sequential_fraction = 1.0 - parallel_fraction;
    return 1.0 / (sequential_fraction + parallel_fraction / num_processors);
}

void analyze_scalability() {
    std::cout << "=== Amdahl's Law Analysis ===\n\n";
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Speedup for different parallelizable fractions:\n\n";
    std::cout << "Cores\\Parallel%  50%    75%    90%    95%    99%\n";
    std::cout << "------------------------------------------------\n";

    for (int cores : {1, 2, 4, 8, 16, 32, 64}) {
        std::cout << std::setw(3) << cores << " cores    ";
        for (double frac : {0.50, 0.75, 0.90, 0.95, 0.99}) {
            double speedup = amdahls_law(frac, cores);
            std::cout << std::setw(6) << speedup << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nKey Insights:\n";
    std::cout << "- Even with 99% parallelizable code, 64 cores gives ~38x speedup\n";
    std::cout << "- The sequential portion becomes the bottleneck\n";
    std::cout << "- Optimization efforts should focus on the sequential part\n\n";
}

int main() {
    analyze_scalability();
    return 0;
}
```

---

## 8. Memory Models and Visibility

### Memory Visibility

Changes made by one thread may not be immediately visible to other threads.

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Non-atomic variable (unsafe)
bool ready = false;
int data = 0;

// Atomic variable (safe)
std::atomic<bool> atomic_ready{false};

void writer() {
    data = 42;
    ready = true; // Other threads might not see this immediately!
}

void reader() {
    while (!ready) {
        // Busy wait - may loop forever due to caching
    }
    std::cout << "Data: " << data << "\n";
}

void demonstrate_visibility() {
    std::cout << "=== Memory Visibility ===\n";
    std::cout << "Without proper synchronization, threads may not see updates\n";
    std::cout << "Use atomic operations or mutexes for proper synchronization\n\n";
}

int main() {
    demonstrate_visibility();
    return 0;
}
```

---

## 9. Best Practices

### 1. Minimize Shared State

```cpp
// GOOD: No shared state
void independent_task(int id, std::vector<int> local_data) {
    // Process local_data
}

// BAD: Shared mutable state
std::vector<int> shared_data;
void problematic_task(int id) {
    // Modifies shared_data - needs synchronization
}
```

### 2. Use High-Level Abstractions

```cpp
// GOOD: Use std::async
auto future = std::async(std::launch::async, compute_task, args);

// LESS GOOD: Manual thread management
std::thread t(compute_task, args);
t.join();
```

### 3. Follow RAII for Locks

```cpp
// GOOD: RAII with lock_guard
{
    std::lock_guard<std::mutex> lock(mutex);
    // Critical section
} // Automatically unlocks

// BAD: Manual locking
mutex.lock();
// Critical section
mutex.unlock(); // Might forget or exception skips
```

---

## 10. Exercises

### Exercise 1: Hardware Discovery
Write a program that detects and displays your system's concurrency capabilities.

### Exercise 2: Benchmark Comparison
Create benchmarks comparing sequential vs. concurrent execution for various workloads.

### Exercise 3: Amdahl's Law Calculator
Implement an interactive calculator for Amdahl's Law that helps predict speedup.

### Exercise 4: Safe Counter
Implement a thread-safe counter class using different synchronization mechanisms.

---

## Key Takeaways

1. **Concurrency** is about structure; **parallelism** is about execution
2. Modern applications must leverage multiple cores for performance
3. Concurrency introduces challenges: race conditions, deadlocks, and synchronization overhead
4. Amdahl's Law shows that the sequential portion limits scalability
5. Memory visibility is a critical concern in concurrent programming
6. High-level abstractions (tasks, futures) are often better than low-level threads
7. Minimize shared mutable state whenever possible
8. Always measure - concurrent code may be slower if overhead exceeds benefits
9. Understanding hardware (cores, caches, memory) is essential
10. Start with correct, simple code; optimize only when profiling shows the need

---

## Summary Checklist

Before moving to Lesson 42, ensure you understand:
- [ ] The difference between concurrency and parallelism
- [ ] Why concurrency is essential in modern programming
- [ ] Basic hardware concepts (cores, caches, memory hierarchy)
- [ ] Different concurrency models (thread-based, task-based, data-parallel)
- [ ] Common challenges (race conditions, deadlocks, starvation)
- [ ] Amdahl's Law and its implications
- [ ] Memory visibility concerns
- [ ] Best practices for concurrent programming

**Next Lesson**: We'll dive deep into `std::thread` and learn how to create, manage, and synchronize threads effectively.

---

**Estimated Word Count**: ~4,800 words

This lesson provides the theoretical foundation and conceptual understanding necessary for the practical multithreading lessons that follow.
