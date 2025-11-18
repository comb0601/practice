/*
 * Lesson 11: Operator Overloading
 * File: 09_type_conversion_operators.cpp
 *
 * Demonstrates type conversion operators:
 * - Conversion constructors
 * - Conversion operators
 * - Explicit conversions
 * - Implicit conversions
 */

#include <iostream>
#include <string>
#include <sstream>

// Fraction class with conversion operators
class Fraction {
private:
    int numerator;
    int denominator;

    // Helper function to simplify fraction
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

    void simplify() {
        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        if (denominator < 0) {  // Keep denominator positive
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    // Regular constructor
    Fraction(int num, int denom = 1) : numerator(num), denominator(denom) {
        if (denom == 0) {
            throw std::runtime_error("Denominator cannot be zero");
        }
        simplify();
        std::cout << "Fraction(" << numerator << "/" << denominator << ") created\n";
    }

    // Conversion constructor: int -> Fraction (implicit)
    // This allows: Fraction f = 5;
    /*
    Fraction(int num) : numerator(num), denominator(1) {
        std::cout << "Implicit conversion: int " << num << " -> Fraction\n";
    }
    */

    // Conversion operator: Fraction -> double
    operator double() const {
        std::cout << "Converting Fraction to double\n";
        return static_cast<double>(numerator) / denominator;
    }

    // Explicit conversion operator: Fraction -> int (truncates)
    explicit operator int() const {
        std::cout << "Converting Fraction to int (truncating)\n";
        return numerator / denominator;
    }

    // Conversion operator: Fraction -> bool
    // True if non-zero
    explicit operator bool() const {
        return numerator != 0;
    }

    // Conversion operator: Fraction -> string
    operator std::string() const {
        std::ostringstream oss;
        oss << numerator << "/" << denominator;
        return oss.str();
    }

    // Getters
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.numerator << "/" << f.denominator;
        return os;
    }
};

// Temperature class with implicit conversions
class Celsius {
private:
    double temp;

public:
    // Regular constructor
    explicit Celsius(double t) : temp(t) {
        std::cout << "Celsius(" << t << ") created\n";
    }

    // Conversion operator: Celsius -> double
    operator double() const {
        return temp;
    }

    // Get raw temperature
    double getTemp() const { return temp; }

    friend std::ostream& operator<<(std::ostream& os, const Celsius& c) {
        os << c.temp << "°C";
        return os;
    }
};

class Fahrenheit {
private:
    double temp;

public:
    // Regular constructor
    explicit Fahrenheit(double t) : temp(t) {
        std::cout << "Fahrenheit(" << t << ") created\n";
    }

    // Conversion constructor: Celsius -> Fahrenheit
    Fahrenheit(const Celsius& c) {
        temp = c.getTemp() * 9.0 / 5.0 + 32.0;
        std::cout << "Converting Celsius to Fahrenheit\n";
    }

    // Conversion operator: Fahrenheit -> double
    operator double() const {
        return temp;
    }

    double getTemp() const { return temp; }

    friend std::ostream& operator<<(std::ostream& os, const Fahrenheit& f) {
        os << f.temp << "°F";
        return os;
    }
};

// Money class demonstrating explicit conversions
class Money {
private:
    int dollars;
    int cents;

public:
    Money(int d, int c = 0) : dollars(d), cents(c) {
        // Normalize cents
        dollars += cents / 100;
        cents = cents % 100;
        if (cents < 0) {
            cents += 100;
            dollars -= 1;
        }
    }

    // Explicit conversion from cents
    explicit Money(int totalCents) {
        dollars = totalCents / 100;
        cents = totalCents % 100;
    }

    // Explicit conversion to double
    explicit operator double() const {
        return dollars + cents / 100.0;
    }

