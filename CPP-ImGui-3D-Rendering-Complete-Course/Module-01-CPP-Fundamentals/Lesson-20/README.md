# Lesson 20: Advanced Functions

## Duration
3 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand function call mechanisms and stack frames
- Master pass-by-value, pass-by-reference, and pass-by-pointer
- Implement functions returning multiple values
- Use static variables in functions
- Understand function scope and lifetime
- Work with function declarations and definitions
- Debug function-related issues effectively

## Introduction

Functions are the building blocks of C++ programs. In this lesson, we dive deeper into advanced function concepts that are crucial for writing efficient, maintainable code. Understanding how functions work at a low level will help you make better design decisions and write more performant code.

## 1. Function Call Mechanism and Stack Frames

### How Function Calls Work

When a function is called, the following happens:
1. Arguments are evaluated and copied (or referenced)
2. A new stack frame is created
3. Control transfers to the function
4. Local variables are created
5. Function executes
6. Return value is prepared
7. Stack frame is destroyed
8. Control returns to caller

### Example: Visualizing Stack Frames

```cpp
#include <iostream>
using namespace std;

void functionC(int x) {
    cout << "In functionC, x = " << x << endl;
    cout << "Address of x in functionC: " << &x << endl;
}

void functionB(int y) {
    cout << "In functionB, y = " << y << endl;
    cout << "Address of y in functionB: " << &y << endl;
    functionC(y * 2);
    cout << "Back in functionB" << endl;
}

void functionA(int z) {
    cout << "In functionA, z = " << z << endl;
    cout << "Address of z in functionA: " << &z << endl;
    functionB(z + 5);
    cout << "Back in functionA" << endl;
}

int main() {
    cout << "Starting in main" << endl;
    functionA(10);
    cout << "Back in main" << endl;
    return 0;
}
```

**Output:**
```
Starting in main
In functionA, z = 10
Address of z in functionA: 0x7ffc8b3d4a3c
In functionB, y = 15
Address of y in functionB: 0x7ffc8b3d4a1c
In functionC, x = 30
Address of x in functionC: 0x7ffc8b3d49fc
Back in functionB
Back in functionA
Back in main
```

## 2. Pass-by-Value vs Pass-by-Reference vs Pass-by-Pointer

### Complete Comparison Example

```cpp
#include <iostream>
using namespace std;

// Pass-by-value: Function receives a copy
void modifyByValue(int x) {
    x = x * 2;
    cout << "Inside modifyByValue: " << x << endl;
}

// Pass-by-reference: Function receives an alias
void modifyByReference(int& x) {
    x = x * 2;
    cout << "Inside modifyByReference: " << x << endl;
}

// Pass-by-pointer: Function receives address
void modifyByPointer(int* x) {
    *x = *x * 2;
    cout << "Inside modifyByPointer: " << *x << endl;
}

// Pass-by-const-reference: Efficient for large objects, prevents modification
void printByConstReference(const int& x) {
    cout << "Value (const ref): " << x << endl;
    // x = 100; // Error: cannot modify const reference
}

int main() {
    int value1 = 10;
    cout << "Before modifyByValue: " << value1 << endl;
    modifyByValue(value1);
    cout << "After modifyByValue: " << value1 << endl;
    cout << endl;

    int value2 = 10;
    cout << "Before modifyByReference: " << value2 << endl;
    modifyByReference(value2);
    cout << "After modifyByReference: " << value2 << endl;
    cout << endl;

    int value3 = 10;
    cout << "Before modifyByPointer: " << value3 << endl;
    modifyByPointer(&value3);
    cout << "After modifyByPointer: " << value3 << endl;
    cout << endl;

    int value4 = 42;
    printByConstReference(value4);

    return 0;
}
```

**Output:**
```
Before modifyByValue: 10
Inside modifyByValue: 20
After modifyByValue: 10

Before modifyByReference: 10
Inside modifyByReference: 20
After modifyByReference: 20

Before modifyByPointer: 10
Inside modifyByPointer: 20
After modifyByPointer: 20

Value (const ref): 42
```

## 3. Returning Multiple Values

### Method 1: Using References

