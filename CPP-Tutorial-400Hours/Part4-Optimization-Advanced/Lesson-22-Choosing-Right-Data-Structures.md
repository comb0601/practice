# Lesson 22: Choosing the Right Data Structures

**Duration**: 10 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Data Structure Selection
2. Array vs Vector vs Deque
3. List vs Forward_List
4. Set vs Unordered_Set
5. Map vs Unordered_Map
6. Stack, Queue, and Priority_Queue
7. Performance Comparison Table
8. Real-World Scenarios and Solutions
9. Custom Data Structures for Optimization
10. Exercises and Best Practices

---

## 1. Introduction to Data Structure Selection

**The golden rule**: The right data structure can turn O(n²) into O(n) or even O(1).

### Why Data Structure Choice Matters

```cpp
#include <iostream>
#include <vector>
#include <set>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Problem: Check if element exists

// Wrong choice: vector with linear search - O(n)
bool existsInVector(const vector<int>& vec, int value) {
    for (int x : vec) {
        if (x == value) return true;
    }
    return false;
}

// Right choice: set with logarithmic search - O(log n)
bool existsInSet(const set<int>& s, int value) {
    return s.find(value) != s.end();
}

int main() {
    const int SIZE = 100000;

    // Setup
    vector<int> vec;
    set<int> s;

    for (int i = 0; i < SIZE; i++) {
        vec.push_back(i);
        s.insert(i);
    }

    int target = SIZE - 1;  // Worst case: last element

    // Test vector
    auto start = high_resolution_clock::now();
    bool found1 = existsInVector(vec, target);
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end - start);

    // Test set
    start = high_resolution_clock::now();
    bool found2 = existsInSet(s, target);
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end - start);

    cout << "Searching for " << target << " in " << SIZE << " elements:\n";
    cout << "Vector (linear search): " << duration1.count() << " μs\n";
    cout << "Set (tree search): " << duration2.count() << " μs\n";
    cout << "Speedup: " << (double)duration1.count() / duration2.count()
         << "x faster!\n";

    return 0;
}
```

**Output (typical)**:
```
Searching for 99999 in 100000 elements:
Vector (linear search): 890 μs
Set (tree search): 0.5 μs
Speedup: 1780x faster!
```

---

## 2. Array vs Vector vs Deque

### Characteristics

| Feature | Array | Vector | Deque |
|---------|-------|--------|-------|
| Size | Fixed | Dynamic | Dynamic |
| Random Access | O(1) | O(1) | O(1) |
| Insert at End | N/A | O(1)* | O(1) |
| Insert at Front | N/A | O(n) | O(1) |
| Insert in Middle | N/A | O(n) | O(n) |
| Memory | Contiguous | Contiguous | Chunked |

*Amortized

### Practical Comparison

```cpp
#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Benchmark insertion at end
template<typename Container>
void benchmarkPushBack(const string& name, int count) {
    Container c;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        c.push_back(i);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << name << " push_back (" << count << " items): "
         << duration.count() << " μs\n";
}

// Benchmark insertion at front
template<typename Container>
void benchmarkPushFront(const string& name, int count) {
    Container c;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        c.push_front(i);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << name << " push_front (" << count << " items): "
         << duration.count() << " μs\n";
}

// Benchmark random access
template<typename Container>
void benchmarkRandomAccess(const string& name, Container& c) {
    int sum = 0;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < c.size(); i += 100) {
        sum += c[i];
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);

    cout << name << " random access: "
         << duration.count() << " ns (sum=" << sum << ")\n";
}

int main() {
    cout << "=== ARRAY vs VECTOR vs DEQUE ===\n\n";

    const int COUNT = 10000;

    // 1. Insertion at back
    cout << "1. Insertion at BACK:\n";
    benchmarkPushBack<vector<int>>("Vector", COUNT);
    benchmarkPushBack<deque<int>>("Deque ", COUNT);
    cout << "\n";

    // 2. Insertion at front
    cout << "2. Insertion at FRONT:\n";
    benchmarkPushFront<deque<int>>("Deque ", COUNT);
    // Note: vector doesn't have push_front - would need insert()
    cout << "Vector: Not supported efficiently (would be O(n) per insert)\n\n";

    // 3. Random access
    cout << "3. Random ACCESS:\n";
    vector<int> vec(COUNT);
    deque<int> deq(COUNT);
    for (int i = 0; i < COUNT; i++) {
        vec[i] = i;
        deq[i] = i;
    }

    benchmarkRandomAccess("Vector", vec);
    benchmarkRandomAccess("Deque ", deq);
    cout << "\n";

    // 4. Memory layout
    cout << "4. MEMORY LAYOUT:\n";
    cout << "Vector: Contiguous memory (cache-friendly)\n";
    cout << "Deque:  Chunked memory (allows front insertion)\n";
    cout << "Array:  Stack allocation (fastest, fixed size)\n\n";

    // Use cases
    cout << "=== WHEN TO USE EACH ===\n\n";

    cout << "Use ARRAY when:\n";
    cout << "  ✓ Size is known at compile time\n";
    cout << "  ✓ Maximum performance is critical\n";
    cout << "  ✓ Stack allocation is acceptable\n\n";

    cout << "Use VECTOR when:\n";
    cout << "  ✓ Dynamic size needed\n";
    cout << "  ✓ Frequent access by index\n";
    cout << "  ✓ Only adding/removing at end\n";
    cout << "  ✓ Cache locality is important\n\n";

    cout << "Use DEQUE when:\n";
    cout << "  ✓ Need to add/remove at both ends\n";
    cout << "  ✓ Don't need iterator stability\n";
    cout << "  ✓ Implementing queue or double-ended queue\n\n";

    return 0;
}
```

