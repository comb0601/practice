# Lesson 52: List and Deque Containers

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to List and Deque
2. List Container (Doubly-Linked List)
3. List Operations and Methods
4. Deque Container (Double-Ended Queue)
5. Deque Operations and Methods
6. Performance Comparison
7. When to Use Each Container
8. Advanced Operations
9. Exercises and Projects

---

## 1. Introduction to List and Deque

The STL provides different sequential containers for different use cases. While **vector** is great for random access, **list** and **deque** excel in different scenarios.

### Container Overview:

```
Vector:
- Contiguous memory
- Fast random access: O(1)
- Slow insert/delete in middle: O(n)
- Fast push_back: O(1)

List:
- Non-contiguous memory (linked nodes)
- No random access: O(n)
- Fast insert/delete anywhere: O(1)
- Bidirectional iteration

Deque:
- Multiple memory chunks
- Fast random access: O(1)
- Fast insert/delete at ends: O(1)
- Slower insert/delete in middle: O(n)
```

---

## 2. List Container (Doubly-Linked List)

### What is a List?

A `std::list` is a doubly-linked list where each element points to both the next and previous elements.

**Structure:**
```
[prev|data|next] <-> [prev|data|next] <-> [prev|data|next]
```

### Creating Lists:

```cpp
#include <iostream>
#include <list>

int main()
{
    // Empty list
    std::list<int> list1;

    // List with 5 elements initialized to 0
    std::list<int> list2(5);

    // List with 5 elements initialized to 10
    std::list<int> list3(5, 10);

    // Initializer list
    std::list<int> list4 = {1, 2, 3, 4, 5};

    // Copy constructor
    std::list<int> list5(list4);

    std::cout << "list3 elements: ";
    for (int x : list3) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "list4 elements: ";
    for (int x : list4) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 3. List Operations and Methods

### Adding Elements:

```cpp
#include <iostream>
#include <list>

