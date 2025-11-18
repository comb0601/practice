# Lesson 48: Memory Ordering and Memory Fences

**Duration**: 8 hours
**Difficulty**: Expert

## Table of Contents
1. The C++ Memory Model
2. Sequential Consistency
3. Acquire-Release Semantics
4. Relaxed Ordering
5. Memory Fences
6. Happens-Before Relationships
7. Common Patterns
8. Practical Examples
9. Exercises

---

## 1. The C++ Memory Model

### Understanding Memory Reordering

The C++ memory model defines how threads interact through memory and what optimizations compilers and CPUs can perform.

```cpp
#include <iostream>
#include <atomic>
#include <thread>

// Global variables
int data = 0;
std::atomic<bool> ready{false};

void writer_unsafe() {
    data = 42;      // Store 1
    ready = true;   // Store 2
    // Compiler/CPU might reorder these!
}

void reader_unsafe() {
    while (!ready) {}  // Spin
    std::cout << data << "\n";  // Might see 0!
}

void explain_memory_model() {
    std::cout << "=== C++ Memory Model ===\n\n";

    std::cout << "Without proper synchronization:\n";
    std::cout << "- Compiler can reorder operations\n";
    std::cout << "- CPU can reorder memory accesses\n";
    std::cout << "- Caches may not be synchronized\n";
    std::cout << "- One thread's writes may not be visible to others\n\n";

    std::cout << "Memory ordering specifies:\n";
    std::cout << "- Which reorderings are allowed\n";
    std::cout << "- When writes become visible\n";
    std::cout << "- Synchronization guarantees\n\n";
}

int main() {
    explain_memory_model();
    return 0;
}
```

---

## 2. Sequential Consistency

### memory_order_seq_cst

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::atomic<int> x{0}, y{0};
std::atomic<int> r1{0}, r2{0};

void thread1_seqcst() {
    x.store(1, std::memory_order_seq_cst);
    r1 = y.load(std::memory_order_seq_cst);
}

void thread2_seqcst() {
    y.store(1, std::memory_order_seq_cst);
    r2 = x.load(std::memory_order_seq_cst);
}

void sequential_consistency_example() {
    std::cout << "=== Sequential Consistency ===\n\n";

    std::cout << "memory_order_seq_cst guarantees:\n";
    std::cout << "- Total order across all threads\n";
    std::cout << "- All threads see operations in same order\n";
    std::cout << "- Strongest memory ordering\n";
    std::cout << "- Default for atomic operations\n";
    std::cout << "- Performance cost: memory fences\n\n";

    // Run test multiple times
    int both_zero = 0;
    for (int i = 0; i < 1000; ++i) {
        x = 0; y = 0; r1 = 0; r2 = 0;

        std::thread t1(thread1_seqcst);
        std::thread t2(thread2_seqcst);
        t1.join();
        t2.join();

        if (r1 == 0 && r2 == 0) {
            ++both_zero;
        }
    }

    std::cout << "Runs where r1=0 and r2=0: " << both_zero << "/1000\n";
    std::cout << "(Should be 0 with seq_cst)\n\n";
}

int main() {
    sequential_consistency_example();
    return 0;
}
```

---

## 3. Acquire-Release Semantics

### memory_order_acquire and memory_order_release

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <string>

std::string message;
std::atomic<bool> ready{false};

void producer_acq_rel() {
    message = "Data from producer";
    ready.store(true, std::memory_order_release);  // Release semantics
}

void consumer_acq_rel() {
    while (!ready.load(std::memory_order_acquire)) {  // Acquire semantics
        // Wait
    }
    std::cout << message << "\n";  // Guaranteed to see message
}

void acquire_release_example() {
    std::cout << "=== Acquire-Release Semantics ===\n\n";

    std::thread producer(producer_acq_rel);
    std::thread consumer(consumer_acq_rel);

    producer.join();
    consumer.join();

    std::cout << "\nAcquire-Release guarantees:\n";
    std::cout << "- Release prevents earlier writes from moving after\n";
    std::cout << "- Acquire prevents later reads from moving before\n";
    std::cout << "- Creates synchronizes-with relationship\n";
    std::cout << "- Lighter than seq_cst\n\n";
}

int main() {
    acquire_release_example();
    return 0;
}
```

