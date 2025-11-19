/*
 * Lesson 11: Operator Overloading - Increment/Decrement Operators
 *
 * This example demonstrates overloading prefix and postfix
 * increment (++) and decrement (--) operators.
 *
 * Compilation: g++ -std=c++17 04_increment_decrement.cpp -o inc_dec
 * Execution: ./inc_dec
 */

#include <iostream>

class Counter {
private:
    int count;

public:
    Counter(int c = 0) : count(c) {}

    // Prefix increment (++counter)
    Counter& operator++() {
        ++count;
        return *this;
    }

    // Postfix increment (counter++)
    // Note: int parameter is dummy to distinguish from prefix
    Counter operator++(int) {
        Counter temp = *this;
        count++;
        return temp;
    }

    // Prefix decrement (--counter)
    Counter& operator--() {
        --count;
        return *this;
    }

    // Postfix decrement (counter--)
    Counter operator--(int) {
        Counter temp = *this;
        count--;
        return temp;
    }

    int getCount() const {
        return count;
    }

    void display() const {
        std::cout << "Count: " << count;
    }
};

int main() {
    std::cout << "=== Increment/Decrement Operators Demo ===" << std::endl << std::endl;

    Counter c1(10);

    std::cout << "Initial counter: ";
    c1.display();
    std::cout << std::endl << std::endl;

    // Prefix increment
    std::cout << "After ++c1: ";
    ++c1;
    c1.display();
    std::cout << std::endl;

    // Postfix increment
    std::cout << "After c1++: ";
    Counter c2 = c1++;
    c1.display();
    std::cout << " (returned value: " << c2.getCount() << ")" << std::endl;

    // Prefix decrement
    std::cout << "After --c1: ";
    --c1;
    c1.display();
    std::cout << std::endl;

    // Postfix decrement
    std::cout << "After c1--: ";
    Counter c3 = c1--;
    c1.display();
    std::cout << " (returned value: " << c3.getCount() << ")" << std::endl;

    std::cout << std::endl << "Demonstration of prefix vs postfix:" << std::endl;
    Counter c4(5);
    std::cout << "c4 = " << c4.getCount() << std::endl;
    std::cout << "++c4 returns: " << (++c4).getCount() << ", c4 is now: " << c4.getCount() << std::endl;

    Counter c5(5);
    std::cout << "c5 = " << c5.getCount() << std::endl;
    std::cout << "c5++ returns: " << (c5++).getCount() << ", c5 is now: " << c5.getCount() << std::endl;

    return 0;
}
