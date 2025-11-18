/*
 * Lesson 21: Move Semantics
 * File: 05_std_move.cpp
 *
 * std::move in Detail
 *
 * This program demonstrates what std::move actually does and when to use it.
 * Important: std::move doesn't move anything - it's just a cast!
 *
 * Compile: g++ -std=c++17 -Wall 05_std_move.cpp -o 05_std_move
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>  // for std::move
#include <memory>

// Class to track operations
class Tracker {
private:
    std::string name;
    int* data;

public:
    Tracker(const std::string& n) : name(n), data(new int(42)) {
        std::cout << "  [" << name << "] Constructed\n";
    }

    Tracker(const Tracker& other)
        : name(other.name + "_copy"), data(new int(*other.data)) {
        std::cout << "  [" << name << "] Copy constructed\n";
    }

    Tracker(Tracker&& other) noexcept
        : name(std::move(other.name)), data(other.data) {
        std::cout << "  [" << name << "] Move constructed\n";
        other.data = nullptr;
    }

    Tracker& operator=(const Tracker& other) {
        std::cout << "  [" << name << "] Copy assigned\n";
        if (this != &other) {
            delete data;
            name = other.name + "_assigned";
            data = new int(*other.data);
        }
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        std::cout << "  [" << name << "] Move assigned\n";
        if (this != &other) {
            delete data;
            name = std::move(other.name);
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    ~Tracker() {
        delete data;
        std::cout << "  [" << name << "] Destroyed\n";
    }
};

// Simulated std::move implementation
template<typename T>
typename std::remove_reference<T>::type&& my_move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

int main() {
    std::cout << "=== std::move ===\n\n";

    // 1. What std::move does
    std::cout << "1. WHAT STD::MOVE DOES:\n";
    std::cout << "std::move doesn't move anything!\n";
    std::cout << "It's just a cast from lvalue to rvalue reference\n";
    std::cout << "The actual moving happens in move constructor/assignment\n\n";

    // 2. Basic std::move usage
    std::cout << "2. BASIC USAGE:\n";
    std::string str1 = "Hello";
    std::cout << "str1 = \"" << str1 << "\"\n";

    std::cout << "\nstd::string str2 = std::move(str1);\n";
    std::string str2 = std::move(str1);

    std::cout << "After move:\n";
    std::cout << "str1 = \"" << str1 << "\" (moved-from, unspecified)\n";
    std::cout << "str2 = \"" << str2 << "\"\n\n";

    // 3. std::move with custom class
    std::cout << "3. STD::MOVE WITH CUSTOM CLASS:\n";
    Tracker t1("Original");

    std::cout << "\nTracker t2 = std::move(t1);\n";
    Tracker t2 = std::move(t1);

    std::cout << "\nTracker t3(\"New\");\n";
    Tracker t3("New");

    std::cout << "\nt3 = std::move(t2);\n";
    t3 = std::move(t2);
    std::cout << "\n";

    // 4. Without std::move (copy)
    std::cout << "4. WITHOUT STD::MOVE (COPY):\n";
    std::vector<std::string> vec1;
    std::string s1 = "World";

    std::cout << "\nPush without move (copy):\n";
    vec1.push_back(s1);
    std::cout << "s1 after push_back: \"" << s1 << "\" (still valid)\n";

    std::cout << "\nPush with move:\n";
    std::string s2 = "Move";
    vec1.push_back(std::move(s2));
    std::cout << "s2 after push_back: \"" << s2 << "\" (moved-from)\n\n";

    // 5. std::move in member initialization
    std::cout << "5. MEMBER INITIALIZATION:\n";
    class Container {
        std::string data;
    public:
        // Good: use std::move for rvalue parameters
        Container(std::string s) : data(std::move(s)) {
            std::cout << "  Container created with: " << data << "\n";
        }
    };

    std::string temp = "Data";
    std::cout << "Creating container with \"" << temp << "\":\n";
    Container c1(temp);  // temp copied to parameter, parameter moved to member
    std::cout << "temp after: \"" << temp << "\" (copied to parameter)\n";

    std::cout << "\nCreating container with temporary:\n";
    Container c2("Temporary");  // No copy, temporary moved to member
    std::cout << "\n";

    // 6. std::move does NOT guarantee a move
    std::cout << "6. STD::MOVE DOESN'T GUARANTEE MOVE:\n";
    const std::string cs = "Const";
    std::cout << "const std::string cs = \"Const\";\n";
    std::cout << "std::string cs2 = std::move(cs);  // COPIES!\n";
    std::string cs2 = std::move(cs);  // Copies! Can't move from const
    std::cout << "cs = \"" << cs << "\" (not moved, because const)\n";
    std::cout << "cs2 = \"" << cs2 << "\"\n\n";

    // 7. Common mistake: std::move on return
    std::cout << "7. COMMON MISTAKE: MOVE ON RETURN:\n";
    std::cout << "BAD:\n";
    std::cout << "std::string func() {\n";
    std::cout << "    std::string result;\n";
    std::cout << "    return std::move(result);  // DON'T DO THIS!\n";
    std::cout << "}\n\n";

    std::cout << "GOOD:\n";
    std::cout << "std::string func() {\n";
    std::cout << "    std::string result;\n";
    std::cout << "    return result;  // Compiler optimizes\n";
    std::cout << "}\n\n";

    // 8. std::move with unique_ptr
    std::cout << "8. STD::MOVE WITH UNIQUE_PTR:\n";
    std::unique_ptr<int> p1 = std::make_unique<int>(42);
    std::cout << "p1 points to: " << *p1 << "\n";

    std::cout << "\nstd::unique_ptr<int> p2 = std::move(p1);\n";
    std::unique_ptr<int> p2 = std::move(p1);

    std::cout << "After move:\n";
    std::cout << "p1 is " << (p1 ? "valid" : "nullptr") << "\n";
    std::cout << "p2 points to: " << *p2 << "\n\n";

    // 9. std::move in algorithms
    std::cout << "9. STD::MOVE IN ALGORITHMS:\n";
    std::vector<std::string> source = {"one", "two", "three"};
    std::vector<std::string> dest;

    std::cout << "Source before move: ";
    for (const auto& s : source) std::cout << s << " ";
    std::cout << "\n";

    std::cout << "\nUsing std::move algorithm:\n";
    std::move(source.begin(), source.end(), std::back_inserter(dest));

    std::cout << "Source after move: ";
    for (const auto& s : source) std::cout << "\"" << s << "\" ";
    std::cout << "(moved-from)\n";

    std::cout << "Dest: ";
    for (const auto& s : dest) std::cout << s << " ";
    std::cout << "\n\n";

    // 10. When to use std::move
    std::cout << "10. WHEN TO USE STD::MOVE:\n";
    std::cout << "\nUSE std::move when:\n";
    std::cout << "  1. Passing to function that takes rvalue reference\n";
    std::cout << "  2. Transferring ownership (unique_ptr, etc.)\n";
    std::cout << "  3. Moving into container (push_back, insert)\n";
    std::cout << "  4. Implementing move constructor/assignment\n";
    std::cout << "  5. Last use of an object you won't need again\n";

    std::cout << "\nDON'T use std::move when:\n";
    std::cout << "  1. Returning local variables (prevents RVO)\n";
    std::cout << "  2. On const objects (won't move anyway)\n";
    std::cout << "  3. On temporary objects (already rvalues)\n";
    std::cout << "  4. You might need the object later\n";
    std::cout << "  5. Passing to function taking const&\n\n";

    // 11. std::move implementation
    std::cout << "11. STD::MOVE IMPLEMENTATION:\n";
    std::cout << "Simplified implementation:\n";
    std::cout << "template<typename T>\n";
    std::cout << "remove_reference_t<T>&& move(T&& arg) noexcept {\n";
    std::cout << "    return static_cast<remove_reference_t<T>&&>(arg);\n";
    std::cout << "}\n\n";

    int x = 10;
    int&& rx = my_move(x);
    std::cout << "Using my_move: " << rx << "\n\n";

    // 12. Moved-from state
    std::cout << "12. MOVED-FROM STATE:\n";
    std::string s3 = "Original";
    std::string s4 = std::move(s3);

    std::cout << "After std::move(s3):\n";
    std::cout << "  s3 is in valid but unspecified state\n";
    std::cout << "  Can safely: destroy it, assign to it\n";
    std::cout << "  Cannot safely: use its value, call methods\n\n";

    // Safe: assign to moved-from object
    s3 = "New value";
    std::cout << "After s3 = \"New value\": " << s3 << " (OK)\n\n";

    // 13. Multiple moves
    std::cout << "13. MULTIPLE MOVES:\n";
    std::string m1 = "Start";
    std::string m2 = std::move(m1);  // m1 moved to m2
    std::string m3 = std::move(m2);  // m2 moved to m3
    std::string m4 = std::move(m3);  // m3 moved to m4

    std::cout << "After chain of moves:\n";
    std::cout << "m1: \"" << m1 << "\"\n";
    std::cout << "m2: \"" << m2 << "\"\n";
    std::cout << "m3: \"" << m3 << "\"\n";
    std::cout << "m4: \"" << m4 << "\"\n\n";

    // 14. std::move with references
    std::cout << "14. STD::MOVE WITH REFERENCES:\n";
    std::string orig = "Original";
    std::string& ref = orig;

    std::cout << "std::move on reference:\n";
    std::string moved = std::move(ref);  // Moves from orig!
    std::cout << "orig: \"" << orig << "\" (moved-from)\n";
    std::cout << "moved: \"" << moved << "\"\n\n";

    // 15. Summary
    std::cout << "15. SUMMARY:\n";
    std::cout << "std::move:\n";
    std::cout << "  - Is a cast to rvalue reference\n";
    std::cout << "  - Doesn't actually move anything\n";
    std::cout << "  - Enables move semantics\n";
    std::cout << "  - Unconditionally casts to rvalue\n";
    std::cout << "  - Should only be used on last use of object\n";
    std::cout << "  - Leaves object in valid but unspecified state\n";
    std::cout << "  - Is noexcept\n";

    std::cout << "\nDestructing objects:\n";
    return 0;
}
