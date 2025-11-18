# Lesson 59: Iterators

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Iterators
2. Iterator Categories
3. Iterator Operations
4. Iterator Adaptors
5. Stream Iterators
6. Insert Iterators
7. Reverse Iterators
8. Exercises and Projects

---

## 1. Introduction to Iterators

Iterators are objects that point to elements in a container and allow traversal through the container.

### Basic Iterator Usage:

```cpp
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Iterator declaration
    std::vector<int>::iterator it;

    // begin() - points to first element
    it = v.begin();
    std::cout << "First element: " << *it << "\n";

    // end() - points ONE PAST last element
    std::vector<int>::iterator endIt = v.end();

    // Iteration
    for (it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 2. Iterator Categories

### Five Iterator Categories:

```cpp
/*
Input Iterator:
- Read-only, forward only
- Example: istream_iterator

Output Iterator:
- Write-only, forward only
- Example: ostream_iterator

Forward Iterator:
- Read/write, forward only, multi-pass
- Example: forward_list::iterator

Bidirectional Iterator:
- Read/write, forward and backward
- Example: list::iterator, map::iterator

Random Access Iterator:
- Read/write, jump to any position
- Example: vector::iterator, array::iterator
*/
```

### Iterator Operations by Category:

```cpp
#include <vector>
#include <list>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};

    // Random access iterator (vector)
    auto vit = v.begin();
    vit += 2;  // Jump forward
    std::cout << "v[2]: " << *vit << "\n";

    // Bidirectional iterator (list)
    auto lit = lst.begin();
    ++lit;  // Forward one
    ++lit;  // Forward one more
    --lit;  // Back one
    std::cout << "list element: " << *lit << "\n";

    // Random access specific
    std::cout << "v[3]: " << v.begin()[3] << "\n";
    // lst.begin()[3];  // ERROR: list doesn't support []

    return 0;
}
```

---

## 3. Iterator Operations

### advance and distance:

```cpp
#include <vector>
#include <iterator>
#include <iostream>

int main()
{
    std::vector<int> v = {10, 20, 30, 40, 50};

    auto it = v.begin();

    // advance - move iterator
    std::advance(it, 2);
    std::cout << "After advance(2): " << *it << "\n";

    // distance - number of elements between iterators
    auto dist = std::distance(v.begin(), it);
    std::cout << "Distance from begin: " << dist << "\n";

    // next and prev (C++11)
    auto nextIt = std::next(it);
    std::cout << "Next element: " << *nextIt << "\n";

    auto prevIt = std::prev(it);
    std::cout << "Previous element: " << *prevIt << "\n";

    return 0;
}
```

---

## 4. Iterator Adaptors

### back_inserter and front_inserter:

```cpp
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2;

    // back_inserter - calls push_back
    std::copy(v1.begin(), v1.end(), std::back_inserter(v2));
    std::cout << "v2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";

    // front_inserter - calls push_front
    std::list<int> lst;
    std::copy(v1.begin(), v1.end(), std::front_inserter(lst));
    std::cout << "lst: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 5. Stream Iterators

### istream_iterator and ostream_iterator:

```cpp
#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

int main()
{
    // ostream_iterator - output to stream
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::copy(v.begin(), v.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    // istream_iterator - input from stream
    std::string data = "10 20 30 40 50";
    std::istringstream iss(data);

    std::vector<int> v2;
    std::copy(std::istream_iterator<int>(iss),
              std::istream_iterator<int>(),
              std::back_inserter(v2));

    std::cout << "Read from stream: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 6. Insert Iterators

### inserter:

```cpp
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::set<int> s = {10, 20, 30};

    // inserter - calls insert()
    std::copy(v.begin(), v.end(), std::inserter(s, s.begin()));

    std::cout << "Set after inserter: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 7. Reverse Iterators

### rbegin and rend:

```cpp
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Reverse iteration
    std::cout << "Reverse: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Converting between iterators
    auto rit = v.rbegin();
    auto it = rit.base();  // Convert to normal iterator

    return 0;
}
```

---

## 8. Exercises and Projects

### Project: Iterator Utilities

```cpp
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

template<typename Iterator>
void printRange(Iterator first, Iterator last, const std::string& label) {
    std::cout << label << ": ";
    for (auto it = first; it != last; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

template<typename Container>
void analyzeContainer(const Container& c) {
    std::cout << "\nContainer Analysis:\n";
    std::cout << "Size: " << std::distance(c.begin(), c.end()) << "\n";

    if (c.empty()) return;

    std::cout << "First: " << *c.begin() << "\n";
    std::cout << "Last: " << *std::prev(c.end()) << "\n";

    auto minIt = std::min_element(c.begin(), c.end());
    auto maxIt = std::max_element(c.begin(), c.end());
    std::cout << "Min: " << *minIt << "\n";
    std::cout << "Max: " << *maxIt << "\n";
}

int main()
{
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7};

    printRange(v.begin(), v.end(), "Forward");
    printRange(v.rbegin(), v.rend(), "Reverse");

    analyzeContainer(v);

    return 0;
}
```

---

## Key Takeaways

1. ✅ Iterators provide unified interface to containers
2. ✅ Five categories: input, output, forward, bidirectional, random access
3. ✅ Use advance() and distance() for iterator manipulation
4. ✅ Stream iterators connect algorithms to I/O
5. ✅ Insert iterators enable output to containers
6. ✅ Reverse iterators traverse backwards
7. ✅ Different containers provide different iterator categories

---

## Next Lesson Preview

In **Lesson 60**, we'll learn about:
- Lambda expressions
- Capture clauses
- Function objects
- std::function

**Great work on iterators!**
