# Lesson 18: STL Containers

**Duration**: 2.5-3 hours

## Overview

STL containers are template classes that manage collections of objects. This lesson covers sequence containers (vector, deque, list), associative containers (map, set), and unordered containers (unordered_map, unordered_set).

## Topics Covered

1. Container Categories
2. Sequence Containers (vector, deque, list)
3. Associative Containers (set, map)
4. Unordered Containers (unordered_set, unordered_map)
5. Container Adapters (stack, queue, priority_queue)

## 1. Container Categories

**Sequence Containers**: Store elements in linear sequence
- `vector` - Dynamic array
- `deque` - Double-ended queue
- `list` - Doubly linked list

**Associative Containers**: Store sorted unique keys
- `set` - Sorted unique elements
- `map` - Sorted key-value pairs

**Unordered Containers**: Hash tables
- `unordered_set` - Hashed unique elements
- `unordered_map` - Hashed key-value pairs

## 2. Sequence Containers

### Example 1: vector, deque, list Comparison - Complete Program

**File: `sequence_containers.cpp`**

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <chrono>

template<typename Container>
void displayContainer(const Container& c, const std::string& name) {
    std::cout << name << ": ";
    for (const auto& val : c) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== Sequence Containers Demo ===" << std::endl;

    // 1. VECTOR - Dynamic array
    std::cout << "\n--- std::vector ---" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};

    vec.push_back(6);  // Add at end: O(1) amortized
    vec.insert(vec.begin() + 2, 99);  // Insert at position: O(n)
    displayContainer(vec, "Vector");

    std::cout << "Access vec[3]: " << vec[3] << std::endl;  // Random access: O(1)
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

    // 2. DEQUE - Double-ended queue
    std::cout << "\n--- std::deque ---" << std::endl;
    std::deque<int> deq = {10, 20, 30, 40, 50};

    deq.push_back(60);   // Add at end: O(1)
    deq.push_front(5);   // Add at front: O(1)
    displayContainer(deq, "Deque");

    std::cout << "Access deq[3]: " << deq[3] << std::endl;  // Random access: O(1)

    // 3. LIST - Doubly linked list
    std::cout << "\n--- std::list ---" << std::endl;
    std::list<int> lst = {100, 200, 300, 400, 500};

    lst.push_back(600);  // Add at end: O(1)
    lst.push_front(50);  // Add at front: O(1)

    // Insert at position (need iterator)
    auto it = lst.begin();
    std::advance(it, 2);
    lst.insert(it, 999);  // Insert: O(1) if you have iterator

    displayContainer(lst, "List");

    // List specific operations
    lst.reverse();
    displayContainer(lst, "List reversed");

    lst.sort();
    displayContainer(lst, "List sorted");

    // Performance comparison
    std::cout << "\n--- Performance Characteristics ---" << std::endl;
    std::cout << "Vector:   Random access O(1), Insert/delete end O(1), Insert middle O(n)" << std::endl;
    std::cout << "Deque:    Random access O(1), Insert/delete both ends O(1), Insert middle O(n)" << std::endl;
    std::cout << "List:     Random access O(n), Insert/delete anywhere O(1) with iterator" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall sequence_containers.cpp -o sequence_containers
./sequence_containers
```

## 3. Associative Containers

### Example 2: set and map - Complete Program

**File: `associative_containers.cpp`**

```cpp
#include <iostream>
#include <set>
#include <map>
#include <string>

