# Lesson 17: STL Introduction

**Duration**: 2.5-3 hours

## Overview

The Standard Template Library (STL) is a powerful collection of template classes and functions that provide common data structures (containers) and algorithms. It's one of the most important parts of modern C++ programming, enabling efficient and reusable code.

## Topics Covered

1. What is the STL?
2. STL Components Overview
3. Iterators
4. Basic Container Usage
5. Basic Algorithm Usage
6. Function Objects (Functors)
7. Best Practices

## 1. What is the STL?

The STL consists of four main components:
- **Containers**: Data structures (vector, list, map, set, etc.)
- **Iterators**: Objects that point to elements in containers
- **Algorithms**: Functions for searching, sorting, modifying data
- **Function Objects**: Objects that act like functions

**Key Benefits:**
- Generic programming using templates
- Highly optimized implementations
- Type-safe
- Extensive functionality
- Standard across all C++ compilers

## 2. STL Components Overview

### Example 1: STL Components Demo - Complete Program

**File: `stl_overview.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    std::cout << "=== STL Components Overview ===" << std::endl;

    // 1. CONTAINER - std::vector
    std::cout << "\n--- Container: std::vector ---" << std::endl;
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3};

    std::cout << "Original vector: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 2. ITERATOR - accessing elements
    std::cout << "\n--- Iterator: Accessing Elements ---" << std::endl;
    std::cout << "Using iterator: ";
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 3. ALGORITHM - sorting
    std::cout << "\n--- Algorithm: std::sort ---" << std::endl;
    std::sort(numbers.begin(), numbers.end());
    std::cout << "Sorted vector: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 4. ALGORITHM - finding
    std::cout << "\n--- Algorithm: std::find ---" << std::endl;
    auto found = std::find(numbers.begin(), numbers.end(), 8);
    if (found != numbers.end()) {
        std::cout << "Found 8 at position: " << (found - numbers.begin()) << std::endl;
    }

    // 5. ALGORITHM - counting
    std::cout << "\n--- Algorithm: std::count ---" << std::endl;
    std::vector<int> values = {1, 2, 3, 2, 4, 2, 5};
    int count = std::count(values.begin(), values.end(), 2);
    std::cout << "Count of 2 in vector: " << count << std::endl;

    // 6. CONTAINER - std::string (yes, string is an STL container!)
    std::cout << "\n--- Container: std::string ---" << std::endl;
    std::string text = "Hello, STL!";
    std::cout << "String: " << text << std::endl;
    std::cout << "Length: " << text.size() << std::endl;
    std::cout << "First char: " << text[0] << std::endl;
    std::cout << "Last char: " << text.back() << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall stl_overview.cpp -o stl_overview
./stl_overview
```

## 3. Iterators

Iterators are objects that point to elements in containers, similar to pointers.

### Example 2: Iterator Types - Complete Program

**File: `iterators_demo.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <string>

int main() {
    std::cout << "=== Iterators Demo ===" << std::endl;

    // Vector with different iterator types
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // 1. Forward iteration
    std::cout << "\n--- Forward Iteration ---" << std::endl;
    std::cout << "Elements: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 2. Reverse iteration
    std::cout << "\n--- Reverse Iteration ---" << std::endl;
    std::cout << "Elements (reversed): ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 3. Const iteration (read-only)
    std::cout << "\n--- Const Iteration ---" << std::endl;
    std::cout << "Read-only access: ";
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << " ";
        // *it = 100;  // ERROR: cannot modify through const iterator
    }
    std::cout << std::endl;

    // 4. Modifying through iterators
    std::cout << "\n--- Modifying Through Iterators ---" << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        *it *= 2;  // Double each element
    }
    std::cout << "After doubling: ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 5. Iterator arithmetic
    std::cout << "\n--- Iterator Arithmetic ---" << std::endl;
    auto it = vec.begin();
    std::cout << "First element: " << *it << std::endl;

    it += 2;  // Move forward 2 positions
    std::cout << "Third element: " << *it << std::endl;

    it = vec.end() - 1;  // Last element
    std::cout << "Last element: " << *it << std::endl;

    // 6. Distance between iterators
    std::cout << "\n--- Iterator Distance ---" << std::endl;
    auto start = vec.begin();
    auto end = vec.end();
    std::cout << "Number of elements: " << (end - start) << std::endl;

    // 7. Using iterators with different containers
    std::cout << "\n--- List Iterators ---" << std::endl;
    std::list<std::string> names = {"Alice", "Bob", "Charlie", "David"};

    std::cout << "Names: ";
    for (auto it = names.begin(); it != names.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 8. Range-based for loop (uses iterators internally)
    std::cout << "\n--- Range-Based For Loop ---" << std::endl;
    std::cout << "Modern syntax: ";
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall iterators_demo.cpp -o iterators_demo
./iterators_demo
```

