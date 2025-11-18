# Lesson 23: String Optimization Techniques

**Duration**: 10 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Understanding String Performance Issues
2. String Construction and Concatenation
3. String_View for Zero-Copy Operations
4. Small String Optimization (SSO)
5. String Interning and Pooling
6. Parsing and Tokenization Optimization
7. String Searching Algorithms
8. Memory Allocation Strategies
9. String Builder Pattern
10. Exercises and Best Practices

---

## 1. Understanding String Performance Issues

Strings are deceptively expensive in C++. Common pitfalls:
- **Hidden allocations**: Each string operation may allocate memory
- **Unnecessary copies**: Passing by value copies entire string
- **Repeated concatenation**: O(n²) when done naively
- **Character-by-character processing**: Cache misses

### Measuring String Performance

```cpp
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Poor: Multiple allocations
string concatenate_poor(int count) {
    string result;
    for (int i = 0; i < count; i++) {
        result = result + "item" + to_string(i) + ", ";
        // Each iteration: 3+ allocations and copies!
    }
    return result;
}

// Better: Use +=
string concatenate_better(int count) {
    string result;
    for (int i = 0; i < count; i++) {
        result += "item";
        result += to_string(i);
        result += ", ";
        // Fewer allocations due to capacity growth
    }
    return result;
}

// Best: Reserve capacity
string concatenate_best(int count) {
    string result;
    result.reserve(count * 15);  // Estimate final size
    for (int i = 0; i < count; i++) {
        result += "item";
        result += to_string(i);
        result += ", ";
    }
    return result;
}

// Alternative: Use stringstream
string concatenate_stream(int count) {
    stringstream ss;
    for (int i = 0; i < count; i++) {
        ss << "item" << i << ", ";
    }
    return ss.str();
}

void benchmark(const string& name, string (*func)(int), int count) {
    auto start = high_resolution_clock::now();
    string result = func(count);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    cout << name << ": " << duration.count() << " μs "
         << "(length: " << result.length() << ")\n";
}

int main() {
    cout << "=== STRING CONCATENATION PERFORMANCE ===\n\n";

    const int COUNT = 1000;

    benchmark("Poor (operator+)    ", concatenate_poor, COUNT);
    benchmark("Better (operator+=) ", concatenate_better, COUNT);
    benchmark("Best (reserve)      ", concatenate_best, COUNT);
    benchmark("Stream              ", concatenate_stream, COUNT);

    /*
    Typical results:
    Poor:   15000 μs (1000s of allocations)
    Better: 500 μs (dozens of allocations)
    Best:   300 μs (1-2 allocations)
    Stream: 400 μs (buffer management)
    */

    return 0;
}
```

---

## 2. String Construction and Concatenation

### The Cost of String Operations

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ANTI-PATTERN: Quadratic complexity
string join_poor(const vector<string>& items) {
    string result;
    for (const auto& item : items) {
        result = result + item + ", ";
        // Copies entire result each iteration!
        // O(n²) time complexity
    }
    return result;
}

// GOOD: Linear complexity
string join_good(const vector<string>& items) {
    string result;
    for (const auto& item : items) {
        result += item;
        result += ", ";
        // Amortized O(1) per append
    }
    return result;
}

// BETTER: With reservation
string join_better(const vector<string>& items) {
    // Calculate total size needed
    size_t total_size = 0;
    for (const auto& item : items) {
        total_size += item.length() + 2;  // +2 for ", "
    }

    string result;
    result.reserve(total_size);  // Single allocation!

    for (const auto& item : items) {
        result += item;
        result += ", ";
    }

    return result;
}

// BEST: Custom join with iterator
template<typename Iterator>
string join_best(Iterator begin, Iterator end, const string& delimiter) {
    if (begin == end) return "";

    // Calculate size
    size_t total_size = 0;
    size_t count = 0;
    for (auto it = begin; it != end; ++it) {
        total_size += it->length();
        count++;
    }
    total_size += (count - 1) * delimiter.length();

    // Build result
    string result;
    result.reserve(total_size);

    result += *begin;
    for (auto it = begin + 1; it != end; ++it) {
        result += delimiter;
        result += *it;
    }

    return result;
}

