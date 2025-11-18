# Lesson 55: Map and Multimap Containers

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Maps
2. Map Container Basics
3. Accessing and Modifying Elements
4. Map Operations
5. Multimap Container
6. Custom Key Types and Comparators
7. Map Algorithms
8. Exercises and Projects

---

## 1. Introduction to Maps

A **map** is an associative container that stores key-value pairs. Each key is unique and maps to exactly one value. Elements are sorted by key.

### Map vs Set:

```
Set:
- Stores single values
- set<int>

Map:
- Stores key-value pairs
- map<string, int>  // key=string, value=int
```

### Real-World Examples:

```cpp
/*
Dictionary: word (key) → definition (value)
Phone book: name (key) → phone number (value)
Student database: ID (key) → student info (value)
Configuration: setting name (key) → setting value (value)
*/
```

---

## 2. Map Container Basics

### Creating Maps:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    // Empty map
    std::map<std::string, int> ages;

    // Initialize with pairs
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // Using make_pair
    std::map<int, std::string> students;
    students.insert(std::make_pair(101, "John"));
    students.insert(std::make_pair(102, "Jane"));

    // Display map
    std::cout << "Scores:\n";
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    return 0;
}
```

**Output:**
```
Scores:
Alice: 95
Bob: 87
Charlie: 92
```

### Map with Different Types:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    // String to int
    std::map<std::string, int> wordCount = {
        {"hello", 5},
        {"world", 3}
    };

    // Int to string
    std::map<int, std::string> idToName = {
        {1, "Alice"},
        {2, "Bob"}
    };

    // String to string
    std::map<std::string, std::string> translations = {
        {"hello", "hola"},
        {"goodbye", "adiós"}
    };

    // String to double
    std::map<std::string, double> prices = {
        {"Apple", 0.99},
        {"Banana", 0.59}
    };

    return 0;
}
```

---

## 3. Accessing and Modifying Elements

### Using operator[]:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> ages;

    // Insert using []
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages["Charlie"] = 35;

    // Access using []
    std::cout << "Alice's age: " << ages["Alice"] << "\n";

    // Modify using []
    ages["Alice"] = 31;
    std::cout << "Alice's new age: " << ages["Alice"] << "\n";

    // WARNING: [] creates element if doesn't exist!
    std::cout << "David's age: " << ages["David"] << "\n";  // Creates entry with value 0
    std::cout << "Map size: " << ages.size() << "\n";  // Now 4 elements!

    return 0;
}
```

### Using at() for Safe Access:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25}
    };

    // at() with bounds checking
    std::cout << "Alice's age: " << ages.at("Alice") << "\n";

    // Throws exception if key doesn't exist
    try {
        std::cout << ages.at("Charlie");  // Exception!
    }
    catch (const std::out_of_range& e) {
        std::cout << "Key not found: " << e.what() << "\n";
    }

    // Modify with at()
    ages.at("Alice") = 31;
    std::cout << "Alice's new age: " << ages.at("Alice") << "\n";

    return 0;
}
```

### Checking if Key Exists:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87}
    };

    // Method 1: find()
    if (scores.find("Alice") != scores.end()) {
        std::cout << "Alice found with score: " << scores["Alice"] << "\n";
    }

    // Method 2: count()
    if (scores.count("Charlie") > 0) {
        std::cout << "Charlie found\n";
    } else {
        std::cout << "Charlie not found\n";
    }

    // Method 3: contains() - C++20
    #ifdef __cpp_lib_map_contains
    if (scores.contains("Bob")) {
        std::cout << "Bob found\n";
    }
    #endif

    return 0;
}
```

---

## 4. Map Operations

### Inserting Elements:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> m;

    // Method 1: operator[]
    m["key1"] = 100;

    // Method 2: insert with pair
    m.insert(std::make_pair("key2", 200));
    m.insert({"key3", 300});  // C++11

    // Method 3: insert with hint
    auto it = m.begin();
    m.insert(it, {"key4", 400});

    // Method 4: emplace (construct in place)
    m.emplace("key5", 500);

    // insert returns pair<iterator, bool>
    auto result = m.insert({"key1", 999});
    if (result.second) {
        std::cout << "Inserted\n";
    } else {
        std::cout << "Key already exists, value not changed\n";
    }

    std::cout << "key1 value: " << m["key1"] << "\n";  // Still 100

    // insert_or_assign() - C++17
    #ifdef __cpp_lib_map_insert_or_assign
    m.insert_or_assign("key1", 999);
    std::cout << "key1 after insert_or_assign: " << m["key1"] << "\n";  // Now 999
    #endif

    // Display all
    for (const auto& [key, value] : m) {  // Structured binding C++17
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}
```

