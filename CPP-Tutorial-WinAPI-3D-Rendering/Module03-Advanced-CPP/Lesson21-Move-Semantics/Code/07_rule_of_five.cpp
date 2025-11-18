/*
 * Lesson 21: Move Semantics
 * File: 07_rule_of_five.cpp
 *
 * The Rule of Five
 *
 * This program demonstrates the Rule of Five: if you define any of the five
 * special member functions, you should consider defining all of them.
 *
 * The Five: Destructor, Copy Constructor, Copy Assignment,
 *          Move Constructor, Move Assignment
 *
 * Compile: g++ -std=c++17 -Wall 07_rule_of_five.cpp -o 07_rule_of_five
 */

#include <iostream>
#include <cstring>
#include <algorithm>

// Complete implementation following Rule of Five
class ResourceManager {
private:
    char* buffer;
    size_t capacity;
    std::string name;

public:
    // Constructor
    ResourceManager(const std::string& n, size_t cap = 100)
        : capacity(cap), name(n) {
        buffer = new char[capacity];
        std::fill(buffer, buffer + capacity, 'X');
        std::cout << "  [" << name << "] Constructor (capacity=" << capacity << ")\n";
    }

    // 1. Destructor
    ~ResourceManager() {
        std::cout << "  [" << name << "] Destructor\n";
        delete[] buffer;
    }

    // 2. Copy Constructor
    ResourceManager(const ResourceManager& other)
        : capacity(other.capacity), name(other.name + "_copy") {
        buffer = new char[capacity];
        std::memcpy(buffer, other.buffer, capacity);
        std::cout << "  [" << name << "] Copy Constructor\n";
    }

    // 3. Copy Assignment Operator
    ResourceManager& operator=(const ResourceManager& other) {
        std::cout << "  [" << name << "] Copy Assignment from [" << other.name << "]\n";

        if (this != &other) {
            // Clean up current resource
            delete[] buffer;

            // Copy from other
            capacity = other.capacity;
            name = other.name + "_assigned";
            buffer = new char[capacity];
            std::memcpy(buffer, other.buffer, capacity);
        }
        return *this;
    }

    // 4. Move Constructor
    ResourceManager(ResourceManager&& other) noexcept
        : buffer(other.buffer), capacity(other.capacity), name(std::move(other.name)) {
        std::cout << "  [" << name << "] Move Constructor\n";

        // Leave other in valid state
        other.buffer = nullptr;
        other.capacity = 0;
    }

    // 5. Move Assignment Operator
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        std::cout << "  [" << name << "] Move Assignment from [" << other.name << "]\n";

        if (this != &other) {
            // Clean up current resource
            delete[] buffer;

            // Transfer ownership
            buffer = other.buffer;
            capacity = other.capacity;
            name = std::move(other.name);

            // Leave other in valid state
            other.buffer = nullptr;
            other.capacity = 0;
        }
        return *this;
    }

    // Helper method
    void print() const {
        std::cout << "  [" << name << "] capacity=" << capacity
                  << ", buffer=" << (buffer ? "valid" : "nullptr") << "\n";
    }
};

// Rule of Zero example: let compiler generate everything
class RuleOfZero {
private:
    std::string name;
    std::vector<int> data;

public:
    RuleOfZero(const std::string& n, size_t size)
        : name(n), data(size) {
        std::cout << "  [" << name << "] Rule of Zero constructor\n";
    }

    // No need to define any special members!
    // Compiler generates optimal versions automatically

    void print() const {
        std::cout << "  [" << name << "] size=" << data.size() << "\n";
    }
};

// Class violating Rule of Five (only defines some)
class BadClass {
private:
    int* data;

public:
    BadClass() : data(new int(42)) {
        std::cout << "  BadClass constructed\n";
    }

    // Has destructor but no move operations (loses efficiency)
    ~BadClass() {
        std::cout << "  BadClass destroyed\n";
        delete data;
    }

    // Has copy constructor but no move constructor
    // (moves will use copy constructor instead)
    BadClass(const BadClass& other) : data(new int(*other.data)) {
        std::cout << "  BadClass copied (should have move!)\n";
    }

    // Missing: Copy assignment, Move constructor, Move assignment
};

// Modern approach: explicitly default or delete
class ModernClass {
private:
    int* data;
    size_t size;

public:
    ModernClass(size_t s) : size(s), data(new int[size]) {
        std::cout << "  ModernClass constructed\n";
    }

    ~ModernClass() {
        std::cout << "  ModernClass destroyed\n";
        delete[] data;
    }

    // Explicitly declare what we want
    ModernClass(const ModernClass& other) : size(other.size), data(new int[size]) {
        std::copy(other.data, other.data + size, data);
        std::cout << "  ModernClass copied\n";
    }

