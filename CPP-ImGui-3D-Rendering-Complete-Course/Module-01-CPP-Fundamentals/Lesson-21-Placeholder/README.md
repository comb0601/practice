# Lesson 21: Function Overloading

## Duration
2.5 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand the concept of function overloading
- Create multiple functions with the same name but different parameters
- Use function overloading for operator-like behavior
- Understand function signature and name mangling
- Avoid common overloading pitfalls
- Design clean APIs using function overloading
- Combine overloading with default arguments

## Introduction

Function overloading is a powerful feature in C++ that allows you to define multiple functions with the same name but different parameters. The compiler selects the appropriate function based on the arguments provided. This feature makes code more intuitive and easier to use.

## 1. Basic Function Overloading

### Simple Overloading Example

```cpp
#include <iostream>
using namespace std;

// Overloaded functions for different types
int add(int a, int b) {
    cout << "Adding two integers" << endl;
    return a + b;
}

double add(double a, double b) {
    cout << "Adding two doubles" << endl;
    return a + b;
}

string add(string a, string b) {
    cout << "Concatenating two strings" << endl;
    return a + b;
}

// Overloaded by number of parameters
int add(int a, int b, int c) {
    cout << "Adding three integers" << endl;
    return a + b + c;
}

int main() {
    cout << "Result: " << add(5, 10) << endl;
    cout << "Result: " << add(3.14, 2.86) << endl;
    cout << "Result: " << add(string("Hello "), string("World")) << endl;
    cout << "Result: " << add(1, 2, 3) << endl;

    return 0;
}
```

**Output:**
```
Adding two integers
Result: 15
Adding two doubles
Result: 6
Concatenating two strings
Result: Hello World
Adding three integers
Result: 6
```

## 2. Overloading by Parameter Types

