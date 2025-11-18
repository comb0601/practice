/*
 * Lesson 39: Operator Overloading Part 2 - More Operators
 * Compile: cl Lesson39_OperatorOverload2.cpp
 */

#include <iostream>
using namespace std;

class Counter {
private:
    int count;

public:
    Counter(int c = 0) : count(c) {}

    // Prefix ++
    Counter& operator++() {
        ++count;
        return *this;
    }

    // Postfix ++
    Counter operator++(int) {
        Counter temp = *this;
        count++;
        return temp;
    }

    // Prefix --
    Counter& operator--() {
        --count;
        return *this;
    }

    // Postfix --
    Counter operator--(int) {
        Counter temp = *this;
        count--;
        return temp;
    }

    int getCount() const { return count; }
};

class Point {
private:
    int x, y;

public:
    Point(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}

    // Stream insertion operator
    friend ostream& operator<<(ostream& os, const Point& p);

    // Relational operators
    bool operator<(const Point& p) const {
        return (x < p.x) || (x == p.x && y < p.y);
    }

    bool operator>(const Point& p) const {
        return p < *this;
    }

    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    // Subscript operator
    int operator[](int index) const {
        return (index == 0) ? x : y;
    }
};

ostream& operator<<(ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

int main() {
    cout << "=== Operator Overloading Part 2 ===" << endl << endl;

    // INCREMENT/DECREMENT
    cout << "=== INCREMENT/DECREMENT ===" << endl;
    Counter c(5);
    cout << "Initial: " << c.getCount() << endl;

    ++c;
    cout << "After ++c: " << c.getCount() << endl;

    c++;
    cout << "After c++: " << c.getCount() << endl;

    --c;
    cout << "After --c: " << c.getCount() << endl;

    Counter d = c++;
    cout << "c after c++: " << c.getCount() << endl;
    cout << "d = c++ (old value): " << d.getCount() << endl << endl;

    // STREAM INSERTION
    cout << "=== STREAM INSERTION ===" << endl;
    Point p1(3, 4);
    cout << "Point: " << p1 << endl << endl;

    // RELATIONAL OPERATORS
    cout << "=== RELATIONAL OPERATORS ===" << endl;
    Point p2(5, 6);
    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;
    cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << endl;
    cout << "p1 > p2: " << (p1 > p2 ? "true" : "false") << endl;
    cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << endl << endl;

    // SUBSCRIPT OPERATOR
    cout << "=== SUBSCRIPT OPERATOR ===" << endl;
    cout << "p1[0] (x): " << p1[0] << endl;
    cout << "p1[1] (y): " << p1[1] << endl;

    return 0;
}
