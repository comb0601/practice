/*
 * Lesson 11: Operator Overloading - Comparison Operators
 *
 * This example demonstrates overloading comparison operators
 * (==, !=, <, >, <=, >=) for a custom Point class.
 *
 * Compilation: g++ -std=c++17 02_comparison_operators.cpp -o comparison_op
 * Execution: ./comparison_op
 */

#include <iostream>
#include <cmath>

class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // Equality operator
    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    // Inequality operator
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // Less than (based on distance from origin)
    bool operator<(const Point& other) const {
        return distanceFromOrigin() < other.distanceFromOrigin();
    }

    // Greater than
    bool operator>(const Point& other) const {
        return other < *this;
    }

    // Less than or equal
    bool operator<=(const Point& other) const {
        return !(*this > other);
    }

    // Greater than or equal
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }

    double distanceFromOrigin() const {
        return std::sqrt(x * x + y * y);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

int main() {
    std::cout << "=== Comparison Operators Demo ===" << std::endl << std::endl;

    Point p1(3, 4);   // Distance: 5
    Point p2(6, 8);   // Distance: 10
    Point p3(3, 4);   // Same as p1

    std::cout << "p1 = "; p1.display();
    std::cout << " [distance: " << p1.distanceFromOrigin() << "]" << std::endl;

    std::cout << "p2 = "; p2.display();
    std::cout << " [distance: " << p2.distanceFromOrigin() << "]" << std::endl;

    std::cout << "p3 = "; p3.display();
    std::cout << " [distance: " << p3.distanceFromOrigin() << "]" << std::endl << std::endl;

    // Equality tests
    std::cout << "p1 == p3: " << (p1 == p3 ? "true" : "false") << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2 ? "true" : "false") << std::endl << std::endl;

    // Comparison tests (based on distance from origin)
    std::cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << std::endl;
    std::cout << "p1 > p2: " << (p1 > p2 ? "true" : "false") << std::endl;
    std::cout << "p1 <= p3: " << (p1 <= p3 ? "true" : "false") << std::endl;
    std::cout << "p1 >= p3: " << (p1 >= p3 ? "true" : "false") << std::endl;

    return 0;
}
