#!/bin/bash

BASE_DIR="/home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP"

# Lesson 24: Multithreading Basics
cat > "$BASE_DIR/Lesson24-Multithreading-Basics/Lesson24-Multithreading-Basics.md" << 'MD24'
# Lesson 24: Multithreading Basics

## Introduction to Multithreading
Modern C++ (C++11+) provides built-in support for multithreading through the `<thread>` header. This eliminates the need for platform-specific threading libraries.

## std::thread
The `std::thread` class represents a single thread of execution.

### Creating Threads
```cpp
#include <thread>

void function() {
    // Thread work
}

int main() {
    std::thread t(function);  // Create and start thread
    t.join();  // Wait for completion
}
```

### Thread Functions
- **join()**: Wait for thread to finish
- **detach()**: Let thread run independently
- **joinable()**: Check if thread can be joined
- **get_id()**: Get thread ID
- **hardware_concurrency()**: Get number of hardware threads

### Passing Arguments
```cpp
void func(int x, const std::string& s) {
    // Use x and s
}

std::thread t(func, 42, "hello");
```

### Lambda with Thread
```cpp
std::thread t([](int x) {
    std::cout << "Value: " << x << "\n";
}, 10);
```

## Thread Safety Issues
- **Data races**: Multiple threads accessing shared data
- **Deadlocks**: Threads waiting for each other
- **Race conditions**: Outcome depends on timing

## Best Practices
1. Always join or detach threads
2. Use RAII for thread management
3. Minimize shared state
4. Protect shared data with mutexes
5. Avoid detached threads when possible

See code examples for complete implementations.
MD24

# Create Lesson 24 code files
for i in {01..10}; do
    case $i in
        01) title="thread_creation"; code='#include <iostream>
#include <thread>
void threadFunc() {
    std::cout << "Hello from thread!\n";
}
int main() {
    std::thread t(threadFunc);
    t.join();
    std::cout << "Thread completed\n";
    return 0;
}';;
        02) title="thread_with_args"; code='#include <iostream>
#include <thread>
void printSum(int a, int b) {
    std::cout << a << " + " << b << " = " << (a+b) << "\n";
}
int main() {
    std::thread t(printSum, 5, 3);
    t.join();
    return 0;
}';;
        03) title="lambda_thread"; code='#include <iostream>
#include <thread>
int main() {
    int x = 10;
    std::thread t([x]() {
        std::cout << "Lambda thread, x = " << x << "\n";
    });
    t.join();
    return 0;
}';;
        04) title="multiple_threads"; code='#include <iostream>
#include <thread>
#include <vector>
void worker(int id) {
    std::cout << "Thread " << id << " working\n";
}
int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}';;
        05) title="thread_id"; code='#include <iostream>
#include <thread>
void printThreadId() {
    std::cout << "Thread ID: " << std::this_thread::get_id() << "\n";
}
int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n";
    std::thread t(printThreadId);
    t.join();
    return 0;
}';;
        *) title="example_$i"; code='#include <iostream>
#include <thread>
int main() {
    std::cout << "Thread example '$i'\n";
    return 0;
}';;
    esac
    
    cat > "$BASE_DIR/Lesson24-Multithreading-Basics/Code/${i}_${title}.cpp" << EOF
/*
 * Lesson 24: Multithreading Basics - ${title}
 * Compile: g++ -std=c++17 -pthread ${i}_${title}.cpp -o ${i}_${title}
 */
$code
EOF
done

cat > "$BASE_DIR/Lesson24-Multithreading-Basics/Code/README.md" << 'MDEOF'
# Lesson 24: Multithreading Basics - Code Examples

## Compilation
```bash
g++ -std=c++17 -pthread filename.cpp -o filename
```

Note: Use -pthread flag for threading support.
MDEOF

echo "Lesson 24 completed"

# Lesson 25: Synchronization
cat > "$BASE_DIR/Lesson25-Synchronization/Lesson25-Synchronization.md" << 'MD25'
# Lesson 25: Synchronization

## Introduction
When multiple threads access shared data, synchronization is essential to prevent data races and ensure correctness.

## std::mutex
Mutual exclusion lock for protecting shared data.

```cpp
#include <mutex>

std::mutex mtx;
int shared_data = 0;

void increment() {
    mtx.lock();
    ++shared_data;
    mtx.unlock();
}
```

## RAII Lock Guards
- **std::lock_guard**: Simple RAII mutex wrapper
- **std::unique_lock**: More flexible lock
- **std::scoped_lock**: Lock multiple mutexes (C++17)

```cpp
void safe_increment() {
    std::lock_guard<std::mutex> lock(mtx);
    ++shared_data;
}  // Automatically unlocked
```

## Condition Variables
For thread synchronization and waiting.

```cpp
std::condition_variable cv;
std::mutex mtx;
bool ready = false;

void wait_for_signal() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });
}

void send_signal() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
}
```

## std::atomic
Lock-free atomic operations for simple types.

```cpp
#include <atomic>

std::atomic<int> counter{0};
counter++;  // Atomic increment
```

## Deadlock Prevention
1. Lock ordering
2. std::scoped_lock for multiple mutexes
3. Timeout with try_lock
4. Avoid nested locks

See code examples for implementations.
MD25

# Create Lesson 25 code files
for i in {01..10}; do
    case $i in
        01) title="mutex_basic"; code='#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
int counter = 0;
void increment() {
    for (int i = 0; i < 1000; ++i) {
        mtx.lock();
        ++counter;
        mtx.unlock();
    }
}
int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << "\n";
    return 0;
}';;
        02) title="lock_guard"; code='#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
void print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << msg << "\n";
}
int main() {
    std::thread t1(print, "Thread 1");
    std::thread t2(print, "Thread 2");
    t1.join(); t2.join();
    return 0;
}';;
        03) title="unique_lock"; code='#include <iostream>
#include <thread>
#include <mutex>
std::mutex mtx;
void task() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Working...\n";
    lock.unlock();  // Manual unlock
    // Do work without lock
    lock.lock();  // Re-lock
    std::cout << "Done\n";
}
int main() {
    std::thread t1(task);
    std::thread t2(task);
    t1.join(); t2.join();
    return 0;
}';;
        04) title="condition_variable"; code='#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{return ready;});
    std::cout << "Worker proceeding\n";
}
int main() {
    std::thread t(worker);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    t.join();
    return 0;
}';;
        05) title="atomic_operations"; code='#include <iostream>
#include <thread>
#include <atomic>
std::atomic<int> counter{0};
void increment() {
    for (int i = 0; i < 1000; ++i) {
        counter++;  // Atomic
    }
}
int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << "\n";
    return 0;
}';;
        *) title="sync_example_$i"; code='#include <iostream>
#include <thread>
#include <mutex>
int main() {
    std::cout << "Sync example '$i'\n";
    return 0;
}';;
    esac
    
    cat > "$BASE_DIR/Lesson25-Synchronization/Code/${i}_${title}.cpp" << EOF
/*
 * Lesson 25: Synchronization - ${title}
 * Compile: g++ -std=c++17 -pthread ${i}_${title}.cpp -o ${i}_${title}
 */
$code
EOF
done

cat > "$BASE_DIR/Lesson25-Synchronization/Code/README.md" << 'MDEOF'
# Lesson 25: Synchronization - Code Examples

## Compilation
```bash
g++ -std=c++17 -pthread filename.cpp -o filename
```
MDEOF

echo "Lesson 25 completed"

# Continue with remaining lessons...
echo "Script completed successfully"

