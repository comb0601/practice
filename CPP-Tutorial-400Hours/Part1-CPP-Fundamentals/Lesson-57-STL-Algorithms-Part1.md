# Lesson 57: STL Algorithms Part 1

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to STL Algorithms
2. Non-Modifying Algorithms
3. Searching Algorithms
4. Counting and Finding
5. Comparing Algorithms
6. Min/Max Algorithms
7. Practical Examples
8. Exercises and Projects

---

## 1. Introduction to STL Algorithms

STL algorithms are template functions that operate on ranges defined by iterators. They work with any container that provides iterators.

### Key Concepts:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {5, 2, 8, 1, 9};

    // Most algorithms work on iterators
    std::sort(v.begin(), v.end());

    // Range: [begin, end)
    // end points ONE PAST the last element

    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 2. Non-Modifying Algorithms

### for_each:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

void print(int x) {
    std::cout << x << " ";
}

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    // for_each with function
    std::for_each(v.begin(), v.end(), print);
    std::cout << "\n";

    // for_each with lambda
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x * 2 << " ";
    });
    std::cout << "\n";

    return 0;
}
```

---

## 3. Searching Algorithms

### find and find_if:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {10, 20, 30, 40, 50};

    // find - search for value
    auto it = std::find(v.begin(), v.end(), 30);
    if (it != v.end()) {
        std::cout << "Found: " << *it << "\n";
        std::cout << "Index: " << (it - v.begin()) << "\n";
    }

    // find_if - search with predicate
    auto it2 = std::find_if(v.begin(), v.end(), [](int x) {
        return x > 35;
    });
    if (it2 != v.end()) {
        std::cout << "First > 35: " << *it2 << "\n";
    }

    // find_if_not
    auto it3 = std::find_if_not(v.begin(), v.end(), [](int x) {
        return x < 30;
    });
    if (it3 != v.end()) {
        std::cout << "First >= 30: " << *it3 << "\n";
    }

    return 0;
}
```

### binary_search (requires sorted range):

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 3, 5, 7, 9, 11, 13};

    // binary_search - returns bool
    bool found = std::binary_search(v.begin(), v.end(), 7);
    std::cout << "7 found? " << (found ? "Yes" : "No") << "\n";

    // lower_bound - first element >= value
    auto it = std::lower_bound(v.begin(), v.end(), 6);
    std::cout << "Lower bound of 6: " << *it << "\n";

    // upper_bound - first element > value
    auto it2 = std::upper_bound(v.begin(), v.end(), 7);
    std::cout << "Upper bound of 7: " << *it2 << "\n";

    return 0;
}
```

---

## 4. Counting and Finding

### count and count_if:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    // count - count occurrences
    int count2 = std::count(v.begin(), v.end(), 2);
    std::cout << "Count of 2: " << count2 << "\n";

    // count_if - count with predicate
    int countEven = std::count_if(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });
    std::cout << "Even numbers: " << countEven << "\n";

    return 0;
}
```

---

## 5. Comparing Algorithms

### equal and mismatch:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    std::vector<int> v3 = {1, 2, 9, 4, 5};

    // equal - check if ranges are equal
    bool eq1 = std::equal(v1.begin(), v1.end(), v2.begin());
    std::cout << "v1 == v2? " << (eq1 ? "Yes" : "No") << "\n";

    bool eq2 = std::equal(v1.begin(), v1.end(), v3.begin());
    std::cout << "v1 == v3? " << (eq2 ? "Yes" : "No") << "\n";

    // mismatch - find first difference
    auto result = std::mismatch(v1.begin(), v1.end(), v3.begin());
    if (result.first != v1.end()) {
        std::cout << "First mismatch: v1[" << *result.first
                  << "] vs v3[" << *result.second << "]\n";
    }

    return 0;
}
```

---

## 6. Min/Max Algorithms

### min_element and max_element:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {5, 2, 8, 1, 9, 3};

    // min_element
    auto minIt = std::min_element(v.begin(), v.end());
    std::cout << "Min: " << *minIt << "\n";
    std::cout << "Min index: " << (minIt - v.begin()) << "\n";

    // max_element
    auto maxIt = std::max_element(v.begin(), v.end());
    std::cout << "Max: " << *maxIt << "\n";

    // minmax_element - returns pair
    auto [min, max] = std::minmax_element(v.begin(), v.end());
    std::cout << "Min: " << *min << ", Max: " << *max << "\n";

    return 0;
}
```

---

## 7. Practical Examples

### Example: Find All Occurrences

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    std::vector<int> indices;

    // Find all occurrences of 2
    auto it = v.begin();
    while ((it = std::find(it, v.end(), 2)) != v.end()) {
        indices.push_back(it - v.begin());
        ++it;
    }

    std::cout << "Indices of 2: ";
    for (int idx : indices) {
        std::cout << idx << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise: Statistics Calculator

```cpp
#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>

class Statistics {
public:
    static double mean(const std::vector<double>& data) {
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    static double median(std::vector<double> data) {
        std::sort(data.begin(), data.end());
        size_t n = data.size();
        return n % 2 == 0 ? (data[n/2 - 1] + data[n/2]) / 2 : data[n/2];
    }

    static double stddev(const std::vector<double>& data) {
        double m = mean(data);
        double sum = 0.0;
        std::for_each(data.begin(), data.end(), [&](double x) {
            sum += (x - m) * (x - m);
        });
        return std::sqrt(sum / data.size());
    }

    static void analyze(const std::vector<double>& data) {
        std::cout << "Mean: " << mean(data) << "\n";
        std::cout << "Median: " << median(data) << "\n";
        std::cout << "Std Dev: " << stddev(data) << "\n";
        std::cout << "Min: " << *std::min_element(data.begin(), data.end()) << "\n";
        std::cout << "Max: " << *std::max_element(data.begin(), data.end()) << "\n";
    }
};

int main()
{
    std::vector<double> scores = {85, 90, 78, 92, 88, 76, 95, 89};
    Statistics::analyze(scores);
    return 0;
}
```

---

## Key Takeaways

1. ✅ STL algorithms work on iterator ranges
2. ✅ Non-modifying algorithms don't change the container
3. ✅ find and find_if for searching
4. ✅ count and count_if for counting
5. ✅ binary_search requires sorted range
6. ✅ min/max_element find extremes
7. ✅ Algorithms are generic and work with any container

---

## Next Lesson Preview

In **Lesson 58**, we'll learn about:
- Modifying algorithms
- Sorting and partitioning
- Transforming and generating
- Removing and replacing

**Great work on STL algorithms!**
