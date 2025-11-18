# Lesson 25: Function Inlining Strategies

**Duration**: 10 hours
**Difficulty**: Advanced

## Table of Contents
1. Understanding Function Call Overhead
2. Inline Keyword and Compiler Behavior
3. Force Inlining vs Compiler Hints
4. When to Inline and When Not To
5. Template Functions and Automatic Inlining
6. Link-Time Optimization (LTO)
7. Measuring Inlining Impact
8. Best Practices and Pitfalls

---

## 1. Understanding Function Call Overhead

Every function call has overhead: stack frame setup, parameter passing, jump, return.

```cpp
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

// Small function - good candidate for inlining
int square(int x) {
    return x * x;
}

// Using the function in a loop
long long compute_with_calls(int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += square(i);  // Function call overhead each iteration
    }
    return sum;
}

// Manually inlined version
long long compute_inlined(int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i * i;  // No function call
    }
    return sum;
}

void demonstrateOverhead() {
    const int N = 100000000;

    auto start = chrono::high_resolution_clock::now();
    long long result1 = compute_with_calls(N);
    auto end = chrono::high_resolution_clock::now();
    auto time1 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    long long result2 = compute_inlined(N);
    end = chrono::high_resolution_clock::now();
    auto time2 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "With function calls: " << time1 << " ms\n";
    cout << "Manually inlined:    " << time2 << " ms\n";
    cout << "Speedup: " << (double)time1 / time2 << "x\n";
}

int main() {
    cout << "=== FUNCTION CALL OVERHEAD ===\n\n";
    demonstrateOverhead();
    return 0;
}
```

---

## 2. Inline Keyword and Compiler Behavior

The `inline` keyword is a **hint**, not a command. Modern compilers decide for themselves.

```cpp
#include <iostream>

// Inline suggestion
inline int add(int a, int b) {
    return a + b;
}

// Compiler likely inlines this even without keyword (small, simple)
int multiply(int a, int b) {
    return a * b;
}

// Compiler likely WON'T inline this (too complex)
inline int complexFunction(int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result += i * j;
        }
    }
    return result;
}

// Class member functions defined in class are implicitly inline
class Math {
public:
    int square(int x) {  // Implicitly inline
        return x * x;
    }

    int cube(int x);  // Not inline (definition outside)
};

int Math::cube(int x) {
    return x * x * x;
}

// Making it inline
class MathOptimized {
public:
    inline int cube(int x);  // Declare as inline
};

inline int MathOptimized::cube(int x) {  // Define as inline
    return x * x * x;
}

int main() {
    cout << "=== INLINE KEYWORD ===\n\n";

    cout << "Inline keyword is a HINT, not a directive\n";
    cout << "Compiler makes the final decision based on:\n";
    cout << "  • Function size\n";
    cout << "  • Complexity\n";
    cout << "  • Call frequency\n";
    cout << "  • Optimization level (-O2, -O3)\n\n";

    Math m;
    cout << "Square of 5: " << m.square(5) << "\n";
    cout << "Cube of 5: " << m.cube(5) << "\n";

    return 0;
}
```

---

## 3. Force Inlining vs Compiler Hints

Different compilers provide ways to **force** inlining.

```cpp
#include <iostream>
#include <chrono>

using namespace std;

// GCC/Clang: Force inline
#ifdef __GNUC__
#define FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif

// Regular inline (compiler decides)
inline int regularInline(int x) {
    return x * x + x;
}

// Forced inline
FORCE_INLINE int forcedInline(int x) {
    return x * x + x;
}

// Never inline
#ifdef __GNUC__
__attribute__((noinline))
#elif defined(_MSC_VER)
__declspec(noinline)
#endif
int neverInline(int x) {
    return x * x + x;
}

// Benchmark
template<typename Func>
void benchmark(const string& name, Func f, int n) {
    auto start = chrono::high_resolution_clock::now();

    volatile long long sum = 0;  // volatile prevents optimization
    for (int i = 0; i < n; i++) {
        sum += f(i);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << name << ": " << duration.count() << " μs (sum=" << sum << ")\n";
}

int main() {
    cout << "=== FORCE INLINE VS HINTS ===\n\n";

    const int N = 10000000;

    benchmark("Regular inline", regularInline, N);
    benchmark("Forced inline ", forcedInline, N);
    benchmark("Never inline  ", neverInline, N);

    cout << "\nNotes:\n";
    cout << "  • __forceinline (MSVC) or __attribute__((always_inline)) (GCC/Clang)\n";
    cout << "  • Use sparingly - compiler usually knows best\n";
    cout << "  • Can increase code size significantly\n";
    cout << "  • May prevent some optimizations\n";

    return 0;
}
```

