/*
 * Lesson 11: Operator Overloading
 * File: 07_assignment_operators.cpp
 *
 * Demonstrates assignment operator overloading including:
 * - Copy assignment (=)
 * - Compound assignment (+=, -=, *=, /=, etc.)
 * - Self-assignment handling
 */

#include <iostream>
#include <cstring>
#include <stdexcept>

// String class with proper assignment operators
class String {
private:
    char* data;
    size_t length;

public:
    // Default constructor
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        std::cout << "String(\"" << data << "\") created\n";
    }

    // Destructor
    ~String() {
        std::cout << "String(\"" << data << "\") destroyed\n";
        delete[] data;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        std::cout << "String copy constructed: \"" << data << "\"\n";
    }

    // Copy assignment operator
    String& operator=(const String& other) {
        std::cout << "Copy assignment: \"" << data << "\" = \"" << other.data << "\"\n";

        // Self-assignment check (critical!)
        if (this == &other) {
            std::cout << "  (self-assignment detected, skipping)\n";
            return *this;
        }

        // Release old memory
        delete[] data;

        // Allocate new memory and copy
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);

        return *this;  // Return reference for chaining
    }

    // Compound assignment: concatenation
    String& operator+=(const String& other) {
        std::cout << "Compound +=: \"" << data << "\" += \"" << other.data << "\"\n";

        size_t newLength = length + other.length;
        char* newData = new char[newLength + 1];

        strcpy(newData, data);
        strcat(newData, other.data);

        delete[] data;
        data = newData;
        length = newLength;

        return *this;
    }

    // Getters
    const char* c_str() const { return data; }
    size_t getLength() const { return length; }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        os << s.data;
        return os;
    }
};

// Number class with compound assignment operators
class Number {
private:
    int value;

public:
    // Constructor
    Number(int v = 0) : value(v) {}

    // Getter
    int getValue() const { return value; }

    // Copy assignment
    Number& operator=(const Number& other) {
        std::cout << "Copy assignment: " << value << " = " << other.value << "\n";
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    // Compound addition assignment
    Number& operator+=(const Number& other) {
        std::cout << value << " += " << other.value;
        value += other.value;
        std::cout << " -> " << value << "\n";
        return *this;
    }

    // Compound subtraction assignment
    Number& operator-=(const Number& other) {
        std::cout << value << " -= " << other.value;
        value -= other.value;
        std::cout << " -> " << value << "\n";
        return *this;
    }

    // Compound multiplication assignment
    Number& operator*=(const Number& other) {
        std::cout << value << " *= " << other.value;
        value *= other.value;
        std::cout << " -> " << value << "\n";
        return *this;
    }

    // Compound division assignment
    Number& operator/=(const Number& other) {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero");
        }
        std::cout << value << " /= " << other.value;
        value /= other.value;
        std::cout << " -> " << value << "\n";
        return *this;
    }

    // Compound modulo assignment
    Number& operator%=(const Number& other) {
        if (other.value == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        std::cout << value << " %= " << other.value;
        value %= other.value;
        std::cout << " -> " << value << "\n";
        return *this;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Number& n) {
        os << n.value;
        return os;
    }
};

// Point class demonstrating assignment with resource management
class Point {
private:
    double* coords;  // Dynamically allocated coordinates

public:
    // Constructor
    Point(double x = 0, double y = 0) {
        coords = new double[2];
        coords[0] = x;
        coords[1] = y;
        std::cout << "Point(" << x << ", " << y << ") created\n";
    }

    // Destructor
    ~Point() {
        std::cout << "Point(" << coords[0] << ", " << coords[1] << ") destroyed\n";
        delete[] coords;
    }

    // Copy constructor
    Point(const Point& other) {
        coords = new double[2];
        coords[0] = other.coords[0];
        coords[1] = other.coords[1];
        std::cout << "Point copy constructed\n";
    }

    // Copy assignment operator (proper implementation)
    Point& operator=(const Point& other) {
        std::cout << "Point assignment: (" << coords[0] << ", " << coords[1]
                  << ") = (" << other.coords[0] << ", " << other.coords[1] << ")\n";

        if (this != &other) {
            // Since we have fixed size, we can just copy values
            coords[0] = other.coords[0];
            coords[1] = other.coords[1];
        }

        return *this;
    }

