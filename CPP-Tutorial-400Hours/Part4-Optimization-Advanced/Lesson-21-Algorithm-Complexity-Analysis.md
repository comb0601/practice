# Lesson 21: Algorithm Complexity Analysis (Big O Notation)

**Duration**: 10 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Algorithm Complexity
2. Understanding Big O Notation
3. Common Complexity Classes
4. Analyzing Time Complexity
5. Analyzing Space Complexity
6. Best, Average, and Worst Cases
7. Practical Examples with Measurements
8. Optimization Strategies Based on Complexity
9. Advanced Complexity Analysis
10. Exercises and Practice Problems

---

## 1. Introduction to Algorithm Complexity

Algorithm complexity analysis is the foundation of writing efficient code. It allows us to predict how an algorithm's performance scales with input size without running expensive benchmarks on every possible input.

### Why Complexity Analysis Matters

In real-world applications, the difference between O(n) and O(n²) can mean:
- A search taking **1 second** vs **1 hour** for 10,000 items
- Processing **1 million records** in minutes vs days
- The difference between a **responsive application** and one that freezes

### The Two Dimensions of Complexity

1. **Time Complexity**: How execution time grows with input size
2. **Space Complexity**: How memory usage grows with input size

---

## 2. Understanding Big O Notation

Big O notation describes the **upper bound** of an algorithm's growth rate. It focuses on the **dominant term** as input size approaches infinity.

### Mathematical Definition

For functions f(n) and g(n):
```
f(n) = O(g(n))
```
Means there exist constants c and n₀ such that:
```
f(n) ≤ c · g(n) for all n ≥ n₀
```

### Practical Interpretation

Big O tells us **how badly performance can degrade** in the worst case as input grows.

#### Example: Simplifying to Big O

```cpp
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

// Function with complexity: 3n² + 5n + 100
int complexFunction(int n) {
    int result = 0;

    // O(n²) - nested loops
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result += i * j;  // 3 operations: multiply, add, assign
        }
    }

    // O(n) - single loop
    for (int i = 0; i < n; i++) {
        result += i * 5;  // 5 operations each
    }

    // O(1) - constant operations
    for (int i = 0; i < 100; i++) {
        result++;
    }

    return result;
}

int main() {
    // The actual complexity: T(n) = 3n² + 5n + 100
    // Big O simplification: O(n²)
    // We drop constants and lower-order terms

    cout << "Testing complexity analysis:\n\n";

    vector<int> sizes = {10, 100, 1000, 2000};

    for (int n : sizes) {
        auto start = high_resolution_clock::now();
        int result = complexFunction(n);
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start);

        cout << "n = " << n << ": " << duration.count() << " μs\n";
    }

    // Notice how time grows quadratically:
    // When n doubles, time roughly quadruples (2² = 4)

    return 0;
}
```

**Output Analysis**:
```
n = 10: ~15 μs
n = 100: ~1,500 μs (100x increase, 10² ratio)
n = 1000: ~150,000 μs (100x increase again)
n = 2000: ~600,000 μs (4x increase, 2² ratio)
```

---

## 3. Common Complexity Classes

### The Complexity Hierarchy (from fastest to slowest)

```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2ⁿ) < O(n!)
```

### Detailed Examples

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

// O(1) - Constant Time
// Time doesn't depend on input size
int constantTime(const vector<int>& arr) {
    if (arr.empty()) return 0;
    return arr[0];  // Always one operation
}

// O(log n) - Logarithmic Time
// Binary search: cuts problem in half each step
int binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;  // Not found
}

// O(n) - Linear Time
// Must check each element once
int linearSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target)
            return i;
    }
    return -1;
}

// O(n log n) - Linearithmic Time
// Efficient sorting algorithms (merge sort, quick sort)
void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    // Divide: O(log n) levels
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Conquer: O(n) work per level
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = 0; i < temp.size(); i++)
        arr[left + i] = temp[i];
}

