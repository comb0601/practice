# Lesson 19: STL Algorithms

## Overview
The Standard Template Library (STL) algorithms provide a rich set of generic operations that work with containers through iterators. These algorithms are powerful, efficient, and type-safe, allowing you to write expressive code without manual loops.

## Topics Covered
- Non-modifying sequence algorithms
- Modifying sequence algorithms
- Sorting and related operations
- Numeric algorithms
- Set operations
- Heap operations
- Min/Max operations
- Algorithm complexity and performance

---

## 1. Non-Modifying Sequence Algorithms

### Example 1: Finding and Counting Elements

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    std::vector<int> numbers = {10, 20, 30, 20, 40, 50, 20, 60};

    // Find first occurrence
    auto it = std::find(numbers.begin(), numbers.end(), 20);
    if (it != numbers.end()) {
        std::cout << "Found 20 at position: "
                  << std::distance(numbers.begin(), it) << std::endl;
    }

    // Find with predicate
    auto it2 = std::find_if(numbers.begin(), numbers.end(),
                            [](int n) { return n > 35; });
    if (it2 != numbers.end()) {
        std::cout << "First number > 35: " << *it2 << std::endl;
    }

    // Count occurrences
    int count = std::count(numbers.begin(), numbers.end(), 20);
    std::cout << "Count of 20: " << count << std::endl;

    // Count with predicate
    int count_even = std::count_if(numbers.begin(), numbers.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "Count of even numbers: " << count_even << std::endl;

    // Check if all/any/none satisfy condition
    bool all_positive = std::all_of(numbers.begin(), numbers.end(),
                                    [](int n) { return n > 0; });
    std::cout << "All positive: " << std::boolalpha << all_positive << std::endl;

    bool any_large = std::any_of(numbers.begin(), numbers.end(),
                                 [](int n) { return n > 50; });
    std::cout << "Any > 50: " << any_large << std::endl;

    bool none_negative = std::none_of(numbers.begin(), numbers.end(),
                                     [](int n) { return n < 0; });
    std::cout << "None negative: " << none_negative << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example1.cpp -o example1`

---

## 2. Modifying Sequence Algorithms

### Example 2: Copy, Transform, and Replace

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
    std::vector<int> source = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> dest;

    // Copy
    std::copy(source.begin(), source.end(), std::back_inserter(dest));
    std::cout << "After copy: ";
    for (int n : dest) std::cout << n << " ";
    std::cout << std::endl;

    // Copy if (even numbers only)
    std::vector<int> evens;
    std::copy_if(source.begin(), source.end(), std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // Transform (square each number)
    std::vector<int> squares;
    std::transform(source.begin(), source.end(), std::back_inserter(squares),
                   [](int n) { return n * n; });
    std::cout << "Squares: ";
    for (int n : squares) std::cout << n << " ";
    std::cout << std::endl;

    // Transform with two ranges (multiply corresponding elements)
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {10, 20, 30, 40, 50};
    std::vector<int> product;
    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(product),
                   [](int a, int b) { return a * b; });
    std::cout << "Products: ";
    for (int n : product) std::cout << n << " ";
    std::cout << std::endl;

    // Replace
    std::vector<int> data = {1, 2, 3, 2, 4, 2, 5};
    std::replace(data.begin(), data.end(), 2, 99);
    std::cout << "After replace 2 with 99: ";
    for (int n : data) std::cout << n << " ";
    std::cout << std::endl;

    // Replace if
    std::replace_if(data.begin(), data.end(),
                    [](int n) { return n > 50; }, 0);
    std::cout << "After replace >50 with 0: ";
    for (int n : data) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example2.cpp -o example2`

---

## 3. Sorting and Related Operations

### Example 3: Sorting Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Person {
    std::string name;
    int age;
    double salary;
};

void printPersons(const std::vector<Person>& people) {
    for (const auto& p : people) {
        std::cout << p.name << " (age " << p.age
                  << ", salary $" << p.salary << ")" << std::endl;
    }
}

