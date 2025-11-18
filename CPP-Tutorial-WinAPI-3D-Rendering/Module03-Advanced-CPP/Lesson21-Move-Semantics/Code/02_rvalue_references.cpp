/*
 * Lesson 21: Move Semantics
 * File: 02_rvalue_references.cpp
 *
 * Rvalue References (&&)
 *
 * This program demonstrates how rvalue references work and how they differ
 * from lvalue references.
 *
 * Compile: g++ -std=c++17 -Wall 02_rvalue_references.cpp -o 02_rvalue_references
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>  // for std::move

// Function overloads to demonstrate binding
void func(const std::string& str) {
    std::cout << "const lvalue reference: " << str << "\n";
}

void func(std::string&& str) {
    std::cout << "rvalue reference: " << str << "\n";
    // Can modify rvalue reference
    str += " (modified)";
    std::cout << "Modified to: " << str << "\n";
}

// Class to track construction
class Tracker {
private:
    std::string name;
    int id;
    static int nextId;

public:
    Tracker(const std::string& n) : name(n), id(nextId++) {
        std::cout << "  [Tracker " << id << " '" << name << "' constructed]\n";
    }

    Tracker(const Tracker& other) : name(other.name), id(nextId++) {
        std::cout << "  [Tracker " << id << " copy-constructed from "
                  << other.id << "]\n";
    }

    Tracker(Tracker&& other) noexcept : name(std::move(other.name)), id(nextId++) {
        std::cout << "  [Tracker " << id << " move-constructed from "
                  << other.id << "]\n";
        other.name = "moved-from";
    }

    ~Tracker() {
        std::cout << "  [Tracker " << id << " '" << name << "' destroyed]\n";
    }

    void print() const {
        std::cout << "  Tracker " << id << ": " << name << "\n";
    }
};

int Tracker::nextId = 1;

// Function accepting rvalue reference
void acceptRvalueRef(Tracker&& t) {
    std::cout << "In acceptRvalueRef:\n";
    t.print();
}

// Function accepting lvalue reference
void acceptLvalueRef(Tracker& t) {
    std::cout << "In acceptLvalueRef:\n";
    t.print();
}

// Function returning by value (creates rvalue)
Tracker createTracker(const std::string& name) {
    Tracker temp(name);
    return temp;  // Returns rvalue (may be moved or copy-elided)
}

int main() {
    std::cout << "=== Rvalue References ===\n\n";

    // 1. Basic rvalue reference binding
    std::cout << "1. BASIC RVALUE REFERENCE BINDING:\n";
    int&& rref1 = 42;
    std::cout << "int&& rref1 = 42;  // Binds rvalue ref to literal\n";
    std::cout << "rref1 = " << rref1 << ", address = " << &rref1 << "\n";

    // Can modify through rvalue reference
    rref1 = 100;
    std::cout << "rref1 = 100;  // Can modify\n";
    std::cout << "rref1 = " << rref1 << "\n\n";

    // 2. Cannot bind rvalue reference to lvalue
    std::cout << "2. BINDING RESTRICTIONS:\n";
    int x = 10;
    // int&& rref2 = x;  // ERROR: cannot bind rvalue ref to lvalue
    std::cout << "int x = 10;\n";
    std::cout << "int&& rref2 = x;  // ERROR: can't bind to lvalue\n";

    // But can use std::move to cast lvalue to rvalue
    int&& rref2 = std::move(x);
    std::cout << "int&& rref2 = std::move(x);  // OK: explicit cast\n";
    std::cout << "rref2 = " << rref2 << "\n\n";

    // 3. Function overload resolution
    std::cout << "3. FUNCTION OVERLOAD RESOLUTION:\n";
    std::string s1 = "Hello";

    func(s1);                           // Calls const lvalue ref version
    func("World");                       // Calls rvalue ref version
    func(std::string("Temporary"));      // Calls rvalue ref version
    func(s1 + " concatenated");         // Calls rvalue ref version
    func(std::move(s1));                // Calls rvalue ref version
    std::cout << "\n";

    // 4. Rvalue reference members
    std::cout << "4. RVALUE REFERENCE AS MEMBER:\n";
    struct Container {
        int&& rref;

        Container(int&& r) : rref(std::move(r)) {
            std::cout << "Container created with rvalue reference\n";
        }
    };

    int temp = 42;
    Container c(std::move(temp));
    std::cout << "c.rref = " << c.rref << "\n\n";

    // 5. Lifetime extension
    std::cout << "5. LIFETIME EXTENSION:\n";
    std::cout << "Without rvalue reference:\n";
    {
        const std::string& ref = std::string("Temporary");
        std::cout << "Temporary string: " << ref << "\n";
        std::cout << "Lifetime extended to end of scope\n";
    }
    std::cout << "Temporary destroyed\n";

    std::cout << "\nWith rvalue reference:\n";
    {
        std::string&& rref = std::string("Temporary");
        std::cout << "Temporary string: " << rref << "\n";
        rref += " extended";
        std::cout << "Modified: " << rref << "\n";
        std::cout << "Lifetime extended to end of scope\n";
    }
    std::cout << "Temporary destroyed\n\n";

    // 6. Named rvalue references are lvalues
    std::cout << "6. NAMED RVALUE REFERENCES ARE LVALUES:\n";
    std::cout << "Creating tracker:\n";
    Tracker t1("Original");

    std::cout << "\nBinding rvalue reference:\n";
    Tracker&& rref3 = std::move(t1);
    std::cout << "rref3 is rvalue reference type\n";
    std::cout << "But rref3 itself is an lvalue (has name)\n";

    // This calls lvalue version because rref3 is an lvalue!
    std::cout << "\nPassing rref3 to function:\n";
    // acceptRvalueRef(rref3);  // ERROR: rref3 is lvalue
    acceptLvalueRef(rref3);     // OK: rref3 is lvalue

    // Must use std::move to pass as rvalue
    std::cout << "\nUsing std::move on rref3:\n";
    acceptRvalueRef(std::move(rref3));
    std::cout << "\n";

    // 7. Function parameters
    std::cout << "7. FUNCTION PARAMETERS:\n";
    auto processInt = [](int&& x) {
        std::cout << "Received rvalue: " << x << "\n";
        x *= 2;
        std::cout << "Modified to: " << x << "\n";
    };

    int y = 10;
    // processInt(y);        // ERROR: can't bind rvalue ref to lvalue
    processInt(20);          // OK: literal is rvalue
    processInt(std::move(y));// OK: std::move casts to rvalue
    std::cout << "\n";

    // 8. Return values
    std::cout << "8. RETURN VALUES:\n";
    std::cout << "Calling createTracker:\n";
    Tracker&& rref4 = createTracker("Returned");
    std::cout << "Assigned to rvalue reference\n";
    rref4.print();
    std::cout << "\n";

    // 9. Vectors and rvalue references
    std::cout << "9. VECTORS AND RVALUE REFERENCES:\n";
    std::vector<Tracker> vec;
    vec.reserve(3);  // Prevent reallocation

    std::cout << "push_back with lvalue:\n";
    Tracker t2("Lvalue");
    vec.push_back(t2);  // Copy

    std::cout << "\npush_back with rvalue:\n";
    vec.push_back(Tracker("Rvalue"));  // Move

    std::cout << "\npush_back with std::move:\n";
    Tracker t3("Moved");
    vec.push_back(std::move(t3));  // Move

    std::cout << "\n";

    // 10. Rvalue reference vs const lvalue reference
    std::cout << "10. RVALUE REF VS CONST LVALUE REF:\n";

    auto constLvalueFunc = [](const std::string& s) {
        std::cout << "const lvalue ref: " << s << "\n";
        // s += "x";  // ERROR: can't modify const
    };

    auto rvalueFunc = [](std::string&& s) {
        std::cout << "rvalue ref: " << s << "\n";
        s += " modified";  // OK: can modify rvalue ref
        std::cout << "After modify: " << s << "\n";
    };

    std::string str = "Test";

    constLvalueFunc(str);           // OK
    constLvalueFunc("Literal");     // OK
    constLvalueFunc(str + "!");     // OK

    std::cout << "\n";

    // rvalueFunc(str);             // ERROR: can't bind to lvalue
    rvalueFunc("Literal");          // OK
    rvalueFunc(std::string("Temp"));// OK
    rvalueFunc(str + "!");          // OK
    rvalueFunc(std::move(str));     // OK

    std::cout << "\n";

    // 11. Summary
    std::cout << "11. SUMMARY:\n";
    std::cout << "Rvalue references (T&&):\n";
    std::cout << "  - Bind to rvalues (temporaries, std::move)\n";
    std::cout << "  - Cannot bind to lvalues (without std::move)\n";
    std::cout << "  - Can modify the referenced object\n";
    std::cout << "  - Enable move semantics\n";
    std::cout << "  - Named rvalue references are themselves lvalues\n";
    std::cout << "  - Extend lifetime of temporaries\n";

    std::cout << "\nDestructing objects:\n";
    return 0;
}
