# Lesson 20: Lambda Expressions and Functional Programming

## Overview
Lambda expressions (anonymous functions) are a powerful feature introduced in C++11 that allow you to define inline, unnamed function objects. They're essential for modern C++ programming, especially when working with STL algorithms and callbacks.

## Topics Covered
- Lambda syntax and basic usage
- Capture clauses (by value, by reference)
- Mutable lambdas
- Generic lambdas (C++14)
- Lambda return types
- Functional programming patterns
- std::function and function objects
- Practical applications

---

## 1. Basic Lambda Syntax

### Example 1: Introduction to Lambdas

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // Basic lambda syntax: [capture](parameters) -> return_type { body }

    // Simplest lambda (no parameters, no capture)
    auto hello = []() {
        std::cout << "Hello from lambda!" << std::endl;
    };
    hello();

    // Lambda with parameters
    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;

    // Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0.0;
        return a / b;
    };
    std::cout << "10 / 3 = " << divide(10.0, 3.0) << std::endl;

    // Inline lambda usage with STL
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Even numbers: ";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        if (n % 2 == 0) {
            std::cout << n << " ";
        }
    });
    std::cout << std::endl;

    // Lambda with count_if
    int even_count = std::count_if(numbers.begin(), numbers.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "Count of even numbers: " << even_count << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example1.cpp -o example1`

---

## 2. Capture Clauses

### Example 2: Capturing Variables

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int x = 10;
    int y = 20;

    // Capture by value (copies)
    auto lambda1 = [x, y]() {
        std::cout << "Captured by value: x=" << x << ", y=" << y << std::endl;
        // x = 15;  // ERROR: can't modify captured-by-value variables
    };
    lambda1();

    // Capture by reference
    auto lambda2 = [&x, &y]() {
        std::cout << "Before modify: x=" << x << ", y=" << y << std::endl;
        x = 15;  // OK: can modify captured-by-reference variables
        y = 25;
        std::cout << "After modify: x=" << x << ", y=" << y << std::endl;
    };
    lambda2();
    std::cout << "Outside lambda: x=" << x << ", y=" << y << std::endl;

    // Capture all by value [=]
    int a = 5, b = 10, c = 15;
    auto lambda3 = [=]() {
        std::cout << "Captured all by value: a=" << a << ", b=" << b
                  << ", c=" << c << std::endl;
    };
    lambda3();

    // Capture all by reference [&]
    auto lambda4 = [&]() {
        a *= 2;
        b *= 2;
        c *= 2;
    };
    lambda4();
    std::cout << "After lambda4: a=" << a << ", b=" << b << ", c=" << c << std::endl;

    // Mixed capture: by value and reference
    int threshold = 50;
    int multiplier = 3;
    auto lambda5 = [threshold, &multiplier](int n) {
        return n * multiplier > threshold;
    };

    std::vector<int> nums = {10, 20, 30, 40, 50};
    std::cout << "\nNumbers where n * " << multiplier << " > " << threshold << ": ";
    for (int n : nums) {
        if (lambda5(n)) {
            std::cout << n << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example2.cpp -o example2`

---

## 3. Mutable Lambdas

### Example 3: Mutable Keyword

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int counter = 0;

    // Non-mutable lambda (ERROR if we try to modify captured-by-value variables)
    // auto increment1 = [counter]() {
    //     counter++;  // ERROR: can't modify
    // };

    // Mutable lambda (allows modification of captured-by-value variables)
    auto increment = [counter]() mutable {
        counter++;
        std::cout << "Inside lambda: counter = " << counter << std::endl;
        return counter;
    };

    std::cout << "Before calls: counter = " << counter << std::endl;
    increment();
    increment();
    increment();
    std::cout << "After calls: counter = " << counter << std::endl;
    // Note: original counter is unchanged because it was captured by value

    // Practical example: generate sequence
    auto sequence_generator = [n = 0]() mutable {
        return n++;
    };

    std::cout << "\nGenerated sequence: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << sequence_generator() << " ";
    }
    std::cout << std::endl;

    // Using mutable lambda with transform
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::vector<int> cumulative;

    std::transform(data.begin(), data.end(), std::back_inserter(cumulative),
                   [sum = 0](int n) mutable {
                       sum += n;
                       return sum;
                   });

    std::cout << "Original: ";
    for (int n : data) std::cout << n << " ";
    std::cout << "\nCumulative: ";
    for (int n : cumulative) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example3.cpp -o example3`

---

## 4. Generic Lambdas (C++14)

### Example 4: Auto Parameters

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Generic lambda with auto parameter (C++14)
    auto print = [](const auto& value) {
        std::cout << value << std::endl;
    };

    print(42);
    print(3.14);
    print("Hello, World!");
    print(std::string("C++ Lambda"));

    // Generic lambda for comparison
    auto compare = [](const auto& a, const auto& b) {
        return a < b;
    };

    std::cout << "\n5 < 10: " << std::boolalpha << compare(5, 10) << std::endl;
    std::cout << "3.14 < 2.71: " << compare(3.14, 2.71) << std::endl;
    std::cout << "\"apple\" < \"banana\": "
              << compare(std::string("apple"), std::string("banana")) << std::endl;

    // Generic lambda with multiple auto parameters
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "\n10 + 20 = " << add(10, 20) << std::endl;
    std::cout << "3.5 + 2.5 = " << add(3.5, 2.5) << std::endl;
    std::cout << "\"Hello\" + \" World\" = "
              << add(std::string("Hello"), std::string(" World")) << std::endl;

    // Using generic lambda with different container types
    std::vector<int> int_vec = {5, 2, 8, 1, 9};
    std::vector<std::string> str_vec = {"apple", "orange", "banana", "grape"};

    auto print_sorted = [](auto& container) {
        std::sort(container.begin(), container.end());
        for (const auto& elem : container) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    };

    std::cout << "\nSorted integers: ";
    print_sorted(int_vec);

    std::cout << "Sorted strings: ";
    print_sorted(str_vec);

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example4.cpp -o example4`