### Complete Type-Based Overloading

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Printer {
public:
    void print(int value) {
        cout << "Integer: " << value << endl;
    }

    void print(double value) {
        cout << "Double: " << value << endl;
    }

    void print(const char* value) {
        cout << "C-String: " << value << endl;
    }

    void print(const string& value) {
        cout << "String: " << value << endl;
    }

    void print(bool value) {
        cout << "Boolean: " << (value ? "true" : "false") << endl;
    }

    void print(const vector<int>& vec) {
        cout << "Vector: [";
        for (size_t i = 0; i < vec.size(); i++) {
            cout << vec[i];
            if (i < vec.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    Printer printer;

    printer.print(42);
    printer.print(3.14159);
    printer.print("Hello");
    printer.print(string("World"));
    printer.print(true);
    printer.print(vector<int>{1, 2, 3, 4, 5});

    return 0;
}
```

## 3. Overloading by Number of Parameters

```cpp
#include <iostream>
using namespace std;

class Rectangle {
private:
    double width;
    double height;

public:
    // Default constructor
    Rectangle() : width(1.0), height(1.0) {
        cout << "Creating 1x1 rectangle" << endl;
    }

    // Single parameter (square)
    Rectangle(double side) : width(side), height(side) {
        cout << "Creating " << side << "x" << side << " square" << endl;
    }

    // Two parameters (rectangle)
    Rectangle(double w, double h) : width(w), height(h) {
        cout << "Creating " << w << "x" << h << " rectangle" << endl;
    }

    double area() const {
        return width * height;
    }

    void display() const {
        cout << "Rectangle(" << width << " x " << height
             << ") = " << area() << " sq units" << endl;
    }
};

int main() {
    Rectangle r1;           // Uses default constructor
    Rectangle r2(5.0);      // Uses single parameter (square)
    Rectangle r3(4.0, 6.0); // Uses two parameters

    r1.display();
    r2.display();
    r3.display();

    return 0;
}
```

## 4. Overloading with Const Parameters

```cpp
#include <iostream>
#include <string>
using namespace std;

class DataProcessor {
public:
    // Non-const version - can modify the string
    void process(string& data) {
        cout << "Processing non-const string (can modify)" << endl;
        data += " [PROCESSED]";
        cout << "Result: " << data << endl;
    }

    // Const version - cannot modify the string
    void process(const string& data) {
        cout << "Processing const string (read-only)" << endl;
        cout << "Data: " << data << endl;
        // data += " [MODIFIED]"; // Error: cannot modify const
    }

    // Pointer overloads
    void display(int* ptr) {
        if (ptr) {
            cout << "Non-const pointer: " << *ptr << endl;
            *ptr = *ptr * 2; // Can modify
        }
    }

    void display(const int* ptr) {
        if (ptr) {
            cout << "Const pointer: " << *ptr << endl;
            // *ptr = *ptr * 2; // Error: cannot modify
        }
    }
};

int main() {
    DataProcessor processor;

    string mutableStr = "Hello";
    const string constStr = "World";

    processor.process(mutableStr);  // Calls non-const version
    processor.process(constStr);    // Calls const version

    cout << endl;

    int value = 42;
    const int constValue = 100;

    processor.display(&value);      // Calls non-const version
    processor.display(&constValue); // Calls const version

    cout << "Modified value: " << value << endl;

    return 0;
}
```

## 5. Overloading with Reference vs Pointer

```cpp
#include <iostream>
using namespace std;

class Modifier {
public:
    // Reference version
    void modify(int& value) {
        cout << "Reference version" << endl;
        value *= 2;
    }

    // Pointer version
    void modify(int* value) {
        cout << "Pointer version" << endl;
        if (value) {
            *value *= 3;
        }
    }

    // Const reference version
    void display(const int& value) {
        cout << "Const reference: " << value << endl;
    }

    // Const pointer version
    void display(const int* value) {
        if (value) {
            cout << "Const pointer: " << *value << endl;
        }
    }
};

int main() {
    Modifier mod;

    int x = 10;
    cout << "Initial x: " << x << endl;

    mod.modify(x);      // Calls reference version
    cout << "After reference modify: " << x << endl;

    mod.modify(&x);     // Calls pointer version
    cout << "After pointer modify: " << x << endl;

    cout << endl;

    mod.display(x);     // Calls const reference version
    mod.display(&x);    // Calls const pointer version

    return 0;
}
```

## 6. Advanced Overloading: Template and Regular Function

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Template version (generic)
template<typename T>
T maximum(T a, T b) {
    cout << "Template version" << endl;
    return (a > b) ? a : b;
}

// Specialized version for pointers (C-strings)
const char* maximum(const char* a, const char* b) {
    cout << "C-string specialized version" << endl;
    return (strcmp(a, b) > 0) ? a : b;
}

// Specialized version for string objects
string maximum(const string& a, const string& b) {
    cout << "String specialized version" << endl;
    return (a > b) ? a : b;
}

// Specialized for vectors (compare by size)
template<typename T>
vector<T> maximum(const vector<T>& a, const vector<T>& b) {
    cout << "Vector specialized version" << endl;
    return (a.size() > b.size()) ? a : b;
}

int main() {
    cout << "Max of 10 and 20: " << maximum(10, 20) << endl;
    cout << endl;

    cout << "Max of 3.14 and 2.71: " << maximum(3.14, 2.71) << endl;
    cout << endl;

    cout << "Max of 'apple' and 'banana': " << maximum("apple", "banana") << endl;
    cout << endl;

    string s1 = "hello", s2 = "world";
    cout << "Max of strings: " << maximum(s1, s2) << endl;
    cout << endl;

    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {1, 2, 3, 4, 5};
    auto result = maximum(v1, v2);
    cout << "Larger vector has " << result.size() << " elements" << endl;

    return 0;
}
```

## 7. Comprehensive Overloading Example

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class Calculator {
public:
    // Basic arithmetic overloads
    int calculate(int a, int b, char op) {
        cout << "Integer calculation" << endl;
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return (b != 0) ? a / b : 0;
            default: return 0;
        }
    }

    double calculate(double a, double b, char op) {
        cout << "Double calculation" << endl;
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return (b != 0.0) ? a / b : 0.0;
            case '^': return pow(a, b);
            default: return 0.0;
        }
    }

    // Vector operations
    vector<int> calculate(const vector<int>& v1, const vector<int>& v2, char op) {
        cout << "Vector calculation" << endl;
        vector<int> result;
        size_t size = min(v1.size(), v2.size());

        for (size_t i = 0; i < size; i++) {
            switch(op) {
                case '+': result.push_back(v1[i] + v2[i]); break;
                case '-': result.push_back(v1[i] - v2[i]); break;
                case '*': result.push_back(v1[i] * v2[i]); break;
                default: result.push_back(0);
            }
        }
        return result;
    }

    // String operations
    string calculate(const string& s1, const string& s2, char op) {
        cout << "String calculation" << endl;
        switch(op) {
            case '+': return s1 + s2;
            case '-': {
                string result = s1;
                size_t pos = result.find(s2);
                if (pos != string::npos) {
                    result.erase(pos, s2.length());
                }
                return result;
            }
            case '*': {
                string result;
                for (char c1 : s1) {
                    for (char c2 : s2) {
                        result += c1;
                        result += c2;
                    }
                }
                return result;
            }
            default: return "";
        }
    }

    // Statistical operations
    double calculate(const vector<double>& values, const string& operation) {
        cout << "Statistical calculation: " << operation << endl;
        if (values.empty()) return 0.0;

        if (operation == "sum") {
            double sum = 0;
            for (double v : values) sum += v;
            return sum;
        }
        else if (operation == "mean") {
            double sum = 0;
            for (double v : values) sum += v;
            return sum / values.size();
        }
        else if (operation == "max") {
            double max = values[0];
            for (double v : values) {
                if (v > max) max = v;
            }
            return max;
        }
        else if (operation == "min") {
            double min = values[0];
            for (double v : values) {
                if (v < min) min = v;
            }
            return min;
        }
        return 0.0;
    }
};

void printVector(const vector<int>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    Calculator calc;

    // Integer operations
    cout << "10 + 5 = " << calc.calculate(10, 5, '+') << endl;
    cout << "10 * 5 = " << calc.calculate(10, 5, '*') << endl;
    cout << endl;

    // Double operations
    cout << "10.5 + 5.3 = " << calc.calculate(10.5, 5.3, '+') << endl;
    cout << "2.0 ^ 8.0 = " << calc.calculate(2.0, 8.0, '^') << endl;
    cout << endl;

    // Vector operations
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = {10, 20, 30, 40, 50};
    cout << "Vector addition: ";
    printVector(calc.calculate(v1, v2, '+'));
    cout << endl;

    // String operations
    cout << "String concat: " << calc.calculate(string("Hello "), string("World"), '+') << endl;
    cout << "String subtract: " << calc.calculate(string("Hello World"), string(" World"), '-') << endl;
    cout << endl;

    // Statistical operations
    vector<double> data = {10.5, 20.3, 15.7, 30.2, 25.8};
    cout << "Sum: " << calc.calculate(data, "sum") << endl;
    cout << "Mean: " << calc.calculate(data, "mean") << endl;
    cout << "Max: " << calc.calculate(data, "max") << endl;
    cout << "Min: " << calc.calculate(data, "min") << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Shape Area Calculator
Create an overloaded `calculateArea` function that can calculate areas for:
- Circle (radius)
- Rectangle (width, height)
- Triangle (base, height)
- Square (side)

### Exercise 2: Smart Print Function
Create an overloaded `print` function that can handle:
- Arrays of different types
- 2D arrays (matrices)
- Vectors
- Custom formatting options (width, precision)

### Exercise 3: Search Function
Implement overloaded `search` functions that can search for:
- An element in an array
- A substring in a string
- An element in a vector
- A key in a map

### Exercise 4: Conversion Functions
Create overloaded `convert` functions for:
- Temperature (Celsius ↔ Fahrenheit ↔ Kelvin)
- Distance (meters ↔ feet ↔ miles)
- Weight (kg ↔ lbs ↔ ounces)

### Exercise 5: Data Validator
Implement overloaded `validate` functions that can validate:
- Email addresses
- Phone numbers
- Credit card numbers
- URLs
- IP addresses

## Exercise Solutions

### Solution 1: Shape Area Calculator

```cpp
#include <iostream>
#include <cmath>
using namespace std;

const double PI = 3.14159265359;

// Circle - single parameter
double calculateArea(double radius) {
    cout << "Calculating circle area" << endl;
    return PI * radius * radius;
}

// Rectangle - two parameters
double calculateArea(double width, double height) {
    cout << "Calculating rectangle area" << endl;
    return width * height;
}

// Triangle - two parameters with tag
struct Triangle {};
double calculateArea(Triangle, double base, double height) {
    cout << "Calculating triangle area" << endl;
    return 0.5 * base * height;
}

// Square - single parameter with tag
struct Square {};
double calculateArea(Square, double side) {
    cout << "Calculating square area" << endl;
    return side * side;
}

// Trapezoid
struct Trapezoid {};
double calculateArea(Trapezoid, double base1, double base2, double height) {
    cout << "Calculating trapezoid area" << endl;
    return 0.5 * (base1 + base2) * height;
}

int main() {
    cout << "Circle (radius=5): " << calculateArea(5.0) << " sq units" << endl;
    cout << endl;

    cout << "Rectangle (4x6): " << calculateArea(4.0, 6.0) << " sq units" << endl;
    cout << endl;

    cout << "Triangle (base=10, height=8): "
         << calculateArea(Triangle{}, 10.0, 8.0) << " sq units" << endl;
    cout << endl;

    cout << "Square (side=7): " << calculateArea(Square{}, 7.0) << " sq units" << endl;
    cout << endl;

    cout << "Trapezoid (bases=5,10, height=6): "
         << calculateArea(Trapezoid{}, 5.0, 10.0, 6.0) << " sq units" << endl;

    return 0;
}
```

### Solution 2: Smart Print Function

```cpp
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// Print array of integers
void print(const int arr[], int size) {
    cout << "Int Array: [";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print array of doubles
void print(const double arr[], int size, int precision = 2) {
    cout << "Double Array: [";
    cout << fixed << setprecision(precision);
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print 2D array (matrix)
void print(const int matrix[][10], int rows, int cols) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
}

// Print vector
template<typename T>
void print(const vector<T>& vec, const string& name = "Vector") {
    cout << name << ": [";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print 2D vector
template<typename T>
void print(const vector<vector<T>>& matrix, const string& name = "Matrix") {
    cout << name << ":" << endl;
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            cout << setw(5) << val;
        }
        cout << endl;
    }
}

// Print with width specification
void print(int value, int width) {
    cout << "Formatted: " << setw(width) << value << endl;
}

// Print double with width and precision
void print(double value, int width, int precision) {
    cout << "Formatted: " << fixed << setw(width) << setprecision(precision) << value << endl;
}

int main() {
    int intArr[] = {1, 2, 3, 4, 5};
    print(intArr, 5);

    double doubleArr[] = {1.234, 2.567, 3.890};
    print(doubleArr, 3, 3);

    int matrix[3][10] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15}
    };
    print(matrix, 3, 5);

    vector<int> vec = {10, 20, 30, 40, 50};
    print(vec, "MyVector");

    vector<vector<int>> matrix2D = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    print(matrix2D, "MyMatrix");

    print(42, 10);
    print(3.14159, 12, 4);

    return 0;
}
```

### Solution 3: Search Function

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// Search in integer array
int search(const int arr[], int size, int target) {
    cout << "Searching in int array for " << target << endl;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

// Search in double array
int search(const double arr[], int size, double target) {
    cout << "Searching in double array for " << target << endl;
    for (int i = 0; i < size; i++) {
        if (abs(arr[i] - target) < 0.0001) return i;
    }
    return -1;
}

// Search substring in string
int search(const string& str, const string& substring) {
    cout << "Searching for \"" << substring << "\" in string" << endl;
    size_t pos = str.find(substring);
    return (pos != string::npos) ? pos : -1;
}

// Search in vector
template<typename T>
int search(const vector<T>& vec, const T& target) {
    cout << "Searching in vector" << endl;
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == target) return i;
    }
    return -1;
}

// Search in map
template<typename K, typename V>
bool search(const map<K, V>& m, const K& key, V& value) {
    cout << "Searching in map for key" << endl;
    auto it = m.find(key);
    if (it != m.end()) {
        value = it->second;
        return true;
    }
    return false;
}

// Search with custom predicate
template<typename T, typename Predicate>
int search(const vector<T>& vec, Predicate pred) {
    cout << "Searching with custom predicate" << endl;
    for (size_t i = 0; i < vec.size(); i++) {
        if (pred(vec[i])) return i;
    }
    return -1;
}

int main() {
    // Array search
    int intArr[] = {10, 20, 30, 40, 50};
    int pos1 = search(intArr, 5, 30);
    cout << "Found at index: " << pos1 << endl << endl;

    // Double array search
    double doubleArr[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    int pos2 = search(doubleArr, 5, 3.3);
    cout << "Found at index: " << pos2 << endl << endl;

    // String search
    string text = "Hello World! This is C++ programming.";
    int pos3 = search(text, "C++");
    cout << "Found at position: " << pos3 << endl << endl;

    // Vector search
    vector<int> vec = {100, 200, 300, 400, 500};
    int pos4 = search(vec, 300);
    cout << "Found at index: " << pos4 << endl << endl;

    // Map search
    map<string, int> ages = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}};
    int age;
    if (search(ages, string("Bob"), age)) {
        cout << "Bob's age: " << age << endl;
    }
    cout << endl;

    // Custom predicate search
    vector<int> numbers = {1, 5, 10, 15, 20, 25};
    int pos5 = search(numbers, [](int x) { return x > 15; });
    cout << "First element > 15 at index: " << pos5 << endl;

    return 0;
}
```

### Solution 4: Conversion Functions

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

// Temperature conversions
double convert(double celsius, const string& from, const string& to) {
    cout << "Converting temperature from " << from << " to " << to << endl;

    // First convert to Celsius if needed
    double celsiusValue = celsius;
    if (from == "Fahrenheit" || from == "F") {
        celsiusValue = (celsius - 32.0) * 5.0 / 9.0;
    } else if (from == "Kelvin" || from == "K") {
        celsiusValue = celsius - 273.15;
    }

    // Then convert to target
    if (to == "Celsius" || to == "C") {
        return celsiusValue;
    } else if (to == "Fahrenheit" || to == "F") {
        return celsiusValue * 9.0 / 5.0 + 32.0;
    } else if (to == "Kelvin" || to == "K") {
        return celsiusValue + 273.15;
    }

    return celsius;
}

// Distance conversions
struct Distance {};
double convert(Distance, double value, const string& from, const string& to) {
    cout << "Converting distance from " << from << " to " << to << endl;

    // Convert to meters first
    double meters = value;
    if (from == "feet" || from == "ft") {
        meters = value * 0.3048;
    } else if (from == "miles" || from == "mi") {
        meters = value * 1609.34;
    } else if (from == "kilometers" || from == "km") {
        meters = value * 1000.0;
    }

    // Convert to target
    if (to == "meters" || to == "m") {
        return meters;
    } else if (to == "feet" || to == "ft") {
        return meters / 0.3048;
    } else if (to == "miles" || to == "mi") {
        return meters / 1609.34;
    } else if (to == "kilometers" || to == "km") {
        return meters / 1000.0;
    }

    return value;
}

// Weight conversions
struct Weight {};
double convert(Weight, double value, const string& from, const string& to) {
    cout << "Converting weight from " << from << " to " << to << endl;

    // Convert to kg first
    double kg = value;
    if (from == "lbs" || from == "pounds") {
        kg = value * 0.453592;
    } else if (from == "ounces" || from == "oz") {
        kg = value * 0.0283495;
    } else if (from == "grams" || from == "g") {
        kg = value / 1000.0;
    }

    // Convert to target
    if (to == "kg" || to == "kilograms") {
        return kg;
    } else if (to == "lbs" || to == "pounds") {
        return kg / 0.453592;
    } else if (to == "ounces" || to == "oz") {
        return kg / 0.0283495;
    } else if (to == "grams" || to == "g") {
        return kg * 1000.0;
    }

    return value;
}

int main() {
    cout << fixed << setprecision(2);

    // Temperature conversions
    cout << "100°C = " << convert(100, "C", "F") << "°F" << endl;
    cout << "32°F = " << convert(32, "F", "C") << "°C" << endl;
    cout << "273.15K = " << convert(273.15, "K", "C") << "°C" << endl;
    cout << endl;

    // Distance conversions
    cout << "1000 meters = " << convert(Distance{}, 1000, "m", "ft") << " feet" << endl;
    cout << "5 miles = " << convert(Distance{}, 5, "mi", "km") << " kilometers" << endl;
    cout << "10 km = " << convert(Distance{}, 10, "km", "mi") << " miles" << endl;
    cout << endl;

    // Weight conversions
    cout << "100 kg = " << convert(Weight{}, 100, "kg", "lbs") << " pounds" << endl;
    cout << "150 lbs = " << convert(Weight{}, 150, "lbs", "kg") << " kg" << endl;
    cout << "16 oz = " << convert(Weight{}, 16, "oz", "lbs") << " pounds" << endl;

    return 0;
}
```