### Erasing Elements:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> m = {
        {"A", 1}, {"B", 2}, {"C", 3}, {"D", 4}, {"E", 5}
    };

    std::cout << "Original size: " << m.size() << "\n";

    // erase() by key
    m.erase("C");
    std::cout << "After erasing C, size: " << m.size() << "\n";

    // erase() by iterator
    auto it = m.find("B");
    if (it != m.end()) {
        m.erase(it);
    }
    std::cout << "After erasing B, size: " << m.size() << "\n";

    // erase() range
    auto first = m.find("A");
    auto last = m.find("E");
    m.erase(first, last);  // Erase [A, E)

    std::cout << "Remaining elements: ";
    for (const auto& pair : m) {
        std::cout << pair.first << " ";
    }
    std::cout << "\n";

    // clear() all elements
    m.clear();
    std::cout << "After clear, size: " << m.size() << "\n";

    return 0;
}
```

### Iterating Through Maps:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // Method 1: Range-based for with pair
    std::cout << "Method 1:\n";
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    // Method 2: Structured binding (C++17)
    std::cout << "\nMethod 2:\n";
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << "\n";
    }

    // Method 3: Iterator
    std::cout << "\nMethod 3:\n";
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // Reverse iteration
    std::cout << "\nReverse:\n";
    for (auto it = scores.rbegin(); it != scores.rend(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    return 0;
}
```

---

## 5. Multimap Container

### What is a Multimap?

A **multimap** allows duplicate keys. Each key can map to multiple values.

### Multimap Operations:

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    // Multimap allows duplicate keys
    std::multimap<std::string, int> grades;

    // Insert same key multiple times
    grades.insert({"Alice", 85});
    grades.insert({"Alice", 90});
    grades.insert({"Alice", 88});
    grades.insert({"Bob", 75});
    grades.insert({"Bob", 80});

    // Display all
    std::cout << "All grades:\n";
    for (const auto& [name, grade] : grades) {
        std::cout << name << ": " << grade << "\n";
    }

    // Count occurrences of key
    std::cout << "\nAlice has " << grades.count("Alice") << " grades\n";

    // Find all values for a key
    std::cout << "\nAlice's grades:\n";
    auto range = grades.equal_range("Alice");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "  " << it->second << "\n";
    }

    // NOTE: operator[] is NOT available for multimap!
    // Must use insert() or emplace()

    return 0;
}
```

### Multimap Example - Phone Book:

```cpp
#include <iostream>
#include <map>
#include <string>

class PhoneBook {
private:
    std::multimap<std::string, std::string> contacts;

public:
    void addContact(const std::string& name, const std::string& number) {
        contacts.insert({name, number});
        std::cout << "Added: " << name << " - " << number << "\n";
    }

    void findContact(const std::string& name) const {
        auto range = contacts.equal_range(name);
        if (range.first == range.second) {
            std::cout << name << " not found\n";
            return;
        }

        std::cout << name << "'s numbers:\n";
        for (auto it = range.first; it != range.second; ++it) {
            std::cout << "  " << it->second << "\n";
        }
    }

    void displayAll() const {
        std::cout << "\n=== Phone Book ===\n";
        for (const auto& [name, number] : contacts) {
            std::cout << name << ": " << number << "\n";
        }
    }
};

int main()
{
    PhoneBook book;

    book.addContact("Alice", "555-1111");
    book.addContact("Alice", "555-2222");  // Second number
    book.addContact("Bob", "555-3333");
    book.addContact("Charlie", "555-4444");

    book.displayAll();

    std::cout << "\n";
    book.findContact("Alice");

    return 0;
}
```

---

## 6. Custom Key Types and Comparators

### Map with Struct Key:

```cpp
#include <iostream>
#include <map>
#include <string>

struct Date {
    int year, month, day;

    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    // Required: comparison operator
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
};

int main()
{
    std::map<Date, std::string> events;

    events.insert({{2024, 1, 1}, "New Year"});
    events.insert({{2024, 7, 4}, "Independence Day"});
    events.insert({{2024, 12, 25}, "Christmas"});

    std::cout << "Events:\n";
    for (const auto& [date, event] : events) {
        std::cout << date.year << "-" << date.month << "-" << date.day
                  << ": " << event << "\n";
    }

    return 0;
}
```

### Custom Comparator:

```cpp
#include <iostream>
#include <map>
#include <string>

// Case-insensitive string comparison
struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char c1, char c2) {
                return std::tolower(c1) < std::tolower(c2);
            }
        );
    }
};

int main()
{
    std::map<std::string, int, CaseInsensitiveCompare> wordCount;

    wordCount["Hello"] = 1;
    wordCount["WORLD"] = 2;
    wordCount["hello"] = 3;  // Overwrites "Hello"

    std::cout << "Word count:\n";
    for (const auto& [word, count] : wordCount) {
        std::cout << word << ": " << count << "\n";
    }

    std::cout << "\nSize: " << wordCount.size() << "\n";  // Only 2 entries

    return 0;
}
```

---

## 7. Map Algorithms

### Merging Maps:

```cpp
#include <iostream>
#include <map>

