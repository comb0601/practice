/*
 * Lesson 11: Operator Overloading - Basic Arithmetic Operators
 *
 * This example demonstrates how to overload basic arithmetic operators
 * (+, -, *, /) for a custom Complex number class.
 *
 * Compilation: g++ -std=c++17 01_basic_operator_overloading.cpp -o basic_op
 * Execution: ./basic_op
 */

#include <iostream>
#include <cmath>

class Complex {
private:
    double real;
    double imag;

public:
    // Constructor
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Overload + operator
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Overload - operator
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // Overload * operator
    Complex operator*(const Complex& other) const {
        // (a + bi)(c + di) = (ac - bd) + (ad + bc)i
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    // Overload / operator
    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return Complex();
        }
        return Complex(
            (real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator
        );
    }

    // Display the complex number
    void display() const {
        if (imag >= 0)
            std::cout << real << " + " << imag << "i";
        else
            std::cout << real << " - " << -imag << "i";
    }

    // Get magnitude
    double magnitude() const {
        return std::sqrt(real * real + imag * imag);
    }
};

int main() {
    std::cout << "=== Basic Operator Overloading Demo ===" << std::endl << std::endl;

    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);

    std::cout << "c1 = ";
    c1.display();
    std::cout << std::endl;

    std::cout << "c2 = ";
    c2.display();
    std::cout << std::endl << std::endl;

    // Addition
    Complex sum = c1 + c2;
    std::cout << "c1 + c2 = ";
    sum.display();
    std::cout << std::endl;

    // Subtraction
    Complex diff = c1 - c2;
    std::cout << "c1 - c2 = ";
    diff.display();
    std::cout << std::endl;

    // Multiplication
    Complex prod = c1 * c2;
    std::cout << "c1 * c2 = ";
    prod.display();
    std::cout << std::endl;

    // Division
    Complex quot = c1 / c2;
    std::cout << "c1 / c2 = ";
    quot.display();
    std::cout << std::endl;

    std::cout << std::endl << "Magnitude of c1: " << c1.magnitude() << std::endl;

    return 0;
}
