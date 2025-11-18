# Lesson 25: Function Templates

## Duration
3 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand and create function templates
- Use template type parameters effectively
- Implement template specialization
- Work with multiple template parameters
- Understand template argument deduction
- Use non-type template parameters
- Debug template-related compilation errors

## Introduction

Function templates enable you to write generic functions that work with multiple data types without code duplication. Templates are a cornerstone of generic programming in C++ and are fundamental to the Standard Template Library (STL).

## 1. Basic Function Templates

### Simple Template Functions

```cpp
#include <iostream>
#include <string>
using namespace std;

// Basic template function
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Template with explicit usage
template <typename T>
void swap_values(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Template for displaying values
template <typename T>
void display(T value) {
    cout << "Value: " << value << endl;
}

// Template for array printing
template <typename T>
void printArray(const T arr[], int size) {
    cout << "[";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    // Using template with different types
    cout << "Max(10, 20) = " << maximum(10, 20) << endl;
    cout << "Max(3.14, 2.71) = " << maximum(3.14, 2.71) << endl;
    cout << "Max('a', 'z') = " << maximum('a', 'z') << endl;
    cout << "Max(string) = " << maximum(string("apple"), string("banana")) << endl;
    cout << endl;

    // Swap examples
    int x = 5, y = 10;
    cout << "Before swap: x=" << x << ", y=" << y << endl;
    swap_values(x, y);
    cout << "After swap: x=" << x << ", y=" << y << endl;
    cout << endl;

    double d1 = 3.14, d2 = 2.71;
    cout << "Before swap: d1=" << d1 << ", d2=" << d2 << endl;
    swap_values(d1, d2);
    cout << "After swap: d1=" << d1 << ", d2=" << d2 << endl;
    cout << endl;

    // Display different types
    display(42);
    display(3.14159);
    display("Hello Templates!");
    cout << endl;

    // Array printing
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3};
    printArray(intArr, 5);
    printArray(doubleArr, 3);

    return 0;
}
```

## 2. Multiple Template Parameters

### Templates with Multiple Types

```cpp
#include <iostream>
#include <utility>
#include <string>
using namespace std;

// Template with two type parameters
template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << "(" << first << ", " << second << ")" << endl;
}

// Template returning pair
template <typename T1, typename T2>
pair<T1, T2> makePair(T1 first, T2 second) {
    return make_pair(first, second);
}

// Template for type conversion
template <typename TFrom, typename TTo>
TTo convert(TFrom value) {
    return static_cast<TTo>(value);
}

// Template for comparing different types
template <typename T1, typename T2>
auto compare(T1 a, T2 b) -> decltype(a < b) {
    return a < b;
}

// Template class for holding two values
template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    Pair(T1 f, T2 s) : first(f), second(s) {}

    void display() const {
        cout << "Pair: (" << first << ", " << second << ")" << endl;
    }

    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
};

int main() {
    printPair(10, 20.5);
    printPair("Age", 25);
    printPair(string("Name"), string("John"));
    cout << endl;

    auto p1 = makePair(100, "Value");
    cout << "Pair: " << p1.first << ", " << p1.second << endl;
    cout << endl;

    int i = 42;
    double d = convert<int, double>(i);
    cout << "Converted: " << d << endl;
    cout << endl;

    cout << "5 < 10.5? " << (compare(5, 10.5) ? "true" : "false") << endl;
    cout << endl;

    Pair<string, int> person("Alice", 30);
    person.display();

    Pair<double, double> coordinates(3.14, 2.71);
    coordinates.display();

    return 0;
}
```

## 3. Template Specialization

### Full and Partial Specialization