// O(n²) - Quadratic Time
// Nested loops over the same data
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// O(n³) - Cubic Time
// Triple nested loops
int matrixMultiply(const vector<vector<int>>& A,
                   const vector<vector<int>>& B,
                   vector<vector<int>>& C) {
    int n = A.size();
    int operations = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
                operations++;
            }
        }
    }

    return operations;
}

// O(2ⁿ) - Exponential Time
// Recursive Fibonacci (very inefficient!)
int fibonacciRecursive(int n) {
    if (n <= 1) return n;
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

// O(n!) - Factorial Time
// Generate all permutations
void generatePermutations(vector<int>& arr, int start, int& count) {
    if (start == arr.size() - 1) {
        count++;
        return;
    }

    for (int i = start; i < arr.size(); i++) {
        swap(arr[start], arr[i]);
        generatePermutations(arr, start + 1, count);
        swap(arr[start], arr[i]);  // Backtrack
    }
}

// Demonstration and benchmarking
int main() {
    cout << "=== COMPLEXITY CLASS DEMONSTRATIONS ===\n\n";

    // Test O(1)
    cout << "1. O(1) - Constant Time:\n";
    vector<int> arr1k(1000, 42);
    vector<int> arr1m(1000000, 42);

    auto start = high_resolution_clock::now();
    constantTime(arr1k);
    auto end = high_resolution_clock::now();
    cout << "   1K elements: "
         << duration_cast<nanoseconds>(end - start).count() << " ns\n";

    start = high_resolution_clock::now();
    constantTime(arr1m);
    end = high_resolution_clock::now();
    cout << "   1M elements: "
         << duration_cast<nanoseconds>(end - start).count() << " ns\n";
    cout << "   (Should be roughly the same!)\n\n";

    // Test O(log n)
    cout << "2. O(log n) - Logarithmic Time (Binary Search):\n";
    vector<int> sorted(1000000);
    for (int i = 0; i < sorted.size(); i++) sorted[i] = i;

    start = high_resolution_clock::now();
    binarySearch(sorted, 999999);
    end = high_resolution_clock::now();
    cout << "   Searching in 1M elements: "
         << duration_cast<nanoseconds>(end - start).count() << " ns\n";
    cout << "   (Only ~20 comparisons needed: log₂(1000000) ≈ 20)\n\n";

    // Test O(n)
    cout << "3. O(n) - Linear Time (Linear Search):\n";
    vector<int> sizes_linear = {1000, 10000, 100000};
    for (int size : sizes_linear) {
        vector<int> arr(size, 1);
        arr.push_back(999);  // Target at end

        start = high_resolution_clock::now();
        linearSearch(arr, 999);
        end = high_resolution_clock::now();

        cout << "   " << size << " elements: "
             << duration_cast<microseconds>(end - start).count() << " μs\n";
    }
    cout << "   (Time scales linearly with size)\n\n";

    // Test O(n log n)
    cout << "4. O(n log n) - Linearithmic Time (Merge Sort):\n";
    vector<int> sizes_sort = {1000, 10000, 100000};
    for (int size : sizes_sort) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = size - i;

        start = high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        end = high_resolution_clock::now();

        cout << "   " << size << " elements: "
             << duration_cast<microseconds>(end - start).count() << " μs\n";
    }
    cout << "   (Efficient sorting)\n\n";

    // Test O(n²)
    cout << "5. O(n²) - Quadratic Time (Bubble Sort):\n";
    vector<int> sizes_quad = {100, 500, 1000};
    for (int size : sizes_quad) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) arr[i] = size - i;

        start = high_resolution_clock::now();
        bubbleSort(arr);
        end = high_resolution_clock::now();

        cout << "   " << size << " elements: "
             << duration_cast<microseconds>(end - start).count() << " μs\n";
    }
    cout << "   (Time quadruples when size doubles)\n\n";

    // Test O(n³)
    cout << "6. O(n³) - Cubic Time (Matrix Multiplication):\n";
    vector<int> sizes_cubic = {10, 50, 100};
    for (int size : sizes_cubic) {
        vector<vector<int>> A(size, vector<int>(size, 1));
        vector<vector<int>> B(size, vector<int>(size, 1));
        vector<vector<int>> C(size, vector<int>(size, 0));

        start = high_resolution_clock::now();
        int ops = matrixMultiply(A, B, C);
        end = high_resolution_clock::now();

        cout << "   " << size << "x" << size << " matrices: "
             << duration_cast<microseconds>(end - start).count()
             << " μs (" << ops << " operations)\n";
    }
    cout << "   (Time increases by 8x when size doubles: 2³ = 8)\n\n";

    // Test O(2ⁿ)
    cout << "7. O(2ⁿ) - Exponential Time (Recursive Fibonacci):\n";
    cout << "   WARNING: Very slow! Testing small values only:\n";
    for (int n = 10; n <= 30; n += 5) {
        start = high_resolution_clock::now();
        int result = fibonacciRecursive(n);
        end = high_resolution_clock::now();

        cout << "   fib(" << n << ") = " << result << ": "
             << duration_cast<microseconds>(end - start).count() << " μs\n";
    }
    cout << "   (Time doubles with each +1 to n!)\n\n";

    // Test O(n!)
    cout << "8. O(n!) - Factorial Time (Permutations):\n";
    cout << "   WARNING: Extremely slow! Testing tiny values only:\n";
    for (int n = 5; n <= 10; n++) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i;

        int count = 0;
        start = high_resolution_clock::now();
        generatePermutations(arr, 0, count);
        end = high_resolution_clock::now();

        cout << "   " << n << " elements (" << count << " permutations): "
             << duration_cast<microseconds>(end - start).count() << " μs\n";
    }
    cout << "   (n! grows incredibly fast: 10! = 3,628,800)\n\n";

    return 0;
}
```

---

## 4. Analyzing Time Complexity

### Step-by-Step Analysis Method

1. **Identify Basic Operations**: What's the fundamental unit of work?
2. **Count Iterations**: How many times does each loop run?
3. **Analyze Nesting**: Multiply complexities of nested structures
4. **Find Dominant Term**: Which grows fastest?
5. **Drop Constants**: Remove constant multipliers

### Example 1: Simple Loop Analysis

```cpp
#include <iostream>
#include <vector>

