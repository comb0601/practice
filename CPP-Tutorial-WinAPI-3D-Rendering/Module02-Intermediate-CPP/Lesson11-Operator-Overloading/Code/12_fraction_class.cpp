/*
 * Lesson 11: Operator Overloading
 * File: 12_fraction_class.cpp
 *
 * Complete Fraction class implementation with:
 * - Automatic simplification
 * - All arithmetic operators
 * - Comparison operators
 * - I/O operators
 * - Type conversions
 */

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>

class Fraction {
private:
    int numerator;
    int denominator;

    // Greatest Common Divisor (Euclidean algorithm)
    int gcd(int a, int b) const {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Least Common Multiple
    int lcm(int a, int b) const {
        return (a * b) / gcd(a, b);
    }

    // Simplify the fraction
    void simplify() {
        if (denominator == 0) {
            throw std::runtime_error("Denominator cannot be zero");
        }

        if (numerator == 0) {
            denominator = 1;
            return;
        }

        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;

        // Keep denominator positive
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    // Constructors
    Fraction(int num = 0, int denom = 1) : numerator(num), denominator(denom) {
        simplify();
    }

    // Getters
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // Addition
    Fraction operator+(const Fraction& other) const {
        int num = numerator * other.denominator + other.numerator * denominator;
        int denom = denominator * other.denominator;
        return Fraction(num, denom);
    }

    // Subtraction
    Fraction operator-(const Fraction& other) const {
        int num = numerator * other.denominator - other.numerator * denominator;
        int denom = denominator * other.denominator;
        return Fraction(num, denom);
    }

    // Multiplication
    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }

    // Division
    Fraction operator/(const Fraction& other) const {
        if (other.numerator == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }

    // Unary minus
    Fraction operator-() const {
        return Fraction(-numerator, denominator);
    }

    // Unary plus
    Fraction operator+() const {
        return *this;
    }

    // Compound assignment operators
    Fraction& operator+=(const Fraction& other) {
        *this = *this + other;
        return *this;
    }

    Fraction& operator-=(const Fraction& other) {
        *this = *this - other;
        return *this;
    }

    Fraction& operator*=(const Fraction& other) {
        *this = *this * other;
        return *this;
    }

    Fraction& operator/=(const Fraction& other) {
        *this = *this / other;
        return *this;
    }

    // Comparison operators
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>(const Fraction& other) const {
        return other < *this;
    }

    bool operator<=(const Fraction& other) const {
        return !(*this > other);
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }

    // Reciprocal
    Fraction reciprocal() const {
        if (numerator == 0) {
            throw std::runtime_error("Cannot take reciprocal of zero");
        }
        return Fraction(denominator, numerator);
    }

    // Absolute value
    Fraction abs() const {
        return Fraction(std::abs(numerator), denominator);
    }

    // Power
    Fraction pow(int exponent) const {
        if (exponent == 0) {
            return Fraction(1, 1);
        }
        if (exponent < 0) {
            return reciprocal().pow(-exponent);
        }
        Fraction result(1, 1);
        Fraction base = *this;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result *= base;
            }
            base *= base;
            exponent /= 2;
        }
        return result;
    }

    // Conversions
    explicit operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

    explicit operator float() const {
        return static_cast<float>(numerator) / denominator;
    }

    explicit operator int() const {
        return numerator / denominator;
    }

    explicit operator bool() const {
        return numerator != 0;
    }

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        if (f.denominator == 1) {
            os << f.numerator;
        } else {
            os << f.numerator << "/" << f.denominator;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Fraction& f) {
        char slash;
        is >> f.numerator;
        if (is.peek() == '/') {
            is >> slash >> f.denominator;
        } else {
            f.denominator = 1;
        }
        f.simplify();
        return is;
    }
};

// Non-member operators for mixed operations
Fraction operator+(int n, const Fraction& f) {
    return Fraction(n) + f;
}

Fraction operator-(int n, const Fraction& f) {
    return Fraction(n) - f;
}

Fraction operator*(int n, const Fraction& f) {
    return Fraction(n) * f;
}

Fraction operator/(int n, const Fraction& f) {
    return Fraction(n) / f;
}

