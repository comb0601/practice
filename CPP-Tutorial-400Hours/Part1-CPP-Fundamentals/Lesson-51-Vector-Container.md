# Lesson 51: Vector Container

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to STL and Vectors
2. Creating and Initializing Vectors
3. Vector Operations and Methods
4. Accessing Elements
5. Modifying Vectors
6. Iterating Through Vectors
7. Vector Capacity vs Size
8. Common Algorithms with Vectors
9. 2D Vectors
10. Exercises and Projects

---

## 1. Introduction to STL and Vectors

The **Standard Template Library (STL)** is a powerful collection of template classes and functions that provide general-purpose data structures and algorithms. The **vector** is one of the most commonly used STL containers.

### What is a Vector?

A vector is a dynamic array that can grow or shrink in size automatically. Unlike regular arrays, vectors manage their own memory.

**Key Features:**
- **Dynamic sizing**: Grows/shrinks automatically
- **Random access**: O(1) access time using index
- **Contiguous memory**: Elements stored sequentially in memory
- **Efficient insertion at end**: O(1) amortized time
- **Type-safe**: Template-based, compile-time type checking

**Vector vs Array:**
```
Array:
- Fixed size
- No built-in methods
- Manual memory management
- int arr[10];

Vector:
- Dynamic size
- Rich set of methods
- Automatic memory management
- std::vector<int> vec;
```

---

## 2. Creating and Initializing Vectors

### Basic Declaration:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Empty vector of integers
    std::vector<int> v1;

    // Vector with 5 elements, all initialized to 0
    std::vector<int> v2(5);

    // Vector with 5 elements, all initialized to 10
    std::vector<int> v3(5, 10);

    // Initializer list (C++11)
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    std::vector<int> v5{10, 20, 30, 40, 50};

    // Copy constructor
    std::vector<int> v6(v4);

    // Range constructor
    std::vector<int> v7(v4.begin(), v4.begin() + 3);  // First 3 elements

    std::cout << "v2 size: " << v2.size() << "\n";
    std::cout << "v3 elements: ";
    for (int x : v3) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "v4 elements: ";
    for (int x : v4) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

**Output:**
```
v2 size: 5
v3 elements: 10 10 10 10 10
v4 elements: 1 2 3 4 5
```

### Different Data Types:

```cpp
#include <iostream>
#include <vector>
#include <string>

int main()
{
    // Vector of doubles
    std::vector<double> prices = {19.99, 29.99, 39.99};

    // Vector of strings
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};

    // Vector of characters
    std::vector<char> letters = {'a', 'b', 'c', 'd'};

    // Vector of booleans
    std::vector<bool> flags = {true, false, true, true};

    std::cout << "Prices: ";
    for (double price : prices) {
        std::cout << "$" << price << " ";
    }
    std::cout << "\n";

    std::cout << "Names: ";
    for (const std::string& name : names) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 3. Vector Operations and Methods

### Essential Methods:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {10, 20, 30};

    // size() - number of elements
    std::cout << "Size: " << vec.size() << "\n";

    // empty() - check if empty
    std::cout << "Is empty? " << (vec.empty() ? "Yes" : "No") << "\n";

    // capacity() - allocated storage
    std::cout << "Capacity: " << vec.capacity() << "\n";

    // max_size() - maximum possible size
    std::cout << "Max size: " << vec.max_size() << "\n";

    // clear() - remove all elements
    vec.clear();
    std::cout << "After clear, size: " << vec.size() << "\n";

    return 0;
}
```

### Adding Elements:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec;

    // push_back() - add element at end
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "After push_back: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // emplace_back() - construct element in place (more efficient)
    vec.emplace_back(40);
    vec.emplace_back(50);

    std::cout << "After emplace_back: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // insert() - insert at specific position
    vec.insert(vec.begin() + 2, 25);  // Insert 25 at index 2

    std::cout << "After insert: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // insert multiple elements
    vec.insert(vec.begin(), 3, 5);  // Insert three 5's at beginning

    std::cout << "After multiple insert: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

