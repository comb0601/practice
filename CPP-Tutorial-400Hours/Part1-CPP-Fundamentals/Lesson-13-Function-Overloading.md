# Lesson 13: Function Overloading

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Function Overloading
2. Overloading by Number of Parameters
3. Overloading by Parameter Types
4. Overloading with Different Signatures
5. Overloading Resolution Rules
6. Common Overloading Patterns
7. Ambiguity and How to Avoid It
8. Exercises and Projects

---

## 1. Introduction to Function Overloading

### What is Function Overloading?

Function overloading allows you to have multiple functions with the same name but different parameters. The compiler determines which function to call based on the arguments provided.

```cpp
#include <iostream>

// Same function name, different parameters
void print(int x)
{
    std::cout << "Integer: " << x << "\n";
}

void print(double x)
{
    std::cout << "Double: " << x << "\n";
}

void print(const std::string& x)
{
    std::cout << "String: " << x << "\n";
}

int main()
{
    print(42);              // Calls print(int)
    print(3.14);            // Calls print(double)
    print("Hello");         // Calls print(string)

    return 0;
}
```

**Output**:
```
Integer: 42
Double: 3.14
String: Hello
```

### Why Use Function Overloading?

```cpp
#include <iostream>

// Without overloading - need different names
int addInt(int a, int b) { return a + b; }
double addDouble(double a, double b) { return a + b; }
std::string concatString(std::string a, std::string b) { return a + b; }

// With overloading - same name, cleaner interface
int add(int a, int b)
{
    return a + b;
}

double add(double a, double b)
{
    return a + b;
}

std::string add(std::string a, std::string b)
{
    return a + b;
}

int main()
{
    std::cout << "Integer: " << add(5, 3) << "\n";
    std::cout << "Double: " << add(2.5, 3.7) << "\n";
    std::cout << "String: " << add(std::string("Hello "), std::string("World")) << "\n";

    return 0;
}
```

---

## 2. Overloading by Number of Parameters

### Different Parameter Counts:

```cpp
#include <iostream>

int sum(int a)
{
    return a;
}

int sum(int a, int b)
{
    return a + b;
}

int sum(int a, int b, int c)
{
    return a + b + c;
}

int sum(int a, int b, int c, int d)
{
    return a + b + c + d;
}

int main()
{
    std::cout << "sum(5) = " << sum(5) << "\n";
    std::cout << "sum(5, 3) = " << sum(5, 3) << "\n";
    std::cout << "sum(5, 3, 2) = " << sum(5, 3, 2) << "\n";
    std::cout << "sum(5, 3, 2, 1) = " << sum(5, 3, 2, 1) << "\n";

    return 0;
}
```

### Practical Example - Drawing Functions:

```cpp
#include <iostream>
#include <string>

void drawLine()
{
    std::cout << "----------\n";
}

void drawLine(int length)
{
    std::cout << std::string(length, '-') << "\n";
}

void drawLine(int length, char symbol)
{
    std::cout << std::string(length, symbol) << "\n";
}

void drawLine(int length, char symbol, bool newline)
{
    std::cout << std::string(length, symbol);
    if (newline)
        std::cout << "\n";
}

int main()
{
    drawLine();                     // Default 10 dashes
    drawLine(20);                   // 20 dashes
    drawLine(15, '=');              // 15 equal signs
    drawLine(10, '*', false);       // 10 asterisks, no newline
    std::cout << " <- No newline above\n";

    return 0;
}
```

---

## 3. Overloading by Parameter Types

### Different Type Signatures:

```cpp
#include <iostream>
#include <string>
#include <cmath>

double absolute(double x)
{
    return std::abs(x);
}

int absolute(int x)
{
    return std::abs(x);
}

long absolute(long x)
{
    return std::abs(x);
}

int main()
{
    std::cout << "abs(-5) = " << absolute(-5) << "\n";
    std::cout << "abs(-5.7) = " << absolute(-5.7) << "\n";
    std::cout << "abs(-1000000L) = " << absolute(-1000000L) << "\n";

    return 0;
}
```

### Mixed Type Overloading:

```cpp
#include <iostream>
#include <string>
#include <vector>

// Convert to string
std::string toString(int value)
{
    return std::to_string(value);
}

std::string toString(double value)
{
    return std::to_string(value);
}

std::string toString(bool value)
{
    return value ? "true" : "false";
}

std::string toString(const std::vector<int>& vec)
{
    std::string result = "[";
    for (size_t i = 0; i < vec.size(); i++)
    {
        result += std::to_string(vec[i]);
        if (i < vec.size() - 1)
            result += ", ";
    }
    result += "]";
    return result;
}

int main()
{
    std::cout << toString(42) << "\n";
    std::cout << toString(3.14159) << "\n";
    std::cout << toString(true) << "\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << toString(numbers) << "\n";

    return 0;
}
```