### Real Example: Choosing Based on Operations

```cpp
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

// Scenario 1: Image processing - sequential access
// Best choice: vector (cache-friendly)
class ImageProcessor {
    vector<uint8_t> pixels;  // Contiguous memory
    int width, height;

public:
    ImageProcessor(int w, int h) : width(w), height(h) {
        pixels.resize(w * h * 3);  // RGB
    }

    // Fast sequential access
    void applyFilter() {
        for (int i = 0; i < pixels.size(); i++) {
            pixels[i] = pixels[i] * 0.8;  // Cache-friendly
        }
    }
};

// Scenario 2: Undo/Redo system
// Best choice: deque (add/remove at both ends)
class UndoSystem {
    deque<string> undoStack;
    deque<string> redoStack;

public:
    void performAction(const string& action) {
        undoStack.push_back(action);  // O(1)
        redoStack.clear();
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push_back(undoStack.back());
            undoStack.pop_back();  // O(1)
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push_back(redoStack.back());
            redoStack.pop_back();  // O(1)
        }
    }
};

int main() {
    // Image processing
    ImageProcessor img(1920, 1080);
    img.applyFilter();

    // Undo/Redo
    UndoSystem editor;
    editor.performAction("Type 'Hello'");
    editor.performAction("Type ' World'");
    editor.undo();
    editor.redo();

    return 0;
}
```

---

## 3. List vs Forward_List

### Linked Lists: When to Use Them

