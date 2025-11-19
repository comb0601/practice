/*
 * Lesson 11: Operator Overloading - Compound Assignment Operators
 *
 * This example demonstrates overloading compound assignment operators
 * (+=, -=, *=, /=, etc.) for custom classes.
 *
 * Compilation: g++ -std=c++17 10_compound_assignment.cpp -o compound
 * Execution: ./compound
 */

#include <iostream>
#include <cmath>

class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Compound assignment operators
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D& operator/=(double scalar) {
        if (scalar == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return *this;
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Regular operators implemented using compound assignments
    Vector2D operator+(const Vector2D& other) const {
        Vector2D result = *this;
        result += other;
        return result;
    }

    Vector2D operator-(const Vector2D& other) const {
        Vector2D result = *this;
        result -= other;
        return result;
    }

    Vector2D operator*(double scalar) const {
        Vector2D result = *this;
        result *= scalar;
        return result;
    }

    Vector2D operator/(double scalar) const {
        Vector2D result = *this;
        result /= scalar;
        return result;
    }

    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

class Matrix2x2 {
private:
    double data[2][2];

public:
    Matrix2x2(double a = 0, double b = 0, double c = 0, double d = 0) {
        data[0][0] = a; data[0][1] = b;
        data[1][0] = c; data[1][1] = d;
    }

    Matrix2x2& operator+=(const Matrix2x2& other) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                data[i][j] += other.data[i][j];
            }
        }
        return *this;
    }

    Matrix2x2& operator-=(const Matrix2x2& other) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                data[i][j] -= other.data[i][j];
            }
        }
        return *this;
    }

    Matrix2x2& operator*=(double scalar) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }

    void display() const {
        std::cout << "[ " << data[0][0] << " " << data[0][1] << " ]\n";
        std::cout << "[ " << data[1][0] << " " << data[1][1] << " ]";
    }
};

int main() {
    std::cout << "=== Compound Assignment Operators Demo ===" << std::endl << std::endl;

    // Vector2D examples
    std::cout << "1. Vector2D Compound Assignments:" << std::endl;
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    std::cout << "v1 = "; v1.display(); std::cout << std::endl;
    std::cout << "v2 = "; v2.display(); std::cout << std::endl;

    std::cout << "\nv1 += v2:" << std::endl;
    v1 += v2;
    std::cout << "v1 = "; v1.display(); std::cout << std::endl;

    std::cout << "\nv1 -= v2:" << std::endl;
    v1 -= v2;
    std::cout << "v1 = "; v1.display(); std::cout << std::endl;

    std::cout << "\nv1 *= 2:" << std::endl;
    v1 *= 2;
    std::cout << "v1 = "; v1.display(); std::cout << std::endl;

    std::cout << "\nv1 /= 2:" << std::endl;
    v1 /= 2;
    std::cout << "v1 = "; v1.display(); std::cout << std::endl;

    // Chaining compound assignments
    std::cout << "\n2. Chaining Compound Assignments:" << std::endl;
    Vector2D v3(5, 5);
    Vector2D v4(1, 1);
    Vector2D v5(2, 2);

    std::cout << "v3 = "; v3.display(); std::cout << std::endl;
    std::cout << "v4 = "; v4.display(); std::cout << std::endl;
    std::cout << "v5 = "; v5.display(); std::cout << std::endl;

    std::cout << "\n(v3 += v4) += v5:" << std::endl;
    (v3 += v4) += v5;
    std::cout << "v3 = "; v3.display(); std::cout << std::endl;

    // Matrix examples
    std::cout << "\n3. Matrix2x2 Compound Assignments:" << std::endl;
    Matrix2x2 m1(1, 2, 3, 4);
    Matrix2x2 m2(5, 6, 7, 8);

    std::cout << "m1 =\n"; m1.display(); std::cout << std::endl;
    std::cout << "\nm2 =\n"; m2.display(); std::cout << std::endl;

    std::cout << "\nm1 += m2:" << std::endl;
    m1 += m2;
    m1.display(); std::cout << std::endl;

    std::cout << "\nm1 *= 0.5:" << std::endl;
    m1 *= 0.5;
    m1.display(); std::cout << std::endl;

    // Using regular operators (implemented via compound)
    std::cout << "\n4. Regular Operators (using compound):" << std::endl;
    Vector2D v6(10, 20);
    Vector2D v7(5, 10);

    Vector2D v8 = v6 + v7;
    std::cout << "v6 + v7 = "; v8.display(); std::cout << std::endl;

    Vector2D v9 = v6 - v7;
    std::cout << "v6 - v7 = "; v9.display(); std::cout << std::endl;

    Vector2D v10 = v6 * 2;
    std::cout << "v6 * 2 = "; v10.display(); std::cout << std::endl;

    return 0;
}
