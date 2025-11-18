# Lesson 54: Set and Multiset Containers

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Associative Containers
2. Set Container Basics
3. Set Operations and Methods
4. Multiset Container
5. Custom Comparators
6. Set Algorithms
7. Performance Characteristics
8. Exercises and Projects

---

## 1. Introduction to Associative Containers

**Associative containers** store elements in a sorted order and provide fast lookup based on keys.

### Types of Associative Containers:

```
Ordered (tree-based - typically Red-Black Tree):
├── set          - Unique keys, sorted
├── multiset     - Duplicate keys allowed, sorted
├── map          - Key-value pairs, unique keys, sorted
└── multimap     - Key-value pairs, duplicate keys, sorted

Unordered (hash-based):
├── unordered_set
├── unordered_multiset
├── unordered_map
└── unordered_multimap
```

### Set vs Vector:

```cpp
/*
Vector:
✅ Fast random access: O(1)
✅ Fast insertion at end: O(1)
❌ Slow search: O(n)
❌ Slow insertion in middle: O(n)
❌ Allows duplicates
❌ Not sorted (unless you sort manually)

Set:
✅ Fast search: O(log n)
✅ Fast insertion: O(log n)
✅ Always sorted automatically
✅ No duplicates
❌ No random access
❌ No indexing with []
*/
```

---

## 2. Set Container Basics

### What is a Set?

A **set** is an ordered container that stores unique elements. Elements are automatically sorted and duplicates are rejected.

### Creating Sets:

```cpp
#include <iostream>
#include <set>

int main()
{
    // Empty set
    std::set<int> s1;

    // Initialize with initializer list
    std::set<int> s2 = {5, 2, 8, 1, 9, 3};

    // Initialize with array
    int arr[] = {4, 7, 1, 9, 3};
    std::set<int> s3(arr, arr + 5);

    // Copy constructor
    std::set<int> s4(s2);

    // Note: Elements are automatically sorted!
    std::cout << "s2 elements (sorted): ";
    for (int x : s2) {
        std::cout << x << " ";
    }
    std::cout << "\n";  // Output: 1 2 3 5 8 9

    return 0;
}
```

### Set with Different Types:

```cpp
#include <iostream>
#include <set>
#include <string>

int main()
{
    // Set of strings
    std::set<std::string> names = {"Alice", "Bob", "Charlie", "David"};

    std::cout << "Names (alphabetically): ";
    for (const std::string& name : names) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    // Set of doubles
    std::set<double> prices = {19.99, 29.99, 9.99, 39.99};

    std::cout << "Prices (sorted): ";
    for (double price : prices) {
        std::cout << "$" << price << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 3. Set Operations and Methods

### Inserting Elements:

```cpp
#include <iostream>
#include <set>

int main()
{
    std::set<int> s;

    // insert() - single element
    auto result1 = s.insert(10);
    std::cout << "Inserted 10: " << (result1.second ? "Success" : "Failed") << "\n";

    // Try to insert duplicate
    auto result2 = s.insert(10);
    std::cout << "Inserted 10 again: " << (result2.second ? "Success" : "Failed") << "\n";

    // Insert multiple elements
    s.insert(20);
    s.insert(5);
    s.insert(15);
    s.insert(25);

    std::cout << "Set contents: ";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Insert with hint (for performance)
    auto it = s.begin();
    s.insert(it, 12);  // Hint: insert near beginning

    // Insert range
    int arr[] = {30, 35, 40};
    s.insert(arr, arr + 3);

    std::cout << "After range insert: ";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // emplace() - construct in place
    s.emplace(50);

    return 0;
}
```

**Output:**
```
Inserted 10: Success
Inserted 10 again: Failed
Set contents: 5 10 15 20 25
After range insert: 5 10 12 15 20 25 30 35 40
```

### Finding and Counting:

```cpp
#include <iostream>
#include <set>

int main()
{
    std::set<int> s = {10, 20, 30, 40, 50};

    // find() - returns iterator
    auto it = s.find(30);
    if (it != s.end()) {
        std::cout << "Found: " << *it << "\n";
    } else {
        std::cout << "Not found\n";
    }

    // Try to find non-existent element
    it = s.find(100);
    if (it != s.end()) {
        std::cout << "Found: " << *it << "\n";
    } else {
        std::cout << "100 not found\n";
    }

    // count() - returns 0 or 1 for set (0 or more for multiset)
    std::cout << "Count of 30: " << s.count(30) << "\n";
    std::cout << "Count of 100: " << s.count(100) << "\n";

    // contains() - C++20
    #ifdef __cpp_lib_containers_ranges
    std::cout << "Contains 30? " << (s.contains(30) ? "Yes" : "No") << "\n";
    #endif

    return 0;
}
```

### Erasing Elements:

```cpp
#include <iostream>
#include <set>