---

## 4. Overloading with Different Signatures

### Combining Count and Type:

```cpp
#include <iostream>
#include <string>

class Rectangle
{
public:
    double width;
    double height;

    // Constructor overloading
    Rectangle()
    {
        width = 1.0;
        height = 1.0;
        std::cout << "Default rectangle: " << width << "x" << height << "\n";
    }

    Rectangle(double size)
    {
        width = height = size;
        std::cout << "Square: " << width << "x" << height << "\n";
    }

    Rectangle(double w, double h)
    {
        width = w;
        height = h;
        std::cout << "Rectangle: " << width << "x" << height << "\n";
    }

    double area() const
    {
        return width * height;
    }
};

int main()
{
    Rectangle r1;           // Default constructor
    Rectangle r2(5.0);      // Square constructor
    Rectangle r3(4.0, 6.0); // Rectangle constructor

    std::cout << "Areas: " << r1.area() << ", "
              << r2.area() << ", " << r3.area() << "\n";

    return 0;
}
```

### Array Processing Overloads:

```cpp
#include <iostream>
#include <vector>

// Process integer array
void process(int arr[], int size)
{
    std::cout << "Processing int array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] * 2 << " ";
    }
    std::cout << "\n";
}

// Process double array
void process(double arr[], int size)
{
    std::cout << "Processing double array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] / 2.0 << " ";
    }
    std::cout << "\n";
}

// Process vector
void process(const std::vector<int>& vec)
{
    std::cout << "Processing vector: ";
    for (int val : vec)
    {
        std::cout << val + 10 << " ";
    }
    std::cout << "\n";
}

int main()
{
    int intArr[] = {2, 4, 6, 8};
    double doubleArr[] = {10.0, 20.0, 30.0, 40.0};
    std::vector<int> vec = {1, 2, 3, 4, 5};

    process(intArr, 4);
    process(doubleArr, 4);
    process(vec);

    return 0;
}
```

---

## 5. Overloading Resolution Rules

### How the Compiler Chooses:

The compiler follows these steps to resolve overloaded functions:

1. Exact match
2. Promotion (char to int, float to double)
3. Standard conversion (int to double)
4. User-defined conversion
5. Ellipsis match

```cpp
#include <iostream>

void func(int x)
{
    std::cout << "func(int): " << x << "\n";
}

void func(double x)
{
    std::cout << "func(double): " << x << "\n";
}

void func(char x)
{
    std::cout << "func(char): " << x << "\n";
}

int main()
{
    func(42);          // Exact match: int
    func(3.14);        // Exact match: double
    func('A');         // Exact match: char

    func(42L);         // Standard conversion: long to int
    func(3.14f);       // Promotion: float to double

    short s = 10;
    func(s);           // Promotion: short to int

    return 0;
}
```

### Const Overloading:

```cpp
#include <iostream>
#include <vector>

class DataContainer
{
private:
    std::vector<int> data;

public:
    DataContainer()
    {
        data = {1, 2, 3, 4, 5};
    }

    // Non-const version - allows modification
    int& at(size_t index)
    {
        std::cout << "Non-const at() called\n";
        return data[index];
    }

    // Const version - read-only
    const int& at(size_t index) const
    {
        std::cout << "Const at() called\n";
        return data[index];
    }
};

int main()
{
    DataContainer container;
    const DataContainer constContainer;

    // Calls non-const version
    container.at(0) = 100;

    // Calls const version
    int value = constContainer.at(0);

    std::cout << "Value: " << value << "\n";

    return 0;
}
```

---

## 6. Common Overloading Patterns

### Pattern 1: Flexible Initialization

```cpp
#include <iostream>
#include <string>
#include <ctime>

class Logger
{
private:
    std::string prefix;
    bool timestamp;

public:
    // Default
    Logger() : prefix("LOG"), timestamp(false) {}

    // With prefix
    Logger(const std::string& p) : prefix(p), timestamp(false) {}

    // With prefix and timestamp
    Logger(const std::string& p, bool ts) : prefix(p), timestamp(ts) {}

    void log(const std::string& message)
    {
        if (timestamp)
        {
            time_t now = time(0);
            std::cout << "[" << now << "] ";
        }
        std::cout << prefix << ": " << message << "\n";
    }
};

int main()
{
    Logger logger1;
    Logger logger2("INFO");
    Logger logger3("DEBUG", true);

    logger1.log("Default logger");
    logger2.log("Info logger");
    logger3.log("Debug logger with timestamp");

    return 0;
}
```

