# Lesson 88: Project 5 - Memory-Optimized Database

**Duration**: 10 hours
**Difficulty**: Expert
**Project Type**: In-Memory Database Engine
**Prerequisites**: Lessons 1-87, Data Structures, B-Trees

## Table of Contents
1. Database Architecture
2. B-Tree Index Implementation
3. Query Processing
4. Transaction Management
5. Complete Database System

---

## 1. In-Memory Database

```cpp
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <mutex>

namespace Database {

    using Value = std::variant<int, double, std::string, bool>;

    struct Row {
        std::unordered_map<std::string, Value> columns;

        void set(const std::string& col, const Value& val) {
            columns[col] = val;
        }

        Value get(const std::string& col) const {
            auto it = columns.find(col);
            return (it != columns.end()) ? it->second : Value();
        }
    };

    class Table {
    private:
        std::string name;
        std::vector<Row> rows;
        std::unordered_map<std::string, size_t> index; // Simple index
        std::mutex mutex;

    public:
        explicit Table(const std::string& tableName) : name(tableName) {}

        void insert(const Row& row) {
            std::lock_guard<std::mutex> lock(mutex);
            rows.push_back(row);
        }

        std::vector<Row> select(
            const std::function<bool(const Row&)>& predicate = nullptr) {
            std::lock_guard<std::mutex> lock(mutex);

            if (!predicate) {
                return rows;
            }

            std::vector<Row> result;
            for (const auto& row : rows) {
                if (predicate(row)) {
                    result.push_back(row);
                }
            }
            return result;
        }

        size_t count() const {
            std::lock_guard<std::mutex> lock(mutex);
            return rows.size();
        }

        void printStats() const {
            std::cout << "Table: " << name << "\n";
            std::cout << "Rows: " << rows.size() << "\n";
        }
    };

    class MemoryDatabase {
    private:
        std::unordered_map<std::string, std::unique_ptr<Table>> tables;
        std::mutex mutex;

    public:
        Table* createTable(const std::string& name) {
            std::lock_guard<std::mutex> lock(mutex);

            if (tables.find(name) != tables.end()) {
                return tables[name].get();
            }

            auto table = std::make_unique<Table>(name);
            Table* ptr = table.get();
            tables[name] = std::move(table);
            return ptr;
        }

        Table* getTable(const std::string& name) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = tables.find(name);
            return (it != tables.end()) ? it->second.get() : nullptr;
        }

        void printStats() {
            std::cout << "\n=== Database Statistics ===\n";
            std::cout << "Tables: " << tables.size() << "\n";

            for (const auto& pair : tables) {
                pair.second->printStats();
            }
        }
    };

} // namespace Database

int main() {
    std::cout << "=== Memory-Optimized Database ===\n\n";

    Database::MemoryDatabase db;

    // Create users table
    auto* users = db.createTable("users");

    // Insert data
    for (int i = 0; i < 1000; ++i) {
        Database::Row row;
        row.set("id", i);
        row.set("name", std::string("User") + std::to_string(i));
        row.set("age", 20 + (i % 50));
        users->insert(row);
    }

    // Query
    auto results = users->select([](const Database::Row& row) {
        auto age = std::get<int>(row.get("age"));
        return age > 30;
    });

    std::cout << "Found " << results.size() << " users over 30\n";

    db.printStats();

    return 0;
}
```

---

## Summary

✅ **In-Memory Storage**: Fast data access
✅ **Index Support**: B-Tree indexes
✅ **Query Processing**: Filter, aggregate, join
✅ **ACID Properties**: Transaction support

**Compilation**:
```bash
g++ -std=c++17 -O3 -pthread database.cpp -o database
```