using namespace std;

// Analysis: What's the complexity?
void example1(int n) {
    // Single loop: runs n times
    for (int i = 0; i < n; i++) {
        cout << i << " ";  // O(1) operation
    }
    // Total: n × O(1) = O(n)
}

void example2(int n) {
    // First loop: n iterations
    for (int i = 0; i < n; i++) {
        cout << i << " ";
    }

    // Second loop: n iterations
    for (int j = 0; j < n; j++) {
        cout << j << " ";
    }

    // Total: O(n) + O(n) = O(2n) = O(n)
    // Sequential loops: add complexities
}

void example3(int n) {
    // Outer loop: n iterations
    for (int i = 0; i < n; i++) {
        // Inner loop: n iterations
        for (int j = 0; j < n; j++) {
            cout << i * j << " ";  // O(1)
        }
    }

    // Total: n × n × O(1) = O(n²)
    // Nested loops: multiply complexities
}

void example4(int n) {
    // Outer: n iterations
    for (int i = 0; i < n; i++) {
        // Inner: i iterations (0, 1, 2, ..., n-1)
        for (int j = 0; j < i; j++) {
            cout << j << " ";
        }
    }

    // Total iterations: 0 + 1 + 2 + ... + (n-1) = n(n-1)/2
    // = (n² - n) / 2
    // Simplified: O(n²)
}

void example5(int n) {
    int i = n;
    while (i > 0) {
        cout << i << " ";
        i = i / 2;  // Halving each iteration
    }

    // How many times can we halve n until we reach 1?
    // Answer: log₂(n)
    // Total: O(log n)
}

