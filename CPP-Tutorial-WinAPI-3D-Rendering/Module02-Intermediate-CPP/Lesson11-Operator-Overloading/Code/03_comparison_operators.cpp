/*
 * Lesson 11: Operator Overloading
 * File: 03_comparison_operators.cpp
 *
 * Demonstrates comparison operator overloading (==, !=, <, >, <=, >=)
 * for a Point class representing 2D coordinates.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class Point {
private:
    int x, y;

public:
    // Constructor
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Equality operator
    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }

    // Inequality operator
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // Less than operator (lexicographic order: first by x, then by y)
    bool operator<(const Point& other) const {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }

    // Greater than operator
    bool operator>(const Point& other) const {
        return other < *this;
    }

    // Less than or equal operator
    bool operator<=(const Point& other) const {
        return !(other < *this);
    }

    // Greater than or equal operator
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }

    // Distance from origin (for sorting by distance)
    double distanceFromOrigin() const {
        return sqrt(x * x + y * y);
    }

    // Stream output operator
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

// Functor for comparing points by distance from origin
class CompareByDistance {
public:
    bool operator()(const Point& p1, const Point& p2) const {
        return p1.distanceFromOrigin() < p2.distanceFromOrigin();
    }
};

int main() {
    std::cout << "=== Comparison Operators Demo ===\n\n";

    // Create points
    Point p1(3, 4);
    Point p2(1, 2);
    Point p3(3, 4);
    Point p4(3, 5);
    Point p5(5, 2);

    std::cout << "Points:\n";
    std::cout << "p1 = " << p1 << "\n";
    std::cout << "p2 = " << p2 << "\n";
    std::cout << "p3 = " << p3 << "\n";
    std::cout << "p4 = " << p4 << "\n";
    std::cout << "p5 = " << p5 << "\n\n";

    // Equality comparison
    std::cout << "--- Equality Comparison ---\n";
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << "\n";
    std::cout << "p1 == p3: " << (p1 == p3 ? "true" : "false") << "\n";
    std::cout << "p1 != p2: " << (p1 != p2 ? "true" : "false") << "\n\n";

    // Relational comparison (lexicographic order)
    std::cout << "--- Relational Comparison (Lexicographic Order) ---\n";
    std::cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << "\n";
    std::cout << "p1 > p2: " << (p1 > p2 ? "true" : "false") << "\n";
    std::cout << "p1 < p4: " << (p1 < p4 ? "true" : "false") << "\n";
    std::cout << "p1 >= p3: " << (p1 >= p3 ? "true" : "false") << "\n";
    std::cout << "p2 <= p5: " << (p2 <= p5 ? "true" : "false") << "\n\n";

    // Sorting with default comparison (lexicographic)
    std::cout << "--- Sorting (Lexicographic Order) ---\n";
    std::vector<Point> points1 = {p1, p2, p3, p4, p5};
    std::cout << "Before sorting: ";
    for (const auto& p : points1) {
        std::cout << p << " ";
    }
    std::cout << "\n";

    std::sort(points1.begin(), points1.end());
    std::cout << "After sorting:  ";
    for (const auto& p : points1) {
        std::cout << p << " ";
    }
    std::cout << "\n\n";

    // Sorting by distance from origin
    std::cout << "--- Sorting (By Distance from Origin) ---\n";
    std::vector<Point> points2 = {p1, p2, p3, p4, p5};
    std::cout << "Distances from origin:\n";
    for (const auto& p : points2) {
        std::cout << p << ": " << p.distanceFromOrigin() << "\n";
    }
    std::cout << "\n";

    std::sort(points2.begin(), points2.end(), CompareByDistance());
    std::cout << "After sorting by distance:\n";
    for (const auto& p : points2) {
        std::cout << p << " (distance: " << p.distanceFromOrigin() << ")\n";
    }
    std::cout << "\n";

    // Using in containers
    std::cout << "--- Using in STL Algorithms ---\n";
    Point target(3, 4);
    auto it = std::find(points1.begin(), points1.end(), target);
    if (it != points1.end()) {
        std::cout << "Found " << target << " in the vector\n";
    }

    int count = std::count(points1.begin(), points1.end(), Point(3, 4));
    std::cout << "Count of (3, 4): " << count << "\n\n";

    // Min and max elements
    std::cout << "--- Min/Max Elements ---\n";
    auto minPoint = std::min_element(points1.begin(), points1.end());
    auto maxPoint = std::max_element(points1.begin(), points1.end());
    std::cout << "Min point (lexicographic): " << *minPoint << "\n";
    std::cout << "Max point (lexicographic): " << *maxPoint << "\n\n";

    auto minDist = std::min_element(points2.begin(), points2.end(), CompareByDistance());
    auto maxDist = std::max_element(points2.begin(), points2.end(), CompareByDistance());
    std::cout << "Closest to origin: " << *minDist
              << " (distance: " << minDist->distanceFromOrigin() << ")\n";
    std::cout << "Farthest from origin: " << *maxDist
              << " (distance: " << maxDist->distanceFromOrigin() << ")\n\n";

    // Binary search (requires sorted container)
    std::cout << "--- Binary Search ---\n";
    Point search(3, 5);
    bool found = std::binary_search(points1.begin(), points1.end(), search);
    std::cout << "Binary search for " << search << ": "
              << (found ? "found" : "not found") << "\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Comparison Operators Demo ===
 *
 * Points:
 * p1 = (3, 4)
 * p2 = (1, 2)
 * p3 = (3, 4)
 * p4 = (3, 5)
 * p5 = (5, 2)
 *
 * --- Equality Comparison ---
 * p1 == p2: false
 * p1 == p3: true
 * p1 != p2: true
 *
 * --- Relational Comparison (Lexicographic Order) ---
 * p1 < p2: false
 * p1 > p2: true
 * p1 < p4: true
 * p1 >= p3: true
 * p2 <= p5: true
 *
 * --- Sorting (Lexicographic Order) ---
 * Before sorting: (3, 4) (1, 2) (3, 4) (3, 5) (5, 2)
 * After sorting:  (1, 2) (3, 4) (3, 4) (3, 5) (5, 2)
 *
 * --- Sorting (By Distance from Origin) ---
 * Distances from origin:
 * (3, 4): 5
 * (1, 2): 2.23607
 * (3, 4): 5
 * (3, 5): 5.83095
 * (5, 2): 5.38516
 *
 * After sorting by distance:
 * (1, 2) (distance: 2.23607)
 * (3, 4) (distance: 5)
 * (3, 4) (distance: 5)
 * (5, 2) (distance: 5.38516)
 * (3, 5) (distance: 5.83095)
 *
 * --- Using in STL Algorithms ---
 * Found (3, 4) in the vector
 * Count of (3, 4): 2
 *
 * --- Min/Max Elements ---
 * Min point (lexicographic): (1, 2)
 * Max point (lexicographic): (5, 2)
 *
 * Closest to origin: (1, 2) (distance: 2.23607)
 * Farthest from origin: (3, 5) (distance: 5.83095)
 *
 * --- Binary Search ---
 * Binary search for (3, 5): found
 */
