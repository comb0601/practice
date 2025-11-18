# Lesson 13: STL Containers - Code Examples

This directory contains 15 executable C++ programs demonstrating STL containers.

## Compilation

```bash
g++ -std=c++17 filename.cpp -o filename
./filename
```

## Examples Overview

### Sequence Containers
- **01_vector_basics.cpp** - Comprehensive vector operations
- **02_deque_demo.cpp** - Double-ended queue operations
- **03_list_operations.cpp** - List manipulations and algorithms
- **13_array_container.cpp** - Fixed-size array container

### Associative Containers
- **04_set_operations.cpp** - Set operations and unique elements
- **05_map_dictionary.cpp** - Map as key-value store
- **11_multiset_demo.cpp** - Multiset with duplicates
- **12_multimap_demo.cpp** - Multimap with duplicate keys

### Unordered Containers
- **06_unordered_set.cpp** - Hash-based set
- **07_unordered_map.cpp** - Hash-based map

### Container Adapters
- **08_stack_demo.cpp** - LIFO stack operations
- **09_queue_demo.cpp** - FIFO queue operations
- **10_priority_queue.cpp** - Priority-based queue

### Practical Applications
- **14_word_frequency.cpp** - Word frequency counter
- **15_container_comparison.cpp** - Performance comparison

## Key Concepts

- **Vector**: Dynamic array, random access
- **Deque**: Double-ended queue
- **List**: Doubly-linked list
- **Set/Map**: Sorted, O(log n) operations
- **Unordered Set/Map**: Hashed, O(1) average operations
- **Stack/Queue**: LIFO/FIFO adapters

## Learning Path

1. Start with vector (01) - most common container
2. Learn other sequence containers (02-03, 13)
3. Master associative containers (04-05, 11-12)
4. Understand unordered containers (06-07)
5. Practice with adapters (08-10)
6. Apply in real scenarios (14-15)