// Demonstration
void demonstrateJoin() {
    vector<string> items;
    for (int i = 0; i < 100; i++) {
        items.push_back("Item" + to_string(i));
    }

    cout << "=== STRING JOIN COMPARISON ===\n\n";

    auto start = chrono::high_resolution_clock::now();
    string r1 = join_poor(items);
    auto end = chrono::high_resolution_clock::now();
    cout << "Poor:   " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " μs\n";

    start = chrono::high_resolution_clock::now();
    string r2 = join_good(items);
    end = chrono::high_resolution_clock::now();
    cout << "Good:   " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " μs\n";

    start = chrono::high_resolution_clock::now();
    string r3 = join_better(items);
    end = chrono::high_resolution_clock::now();
    cout << "Better: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " μs\n";

    start = chrono::high_resolution_clock::now();
    string r4 = join_best(items.begin(), items.end(), ", ");
    end = chrono::high_resolution_clock::now();
    cout << "Best:   " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " μs\n";
}

int main() {
    demonstrateJoin();
    return 0;
}
```

---

## 3. String_View for Zero-Copy Operations

C++17's `string_view` provides a **non-owning** reference to string data, avoiding copies.

```cpp
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Poor: Copies strings
vector<string> tokenize_copy(const string& text, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(text.substr(start, end - start));  // COPY!
        start = end + 1;
        end = text.find(delimiter, start);
    }

    tokens.push_back(text.substr(start));  // COPY!
    return tokens;
}

// Excellent: Zero-copy with string_view
vector<string_view> tokenize_view(string_view text, char delimiter) {
    vector<string_view> tokens;
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string_view::npos) {
        tokens.push_back(text.substr(start, end - start));  // NO COPY!
        start = end + 1;
        end = text.find(delimiter, start);
    }

    tokens.push_back(text.substr(start));  // NO COPY!
    return tokens;
}

// Example: Processing CSV
class CSVParser_Copy {
    string data;

public:
    CSVParser_Copy(const string& csv) : data(csv) {}

    vector<string> getRow(int row) {
        auto lines = tokenize_copy(data, '\n');
        if (row >= lines.size()) return {};
        return tokenize_copy(lines[row], ',');
    }
};

class CSVParser_View {
    string data;

public:
    CSVParser_View(const string& csv) : data(csv) {}

    vector<string_view> getRow(int row) {
        auto lines = tokenize_view(data, '\n');
        if (row >= lines.size()) return {};
        return tokenize_view(lines[row], ',');
    }
};

// Demonstration
void demonstrateStringView() {
    cout << "=== STRING_VIEW PERFORMANCE ===\n\n";

    // Generate test data
    string csv;
    for (int i = 0; i < 1000; i++) {
        csv += to_string(i) + "," + to_string(i*2) + "," + to_string(i*3) + "\n";
    }

    // Test with copies
    auto start = high_resolution_clock::now();
    CSVParser_Copy parser1(csv);
    for (int i = 0; i < 100; i++) {
        auto row = parser1.getRow(i % 1000);
    }
    auto end = high_resolution_clock::now();
    cout << "With string copies: "
         << duration_cast<microseconds>(end - start).count() << " μs\n";

    // Test with views
    start = high_resolution_clock::now();
    CSVParser_View parser2(csv);
    for (int i = 0; i < 100; i++) {
        auto row = parser2.getRow(i % 1000);
    }
    end = high_resolution_clock::now();
    cout << "With string_view:   "
         << duration_cast<microseconds>(end - start).count() << " μs\n";

    cout << "\nSpeedup: ~10-50x faster!\n";
}