int main() {
    std::cout << "=== Fraction Class Demo ===\n\n";

    // Create fractions
    std::cout << "--- Creating Fractions ---\n";
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    Fraction f3(2, 4);  // Will be simplified to 1/2
    Fraction f4(6, 8);  // Will be simplified to 3/4

    std::cout << "f1 = " << f1 << "\n";
    std::cout << "f2 = " << f2 << "\n";
    std::cout << "f3 = " << f3 << " (simplified from 2/4)\n";
    std::cout << "f4 = " << f4 << " (simplified from 6/8)\n\n";

    // Arithmetic operations
    std::cout << "--- Arithmetic Operations ---\n";
    std::cout << "f1 + f2 = " << (f1 + f2) << "\n";
    std::cout << "f1 - f2 = " << (f1 - f2) << "\n";
    std::cout << "f1 * f2 = " << (f1 * f2) << "\n";
    std::cout << "f1 / f2 = " << (f1 / f2) << "\n";
    std::cout << "-f1 = " << -f1 << "\n\n";

    // Complex expressions
    std::cout << "--- Complex Expressions ---\n";
    Fraction result1 = f1 + f2 * f4;
    std::cout << "f1 + f2 * f4 = " << result1 << "\n";

    Fraction result2 = (f1 + f2) / (f1 - f2);
    std::cout << "(f1 + f2) / (f1 - f2) = " << result2 << "\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    std::cout << "f1 = " << f1 << ", f3 = " << f3 << "\n";
    std::cout << "f1 == f3: " << (f1 == f3 ? "true" : "false") << "\n";
    std::cout << "f1 < f4: " << (f1 < f4 ? "true" : "false") << "\n";
    std::cout << "f4 > f2: " << (f4 > f2 ? "true" : "false") << "\n\n";

    // Compound assignment
    std::cout << "--- Compound Assignment ---\n";
    Fraction f5(1, 4);
    std::cout << "f5 = " << f5 << "\n";
    f5 += Fraction(1, 4);
    std::cout << "After f5 += 1/4: " << f5 << "\n";
    f5 *= Fraction(2, 1);
    std::cout << "After f5 *= 2: " << f5 << "\n";
    f5 -= Fraction(1, 3);
    std::cout << "After f5 -= 1/3: " << f5 << "\n\n";

    // Reciprocal
    std::cout << "--- Reciprocal ---\n";
    Fraction f6(3, 4);
    std::cout << "f6 = " << f6 << "\n";
    std::cout << "Reciprocal of f6 = " << f6.reciprocal() << "\n";
    std::cout << "f6 * reciprocal(f6) = " << (f6 * f6.reciprocal()) << "\n\n";

    // Power
    std::cout << "--- Power ---\n";
    Fraction f7(2, 3);
    std::cout << "f7 = " << f7 << "\n";
    std::cout << "f7^0 = " << f7.pow(0) << "\n";
    std::cout << "f7^1 = " << f7.pow(1) << "\n";
    std::cout << "f7^2 = " << f7.pow(2) << "\n";
    std::cout << "f7^3 = " << f7.pow(3) << "\n";
    std::cout << "f7^-1 = " << f7.pow(-1) << "\n";
    std::cout << "f7^-2 = " << f7.pow(-2) << "\n\n";

    // Mixed operations with integers
    std::cout << "--- Mixed Operations ---\n";
    Fraction f8(1, 3);
    std::cout << "f8 = " << f8 << "\n";
    std::cout << "2 + f8 = " << (2 + f8) << "\n";
    std::cout << "f8 + 2 = " << (f8 + Fraction(2)) << "\n";
    std::cout << "3 * f8 = " << (3 * f8) << "\n";
    std::cout << "5 - f8 = " << (5 - f8) << "\n\n";

    // Type conversions
    std::cout << "--- Type Conversions ---\n";
    Fraction f9(3, 4);
    std::cout << "f9 = " << f9 << "\n";
    std::cout << "As double: " << static_cast<double>(f9) << "\n";
    std::cout << "As int (truncated): " << static_cast<int>(f9) << "\n";
    std::cout << "As bool: " << (static_cast<bool>(f9) ? "true" : "false") << "\n";

    Fraction f10(0, 1);
    std::cout << "f10 = " << f10 << "\n";
    std::cout << "As bool: " << (static_cast<bool>(f10) ? "true" : "false") << "\n\n";

    // Sorting fractions
    std::cout << "--- Sorting Fractions ---\n";
    std::vector<Fraction> fractions = {
        Fraction(1, 2), Fraction(1, 3), Fraction(2, 3),
        Fraction(1, 4), Fraction(3, 4), Fraction(1, 6)
    };

    std::cout << "Unsorted: ";
    for (const auto& f : fractions) {
        std::cout << f << " ";
    }
    std::cout << "\n";

    std::sort(fractions.begin(), fractions.end());

    std::cout << "Sorted:   ";
    for (const auto& f : fractions) {
        std::cout << f << " ";
    }
    std::cout << "\n\n";

    // Real-world example: Recipe scaling
    std::cout << "--- Recipe Scaling Example ---\n";
    std::cout << "Original recipe (serves 4):\n";
    Fraction flour(2, 1);      // 2 cups
    Fraction sugar(1, 2);      // 1/2 cup
    Fraction butter(1, 4);     // 1/4 cup

    std::cout << "  Flour: " << flour << " cups\n";
    std::cout << "  Sugar: " << sugar << " cups\n";
    std::cout << "  Butter: " << butter << " cups\n\n";

    Fraction scaleFactor(3, 2);  // 1.5x (serves 6)
    std::cout << "Scaled recipe (serves 6, multiply by " << scaleFactor << "):\n";
    std::cout << "  Flour: " << (flour * scaleFactor) << " cups\n";
    std::cout << "  Sugar: " << (sugar * scaleFactor) << " cups\n";
    std::cout << "  Butter: " << (butter * scaleFactor) << " cups\n\n";

    // Financial calculations
    std::cout << "--- Financial Example ---\n";
    Fraction principal(1000, 1);      // $1000
    Fraction interestRate(1, 20);     // 5% = 1/20
    Fraction time(3, 1);              // 3 years

    Fraction simpleInterest = principal * interestRate * time;
    std::cout << "Principal: $" << principal << "\n";
    std::cout << "Interest rate: " << interestRate << " (5%)\n";
    std::cout << "Time: " << time << " years\n";
    std::cout << "Simple interest: $" << simpleInterest << "\n";
    std::cout << "Total amount: $" << (principal + simpleInterest) << "\n\n";

    // Continued fractions (approximating π)
    std::cout << "--- Mathematical Constants ---\n";
    Fraction piApprox(22, 7);  // Common approximation
    std::cout << "π ≈ " << piApprox << " = " << static_cast<double>(piApprox) << "\n";
    std::cout << "Actual π = " << M_PI << "\n";
    std::cout << "Error: " << fabs(static_cast<double>(piApprox) - M_PI) << "\n";

    return 0;
}

