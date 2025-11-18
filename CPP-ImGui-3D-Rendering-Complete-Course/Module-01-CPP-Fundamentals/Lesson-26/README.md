# Lesson 26: Lambda Expressions

## Duration
3 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand and write lambda expressions
- Use different capture modes effectively
- Work with std::function and function pointers
- Apply lambdas with STL algorithms
- Implement functional programming patterns
- Understand lambda type deduction
- Use mutable lambdas and return type deduction

## Introduction

Lambda expressions, introduced in C++11, provide a concise way to create anonymous function objects. They are particularly useful for short snippets of code that are passed to algorithms or used as callbacks. Lambdas make code more readable and maintainable by keeping logic close to where it's used.

## 1. Basic Lambda Syntax

### Simple Lambdas

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // Basic lambda with no parameters
    auto greet = []() {
        cout << "Hello from lambda!" << endl;
    };
    greet();

    // Lambda with parameters
    auto add = [](int a, int b) {
        return a + b;
    };
    cout << "5 + 3 = " << add(5, 3) << endl;

    // Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        return (b != 0) ? a / b : 0.0;
    };
    cout << "10 / 3 = " << divide(10, 3) << endl;

    // Immediately invoked lambda
    int result = [](int x) {
        return x * x;
    }(7);
    cout << "7^2 = " << result << endl;

    // Lambda with STL algorithm
    vector<int> numbers = {5, 2, 8, 1, 9, 3};

    // Sort ascending
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a < b;
    });

    cout << "Sorted: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;

    return 0;
}
```

## 2. Capture Modes

### Value and Reference Captures

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int x = 10;
    int y = 20;
    int z = 30;

    // Capture by value
    auto byValue = [x]() {
        cout << "Captured x by value: " << x << endl;
        // x++; // Error: cannot modify
    };
    byValue();

    // Capture by reference
    auto byReference = [&x]() {
        cout << "Captured x by reference: " << x << endl;
        x++; // OK: can modify
    };
    byReference();
    cout << "x after modification: " << x << endl;

    // Capture multiple variables
    auto multiple = [x, &y]() {
        cout << "x=" << x << ", y=" << y << endl;
        y++; // Can modify y (reference)
        // x++; // Error: x is by value
    };
    multiple();
    cout << "y after modification: " << y << endl;

    // Capture all by value
    auto captureAllValue = [=]() {
        cout << "x=" << x << ", y=" << y << ", z=" << z << endl;
    };
    captureAllValue();

    // Capture all by reference
    auto captureAllRef = [&]() {
        x += 10;
        y += 10;
        z += 10;
    };
    captureAllRef();
    cout << "After modifying all: x=" << x << ", y=" << y << ", z=" << z << endl;

    // Mixed capture: all by value, specific by reference
    x = 1; y = 2; z = 3;
    auto mixed1 = [=, &y]() {
        cout << "x=" << x << ", y=" << y << ", z=" << z << endl;
        y++;
    };
    mixed1();

    // Mixed capture: all by reference, specific by value
    auto mixed2 = [&, x]() {
        cout << "x=" << x << ", y=" << y << ", z=" << z << endl;
        y++;
        z++;
        // x++; // Error: x is by value
    };
    mixed2();
    cout << "Final: x=" << x << ", y=" << y << ", z=" << z << endl;

    return 0;
}
```

## 3. Mutable Lambdas

### Modifying Captured Values

```cpp
#include <iostream>
using namespace std;

int main() {
    int counter = 0;

    // Mutable lambda - can modify captured values
    auto increment = [counter]() mutable {
        counter++;
        cout << "Lambda counter: " << counter << endl;
        return counter;
    };

    increment(); // 1
    increment(); // 2
    increment(); // 3
    cout << "Original counter: " << counter << endl; // Still 0

    // Mutable lambda with reference capture
    auto incrementRef = [&counter]() {
        counter++;
        cout << "Lambda counter (ref): " << counter << endl;
    };

    incrementRef(); // 1
    incrementRef(); // 2
    cout << "Original counter: " << counter << endl; // 2

    // State machine using mutable lambda
    auto stateMachine = [state = 0]() mutable {
        state++;
        cout << "State: " << state << endl;
        return state;
    };

    stateMachine(); // 1
    stateMachine(); // 2
    stateMachine(); // 3

    return 0;
}
```