```cpp
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// Generic template
template <typename T>
T maximum(T a, T b) {
    cout << "Generic template" << endl;
    return (a > b) ? a : b;
}

// Specialization for const char*
template <>
const char* maximum<const char*>(const char* a, const char* b) {
    cout << "Specialized for const char*" << endl;
    return (strcmp(a, b) > 0) ? a : b;
}

// Generic comparison template
template <typename T>
bool areEqual(T a, T b) {
    return a == b;
}

// Specialization for floating point comparison
template <>
bool areEqual<double>(double a, double b) {
    const double epsilon = 1e-9;
    return abs(a - b) < epsilon;
}

// Generic print template
template <typename T>
void print(T value) {
    cout << "Value: " << value << endl;
}

// Specialization for bool
template <>
void print<bool>(bool value) {
    cout << "Boolean: " << (value ? "true" : "false") << endl;
}

// Specialization for const char*
template <>
void print<const char*>(const char* value) {
    cout << "String: \"" << value << "\"" << endl;
}

int main() {
    cout << "=== Maximum Function ===" << endl;
    cout << maximum(10, 20) << endl;
    cout << maximum(3.14, 2.71) << endl;
    cout << maximum("apple", "banana") << endl;
    cout << endl;

    cout << "=== Equality Comparison ===" << endl;
    cout << "10 == 10? " << areEqual(10, 10) << endl;
    cout << "10 == 20? " << areEqual(10, 20) << endl;
    cout << "3.14 == 3.14? " << areEqual(3.14, 3.14) << endl;
    cout << "3.14 == 3.14000001? " << areEqual(3.14, 3.14000001) << endl;
    cout << endl;

    cout << "=== Print Function ===" << endl;
    print(42);
    print(3.14);
    print(true);
    print("Hello");

    return 0;
}
```

## 4. Non-Type Template Parameters

### Using Values as Template Parameters

```cpp
#include <iostream>
#include <array>
using namespace std;

// Template with non-type parameter
template <typename T, int SIZE>
class FixedArray {
private:
    T data[SIZE];

public:
    FixedArray() {
        for (int i = 0; i < SIZE; i++) {
            data[i] = T();
        }
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return SIZE;
    }

    void fill(T value) {
        for (int i = 0; i < SIZE; i++) {
            data[i] = value;
        }
    }

    void print() const {
        cout << "[";
        for (int i = 0; i < SIZE; i++) {
            cout << data[i];
            if (i < SIZE - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

// Function template with non-type parameter
template <int N>
int factorial() {
    return N * factorial<N - 1>();
}

template <>
int factorial<0>() {
    return 1;
}

// Template for power calculation
template <int BASE, int EXP>
struct Power {
    static const int value = BASE * Power<BASE, EXP - 1>::value;
};

template <int BASE>
struct Power<BASE, 0> {
    static const int value = 1;
};

// Array sum template
template <typename T, int SIZE>
T arraySum(const T (&arr)[SIZE]) {
    T sum = T();
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    cout << "=== Fixed Array ===" << endl;
    FixedArray<int, 5> arr1;
    arr1.fill(7);
    arr1.print();

    FixedArray<double, 3> arr2;
    arr2[0] = 1.1;
    arr2[1] = 2.2;
    arr2[2] = 3.3;
    arr2.print();
    cout << endl;

    cout << "=== Compile-Time Factorial ===" << endl;
    cout << "5! = " << factorial<5>() << endl;
    cout << "10! = " << factorial<10>() << endl;
    cout << endl;

    cout << "=== Compile-Time Power ===" << endl;
    cout << "2^10 = " << Power<2, 10>::value << endl;
    cout << "3^4 = " << Power<3, 4>::value << endl;
    cout << endl;

    cout << "=== Array Sum ===" << endl;
    int intArr[] = {1, 2, 3, 4, 5};
    cout << "Sum of int array: " << arraySum(intArr) << endl;

    double doubleArr[] = {1.1, 2.2, 3.3};
    cout << "Sum of double array: " << arraySum(doubleArr) << endl;

    return 0;
}
```

## 5. Template Argument Deduction

### Automatic Type Inference

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Basic deduction
template <typename T>
void process(T value) {
    cout << "Processing value: " << value << endl;
}

// Deduction with references
template <typename T>
void processRef(T& value) {
    cout << "Processing reference: " << value << endl;
    value = value + value;  // Modify original
}

