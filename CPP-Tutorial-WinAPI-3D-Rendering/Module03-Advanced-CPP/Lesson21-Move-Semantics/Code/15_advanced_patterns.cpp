/*
 * Lesson 21: Move Semantics
 * File: 15_advanced_patterns.cpp
 *
 * Advanced Move Semantics Patterns
 *
 * This program demonstrates advanced patterns and techniques using move
 * semantics including copy-and-swap, move-if-noexcept, and more.
 *
 * Compile: g++ -std=c++17 -Wall 15_advanced_patterns.cpp -o 15_advanced_patterns
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <type_traits>

// Pattern 1: Copy-and-Swap Idiom
class CopySwapClass {
private:
    int* data;
    size_t size;

public:
    CopySwapClass(size_t s = 0) : size(s) {
        data = size > 0 ? new int[size]() : nullptr;
        std::cout << "  CopySwapClass constructed (size=" << size << ")\n";
    }

    ~CopySwapClass() {
        delete[] data;
    }

    // Copy constructor
    CopySwapClass(const CopySwapClass& other) : size(other.size) {
        data = size > 0 ? new int[size] : nullptr;
        std::copy(other.data, other.data + size, data);
        std::cout << "  CopySwapClass copy constructed\n";
    }

    // Move constructor
    CopySwapClass(CopySwapClass&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "  CopySwapClass move constructed\n";
    }

    // Unified assignment using copy-and-swap
    CopySwapClass& operator=(CopySwapClass other) {  // Pass by value
        std::cout << "  CopySwapClass unified assignment\n";
        swap(other);
        return *this;
    }

    void swap(CopySwapClass& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
    }

    size_t getSize() const { return size; }
};

// Pattern 2: Move-if-noexcept
class MayThrow {
private:
    std::string data;

public:
    MayThrow(std::string d = "") : data(std::move(d)) {}

    // Move constructor that might throw (BAD!)
    MayThrow(MayThrow&& other) : data(std::move(other.data)) {
        std::cout << "  MayThrow move constructed\n";
    }
};

class NoThrow {
private:
    std::string data;

public:
    NoThrow(std::string d = "") : data(std::move(d)) {}

    // Move constructor guaranteed not to throw (GOOD!)
    NoThrow(NoThrow&& other) noexcept : data(std::move(other.data)) {
        std::cout << "  NoThrow move constructed\n";
    }
};

// Pattern 3: Conditional move
template<typename T>
T conditionalMove(T& obj, bool shouldMove) {
    if (shouldMove) {
        return std::move(obj);  // Move
    } else {
        return obj;  // Copy
    }
}

// Pattern 4: Move and reset pattern
class MoveReset {
private:
    std::unique_ptr<int> resource;

public:
    MoveReset() : resource(std::make_unique<int>(42)) {
        std::cout << "  MoveReset created with resource\n";
    }

    // Move and reset: transfer ownership and create new resource
    MoveReset extractAndReset() {
        MoveReset temp;
        temp.resource = std::move(resource);  // Move current resource out
        resource = std::make_unique<int>(42);  // Reset with new resource
        std::cout << "  Resource extracted and reset\n";
        return temp;
    }

    bool hasResource() const { return resource != nullptr; }
    int getValue() const { return resource ? *resource : -1; }
};

// Pattern 5: Optional move (std::optional-like)
template<typename T>
class Optional {
private:
    bool hasValue;
    alignas(T) char storage[sizeof(T)];

public:
    Optional() : hasValue(false) {}

    Optional(T value) : hasValue(true) {
        new (storage) T(std::move(value));
    }

    ~Optional() {
        if (hasValue) {
            reinterpret_cast<T*>(storage)->~T();
        }
    }

    // Move constructor
    Optional(Optional&& other) noexcept : hasValue(other.hasValue) {
        if (hasValue) {
            new (storage) T(std::move(*other.get()));
        }
    }

    // Move assignment
    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            if (hasValue) {
                reinterpret_cast<T*>(storage)->~T();
            }
            hasValue = other.hasValue;
            if (hasValue) {
                new (storage) T(std::move(*other.get()));
            }
        }
        return *this;
    }

    bool has_value() const { return hasValue; }

    T* get() {
        return hasValue ? reinterpret_cast<T*>(storage) : nullptr;
    }

    T value_or(T defaultValue) && {  // Rvalue ref-qualified
        if (hasValue) {
            return std::move(*get());  // Move out value
        }
        return defaultValue;
    }
};

// Pattern 6: Builder with move
class Builder {
private:
    std::string result;

public:
    Builder& append(std::string str) {
        result += std::move(str);  // Move into result
        return *this;
    }

    Builder& appendCopy(const std::string& str) {
        result += str;  // Copy
        return *this;
    }

    // Rvalue ref-qualified getter (consumes object)
    std::string build() && {
        return std::move(result);  // Move result out
    }

    // Lvalue ref-qualified getter (copies)
    std::string build() const & {
        return result;  // Copy
    }
};

// Pattern 7: Sink arguments
class Container {
private:
    std::vector<std::string> items;

public:
    // Sink argument: accepts both lvalue and rvalue
    void add(std::string item) {
        items.push_back(std::move(item));  // Always move into container
    }

    size_t size() const { return items.size(); }
};

int main() {
    std::cout << "=== Advanced Move Semantics Patterns ===\n\n";

    // 1. Copy-and-swap idiom
    std::cout << "1. COPY-AND-SWAP IDIOM:\n";
    {
        CopySwapClass obj1(10);
        CopySwapClass obj2(20);

        std::cout << "\nAssigning lvalue (copy then swap):\n";
        obj1 = obj2;

        std::cout << "\nAssigning rvalue (move then swap):\n";
        obj1 = CopySwapClass(30);

        std::cout << "\nBenefit: Single assignment operator handles both copy and move\n";
    }
    std::cout << "\n";

    // 2. Move-if-noexcept
    std::cout << "2. MOVE-IF-NOEXCEPT:\n";
    {
        std::cout << "MayThrow is_nothrow_move_constructible: "
                  << std::is_nothrow_move_constructible<MayThrow>::value << "\n";
        std::cout << "NoThrow is_nothrow_move_constructible: "
                  << std::is_nothrow_move_constructible<NoThrow>::value << "\n\n";

        std::cout << "std::move_if_noexcept behavior:\n";
        NoThrow nt1("data");
        NoThrow nt2 = std::move_if_noexcept(nt1);  // Will move (noexcept)

        MayThrow mt1("data");
        MayThrow mt2 = std::move_if_noexcept(mt1);  // Will copy (may throw)

        std::cout << "\nVector reallocation uses move_if_noexcept:\n";
        std::cout << "  - If move is noexcept: uses move\n";
        std::cout << "  - If move may throw: uses copy (for safety)\n";
    }
    std::cout << "\n";

    // 3. Conditional move
    std::cout << "3. CONDITIONAL MOVE:\n";
    {
        std::string str = "Data";

        std::cout << "Conditional move (false):\n";
        std::string copy = conditionalMove(str, false);
        std::cout << "Original: \"" << str << "\" (still valid)\n";

        std::cout << "\nConditional move (true):\n";
        std::string moved = conditionalMove(str, true);
        std::cout << "Original: \"" << str << "\" (moved-from)\n";
    }
    std::cout << "\n";

    // 4. Move and reset
    std::cout << "4. MOVE AND RESET:\n";
    {
        MoveReset obj;
        std::cout << "Initial value: " << obj.getValue() << "\n";

        std::cout << "\nExtracting and resetting:\n";
        MoveReset extracted = obj.extractAndReset();

        std::cout << "Original has resource: " << obj.hasResource() << "\n";
        std::cout << "Original value: " << obj.getValue() << "\n";
        std::cout << "Extracted value: " << extracted.getValue() << "\n";
    }
    std::cout << "\n";

    // 5. Optional with move
    std::cout << "5. OPTIONAL WITH MOVE:\n";
    {
        Optional<std::string> opt1(std::string("Value"));
        std::cout << "opt1 has value: " << opt1.has_value() << "\n";

        std::cout << "\nMoving optional:\n";
        Optional<std::string> opt2 = std::move(opt1);
        std::cout << "opt2 has value: " << opt2.has_value() << "\n";

        std::cout << "\nExtracting value with move:\n";
        std::string value = std::move(opt2).value_or("default");
        std::cout << "Extracted: \"" << value << "\"\n";
    }
    std::cout << "\n";

    // 6. Builder with ref-qualifiers
    std::cout << "6. BUILDER WITH REF-QUALIFIERS:\n";
    {
        std::cout << "Building and moving result:\n";
        std::string result1 = Builder()
            .append("Hello ")
            .append("World")
            .build();  // Calls rvalue overload (moves)

        std::cout << "Result: \"" << result1 << "\"\n";

        std::cout << "\nBuilding and copying:\n";
        Builder builder;
        builder.append("Test");
        std::string result2 = builder.build();  // Calls lvalue overload (copies)

        std::cout << "Result: \"" << result2 << "\"\n";
        std::cout << "Builder still valid for reuse\n";
    }
    std::cout << "\n";

    // 7. Sink arguments
    std::cout << "7. SINK ARGUMENTS:\n";
    {
        Container cont;

        std::string str1 = "Item1";
        std::cout << "Adding lvalue (copied to parameter, moved to container):\n";
        cont.add(str1);
        std::cout << "str1: \"" << str1 << "\" (still valid)\n";

        std::cout << "\nAdding rvalue (moved to parameter, moved to container):\n";
        cont.add("Item2");

        std::cout << "\nAdding with std::move:\n";
        std::string str2 = "Item3";
        cont.add(std::move(str2));
        std::cout << "str2: \"" << str2 << "\" (moved-from)\n";

        std::cout << "\nContainer size: " << cont.size() << "\n";
    }
    std::cout << "\n";

    // 8. Perfect forwarding factory
    std::cout << "8. PERFECT FORWARDING FACTORY:\n";
    {
        auto makeVector = [](auto&&... args) {
            std::vector<std::string> vec;
            vec.reserve(sizeof...(args));
            (vec.push_back(std::forward<decltype(args)>(args)), ...);
            return vec;
        };

        std::string s1 = "First";
        std::string s2 = "Second";

        auto vec = makeVector(s1, std::move(s2), "Third");
        std::cout << "Vector created with " << vec.size() << " elements\n";
        std::cout << "s1: \"" << s1 << "\" (copied)\n";
        std::cout << "s2: \"" << s2 << "\" (moved)\n";
    }
    std::cout << "\n";

    // 9. Move with exception safety
    std::cout << "9. EXCEPTION SAFETY:\n";
    {
        std::cout << "Strong exception guarantee:\n";
        std::cout << "  1. Allocate new resources\n";
        std::cout << "  2. If allocation fails, original unchanged\n";
        std::cout << "  3. Only after success, swap/move resources\n";
        std::cout << "  4. Mark move operations noexcept\n";
        std::cout << "\nWhy noexcept matters:\n";
        std::cout << "  - Enables moves in exception-safe code\n";
        std::cout << "  - STL containers rely on noexcept\n";
        std::cout << "  - Better performance (no try/catch)\n";
    }
    std::cout << "\n";

    // 10. Summary of patterns
    std::cout << "10. PATTERN SUMMARY:\n";
    std::cout << "\nCopy-and-Swap:\n";
    std::cout << "  - Single assignment operator\n";
    std::cout << "  - Exception safe\n";
    std::cout << "  - Pass parameter by value\n\n";

    std::cout << "Move-if-noexcept:\n";
    std::cout << "  - Conditional move based on noexcept\n";
    std::cout << "  - Used by STL containers\n";
    std::cout << "  - Safety over performance\n\n";

    std::cout << "Sink Arguments:\n";
    std::cout << "  - Pass by value + std::move\n";
    std::cout << "  - Single function for copy/move\n";
    std::cout << "  - Efficient and simple\n\n";

    std::cout << "Ref-qualifiers:\n";
    std::cout << "  - Different behavior for lvalue/rvalue\n";
    std::cout << "  - Enable move-out operations\n";
    std::cout << "  - Used in Optional, Builder patterns\n\n";

    std::cout << "Move-and-Reset:\n";
    std::cout << "  - Transfer ownership\n";
    std::cout << "  - Reacquire new resource\n";
    std::cout << "  - Useful for resource pools\n";

    return 0;
}