## 4. Lambdas with STL Algorithms

### Functional Programming with Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // for_each
    cout << "Numbers: ";
    for_each(numbers.begin(), numbers.end(), [](int n) {
        cout << n << " ";
    });
    cout << endl;

    // transform
    vector<int> squared;
    transform(numbers.begin(), numbers.end(), back_inserter(squared),
             [](int n) { return n * n; });

    cout << "Squared: ";
    for (int n : squared) cout << n << " ";
    cout << endl;

    // count_if
    int evenCount = count_if(numbers.begin(), numbers.end(),
                            [](int n) { return n % 2 == 0; });
    cout << "Even numbers: " << evenCount << endl;

    // find_if
    auto it = find_if(numbers.begin(), numbers.end(),
                     [](int n) { return n > 5; });
    if (it != numbers.end()) {
        cout << "First number > 5: " << *it << endl;
    }

    // remove_if
    vector<int> filtered = numbers;
    filtered.erase(
        remove_if(filtered.begin(), filtered.end(),
                 [](int n) { return n % 2 == 0; }),
        filtered.end()
    );

    cout << "Odd numbers only: ";
    for (int n : filtered) cout << n << " ";
    cout << endl;

    // accumulate with lambda
    int sum = accumulate(numbers.begin(), numbers.end(), 0,
                        [](int acc, int n) { return acc + n; });
    cout << "Sum: " << sum << endl;

    // Custom sorting with complex criteria
    vector<string> words = {"apple", "pie", "banana", "a", "zoo"};

    // Sort by length, then alphabetically
    sort(words.begin(), words.end(), [](const string& a, const string& b) {
        if (a.length() != b.length()) {
            return a.length() < b.length();
        }
        return a < b;
    });

    cout << "Sorted words: ";
    for (const auto& w : words) cout << w << " ";
    cout << endl;

    return 0;
}
```

## 5. std::function and Function Objects

### Working with Function Types

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <map>
using namespace std;

// Function that takes a function as parameter
void applyOperation(int x, int y, function<int(int, int)> op) {
    cout << "Result: " << op(x, y) << endl;
}

// Function that returns a function
function<int(int)> makeMultiplier(int factor) {
    return [factor](int x) {
        return x * factor;
    };
}

class Calculator {
private:
    map<string, function<double(double, double)>> operations;

public:
    Calculator() {
        // Register operations as lambdas
        operations["add"] = [](double a, double b) { return a + b; };
        operations["subtract"] = [](double a, double b) { return a - b; };
        operations["multiply"] = [](double a, double b) { return a * b; };
        operations["divide"] = [](double a, double b) {
            return (b != 0) ? a / b : 0.0;
        };
    }

    double calculate(string op, double a, double b) {
        if (operations.find(op) != operations.end()) {
            return operations[op](a, b);
        }
        return 0.0;
    }
};

int main() {
    // Using std::function
    function<int(int, int)> add = [](int a, int b) { return a + b; };
    function<int(int, int)> multiply = [](int a, int b) { return a * b; };

    cout << "10 + 5 = " << add(10, 5) << endl;
    cout << "10 * 5 = " << multiply(10, 5) << endl;
    cout << endl;

    // Passing functions as arguments
    applyOperation(10, 20, add);
    applyOperation(10, 20, multiply);
    applyOperation(10, 20, [](int a, int b) { return a - b; });
    cout << endl;

    // Factory function
    auto times2 = makeMultiplier(2);
    auto times10 = makeMultiplier(10);

    cout << "5 * 2 = " << times2(5) << endl;
    cout << "5 * 10 = " << times10(5) << endl;
    cout << endl;

    // Calculator example
    Calculator calc;
    cout << "10 + 5 = " << calc.calculate("add", 10, 5) << endl;
    cout << "10 - 5 = " << calc.calculate("subtract", 10, 5) << endl;
    cout << "10 * 5 = " << calc.calculate("multiply", 10, 5) << endl;
    cout << "10 / 5 = " << calc.calculate("divide", 10, 5) << endl;

    return 0;
}
```

