# Lesson 58: STL Algorithms Part 2

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Modifying Algorithms
2. Sorting Algorithms
3. Partitioning Algorithms
4. Transform and Generate
5. Remove and Replace
6. Copying and Moving
7. Set Operations
8. Exercises and Projects

---

## 1. Modifying Algorithms

### fill and fill_n:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v(10);

    // fill - fill range with value
    std::fill(v.begin(), v.end(), 5);

    std::cout << "After fill: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // fill_n - fill n elements
    std::fill_n(v.begin(), 5, 99);

    std::cout << "After fill_n: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### generate:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v(10);
    int counter = 0;

    std::generate(v.begin(), v.end(), [&counter]() {
        return counter++;
    });

    std::cout << "Generated: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 2. Sorting Algorithms

### sort and partial_sort:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7};

    // sort
    std::sort(v.begin(), v.end());
    std::cout << "Sorted: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // sort descending
    v = {5, 2, 8, 1, 9, 3, 7};
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::cout << "Descending: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // partial_sort - sort first n elements
    v = {5, 2, 8, 1, 9, 3, 7};
    std::partial_sort(v.begin(), v.begin() + 3, v.end());
    std::cout << "Partial sort (top 3): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 3. Partitioning Algorithms

### partition and stable_partition:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // partition - reorder so predicate is true first
    auto it = std::partition(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });

    std::cout << "Partitioned (evens first): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Partition point index: " << (it - v.begin()) << "\n";

    return 0;
}
```

---

## 4. Transform and Generate

### transform:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2(5);

    // Unary transform
    std::transform(v1.begin(), v1.end(), v2.begin(), [](int x) {
        return x * 2;
    });

    std::cout << "Transformed: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";

    // Binary transform
    std::vector<int> v3 = {10, 20, 30, 40, 50};
    std::vector<int> v4(5);

    std::transform(v1.begin(), v1.end(), v3.begin(), v4.begin(),
                  [](int a, int b) { return a + b; });

    std::cout << "Sum: ";
    for (int x : v4) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 5. Remove and Replace

### remove and remove_if:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    // remove - doesn't actually remove, returns new end
    auto newEnd = std::remove(v.begin(), v.end(), 2);
    v.erase(newEnd, v.end());  // Actually remove

    std::cout << "After remove: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // remove_if
    v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    newEnd = std::remove_if(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });
    v.erase(newEnd, v.end());

    std::cout << "After remove_if (odd only): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### replace:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    // replace
    std::replace(v.begin(), v.end(), 2, 99);
    std::cout << "After replace: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // replace_if
    v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::replace_if(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    }, 0);

    std::cout << "After replace_if: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 6. Copying and Moving

### copy and move:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2(5);

    // copy
    std::copy(v1.begin(), v1.end(), v2.begin());
    std::cout << "Copied: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";

    // copy_if
    std::vector<int> v3;
    std::copy_if(v1.begin(), v1.end(), std::back_inserter(v3), [](int x) {
        return x > 2;
    });
    std::cout << "Copy if > 2: ";
    for (int x : v3) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 7. Set Operations

### set_union and set_intersection:

```cpp
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {3, 4, 5, 6, 7};
    std::vector<int> result;

    // set_union
    std::set_union(v1.begin(), v1.end(),
                   v2.begin(), v2.end(),
                   std::back_inserter(result));
    std::cout << "Union: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    // set_intersection
    result.clear();
    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(result));
    std::cout << "Intersection: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Project: Data Processor

```cpp
#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

class DataProcessor {
public:
    static void removeOutliers(std::vector<double>& data, double threshold) {
        double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

        auto newEnd = std::remove_if(data.begin(), data.end(), [mean, threshold](double x) {
            return std::abs(x - mean) > threshold;
        });
        data.erase(newEnd, data.end());
    }

    static void normalize(std::vector<double>& data) {
        auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
        double min = *minIt;
        double max = *maxIt;
        double range = max - min;

        std::transform(data.begin(), data.end(), data.begin(), [min, range](double x) {
            return (x - min) / range;
        });
    }

    static void process(std::vector<double>& data) {
        std::cout << "Original: ";
        for (double x : data) std::cout << x << " ";
        std::cout << "\n";

        removeOutliers(data, 20.0);
        std::cout << "After removing outliers: ";
        for (double x : data) std::cout << x << " ";
        std::cout << "\n";

        normalize(data);
        std::cout << "Normalized: ";
        for (double x : data) std::cout << x << " ";
        std::cout << "\n";
    }
};

int main()
{
    std::vector<double> data = {10, 12, 15, 11, 13, 100, 14, 12, 11};
    DataProcessor::process(data);
    return 0;
}
```

---

## Key Takeaways

1. ✅ Modifying algorithms change container contents
2. ✅ sort provides O(n log n) sorting
3. ✅ partition reorders elements
4. ✅ transform applies function to elements
5. ✅ remove doesn't delete, just reorders
6. ✅ Must call erase after remove
7. ✅ Set operations work on sorted ranges

---

## Next Lesson Preview

In **Lesson 59**, we'll learn about:
- Iterator categories
- Iterator operations
- Iterator adaptors
- Custom iterators

**Excellent progress!**
