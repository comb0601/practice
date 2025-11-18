# Lesson 47: Atomic Operations

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to std::atomic
2. Atomic Types
3. Atomic Operations
4. fetch_add, fetch_sub, fetch_and, fetch_or
5. compare_exchange Operations
6. Memory Order Basics
7. Atomic Flags
8. Atomic Pointers
9. Atomic Structures (C++20)
10. Performance Considerations
11. Practical Examples
12. Exercises

---

## 1. Introduction to std::atomic

### What is std::atomic?

`std::atomic` provides atomic operations on objects, ensuring thread-safe access without explicit locking.

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

// Non-atomic (unsafe)
int unsafe_counter = 0;

// Atomic (safe)
std::atomic<int> safe_counter{0};

void unsafe_increment() {
    for (int i = 0; i < 100000; ++i) {
        ++unsafe_counter; // Race condition!
    }
}

void safe_increment() {
    for (int i = 0; i < 100000; ++i) {
        ++safe_counter; // Thread-safe
    }
}

void demonstrate_atomic() {
    std::cout << "=== std::atomic Demonstration ===\n\n";

    std::vector<std::thread> threads;

    // Test unsafe version
    unsafe_counter = 0;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(unsafe_increment);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Unsafe counter: " << unsafe_counter << " (expected: 1000000)\n";

    // Test safe version
    threads.clear();
    safe_counter = 0;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(safe_increment);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Safe counter: " << safe_counter << " (expected: 1000000)\n\n";
}

int main() {
    demonstrate_atomic();
    return 0;
}
```

---

## 2. Atomic Types

### Standard Atomic Types

```cpp
#include <iostream>
#include <atomic>
#include <cstdint>

void demonstrate_atomic_types() {
    std::cout << "=== Atomic Types ===\n\n";

    // Integer atomics
    std::atomic<bool> atomic_bool{false};
    std::atomic<char> atomic_char{0};
    std::atomic<int> atomic_int{0};
    std::atomic<long> atomic_long{0};
    std::atomic<long long> atomic_long_long{0};

    // Unsigned integers
    std::atomic<unsigned int> atomic_uint{0};
    std::atomic<std::size_t> atomic_size_t{0};

    // Fixed-width integers (preferred)
    std::atomic<int8_t> atomic_int8{0};
    std::atomic<int16_t> atomic_int16{0};
    std::atomic<int32_t> atomic_int32{0};
    std::atomic<int64_t> atomic_int64{0};

    std::atomic<uint8_t> atomic_uint8{0};
    std::atomic<uint16_t> atomic_uint16{0};
    std::atomic<uint32_t> atomic_uint32{0};
    std::atomic<uint64_t> atomic_uint64{0};

    // Pointers
    int value = 42;
    std::atomic<int*> atomic_ptr{&value};

    // User-defined types (must be trivially copyable)
    struct Point {
        int x, y;
    };
    std::atomic<Point> atomic_point{{0, 0}};

    std::cout << "All atomic types initialized\n";
    std::cout << "Note: Use lock-free atomics for best performance\n\n";

    // Check if lock-free
    std::cout << "is_lock_free() checks:\n";
    std::cout << "atomic<int>: " << atomic_int.is_lock_free() << "\n";
    std::cout << "atomic<int64_t>: " << atomic_int64.is_lock_free() << "\n";
    std::cout << "atomic<Point>: " << atomic_point.is_lock_free() << "\n\n";
}

int main() {
    demonstrate_atomic_types();
    return 0;
}
```

---

## 3. Atomic Operations

### Basic Operations

```cpp
#include <iostream>
#include <atomic>
#include <thread>

void demonstrate_basic_operations() {
    std::cout << "=== Basic Atomic Operations ===\n\n";

    std::atomic<int> counter{10};

    std::cout << "Initial value: " << counter.load() << "\n";

    // Store
    counter.store(20);
    std::cout << "After store(20): " << counter.load() << "\n";

    // Exchange (atomic swap)
    int old = counter.exchange(30);
    std::cout << "exchange(30) returned: " << old << "\n";
    std::cout << "New value: " << counter.load() << "\n";

    // Arithmetic operations
    counter++;
    std::cout << "After ++: " << counter.load() << "\n";

    counter--;
    std::cout << "After --: " << counter.load() << "\n";

    counter += 5;
    std::cout << "After += 5: " << counter.load() << "\n";

    counter -= 3;
    std::cout << "After -= 3: " << counter.load() << "\n\n";
}