int main()
{
    std::list<int> lst;

    // push_back() - add at end
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    // push_front() - add at beginning
    lst.push_front(5);
    lst.push_front(1);

    std::cout << "After push operations: ";
    for (int x : lst) {
        std::cout << x << " ";
    }
    std::cout << "\n";  // Output: 1 5 10 20 30

    // insert() - insert at position
    auto it = lst.begin();
    ++it; ++it;  // Move to third position
    lst.insert(it, 7);

    std::cout << "After insert: ";
    for (int x : lst) {
        std::cout << x << " ";
    }
    std::cout << "\n";  // Output: 1 5 7 10 20 30

    // emplace_back and emplace_front
    lst.emplace_back(40);
    lst.emplace_front(0);

    std::cout << "After emplace: ";
    for (int x : lst) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Removing Elements:

```cpp
#include <iostream>
#include <list>

int main()
{
    std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // pop_back() - remove last
    lst.pop_back();
    std::cout << "After pop_back: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // pop_front() - remove first
    lst.pop_front();
    std::cout << "After pop_front: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // erase() - remove at position
    auto it = lst.begin();
    ++it;  // Second element
    lst.erase(it);
    std::cout << "After erase: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // remove() - remove all occurrences of value
    lst = {1, 2, 3, 2, 4, 2, 5};
    lst.remove(2);
    std::cout << "After remove(2): ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // remove_if() - remove based on condition
    lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    lst.remove_if([](int x) { return x % 2 == 0; });  // Remove even numbers
    std::cout << "After remove_if (even): ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### List-Specific Operations:

```cpp
#include <iostream>
#include <list>

int main()
{
    std::list<int> list1 = {1, 3, 5, 7, 9};
    std::list<int> list2 = {2, 4, 6, 8, 10};

    // merge() - merge two sorted lists
    list1.merge(list2);
    std::cout << "After merge: ";
    for (int x : list1) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "list2 size: " << list2.size() << "\n";  // Now empty

    // splice() - transfer elements from another list
    list1 = {1, 2, 3};
    list2 = {10, 20, 30};
    auto it = list1.begin();
    ++it;  // Second position
    list1.splice(it, list2);  // Insert all of list2 at position
    std::cout << "After splice: ";
    for (int x : list1) std::cout << x << " ";
    std::cout << "\n";

    // reverse()
    list1.reverse();
    std::cout << "After reverse: ";
    for (int x : list1) std::cout << x << " ";
    std::cout << "\n";

    // sort()
    list1.sort();
    std::cout << "After sort: ";
    for (int x : list1) std::cout << x << " ";
    std::cout << "\n";

    // unique() - remove consecutive duplicates
    list1 = {1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
    list1.unique();
    std::cout << "After unique: ";
    for (int x : list1) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Accessing Elements:

```cpp
#include <iostream>
#include <list>

int main()
{
    std::list<int> lst = {10, 20, 30, 40, 50};

    // front() - first element
    std::cout << "First: " << lst.front() << "\n";

    // back() - last element
    std::cout << "Last: " << lst.back() << "\n";

    // Note: No operator[] or at() for lists!
    // Must use iterators for middle elements

    // Accessing middle elements
    auto it = lst.begin();
    std::advance(it, 2);  // Move iterator forward 2 positions
    std::cout << "Third element: " << *it << "\n";

    return 0;
}
```

---

## 4. Deque Container (Double-Ended Queue)

### What is a Deque?

A `std::deque` (pronounced "deck") is a double-ended queue that allows fast insertion and deletion at both ends, plus random access.

**Advantages:**
- Fast insert/delete at both ends: O(1)
- Random access: O(1)
- No memory reallocation like vector

**Disadvantages:**
- Slightly slower than vector for random access
- More memory overhead than vector

### Creating Deques:

```cpp
#include <iostream>
#include <deque>

int main()
{
    // Empty deque
    std::deque<int> dq1;

    // Deque with 5 elements initialized to 0
    std::deque<int> dq2(5);

    // Deque with 5 elements initialized to 10
    std::deque<int> dq3(5, 10);

    // Initializer list
    std::deque<int> dq4 = {1, 2, 3, 4, 5};

    // Copy constructor
    std::deque<int> dq5(dq4);

    std::cout << "dq3 elements: ";
    for (int x : dq3) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "dq4 elements: ";
    for (int x : dq4) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 5. Deque Operations and Methods

### Adding and Removing Elements:

```cpp
#include <iostream>
#include <deque>

int main()
{
    std::deque<int> dq;

    // push_back() - add at end
    dq.push_back(30);
    dq.push_back(40);
    dq.push_back(50);

    // push_front() - add at beginning
    dq.push_front(20);
    dq.push_front(10);

    std::cout << "After push operations: ";
    for (int x : dq) {
        std::cout << x << " ";
    }
    std::cout << "\n";  // Output: 10 20 30 40 50

    // pop_back() - remove last
    dq.pop_back();
    std::cout << "After pop_back: ";
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n";

    // pop_front() - remove first
    dq.pop_front();
    std::cout << "After pop_front: ";
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n";

    // insert() - insert at position
    auto it = dq.begin() + 1;
    dq.insert(it, 25);
    std::cout << "After insert: ";
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Random Access:

```cpp
#include <iostream>
#include <deque>

int main()
{
    std::deque<int> dq = {10, 20, 30, 40, 50};

    // operator[]
    std::cout << "dq[0]: " << dq[0] << "\n";
    std::cout << "dq[2]: " << dq[2] << "\n";

    // at() - with bounds checking
    std::cout << "dq.at(1): " << dq.at(1) << "\n";

    // front() and back()
    std::cout << "First: " << dq.front() << "\n";
    std::cout << "Last: " << dq.back() << "\n";

    // Modifying elements
    dq[0] = 100;
    dq.at(1) = 200;

    std::cout << "After modification: ";
    for (int x : dq) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Exception handling with at()
    try {
        std::cout << dq.at(100);  // Out of bounds
    }
    catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

### Deque as a Queue:

```cpp
#include <iostream>
#include <deque>
#include <string>

// Simple queue implementation using deque
class Queue {
private:
    std::deque<std::string> data;

public:
    void enqueue(const std::string& item) {
        data.push_back(item);
    }

    std::string dequeue() {
        if (data.empty()) {
            return "";
        }
        std::string front = data.front();
        data.pop_front();
        return front;
    }

    bool isEmpty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

int main()
{
    Queue q;

    q.enqueue("Task 1");
    q.enqueue("Task 2");
    q.enqueue("Task 3");

    std::cout << "Queue size: " << q.size() << "\n";

    while (!q.isEmpty()) {
        std::cout << "Processing: " << q.dequeue() << "\n";
    }

    return 0;
}
```

---

## 6. Performance Comparison

### Benchmark Test:

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <chrono>

template<typename Container>
void testPushBack(const std::string& name)
{
    auto start = std::chrono::high_resolution_clock::now();

    Container c;
    for (int i = 0; i < 1000000; i++) {
        c.push_back(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << name << " push_back: " << duration.count() << " ms\n";
}

template<typename Container>
void testPushFront(const std::string& name)
{
    auto start = std::chrono::high_resolution_clock::now();

    Container c;
    for (int i = 0; i < 100000; i++) {  // Smaller for vector
        c.push_front(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << name << " push_front: " << duration.count() << " ms\n";
}

int main()
{
    std::cout << "Performance Comparison:\n";
    std::cout << "======================\n\n";

    testPushBack<std::vector<int>>("Vector");
    testPushBack<std::list<int>>("List");
    testPushBack<std::deque<int>>("Deque");

    std::cout << "\n";

    testPushFront<std::list<int>>("List");
    testPushFront<std::deque<int>>("Deque");

    return 0;
}
```

### Random Access Comparison:

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <chrono>

int main()
{
    const int SIZE = 1000000;

    std::vector<int> vec(SIZE);
    std::deque<int> dq(SIZE);
    std::list<int> lst(SIZE);

    // Vector random access
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        int index = rand() % SIZE;
        vec[index] = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto vecTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Deque random access
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        int index = rand() % SIZE;
        dq[index] = i;
    }
    end = std::chrono::high_resolution_clock::now();
    auto dqTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Random Access Performance:\n";
    std::cout << "Vector: " << vecTime.count() << " μs\n";
    std::cout << "Deque:  " << dqTime.count() << " μs\n";
    std::cout << "\nNote: List doesn't support random access\n";

    return 0;
}
```

---

## 7. When to Use Each Container

### Decision Guide:

```cpp
/*
USE VECTOR when:
✅ Need random access frequently
✅ Mostly adding/removing at end
✅ Want cache-friendly contiguous memory
✅ Size is relatively stable
Example: Array of game objects, particle systems

USE LIST when:
✅ Frequent insertions/deletions in middle
✅ Don't need random access
✅ Maintaining sorted order with frequent updates
✅ Need iterator stability (iterators don't invalidate)
Example: Undo/redo systems, playlist management

USE DEQUE when:
✅ Need random access
✅ Frequent insertions/deletions at both ends
✅ Implementing queues or double-ended queues
✅ Want vector-like interface without reallocation
Example: Task queues, sliding window algorithms
*/
```

### Practical Example - Choosing Container:

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>

// Scenario 1: High-score list (frequent sorting and insertion)
class HighScoreList {
private:
    std::list<int> scores;  // List for easy insertion

public:
    void addScore(int score) {
        scores.push_back(score);
        scores.sort(std::greater<int>());  // Sort descending
        if (scores.size() > 10) {
            scores.pop_back();  // Keep only top 10
        }
    }

    void display() const {
        std::cout << "Top Scores: ";
        for (int score : scores) {
            std::cout << score << " ";
        }
        std::cout << "\n";
    }
};

// Scenario 2: Print job queue (FIFO)
class PrintQueue {
private:
    std::deque<std::string> jobs;  // Deque for efficient queue operations

public:
    void addJob(const std::string& job) {
        jobs.push_back(job);
    }

    void processJob() {
        if (!jobs.empty()) {
            std::cout << "Processing: " << jobs.front() << "\n";
            jobs.pop_front();
        }
    }

    bool hasJobs() const {
        return !jobs.empty();
    }
};

// Scenario 3: Game objects (frequent random access)
class GameObject {
public:
    int id;
    std::string name;
    GameObject(int i, std::string n) : id(i), name(n) {}
};

class GameWorld {
private:
    std::vector<GameObject> objects;  // Vector for fast access

public:
    void addObject(int id, const std::string& name) {
        objects.emplace_back(id, name);
    }

    GameObject& getObject(size_t index) {
        return objects[index];
    }

    void update() {
        for (auto& obj : objects) {
            // Fast iteration through contiguous memory
            std::cout << "Updating " << obj.name << "\n";
        }
    }
};

int main()
{
    // Test high score list
    HighScoreList scores;
    scores.addScore(100);
    scores.addScore(500);
    scores.addScore(250);
    scores.addScore(750);
    scores.display();

    std::cout << "\n";

    // Test print queue
    PrintQueue printer;
    printer.addJob("Document1.pdf");
    printer.addJob("Photo.jpg");
    printer.addJob("Report.docx");

    while (printer.hasJobs()) {
        printer.processJob();
    }

    return 0;
}
```

---

## 8. Advanced Operations

### List Sorting with Custom Comparator:

```cpp
#include <iostream>
#include <list>
#include <string>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(n), age(a) {}
};

bool compareByAge(const Person& a, const Person& b)
{
    return a.age < b.age;
}

int main()
{
    std::list<Person> people = {
        Person("Alice", 30),
        Person("Bob", 25),
        Person("Charlie", 35),
        Person("David", 28)
    };

    // Sort by age
    people.sort(compareByAge);

    std::cout << "Sorted by age:\n";
    for (const auto& person : people) {
        std::cout << person.name << " (" << person.age << ")\n";
    }

    // Sort by name using lambda
    people.sort([](const Person& a, const Person& b) {
        return a.name < b.name;
    });

    std::cout << "\nSorted by name:\n";
    for (const auto& person : people) {
        std::cout << person.name << " (" << person.age << ")\n";
    }

    return 0;
}
```

### Deque for Sliding Window:

```cpp
#include <iostream>
#include <deque>
#include <vector>

// Find maximum in each sliding window of size k
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k)
{
    std::vector<int> result;
    std::deque<int> dq;  // Store indices

    for (size_t i = 0; i < nums.size(); i++) {
        // Remove elements outside current window
        while (!dq.empty() && dq.front() <= (int)i - k) {
            dq.pop_front();
        }

        // Remove smaller elements (they won't be max)
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }

        dq.push_back(i);

        // Add to result if window is complete
        if (i >= (size_t)k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }

    return result;
}

int main()
{
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    std::vector<int> result = maxSlidingWindow(nums, k);

    std::cout << "Array: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Max in each window of size " << k << ": ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 9. Exercises and Projects

### Exercise 1: Browser History

```cpp
#include <iostream>
#include <list>
#include <string>

class BrowserHistory {
private:
    std::list<std::string> history;
    std::list<std::string>::iterator current;

public:
    BrowserHistory() {
        history.push_back("home");
        current = history.begin();
    }

    void visit(const std::string& url) {
        // Erase forward history
        auto next = current;
        ++next;
        history.erase(next, history.end());

        // Add new page
        history.push_back(url);
        ++current;
    }

    std::string back() {
        if (current != history.begin()) {
            --current;
        }
        return *current;
    }

    std::string forward() {
        auto next = current;
        ++next;
        if (next != history.end()) {
            current = next;
        }
        return *current;
    }

    void display() const {
        std::cout << "History: ";
        for (auto it = history.begin(); it != history.end(); ++it) {
            if (it == current) {
                std::cout << "[" << *it << "] ";
            } else {
                std::cout << *it << " ";
            }
        }
        std::cout << "\n";
    }
};

int main()
{
    BrowserHistory browser;

    browser.visit("google.com");
    browser.visit("youtube.com");
    browser.visit("github.com");
    browser.display();

    std::cout << "Back to: " << browser.back() << "\n";
    browser.display();

    std::cout << "Back to: " << browser.back() << "\n";
    browser.display();

    std::cout << "Forward to: " << browser.forward() << "\n";
    browser.display();

    browser.visit("stackoverflow.com");
    browser.display();

    return 0;
}
```

### Exercise 2: Task Scheduler

```cpp
#include <iostream>
#include <deque>
#include <string>

struct Task {
    std::string name;
    int priority;  // 1 = high, 2 = medium, 3 = low

    Task(std::string n, int p) : name(n), priority(p) {}
};

class TaskScheduler {
private:
    std::deque<Task> highPriority;
    std::deque<Task> mediumPriority;
    std::deque<Task> lowPriority;

public:
    void addTask(const std::string& name, int priority) {
        if (priority == 1) {
            highPriority.emplace_back(name, priority);
        } else if (priority == 2) {
            mediumPriority.emplace_back(name, priority);
        } else {
            lowPriority.emplace_back(name, priority);
        }
    }

    void processNext() {
        if (!highPriority.empty()) {
            std::cout << "Processing HIGH: " << highPriority.front().name << "\n";
            highPriority.pop_front();
        } else if (!mediumPriority.empty()) {
            std::cout << "Processing MEDIUM: " << mediumPriority.front().name << "\n";
            mediumPriority.pop_front();
        } else if (!lowPriority.empty()) {
            std::cout << "Processing LOW: " << lowPriority.front().name << "\n";
            lowPriority.pop_front();
        } else {
            std::cout << "No tasks to process\n";
        }
    }

    void displayAll() const {
        std::cout << "\nCurrent Tasks:\n";
        std::cout << "HIGH: ";
        for (const auto& task : highPriority) std::cout << task.name << " ";
        std::cout << "\nMEDIUM: ";
        for (const auto& task : mediumPriority) std::cout << task.name << " ";
        std::cout << "\nLOW: ";
        for (const auto& task : lowPriority) std::cout << task.name << " ";
        std::cout << "\n\n";
    }
};

int main()
{
    TaskScheduler scheduler;

    scheduler.addTask("Email Report", 2);
    scheduler.addTask("Fix Bug", 1);
    scheduler.addTask("Update Docs", 3);
    scheduler.addTask("Security Patch", 1);
    scheduler.addTask("Code Review", 2);

    scheduler.displayAll();

    std::cout << "Processing tasks:\n";
    for (int i = 0; i < 5; i++) {
        scheduler.processNext();
    }

    scheduler.displayAll();

    return 0;
}
```

### Project: Text Editor with Undo/Redo

```cpp
#include <iostream>
#include <list>
#include <string>

class TextEditor {
private:
    std::string content;
    std::list<std::string> undoStack;
    std::list<std::string> redoStack;

    void saveState() {
        undoStack.push_back(content);
        redoStack.clear();  // Clear redo on new action
        if (undoStack.size() > 20) {  // Keep only last 20 states
            undoStack.pop_front();
        }
    }

public:
    void write(const std::string& text) {
        saveState();
        content += text;
    }

    void deleteLast(size_t n = 1) {
        saveState();
        if (n < content.size()) {
            content.erase(content.size() - n);
        } else {
            content.clear();
        }
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push_back(content);
            content = undoStack.back();
            undoStack.pop_back();
            std::cout << "Undo performed\n";
        } else {
            std::cout << "Nothing to undo\n";
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push_back(content);
            content = redoStack.back();
            redoStack.pop_back();
            std::cout << "Redo performed\n";
        } else {
            std::cout << "Nothing to redo\n";
        }
    }

    void display() const {
        std::cout << "Content: \"" << content << "\"\n";
    }
};

int main()
{
    TextEditor editor;

    editor.write("Hello");
    editor.display();

    editor.write(" World");
    editor.display();

    editor.write("!");
    editor.display();

    editor.undo();
    editor.display();

    editor.undo();
    editor.display();

    editor.redo();
    editor.display();

    editor.write(" C++");
    editor.display();

    return 0;
}
```

---

## Key Takeaways

1. ✅ List is a doubly-linked list with O(1) insert/delete anywhere
2. ✅ Deque is a double-ended queue with fast operations at both ends
3. ✅ List has no random access; must use iterators
4. ✅ Deque supports random access like vector
5. ✅ Use list for frequent insertions/deletions in middle
6. ✅ Use deque for queue operations or when adding/removing at both ends
7. ✅ List provides merge(), splice(), and unique() operations
8. ✅ Deque is ideal for implementing queues and sliding windows
9. ✅ Choose container based on access patterns and performance needs
10. ✅ Vector is still best for most cases with random access

---

## Summary Checklist

Before moving to Lesson 53, ensure you can:
- [ ] Create and initialize lists and deques
- [ ] Add/remove elements from both ends
- [ ] Use list-specific operations (merge, splice, sort)
- [ ] Access deque elements with random access
- [ ] Understand performance differences between containers
- [ ] Choose appropriate container for specific use cases
- [ ] Implement queues using deque
- [ ] Use lists for linked-list operations
- [ ] Handle iterator operations with lists
- [ ] Apply containers to real-world problems

---

## Next Lesson Preview

In **Lesson 53**, we'll learn about:
- Stack adapter (LIFO)
- Queue adapter (FIFO)
- Priority queue
- When to use each adapter
- Practical applications

**Great progress on sequential containers!**