```cpp
#include <iostream>
using namespace std;

void calculate(int a, int b, int& sum, int& product, int& difference) {
    sum = a + b;
    product = a * b;
    difference = a - b;
}

int main() {
    int x = 15, y = 5;
    int sum, product, diff;

    calculate(x, y, sum, product, diff);

    cout << "Sum: " << sum << endl;
    cout << "Product: " << product << endl;
    cout << "Difference: " << diff << endl;

    return 0;
}
```

### Method 2: Using Structures

```cpp
#include <iostream>
using namespace std;

struct MathResults {
    int sum;
    int product;
    int difference;
    double average;
};

MathResults calculate(int a, int b) {
    MathResults result;
    result.sum = a + b;
    result.product = a * b;
    result.difference = a - b;
    result.average = (a + b) / 2.0;
    return result;
}

int main() {
    int x = 20, y = 8;
    MathResults results = calculate(x, y);

    cout << "Sum: " << results.sum << endl;
    cout << "Product: " << results.product << endl;
    cout << "Difference: " << results.difference << endl;
    cout << "Average: " << results.average << endl;

    return 0;
}
```

### Method 3: Using std::pair and std::tuple (C++11)

```cpp
#include <iostream>
#include <tuple>
#include <utility>
using namespace std;

// Using pair for two values
pair<int, int> divideWithRemainder(int dividend, int divisor) {
    return make_pair(dividend / divisor, dividend % divisor);
}

// Using tuple for multiple values
tuple<int, int, int, double> calculateAll(int a, int b) {
    int sum = a + b;
    int product = a * b;
    int diff = a - b;
    double avg = (a + b) / 2.0;
    return make_tuple(sum, product, diff, avg);
}

int main() {
    // Using pair
    auto result1 = divideWithRemainder(17, 5);
    cout << "17 / 5 = " << result1.first << " remainder " << result1.second << endl;

    // Using structured bindings (C++17)
    auto [quotient, remainder] = divideWithRemainder(25, 7);
    cout << "25 / 7 = " << quotient << " remainder " << remainder << endl;

    // Using tuple
    auto [sum, product, diff, avg] = calculateAll(12, 8);
    cout << "Sum: " << sum << endl;
    cout << "Product: " << product << endl;
    cout << "Difference: " << diff << endl;
    cout << "Average: " << avg << endl;

    return 0;
}
```

## 4. Static Variables in Functions

Static variables maintain their value between function calls.

```cpp
#include <iostream>
using namespace std;

void counterFunction() {
    static int count = 0;  // Initialized only once
    count++;
    cout << "Function called " << count << " time(s)" << endl;
}

int generateUniqueID() {
    static int id = 1000;
    return id++;
}

void demonstrateScope() {
    int localVar = 0;      // Created and destroyed each call
    static int staticVar = 0;  // Persists between calls

    localVar++;
    staticVar++;

    cout << "Local: " << localVar << ", Static: " << staticVar << endl;
}

int main() {
    cout << "=== Counter Function ===" << endl;
    counterFunction();
    counterFunction();
    counterFunction();

    cout << "\n=== Unique ID Generator ===" << endl;
    cout << "ID 1: " << generateUniqueID() << endl;
    cout << "ID 2: " << generateUniqueID() << endl;
    cout << "ID 3: " << generateUniqueID() << endl;

    cout << "\n=== Scope Demonstration ===" << endl;
    demonstrateScope();
    demonstrateScope();
    demonstrateScope();

    return 0;
}
```

**Output:**
```
=== Counter Function ===
Function called 1 time(s)
Function called 2 time(s)
Function called 3 time(s)

=== Unique ID Generator ===
ID 1: 1000
ID 2: 1001
ID 3: 1002

=== Scope Demonstration ===
Local: 1, Static: 1
Local: 1, Static: 2
Local: 1, Static: 3
```

## 5. Function Declarations and Definitions

### Forward Declarations

```cpp
#include <iostream>
using namespace std;

// Forward declarations
void functionA();
void functionB();
int multiply(int a, int b);

int main() {
    functionA();
    cout << "5 * 7 = " << multiply(5, 7) << endl;
    return 0;
}

// Definitions
void functionA() {
    cout << "Function A called" << endl;
    functionB();
}

void functionB() {
    cout << "Function B called" << endl;
}

int multiply(int a, int b) {
    return a * b;
}
```

### Separating Declaration and Definition