int main() {
    // Simple sorting
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::sort(numbers.begin(), numbers.end());
    std::cout << "Sorted ascending: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    std::cout << "Sorted descending: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // Partial sort (sort first 3 elements)
    std::vector<int> data = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::partial_sort(data.begin(), data.begin() + 3, data.end());
    std::cout << "Partial sort (first 3): ";
    for (int n : data) std::cout << n << " ";
    std::cout << std::endl;

    // Nth element (find the 5th smallest element)
    std::vector<int> nums = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::nth_element(nums.begin(), nums.begin() + 4, nums.end());
    std::cout << "5th smallest element: " << nums[4] << std::endl;

    // Sorting complex objects
    std::vector<Person> people = {
        {"Alice", 30, 75000.0},
        {"Bob", 25, 60000.0},
        {"Charlie", 35, 90000.0},
        {"Diana", 28, 70000.0}
    };

    std::cout << "\nOriginal order:" << std::endl;
    printPersons(people);

    // Sort by age
    std::sort(people.begin(), people.end(),
              [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "\nSorted by age:" << std::endl;
    printPersons(people);

    // Sort by salary (descending)
    std::sort(people.begin(), people.end(),
              [](const Person& a, const Person& b) { return a.salary > b.salary; });
    std::cout << "\nSorted by salary (desc):" << std::endl;
    printPersons(people);

    // Stable sort (preserves relative order of equal elements)
    std::vector<Person> people2 = {
        {"Alice", 30, 75000.0},
        {"Bob", 30, 60000.0},
        {"Charlie", 25, 90000.0},
        {"Diana", 30, 70000.0}
    };

    std::stable_sort(people2.begin(), people2.end(),
                     [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "\nStable sort by age (preserves original order for same age):" << std::endl;
    printPersons(people2);

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example3.cpp -o example3`

---

## 4. Binary Search Operations (on sorted ranges)

### Example 4: Binary Search and Bounds

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> sorted = {10, 20, 30, 30, 30, 40, 50, 60};

    // Binary search (returns bool)
    bool found = std::binary_search(sorted.begin(), sorted.end(), 30);
    std::cout << "30 found: " << std::boolalpha << found << std::endl;

    // Lower bound (first position where value could be inserted)
    auto lb = std::lower_bound(sorted.begin(), sorted.end(), 30);
    std::cout << "Lower bound of 30 at index: "
              << std::distance(sorted.begin(), lb) << std::endl;

    // Upper bound (last position where value could be inserted)
    auto ub = std::upper_bound(sorted.begin(), sorted.end(), 30);
    std::cout << "Upper bound of 30 at index: "
              << std::distance(sorted.begin(), ub) << std::endl;

    // Equal range (returns pair of lower and upper bound)
    auto range = std::equal_range(sorted.begin(), sorted.end(), 30);
    std::cout << "Equal range of 30: ["
              << std::distance(sorted.begin(), range.first) << ", "
              << std::distance(sorted.begin(), range.second) << ")" << std::endl;
    std::cout << "Count of 30 using equal_range: "
              << std::distance(range.first, range.second) << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example4.cpp -o example4`

---

## 5. Numeric Algorithms

### Example 5: Accumulate, Inner Product, and More

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Sum
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum: " << sum << std::endl;

    // Product
    int product = std::accumulate(numbers.begin(), numbers.end(), 1,
                                  [](int a, int b) { return a * b; });
    std::cout << "Product: " << product << std::endl;

    // Inner product (dot product)
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {10, 20, 30, 40, 50};
    int dot = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
    std::cout << "Dot product: " << dot << std::endl;

    // Partial sum (running total)
    std::vector<int> partial(numbers.size());
    std::partial_sum(numbers.begin(), numbers.end(), partial.begin());
    std::cout << "Partial sums: ";
    for (int n : partial) std::cout << n << " ";
    std::cout << std::endl;

    // Adjacent difference
    std::vector<int> diffs(numbers.size());
    std::adjacent_difference(numbers.begin(), numbers.end(), diffs.begin());
    std::cout << "Adjacent differences: ";
    for (int n : diffs) std::cout << n << " ";
    std::cout << std::endl;

    // Iota (fill with sequential values)
    std::vector<int> sequence(10);
    std::iota(sequence.begin(), sequence.end(), 100);
    std::cout << "Sequence from 100: ";
    for (int n : sequence) std::cout << n << " ";
    std::cout << std::endl;

    // GCD and LCM (C++17)
    int a = 48, b = 18;
    std::cout << "GCD(" << a << ", " << b << ") = "
              << std::gcd(a, b) << std::endl;
    std::cout << "LCM(" << a << ", " << b << ") = "
              << std::lcm(a, b) << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example5.cpp -o example5`

---

## 6. Set Operations

### Example 6: Set Union, Intersection, and Difference

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

void printVector(const std::string& label, const std::vector<int>& v) {
    std::cout << label << ": ";
    for (int n : v) std::cout << n << " ";
    std::cout << std::endl;
}

int main() {
    std::vector<int> set1 = {1, 2, 3, 4, 5, 6};
    std::vector<int> set2 = {4, 5, 6, 7, 8, 9};

    printVector("Set 1", set1);
    printVector("Set 2", set2);

    // Union
    std::vector<int> union_result;
    std::set_union(set1.begin(), set1.end(),
                   set2.begin(), set2.end(),
                   std::back_inserter(union_result));
    printVector("Union", union_result);

    // Intersection
    std::vector<int> intersection_result;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::back_inserter(intersection_result));
    printVector("Intersection", intersection_result);

    // Difference (elements in set1 but not in set2)
    std::vector<int> difference_result;
    std::set_difference(set1.begin(), set1.end(),
                        set2.begin(), set2.end(),
                        std::back_inserter(difference_result));
    printVector("Difference (1 - 2)", difference_result);

    // Symmetric difference (elements in either set but not in both)
    std::vector<int> sym_diff_result;
    std::set_symmetric_difference(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  std::back_inserter(sym_diff_result));
    printVector("Symmetric Difference", sym_diff_result);

    // Includes (check if set2 is subset of set1)
    bool is_subset = std::includes(set1.begin(), set1.end(),
                                   intersection_result.begin(),
                                   intersection_result.end());
    std::cout << "Intersection is subset of Set 1: "
              << std::boolalpha << is_subset << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example6.cpp -o example6`

---

## 7. Heap Operations

### Example 7: Heap Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void printVector(const std::string& label, const std::vector<int>& v) {
    std::cout << label << ": ";
    for (int n : v) std::cout << n << " ";
    std::cout << std::endl;
}

int main() {
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};

    printVector("Original", data);

    // Make heap (max heap by default)
    std::make_heap(data.begin(), data.end());
    printVector("After make_heap", data);
    std::cout << "Max element (front): " << data.front() << std::endl;

    // Pop heap (move max to end)
    std::pop_heap(data.begin(), data.end());
    int max = data.back();
    data.pop_back();
    std::cout << "Popped max: " << max << std::endl;
    printVector("After pop_heap", data);

    // Push heap (add new element)
    data.push_back(10);
    std::push_heap(data.begin(), data.end());
    printVector("After push_heap(10)", data);

    // Sort heap (sorts in ascending order, destroys heap property)
    std::sort_heap(data.begin(), data.end());
    printVector("After sort_heap", data);

    // Is heap?
    bool is_heap_now = std::is_heap(data.begin(), data.end());
    std::cout << "Is heap after sort: " << std::boolalpha << is_heap_now << std::endl;

    // Min heap example
    std::vector<int> minheap = {3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(minheap.begin(), minheap.end(), std::greater<int>());
    printVector("Min heap", minheap);
    std::cout << "Min element (front): " << minheap.front() << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example7.cpp -o example7`

---

## 8. Min/Max Operations

### Example 8: Min, Max, and Minmax

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // Min and max of two values
    int a = 10, b = 20;
    std::cout << "Min of " << a << " and " << b << ": "
              << std::min(a, b) << std::endl;
    std::cout << "Max of " << a << " and " << b << ": "
              << std::max(a, b) << std::endl;

    // Minmax of two values (returns pair)
    auto [min_val, max_val] = std::minmax(a, b);
    std::cout << "Minmax: {" << min_val << ", " << max_val << "}" << std::endl;

    // Min element in range
    auto min_it = std::min_element(numbers.begin(), numbers.end());
    std::cout << "Min element: " << *min_it << " at index "
              << std::distance(numbers.begin(), min_it) << std::endl;

    // Max element in range
    auto max_it = std::max_element(numbers.begin(), numbers.end());
    std::cout << "Max element: " << *max_it << " at index "
              << std::distance(numbers.begin(), max_it) << std::endl;

    // Minmax element (returns pair of iterators)
    auto [min_it2, max_it2] = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "Minmax elements: {" << *min_it2 << ", " << *max_it2 << "}" << std::endl;

    // Clamp (C++17) - constrain value to range
    int value = 15;
    int clamped = std::clamp(value, 5, 10);
    std::cout << "Clamp " << value << " to [5, 10]: " << clamped << std::endl;

    value = 3;
    clamped = std::clamp(value, 5, 10);
    std::cout << "Clamp " << value << " to [5, 10]: " << clamped << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example8.cpp -o example8`

---

## 9. Permutations and Partitioning

### Example 9: Next/Prev Permutation and Partition

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

void printVector(const std::vector<int>& v) {
    for (int n : v) std::cout << n << " ";
    std::cout << std::endl;
}

int main() {
    // Next permutation
    std::vector<int> perm = {1, 2, 3};
    std::cout << "All permutations of {1, 2, 3}:" << std::endl;
    do {
        printVector(perm);
    } while (std::next_permutation(perm.begin(), perm.end()));

    // Previous permutation
    std::vector<int> perm2 = {3, 2, 1};
    std::cout << "\nReverse permutations of {3, 2, 1}:" << std::endl;
    do {
        printVector(perm2);
    } while (std::prev_permutation(perm2.begin(), perm2.end()));

    // Partition (rearrange so elements satisfying predicate come first)
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto partition_point = std::partition(data.begin(), data.end(),
                                         [](int n) { return n % 2 == 0; });
    std::cout << "\nAfter partition (evens first): ";
    printVector(data);
    std::cout << "Partition point at index: "
              << std::distance(data.begin(), partition_point) << std::endl;

    // Stable partition (preserves relative order)
    std::vector<int> data2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::stable_partition(data2.begin(), data2.end(),
                          [](int n) { return n % 2 == 0; });
    std::cout << "After stable_partition (evens first): ";
    printVector(data2);

    // Is partitioned?
    bool is_part = std::is_partitioned(data2.begin(), data2.end(),
                                       [](int n) { return n % 2 == 0; });
    std::cout << "Is partitioned by even: " << std::boolalpha << is_part << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example9.cpp -o example9`

---

## 10. Remove and Unique Operations

### Example 10: Remove, Erase, and Unique

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

void printVector(const std::string& label, const std::vector<int>& v) {
    std::cout << label << ": ";
    for (int n : v) std::cout << n << " ";
    std::cout << " (size: " << v.size() << ")" << std::endl;
}

int main() {
    // Remove (doesn't actually remove, moves to end)
    std::vector<int> data1 = {1, 2, 3, 2, 4, 2, 5};
    printVector("Original", data1);

    auto new_end = std::remove(data1.begin(), data1.end(), 2);
    printVector("After remove(2) - before erase", data1);

    // Erase-remove idiom (actually remove elements)
    data1.erase(new_end, data1.end());
    printVector("After erase", data1);

    // Remove_if
    std::vector<int> data2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    data2.erase(std::remove_if(data2.begin(), data2.end(),
                               [](int n) { return n % 2 == 0; }),
                data2.end());
    printVector("After remove_if (evens)", data2);

    // Unique (remove consecutive duplicates)
    std::vector<int> data3 = {1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
    printVector("Before unique", data3);

    auto new_end2 = std::unique(data3.begin(), data3.end());
    data3.erase(new_end2, data3.end());
    printVector("After unique", data3);

    // Unique on unsorted data (only removes consecutive duplicates)
    std::vector<int> data4 = {1, 2, 1, 3, 2, 4, 3, 5};
    printVector("Unsorted before unique", data4);

    auto new_end3 = std::unique(data4.begin(), data4.end());
    data4.erase(new_end3, data4.end());
    printVector("Unsorted after unique", data4);

    // To remove all duplicates, sort first
    std::vector<int> data5 = {1, 2, 1, 3, 2, 4, 3, 5};
    std::sort(data5.begin(), data5.end());
    printVector("After sort", data5);

    data5.erase(std::unique(data5.begin(), data5.end()), data5.end());
    printVector("After unique", data5);

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example10.cpp -o example10`

---

## Project 1: Data Analysis Tool

A complete program demonstrating various STL algorithms for statistical analysis.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <iomanip>

class Statistics {
private:
    std::vector<double> data;

public:
    Statistics(const std::vector<double>& values) : data(values) {
        std::sort(data.begin(), data.end());
    }

    double mean() const {
        if (data.empty()) return 0.0;
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    double median() const {
        if (data.empty()) return 0.0;

        size_t n = data.size();
        if (n % 2 == 0) {
            return (data[n/2 - 1] + data[n/2]) / 2.0;
        } else {
            return data[n/2];
        }
    }

    double mode() const {
        if (data.empty()) return 0.0;

        double current_mode = data[0];
        int max_count = 0;

        for (size_t i = 0; i < data.size(); ) {
            double value = data[i];
            auto range = std::equal_range(data.begin(), data.end(), value);
            int count = std::distance(range.first, range.second);

            if (count > max_count) {
                max_count = count;
                current_mode = value;
            }

            i += count;
        }

        return current_mode;
    }

    double variance() const {
        if (data.size() < 2) return 0.0;

        double m = mean();
        double sum_sq_diff = std::accumulate(data.begin(), data.end(), 0.0,
            [m](double acc, double val) {
                return acc + (val - m) * (val - m);
            });

        return sum_sq_diff / (data.size() - 1);
    }

    double stddev() const {
        return std::sqrt(variance());
    }

    double min() const {
        if (data.empty()) return 0.0;
        return *std::min_element(data.begin(), data.end());
    }

    double max() const {
        if (data.empty()) return 0.0;
        return *std::max_element(data.begin(), data.end());
    }

    double range() const {
        return max() - min();
    }

    double percentile(double p) const {
        if (data.empty()) return 0.0;
        if (p < 0.0 || p > 100.0) return 0.0;

        double index = (p / 100.0) * (data.size() - 1);
        size_t lower = static_cast<size_t>(std::floor(index));
        size_t upper = static_cast<size_t>(std::ceil(index));

        if (lower == upper) {
            return data[lower];
        }

        double fraction = index - lower;
        return data[lower] * (1 - fraction) + data[upper] * fraction;
    }

    std::vector<double> quartiles() const {
        return {percentile(25), percentile(50), percentile(75)};
    }

    void printSummary() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n===== Statistical Summary =====" << std::endl;
        std::cout << "Count:      " << data.size() << std::endl;
        std::cout << "Mean:       " << mean() << std::endl;
        std::cout << "Median:     " << median() << std::endl;
        std::cout << "Mode:       " << mode() << std::endl;
        std::cout << "Std Dev:    " << stddev() << std::endl;
        std::cout << "Variance:   " << variance() << std::endl;
        std::cout << "Min:        " << min() << std::endl;
        std::cout << "Max:        " << max() << std::endl;
        std::cout << "Range:      " << range() << std::endl;

        auto q = quartiles();
        std::cout << "Q1 (25%):   " << q[0] << std::endl;
        std::cout << "Q2 (50%):   " << q[1] << std::endl;
        std::cout << "Q3 (75%):   " << q[2] << std::endl;
        std::cout << "===============================" << std::endl;
    }
};

int main() {
    // Sample dataset: test scores
    std::vector<double> scores = {
        85, 92, 78, 90, 88, 76, 95, 89, 84, 91,
        87, 93, 82, 88, 90, 85, 79, 88, 92, 86,
        88, 91, 83, 87, 89, 94, 81, 88, 90, 85
    };

    std::cout << "Test Scores Analysis" << std::endl;
    std::cout << "Total students: " << scores.size() << std::endl;

    Statistics stats(scores);
    stats.printSummary();

    // Find outliers (values outside 2 standard deviations)
    double mean = stats.mean();
    double sd = stats.stddev();
    double lower_bound = mean - 2 * sd;
    double upper_bound = mean + 2 * sd;

    std::vector<double> sorted_scores = scores;
    std::sort(sorted_scores.begin(), sorted_scores.end());

    std::vector<double> outliers;
    std::copy_if(sorted_scores.begin(), sorted_scores.end(),
                 std::back_inserter(outliers),
                 [lower_bound, upper_bound](double score) {
                     return score < lower_bound || score > upper_bound;
                 });

    std::cout << "\nOutliers (outside 2 std devs):" << std::endl;
    if (outliers.empty()) {
        std::cout << "None" << std::endl;
    } else {
        for (double score : outliers) {
            std::cout << score << " ";
        }
        std::cout << std::endl;
    }

    // Grade distribution
    auto count_A = std::count_if(scores.begin(), scores.end(),
                                 [](double s) { return s >= 90; });
    auto count_B = std::count_if(scores.begin(), scores.end(),
                                 [](double s) { return s >= 80 && s < 90; });
    auto count_C = std::count_if(scores.begin(), scores.end(),
                                 [](double s) { return s >= 70 && s < 80; });
    auto count_F = std::count_if(scores.begin(), scores.end(),
                                 [](double s) { return s < 70; });

    std::cout << "\nGrade Distribution:" << std::endl;
    std::cout << "A (90-100): " << count_A << std::endl;
    std::cout << "B (80-89):  " << count_B << std::endl;
    std::cout << "C (70-79):  " << count_C << std::endl;
    std::cout << "F (<70):    " << count_F << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project1.cpp -o project1`

---

## Project 2: Text Processing and Analysis

A comprehensive text analysis tool using STL algorithms.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cctype>
#include <sstream>

class TextAnalyzer {
private:
    std::string text;
    std::vector<std::string> words;

    std::string toLowerCase(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    bool isAlpha(char c) const {
        return std::isalpha(static_cast<unsigned char>(c));
    }

    void extractWords() {
        words.clear();
        std::string word;

        for (char c : text) {
            if (isAlpha(c)) {
                word += c;
            } else if (!word.empty()) {
                words.push_back(toLowerCase(word));
                word.clear();
            }
        }

        if (!word.empty()) {
            words.push_back(toLowerCase(word));
        }
    }

public:
    TextAnalyzer(const std::string& input) : text(input) {
        extractWords();
    }

    size_t characterCount() const {
        return text.size();
    }

    size_t wordCount() const {
        return words.size();
    }

    size_t uniqueWordCount() const {
        std::vector<std::string> sorted = words;
        std::sort(sorted.begin(), sorted.end());
        auto last = std::unique(sorted.begin(), sorted.end());
        return std::distance(sorted.begin(), last);
    }

    size_t sentenceCount() const {
        return std::count_if(text.begin(), text.end(),
                            [](char c) { return c == '.' || c == '!' || c == '?'; });
    }

    double averageWordLength() const {
        if (words.empty()) return 0.0;

        size_t total_length = std::accumulate(words.begin(), words.end(), 0,
            [](size_t sum, const std::string& word) {
                return sum + word.length();
            });

        return static_cast<double>(total_length) / words.size();
    }

    std::map<std::string, int> wordFrequency() const {
        std::map<std::string, int> freq;

        for (const auto& word : words) {
            freq[word]++;
        }

        return freq;
    }

    std::vector<std::pair<std::string, int>> topWords(int n) const {
        auto freq = wordFrequency();

        std::vector<std::pair<std::string, int>> pairs(freq.begin(), freq.end());

        // Sort by frequency (descending)
        std::sort(pairs.begin(), pairs.end(),
                 [](const auto& a, const auto& b) {
                     if (a.second != b.second) {
                         return a.second > b.second;
                     }
                     return a.first < b.first;  // Alphabetical for ties
                 });

        if (pairs.size() > static_cast<size_t>(n)) {
            pairs.resize(n);
        }

        return pairs;
    }

    std::string longestWord() const {
        if (words.empty()) return "";

        auto it = std::max_element(words.begin(), words.end(),
            [](const std::string& a, const std::string& b) {
                return a.length() < b.length();
            });

        return *it;
    }

    std::string shortestWord() const {
        if (words.empty()) return "";

        auto it = std::min_element(words.begin(), words.end(),
            [](const std::string& a, const std::string& b) {
                return a.length() < b.length();
            });

        return *it;
    }

    int countWord(const std::string& word) const {
        std::string lower = toLowerCase(word);
        return std::count(words.begin(), words.end(), lower);
    }

    std::vector<std::string> findWordsStartingWith(char letter) const {
        std::vector<std::string> result;
        char lower_letter = std::tolower(static_cast<unsigned char>(letter));

        std::copy_if(words.begin(), words.end(), std::back_inserter(result),
                    [lower_letter](const std::string& word) {
                        return !word.empty() && word[0] == lower_letter;
                    });

        // Remove duplicates
        std::sort(result.begin(), result.end());
        auto last = std::unique(result.begin(), result.end());
        result.erase(last, result.end());

        return result;
    }

    void printAnalysis() const {
        std::cout << "\n===== Text Analysis Report =====" << std::endl;
        std::cout << "Total characters:    " << characterCount() << std::endl;
        std::cout << "Total words:         " << wordCount() << std::endl;
        std::cout << "Unique words:        " << uniqueWordCount() << std::endl;
        std::cout << "Sentences:           " << sentenceCount() << std::endl;
        std::cout << "Average word length: " << std::fixed << std::setprecision(2)
                  << averageWordLength() << std::endl;
        std::cout << "Longest word:        " << longestWord() << std::endl;
        std::cout << "Shortest word:       " << shortestWord() << std::endl;

        std::cout << "\nTop 10 Most Frequent Words:" << std::endl;
        auto top = topWords(10);
        for (size_t i = 0; i < top.size(); ++i) {
            std::cout << (i + 1) << ". \"" << top[i].first << "\" - "
                      << top[i].second << " times" << std::endl;
        }

        std::cout << "================================" << std::endl;
    }
};

int main() {
    std::string sample_text =
        "The quick brown fox jumps over the lazy dog. "
        "The dog was not amused by the fox's antics. "
        "However, the fox continued to jump and play. "
        "The lazy dog decided to take a nap instead. "
        "Meanwhile, the quick fox found another dog to play with. "
        "This new dog was much more energetic than the lazy dog. "
        "They played together all day long in the sunny meadow.";

    TextAnalyzer analyzer(sample_text);
    analyzer.printAnalysis();

    // Additional queries
    std::cout << "\n===== Additional Queries =====" << std::endl;

    std::string search_word = "dog";
    std::cout << "Occurrences of \"" << search_word << "\": "
              << analyzer.countWord(search_word) << std::endl;

    char start_letter = 't';
    auto words_starting = analyzer.findWordsStartingWith(start_letter);
    std::cout << "\nWords starting with '" << start_letter << "':" << std::endl;
    for (const auto& word : words_starting) {
        std::cout << "  " << word << std::endl;
    }

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project2.cpp -o project2`

---

## Project 3: Custom Sort and Search Tool

A flexible sorting and searching utility demonstrating algorithm customization.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>

struct Employee {
    int id;
    std::string name;
    std::string department;
    double salary;
    int years_of_service;
};

class EmployeeDatabase {
private:
    std::vector<Employee> employees;

public:
    EmployeeDatabase() {
        // Initialize with sample data
        employees = {
            {101, "Alice Johnson", "Engineering", 95000, 5},
            {102, "Bob Smith", "Marketing", 75000, 3},
            {103, "Carol Davis", "Engineering", 105000, 8},
            {104, "David Brown", "Sales", 70000, 2},
            {105, "Eve Wilson", "Engineering", 85000, 4},
            {106, "Frank Miller", "Marketing", 80000, 6},
            {107, "Grace Lee", "Sales", 72000, 3},
            {108, "Henry Clark", "Engineering", 115000, 10},
            {109, "Ivy Martinez", "HR", 65000, 4},
            {110, "Jack Anderson", "Engineering", 90000, 5}
        };
    }

    void sortById(bool ascending = true) {
        if (ascending) {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.id < b.id;
                     });
        } else {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.id > b.id;
                     });
        }
    }

    void sortByName(bool ascending = true) {
        if (ascending) {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.name < b.name;
                     });
        } else {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.name > b.name;
                     });
        }
    }

    void sortBySalary(bool ascending = true) {
        if (ascending) {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.salary < b.salary;
                     });
        } else {
            std::sort(employees.begin(), employees.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.salary > b.salary;
                     });
        }
    }

    void sortByDepartmentThenSalary() {
        std::sort(employees.begin(), employees.end(),
                 [](const Employee& a, const Employee& b) {
                     if (a.department != b.department) {
                         return a.department < b.department;
                     }
                     return a.salary > b.salary;  // Higher salary first within dept
                 });
    }

    Employee* findById(int id) {
        // Binary search (requires sorted by ID)
        sortById();

        auto it = std::lower_bound(employees.begin(), employees.end(), id,
                                   [](const Employee& emp, int id) {
                                       return emp.id < id;
                                   });

        if (it != employees.end() && it->id == id) {
            return &(*it);
        }

        return nullptr;
    }

    std::vector<Employee> findByDepartment(const std::string& dept) {
        std::vector<Employee> result;

        std::copy_if(employees.begin(), employees.end(),
                    std::back_inserter(result),
                    [&dept](const Employee& emp) {
                        return emp.department == dept;
                    });

        return result;
    }

    std::vector<Employee> findBySalaryRange(double min_salary, double max_salary) {
        std::vector<Employee> result;

        std::copy_if(employees.begin(), employees.end(),
                    std::back_inserter(result),
                    [min_salary, max_salary](const Employee& emp) {
                        return emp.salary >= min_salary && emp.salary <= max_salary;
                    });

        // Sort by salary
        std::sort(result.begin(), result.end(),
                 [](const Employee& a, const Employee& b) {
                     return a.salary < b.salary;
                 });

        return result;
    }

    std::vector<Employee> topNBySalary(int n) {
        std::vector<Employee> sorted = employees;

        // Partial sort to get top N
        if (n < static_cast<int>(sorted.size())) {
            std::partial_sort(sorted.begin(), sorted.begin() + n, sorted.end(),
                            [](const Employee& a, const Employee& b) {
                                return a.salary > b.salary;
                            });
            sorted.resize(n);
        } else {
            std::sort(sorted.begin(), sorted.end(),
                     [](const Employee& a, const Employee& b) {
                         return a.salary > b.salary;
                     });
        }

        return sorted;
    }

    void printEmployee(const Employee& emp) const {
        std::cout << std::setw(5) << emp.id << " | "
                  << std::setw(20) << std::left << emp.name << " | "
                  << std::setw(12) << emp.department << " | $"
                  << std::setw(8) << std::right << std::fixed << std::setprecision(2)
                  << emp.salary << " | "
                  << emp.years_of_service << " years" << std::endl;
    }

    void printAll() const {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << std::setw(5) << "ID" << " | "
                  << std::setw(20) << std::left << "Name" << " | "
                  << std::setw(12) << "Department" << " | "
                  << std::setw(10) << std::right << "Salary" << " | "
                  << "Service" << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (const auto& emp : employees) {
            printEmployee(emp);
        }

        std::cout << std::string(80, '=') << std::endl;
    }

    void printStatistics() const {
        if (employees.empty()) return;

        double total_salary = std::accumulate(employees.begin(), employees.end(), 0.0,
            [](double sum, const Employee& emp) {
                return sum + emp.salary;
            });

        double avg_salary = total_salary / employees.size();

        auto [min_it, max_it] = std::minmax_element(employees.begin(), employees.end(),
            [](const Employee& a, const Employee& b) {
                return a.salary < b.salary;
            });

        std::cout << "\n===== Statistics =====" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Total employees:  " << employees.size() << std::endl;
        std::cout << "Average salary:   $" << avg_salary << std::endl;
        std::cout << "Highest salary:   $" << max_it->salary
                  << " (" << max_it->name << ")" << std::endl;
        std::cout << "Lowest salary:    $" << min_it->salary
                  << " (" << min_it->name << ")" << std::endl;
        std::cout << "======================" << std::endl;
    }
};