---

## 4. When to Inline and When Not To

### Good Candidates for Inlining

```cpp
#include <iostream>

// ✅ GOOD: Small, frequently called
inline int max(int a, int b) {
    return (a > b) ? a : b;
}

// ✅ GOOD: Simple accessor
class Point {
    int x, y;
public:
    int getX() const { return x; }  // Inline
    int getY() const { return y; }  // Inline
};

// ✅ GOOD: Template functions (automatically inline)
template<typename T>
T square(T x) {
    return x * x;
}

// ✅ GOOD: Wrapper around intrinsic
inline int fastAbs(int x) {
    return (x < 0) ? -x : x;
}

// ❌ BAD: Large function
inline void processLargeData(int* data, int size) {
    // 100+ lines of complex logic
    // Inlining would bloat code size
}

// ❌ BAD: Recursive function
inline int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
    // Recursion makes inlining problematic
}

// ❌ BAD: Virtual function
class Base {
public:
    virtual int compute(int x) {  // Can't be inlined polymorphically
        return x * 2;
    }
};

// ❌ BAD: Contains loops
inline long long sumRange(int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i;
    }
    return sum;  // Too complex for inlining benefit
}

void demonstrateGoodBad() {
    cout << "=== WHEN TO INLINE ===\n\n";

    cout << "✅ INLINE WHEN:\n";
    cout << "  • Function is small (1-5 lines)\n";
    cout << "  • Called frequently in hot paths\n";
    cout << "  • Getters/setters\n";
    cout << "  • Simple mathematical operations\n";
    cout << "  • Template functions\n\n";

    cout << "❌ DON'T INLINE WHEN:\n";
    cout << "  • Function is large (>10 lines)\n";
    cout << "  • Contains loops or complex logic\n";
    cout << "  • Recursive\n";
    cout << "  • Virtual (can't be)\n";
    cout << "  • Rarely called\n";
    cout << "  • Would cause excessive code bloat\n";
}

int main() {
    demonstrateGoodBad();
    return 0;
}
```

---

## 5. Template Functions and Automatic Inlining

Templates are automatically inline and defined in headers.

```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Template function (automatically inline)
template<typename T>
T add(T a, T b) {
    return a + b;
}

// Template class methods (inline)
template<typename T>
class Vector3D {
    T x, y, z;

public:
    Vector3D(T x, T y, T z) : x(x), y(y), z(z) {}

    // These are all implicitly inline
    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }

    T dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    T lengthSquared() const {
        return x*x + y*y + z*z;
    }
};

// Performance benefit of template inlining
template<typename T>
T genericSum(const vector<T>& data) {
    T sum = 0;
    for (const T& val : data) {
        sum += val;  // Type-specific addition (inlined)
    }
    return sum;
}

void demonstrateTemplateInlining() {
    const int SIZE = 1000000;
    vector<int> intData(SIZE, 1);
    vector<double> doubleData(SIZE, 1.5);

    auto start = chrono::high_resolution_clock::now();
    int sum1 = genericSum(intData);
    auto end = chrono::high_resolution_clock::now();
    auto time1 = chrono::duration_cast<chrono::microseconds>(end - start);

    start = chrono::high_resolution_clock::now();
    double sum2 = genericSum(doubleData);
    end = chrono::high_resolution_clock::now();
    auto time2 = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Integer sum: " << sum1 << " in " << time1.count() << " μs\n";
    cout << "Double sum:  " << sum2 << " in " << time2.count() << " μs\n";
    cout << "\nTemplate functions are automatically specialized and inlined!\n";
}

int main() {
    cout << "=== TEMPLATE INLINING ===\n\n";
    demonstrateTemplateInlining();

    cout << "\nKey Points:\n";
    cout << "  • Template functions must be in headers\n";
    cout << "  • Automatically inline\n";
    cout << "  • Compiler generates specialized code\n";
    cout << "  • Zero abstraction overhead when optimized\n";

    return 0;
}
```