int main() {
    demonstrate_basic_operations();
    return 0;
}
```

---

## 4. fetch Operations

### fetch_add and fetch_sub

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

void demonstrate_fetch_operations() {
    std::cout << "=== fetch_* Operations ===\n\n";

    std::atomic<int> counter{100};

    // fetch_add: returns old value, increments by argument
    int old = counter.fetch_add(5);
    std::cout << "fetch_add(5) returned: " << old << "\n";
    std::cout << "New value: " << counter.load() << "\n\n";

    // fetch_sub: returns old value, decrements by argument
    old = counter.fetch_sub(3);
    std::cout << "fetch_sub(3) returned: " << old << "\n";
    std::cout << "New value: " << counter.load() << "\n\n";

    // fetch_and: bitwise AND
    std::atomic<unsigned int> flags{0b1111};
    old = flags.fetch_and(0b1100);
    std::cout << "fetch_and(0b1100) returned: " << old << "\n";
    std::cout << "New value: " << flags.load() << "\n\n";

    // fetch_or: bitwise OR
    flags.store(0b0011);
    old = flags.fetch_or(0b1100);
    std::cout << "fetch_or(0b1100) returned: " << old << "\n";
    std::cout << "New value: " << flags.load() << "\n\n";

    // fetch_xor: bitwise XOR
    flags.store(0b1010);
    old = flags.fetch_xor(0b1111);
    std::cout << "fetch_xor(0b1111) returned: " << old << "\n";
    std::cout << "New value: " << flags.load() << "\n\n";
}

int main() {
    demonstrate_fetch_operations();
    return 0;
}
```

### Practical Example: Thread-Safe Statistics

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class ThreadSafeStatistics {
public:
    void add_value(int value) {
        count_.fetch_add(1);
        sum_.fetch_add(value);

        // Update min
        int current_min = min_.load();
        while (value < current_min &&
               !min_.compare_exchange_weak(current_min, value)) {
            // Retry
        }

        // Update max
        int current_max = max_.load();
        while (value > current_max &&
               !max_.compare_exchange_weak(current_max, value)) {
            // Retry
        }
    }

    void print_stats() const {
        int c = count_.load();
        int s = sum_.load();
        std::cout << "Count: " << c << "\n";
        std::cout << "Sum: " << s << "\n";
        std::cout << "Average: " << (c > 0 ? static_cast<double>(s) / c : 0) << "\n";
        std::cout << "Min: " << min_.load() << "\n";
        std::cout << "Max: " << max_.load() << "\n";
    }

private:
    std::atomic<int> count_{0};
    std::atomic<int> sum_{0};
    std::atomic<int> min_{INT_MAX};
    std::atomic<int> max_{INT_MIN};
};

