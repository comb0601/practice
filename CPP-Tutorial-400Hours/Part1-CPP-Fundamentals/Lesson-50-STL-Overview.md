# Lesson 50: STL Overview

**Duration**: 10 hours
**Difficulty**: Intermediate

## Table of Contents
1. What is STL?
2. STL Components
3. Containers Overview
4. Iterators
5. Algorithms
6. Common Containers
7. Common Algorithms
8. Exercises
9. Practice Projects

---

## 1. What is STL?

**Standard Template Library (STL)** is a collection of template classes and functions.

### Components:
1. **Containers**: Store data
2. **Iterators**: Access elements
3. **Algorithms**: Operate on data
4. **Function Objects**: Functors

---

## 2. STL Components

```
STL Architecture:

Algorithms ←→ Iterators ←→ Containers
              ↓
         Allocators
```

---

## 3. Containers Overview

### Sequence Containers:
- **vector**: Dynamic array
- **deque**: Double-ended queue
- **list**: Doubly-linked list
- **array**: Fixed-size array
- **forward_list**: Singly-linked list

### Associative Containers:
- **set**: Unique sorted elements
- **map**: Key-value pairs
- **multiset**: Sorted elements (duplicates allowed)
- **multimap**: Multiple values per key

### Container Adaptors:
- **stack**: LIFO
- **queue**: FIFO
- **priority_queue**: Priority-based

---

## 4. Vector Example

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    
    // Add elements
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // Access elements
    cout << "First: " << v[0] << endl;
    cout << "Size: " << v.size() << endl;
    
    // Iterate
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    
    // Iterator
    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    return 0;
}
```

---

## 5. Map Example

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> ages;
    
    // Insert
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages.insert({"Charlie", 35});
    
    // Access
    cout << "Alice's age: " << ages["Alice"] << endl;
    
    // Iterate
    for (const auto& pair : ages) {
        cout << pair.first << ": " << pair.second << endl;
    }
    
    // Find
    if (ages.find("Bob") != ages.end()) {
        cout << "Bob found!" << endl;
    }
    
    return 0;
}
```

---

## 6. Set Example

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s;
    
    // Insert
    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);  // Duplicate - ignored
    
    // Automatically sorted
    for (int x : s) {
        cout << x << " ";  // 10 20 30
    }
    cout << endl;
    
    // Check existence
    if (s.count(20)) {
        cout << "20 exists" << endl;
    }
    
    return 0;
}
```

---

## 7. Common Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v = {5, 2, 8, 1, 9, 3};
    
    // Sort
    sort(v.begin(), v.end());
    for (int x : v) cout << x << " ";
    cout << endl;
    
    // Find
    auto it = find(v.begin(), v.end(), 8);
    if (it != v.end()) {
        cout << "Found 8" << endl;
    }
    
    // Count
    int count = count_if(v.begin(), v.end(), [](int x) { return x > 5; });
    cout << "Elements > 5: " << count << endl;
    
    // Max element
    auto maxIt = max_element(v.begin(), v.end());
    cout << "Max: " << *maxIt << endl;
    
    // Reverse
    reverse(v.begin(), v.end());
    for (int x : v) cout << x << " ";
    cout << endl;
    
    return 0;
}
```

---

## 8. Complete Example: Student Management

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
using namespace std;

struct Student {
    int id;
    string name;
    double gpa;
    
    Student(int i, string n, double g) : id(i), name(n), gpa(g) {}
};

int main() {
    vector<Student> students;
    
    students.push_back(Student(1, "Alice", 3.8));
    students.push_back(Student(2, "Bob", 3.5));
    students.push_back(Student(3, "Charlie", 3.9));
    
    // Sort by GPA
    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b) {
             return a.gpa > b.gpa;
         });
    
    cout << "Students sorted by GPA:" << endl;
    for (const auto& s : students) {
        cout << s.name << ": " << s.gpa << endl;
    }
    
    // Find student
    auto it = find_if(students.begin(), students.end(),
                      [](const Student& s) { return s.name == "Bob"; });
    
    if (it != students.end()) {
        cout << "\nFound Bob - GPA: " << it->gpa << endl;
    }
    
    // Average GPA
    double total = 0;
    for (const auto& s : students) {
        total += s.gpa;
    }
    cout << "Average GPA: " << total / students.size() << endl;
    
    return 0;
}
```

---

## 9. STL Best Practices

1. **Use appropriate container** for your needs
2. **Prefer algorithms** over loops
3. **Use iterators** for flexibility
4. **const correctness**: `const auto&` in range loops
5. **Reserve capacity**: `vector::reserve()` for performance
6. **Understand complexity**: Know Big-O for operations

---

## Key Takeaways

1. **STL**: Containers + Iterators + Algorithms
2. **vector**: Most common container
3. **map/set**: Associative containers
4. **Iterators**: Connect containers and algorithms
5. **Algorithms**: sort, find, count, etc.
6. **Type-safe**: Templates provide safety
7. **Efficient**: Optimized implementations
8. **Standard**: Portable across compilers

---

## Summary: Course Completion

**Congratulations!** You've completed Lessons 31-50 covering:

### Object-Oriented Programming (31-40):
- Classes and Objects
- Constructors and Destructors
- Member Functions
- Access Specifiers
- Encapsulation
- Static Members
- Friend Functions
- Operator Overloading
- Copy/Move Semantics

### Inheritance and Polymorphism (41-45):
- Inheritance Basics
- Types of Inheritance
- Multiple Inheritance
- Polymorphism
- Abstract Classes

### Templates and STL (46-50):
- Function Templates
- Class Templates
- Template Specialization
- Variadic Templates
- Standard Template Library

**Next Steps**:
- Practice with real projects
- Explore advanced C++ features
- Study design patterns
- Contribute to open source
- Build your portfolio

**Excellent work! You're now ready for advanced C++ development!**
