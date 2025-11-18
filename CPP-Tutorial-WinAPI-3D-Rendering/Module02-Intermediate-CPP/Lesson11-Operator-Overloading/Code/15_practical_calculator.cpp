/*
 * Lesson 11: Operator Overloading
 * File: 15_practical_calculator.cpp
 *
 * Comprehensive practical example: Scientific calculator with
 * various numeric types demonstrating operator overloading in action.
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

// Complex number for complex calculations
class Complex {
private:
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    Complex operator/(const Complex& other) const {
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0) throw std::runtime_error("Division by zero");
        return Complex(
            (real * other.real + imag * other.imag) / denom,
            (imag * other.real - real * other.imag) / denom
        );
    }

    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << "+" << c.imag << "i";
        else os << c.imag << "i";
        return os;
    }
};

// Fraction for exact arithmetic
class Fraction {
private:
    int num, denom;

    int gcd(int a, int b) const {
        a = abs(a); b = abs(b);
        while (b) { int t = b; b = a % b; a = t; }
        return a;
    }

    void simplify() {
        int g = gcd(num, denom);
        num /= g;
        denom /= g;
        if (denom < 0) { num = -num; denom = -denom; }
    }

public:
    Fraction(int n = 0, int d = 1) : num(n), denom(d) {
        if (d == 0) throw std::runtime_error("Zero denominator");
        simplify();
    }

    Fraction operator+(const Fraction& other) const {
        return Fraction(num * other.denom + other.num * denom, denom * other.denom);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(num * other.denom - other.num * denom, denom * other.denom);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(num * other.num, denom * other.denom);
    }

    Fraction operator/(const Fraction& other) const {
        if (other.num == 0) throw std::runtime_error("Division by zero");
        return Fraction(num * other.denom, denom * other.num);
    }

    explicit operator double() const {
        return static_cast<double>(num) / denom;
    }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        if (f.denom == 1) os << f.num;
        else os << f.num << "/" << f.denom;
        return os;
    }
};

// Vector for vector calculations
class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    double operator*(const Vector2D& other) const {  // Dot product
        return x * other.x + y * other.y;
    }

    double magnitude() const {
        return sqrt(x * x + y * y);
    }

    Vector2D normalize() const {
        double mag = magnitude();
        if (mag == 0) throw std::runtime_error("Cannot normalize zero vector");
        return *this * (1.0 / mag);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

friend Vector2D operator*(double scalar, const Vector2D& v) {
    return v * scalar;
}

// Matrix2x2 for small matrix operations
class Matrix2x2 {
private:
    double data[2][2];

public:
    Matrix2x2(double a = 0, double b = 0, double c = 0, double d = 0) {
        data[0][0] = a; data[0][1] = b;
        data[1][0] = c; data[1][1] = d;
    }

    double& operator()(int i, int j) {
        if (i < 0 || i >= 2 || j < 0 || j >= 2)
            throw std::out_of_range("Matrix index out of range");
        return data[i][j];
    }

    const double& operator()(int i, int j) const {
        if (i < 0 || i >= 2 || j < 0 || j >= 2)
            throw std::out_of_range("Matrix index out of range");
        return data[i][j];
    }

    Matrix2x2 operator+(const Matrix2x2& other) const {
        return Matrix2x2(
            data[0][0] + other.data[0][0], data[0][1] + other.data[0][1],
            data[1][0] + other.data[1][0], data[1][1] + other.data[1][1]
        );
    }

    Matrix2x2 operator*(const Matrix2x2& other) const {
        return Matrix2x2(
            data[0][0] * other.data[0][0] + data[0][1] * other.data[1][0],
            data[0][0] * other.data[0][1] + data[0][1] * other.data[1][1],
            data[1][0] * other.data[0][0] + data[1][1] * other.data[1][0],
            data[1][0] * other.data[0][1] + data[1][1] * other.data[1][1]
        );
    }

    double determinant() const {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        os << "[" << m.data[0][0] << " " << m.data[0][1] << "]\n";
        os << "[" << m.data[1][0] << " " << m.data[1][1] << "]";
        return os;
    }
};

// Measurement with units
class Distance {
private:
    double meters;

public:
    explicit Distance(double m) : meters(m) {}

    static Distance fromKilometers(double km) { return Distance(km * 1000); }
    static Distance fromMiles(double miles) { return Distance(miles * 1609.34); }
    static Distance fromFeet(double feet) { return Distance(feet * 0.3048); }

    double toMeters() const { return meters; }
    double toKilometers() const { return meters / 1000; }
    double toMiles() const { return meters / 1609.34; }
    double toFeet() const { return meters / 0.3048; }

    Distance operator+(const Distance& other) const {
        return Distance(meters + other.meters);
    }

    Distance operator-(const Distance& other) const {
        return Distance(meters - other.meters);
    }

    Distance operator*(double scalar) const {
        return Distance(meters * scalar);
    }

    Distance operator/(double scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Distance(meters / scalar);
    }

    bool operator<(const Distance& other) const {
        return meters < other.meters;
    }

    bool operator>(const Distance& other) const {
        return meters > other.meters;
    }

    friend std::ostream& operator<<(std::ostream& os, const Distance& d) {
        os << d.meters << "m";
        return os;
    }
};

// Calculator demo function
void demonstrateCalculator() {
    std::cout << "=== SCIENTIFIC CALCULATOR DEMO ===\n\n";

    // Complex number calculations
    std::cout << "--- Complex Number Calculations ---\n";
    Complex c1(3, 4);
    Complex c2(1, 2);
    std::cout << "c1 = " << c1 << "\n";
    std::cout << "c2 = " << c2 << "\n";
    std::cout << "c1 + c2 = " << (c1 + c2) << "\n";
    std::cout << "c1 * c2 = " << (c1 * c2) << "\n";
    std::cout << "|c1| = " << c1.magnitude() << "\n\n";

    // Fraction calculations
    std::cout << "--- Fraction Calculations ---\n";
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    Fraction f3(1, 4);
    std::cout << "f1 = " << f1 << "\n";
    std::cout << "f2 = " << f2 << "\n";
    std::cout << "f3 = " << f3 << "\n";
    std::cout << "f1 + f2 + f3 = " << (f1 + f2 + f3) << "\n";
    std::cout << "f1 * f2 / f3 = " << (f1 * f2 / f3) << "\n";
    std::cout << "As decimal: " << static_cast<double>(f1 + f2) << "\n\n";

    // Vector calculations
    std::cout << "--- Vector Calculations ---\n";
    Vector2D v1(3, 4);
    Vector2D v2(1, 0);
    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";
    std::cout << "v1 + v2 = " << (v1 + v2) << "\n";
    std::cout << "v1 · v2 = " << (v1 * v2) << "\n";
    std::cout << "|v1| = " << v1.magnitude() << "\n";
    std::cout << "v1 normalized = " << v1.normalize() << "\n\n";

    // Matrix calculations
    std::cout << "--- Matrix Calculations ---\n";
    Matrix2x2 m1(1, 2, 3, 4);
    Matrix2x2 m2(5, 6, 7, 8);
    std::cout << "m1 =\n" << m1 << "\n\n";
    std::cout << "m2 =\n" << m2 << "\n\n";
    std::cout << "m1 + m2 =\n" << (m1 + m2) << "\n\n";
    std::cout << "m1 * m2 =\n" << (m1 * m2) << "\n\n";
    std::cout << "det(m1) = " << m1.determinant() << "\n\n";

    // Distance/unit calculations
    std::cout << "--- Distance Calculations ---\n";
    Distance d1 = Distance::fromKilometers(5);
    Distance d2 = Distance::fromMiles(3);
    std::cout << "d1 = 5 km = " << d1 << " = " << d1.toMiles() << " miles\n";
    std::cout << "d2 = 3 miles = " << d2 << " = " << d2.toKilometers() << " km\n";
    std::cout << "d1 + d2 = " << (d1 + d2) << " = "
              << (d1 + d2).toKilometers() << " km\n";
    std::cout << "d1 > d2: " << (d1 > d2 ? "true" : "false") << "\n\n";

    // Real-world problem: Projectile motion
    std::cout << "--- Physics: Projectile Motion ---\n";
    Vector2D initialVelocity(20, 30);  // m/s
    double time = 2.0;  // seconds
    double gravity = 9.8;  // m/s²

    Vector2D gravityVector(0, -gravity * time);
    Vector2D finalVelocity = initialVelocity + gravityVector;

    std::cout << "Initial velocity: " << initialVelocity << " m/s\n";
    std::cout << "After " << time << " seconds:\n";
    std::cout << "Final velocity: " << finalVelocity << " m/s\n";
    std::cout << "Speed: " << finalVelocity.magnitude() << " m/s\n\n";

    // Engineering: Stress calculation
    std::cout << "--- Engineering: Stress Tensor ---\n";
    Matrix2x2 stress(100, 50, 50, 100);  // MPa
    std::cout << "Stress tensor (MPa):\n" << stress << "\n\n";
    std::cout << "Determinant: " << stress.determinant() << " MPa²\n\n";

    // Financial: Compound interest with fractions
    std::cout << "--- Financial: Interest Calculation ---\n";
    Fraction principal(1000, 1);
    Fraction rate(5, 100);  // 5%
    Fraction time(2, 1);    // 2 years
    Fraction interest = principal * rate * time;
    std::cout << "Principal: $" << principal << "\n";
    std::cout << "Rate: " << rate << " (5%)\n";
    std::cout << "Time: " << time << " years\n";
    std::cout << "Simple interest: $" << interest << "\n";
    std::cout << "Total: $" << (principal + interest) << "\n";
    std::cout << "In decimal: $" << static_cast<double>(principal + interest) << "\n\n";

    // Geometry: Triangle area using vectors
    std::cout << "--- Geometry: Triangle Area ---\n";
    Vector2D p1(0, 0);
    Vector2D p2(4, 0);
    Vector2D p3(0, 3);
    Vector2D side1 = p2 - p1;
    Vector2D side2 = p3 - p1;
    // Area = 0.5 * |cross product| = 0.5 * |x1*y2 - x2*y1|
    double area = 0.5 * fabs(side1.getX() * side2.getY() - side1.getY() * side2.getX());
    std::cout << "Triangle vertices: " << p1 << ", " << p2 << ", " << p3 << "\n";
    std::cout << "Area: " << area << " square units\n\n";

    // Signal processing: Complex impedance
    std::cout << "--- Electronics: Complex Impedance ---\n";
    Complex z1(100, 50);   // Resistor + inductor
    Complex z2(100, -50);  // Resistor + capacitor
    Complex zTotal = z1 + z2;
    std::cout << "Z1 (R + jωL) = " << z1 << " Ω\n";
    std::cout << "Z2 (R - j/ωC) = " << z2 << " Ω\n";
    std::cout << "Z_total (series) = " << zTotal << " Ω\n";
    std::cout << "|Z_total| = " << zTotal.magnitude() << " Ω\n";
}

int main() {
    try {
        demonstrateCalculator();

        std::cout << "\n=== CALCULATOR SESSION COMPLETE ===\n";
        std::cout << "This demo showed operator overloading in:\n";
        std::cout << "- Complex number arithmetic\n";
        std::cout << "- Exact fraction calculations\n";
        std::cout << "- Vector operations\n";
        std::cout << "- Matrix algebra\n";
        std::cout << "- Unit conversions\n";
        std::cout << "- Real-world applications\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

/*
 * OUTPUT (partial):
 * =================================
 * === SCIENTIFIC CALCULATOR DEMO ===
 *
 * --- Complex Number Calculations ---
 * c1 = 3+4i
 * c2 = 1+2i
 * c1 + c2 = 4+6i
 * c1 * c2 = -5+10i
 * |c1| = 5
 *
 * --- Fraction Calculations ---
 * f1 = 1/2
 * f2 = 1/3
 * f3 = 1/4
 * f1 + f2 + f3 = 13/12
 * f1 * f2 / f3 = 2/3
 * As decimal: 0.833333
 *
 * --- Vector Calculations ---
 * v1 = (3, 4)
 * v2 = (1, 0)
 * v1 + v2 = (4, 4)
 * v1 · v2 = 3
 * |v1| = 5
 * v1 normalized = (0.6, 0.8)
 *
 * ... (more output follows)
 */