**Output:**
```
After push_back: 10 20 30
After emplace_back: 10 20 30 40 50
After insert: 10 20 25 30 40 50
After multiple insert: 5 5 5 10 20 25 30 40 50
```

### Removing Elements:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // pop_back() - remove last element
    vec.pop_back();
    std::cout << "After pop_back: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // erase() - remove element at position
    vec.erase(vec.begin() + 1);  // Remove element at index 1
    std::cout << "After erase: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // erase range
    vec = {10, 20, 30, 40, 50, 60, 70};
    vec.erase(vec.begin() + 2, vec.begin() + 5);  // Remove elements 30, 40, 50
    std::cout << "After range erase: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 4. Accessing Elements

### Multiple Access Methods:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // [] operator (no bounds checking)
    std::cout << "vec[0]: " << vec[0] << "\n";
    std::cout << "vec[2]: " << vec[2] << "\n";

    // at() method (with bounds checking)
    std::cout << "vec.at(1): " << vec.at(1) << "\n";

    // front() - first element
    std::cout << "First element: " << vec.front() << "\n";

    // back() - last element
    std::cout << "Last element: " << vec.back() << "\n";

    // data() - pointer to underlying array
    int* ptr = vec.data();
    std::cout << "First element via pointer: " << *ptr << "\n";
    std::cout << "Second element via pointer: " << *(ptr + 1) << "\n";

    // Modifying elements
    vec[0] = 100;
    vec.at(1) = 200;

    std::cout << "After modification: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Exception handling with at()
    try {
        std::cout << vec.at(100);  // Out of bounds!
    }
    catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 5. Modifying Vectors

### Resize and Reserve:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {1, 2, 3};

    std::cout << "Initial - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << "\n";

    // resize() - change the number of elements
    vec.resize(5);  // Add 2 elements (initialized to 0)
    std::cout << "After resize(5) - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << "\n";
    std::cout << "Elements: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // resize with value
    vec.resize(7, 99);  // Add 2 elements, initialized to 99
    std::cout << "After resize(7, 99): ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // reserve() - allocate storage without changing size
    vec.clear();
    vec.reserve(100);
    std::cout << "After reserve(100) - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << "\n";

    // shrink_to_fit() - reduce capacity to size
    vec = {1, 2, 3};
    vec.reserve(100);
    std::cout << "Before shrink - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << "\n";
    vec.shrink_to_fit();
    std::cout << "After shrink - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << "\n";

    return 0;
}
```

### Swap and Assignment:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {10, 20, 30, 40, 50};

    std::cout << "Before swap:\n";
    std::cout << "v1: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\nv2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n\n";

    // swap() - exchange contents
    v1.swap(v2);

    std::cout << "After swap:\n";
    std::cout << "v1: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\nv2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n\n";

    // assign() - replace contents
    v1.assign(3, 100);  // 3 elements, all 100
    std::cout << "After assign(3, 100): ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";

    // assign with initializer list
    v1.assign({5, 10, 15, 20});
    std::cout << "After assign with list: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 6. Iterating Through Vectors

### Different Iteration Methods:

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // Method 1: Index-based loop
    std::cout << "Index-based loop: ";
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";

    // Method 2: Iterator
    std::cout << "Iterator: ";
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Method 3: Auto iterator (C++11)
    std::cout << "Auto iterator: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Method 4: Range-based for loop (C++11) - RECOMMENDED
    std::cout << "Range-based for: ";
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Method 5: Range-based with reference (for modification)
    std::cout << "Modifying with range-based for: ";
    for (int& x : vec) {
        x *= 2;  // Double each element
    }
    for (int x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Method 6: Reverse iteration
    std::cout << "Reverse iteration: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 7. Vector Capacity vs Size

### Understanding Growth:

```cpp
#include <iostream>
#include <vector>

