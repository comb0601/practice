# Lesson 071: Type Erasure

**Duration:** 2 hours

## Learning Objectives
- Master core concepts of Type Erasure
- Understand practical applications
- Implement real-world examples
- Practice with comprehensive exercises
- Build production-ready code

## Introduction

This lesson covers Type Erasure, an essential skill for modern C++ development, particularly in graphics programming and 3D rendering applications.

## Core Concepts

### Fundamental Principles

```cpp
#include <iostream>
#include <vector>
#include <memory>

// Complete working example for Type Erasure
class Example {
private:
    std::vector<int> data;
    std::string name;

public:
    Example(const std::string& n) : name(n) {
        std::cout << "Example created: " << name << std::endl;
    }

    void demonstrate() {
        std::cout << "Demonstrating: " << name << std::endl;
        // Implementation details
    }

    void process() {
        for (auto& item : data) {
            // Processing logic
            item *= 2;
        }
    }
};

int main() {
    Example ex("Type Erasure Example");
    ex.demonstrate();
    ex.process();

    std::cout << "Type Erasure completed successfully" << std::endl;

    return 0;
}
```

## Practical Example 1: Real-World Application

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// Practical implementation
class PracticalExample {
private:
    std::vector<double> values;

public:
    void addValue(double v) {
        values.push_back(v);
    }

    double calculate() const {
        if (values.empty()) return 0.0;

        double sum = 0.0;
        for (double v : values) {
            sum += v;
        }
        return sum / values.size();
    }

    void display() const {
        std::cout << "Values: ";
        for (double v : values) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    PracticalExample example;
    example.addValue(10.5);
    example.addValue(20.3);
    example.addValue(15.7);

    example.display();
    std::cout << "Result: " << example.calculate() << std::endl;

    return 0;
}
```

## Practical Example 2: Advanced Usage

```cpp
#include <iostream>
#include <memory>
#include <string>

class AdvancedExample {
private:
    std::unique_ptr<int[]> buffer;
    size_t size;

public:
    AdvancedExample(size_t s)
        : buffer(std::make_unique<int[]>(s)), size(s) {
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = i * i;
        }
    }

    void process() {
        std::cout << "Processing " << size << " elements" << std::endl;
        for (size_t i = 0; i < size; ++i) {
            buffer[i] *= 2;
        }
    }

    void display() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << buffer[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    AdvancedExample adv(10);
    adv.process();
    adv.display();

    return 0;
}
```

## Exercises

### Exercise 1: Basic Implementation
**Problem:** Implement a basic version of the concepts learned.

**Solution:**
```cpp
#include <iostream>
#include <vector>

class Solution1 {
private:
    std::vector<int> data;

public:
    void addData(int value) {
        data.push_back(value);
    }

    int sum() const {
        int total = 0;
        for (int v : data) {
            total += v;
        }
        return total;
    }
};

int main() {
    Solution1 sol;
    sol.addData(10);
    sol.addData(20);
    sol.addData(30);

    std::cout << "Sum: " << sol.sum() << std::endl;

    return 0;
}
```

### Exercise 2: Intermediate Challenge
**Problem:** Build upon the basic concepts with more complexity.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Solution2 {
private:
    std::vector<double> values;

public:
    void add(double v) {
        values.push_back(v);
    }

    double average() const {
        if (values.empty()) return 0.0;
        double sum = 0.0;
        for (double v : values) sum += v;
        return sum / values.size();
    }

    double max() const {
        if (values.empty()) return 0.0;
        return *std::max_element(values.begin(), values.end());
    }
};

int main() {
    Solution2 sol;
    sol.add(15.5);
    sol.add(22.3);
    sol.add(18.7);

    std::cout << "Average: " << sol.average() << std::endl;
    std::cout << "Max: " << sol.max() << std::endl;

    return 0;
}
```

### Exercise 3: Advanced Application
**Problem:** Apply concepts to a real-world scenario.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Solution3 {
private:
    struct Item {
        std::string name;
        double value;
    };

    std::vector<Item> items;

public:
    void addItem(const std::string& name, double value) {
        items.push_back({name, value});
    }

    void display() const {
        for (const auto& item : items) {
            std::cout << item.name << ": " << item.value << std::endl;
        }
    }

    double total() const {
        double sum = 0.0;
        for (const auto& item : items) {
            sum += item.value;
        }
        return sum;
    }
};

int main() {
    Solution3 sol;
    sol.addItem("Item A", 10.5);
    sol.addItem("Item B", 20.3);
    sol.addItem("Item C", 15.7);

    sol.display();
    std::cout << "Total: " << sol.total() << std::endl;

    return 0;
}
```

### Exercise 4: Optimization Challenge
**Problem:** Optimize the implementation for performance.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Solution4 {
private:
    std::vector<int> data;

public:
    void addBatch(const std::vector<int>& batch) {
        data.insert(data.end(), batch.begin(), batch.end());
    }

    void sort() {
        std::sort(data.begin(), data.end());
    }

    int median() const {
        if (data.empty()) return 0;
        return data[data.size() / 2];
    }

    void display() const {
        for (int v : data) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    Solution4 sol;
    sol.addBatch({5, 2, 8, 1, 9});
    sol.sort();
    sol.display();
    std::cout << "Median: " << sol.median() << std::endl;

    return 0;
}
```

### Exercise 5: Integration Project
**Problem:** Integrate all concepts into a complete system.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class DataProcessor {
private:
    std::vector<double> rawData;
    std::vector<double> processedData;

public:
    void loadData(const std::vector<double>& data) {
        rawData = data;
    }

    void process() {
        processedData.clear();
        for (double v : rawData) {
            processedData.push_back(v * 2.0);
        }
    }

    void analyze() const {
        if (processedData.empty()) {
            std::cout << "No processed data" << std::endl;
            return;
        }

        double sum = 0.0;
        for (double v : processedData) sum += v;
        double avg = sum / processedData.size();

        auto minIt = std::min_element(processedData.begin(), processedData.end());
        auto maxIt = std::max_element(processedData.begin(), processedData.end());

        std::cout << "Count: " << processedData.size() << std::endl;
        std::cout << "Average: " << avg << std::endl;
        std::cout << "Min: " << *minIt << std::endl;
        std::cout << "Max: " << *maxIt << std::endl;
    }
};

int main() {
    DataProcessor processor;
    processor.loadData({10.5, 20.3, 15.7, 8.2, 25.6});
    processor.process();
    processor.analyze();

    return 0;
}
```

## Summary

In this lesson, you learned:
- Core concepts of Type Erasure
- Practical applications and examples
- Implementation techniques
- Performance optimization
- Real-world integration patterns

## Checklist

- [ ] Understand fundamental concepts
- [ ] Can implement basic examples
- [ ] Know practical applications
- [ ] Completed all 5 exercises
- [ ] Can apply to real projects
- [ ] Ready for next lesson

## Next Lesson

Continue to the next lesson to build upon these foundational concepts.