### Solution 5: Data Validator

```cpp
#include <iostream>
#include <string>
#include <regex>
using namespace std;

// Validate email
bool validate(const string& email, const string& type = "email") {
    if (type == "email") {
        cout << "Validating email: " << email << endl;
        regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return regex_match(email, pattern);
    }
    return false;
}

// Validate phone number
struct PhoneNumber {};
bool validate(PhoneNumber, const string& phone) {
    cout << "Validating phone: " << phone << endl;
    // Matches: (123) 456-7890, 123-456-7890, 1234567890
    regex pattern(R"((\(\d{3}\)\s?|\d{3}-)?\d{3}-?\d{4})");
    return regex_match(phone, pattern);
}

// Validate credit card
struct CreditCard {};
bool validate(CreditCard, const string& cardNumber) {
    cout << "Validating credit card: " << cardNumber << endl;

    // Remove spaces and dashes
    string cleaned;
    for (char c : cardNumber) {
        if (isdigit(c)) cleaned += c;
    }

    // Check length
    if (cleaned.length() < 13 || cleaned.length() > 19) {
        return false;
    }

    // Luhn algorithm
    int sum = 0;
    bool alternate = false;
    for (int i = cleaned.length() - 1; i >= 0; i--) {
        int digit = cleaned[i] - '0';
        if (alternate) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
        alternate = !alternate;
    }

    return (sum % 10 == 0);
}

// Validate URL
struct URL {};
bool validate(URL, const string& url) {
    cout << "Validating URL: " << url << endl;
    regex pattern(R"((http|https)://[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(/[^\s]*)?)");
    return regex_match(url, pattern);
}

// Validate IP address
struct IPAddress {};
bool validate(IPAddress, const string& ip) {
    cout << "Validating IP: " << ip << endl;
    regex pattern(R"((\d{1,3}\.){3}\d{1,3})");

    if (!regex_match(ip, pattern)) {
        return false;
    }

    // Check each octet is in range 0-255
    size_t start = 0, end = 0;
    for (int i = 0; i < 4; i++) {
        end = ip.find('.', start);
        if (end == string::npos) end = ip.length();

        int octet = stoi(ip.substr(start, end - start));
        if (octet < 0 || octet > 255) return false;

        start = end + 1;
    }

    return true;
}

int main() {
    // Email validation
    cout << "john@example.com: " << (validate("john@example.com") ? "Valid" : "Invalid") << endl;
    cout << "invalid.email: " << (validate("invalid.email") ? "Valid" : "Invalid") << endl;
    cout << endl;

    // Phone validation
    cout << "(123) 456-7890: " << (validate(PhoneNumber{}, "(123) 456-7890") ? "Valid" : "Invalid") << endl;
    cout << "123-456-7890: " << (validate(PhoneNumber{}, "123-456-7890") ? "Valid" : "Invalid") << endl;
    cout << "12345: " << (validate(PhoneNumber{}, "12345") ? "Valid" : "Invalid") << endl;
    cout << endl;

    // Credit card validation
    cout << "4532015112830366: " << (validate(CreditCard{}, "4532015112830366") ? "Valid" : "Invalid") << endl;
    cout << "1234567890123456: " << (validate(CreditCard{}, "1234567890123456") ? "Valid" : "Invalid") << endl;
    cout << endl;

    // URL validation
    cout << "https://www.example.com: " << (validate(URL{}, "https://www.example.com") ? "Valid" : "Invalid") << endl;
    cout << "http://test.org/path: " << (validate(URL{}, "http://test.org/path") ? "Valid" : "Invalid") << endl;
    cout << "not-a-url: " << (validate(URL{}, "not-a-url") ? "Valid" : "Invalid") << endl;
    cout << endl;

    // IP address validation
    cout << "192.168.1.1: " << (validate(IPAddress{}, "192.168.1.1") ? "Valid" : "Invalid") << endl;
    cout << "256.256.256.256: " << (validate(IPAddress{}, "256.256.256.256") ? "Valid" : "Invalid") << endl;
    cout << "192.168.1: " << (validate(IPAddress{}, "192.168.1") ? "Valid" : "Invalid") << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- How to overload functions based on parameter types, count, and qualifiers
- The importance of function signatures in overload resolution
- How to combine overloading with templates and const-correctness
- Best practices for designing overloaded function APIs
- Common pitfalls and how to avoid them

## Checklist

- [ ] I understand how function overloading works
- [ ] I can create overloaded functions with different parameter types
- [ ] I can overload functions based on parameter count
- [ ] I understand const-correctness in function overloading
- [ ] I can combine overloading with templates
- [ ] I can design clean, intuitive APIs using overloading
- [ ] I completed all exercises and understand their solutions
- [ ] I know when to use overloading vs templates

## Next Steps
In the next lesson, we'll explore Default Arguments, learning how to provide default values for function parameters to create flexible and user-friendly APIs.