---

## 6. Link-Time Optimization (LTO)

LTO enables inlining across compilation units.

```cpp
/*
Without LTO: Functions in different .cpp files can't be inlined
With LTO: Compiler can inline across files

Enable LTO:
  GCC/Clang: -flto
  MSVC: /GL (generate), /LTCG (link)

Example project structure:

// math.h
int square(int x);

// math.cpp
int square(int x) {
    return x * x;
}

// main.cpp
#include "math.h"
int main() {
    int result = square(5);  // Without LTO: function call
                             // With LTO: inlined!
}

Compile:
  g++ -O3 -flto math.cpp main.cpp -o program

Benefits:
  • Whole-program optimization
  • Cross-file inlining
  • Better dead code elimination
  • 5-15% performance improvement

Downsides:
  • Longer compile/link times
  • More memory usage during build
  • Harder to debug
*/

#include <iostream>

void demonstrateLTO() {
    cout << "=== LINK-TIME OPTIMIZATION ===\n\n";

    cout << "Enable LTO for maximum inlining:\n\n";

    cout << "GCC/Clang:\n";
    cout << "  g++ -O3 -flto -fuse-linker-plugin file1.cpp file2.cpp\n\n";

    cout << "MSVC:\n";
    cout << "  cl /O2 /GL file1.cpp file2.cpp /link /LTCG\n\n";

    cout << "CMake:\n";
    cout << "  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)\n\n";

    cout << "Benefits:\n";
    cout << "  ✓ Inlining across files\n";
    cout << "  ✓ Better dead code elimination\n";
    cout << "  ✓ Improved constant propagation\n";
    cout << "  ✓ Typically 5-15% faster executables\n";
}

int main() {
    demonstrateLTO();
    return 0;
}
```

---

## 7. Measuring Inlining Impact

```cpp
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

// Check if function was inlined using compiler reports:
// GCC: -Winline
// Clang: -Rpass=inline
// MSVC: /Qvec-report:2

// Test case: Vector dot product

struct Vector3 {
    float x, y, z;
};

// Non-inline version
float dot_noinline(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Inline version
inline float dot_inline(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Force inline version
#ifdef _MSC_VER
__forceinline float dot_forced(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
#else
__attribute__((always_inline)) inline float dot_forced(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
#endif

template<typename Func>
void benchmarkDot(const string& name, Func f) {
    const int N = 10000000;
    Vector3 a = {1.0f, 2.0f, 3.0f};
    Vector3 b = {4.0f, 5.0f, 6.0f};

    auto start = chrono::high_resolution_clock::now();

    volatile float sum = 0;
    for (int i = 0; i < N; i++) {
        sum += f(a, b);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << name << ": " << duration.count() << " ms\n";
}

void measureInlining() {
    cout << "=== MEASURING INLINING IMPACT ===\n\n";

    benchmarkDot("No inline   ", dot_noinline);
    benchmarkDot("Inline hint ", dot_inline);
    benchmarkDot("Force inline", dot_forced);

    cout << "\nTo verify inlining, check:\n";
    cout << "  • Assembly output (objdump, Compiler Explorer)\n";
    cout << "  • Profiler (function should not appear in call stack)\n";
    cout << "  • Compiler optimization reports\n";
}

int main() {
    measureInlining();
    return 0;
}
```

---

## 8. Best Practices and Pitfalls