```cpp
#include <iostream>
using namespace std;

// Declaration (interface)
class Calculator {
public:
    int add(int a, int b);
    int subtract(int a, int b);
    int multiply(int a, int b);
    double divide(int a, int b);
};

// Definition (implementation)
int Calculator::add(int a, int b) {
    return a + b;
}

int Calculator::subtract(int a, int b) {
    return a - b;
}

int Calculator::multiply(int a, int b) {
    return a * b;
}

double Calculator::divide(int a, int b) {
    if (b == 0) {
        cerr << "Error: Division by zero!" << endl;
        return 0.0;
    }
    return static_cast<double>(a) / b;
}

int main() {
    Calculator calc;

    cout << "10 + 5 = " << calc.add(10, 5) << endl;
    cout << "10 - 5 = " << calc.subtract(10, 5) << endl;
    cout << "10 * 5 = " << calc.multiply(10, 5) << endl;
    cout << "10 / 5 = " << calc.divide(10, 5) << endl;
    cout << "10 / 0 = " << calc.divide(10, 0) << endl;

    return 0;
}
```

## 6. Comprehensive Function Example

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to find min and max
void findMinMax(const vector<int>& numbers, int& min, int& max) {
    if (numbers.empty()) {
        min = max = 0;
        return;
    }

    min = max = numbers[0];
    for (size_t i = 1; i < numbers.size(); i++) {
        if (numbers[i] < min) min = numbers[i];
        if (numbers[i] > max) max = numbers[i];
    }
}

// Function to calculate statistics
struct Statistics {
    double mean;
    int median;
    int range;
};

Statistics calculateStats(vector<int> numbers) {
    Statistics stats;

    if (numbers.empty()) {
        stats.mean = 0;
        stats.median = 0;
        stats.range = 0;
        return stats;
    }

    // Calculate mean
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    stats.mean = static_cast<double>(sum) / numbers.size();

    // Calculate median (requires sorting)
    sort(numbers.begin(), numbers.end());
    size_t mid = numbers.size() / 2;
    stats.median = (numbers.size() % 2 == 0)
                   ? (numbers[mid-1] + numbers[mid]) / 2
                   : numbers[mid];

    // Calculate range
    stats.range = numbers.back() - numbers.front();

    return stats;
}

