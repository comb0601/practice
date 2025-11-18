/*
 * Lesson 21: Move Semantics
 * File: 06_perfect_forwarding.cpp
 *
 * Perfect Forwarding with std::forward
 *
 * This program demonstrates perfect forwarding, which preserves the value
 * category (lvalue/rvalue) when forwarding arguments to other functions.
 *
 * Compile: g++ -std=c++17 -Wall 06_perfect_forwarding.cpp -o 06_perfect_forwarding
 */

#include <iostream>
#include <string>
#include <utility>  // for std::forward
#include <memory>

// Class to track how it's constructed
class Widget {
private:
    std::string name;

public:
    Widget(const std::string& n) : name(n) {
        std::cout << "  Widget(const string&): " << name << "\n";
    }

    Widget(const Widget& other) : name(other.name + "_copy") {
        std::cout << "  Widget(const Widget&): " << name << "\n";
    }

    Widget(Widget&& other) noexcept : name(std::move(other.name)) {
        std::cout << "  Widget(Widget&&): " << name << "\n";
    }

    const std::string& getName() const { return name; }
};

// Helper functions to see what we're calling
void process(const Widget& w) {
    std::cout << "  process(const Widget&): " << w.getName() << "\n";
}

void process(Widget&& w) {
    std::cout << "  process(Widget&&): " << w.getName() << "\n";
}

// 1. The problem: without forwarding
template<typename T>
void badWrapper(T param) {
    // param is always an lvalue here, even if T is rvalue reference
    process(param);  // Always calls lvalue version!
}

// 2. Attempt to fix with rvalue reference
template<typename T>
void stillBadWrapper(T&& param) {
    // param is still an lvalue (it has a name)
    process(param);  // Still calls lvalue version!
}

// 3. The solution: perfect forwarding
template<typename T>
void goodWrapper(T&& param) {
    // std::forward preserves the value category
    process(std::forward<T>(param));  // Calls correct version!
}