```cpp
#include <iostream>
#include <list>
#include <forward_list>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Comparison: Vector vs List for insertions in middle

void testVectorInsertion(int count) {
    vector<int> vec;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        vec.insert(vec.begin() + vec.size() / 2, i);  // Insert in middle
    }

    auto end = high_resolution_clock::now();
    cout << "Vector middle insertion (" << count << "): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void testListInsertion(int count) {
    list<int> lst;

    auto start = high_resolution_clock::now();

    auto it = lst.begin();
    for (int i = 0; i < count; i++) {
        if (i % 2 == 0 && !lst.empty()) {
            it = lst.begin();
            advance(it, lst.size() / 2);
        }
        lst.insert(it, i);  // O(1) insertion once iterator is there
    }

    auto end = high_resolution_clock::now();
    cout << "List middle insertion (" << count << "): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

// Example: Frequent insertions/deletions
class TaskScheduler {
    list<string> tasks;  // Better for frequent insert/remove

public:
    void addTask(const string& task) {
        tasks.push_back(task);
    }

    // Remove completed tasks efficiently
    void removeCompleted() {
        tasks.remove_if([](const string& task) {
            return task.find("[DONE]") != string::npos;
        });
    }

    // Insert high-priority task
    void insertHighPriority(const string& task) {
        // Find position and insert - O(n) to find, O(1) to insert
        auto it = tasks.begin();
        advance(it, min(3, (int)tasks.size()));
        tasks.insert(it, task);
    }

    void display() {
        for (const auto& task : tasks) {
            cout << "  - " << task << "\n";
        }
    }
};

int main() {
    cout << "=== LIST vs VECTOR COMPARISON ===\n\n";

    // Insertion performance
    cout << "1. Middle Insertion Performance:\n";
    testVectorInsertion(1000);
    testListInsertion(1000);
    cout << "\n";

    // Memory overhead
    cout << "2. Memory Overhead:\n";
    cout << "Vector<int>: ~4 bytes per element\n";
    cout << "List<int>:   ~20 bytes per element (data + 2 pointers)\n";
    cout << "Forward_list<int>: ~12 bytes (data + 1 pointer)\n\n";

    // Real-world usage
    cout << "3. Real-world Example (Task Scheduler):\n";
    TaskScheduler scheduler;
    scheduler.addTask("Write code");
    scheduler.addTask("Test code");
    scheduler.addTask("[DONE] Design system");
    scheduler.addTask("Deploy");

    cout << "Before:\n";
    scheduler.display();

    scheduler.removeCompleted();
    scheduler.insertHighPriority("Fix critical bug");

    cout << "\nAfter:\n";
    scheduler.display();

    cout << "\n=== WHEN TO USE EACH ===\n\n";

    cout << "Use LIST when:\n";
    cout << "  ✓ Frequent insertions/deletions in middle\n";
    cout << "  ✓ No need for random access\n";
    cout << "  ✓ Iterator stability is important\n";
    cout << "  ✓ Splicing operations needed\n\n";

    cout << "Use FORWARD_LIST when:\n";
    cout << "  ✓ Singly-linked list is sufficient\n";
    cout << "  ✓ Memory is constrained\n";
    cout << "  ✓ Only forward iteration needed\n\n";

    cout << "Use VECTOR when:\n";
    cout << "  ✓ Random access needed\n";
    cout << "  ✓ Memory efficiency important\n";
    cout << "  ✓ Cache locality matters\n";
    cout << "  ✓ Mostly appending to end\n\n";

    return 0;
}
```

---

## 4. Set vs Unordered_Set

### Hash Table vs Binary Search Tree

```cpp
#include <iostream>
#include <set>
#include <unordered_set>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Comprehensive comparison
void compareSetPerformance() {
    const int COUNT = 100000;

    set<int> orderedSet;
    unordered_set<int> hashSet;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);

    vector<int> testData;
    for (int i = 0; i < COUNT; i++) {
        testData.push_back(dis(gen));
    }

    // 1. INSERTION
    cout << "1. INSERTION (" << COUNT << " elements):\n";

    auto start = high_resolution_clock::now();
    for (int x : testData) {
        orderedSet.insert(x);
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);

    start = high_resolution_clock::now();
    for (int x : testData) {
        hashSet.insert(x);
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);

    cout << "   set (ordered):          " << duration1.count() << " ms\n";
    cout << "   unordered_set (hash):   " << duration2.count() << " ms\n\n";

    // 2. SEARCH
    cout << "2. SEARCH (1000 random lookups):\n";

    start = high_resolution_clock::now();
    int found1 = 0;
    for (int i = 0; i < 1000; i++) {
        if (orderedSet.find(testData[i]) != orderedSet.end())
            found1++;
    }
    end = high_resolution_clock::now();
    duration1 = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();
    int found2 = 0;
    for (int i = 0; i < 1000; i++) {
        if (hashSet.find(testData[i]) != hashSet.end())
            found2++;
    }
    end = high_resolution_clock::now();
    duration2 = duration_cast<microseconds>(end - start);

    cout << "   set (O(log n)):         " << duration1.count() << " μs\n";
    cout << "   unordered_set (O(1)):   " << duration2.count() << " μs\n\n";

    // 3. ITERATION
    cout << "3. ITERATION (full traversal):\n";

    start = high_resolution_clock::now();
    long long sum1 = 0;
    for (int x : orderedSet) sum1 += x;
    end = high_resolution_clock::now();
    duration1 = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();
    long long sum2 = 0;
    for (int x : hashSet) sum2 += x;
    end = high_resolution_clock::now();
    duration2 = duration_cast<microseconds>(end - start);

    cout << "   set (sorted order):     " << duration1.count() << " μs\n";
    cout << "   unordered_set (random): " << duration2.count() << " μs\n\n";

    // 4. MEMORY
    cout << "4. MEMORY CHARACTERISTICS:\n";
    cout << "   set:           ~3 pointers + data per node\n";
    cout << "   unordered_set: ~1 pointer + data, + bucket array\n\n";
}

// Real example: Unique word counter
class WordCounter {
    unordered_set<string> uniqueWords_fast;  // Fast lookups
    set<string> uniqueWords_sorted;          // Sorted output

public:
    void addText(const string& text) {
        // Use unordered_set for fast insertion
        uniqueWords_fast.insert(text);
    }

    void displaySorted() {
        // Copy to set for sorted display
        uniqueWords_sorted = set<string>(
            uniqueWords_fast.begin(),
            uniqueWords_fast.end()
        );

        cout << "Unique words (sorted):\n";
        for (const auto& word : uniqueWords_sorted) {
            cout << "  " << word << "\n";
        }
    }

    size_t count() const {
        return uniqueWords_fast.size();
    }
};

int main() {
    cout << "=== SET vs UNORDERED_SET ===\n\n";

    compareSetPerformance();

    cout << "=== WHEN TO USE EACH ===\n\n";

    cout << "Use SET when:\n";
    cout << "  ✓ Need sorted order\n";
    cout << "  ✓ Range queries needed (lower_bound, upper_bound)\n";
    cout << "  ✓ Predictable performance (guaranteed O(log n))\n\n";

    cout << "Use UNORDERED_SET when:\n";
    cout << "  ✓ Don't care about order\n";
    cout << "  ✓ Need fastest possible lookup (O(1) average)\n";
    cout << "  ✓ Hash function available for your type\n\n";

    // Real example
    cout << "=== REAL EXAMPLE ===\n\n";
    WordCounter counter;
    counter.addText("hello");
    counter.addText("world");
    counter.addText("hello");
    counter.addText("cpp");

    cout << "Total unique words: " << counter.count() << "\n\n";
    counter.displaySorted();

    return 0;
}
```