---

## 5. Lambda Return Types

### Example 5: Explicit and Deduced Return Types

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // Deduced return type (simple case)
    auto multiply = [](int a, int b) {
        return a * b;  // Return type deduced as int
    };

    // Explicit return type
    auto safe_divide = [](double a, double b) -> double {
        if (b == 0) return 0.0;
        return a / b;
    };

    std::cout << "10 * 5 = " << multiply(10, 5) << std::endl;
    std::cout << "10 / 3 = " << safe_divide(10.0, 3.0) << std::endl;

    // Multiple return statements (needs explicit type)
    auto get_grade = [](int score) -> std::string {
        if (score >= 90) return "A";
        if (score >= 80) return "B";
        if (score >= 70) return "C";
        if (score >= 60) return "D";
        return "F";
    };

    std::cout << "\nScore 85 = Grade " << get_grade(85) << std::endl;
    std::cout << "Score 92 = Grade " << get_grade(92) << std::endl;

    // Returning different types from branches
    auto get_status = [](int value) -> std::string {
        if (value < 0) {
            return "negative";
        } else if (value == 0) {
            return "zero";
        } else {
            return "positive";
        }
    };

    std::cout << "\nStatus of -5: " << get_status(-5) << std::endl;
    std::cout << "Status of 10: " << get_status(10) << std::endl;

    // Return type deduction with auto (C++14)
    auto factorial = [](int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;  // Deduced as int
    };

    std::cout << "\n5! = " << factorial(5) << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example5.cpp -o example5`

---

## 6. std::function and Function Objects

### Example 6: Storing Lambdas

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>

int main() {
    // std::function can store any callable
    std::function<int(int, int)> operation;

    // Store lambda in std::function
    operation = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << operation(5, 3) << std::endl;

    // Change the function
    operation = [](int a, int b) { return a * b; };
    std::cout << "5 * 3 = " << operation(5, 3) << std::endl;

    // Vector of functions
    std::vector<std::function<int(int)>> transformations;

    transformations.push_back([](int x) { return x * 2; });
    transformations.push_back([](int x) { return x * x; });
    transformations.push_back([](int x) { return x + 10; });

    int value = 5;
    std::cout << "\nApplying transformations to " << value << ":" << std::endl;
    for (const auto& transform : transformations) {
        std::cout << "Result: " << transform(value) << std::endl;
    }

    // Map of operations
    std::map<std::string, std::function<double(double, double)>> calculator;

    calculator["add"] = [](double a, double b) { return a + b; };
    calculator["subtract"] = [](double a, double b) { return a - b; };
    calculator["multiply"] = [](double a, double b) { return a * b; };
    calculator["divide"] = [](double a, double b) {
        return b != 0 ? a / b : 0.0;
    };

    std::cout << "\nCalculator:" << std::endl;
    std::cout << "10 + 5 = " << calculator["add"](10, 5) << std::endl;
    std::cout << "10 - 5 = " << calculator["subtract"](10, 5) << std::endl;
    std::cout << "10 * 5 = " << calculator["multiply"](10, 5) << std::endl;
    std::cout << "10 / 5 = " << calculator["divide"](10, 5) << std::endl;

    // Function that returns a function
    auto make_multiplier = [](int factor) {
        return [factor](int value) {
            return value * factor;
        };
    };

    auto times_2 = make_multiplier(2);
    auto times_10 = make_multiplier(10);

    std::cout << "\n7 * 2 = " << times_2(7) << std::endl;
    std::cout << "7 * 10 = " << times_10(7) << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example6.cpp -o example6`

