/**
 * Comprehensive Template Demo
 * Combines multiple template concepts in practical examples
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

// Generic Container wrapper
template <typename T, typename Container = vector<T>>
class GenericContainer {
private:
    Container data;

public:
    void add(const T& value) {
        data.push_back(value);
    }

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    void print() const {
        cout << "[";
        for (size_t i = 0; i < data.size(); i++) {
            cout << data[i];
            if (i < data.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }

    // Apply function to all elements
    template <typename Func>
    void forEach(Func func) {
        for (auto& elem : data) {
            func(elem);
        }
    }

    // Transform all elements
    template <typename Func>
    void transform(Func func) {
        for (auto& elem : data) {
            elem = func(elem);
        }
    }

    // Filter elements
    template <typename Pred>
    GenericContainer filter(Pred predicate) const {
        GenericContainer result;
        for (const auto& elem : data) {
            if (predicate(elem)) {
                result.add(elem);
            }
        }
        return result;
    }

    // Find element
    template <typename Pred>
    int findIf(Pred predicate) const {
        for (size_t i = 0; i < data.size(); i++) {
            if (predicate(data[i])) {
                return i;
            }
        }
        return -1;
    }
};

// Generic statistics calculator
template <typename T>
struct Statistics {
    T min;
    T max;
    T sum;
    double average;
    size_t count;
};

template <typename T>
Statistics<T> calculateStatistics(const vector<T>& data) {
    if (data.empty()) {
        return Statistics<T>{T(), T(), T(), 0.0, 0};
    }

    Statistics<T> stats;
    stats.min = data[0];
    stats.max = data[0];
    stats.sum = T();
    stats.count = data.size();

    for (const T& value : data) {
        if (value < stats.min) stats.min = value;
        if (value > stats.max) stats.max = value;
        stats.sum += value;
    }

    stats.average = static_cast<double>(stats.sum) / stats.count;
    return stats;
}

// Generic pair operations
template <typename T1, typename T2>
class PairOps {
public:
    static void printPair(const pair<T1, T2>& p) {
        cout << "(" << p.first << ", " << p.second << ")";
    }

    static void printVector(const vector<pair<T1, T2>>& vec) {
        cout << "[";
        for (size_t i = 0; i < vec.size(); i++) {
            printPair(vec[i]);
            if (i < vec.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }

    static vector<T1> getFirstElements(const vector<pair<T1, T2>>& vec) {
        vector<T1> result;
        for (const auto& p : vec) {
            result.push_back(p.first);
        }
        return result;
    }

    static vector<T2> getSecondElements(const vector<pair<T1, T2>>& vec) {
        vector<T2> result;
        for (const auto& p : vec) {
            result.push_back(p.second);
        }
        return result;
    }
};

// Generic range checker
template <typename T>
bool inRange(const T& value, const T& min, const T& max) {
    return value >= min && value <= max;
}

// Generic accumulator
template <typename T>
T accumulate(const vector<T>& vec, T initial = T()) {
    T result = initial;
    for (const T& value : vec) {
        result += value;
    }
    return result;
}

// Generic product
template <typename T>
T product(const vector<T>& vec, T initial = T(1)) {
    T result = initial;
    for (const T& value : vec) {
        result *= value;
    }
    return result;
}

// Count if
template <typename T, typename Pred>
size_t countIf(const vector<T>& vec, Pred predicate) {
    size_t count = 0;
    for (const T& value : vec) {
        if (predicate(value)) {
            count++;
        }
    }
    return count;
}

// All of
template <typename T, typename Pred>
bool allOf(const vector<T>& vec, Pred predicate) {
    for (const T& value : vec) {
        if (!predicate(value)) {
            return false;
        }
    }
    return true;
}

// Any of
template <typename T, typename Pred>
bool anyOf(const vector<T>& vec, Pred predicate) {
    for (const T& value : vec) {
        if (predicate(value)) {
            return true;
        }
    }
    return false;
}

int main() {
    cout << "=== Comprehensive Template Demo ===" << endl << endl;

    // Generic Container
    cout << "Generic Container:" << endl;
    GenericContainer<int> numbers;
    numbers.add(5);
    numbers.add(10);
    numbers.add(15);
    numbers.add(20);
    cout << "Original: ";
    numbers.print();

    // Transform: double each value
    numbers.transform([](int x) { return x * 2; });
    cout << "After doubling: ";
    numbers.print();

    // Filter: only even numbers (will be all after doubling)
    auto evens = numbers.filter([](int x) { return x % 2 == 0; });
    cout << "Even numbers: ";
    evens.print();
    cout << endl;

    // Statistics
    cout << "Statistics:" << endl;
    vector<int> data = {23, 45, 12, 67, 34, 89, 56, 78, 90, 21};
    auto stats = calculateStatistics(data);
    cout << "Data: ";
    for (int val : data) cout << val << " ";
    cout << endl;
    cout << "Min: " << stats.min << endl;
    cout << "Max: " << stats.max << endl;
    cout << "Sum: " << stats.sum << endl;
    cout << "Average: " << stats.average << endl;
    cout << "Count: " << stats.count << endl;
    cout << endl;

    // Pair operations
    cout << "Pair Operations:" << endl;
    vector<pair<string, int>> students = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92},
        {"Diana", 88}
    };

    cout << "Students: ";
    PairOps<string, int>::printVector(students);

    auto names = PairOps<string, int>::getFirstElements(students);
    cout << "Names: ";
    for (const auto& name : names) cout << name << " ";
    cout << endl;

    auto scores = PairOps<string, int>::getSecondElements(students);
    cout << "Scores: ";
    for (int score : scores) cout << score << " ";
    cout << endl;
    cout << endl;

    // Range checking
    cout << "Range Checking:" << endl;
    cout << "5 in range [0, 10]: " << (inRange(5, 0, 10) ? "Yes" : "No") << endl;
    cout << "15 in range [0, 10]: " << (inRange(15, 0, 10) ? "Yes" : "No") << endl;
    cout << "3.14 in range [0.0, 5.0]: " << (inRange(3.14, 0.0, 5.0) ? "Yes" : "No") << endl;
    cout << endl;

    // Accumulation
    cout << "Accumulation:" << endl;
    vector<int> nums = {1, 2, 3, 4, 5};
    cout << "Numbers: ";
    for (int n : nums) cout << n << " ";
    cout << endl;
    cout << "Sum: " << accumulate(nums) << endl;
    cout << "Product: " << product(nums) << endl;
    cout << endl;

    // Predicates
    cout << "Predicate Functions:" << endl;
    vector<int> values = {2, 4, 6, 8, 10};
    cout << "Values: ";
    for (int v : values) cout << v << " ";
    cout << endl;

    auto isEven = [](int x) { return x % 2 == 0; };
    auto greaterThan5 = [](int x) { return x > 5; };

    cout << "Count of even numbers: " << countIf(values, isEven) << endl;
    cout << "Count > 5: " << countIf(values, greaterThan5) << endl;
    cout << "All even: " << (allOf(values, isEven) ? "Yes" : "No") << endl;
    cout << "Any > 5: " << (anyOf(values, greaterThan5) ? "Yes" : "No") << endl;
    cout << endl;

    // String container
    cout << "String Container:" << endl;
    GenericContainer<string> words;
    words.add("hello");
    words.add("world");
    words.add("template");
    words.add("programming");
    cout << "Words: ";
    words.print();

    // Find word with length > 5
    int idx = words.findIf([](const string& s) { return s.length() > 5; });
    if (idx != -1) {
        cout << "First word with length > 5: " << words[idx] << " at index " << idx << endl;
    }

    // Filter words with length > 5
    auto longWords = words.filter([](const string& s) { return s.length() > 5; });
    cout << "Words with length > 5: ";
    longWords.print();

    return 0;
}