---

## 5. Map vs Unordered_Map

### Key-Value Storage Comparison

```cpp
#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

// Performance comparison
void compareMapPerformance() {
    const int COUNT = 50000;

    map<string, int> orderedMap;
    unordered_map<string, int> hashMap;

    // Generate test data
    vector<pair<string, int>> testData;
    for (int i = 0; i < COUNT; i++) {
        testData.push_back({"key_" + to_string(i), i});
    }

    // 1. INSERTION
    cout << "1. INSERTION (" << COUNT << " entries):\n";

    auto start = high_resolution_clock::now();
    for (const auto& [key, value] : testData) {
        orderedMap[key] = value;
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);

    start = high_resolution_clock::now();
    for (const auto& [key, value] : testData) {
        hashMap[key] = value;
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);

    cout << "   map:            " << duration1.count() << " ms\n";
    cout << "   unordered_map:  " << duration2.count() << " ms\n\n";

    // 2. LOOKUP
    cout << "2. LOOKUP (10000 random accesses):\n";

    start = high_resolution_clock::now();
    int sum1 = 0;
    for (int i = 0; i < 10000; i++) {
        sum1 += orderedMap["key_" + to_string(i * 5)];
    }
    end = high_resolution_clock::now();
    duration1 = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();
    int sum2 = 0;
    for (int i = 0; i < 10000; i++) {
        sum2 += hashMap["key_" + to_string(i * 5)];
    }
    end = high_resolution_clock::now();
    duration2 = duration_cast<microseconds>(end - start);

    cout << "   map:            " << duration1.count() << " μs\n";
    cout << "   unordered_map:  " << duration2.count() << " μs\n\n";
}

// Real example: Configuration system
class ConfigSystem {
    unordered_map<string, string> config;  // Fast lookup

public:
    void loadConfig() {
        config["window_width"] = "1920";
        config["window_height"] = "1080";
        config["fullscreen"] = "true";
        config["vsync"] = "true";
        config["max_fps"] = "144";
    }

    string get(const string& key, const string& defaultVal = "") {
        auto it = config.find(key);  // O(1) average
        return (it != config.end()) ? it->second : defaultVal;
    }

    int getInt(const string& key, int defaultVal = 0) {
        string val = get(key);
        return val.empty() ? defaultVal : stoi(val);
    }

    bool getBool(const string& key, bool defaultVal = false) {
        string val = get(key);
        return val.empty() ? defaultVal : (val == "true");
    }
};

// Real example: Frequency counter
class FrequencyCounter {
    unordered_map<string, int> frequencies;

public:
    void addWord(const string& word) {
        frequencies[word]++;  // O(1) average
    }

    void displayTop(int n) {
        // Convert to vector for sorting
        vector<pair<string, int>> sorted(
            frequencies.begin(),
            frequencies.end()
        );

        // Sort by frequency
        sort(sorted.begin(), sorted.end(),
             [](const auto& a, const auto& b) {
                 return a.second > b.second;
             });

        cout << "Top " << n << " most frequent words:\n";
        for (int i = 0; i < min(n, (int)sorted.size()); i++) {
            cout << "  " << sorted[i].first
                 << ": " << sorted[i].second << "\n";
        }
    }
};

int main() {
    cout << "=== MAP vs UNORDERED_MAP ===\n\n";

    compareMapPerformance();

    cout << "=== REAL EXAMPLES ===\n\n";

    // Example 1: Config system
    cout << "1. Configuration System:\n";
    ConfigSystem config;
    config.loadConfig();

    cout << "Window size: "
         << config.getInt("window_width") << "x"
         << config.getInt("window_height") << "\n";
    cout << "Fullscreen: "
         << (config.getBool("fullscreen") ? "Yes" : "No") << "\n\n";

    // Example 2: Frequency counter
    cout << "2. Word Frequency Counter:\n";
    FrequencyCounter counter;

    string text[] = {"the", "quick", "brown", "fox", "jumps",
                     "over", "the", "lazy", "dog", "the",
                     "fox", "is", "quick"};

    for (const auto& word : text) {
        counter.addWord(word);
    }

    counter.displayTop(5);

    cout << "\n=== WHEN TO USE EACH ===\n\n";

    cout << "Use MAP when:\n";
    cout << "  ✓ Need sorted keys\n";
    cout << "  ✓ Range queries (lower_bound, upper_bound)\n";
    cout << "  ✓ Ordered iteration important\n\n";

    cout << "Use UNORDERED_MAP when:\n";
    cout << "  ✓ Fastest lookup/insertion needed\n";
    cout << "  ✓ Don't care about key order\n";
    cout << "  ✓ O(1) average case is critical\n\n";

    return 0;
}
```

