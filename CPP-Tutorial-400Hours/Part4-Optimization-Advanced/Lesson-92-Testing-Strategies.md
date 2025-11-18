# Lesson 92: Testing Strategies (Unit, Integration, Performance)

**Duration**: 8 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-91, Software Testing Fundamentals

## Table of Contents
1. Testing Pyramid
2. Unit Testing
3. Integration Testing
4. Performance Testing
5. Test Automation
6. Complete Testing Framework

---

## 1. Testing Pyramid

```
        /\
       /  \     E2E Tests (Few)
      /────\
     /      \   Integration Tests (Some)
    /────────\
   /          \ Unit Tests (Many)
  /────────────\
```

---

## 2. Unit Testing with Custom Framework

```cpp
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <chrono>

namespace Testing {

    class TestResult {
    public:
        bool passed;
        std::string message;
        double duration;

        TestResult(bool p, const std::string& msg, double dur)
            : passed(p), message(msg), duration(dur) {}
    };

    class TestCase {
    private:
        std::string name;
        std::function<void()> testFunc;

    public:
        TestCase(const std::string& n, std::function<void()> func)
            : name(n), testFunc(func) {}

        TestResult run() {
            auto start = std::chrono::high_resolution_clock::now();

            try {
                testFunc();
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<double, std::milli>(end - start).count();

                return TestResult(true, "PASS", duration);
            } catch (const std::exception& e) {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<double, std::milli>(end - start).count();

                return TestResult(false, std::string("FAIL: ") + e.what(), duration);
            }
        }

        const std::string& getName() const { return name; }
    };

    class TestSuite {
    private:
        std::string name;
        std::vector<TestCase> tests;

    public:
        explicit TestSuite(const std::string& n) : name(n) {}

        void addTest(const std::string& testName, std::function<void()> testFunc) {
            tests.emplace_back(testName, testFunc);
        }

        void runAll() {
            std::cout << "\n========================================\n";
            std::cout << "Test Suite: " << name << "\n";
            std::cout << "========================================\n";

            int passed = 0;
            int failed = 0;
            double totalTime = 0;

            for (auto& test : tests) {
                std::cout << "Running: " << test.getName() << "... ";

                TestResult result = test.run();
                totalTime += result.duration;

                if (result.passed) {
                    std::cout << "✓ PASS (" << result.duration << " ms)\n";
                    passed++;
                } else {
                    std::cout << "✗ " << result.message
                              << " (" << result.duration << " ms)\n";
                    failed++;
                }
            }

            std::cout << "\n----------------------------------------\n";
            std::cout << "Results: " << passed << " passed, " << failed << " failed\n";
            std::cout << "Total Time: " << totalTime << " ms\n";
            std::cout << "========================================\n";
        }
    };

    // Assertion helpers
    class Assert {
    public:
        static void equals(int expected, int actual) {
            if (expected != actual) {
                throw std::runtime_error("Expected " + std::to_string(expected) +
                                       " but got " + std::to_string(actual));
            }
        }

        static void isTrue(bool condition, const std::string& message = "") {
            if (!condition) {
                throw std::runtime_error(message.empty() ? "Assertion failed" : message);
            }
        }

        static void isFalse(bool condition, const std::string& message = "") {
            if (condition) {
                throw std::runtime_error(message.empty() ? "Assertion failed" : message);
            }
        }

        template<typename T>
        static void notNull(T* ptr) {
            if (ptr == nullptr) {
                throw std::runtime_error("Expected non-null pointer");
            }
        }
    };

} // namespace Testing

// Example: Testing a Stack class
class Stack {
private:
    std::vector<int> data;

public:
    void push(int value) {
        data.push_back(value);
    }

    int pop() {
        if (data.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        int value = data.back();
        data.pop_back();
        return value;
    }

    bool isEmpty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

// Unit Tests for Stack
void testStackCreation() {
    Stack stack;
    Testing::Assert::isTrue(stack.isEmpty(), "New stack should be empty");
    Testing::Assert::equals(0, static_cast<int>(stack.size()));
}

void testStackPush() {
    Stack stack;
    stack.push(10);
    Testing::Assert::isFalse(stack.isEmpty());
    Testing::Assert::equals(1, static_cast<int>(stack.size()));
}

void testStackPop() {
    Stack stack;
    stack.push(10);
    stack.push(20);
    Testing::Assert::equals(20, stack.pop());
    Testing::Assert::equals(10, stack.pop());
    Testing::Assert::isTrue(stack.isEmpty());
}

void testStackPopEmpty() {
    Stack stack;
    try {
        stack.pop();
        Testing::Assert::isTrue(false, "Should have thrown exception");
    } catch (const std::runtime_error&) {
        // Expected
    }
}

//---

## 3. Integration Testing

```cpp
// Database integration test
class DatabaseIntegrationTest {
private:
    std::unique_ptr<Database> testDB;

public:
    void setUp() {
        testDB = std::make_unique<Database>(":memory:"); // SQLite in-memory
        testDB->createTables();
    }