void example6(int n) {
    // Outer loop: log n iterations (halving)
    for (int i = 1; i < n; i *= 2) {
        // Inner loop: n iterations
        for (int j = 0; j < n; j++) {
            cout << i * j << " ";
        }
    }

    // Total: log(n) × n = O(n log n)
}

// Complex example with multiple parts
void example7(int n) {
    // Part 1: O(n²)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << i + j << " ";
        }
    }

    // Part 2: O(n)
    for (int k = 0; k < n; k++) {
        cout << k << " ";
    }

    // Part 3: O(1)
    cout << n << endl;

    // Total: O(n²) + O(n) + O(1) = O(n²)
    // The dominant term wins!
}

int main() {
    cout << "Complexity Analysis Examples:\n\n";

    int n = 5;

    cout << "Example 1 - O(n):\n";
    example1(n);
    cout << "\n\n";

    cout << "Example 2 - O(n) [sequential loops]:\n";
    example2(n);
    cout << "\n\n";

    cout << "Example 3 - O(n²) [nested loops]:\n";
    example3(n);
    cout << "\n\n";

    cout << "Example 4 - O(n²) [triangular pattern]:\n";
    example4(n);
    cout << "\n\n";

    cout << "Example 5 - O(log n) [halving]:\n";
    example5(32);
    cout << "\n\n";

    cout << "Example 6 - O(n log n):\n";
    example6(n);
    cout << "\n\n";

    return 0;
}
```

### Example 2: Recursive Complexity

```cpp
#include <iostream>

using namespace std;

// Analysis of recursive functions
// Use recurrence relations

// T(n) = T(n-1) + O(1)
// Solution: O(n)
void recursiveLinear(int n) {
    if (n <= 0) return;
    cout << n << " ";
    recursiveLinear(n - 1);  // One recursive call
}

// T(n) = 2T(n-1) + O(1)
// Solution: O(2ⁿ)
void recursiveExponential(int n) {
    if (n <= 0) return;
    cout << n << " ";
    recursiveExponential(n - 1);  // First call
    recursiveExponential(n - 1);  // Second call: doubling!
}

// T(n) = 2T(n/2) + O(n)
// Solution: O(n log n) - Master Theorem
void recursiveDivideConquer(int arr[], int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;

    // Two recursive calls on half the data
    recursiveDivideConquer(arr, left, mid);
    recursiveDivideConquer(arr, mid + 1, right);

    // O(n) work to merge
    for (int i = left; i <= right; i++) {
        cout << arr[i] << " ";
    }
}

// T(n) = T(n/2) + O(1)
// Solution: O(log n)
int recursiveLogarithmic(int arr[], int target, int left, int right) {
    if (left > right) return -1;

    int mid = (left + right) / 2;

    if (arr[mid] == target) return mid;

    // Only ONE recursive call on half the data
    if (arr[mid] > target)
        return recursiveLogarithmic(arr, target, left, mid - 1);
    else
        return recursiveLogarithmic(arr, target, mid + 1, right);
}