// Deduction with const reference
template <typename T>
void processConstRef(const T& value) {
    cout << "Processing const reference: " << value << endl;
}

// Deduction with pointers
template <typename T>
void processPtr(T* ptr) {
    if (ptr) {
        cout << "Processing pointer: " << *ptr << endl;
    }
}

// Deduction with arrays
template <typename T, size_t N>
void processArray(T (&arr)[N]) {
    cout << "Array of " << N << " elements: ";
    for (size_t i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Return type deduction (C++14)
template <typename T1, typename T2>
auto add(T1 a, T2 b) {
    return a + b;
}

// Multiple parameter deduction
template <typename T>
T sum(T first) {
    return first;
}

template <typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

int main() {
    cout << "=== Basic Deduction ===" << endl;
    process(42);           // T = int
    process(3.14);         // T = double
    process("Hello");      // T = const char*
    process(string("World")); // T = string
    cout << endl;

    cout << "=== Reference Deduction ===" << endl;
    int x = 5;
    cout << "Before: " << x << endl;
    processRef(x);
    cout << "After: " << x << endl;
    cout << endl;

    cout << "=== Const Reference Deduction ===" << endl;
    processConstRef(10);
    processConstRef(3.14);
    cout << endl;

    cout << "=== Pointer Deduction ===" << endl;
    int val = 100;
    processPtr(&val);
    cout << endl;

    cout << "=== Array Deduction ===" << endl;
    int arr1[] = {1, 2, 3, 4, 5};
    double arr2[] = {1.1, 2.2, 3.3};
    processArray(arr1);
    processArray(arr2);
    cout << endl;

    cout << "=== Return Type Deduction ===" << endl;
    auto result1 = add(10, 20);        // int
    auto result2 = add(10, 20.5);      // double
    auto result3 = add(string("Hello"), string(" World")); // string
    cout << "10 + 20 = " << result1 << endl;
    cout << "10 + 20.5 = " << result2 << endl;
    cout << "Hello + World = " << result3 << endl;
    cout << endl;

    cout << "=== Variadic Template ===" << endl;
    cout << "Sum: " << sum(1, 2, 3, 4, 5) << endl;
    cout << "Sum: " << sum(1.1, 2.2, 3.3) << endl;

    return 0;
}
```

## 6. Advanced Template Techniques

### SFINAE and Template Constraints

```cpp
#include <iostream>
#include <type_traits>
#include <string>
using namespace std;

// Enable if for integer types
template <typename T>
typename enable_if<is_integral<T>::value, T>::type
processNumber(T value) {
    cout << "Processing integer: " << value << endl;
    return value * 2;
}

// Enable if for floating point types
template <typename T>
typename enable_if<is_floating_point<T>::value, T>::type
processNumber(T value) {
    cout << "Processing floating point: " << value << endl;
    return value * 3.14;
}

// Template with static assertion
template <typename T>
class Container {
    static_assert(is_default_constructible<T>::value,
                 "T must be default constructible");

private:
    T data;

public:
    Container() : data() {}
    Container(T val) : data(val) {}

    T get() const { return data; }
    void set(T val) { data = val; }
};

// Concept-like template (C++11 style)
template <typename T>
typename enable_if<is_arithmetic<T>::value, void>::type
printType(T value) {
    cout << value << " is an arithmetic type" << endl;
}

template <typename T>
typename enable_if<!is_arithmetic<T>::value, void>::type
printType(T value) {
    cout << value << " is NOT an arithmetic type" << endl;
}

int main() {
    cout << "=== Process Number ===" << endl;
    cout << "Result: " << processNumber(10) << endl;
    cout << "Result: " << processNumber(3.14) << endl;
    cout << endl;

    cout << "=== Container ===" << endl;
    Container<int> c1(42);
    cout << "Container value: " << c1.get() << endl;

    Container<string> c2("Hello");
    cout << "Container value: " << c2.get() << endl;
    cout << endl;

    cout << "=== Type Printing ===" << endl;
    printType(42);
    printType(3.14);
    printType(string("Hello"));

    return 0;
}
```

## Exercises

### Exercise 1: Generic Algorithms
Implement template functions for: binary search, bubble sort, merge sort, and finding median.

### Exercise 2: Template Container
Create a generic dynamic array template class with resize, push_back, pop_back, and iterators.

### Exercise 3: Mathematical Templates
Implement template functions for: dot product, cross product, matrix operations, and vector normalization.

### Exercise 4: Smart Comparator
Create a template-based comparison framework that can compare any types with custom comparators.

### Exercise 5: Template Metaprogramming
Use templates for compile-time computation: Fibonacci, prime checking, and GCD.

## Exercise Solutions

### Solution 1: Generic Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Binary search template
template <typename T>
int binarySearch(const vector<T>& arr, T target) {
    int left = 0, right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }

    return -1;
}

// Bubble sort template
template <typename T>
void bubbleSort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge function for merge sort
template <typename T>
void merge(vector<T>& arr, int left, int mid, int right) {
    vector<T> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
}

// Merge sort template
template <typename T>
void mergeSort(vector<T>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Find median template
template <typename T>
double findMedian(vector<T> arr) {
    sort(arr.begin(), arr.end());
    int n = arr.size();

    if (n % 2 == 0) {
        return (arr[n/2 - 1] + arr[n/2]) / 2.0;
    } else {
        return arr[n/2];
    }
}

// Print vector template
template <typename T>
void printVector(const vector<T>& arr, const string& name) {
    cout << name << ": ";
    for (const T& val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr1 = {1, 3, 5, 7, 9, 11, 13, 15};
    cout << "Binary search for 7: index " << binarySearch(arr1, 7) << endl;
    cout << "Binary search for 10: index " << binarySearch(arr1, 10) << endl;
    cout << endl;

    vector<int> arr2 = {64, 34, 25, 12, 22, 11, 90};
    printVector(arr2, "Before bubble sort");
    bubbleSort(arr2);
    printVector(arr2, "After bubble sort");
    cout << endl;

    vector<double> arr3 = {3.14, 1.41, 2.71, 0.58, 1.73};
    printVector(arr3, "Before merge sort");
    mergeSort(arr3, 0, arr3.size() - 1);
    printVector(arr3, "After merge sort");
    cout << endl;

    vector<int> arr4 = {5, 2, 8, 1, 9};
    cout << "Median: " << findMedian(arr4) << endl;

    return 0;
}
```

### Solution 2: Template Container

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t capacity;
    size_t count;

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), capacity(0), count(0) {}

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (count >= capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }
        data[count++] = value;
    }

    void pop_back() {
        if (count > 0) {
            count--;
        }
    }

    T& operator[](size_t index) {
        if (index >= count) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= count) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }

    // Iterator support
    T* begin() { return data; }
    T* end() { return data + count; }
    const T* begin() const { return data; }
    const T* end() const { return data + count; }

    void print() const {
        cout << "[";
        for (size_t i = 0; i < count; i++) {
            cout << data[i];
            if (i < count - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    DynamicArray<int> arr;

    for (int i = 1; i <= 10; i++) {
        arr.push_back(i * 10);
    }

    cout << "Array: ";
    arr.print();

    cout << "Size: " << arr.size() << endl;
    cout << "Element at index 5: " << arr[5] << endl;

    arr.pop_back();
    arr.pop_back();

    cout << "After removing 2 elements: ";
    arr.print();

    cout << "Using iterator: ";
    for (auto& val : arr) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

### Solution 3: Mathematical Templates

```cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Dot product
template <typename T>
T dotProduct(const vector<T>& a, const vector<T>& b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Vectors must be same size");
    }

    T result = T();
    for (size_t i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}

// Cross product (3D vectors only)
template <typename T>
vector<T> crossProduct(const vector<T>& a, const vector<T>& b) {
    if (a.size() != 3 || b.size() != 3) {
        throw invalid_argument("Cross product requires 3D vectors");
    }

    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
}

// Vector magnitude
template <typename T>
double magnitude(const vector<T>& v) {
    T sumOfSquares = T();
    for (const T& val : v) {
        sumOfSquares += val * val;
    }
    return sqrt(static_cast<double>(sumOfSquares));
}

// Vector normalization
template <typename T>
vector<double> normalize(const vector<T>& v) {
    double mag = magnitude(v);
    vector<double> result;

    if (mag > 0) {
        for (const T& val : v) {
            result.push_back(val / mag);
        }
    }

    return result;
}

// Matrix addition
template <typename T>
vector<vector<T>> matrixAdd(const vector<vector<T>>& a,
                           const vector<vector<T>>& b) {
    if (a.size() != b.size() || a[0].size() != b[0].size()) {
        throw invalid_argument("Matrices must be same size");
    }

    vector<vector<T>> result(a.size(), vector<T>(a[0].size()));
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < a[0].size(); j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

// Matrix multiplication
template <typename T>
vector<vector<T>> matrixMultiply(const vector<vector<T>>& a,
                                const vector<vector<T>>& b) {
    if (a[0].size() != b.size()) {
        throw invalid_argument("Invalid matrix dimensions");
    }

    vector<vector<T>> result(a.size(), vector<T>(b[0].size(), T()));

    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < b[0].size(); j++) {
            for (size_t k = 0; k < b.size(); k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

template <typename T>
void printVector(const vector<T>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i < v.size() - 1) cout << ", ";
    }
    cout << "]";
}

int main() {
    vector<double> v1 = {1, 2, 3};
    vector<double> v2 = {4, 5, 6};

    cout << "v1: "; printVector(v1); cout << endl;
    cout << "v2: "; printVector(v2); cout << endl;

    cout << "Dot product: " << dotProduct(v1, v2) << endl;

    auto cross = crossProduct(v1, v2);
    cout << "Cross product: "; printVector(cross); cout << endl;

    cout << "Magnitude of v1: " << magnitude(v1) << endl;

    auto normalized = normalize(v1);
    cout << "Normalized v1: "; printVector(normalized); cout << endl;

    vector<vector<int>> m1 = {{1, 2}, {3, 4}};
    vector<vector<int>> m2 = {{5, 6}, {7, 8}};

    auto mSum = matrixAdd(m1, m2);
    cout << "\nMatrix sum:" << endl;
    for (const auto& row : mSum) {
        printVector(row);
        cout << endl;
    }

    auto mProd = matrixMultiply(m1, m2);
    cout << "\nMatrix product:" << endl;
    for (const auto& row : mProd) {
        printVector(row);
        cout << endl;
    }

    return 0;
}
```

### Solution 4 & 5: Due to space, showing key concepts

```cpp
// Smart Comparator Framework
template <typename T, typename Comparator>
class SmartSort {
public:
    static void sort(vector<T>& data, Comparator comp) {
        std::sort(data.begin(), data.end(), comp);
    }
};

// Template Metaprogramming
template <int N>
struct Fibonacci {
    static const int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template <>
struct Fibonacci<0> { static const int value = 0; };

template <>
struct Fibonacci<1> { static const int value = 1; };

int main() {
    cout << "Fibonacci<10> = " << Fibonacci<10>::value << endl;
    return 0;
}
```

## Summary

In this lesson, you learned:
- How to create and use function templates
- Template specialization techniques
- Working with multiple template parameters
- Non-type template parameters
- Template argument deduction
- Advanced template techniques including SFINAE
- Best practices for template programming

## Checklist

- [ ] I understand basic function template syntax
- [ ] I can create templates with multiple parameters
- [ ] I know how to specialize templates
- [ ] I understand non-type template parameters
- [ ] I can work with template argument deduction
- [ ] I understand SFINAE basics
- [ ] I completed all exercises and understand their solutions
- [ ] I can debug template compilation errors

## Next Steps
In the next lesson, we'll explore Lambda Expressions, learning how to write inline anonymous functions with captures for powerful functional programming.