---

## 6. Stack, Queue, and Priority_Queue

### Specialized Container Adapters

```cpp
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

// Example 1: Balanced parentheses checker (Stack)
bool isBalanced(const string& expr) {
    stack<char> s;

    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        }
        else if (c == ')' || c == ']' || c == '}') {
            if (s.empty()) return false;

            char top = s.top();
            s.pop();

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }
        }
    }

    return s.empty();
}

// Example 2: Task queue (Queue)
class TaskQueue {
    queue<string> tasks;

public:
    void addTask(const string& task) {
        tasks.push(task);  // O(1)
        cout << "Added: " << task << "\n";
    }

    void processNext() {
        if (!tasks.empty()) {
            string task = tasks.front();
            tasks.pop();  // O(1)
            cout << "Processing: " << task << "\n";
        } else {
            cout << "No tasks to process\n";
        }
    }

    int remaining() const {
        return tasks.size();
    }
};

// Example 3: Event scheduler (Priority Queue)
struct Event {
    int priority;
    string description;

    bool operator<(const Event& other) const {
        // Higher priority = lower number (min-heap behavior inverted)
        return priority > other.priority;
    }
};

class EventScheduler {
    priority_queue<Event> events;

public:
    void addEvent(int priority, const string& desc) {
        events.push({priority, desc});
        cout << "Scheduled (priority " << priority << "): "
             << desc << "\n";
    }

    void processNext() {
        if (!events.empty()) {
            Event e = events.top();
            events.pop();

            cout << "Processing priority " << e.priority
                 << ": " << e.description << "\n";
        }
    }

    int remaining() const {
        return events.size();
    }
};

// Example 4: Dijkstra's shortest path (Priority Queue)
struct Node {
    int id;
    int distance;

    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

void demonstrateDijkstra() {
    // Min-heap: smallest distance first
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    pq.push({0, 0});    // Start node, distance 0
    pq.push({1, 5});
    pq.push({2, 3});
    pq.push({3, 7});

    cout << "Processing nodes in order of distance:\n";
    while (!pq.empty()) {
        Node n = pq.top();
        pq.pop();

        cout << "  Node " << n.id << " at distance " << n.distance << "\n";
    }
}

int main() {
    cout << "=== STACK, QUEUE, PRIORITY_QUEUE ===\n\n";

    // 1. Stack example
    cout << "1. STACK - Balanced Parentheses:\n";
    string expressions[] = {
        "((()))",
        "([{}])",
        "((]",
        "{[()]}"
    };

    for (const auto& expr : expressions) {
        cout << "   \"" << expr << "\": "
             << (isBalanced(expr) ? "Balanced" : "Not balanced") << "\n";
    }
    cout << "\n";

    // 2. Queue example
    cout << "2. QUEUE - Task Processing:\n";
    TaskQueue taskQueue;
    taskQueue.addTask("Read email");
    taskQueue.addTask("Write code");
    taskQueue.addTask("Test features");

    cout << "\nProcessing tasks:\n";
    while (taskQueue.remaining() > 0) {
        taskQueue.processNext();
    }
    cout << "\n";

    // 3. Priority Queue example
    cout << "3. PRIORITY_QUEUE - Event Scheduling:\n";
    EventScheduler scheduler;
    scheduler.addEvent(5, "Regular task");
    scheduler.addEvent(1, "CRITICAL: Server down");
    scheduler.addEvent(3, "Important: Deploy update");
    scheduler.addEvent(10, "Low: Update docs");

    cout << "\nProcessing by priority:\n";
    while (scheduler.remaining() > 0) {
        scheduler.processNext();
    }
    cout << "\n";

    // 4. Advanced: Dijkstra
    cout << "4. ADVANCED - Shortest Path (Dijkstra):\n";
    demonstrateDijkstra();

    cout << "\n=== WHEN TO USE EACH ===\n\n";

    cout << "Use STACK when:\n";
    cout << "  ✓ LIFO (Last In, First Out) needed\n";
    cout << "  ✓ Recursion simulation\n";
    cout << "  ✓ Undo/backtracking operations\n";
    cout << "  ✓ Expression evaluation\n\n";

    cout << "Use QUEUE when:\n";
    cout << "  ✓ FIFO (First In, First Out) needed\n";
    cout << "  ✓ Task scheduling\n";
    cout << "  ✓ BFS (Breadth-First Search)\n";
    cout << "  ✓ Buffer management\n\n";

    cout << "Use PRIORITY_QUEUE when:\n";
    cout << "  ✓ Always need min/max element\n";
    cout << "  ✓ Event-driven simulation\n";
    cout << "  ✓ Dijkstra's algorithm\n";
    cout << "  ✓ Task scheduling by priority\n\n";

    return 0;
}
```