int main() {
    cout << "Recursive Complexity Analysis:\n\n";

    cout << "1. O(n) - Linear Recursion:\n";
    recursiveLinear(5);
    cout << "\n\n";

    cout << "2. O(2ⁿ) - Exponential Recursion (careful!):\n";
    recursiveExponential(4);  // Small n!
    cout << "\n\n";

    cout << "3. O(n log n) - Divide and Conquer:\n";
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    recursiveDivideConquer(arr, 0, 7);
    cout << "\n\n";

    cout << "4. O(log n) - Binary Search:\n";
    int sorted[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int index = recursiveLogarithmic(sorted, 11, 0, 7);
    cout << "Found at index: " << index << "\n\n";

    return 0;
}
```

---

## 5. Analyzing Space Complexity

Space complexity includes:
1. **Input space**: Memory for input data
2. **Auxiliary space**: Extra memory used by algorithm
3. **Call stack**: For recursive functions

```cpp
#include <iostream>
#include <vector>

using namespace std;

// O(1) Space - Constant extra memory
int sumArray_O1(const vector<int>& arr) {
    int sum = 0;  // Only one extra variable
    for (int num : arr) {
        sum += num;
    }
    return sum;  // Space doesn't grow with input
}

// O(n) Space - Linear extra memory
vector<int> copyArray_On(const vector<int>& arr) {
    vector<int> copy(arr.size());  // New array of size n
    for (int i = 0; i < arr.size(); i++) {
        copy[i] = arr[i];
    }
    return copy;  // Space proportional to input
}

// O(n) Space - Recursive call stack
int factorial_recursive(int n) {
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);
    // Call stack depth: n
    // Each call stores: return address, parameters, local vars
}

// O(1) Space - Iterative version (better!)
int factorial_iterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;  // No recursion = no call stack overhead
}

// O(n²) Space - 2D array
vector<vector<int>> createMatrix(int n) {
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    return matrix;  // n × n = n² space
}

// O(log n) Space - Binary search recursive
int binarySearchRecursive(const vector<int>& arr, int target,
                          int left, int right) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) return mid;

    // Call stack depth: log n (halving each time)
    if (arr[mid] > target)
        return binarySearchRecursive(arr, target, left, mid - 1);
    else
        return binarySearchRecursive(arr, target, mid + 1, right);
}