### Producer-Consumer with Acquire-Release

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

template<typename T>
class SPSCQueue {
public:
    SPSCQueue() : buffer_(new T[SIZE]) {}

    ~SPSCQueue() { delete[] buffer_; }

    bool push(const T& value) {
        size_t write = write_pos_.load(std::memory_order_relaxed);
        size_t next = (write + 1) % SIZE;

        if (next == read_pos_.load(std::memory_order_acquire)) {
            return false;  // Queue full
        }

        buffer_[write] = value;
        write_pos_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& value) {
        size_t read = read_pos_.load(std::memory_order_relaxed);

        if (read == write_pos_.load(std::memory_order_acquire)) {
            return false;  // Queue empty
        }

        value = buffer_[read];
        read_pos_.store((read + 1) % SIZE, std::memory_order_release);
        return true;
    }

private:
    static constexpr size_t SIZE = 1024;
    T* buffer_;
    std::atomic<size_t> read_pos_{0};
    std::atomic<size_t> write_pos_{0};
};

void spsc_queue_example() {
    std::cout << "=== SPSC Queue with Acquire-Release ===\n\n";

    SPSCQueue<int> queue;
    std::atomic<bool> done{false};

    std::thread producer([&]() {
        for (int i = 0; i < 1000; ++i) {
            while (!queue.push(i)) {
                std::this_thread::yield();
            }
        }
        done.store(true, std::memory_order_release);
    });

    std::thread consumer([&]() {
        int value;
        int count = 0;
        while (!done.load(std::memory_order_acquire) || queue.pop(value)) {
            if (queue.pop(value)) {
                ++count;
            } else {
                std::this_thread::yield();
            }
        }
        std::cout << "Consumed " << count << " items\n";
    });

    producer.join();
    consumer.join();
    std::cout << "\n";
}

int main() {
    spsc_queue_example();
    return 0;
}
```

---

## 4. Relaxed Ordering

### memory_order_relaxed

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::atomic<int> counter{0};

void increment_relaxed() {
    for (int i = 0; i < 100000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

void relaxed_ordering_example() {
    std::cout << "=== Relaxed Ordering ===\n\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(increment_relaxed);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter: " << counter.load(std::memory_order_relaxed) << "\n\n";

    std::cout << "Relaxed ordering:\n";
    std::cout << "- No synchronization\n";
    std::cout << "- Only atomicity guaranteed\n";
    std::cout << "- Fastest memory order\n";
    std::cout << "- Use for independent counters\n";
    std::cout << "- DON'T use for synchronization\n\n";
}

int main() {
    relaxed_ordering_example();
    return 0;
}
```

---

## 5. Memory Fences

### std::atomic_thread_fence

```cpp
#include <iostream>
#include <atomic>
#include <thread>

int data1 = 0, data2 = 0;
std::atomic<int> flag{0};

void writer_with_fence() {
    data1 = 42;
    data2 = 100;
    std::atomic_thread_fence(std::memory_order_release);  // Fence
    flag.store(1, std::memory_order_relaxed);
}

void reader_with_fence() {
    while (flag.load(std::memory_order_relaxed) == 0) {
        // Wait
    }
    std::atomic_thread_fence(std::memory_order_acquire);  // Fence
    std::cout << "data1: " << data1 << ", data2: " << data2 << "\n";
}

void fence_example() {
    std::cout << "=== Memory Fences ===\n\n";

    std::thread writer(writer_with_fence);
    std::thread reader(reader_with_fence);

    writer.join();
    reader.join();

    std::cout << "\nMemory fences:\n";
    std::cout << "- Prevent reordering across the fence\n";
    std::cout << "- Can use relaxed atomics with fences\n";
    std::cout << "- Separate synchronization from data\n";
    std::cout << "- Sometimes more efficient\n\n";
}

int main() {
    fence_example();
    return 0;
}
```

---

## 6. Happens-Before Relationships

### Understanding Happens-Before

