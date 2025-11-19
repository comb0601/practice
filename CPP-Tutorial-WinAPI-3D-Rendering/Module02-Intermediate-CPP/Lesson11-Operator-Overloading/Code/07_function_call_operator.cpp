/*
 * Lesson 11: Operator Overloading - Function Call Operator
 *
 * This example demonstrates overloading the function call operator ()
 * to create functor objects (objects that can be called like functions).
 *
 * Compilation: g++ -std=c++17 07_function_call_operator.cpp -o functor
 * Execution: ./functor
 */

#include <iostream>
#include <vector>
#include <algorithm>

// Simple functor for addition
class Adder {
private:
    int value;

public:
    Adder(int v) : value(v) {}

    // Overload () operator
    int operator()(int x) const {
        return x + value;
    }
};

// Functor for range checking
class InRange {
private:
    int min, max;

public:
    InRange(int mn, int mx) : min(mn), max(mx) {}

    bool operator()(int value) const {
        return value >= min && value <= max;
    }
};

// Functor for mathematical operations
class Calculator {
public:
    // Single parameter - square
    double operator()(double x) const {
        return x * x;
    }

    // Two parameters - power
    double operator()(double base, double exponent) const {
        return std::pow(base, exponent);
    }

    // Three parameters - weighted sum
    double operator()(double a, double b, double c) const {
        return a + 2*b + 3*c;
    }
};

// Comparison functor for sorting
class CompareLength {
public:
    bool operator()(const std::string& a, const std::string& b) const {
        return a.length() < b.length();
    }
};

int main() {
    std::cout << "=== Function Call Operator Demo ===" << std::endl << std::endl;

    // Using Adder functor
    std::cout << "1. Adder Functor:" << std::endl;
    Adder add5(5);
    std::cout << "add5(10) = " << add5(10) << std::endl;
    std::cout << "add5(20) = " << add5(20) << std::endl;

    // Using with algorithms
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "\nOriginal numbers: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    std::transform(numbers.begin(), numbers.end(), numbers.begin(), add5);
    std::cout << "After adding 5: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // Using InRange functor
    std::cout << "\n2. InRange Functor:" << std::endl;
    InRange between10and20(10, 20);
    std::vector<int> values = {5, 12, 18, 25, 15, 3, 20};

    std::cout << "Values in range [10, 20]: ";
    for (int v : values) {
        if (between10and20(v)) {
            std::cout << v << " ";
        }
    }
    std::cout << std::endl;

    // Using Calculator with different parameter counts
    std::cout << "\n3. Calculator Functor (overloaded):" << std::endl;
    Calculator calc;
    std::cout << "calc(5) [square] = " << calc(5) << std::endl;
    std::cout << "calc(2, 3) [power] = " << calc(2, 3) << std::endl;
    std::cout << "calc(1, 2, 3) [weighted sum] = " << calc(1, 2, 3) << std::endl;

    // Using CompareLength for sorting
    std::cout << "\n4. Sorting with Custom Comparator:" << std::endl;
    std::vector<std::string> words = {"elephant", "cat", "dog", "butterfly", "ant"};

    std::cout << "Before sorting: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << std::endl;

    std::sort(words.begin(), words.end(), CompareLength());

    std::cout << "After sorting by length: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << std::endl;

    return 0;
}