```cpp
#include <iostream>

/*
BEST PRACTICES:

1. Let the compiler decide (use inline as hint)
   ✅ inline int square(int x) { return x * x; }

2. Keep inline functions small
   ✅ 1-5 lines ideal
   ❌ Avoid >20 lines

3. Put inline functions in headers
   ✅ Enables inlining across translation units
   ❌ Putting in .cpp defeats the purpose

4. Use templates for generic small functions
   ✅ Automatically inline + type safety

5. Enable optimization flags
   ✅ -O2 or -O3 (GCC/Clang)
   ✅ /O2 (MSVC)

6. Consider LTO for release builds
   ✅ Enables cross-file inlining

7. Profile before forcing inline
   ✅ Measure actual impact
   ❌ Don't blindly force inline

COMMON PITFALLS:

1. Code bloat
   ❌ Inlining large functions increases binary size
   ✅ Can hurt I-cache performance

2. Longer compile times
   ❌ Inline functions in headers = recompile everything

3. Harder debugging
   ❌ Inlined code doesn't appear in stack traces
   ✅ Use Debug builds without inlining

4. Virtual functions can't be inlined (polymorphic calls)
   ❌ virtual inline int foo() { ... }
   ✅ Only inlined for static calls

5. Recursive functions
   ❌ inline int factorial(int n) { return n * factorial(n-1); }
   ✅ Partial inlining possible, but limited

6. Function pointers
   ❌ Taking address of inline function prevents inlining
   ✅ auto f = &inlineFunc; // Can't inline this call

OPTIMIZATION TIPS:

1. Profile hot paths first
   → Find functions called millions of times

2. Inline small helpers in hot loops
   → Maximum impact

3. Use constexpr for compile-time computation
   → Better than runtime inline

4. Consider __restrict for pointer parameters
   → Helps compiler optimize

5. Check compiler output
   → Verify inlining actually happened
*/

// Example: Good inline usage
class GameObject {
    float x, y, z;
    bool active;

public:
    // Good: small getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    bool isActive() const { return active; }

    // Good: small setter
    void setActive(bool a) { active = a; }

    // Bad: complex logic
    void updatePhysics(float deltaTime, const World& world) {
        // Many lines of complex physics
        // Don't inline this!
    }
};

void demonstrateBestPractices() {
    cout << "=== INLINING BEST PRACTICES ===\n\n";

    cout << "DO:\n";
    cout << "  ✓ Inline small, frequently-called functions\n";
    cout << "  ✓ Use optimization flags (-O2/-O3)\n";
    cout << "  ✓ Profile before optimizing\n";
    cout << "  ✓ Keep inline functions simple\n";
    cout << "  ✓ Put inline functions in headers\n\n";

    cout << "DON'T:\n";
    cout << "  ✗ Force inline large functions\n";
    cout << "  ✗ Inline functions with loops\n";
    cout << "  ✗ Inline recursive functions\n";
    cout << "  ✗ Inline virtual functions (doesn't work polymorphically)\n";
    cout << "  ✗ Assume inline always helps\n";
}

int main() {
    demonstrateBestPractices();
    return 0;
}
```

---

## Key Takeaways

1. ✅ **Inline is a hint**, compiler decides
2. ✅ **Small functions** benefit most
3. ✅ **Templates** automatically inline
4. ✅ **LTO** enables cross-file inlining
5. ✅ **Profile** to verify improvements
6. ✅ **Avoid** inlining large/complex functions
7. ✅ **Code bloat** is a real concern
8. ✅ **Trust the compiler** (it's usually right)

---

## Summary Checklist

Before moving to Lesson 26, ensure you can:
- [ ] Understand function call overhead
- [ ] Use inline keyword appropriately
- [ ] Force inline when necessary
- [ ] Identify good inlining candidates
- [ ] Enable LTO for cross-file optimization
- [ ] Measure inlining impact
- [ ] Avoid common pitfalls

**Congratulations on mastering Function Inlining!**

Next lesson: Virtual Function Call Optimization
