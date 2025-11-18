# Lesson 44: Race Conditions and Data Races

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Understanding Race Conditions
2. Data Races Explained
3. Race Condition vs Data Race
4. Common Race Condition Patterns
5. Detecting Race Conditions
6. The Lost Update Problem
7. Read-Modify-Write Operations
8. Memory Visibility Issues
9. Practical Examples
10. Prevention Strategies
11. Tools for Detection
12. Exercises

---

## 1. Understanding Race Conditions

### What is a Race Condition?

A **race condition** occurs when the behavior of software depends on the relative timing or interleaving of multiple threads. The outcome becomes unpredictable and may vary between executions.

### Simple Race Condition Example

```cpp
#include <iostream>
#include <thread>
#include <vector>

int shared_counter = 0; // Shared resource

void unsafe_increment() {
    for (int i = 0; i < 100000; ++i) {
        ++shared_counter; // NOT thread-safe!
    }
}

void demonstrate_race_condition() {
    std::cout << "=== Race Condition Demonstration ===\n\n";

    const int num_threads = 10;
    std::vector<std::thread> threads;

    // Create multiple threads incrementing shared counter
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(unsafe_increment);
    }

    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected value: " << (num_threads * 100000) << "\n";
    std::cout << "Actual value: " << shared_counter << "\n";
    std::cout << "Difference: " << (num_threads * 100000 - shared_counter) << "\n\n";

    if (shared_counter != num_threads * 100000) {
        std::cout << "RACE CONDITION DETECTED!\n";
        std::cout << "Lost updates occurred due to concurrent access.\n";
    }
}

int main() {
    demonstrate_race_condition();
    return 0;
}
```

**Why This Happens**:
The `++shared_counter` operation is actually three operations:
1. Read current value
2. Increment value
3. Write back to memory

These can interleave between threads, causing lost updates.

---

## 2. Data Races Explained

### What is a Data Race?

A **data race** occurs when:
1. Two or more threads access the same memory location
2. At least one access is a write
3. The accesses are not synchronized

Data races cause **undefined behavior** in C++.

### Data Race Example

```cpp
#include <iostream>
#include <thread>

int global_data = 0;

void writer() {
    global_data = 42; // Write
}

void reader() {
    std::cout << "Value: " << global_data << "\n"; // Read
}

void demonstrate_data_race() {
    std::cout << "=== Data Race Example ===\n\n";

    // Two threads accessing same variable without synchronization
    std::thread t1(writer);
    std::thread t2(reader);

    t1.join();
    t2.join();

    std::cout << "\nThis is a data race!\n";
    std::cout << "Reader might see: 0, 42, or garbage value\n";
    std::cout << "Behavior is UNDEFINED\n\n";
}

int main() {
    demonstrate_data_race();
    return 0;
}
```

### Assembly-Level View

```cpp
#include <iostream>
#include <thread>
#include <atomic>

// What ++counter actually does:
// 1. mov eax, [counter]    // Read
// 2. inc eax               // Increment
// 3. mov [counter], eax    // Write

void explain_assembly_level() {
    std::cout << "=== Assembly-Level Race Condition ===\n\n";

    std::cout << "The C++ code: ++counter\n";
    std::cout << "Compiles to (pseudo-assembly):\n";
    std::cout << "  1. READ counter into register\n";
    std::cout << "  2. INCREMENT register\n";
    std::cout << "  3. WRITE register back to counter\n\n";

    std::cout << "Thread interleaving example:\n";
    std::cout << "Initial value: 5\n\n";
    std::cout << "Thread A          Thread B          counter\n";
    std::cout << "--------          --------          -------\n";
    std::cout << "READ (5)                               5\n";
    std::cout << "                  READ (5)             5\n";
    std::cout << "INC (6)                                5\n";
    std::cout << "                  INC (6)              5\n";
    std::cout << "WRITE (6)                              6\n";
    std::cout << "                  WRITE (6)            6\n\n";
    std::cout << "Expected: 7, Got: 6 (Lost update!)\n\n";
}

int main() {
    explain_assembly_level();
    return 0;
}
```

---

## 3. Race Condition vs Data Race

### Key Differences

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int counter = 0;

// Race condition WITHOUT data race
// (synchronized but logic error possible)
void safe_but_racy() {
    // Thread A
    auto threadA = []() {
        std::lock_guard<std::mutex> lock(mtx);
        if (counter == 0) {
            // Thread might be preempted here
            counter = 1;
        }
    };

    // Thread B (same logic)
    auto threadB = []() {
        std::lock_guard<std::mutex> lock(mtx);
        if (counter == 0) {
            counter = 2;
        }
    };

    // Result depends on thread scheduling (race condition)
    // But no data race (proper synchronization)
}

