# Lesson 14: STL Algorithms in C++

**Estimated Time**: 4-5 hours
**Difficulty**: Intermediate
**Prerequisites**: STL Containers, Templates, Iterators

## Table of Contents
1. [Introduction](#introduction)
2. [Non-Modifying Algorithms](#non-modifying)
3. [Modifying Algorithms](#modifying)
4. [Sorting Algorithms](#sorting)
5. [Binary Search Algorithms](#binary-search)
6. [Set Operations](#set-operations)
7. [Heap Operations](#heap)
8. [Min/Max Operations](#minmax)
9. [Numeric Algorithms](#numeric)
10. [Best Practices](#best-practices)

## 1. Introduction {#introduction}

STL algorithms are function templates that operate on containers through iterators. They provide efficient, tested implementations of common operations.

### Benefits
- **Reusability**: Generic across container types
- **Efficiency**: Optimized implementations
- **Expressiveness**: Clear, declarative code
- **Correctness**: Well-tested standard library code

### Algorithm Categories
- Non-modifying: find, count, search
- Modifying: copy, fill, transform
- Removing: remove, unique
- Sorting: sort, partial_sort
- Binary search: binary_search, lower_bound
- Set operations: union, intersection
- Heap: make_heap, push_heap
- Min/Max: min_element, max_element
- Numeric: accumulate, inner_product

## 2. Non-Modifying Algorithms {#non-modifying}

### find, find_if, find_if_not
```cpp
auto it = find(v.begin(), v.end(), 5);
auto it2 = find_if(v.begin(), v.end(), [](int x){ return x > 5; });
```

### count, count_if
```cpp
int n = count(v.begin(), v.end(), 5);
int n2 = count_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
```

### all_of, any_of, none_of
```cpp
bool all = all_of(v.begin(), v.end(), [](int x){ return x > 0; });
bool any = any_of(v.begin(), v.end(), [](int x){ return x < 0; });
bool none = none_of(v.begin(), v.end(), [](int x){ return x == 0; });
```

### for_each
```cpp
for_each(v.begin(), v.end(), [](int x){ cout << x << " "; });
```

### equal, mismatch
```cpp
bool eq = equal(v1.begin(), v1.end(), v2.begin());
auto [it1, it2] = mismatch(v1.begin(), v1.end(), v2.begin());
```

## 3. Modifying Algorithms {#modifying}

### copy, copy_if, copy_n
```cpp
copy(src.begin(), src.end(), dest.begin());
copy_if(src.begin(), src.end(), back_inserter(dest), predicate);
copy_n(src.begin(), 5, dest.begin());
```

### move, move_backward
```cpp
move(src.begin(), src.end(), dest.begin());
```

### fill, fill_n
```cpp
fill(v.begin(), v.end(), 42);
fill_n(v.begin(), 5, 0);
```

### transform
```cpp
transform(v.begin(), v.end(), v.begin(), [](int x){ return x * 2; });
```

### generate, generate_n
```cpp
generate(v.begin(), v.end(), rand);
```

### replace, replace_if
```cpp
replace(v.begin(), v.end(), 0, 1);
replace_if(v.begin(), v.end(), [](int x){ return x < 0; }, 0);
```

### remove, remove_if
```cpp
auto new_end = remove(v.begin(), v.end(), 0);
v.erase(new_end, v.end());
```

### unique
```cpp
auto new_end = unique(v.begin(), v.end());
v.erase(new_end, v.end());
```

### reverse, rotate
```cpp
reverse(v.begin(), v.end());
rotate(v.begin(), v.begin() + 3, v.end());
```

### shuffle (C++11)
```cpp
shuffle(v.begin(), v.end(), rng);
```

## 4. Sorting Algorithms {#sorting}

### sort
```cpp
sort(v.begin(), v.end());  // Ascending
sort(v.begin(), v.end(), greater<int>());  // Descending
sort(v.begin(), v.end(), [](int a, int b){ return abs(a) < abs(b); });
```

### stable_sort
Maintains relative order of equal elements.
```cpp
stable_sort(v.begin(), v.end());
```

### partial_sort
```cpp
partial_sort(v.begin(), v.begin() + 5, v.end());  // Sort first 5
```

### nth_element
```cpp
nth_element(v.begin(), v.begin() + 5, v.end());  // 5th element in sorted position
```

### is_sorted, is_sorted_until
```cpp
bool sorted = is_sorted(v.begin(), v.end());
auto it = is_sorted_until(v.begin(), v.end());
```

## 5. Binary Search Algorithms {#binary-search}

Require sorted ranges.

### binary_search
```cpp
bool found = binary_search(v.begin(), v.end(), 5);
```

### lower_bound, upper_bound
```cpp
auto lower = lower_bound(v.begin(), v.end(), 5);  // First >= 5
auto upper = upper_bound(v.begin(), v.end(), 5);  // First > 5
```

### equal_range
```cpp
auto [lower, upper] = equal_range(v.begin(), v.end(), 5);
```

## 6. Set Operations {#set-operations}

Require sorted ranges.

### set_union, set_intersection, set_difference
```cpp
set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(result));
set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(result));
set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(result));
```

### includes
```cpp
bool subset = includes(s1.begin(), s1.end(), s2.begin(), s2.end());
```

## 7. Heap Operations {#heap}

### make_heap, push_heap, pop_heap
```cpp
make_heap(v.begin(), v.end());
v.push_back(10);
push_heap(v.begin(), v.end());
pop_heap(v.begin(), v.end());
v.pop_back();
```

### sort_heap
```cpp
sort_heap(v.begin(), v.end());
```

### is_heap
```cpp
bool is_heap_val = is_heap(v.begin(), v.end());
```

## 8. Min/Max Operations {#minmax}

### min, max, minmax
```cpp
int minimum = min(a, b);
int maximum = max(a, b);
auto [min_val, max_val] = minmax(a, b);
```

### min_element, max_element, minmax_element
```cpp
auto min_it = min_element(v.begin(), v.end());
auto max_it = max_element(v.begin(), v.end());
auto [min_it, max_it] = minmax_element(v.begin(), v.end());
```

### clamp (C++17)
```cpp
int clamped = clamp(value, low, high);
```

## 9. Numeric Algorithms {#numeric}

Include `<numeric>`.

### accumulate
```cpp
int sum = accumulate(v.begin(), v.end(), 0);
int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
```

### inner_product
```cpp
int dot = inner_product(v1.begin(), v1.end(), v2.begin(), 0);
```

### adjacent_difference
```cpp
adjacent_difference(v.begin(), v.end(), result.begin());
```

### partial_sum
```cpp
partial_sum(v.begin(), v.end(), result.begin());
```

### iota
```cpp
iota(v.begin(), v.end(), 0);  // Fill with 0, 1, 2, ...
```

## 10. Best Practices {#best-practices}

1. **Prefer algorithms over loops**
2. **Use lambdas for predicates**
3. **Remember iterator invalidation**
4. **Choose appropriate container**
5. **Understand complexity guarantees**
6. **Use range-based for when appropriate**
7. **Consider parallel algorithms (C++17)**

## Summary

STL algorithms provide powerful, generic operations on containers. Master these to write efficient, expressive C++ code.

**Next**: Lesson 15 - Exception Handling
