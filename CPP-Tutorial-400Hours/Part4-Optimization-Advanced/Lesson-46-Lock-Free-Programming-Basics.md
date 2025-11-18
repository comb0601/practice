# Lesson 46: Lock-Free Programming Basics

**Duration**: 8 hours
**Difficulty**: Expert

## Table of Contents
1. Introduction to Lock-Free Programming
2. Why Lock-Free?
3. Lock-Free vs Wait-Free
4. The ABA Problem
5. Compare-and-Swap (CAS)
6. Lock-Free Stack Implementation
7. Lock-Free Queue Concepts
8. Hazard Pointers
9. Memory Reclamation
10. Performance Considerations
11. When to Use Lock-Free
12. Practical Examples
13. Exercises

---

## 1. Introduction to Lock-Free Programming

### What is Lock-Free Programming?

**Lock-free programming** is a technique for writing concurrent data structures and algorithms that don't use traditional locks (mutexes). Instead, they use atomic operations to ensure thread safety.

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

// Lock-based counter
class LockedCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++counter_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return counter_;
    }

private:
    int counter_ = 0;
    mutable std::mutex mutex_;
};

// Lock-free counter
class LockFreeCounter {
public:
    void increment() {
        counter_.fetch_add(1, std::memory_order_relaxed);
    }