void statistics_example() {
    std::cout << "=== Thread-Safe Statistics ===\n\n";

    ThreadSafeStatistics stats;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&stats, i]() {
            for (int j = 0; j < 100; ++j) {
                stats.add_value(i * 100 + j);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    stats.print_stats();
    std::cout << "\n";
}

int main() {
    statistics_example();
    return 0;
}
```

---

## 5. compare_exchange Operations

### compare_exchange_weak vs compare_exchange_strong

```cpp
#include <iostream>
#include <atomic>

void demonstrate_compare_exchange() {
    std::cout << "=== compare_exchange Operations ===\n\n";

    std::atomic<int> value{100};

    // compare_exchange_strong
    int expected = 100;
    bool success = value.compare_exchange_strong(expected, 200);
    std::cout << "compare_exchange_strong(100, 200):\n";
    std::cout << "  Success: " << success << "\n";
    std::cout << "  Value: " << value.load() << "\n";
    std::cout << "  Expected: " << expected << "\n\n";

    // Failed compare_exchange
    expected = 100;  // Wrong expected value
    success = value.compare_exchange_strong(expected, 300);
    std::cout << "compare_exchange_strong(100, 300):\n";
    std::cout << "  Success: " << success << "\n";
    std::cout << "  Value: " << value.load() << "\n";
    std::cout << "  Expected updated to: " << expected << "\n\n";

    // compare_exchange_weak in loop
    value.store(10);
    expected = value.load();
    while (!value.compare_exchange_weak(expected, expected + 1)) {
        // Spurious failure possible, retry
        std::cout << "  Retrying...\n";
    }
    std::cout << "Successfully incremented to: " << value.load() << "\n\n";
}

int main() {
    demonstrate_compare_exchange();
    return 0;
}
```

### Lock-Free Stack Using CAS

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T>
class LockFreeStack {
public:
    ~LockFreeStack() {
        while (Node* node = head_.load()) {
            head_.store(node->next);
            delete node;
        }
    }

    void push(const T& value) {
        Node* new_node = new Node{value, nullptr};
        new_node->next = head_.load();

        // CAS loop
        while (!head_.compare_exchange_weak(new_node->next, new_node)) {
            // Automatically retries with updated next value
        }
    }

    bool pop(T& result) {
        Node* old_head = head_.load();

        while (old_head && !head_.compare_exchange_weak(old_head, old_head->next)) {
            // Retry
        }

        if (old_head) {
            result = old_head->data;
            delete old_head;
            return true;
        }

        return false;
    }

private:
    struct Node {
        T data;
        Node* next;
    };

    std::atomic<Node*> head_{nullptr};
};

void lockfree_stack_example() {
    std::cout << "=== Lock-Free Stack with CAS ===\n\n";

    LockFreeStack<int> stack;
    std::vector<std::thread> threads;

    // Pushers
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&stack, i]() {
            for (int j = 0; j < 10; ++j) {
                stack.push(i * 10 + j);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Pop all
    int value;
    int count = 0;
    while (stack.pop(value)) {
        ++count;
    }

    std::cout << "Pushed 50 items, popped " << count << " items\n\n";
}

int main() {
    lockfree_stack_example();
    return 0;
}
```

---

## 6. Memory Order Basics

### Memory Orderings

```cpp
#include <iostream>
#include <atomic>

void explain_memory_orders() {
    std::cout << "=== Memory Orderings ===\n\n";

    std::cout << "1. memory_order_relaxed:\n";
    std::cout << "   - No synchronization\n";
    std::cout << "   - Only atomicity guaranteed\n";
    std::cout << "   - Fastest\n\n";

    std::cout << "2. memory_order_acquire:\n";
    std::cout << "   - Prevents reordering of reads before this operation\n";
    std::cout << "   - Used for loads\n\n";

    std::cout << "3. memory_order_release:\n";
    std::cout << "   - Prevents reordering of writes after this operation\n";
    std::cout << "   - Used for stores\n\n";

    std::cout << "4. memory_order_acq_rel:\n";
    std::cout << "   - Both acquire and release\n";
    std::cout << "   - Used for read-modify-write\n\n";

    std::cout << "5. memory_order_seq_cst:\n";
    std::cout << "   - Sequential consistency\n";
    std::cout << "   - Strongest guarantee\n";
    std::cout << "   - Default for atomics\n";
    std::cout << "   - Slowest\n\n";
}

int main() {
    explain_memory_orders();
    return 0;
}
```

### Using Different Memory Orders

```cpp
#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> data{0};
std::atomic<bool> ready{false};

void producer() {
    data.store(42, std::memory_order_relaxed);
    ready.store(true, std::memory_order_release);  // Synchronize
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {  // Synchronize
        // Wait
    }
    std::cout << "Data: " << data.load(std::memory_order_relaxed) << "\n";
}

void memory_order_example() {
    std::cout << "=== Memory Order Example ===\n\n";

    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    std::cout << "\n";
}

int main() {
    memory_order_example();
    return 0;
}
```

---

## 7. Atomic Flags

### std::atomic_flag

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class SpinLock {
public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            // Spin wait
        }
    }

    void unlock() {
        flag_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

int shared_counter = 0;
SpinLock spinlock;

void increment_with_spinlock() {
    for (int i = 0; i < 10000; ++i) {
        spinlock.lock();
        ++shared_counter;
        spinlock.unlock();
    }
}

void spinlock_example() {
    std::cout << "=== Spinlock with atomic_flag ===\n\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(increment_with_spinlock);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: 100000\n";
    std::cout << "Actual: " << shared_counter << "\n\n";
}

int main() {
    spinlock_example();
    return 0;
}
```

---

## 8. Atomic Pointers

### Working with Atomic Pointers

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <memory>

struct Node {
    int data;
    Node* next;
};

std::atomic<Node*> head{nullptr};

void atomic_pointer_example() {
    std::cout << "=== Atomic Pointers ===\n\n";

    // Create nodes
    Node* node1 = new Node{1, nullptr};
    Node* node2 = new Node{2, nullptr};
    Node* node3 = new Node{3, nullptr};

    // Atomic pointer operations
    head.store(node1);
    std::cout << "Head points to node with data: " << head.load()->data << "\n";

    Node* old_head = head.exchange(node2);
    std::cout << "Old head had data: " << old_head->data << "\n";
    std::cout << "New head has data: " << head.load()->data << "\n";

    // CAS with pointers
    Node* expected = node2;
    if (head.compare_exchange_strong(expected, node3)) {
        std::cout << "Successfully swapped to node3\n";
    }

    std::cout << "Final head data: " << head.load()->data << "\n\n";

    // Cleanup
    delete node1;
    delete node2;
    delete node3;
}

int main() {
    atomic_pointer_example();
    return 0;
}
```

---

## 9. Performance Considerations

### Performance Comparison

```cpp
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

const int ITERATIONS = 1000000;
const int THREADS = 4;

// Mutex-based
std::mutex mtx;
int mutex_counter = 0;

void mutex_increment() {
    for (int i = 0; i < ITERATIONS; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++mutex_counter;
    }
}

// Atomic-based
std::atomic<int> atomic_counter{0};

void atomic_increment() {
    for (int i = 0; i < ITERATIONS; ++i) {
        ++atomic_counter;
    }
}

// Relaxed atomic
std::atomic<int> relaxed_counter{0};

void relaxed_increment() {
    for (int i = 0; i < ITERATIONS; ++i) {
        relaxed_counter.fetch_add(1, std::memory_order_relaxed);
    }
}

template<typename Func>
long long benchmark(Func func, const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(func);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << name << ": " << duration.count() << "ms\n";
    return duration.count();
}

void performance_comparison() {
    std::cout << "=== Performance Comparison ===\n\n";

    auto mutex_time = benchmark(mutex_increment, "Mutex        ");
    auto atomic_time = benchmark(atomic_increment, "Atomic       ");
    auto relaxed_time = benchmark(relaxed_increment, "Atomic Relaxed");

    std::cout << "\nSpeedup vs Mutex:\n";
    std::cout << "Atomic: " << (double)mutex_time / atomic_time << "x\n";
    std::cout << "Relaxed: " << (double)mutex_time / relaxed_time << "x\n\n";
}

int main() {
    performance_comparison();
    return 0;
}
```

---

## 10. Exercises

### Exercise 1: Atomic Counter
Implement a thread-safe counter using atomics.

### Exercise 2: Lock-Free Queue
Create a simple lock-free queue using compare_exchange.

### Exercise 3: Memory Ordering
Experiment with different memory orders and measure performance.

### Exercise 4: Atomic Statistics
Build a thread-safe statistics collector using atomic operations.

---

## Key Takeaways

1. `std::atomic` provides lock-free thread-safe operations
2. Use `is_lock_free()` to check hardware support
3. `fetch_*` operations return old value before modification
4. `compare_exchange` is foundation of lock-free algorithms
5. Memory ordering affects performance and correctness
6. `memory_order_relaxed` is fastest but provides no synchronization
7. `memory_order_seq_cst` is default and safest
8. Atomics are faster than mutexes for simple operations
9. Not all types can be atomic (must be trivially copyable)
10. Use atomics for counters, flags, and simple synchronization

---

## Summary Checklist

Before moving to Lesson 48, ensure you understand:
- [ ] What std::atomic provides
- [ ] Different atomic types available
- [ ] Basic atomic operations (load, store, exchange)
- [ ] fetch_* operations and their uses
- [ ] compare_exchange_weak vs compare_exchange_strong
- [ ] Memory ordering basics
- [ ] std::atomic_flag and spinlocks
- [ ] Atomic pointers
- [ ] Performance trade-offs
- [ ] When to use atomics vs mutexes

**Next Lesson**: Memory Ordering and Memory Fences - deep dive into memory models.

---

**Estimated Word Count**: ~4,600 words