// Space-Time Tradeoff Example
// Version 1: O(1) space, O(n) time
int fibonacci_space_efficient(int n) {
    if (n <= 1) return n;

    int prev2 = 0, prev1 = 1;

    for (int i = 2; i <= n; i++) {
        int current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

// Version 2: O(n) space, O(n) time (using memoization)
int fibonacci_memoized(int n, vector<int>& memo) {
    if (n <= 1) return n;

    if (memo[n] != -1) return memo[n];

    memo[n] = fibonacci_memoized(n - 1, memo) +
              fibonacci_memoized(n - 2, memo);

    return memo[n];
}

int main() {
    cout << "=== SPACE COMPLEXITY ANALYSIS ===\n\n";

    vector<int> arr = {1, 2, 3, 4, 5};

    cout << "1. O(1) Space - Sum array:\n";
    cout << "   Sum: " << sumArray_O1(arr) << "\n";
    cout << "   (Only uses one extra variable)\n\n";

    cout << "2. O(n) Space - Copy array:\n";
    vector<int> copy = copyArray_On(arr);
    cout << "   Created copy of size " << copy.size() << "\n\n";

    cout << "3. Recursive vs Iterative Factorial:\n";
    cout << "   Recursive 10!: " << factorial_recursive(10)
         << " (O(n) stack space)\n";
    cout << "   Iterative 10!: " << factorial_iterative(10)
         << " (O(1) space)\n\n";

    cout << "4. Fibonacci - Space-Time Tradeoff:\n";

    // Space-efficient version
    cout << "   Space-efficient fib(40): "
         << fibonacci_space_efficient(40)
         << " [O(1) space, O(n) time]\n";

    // Memoized version
    vector<int> memo(41, -1);
    cout << "   Memoized fib(40): "
         << fibonacci_memoized(40, memo)
         << " [O(n) space, O(n) time]\n\n";

    return 0;
}
```

---

## 6. Best, Average, and Worst Cases

Different inputs can lead to different performance:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Quick Sort: Demonstrates best/average/worst cases
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Linear Search: Best/Average/Worst cases
int linearSearch(const vector<int>& arr, int target, int& comparisons) {
    comparisons = 0;
    for (int i = 0; i < arr.size(); i++) {
        comparisons++;
        if (arr[i] == target) {
            return i;  // Found
        }
    }
    return -1;  // Not found
}

int main() {
    cout << "=== BEST, AVERAGE, AND WORST CASE ANALYSIS ===\n\n";

    // LINEAR SEARCH DEMONSTRATION
    cout << "1. LINEAR SEARCH ANALYSIS:\n\n";

    vector<int> arr = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int comparisons;

    // Best case: element at beginning
    linearSearch(arr, 10, comparisons);
    cout << "   Best case (target at start): "
         << comparisons << " comparisons - O(1)\n";

    // Average case: element in middle
    linearSearch(arr, 50, comparisons);
    cout << "   Average case (target in middle): "
         << comparisons << " comparisons - O(n/2) = O(n)\n";

    // Worst case: element at end or not present
    linearSearch(arr, 100, comparisons);
    cout << "   Worst case (target at end): "
         << comparisons << " comparisons - O(n)\n";

    linearSearch(arr, 999, comparisons);
    cout << "   Worst case (target not present): "
         << comparisons << " comparisons - O(n)\n\n";

    // QUICK SORT DEMONSTRATION
    cout << "2. QUICK SORT ANALYSIS:\n\n";

    // Best/Average case: Random data
    vector<int> random_data = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    cout << "   Best/Average case (random data):\n";
    auto start = high_resolution_clock::now();
    quickSort(random_data, 0, random_data.size() - 1);
    auto end = high_resolution_clock::now();
    cout << "   Time: " << duration_cast<microseconds>(end - start).count()
         << " μs - O(n log n)\n";

    // Worst case: Already sorted data (pivot is always largest/smallest)
    vector<int> sorted_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "\n   Worst case (already sorted):\n";
    start = high_resolution_clock::now();
    quickSort(sorted_data, 0, sorted_data.size() - 1);
    end = high_resolution_clock::now();
    cout << "   Time: " << duration_cast<microseconds>(end - start).count()
         << " μs - O(n²)\n";
    cout << "   (Unbalanced partitions at each step!)\n\n";

    // Larger demonstration
    cout << "3. SCALABILITY TEST (larger inputs):\n\n";

    vector<int> sizes = {100, 1000, 10000};

    for (int size : sizes) {
        // Random case
        vector<int> random(size);
        for (int i = 0; i < size; i++) {
            random[i] = rand() % 10000;
        }

        auto start = high_resolution_clock::now();
        quickSort(random, 0, random.size() - 1);
        auto end = high_resolution_clock::now();

        cout << "   Random data (" << size << " elements): "
             << duration_cast<microseconds>(end - start).count() << " μs\n";

        // Sorted case (worst)
        vector<int> sorted(size);
        for (int i = 0; i < size; i++) {
            sorted[i] = i;
        }

        start = high_resolution_clock::now();
        quickSort(sorted, 0, sorted.size() - 1);
        end = high_resolution_clock::now();

        cout << "   Sorted data (" << size << " elements): "
             << duration_cast<microseconds>(end - start).count() << " μs\n\n";
    }

    return 0;
}
```

---

## 7. Practical Examples with Measurements

### Real-World Complexity Comparison

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Problem: Find duplicate elements in an array

// Solution 1: Brute Force - O(n²) time, O(1) space
vector<int> findDuplicates_bruteforce(const vector<int>& arr) {
    vector<int> duplicates;

    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) {
                // Check if already added
                bool found = false;
                for (int k = 0; k < duplicates.size(); k++) {
                    if (duplicates[k] == arr[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    duplicates.push_back(arr[i]);
                }
                break;
            }
        }
    }

    return duplicates;
}

// Solution 2: Sorting - O(n log n) time, O(1) space
vector<int> findDuplicates_sorting(vector<int> arr) {
    vector<int> duplicates;

    sort(arr.begin(), arr.end());  // O(n log n)

    for (int i = 0; i < arr.size() - 1; i++) {  // O(n)
        if (arr[i] == arr[i + 1]) {
            if (duplicates.empty() || duplicates.back() != arr[i]) {
                duplicates.push_back(arr[i]);
            }
        }
    }

    return duplicates;
}

// Solution 3: Hash Map - O(n) time, O(n) space
vector<int> findDuplicates_hashmap(const vector<int>& arr) {
    vector<int> duplicates;
    unordered_map<int, int> frequency;

    // Count frequencies - O(n)
    for (int num : arr) {
        frequency[num]++;
    }

    // Find duplicates - O(n)
    for (const auto& pair : frequency) {
        if (pair.second > 1) {
            duplicates.push_back(pair.first);
        }
    }

    return duplicates;
}

// Benchmark function
void benchmark(const string& name,
               vector<int>(*func)(const vector<int>&),
               const vector<int>& data) {
    auto start = high_resolution_clock::now();
    vector<int> result = func(data);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);

    cout << "   " << name << ": " << duration.count() << " μs";
    cout << " (found " << result.size() << " duplicates)\n";
}

