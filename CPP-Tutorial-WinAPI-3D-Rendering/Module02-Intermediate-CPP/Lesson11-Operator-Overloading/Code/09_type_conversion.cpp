/*
 * Lesson 11: Operator Overloading - Type Conversion Operators
 *
 * This example demonstrates conversion operators that allow
 * implicit and explicit type conversions for custom classes.
 *
 * Compilation: g++ -std=c++17 09_type_conversion.cpp -o conversion
 * Execution: ./conversion
 */

#include <iostream>
#include <string>

class Fraction {
private:
    int numerator;
    int denominator;

    int gcd(int a, int b) const {
        return b == 0 ? a : gcd(b, a % b);
    }

    void simplify() {
        int g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        simplify();
    }

    // Conversion to double
    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

    // Explicit conversion to int (C++11)
    explicit operator int() const {
        return numerator / denominator;
    }

    // Conversion to string
    operator std::string() const {
        return std::to_string(numerator) + "/" + std::to_string(denominator);
    }

    // Conversion to bool (for checking if non-zero)
    explicit operator bool() const {
        return numerator != 0;
    }

    void display() const {
        std::cout << numerator << "/" << denominator;
    }
};

class Temperature {
private:
    double celsius;

public:
    Temperature(double c = 0) : celsius(c) {}

    // Conversion to Fahrenheit
    operator double() const {
        return (celsius * 9.0 / 5.0) + 32.0;
    }

    double getCelsius() const {
        return celsius;
    }
};

class Distance {
private:
    double meters;

public:
    Distance(double m = 0) : meters(m) {}

    // Conversion to feet
    operator double() const {
        return meters * 3.28084;
    }

    // Explicit conversion to int (whole meters)
    explicit operator int() const {
        return static_cast<int>(meters);
    }

    double getMeters() const {
        return meters;
    }
};

int main() {
    std::cout << "=== Type Conversion Operators Demo ===" << std::endl << std::endl;

    // Fraction conversions
    std::cout << "1. Fraction Conversions:" << std::endl;
    Fraction f1(3, 4);

    std::cout << "Fraction: ";
    f1.display();
    std::cout << std::endl;

    // Implicit conversion to double
    double d = f1;
    std::cout << "As double: " << d << std::endl;

    // Explicit conversion to int
    int i = static_cast<int>(f1);
    std::cout << "As int (explicit): " << i << std::endl;

    // Conversion to string
    std::string s = static_cast<std::string>(f1);
    std::cout << "As string: " << s << std::endl;

    // Conversion to bool
    Fraction f2(0, 1);
    if (f1) {
        std::cout << "f1 is non-zero (true)" << std::endl;
    }
    if (!f2) {
        std::cout << "f2 is zero (false)" << std::endl;
    }

    // Temperature conversion
    std::cout << "\n2. Temperature Conversion:" << std::endl;
    Temperature temp(100);
    std::cout << "Temperature: " << temp.getCelsius() << "°C" << std::endl;
    double fahrenheit = temp;  // Implicit conversion
    std::cout << "In Fahrenheit: " << fahrenheit << "°F" << std::endl;

    // Distance conversion
    std::cout << "\n3. Distance Conversion:" << std::endl;
    Distance dist(10);
    std::cout << "Distance: " << dist.getMeters() << " meters" << std::endl;
    double feet = dist;  // Implicit conversion
    std::cout << "In feet: " << feet << " ft" << std::endl;
    int wholemeters = static_cast<int>(dist);  // Explicit conversion
    std::cout << "Whole meters: " << wholemeters << std::endl;

    // Using conversions in expressions
    std::cout << "\n4. Using Conversions in Expressions:" << std::endl;
    Fraction f3(1, 2);
    Fraction f4(1, 3);

    std::cout << "f3 = "; f3.display(); std::cout << " = " << static_cast<double>(f3) << std::endl;
    std::cout << "f4 = "; f4.display(); std::cout << " = " << static_cast<double>(f4) << std::endl;

    double sum = static_cast<double>(f3) + static_cast<double>(f4);
    std::cout << "Sum as double: " << sum << std::endl;

    return 0;
}