void explain_difference() {
    std::cout << "=== Race Condition vs Data Race ===\n\n";

    std::cout << "DATA RACE:\n";
    std::cout << "- Concurrent access to shared data\n";
    std::cout << "- At least one write\n";
    std::cout << "- No synchronization\n";
    std::cout << "- Result: UNDEFINED BEHAVIOR\n\n";

    std::cout << "RACE CONDITION:\n";
    std::cout << "- Behavior depends on thread timing\n";
    std::cout << "- May have proper synchronization\n";
    std::cout << "- Logic/design issue\n";
    std::cout << "- Result: UNPREDICTABLE but DEFINED\n\n";

    std::cout << "Key insight: You can have:\n";
    std::cout << "- Data race without race condition (rare)\n";
    std::cout << "- Race condition without data race (possible)\n";
    std::cout << "- Both together (common bug)\n";
    std::cout << "- Neither (ideal!)\n\n";
}

int main() {
    explain_difference();
    return 0;
}
```

---

## 4. Common Race Condition Patterns

### Pattern 1: Lost Update

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class BankAccount {
public:
    // UNSAFE version
    void unsafe_deposit(int amount) {
        int current_balance = balance_;
        // Simulate processing time
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        balance_ = current_balance + amount;
    }

    // SAFE version
    void safe_deposit(int amount) {
        std::lock_guard<std::mutex> lock(mutex_);
        balance_ += amount;
    }

    int get_balance() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return balance_;
    }

private:
    int balance_ = 0;
    mutable std::mutex mutex_;
};

void demonstrate_lost_update() {
    std::cout << "=== Lost Update Pattern ===\n\n";

    // Test unsafe version
    {
        BankAccount account;
        std::vector<std::thread> threads;

        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(&BankAccount::unsafe_deposit, &account, 100);
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Unsafe: Expected 1000, Got " << account.get_balance() << "\n";
    }

    // Test safe version
    {
        BankAccount account;
        std::vector<std::thread> threads;

        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(&BankAccount::safe_deposit, &account, 100);
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Safe: Expected 1000, Got " << account.get_balance() << "\n\n";
    }
}

int main() {
    demonstrate_lost_update();
    return 0;
}
```

### Pattern 2: Check-Then-Act

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>

class Resource {
public:
    void use() {
        std::cout << "Using resource\n";
    }
};

class ResourceManager {
public:
    // UNSAFE: Check-then-act race condition
    Resource* get_resource_unsafe() {
        if (!resource_) { // CHECK
            // Another thread might create resource here!
            resource_ = std::make_unique<Resource>(); // ACT
        }
        return resource_.get();
    }

    // SAFE: Atomic check-and-act
    Resource* get_resource_safe() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!resource_) {
            resource_ = std::make_unique<Resource>();
        }
        return resource_.get();
    }

private:
    std::unique_ptr<Resource> resource_;
    std::mutex mutex_;
};

void demonstrate_check_then_act() {
    std::cout << "=== Check-Then-Act Pattern ===\n\n";

    std::cout << "The problem:\n";
    std::cout << "1. Thread A checks: resource is null\n";
    std::cout << "2. Thread B checks: resource is null\n";
    std::cout << "3. Thread A creates resource\n";
    std::cout << "4. Thread B creates resource (memory leak!)\n\n";

    std::cout << "Solution: Lock both check and act together\n\n";
}

int main() {
    demonstrate_check_then_act();
    return 0;
}
```

### Pattern 3: Read-Modify-Write

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

class Statistics {
public:
    // UNSAFE version
    void unsafe_update(int value) {
        total_ += value;  // READ-MODIFY-WRITE
        count_++;         // READ-MODIFY-WRITE
    }

    // SAFE version using atomics
    void safe_update(int value) {
        total_.fetch_add(value);
        count_.fetch_add(1);
    }

    double unsafe_average() const {
        return count_ > 0 ? static_cast<double>(total_) / count_ : 0.0;
    }

    double safe_average() const {
        int c = count_.load();
        return c > 0 ? static_cast<double>(total_.load()) / c : 0.0;
    }

private:
    // For demonstration - mixing atomic and non-atomic
    std::atomic<int> total_{0};
    std::atomic<int> count_{0};
};

void demonstrate_read_modify_write() {
    std::cout << "=== Read-Modify-Write Pattern ===\n\n";

    Statistics stats;
    std::vector<std::thread> threads;

    for (int i = 0; i < 100; ++i) {
        threads.emplace_back(&Statistics::safe_update, &stats, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Average: " << stats.safe_average() << "\n\n";
}

int main() {
    demonstrate_read_modify_write();
    return 0;
}
```

### Pattern 4: Time-of-Check to Time-of-Use (TOCTOU)

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <string>