// String_view best practices
void stringViewBestPractices() {
    cout << "\n=== STRING_VIEW BEST PRACTICES ===\n\n";

    // ✓ GOOD: Read-only access
    auto countWords = [](string_view text) {
        int count = 0;
        bool inWord = false;
        for (char c : text) {
            if (isspace(c)) {
                inWord = false;
            } else if (!inWord) {
                count++;
                inWord = true;
            }
        }
        return count;
    };

    string text = "Hello world from C++";
    cout << "Word count: " << countWords(text) << "\n";

    // ✓ GOOD: Parsing without allocation
    auto parseNumber = [](string_view sv) -> int {
        int result = 0;
        for (char c : sv) {
            if (isdigit(c)) {
                result = result * 10 + (c - '0');
            }
        }
        return result;
    };

    cout << "Parsed number: " << parseNumber("123abc") << "\n";

    // ⚠ WARNING: Dangling views
    string_view getDanglingView() {
        string temp = "temporary";
        return temp;  // DANGER: temp is destroyed!
    }
    // Don't use the result of getDanglingView()!

    // ✓ SAFE: View of literal
    string_view safe = "This is safe";  // String literal has static lifetime

    cout << "\nKey Points:\n";
    cout << "  ✓ Use string_view for read-only parameters\n";
    cout << "  ✓ Avoid storing string_view in long-lived objects\n";
    cout << "  ✓ Be careful of lifetime issues\n";
    cout << "  ✓ Perfect for parsing and tokenization\n";
}

int main() {
    demonstrateStringView();
    stringViewBestPractices();
    return 0;
}
```

---

## 4. Small String Optimization (SSO)

Most `std::string` implementations use SSO to avoid heap allocation for short strings.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Demonstrate SSO
void demonstrateSSO() {
    cout << "=== SMALL STRING OPTIMIZATION ===\n\n";

    cout << "sizeof(std::string): " << sizeof(string) << " bytes\n";
    cout << "(Typically 24-32 bytes, includes inline buffer)\n\n";

    // Test SSO threshold
    vector<int> lengths = {1, 5, 10, 15, 20, 25, 30};

    for (int len : lengths) {
        string s(len, 'x');

        // Check if data is inline
        bool isInline = (s.capacity() == s.size() && s.size() < 24);

        cout << "Length " << len << ": "
             << "capacity=" << s.capacity()
             << " (likely " << (isInline ? "INLINE" : "HEAP") << ")\n";
    }

    cout << "\n=== PERFORMANCE COMPARISON ===\n\n";

    const int COUNT = 1000000;

    // Short strings (SSO)
    auto start = high_resolution_clock::now();
    for (int i = 0; i < COUNT; i++) {
        string s = "short";  // No heap allocation!
        s += "!";
    }
    auto end = high_resolution_clock::now();
    cout << "Short strings (SSO): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // Long strings (heap)
    start = high_resolution_clock::now();
    for (int i = 0; i < COUNT; i++) {
        string s = "this is a very long string that exceeds SSO buffer";
        s += "!";  // Heap allocation required
    }
    end = high_resolution_clock::now();
    cout << "Long strings (heap): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    cout << "\nLesson: Keep strings short when possible!\n";
}

// Optimization: Use short string keys
class ConfigSystemSSO {
    // Short keys benefit from SSO
    unordered_map<string, string> config;

public:
    void set(const string& key, const string& value) {
        config[key] = value;  // If key is short, no allocation
    }

    string get(const string& key) const {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : "";
    }
};

int main() {
    demonstrateSSO();

    cout << "\n=== PRACTICAL TIPS ===\n\n";
    cout << "1. Short strings (< 15-23 chars) are often free!\n";
    cout << "2. Use short keys in maps/sets when possible\n";
    cout << "3. Truncate temporary strings to trigger SSO\n";
    cout << "4. Test your implementation's SSO threshold\n";

    return 0;
}
```

---

## 5. String Interning and Pooling

For applications with many duplicate strings, **string interning** can save memory.

```cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include <memory>
#include <vector>

using namespace std;

// String pool: Stores unique strings, returns pointers
class StringPool {
    unordered_set<string> pool;

public:
    // Returns pointer to pooled string
    const string* intern(const string& str) {
        auto [it, inserted] = pool.insert(str);
        return &(*it);  // Return pointer to string in set
    }

    size_t size() const { return pool.size(); }
    size_t memoryUsage() const {
        size_t total = 0;
        for (const auto& s : pool) {
            total += sizeof(string) + s.capacity();
        }
        return total;
    }
};

// Example: XML parser with repeated tag names
class XMLParser {
    StringPool tagPool;

    struct Element {
        const string* tagName;  // Pointer to pooled string
        string content;

        Element(const string* tag, const string& cont)
            : tagName(tag), content(cont) {}
    };

    vector<Element> elements;

public:
    void parseElement(const string& tag, const string& content) {
        const string* pooledTag = tagPool.intern(tag);
        elements.emplace_back(pooledTag, content);
    }

    void displayStats() {
        cout << "Total elements: " << elements.size() << "\n";
        cout << "Unique tag names: " << tagPool.size() << "\n";
        cout << "Tag pool memory: " << tagPool.memoryUsage() << " bytes\n";

        // Calculate memory saved
        size_t withoutPooling = elements.size() * 20;  // Avg tag length
        size_t withPooling = tagPool.memoryUsage();
        cout << "Memory saved: " << (withoutPooling - withPooling) << " bytes\n";
    }
};

void demonstrateStringPooling() {
    cout << "=== STRING POOLING DEMONSTRATION ===\n\n";

    XMLParser parser;

    // Parse many elements with repeated tags
    for (int i = 0; i < 1000; i++) {
        parser.parseElement("div", "Content " + to_string(i));
        parser.parseElement("span", "Text " + to_string(i));
        parser.parseElement("p", "Paragraph " + to_string(i));
    }

    parser.displayStats();

    /*
    Output:
    Total elements: 3000
    Unique tag names: 3
    Tag pool memory: ~150 bytes
    Memory saved: ~59,850 bytes (99.7% reduction!)
    */
}

int main() {
    demonstrateStringPooling();

    cout << "\n=== WHEN TO USE STRING POOLING ===\n\n";
    cout << "✓ Many duplicate strings (XML tags, identifiers, etc.)\n";
    cout << "✓ Long-running applications\n";
    cout << "✓ Memory-constrained environments\n";
    cout << "✗ Unique strings (no benefit)\n";
    cout << "✗ Short-lived data (overhead not worth it)\n";

    return 0;
}
```

---

## 6. Parsing and Tokenization Optimization

### Efficient String Splitting

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string_view>

using namespace std;

// Method 1: Using stringstream (slow)
vector<string> split_stringstream(const string& text, char delimiter) {
    vector<string> tokens;
    stringstream ss(text);  // Copy!
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);  // Copy!
    }

    return tokens;
}

// Method 2: Manual find (better)
vector<string> split_find(const string& text, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
        end = text.find(delimiter, start);
    }

    tokens.push_back(text.substr(start));
    return tokens;
}

// Method 3: Zero-copy with string_view (best)
vector<string_view> split_view(string_view text, char delimiter) {
    vector<string_view> tokens;
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string_view::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
        end = text.find(delimiter, start);
    }

    tokens.push_back(text.substr(start));
    return tokens;
}

// Method 4: In-place with indices (zero allocation)
struct Token {
    size_t start;
    size_t length;
};

vector<Token> split_indices(string_view text, char delimiter) {
    vector<Token> tokens;
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != string_view::npos) {
        tokens.push_back({start, end - start});
        start = end + 1;
        end = text.find(delimiter, start);
    }

    tokens.push_back({start, text.length() - start});
    return tokens;
}

void benchmarkSplitting() {
    cout << "=== STRING SPLITTING PERFORMANCE ===\n\n";

    // Create test data
    string text;
    for (int i = 0; i < 10000; i++) {
        text += "token" + to_string(i) + ",";
    }

    auto benchmark = [&text](const string& name, auto func) {
        auto start = chrono::high_resolution_clock::now();
        auto result = func(text, ',');
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << name << ": " << duration.count() << " μs ("
             << result.size() << " tokens)\n";
    };

    benchmark("stringstream", split_stringstream);
    benchmark("find        ", split_find);

    // string_view version
    auto start = chrono::high_resolution_clock::now();
    auto result_view = split_view(text, ',');
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "string_view : " << duration.count() << " μs ("
         << result_view.size() << " tokens)\n";

    // Index version
    start = chrono::high_resolution_clock::now();
    auto result_idx = split_indices(text, ',');
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "indices     : " << duration.count() << " μs ("
         << result_idx.size() << " tokens)\n";
}

