/*
 * Lesson 21: Move Semantics
 * File: 09_performance_benchmark.cpp
 *
 * Performance Benchmarks: Copy vs Move
 *
 * This program provides comprehensive performance benchmarks comparing
 * copy and move operations for different data structures and sizes.
 *
 * Compile: g++ -std=c++17 -Wall -O2 09_performance_benchmark.cpp -o 09_performance_benchmark
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <memory>

// Helper class for timing
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    std::string name;

public:
    Timer(const std::string& n) : name(n) {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "  " << name << ": " << duration.count() << " μs\n";
    }
};

// Test class with dynamically allocated data
class LargeObject {
private:
    std::vector<double> data;
    std::string name;

public:
    LargeObject(size_t size, const std::string& n = "")
        : data(size), name(n) {}

    LargeObject(const LargeObject& other)
        : data(other.data), name(other.name) {}

    LargeObject(LargeObject&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name)) {}

    LargeObject& operator=(const LargeObject& other) {
        data = other.data;
        name = other.name;
        return *this;
    }

    LargeObject& operator=(LargeObject&& other) noexcept {
        data = std::move(other.data);
        name = std::move(other.name);
        return *this;
    }

    size_t size() const { return data.size(); }
};

// Benchmark function
template<typename Func>
double benchmark(const std::string& name, Func func, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        func();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avgTime = static_cast<double>(duration.count()) / iterations;

    std::cout << "  " << std::left << std::setw(40) << name
              << std::right << std::setw(10) << std::fixed << std::setprecision(2)
              << avgTime << " μs\n";

    return avgTime;
}

int main() {
    std::cout << "=== Performance Benchmark: Copy vs Move ===\n\n";

    // 1. String performance
    std::cout << "1. STRING OPERATIONS (1000 iterations):\n";
    {
        const std::string source(10000, 'x');

        std::cout << "String size: " << source.size() << " bytes\n\n";

        auto copyTime = benchmark("Copy construction", [&]() {
            std::string temp = source;
        });

        auto moveTime = benchmark("Move construction", [&]() {
            std::string temp = source;
            std::string moved = std::move(temp);
        });

        std::cout << "\nSpeedup: " << (copyTime / moveTime) << "x\n";
    }
    std::cout << "\n";

    // 2. Vector performance
    std::cout << "2. VECTOR OPERATIONS (1000 iterations):\n";
    {
        const std::vector<int> source(100000, 42);

        std::cout << "Vector size: " << source.size() << " elements\n\n";

        auto copyTime = benchmark("Copy construction", [&]() {
            std::vector<int> temp = source;
        });

        auto moveTime = benchmark("Move construction", [&]() {
            std::vector<int> temp = source;
            std::vector<int> moved = std::move(temp);
        });

        std::cout << "\nSpeedup: " << (copyTime / moveTime) << "x\n";
    }
    std::cout << "\n";

    // 3. Large object performance
    std::cout << "3. LARGE OBJECT (1000 iterations):\n";
    {
        const LargeObject source(50000, "test");

        std::cout << "Object data size: " << source.size() << " elements\n\n";

        auto copyTime = benchmark("Copy construction", [&]() {
            LargeObject temp = source;
        });

        auto moveTime = benchmark("Move construction", [&]() {
            LargeObject temp = source;
            LargeObject moved = std::move(temp);
        });

        std::cout << "\nSpeedup: " << (copyTime / moveTime) << "x\n";
    }
    std::cout << "\n";

    // 4. Container of objects
    std::cout << "4. VECTOR OF OBJECTS (100 iterations):\n";
    {
        std::vector<LargeObject> source;
        for (int i = 0; i < 100; ++i) {
            source.emplace_back(1000, "obj" + std::to_string(i));
        }

        std::cout << "Vector size: " << source.size() << " objects\n\n";

        auto copyTime = benchmark("Copy container", [&]() {
            std::vector<LargeObject> temp = source;
        }, 100);

        auto moveTime = benchmark("Move container", [&]() {
            std::vector<LargeObject> temp = source;
            std::vector<LargeObject> moved = std::move(temp);
        }, 100);

        std::cout << "\nSpeedup: " << (copyTime / moveTime) << "x\n";
    }
    std::cout << "\n";

    // 5. Small object overhead
    std::cout << "5. SMALL OBJECT (10000 iterations):\n";
    {
        struct SmallObj {
            int a, b, c;
        };

        SmallObj source{1, 2, 3};

        std::cout << "Object size: " << sizeof(SmallObj) << " bytes\n\n";

        auto copyTime = benchmark("Copy small object", [&]() {
            SmallObj temp = source;
        }, 10000);

        auto moveTime = benchmark("Move small object", [&]() {
            SmallObj temp = source;
            SmallObj moved = std::move(temp);
        }, 10000);

        std::cout << "\nNote: For small objects, move is not faster\n";
        std::cout << "Overhead ratio: " << (moveTime / copyTime) << "x\n";
    }
    std::cout << "\n";

    // 6. String Small String Optimization (SSO)
    std::cout << "6. STRING SSO vs HEAP (1000 iterations):\n";
    {
        const std::string shortStr = "short";  // SSO
        const std::string longStr(1000, 'x');  // Heap

        std::cout << "Short string length: " << shortStr.length() << " (SSO)\n";
        std::cout << "Long string length: " << longStr.length() << " (heap)\n\n";

        auto shortCopy = benchmark("Copy short string", [&]() {
            std::string temp = shortStr;
        });

        auto shortMove = benchmark("Move short string", [&]() {
            std::string temp = shortStr;
            std::string moved = std::move(temp);
        });

        auto longCopy = benchmark("Copy long string", [&]() {
            std::string temp = longStr;
        });

        auto longMove = benchmark("Move long string", [&]() {
            std::string temp = longStr;
            std::string moved = std::move(temp);
        });

        std::cout << "\nShort string speedup: " << (shortCopy / shortMove) << "x\n";
        std::cout << "Long string speedup: " << (longCopy / longMove) << "x\n";
        std::cout << "Note: SSO strings don't benefit much from move\n";
    }
    std::cout << "\n";

    // 7. Vector push_back vs emplace_back
    std::cout << "7. PUSH_BACK VS EMPLACE_BACK (1000 iterations):\n";
    {
        const int size = 100;

        auto pushCopy = benchmark("push_back (copy)", [&]() {
            std::vector<LargeObject> vec;
            vec.reserve(size);
            LargeObject obj(1000);
            for (int i = 0; i < size; ++i) {
                vec.push_back(obj);
            }
        }, 1000);

        auto pushMove = benchmark("push_back (move)", [&]() {
            std::vector<LargeObject> vec;
            vec.reserve(size);
            for (int i = 0; i < size; ++i) {
                vec.push_back(LargeObject(1000));
            }
        }, 1000);

        auto emplace = benchmark("emplace_back", [&]() {
            std::vector<LargeObject> vec;
            vec.reserve(size);
            for (int i = 0; i < size; ++i) {
                vec.emplace_back(1000);
            }
        }, 1000);

        std::cout << "\npush_back(copy) vs push_back(move): "
                  << (pushCopy / pushMove) << "x\n";
        std::cout << "push_back(move) vs emplace_back: "
                  << (pushMove / emplace) << "x\n";
    }
    std::cout << "\n";

    // 8. Return value optimization
    std::cout << "8. RETURN VALUE OPTIMIZATION (10000 iterations):\n";
    {
        auto withMove = []() {
            std::vector<int> vec(1000);
            return std::move(vec);  // BAD: prevents RVO
        };

        auto withoutMove = []() {
            std::vector<int> vec(1000);
            return vec;  // GOOD: allows RVO
        };

        auto moveTime = benchmark("Return with std::move", withMove, 10000);
        auto rvoTime = benchmark("Return without move (RVO)", withoutMove, 10000);

        std::cout << "\nRVO speedup: " << (moveTime / rvoTime) << "x\n";
        std::cout << "Note: RVO can be faster than move!\n";
    }
    std::cout << "\n";

    // 9. Memory allocation comparison
    std::cout << "9. MEMORY OPERATIONS (1000 iterations):\n";
    {
        const size_t size = 100000;

        auto allocCopy = benchmark("Allocate + Copy", [&]() {
            std::vector<int> source(size, 42);
            std::vector<int> dest = source;  // Allocate + copy
        }, 1000);

        auto allocMove = benchmark("Allocate + Move", [&]() {
            std::vector<int> source(size, 42);
            std::vector<int> dest = std::move(source);  // Just pointer copy
        }, 1000);

        std::cout << "\nSpeedup: " << (allocCopy / allocMove) << "x\n";
        std::cout << "Move avoids: " << size * sizeof(int) / 1024 << " KB allocation\n";
    }
    std::cout << "\n";

    // 10. Summary
    std::cout << "10. SUMMARY:\n";
    std::cout << "Move is faster when:\n";
    std::cout << "  - Object manages heap memory (vector, string, unique_ptr)\n";
    std::cout << "  - Object is large (> cache line size)\n";
    std::cout << "  - String exceeds SSO threshold\n";
    std::cout << "  - Container has many elements\n\n";

    std::cout << "Move has no benefit when:\n";
    std::cout << "  - Object is small (POD types, small structs)\n";
    std::cout << "  - String uses SSO\n";
    std::cout << "  - Object has no dynamic allocation\n";
    std::cout << "  - Compiler can apply RVO\n\n";

    std::cout << "Best practices:\n";
    std::cout << "  - Profile your specific use case\n";
    std::cout << "  - Don't move small objects\n";
    std::cout << "  - Don't prevent RVO with std::move on return\n";
    std::cout << "  - Use emplace operations when possible\n";
    std::cout << "  - Reserve capacity to avoid reallocations\n";

    return 0;
}