class Cache {
public:
    // UNSAFE: TOCTOU race
    std::string get_unsafe(const std::string& key) {
        if (contains_unsafe(key)) { // TIME OF CHECK
            // Another thread might remove key here!
            return data_[key]; // TIME OF USE - potential crash!
        }
        return "";
    }

    // SAFE: Atomic check and use
    std::string get_safe(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = data_.find(key);
        return (it != data_.end()) ? it->second : "";
    }

    bool contains_unsafe(const std::string& key) {
        return data_.find(key) != data_.end();
    }

    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_[key] = value;
    }

private:
    std::unordered_map<std::string, std::string> data_;
    std::mutex mutex_;
};

void demonstrate_toctou() {
    std::cout << "=== TOCTOU (Time-of-Check to Time-of-Use) ===\n\n";

    std::cout << "The vulnerability:\n";
    std::cout << "1. Thread A: Checks if key exists (TRUE)\n";
    std::cout << "2. Thread B: Removes the key\n";
    std::cout << "3. Thread A: Tries to use key (CRASH/ERROR!)\n\n";

    std::cout << "Solution: Make check and use atomic\n\n";
}

int main() {
    demonstrate_toctou();
    return 0;
}
```

---

## 5. Detecting Race Conditions

### Manual Detection Techniques

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// Bad: Global mutable state
int global_var = 0;

// Good: Protected global state
std::mutex global_mutex;
int protected_global = 0;

void manual_detection_checklist() {
    std::cout << "=== Race Condition Detection Checklist ===\n\n";

    std::cout << "Ask these questions:\n\n";

    std::cout << "1. SHARED MUTABLE STATE?\n";
    std::cout << "   - Is data accessed by multiple threads?\n";
    std::cout << "   - Is it modified (not const)?\n\n";

    std::cout << "2. SYNCHRONIZATION?\n";
    std::cout << "   - Is access protected by mutex/atomic?\n";
    std::cout << "   - Are all accesses protected consistently?\n\n";

    std::cout << "3. ORDERING GUARANTEES?\n";
    std::cout << "   - Does order of operations matter?\n";
    std::cout << "   - Are there happens-before relationships?\n\n";

    std::cout << "4. COMPOUND OPERATIONS?\n";
    std::cout << "   - Are there check-then-act patterns?\n";
    std::cout << "   - Are read-modify-write operations atomic?\n\n";

    std::cout << "5. LIFETIME ISSUES?\n";
    std::cout << "   - Can objects be destroyed while in use?\n";
    std::cout << "   - Are references/pointers to local variables shared?\n\n";
}

int main() {
    manual_detection_checklist();
    return 0;
}
```

### Code Review Patterns

```cpp
#include <iostream>
#include <thread>
#include <mutex>

class SafetyAnalysis {
public:
    void analyze() {
        std::cout << "=== Code Patterns to Watch For ===\n\n";

        std::cout << "DANGEROUS PATTERNS:\n\n";

        std::cout << "1. Unprotected global/static variables:\n";
        std::cout << "   ❌ static int counter = 0;\n";
        std::cout << "   ✅ static std::atomic<int> counter{0};\n\n";

        std::cout << "2. Naked increment/decrement:\n";
        std::cout << "   ❌ shared_value++;\n";
        std::cout << "   ✅ shared_value.fetch_add(1);\n\n";

        std::cout << "3. Unlocked container access:\n";
        std::cout << "   ❌ shared_vector.push_back(x);\n";
        std::cout << "   ✅ {lock_guard lock(mtx); shared_vector.push_back(x);}\n\n";

        std::cout << "4. Double-checked locking (naive):\n";
        std::cout << "   ❌ if (!ptr) { lock(); if (!ptr) ptr = new T(); }\n";
        std::cout << "   ✅ call_once(flag, []{ ptr = new T(); });\n\n";

        std::cout << "5. Iterating shared containers:\n";
        std::cout << "   ❌ for (auto& x : shared_vector)\n";
        std::cout << "   ✅ {lock_guard lock(mtx); for (auto& x : shared_vector)}\n\n";
    }
};

int main() {
    SafetyAnalysis analysis;
    analysis.analyze();
    return 0;
}
```

---

## 6. The Lost Update Problem

### Detailed Lost Update Example

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

class TicketCounter {
public:
    // Unsafe: Lost updates possible
    int unsafe_get_and_increment() {
        int ticket = counter_;
        std::this_thread::sleep_for(std::chrono::microseconds(1)); // Simulate delay
        counter_ = ticket + 1;
        return ticket;
    }

    // Safe: Using atomic
    int safe_get_and_increment() {
        return atomic_counter_.fetch_add(1);
    }

    int unsafe_count() const { return counter_; }
    int safe_count() const { return atomic_counter_.load(); }

private:
    int counter_ = 0;
    std::atomic<int> atomic_counter_{0};
};