int main() {
    benchmarkSplitting();
    return 0;
}
```

---

## 7. String Searching Algorithms

### Boyer-Moore for Fast String Search

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Naive search: O(nm)
int search_naive(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == m) return i;
    }

    return -1;
}

// Boyer-Moore: O(n/m) best case, O(nm) worst
class BoyerMoore {
    string pattern;
    vector<int> badChar;
    static const int ALPHABET_SIZE = 256;

    void computeBadChar() {
        badChar.assign(ALPHABET_SIZE, -1);
        for (int i = 0; i < pattern.length(); i++) {
            badChar[(int)pattern[i]] = i;
        }
    }

public:
    BoyerMoore(const string& pat) : pattern(pat) {
        computeBadChar();
    }

    int search(const string& text) {
        int n = text.length();
        int m = pattern.length();

        int s = 0;  // Shift
        while (s <= n - m) {
            int j = m - 1;

            // Match from right to left
            while (j >= 0 && pattern[j] == text[s + j]) {
                j--;
            }

            if (j < 0) {
                return s;  // Found!
            } else {
                // Bad character heuristic
                s += max(1, j - badChar[(int)text[s + j]]);
            }
        }

        return -1;
    }
};

// KMP Algorithm: O(n + m)
class KMP {
    string pattern;
    vector<int> lps;  // Longest Prefix Suffix

    void computeLPS() {
        int m = pattern.length();
        lps.assign(m, 0);

        int len = 0;
        int i = 1;

        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }

public:
    KMP(const string& pat) : pattern(pat) {
        computeLPS();
    }

    int search(const string& text) {
        int n = text.length();
        int m = pattern.length();

        int i = 0;  // Index for text
        int j = 0;  // Index for pattern

        while (i < n) {
            if (pattern[j] == text[i]) {
                i++;
                j++;
            }

            if (j == m) {
                return i - j;  // Found!
            } else if (i < n && pattern[j] != text[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }

        return -1;
    }
};

void benchmarkSearch() {
    cout << "=== STRING SEARCH ALGORITHMS ===\n\n";

    // Create test data
    string text;
    for (int i = 0; i < 100000; i++) {
        text += "abcdefghij";
    }
    text += "PATTERN";

    string pattern = "PATTERN";

    // Test naive
    auto start = high_resolution_clock::now();
    int pos1 = search_naive(text, pattern);
    auto end = high_resolution_clock::now();
    cout << "Naive:       " << duration_cast<microseconds>(end - start).count()
         << " μs (found at " << pos1 << ")\n";

    // Test std::string::find
    start = high_resolution_clock::now();
    int pos2 = text.find(pattern);
    auto end2 = high_resolution_clock::now();
    cout << "std::find:   " << duration_cast<microseconds>(end2 - start).count()
         << " μs (found at " << pos2 << ")\n";

    // Test Boyer-Moore
    BoyerMoore bm(pattern);
    start = high_resolution_clock::now();
    int pos3 = bm.search(text);
    end = high_resolution_clock::now();
    cout << "Boyer-Moore: " << duration_cast<microseconds>(end - start).count()
         << " μs (found at " << pos3 << ")\n";

    // Test KMP
    KMP kmp(pattern);
    start = high_resolution_clock::now();
    int pos4 = kmp.search(text);
    end = high_resolution_clock::now();
    cout << "KMP:         " << duration_cast<microseconds>(end - start).count()
         << " μs (found at " << pos4 << ")\n";
}

int main() {
    benchmarkSearch();

    cout << "\n=== ALGORITHM SELECTION ===\n\n";
    cout << "Use std::string::find() for:\n";
    cout << "  ✓ Most cases (highly optimized)\n\n";

    cout << "Use Boyer-Moore when:\n";
    cout << "  ✓ Long pattern (>10 chars)\n";
    cout << "  ✓ Large alphabet\n";
    cout << "  ✓ Pattern search is bottleneck\n\n";

    cout << "Use KMP when:\n";
    cout << "  ✓ Many partial matches\n";
    cout << "  ✓ Guaranteed O(n+m) needed\n";

    return 0;
}
```