// Function to print vector
void printVector(const vector<int>& vec, const string& name) {
    cout << name << ": [";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    vector<int> data = {45, 23, 67, 12, 89, 34, 56, 78, 90, 21};

    printVector(data, "Data");

    int min, max;
    findMinMax(data, min, max);
    cout << "\nMin: " << min << endl;
    cout << "Max: " << max << endl;

    Statistics stats = calculateStats(data);
    cout << "\nStatistics:" << endl;
    cout << "Mean: " << stats.mean << endl;
    cout << "Median: " << stats.median << endl;
    cout << "Range: " << stats.range << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Swap Function
Write three versions of a swap function: using pointers, references, and returning a pair. Compare their efficiency and usage.

### Exercise 2: Array Statistics
Create a function that takes an array and its size, then returns (via reference parameters) the sum, average, minimum, and maximum values.

### Exercise 3: Fibonacci with Static
Write a Fibonacci function that uses a static variable to cache previously calculated values for efficiency.

### Exercise 4: String Analyzer
Create a function that analyzes a string and returns (using a structure): character count, word count, vowel count, and consonant count.

### Exercise 5: Matrix Operations
Write functions to perform basic matrix operations (add, subtract, multiply) that work with 2D vectors. The functions should validate dimensions and return results appropriately.

## Exercise Solutions

### Solution 1: Swap Function

```cpp
#include <iostream>
#include <utility>
#include <chrono>
using namespace std;

// Version 1: Using pointers
void swapWithPointers(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Version 2: Using references
void swapWithReferences(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Version 3: Using pair return
pair<int, int> swapWithPair(int a, int b) {
    return make_pair(b, a);
}

int main() {
    // Test pointer version
    int x1 = 10, y1 = 20;
    cout << "Before (pointers): x = " << x1 << ", y = " << y1 << endl;
    swapWithPointers(&x1, &y1);
    cout << "After (pointers): x = " << x1 << ", y = " << y1 << endl;
    cout << endl;

    // Test reference version
    int x2 = 10, y2 = 20;
    cout << "Before (references): x = " << x2 << ", y = " << y2 << endl;
    swapWithReferences(x2, y2);
    cout << "After (references): x = " << x2 << ", y = " << y2 << endl;
    cout << endl;

    // Test pair version
    int x3 = 10, y3 = 20;
    cout << "Before (pair): x = " << x3 << ", y = " << y3 << endl;
    auto result = swapWithPair(x3, y3);
    x3 = result.first;
    y3 = result.second;
    cout << "After (pair): x = " << x3 << ", y = " << y3 << endl;
    cout << endl;

    // Performance comparison
    const int ITERATIONS = 1000000;

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        swapWithPointers(&x1, &y1);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        swapWithReferences(x2, y2);
    }
    end = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        auto r = swapWithPair(x3, y3);
        x3 = r.first; y3 = r.second;
    }
    end = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Performance (" << ITERATIONS << " iterations):" << endl;
    cout << "Pointers: " << duration1 << " microseconds" << endl;
    cout << "References: " << duration2 << " microseconds" << endl;
    cout << "Pair: " << duration3 << " microseconds" << endl;

    return 0;
}
```

### Solution 2: Array Statistics

```cpp
#include <iostream>
#include <limits>
using namespace std;

void calculateArrayStats(const int arr[], int size,
                        int& sum, double& average,
                        int& minimum, int& maximum) {
    if (size <= 0) {
        sum = 0;
        average = 0.0;
        minimum = 0;
        maximum = 0;
        return;
    }

    sum = 0;
    minimum = arr[0];
    maximum = arr[0];

    for (int i = 0; i < size; i++) {
        sum += arr[i];
        if (arr[i] < minimum) minimum = arr[i];
        if (arr[i] > maximum) maximum = arr[i];
    }

    average = static_cast<double>(sum) / size;
}

int main() {
    int numbers[] = {15, 23, 8, 42, 67, 12, 89, 34, 56, 3};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    int sum, min, max;
    double avg;

    calculateArrayStats(numbers, size, sum, avg, min, max);

    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl << endl;

    cout << "Statistics:" << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
    cout << "Minimum: " << min << endl;
    cout << "Maximum: " << max << endl;

    return 0;
}
```

### Solution 3: Fibonacci with Static

```cpp
#include <iostream>
#include <map>
using namespace std;

long long fibonacci(int n) {
    static map<int, long long> cache;

    // Base cases
    if (n <= 1) return n;

    // Check cache
    if (cache.find(n) != cache.end()) {
        return cache[n];
    }

    // Calculate and cache
    long long result = fibonacci(n - 1) + fibonacci(n - 2);
    cache[n] = result;

    return result;
}

void printFibonacciSequence(int count) {
    cout << "First " << count << " Fibonacci numbers:" << endl;
    for (int i = 0; i < count; i++) {
        cout << "F(" << i << ") = " << fibonacci(i) << endl;
    }
}

int main() {
    printFibonacciSequence(20);

    cout << "\nCalculating F(40)..." << endl;
    cout << "F(40) = " << fibonacci(40) << endl;

    // This would be very slow without caching
    cout << "\nCalculating F(50)..." << endl;
    cout << "F(50) = " << fibonacci(50) << endl;

    return 0;
}
```

### Solution 4: String Analyzer

```cpp
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct StringAnalysis {
    int charCount;
    int wordCount;
    int vowelCount;
    int consonantCount;
    int digitCount;
    int spaceCount;
};

StringAnalysis analyzeString(const string& text) {
    StringAnalysis analysis = {0, 0, 0, 0, 0, 0};

    bool inWord = false;

    for (char ch : text) {
        // Count all characters
        analysis.charCount++;

        // Count spaces
        if (isspace(ch)) {
            analysis.spaceCount++;
            inWord = false;
        }
        // Count words
        else if (!inWord) {
            analysis.wordCount++;
            inWord = true;
        }

        // Count vowels and consonants
        if (isalpha(ch)) {
            char lower = tolower(ch);
            if (lower == 'a' || lower == 'e' || lower == 'i' ||
                lower == 'o' || lower == 'u') {
                analysis.vowelCount++;
            } else {
                analysis.consonantCount++;
            }
        }

        // Count digits
        if (isdigit(ch)) {
            analysis.digitCount++;
        }
    }

    return analysis;
}

void printAnalysis(const StringAnalysis& analysis) {
    cout << "String Analysis:" << endl;
    cout << "----------------" << endl;
    cout << "Total characters: " << analysis.charCount << endl;
    cout << "Words: " << analysis.wordCount << endl;
    cout << "Vowels: " << analysis.vowelCount << endl;
    cout << "Consonants: " << analysis.consonantCount << endl;
    cout << "Digits: " << analysis.digitCount << endl;
    cout << "Spaces: " << analysis.spaceCount << endl;
}

int main() {
    string text1 = "Hello World! This is a test string with 123 numbers.";
    cout << "Text: \"" << text1 << "\"" << endl;
    StringAnalysis result1 = analyzeString(text1);
    printAnalysis(result1);

    cout << "\n";

    string text2 = "C++ Programming is awesome in 2024!";
    cout << "Text: \"" << text2 << "\"" << endl;
    StringAnalysis result2 = analyzeString(text2);
    printAnalysis(result2);

    return 0;
}
```

### Solution 5: Matrix Operations

```cpp
#include <iostream>
#include <vector>
using namespace std;

using Matrix = vector<vector<int>>;

void printMatrix(const Matrix& mat, const string& name) {
    cout << name << ":" << endl;
    for (const auto& row : mat) {
        for (int val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

bool addMatrices(const Matrix& a, const Matrix& b, Matrix& result) {
    // Check dimensions
    if (a.empty() || b.empty() || a.size() != b.size() ||
        a[0].size() != b[0].size()) {
        return false;
    }

    int rows = a.size();
    int cols = a[0].size();
    result.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    return true;
}

bool subtractMatrices(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.empty() || b.empty() || a.size() != b.size() ||
        a[0].size() != b[0].size()) {
        return false;
    }

    int rows = a.size();
    int cols = a[0].size();
    result.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }

    return true;
}

bool multiplyMatrices(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.empty() || b.empty() || a[0].size() != b.size()) {
        return false;
    }

    int rowsA = a.size();
    int colsA = a[0].size();
    int colsB = b[0].size();

    result.resize(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return true;
}

int main() {
    Matrix mat1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    Matrix mat2 = {
        {7, 8, 9},
        {10, 11, 12}
    };

    Matrix mat3 = {
        {1, 2},
        {3, 4},
        {5, 6}
    };

    printMatrix(mat1, "Matrix 1");
    printMatrix(mat2, "Matrix 2");
    printMatrix(mat3, "Matrix 3");

    Matrix result;

    // Addition
    if (addMatrices(mat1, mat2, result)) {
        printMatrix(result, "Matrix 1 + Matrix 2");
    } else {
        cout << "Cannot add matrices (dimension mismatch)" << endl;
    }

    // Subtraction
    if (subtractMatrices(mat1, mat2, result)) {
        printMatrix(result, "Matrix 1 - Matrix 2");
    } else {
        cout << "Cannot subtract matrices (dimension mismatch)" << endl;
    }

    // Multiplication
    if (multiplyMatrices(mat1, mat3, result)) {
        printMatrix(result, "Matrix 1 * Matrix 3");
    } else {
        cout << "Cannot multiply matrices (dimension mismatch)" << endl;
    }

    return 0;
}
```

## Summary

In this lesson, you learned:
- How function calls work with stack frames
- The differences between pass-by-value, pass-by-reference, and pass-by-pointer
- Multiple techniques for returning multiple values from functions
- How to use static variables to maintain state between function calls
- The importance of function declarations and definitions
- How to design and implement complex function-based programs

## Checklist

- [ ] I understand how stack frames work during function calls
- [ ] I can choose between pass-by-value, pass-by-reference, and pass-by-pointer appropriately
- [ ] I know multiple methods to return multiple values from a function
- [ ] I understand when and how to use static variables in functions
- [ ] I can properly declare and define functions
- [ ] I can implement complex functions with multiple parameters and return values
- [ ] I completed all exercises and understand their solutions
- [ ] I can debug function-related issues in my code

## Next Steps
In the next lesson, we'll explore Function Overloading, learning how to create multiple functions with the same name but different parameters.
