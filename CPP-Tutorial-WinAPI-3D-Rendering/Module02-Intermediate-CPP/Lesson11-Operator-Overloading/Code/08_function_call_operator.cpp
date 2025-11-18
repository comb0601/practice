/*
 * Lesson 11: Operator Overloading
 * File: 08_function_call_operator.cpp
 *
 * Demonstrates function call operator () overloading (functors).
 * Shows various uses including predicates, callbacks, and stateful functions.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Simple multiplier functor
class Multiplier {
private:
    int factor;

public:
    // Constructor
    Multiplier(int f) : factor(f) {
        std::cout << "Multiplier created with factor " << factor << "\n";
    }

    // Function call operator
    int operator()(int x) const {
        return x * factor;
    }

    // Overload with two parameters
    int operator()(int x, int y) const {
        return (x + y) * factor;
    }
};

// Stateful functor that counts calls
class CallCounter {
private:
    mutable int callCount;  // mutable allows modification in const methods
    std::string name;

public:
    CallCounter(const std::string& n) : callCount(0), name(n) {}

    void operator()() const {
        callCount++;
        std::cout << name << " called " << callCount << " time(s)\n";
    }

    int getCallCount() const { return callCount; }
};

// Predicate functor for filtering
class IsEven {
public:
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

class InRange {
private:
    int min, max;

public:
    InRange(int minVal, int maxVal) : min(minVal), max(maxVal) {}

    bool operator()(int x) const {
        return x >= min && x <= max;
    }
};

// Comparator functor for custom sorting
class CompareByLength {
public:
    bool operator()(const std::string& a, const std::string& b) const {
        return a.length() < b.length();
    }
};

class CompareAbsolute {
public:
    bool operator()(int a, int b) const {
        return abs(a) < abs(b);
    }
};

// Accumulator functor
class Accumulator {
private:
    int sum;

public:
    Accumulator() : sum(0) {}

    void operator()(int x) {
        sum += x;
    }

    int getSum() const { return sum; }
};

// Function object for mathematical operations
class Polynomial {
private:
    std::vector<double> coefficients;  // a0 + a1*x + a2*x^2 + ...

public:
    Polynomial(const std::vector<double>& coef) : coefficients(coef) {}

    // Evaluate polynomial at x
    double operator()(double x) const {
        double result = 0;
        double powerOfX = 1;
        for (double coef : coefficients) {
            result += coef * powerOfX;
            powerOfX *= x;
        }
        return result;
    }

    void display() const {
        std::cout << "P(x) = " << coefficients[0];
        for (size_t i = 1; i < coefficients.size(); i++) {
            std::cout << " + " << coefficients[i] << "*x^" << i;
        }
        std::cout << "\n";
    }
};

// Generator functor
class SequenceGenerator {
private:
    int current;
    int step;

public:
    SequenceGenerator(int start = 0, int step = 1)
        : current(start), step(step) {}

    int operator()() {
        int result = current;
        current += step;
        return result;
    }
};

// Matrix accessor functor
class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    Matrix(size_t r, size_t c, int initVal = 0) : rows(r), cols(c) {
        data.resize(rows, std::vector<int>(cols, initVal));
    }

    // Function call operator for element access
    int& operator()(size_t row, size_t col) {
        return data[row][col];
    }

    const int& operator()(size_t row, size_t col) const {
        return data[row][col];
    }

    void display() const {
        for (size_t i = 0; i < rows; i++) {
            std::cout << "[";
            for (size_t j = 0; j < cols; j++) {
                std::cout << data[i][j];
                if (j < cols - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }
};

int main() {
    std::cout << "=== Function Call Operator Demo ===\n\n";

    // Multiplier demo
    std::cout << "--- Multiplier Functor ---\n";
    Multiplier times3(3);
    std::cout << "times3(10) = " << times3(10) << "\n";
    std::cout << "times3(5, 7) = " << times3(5, 7) << "\n\n";

    Multiplier times5(5);
    std::cout << "times5(8) = " << times5(8) << "\n\n";

    // Call counter demo
    std::cout << "--- Call Counter (Stateful Functor) ---\n";
    CallCounter counter("MyFunction");
    counter();
    counter();
    counter();
    std::cout << "Total calls: " << counter.getCallCount() << "\n\n";

    // Predicate functors with STL algorithms
    std::cout << "--- Predicate Functors with STL ---\n";
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Original: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    // Count even numbers
    int evenCount = std::count_if(numbers.begin(), numbers.end(), IsEven());
    std::cout << "Even numbers: " << evenCount << "\n";

    // Filter numbers in range [3, 7]
    int inRangeCount = std::count_if(numbers.begin(), numbers.end(), InRange(3, 7));
    std::cout << "Numbers in range [3, 7]: " << inRangeCount << "\n\n";

    // Comparator functors for sorting
    std::cout << "--- Comparator Functors ---\n";
    std::vector<std::string> words = {"apple", "banana", "fig", "elderberry", "date"};
    std::cout << "Original words: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";

    std::sort(words.begin(), words.end(), CompareByLength());
    std::cout << "Sorted by length: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n\n";

    std::vector<int> nums = {-5, 3, -10, 8, -2, 7, -9};
    std::cout << "Original numbers: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    std::sort(nums.begin(), nums.end(), CompareAbsolute());
    std::cout << "Sorted by absolute value: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n\n";

    // Accumulator demo
    std::cout << "--- Accumulator Functor ---\n";
    std::vector<int> values = {1, 2, 3, 4, 5};
    Accumulator acc;
    acc = std::for_each(values.begin(), values.end(), acc);
    std::cout << "Sum of values: " << acc.getSum() << "\n\n";

    // Polynomial evaluation
    std::cout << "--- Polynomial Functor ---\n";
    // P(x) = 2 + 3x + 4x^2
    Polynomial poly({2, 3, 4});
    poly.display();
    std::cout << "P(0) = " << poly(0) << "\n";
    std::cout << "P(1) = " << poly(1) << "\n";
    std::cout << "P(2) = " << poly(2) << "\n";
    std::cout << "P(3) = " << poly(3) << "\n\n";

    // Generator functor
    std::cout << "--- Sequence Generator ---\n";
    SequenceGenerator gen1(0, 1);  // 0, 1, 2, 3, ...
    std::cout << "Sequence starting at 0, step 1:\n";
    for (int i = 0; i < 10; i++) {
        std::cout << gen1() << " ";
    }
    std::cout << "\n\n";

    SequenceGenerator gen2(10, 5);  // 10, 15, 20, 25, ...
    std::cout << "Sequence starting at 10, step 5:\n";
    for (int i = 0; i < 10; i++) {
        std::cout << gen2() << " ";
    }
    std::cout << "\n\n";

    // Using generator with STL
    std::cout << "Using generator with std::generate:\n";
    std::vector<int> generated(10);
    SequenceGenerator gen3(100, 10);
    std::generate(generated.begin(), generated.end(), gen3);
    std::cout << "Generated: ";
    for (int n : generated) std::cout << n << " ";
    std::cout << "\n\n";

    // Matrix functor
    std::cout << "--- Matrix with Function Call Operator ---\n";
    Matrix mat(3, 3);

    // Fill matrix
    int value = 1;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            mat(i, j) = value++;  // Using operator()
        }
    }

    std::cout << "Matrix:\n";
    mat.display();

    std::cout << "\nAccessing elements:\n";
    std::cout << "mat(0, 0) = " << mat(0, 0) << "\n";
    std::cout << "mat(1, 1) = " << mat(1, 1) << "\n";
    std::cout << "mat(2, 2) = " << mat(2, 2) << "\n\n";

    // Transform with functor
    std::cout << "--- Transform with Functors ---\n";
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> output(input.size());

    std::transform(input.begin(), input.end(), output.begin(), Multiplier(10));
    std::cout << "Input:  ";
    for (int n : input) std::cout << n << " ";
    std::cout << "\nOutput (x10): ";
    for (int n : output) std::cout << n << " ";
    std::cout << "\n\n";

    // Functors vs function pointers
    std::cout << "--- Why Functors? ---\n";
    std::cout << "Advantages of functors over function pointers:\n";
    std::cout << "1. Can maintain state (like CallCounter)\n";
    std::cout << "2. Can be inlined by compiler (better performance)\n";
    std::cout << "3. Can have multiple operator() overloads\n";
    std::cout << "4. Can store data (like Multiplier's factor)\n";
    std::cout << "5. Type-safe (compile-time checking)\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Function Call Operator Demo ===
 *
 * --- Multiplier Functor ---
 * Multiplier created with factor 3
 * times3(10) = 30
 * times3(5, 7) = 36
 *
 * Multiplier created with factor 5
 * times5(8) = 40
 *
 * --- Call Counter (Stateful Functor) ---
 * MyFunction called 1 time(s)
 * MyFunction called 2 time(s)
 * MyFunction called 3 time(s)
 * Total calls: 3
 *
 * --- Predicate Functors with STL ---
 * Original: 1 2 3 4 5 6 7 8 9 10
 * Even numbers: 5
 * Numbers in range [3, 7]: 5
 *
 * --- Comparator Functors ---
 * Original words: apple banana fig elderberry date
 * Sorted by length: fig date apple banana elderberry
 *
 * Original numbers: -5 3 -10 8 -2 7 -9
 * Sorted by absolute value: -2 3 -5 7 8 -9 -10
 *
 * --- Accumulator Functor ---
 * Sum of values: 15
 *
 * --- Polynomial Functor ---
 * P(x) = 2 + 3*x^1 + 4*x^2
 * P(0) = 2
 * P(1) = 9
 * P(2) = 24
 * P(3) = 47
 *
 * --- Sequence Generator ---
 * Sequence starting at 0, step 1:
 * 0 1 2 3 4 5 6 7 8 9
 *
 * Sequence starting at 10, step 5:
 * 10 15 20 25 30 35 40 45 50 55
 *
 * Using generator with std::generate:
 * Generated: 100 110 120 130 140 150 160 170 180 190
 *
 * --- Matrix with Function Call Operator ---
 * Matrix:
 * [1, 2, 3]
 * [4, 5, 6]
 * [7, 8, 9]
 *
 * Accessing elements:
 * mat(0, 0) = 1
 * mat(1, 1) = 5
 * mat(2, 2) = 9
 *
 * --- Transform with Functors ---
 * Input:  1 2 3 4 5
 * Output (x10): 10 20 30 40 50
 *
 * --- Why Functors? ---
 * Advantages of functors over function pointers:
 * 1. Can maintain state (like CallCounter)
 * 2. Can be inlined by compiler (better performance)
 * 3. Can have multiple operator() overloads
 * 4. Can store data (like Multiplier's factor)
 * 5. Type-safe (compile-time checking)
 */