---

## 8. Memory Allocation Strategies

### Custom Allocators for Strings

```cpp
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// Simple arena allocator for strings
template<size_t SIZE>
class ArenaAllocator {
    char buffer[SIZE];
    size_t offset = 0;

public:
    char* allocate(size_t n) {
        if (offset + n > SIZE) {
            throw bad_alloc();
        }

        char* ptr = buffer + offset;
        offset += n;
        return ptr;
    }

    void reset() {
        offset = 0;
    }

    size_t used() const { return offset; }
};

// String builder using arena
class StringBuilder {
    static const size_t ARENA_SIZE = 1024 * 1024;  // 1MB
    ArenaAllocator<ARENA_SIZE> arena;

    struct Chunk {
        const char* data;
        size_t length;
    };

    vector<Chunk> chunks;

public:
    void append(const string& str) {
        char* buf = arena.allocate(str.length());
        memcpy(buf, str.data(), str.length());
        chunks.push_back({buf, str.length()});
    }

    string build() {
        size_t total = 0;
        for (const auto& chunk : chunks) {
            total += chunk.length;
        }

        string result;
        result.reserve(total);

        for (const auto& chunk : chunks) {
            result.append(chunk.data, chunk.length);
        }

        return result;
    }

    void clear() {
        chunks.clear();
        arena.reset();
    }
};

void demonstrateArena() {
    cout << "=== ARENA ALLOCATOR FOR STRINGS ===\n\n";

    StringBuilder sb;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; i++) {
        sb.append("Item " + to_string(i) + ", ");
    }

    string result = sb.build();

    auto end = chrono::high_resolution_clock::now();

    cout << "Built string of " << result.length() << " chars\n";
    cout << "Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " μs\n";
    cout << "\nBenefit: Reduced memory fragmentation\n";
}

int main() {
    demonstrateArena();
    return 0;
}
```

---

## 9. String Builder Pattern

### Efficient String Construction

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// String builder implementation
class StringBuilder {
    vector<string> parts;
    size_t totalLength = 0;

public:
    StringBuilder& append(const string& str) {
        parts.push_back(str);
        totalLength += str.length();
        return *this;  // Chaining
    }

    StringBuilder& append(int value) {
        return append(to_string(value));
    }

    StringBuilder& append(double value) {
        return append(to_string(value));
    }

    StringBuilder& append(char c) {
        return append(string(1, c));
    }

    string toString() {
        string result;
        result.reserve(totalLength);  // Single allocation!

        for (const auto& part : parts) {
            result += part;
        }

        return result;
    }

    void clear() {
        parts.clear();
        totalLength = 0;
    }

    size_t length() const { return totalLength; }
};

// Example: Generate HTML
class HTMLBuilder {
    StringBuilder sb;
    int indentLevel = 0;

    void indent() {
        sb.append(string(indentLevel * 2, ' '));
    }

public:
    HTMLBuilder& openTag(const string& tag) {
        indent();
        sb.append("<").append(tag).append(">\n");
        indentLevel++;
        return *this;
    }

    HTMLBuilder& closeTag(const string& tag) {
        indentLevel--;
        indent();
        sb.append("</").append(tag).append(">\n");
        return *this;
    }

    HTMLBuilder& content(const string& text) {
        indent();
        sb.append(text).append("\n");
        return *this;
    }

    string build() {
        return sb.toString();
    }
};