### Pattern 2: Type Conversion

```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// Convert various types to formatted strings
std::string format(int value)
{
    return std::to_string(value);
}

std::string format(double value, int precision = 2)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string format(bool value)
{
    return value ? "true" : "false";
}

std::string format(const char* value)
{
    return std::string(value);
}

int main()
{
    std::cout << "Integer: " << format(42) << "\n";
    std::cout << "Double: " << format(3.14159) << "\n";
    std::cout << "Double (4 decimals): " << format(3.14159, 4) << "\n";
    std::cout << "Boolean: " << format(true) << "\n";
    std::cout << "C-String: " << format("Hello") << "\n";

    return 0;
}
```

### Pattern 3: Flexible Search

```cpp
#include <iostream>
#include <vector>
#include <string>

class Database
{
private:
    struct Record
    {
        int id;
        std::string name;
        int age;
    };

    std::vector<Record> records;

public:
    Database()
    {
        records = {
            {1, "Alice", 25},
            {2, "Bob", 30},
            {3, "Charlie", 25}
        };
    }

    // Find by ID
    void find(int id)
    {
        for (const auto& rec : records)
        {
            if (rec.id == id)
            {
                std::cout << "Found by ID: " << rec.name << "\n";
                return;
            }
        }
        std::cout << "Not found\n";
    }

    // Find by name
    void find(const std::string& name)
    {
        for (const auto& rec : records)
        {
            if (rec.name == name)
            {
                std::cout << "Found by name: ID=" << rec.id << "\n";
                return;
            }
        }
        std::cout << "Not found\n";
    }

    // Find by age range
    void find(int minAge, int maxAge)
    {
        std::cout << "Records in age range " << minAge << "-" << maxAge << ":\n";
        for (const auto& rec : records)
        {
            if (rec.age >= minAge && rec.age <= maxAge)
            {
                std::cout << "  " << rec.name << " (age " << rec.age << ")\n";
            }
        }
    }
};

int main()
{
    Database db;

    db.find(2);                    // Find by ID
    db.find("Charlie");            // Find by name
    db.find(20, 30);               // Find by age range

    return 0;
}
```

---

## 7. Ambiguity and How to Avoid It

### Ambiguous Calls:

```cpp
#include <iostream>

// ❌ AMBIGUOUS: Compiler can't decide
void func(int x, double y)
{
    std::cout << "func(int, double)\n";
}

void func(double x, int y)
{
    std::cout << "func(double, int)\n";
}

int main()
{
    func(1, 2.0);      // OK: exact match to func(int, double)
    func(1.0, 2);      // OK: exact match to func(double, int)
    // func(1, 2);     // ERROR: Ambiguous! Could match either

    return 0;
}
```

### Avoiding Ambiguity with Default Parameters:

```cpp
#include <iostream>

// ❌ PROBLEMATIC: Ambiguous with default parameter
void print(int x)
{
    std::cout << "One parameter: " << x << "\n";
}

void print(int x, int y = 0)
{
    std::cout << "Two parameters: " << x << ", " << y << "\n";
}

// Solution 1: Use different names
void printSingle(int x)
{
    std::cout << "Single: " << x << "\n";
}

void printPair(int x, int y = 0)
{
    std::cout << "Pair: " << x << ", " << y << "\n";
}

// Solution 2: Make parameters more distinct
void display(int x)
{
    std::cout << "display(int): " << x << "\n";
}

void display(int x, double y)
{
    std::cout << "display(int, double): " << x << ", " << y << "\n";
}

int main()
{
    // print(5);          // ERROR: Ambiguous

    // Using solutions
    printSingle(5);
    printPair(5);
    printPair(5, 10);

    display(5);
    display(5, 3.14);

    return 0;
}
```

### Clear Overload Design:

