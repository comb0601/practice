/*
 * Lesson 38: Operator Overloading Part 1 - Basic Operators
 * Compile: cl Lesson38_OperatorOverload1.cpp
 */

#include <iostream>
using namespace std;

class Complex {
private:
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Operator+ overload
    Complex operator+(const Complex& c) const {
        return Complex(real + c.real, imag + c.imag);
    }

    // Operator- overload
    Complex operator-(const Complex& c) const {
        return Complex(real - c.real, imag - c.imag);
    }

    // Operator* overload
    Complex operator*(const Complex& c) const {
        return Complex(real * c.real - imag * c.imag, real * c.imag + imag * c.real);
    }

    // Operator== overload
    bool operator==(const Complex& c) const {
        return (real == c.real && imag == c.imag);
    }

    void display() const {
        cout << real;
        if (imag >= 0) cout << " + " << imag << "i";
        else cout << " - " << (-imag) << "i";
    }
};

class Vector {
private:
    double x, y;

public:
    Vector(double xVal = 0, double yVal = 0) : x(xVal), y(yVal) {}

    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y);
    }

    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y);
    }

    double operator*(const Vector& v) const {  // Dot product
        return x * v.x + y * v.y;
    }

    Vector operator*(double scalar) const {  // Scalar multiplication
        return Vector(x * scalar, y * scalar);
    }

    void display() const {
        cout << "(" << x << ", " << y << ")";
    }
};

int main() {
    cout << "=== Operator Overloading Demo ===" << endl << endl;

    // COMPLEX NUMBERS
    cout << "=== COMPLEX NUMBERS ===" << endl;
    Complex c1(3, 4), c2(1, 2);

    cout << "c1 = "; c1.display(); cout << endl;
    cout << "c2 = "; c2.display(); cout << endl << endl;

    Complex sum = c1 + c2;
    cout << "c1 + c2 = "; sum.display(); cout << endl;

    Complex diff = c1 - c2;
    cout << "c1 - c2 = "; diff.display(); cout << endl;

    Complex prod = c1 * c2;
    cout << "c1 * c2 = "; prod.display(); cout << endl;

    cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << endl << endl;

    // VECTORS
    cout << "=== VECTORS ===" << endl;
    Vector v1(3, 4), v2(1, 2);

    cout << "v1 = "; v1.display(); cout << endl;
    cout << "v2 = "; v2.display(); cout << endl << endl;

    Vector vSum = v1 + v2;
    cout << "v1 + v2 = "; vSum.display(); cout << endl;

    double dotProduct = v1 * v2;
    cout << "v1 · v2 = " << dotProduct << endl;

    Vector scaled = v1 * 2;
    cout << "v1 * 2 = "; scaled.display(); cout << endl;

    return 0;
}