int main()
{
    std::set<int> s = {10, 20, 30, 40, 50, 60, 70};

    std::cout << "Original set: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // erase() by value
    s.erase(30);
    std::cout << "After erasing 30: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // erase() by iterator
    auto it = s.find(50);
    if (it != s.end()) {
        s.erase(it);
    }
    std::cout << "After erasing 50: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // erase() range
    it = s.find(20);
    auto it2 = s.find(60);
    s.erase(it, it2);  // Erase [20, 60)
    std::cout << "After erasing range: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // clear() - remove all
    s.clear();
    std::cout << "After clear, size: " << s.size() << "\n";

    return 0;
}
```

### Bounds and Ranges:

```cpp
#include <iostream>
#include <set>

int main()
{
    std::set<int> s = {10, 20, 30, 40, 50, 60, 70, 80, 90};

    // lower_bound() - first element >= value
    auto lb = s.lower_bound(35);
    std::cout << "Lower bound of 35: " << *lb << "\n";  // 40

    // upper_bound() - first element > value
    auto ub = s.upper_bound(50);
    std::cout << "Upper bound of 50: " << *ub << "\n";  // 60

    // equal_range() - returns pair of lower_bound and upper_bound
    auto range = s.equal_range(50);
    std::cout << "Equal range of 50: ["
              << *range.first << ", " << *range.second << ")\n";

    // Print range [30, 70)
    std::cout << "Elements in [30, 70): ";
    auto start = s.lower_bound(30);
    auto end = s.lower_bound(70);
    for (auto it = start; it != end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 4. Multiset Container

### What is a Multiset?

A **multiset** is like a set but allows duplicate elements. Elements are still automatically sorted.

### Multiset Operations:

```cpp
#include <iostream>
#include <set>

int main()
{
    // Multiset allows duplicates
    std::multiset<int> ms = {5, 2, 8, 2, 9, 5, 3, 8, 5};

    std::cout << "Multiset contents: ";
    for (int x : ms) {
        std::cout << x << " ";
    }
    std::cout << "\n";  // Output: 2 2 3 5 5 5 8 8 9

    // count() - returns number of occurrences
    std::cout << "Count of 5: " << ms.count(5) << "\n";  // 3
    std::cout << "Count of 8: " << ms.count(8) << "\n";  // 2

    // insert() - always succeeds (unlike set)
    ms.insert(5);
    std::cout << "After inserting another 5, count: " << ms.count(5) << "\n";  // 4

    // erase() by value - removes ALL occurrences
    ms.erase(5);
    std::cout << "After erasing 5 (all): ";
    for (int x : ms) std::cout << x << " ";
    std::cout << "\n";

    // To erase just one occurrence, use iterator
    ms = {1, 2, 2, 3, 3, 3, 4};
    auto it = ms.find(3);
    if (it != ms.end()) {
        ms.erase(it);  // Erase just one 3
    }
    std::cout << "After erasing one 3: ";
    for (int x : ms) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Finding All Duplicates:

```cpp
#include <iostream>
#include <set>

int main()
{
    std::multiset<int> ms = {1, 2, 2, 2, 3, 4, 4, 5};

    // Find all occurrences of 2
    std::cout << "All occurrences of 2: ";
    auto range = ms.equal_range(2);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Frequency count
    std::cout << "\nFrequency count:\n";
    for (int val = 1; val <= 5; val++) {
        int count = ms.count(val);
        if (count > 0) {
            std::cout << val << " appears " << count << " time(s)\n";
        }
    }

    return 0;
}
```

---

## 5. Custom Comparators

### Set with Custom Comparator:

```cpp
#include <iostream>
#include <set>
#include <string>

// Comparator for descending order
struct DescendingOrder {
    bool operator()(int a, int b) const {
        return a > b;  // Reverse comparison
    }
};

int main()
{
    // Default: ascending order
    std::set<int> s1 = {5, 2, 8, 1, 9};
    std::cout << "Ascending: ";
    for (int x : s1) std::cout << x << " ";
    std::cout << "\n";

    // Custom: descending order
    std::set<int, DescendingOrder> s2 = {5, 2, 8, 1, 9};
    std::cout << "Descending: ";
    for (int x : s2) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Set of Custom Objects:

```cpp
#include <iostream>
#include <set>
#include <string>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(n), age(a) {}

    // Required for set: comparison operator
    bool operator<(const Person& other) const {
        return age < other.age;  // Sort by age
    }
};

int main()
{
    std::set<Person> people;

    people.emplace("Alice", 30);
    people.emplace("Bob", 25);
    people.emplace("Charlie", 35);
    people.emplace("David", 28);

    std::cout << "People sorted by age:\n";
    for (const auto& person : people) {
        std::cout << person.name << " (" << person.age << ")\n";
    }

    return 0;
}
```

### Lambda Comparator:

```cpp
#include <iostream>
#include <set>
#include <string>

int main()
{
    // Lambda comparator for string length
    auto compareLength = [](const std::string& a, const std::string& b) {
        if (a.length() != b.length()) {
            return a.length() < b.length();
        }
        return a < b;  // Alphabetical if same length
    };

    std::set<std::string, decltype(compareLength)> words(compareLength);

    words.insert("apple");
    words.insert("banana");
    words.insert("cat");
    words.insert("dog");
    words.insert("elephant");
    words.insert("bee");

    std::cout << "Words sorted by length:\n";
    for (const std::string& word : words) {
        std::cout << word << " (" << word.length() << ")\n";
    }

    return 0;
}
```

---

## 6. Set Algorithms

### Union of Sets:

```cpp
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

int main()
{
    std::set<int> s1 = {1, 2, 3, 4, 5};
    std::set<int> s2 = {4, 5, 6, 7, 8};

    std::set<int> result;

    // set_union
    std::set_union(s1.begin(), s1.end(),
                   s2.begin(), s2.end(),
                   std::inserter(result, result.begin()));

    std::cout << "Union: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";  // Output: 1 2 3 4 5 6 7 8

    return 0;
}
```

### Intersection and Difference:

```cpp
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

int main()
{
    std::set<int> s1 = {1, 2, 3, 4, 5, 6};
    std::set<int> s2 = {4, 5, 6, 7, 8, 9};

    std::set<int> intersection, difference, symDiff;

    // Intersection (common elements)
    std::set_intersection(s1.begin(), s1.end(),
                          s2.begin(), s2.end(),
                          std::inserter(intersection, intersection.begin()));

    std::cout << "Intersection: ";
    for (int x : intersection) std::cout << x << " ";
    std::cout << "\n";  // Output: 4 5 6

    // Difference (in s1 but not in s2)
    std::set_difference(s1.begin(), s1.end(),
                        s2.begin(), s2.end(),
                        std::inserter(difference, difference.begin()));

    std::cout << "Difference (s1 - s2): ";
    for (int x : difference) std::cout << x << " ";
    std::cout << "\n";  // Output: 1 2 3

    // Symmetric difference (in one but not both)
    std::set_symmetric_difference(s1.begin(), s1.end(),
                                  s2.begin(), s2.end(),
                                  std::inserter(symDiff, symDiff.begin()));

    std::cout << "Symmetric difference: ";
    for (int x : symDiff) std::cout << x << " ";
    std::cout << "\n";  // Output: 1 2 3 7 8 9

    return 0;
}
```

### Subset Check:

```cpp
#include <iostream>
#include <set>
#include <algorithm>

int main()
{
    std::set<int> s1 = {1, 2, 3, 4, 5};
    std::set<int> s2 = {2, 3, 4};
    std::set<int> s3 = {2, 3, 6};

    // Check if s2 is subset of s1
    bool isSubset = std::includes(s1.begin(), s1.end(),
                                  s2.begin(), s2.end());
    std::cout << "s2 is subset of s1? " << (isSubset ? "Yes" : "No") << "\n";

    // Check if s3 is subset of s1
    isSubset = std::includes(s1.begin(), s1.end(),
                             s3.begin(), s3.end());
    std::cout << "s3 is subset of s1? " << (isSubset ? "Yes" : "No") << "\n";

    return 0;
}
```

---

## 7. Performance Characteristics

### Time Complexity:

```cpp
/*
Operation         Set/Multiset    Vector
=========================================
Insert            O(log n)        O(n)
Delete            O(log n)        O(n)
Search            O(log n)        O(n)
Access by index   Not supported   O(1)
Min/Max element   O(1)*           O(n)

* Using begin() or rbegin()

Space Complexity: O(n) for both
*/
```

### Performance Benchmark:

```cpp
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>

int main()
{
    const int N = 100000;

    // Set insertion
    auto start = std::chrono::high_resolution_clock::now();
    std::set<int> s;
    for (int i = 0; i < N; i++) {
        s.insert(rand());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto setTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Vector insertion and sort
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> v;
    for (int i = 0; i < N; i++) {
        v.push_back(rand());
    }
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());  // Remove duplicates
    end = std::chrono::high_resolution_clock::now();
    auto vecTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Set (insert & auto-sort): " << setTime.count() << " ms\n";
    std::cout << "Vector (insert, sort, unique): " << vecTime.count() << " ms\n";

    // Search performance
    int target = s.size() > 0 ? *s.begin() : 0;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        s.find(target);
    }
    end = std::chrono::high_resolution_clock::now();
    auto setSearchTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        std::find(v.begin(), v.end(), target);
    }
    end = std::chrono::high_resolution_clock::now();
    auto vecSearchTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "\nSet search (10000 ops): " << setSearchTime.count() << " μs\n";
    std::cout << "Vector search (10000 ops): " << vecSearchTime.count() << " μs\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Remove Duplicates

```cpp
#include <iostream>
#include <vector>
#include <set>

std::vector<int> removeDuplicates(const std::vector<int>& vec)
{
    std::set<int> uniqueSet(vec.begin(), vec.end());
    return std::vector<int>(uniqueSet.begin(), uniqueSet.end());
}

int main()
{
    std::vector<int> numbers = {5, 2, 8, 2, 9, 5, 3, 8, 5, 1};

    std::cout << "Original: ";
    for (int x : numbers) std::cout << x << " ";
    std::cout << "\n";

    std::vector<int> unique = removeDuplicates(numbers);

    std::cout << "Without duplicates (sorted): ";
    for (int x : unique) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Exercise 2: Word Frequency Counter

```cpp
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>

int main()
{
    std::string text = "the quick brown fox jumps over the lazy dog the fox is quick";

    // Split into words
    std::multiset<std::string> words;
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        words.insert(word);
    }

    // Get unique words
    std::set<std::string> uniqueWords(words.begin(), words.end());

    std::cout << "Word frequencies:\n";
    std::cout << "=================\n";
    for (const std::string& w : uniqueWords) {
        std::cout << w << ": " << words.count(w) << "\n";
    }

    std::cout << "\nTotal words: " << words.size() << "\n";
    std::cout << "Unique words: " << uniqueWords.size() << "\n";

    return 0;
}
```

### Project: Student Management System

```cpp
#include <iostream>
#include <set>
#include <string>

struct Student {
    int id;
    std::string name;
    double gpa;

    Student(int i, std::string n, double g) : id(i), name(n), gpa(g) {}

    bool operator<(const Student& other) const {
        return id < other.id;  // Sort by ID
    }

    void display() const {
        std::cout << "ID: " << id << ", Name: " << name << ", GPA: " << gpa << "\n";
    }
};

class StudentDatabase {
private:
    std::set<Student> students;

public:
    void addStudent(int id, const std::string& name, double gpa) {
        auto result = students.emplace(id, name, gpa);
        if (result.second) {
            std::cout << "Added student: " << name << "\n";
        } else {
            std::cout << "Student with ID " << id << " already exists\n";
        }
    }

    void removeStudent(int id) {
        Student temp(id, "", 0.0);
        auto it = students.find(temp);
        if (it != students.end()) {
            std::cout << "Removed student: " << it->name << "\n";
            students.erase(it);
        } else {
            std::cout << "Student with ID " << id << " not found\n";
        }
    }

    void findStudent(int id) const {
        Student temp(id, "", 0.0);
        auto it = students.find(temp);
        if (it != students.end()) {
            std::cout << "Found: ";
            it->display();
        } else {
            std::cout << "Student with ID " << id << " not found\n";
        }
    }

    void displayAll() const {
        std::cout << "\n=== All Students ===\n";
        for (const auto& student : students) {
            student.display();
        }
        std::cout << "Total students: " << students.size() << "\n\n";
    }

    void displayByGPA(double minGPA) const {
        std::cout << "\nStudents with GPA >= " << minGPA << ":\n";
        for (const auto& student : students) {
            if (student.gpa >= minGPA) {
                student.display();
            }
        }
    }
};

int main()
{
    StudentDatabase db;

    db.addStudent(1001, "Alice Johnson", 3.8);
    db.addStudent(1002, "Bob Smith", 3.2);
    db.addStudent(1003, "Charlie Brown", 3.9);
    db.addStudent(1004, "David Lee", 2.8);
    db.addStudent(1005, "Eve Davis", 3.5);

    db.displayAll();

    db.findStudent(1003);
    db.findStudent(9999);

    db.displayByGPA(3.5);

    db.removeStudent(1002);
    db.displayAll();

    // Try to add duplicate
    db.addStudent(1001, "Alice Duplicate", 4.0);

    return 0;
}
```

### Project: Unique Visitors Tracker

```cpp
#include <iostream>
#include <set>
#include <string>
#include <ctime>

struct Visitor {
    std::string ipAddress;
    time_t timestamp;

    Visitor(std::string ip) : ipAddress(ip) {
        timestamp = std::time(nullptr);
    }

    bool operator<(const Visitor& other) const {
        return ipAddress < other.ipAddress;
    }
};

class WebsiteAnalytics {
private:
    std::set<Visitor> uniqueVisitors;
    std::multiset<std::string> allVisits;

public:
    void recordVisit(const std::string& ipAddress) {
        uniqueVisitors.emplace(ipAddress);
        allVisits.insert(ipAddress);
    }

    void displayStats() const {
        std::cout << "\n=== Website Analytics ===\n";
        std::cout << "Total visits: " << allVisits.size() << "\n";
        std::cout << "Unique visitors: " << uniqueVisitors.size() << "\n";

        // Find most frequent visitor
        std::string mostFrequent;
        int maxCount = 0;
        for (const auto& visitor : uniqueVisitors) {
            int count = allVisits.count(visitor.ipAddress);
            if (count > maxCount) {
                maxCount = count;
                mostFrequent = visitor.ipAddress;
            }
        }

        std::cout << "Most frequent visitor: " << mostFrequent
                  << " (" << maxCount << " visits)\n";
    }

    void displayUniqueVisitors() const {
        std::cout << "\n=== Unique Visitors ===\n";
        for (const auto& visitor : uniqueVisitors) {
            std::cout << visitor.ipAddress
                      << " (Visits: " << allVisits.count(visitor.ipAddress) << ")\n";
        }
    }
};

int main()
{
    WebsiteAnalytics analytics;

    // Simulate visits
    analytics.recordVisit("192.168.1.100");
    analytics.recordVisit("192.168.1.101");
    analytics.recordVisit("192.168.1.100");  // Repeat visitor
    analytics.recordVisit("192.168.1.102");
    analytics.recordVisit("192.168.1.100");  // Repeat visitor
    analytics.recordVisit("192.168.1.103");
    analytics.recordVisit("192.168.1.101");  // Repeat visitor

    analytics.displayStats();
    analytics.displayUniqueVisitors();

    return 0;
}
```

---

## Key Takeaways

1. ✅ Set stores unique elements in sorted order automatically
2. ✅ Multiset allows duplicates but maintains sorted order
3. ✅ Both provide O(log n) insert, delete, and search
4. ✅ No random access or indexing with []
5. ✅ Use custom comparators to change sorting order
6. ✅ Set algorithms: union, intersection, difference
7. ✅ Ideal for maintaining unique sorted collections
8. ✅ Use multiset when you need duplicates with sorted order
9. ✅ Insert returns pair<iterator, bool> for set
10. ✅ Always prefer set over vector for frequent searches

---

## Summary Checklist

Before moving to Lesson 55, ensure you can:
- [ ] Create and initialize sets and multisets
- [ ] Insert and remove elements
- [ ] Search for elements with find() and count()
- [ ] Use lower_bound, upper_bound, and equal_range
- [ ] Understand differences between set and multiset
- [ ] Create custom comparators for custom sorting
- [ ] Apply set algorithms (union, intersection, difference)
- [ ] Choose between set and vector based on requirements
- [ ] Work with sets of custom objects
- [ ] Build real-world applications using sets

---

## Next Lesson Preview

In **Lesson 55**, we'll learn about:
- Map and multimap containers
- Key-value pair associations
- Accessing and modifying mapped values
- Map algorithms and operations
- When to use maps vs other containers

**Excellent work mastering sets!**