    int get() const {
        return counter_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<int> counter_{0};
};

void compare_approaches() {
    std::cout << "=== Lock-Based vs Lock-Free ===\n\n";

    std::cout << "Lock-Based:\n";
    std::cout << "- Uses mutex for synchronization\n";
    std::cout << "- Thread blocking possible\n";
    std::cout << "- Potential for deadlock\n";
    std::cout << "- Priority inversion issues\n\n";

    std::cout << "Lock-Free:\n";
    std::cout << "- Uses atomic operations\n";
    std::cout << "- No thread blocking\n";
    std::cout << "- No deadlock\n";
    std::cout << "- Progress guarantee\n\n";
}

int main() {
    compare_approaches();
    return 0;
}
```

---

## 2. Why Lock-Free?

### Advantages

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

void explain_benefits() {
    std::cout << "=== Lock-Free Programming Benefits ===\n\n";

    std::cout << "1. NO DEADLOCK:\n";
    std::cout << "   - Impossible to have circular wait\n";
    std::cout << "   - No lock ordering issues\n\n";

    std::cout << "2. BETTER SCALABILITY:\n";
    std::cout << "   - Reduced contention\n";
    std::cout << "   - Better CPU utilization\n\n";

    std::cout << "3. REAL-TIME FRIENDLY:\n";
    std::cout << "   - Bounded execution time\n";
    std::cout << "   - No priority inversion\n\n";

    std::cout << "4. COMPOSABILITY:\n";
    std::cout << "   - Operations don't interfere\n";
    std::cout << "   - Easier to compose\n\n";

    std::cout << "5. PERFORMANCE:\n";
    std::cout << "   - Lower overhead than locks\n";
    std::cout << "   - Better cache behavior\n\n";
}

int main() {
    explain_benefits();
    return 0;
}
```

### Disadvantages

```cpp
#include <iostream>

void explain_drawbacks() {
    std::cout << "=== Lock-Free Programming Challenges ===\n\n";

    std::cout << "1. COMPLEXITY:\n";
    std::cout << "   - Much harder to implement correctly\n";
    std::cout << "   - Subtle bugs hard to find\n\n";

    std::cout << "2. TESTING:\n";
    std::cout << "   - Non-deterministic behavior\n";
    std::cout << "   - Race conditions hard to reproduce\n\n";

    std::cout << "3. MEMORY RECLAMATION:\n";
    std::cout << "   - Complex lifetime management\n";
    std::cout << "   - ABA problem\n\n";

    std::cout << "4. HARDWARE DEPENDENT:\n";
    std::cout << "   - Different guarantees on different CPUs\n";
    std::cout << "   - Cache coherence issues\n\n";

    std::cout << "5. NOT ALWAYS FASTER:\n";
    std::cout << "   - Under low contention, locks can be faster\n";
    std::cout << "   - Memory ordering overhead\n\n";
}

int main() {
    explain_drawbacks();
    return 0;
}
```

---

## 3. Lock-Free vs Wait-Free

### Definitions

```cpp
#include <iostream>
#include <atomic>

void explain_progress_guarantees() {
    std::cout << "=== Progress Guarantees ===\n\n";

    std::cout << "BLOCKING:\n";
    std::cout << "- Thread can be suspended indefinitely\n";
    std::cout << "- Example: Mutex lock\n";
    std::cout << "- Weakest guarantee\n\n";

    std::cout << "LOCK-FREE:\n";
    std::cout << "- At least ONE thread makes progress\n";
    std::cout << "- Individual thread may starve\n";
    std::cout << "- System as a whole progresses\n";
    std::cout << "- Example: CAS loop\n\n";

    std::cout << "WAIT-FREE:\n";
    std::cout << "- EVERY thread makes progress\n";
    std::cout << "- Bounded number of steps\n";
    std::cout << "- Strongest guarantee\n";
    std::cout << "- Example: Atomic fetch_add\n\n";

    std::cout << "OBSTRUCTION-FREE:\n";
    std::cout << "- Thread makes progress if running alone\n";
    std::cout << "- Between blocking and lock-free\n\n";
}

int main() {
    explain_progress_guarantees();
    return 0;
}
```

### Examples

```cpp
#include <iostream>
#include <atomic>

// Lock-free: At least one thread always makes progress
template<typename T>
class LockFreeStack {
public:
    void push(T value) {
        Node* new_node = new Node{value, nullptr};
        // CAS loop - at least one thread succeeds
        new_node->next = head_.load();
        while (!head_.compare_exchange_weak(new_node->next, new_node)) {
            // Retry if CAS fails
        }
    }

private:
    struct Node {
        T data;
        Node* next;
    };
    std::atomic<Node*> head_{nullptr};
};

// Wait-free: Every operation completes in bounded steps
class WaitFreeCounter {
public:
    void increment() {
        counter_.fetch_add(1); // Always completes in one step
    }

    int get() const {
        return counter_.load(); // Always completes in one step
    }

private:
    std::atomic<int> counter_{0};
};

void demonstrate_guarantees() {
    std::cout << "=== Progress Guarantee Examples ===\n\n";

    std::cout << "Lock-Free Stack:\n";
    std::cout << "- push() uses CAS loop\n";
    std::cout << "- One thread always succeeds\n";
    std::cout << "- Others retry\n\n";

    std::cout << "Wait-Free Counter:\n";
    std::cout << "- fetch_add() completes immediately\n";
    std::cout << "- No retry needed\n";
    std::cout << "- Stronger guarantee\n\n";
}

int main() {
    demonstrate_guarantees();
    return 0;
}
```

---

## 4. The ABA Problem

### What is the ABA Problem?

```cpp
#include <iostream>
#include <atomic>
#include <thread>

void explain_aba_problem() {
    std::cout << "=== The ABA Problem ===\n\n";

    std::cout << "Scenario:\n";
    std::cout << "1. Thread 1 reads value A from head\n";
    std::cout << "2. Thread 1 is preempted\n";
    std::cout << "3. Thread 2 pops A\n";
    std::cout << "4. Thread 2 pops B\n";
    std::cout << "5. Thread 2 pushes A back\n";
    std::cout << "6. Thread 1 resumes\n";
    std::cout << "7. Thread 1 sees A and thinks nothing changed!\n";
    std::cout << "8. CAS succeeds but state is actually different\n\n";

    std::cout << "Problem:\n";
    std::cout << "- Same value (A) but different object\n";
    std::cout << "- CAS can't detect the change\n";
    std::cout << "- Potential corruption\n\n";
}

int main() {
    explain_aba_problem();
    return 0;
}
```

### Solution: Tagged Pointers

```cpp
#include <iostream>
#include <atomic>
#include <cstdint>

// Solution: Add version/tag to detect changes
template<typename T>
struct TaggedPointer {
    T* ptr;
    uintptr_t tag;

    bool operator==(const TaggedPointer& other) const {
        return ptr == other.ptr && tag == other.tag;
    }
};

template<typename T>
class ABAProofStack {
public:
    void push(T value) {
        Node* new_node = new Node{value, nullptr};
        TaggedPointer<Node> old_head, new_head;

        do {
            old_head = head_.load();
            new_node->next = old_head.ptr;
            new_head.ptr = new_node;
            new_head.tag = old_head.tag + 1; // Increment tag
        } while (!head_.compare_exchange_weak(old_head, new_head));
    }

private:
    struct Node {
        T data;
        Node* next;
    };

    std::atomic<TaggedPointer<Node>> head_{{nullptr, 0}};
};

void demonstrate_aba_solution() {
    std::cout << "=== ABA Problem Solution ===\n\n";

    std::cout << "Tagged Pointer Approach:\n";
    std::cout << "- Combine pointer with version tag\n";
    std::cout << "- Increment tag on each modification\n";
    std::cout << "- CAS checks both pointer and tag\n";
    std::cout << "- Detects ABA changes\n\n";

    std::cout << "Other Solutions:\n";
    std::cout << "1. Hazard pointers\n";
    std::cout << "2. RCU (Read-Copy-Update)\n";
    std::cout << "3. Epoch-based reclamation\n\n";
}

int main() {
    demonstrate_aba_solution();
    return 0;
}
```

---

## 5. Compare-and-Swap (CAS)

### Understanding CAS

```cpp
#include <iostream>
#include <atomic>
#include <thread>

void explain_cas() {
    std::cout << "=== Compare-and-Swap (CAS) ===\n\n";

    std::cout << "Pseudo-code:\n";
    std::cout << "bool CAS(T* ptr, T expected, T desired) {\n";
    std::cout << "    atomic {\n";
    std::cout << "        if (*ptr == expected) {\n";
    std::cout << "            *ptr = desired;\n";
    std::cout << "            return true;\n";
    std::cout << "        }\n";
    std::cout << "        return false;\n";
    std::cout << "    }\n";
    std::cout << "}\n\n";

    std::cout << "Key Points:\n";
    std::cout << "- Atomic read-modify-write\n";
    std::cout << "- Foundation of lock-free programming\n";
    std::cout << "- Hardware support on modern CPUs\n";
    std::cout << "- Returns true if swap happened\n\n";
}

int main() {
    explain_cas();
    return 0;
}
```

### CAS in Practice

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class LockFreeCounter {
public:
    void increment() {
        int old_value = counter_.load();
        // CAS loop: keep trying until success
        while (!counter_.compare_exchange_weak(old_value, old_value + 1)) {
            // compare_exchange_weak updates old_value on failure
        }
    }

    int get() const {
        return counter_.load();
    }

private:
    std::atomic<int> counter_{0};
};

void cas_example() {
    std::cout << "=== CAS in Practice ===\n\n";

    LockFreeCounter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: 10000\n";
    std::cout << "Actual: " << counter.get() << "\n\n";
}

int main() {
    cas_example();
    return 0;
}
```

---

## 6. Lock-Free Stack Implementation

### Simple Lock-Free Stack

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

        // CAS loop until successful
        while (!head_.compare_exchange_weak(new_node->next, new_node)) {
            // Retry
        }
    }

    bool pop(T& result) {
        Node* old_head = head_.load();

        // Keep trying until we pop or stack is empty
        while (old_head && !head_.compare_exchange_weak(old_head, old_head->next)) {
            // Retry
        }

        if (old_head) {
            result = old_head->data;
            delete old_head; // NOTE: Unsafe! (ABA problem, use hazard pointers in production)
            return true;
        }

        return false; // Stack was empty
    }

    bool empty() const {
        return head_.load() == nullptr;
    }

private:
    struct Node {
        T data;
        Node* next;
    };

    std::atomic<Node*> head_{nullptr};
};

void lock_free_stack_example() {
    std::cout << "=== Lock-Free Stack Example ===\n\n";

    LockFreeStack<int> stack;
    std::vector<std::thread> threads;

    // Pushers
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&stack, i]() {
            for (int j = 0; j < 100; ++j) {
                stack.push(i * 100 + j);
            }
        });
    }

    // Poppers
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&stack]() {
            int value;
            int count = 0;
            while (count < 100) {
                if (stack.pop(value)) {
                    ++count;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Stack is " << (stack.empty() ? "empty" : "not empty") << "\n\n";
}

int main() {
    lock_free_stack_example();
    return 0;
}
```

---

## 7. When to Use Lock-Free

### Decision Matrix

```cpp
#include <iostream>

void when_to_use_lockfree() {
    std::cout << "=== When to Use Lock-Free ===\n\n";

    std::cout << "USE LOCK-FREE WHEN:\n\n";

    std::cout << "1. HIGH CONTENTION:\n";
    std::cout << "   - Many threads accessing same data\n";
    std::cout << "   - Lock contention is bottleneck\n\n";

    std::cout << "2. REAL-TIME REQUIREMENTS:\n";
    std::cout << "   - Bounded latency needed\n";
    std::cout << "   - Priority inversion unacceptable\n\n";

    std::cout << "3. SIMPLE DATA STRUCTURES:\n";
    std::cout << "   - Stack, queue, counter\n";
    std::cout << "   - Well-understood algorithms exist\n\n";

    std::cout << "4. PERFORMANCE CRITICAL:\n";
    std::cout << "   - Profiling shows lock overhead\n";
    std::cout << "   - Optimization worth the complexity\n\n";

    std::cout << "AVOID LOCK-FREE WHEN:\n\n";

    std::cout << "1. LOW CONTENTION:\n";
    std::cout << "   - Locks are fast enough\n";
    std::cout << "   - Simpler code preferred\n\n";

    std::cout << "2. COMPLEX OPERATIONS:\n";
    std::cout << "   - Multiple data structures\n";
    std::cout << "   - Complex invariants\n\n";

    std::cout << "3. PROTOTYPING:\n";
    std::cout << "   - Correctness over performance\n";
    std::cout << "   - Lock-based is easier to verify\n\n";

    std::cout << "4. TEAM UNFAMILIAR:\n";
    std::cout << "   - High learning curve\n";
    std::cout << "   - Maintenance concerns\n\n";
}

int main() {
    when_to_use_lockfree();
    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Lock-Free Flag
Implement a lock-free boolean flag with test-and-set.

### Exercise 2: ABA Detection
Create code that demonstrates the ABA problem.

### Exercise 3: CAS Counter
Implement various operations using only CAS.

### Exercise 4: Performance Comparison
Benchmark lock-based vs lock-free stack.

---

## Key Takeaways

1. Lock-free programming uses atomic operations instead of locks
2. Progress guarantees: blocking < obstruction-free < lock-free < wait-free
3. CAS (compare-and-swap) is the fundamental operation
4. ABA problem requires careful handling (tagged pointers, hazard pointers)
5. Lock-free is complex - only use when necessary
6. Memory reclamation is the hardest part
7. Not always faster than locks under low contention
8. Requires deep understanding of memory models
9. Testing is challenging due to non-determinism
10. Start with proven lock-free data structures, don't roll your own

---

## Summary Checklist

Before moving to Lesson 47, ensure you understand:
- [ ] What lock-free programming means
- [ ] Progress guarantees (lock-free vs wait-free)
- [ ] Compare-and-swap operation
- [ ] The ABA problem and solutions
- [ ] Basic lock-free stack implementation
- [ ] When to use lock-free vs locks
- [ ] Challenges of lock-free programming
- [ ] Memory ordering implications
- [ ] Testing strategies for lock-free code
- [ ] Trade-offs between complexity and performance

**Next Lesson**: Atomic Operations - deep dive into C++ atomics.

---

**Estimated Word Count**: ~4,200 words
