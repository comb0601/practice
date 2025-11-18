/*
 * Lesson 21: Move Semantics
 * File: 10_common_pitfalls.cpp
 *
 * Common Pitfalls and Mistakes with Move Semantics
 *
 * This program demonstrates common mistakes when using move semantics
 * and how to avoid them.
 *
 * Compile: g++ -std=c++17 -Wall 10_common_pitfalls.cpp -o 10_common_pitfalls
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Resource {
private:
    std::string name;
    int* data;

public:
    Resource(const std::string& n) : name(n), data(new int(42)) {
        std::cout << "  [" << name << "] Created\n";
    }

    Resource(const Resource& other)
        : name(other.name + "_copy"), data(new int(*other.data)) {
        std::cout << "  [" << name << "] Copied\n";
    }

    Resource(Resource&& other) noexcept
        : name(std::move(other.name)), data(other.data) {
        std::cout << "  [" << name << "] Moved\n";
        other.data = nullptr;
    }

    ~Resource() {
        std::cout << "  [" << name << "] Destroyed\n";
        delete data;
    }

    const std::string& getName() const { return name; }
    int getValue() const { return data ? *data : -1; }
};

int main() {
    std::cout << "=== Common Pitfalls with Move Semantics ===\n\n";

    // PITFALL 1: Using moved-from objects
    std::cout << "PITFALL 1: USING MOVED-FROM OBJECTS\n";
    {
        std::string str1 = "Hello";
        std::string str2 = std::move(str1);

        std::cout << "After move:\n";
        std::cout << "str2 = \"" << str2 << "\" (OK)\n";

        // BAD: Using moved-from object
        std::cout << "\nBAD: Using moved-from str1:\n";
        std::cout << "str1 = \"" << str1 << "\" (unspecified state!)\n";

        // GOOD: Moved-from objects can be assigned to
        std::cout << "\nGOOD: Assigning to moved-from object:\n";
        str1 = "New value";
        std::cout << "str1 = \"" << str1 << "\" (OK after assignment)\n";
    }
    std::cout << "\n";

    // PITFALL 2: Moving const objects
    std::cout << "PITFALL 2: MOVING CONST OBJECTS\n";
    {
        const std::string cs1 = "Const";
        std::cout << "const std::string cs1 = \"" << cs1 << "\"\n";

        std::cout << "\nBAD: std::move on const object:\n";
        std::string cs2 = std::move(cs1);  // Copies! Can't move from const
        std::cout << "cs1 = \"" << cs1 << "\" (not moved!)\n";
        std::cout << "cs2 = \"" << cs2 << "\" (copied, not moved)\n";
        std::cout << "Note: std::move on const objects results in copy\n";
    }
    std::cout << "\n";

    // PITFALL 3: std::move on return values
    std::cout << "PITFALL 3: STD::MOVE ON RETURN VALUES\n";
    {
        auto badReturn = []() {
            std::vector<int> vec(1000);
            return std::move(vec);  // BAD: Prevents RVO!
        };

        auto goodReturn = []() {
            std::vector<int> vec(1000);
            return vec;  // GOOD: Allows RVO or move
        };

        std::cout << "BAD:\n";
        std::cout << "return std::move(vec);  // Prevents optimization\n";

        std::cout << "\nGOOD:\n";
        std::cout << "return vec;  // Allows RVO or automatic move\n";

        std::cout << "\nNote: Never use std::move on return values!\n";
    }
    std::cout << "\n";

    // PITFALL 4: Forgetting noexcept
    std::cout << "PITFALL 4: FORGETTING NOEXCEPT\n";
    {
        class WithoutNoexcept {
        public:
            WithoutNoexcept() = default;
            // BAD: Missing noexcept
            WithoutNoexcept(WithoutNoexcept&&) { }
        };

        class WithNoexcept {
        public:
            WithNoexcept() = default;
            // GOOD: Has noexcept
            WithNoexcept(WithNoexcept&&) noexcept { }
        };

        std::cout << "Without noexcept: move might not be used by STL\n";
        std::cout << "With noexcept: move will be used by STL\n";

        std::cout << "\nExample: vector reallocation\n";
        std::cout << "  - If move is noexcept: uses move\n";
        std::cout << "  - If move can throw: uses copy (for safety)\n";

        std::cout << "\nAlways mark move operations as noexcept!\n";
    }
    std::cout << "\n";

    // PITFALL 5: Not implementing both move operations
    std::cout << "PITFALL 5: INCOMPLETE MOVE IMPLEMENTATION\n";
    {
        class IncompleteMoves {
        private:
            int* data;
        public:
            IncompleteMoves() : data(new int(42)) {}
            ~IncompleteMoves() { delete data; }

            // BAD: Only move constructor, no move assignment
            IncompleteMoves(IncompleteMoves&& other) noexcept
                : data(other.data) {
                other.data = nullptr;
            }

            // Missing: Move assignment operator
        };

        std::cout << "BAD: Only implementing move constructor\n";
        std::cout << "GOOD: Implement both move constructor and move assignment\n";
        std::cout << "      Or implement neither (Rule of Zero)\n";
    }
    std::cout << "\n";

    // PITFALL 6: Self-move without check
    std::cout << "PITFALL 6: SELF-MOVE WITHOUT CHECK\n";
    {
        class NoSelfCheck {
            int* data;
        public:
            NoSelfCheck() : data(new int(42)) {}
            ~NoSelfCheck() { delete data; }

            // BAD: No self-assignment check
            NoSelfCheck& operator=(NoSelfCheck&& other) noexcept {
                delete data;  // Dangerous if this == &other!
                data = other.data;
                other.data = nullptr;
                return *this;
            }
        };

        class WithSelfCheck {
            int* data;
        public:
            WithSelfCheck() : data(new int(42)) {}
            ~WithSelfCheck() { delete data; }

            // GOOD: Has self-assignment check
            WithSelfCheck& operator=(WithSelfCheck&& other) noexcept {
                if (this != &other) {  // Check!
                    delete data;
                    data = other.data;
                    other.data = nullptr;
                }
                return *this;
            }
        };

        std::cout << "BAD: No self-move check\n";
        std::cout << "GOOD: Always check for self-assignment\n";
        std::cout << "      if (this != &other) { ... }\n";
    }
    std::cout << "\n";

    // PITFALL 7: Moving twice
    std::cout << "PITFALL 7: MOVING FROM SAME OBJECT TWICE\n";
    {
        std::string str1 = "Original";
        std::string str2 = std::move(str1);  // First move: OK

        std::cout << "After first move:\n";
        std::cout << "str1 = \"" << str1 << "\" (moved-from)\n";
        std::cout << "str2 = \"" << str2 << "\"\n";

        std::cout << "\nBAD: Moving from already moved-from object:\n";
        std::string str3 = std::move(str1);  // Second move: BAD!
        std::cout << "str3 = \"" << str3 << "\" (undefined behavior!)\n";
    }
    std::cout << "\n";

    // PITFALL 8: Expecting specific moved-from state
    std::cout << "PITFALL 8: EXPECTING SPECIFIC MOVED-FROM STATE\n";
    {
        std::string str1 = "Test";
        std::string str2 = std::move(str1);

        std::cout << "After move:\n";

        // BAD: Expecting moved-from object to be empty
        std::cout << "BAD: Expecting str1.empty() == true\n";
        std::cout << "     Actual: " << (str1.empty() ? "empty" : "not empty") << "\n";
        std::cout << "     State is unspecified!\n";

        std::cout << "\nGOOD: Only assume moved-from object is:\n";
        std::cout << "      - Valid (can be destroyed)\n";
        std::cout << "      - Assignable (can assign to it)\n";
        std::cout << "      Don't assume any other state!\n";
    }
    std::cout << "\n";

    // PITFALL 9: Moving non-moveable types
    std::cout << "PITFALL 9: MOVING NON-MOVEABLE TYPES\n";
    {
        class NonMoveable {
        public:
            NonMoveable() = default;
            NonMoveable(const NonMoveable&) = default;
            NonMoveable(NonMoveable&&) = delete;  // Move deleted
        };

        NonMoveable nm1;
        // NonMoveable nm2 = std::move(nm1);  // ERROR: move deleted

        std::cout << "Some types don't support move (deleted or not implemented)\n";
        std::cout << "Check if type is moveable before using std::move\n";
        std::cout << "  std::is_move_constructible<T>::value\n";
    }
    std::cout << "\n";

    // PITFALL 10: Inefficient member initialization
    std::cout << "PITFALL 10: NOT MOVING IN MEMBER INITIALIZATION\n";
    {
        class BadInit {
            std::string name;
        public:
            // BAD: Copies parameter even though it could be moved
            BadInit(std::string n) : name(n) { }
        };

        class GoodInit {
            std::string name;
        public:
            // GOOD: Moves parameter
            GoodInit(std::string n) : name(std::move(n)) { }
        };

        std::cout << "BAD:\n";
        std::cout << "BadInit(std::string n) : name(n) { }\n";
        std::cout << "    // Copies n even when n is temporary\n\n";

        std::cout << "GOOD:\n";
        std::cout << "GoodInit(std::string n) : name(std::move(n)) { }\n";
        std::cout << "    // Moves n into name\n";
    }
    std::cout << "\n";

    // PITFALL 11: Moving in a loop
    std::cout << "PITFALL 11: UNNECESSARY MOVES IN LOOP\n";
    {
        std::vector<std::string> vec;
        vec.reserve(3);

        std::string str = "Value";

        std::cout << "BAD: Moving in loop:\n";
        for (int i = 0; i < 3; ++i) {
            vec.push_back(str);  // Last iteration should move
        }
        std::cout << "str = \"" << str << "\" (still valid)\n";

        std::cout << "\nGOOD: Move only on last use:\n";
        vec.clear();
        for (int i = 0; i < 2; ++i) {
            vec.push_back(str);
        }
        vec.push_back(std::move(str));  // Move only last time
        std::cout << "str = \"" << str << "\" (moved-from)\n";
    }
    std::cout << "\n";

    // PITFALL 12: Not leaving moved-from in valid state
    std::cout << "PITFALL 12: INVALID MOVED-FROM STATE\n";
    {
        class BadMove {
            int* data;
        public:
            BadMove() : data(new int(42)) {}

            // BAD: Doesn't nullify pointer
            BadMove(BadMove&& other) noexcept : data(other.data) {
                // Missing: other.data = nullptr;
            }

            ~BadMove() {
                delete data;  // Double delete if data not nullified!
            }
        };

        std::cout << "BAD: Not nullifying pointers in move constructor\n";
        std::cout << "     Results in double delete!\n\n";

        std::cout << "GOOD: Always leave moved-from object in valid state:\n";
        std::cout << "      - Nullify pointers\n";
        std::cout << "      - Reset sizes to zero\n";
        std::cout << "      - Close file handles\n";
    }
    std::cout << "\n";

    // Summary
    std::cout << "SUMMARY - AVOID THESE PITFALLS:\n";
    std::cout << "1.  Don't use moved-from objects (except to assign/destroy)\n";
    std::cout << "2.  Don't move from const objects (won't actually move)\n";
    std::cout << "3.  Don't use std::move on return values (prevents RVO)\n";
    std::cout << "4.  Always mark move operations noexcept\n";
    std::cout << "5.  Implement both move constructor and move assignment\n";
    std::cout << "6.  Check for self-assignment in move assignment\n";
    std::cout << "7.  Don't move from the same object twice\n";
    std::cout << "8.  Don't expect specific moved-from state\n";
    std::cout << "9.  Check if type is moveable before moving\n";
    std::cout << "10. Use std::move in member initialization\n";
    std::cout << "11. Only move on last use of object\n";
    std::cout << "12. Leave moved-from objects in valid state\n";

    return 0;
}