    ModernClass& operator=(const ModernClass& other) {
        std::cout << "  ModernClass copy assigned\n";
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    ModernClass(ModernClass&& other) noexcept
        : data(other.data), size(other.size) {
        std::cout << "  ModernClass moved\n";
        other.data = nullptr;
        other.size = 0;
    }

    ModernClass& operator=(ModernClass&& other) noexcept {
        std::cout << "  ModernClass move assigned\n";
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

// Move-only class: delete copy operations
class MoveOnly {
private:
    std::unique_ptr<int> data;
    std::string name;

public:
    MoveOnly(const std::string& n) : data(std::make_unique<int>(42)), name(n) {
        std::cout << "  [" << name << "] MoveOnly constructed\n";
    }

    ~MoveOnly() {
        std::cout << "  [" << name << "] MoveOnly destroyed\n";
    }

    // Delete copy operations
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    // Default move operations (or implement explicitly)
    MoveOnly(MoveOnly&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name)) {
        std::cout << "  [" << name << "] MoveOnly moved\n";
    }

    MoveOnly& operator=(MoveOnly&& other) noexcept {
        std::cout << "  [" << name << "] MoveOnly move assigned\n";
        data = std::move(other.data);
        name = std::move(other.name);
        return *this;
    }
};

int main() {
    std::cout << "=== Rule of Five ===\n\n";

    // 1. Complete Rule of Five
    std::cout << "1. RULE OF FIVE - ALL OPERATIONS:\n";
    {
        ResourceManager r1("Original", 50);

        std::cout << "\nCopy constructor:\n";
        ResourceManager r2 = r1;

        std::cout << "\nMove constructor:\n";
        ResourceManager r3 = std::move(r1);

        std::cout << "\nCopy assignment:\n";
        r2 = r3;

        std::cout << "\nMove assignment:\n";
        r2 = std::move(r3);

        std::cout << "\nFinal states:\n";
        r1.print();
        r2.print();
        r3.print();

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 2. Rule of Zero
    std::cout << "2. RULE OF ZERO (PREFERRED):\n";
    {
        std::cout << "Using standard library containers:\n";
        RuleOfZero z1("Zero1", 10);

        std::cout << "\nCopy (auto-generated):\n";
        RuleOfZero z2 = z1;

        std::cout << "\nMove (auto-generated):\n";
        RuleOfZero z3 = std::move(z1);

        z1.print();
        z2.print();
        z3.print();

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 3. Violating Rule of Five
    std::cout << "3. VIOLATING RULE OF FIVE (BAD):\n";
    {
        BadClass b1;

        std::cout << "\nCopy works:\n";
        BadClass b2 = b1;

        std::cout << "\nMove uses copy (inefficient!):\n";
        BadClass b3 = std::move(b1);  // Calls copy constructor!

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 4. Modern approach with explicit defaults/deletes
    std::cout << "4. MODERN APPROACH:\n";
    {
        ModernClass m1(10);

        std::cout << "\nCopy:\n";
        ModernClass m2 = m1;

        std::cout << "\nMove:\n";
        ModernClass m3 = std::move(m1);

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 5. Move-only class
    std::cout << "5. MOVE-ONLY CLASS:\n";
    {
        MoveOnly mo1("MoveOnly1");

        // MoveOnly mo2 = mo1;  // ERROR: copy deleted

        std::cout << "\nMove construction:\n";
        MoveOnly mo2 = std::move(mo1);

        std::cout << "\nMove assignment:\n";
        MoveOnly mo3("MoveOnly3");
        mo3 = std::move(mo2);

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 6. The five special members
    std::cout << "6. THE FIVE SPECIAL MEMBERS:\n";
    std::cout << "1. Destructor:         ~T()\n";
    std::cout << "2. Copy Constructor:   T(const T&)\n";
    std::cout << "3. Copy Assignment:    T& operator=(const T&)\n";
    std::cout << "4. Move Constructor:   T(T&&) noexcept\n";
    std::cout << "5. Move Assignment:    T& operator=(T&&) noexcept\n\n";

    // 7. When to define them
    std::cout << "7. WHEN TO DEFINE:\n";
    std::cout << "Define all five if:\n";
    std::cout << "  - Managing resources manually (raw pointers, handles)\n";
    std::cout << "  - Need custom copy/move behavior\n";
    std::cout << "  - Performance-critical code\n\n";

    std::cout << "Follow Rule of Zero if:\n";
    std::cout << "  - Using standard containers/smart pointers\n";
    std::cout << "  - Default behavior is correct\n";
    std::cout << "  - Preferred approach when possible\n\n";

    // 8. Compiler-generated versions
    std::cout << "8. COMPILER-GENERATED VERSIONS:\n";
    std::cout << "Destructor: default if not defined\n";
    std::cout << "Copy constructor: default if no move operations defined\n";
    std::cout << "Copy assignment: default if no move operations defined\n";
    std::cout << "Move constructor: default if no special members defined\n";
    std::cout << "Move assignment: default if no special members defined\n\n";

    std::cout << "Warning: Defining ANY special member prevents automatic\n";
    std::cout << "generation of move operations!\n\n";

    // 9. = default and = delete
    std::cout << "9. EXPLICIT DEFAULT AND DELETE:\n";
    std::cout << "class MyClass {\n";
    std::cout << "    MyClass() = default;                  // Use default\n";
    std::cout << "    MyClass(const MyClass&) = delete;     // Prevent copy\n";
    std::cout << "    MyClass(MyClass&&) = default;         // Use default move\n";
    std::cout << "};\n\n";

    // 10. Best practices
    std::cout << "10. BEST PRACTICES:\n";
    std::cout << "1. Prefer Rule of Zero (use smart pointers, containers)\n";
    std::cout << "2. If implementing any, consider all five\n";
    std::cout << "3. Mark move operations noexcept\n";
    std::cout << "4. Explicitly = default or = delete\n";
    std::cout << "5. Test all five operations\n";
    std::cout << "6. Check for self-assignment in assignment operators\n";
    std::cout << "7. Leave moved-from objects in valid state\n";
    std::cout << "8. Don't allocate in move operations\n";

    return 0;
}