---

## 7. Functional Programming Patterns

### Example 7: Map, Filter, Reduce

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

template<typename Container, typename Func>
auto map(const Container& input, Func func) {
    Container output;
    std::transform(input.begin(), input.end(),
                  std::back_inserter(output), func);
    return output;
}

template<typename Container, typename Predicate>
auto filter(const Container& input, Predicate pred) {
    Container output;
    std::copy_if(input.begin(), input.end(),
                std::back_inserter(output), pred);
    return output;
}

template<typename Container, typename T, typename Func>
T reduce(const Container& input, T init, Func func) {
    return std::accumulate(input.begin(), input.end(), init, func);
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Original: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // Map: square each number
    auto squared = map(numbers, [](int n) { return n * n; });
    std::cout << "Squared: ";
    for (int n : squared) std::cout << n << " ";
    std::cout << std::endl;

    // Filter: keep only even numbers
    auto evens = filter(numbers, [](int n) { return n % 2 == 0; });
    std::cout << "Evens: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // Reduce: sum all numbers
    int sum = reduce(numbers, 0, [](int acc, int n) { return acc + n; });
    std::cout << "Sum: " << sum << std::endl;

    // Reduce: product of all numbers
    int product = reduce(numbers, 1, [](int acc, int n) { return acc * n; });
    std::cout << "Product: " << product << std::endl;

    // Chaining operations
    auto result = map(
        filter(numbers, [](int n) { return n % 2 == 0; }),
        [](int n) { return n * n; }
    );

    std::cout << "\nSquared evens: ";
    for (int n : result) std::cout << n << " ";
    std::cout << std::endl;

    // Complex chaining
    auto sum_of_squared_evens = reduce(
        map(
            filter(numbers, [](int n) { return n % 2 == 0; }),
            [](int n) { return n * n; }
        ),
        0,
        [](int acc, int n) { return acc + n; }
    );

    std::cout << "Sum of squared evens: " << sum_of_squared_evens << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example7.cpp -o example7`

---

## 8. Immediately Invoked Lambda Expressions (IIFE)

### Example 8: IIFE Pattern

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // Initialize const variable with complex logic
    const int value = []() {
        int result = 0;
        for (int i = 1; i <= 10; ++i) {
            result += i * i;
        }
        return result;
    }();  // <- Immediately invoked

    std::cout << "Computed value: " << value << std::endl;

    // Complex initialization
    const auto data = []() {
        std::vector<int> temp;
        for (int i = 1; i <= 10; ++i) {
            if (i % 2 == 0) {
                temp.push_back(i * i);
            }
        }
        return temp;
    }();

    std::cout << "Data: ";
    for (int n : data) std::cout << n << " ";
    std::cout << std::endl;

    // Conditional initialization
    const std::string message = [](bool error) {
        if (error) {
            return std::string("Error occurred!");
        } else {
            return std::string("Success!");
        }
    }(false);  // Pass argument to IIFE

    std::cout << "Message: " << message << std::endl;

    // Scoped execution
    std::cout << "\nExecuting scoped code block:" << std::endl;
    [&]() {
        std::cout << "  This runs immediately" << std::endl;
        std::cout << "  Can access outer scope" << std::endl;
        std::cout << "  Useful for complex initialization" << std::endl;
    }();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example8.cpp -o example8`

---

## Project 1: Event System with Lambdas

A flexible event handling system using lambda expressions.

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class EventSystem {
public:
    using EventHandler = std::function<void(const std::string&)>;

private:
    std::map<std::string, std::vector<EventHandler>> listeners;
    int next_id = 0;

public:
    // Register event listener
    int on(const std::string& event, EventHandler handler) {
        listeners[event].push_back(handler);
        return next_id++;
    }

    // Emit event
    void emit(const std::string& event, const std::string& data = "") {
        if (listeners.find(event) != listeners.end()) {
            for (const auto& handler : listeners[event]) {
                handler(data);
            }
        }
    }

    // Remove all listeners for an event
    void off(const std::string& event) {
        listeners.erase(event);
    }

    // Get listener count for event
    size_t listenerCount(const std::string& event) const {
        auto it = listeners.find(event);
        if (it != listeners.end()) {
            return it->second.size();
        }
        return 0;
    }
};

class Logger {
private:
    std::string name;
    std::vector<std::string> logs;

public:
    Logger(const std::string& logger_name) : name(logger_name) {}

    void log(const std::string& message) {
        std::string entry = "[" + name + "] " + message;
        logs.push_back(entry);
        std::cout << entry << std::endl;
    }

    const std::vector<std::string>& getLogs() const {
        return logs;
    }
};

int main() {
    EventSystem events;
    Logger logger("System");

    std::cout << "=== Event System Demo ===" << std::endl;

    // Register event handlers using lambdas
    events.on("user_login", [&logger](const std::string& username) {
        logger.log("User logged in: " + username);
    });

    events.on("user_login", [](const std::string& username) {
        std::cout << "Welcome, " << username << "!" << std::endl;
    });

    events.on("user_logout", [&logger](const std::string& username) {
        logger.log("User logged out: " + username);
    });

    events.on("user_logout", [](const std::string& username) {
        std::cout << "Goodbye, " << username << "!" << std::endl;
    });

    events.on("error", [&logger](const std::string& error_msg) {
        logger.log("ERROR: " + error_msg);
    });

    // Data processing events
    int processed_count = 0;
    events.on("data_processed", [&processed_count](const std::string& data) {
        processed_count++;
        std::cout << "Processed item #" << processed_count << ": " << data << std::endl;
    });

    // Emit events
    std::cout << "\n--- User Actions ---" << std::endl;
    events.emit("user_login", "Alice");
    events.emit("user_login", "Bob");

    std::cout << "\n--- Data Processing ---" << std::endl;
    events.emit("data_processed", "Record 1");
    events.emit("data_processed", "Record 2");
    events.emit("data_processed", "Record 3");

    std::cout << "\n--- Error Event ---" << std::endl;
    events.emit("error", "Connection timeout");

    std::cout << "\n--- User Logout ---" << std::endl;
    events.emit("user_logout", "Alice");
    events.emit("user_logout", "Bob");

    // Statistics
    std::cout << "\n--- Statistics ---" << std::endl;
    std::cout << "Total items processed: " << processed_count << std::endl;
    std::cout << "Login listeners: " << events.listenerCount("user_login") << std::endl;
    std::cout << "Logout listeners: " << events.listenerCount("user_logout") << std::endl;

    // Display all logs
    std::cout << "\n--- Complete Log ---" << std::endl;
    for (const auto& log : logger.getLogs()) {
        std::cout << log << std::endl;
    }

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project1.cpp -o project1`

---

## Project 2: Data Processing Pipeline

A functional programming approach to data transformation.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cctype>

template<typename T>
class Pipeline {
private:
    std::vector<T> data;

public:
    Pipeline(const std::vector<T>& input) : data(input) {}

    // Map operation
    template<typename Func>
    Pipeline<typename std::result_of<Func(T)>::type> map(Func func) const {
        using ResultType = typename std::result_of<Func(T)>::type;
        std::vector<ResultType> result;
        std::transform(data.begin(), data.end(),
                      std::back_inserter(result), func);
        return Pipeline<ResultType>(result);
    }

    // Filter operation
    template<typename Predicate>
    Pipeline<T> filter(Predicate pred) const {
        std::vector<T> result;
        std::copy_if(data.begin(), data.end(),
                    std::back_inserter(result), pred);
        return Pipeline<T>(result);
    }

    // Reduce operation
    template<typename U, typename Func>
    U reduce(U init, Func func) const {
        return std::accumulate(data.begin(), data.end(), init, func);
    }

    // Take first n elements
    Pipeline<T> take(size_t n) const {
        std::vector<T> result;
        size_t count = std::min(n, data.size());
        std::copy_n(data.begin(), count, std::back_inserter(result));
        return Pipeline<T>(result);
    }

    // Skip first n elements
    Pipeline<T> skip(size_t n) const {
        std::vector<T> result;
        if (n < data.size()) {
            std::copy(data.begin() + n, data.end(),
                     std::back_inserter(result));
        }
        return Pipeline<T>(result);
    }

    // Sort
    template<typename Compare = std::less<T>>
    Pipeline<T> sort(Compare comp = Compare()) const {
        std::vector<T> result = data;
        std::sort(result.begin(), result.end(), comp);
        return Pipeline<T>(result);
    }

    // Get result
    const std::vector<T>& get() const {
        return data;
    }

    // Print
    void print() const {
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    std::cout << "=== Data Processing Pipeline Demo ===" << std::endl;

    // Example 1: Number processing
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    std::cout << "\nOriginal numbers:" << std::endl;
    Pipeline<int>(numbers).print();

    std::cout << "\nEven numbers squared:" << std::endl;
    Pipeline<int>(numbers)
        .filter([](int n) { return n % 2 == 0; })
        .map([](int n) { return n * n; })
        .print();

    std::cout << "\nFirst 5 odd numbers tripled:" << std::endl;
    Pipeline<int>(numbers)
        .filter([](int n) { return n % 2 != 0; })
        .map([](int n) { return n * 3; })
        .take(5)
        .print();

    // Example 2: String processing
    std::vector<std::string> words = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon"
    };

    std::cout << "\nOriginal words:" << std::endl;
    Pipeline<std::string>(words).print();

    std::cout << "\nWords longer than 5 chars, uppercase:" << std::endl;
    Pipeline<std::string>(words)
        .filter([](const std::string& s) { return s.length() > 5; })
        .map([](const std::string& s) {
            std::string upper = s;
            std::transform(upper.begin(), upper.end(), upper.begin(),
                          [](unsigned char c) { return std::toupper(c); });
            return upper;
        })
        .print();

    // Example 3: Complex chaining
    std::cout << "\nSum of squares of even numbers:" << std::endl;
    int sum = Pipeline<int>(numbers)
        .filter([](int n) { return n % 2 == 0; })
        .map([](int n) { return n * n; })
        .reduce(0, [](int acc, int n) { return acc + n; });
    std::cout << sum << std::endl;

    // Example 4: String length analysis
    std::cout << "\nAverage length of long words (>6 chars):" << std::endl;
    auto long_words = Pipeline<std::string>(words)
        .filter([](const std::string& s) { return s.length() > 6; });

    auto lengths = long_words.map([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    int total_length = lengths.reduce(0, [](int acc, int len) {
        return acc + len;
    });

    auto word_list = long_words.get();
    if (!word_list.empty()) {
        double avg = static_cast<double>(total_length) / word_list.size();
        std::cout << "Average: " << avg << " characters" << std::endl;
        std::cout << "Words: ";
        long_words.print();
    }

    // Example 5: Top-N pattern
    std::cout << "\nTop 5 numbers (descending):" << std::endl;
    Pipeline<int>(numbers)
        .sort([](int a, int b) { return a > b; })
        .take(5)
        .print();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project2.cpp -o project2`

---

## Project 3: Query Builder with Lambdas

A SQL-like query interface using lambda expressions.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>

struct Person {
    std::string name;
    int age;
    std::string city;
    double salary;

    void print() const {
        std::cout << std::setw(15) << std::left << name
                  << std::setw(5) << age
                  << std::setw(15) << city
                  << "$" << std::fixed << std::setprecision(2) << salary
                  << std::endl;
    }
};

template<typename T>
class Query {
private:
    std::vector<T> data;

public:
    Query(const std::vector<T>& input) : data(input) {}

    // WHERE clause
    Query<T> where(std::function<bool(const T&)> predicate) const {
        std::vector<T> result;
        std::copy_if(data.begin(), data.end(),
                    std::back_inserter(result), predicate);
        return Query<T>(result);
    }

    // ORDER BY clause
    Query<T> orderBy(std::function<bool(const T&, const T&)> comparator) const {
        std::vector<T> result = data;
        std::sort(result.begin(), result.end(), comparator);
        return Query<T>(result);
    }

    // SELECT (map to different type)
    template<typename U>
    Query<U> select(std::function<U(const T&)> selector) const {
        std::vector<U> result;
        std::transform(data.begin(), data.end(),
                      std::back_inserter(result), selector);
        return Query<U>(result);
    }

    // LIMIT
    Query<T> limit(size_t n) const {
        std::vector<T> result;
        size_t count = std::min(n, data.size());
        std::copy_n(data.begin(), count, std::back_inserter(result));
        return Query<T>(result);
    }

    // COUNT
    size_t count() const {
        return data.size();
    }

    // ANY
    bool any(std::function<bool(const T&)> predicate) const {
        return std::any_of(data.begin(), data.end(), predicate);
    }

    // ALL
    bool all(std::function<bool(const T&)> predicate) const {
        return std::all_of(data.begin(), data.end(), predicate);
    }

    // Get results
    const std::vector<T>& results() const {
        return data;
    }

    // Print results (for Person type)
    void print() const {
        for (const auto& item : data) {
            item.print();
        }
    }
};

int main() {
    // Sample data
    std::vector<Person> people = {
        {"Alice Johnson", 28, "New York", 75000},
        {"Bob Smith", 35, "Los Angeles", 85000},
        {"Carol Davis", 42, "Chicago", 95000},
        {"David Brown", 31, "New York", 70000},
        {"Eve Wilson", 29, "Los Angeles", 80000},
        {"Frank Miller", 45, "Chicago", 100000},
        {"Grace Lee", 33, "New York", 90000},
        {"Henry Clark", 38, "Los Angeles", 88000},
        {"Ivy Martinez", 27, "Chicago", 72000},
        {"Jack Anderson", 40, "New York", 92000}
    };

    std::cout << "=== Query Builder Demo ===" << std::endl;

    // Query 1: People in New York
    std::cout << "\nPeople in New York:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    Query<Person>(people)
        .where([](const Person& p) { return p.city == "New York"; })
        .print();

    // Query 2: People older than 35, ordered by salary (descending)
    std::cout << "\nPeople older than 35, ordered by salary:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    Query<Person>(people)
        .where([](const Person& p) { return p.age > 35; })
        .orderBy([](const Person& a, const Person& b) {
            return a.salary > b.salary;
        })
        .print();

    // Query 3: Top 3 highest earners
    std::cout << "\nTop 3 highest earners:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    Query<Person>(people)
        .orderBy([](const Person& a, const Person& b) {
            return a.salary > b.salary;
        })
        .limit(3)
        .print();

    // Query 4: People in Los Angeles earning more than 80k
    std::cout << "\nPeople in Los Angeles earning > $80k:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    Query<Person>(people)
        .where([](const Person& p) {
            return p.city == "Los Angeles" && p.salary > 80000;
        })
        .print();

    // Query 5: Select just names of people under 30
    std::cout << "\nNames of people under 30:" << std::endl;
    auto young_names = Query<Person>(people)
        .where([](const Person& p) { return p.age < 30; })
        .select<std::string>([](const Person& p) { return p.name; })
        .results();

    for (const auto& name : young_names) {
        std::cout << "  - " << name << std::endl;
    }

    // Query 6: Statistics
    std::cout << "\n=== Statistics ===" << std::endl;
    size_t ny_count = Query<Person>(people)
        .where([](const Person& p) { return p.city == "New York"; })
        .count();
    std::cout << "People in New York: " << ny_count << std::endl;

    bool any_millionaire = Query<Person>(people)
        .any([](const Person& p) { return p.salary >= 1000000; });
    std::cout << "Any millionaires: " << std::boolalpha << any_millionaire << std::endl;

    bool all_adults = Query<Person>(people)
        .all([](const Person& p) { return p.age >= 18; });
    std::cout << "All adults (18+): " << all_adults << std::endl;

    // Query 7: Complex multi-condition
    std::cout << "\nPeople aged 30-40 in major cities, top 5 by salary:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    Query<Person>(people)
        .where([](const Person& p) {
            return p.age >= 30 && p.age <= 40 &&
                   (p.city == "New York" || p.city == "Los Angeles" || p.city == "Chicago");
        })
        .orderBy([](const Person& a, const Person& b) {
            return a.salary > b.salary;
        })
        .limit(5)
        .print();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project3.cpp -o project3`

---

## Best Practices

1. **Use auto for lambda types** - Let the compiler deduce the type
2. **Prefer lambdas over functors** - More concise for simple operations
3. **Capture by reference carefully** - Avoid dangling references
4. **Use generic lambdas** - More flexible and reusable
5. **Keep lambdas simple** - Complex logic should be in named functions
6. **Use std::function when needed** - For storing and passing lambdas
7. **Consider performance** - Capturing large objects by value can be expensive

---

## Summary

In this lesson, you learned about:
- Lambda expression syntax: `[capture](parameters) -> return_type { body }`
- Capture clauses: by value `[=]`, by reference `[&]`, mixed `[x, &y]`
- Mutable lambdas for modifying captured-by-value variables
- Generic lambdas with `auto` parameters (C++14)
- Explicit and deduced return types
- std::function for storing callable objects
- Functional programming patterns: map, filter, reduce
- Immediately Invoked Function Expressions (IIFE)
- Practical applications: event systems, pipelines, query builders

## Exercises

1. Create a lambda that sorts a vector of strings by length.
2. Implement a custom `for_each` function that accepts a lambda.
3. Write a lambda that captures a vector by reference and appends elements to it.
4. Create a function that returns a lambda (closure).
5. Implement a memoization function using lambdas.
6. Build a simple calculator using a map of lambdas.
7. Create a validation system using composable lambda predicates.
8. Implement recursive lambda for computing Fibonacci numbers.
9. Build a lazy evaluation system using lambdas.
10. Create a command pattern implementation with lambdas.

## Next Lesson
Part 2: Advanced C++ (Lessons 21-40)

---

## Completion: Part 1 - C++ Fundamentals

**Congratulations!** You have completed Part 1 of the C++/Qt/3D Rendering Course.

You have learned:
- C++ basics, control flow, and functions
- Arrays, pointers, references, and strings
- Object-oriented programming (classes, inheritance, polymorphism)
- Memory management and dynamic allocation
- Templates and generic programming
- File I/O operations
- Exception handling and preprocessor directives
- Namespaces and code organization
- STL containers and iterators
- STL algorithms
- Lambda expressions and functional programming

**Next:** Part 2 will cover advanced C++ topics including move semantics, smart pointers, RAII, design patterns, and more!
