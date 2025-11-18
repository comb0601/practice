/*
 * Lesson 91 - Example 13: Function Call Overhead
 *
 * Demonstrates the cost of function calls and the benefit of inlining.
 * Compares function calls vs inline functions vs macros.
 */

#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMilliseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    }
};

// Regular function (has call overhead)
int addRegular(int a, int b) {
    return a + b;
}

// Inline function (compiler may eliminate call overhead)
inline int addInline(int a, int b) {
    return a + b;
}

// Force inline (compiler MUST inline if possible)
#ifdef _MSC_VER
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline __attribute__((always_inline))
#endif

FORCE_INLINE int addForceInline(int a, int b) {
    return a + b;
}

// Macro (always inlined, but not type-safe)
#define ADD_MACRO(a, b) ((a) + (b))

// More complex function to show overhead better
int complexFunctionRegular(int x) {
    return (x * x) + (x * 2) + 1;
}

inline int complexFunctionInline(int x) {
    return (x * x) + (x * 2) + 1;
}

FORCE_INLINE int complexFunctionForceInline(int x) {
    return (x * x) + (x * 2) + 1;
}

#define COMPLEX_MACRO(x) (((x) * (x)) + ((x) * 2) + 1)

int main() {
    std::cout << "=== Function Call Overhead Example ===\n\n";

    const int ITERATIONS = 100000000;
    Timer timer;

    // Test 1: Simple addition
    std::cout << "Test 1: Simple Addition (100 million calls)\n\n";

    // Regular function
    timer.reset();
    volatile long long sum1 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        sum1 += addRegular(i, 1);
    }
    double timeRegular = timer.elapsedMilliseconds();

    // Inline function
    timer.reset();
    volatile long long sum2 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        sum2 += addInline(i, 1);
    }
    double timeInline = timer.elapsedMilliseconds();

    // Force inline function
    timer.reset();
    volatile long long sum3 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        sum3 += addForceInline(i, 1);
    }
    double timeForceInline = timer.elapsedMilliseconds();

    // Macro
    timer.reset();
    volatile long long sum4 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        sum4 += ADD_MACRO(i, 1);
    }
    double timeMacro = timer.elapsedMilliseconds();

    std::cout << "Regular function:     " << timeRegular << " ms\n";
    std::cout << "Inline function:      " << timeInline << " ms\n";
    std::cout << "Force inline:         " << timeForceInline << " ms\n";
    std::cout << "Macro:                " << timeMacro << " ms\n\n";

    // Test 2: Complex function
    std::cout << "Test 2: Complex Function (100 million calls)\n\n";

    timer.reset();
    volatile long long result1 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        result1 += complexFunctionRegular(i);
    }
    double timeComplexRegular = timer.elapsedMilliseconds();

    timer.reset();
    volatile long long result2 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        result2 += complexFunctionInline(i);
    }
    double timeComplexInline = timer.elapsedMilliseconds();

    timer.reset();
    volatile long long result3 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        result3 += complexFunctionForceInline(i);
    }
    double timeComplexForceInline = timer.elapsedMilliseconds();

    timer.reset();
    volatile long long result4 = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        result4 += COMPLEX_MACRO(i);
    }
    double timeComplexMacro = timer.elapsedMilliseconds();

    std::cout << "Regular function:     " << timeComplexRegular << " ms\n";
    std::cout << "Inline function:      " << timeComplexInline << " ms\n";
    std::cout << "Force inline:         " << timeComplexForceInline << " ms\n";
    std::cout << "Macro:                " << timeComplexMacro << " ms\n\n";

    // Analysis
    std::cout << "========== ANALYSIS ==========\n\n";

    std::cout << "Function Call Overhead:\n";
    std::cout << "  - Each function call has cost:\n";
    std::cout << "    1. Push parameters to stack\n";
    std::cout << "    2. Jump to function address\n";
    std::cout << "    3. Execute function\n";
    std::cout << "    4. Return to caller\n";
    std::cout << "  - For tiny functions, overhead > actual work!\n\n";

    std::cout << "Inlining:\n";
    std::cout << "  - Compiler replaces call with function body\n";
    std::cout << "  - No call overhead\n";
    std::cout << "  - Increased code size (trade-off)\n\n";

    std::cout << "When to Inline:\n";
    std::cout << "  ✓ Small functions (1-5 lines)\n";
    std::cout << "  ✓ Frequently called functions\n";
    std::cout << "  ✓ Functions in tight loops\n";
    std::cout << "  ✗ Large functions (increases code size)\n";
    std::cout << "  ✗ Rarely called functions\n\n";

    std::cout << "Inline vs Macro:\n";
    std::cout << "  Inline:  Type-safe, debuggable, C++ preferred\n";
    std::cout << "  Macro:   No type checking, text substitution\n\n";

    std::cout << "Compiler Behavior:\n";
    std::cout << "  - In Release mode, compiler often inlines automatically\n";
    std::cout << "  - 'inline' is a HINT, not a command\n";
    std::cout << "  - Use __forceinline (MSVC) or always_inline (GCC) to force\n\n";

    std::cout << "Expected Results:\n";
    std::cout << "  Debug build:   Regular >> Inline ≈ Macro\n";
    std::cout << "  Release build: Regular ≈ Inline ≈ Macro (all optimized)\n\n";

    std::cout << "Key Takeaway:\n";
    std::cout << "  Let the compiler inline in Release mode.\n";
    std::cout << "  Only force inline for proven bottlenecks.\n";

    return 0;
}
