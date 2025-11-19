/*
 * Lesson 11: Operator Overloading - Comma Operator
 *
 * This example demonstrates overloading the comma operator (,)
 * though it's rarely used in practice.
 *
 * Compilation: g++ -std=c++17 12_comma_operator.cpp -o comma
 * Execution: ./comma
 */

#include <iostream>
#include <vector>

// Example 1: Building a list
class ListBuilder {
private:
    std::vector<int> values;

public:
    ListBuilder(int value) {
        values.push_back(value);
    }

    // Overload comma operator to add more values
    ListBuilder& operator,(int value) {
        values.push_back(value);
        return *this;
    }

    const std::vector<int>& getValues() const {
        return values;
    }

    void display() const {
        std::cout << "[";
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i];
            if (i < values.size() - 1) std::cout << ", ";
        }
        std::cout << "]";
    }
};

// Example 2: Sequential operations logger
class Logger {
private:
    std::vector<std::string> messages;

public:
    Logger(const std::string& msg) {
        messages.push_back(msg);
        std::cout << "[LOG] " << msg << std::endl;
    }

    Logger& operator,(const std::string& msg) {
        messages.push_back(msg);
        std::cout << "[LOG] " << msg << std::endl;
        return *this;
    }

    void showAll() const {
        std::cout << "\n=== All Logged Messages ===" << std::endl;
        for (size_t i = 0; i < messages.size(); ++i) {
            std::cout << i + 1 << ". " << messages[i] << std::endl;
        }
    }
};

// Example 3: Point initialization helper
class Point {
private:
    double x, y, z;
    int initialized;  // Track which components are set

public:
    Point() : x(0), y(0), z(0), initialized(0) {}

    Point& setX(double val) {
        x = val;
        initialized |= 1;
        return *this;
    }

    Point& setY(double val) {
        y = val;
        initialized |= 2;
        return *this;
    }

    Point& setZ(double val) {
        z = val;
        initialized |= 4;
        return *this;
    }

    // Comma operator for chaining
    Point& operator,(Point& (*func)(Point&)) {
        return func(*this);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }

    bool isFullyInitialized() const {
        return initialized == 7;  // All three bits set
    }
};

// Example 4: Expression evaluator
class Expression {
private:
    double result;

public:
    Expression(double value) : result(value) {}

    Expression& operator,(double value) {
        result = value;  // Comma operator evaluates and discards left, keeps right
        return *this;
    }

    double getResult() const {
        return result;
    }
};

int main() {
    std::cout << "=== Comma Operator Demo ===" << std::endl << std::endl;

    // Example 1: List builder
    std::cout << "1. ListBuilder with Comma Operator:" << std::endl;
    ListBuilder list = (ListBuilder(1), 2, 3, 4, 5);
    std::cout << "Built list: ";
    list.display();
    std::cout << std::endl;

    ListBuilder list2 = (ListBuilder(10), 20, 30);
    std::cout << "Another list: ";
    list2.display();
    std::cout << std::endl;

    // Example 2: Sequential logger
    std::cout << "\n2. Sequential Logger:" << std::endl;
    Logger log = (Logger("Starting application"),
                  "Initializing components",
                  "Loading configuration",
                  "Application ready");

    log.showAll();

    // Example 3: Standard comma operator behavior
    std::cout << "\n3. Standard Comma Operator (for comparison):" << std::endl;
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++);  // Evaluates all, returns last
    std::cout << "After (a++, b++, c++): " << std::endl;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    std::cout << "result = " << result << std::endl;

    // Example 4: Expression evaluator
    std::cout << "\n4. Expression Evaluator:" << std::endl;
    Expression expr = (Expression(1.0), 2.0, 3.0, 4.0);
    std::cout << "Expression result: " << expr.getResult() << std::endl;
    std::cout << "(Comma evaluates left to right, keeps rightmost)" << std::endl;

    // Example 5: Multiple operations in one line
    std::cout << "\n5. Multiple List Operations:" << std::endl;
    ListBuilder combined = (ListBuilder(100), 200, 300, 400);
    std::cout << "Combined: ";
    combined.display();
    std::cout << std::endl;

    // Warning example
    std::cout << "\n6. Important Note:" << std::endl;
    std::cout << "Overloading the comma operator is rarely recommended!" << std::endl;
    std::cout << "It can make code confusing and break expected C++ semantics." << std::endl;
    std::cout << "Consider using named methods instead for clarity." << std::endl;

    return 0;
}