```cpp
#include <iostream>
#include <string>
#include <vector>

class Calculator
{
public:
    // Clear, distinct overloads
    int add(int a, int b)
    {
        return a + b;
    }

    double add(double a, double b)
    {
        return a + b;
    }

    std::string add(const std::string& a, const std::string& b)
    {
        return a + b;
    }

    int add(const std::vector<int>& numbers)
    {
        int sum = 0;
        for (int n : numbers)
            sum += n;
        return sum;
    }
};

int main()
{
    Calculator calc;

    std::cout << "5 + 3 = " << calc.add(5, 3) << "\n";
    std::cout << "2.5 + 3.7 = " << calc.add(2.5, 3.7) << "\n";
    std::cout << "Hello + World = " << calc.add(std::string("Hello "), std::string("World")) << "\n";

    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::cout << "Sum of vector = " << calc.add(nums) << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Overloaded Math Functions

```cpp
#include <iostream>
#include <cmath>

// Power functions
int power(int base, int exp)
{
    int result = 1;
    for (int i = 0; i < exp; i++)
    {
        result *= base;
    }
    return result;
}

double power(double base, int exp)
{
    return std::pow(base, exp);
}

double power(double base, double exp)
{
    return std::pow(base, exp);
}

// Max functions
int max(int a, int b)
{
    return (a > b) ? a : b;
}

double max(double a, double b)
{
    return (a > b) ? a : b;
}

int max(int a, int b, int c)
{
    return max(max(a, b), c);
}

double max(double a, double b, double c)
{
    return max(max(a, b), c);
}

// Average functions
double average(int a, int b)
{
    return (a + b) / 2.0;
}

double average(int a, int b, int c)
{
    return (a + b + c) / 3.0;
}

double average(const std::vector<int>& numbers)
{
    if (numbers.empty())
        return 0.0;

    int sum = 0;
    for (int n : numbers)
        sum += n;

    return static_cast<double>(sum) / numbers.size();
}

int main()
{
    std::cout << "=== Power Tests ===\n";
    std::cout << "2^3 = " << power(2, 3) << "\n";
    std::cout << "2.5^2 = " << power(2.5, 2) << "\n";
    std::cout << "2.0^3.5 = " << power(2.0, 3.5) << "\n\n";

    std::cout << "=== Max Tests ===\n";
    std::cout << "max(5, 3) = " << max(5, 3) << "\n";
    std::cout << "max(2.7, 8.3) = " << max(2.7, 8.3) << "\n";
    std::cout << "max(5, 3, 7) = " << max(5, 3, 7) << "\n\n";

    std::cout << "=== Average Tests ===\n";
    std::cout << "avg(10, 20) = " << average(10, 20) << "\n";
    std::cout << "avg(10, 20, 30) = " << average(10, 20, 30) << "\n";

    std::vector<int> nums = {5, 10, 15, 20, 25};
    std::cout << "avg(vector) = " << average(nums) << "\n";

    return 0;
}
```

### Exercise 2: Shape Area Calculator

```cpp
#include <iostream>
#include <cmath>

const double PI = 3.14159265359;

// Circle
double area(double radius)
{
    return PI * radius * radius;
}

// Rectangle
double area(double width, double height)
{
    return width * height;
}

// Triangle
double area(double base, double height, bool isTriangle)
{
    if (isTriangle)
        return 0.5 * base * height;
    return 0.0;
}