int main() {
    EmployeeDatabase db;

    std::cout << "Employee Database Management System" << std::endl;

    // Show original data
    std::cout << "\nOriginal Data:" << std::endl;
    db.printAll();

    // Sort by salary
    std::cout << "\nSorted by Salary (Descending):" << std::endl;
    db.sortBySalary(false);
    db.printAll();

    // Sort by department then salary
    std::cout << "\nSorted by Department, then Salary (Descending):" << std::endl;
    db.sortByDepartmentThenSalary();
    db.printAll();

    // Find by ID
    std::cout << "\nSearch for ID 105:" << std::endl;
    Employee* emp = db.findById(105);
    if (emp) {
        db.printEmployee(*emp);
    } else {
        std::cout << "Not found" << std::endl;
    }

    // Find by department
    std::cout << "\nEngineering Department:" << std::endl;
    auto eng_emps = db.findByDepartment("Engineering");
    for (const auto& e : eng_emps) {
        db.printEmployee(e);
    }

    // Salary range query
    std::cout << "\nEmployees with salary between $70,000 and $90,000:" << std::endl;
    auto range_emps = db.findBySalaryRange(70000, 90000);
    for (const auto& e : range_emps) {
        db.printEmployee(e);
    }

    // Top N by salary
    std::cout << "\nTop 5 Highest Paid Employees:" << std::endl;
    auto top5 = db.topNBySalary(5);
    for (const auto& e : top5) {
        db.printEmployee(e);
    }

    // Statistics
    db.printStatistics();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project3.cpp -o project3`

---

## Algorithm Complexity Reference

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `find`, `count` | O(n) | O(1) |
| `sort` | O(n log n) | O(log n) |
| `stable_sort` | O(n log²n) | O(n) |
| `partial_sort` | O(n log k) | O(1) |
| `nth_element` | O(n) average | O(1) |
| `binary_search` | O(log n) | O(1) |
| `lower_bound`, `upper_bound` | O(log n) | O(1) |
| `merge` | O(n + m) | O(1) |
| `partition` | O(n) | O(1) |
| `unique` | O(n) | O(1) |
| `reverse` | O(n) | O(1) |
| `rotate` | O(n) | O(1) |
| `next_permutation` | O(n) | O(1) |
| `accumulate` | O(n) | O(1) |
| `transform` | O(n) | O(1) |

---

## Summary

In this lesson, you learned about:
- Non-modifying algorithms: find, count, all_of, any_of, none_of
- Modifying algorithms: copy, transform, replace, remove
- Sorting operations: sort, stable_sort, partial_sort, nth_element
- Binary search operations: binary_search, lower_bound, upper_bound
- Numeric algorithms: accumulate, inner_product, partial_sum
- Set operations: set_union, set_intersection, set_difference
- Heap operations: make_heap, push_heap, pop_heap
- Min/max operations: min_element, max_element, clamp
- Permutations and partitioning
- Remove-erase idiom and unique operation

## Exercises

1. Write a program that finds the k-th largest element in an unsorted array using `nth_element`.
2. Implement a merge sort using `std::merge` and recursion.
3. Create a word anagram detector using sorting and comparison.
4. Write a program that generates all permutations of a string.
5. Implement a median filter for signal processing using `nth_element`.
6. Create a spell checker using binary search on a sorted dictionary.
7. Write a program to find all pairs of numbers in an array that sum to a target value.
8. Implement a priority queue using heap operations.
9. Create a text justification algorithm using various STL algorithms.
10. Write a program that detects palindromes using `std::equal` and reverse iterators.

## Next Lesson
Lesson 20: Lambda Expressions and Functional Programming
