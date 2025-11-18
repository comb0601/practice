/*
 * Lesson 21: Move Semantics
 * File: 08_move_stl_containers.cpp
 *
 * Move Semantics with STL Containers
 *
 * This program demonstrates how move semantics improves STL container
 * performance and how to use move operations with containers.
 *
 * Compile: g++ -std=c++17 -Wall -O2 08_move_stl_containers.cpp -o 08_move_stl_containers
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <chrono>
#include <memory>

// Tracking class
class Element {
private:
    std::string data;
    static int copyCount;
    static int moveCount;

public:
    Element(const std::string& d = "") : data(d) {}

    Element(const Element& other) : data(other.data) {
        ++copyCount;
    }

    Element(Element&& other) noexcept : data(std::move(other.data)) {
        ++moveCount;
    }

    Element& operator=(const Element& other) {
        data = other.data;
        ++copyCount;
        return *this;
    }

    Element& operator=(Element&& other) noexcept {
        data = std::move(other.data);
        ++moveCount;
        return *this;
    }

    static void resetCounters() { copyCount = moveCount = 0; }
    static void printCounters() {
        std::cout << "  Copies: " << copyCount << ", Moves: " << moveCount << "\n";
    }

    const std::string& getData() const { return data; }
};

int Element::copyCount = 0;
int Element::moveCount = 0;

int main() {
    std::cout << "=== Move Semantics with STL Containers ===\n\n";

    // 1. Vector push_back vs emplace_back
    std::cout << "1. VECTOR: PUSH_BACK VS EMPLACE_BACK:\n";
    {
        std::vector<Element> vec;
        vec.reserve(10);  // Prevent reallocation
        Element::resetCounters();

        std::cout << "\npush_back with lvalue:\n";
        Element e1("data1");
        vec.push_back(e1);
        Element::printCounters();

        Element::resetCounters();
        std::cout << "\npush_back with std::move:\n";
        Element e2("data2");
        vec.push_back(std::move(e2));
        Element::printCounters();

        Element::resetCounters();
        std::cout << "\npush_back with temporary:\n";
        vec.push_back(Element("data3"));
        Element::printCounters();

        Element::resetCounters();
        std::cout << "\nemplace_back (constructs in place):\n";
        vec.emplace_back("data4");
        Element::printCounters();
    }
    std::cout << "\n";

    // 2. Moving entire containers
    std::cout << "2. MOVING ENTIRE CONTAINERS:\n";
    {
        std::vector<std::string> vec1(1000, "data");
        std::cout << "vec1 size: " << vec1.size() << "\n";

        std::cout << "\nCopy assignment:\n";
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::string> vec2 = vec1;  // Copy
        auto end = std::chrono::high_resolution_clock::now();
        auto copyTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Time: " << copyTime.count() << " μs\n";

        std::cout << "\nMove assignment:\n";
        start = std::chrono::high_resolution_clock::now();
        std::vector<std::string> vec3 = std::move(vec1);  // Move
        end = std::chrono::high_resolution_clock::now();
        auto moveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Time: " << moveTime.count() << " μs\n";

        std::cout << "vec1 size after move: " << vec1.size() << "\n";
        std::cout << "vec3 size: " << vec3.size() << "\n";
    }
    std::cout << "\n";

    // 3. std::move algorithm
    std::cout << "3. STD::MOVE ALGORITHM:\n";
    {
        std::vector<std::string> source = {"one", "two", "three", "four"};
        std::vector<std::string> dest;

        std::cout << "Source before: ";
        for (const auto& s : source) std::cout << s << " ";
        std::cout << "\n";

        std::cout << "\nMoving elements:\n";
        std::move(source.begin(), source.end(), std::back_inserter(dest));

        std::cout << "Source after: ";
        for (const auto& s : source) std::cout << "\"" << s << "\" ";
        std::cout << "\n";

        std::cout << "Dest: ";
        for (const auto& s : dest) std::cout << s << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    // 4. Map with move
    std::cout << "4. MAP WITH MOVE:\n";
    {
        std::map<std::string, std::vector<int>> map1;

        std::cout << "Inserting with copy:\n";
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        map1["key1"] = vec1;  // Copy
        std::cout << "vec1 size after: " << vec1.size() << " (still valid)\n";

        std::cout << "\nInserting with move:\n";
        std::vector<int> vec2 = {6, 7, 8, 9, 10};
        map1["key2"] = std::move(vec2);  // Move
        std::cout << "vec2 size after: " << vec2.size() << " (moved-from)\n";

        std::cout << "\nInserting with emplace:\n";
        map1.emplace("key3", std::vector<int>{11, 12, 13});

        std::cout << "\nMap contents:\n";
        for (const auto& [key, vec] : map1) {
            std::cout << "  " << key << ": size=" << vec.size() << "\n";
        }
    }
    std::cout << "\n";

    // 5. Vector reallocation
    std::cout << "5. VECTOR REALLOCATION:\n";
    {
        Element::resetCounters();
        std::vector<Element> vec;

        std::cout << "Adding elements without reserve:\n";
        for (int i = 0; i < 5; ++i) {
            vec.push_back(Element("data"));
            std::cout << "  Size: " << vec.size()
                      << ", Capacity: " << vec.capacity() << "\n";
        }
        Element::printCounters();
        std::cout << "Note: Reallocation causes moves/copies\n";

        Element::resetCounters();
        std::vector<Element> vec2;
        vec2.reserve(5);

        std::cout << "\nAdding elements with reserve:\n";
        for (int i = 0; i < 5; ++i) {
            vec2.push_back(Element("data"));
        }
        Element::printCounters();
        std::cout << "Note: No reallocation, fewer operations\n";
    }
    std::cout << "\n";

    // 6. Removing elements
    std::cout << "6. REMOVING ELEMENTS:\n";
    {
        std::vector<std::string> vec = {"one", "two", "three", "four", "five"};

        std::cout << "Before erase: ";
        for (const auto& s : vec) std::cout << s << " ";
        std::cout << "\n";

        std::cout << "\nErasing second element (moves remaining elements):\n";
        vec.erase(vec.begin() + 1);

        std::cout << "After erase: ";
        for (const auto& s : vec) std::cout << s << " ";
        std::cout << "\n";

        std::cout << "\nNote: Elements after erased position are moved\n";
    }
    std::cout << "\n";

    // 7. Unique_ptr in containers
    std::cout << "7. UNIQUE_PTR IN CONTAINERS:\n";
    {
        std::vector<std::unique_ptr<int>> vec;

        std::cout << "Adding unique_ptr (must move):\n";
        vec.push_back(std::make_unique<int>(42));
        vec.push_back(std::make_unique<int>(99));

        auto ptr = std::make_unique<int>(123);
        // vec.push_back(ptr);  // ERROR: can't copy unique_ptr
        vec.push_back(std::move(ptr));  // OK: move

        std::cout << "Vector size: " << vec.size() << "\n";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << "  [" << i << "]: " << *vec[i] << "\n";
        }
    }
    std::cout << "\n";

    // 8. Move-only types
    std::cout << "8. MOVE-ONLY TYPES:\n";
    {
        class MoveOnly {
            std::string data;
        public:
            MoveOnly(std::string d) : data(std::move(d)) {
                std::cout << "  MoveOnly created: " << data << "\n";
            }
            MoveOnly(const MoveOnly&) = delete;
            MoveOnly(MoveOnly&& other) noexcept : data(std::move(other.data)) {
                std::cout << "  MoveOnly moved\n";
            }
            MoveOnly& operator=(const MoveOnly&) = delete;
            MoveOnly& operator=(MoveOnly&&) noexcept = default;
            const std::string& getData() const { return data; }
        };

        std::vector<MoveOnly> vec;
        vec.reserve(3);

        std::cout << "Adding move-only objects:\n";
        vec.push_back(MoveOnly("first"));
        vec.emplace_back("second");  // Better: construct in place

        MoveOnly obj("third");
        vec.push_back(std::move(obj));  // Must use std::move
    }
    std::cout << "\n";

    // 9. Sorting with move
    std::cout << "9. SORTING WITH MOVE:\n";
    {
        Element::resetCounters();
        std::vector<Element> vec;
        vec.reserve(5);
        for (int i = 5; i > 0; --i) {
            vec.emplace_back(std::to_string(i));
        }

        std::cout << "Before sort: ";
        for (const auto& e : vec) std::cout << e.getData() << " ";
        std::cout << "\n";

        Element::resetCounters();
        std::cout << "\nSorting (uses move operations):\n";
        std::sort(vec.begin(), vec.end(),
            [](const Element& a, const Element& b) {
                return a.getData() < b.getData();
            });

        std::cout << "After sort: ";
        for (const auto& e : vec) std::cout << e.getData() << " ";
        std::cout << "\n";

        Element::printCounters();
    }
    std::cout << "\n";

    // 10. Returning containers
    std::cout << "10. RETURNING CONTAINERS:\n";
    {
        auto createVector = []() {
            std::cout << "  Creating vector\n";
            std::vector<std::string> result;
            result.reserve(3);
            result.push_back("one");
            result.push_back("two");
            result.push_back("three");
            return result;  // Move or RVO
        };

        std::cout << "Calling function:\n";
        std::vector<std::string> vec = createVector();
        std::cout << "Received vector, size: " << vec.size() << "\n";
        std::cout << "Note: No explicit move needed (RVO or automatic move)\n";
    }
    std::cout << "\n";

    // 11. Container swap
    std::cout << "11. CONTAINER SWAP (O(1)):\n";
    {
        std::vector<std::string> vec1 = {"one", "two", "three"};
        std::vector<std::string> vec2 = {"four", "five"};

        std::cout << "Before swap:\n";
        std::cout << "  vec1: " << vec1.size() << " elements\n";
        std::cout << "  vec2: " << vec2.size() << " elements\n";

        std::cout << "\nSwapping (just pointer swap):\n";
        vec1.swap(vec2);

        std::cout << "After swap:\n";
        std::cout << "  vec1: " << vec1.size() << " elements\n";
        std::cout << "  vec2: " << vec2.size() << " elements\n";
    }
    std::cout << "\n";

    // 12. Performance comparison
    std::cout << "12. PERFORMANCE COMPARISON:\n";
    {
        const size_t SIZE = 10000;
        std::vector<std::vector<int>> source;
        for (size_t i = 0; i < SIZE; ++i) {
            source.push_back(std::vector<int>(100, i));
        }

        std::cout << "Copying " << SIZE << " vectors:\n";
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<int>> dest1 = source;
        auto end = std::chrono::high_resolution_clock::now();
        auto copyTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Time: " << copyTime.count() << " ms\n";

        std::cout << "\nMoving " << SIZE << " vectors:\n";
        start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<int>> dest2 = std::move(source);
        end = std::chrono::high_resolution_clock::now();
        auto moveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Time: " << moveTime.count() << " μs\n";

        std::cout << "\nSpeedup: " << (copyTime.count() * 1000.0 / moveTime.count())
                  << "x faster\n";
    }
    std::cout << "\n";

    // 13. Best practices
    std::cout << "13. BEST PRACTICES:\n";
    std::cout << "1. Use emplace_back instead of push_back when possible\n";
    std::cout << "2. Reserve capacity to avoid reallocations\n";
    std::cout << "3. Move large containers instead of copying\n";
    std::cout << "4. Use std::move algorithm for moving ranges\n";
    std::cout << "5. Return containers by value (RVO or move)\n";
    std::cout << "6. Swap for O(1) exchange\n";
    std::cout << "7. Use move-only types (unique_ptr) in containers\n";

    return 0;
}