// Factory function using perfect forwarding
template<typename T, typename... Args>
std::unique_ptr<T> make_unique_perfect(Args&&... args) {
    std::cout << "  Creating object with perfect forwarding\n";
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Container that perfect forwards constructor arguments
template<typename T>
class Container {
private:
    T value;

public:
    // Perfect forwarding constructor
    template<typename U>
    explicit Container(U&& val) : value(std::forward<U>(val)) {
        std::cout << "  Container created via perfect forwarding\n";
    }
};

// Multiple arguments with perfect forwarding
template<typename T, typename Arg1, typename Arg2>
T construct(Arg1&& arg1, Arg2&& arg2) {
    return T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
}

// Class with multiple constructors
class MultiConstructor {
private:
    std::string s;
    int n;

public:
    MultiConstructor(const std::string& str, int num)
        : s(str), n(num) {
        std::cout << "  MultiConstructor(const string&, int)\n";
    }

    MultiConstructor(std::string&& str, int num)
        : s(std::move(str)), n(num) {
        std::cout << "  MultiConstructor(string&&, int)\n";
    }

    void print() const {
        std::cout << "  " << s << ", " << n << "\n";
    }
};

int main() {
    std::cout << "=== Perfect Forwarding ===\n\n";

    // 1. The problem without forwarding
    std::cout << "1. WITHOUT PERFECT FORWARDING:\n";
    Widget w1("Widget1");

    std::cout << "\nCalling badWrapper with lvalue:\n";
    badWrapper(w1);

    std::cout << "\nCalling badWrapper with rvalue:\n";
    badWrapper(Widget("Temp"));  // Should call rvalue version but doesn't!

    std::cout << "\n";

    // 2. Still problematic with rvalue reference
    std::cout << "2. RVALUE REFERENCE STILL NOT ENOUGH:\n";

    std::cout << "\nCalling stillBadWrapper with lvalue:\n";
    stillBadWrapper(w1);

    std::cout << "\nCalling stillBadWrapper with rvalue:\n";
    stillBadWrapper(Widget("Temp"));  // param is lvalue inside!

    std::cout << "\n";

    // 3. Perfect forwarding solution
    std::cout << "3. PERFECT FORWARDING (CORRECT):\n";

    std::cout << "\nCalling goodWrapper with lvalue:\n";
    goodWrapper(w1);  // Forwards as lvalue

    std::cout << "\nCalling goodWrapper with rvalue:\n";
    goodWrapper(Widget("Temp"));  // Forwards as rvalue

    std::cout << "\n";

    // 4. Forwarding references (universal references)
    std::cout << "4. FORWARDING REFERENCES:\n";
    std::cout << "T&& is a forwarding reference when:\n";
    std::cout << "  - T is a template parameter being deduced\n";
    std::cout << "  - No cv-qualifiers present\n\n";

    std::cout << "Examples:\n";
    std::cout << "template<typename T>\n";
    std::cout << "void func(T&& param);          // Forwarding reference\n\n";

    std::cout << "void func(Widget&& param);     // Rvalue reference (T known)\n";
    std::cout << "template<typename T>\n";
    std::cout << "void func(const T&& param);    // Rvalue reference (const)\n";
    std::cout << "template<typename T>\n";
    std::cout << "void func(std::vector<T>&& p); // Rvalue reference (not T&&)\n\n";

    // 5. Reference collapsing rules
    std::cout << "5. REFERENCE COLLAPSING:\n";
    std::cout << "T&  &  -> T&    (lvalue ref to lvalue ref = lvalue ref)\n";
    std::cout << "T&  && -> T&    (rvalue ref to lvalue ref = lvalue ref)\n";
    std::cout << "T&& &  -> T&    (lvalue ref to rvalue ref = lvalue ref)\n";
    std::cout << "T&& && -> T&&   (rvalue ref to rvalue ref = rvalue ref)\n\n";

    // 6. Factory function with perfect forwarding
    std::cout << "6. FACTORY FUNCTION:\n";

    std::string str = "Test";
    std::cout << "\nCreating with lvalue:\n";
    auto p1 = make_unique_perfect<Widget>(str);

    std::cout << "\nCreating with rvalue:\n";
    auto p2 = make_unique_perfect<Widget>(std::string("Temp"));

    std::cout << "\nCreating with string literal:\n";
    auto p3 = make_unique_perfect<Widget>("Literal");

    std::cout << "\n";

    // 7. Container with perfect forwarding
    std::cout << "7. CONTAINER WITH FORWARDING:\n";

    Widget w2("Original");
    std::cout << "\nCreating container with lvalue:\n";
    Container<Widget> c1(w2);

    std::cout << "\nCreating container with rvalue:\n";
    Container<Widget> c2(Widget("Temp"));

    std::cout << "\n";

    // 8. Multiple arguments
    std::cout << "8. MULTIPLE ARGUMENTS:\n";

    std::string s1 = "Lvalue";
    std::cout << "\nConstructing with lvalue + rvalue:\n";
    auto mc1 = construct<MultiConstructor>(s1, 42);

    std::cout << "\nConstructing with rvalue + rvalue:\n";
    auto mc2 = construct<MultiConstructor>(std::string("Rvalue"), 99);

    std::cout << "\n";

    // 9. std::forward implementation concept
    std::cout << "9. STD::FORWARD CONCEPT:\n";
    std::cout << "std::forward<T> does:\n";
    std::cout << "  - If T is lvalue reference: cast to lvalue reference\n";
    std::cout << "  - If T is not reference: cast to rvalue reference\n\n";

    std::cout << "Simplified:\n";
    std::cout << "template<typename T>\n";
    std::cout << "T&& forward(remove_reference_t<T>& arg) noexcept {\n";
    std::cout << "    return static_cast<T&&>(arg);\n";
    std::cout << "}\n\n";

    // 10. Perfect forwarding with variadic templates
    std::cout << "10. VARIADIC PERFECT FORWARDING:\n";

    auto variadicFactory = [](auto&&... args) {
        std::cout << "  Forwarding " << sizeof...(args) << " arguments\n";
        return MultiConstructor(std::forward<decltype(args)>(args)...);
    };

    std::string name = "Forward";
    std::cout << "\nCalling variadic factory:\n";
    auto mc3 = variadicFactory(name, 123);
    mc3.print();

    std::cout << "\n";

    // 11. When perfect forwarding matters
    std::cout << "11. WHEN PERFECT FORWARDING MATTERS:\n";
    std::cout << "Use perfect forwarding when:\n";
    std::cout << "  - Writing generic wrapper functions\n";
    std::cout << "  - Implementing factory functions (make_unique, emplace)\n";
    std::cout << "  - Creating forwarding constructors\n";
    std::cout << "  - Writing container operations (emplace_back)\n";
    std::cout << "  - Any time you need to preserve value category\n\n";

    // 12. Perfect forwarding vs std::move
    std::cout << "12. PERFECT FORWARDING VS STD::MOVE:\n";
    std::cout << "std::move:\n";
    std::cout << "  - Unconditionally casts to rvalue\n";
    std::cout << "  - Use when you know you want to move\n";
    std::cout << "  - Doesn't preserve value category\n\n";

    std::cout << "std::forward:\n";
    std::cout << "  - Conditionally casts based on template parameter\n";
    std::cout << "  - Use in template forwarding functions\n";
    std::cout << "  - Preserves value category\n\n";

    // 13. Common pattern: wrapper functions
    std::cout << "13. WRAPPER FUNCTION PATTERN:\n";
    std::cout << "template<typename... Args>\n";
    std::cout << "auto wrapper(Args&&... args) {\n";
    std::cout << "    // Do something before\n";
    std::cout << "    auto result = target(std::forward<Args>(args)...);\n";
    std::cout << "    // Do something after\n";
    std::cout << "    return result;\n";
    std::cout << "}\n\n";

    // 14. Perfect forwarding with return values
    std::cout << "14. FORWARDING RETURN VALUES:\n";

    auto forwardReturn = [](auto&& func, auto&&... args) {
        std::cout << "  Forwarding call to function\n";
        return std::forward<decltype(func)>(func)(
            std::forward<decltype(args)>(args)...
        );
    };

    auto makeWidget = [](const std::string& name) {
        return Widget(name);
    };

    std::cout << "\nForwarding function call:\n";
    Widget w3 = forwardReturn(makeWidget, "Forwarded");

    std::cout << "\n";

    // 15. Summary
    std::cout << "15. SUMMARY:\n";
    std::cout << "Perfect Forwarding:\n";
    std::cout << "  - Preserves value category (lvalue/rvalue)\n";
    std::cout << "  - Uses forwarding references (T&&)\n";
    std::cout << "  - Requires std::forward<T>\n";
    std::cout << "  - Enables efficient generic code\n";
    std::cout << "  - Essential for factories and wrappers\n\n";

    std::cout << "Pattern:\n";
    std::cout << "template<typename T>\n";
    std::cout << "void wrapper(T&& arg) {\n";
    std::cout << "    target(std::forward<T>(arg));\n";
    std::cout << "}\n";

    std::cout << "\nDestructing objects:\n";
    return 0;
}
