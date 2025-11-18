# Lesson 56: Unordered Containers

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Hash Tables
2. unordered_set Container
3. unordered_map Container
4. Hash Functions
5. Collision Handling
6. Performance Comparison
7. Custom Hash Functions
8. Exercises and Projects

---

## 1. Introduction to Hash Tables

**Hash tables** provide O(1) average-case lookup, insert, and delete operations by using a hash function to map keys to array indices.

### Ordered vs Unordered:

```
Ordered (Tree-based: Red-Black Tree):
├── set / map
├── O(log n) operations
├── Sorted order
└── Iterates in sorted order

Unordered (Hash table):
├── unordered_set / unordered_map
├── O(1) average operations
├── No guaranteed order
└── Iterates in arbitrary order
```

---

## 2. unordered_set Container

### Basic Operations:

```cpp
#include <iostream>
#include <unordered_set>

int main()
{
    // Create unordered_set
    std::unordered_set<int> uset = {5, 2, 8, 1, 9, 3};

    // Insert
    uset.insert(10);
    uset.insert(5);  // Duplicate, ignored

    // Elements in arbitrary order (not sorted!)
    std::cout << "Elements: ";
    for (int x : uset) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Find - O(1) average
    if (uset.find(8) != uset.end()) {
        std::cout << "Found 8\n";
    }

    // Count
    std::cout << "Count of 5: " << uset.count(5) << "\n";

    // Erase
    uset.erase(2);

    // Size
    std::cout << "Size: " << uset.size() << "\n";

    return 0;
}
```

---

## 3. unordered_map Container

### Basic Operations:

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

int main()
{
    std::unordered_map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    // Insert/Update
    ages["David"] = 28;
    ages["Alice"] = 31;  // Update

    // Access
    std::cout << "Alice's age: " << ages["Alice"] << "\n";

    // Find
    auto it = ages.find("Bob");
    if (it != ages.end()) {
        std::cout << "Bob: " << it->second << "\n";
    }

    // Iterate (arbitrary order)
    std::cout << "\nAll ages:\n";
    for (const auto& [name, age] : ages) {
        std::cout << name << ": " << age << "\n";
    }

    return 0;
}
```

---

## 4. Hash Functions

### Built-in Hash Functions:

```cpp
#include <iostream>
#include <unordered_set>
#include <string>

int main()
{
    std::hash<int> intHash;
    std::hash<std::string> stringHash;

    std::cout << "Hash of 42: " << intHash(42) << "\n";
    std::cout << "Hash of 'hello': " << stringHash("hello") << "\n";

    return 0;
}
```

---

## 5. Collision Handling

### Bucket Information:

```cpp
#include <iostream>
#include <unordered_set>

int main()
{
    std::unordered_set<int> uset = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Bucket count: " << uset.bucket_count() << "\n";
    std::cout << "Load factor: " << uset.load_factor() << "\n";
    std::cout << "Max load factor: " << uset.max_load_factor() << "\n";

    // Elements per bucket
    for (size_t i = 0; i < uset.bucket_count(); ++i) {
        std::cout << "Bucket " << i << ": " << uset.bucket_size(i) << " elements\n";
    }

    return 0;
}
```

---

## 6. Performance Comparison

### Benchmark:

```cpp
#include <iostream>
#include <set>
#include <unordered_set>
#include <chrono>

int main()
{
    const int N = 1000000;

    // unordered_set insertion
    auto start = std::chrono::high_resolution_clock::now();
    std::unordered_set<int> uset;
    for (int i = 0; i < N; i++) {
        uset.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto usetTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // set insertion
    start = std::chrono::high_resolution_clock::now();
    std::set<int> oset;
    for (int i = 0; i < N; i++) {
        oset.insert(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto setTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "unordered_set insert: " << usetTime.count() << " ms\n";
    std::cout << "set insert: " << setTime.count() << " ms\n";

    return 0;
}
```

---

## 7. Custom Hash Functions

### Hash for Custom Class:

```cpp
#include <iostream>
#include <unordered_set>
#include <string>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(n), age(a) {}

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// Custom hash function
struct PersonHash {
    size_t operator()(const Person& p) const {
        return std::hash<std::string>()(p.name) ^ (std::hash<int>()(p.age) << 1);
    }
};

int main()
{
    std::unordered_set<Person, PersonHash> people;

    people.emplace("Alice", 30);
    people.emplace("Bob", 25);

    Person searchPerson("Alice", 30);
    if (people.find(searchPerson) != people.end()) {
        std::cout << "Found Alice\n";
    }

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise: Word Frequency Counter

```cpp
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

int main()
{
    std::string text = "the quick brown fox jumps over the lazy dog the fox";
    std::unordered_map<std::string, int> wordCount;

    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        wordCount[word]++;
    }

    std::cout << "Word frequencies:\n";
    for (const auto& [w, count] : wordCount) {
        std::cout << w << ": " << count << "\n";
    }

    return 0;
}
```

### Project: Cache System

```cpp
#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

template<typename K, typename V>
class LRUCache {
private:
    int capacity;
    std::list<std::pair<K, V>> items;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    V get(const K& key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            throw std::runtime_error("Key not found");
        }

        // Move to front
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }

    void put(const K& key, const V& value) {
        auto it = cache.find(key);

        // If exists, remove old
        if (it != cache.end()) {
            items.erase(it->second);
            cache.erase(it);
        }

        // Add to front
        items.push_front({key, value});
        cache[key] = items.begin();

        // Evict if over capacity
        if (items.size() > (size_t)capacity) {
            auto last = items.back();
            cache.erase(last.first);
            items.pop_back();
        }
    }

    void display() const {
        std::cout << "Cache contents:\n";
        for (const auto& [k, v] : items) {
            std::cout << k << ": " << v << "\n";
        }
    }
};

int main()
{
    LRUCache<std::string, int> cache(3);

    cache.put("A", 1);
    cache.put("B", 2);
    cache.put("C", 3);
    cache.display();

    std::cout << "\nGet A: " << cache.get("A") << "\n\n";

    cache.put("D", 4);  // Evicts B
    cache.display();

    return 0;
}
```

---

## Key Takeaways

1. ✅ Unordered containers use hash tables for O(1) average operations
2. ✅ No guaranteed order (unlike ordered containers)
3. ✅ Faster for lookups when order doesn't matter
4. ✅ Custom hash functions needed for custom types
5. ✅ Must implement operator== for unordered containers
6. ✅ Load factor affects performance
7. ✅ Use when order doesn't matter and speed is critical
8. ✅ Collision handling via chaining (buckets)

---

## Summary Checklist

Before moving to Lesson 57, ensure you can:
- [ ] Use unordered_set and unordered_map
- [ ] Understand hash functions and collisions
- [ ] Compare ordered vs unordered performance
- [ ] Create custom hash functions
- [ ] Choose between ordered and unordered containers
- [ ] Implement real-world hash-based solutions

---

## Next Lesson Preview

In **Lesson 57**, we'll learn about:
- STL Algorithms Part 1
- Non-modifying algorithms
- Searching and counting
- Comparing and testing
- Algorithm complexity

**Great work on unordered containers!**