```cpp
#include <iostream>
#include <atomic>
#include <thread>

void explain_happens_before() {
    std::cout << "=== Happens-Before Relationships ===\n\n";

    std::cout << "A happens-before B means:\n";
    std::cout << "- A's effects are visible to B\n";
    std::cout << "- A is sequenced before B\n\n";

    std::cout << "Established by:\n";
    std::cout << "1. Sequenced-before (same thread)\n";
    std::cout << "2. Synchronizes-with (acquire-release)\n";
    std::cout << "3. Thread start/join\n";
    std::cout << "4. Transitivity\n\n";

    std::cout << "Example:\n";
    std::cout << "Thread 1: x.store(1, release)  // A\n";
    std::cout << "Thread 2: while(!x.load(acquire)) // B\n";
    std::cout << "         : use(data)              // C\n\n";
    std::cout << "A synchronizes-with B\n";
    std::cout << "B happens-before C\n";
    std::cout << "Therefore A happens-before C (transitive)\n\n";
}

int main() {
    explain_happens_before();
    return 0;
}
```

---

## 7. Common Patterns

### Pattern 1: Message Passing

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <string>

std::string payload;
std::atomic<bool> ready{false};

void send_message(const std::string& msg) {
    payload = msg;  // Non-atomic write
    ready.store(true, std::memory_order_release);  // Synchronize
}

void receive_message() {
    while (!ready.load(std::memory_order_acquire)) {}  // Synchronize
    std::cout << "Received: " << payload << "\n";  // Safe to read
}

void message_passing_pattern() {
    std::cout << "=== Message Passing Pattern ===\n\n";

    std::thread sender([](){ send_message("Hello, World!"); });
    std::thread receiver(receive_message);

    sender.join();
    receiver.join();
    std::cout << "\n";
}

int main() {
    message_passing_pattern();
    return 0;
}
```

### Pattern 2: Double-Checked Locking

```cpp
#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>

template<typename T>
class Singleton {
public:
    static T* getInstance() {
        T* ptr = instance_.load(std::memory_order_acquire);
        if (!ptr) {
            std::lock_guard<std::mutex> lock(mutex_);
            ptr = instance_.load(std::memory_order_relaxed);
            if (!ptr) {
                ptr = new T();
                instance_.store(ptr, std::memory_order_release);
            }
        }
        return ptr;
    }

private:
    static std::atomic<T*> instance_;
    static std::mutex mutex_;
};

template<typename T>
std::atomic<T*> Singleton<T>::instance_{nullptr};

template<typename T>
std::mutex Singleton<T>::mutex_;

void double_checked_locking_pattern() {
    std::cout << "=== Double-Checked Locking (Correct) ===\n\n";
    std::cout << "Uses acquire-release for proper synchronization\n\n";
}

int main() {
    double_checked_locking_pattern();
    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Memory Ordering Comparison
Benchmark different memory orderings and measure performance.

### Exercise 2: Custom Synchronization
Implement a simple synchronization primitive using acquire-release.

### Exercise 3: Find the Bug
Identify memory ordering bugs in provided code.

### Exercise 4: Lock-Free Queue
Implement a lock-free queue with proper memory ordering.

---

## Key Takeaways

1. C++ memory model defines how threads interact through memory
2. **Sequential consistency** (seq_cst): strongest, slowest
3. **Acquire-Release**: middle ground, most common
4. **Relaxed**: weakest, fastest, no synchronization
5. Memory fences provide synchronization without atomic operations
6. **Happens-before** relationships ensure visibility
7. Use acquire-release for most synchronization needs
8. Use relaxed for independent counters only
9. Memory ordering is subtle - start with seq_cst
10. Profile before optimizing memory orders

---

## Summary Checklist

Before moving to Lesson 49, ensure you understand:
- [ ] The C++ memory model basics
- [ ] Sequential consistency guarantees
- [ ] Acquire-release semantics
- [ ] Relaxed ordering limitations
- [ ] Memory fences usage
- [ ] Happens-before relationships
- [ ] Common patterns (message passing, double-checked locking)
- [ ] When to use each memory order
- [ ] Performance trade-offs
- [ ] Testing strategies for memory ordering

**Next Lesson**: Condition Variables - thread synchronization and waiting.

---

**Estimated Word Count**: ~4,100 words