    void tearDown() {
        testDB.reset();
    }

    void testUserCRUD() {
        // Create
        User user{0, "test@example.com", "password"};
        int userId = testDB->insertUser(user);
        Testing::Assert::isTrue(userId > 0);

        // Read
        auto retrieved = testDB->getUserById(userId);
        Testing::Assert::notNull(retrieved.get());
        Testing::Assert::equals(user.email, retrieved->email);

        // Update
        retrieved->email = "updated@example.com";
        testDB->updateUser(*retrieved);

        auto updated = testDB->getUserById(userId);
        Testing::Assert::equals("updated@example.com", updated->email);

        // Delete
        testDB->deleteUser(userId);
        auto deleted = testDB->getUserById(userId);
        Testing::Assert::equals(nullptr, deleted.get());
    }
};
```

---

## 4. Performance Testing

```cpp
namespace PerformanceTesting {

    class Benchmark {
    private:
        std::string name;
        std::function<void()> func;
        int iterations;

    public:
        Benchmark(const std::string& n, std::function<void()> f, int iters)
            : name(n), func(f), iterations(iters) {}

        void run() {
            std::cout << "\nBenchmark: " << name << "\n";
            std::cout << "Iterations: " << iterations << "\n";

            // Warm-up
            for (int i = 0; i < 10; ++i) {
                func();
            }

            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < iterations; ++i) {
                func();
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(end - start).count();

            double avgTime = duration / iterations;
            double opsPerSec = 1000.0 / avgTime;

            std::cout << "Total Time: " << duration << " ms\n";
            std::cout << "Average Time: " << avgTime << " ms\n";
            std::cout << "Operations/sec: " << opsPerSec << "\n";
        }
    };

} // namespace PerformanceTesting
```

---

## 5. Complete Testing Demo

```cpp
int main() {
    std::cout << "=== Testing Framework Demo ===\n";

    // Unit Tests
    Testing::TestSuite unitTests("Stack Unit Tests");
    unitTests.addTest("Stack Creation", testStackCreation);
    unitTests.addTest("Stack Push", testStackPush);
    unitTests.addTest("Stack Pop", testStackPop);
    unitTests.addTest("Stack Pop Empty", testStackPopEmpty);
    unitTests.runAll();

    // Performance Tests
    PerformanceTesting::Benchmark bench(
        "Vector Push Back",
        []() {
            std::vector<int> v;
            for (int i = 0; i < 1000; ++i) {
                v.push_back(i);
            }
        },
        10000
    );
    bench.run();

    return 0;
}
```

---

## Summary

✅ **Unit Testing**: Test individual components
✅ **Integration Testing**: Test component interactions
✅ **Performance Testing**: Benchmark critical paths
✅ **Test Automation**: Continuous testing

**Compilation**:
```bash
g++ -std=c++17 -O3 testing_framework.cpp -o tests
./tests
```