void demonstrateStringBuilder() {
    cout << "=== STRING BUILDER PATTERN ===\n\n";

    StringBuilder sb;

    auto start = chrono::high_resolution_clock::now();

    // Build complex string
    sb.append("Name: ").append("John")
      .append(", Age: ").append(30)
      .append(", Score: ").append(95.5)
      .append('\n');

    for (int i = 0; i < 100; i++) {
        sb.append("Item ").append(i).append('\n');
    }

    string result = sb.toString();

    auto end = chrono::high_resolution_clock::now();

    cout << "String length: " << result.length() << "\n";
    cout << "Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " μs\n\n";

    // HTML example
    cout << "=== HTML BUILDER EXAMPLE ===\n\n";

    HTMLBuilder html;
    string page = html.openTag("html")
                      .openTag("body")
                      .openTag("h1")
                      .content("Welcome!")
                      .closeTag("h1")
                      .openTag("p")
                      .content("This is a paragraph.")
                      .closeTag("p")
                      .closeTag("body")
                      .closeTag("html")
                      .build();

    cout << page;
}

int main() {
    demonstrateStringBuilder();
    return 0;
}
```

---

## 10. Exercises and Best Practices

### Exercise 1: Optimize String Processing

```cpp
// Task: Find and replace all occurrences
// Poor version
string replaceAll_poor(string text, const string& from, const string& to) {
    size_t pos = 0;
    while ((pos = text.find(from, pos)) != string::npos) {
        text.replace(pos, from.length(), to);  // Expensive!
        pos += to.length();
    }
    return text;
}

// Optimized version
string replaceAll_optimized(const string& text, const string& from, const string& to) {
    string result;
    result.reserve(text.length());  // At least this much

    size_t lastPos = 0;
    size_t pos = 0;

    while ((pos = text.find(from, lastPos)) != string::npos) {
        result.append(text, lastPos, pos - lastPos);
        result.append(to);
        lastPos = pos + from.length();
    }

    result.append(text, lastPos, string::npos);
    return result;
}
```

### Best Practices Summary

```cpp
/*
STRING OPTIMIZATION BEST PRACTICES:

1. AVOID UNNECESSARY COPIES
   ✓ Pass by const reference: void f(const string& s)
   ✓ Use string_view for read-only parameters
   ✓ Move when transferring ownership: return move(str)

2. RESERVE CAPACITY
   ✓ str.reserve(estimatedSize) before concatenation
   ✓ Avoids multiple reallocations
   ✓ Can be 10-100x faster

3. USE STRING_VIEW
   ✓ For parsing and tokenization
   ✓ Passing substrings without copy
   ✓ Be careful of lifetime!

4. PREFER += OVER +
   ✓ str += "text" instead of str = str + "text"
   ✓ Avoids creating temporary strings
   ✓ Better memory reuse

5. LEVERAGE SSO
   ✓ Keep strings < 15-23 chars when possible
   ✓ Short keys in maps benefit greatly
   ✓ Test your implementation's threshold

6. BATCH OPERATIONS
   ✓ Build once, use many times
   ✓ StringBuilder pattern for complex construction
   ✓ String pooling for duplicates

7. CHOOSE RIGHT ALGORITHM
   ✓ std::string::find() is usually best
   ✓ Boyer-Moore for long patterns
   ✓ KMP for many partial matches

8. MEASURE PERFORMANCE
   ✓ Profile before optimizing
   ✓ Benchmark alternatives
   ✓ Verify improvements
*/
```

---

## Key Takeaways

1. ✅ **Reserve capacity** to avoid reallocations
2. ✅ **Use string_view** for read-only access (zero-copy)
3. ✅ **Leverage SSO** for short strings
4. ✅ **Avoid operator+** for concatenation
5. ✅ **String pooling** saves memory with duplicates
6. ✅ **Choose efficient algorithms** for searching
7. ✅ **StringBuilder pattern** for complex construction
8. ✅ **Profile and measure** actual performance

---

## Summary Checklist

Before moving to Lesson 24, ensure you can:
- [ ] Explain why string operations can be slow
- [ ] Use reserve() to optimize concatenation
- [ ] Apply string_view for zero-copy operations
- [ ] Understand Small String Optimization
- [ ] Implement string pooling when beneficial
- [ ] Choose appropriate parsing techniques
- [ ] Select efficient search algorithms
- [ ] Build strings efficiently with StringBuilder pattern

**Congratulations on mastering String Optimization!**

Next lesson: Loop Optimization
