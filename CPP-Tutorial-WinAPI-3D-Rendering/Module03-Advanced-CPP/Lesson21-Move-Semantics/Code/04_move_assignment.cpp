/*
 * Lesson 21: Move Semantics
 * File: 04_move_assignment.cpp
 *
 * Move Assignment Operator Implementation
 *
 * This program demonstrates how to implement a move assignment operator
 * and shows the differences from move constructor.
 *
 * Compile: g++ -std=c++17 -Wall 04_move_assignment.cpp -o 04_move_assignment
 */

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

// Resource management class
class Resource {
private:
    int* data;
    size_t size;
    std::string name;

public:
    // Constructor
    Resource(const std::string& n = "unnamed", size_t s = 10)
        : size(s), name(n) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
        std::cout << "  [" << name << "] Constructed (size=" << size << ")\n";
    }

    // Copy constructor
    Resource(const Resource& other)
        : size(other.size), name(other.name + "_copy") {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "  [" << name << "] Copy constructed from ["
                  << other.name << "]\n";
    }

    // Move constructor
    Resource(Resource&& other) noexcept
        : data(other.data), size(other.size), name(std::move(other.name)) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "  [" << name << "] Move constructed\n";
    }

    // Copy assignment operator
    Resource& operator=(const Resource& other) {
        std::cout << "  [" << name << "] Copy assignment from ["
                  << other.name << "]\n";

        if (this != &other) {
            // Release current resources
            delete[] data;

            // Copy from other
            size = other.size;
            name = other.name + "_assigned";
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Move assignment operator
    Resource& operator=(Resource&& other) noexcept {
        std::cout << "  [" << name << "] Move assignment from ["
                  << other.name << "]\n";

        if (this != &other) {
            // Release current resources
            delete[] data;

            // Transfer ownership from other
            data = other.data;
            size = other.size;
            name = std::move(other.name);

            // Leave other in valid state
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Destructor
    ~Resource() {
        if (data) {
            std::cout << "  [" << name << "] Destroyed (size=" << size << ")\n";
            delete[] data;
        } else {
            std::cout << "  [" << name << "] Destroyed (moved-from)\n";
        }
    }

    void print() const {
        std::cout << "  [" << name << "] size=" << size << ", data=";
        if (data && size > 0) {
            std::cout << "[";
            for (size_t i = 0; i < std::min(size, size_t(5)); ++i) {
                std::cout << data[i];
                if (i < size - 1 && i < 4) std::cout << ", ";
            }
            if (size > 5) std::cout << ", ...";
            std::cout << "]";
        } else {
            std::cout << "nullptr";
        }
        std::cout << "\n";
    }
};

// Simplified string class
class SimpleString {
private:
    char* str;

public:
    SimpleString(const char* s = "") {
        str = new char[std::strlen(s) + 1];
        std::strcpy(str, s);
        std::cout << "  Constructed: \"" << str << "\"\n";
    }

    // Copy assignment
    SimpleString& operator=(const SimpleString& other) {
        std::cout << "  Copy assignment: \"" << other.str << "\"\n";
        if (this != &other) {
            delete[] str;
            str = new char[std::strlen(other.str) + 1];
            std::strcpy(str, other.str);
        }
        return *this;
    }

    // Move assignment
    SimpleString& operator=(SimpleString&& other) noexcept {
        std::cout << "  Move assignment: \"" << other.str << "\"\n";
        if (this != &other) {
            delete[] str;
            str = other.str;
            other.str = nullptr;
        }
        return *this;
    }

    ~SimpleString() {
        if (str) {
            std::cout << "  Destroyed: \"" << str << "\"\n";
        } else {
            std::cout << "  Destroyed: (moved-from)\n";
        }
        delete[] str;
    }

    const char* c_str() const { return str ? str : ""; }
};

int main() {
    std::cout << "=== Move Assignment Operator ===\n\n";

    // 1. Basic move assignment
    std::cout << "1. BASIC MOVE ASSIGNMENT:\n";
    Resource r1("Resource1");
    Resource r2("Resource2");

    std::cout << "\nBefore assignment:\n";
    r1.print();
    r2.print();

    std::cout << "\nMove assignment: r1 = std::move(r2)\n";
    r1 = std::move(r2);

    std::cout << "\nAfter assignment:\n";
    r1.print();
    r2.print();
    std::cout << "\n";

    // 2. Self-assignment check
    std::cout << "2. SELF-ASSIGNMENT:\n";
    Resource r3("Resource3");
    std::cout << "Self-assignment: r3 = std::move(r3)\n";
    r3 = std::move(r3);  // Should be safe due to self-check
    std::cout << "After self-assignment:\n";
    r3.print();
    std::cout << "\n";

    // 3. Copy assignment vs Move assignment
    std::cout << "3. COPY VS MOVE ASSIGNMENT:\n";
    Resource r4("Resource4");
    Resource r5("Resource5");
    Resource r6("Resource6");

    std::cout << "\nCopy assignment:\n";
    r5 = r4;  // Copy

    std::cout << "\nMove assignment:\n";
    r6 = std::move(r4);  // Move

    std::cout << "\nStates:\n";
    r4.print();
    r5.print();
    r6.print();
    std::cout << "\n";

    // 4. Chain assignment
    std::cout << "4. CHAIN ASSIGNMENT:\n";
    Resource r7("R7");
    Resource r8("R8");
    Resource r9("R9");

    std::cout << "\nChain move assignment: r9 = r8 = std::move(r7)\n";
    r9 = r8 = std::move(r7);

    std::cout << "\nStates:\n";
    r7.print();
    r8.print();
    r9.print();
    std::cout << "\n";

    // 5. Assignment in expressions
    std::cout << "5. ASSIGNMENT IN EXPRESSIONS:\n";
    Resource r10("R10", 5);
    Resource r11("R11", 5);

    std::cout << "\nMove assignment in expression:\n";
    if ((r11 = std::move(r10), r11.print(), true)) {
        std::cout << "Assignment completed\n";
    }
    std::cout << "\n";

    // 6. Vector reallocation
    std::cout << "6. VECTOR OPERATIONS:\n";
    std::vector<Resource> vec;
    std::cout << "Creating resources:\n";
    Resource r12("R12", 3);
    Resource r13("R13", 3);

    std::cout << "\nPush back (copy):\n";
    vec.push_back(r12);

    std::cout << "\nPush back (move):\n";
    vec.push_back(std::move(r13));

    std::cout << "\nVector size: " << vec.size() << "\n\n";

    // 7. Assigning temporary
    std::cout << "7. ASSIGNING TEMPORARY:\n";
    Resource r14("R14");
    std::cout << "\nAssigning temporary:\n";
    r14 = Resource("Temporary", 15);
    r14.print();
    std::cout << "\n";

    // 8. String move assignment
    std::cout << "8. STRING MOVE ASSIGNMENT:\n";
    {
        SimpleString s1("Hello");
        SimpleString s2("World");

        std::cout << "\nMove assignment:\n";
        s1 = std::move(s2);

        std::cout << "\nValues:\n";
        std::cout << "s1: \"" << s1.c_str() << "\"\n";
        std::cout << "s2: \"" << s2.c_str() << "\" (moved-from)\n";

        std::cout << "\nDestroying strings:\n";
    }
    std::cout << "\n";

    // 9. Assignment operator overload resolution
    std::cout << "9. OVERLOAD RESOLUTION:\n";
    Resource r15("R15");
    Resource r16("R16");
    const Resource r17("R17_const");

    std::cout << "\nAssigning from lvalue (copy):\n";
    r15 = r16;

    std::cout << "\nAssigning from rvalue (move):\n";
    r15 = std::move(r16);

    std::cout << "\nAssigning from const (copy, can't move):\n";
    r15 = r17;  // Calls copy assignment

    std::cout << "\n";

    // 10. Move assignment implementation pattern
    std::cout << "10. IMPLEMENTATION PATTERN:\n";
    std::cout << "Move assignment operator should:\n";
    std::cout << "  1. Check for self-assignment (if this != &other)\n";
    std::cout << "  2. Release current resources (delete[] data)\n";
    std::cout << "  3. Transfer ownership (data = other.data)\n";
    std::cout << "  4. Nullify source (other.data = nullptr)\n";
    std::cout << "  5. Return *this\n";
    std::cout << "  6. Mark as noexcept\n";
    std::cout << "  7. Handle moved-from state gracefully\n";

    std::cout << "\nExample template:\n";
    std::cout << "T& operator=(T&& other) noexcept {\n";
    std::cout << "    if (this != &other) {\n";
    std::cout << "        // Release current\n";
    std::cout << "        // Transfer from other\n";
    std::cout << "        // Nullify other\n";
    std::cout << "    }\n";
    std::cout << "    return *this;\n";
    std::cout << "}\n";

    std::cout << "\nDestructing objects:\n";
    return 0;
}