## 6. Advanced Lambda Techniques

### Recursive Lambdas and More

```cpp
#include <iostream>
#include <functional>
#include <vector>
using namespace std;

int main() {
    // Recursive lambda using std::function
    function<int(int)> factorial = [&factorial](int n) -> int {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    };

    cout << "5! = " << factorial(5) << endl;
    cout << "10! = " << factorial(10) << endl;
    cout << endl;

    // Lambda with init capture (C++14)
    auto counter = [count = 0]() mutable {
        return ++count;
    };

    cout << "Counter: " << counter() << endl; // 1
    cout << "Counter: " << counter() << endl; // 2
    cout << "Counter: " << counter() << endl; // 3
    cout << endl;

    // Generic lambda (C++14)
    auto print = [](const auto& x) {
        cout << x << endl;
    };

    print(42);
    print(3.14);
    print("Hello");
    print(string("World"));
    cout << endl;

    // Lambda as predicate builder
    auto makeRangeChecker = [](int min, int max) {
        return [min, max](int value) {
            return value >= min && value <= max;
        };
    };

    auto inRange = makeRangeChecker(10, 20);
    cout << "15 in range [10, 20]? " << (inRange(15) ? "Yes" : "No") << endl;
    cout << "25 in range [10, 20]? " << (inRange(25) ? "Yes" : "No") << endl;
    cout << endl;

    // Composing lambdas
    auto addOne = [](int x) { return x + 1; };
    auto multiplyByTwo = [](int x) { return x * 2; };

    auto compose = [](auto f, auto g) {
        return [=](auto x) { return f(g(x)); };
    };

    auto addThenMultiply = compose(multiplyByTwo, addOne);
    cout << "(5 + 1) * 2 = " << addThenMultiply(5) << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Custom Sorting
Use lambdas to sort a vector of custom objects (Person, Product, etc.) by different criteria.

### Exercise 2: Filter and Transform
Create a data processing pipeline using lambdas with filter, map, and reduce operations.

### Exercise 3: Event System
Implement an event system using lambdas as event handlers.

### Exercise 4: Function Composition
Build a framework for composing functions using lambdas.

### Exercise 5: State Machine
Create a state machine using lambdas to represent states and transitions.

## Exercise Solutions

### Solution 1: Custom Sorting

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct Person {
    string name;
    int age;
    double salary;

    void print() const {
        cout << name << " (Age: " << age << ", Salary: $" << salary << ")" << endl;
    }
};

int main() {
    vector<Person> people = {
        {"Alice", 30, 75000},
        {"Bob", 25, 60000},
        {"Charlie", 35, 80000},
        {"Diana", 28, 70000}
    };

    // Sort by age
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;
    });

    cout << "Sorted by age:" << endl;
    for (const auto& p : people) p.print();
    cout << endl;

    // Sort by salary (descending)
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.salary > b.salary;
    });

    cout << "Sorted by salary (desc):" << endl;
    for (const auto& p : people) p.print();
    cout << endl;

    // Sort by name length, then alphabetically
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        if (a.name.length() != b.name.length()) {
            return a.name.length() < b.name.length();
        }
        return a.name < b.name;
    });

    cout << "Sorted by name length:" << endl;
    for (const auto& p : people) p.print();

    return 0;
}
```

### Solution 2: Filter and Transform

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

template <typename T>
class Pipeline {
private:
    vector<T> data;

public:
    Pipeline(const vector<T>& input) : data(input) {}