void demonstrate_lost_updates() {
    std::cout << "=== Lost Update Problem ===\n\n";

    const int num_threads = 10;
    const int tickets_per_thread = 100;

    // Test unsafe version
    {
        TicketCounter counter;
        std::vector<std::thread> threads;
        std::vector<std::vector<int>> tickets(num_threads);

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&counter, &tickets, i, tickets_per_thread]() {
                for (int j = 0; j < tickets_per_thread; ++j) {
                    tickets[i].push_back(counter.unsafe_get_and_increment());
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Unsafe version:\n";
        std::cout << "Expected tickets issued: " << num_threads * tickets_per_thread << "\n";
        std::cout << "Final counter value: " << counter.unsafe_count() << "\n";
        std::cout << "Lost updates: " << (num_threads * tickets_per_thread - counter.unsafe_count()) << "\n\n";
    }

    // Test safe version
    {
        TicketCounter counter;
        std::vector<std::thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&counter, tickets_per_thread]() {
                for (int j = 0; j < tickets_per_thread; ++j) {
                    counter.safe_get_and_increment();
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Safe version:\n";
        std::cout << "Expected tickets issued: " << num_threads * tickets_per_thread << "\n";
        std::cout << "Final counter value: " << counter.safe_count() << "\n";
        std::cout << "Lost updates: 0 (guaranteed)\n\n";
    }
}

int main() {
    demonstrate_lost_updates();
    return 0;
}
```

---

## 7. Prevention Strategies

### Strategy 1: Avoid Shared Mutable State

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>

// BEST: No shared state - each thread has its own data
void no_sharing_example() {
    std::cout << "=== Strategy 1: No Shared State ===\n\n";

    std::vector<int> data(1000000, 1);
    const int num_threads = 4;
    std::vector<int> partial_sums(num_threads, 0);
    std::vector<std::thread> threads;

    size_t chunk_size = data.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&data, &partial_sums, i, chunk_size]() {
            size_t start = i * chunk_size;
            size_t end = (i == 3) ? data.size() : (i + 1) * chunk_size;

            // Each thread writes to its own element - no sharing!
            partial_sums[i] = std::accumulate(data.begin() + start, data.begin() + end, 0);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int total = std::accumulate(partial_sums.begin(), partial_sums.end(), 0);
    std::cout << "Total sum: " << total << "\n";
    std::cout << "No synchronization needed!\n\n";
}

int main() {
    no_sharing_example();
    return 0;
}
```

### Strategy 2: Immutable Data

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <memory>

class ImmutableData {
public:
    ImmutableData(std::vector<int> data) : data_(std::move(data)) {}

    // Read-only access - safe for concurrent reading
    int get(size_t index) const {
        return data_[index];
    }

    size_t size() const {
        return data_.size();
    }

private:
    const std::vector<int> data_; // Immutable
};

void immutable_data_example() {
    std::cout << "=== Strategy 2: Immutable Data ===\n\n";

    auto data = std::make_shared<ImmutableData>(std::vector<int>(100, 42));

    std::vector<std::thread> threads;

    // Multiple threads can safely read immutable data
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([data, i]() {
            std::cout << "Thread " << i << ": data[0] = " << data->get(0) << "\n";
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nImmutable data is inherently thread-safe for reading!\n\n";
}

int main() {
    immutable_data_example();
    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Race Condition Hunter
Find and fix race conditions in provided code samples.

### Exercise 2: Lost Update Calculator
Calculate expected vs actual values in concurrent scenarios.

### Exercise 3: Pattern Recognition
Identify race condition patterns in real-world code.

### Exercise 4: Safe Refactoring
Refactor unsafe concurrent code to be thread-safe.

---

## Key Takeaways

1. **Race conditions** depend on timing; **data races** are undefined behavior
2. `++counter` is NOT atomic - it's three separate operations
3. Common patterns: lost update, check-then-act, read-modify-write, TOCTOU
4. Always protect shared mutable state
5. Use atomics for simple counters and flags
6. Use mutexes for complex operations
7. Prefer immutable data and no-sharing designs
8. Code review should specifically look for race patterns
9. Tools like ThreadSanitizer can detect races at runtime
10. Prevention is better than detection - design for thread safety

---

## Summary Checklist

Before moving to Lesson 45, ensure you understand:
- [ ] The difference between race conditions and data races
- [ ] Why `++counter` is not thread-safe
- [ ] Common race condition patterns
- [ ] How to identify potential races in code
- [ ] The lost update problem
- [ ] Read-modify-write issues
- [ ] Check-then-act vulnerabilities
- [ ] TOCTOU problems
- [ ] Prevention strategies (immutability, no-sharing)
- [ ] When to use different synchronization primitives

**Next Lesson**: Mutex and Locks - proper synchronization techniques.

---

**Estimated Word Count**: ~5,100 words
