/*
 * Lesson 11: Operator Overloading - Unary Operators
 *
 * This example demonstrates overloading unary operators
 * (+, -, !, ~) for custom classes.
 *
 * Compilation: g++ -std=c++17 11_unary_operators.cpp -o unary
 * Execution: ./unary
 */

#include <iostream>
#include <cmath>

class Vector3D {
private:
    double x, y, z;

public:
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Unary plus (returns copy)
    Vector3D operator+() const {
        return *this;
    }

    // Unary minus (negation)
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }

    // Logical NOT (returns true if zero vector)
    bool operator!() const {
        return x == 0 && y == 0 && z == 0;
    }

    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class BitFlags {
private:
    unsigned int flags;

public:
    BitFlags(unsigned int f = 0) : flags(f) {}

    // Bitwise NOT
    BitFlags operator~() const {
        return BitFlags(~flags);
    }

    // Logical NOT
    bool operator!() const {
        return flags == 0;
    }

    unsigned int getFlags() const {
        return flags;
    }

    void display() const {
        std::cout << "0x" << std::hex << flags << std::dec;
    }
};

class Signal {
private:
    double amplitude;
    bool inverted;

public:
    Signal(double amp = 1.0, bool inv = false)
        : amplitude(amp), inverted(inv) {}

    // Unary minus - invert signal
    Signal operator-() const {
        return Signal(amplitude, !inverted);
    }

    // Unary plus - ensure positive
    Signal operator+() const {
        return Signal(std::abs(amplitude), false);
    }

    void display() const {
        std::cout << (inverted ? "-" : "+") << amplitude;
    }
};

class Counter {
private:
    int value;

public:
    Counter(int v = 0) : value(v) {}

    // Address-of operator (rarely overloaded, shown for completeness)
    Counter* operator&() {
        std::cout << "[Address-of operator called] ";
        return this;
    }

    int getValue() const {
        return value;
    }

    void display() const {
        std::cout << "Counter: " << value;
    }
};

int main() {
    std::cout << "=== Unary Operators Demo ===" << std::endl << std::endl;

    // Vector3D unary operators
    std::cout << "1. Vector3D Unary Operators:" << std::endl;
    Vector3D v1(3, 4, 5);

    std::cout << "v1 = "; v1.display();
    std::cout << " (magnitude: " << v1.magnitude() << ")" << std::endl;

    Vector3D v2 = -v1;
    std::cout << "-v1 = "; v2.display();
    std::cout << " (magnitude: " << v2.magnitude() << ")" << std::endl;

    Vector3D v3 = +v1;
    std::cout << "+v1 = "; v3.display();
    std::cout << " (magnitude: " << v3.magnitude() << ")" << std::endl;

    // Logical NOT
    Vector3D zero(0, 0, 0);
    std::cout << "\nLogical NOT:" << std::endl;
    std::cout << "!v1 (non-zero) = " << (!v1 ? "true" : "false") << std::endl;
    std::cout << "!zero = " << (!zero ? "true" : "false") << std::endl;

    // BitFlags
    std::cout << "\n2. BitFlags Operators:" << std::endl;
    BitFlags flags1(0xABCD);

    std::cout << "flags1 = "; flags1.display(); std::cout << std::endl;

    BitFlags flags2 = ~flags1;
    std::cout << "~flags1 = "; flags2.display(); std::cout << std::endl;

    BitFlags flags3(0);
    std::cout << "flags3 = "; flags3.display();
    std::cout << " (!flags3 = " << (!flags3 ? "true" : "false") << ")" << std::endl;

    // Signal
    std::cout << "\n3. Signal Operators:" << std::endl;
    Signal sig1(5.0, false);

    std::cout << "sig1 = "; sig1.display(); std::cout << std::endl;

    Signal sig2 = -sig1;
    std::cout << "-sig1 = "; sig2.display(); std::cout << std::endl;

    Signal sig3 = --sig1;  // Double negation
    std::cout << "--sig1 = "; sig3.display(); std::cout << std::endl;

    Signal sig4(5.0, true);
    Signal sig5 = +sig4;
    std::cout << "sig4 = "; sig4.display(); std::cout << std::endl;
    std::cout << "+sig4 = "; sig5.display(); std::cout << std::endl;

    // Address-of operator
    std::cout << "\n4. Address-of Operator:" << std::endl;
    Counter c1(42);
    c1.display(); std::cout << std::endl;

    Counter* ptr = &c1;  // Calls overloaded operator&
    std::cout << "Pointer value: " << ptr->getValue() << std::endl;

    // Combining unary operators
    std::cout << "\n5. Combining Unary Operators:" << std::endl;
    Vector3D v4(1, 2, 3);
    Vector3D v5 = -(-v4);  // Double negation
    std::cout << "v4 = "; v4.display(); std::cout << std::endl;
    std::cout << "-(-v4) = "; v5.display(); std::cout << std::endl;

    return 0;
}