## 4. Basic Container Usage

### Example 3: Container Operations - Complete Program

**File: `container_basics.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "=== Container Basics Demo ===" << std::endl;

    // Creating vectors
    std::cout << "\n--- Creating Vectors ---" << std::endl;

    std::vector<int> v1;  // Empty vector
    std::vector<int> v2(5);  // 5 elements, default-initialized to 0
    std::vector<int> v3(5, 100);  // 5 elements, all initialized to 100
    std::vector<int> v4 = {1, 2, 3, 4, 5};  // Initializer list
    std::vector<int> v5(v4);  // Copy constructor

    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << ", elements: ";
    for (int val : v2) std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "v3 size: " << v3.size() << ", elements: ";
    for (int val : v3) std::cout << val << " ";
    std::cout << std::endl;

    // Adding elements
    std::cout << "\n--- Adding Elements ---" << std::endl;
    std::vector<int> nums;

    nums.push_back(10);
    nums.push_back(20);
    nums.push_back(30);
    std::cout << "After push_back: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    nums.insert(nums.begin() + 1, 15);  // Insert 15 at position 1
    std::cout << "After insert: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // Accessing elements
    std::cout << "\n--- Accessing Elements ---" << std::endl;
    std::vector<int> data = {100, 200, 300, 400, 500};

    std::cout << "data[0] = " << data[0] << std::endl;
    std::cout << "data[2] = " << data[2] << std::endl;
    std::cout << "data.at(1) = " << data.at(1) << std::endl;
    std::cout << "data.front() = " << data.front() << std::endl;
    std::cout << "data.back() = " << data.back() << std::endl;

    // Removing elements
    std::cout << "\n--- Removing Elements ---" << std::endl;
    std::vector<int> items = {1, 2, 3, 4, 5};

    std::cout << "Original: ";
    for (int i : items) std::cout << i << " ";
    std::cout << std::endl;

    items.pop_back();  // Remove last element
    std::cout << "After pop_back: ";
    for (int i : items) std::cout << i << " ";
    std::cout << std::endl;

    items.erase(items.begin() + 1);  // Remove element at position 1
    std::cout << "After erase: ";
    for (int i : items) std::cout << i << " ";
    std::cout << std::endl;

    // Size and capacity
    std::cout << "\n--- Size and Capacity ---" << std::endl;
    std::vector<int> vec;
    std::cout << "Initial - Size: " << vec.size()
              << ", Capacity: " << vec.capacity() << std::endl;

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        std::cout << "After adding " << i << " - Size: " << vec.size()
                  << ", Capacity: " << vec.capacity() << std::endl;
    }

    // Clearing and checking empty
    std::cout << "\n--- Clear and Empty ---" << std::endl;
    std::vector<int> temp = {1, 2, 3};
    std::cout << "Before clear - Size: " << temp.size()
              << ", Empty: " << temp.empty() << std::endl;

    temp.clear();
    std::cout << "After clear - Size: " << temp.size()
              << ", Empty: " << temp.empty() << std::endl;

    // Resizing
    std::cout << "\n--- Resizing ---" << std::endl;
    std::vector<int> resizable = {1, 2, 3};
    std::cout << "Original size: " << resizable.size() << std::endl;

    resizable.resize(5);  // Grow to 5 elements
    std::cout << "After resize(5): ";
    for (int val : resizable) std::cout << val << " ";
    std::cout << std::endl;

    resizable.resize(7, 99);  // Grow to 7, new elements are 99
    std::cout << "After resize(7, 99): ";
    for (int val : resizable) std::cout << val << " ";
    std::cout << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall container_basics.cpp -o container_basics
./container_basics
```