int main()
{
    std::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
    std::map<int, std::string> m2 = {{3, "three"}, {4, "four"}};

    // Merge m2 into m1
    m1.insert(m2.begin(), m2.end());

    std::cout << "Merged map:\n";
    for (const auto& [key, value] : m1) {
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}
```

### Finding Min/Max Keys:

```cpp
#include <iostream>
#include <map>

int main()
{
    std::map<int, std::string> m = {
        {10, "ten"},
        {5, "five"},
        {20, "twenty"},
        {15, "fifteen"}
    };

    // Maps are sorted by key
    // First element has minimum key
    std::cout << "Min key: " << m.begin()->first << "\n";

    // Last element has maximum key
    std::cout << "Max key: " << m.rbegin()->first << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Character Frequency

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::string text = "hello world";
    std::map<char, int> freq;

    // Count frequency
    for (char c : text) {
        if (c != ' ') {
            freq[c]++;
        }
    }

    std::cout << "Character frequencies:\n";
    for (const auto& [ch, count] : freq) {
        std::cout << ch << ": " << count << "\n";
    }

    return 0;
}
```

### Exercise 2: Student Grade Management

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <numeric>

class GradeBook {
private:
    std::map<std::string, std::vector<int>> grades;

public:
    void addGrade(const std::string& student, int grade) {
        grades[student].push_back(grade);
        std::cout << "Added grade " << grade << " for " << student << "\n";
    }

    double getAverage(const std::string& student) const {
        auto it = grades.find(student);
        if (it == grades.end() || it->second.empty()) {
            return 0.0;
        }

        double sum = std::accumulate(it->second.begin(), it->second.end(), 0.0);
        return sum / it->second.size();
    }

    void displayGrades(const std::string& student) const {
        auto it = grades.find(student);
        if (it == grades.end()) {
            std::cout << student << " not found\n";
            return;
        }

        std::cout << student << "'s grades: ";
        for (int grade : it->second) {
            std::cout << grade << " ";
        }
        std::cout << " (Avg: " << getAverage(student) << ")\n";
    }

    void displayAll() const {
        std::cout << "\n=== Grade Book ===\n";
        for (const auto& [student, gradeList] : grades) {
            std::cout << student << ": ";
            for (int grade : gradeList) {
                std::cout << grade << " ";
            }
            std::cout << "(Avg: " << getAverage(student) << ")\n";
        }
    }
};

int main()
{
    GradeBook book;

    book.addGrade("Alice", 85);
    book.addGrade("Alice", 90);
    book.addGrade("Alice", 88);
    book.addGrade("Bob", 75);
    book.addGrade("Bob", 80);
    book.addGrade("Charlie", 95);

    book.displayAll();

    std::cout << "\n";
    book.displayGrades("Alice");

    return 0;
}
```

### Project: Inventory Management System

```cpp
#include <iostream>
#include <map>
#include <string>
#include <iomanip>

struct Product {
    std::string name;
    double price;
    int quantity;

    Product(std::string n = "", double p = 0.0, int q = 0)
        : name(n), price(p), quantity(q) {}

    void display() const {
        std::cout << std::left << std::setw(20) << name
                  << " $" << std::setw(8) << std::fixed << std::setprecision(2) << price
                  << " Qty: " << quantity << "\n";
    }
};

class Inventory {
private:
    std::map<int, Product> products;  // ID -> Product
    int nextId = 1000;

public:
    int addProduct(const std::string& name, double price, int quantity) {
        int id = nextId++;
        products[id] = Product(name, price, quantity);
        std::cout << "Added product ID " << id << ": " << name << "\n";
        return id;
    }

    void updateQuantity(int id, int quantity) {
        auto it = products.find(id);
        if (it != products.end()) {
            it->second.quantity = quantity;
            std::cout << "Updated quantity for ID " << id << "\n";
        } else {
            std::cout << "Product ID " << id << " not found\n";
        }
    }

    void updatePrice(int id, double price) {
        auto it = products.find(id);
        if (it != products.end()) {
            it->second.price = price;
            std::cout << "Updated price for ID " << id << "\n";
        } else {
            std::cout << "Product ID " << id << " not found\n";
        }
    }

    void removeProduct(int id) {
        if (products.erase(id)) {
            std::cout << "Removed product ID " << id << "\n";
        } else {
            std::cout << "Product ID " << id << " not found\n";
        }
    }

    void displayAll() const {
        std::cout << "\n=== Inventory ===\n";
        std::cout << std::left << std::setw(6) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << "Quantity\n";
        std::cout << std::string(50, '-') << "\n";

        for (const auto& [id, product] : products) {
            std::cout << std::setw(6) << id;
            product.display();
        }

        std::cout << "\nTotal products: " << products.size() << "\n";
    }

    double getTotalValue() const {
        double total = 0.0;
        for (const auto& [id, product] : products) {
            total += product.price * product.quantity;
        }
        return total;
    }

    void displayLowStock(int threshold) const {
        std::cout << "\n=== Low Stock (< " << threshold << ") ===\n";
        bool found = false;
        for (const auto& [id, product] : products) {
            if (product.quantity < threshold) {
                std::cout << "ID " << id << ": ";
                product.display();
                found = true;
            }
        }
        if (!found) {
            std::cout << "No low stock items\n";
        }
    }
};

int main()
{
    Inventory inv;

    int id1 = inv.addProduct("Laptop", 999.99, 10);
    int id2 = inv.addProduct("Mouse", 29.99, 50);
    int id3 = inv.addProduct("Keyboard", 79.99, 5);
    int id4 = inv.addProduct("Monitor", 299.99, 15);

    inv.displayAll();

    std::cout << "\nTotal inventory value: $"
              << std::fixed << std::setprecision(2)
              << inv.getTotalValue() << "\n";

    inv.displayLowStock(10);

    inv.updateQuantity(id3, 25);
    inv.updatePrice(id1, 899.99);

    inv.displayAll();

    inv.removeProduct(id2);
    inv.displayAll();

    return 0;
}
```

### Project: Word Translation Dictionary

```cpp
#include <iostream>
#include <map>
#include <string>

class Dictionary {
private:
    std::map<std::string, std::string> translations;

public:
    void addWord(const std::string& english, const std::string& spanish) {
        translations[english] = spanish;
        std::cout << "Added: " << english << " → " << spanish << "\n";
    }

    void translate(const std::string& word) const {
        auto it = translations.find(word);
        if (it != translations.end()) {
            std::cout << word << " in Spanish: " << it->second << "\n";
        } else {
            std::cout << "Translation not found for: " << word << "\n";
        }
    }

    void displayAll() const {
        std::cout << "\n=== Dictionary ===\n";
        for (const auto& [english, spanish] : translations) {
            std::cout << english << " → " << spanish << "\n";
        }
        std::cout << "\nTotal words: " << translations.size() << "\n";
    }

    void searchByPrefix(const std::string& prefix) const {
        std::cout << "\nWords starting with '" << prefix << "':\n";
        bool found = false;

        // Use lower_bound to find first word >= prefix
        auto it = translations.lower_bound(prefix);

        // Continue while key starts with prefix
        while (it != translations.end() &&
               it->first.substr(0, prefix.length()) == prefix) {
            std::cout << it->first << " → " << it->second << "\n";
            found = true;
            ++it;
        }

        if (!found) {
            std::cout << "No words found\n";
        }
    }
};

int main()
{
    Dictionary dict;

    dict.addWord("hello", "hola");
    dict.addWord("goodbye", "adiós");
    dict.addWord("house", "casa");
    dict.addWord("car", "coche");
    dict.addWord("cat", "gato");
    dict.addWord("dog", "perro");
    dict.addWord("happy", "feliz");
    dict.addWord("help", "ayuda");

    dict.displayAll();

    std::cout << "\n";
    dict.translate("hello");
    dict.translate("world");

    dict.searchByPrefix("h");
    dict.searchByPrefix("ca");

    return 0;
}
```

---

## Key Takeaways

1. ✅ Map stores key-value pairs with unique keys
2. ✅ Multimap allows duplicate keys
3. ✅ Elements are sorted by key automatically
4. ✅ Use [] for insertion/access (creates if doesn't exist)
5. ✅ Use at() for safe access with exception
6. ✅ Use find() or count() to check existence
7. ✅ Both provide O(log n) operations
8. ✅ Perfect for lookups, dictionaries, databases
9. ✅ Can use custom key types with operator<
10. ✅ Multimap requires equal_range() for all values

---

## Summary Checklist

Before moving to Lesson 56, ensure you can:
- [ ] Create and initialize maps and multimaps
- [ ] Insert and access elements using [] and at()
- [ ] Check if keys exist with find() and count()
- [ ] Erase elements by key or iterator
- [ ] Iterate through maps using range-based for
- [ ] Use structured bindings for cleaner code
- [ ] Work with multimaps and equal_range()
- [ ] Create custom key types with operator<
- [ ] Apply maps to real-world problems
- [ ] Choose between map and multimap appropriately

---

## Next Lesson Preview

In **Lesson 56**, we'll learn about:
- Unordered containers (hash tables)
- unordered_set and unordered_map
- Hash functions and collision handling
- Performance comparison with ordered containers
- When to use unordered vs ordered

**Excellent work mastering maps!**