void printInfo(const std::vector<int>& vec, const std::string& message)
{
    std::cout << message << "\n";
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << "\n\n";
}

int main()
{
    std::vector<int> vec;
    printInfo(vec, "Initial empty vector:");

    // Observe capacity growth
    for (int i = 1; i <= 10; i++) {
        vec.push_back(i);
        std::cout << "After push_back(" << i << ") - Size: " << vec.size()
                  << ", Capacity: " << vec.capacity() << "\n";
    }

    std::cout << "\n";

    // Pre-allocating with reserve
    std::vector<int> vec2;
    vec2.reserve(10);
    printInfo(vec2, "After reserve(10):");

    for (int i = 1; i <= 10; i++) {
        vec2.push_back(i);
    }
    printInfo(vec2, "After 10 push_backs:");

    return 0;
}
```

### Performance Comparison:

```cpp
#include <iostream>
#include <vector>
#include <chrono>

int main()
{
    const int NUM_ELEMENTS = 1000000;

    // Without reserve
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> v1;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        v1.push_back(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // With reserve
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> v2;
    v2.reserve(NUM_ELEMENTS);
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        v2.push_back(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Without reserve: " << duration1.count() << " ms\n";
    std::cout << "With reserve: " << duration2.count() << " ms\n";
    std::cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x\n";

    return 0;
}
```

---

## 8. Common Algorithms with Vectors

### Searching and Finding:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> vec = {10, 20, 30, 40, 50, 30, 60};

    // find() - find first occurrence
    auto it = std::find(vec.begin(), vec.end(), 30);
    if (it != vec.end()) {
        std::cout << "Found 30 at index: " << (it - vec.begin()) << "\n";
    }

    // find_if() - find with condition
    auto it2 = std::find_if(vec.begin(), vec.end(), [](int x) { return x > 40; });
    if (it2 != vec.end()) {
        std::cout << "First element > 40: " << *it2 << "\n";
    }

    // count() - count occurrences
    int count = std::count(vec.begin(), vec.end(), 30);
    std::cout << "Number of 30s: " << count << "\n";

    // count_if() - count with condition
    int countGreater = std::count_if(vec.begin(), vec.end(), [](int x) { return x > 30; });
    std::cout << "Numbers > 30: " << countGreater << "\n";

    return 0;
}
```

### Sorting and Manipulating:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> vec = {50, 20, 60, 10, 30, 40};

    // sort() - ascending order
    std::sort(vec.begin(), vec.end());
    std::cout << "Sorted ascending: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // sort descending
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    std::cout << "Sorted descending: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // reverse()
    std::reverse(vec.begin(), vec.end());
    std::cout << "Reversed: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // min_element() and max_element()
    auto minIt = std::min_element(vec.begin(), vec.end());
    auto maxIt = std::max_element(vec.begin(), vec.end());
    std::cout << "Min: " << *minIt << ", Max: " << *maxIt << "\n";

    // remove duplicates (requires sorting first)
    vec = {1, 2, 2, 3, 3, 3, 4, 5, 5};
    std::sort(vec.begin(), vec.end());
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
    std::cout << "After removing duplicates: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 9. 2D Vectors

### Creating 2D Vectors:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Method 1: Initialize with size
    int rows = 3, cols = 4;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    std::cout << "3x4 matrix initialized to 0:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Method 2: Initializer list
    std::vector<std::vector<int>> matrix2 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "\n3x3 matrix with initializer:\n";
    for (const auto& row : matrix2) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    // Method 3: Building dynamically
    std::vector<std::vector<int>> jagged;
    jagged.push_back({1, 2});
    jagged.push_back({3, 4, 5});
    jagged.push_back({6, 7, 8, 9});

    std::cout << "\nJagged array:\n";
    for (size_t i = 0; i < jagged.size(); i++) {
        std::cout << "Row " << i << ": ";
        for (int val : jagged[i]) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Matrix Operations:

```cpp
#include <iostream>
#include <vector>

// Function to print matrix
void printMatrix(const std::vector<std::vector<int>>& matrix)
{
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << "\t";
        }
        std::cout << "\n";
    }
}

// Matrix addition
std::vector<std::vector<int>> addMatrices(
    const std::vector<std::vector<int>>& a,
    const std::vector<std::vector<int>>& b)
{
    int rows = a.size();
    int cols = a[0].size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int main()
{
    std::vector<std::vector<int>> matrix1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::vector<std::vector<int>> matrix2 = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    std::cout << "Matrix 1:\n";
    printMatrix(matrix1);

    std::cout << "\nMatrix 2:\n";
    printMatrix(matrix2);

    std::vector<std::vector<int>> sum = addMatrices(matrix1, matrix2);
    std::cout << "\nSum:\n";
    printMatrix(sum);

    return 0;
}
```

---

## 10. Exercises and Projects

### Exercise 1: Vector Statistics

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
    std::vector<int> numbers = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    // Sum
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum: " << sum << "\n";

    // Average
    double average = sum / (double)numbers.size();
    std::cout << "Average: " << average << "\n";

    // Min and Max
    int minVal = *std::min_element(numbers.begin(), numbers.end());
    int maxVal = *std::max_element(numbers.begin(), numbers.end());
    std::cout << "Min: " << minVal << ", Max: " << maxVal << "\n";

    // Range
    std::cout << "Range: " << (maxVal - minVal) << "\n";

    return 0;
}
```

### Exercise 2: Remove Odd Numbers

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Original: ";
    for (int x : numbers) std::cout << x << " ";
    std::cout << "\n";

    // Remove odd numbers
    numbers.erase(
        std::remove_if(numbers.begin(), numbers.end(),
                      [](int x) { return x % 2 != 0; }),
        numbers.end()
    );

    std::cout << "After removing odds: ";
    for (int x : numbers) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Exercise 3: Student Grade System

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

struct Student {
    std::string name;
    std::vector<int> grades;

    double getAverage() const {
        return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }

    char getLetterGrade() const {
        double avg = getAverage();
        if (avg >= 90) return 'A';
        if (avg >= 80) return 'B';
        if (avg >= 70) return 'C';
        if (avg >= 60) return 'D';
        return 'F';
    }
};

int main()
{
    std::vector<Student> students = {
        {"Alice", {85, 90, 88, 92}},
        {"Bob", {78, 72, 80, 75}},
        {"Charlie", {95, 98, 96, 94}},
        {"David", {65, 70, 68, 72}}
    };

    std::cout << "Student Grade Report:\n";
    std::cout << "=====================\n";
    for (const auto& student : students) {
        std::cout << student.name << ":\n";
        std::cout << "  Grades: ";
        for (int grade : student.grades) {
            std::cout << grade << " ";
        }
        std::cout << "\n";
        std::cout << "  Average: " << student.getAverage() << "\n";
        std::cout << "  Letter Grade: " << student.getLetterGrade() << "\n\n";
    }

    // Find top student
    auto topStudent = std::max_element(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getAverage() < b.getAverage();
        });

    std::cout << "Top Student: " << topStudent->name
              << " with average " << topStudent->getAverage() << "\n";

    return 0;
}
```

### Project: Dynamic Array Manager

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class ArrayManager {
private:
    std::vector<int> data;

public:
    void addElement(int value) {
        data.push_back(value);
        std::cout << "Added " << value << "\n";
    }

    void removeElement(int value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            data.erase(it);
            std::cout << "Removed " << value << "\n";
        } else {
            std::cout << value << " not found\n";
        }
    }

    void display() const {
        std::cout << "Array: [";
        for (size_t i = 0; i < data.size(); i++) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    void sort() {
        std::sort(data.begin(), data.end());
        std::cout << "Array sorted\n";
    }

    void reverse() {
        std::reverse(data.begin(), data.end());
        std::cout << "Array reversed\n";
    }

    void search(int value) const {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            std::cout << value << " found at index " << (it - data.begin()) << "\n";
        } else {
            std::cout << value << " not found\n";
        }
    }

    void statistics() const {
        if (data.empty()) {
            std::cout << "Array is empty\n";
            return;
        }

        int sum = std::accumulate(data.begin(), data.end(), 0);
        int minVal = *std::min_element(data.begin(), data.end());
        int maxVal = *std::max_element(data.begin(), data.end());

        std::cout << "Statistics:\n";
        std::cout << "  Size: " << data.size() << "\n";
        std::cout << "  Sum: " << sum << "\n";
        std::cout << "  Average: " << (double)sum / data.size() << "\n";
        std::cout << "  Min: " << minVal << "\n";
        std::cout << "  Max: " << maxVal << "\n";
    }
};

int main()
{
    ArrayManager manager;

    manager.addElement(50);
    manager.addElement(20);
    manager.addElement(80);
    manager.addElement(10);
    manager.addElement(60);

    manager.display();
    manager.statistics();

    manager.sort();
    manager.display();

    manager.search(80);
    manager.search(100);

    manager.removeElement(20);
    manager.display();

    manager.reverse();
    manager.display();

    return 0;
}
```

---

## Key Takeaways

1. ✅ Vectors are dynamic arrays that grow/shrink automatically
2. ✅ Use `push_back()` or `emplace_back()` to add elements
3. ✅ Access elements with `[]`, `at()`, `front()`, or `back()`
4. ✅ Use `reserve()` to pre-allocate memory for better performance
5. ✅ Size is the number of elements, capacity is allocated storage
6. ✅ Range-based for loops are the cleanest way to iterate
7. ✅ STL algorithms work seamlessly with vectors
8. ✅ 2D vectors are vectors of vectors
9. ✅ Always initialize vectors to avoid undefined behavior
10. ✅ Vectors automatically manage memory (no manual delete needed)

---

## Common Mistakes

### Mistake 1: Using size() in loop with modification
```cpp
❌ for (size_t i = 0; i < vec.size(); i++) {
    vec.push_back(i);  // Infinite loop!
}

✅ size_t n = vec.size();
   for (size_t i = 0; i < n; i++) {
    vec.push_back(i);
}
```

### Mistake 2: Not reserving capacity
```cpp
❌ vector<int> v;
   for (int i = 0; i < 1000000; i++) {
    v.push_back(i);  // Many reallocations
}

✅ vector<int> v;
   v.reserve(1000000);
   for (int i = 0; i < 1000000; i++) {
    v.push_back(i);
}
```

### Mistake 3: Iterator invalidation
```cpp
❌ for (auto it = vec.begin(); it != vec.end(); ++it) {
    vec.push_back(*it);  // Invalidates iterators!
}

✅ size_t n = vec.size();
   for (size_t i = 0; i < n; i++) {
    vec.push_back(vec[i]);
}
```

---

## Summary Checklist

Before moving to Lesson 52, ensure you can:
- [ ] Create and initialize vectors in multiple ways
- [ ] Add and remove elements using push_back, pop_back, insert, erase
- [ ] Access elements safely with at() and []
- [ ] Iterate through vectors using different methods
- [ ] Understand size vs capacity
- [ ] Use reserve() for performance optimization
- [ ] Apply STL algorithms to vectors
- [ ] Create and work with 2D vectors
- [ ] Sort, search, and manipulate vector contents
- [ ] Write programs that use vectors effectively

---

## Next Lesson Preview

In **Lesson 52**, we'll learn about:
- List container (doubly-linked list)
- Deque container (double-ended queue)
- When to use each container
- Performance comparisons
- Container adaptors

**Excellent work mastering vectors!**