    // Compound assignment (translate point)
    Point& operator+=(const Point& other) {
        coords[0] += other.coords[0];
        coords[1] += other.coords[1];
        return *this;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.coords[0] << ", " << p.coords[1] << ")";
        return os;
    }
};

int main() {
    std::cout << "=== Assignment Operators Demo ===\n\n";

    // String class demo
    std::cout << "--- String Class (Copy Assignment) ---\n";
    String s1("Hello");
    String s2("World");

    std::cout << "\nBefore assignment:\n";
    std::cout << "s1 = " << s1 << "\n";
    std::cout << "s2 = " << s2 << "\n";

    std::cout << "\nAssignment s2 = s1:\n";
    s2 = s1;

    std::cout << "\nAfter assignment:\n";
    std::cout << "s1 = " << s1 << "\n";
    std::cout << "s2 = " << s2 << "\n";

    // Self-assignment test
    std::cout << "\n--- Self-Assignment Test ---\n";
    std::cout << "s1 = s1:\n";
    s1 = s1;
    std::cout << "s1 = " << s1 << " (unchanged)\n";

    // Chained assignment
    std::cout << "\n--- Chained Assignment ---\n";
    String s3("AAA");
    String s4("BBB");
    String s5("CCC");
    std::cout << "\nBefore: s3=" << s3 << ", s4=" << s4 << ", s5=" << s5 << "\n";
    std::cout << "\nExecuting: s5 = s4 = s3\n";
    s5 = s4 = s3;
    std::cout << "\nAfter: s3=" << s3 << ", s4=" << s4 << ", s5=" << s5 << "\n";

    // Compound assignment (concatenation)
    std::cout << "\n--- Compound Assignment (String) ---\n";
    String s6("Hello");
    String s7(" World");
    std::cout << "s6 = " << s6 << "\n";
    std::cout << "s7 = " << s7 << "\n";
    std::cout << "\ns6 += s7:\n";
    s6 += s7;
    std::cout << "Result: s6 = " << s6 << "\n";

    // Number class demo
    std::cout << "\n--- Number Class (Compound Assignments) ---\n";
    Number n1(100);
    Number n2(5);

    std::cout << "\nInitial values: n1 = " << n1 << ", n2 = " << n2 << "\n\n";

    Number n3 = n1;  // Copy constructor
    std::cout << "n3 += n2:\n";
    n3 += n2;
    std::cout << "Result: n3 = " << n3 << "\n\n";

    Number n4 = n1;
    std::cout << "n4 -= n2:\n";
    n4 -= n2;
    std::cout << "Result: n4 = " << n4 << "\n\n";

    Number n5 = n1;
    std::cout << "n5 *= n2:\n";
    n5 *= n2;
    std::cout << "Result: n5 = " << n5 << "\n\n";

    Number n6 = n1;
    std::cout << "n6 /= n2:\n";
    n6 /= n2;
    std::cout << "Result: n6 = " << n6 << "\n\n";

    Number n7 = n1;
    std::cout << "n7 %= n2:\n";
    n7 %= n2;
    std::cout << "Result: n7 = " << n7 << "\n";

    // Chained compound assignments
    std::cout << "\n--- Chained Compound Assignments ---\n";
    Number n8(100);
    Number n9(10);
    Number n10(2);
    std::cout << "Initial: n8 = " << n8 << ", n9 = " << n9 << ", n10 = " << n10 << "\n";
    std::cout << "\nExecuting: n8 -= n9 -= n10\n";
    n8 -= n9 -= n10;
    std::cout << "Result: n8 = " << n8 << ", n9 = " << n9 << "\n";

    // Point class demo
    std::cout << "\n--- Point Class (Resource Management) ---\n";
    Point p1(10, 20);
    Point p2(5, 7);

    std::cout << "\nBefore assignment:\n";
    std::cout << "p1 = " << p1 << "\n";
    std::cout << "p2 = " << p2 << "\n";

    std::cout << "\nAssignment p1 = p2:\n";
    p1 = p2;

    std::cout << "\nAfter assignment:\n";
    std::cout << "p1 = " << p1 << "\n";
    std::cout << "p2 = " << p2 << "\n";

    // Compound assignment (translation)
    std::cout << "\n--- Point Translation (Compound +=) ---\n";
    Point p3(100, 200);
    Point p4(10, 20);
    std::cout << "p3 = " << p3 << "\n";
    std::cout << "p4 = " << p4 << "\n";
    std::cout << "p3 += p4:\n";
    p3 += p4;
    std::cout << "Result: p3 = " << p3 << "\n";

    std::cout << "\n--- Program End (Objects Being Destroyed) ---\n";

    return 0;
}

/*
 * EXPECTED OUTPUT (partial):
 * =================================
 * === Assignment Operators Demo ===
 *
 * --- String Class (Copy Assignment) ---
 * String("Hello") created
 * String("World") created
 *
 * Before assignment:
 * s1 = Hello
 * s2 = World
 *
 * Assignment s2 = s1:
 * Copy assignment: "World" = "Hello"
 *
 * After assignment:
 * s1 = Hello
 * s2 = Hello
 *
 * --- Self-Assignment Test ---
 * s1 = s1:
 * Copy assignment: "Hello" = "Hello"
 *   (self-assignment detected, skipping)
 * s1 = Hello (unchanged)
 *
 * --- Chained Assignment ---
 * String("AAA") created
 * String("BBB") created
 * String("CCC") created
 *
 * Before: s3=AAA, s4=BBB, s5=CCC
 *
 * Executing: s5 = s4 = s3
 * Copy assignment: "BBB" = "AAA"
 * Copy assignment: "CCC" = "AAA"
 *
 * After: s3=AAA, s4=AAA, s5=AAA
 *
 * --- Compound Assignment (String) ---
 * String("Hello") created
 * String(" World") created
 * s6 = Hello
 * s7 =  World
 *
 * s6 += s7:
 * Compound +=: "Hello" += " World"
 * Result: s6 = Hello World
 *
 * ... (more output follows)
 */