---

## 7. Performance Comparison Table

### Complete Performance Reference

```cpp
#include <iostream>
#include <iomanip>

using namespace std;

void printPerformanceTable() {
    cout << "╔════════════════════╦══════════╦══════════╦════════════╦════════════╦══════════╗\n";
    cout << "║ Container          ║  Access  ║  Search  ║ Insert End ║Insert Front║  Memory  ║\n";
    cout << "╠════════════════════╬══════════╬══════════╬════════════╬════════════╬══════════╣\n";
    cout << "║ vector             ║   O(1)   ║   O(n)   ║   O(1)*    ║    O(n)    ║ Excellent║\n";
    cout << "║ deque              ║   O(1)   ║   O(n)   ║   O(1)     ║    O(1)    ║   Good   ║\n";
    cout << "║ list               ║   O(n)   ║   O(n)   ║   O(1)     ║    O(1)    ║   Poor   ║\n";
    cout << "║ forward_list       ║   O(n)   ║   O(n)   ║   O(1)**   ║    O(1)    ║   Fair   ║\n";
    cout << "║ set                ║   O(n)   ║ O(log n) ║  O(log n)  ║  O(log n)  ║   Fair   ║\n";
    cout << "║ unordered_set      ║   O(n)   ║   O(1)*  ║   O(1)*    ║   O(1)*    ║   Fair   ║\n";
    cout << "║ map                ║   O(n)   ║ O(log n) ║  O(log n)  ║  O(log n)  ║   Fair   ║\n";
    cout << "║ unordered_map      ║   O(n)   ║   O(1)*  ║   O(1)*    ║   O(1)*    ║   Fair   ║\n";
    cout << "╚════════════════════╩══════════╩══════════╩════════════╩════════════╩══════════╝\n";
    cout << "\n* Amortized / Average case\n";
    cout << "** Only at known position\n\n";
}

void printUseCaseGuide() {
    cout << "╔════════════════════════════════════╦═══════════════════════════════════════╗\n";
    cout << "║ Use Case                           ║ Best Container                        ║\n";
    cout << "╠════════════════════════════════════╬═══════════════════════════════════════╣\n";
    cout << "║ Random access + dynamic size       ║ vector                                ║\n";
    cout << "║ Frequent insert/delete at ends     ║ deque                                 ║\n";
    cout << "║ Frequent insert/delete in middle   ║ list                                  ║\n";
    cout << "║ Unique sorted elements             ║ set                                   ║\n";
    cout << "║ Unique elements, fast lookup       ║ unordered_set                         ║\n";
    cout << "║ Key-value pairs, sorted            ║ map                                   ║\n";
    cout << "║ Key-value pairs, fast lookup       ║ unordered_map                         ║\n";
    cout << "║ LIFO operations                    ║ stack                                 ║\n";
    cout << "║ FIFO operations                    ║ queue                                 ║\n";
    cout << "║ Priority-based access              ║ priority_queue                        ║\n";
    cout << "╚════════════════════════════════════╩═══════════════════════════════════════╝\n\n";
}

int main() {
    cout << "=== C++ CONTAINER PERFORMANCE REFERENCE ===\n\n";

    printPerformanceTable();
    printUseCaseGuide();

    return 0;
}
```