## 5. Basic Algorithm Usage

### Example 4: Common Algorithms - Complete Program

**File: `algorithm_basics.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

int main() {
    std::cout << "=== Algorithm Basics Demo ===" << std::endl;

    // 1. Sorting
    std::cout << "\n--- std::sort ---" << std::endl;
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::cout << "Before sort: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    std::sort(numbers.begin(), numbers.end());
    std::cout << "After sort (ascending): ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    std::cout << "After sort (descending): ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // 2. Searching
    std::cout << "\n--- std::find ---" << std::endl;
    std::vector<int> data = {10, 20, 30, 40, 50};

    auto it = std::find(data.begin(), data.end(), 30);
    if (it != data.end()) {
        std::cout << "Found 30 at position: " << (it - data.begin()) << std::endl;
    } else {
        std::cout << "30 not found" << std::endl;
    }

    it = std::find(data.begin(), data.end(), 99);
    if (it != data.end()) {
        std::cout << "Found 99" << std::endl;
    } else {
        std::cout << "99 not found" << std::endl;
    }

    // 3. Binary search (requires sorted container)
    std::cout << "\n--- std::binary_search ---" << std::endl;
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};

    bool found = std::binary_search(sorted.begin(), sorted.end(), 7);
    std::cout << "7 found: " << (found ? "yes" : "no") << std::endl;

    found = std::binary_search(sorted.begin(), sorted.end(), 6);
    std::cout << "6 found: " << (found ? "yes" : "no") << std::endl;

    // 4. Counting
    std::cout << "\n--- std::count ---" << std::endl;
    std::vector<int> values = {1, 2, 3, 2, 4, 2, 5, 2};
    int count = std::count(values.begin(), values.end(), 2);
    std::cout << "Count of 2: " << count << std::endl;

    // 5. Min and Max
    std::cout << "\n--- std::min_element, std::max_element ---" << std::endl;
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};

    auto min_it = std::min_element(nums.begin(), nums.end());
    auto max_it = std::max_element(nums.begin(), nums.end());

    std::cout << "Min element: " << *min_it << std::endl;
    std::cout << "Max element: " << *max_it << std::endl;

    // 6. Accumulate (sum)
    std::cout << "\n--- std::accumulate ---" << std::endl;
    std::vector<int> sequence = {1, 2, 3, 4, 5};
    int sum = std::accumulate(sequence.begin(), sequence.end(), 0);
    std::cout << "Sum of elements: " << sum << std::endl;

    int product = std::accumulate(sequence.begin(), sequence.end(), 1,
                                  std::multiplies<int>());
    std::cout << "Product of elements: " << product << std::endl;

    // 7. Transform
    std::cout << "\n--- std::transform ---" << std::endl;
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> output(input.size());

    std::transform(input.begin(), input.end(), output.begin(),
                   [](int x) { return x * 2; });

    std::cout << "Original: ";
    for (int n : input) std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "Transformed (×2): ";
    for (int n : output) std::cout << n << " ";
    std::cout << std::endl;

    // 8. Reverse
    std::cout << "\n--- std::reverse ---" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "Before reverse: ";
    for (int n : vec) std::cout << n << " ";
    std::cout << std::endl;

    std::reverse(vec.begin(), vec.end());
    std::cout << "After reverse: ";
    for (int n : vec) std::cout << n << " ";
    std::cout << std::endl;

    // 9. Fill
    std::cout << "\n--- std::fill ---" << std::endl;
    std::vector<int> filled(5);
    std::fill(filled.begin(), filled.end(), 42);
    std::cout << "Filled with 42: ";
    for (int n : filled) std::cout << n << " ";
    std::cout << std::endl;

    // 10. Copy
    std::cout << "\n--- std::copy ---" << std::endl;
    std::vector<int> source = {10, 20, 30};
    std::vector<int> destination(source.size());

    std::copy(source.begin(), source.end(), destination.begin());
    std::cout << "Copied: ";
    for (int n : destination) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall algorithm_basics.cpp -o algorithm_basics
./algorithm_basics
```

