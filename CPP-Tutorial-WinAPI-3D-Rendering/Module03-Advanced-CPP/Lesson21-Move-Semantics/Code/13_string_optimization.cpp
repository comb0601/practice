/*
 * Lesson 21: Move Semantics
 * File: 13_string_optimization.cpp
 *
 * String Optimization with Move Semantics
 *
 * This program explores how move semantics optimizes string operations,
 * including Small String Optimization (SSO) and heap allocation.
 *
 * Compile: g++ -std=c++17 -Wall -O2 13_string_optimization.cpp -o 13_string_optimization
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

// Custom string class to demonstrate concepts
class MyString {
private:
    char* data;
    size_t length;
    size_t capacity;
    static constexpr size_t SSO_BUFFER_SIZE = 16;
    char ssoBuffer[SSO_BUFFER_SIZE];
    static int copyCount;
    static int moveCount;

public:
    // Constructor
    MyString(const char* str = "") {
        length = std::strlen(str);

        if (length < SSO_BUFFER_SIZE) {
            // Use SSO buffer
            std::strcpy(ssoBuffer, str);
            data = ssoBuffer;
            capacity = SSO_BUFFER_SIZE;
            std::cout << "  [SSO] String created: \"" << data << "\"\n";
        } else {
            // Allocate on heap
            capacity = length + 1;
            data = new char[capacity];
            std::strcpy(data, str);
            std::cout << "  [HEAP] String created: \"" << data << "\"\n";
        }
    }

    // Copy constructor
    MyString(const MyString& other) : length(other.length), capacity(other.capacity) {
        ++copyCount;

        if (other.data == other.ssoBuffer) {
            // Copy SSO buffer
            std::strcpy(ssoBuffer, other.ssoBuffer);
            data = ssoBuffer;
            std::cout << "  [SSO] String copied: \"" << data << "\"\n";
        } else {
            // Allocate and copy heap data
            data = new char[capacity];
            std::strcpy(data, other.data);
            std::cout << "  [HEAP] String copied: \"" << data << "\"\n";
        }
    }

    // Move constructor
    MyString(MyString&& other) noexcept : length(other.length), capacity(other.capacity) {
        ++moveCount;

        if (other.data == other.ssoBuffer) {
            // Must copy SSO buffer (can't steal it)
            std::strcpy(ssoBuffer, other.ssoBuffer);
            data = ssoBuffer;
            std::cout << "  [SSO] String moved (copied buffer): \"" << data << "\"\n";
        } else {
            // Steal heap pointer
            data = other.data;
            other.data = nullptr;
            other.length = 0;
            std::cout << "  [HEAP] String moved (stole pointer): \"" << data << "\"\n";
        }
    }

    ~MyString() {
        if (data != ssoBuffer) {
            delete[] data;
        }
    }

    const char* c_str() const { return data ? data : ""; }
    size_t size() const { return length; }
    bool usesSSO() const { return data == ssoBuffer; }

    static void resetCounters() { copyCount = moveCount = 0; }
    static void printCounters() {
        std::cout << "  Copies: " << copyCount << ", Moves: " << moveCount << "\n";
    }
};

int MyString::copyCount = 0;
int MyString::moveCount = 0;

// Benchmark function
template<typename Func>
double timeit(Func func, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / (double)iterations;
}

int main() {
    std::cout << "=== String Optimization with Move Semantics ===\n\n";

    // 1. Small String Optimization demonstration
    std::cout << "1. SMALL STRING OPTIMIZATION (SSO):\n";
    {
        MyString short1("Short");     // Uses SSO
        MyString long1("This is a very long string that exceeds SSO buffer");  // Heap

        std::cout << "\nShort string uses SSO: " << (short1.usesSSO() ? "Yes" : "No") << "\n";
        std::cout << "Long string uses SSO: " << (long1.usesSSO() ? "Yes" : "No") << "\n";

        MyString::resetCounters();
        std::cout << "\nMoving short string:\n";
        MyString short2 = std::move(short1);
        MyString::printCounters();

        MyString::resetCounters();
        std::cout << "\nMoving long string:\n";
        MyString long2 = std::move(long1);
        MyString::printCounters();

        std::cout << "\nNote: SSO strings must be copied on move\n";
        std::cout << "      Long strings can have pointer stolen\n";
    }
    std::cout << "\n";

    // 2. std::string move performance
    std::cout << "2. STD::STRING MOVE PERFORMANCE:\n";
    {
        std::string shortStr = "Short";
        std::string longStr(1000, 'x');

        std::cout << "Short string length: " << shortStr.length() << " (likely SSO)\n";
        std::cout << "Long string length: " << longStr.length() << " (heap)\n\n";

        auto shortCopyTime = timeit([&]() {
            std::string temp = shortStr;
        });

        auto shortMoveTime = timeit([&]() {
            std::string temp = shortStr;
            std::string moved = std::move(temp);
        });

        auto longCopyTime = timeit([&]() {
            std::string temp = longStr;
        });

        auto longMoveTime = timeit([&]() {
            std::string temp = longStr;
            std::string moved = std::move(temp);
        });

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Short string copy: " << shortCopyTime << " μs\n";
        std::cout << "Short string move: " << shortMoveTime << " μs\n";
        std::cout << "Speedup: " << (shortCopyTime / shortMoveTime) << "x\n\n";

        std::cout << "Long string copy: " << longCopyTime << " μs\n";
        std::cout << "Long string move: " << longMoveTime << " μs\n";
        std::cout << "Speedup: " << (longCopyTime / longMoveTime) << "x\n";
    }
    std::cout << "\n";

    // 3. String concatenation with move
    std::cout << "3. STRING CONCATENATION OPTIMIZATION:\n";
    {
        std::cout << "Without move:\n";
        auto noMove = []() {
            std::string result;
            std::string a = "Hello ";
            std::string b = "World ";
            std::string c = "from C++!";
            result = a + b + c;  // Multiple copies
            return result;
        };

        std::cout << "\nWith move:\n";
        auto withMove = []() {
            std::string result;
            std::string a = "Hello ";
            std::string b = "World ";
            std::string c = "from C++!";
            result = std::move(a);
            result += std::move(b);
            result += std::move(c);  // Moves instead of copies
            return result;
        };

        auto noMoveTime = timeit(noMove, 10000);
        auto withMoveTime = timeit(withMove, 10000);

        std::cout << "\nWithout move: " << noMoveTime << " μs\n";
        std::cout << "With move: " << withMoveTime << " μs\n";
        std::cout << "Speedup: " << (noMoveTime / withMoveTime) << "x\n";
    }
    std::cout << "\n";

    // 4. Building large strings
    std::cout << "4. BUILDING LARGE STRINGS:\n";
    {
        std::cout << "Method 1: Multiple concatenations (slow):\n";
        auto method1 = []() {
            std::string result;
            for (int i = 0; i < 100; ++i) {
                result = result + "data";  // Creates temporaries
            }
            return result;
        };

        std::cout << "Method 2: operator+= (better):\n";
        auto method2 = []() {
            std::string result;
            for (int i = 0; i < 100; ++i) {
                result += "data";  // Appends in place
            }
            return result;
        };

        std::cout << "Method 3: Reserve + append (best):\n";
        auto method3 = []() {
            std::string result;
            result.reserve(400);  // Preallocate
            for (int i = 0; i < 100; ++i) {
                result += "data";
            }
            return result;
        };

        auto time1 = timeit(method1, 1000);
        auto time2 = timeit(method2, 1000);
        auto time3 = timeit(method3, 1000);

        std::cout << "\nMethod 1: " << time1 << " μs\n";
        std::cout << "Method 2: " << time2 << " μs (move-enabled)\n";
        std::cout << "Method 3: " << time3 << " μs (best)\n";
    }
    std::cout << "\n";

    // 5. Vector of strings
    std::cout << "5. VECTOR OF STRINGS:\n";
    {
        std::cout << "push_back vs emplace_back:\n";

        auto pushCopy = []() {
            std::vector<std::string> vec;
            vec.reserve(100);
            std::string str = "data";
            for (int i = 0; i < 100; ++i) {
                vec.push_back(str);  // Copy
            }
        };

        auto pushMove = []() {
            std::vector<std::string> vec;
            vec.reserve(100);
            for (int i = 0; i < 100; ++i) {
                vec.push_back(std::string("data"));  // Move
            }
        };

        auto emplace = []() {
            std::vector<std::string> vec;
            vec.reserve(100);
            for (int i = 0; i < 100; ++i) {
                vec.emplace_back("data");  // Construct in place
            }
        };

        auto pushCopyTime = timeit(pushCopy, 1000);
        auto pushMoveTime = timeit(pushMove, 1000);
        auto emplaceTime = timeit(emplace, 1000);

        std::cout << "push_back (copy): " << pushCopyTime << " μs\n";
        std::cout << "push_back (move): " << pushMoveTime << " μs\n";
        std::cout << "emplace_back: " << emplaceTime << " μs (fastest)\n";
    }
    std::cout << "\n";

    // 6. String swapping
    std::cout << "6. STRING SWAPPING:\n";
    {
        std::string str1(10000, 'A');
        std::string str2(10000, 'B');

        std::cout << "Swapping large strings:\n";

        auto swapTime = timeit([&]() {
            std::string temp = str1;
            str1 = str2;
            str2 = temp;
        }, 10000);

        std::cout << "Using copy: " << swapTime << " μs\n";

        auto swapOptTime = timeit([&]() {
            str1.swap(str2);  // O(1) pointer swap
        }, 10000);

        std::cout << "Using swap(): " << swapOptTime << " μs\n";
        std::cout << "Speedup: " << (swapTime / swapOptTime) << "x\n";
    }
    std::cout << "\n";

    // 7. Passing strings to functions
    std::cout << "7. PASSING STRINGS TO FUNCTIONS:\n";
    {
        auto byConstRef = [](const std::string& str) {
            volatile int len = str.length();  // Use the string
            (void)len;
        };

        auto byValue = [](std::string str) {
            volatile int len = str.length();
            (void)len;
        };

        std::string str = "Test string";

        auto constRefTime = timeit([&]() {
            byConstRef(str);
        }, 100000);

        auto valueTime = timeit([&]() {
            byValue(str);  // Copies
        }, 100000);

        auto valueMoveTime = timeit([&]() {
            byValue(std::move(str));  // Moves
            str = "Test string";  // Restore
        }, 100000);

        std::cout << "Pass by const ref: " << constRefTime << " μs (no copy)\n";
        std::cout << "Pass by value (copy): " << valueTime << " μs\n";
        std::cout << "Pass by value (move): " << valueMoveTime << " μs\n";
    }
    std::cout << "\n";

    // 8. Returning strings
    std::cout << "8. RETURNING STRINGS:\n";
    {
        auto returnValue = []() {
            std::string result = "Return value";
            return result;  // RVO or move
        };

        auto returnMove = []() {
            std::string result = "Return value";
            return std::move(result);  // Prevents RVO!
        };

        auto returnRVOTime = timeit(returnValue, 100000);
        auto returnMoveTime = timeit(returnMove, 100000);

        std::cout << "Return by value (RVO): " << returnRVOTime << " μs\n";
        std::cout << "Return with std::move: " << returnMoveTime << " μs\n";
        std::cout << "Note: RVO can be faster than move!\n";
    }
    std::cout << "\n";

    // 9. Summary
    std::cout << "9. OPTIMIZATION SUMMARY:\n";
    std::cout << "SSO (Small String Optimization):\n";
    std::cout << "  - Short strings stored in object (no allocation)\n";
    std::cout << "  - Move doesn't help for SSO strings\n";
    std::cout << "  - Typical SSO buffer: 15-22 bytes\n\n";

    std::cout << "Move optimization for strings:\n";
    std::cout << "  - Significant for long strings (heap allocated)\n";
    std::cout << "  - Minimal benefit for short strings (SSO)\n";
    std::cout << "  - Use emplace_back instead of push_back\n";
    std::cout << "  - Reserve capacity when building strings\n";
    std::cout << "  - Use swap() for O(1) exchange\n";
    std::cout << "  - Don't std::move on return (prevents RVO)\n\n";

    std::cout << "Best practices:\n";
    std::cout << "  1. Reserve capacity for known size\n";
    std::cout << "  2. Use += instead of + for concatenation\n";
    std::cout << "  3. Move into containers (push_back, insert)\n";
    std::cout << "  4. Return by value (enables RVO)\n";
    std::cout << "  5. Use swap() for large string exchange\n";
    std::cout << "  6. Profile to find actual bottlenecks\n";

    return 0;
}