    Pipeline<T>& filter(function<bool(const T&)> predicate) {
        vector<T> result;
        copy_if(data.begin(), data.end(), back_inserter(result), predicate);
        data = result;
        return *this;
    }

    template <typename R>
    Pipeline<R> map(function<R(const T&)> transform) {
        vector<R> result;
        std::transform(data.begin(), data.end(), back_inserter(result), transform);
        return Pipeline<R>(result);
    }

    T reduce(T initial, function<T(T, const T&)> accumulator) {
        return accumulate(data.begin(), data.end(), initial, accumulator);
    }

    vector<T> collect() const {
        return data;
    }

    void print() const {
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }
};

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Filter even numbers, multiply by 2, sum them
    int result = Pipeline<int>(numbers)
        .filter([](int x) { return x % 2 == 0; })
        .map<int>([](int x) { return x * 2; })
        .reduce(0, [](int acc, int x) { return acc + x; });

    cout << "Result: " << result << endl;

    // Filter numbers > 5, square them, collect
    auto squared = Pipeline<int>(numbers)
        .filter([](int x) { return x > 5; })
        .map<int>([](int x) { return x * x; })
        .collect();

    cout << "Squared (>5): ";
    for (int x : squared) cout << x << " ";
    cout << endl;

    return 0;
}
```

### Solution 3: Event System

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <map>
using namespace std;

class EventSystem {
private:
    map<string, vector<function<void()>>> handlers;

public:
    void on(const string& event, function<void()> handler) {
        handlers[event].push_back(handler);
    }

    void emit(const string& event) {
        if (handlers.find(event) != handlers.end()) {
            for (auto& handler : handlers[event]) {
                handler();
            }
        }
    }
};

template <typename T>
class EventSystemTyped {
private:
    map<string, vector<function<void(T)>>> handlers;

public:
    void on(const string& event, function<void(T)> handler) {
        handlers[event].push_back(handler);
    }

    void emit(const string& event, T data) {
        if (handlers.find(event) != handlers.end()) {
            for (auto& handler : handlers[event]) {
                handler(data);
            }
        }
    }
};

int main() {
    EventSystem events;

    // Register event handlers
    events.on("start", []() {
        cout << "Application started!" << endl;
    });

    events.on("start", []() {
        cout << "Initializing..." << endl;
    });

    events.on("stop", []() {
        cout << "Application stopped!" << endl;
    });

    // Emit events
    events.emit("start");
    cout << endl;
    events.emit("stop");
    cout << endl;

    // Typed event system
    EventSystemTyped<string> typedEvents;

    typedEvents.on("message", [](string msg) {
        cout << "Message received: " << msg << endl;
    });

    typedEvents.on("message", [](string msg) {
        cout << "Logging: " << msg << endl;
    });

    typedEvents.emit("message", "Hello Events!");

    return 0;
}
```

### Solution 4 & 5: Abbreviated due to space

```cpp
// Function Composition
template <typename F, typename G>
auto compose(F f, G g) {
    return [=](auto x) { return f(g(x)); };
}

// State Machine
class StateMachine {
    function<void()> currentState;
public:
    void setState(function<void()> state) {
        currentState = state;
    }
    void execute() { currentState(); }
};
```

## Summary

In this lesson, you learned:
- Lambda expression syntax and basic usage
- Different capture modes (value, reference, mixed)
- Mutable lambdas for stateful operations
- Using lambdas with STL algorithms
- std::function for storing lambdas
- Advanced techniques including recursive and generic lambdas
- Functional programming patterns with lambdas

## Checklist

- [ ] I understand lambda syntax
- [ ] I can use different capture modes
- [ ] I know when to use mutable lambdas
- [ ] I can apply lambdas with STL algorithms
- [ ] I understand std::function
- [ ] I can write generic lambdas (C++14)
- [ ] I completed all exercises
- [ ] I understand when to use lambdas vs regular functions

## Next Steps
In the next lesson, we'll explore Advanced Pointers, diving deep into pointer arithmetic, multi-level pointers, and advanced pointer techniques.