// Triangle using Heron's formula
double area(double a, double b, double c)
{
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

void displayArea(const std::string& shape, double value)
{
    std::cout << shape << " area: " << value << "\n";
}

int main()
{
    std::cout << "=== AREA CALCULATOR ===\n\n";

    // Circle
    double circleArea = area(5.0);
    displayArea("Circle (r=5)", circleArea);

    // Rectangle
    double rectArea = area(4.0, 6.0);
    displayArea("Rectangle (4x6)", rectArea);

    // Triangle (base-height)
    double triArea1 = area(10.0, 5.0, true);
    displayArea("Triangle (b=10, h=5)", triArea1);

    // Triangle (three sides)
    double triArea2 = area(3.0, 4.0, 5.0);
    displayArea("Triangle (sides 3,4,5)", triArea2);

    return 0;
}
```

### Exercise 3: Flexible Display Functions

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Display single value
void display(int value)
{
    std::cout << "Integer: " << value << "\n";
}

void display(double value)
{
    std::cout << "Double: " << std::fixed << std::setprecision(2) << value << "\n";
}

void display(const std::string& value)
{
    std::cout << "String: " << value << "\n";
}

void display(bool value)
{
    std::cout << "Boolean: " << (value ? "true" : "false") << "\n";
}

// Display with label
void display(const std::string& label, int value)
{
    std::cout << label << ": " << value << "\n";
}

void display(const std::string& label, double value)
{
    std::cout << label << ": " << std::fixed << std::setprecision(2) << value << "\n";
}

// Display array
void display(const int arr[], int size)
{
    std::cout << "Array: [";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i];
        if (i < size - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

// Display vector
void display(const std::vector<int>& vec)
{
    std::cout << "Vector: [";
    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i];
        if (i < vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

// Display table header
void display(const std::vector<std::string>& headers)
{
    for (const auto& header : headers)
    {
        std::cout << std::left << std::setw(15) << header;
    }
    std::cout << "\n" << std::string(headers.size() * 15, '-') << "\n";
}

int main()
{
    display(42);
    display(3.14159);
    display("Hello, World!");
    display(true);

    std::cout << "\n";

    display("Age", 25);
    display("Price", 19.99);

    std::cout << "\n";

    int arr[] = {1, 2, 3, 4, 5};
    display(arr, 5);

    std::vector<int> vec = {10, 20, 30, 40, 50};
    display(vec);

    std::cout << "\n";

    std::vector<std::string> headers = {"Name", "Age", "Score"};
    display(headers);

    return 0;
}
```

---

## Practice Project: Multi-Format Data Converter

```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

class Converter
{
public:
    // Temperature conversions
    double convert(double celsius, char toUnit)
    {
        switch (toUnit)
        {
            case 'F':  // Celsius to Fahrenheit
                return (celsius * 9.0 / 5.0) + 32.0;
            case 'K':  // Celsius to Kelvin
                return celsius + 273.15;
            default:
                return celsius;
        }
    }

    double convert(double value, char fromUnit, char toUnit)
    {
        // Temperature conversion with from/to units
        double celsius;

        // First convert to Celsius
        switch (fromUnit)
        {
            case 'C':
                celsius = value;
                break;
            case 'F':
                celsius = (value - 32.0) * 5.0 / 9.0;
                break;
            case 'K':
                celsius = value - 273.15;
                break;
            default:
                return value;
        }

        // Then convert to target unit
        return convert(celsius, toUnit);
    }

    // Distance conversions
    double convert(double meters, const std::string& toUnit)
    {
        if (toUnit == "km")
            return meters / 1000.0;
        else if (toUnit == "cm")
            return meters * 100.0;
        else if (toUnit == "mm")
            return meters * 1000.0;
        else if (toUnit == "miles")
            return meters / 1609.34;
        else if (toUnit == "feet")
            return meters * 3.28084;
        else if (toUnit == "inches")
            return meters * 39.3701;
        else
            return meters;
    }

    // Number base conversions
    std::string convert(int decimal, int toBase)
    {
        if (toBase < 2 || toBase > 16)
            return "Invalid base";

        if (decimal == 0)
            return "0";

        std::string result = "";
        const char digits[] = "0123456789ABCDEF";
        bool negative = decimal < 0;

        decimal = std::abs(decimal);

        while (decimal > 0)
        {
            result = digits[decimal % toBase] + result;
            decimal /= toBase;
        }

        if (negative)
            result = "-" + result;

        return result;
    }

    // String format conversions
    std::string convert(const std::string& str, const std::string& format)
    {
        std::string result = str;

        if (format == "upper")
        {
            for (char& c : result)
                c = std::toupper(c);
        }
        else if (format == "lower")
        {
            for (char& c : result)
                c = std::tolower(c);
        }
        else if (format == "title")
        {
            bool newWord = true;
            for (char& c : result)
            {
                if (std::isspace(c))
                {
                    newWord = true;
                }
                else if (newWord)
                {
                    c = std::toupper(c);
                    newWord = false;
                }
                else
                {
                    c = std::tolower(c);
                }
            }
        }

        return result;
    }

    // Vector sum conversion
    double convert(const std::vector<int>& vec)
    {
        double sum = 0.0;
        for (int val : vec)
            sum += val;
        return sum;
    }

    // Vector average conversion
    double convert(const std::vector<double>& vec)
    {
        if (vec.empty())
            return 0.0;

        double sum = 0.0;
        for (double val : vec)
            sum += val;
        return sum / vec.size();
    }
};

void displayMenu()
{
    std::cout << "\n===== MULTI-FORMAT CONVERTER =====\n";
    std::cout << "1. Temperature Conversion\n";
    std::cout << "2. Distance Conversion\n";
    std::cout << "3. Number Base Conversion\n";
    std::cout << "4. String Format Conversion\n";
    std::cout << "5. Vector Operations\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main()
{
    Converter converter;
    int choice;

    do
    {
        displayMenu();
        std::cin >> choice;

        switch (choice)
        {
            case 1:
            {
                std::cout << "\nTemperature Conversion\n";
                std::cout << "Enter value: ";
                double temp;
                std::cin >> temp;

                std::cout << "From (C/F/K): ";
                char from;
                std::cin >> from;

                std::cout << "To (C/F/K): ";
                char to;
                std::cin >> to;

                double result = converter.convert(temp, from, to);
                std::cout << temp << "°" << from << " = "
                         << std::fixed << std::setprecision(2)
                         << result << "°" << to << "\n";
                break;
            }

            case 2:
            {
                std::cout << "\nDistance Conversion\n";
                std::cout << "Enter meters: ";
                double meters;
                std::cin >> meters;

                std::cout << "To km: " << converter.convert(meters, "km") << "\n";
                std::cout << "To miles: " << converter.convert(meters, "miles") << "\n";
                std::cout << "To feet: " << converter.convert(meters, "feet") << "\n";
                break;
            }

            case 3:
            {
                std::cout << "\nNumber Base Conversion\n";
                std::cout << "Enter decimal number: ";
                int num;
                std::cin >> num;

                std::cout << "Binary (base 2): " << converter.convert(num, 2) << "\n";
                std::cout << "Octal (base 8): " << converter.convert(num, 8) << "\n";
                std::cout << "Hexadecimal (base 16): " << converter.convert(num, 16) << "\n";
                break;
            }

            case 4:
            {
                std::cout << "\nString Format Conversion\n";
                std::cin.ignore();
                std::cout << "Enter text: ";
                std::string text;
                std::getline(std::cin, text);

                std::cout << "Uppercase: " << converter.convert(text, "upper") << "\n";
                std::cout << "Lowercase: " << converter.convert(text, "lower") << "\n";
                std::cout << "Title Case: " << converter.convert(text, "title") << "\n";
                break;
            }

            case 5:
            {
                std::cout << "\nVector Operations\n";
                std::vector<int> intVec = {1, 2, 3, 4, 5};
                std::vector<double> doubleVec = {1.5, 2.5, 3.5, 4.5, 5.5};

                std::cout << "Integer vector sum: " << converter.convert(intVec) << "\n";
                std::cout << "Double vector average: " << converter.convert(doubleVec) << "\n";
                break;
            }

            case 0:
                std::cout << "Goodbye!\n";
                break;

            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Return Type Only
```cpp
// ❌ ERROR: Cannot overload by return type alone
int getValue() { return 42; }
double getValue() { return 3.14; }  // ERROR!

// ✅ CORRECT: Must differ in parameters
int getIntValue() { return 42; }
double getDoubleValue() { return 3.14; }
```

### Mistake 2: Default Parameters Creating Ambiguity
```cpp
// ❌ AMBIGUOUS
void func(int x) { }
void func(int x, int y = 0) { }  // Ambiguous with func(5)

// ✅ BETTER
void func(int x) { }
void func(int x, int y, int z = 0) { }  // Not ambiguous
```

### Mistake 3: Implicit Conversions
```cpp
// Can cause unexpected behavior
void process(int x) { std::cout << "int\n"; }
void process(double x) { std::cout << "double\n"; }

int main() {
    process(5);      // Calls process(int)
    process(5.0);    // Calls process(double)
    process('A');    // Calls process(int) - char promoted to int
}
```

---

## Key Takeaways

1. Function overloading allows multiple functions with same name but different parameters
2. Overloading can be by parameter count, type, or both
3. Return type alone cannot differentiate overloaded functions
4. Compiler uses exact match, promotion, and conversion rules
5. Avoid ambiguous overloads
6. Const can be used to overload member functions
7. Use overloading to create intuitive, flexible interfaces
8. Document overloaded functions clearly

---

## Summary Checklist

Before moving to Lesson 14, ensure you can:
- [ ] Create overloaded functions
- [ ] Overload by parameter count
- [ ] Overload by parameter types
- [ ] Understand overload resolution rules
- [ ] Avoid ambiguous overloads
- [ ] Use const overloading
- [ ] Apply common overloading patterns
- [ ] Debug overloading issues

---

## Next Lesson Preview

In **Lesson 14**, we'll explore:
- Recursion fundamentals
- Base cases and recursive cases
- Recursive vs iterative solutions
- Common recursive algorithms
- Tail recursion and optimization

**Master function overloading!**