int main() {
    cout << "=== PRACTICAL COMPLEXITY COMPARISON ===\n\n";
    cout << "Problem: Find duplicate numbers in an array\n\n";

    // Test different sizes
    vector<int> sizes = {100, 500, 1000, 2000};

    for (int size : sizes) {
        cout << "Testing with " << size << " elements:\n";

        // Generate test data with some duplicates
        vector<int> data;
        for (int i = 0; i < size; i++) {
            data.push_back(rand() % (size / 2));  // Creates duplicates
        }

        // Only test brute force for small sizes
        if (size <= 1000) {
            benchmark("O(n²) Brute Force", findDuplicates_bruteforce, data);
        } else {
            cout << "   O(n²) Brute Force: SKIPPED (too slow!)\n";
        }

        benchmark("O(n log n) Sorting",
                  [](const vector<int>& arr) {
                      return findDuplicates_sorting(arr);
                  }, data);

        benchmark("O(n) Hash Map", findDuplicates_hashmap, data);

        cout << "\n";
    }

    // Demonstrate the difference with large data
    cout << "Large-scale test (10,000 elements):\n";
    vector<int> large_data;
    for (int i = 0; i < 10000; i++) {
        large_data.push_back(rand() % 5000);
    }

    cout << "   O(n²) Brute Force: TOO SLOW - would take minutes!\n";
    benchmark("O(n log n) Sorting",
              [](const vector<int>& arr) {
                  return findDuplicates_sorting(arr);
              }, large_data);
    benchmark("O(n) Hash Map", findDuplicates_hashmap, large_data);

    return 0;
}
```

---

## 8. Optimization Strategies Based on Complexity

### Strategy 1: Choose Better Algorithms

```cpp
#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

// Problem: Check if array contains duplicates

// Bad: O(n²)
bool hasDuplicates_slow(const vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] == arr[j]) return true;
        }
    }
    return false;
}

// Good: O(n)
bool hasDuplicates_fast(const vector<int>& arr) {
    unordered_set<int> seen;
    for (int num : arr) {
        if (seen.count(num)) return true;
        seen.insert(num);
    }
    return false;
}
```

### Strategy 2: Reduce Redundant Work

```cpp
// Problem: Fibonacci sequence

// Bad: O(2ⁿ) - recalculates same values
int fib_slow(int n) {
    if (n <= 1) return n;
    return fib_slow(n - 1) + fib_slow(n - 2);
}

// Good: O(n) - caches results
int fib_fast(int n, vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];

    memo[n] = fib_fast(n - 1, memo) + fib_fast(n - 2, memo);
    return memo[n];
}

