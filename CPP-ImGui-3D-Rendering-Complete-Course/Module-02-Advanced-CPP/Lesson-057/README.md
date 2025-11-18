# Lesson 57: STL Algorithms Deep Dive

**Duration:** 2 hours

## Learning Objectives
- Master common STL algorithms
- Understand algorithm categories
- Learn algorithm composition
- Use iterators effectively with algorithms
- Optimize code with STL algorithms

## Introduction

The C++ Standard Template Library provides a rich set of algorithms that operate on containers through iterators. Understanding these algorithms is crucial for writing efficient, expressive C++ code.

## Non-Modifying Sequence Operations

### find, find_if, count

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // find
    auto it = std::find(nums.begin(), nums.end(), 5);
    if (it != nums.end()) {
        std::cout << "Found: " << *it << std::endl;
    }

    // find_if
    auto it2 = std::find_if(nums.begin(), nums.end(),
                            [](int n) { return n > 7; });
    std::cout << "First > 7: " << *it2 << std::endl;

    // count
    int evenCount = std::count_if(nums.begin(), nums.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: " << evenCount << std::endl;

    return 0;
}
```

## Modifying Sequence Operations

### transform, replace, remove

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // transform
    std::transform(nums.begin(), nums.end(), nums.begin(),
                   [](int n) { return n * 2; });

    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // replace
    std::replace(nums.begin(), nums.end(), 6, 100);

    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // remove_if (doesn't actually remove, returns new end)
    auto newEnd = std::remove_if(nums.begin(), nums.end(),
                                  [](int n) { return n > 5; });
    nums.erase(newEnd, nums.end());

    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

## Sorting and Searching

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};

    // sort
    std::sort(nums.begin(), nums.end());

    // binary_search (requires sorted range)
    bool found = std::binary_search(nums.begin(), nums.end(), 5);
    std::cout << "5 found: " << found << std::endl;

    // lower_bound, upper_bound
    auto lower = std::lower_bound(nums.begin(), nums.end(), 5);
    auto upper = std::upper_bound(nums.begin(), nums.end(), 5);

    std::cout << "Lower bound index: "
              << (lower - nums.begin()) << std::endl;

    return 0;
}
```

## Exercises

### Exercise 1: Data Processing Pipeline

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> data = {10, 25, 5, 30, 15, 20, 8, 12};

    // Remove values less than 10
    auto newEnd = std::remove_if(data.begin(), data.end(),
                                  [](int n) { return n < 10; });
    data.erase(newEnd, data.end());

    // Sort
    std::sort(data.begin(), data.end());

    // Calculate sum
    int sum = std::accumulate(data.begin(), data.end(), 0);

    std::cout << "Processed data: ";
    for (int n : data) std::cout << n << " ";
    std::cout << "\nSum: " << sum << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Non-modifying sequence operations
- Modifying sequence operations
- Sorting and searching algorithms
- Algorithm composition patterns
- Iterator usage with algorithms

## Checklist

- [ ] Understand find variants
- [ ] Can use transform effectively
- [ ] Know sorting algorithms
- [ ] Can compose algorithms
- [ ] Completed exercises

## Next Lesson

In Lesson 58, we'll explore **Multithreading Basics** with std::thread and synchronization.
