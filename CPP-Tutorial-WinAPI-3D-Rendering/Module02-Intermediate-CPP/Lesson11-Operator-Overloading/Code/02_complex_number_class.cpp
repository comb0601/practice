/*
 * Lesson 11: Operator Overloading
 * File: 02_complex_number_class.cpp
 *
 * Comprehensive Complex Number class with full operator overloading.
 * Demonstrates arithmetic, comparison, and I/O operators.
 */

#include <iostream>
#include <cmath>
#include <stdexcept>

class Complex {
private:
    double real;
    double imag;

public:
    // Constructors
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Getters
    double getReal() const { return real; }
    double getImag() const { return imag; }

    // Addition
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Subtraction
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // Multiplication: (a + bi)(c + di) = (ac - bd) + (ad + bc)i
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    // Division: (a + bi)/(c + di) = [(ac + bd) + (bc - ad)i] / (c^2 + d^2)
    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Complex(
            (real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator
        );
    }

    // Unary minus
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    // Compound assignment operators
    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    Complex& operator-=(const Complex& other) {
        real -= other.real;
        imag -= other.imag;
        return *this;
    }

    Complex& operator*=(const Complex& other) {
        double newReal = real * other.real - imag * other.imag;
        double newImag = real * other.imag + imag * other.real;
        real = newReal;
        imag = newImag;
        return *this;
    }

    // Comparison operators
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // Utility functions
    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }

    double phase() const {
        return atan2(imag, real);
    }

    Complex conjugate() const {
        return Complex(real, -imag);
    }

    // Stream output operator
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0)
            os << " + " << c.imag << "i";
        else
            os << " - " << -c.imag << "i";
        return os;
    }

    // Stream input operator
    friend std::istream& operator>>(std::istream& is, Complex& c) {
        std::cout << "Enter real part: ";
        is >> c.real;
        std::cout << "Enter imaginary part: ";
        is >> c.imag;
        return is;
    }
};

int main() {
    std::cout << "=== Complex Number Class Demo ===\n\n";

    // Create complex numbers
    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);
    Complex c3(2.0, -1.0);

    std::cout << "c1 = " << c1 << "\n";
    std::cout << "c2 = " << c2 << "\n";
    std::cout << "c3 = " << c3 << "\n\n";

    // Arithmetic operations
    std::cout << "--- Arithmetic Operations ---\n";
    std::cout << "c1 + c2 = " << (c1 + c2) << "\n";
    std::cout << "c1 - c2 = " << (c1 - c2) << "\n";
    std::cout << "c1 * c2 = " << (c1 * c2) << "\n";
    std::cout << "c1 / c2 = " << (c1 / c2) << "\n\n";

    // Unary minus
    std::cout << "--- Unary Operators ---\n";
    std::cout << "-c1 = " << -c1 << "\n\n";

    // Compound assignment
    std::cout << "--- Compound Assignment ---\n";
    Complex c4 = c1;
    std::cout << "c4 = c1 = " << c4 << "\n";
    c4 += c2;
    std::cout << "c4 += c2 = " << c4 << "\n";
    c4 -= c3;
    std::cout << "c4 -= c3 = " << c4 << "\n";
    c4 *= Complex(2, 0);
    std::cout << "c4 *= (2 + 0i) = " << c4 << "\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    std::cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << "\n";
    std::cout << "c1 != c2: " << (c1 != c2 ? "true" : "false") << "\n";
    Complex c5(3.0, 4.0);
    std::cout << "c1 == c5: " << (c1 == c5 ? "true" : "false") << "\n\n";

    // Utility functions
    std::cout << "--- Utility Functions ---\n";
    std::cout << "Magnitude of c1: " << c1.magnitude() << "\n";
    std::cout << "Phase of c1 (radians): " << c1.phase() << "\n";
    std::cout << "Conjugate of c1: " << c1.conjugate() << "\n\n";

    // Complex expressions
    std::cout << "--- Complex Expressions ---\n";
    Complex result = (c1 + c2) * c3 / (c1 - c2);
    std::cout << "(c1 + c2) * c3 / (c1 - c2) = " << result << "\n\n";

    // Division by zero
    std::cout << "--- Error Handling ---\n";
    try {
        Complex zero(0, 0);
        Complex error = c1 / zero;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n\n";
    }

    // Demonstrate mathematical identity: (a + bi)(a - bi) = a^2 + b^2
    std::cout << "--- Mathematical Identity ---\n";
    std::cout << "c1 * conjugate(c1) = " << (c1 * c1.conjugate()) << "\n";
    std::cout << "This should equal: " << (c1.getReal() * c1.getReal() + c1.getImag() * c1.getImag()) << " + 0i\n\n";

    // Powers of i: i^2 = -1
    std::cout << "--- Powers of i ---\n";
    Complex i(0, 1);
    std::cout << "i = " << i << "\n";
    std::cout << "i^2 = " << (i * i) << "\n";
    std::cout << "i^3 = " << (i * i * i) << "\n";
    std::cout << "i^4 = " << (i * i * i * i) << "\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Complex Number Class Demo ===
 *
 * c1 = 3 + 4i
 * c2 = 1 + 2i
 * c3 = 2 - 1i
 *
 * --- Arithmetic Operations ---
 * c1 + c2 = 4 + 6i
 * c1 - c2 = 2 + 2i
 * c1 * c2 = -5 + 10i
 * c1 / c2 = 2.2 + 0.4i
 *
 * --- Unary Operators ---
 * -c1 = -3 - 4i
 *
 * --- Compound Assignment ---
 * c4 = c1 = 3 + 4i
 * c4 += c2 = 4 + 6i
 * c4 -= c3 = 2 + 7i
 * c4 *= (2 + 0i) = 4 + 14i
 *
 * --- Comparison ---
 * c1 == c2: false
 * c1 != c2: true
 * c1 == c5: true
 *
 * --- Utility Functions ---
 * Magnitude of c1: 5
 * Phase of c1 (radians): 0.927295
 * Conjugate of c1: 3 - 4i
 *
 * --- Complex Expressions ---
 * (c1 + c2) * c3 / (c1 - c2) = 3.5 - 5.5i
 *
 * --- Error Handling ---
 * Exception: Division by zero
 *
 * --- Mathematical Identity ---
 * c1 * conjugate(c1) = 25 + 0i
 * This should equal: 25 + 0i
 *
 * --- Powers of i ---
 * i = 0 + 1i
 * i^2 = -1 + 0i
 * i^3 = -0 - 1i
 * i^4 = 1 - 0i
 */