## 6. Function Objects (Functors)

### Example 5: Functors and Predicates - Complete Program

**File: `functors_demo.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// Custom functor class
class Multiplier {
private:
    int factor;
public:
    Multiplier(int f) : factor(f) {}

    int operator()(int x) const {
        return x * factor;
    }
};

// Predicate functor
class IsEven {
public:
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

class GreaterThan {
private:
    int threshold;
public:
    GreaterThan(int t) : threshold(t) {}

    bool operator()(int x) const {
        return x > threshold;
    }
};

int main() {
    std::cout << "=== Functors Demo ===" << std::endl;

    // 1. Using custom functor
    std::cout << "\n--- Custom Functor: Multiplier ---" << std::endl;
    Multiplier times3(3);
    std::cout << "5 × 3 = " << times3(5) << std::endl;
    std::cout << "10 × 3 = " << times3(10) << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> result(numbers.size());

    std::transform(numbers.begin(), numbers.end(), result.begin(), times3);
    std::cout << "Original: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;
    std::cout << "Multiplied by 3: ";
    for (int n : result) std::cout << n << " ";
    std::cout << std::endl;

    // 2. Using predicate functors
    std::cout << "\n--- Predicate Functor: IsEven ---" << std::endl;
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int evenCount = std::count_if(values.begin(), values.end(), IsEven());
    std::cout << "Number of even values: " << evenCount << std::endl;

    auto firstEven = std::find_if(values.begin(), values.end(), IsEven());
    if (firstEven != values.end()) {
        std::cout << "First even number: " << *firstEven << std::endl;
    }

    // 3. Using GreaterThan predicate
    std::cout << "\n--- Predicate: GreaterThan ---" << std::endl;
    GreaterThan gt5(5);

    int countGT5 = std::count_if(values.begin(), values.end(), gt5);
    std::cout << "Numbers greater than 5: " << countGT5 << std::endl;

    // 4. Built-in functors
    std::cout << "\n--- Built-in Functors ---" << std::endl;
    std::vector<int> nums = {5, 2, 8, 1, 9};

    std::cout << "Before sort: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // Using std::greater<int>() for descending sort
    std::sort(nums.begin(), nums.end(), std::greater<int>());
    std::cout << "Sorted (descending): ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // Using std::less<int>() for ascending sort
    std::sort(nums.begin(), nums.end(), std::less<int>());
    std::cout << "Sorted (ascending): ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // 5. Lambda expressions (modern alternative to functors)
    std::cout << "\n--- Lambda Expressions ---" << std::endl;
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Count numbers divisible by 3
    int divisibleBy3 = std::count_if(data.begin(), data.end(),
                                     [](int x) { return x % 3 == 0; });
    std::cout << "Numbers divisible by 3: " << divisibleBy3 << std::endl;

    // Transform with lambda
    std::vector<int> squared(data.size());
    std::transform(data.begin(), data.end(), squared.begin(),
                   [](int x) { return x * x; });
    std::cout << "Squared: ";
    for (int n : squared) std::cout << n << " ";
    std::cout << std::endl;

    // 6. Combining functors
    std::cout << "\n--- Combining Functors ---" << std::endl;
    std::vector<int> combined = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Find first even number greater than 5
    auto it = std::find_if(combined.begin(), combined.end(),
                          [](int x) { return x > 5 && x % 2 == 0; });
    if (it != combined.end()) {
        std::cout << "First even number > 5: " << *it << std::endl;
    }

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall functors_demo.cpp -o functors_demo
./functors_demo
```

## Complete Project: Student Grade Management System