int main() {
    std::cout << "=== Associative Containers Demo ===" << std::endl;

    // 1. SET - Sorted unique elements
    std::cout << "\n--- std::set ---" << std::endl;
    std::set<int> numbers = {5, 2, 8, 1, 9, 3, 5, 2};  // Duplicates ignored

    std::cout << "Set elements (sorted): ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Insert elements
    numbers.insert(7);
    numbers.insert(4);
    numbers.insert(1);  // Duplicate, will be ignored

    std::cout << "After insertions: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Find element
    if (numbers.find(5) != numbers.end()) {
        std::cout << "5 is in the set" << std::endl;
    }

    // Count element (returns 0 or 1 for set)
    std::cout << "Count of 3: " << numbers.count(3) << std::endl;
    std::cout << "Count of 99: " << numbers.count(99) << std::endl;

    // Erase element
    numbers.erase(8);
    std::cout << "After erasing 8: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 2. MAP - Sorted key-value pairs
    std::cout << "\n--- std::map ---" << std::endl;
    std::map<std::string, int> ages;

    // Insert elements
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages["Charlie"] = 28;
    ages.insert({"David", 35});
    ages.insert(std::make_pair("Eve", 27));

    std::cout << "Map contents:" << std::endl;
    for (const auto& [name, age] : ages) {
        std::cout << name << ": " << age << std::endl;
    }

    // Access elements
    std::cout << "\nAlice's age: " << ages["Alice"] << std::endl;

    // Check if key exists
    if (ages.find("Bob") != ages.end()) {
        std::cout << "Bob is in the map" << std::endl;
    }

    // Erase key
    ages.erase("Charlie");

    // Iterate
    std::cout << "\nAfter erasing Charlie:" << std::endl;
    for (const auto& pair : ages) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // multiset - allows duplicates
    std::cout << "\n--- std::multiset ---" << std::endl;
    std::multiset<int> mset = {5, 2, 8, 1, 9, 3, 5, 2};  // Duplicates allowed

    std::cout << "Multiset elements: ";
    for (int num : mset) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Count of 2: " << mset.count(2) << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall associative_containers.cpp -o associative_containers
./associative_containers
```

## 4. Unordered Containers

### Example 3: unordered_set and unordered_map - Complete Program

**File: `unordered_containers.cpp`**

```cpp
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>

int main() {
    std::cout << "=== Unordered Containers Demo ===" << std::endl;

    // 1. UNORDERED_SET - Hash set
    std::cout << "\n--- std::unordered_set ---" << std::endl;
    std::unordered_set<std::string> words = {"apple", "banana", "cherry", "date"};

    words.insert("elderberry");
    words.insert("fig");
    words.insert("apple");  // Duplicate, ignored

    std::cout << "Unordered set (not sorted): ";
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    // Fast lookup: O(1) average
    if (words.find("banana") != words.end()) {
        std::cout << "Found 'banana'" << std::endl;
    }

    std::cout << "Size: " << words.size() << std::endl;
    std::cout << "Bucket count: " << words.bucket_count() << std::endl;

    // 2. UNORDERED_MAP - Hash map
    std::cout << "\n--- std::unordered_map ---" << std::endl;
    std::unordered_map<std::string, double> prices;

    prices["apple"] = 1.50;
    prices["banana"] = 0.75;
    prices["cherry"] = 2.00;
    prices["date"] = 3.25;

    std::cout << "Product prices:" << std::endl;
    for (const auto& [product, price] : prices) {
        std::cout << product << ": $" << price << std::endl;
    }

    // Fast lookup: O(1) average
    std::cout << "\nPrice of banana: $" << prices["banana"] << std::endl;

    // Check existence
    std::string item = "grape";
    if (prices.find(item) != prices.end()) {
        std::cout << item << " found" << std::endl;
    } else {
        std::cout << item << " not found" << std::endl;
    }

    // Performance comparison
    std::cout << "\n--- Performance: set vs unordered_set ---" << std::endl;
    std::cout << "set:              Insert/find/erase O(log n), sorted order" << std::endl;
    std::cout << "unordered_set:    Insert/find/erase O(1) average, no order" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall unordered_containers.cpp -o unordered_containers
./unordered_containers
```

## 5. Container Adapters

### Example 4: stack, queue, priority_queue - Complete Program

**File: `container_adapters.cpp`**

```cpp
#include <iostream>
#include <stack>
#include <queue>
#include <string>

int main() {
    std::cout << "=== Container Adapters Demo ===" << std::endl;

    // 1. STACK - LIFO (Last In, First Out)
    std::cout << "\n--- std::stack ---" << std::endl;
    std::stack<int> stk;

    stk.push(10);
    stk.push(20);
    stk.push(30);
    stk.push(40);

    std::cout << "Stack size: " << stk.size() << std::endl;
    std::cout << "Top element: " << stk.top() << std::endl;

    std::cout << "Popping elements: ";
    while (!stk.empty()) {
        std::cout << stk.top() << " ";
        stk.pop();
    }
    std::cout << std::endl;

    // 2. QUEUE - FIFO (First In, First Out)
    std::cout << "\n--- std::queue ---" << std::endl;
    std::queue<std::string> q;

    q.push("First");
    q.push("Second");
    q.push("Third");
    q.push("Fourth");

    std::cout << "Queue size: " << q.size() << std::endl;
    std::cout << "Front: " << q.front() << ", Back: " << q.back() << std::endl;

    std::cout << "Processing queue: ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;

    // 3. PRIORITY_QUEUE - Max heap by default
    std::cout << "\n--- std::priority_queue ---" << std::endl;
    std::priority_queue<int> pq;

    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    pq.push(40);

    std::cout << "Priority queue (max heap): ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;

    // Min heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

    min_pq.push(30);
    min_pq.push(10);
    min_pq.push(50);
    min_pq.push(20);

    std::cout << "Priority queue (min heap): ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall container_adapters.cpp -o container_adapters
./container_adapters
```

## Complete Project: Inventory Management System

**File: `inventory_system.cpp`**

```cpp
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Product {
    std::string id;
    std::string name;
    std::string category;
    double price;
    int quantity;

    void display() const {
        std::cout << std::left << std::setw(10) << id
                  << std::setw(20) << name
                  << std::setw(15) << category
                  << "$" << std::setw(8) << std::fixed << std::setprecision(2) << price
                  << "Qty: " << quantity << std::endl;
    }
};

class InventorySystem {
private:
    std::unordered_map<std::string, Product> products;  // id -> product
    std::map<std::string, std::set<std::string>> categorized;  // category -> {ids}

public:
    void addProduct(const Product& product) {
        products[product.id] = product;
        categorized[product.category].insert(product.id);
        std::cout << "Added product: " << product.name << std::endl;
    }

    void removeProduct(const std::string& id) {
        auto it = products.find(id);
        if (it != products.end()) {
            categorized[it->second.category].erase(id);
            std::cout << "Removed product: " << it->second.name << std::endl;
            products.erase(it);
        } else {
            std::cout << "Product not found: " << id << std::endl;
        }
    }

    void updateQuantity(const std::string& id, int delta) {
        auto it = products.find(id);
        if (it != products.end()) {
            it->second.quantity += delta;
            std::cout << "Updated " << it->second.name
                      << " quantity to: " << it->second.quantity << std::endl;
        }
    }

    void displayAllProducts() const {
        std::cout << "\n=== All Products ===" << std::endl;
        std::cout << std::left << std::setw(10) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Category"
                  << std::setw(12) << "Price"
                  << "Quantity" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        for (const auto& [id, product] : products) {
            product.display();
        }
    }

    void displayByCategory(const std::string& category) const {
        std::cout << "\n=== Products in " << category << " ===" << std::endl;

        auto it = categorized.find(category);
        if (it == categorized.end() || it->second.empty()) {
            std::cout << "No products in this category" << std::endl;
            return;
        }

        for (const auto& id : it->second) {
            products.at(id).display();
        }
    }

    void displayLowStock(int threshold) const {
        std::cout << "\n=== Low Stock Products (< " << threshold << ") ===" << std::endl;

        std::vector<Product> lowStock;
        for (const auto& [id, product] : products) {
            if (product.quantity < threshold) {
                lowStock.push_back(product);
            }
        }

        if (lowStock.empty()) {
            std::cout << "No products with low stock" << std::endl;
            return;
        }

        std::sort(lowStock.begin(), lowStock.end(),
                 [](const Product& a, const Product& b) {
                     return a.quantity < b.quantity;
                 });

        for (const auto& product : lowStock) {
            product.display();
        }
    }

    void displayStatistics() const {
        std::cout << "\n=== Inventory Statistics ===" << std::endl;
        std::cout << "Total products: " << products.size() << std::endl;
        std::cout << "Categories: " << categorized.size() << std::endl;

        double totalValue = 0;
        int totalItems = 0;

        for (const auto& [id, product] : products) {
            totalValue += product.price * product.quantity;
            totalItems += product.quantity;
        }

        std::cout << "Total inventory value: $" << std::fixed
                  << std::setprecision(2) << totalValue << std::endl;
        std::cout << "Total items in stock: " << totalItems << std::endl;

        std::cout << "\nProducts per category:" << std::endl;
        for (const auto& [category, ids] : categorized) {
            std::cout << "  " << category << ": " << ids.size() << std::endl;
        }
    }

    Product* findProduct(const std::string& id) {
        auto it = products.find(id);
        return (it != products.end()) ? &it->second : nullptr;
    }
};

int main() {
    std::cout << "=== Inventory Management System ===" << std::endl;

    InventorySystem inventory;

    // Add products
    inventory.addProduct({"P001", "Laptop", "Electronics", 999.99, 15});
    inventory.addProduct({"P002", "Mouse", "Electronics", 29.99, 50});
    inventory.addProduct({"P003", "Desk", "Furniture", 299.99, 8});
    inventory.addProduct({"P004", "Chair", "Furniture", 199.99, 12});
    inventory.addProduct({"P005", "Notebook", "Stationery", 5.99, 100});
    inventory.addProduct({"P006", "Pen", "Stationery", 1.99, 200});
    inventory.addProduct({"P007", "Monitor", "Electronics", 399.99, 3});

    // Display all products
    inventory.displayAllProducts();

    // Display statistics
    inventory.displayStatistics();

    // Display by category
    inventory.displayByCategory("Electronics");
    inventory.displayByCategory("Furniture");

    // Update quantities
    inventory.updateQuantity("P002", -10);  // Sell 10 mice
    inventory.updateQuantity("P007", -2);   // Sell 2 monitors

    // Display low stock
    inventory.displayLowStock(10);

    // Find and display specific product
    std::cout << "\n=== Product Lookup ===" << std::endl;
    Product* found = inventory.findProduct("P001");
    if (found) {
        std::cout << "Found: ";
        found->display();
    }

    // Remove product
    inventory.removeProduct("P006");

    // Final statistics
    inventory.displayStatistics();

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall inventory_system.cpp -o inventory_system
./inventory_system
```

## Container Selection Guide

| Need | Container |
|------|-----------|
| Fast random access | `vector`, `deque` |
| Fast insertion/deletion at ends | `deque`, `list` |
| Fast insertion/deletion anywhere | `list` |
| Sorted unique elements | `set` |
| Sorted key-value pairs | `map` |
| Fast lookup (unordered) | `unordered_set`, `unordered_map` |
| LIFO operations | `stack` |
| FIFO operations | `queue` |
| Priority-based access | `priority_queue` |

## Summary

In this lesson, you learned:

- **Sequence containers** - vector, deque, list and their trade-offs
- **Associative containers** - set, map with sorted elements
- **Unordered containers** - unordered_set, unordered_map with hash tables
- **Container adapters** - stack, queue, priority_queue
- **Container selection** - Choosing the right container for your needs

## Next Lesson

Lesson 19: STL Algorithms

## Additional Resources

- C++ Reference: Containers
- Effective STL by Scott Meyers
- C++ Performance: Choosing the Right Container

---

**Exercise**: Extend the inventory system to support transactions, sales history tracking, and automatic reorder suggestions.
