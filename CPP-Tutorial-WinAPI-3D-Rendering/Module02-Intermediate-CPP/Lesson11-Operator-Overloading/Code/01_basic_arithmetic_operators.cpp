/*
 * Lesson 11: Operator Overloading
 * File: 01_basic_arithmetic_operators.cpp
 *
 * Demonstrates basic arithmetic operator overloading (+, -, *, /)
 * for a simple Number class.
 */

#include <iostream>
#include <stdexcept>

class Number {
private:
    int value;

public:
    // Constructor
    Number(int v = 0) : value(v) {
        std::cout << "Number(" << value << ") created\n";
    }

    // Getter
    int getValue() const { return value; }

    // Addition operator
    Number operator+(const Number& other) const {
        std::cout << "Calling operator+: " << value << " + " << other.value << "\n";
        return Number(value + other.value);
    }

    // Subtraction operator
    Number operator-(const Number& other) const {
        std::cout << "Calling operator-: " << value << " - " << other.value << "\n";
        return Number(value - other.value);
    }

    // Multiplication operator
    Number operator*(const Number& other) const {
        std::cout << "Calling operator*: " << value << " * " << other.value << "\n";
        return Number(value * other.value);
    }

    // Division operator
    Number operator/(const Number& other) const {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero");
        }
        std::cout << "Calling operator/: " << value << " / " << other.value << "\n";
        return Number(value / other.value);
    }

    // Modulo operator
    Number operator%(const Number& other) const {
        if (other.value == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        std::cout << "Calling operator%: " << value << " % " << other.value << "\n";
        return Number(value % other.value);
    }

    // Display function
    void display() const {
        std::cout << "Value: " << value << "\n";
    }
};

int main() {
    std::cout << "=== Basic Arithmetic Operators Demo ===\n\n";

    // Create number objects
    Number a(10);
    Number b(3);

    std::cout << "\n--- Addition ---\n";
    Number sum = a + b;
    sum.display();

    std::cout << "\n--- Subtraction ---\n";
    Number diff = a - b;
    diff.display();

    std::cout << "\n--- Multiplication ---\n";
    Number prod = a * b;
    prod.display();

    std::cout << "\n--- Division ---\n";
    Number quot = a / b;
    quot.display();

    std::cout << "\n--- Modulo ---\n";
    Number mod = a % b;
    mod.display();

    std::cout << "\n--- Chaining Operations ---\n";
    Number result = (a + b) * (a - b);
    result.display();

    std::cout << "\n--- Error Handling: Division by Zero ---\n";
    try {
        Number zero(0);
        Number error = a / zero;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }

    std::cout << "\n--- Complex Expression ---\n";
    Number x(5), y(2), z(3);
    Number complex = (x + y) * z - x / y;
    std::cout << "Result of (5 + 2) * 3 - 5 / 2 = ";
    complex.display();

    return 0;
}

/*
 * OUTPUT:
 * =================================
 * === Basic Arithmetic Operators Demo ===
 *
 * Number(10) created
 * Number(3) created
 *
 * --- Addition ---
 * Calling operator+: 10 + 3
 * Number(13) created
 * Value: 13
 *
 * --- Subtraction ---
 * Calling operator-: 10 - 3
 * Number(7) created
 * Value: 7
 *
 * --- Multiplication ---
 * Calling operator*: 10 * 3
 * Number(30) created
 * Value: 30
 *
 * --- Division ---
 * Calling operator/: 10 / 3
 * Number(3) created
 * Value: 3
 *
 * --- Modulo ---
 * Calling operator%: 10 % 3
 * Number(1) created
 * Value: 1
 *
 * --- Chaining Operations ---
 * Calling operator+: 10 + 3
 * Number(13) created
 * Calling operator-: 10 - 3
 * Number(7) created
 * Calling operator*: 13 * 7
 * Number(91) created
 * Value: 91
 *
 * --- Error Handling: Division by Zero ---
 * Number(0) created
 * Exception caught: Division by zero
 *
 * --- Complex Expression ---
 * Number(5) created
 * Number(2) created
 * Number(3) created
 * Calling operator+: 5 + 2
 * Number(7) created
 * Calling operator*: 7 * 3
 * Number(21) created
 * Calling operator/: 5 / 2
 * Number(2) created
 * Calling operator-: 21 - 2
 * Number(19) created
 * Result of (5 + 2) * 3 - 5 / 2 = Value: 19
 */
