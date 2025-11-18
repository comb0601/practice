/*
 * Lesson 21: Move Semantics
 * File: 01_lvalue_rvalue_basics.cpp
 *
 * Understanding Lvalues and Rvalues
 *
 * This program demonstrates the fundamental difference between lvalues and rvalues,
 * which is essential for understanding move semantics.
 *
 * Compile: g++ -std=c++17 -Wall 01_lvalue_rvalue_basics.cpp -o 01_lvalue_rvalue_basics
 */

#include <iostream>
#include <string>

// Function overloads to distinguish lvalues from rvalues
void processValue(int& value) {
    std::cout << "Lvalue reference: " << value << "\n";
}

void processValue(int&& value) {
    std::cout << "Rvalue reference: " << value << "\n";
}

// Template function to show reference types
template<typename T>
void showType(T& value) {
    std::cout << "Lvalue reference template\n";
}

template<typename T>
void showType(T&& value) {
    std::cout << "Rvalue reference template (forwarding reference)\n";
}

// Function returning a value (creates rvalue)
int getValue() {
    return 42;
}

// Demonstration class
class Demo {
private:
    std::string name;

public:
    Demo(const std::string& n) : name(n) {
        std::cout << "Demo created: " << name << "\n";
    }

    ~Demo() {
        std::cout << "Demo destroyed: " << name << "\n";
    }

    void print() const {
        std::cout << "Demo name: " << name << "\n";
    }
};

int main() {
    std::cout << "=== Lvalue and Rvalue Basics ===\n\n";

    // 1. Basic lvalue examples
    std::cout << "1. LVALUE EXAMPLES:\n";
    int x = 10;
    std::cout << "int x = 10;  // x is an lvalue\n";
    std::cout << "Address of x: " << &x << "\n";
    x = 20;  // Can assign to lvalue
    std::cout << "x = 20;  // Can modify lvalue\n\n";

    // 2. Basic rvalue examples
    std::cout << "2. RVALUE EXAMPLES:\n";
    std::cout << "42 is an rvalue (literal)\n";
    std::cout << "x + 5 is an rvalue (temporary result)\n";
    // int* p = &(x + 5);  // ERROR: Cannot take address of rvalue
    std::cout << "Cannot take address of (x + 5)\n\n";

    // 3. Function overload resolution
    std::cout << "3. FUNCTION OVERLOAD RESOLUTION:\n";
    int y = 100;
    processValue(y);           // Calls lvalue version
    processValue(200);         // Calls rvalue version
    processValue(x + y);       // Calls rvalue version
    processValue(getValue());  // Calls rvalue version
    std::cout << "\n";

    // 4. Reference binding
    std::cout << "4. REFERENCE BINDING:\n";
    int z = 50;

    // Lvalue references
    int& lref = z;             // OK: lvalue ref to lvalue
    std::cout << "int& lref = z;  // OK\n";
    // int& lref2 = 50;        // ERROR: can't bind lvalue ref to rvalue
    std::cout << "int& lref2 = 50;  // ERROR\n";

    // Const lvalue references (can bind to rvalues)
    const int& clref = 50;     // OK: const lvalue ref can bind to rvalue
    std::cout << "const int& clref = 50;  // OK (special case)\n";

    // Rvalue references
    // int&& rref = z;         // ERROR: can't bind rvalue ref to lvalue
    std::cout << "int&& rref = z;  // ERROR\n";
    int&& rref = 50;           // OK: rvalue ref to rvalue
    std::cout << "int&& rref = 50;  // OK\n";
    int&& rref2 = getValue();  // OK: rvalue ref to temporary
    std::cout << "int&& rref2 = getValue();  // OK\n\n";

    // 5. Lifetime extension with rvalue references
    std::cout << "5. LIFETIME EXTENSION:\n";
    {
        // Temporary would normally be destroyed at end of expression
        // But rvalue reference extends its lifetime
        Demo&& tempRef = Demo("Temporary");
        std::cout << "Temporary lifetime extended\n";
        tempRef.print();
        std::cout << "Temporary will be destroyed at end of scope\n";
    }
    std::cout << "Scope ended\n\n";

    // 6. Named rvalue references are lvalues!
    std::cout << "6. NAMED RVALUE REFERENCES ARE LVALUES:\n";
    int&& rref3 = 100;
    std::cout << "int&& rref3 = 100;\n";
    std::cout << "rref3 is an rvalue reference type\n";
    std::cout << "But rref3 itself is an lvalue (has a name and address)\n";
    std::cout << "Address of rref3: " << &rref3 << "\n";

    // This means it binds to lvalue overload
    processValue(rref3);  // Calls lvalue version!
    std::cout << "\n";

    // 7. Array and string lvalues/rvalues
    std::cout << "7. ARRAYS AND STRINGS:\n";
    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << "Array elements are lvalues:\n";
    std::cout << "arr[0] address: " << &arr[0] << "\n";

    std::string str = "Hello";
    std::cout << "\nString variable is lvalue:\n";
    std::cout << "str address: " << &str << "\n";

    // String literals are lvalues (special case)
    std::cout << "\nString literal \"World\" has address: " << (void*)"World" << "\n";
    std::cout << "(String literals are lvalues in C++)\n\n";

    // 8. Member access
    std::cout << "8. MEMBER ACCESS:\n";
    struct Point {
        int x, y;
    };

    Point p1 = {10, 20};
    std::cout << "Point p1 is lvalue\n";
    std::cout << "p1.x is also lvalue: address = " << &p1.x << "\n";

    // But member of temporary is xvalue
    std::cout << "Point{30, 40}.x is xvalue (member of temporary)\n\n";

    // 9. Dereferencing
    std::cout << "9. DEREFERENCING:\n";
    int value = 42;
    int* ptr = &value;
    std::cout << "*ptr is lvalue (result of dereferencing)\n";
    std::cout << "Can take address: " << &(*ptr) << "\n";
    *ptr = 100;  // Can assign to dereferenced pointer
    std::cout << "Can assign: *ptr = 100\n\n";

    // 10. Summary
    std::cout << "10. SUMMARY:\n";
    std::cout << "LVALUES:\n";
    std::cout << "  - Named variables (x, y, str)\n";
    std::cout << "  - Array elements (arr[0])\n";
    std::cout << "  - Dereferenced pointers (*ptr)\n";
    std::cout << "  - Functions returning lvalue references\n";
    std::cout << "  - String literals (special case)\n";
    std::cout << "  - Have persistent address\n";
    std::cout << "\nRVALUES:\n";
    std::cout << "  - Literals (42, 3.14)\n";
    std::cout << "  - Temporary objects (Demo(\"temp\"))\n";
    std::cout << "  - Arithmetic expressions (x + y)\n";
    std::cout << "  - Functions returning by value\n";
    std::cout << "  - Casts to rvalue (std::move)\n";
    std::cout << "  - No persistent address\n";
    std::cout << "  - About to be destroyed\n";

    return 0;
}