    // Explicit conversion to int (total cents)
    explicit operator int() const {
        return dollars * 100 + cents;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        os << "$" << m.dollars << ".";
        if (m.cents < 10) os << "0";
        os << m.cents;
        return os;
    }
};

// Distance class with user-defined conversions
class Meters {
private:
    double value;

public:
    explicit Meters(double v) : value(v) {}

    operator double() const { return value; }

    friend std::ostream& operator<<(std::ostream& os, const Meters& m) {
        os << m.value << "m";
        return os;
    }
};

class Feet {
private:
    double value;

public:
    explicit Feet(double v) : value(v) {}

    // Conversion: Meters -> Feet
    Feet(const Meters& m) {
        value = static_cast<double>(m) * 3.28084;
    }

    operator double() const { return value; }

    friend std::ostream& operator<<(std::ostream& os, const Feet& f) {
        os << f.value << "ft";
        return os;
    }
};

int main() {
    std::cout << "=== Type Conversion Operators Demo ===\n\n";

    // Fraction conversions
    std::cout << "--- Fraction Conversions ---\n";
    Fraction f1(3, 4);
    std::cout << "f1 = " << f1 << "\n\n";

    // Implicit conversion to double
    std::cout << "Implicit conversion to double:\n";
    double d1 = f1;  // Calls operator double()
    std::cout << "double d1 = f1 = " << d1 << "\n\n";

    // Explicit conversion to int
    std::cout << "Explicit conversion to int:\n";
    // int i1 = f1;  // Error: explicit conversion required
    int i1 = static_cast<int>(f1);  // Calls explicit operator int()
    std::cout << "int i1 = static_cast<int>(f1) = " << i1 << "\n\n";

    // Conversion to string
    std::cout << "Conversion to string:\n";
    std::string s1 = static_cast<std::string>(f1);
    std::cout << "string s1 = " << s1 << "\n\n";

    // Conversion to bool
    std::cout << "Conversion to bool:\n";
    Fraction f2(0, 1);
    Fraction f3(5, 2);
    if (static_cast<bool>(f3)) {
        std::cout << f3 << " is non-zero (true)\n";
    }
    if (!static_cast<bool>(f2)) {
        std::cout << f2 << " is zero (false)\n";
    }
    std::cout << "\n";

    // Temperature conversions
    std::cout << "--- Temperature Conversions ---\n";
    Celsius c1(100.0);  // Boiling point of water
    std::cout << "c1 = " << c1 << "\n";

    Fahrenheit f4(c1);  // Conversion constructor
    std::cout << "Fahrenheit f4(c1) = " << f4 << "\n\n";

    Celsius c2(0.0);  // Freezing point
    std::cout << "c2 = " << c2 << "\n";
    Fahrenheit f5(c2);
    std::cout << "Fahrenheit f5(c2) = " << f5 << "\n\n";

    // Money conversions
    std::cout << "--- Money Conversions (Explicit) ---\n";
    Money m1(10, 50);
    std::cout << "m1 = " << m1 << "\n";

    // Explicit conversion to double
    double moneyAsDouble = static_cast<double>(m1);
    std::cout << "As double: " << moneyAsDouble << "\n";

    // Explicit conversion to int (total cents)
    int totalCents = static_cast<int>(m1);
    std::cout << "Total cents: " << totalCents << "\n\n";

    // Money from cents
    Money m2 = Money(500);  // Explicit constructor
    std::cout << "m2 (from 500 cents) = " << m2 << "\n\n";

    // Distance conversions
    std::cout << "--- Distance Conversions ---\n";
    Meters m3(10.0);
    std::cout << "m3 = " << m3 << "\n";

    Feet f6(m3);  // Conversion constructor
    std::cout << "Feet f6(m3) = " << f6 << "\n\n";

    // Using in expressions
    std::cout << "--- Using Conversions in Expressions ---\n";
    Fraction f7(1, 2);
    Fraction f8(1, 3);
    std::cout << "f7 = " << f7 << "\n";
    std::cout << "f8 = " << f8 << "\n";

    // Fractions converted to double for arithmetic
    double sum = static_cast<double>(f7) + static_cast<double>(f8);
    std::cout << "f7 + f8 (as doubles) = " << sum << "\n\n";

    // Demonstrating why explicit is important
    std::cout << "--- Why 'explicit' Matters ---\n";
    std::cout << "Without 'explicit', unintended conversions can occur:\n";
    std::cout << "Money m = 500;  // Would implicitly convert int to Money\n";
    std::cout << "This can lead to bugs and ambiguous code.\n\n";
    std::cout << "With 'explicit', you must be explicit:\n";
    std::cout << "Money m = Money(500);  // Clear intent\n\n";

    // Safe bool idiom
    std::cout << "--- Safe Bool Usage ---\n";
    Fraction f9(5, 2);
    Fraction f10(0, 1);

    if (static_cast<bool>(f9)) {
        std::cout << f9 << " is non-zero, can use safely\n";
    }

    if (!static_cast<bool>(f10)) {
        std::cout << f10 << " is zero, division would fail\n";
    }

    std::cout << "\n--- Conversion Best Practices ---\n";
    std::cout << "1. Use 'explicit' for single-argument constructors\n";
    std::cout << "2. Use 'explicit' for conversion operators that might be surprising\n";
    std::cout << "3. Only provide implicit conversions when they're natural and safe\n";
    std::cout << "4. Document which conversions are available\n";
    std::cout << "5. Be careful with ambiguous conversions\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Type Conversion Operators Demo ===
 *
 * --- Fraction Conversions ---
 * Fraction(3/4) created
 * f1 = 3/4
 *
 * Implicit conversion to double:
 * Converting Fraction to double
 * double d1 = f1 = 0.75
 *
 * Explicit conversion to int:
 * Converting Fraction to int (truncating)
 * int i1 = static_cast<int>(f1) = 0
 *
 * Conversion to string:
 * string s1 = 3/4
 *
 * Conversion to bool:
 * Fraction(0/1) created
 * Fraction(5/2) created
 * 5/2 is non-zero (true)
 * 0/1 is zero (false)
 *
 * --- Temperature Conversions ---
 * Celsius(100) created
 * c1 = 100°C
 * Converting Celsius to Fahrenheit
 * Fahrenheit(212) created
 * Fahrenheit f4(c1) = 212°F
 *
 * Celsius(0) created
 * c2 = 0°C
 * Converting Celsius to Fahrenheit
 * Fahrenheit(32) created
 * Fahrenheit f5(c2) = 32°F
 *
 * --- Money Conversions (Explicit) ---
 * m1 = $10.50
 * As double: 10.5
 * Total cents: 1050
 *
 * m2 (from 500 cents) = $5.00
 *
 * --- Distance Conversions ---
 * m3 = 10m
 * Feet f6(m3) = 32.8084ft
 *
 * --- Using Conversions in Expressions ---
 * Fraction(1/2) created
 * Fraction(1/3) created
 * f7 = 1/2
 * f8 = 1/3
 * Converting Fraction to double
 * Converting Fraction to double
 * f7 + f8 (as doubles) = 0.833333
 *
 * --- Why 'explicit' Matters ---
 * Without 'explicit', unintended conversions can occur:
 * Money m = 500;  // Would implicitly convert int to Money
 * This can lead to bugs and ambiguous code.
 *
 * With 'explicit', you must be explicit:
 * Money m = Money(500);  // Clear intent
 *
 * --- Safe Bool Usage ---
 * 5/2 is non-zero, can use safely
 * 0/1 is zero, division would fail
 *
 * --- Conversion Best Practices ---
 * 1. Use 'explicit' for single-argument constructors
 * 2. Use 'explicit' for conversion operators that might be surprising
 * 3. Only provide implicit conversions when they're natural and safe
 * 4. Document which conversions are available
 * 5. Be careful with ambiguous conversions
 */
