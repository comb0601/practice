/*
 * Lesson 11: Operator Overloading - Best Practices and Guidelines
 *
 * This example demonstrates best practices, common pitfalls,
 * and guidelines for operator overloading.
 *
 * Compilation: g++ -std=c++17 15_best_practices.cpp -o best_practices
 * Execution: ./best_practices
 */

#include <iostream>
#include <string>
#include <stdexcept>

// BEST PRACTICE 1: Maintain semantic consistency
// Operators should behave as users naturally expect

class Money {
private:
    long cents;  // Store as cents to avoid floating-point issues

public:
    Money(double dollars = 0) : cents(static_cast<long>(dollars * 100 + 0.5)) {}

    // Good: + for addition makes sense
    Money operator+(const Money& other) const {
        Money result;
        result.cents = cents + other.cents;
        return result;
    }

    // Good: Comparison makes sense
    bool operator<(const Money& other) const {
        return cents < other.cents;
    }

    // Good: Output operator for display
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        os << "$" << (m.cents / 100) << "."
           << (m.cents % 100 < 10 ? "0" : "") << (m.cents % 100);
        return os;
    }

    // BAD EXAMPLE (commented out): Don't use operators in confusing ways
    // Money operator*(const Money& other) const {
    //     // Multiplying dollars by dollars doesn't make semantic sense!
    //     // Use a named function instead
    // }
};

// BEST PRACTICE 2: Return types should match semantics
class Counter {
private:
    int value;

public:
    Counter(int v = 0) : value(v) {}

    // Good: Prefix operators return reference (for efficiency and chaining)
    Counter& operator++() {
        ++value;
        return *this;
    }

    // Good: Postfix operators return value (to preserve pre-increment value)
    Counter operator++(int) {
        Counter temp = *this;
        ++value;
        return temp;
    }

    // Good: Arithmetic operators return by value (new object)
    Counter operator+(const Counter& other) const {
        return Counter(value + other.value);
    }

    // Good: Compound assignment returns reference
    Counter& operator+=(const Counter& other) {
        value += other.value;
        return *this;
    }

    int getValue() const { return value; }
};

// BEST PRACTICE 3: Implement related operators consistently
class Rational {
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
    }

public:
    Rational(int num = 0, int den = 1) : numerator(num), denominator(den) {
        if (den == 0) throw std::invalid_argument("Denominator cannot be zero");
        simplify();
    }

    // Implement == first
    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    // Implement != using ==
    bool operator!=(const Rational& other) const {
        return !(*this == other);
    }

    // Implement < first
    bool operator<(const Rational& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    // Implement other comparisons using < and ==
    bool operator<=(const Rational& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Rational& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Rational& other) const {
        return !(*this < other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        os << r.numerator << "/" << r.denominator;
        return os;
    }
};

// BEST PRACTICE 4: Use member vs non-member appropriately
class String {
private:
    std::string data;

public:
    String(const std::string& s = "") : data(s) {}

    // Member operator (has implicit access to *this)
    String& operator+=(const String& other) {
        data += other.data;
        return *this;
    }

    // Non-member operator (more symmetric, allows implicit conversions on both sides)
    friend String operator+(const String& lhs, const String& rhs) {
        String result = lhs;
        result += rhs;
        return result;
    }

    // Stream operators should always be non-member friends
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        os << s.data;
        return os;
    }

    const std::string& str() const { return data; }
};

// BEST PRACTICE 5: Const-correctness
class Point3D {
private:
    double x, y, z;

public:
    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Arithmetic operators should be const (don't modify operands)
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    // Comparison operators should be const
    bool operator==(const Point3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Compound assignment modifies, so not const
    Point3D& operator+=(const Point3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

// BEST PRACTICE 6: Avoid common pitfalls
class SafeArray {
private:
    int* data;
    size_t size;

public:
    SafeArray(size_t s) : size(s) {
        data = new int[size]();
    }

    ~SafeArray() {
        delete[] data;
    }

    // IMPORTANT: Implement Rule of Three (or Five in C++11)
    // Copy constructor
    SafeArray(const SafeArray& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator with self-assignment check
    SafeArray& operator=(const SafeArray& other) {
        if (this != &other) {  // Critical: check for self-assignment
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Subscript operator with bounds checking
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
};

int main() {
    std::cout << "=== Operator Overloading Best Practices ===" << std::endl << std::endl;

    // Practice 1: Semantic consistency
    std::cout << "1. Semantic Consistency:" << std::endl;
    Money m1(10.50);
    Money m2(5.25);
    Money total = m1 + m2;
    std::cout << m1 << " + " << m2 << " = " << total << std::endl;

    // Practice 2: Appropriate return types
    std::cout << "\n2. Return Types:" << std::endl;
    Counter c1(5);
    Counter c2 = ++c1;  // Returns reference
    Counter c3 = c1++;  // Returns value
    std::cout << "After ++c1 and c1++: " << c1.getValue() << std::endl;

    // Practice 3: Consistent related operators
    std::cout << "\n3. Related Operators:" << std::endl;
    Rational r1(1, 2);
    Rational r2(1, 3);
    std::cout << r1 << " < " << r2 << ": " << (r1 < r2 ? "true" : "false") << std::endl;
    std::cout << r1 << " > " << r2 << ": " << (r1 > r2 ? "true" : "false") << std::endl;

    // Practice 4: Member vs non-member
    std::cout << "\n4. Member vs Non-member:" << std::endl;
    String s1("Hello");
    String s2(" World");
    String s3 = s1 + s2;  // Works with non-member operator
    std::cout << "Result: " << s3 << std::endl;

    // Practice 5: Const-correctness
    std::cout << "\n5. Const-correctness:" << std::endl;
    const Point3D p1(1, 2, 3);
    const Point3D p2(4, 5, 6);
    Point3D p3 = p1 + p2;  // Can call const operators on const objects
    std::cout << "Sum: "; p3.display(); std::cout << std::endl;

    // Practice 6: Safe implementation
    std::cout << "\n6. Safe Implementation:" << std::endl;
    SafeArray arr1(5);
    arr1[0] = 10;
    SafeArray arr2 = arr1;  // Uses copy constructor
    arr2 = arr1;  // Uses assignment operator (with self-assignment check)
    std::cout << "Safe copy and assignment completed" << std::endl;

    std::cout << "\n=== Summary of Best Practices ===" << std::endl;
    std::cout << "1. Maintain semantic consistency" << std::endl;
    std::cout << "2. Use appropriate return types" << std::endl;
    std::cout << "3. Implement related operators consistently" << std::endl;
    std::cout << "4. Choose member vs non-member appropriately" << std::endl;
    std::cout << "5. Follow const-correctness" << std::endl;
    std::cout << "6. Implement Rule of Three/Five" << std::endl;
    std::cout << "7. Always check for self-assignment" << std::endl;
    std::cout << "8. Provide bounds checking where appropriate" << std::endl;

    return 0;
}
