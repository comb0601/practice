/**
 * Variadic Templates
 * Demonstrates templates with variable number of arguments
 */

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Base case: no arguments
void print() {
    cout << endl;
}

// Recursive case: print first, then rest
template <typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // Recursive call
}

// Sum function - base case
int sum() {
    return 0;
}

// Sum function - recursive case
template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// Count arguments using sizeof...
template <typename... Args>
size_t count(Args... args) {
    return sizeof...(args);
}

// Get maximum value
template <typename T>
T maximum(T value) {
    return value;
}

template <typename T, typename... Args>
T maximum(T first, Args... rest) {
    T max_rest = maximum(rest...);
    return (first > max_rest) ? first : max_rest;
}

// Get minimum value
template <typename T>
T minimum(T value) {
    return value;
}

template <typename T, typename... Args>
T minimum(T first, Args... rest) {
    T min_rest = minimum(rest...);
    return (first < min_rest) ? first : min_rest;
}

// All same type checker
template <typename T>
bool allEqual(T value) {
    return true;
}

template <typename T, typename... Args>
bool allEqual(T first, T second, Args... rest) {
    return (first == second) && allEqual(second, rest...);
}

// String concatenation
string concat() {
    return "";
}

template <typename T, typename... Args>
string concat(T first, Args... rest) {
    ostringstream oss;
    oss << first << concat(rest...);
    return oss.str();
}

// Average calculation
template <typename... Args>
double average(Args... args) {
    return static_cast<double>(sum(args...)) / sizeof...(args);
}

// Contains value
template <typename T>
bool contains(T value, T target) {
    return value == target;
}

template <typename T, typename... Args>
bool contains(T target, T first, Args... rest) {
    return (first == target) || contains(target, rest...);
}

int main() {
    cout << "=== Variadic Templates ===" << endl << endl;

    // Print with different types
    cout << "Print function:" << endl;
    print(1, 2, 3, 4, 5);
    print("Hello", "World", "from", "C++");
    print(1, 2.5, "mixed", 'X', true);
    cout << endl;

    // Sum
    cout << "Sum function:" << endl;
    cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << endl;
    cout << "sum(10, 20, 30) = " << sum(10, 20, 30) << endl;
    cout << "sum(1.5, 2.5, 3.5, 4.5) = " << sum(1.5, 2.5, 3.5, 4.5) << endl;
    cout << endl;

    // Count arguments
    cout << "Count function:" << endl;
    cout << "count(1, 2, 3) = " << count(1, 2, 3) << endl;
    cout << "count('a', 'b', 'c', 'd', 'e') = " << count('a', 'b', 'c', 'd', 'e') << endl;
    cout << "count(\"one\", \"two\") = " << count("one", "two") << endl;
    cout << endl;

    // Maximum
    cout << "Maximum function:" << endl;
    cout << "max(5, 3, 9, 1, 7) = " << maximum(5, 3, 9, 1, 7) << endl;
    cout << "max(3.14, 2.71, 1.41, 5.67) = " << maximum(3.14, 2.71, 1.41, 5.67) << endl;
    cout << endl;

    // Minimum
    cout << "Minimum function:" << endl;
    cout << "min(5, 3, 9, 1, 7) = " << minimum(5, 3, 9, 1, 7) << endl;
    cout << "min(3.14, 2.71, 1.41, 5.67) = " << minimum(3.14, 2.71, 1.41, 5.67) << endl;
    cout << endl;

    // All equal
    cout << "All equal checker:" << endl;
    cout << "allEqual(5, 5, 5, 5) = " << (allEqual(5, 5, 5, 5) ? "true" : "false") << endl;
    cout << "allEqual(5, 5, 3, 5) = " << (allEqual(5, 5, 3, 5) ? "true" : "false") << endl;
    cout << endl;

    // String concatenation
    cout << "Concatenation:" << endl;
    cout << "concat(\"Hello\", \" \", \"World\", \"!\") = "
         << concat("Hello", " ", "World", "!") << endl;
    cout << "concat(1, 2, 3, 4, 5) = " << concat(1, 2, 3, 4, 5) << endl;
    cout << endl;

    // Average
    cout << "Average function:" << endl;
    cout << "average(1, 2, 3, 4, 5) = " << average(1, 2, 3, 4, 5) << endl;
    cout << "average(10, 20, 30, 40) = " << average(10, 20, 30, 40) << endl;
    cout << "average(1.5, 2.5, 3.5) = " << average(1.5, 2.5, 3.5) << endl;
    cout << endl;

    // Contains
    cout << "Contains function:" << endl;
    cout << "contains(5, 1, 2, 3, 4, 5) = "
         << (contains(5, 1, 2, 3, 4, 5) ? "true" : "false") << endl;
    cout << "contains(10, 1, 2, 3, 4, 5) = "
         << (contains(10, 1, 2, 3, 4, 5) ? "true" : "false") << endl;

    return 0;
}