---

## 8. Real-World Scenarios and Solutions

### Scenario 1: Social Network Friend Suggestions

```cpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

class SocialNetwork {
    // User ID -> Set of friend IDs
    unordered_map<int, unordered_set<int>> friendships;

public:
    void addFriendship(int user1, int user2) {
        friendships[user1].insert(user2);  // O(1) average
        friendships[user2].insert(user1);  // O(1) average
    }

    // Find mutual friends: O(min(F1, F2)) where F = friends count
    vector<int> getMutualFriends(int user1, int user2) {
        vector<int> mutual;

        const auto& friends1 = friendships[user1];
        const auto& friends2 = friendships[user2];

        // Iterate through smaller set
        const auto& smaller = (friends1.size() < friends2.size())
                              ? friends1 : friends2;
        const auto& larger = (friends1.size() < friends2.size())
                             ? friends2 : friends1;

        for (int friendId : smaller) {
            if (larger.count(friendId)) {  // O(1) average
                mutual.push_back(friendId);
            }
        }

        return mutual;
    }

    // Friend suggestions: friends of friends
    vector<int> suggestFriends(int userId) {
        unordered_map<int, int> suggestions;  // ID -> count
        const auto& userFriends = friendships[userId];

        // For each friend
        for (int friendId : userFriends) {
            // Check their friends
            for (int suggestionId : friendships[friendId]) {
                // Skip self and existing friends
                if (suggestionId != userId &&
                    !userFriends.count(suggestionId)) {
                    suggestions[suggestionId]++;
                }
            }
        }

        // Sort by number of mutual connections
        vector<pair<int, int>> sorted(suggestions.begin(),
                                      suggestions.end());
        sort(sorted.begin(), sorted.end(),
             [](const auto& a, const auto& b) {
                 return a.second > b.second;
             });

        vector<int> result;
        for (const auto& [id, count] : sorted) {
            result.push_back(id);
        }

        return result;
    }
};

void demonstrateSocialNetwork() {
    SocialNetwork network;

    // Build network
    network.addFriendship(1, 2);
    network.addFriendship(1, 3);
    network.addFriendship(2, 3);
    network.addFriendship(2, 4);
    network.addFriendship(3, 4);
    network.addFriendship(3, 5);
    network.addFriendship(4, 6);

    // Find mutual friends
    cout << "Mutual friends of users 1 and 3:\n";
    auto mutual = network.getMutualFriends(1, 3);
    for (int id : mutual) {
        cout << "  User " << id << "\n";
    }

    // Friend suggestions
    cout << "\nFriend suggestions for user 1:\n";
    auto suggestions = network.suggestFriends(1);
    for (int id : suggestions) {
        cout << "  User " << id << "\n";
    }
}
```

### Scenario 2: LRU Cache

