# Lesson 13: STL Containers in C++

**Estimated Time**: 4-5 hours
**Difficulty**: Intermediate
**Prerequisites**: Templates, Classes, Iterators

---

## Table of Contents
1. [Introduction to STL](#introduction)
2. [Container Categories](#categories)
3. [Vector](#vector)
4. [Deque](#deque)
5. [List](#list)
6. [Forward List](#forward-list)
7. [Array](#array)
8. [Set and Multiset](#set)
9. [Map and Multimap](#map)
10. [Unordered Set](#unordered-set)
11. [Unordered Map](#unordered-map)
12. [Stack](#stack)
13. [Queue and Priority Queue](#queue)
14. [Container Adapters](#adapters)
15. [Choosing the Right Container](#choosing)
16. [Performance Comparison](#performance)
17. [Exercises](#exercises)

---

## 1. Introduction to STL {#introduction}

The **Standard Template Library (STL)** is a powerful set of C++ template classes providing general-purpose containers, algorithms, and iterators. Containers are objects that store collections of other objects.

### Key Benefits

1. **Reusability**: Pre-built, tested data structures
2. **Efficiency**: Highly optimized implementations
3. **Type Safety**: Template-based, compile-time type checking
4. **Consistency**: Uniform interface across containers
5. **Interoperability**: Works seamlessly with STL algorithms

### Container Requirements

All STL containers must support:
- Copy construction and assignment
- Destruction
- Equality and inequality comparison
- Size queries
- Iterator access

---

## 2. Container Categories {#categories}

### Sequence Containers
Store elements in **linear sequence**. You control element ordering.

- `vector` - Dynamic array
- `deque` - Double-ended queue
- `list` - Doubly-linked list
- `forward_list` - Singly-linked list
- `array` - Fixed-size array

### Associative Containers
Store elements in **sorted order** based on keys. Fast lookup.

- `set` - Unique sorted keys
- `multiset` - Sorted keys (duplicates allowed)
- `map` - Unique sorted key-value pairs
- `multimap` - Sorted key-value pairs (duplicate keys allowed)

### Unordered Associative Containers
Store elements in **hash tables**. Very fast lookup.

- `unordered_set` - Unique keys (hashed)
- `unordered_multiset` - Keys with duplicates (hashed)
- `unordered_map` - Unique key-value pairs (hashed)
- `unordered_multimap` - Key-value pairs, duplicate keys (hashed)

### Container Adapters
Provide restricted interfaces to other containers.

- `stack` - LIFO (Last In First Out)
- `queue` - FIFO (First In First Out)
- `priority_queue` - Priority-based queue

---

## 3. Vector {#vector}

`vector` is a dynamic array that can grow and shrink automatically.

### Key Features

- Continuous memory allocation
- Random access: O(1)
- Insertion/deletion at end: O(1) amortized
- Insertion/deletion in middle: O(n)
- Automatic resizing

### Common Operations

```cpp
#include <vector>
using namespace std;

vector<int> vec;                    // Empty vector
vector<int> vec2(10);               // 10 elements, value-initialized
vector<int> vec3(10, 5);            // 10 elements, all initialized to 5
vector<int> vec4 = {1, 2, 3, 4, 5}; // Initializer list

// Adding elements
vec.push_back(10);                  // Add to end
vec.emplace_back(20);               // Construct in place
vec.insert(vec.begin(), 5);         // Insert at position

// Accessing elements
int first = vec.front();            // First element
int last = vec.back();              // Last element
int third = vec[2];                 // Index access (no bounds check)
int fourth = vec.at(3);             // Bounds-checked access

// Removing elements
vec.pop_back();                     // Remove last
vec.erase(vec.begin() + 2);         // Remove at position
vec.clear();                        // Remove all

// Size and capacity
size_t size = vec.size();           // Number of elements
size_t cap = vec.capacity();        // Allocated space
bool empty = vec.empty();           // Check if empty
vec.reserve(100);                   // Reserve space
vec.shrink_to_fit();                // Reduce capacity to size

// Iteration
for (int val : vec) { }             // Range-based for
for (auto it = vec.begin(); it != vec.end(); ++it) { }
```

### Performance Characteristics

| Operation | Complexity |
|-----------|-----------|
| Random access | O(1) |
| Insert/remove at end | O(1) amortized |
| Insert/remove at middle | O(n) |
| Search | O(n) |

---

## 4. Deque {#deque}

`deque` (double-ended queue) allows fast insertion/deletion at both ends.

### Key Features

- Not necessarily continuous memory
- Random access: O(1)
- Fast insertion/deletion at both ends: O(1)
- Slightly more overhead than vector

### Common Operations

```cpp
#include <deque>
using namespace std;

deque<int> dq;

// Adding elements
dq.push_back(10);     // Add to end
dq.push_front(5);     // Add to front
dq.emplace_back(20);
dq.emplace_front(1);

// Accessing
int first = dq.front();
int last = dq.back();
int elem = dq[2];

// Removing
dq.pop_back();
dq.pop_front();

// Similar size/capacity operations as vector
```

---

## 5. List {#list}

`list` is a doubly-linked list allowing fast insertion/deletion anywhere.

### Key Features

- Non-continuous memory
- No random access
- Fast insertion/deletion anywhere: O(1)
- Bidirectional iteration
- Higher memory overhead

### Common Operations

```cpp
#include <list>
using namespace std;

list<int> lst = {1, 2, 3, 4, 5};

// Adding elements
lst.push_back(10);
lst.push_front(0);
lst.insert(++lst.begin(), 15);  // Insert after first

// Removing elements
lst.pop_back();
lst.pop_front();
lst.remove(3);                  // Remove all elements with value 3
lst.remove_if([](int x) { return x > 10; });

// Unique operations
lst.sort();                     // Sort in place
lst.reverse();                  // Reverse in place
lst.unique();                   // Remove consecutive duplicates

// Splicing (move elements between lists)
list<int> lst2 = {100, 200};
lst.splice(lst.begin(), lst2);  // Move all from lst2 to lst
```

---

## 6. Forward List {#forward-list}

`forward_list` is a singly-linked list (C++11).

### Key Features

- Minimal memory overhead
- Forward iteration only
- No `size()` operation
- More memory-efficient than list

```cpp
#include <forward_list>
using namespace std;

forward_list<int> flst = {1, 2, 3, 4, 5};

flst.push_front(0);
flst.insert_after(flst.begin(), 10);
flst.remove(3);
flst.sort();
```

---

## 7. Array {#array}

`array` is a fixed-size array wrapper (C++11).

### Key Features

- Fixed size determined at compile time
- No dynamic allocation
- Random access: O(1)
- Stack-based (usually)

```cpp
#include <array>
using namespace std;

array<int, 5> arr = {1, 2, 3, 4, 5};

arr[0] = 10;
arr.at(1) = 20;

int first = arr.front();
int last = arr.back();
size_t size = arr.size();  // Always 5

arr.fill(0);  // Fill all with 0
```

---

## 8. Set and Multiset {#set}

`set` stores **unique sorted elements**. `multiset` allows duplicates.

### Key Features

- Elements stored in sorted order
- Fast lookup: O(log n)
- Implemented as balanced binary tree (typically Red-Black tree)
- No duplicate keys in `set`

### Set Operations

```cpp
#include <set>
using namespace std;

set<int> s;

// Insertion
s.insert(10);
s.insert(5);
s.insert(20);
s.emplace(15);

// Lookup
bool found = (s.find(10) != s.end());
bool contains = (s.count(5) > 0);

// Removal
s.erase(10);
s.erase(s.find(5));

// Iteration (always sorted)
for (int val : s) { }  // Outputs: 15, 20 (if only these remain)

// Range operations
auto lower = s.lower_bound(10);  // First >= 10
auto upper = s.upper_bound(20);  // First > 20
```

### Multiset

```cpp
#include <set>
using namespace std;

multiset<int> ms = {1, 2, 2, 3, 3, 3, 4};

ms.insert(2);  // Now three 2's
int count = ms.count(3);  // Returns 3

// Remove all occurrences
ms.erase(3);

// Remove single occurrence
auto it = ms.find(2);
if (it != ms.end()) {
    ms.erase(it);  // Removes only one 2
}
```

---

## 9. Map and Multimap {#map}

`map` stores **unique key-value pairs** in sorted order. `multimap` allows duplicate keys.

### Key Features

- Keys sorted automatically
- Fast lookup by key: O(log n)
- Implemented as balanced binary tree
- Unique keys in `map`

### Map Operations

```cpp
#include <map>
using namespace std;

map<string, int> ages;

// Insertion
ages["Alice"] = 25;
ages["Bob"] = 30;
ages.insert({"Charlie", 35});
ages.emplace("Diana", 28);

// Access
int aliceAge = ages["Alice"];
int bobAge = ages.at("Bob");  // Throws if not found

// Lookup
if (ages.find("Charlie") != ages.end()) { }
if (ages.count("Eve") > 0) { }

// Removal
ages.erase("Bob");

// Iteration
for (const auto& pair : ages) {
    cout << pair.first << ": " << pair.second << endl;
}

// C++17 structured bindings
for (const auto& [name, age] : ages) {
    cout << name << ": " << age << endl;
}
```

### Multimap

```cpp
#include <map>
using namespace std;

multimap<string, int> scores;
scores.insert({"Alice", 95});
scores.insert({"Alice", 87});  // Multiple values for same key
scores.insert({"Bob", 92});

// Find range for key
auto range = scores.equal_range("Alice");
for (auto it = range.first; it != range.second; ++it) {
    cout << it->second << endl;  // Prints 95 and 87
}
```

---

## 10. Unordered Set {#unordered-set}

`unordered_set` stores unique elements using hash table.

### Key Features

- Elements not sorted
- Very fast lookup: O(1) average
- Uses hash function
- Requires hashable type

```cpp
#include <unordered_set>
using namespace std;

unordered_set<int> us = {1, 2, 3, 4, 5};

us.insert(6);
us.erase(3);

bool found = (us.find(4) != us.end());

// Custom hash for custom types
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };
}

unordered_set<Point> points;
```

---

## 11. Unordered Map {#unordered-map}

`unordered_map` stores key-value pairs using hash table.

```cpp
#include <unordered_map>
using namespace std;

unordered_map<string, int> inventory;

inventory["apple"] = 10;
inventory["banana"] = 5;

int apples = inventory["apple"];

if (inventory.find("orange") != inventory.end()) { }

// Faster than map for lookup, but no ordering
```

---

## 12. Stack {#stack}

`stack` provides LIFO (Last In First Out) access.

```cpp
#include <stack>
using namespace std;

stack<int> stk;

stk.push(10);
stk.push(20);
stk.push(30);

int top = stk.top();  // 30
stk.pop();            // Removes 30
int newTop = stk.top(); // 20

bool empty = stk.empty();
size_t size = stk.size();
```

---

## 13. Queue and Priority Queue {#queue}

### Queue (FIFO)

```cpp
#include <queue>
using namespace std;

queue<int> q;

q.push(10);
q.push(20);
q.push(30);

int front = q.front();  // 10
int back = q.back();    // 30
q.pop();                // Removes 10
```

### Priority Queue

Elements ordered by priority (max-heap by default).

```cpp
#include <queue>
using namespace std;

priority_queue<int> pq;

pq.push(30);
pq.push(10);
pq.push(20);

int top = pq.top();  // 30 (largest)
pq.pop();            // Removes 30

// Min-heap
priority_queue<int, vector<int>, greater<int>> minHeap;
minHeap.push(30);
minHeap.push(10);
minHeap.push(20);
int minTop = minHeap.top();  // 10 (smallest)
```

---

## 14. Container Adapters {#adapters}

Container adapters provide restricted interfaces:

| Adapter | Underlying | Interface |
|---------|-----------|-----------|
| stack | deque (default) | LIFO |
| queue | deque (default) | FIFO |
| priority_queue | vector (default) | Priority-based |

You can specify the underlying container:

```cpp
stack<int, vector<int>> stk;        // Stack using vector
queue<int, list<int>> q;            // Queue using list
priority_queue<int, vector<int>> pq; // Priority queue using vector
```

---

## 15. Choosing the Right Container {#choosing}

### Decision Guide

**Need random access?**
- Yes → `vector`, `deque`, or `array`
- No → `list`, `forward_list`, or associative containers

**Frequent insertions/deletions at ends?**
- Both ends → `deque`
- Back only → `vector`
- Front only → `deque` or `list`

**Frequent insertions/deletions in middle?**
- Yes → `list` or `forward_list`
- No → `vector` or `deque`

**Need fast lookup by key?**
- Sorted → `set` or `map`
- Unsorted/fastest → `unordered_set` or `unordered_map`

**Fixed size known at compile time?**
- Yes → `array`
- No → `vector` or `deque`

**Need LIFO/FIFO?**
- LIFO → `stack`
- FIFO → `queue`
- Priority → `priority_queue`

---

## 16. Performance Comparison {#performance}

### Time Complexity

| Container | Access | Insert/Delete Front | Insert/Delete Back | Insert/Delete Middle | Search |
|-----------|--------|---------------------|-------------------|---------------------|--------|
| vector | O(1) | O(n) | O(1)* | O(n) | O(n) |
| deque | O(1) | O(1) | O(1) | O(n) | O(n) |
| list | - | O(1) | O(1) | O(1) | O(n) |
| set | - | O(log n) | O(log n) | O(log n) | O(log n) |
| map | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) |
| unordered_set | - | O(1)** | O(1)** | O(1)** | O(1)** |
| unordered_map | O(1)** | O(1)** | O(1)** | O(1)** | O(1)** |

\* Amortized
\*\* Average case

### Memory Overhead

- `vector`: Low (just array + size/capacity)
- `deque`: Medium (multiple chunks)
- `list`: High (two pointers per node)
- `set/map`: High (tree nodes with pointers)
- `unordered_set/map`: Medium to High (hash table + buckets)

---

## 17. Exercises {#exercises}

### Exercise 1: Vector Operations
Create a program managing a collection of students using `vector`.

### Exercise 2: Map for Word Count
Read text and count word frequencies using `map`.

### Exercise 3: Set Operations
Implement set union, intersection, and difference using `set`.

### Exercise 4: Queue Simulation
Simulate a customer service queue using `queue`.

### Exercise 5: Priority Task Manager
Implement a task manager using `priority_queue`.

### Exercise 6: Unique Elements
Remove duplicates from a vector using `set`.

### Exercise 7: Adjacency List
Implement a graph using `vector` of `list`.

### Exercise 8: Phone Book
Create a phone book using `unordered_map`.

### Exercise 9: Balanced Parentheses
Check balanced parentheses using `stack`.

### Exercise 10: Top K Elements
Find K largest elements using `priority_queue`.

---

## Summary

STL containers provide powerful, efficient data structures:

**Key Takeaways**:
1. Choose containers based on access patterns
2. `vector` is the default choice for most cases
3. Use `set`/`map` when you need sorted, unique keys
4. Use `unordered_set`/`unordered_map` for fastest lookup
5. `list` for frequent mid-sequence insertions
6. Container adapters provide specialized interfaces
7. All containers work with STL algorithms
8. Consider memory and time complexity
9. Use modern C++ features (emplace, range-for, auto)
10. Profile before optimizing container choice

**Next Steps**:
- Master iterators for container traversal
- Learn STL algorithms that work with containers
- Practice choosing appropriate containers
- Understand performance characteristics
- Explore container customization

---

**Continue to Lesson 14: STL Algorithms** to learn how to process container elements efficiently.
