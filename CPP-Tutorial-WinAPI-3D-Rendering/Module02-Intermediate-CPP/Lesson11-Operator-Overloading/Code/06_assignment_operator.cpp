/*
 * Lesson 11: Operator Overloading - Assignment Operator
 *
 * This example demonstrates proper overloading of the assignment operator
 * with deep copy semantics and self-assignment check.
 *
 * Compilation: g++ -std=c++17 06_assignment_operator.cpp -o assignment
 * Execution: ./assignment
 */

#include <iostream>
#include <cstring>

class String {
private:
    char* str;
    size_t length;

public:
    // Constructor
    String(const char* s = "") {
        length = std::strlen(s);
        str = new char[length + 1];
        std::strcpy(str, s);
        std::cout << "Constructor called for: " << str << std::endl;
    }

    // Destructor
    ~String() {
        std::cout << "Destructor called for: " << str << std::endl;
        delete[] str;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        str = new char[length + 1];
        std::strcpy(str, other.str);
        std::cout << "Copy constructor called for: " << str << std::endl;
    }

    // Assignment operator
    String& operator=(const String& other) {
        std::cout << "Assignment operator called" << std::endl;

        // Check for self-assignment
        if (this == &other) {
            std::cout << "Self-assignment detected, returning *this" << std::endl;
            return *this;
        }

        // Free existing resource
        delete[] str;

        // Allocate new resource and copy
        length = other.length;
        str = new char[length + 1];
        std::strcpy(str, other.str);

        return *this;
    }

    // Overload += operator
    String& operator+=(const String& other) {
        size_t newLength = length + other.length;
        char* newStr = new char[newLength + 1];

        std::strcpy(newStr, str);
        std::strcat(newStr, other.str);

        delete[] str;
        str = newStr;
        length = newLength;

        return *this;
    }

    void display() const {
        std::cout << str;
    }

    const char* c_str() const {
        return str;
    }
};

int main() {
    std::cout << "=== Assignment Operator Demo ===" << std::endl << std::endl;

    String s1("Hello");
    String s2("World");

    std::cout << "\nInitial strings:" << std::endl;
    std::cout << "s1 = "; s1.display(); std::cout << std::endl;
    std::cout << "s2 = "; s2.display(); std::cout << std::endl;

    std::cout << "\nAssigning s2 = s1:" << std::endl;
    s2 = s1;
    std::cout << "s2 = "; s2.display(); std::cout << std::endl;

    std::cout << "\nTesting self-assignment (s1 = s1):" << std::endl;
    s1 = s1;

    std::cout << "\nChained assignment (s3 = s2 = s1):" << std::endl;
    String s3("Test");
    s3 = s2 = s1;
    std::cout << "s3 = "; s3.display(); std::cout << std::endl;

    std::cout << "\nUsing += operator:" << std::endl;
    String s4("C++");
    String s5(" Programming");
    std::cout << "Before: s4 = "; s4.display(); std::cout << std::endl;
    s4 += s5;
    std::cout << "After s4 += s5: "; s4.display(); std::cout << std::endl;

    std::cout << "\nProgram ending (destructors will be called):" << std::endl;
    return 0;
}