**File: `student_grades_stl.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iomanip>

struct Student {
    std::string name;
    int id;
    std::vector<double> grades;

    Student(const std::string& n, int i) : name(n), id(i) {}

    void addGrade(double grade) {
        grades.push_back(grade);
    }

    double getAverage() const {
        if (grades.empty()) return 0.0;
        double sum = std::accumulate(grades.begin(), grades.end(), 0.0);
        return sum / grades.size();
    }

    double getHighest() const {
        if (grades.empty()) return 0.0;
        return *std::max_element(grades.begin(), grades.end());
    }

    double getLowest() const {
        if (grades.empty()) return 0.0;
        return *std::min_element(grades.begin(), grades.end());
    }

    char getLetterGrade() const {
        double avg = getAverage();
        if (avg >= 90) return 'A';
        if (avg >= 80) return 'B';
        if (avg >= 70) return 'C';
        if (avg >= 60) return 'D';
        return 'F';
    }

    void display() const {
        std::cout << "ID: " << id << " | Name: " << name << std::endl;
        std::cout << "Grades: ";
        for (double grade : grades) {
            std::cout << std::fixed << std::setprecision(1) << grade << " ";
        }
        std::cout << std::endl;
        std::cout << "Average: " << std::fixed << std::setprecision(2) << getAverage();
        std::cout << " | Letter: " << getLetterGrade();
        std::cout << " | Highest: " << getHighest();
        std::cout << " | Lowest: " << getLowest() << std::endl;
    }
};

class GradeBook {
private:
    std::vector<Student> students;

public:
    void addStudent(const Student& student) {
        students.push_back(student);
    }

    Student* findStudent(int id) {
        auto it = std::find_if(students.begin(), students.end(),
                              [id](const Student& s) { return s.id == id; });
        return (it != students.end()) ? &(*it) : nullptr;
    }

    void displayAllStudents() const {
        std::cout << "\n=== All Students ===" << std::endl;
        for (const auto& student : students) {
            student.display();
            std::cout << "---" << std::endl;
        }
    }

    void displayTopStudents(int n) const {
        std::cout << "\n=== Top " << n << " Students ===" << std::endl;

        // Create a copy and sort by average grade
        std::vector<Student> sorted = students;
        std::sort(sorted.begin(), sorted.end(),
                 [](const Student& a, const Student& b) {
                     return a.getAverage() > b.getAverage();
                 });

        int count = std::min(n, static_cast<int>(sorted.size()));
        for (int i = 0; i < count; ++i) {
            sorted[i].display();
            std::cout << "---" << std::endl;
        }
    }

    void displayStatistics() const {
        std::cout << "\n=== Class Statistics ===" << std::endl;
        std::cout << "Total students: " << students.size() << std::endl;

        if (students.empty()) return;

        // Calculate class average
        std::vector<double> averages;
        for (const auto& student : students) {
            averages.push_back(student.getAverage());
        }

        double classAvg = std::accumulate(averages.begin(), averages.end(), 0.0)
                         / averages.size();
        std::cout << "Class average: " << std::fixed << std::setprecision(2)
                  << classAvg << std::endl;

        double highestAvg = *std::max_element(averages.begin(), averages.end());
        double lowestAvg = *std::min_element(averages.begin(), averages.end());
        std::cout << "Highest average: " << highestAvg << std::endl;
        std::cout << "Lowest average: " << lowestAvg << std::endl;

        // Count students by letter grade
        int countA = std::count_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.getLetterGrade() == 'A'; });
        int countB = std::count_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.getLetterGrade() == 'B'; });
        int countC = std::count_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.getLetterGrade() == 'C'; });
        int countD = std::count_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.getLetterGrade() == 'D'; });
        int countF = std::count_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.getLetterGrade() == 'F'; });

        std::cout << "\nGrade Distribution:" << std::endl;
        std::cout << "A: " << countA << std::endl;
        std::cout << "B: " << countB << std::endl;
        std::cout << "C: " << countC << std::endl;
        std::cout << "D: " << countD << std::endl;
        std::cout << "F: " << countF << std::endl;
    }

    void searchByName(const std::string& query) const {
        std::cout << "\n=== Search Results for '" << query << "' ===" << std::endl;

        bool found = false;
        for (const auto& student : students) {
            if (student.name.find(query) != std::string::npos) {
                student.display();
                std::cout << "---" << std::endl;
                found = true;
            }
        }

        if (!found) {
            std::cout << "No students found matching '" << query << "'" << std::endl;
        }
    }
};

int main() {
    std::cout << "=== Student Grade Management System ===" << std::endl;
    std::cout << "STL Demonstration Project" << std::endl;

    GradeBook gradebook;

    // Add students with grades
    Student alice("Alice Johnson", 101);
    alice.addGrade(95.0);
    alice.addGrade(92.0);
    alice.addGrade(88.0);
    alice.addGrade(94.0);
    gradebook.addStudent(alice);

    Student bob("Bob Smith", 102);
    bob.addGrade(78.0);
    bob.addGrade(82.0);
    bob.addGrade(75.0);
    bob.addGrade(80.0);
    gradebook.addStudent(bob);

    Student charlie("Charlie Brown", 103);
    charlie.addGrade(88.0);
    charlie.addGrade(85.0);
    charlie.addGrade(90.0);
    charlie.addGrade(87.0);
    gradebook.addStudent(charlie);

    Student diana("Diana Prince", 104);
    diana.addGrade(92.0);
    diana.addGrade(95.0);
    diana.addGrade(93.0);
    diana.addGrade(96.0);
    gradebook.addStudent(diana);

    Student eve("Eve Adams", 105);
    eve.addGrade(65.0);
    eve.addGrade(70.0);
    eve.addGrade(68.0);
    eve.addGrade(72.0);
    gradebook.addStudent(eve);

    // Display all students
    gradebook.displayAllStudents();

    // Display statistics
    gradebook.displayStatistics();

    // Display top 3 students
    gradebook.displayTopStudents(3);

    // Search for students
    gradebook.searchByName("Bob");
    gradebook.searchByName("Brown");

    // Find specific student
    std::cout << "\n=== Finding Student by ID ===" << std::endl;
    Student* found = gradebook.findStudent(103);
    if (found) {
        std::cout << "Found student:" << std::endl;
        found->display();
    } else {
        std::cout << "Student not found" << std::endl;
    }

    std::cout << "\n=== Program Complete ===" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall student_grades_stl.cpp -o student_grades_stl
./student_grades_stl
```