/*
 * OUTPUT:
 * =================================
 * === Fraction Class Demo ===
 *
 * --- Creating Fractions ---
 * f1 = 1/2
 * f2 = 1/3
 * f3 = 1/2 (simplified from 2/4)
 * f4 = 3/4 (simplified from 6/8)
 *
 * --- Arithmetic Operations ---
 * f1 + f2 = 5/6
 * f1 - f2 = 1/6
 * f1 * f2 = 1/6
 * f1 / f2 = 3/2
 * -f1 = -1/2
 *
 * --- Complex Expressions ---
 * f1 + f2 * f4 = 3/4
 * (f1 + f2) / (f1 - f2) = 5
 *
 * --- Comparison ---
 * f1 = 1/2, f3 = 1/2
 * f1 == f3: true
 * f1 < f4: true
 * f4 > f2: true
 *
 * --- Compound Assignment ---
 * f5 = 1/4
 * After f5 += 1/4: 1/2
 * After f5 *= 2: 1
 * After f5 -= 1/3: 2/3
 *
 * --- Reciprocal ---
 * f6 = 3/4
 * Reciprocal of f6 = 4/3
 * f6 * reciprocal(f6) = 1
 *
 * --- Power ---
 * f7 = 2/3
 * f7^0 = 1
 * f7^1 = 2/3
 * f7^2 = 4/9
 * f7^3 = 8/27
 * f7^-1 = 3/2
 * f7^-2 = 9/4
 *
 * ... (more output follows)
 */