```cpp
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

template<typename K, typename V>
class LRUCache {
    int capacity;
    list<pair<K, V>> items;  // Most recent at front
    unordered_map<K, typename list<pair<K, V>>::iterator> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    V get(const K& key) {
        auto it = cache.find(key);

        if (it == cache.end()) {
            throw runtime_error("Key not found");
        }

        // Move to front (most recently used)
        items.splice(items.begin(), items, it->second);

        return it->second->second;
    }

    void put(const K& key, const V& value) {
        auto it = cache.find(key);

        // If exists, remove old entry
        if (it != cache.end()) {
            items.erase(it->second);
        }

        // Add to front
        items.push_front({key, value});
        cache[key] = items.begin();

        // Evict if over capacity
        if (items.size() > capacity) {
            auto last = items.back();
            cache.erase(last.first);
            items.pop_back();
        }
    }

    void display() {
        cout << "Cache contents (most recent first):\n";
        for (const auto& [key, value] : items) {
            cout << "  " << key << ": " << value << "\n";
        }
    }
};

void demonstrateLRUCache() {
    LRUCache<string, int> cache(3);  // Capacity: 3

    cout << "Adding items:\n";
    cache.put("page1", 100);
    cache.put("page2", 200);
    cache.put("page3", 300);
    cache.display();

    cout << "\nAccessing page1 (moves to front):\n";
    cache.get("page1");
    cache.display();

    cout << "\nAdding page4 (evicts least recently used):\n";
    cache.put("page4", 400);
    cache.display();
}
```

---

## 9. Custom Data Structures for Optimization

### Custom Hash Functions

```cpp
#include <iostream>
#include <unordered_set>
#include <functional>

using namespace std;

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Custom hash function
struct PointHash {
    size_t operator()(const Point& p) const {
        // Combine hash values
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
    }
};

void demonstrateCustomHash() {
    unordered_set<Point, PointHash> points;

    points.insert({1, 2});
    points.insert({3, 4});
    points.insert({1, 2});  // Duplicate, won't be added

    cout << "Unique points: " << points.size() << "\n";

    if (points.count({1, 2})) {
        cout << "Point (1,2) found!\n";
    }
}
```

---

## 10. Exercises and Best Practices

### Exercise 1: Choose the Right Container

```cpp
// For each scenario, choose the best container:

// 1. Storing user IDs (unique, fast lookup, no order needed)
// Answer: unordered_set<int>

// 2. Undo/Redo functionality
// Answer: deque<Action> or two stacks

// 3. Phone book (name -> number, sorted by name)
// Answer: map<string, string>

// 4. Task queue (process in order received)
// Answer: queue<Task>

// 5. Top K frequent elements
// Answer: priority_queue + unordered_map
```

### Best Practices

```cpp
/*
1. Default to vector
   - Unless you have a specific reason to use something else
   - Best cache locality and memory efficiency

2. Use unordered_* for lookups
   - When you don't need sorted order
   - 3-10x faster than ordered versions

3. Reserve capacity when size known
   - vector.reserve(n) avoids reallocations
   - Major performance boost for large data

4. Prefer emplace over push
   - emplace_back constructs in-place
   - Avoids copy/move

5. Use const& for iteration
   - for (const auto& item : container)
   - Avoids unnecessary copies

6. Profile before optimizing
   - Measure actual performance
   - Don't assume; verify!
*/
```

---

## Key Takeaways

1. ✅ **Container choice dramatically affects performance**
2. ✅ **vector is the default choice** (best cache locality)
3. ✅ **Use unordered_* for fast lookups** when order doesn't matter
4. ✅ **list/forward_list only for frequent mid-insertions**
5. ✅ **priority_queue for always-sorted access**
6. ✅ **Understand time complexity** of each operation
7. ✅ **Profile real-world usage** before optimizing
8. ✅ **Memory layout matters** (cache effects)

---

## Summary Checklist

Before moving to Lesson 23, ensure you can:
- [ ] Explain performance differences between containers
- [ ] Choose appropriate container for specific use cases
- [ ] Understand Big O complexity of container operations
- [ ] Use specialized containers (stack, queue, priority_queue)
- [ ] Implement custom hash functions
- [ ] Apply containers to real-world problems
- [ ] Benchmark container performance
- [ ] Optimize based on access patterns

**Congratulations on mastering Data Structure Selection!**

Next lesson: String Optimization Techniques