## Best Practices

1. **Use the right container for the job**
   - `vector` for dynamic arrays
   - `list` for frequent insertions/deletions
   - `set`/`map` for sorted unique elements

2. **Prefer algorithms over raw loops**
   - More expressive and less error-prone
   - Often more optimized

3. **Use iterators consistently**
   - `begin()` and `end()` for ranges
   - `rbegin()` and `rend()` for reverse iteration

4. **Use const iterators when not modifying**
   - `cbegin()` and `cend()`
   - Prevents accidental modifications

5. **Reserve capacity for vectors when size is known**
   - Reduces reallocations
   - Improves performance

6. **Use range-based for loops**
   - Simpler syntax
   - Less error-prone

7. **Prefer lambda expressions over functors for simple operations**
   - More concise
   - Easier to read inline

## Summary

In this lesson, you learned:

- **STL components** - Containers, iterators, algorithms, function objects
- **Iterators** - Forward, reverse, const, and their operations
- **Container basics** - Creating, modifying, accessing elements
- **Common algorithms** - sort, find, count, accumulate, transform
- **Functors** - Function objects and predicates
- **Best practices** - Using STL effectively and efficiently

The STL is fundamental to modern C++ programming and provides powerful tools for working with data structures and algorithms.

## Next Lesson

Lesson 18: STL Containers (Detailed)

## Additional Resources

- C++ Reference: STL
- Effective STL by Scott Meyers
- C++ Standard Library Tutorial
- Practice: Convert existing array-based code to use STL containers

---

**Exercise**: Create a task management system using STL containers and algorithms with sorting, filtering, and searching capabilities.