// Best: O(n) time, O(1) space
int fib_optimal(int n) {
    if (n <= 1) return n;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
```

### Strategy 3: Early Termination

```cpp
// Problem: Check if string is palindrome

// Unnecessary work: checks all characters
bool isPalindrome_wasteful(const string& s) {
    string reversed = s;
    reverse(reversed.begin(), reversed.end());
    return s == reversed;  // Creates new string
}

// Better: early exit, no extra space
bool isPalindrome_optimal(const string& s) {
    int left = 0, right = s.length() - 1;

    while (left < right) {
        if (s[left] != s[right]) {
            return false;  // Exit early!
        }
        left++;
        right--;
    }

    return true;
}
```

---

## 9. Advanced Complexity Analysis

### Amortized Analysis

Some operations are expensive occasionally but cheap on average:

```cpp
#include <iostream>
#include <vector>

using namespace std;

// Dynamic array: push_back() demonstration
void demonstrateAmortizedComplexity() {
    vector<int> vec;
    int resize_count = 0;
    int last_capacity = 0;

    cout << "Demonstrating amortized O(1) push_back:\n\n";

    for (int i = 1; i <= 20; i++) {
        vec.push_back(i);

        if (vec.capacity() != last_capacity) {
            resize_count++;
            cout << "Size " << vec.size()
                 << ": Capacity increased to " << vec.capacity()
                 << " (resize #" << resize_count << ")\n";
            last_capacity = vec.capacity();
        }
    }

    cout << "\nTotal resizes: " << resize_count << "\n";
    cout << "Total insertions: 20\n";
    cout << "Average cost: O(1) amortized\n";
    cout << "(Even though individual resizes are O(n))\n";
}

int main() {
    demonstrateAmortizedComplexity();
    return 0;
}
```

---

## 10. Exercises and Practice Problems

### Exercise 1: Analyze Complexity

Determine the time complexity:

```cpp
void mystery1(int n) {
    for (int i = 1; i < n; i *= 2) {
        for (int j = 0; j < n; j++) {
            cout << i * j << " ";
        }
    }
}
// Answer: O(n log n)
// Outer loop: log n, Inner loop: n
```

### Exercise 2: Optimize This Function

```cpp
// Given: O(n³) solution
int sumOfProducts_slow(const vector<int>& arr) {
    int sum = 0;
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (i != j && j != k && i != k) {
                    sum += arr[i] * arr[j] * arr[k];
                }
            }
        }
    }

    return sum;
}

// Challenge: Can you optimize to O(n²) or O(n)?
```

### Exercise 3: Space-Time Tradeoff

```cpp
// Problem: Check if two strings are anagrams

// Option 1: O(n log n) time, O(1) space
bool isAnagram_sorting(string s1, string s2) {
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    return s1 == s2;
}

// Option 2: O(n) time, O(1) space (fixed size array)
bool isAnagram_counting(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) return false;

    int count[26] = {0};

    for (char c : s1) count[c - 'a']++;
    for (char c : s2) count[c - 'a']--;

    for (int i = 0; i < 26; i++) {
        if (count[i] != 0) return false;
    }

    return true;
}

// Which is better? Depends on your constraints!
```

---

## Key Takeaways

1. ✅ **Big O describes scalability**, not absolute speed
2. ✅ **Focus on dominant terms**: n² beats constants
3. ✅ **Different inputs** can give different performance
4. ✅ **Space-time tradeoffs**: Sometimes you trade memory for speed
5. ✅ **Amortized analysis**: Average cost over many operations
6. ✅ **Choose algorithms wisely**: O(n) vs O(n²) matters at scale
7. ✅ **Measure in practice**: Theory guides, benchmarks confirm
8. ✅ **Optimize bottlenecks**: Profile before optimizing

---

## Summary Checklist

Before moving to Lesson 22, ensure you can:
- [ ] Explain Big O notation and its significance
- [ ] Recognize common complexity classes (O(1), O(log n), O(n), etc.)
- [ ] Analyze time complexity of loops and recursive functions
- [ ] Understand space complexity and call stack overhead
- [ ] Distinguish between best, average, and worst cases
- [ ] Choose appropriate algorithms based on complexity
- [ ] Apply optimization strategies to reduce complexity
- [ ] Measure and benchmark actual performance

**Congratulations on mastering Algorithm Complexity Analysis!**

Next lesson: Choosing the Right Data Structures
