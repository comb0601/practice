# Lesson 15: Inline Functions and Default Arguments

**Duration**: 4 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Inline Functions
2. When to Use Inline
3. Inline vs Macros
4. Default Arguments Deep Dive
5. Default Arguments Best Practices
6. Combining Inline and Defaults
7. Function Templates Introduction
8. Exercises and Projects

---

## 1. Introduction to Inline Functions

### What are Inline Functions?

Inline functions suggest to the compiler to insert the function's code directly at the call site, avoiding function call overhead.

```cpp
#include <iostream>

// Regular function
int add(int a, int b)
{
    return a + b;
}

// Inline function
inline int addInline(int a, int b)
{
    return a + b;
}

int main()
{
    int result1 = add(5, 3);         // Function call overhead
    int result2 = addInline(5, 3);   // Potentially inlined

    std::cout << "Result 1: " << result1 << "\n";
    std::cout << "Result 2: " << result2 << "\n";

    return 0;
}
```

### How Inline Works:

```cpp
#include <iostream>

inline int square(int x)
{
    return x * x;
}

inline int cube(int x)
{
    return x * x * x;
}

int main()
{
    // Without inline, this would be:
    // - Call square function
    // - Push arguments, jump, execute, return
    // - Call cube function
    // - Push arguments, jump, execute, return

    // With inline, compiler might expand to:
    // int a = 5 * 5;
    // int b = 6 * 6 * 6;

    int a = square(5);
    int b = cube(6);

    std::cout << "Square: " << a << "\n";
    std::cout << "Cube: " << b << "\n";

    return 0;
}
```

### Inline Class Member Functions:

```cpp
#include <iostream>

class Rectangle
{
private:
    double width;
    double height;

public:
    // Constructor (automatically inline when defined in class)
    Rectangle(double w, double h) : width(w), height(h) {}

    // Inline member functions
    inline double getWidth() const { return width; }
    inline double getHeight() const { return height; }

    // Functions defined in class body are implicitly inline
    double area() const
    {
        return width * height;
    }

    double perimeter() const
    {
        return 2 * (width + height);
    }
};

int main()
{
    Rectangle rect(5.0, 3.0);

    std::cout << "Width: " << rect.getWidth() << "\n";
    std::cout << "Height: " << rect.getHeight() << "\n";
    std::cout << "Area: " << rect.area() << "\n";
    std::cout << "Perimeter: " << rect.perimeter() << "\n";

    return 0;
}
```

---

## 2. When to Use Inline

### Good Candidates for Inline:

```cpp
#include <iostream>

// ✅ GOOD: Small, frequently called functions
inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

inline int min(int a, int b)
{
    return (a < b) ? a : b;
}

inline bool isEven(int n)
{
    return n % 2 == 0;
}

inline bool isPositive(int n)
{
    return n > 0;
}

// ✅ GOOD: Simple getters/setters
class Point
{
private:
    int x, y;

public:
    inline int getX() const { return x; }
    inline int getY() const { return y; }
    inline void setX(int newX) { x = newX; }
    inline void setY(int newY) { y = newY; }
};

int main()
{
    std::cout << "Max(5, 3): " << max(5, 3) << "\n";
    std::cout << "Min(5, 3): " << min(5, 3) << "\n";
    std::cout << "Is 4 even? " << (isEven(4) ? "Yes" : "No") << "\n";

    return 0;
}
```

### Bad Candidates for Inline:

```cpp
#include <iostream>
#include <vector>

// ❌ BAD: Large function with complex logic
// inline void sortAndProcess(std::vector<int>& vec)
// {
//     // Complex sorting algorithm
//     for (size_t i = 0; i < vec.size(); i++)
//     {
//         for (size_t j = i + 1; j < vec.size(); j++)
//         {
//             if (vec[i] > vec[j])
//             {
//                 std::swap(vec[i], vec[j]);
//             }
//         }
//     }
//     // More processing...
// }

// ❌ BAD: Recursive function
// inline int fibonacci(int n)
// {
//     if (n <= 1)
//         return n;
//     return fibonacci(n - 1) + fibonacci(n - 2);
// }

// ✅ BETTER: Regular functions
void sortAndProcess(std::vector<int>& vec)
{
    // Implementation...
}

int fibonacci(int n)
{
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
    std::cout << "Use regular functions for complex operations\n";
    return 0;
}
```

### Inline Guidelines:

```cpp
#include <iostream>

// Use inline for:
// 1. Small functions (1-3 lines)
// 2. Frequently called functions
// 3. Performance-critical code
// 4. Getters and setters

class PerformanceCritical
{
private:
    double data[1000];
    int size;

public:
    // These are called millions of times
    inline double& operator[](int index) { return data[index]; }
    inline int getSize() const { return size; }

    // This is called rarely
    void initialize();  // Not inline
};

void PerformanceCritical::initialize()
{
    // Complex initialization
    size = 1000;
    for (int i = 0; i < size; i++)
    {
        data[i] = 0.0;
    }
}

int main()
{
    PerformanceCritical obj;
    obj.initialize();

    // These calls benefit from being inline
    for (int i = 0; i < obj.getSize(); i++)
    {
        obj[i] = i * 2.0;
    }

    return 0;
}
```

---

## 3. Inline vs Macros

### Macros (Old C Style):

```cpp
#include <iostream>

// ❌ MACRO: No type safety, no scope
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    int a = 5;

    // Macro expansion can have side effects
    std::cout << "Square: " << SQUARE(a++) << "\n";  // a++ executed twice!
    std::cout << "a is now: " << a << "\n";  // Unexpected value!

    return 0;
}
```

### Inline Functions (Modern C++):

```cpp
#include <iostream>

// ✅ INLINE: Type safe, proper scope
inline int square(int x)
{
    return x * x;
}

inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Overloading works (impossible with macros)
inline double square(double x)
{
    return x * x;
}

inline double max(double a, double b)
{
    return (a > b) ? a : b;
}

int main()
{
    int a = 5;

    std::cout << "Square: " << square(a++) << "\n";  // a++ executed once
    std::cout << "a is now: " << a << "\n";  // Expected value: 6

    std::cout << "Max int: " << max(5, 3) << "\n";
    std::cout << "Max double: " << max(5.5, 3.2) << "\n";

    return 0;
}
```

### Comparison:

```cpp
#include <iostream>

// Macro issues
#define MACRO_MAX(a, b) ((a) > (b) ? (a) : (b))

// Inline advantages
inline int inlineMax(int a, int b)
{
    return (a > b) ? a : b;
}

void demonstrateDifference()
{
    int x = 5, y = 3;

    // Macro: Evaluates arguments multiple times
    int result1 = MACRO_MAX(x++, y++);  // x and y incremented twice!

    x = 5; y = 3;

    // Inline: Arguments evaluated once
    int result2 = inlineMax(x++, y++);  // x and y incremented once

    std::cout << "Type safety, scope, debugging are better with inline\n";
}

int main()
{
    demonstrateDifference();
    return 0;
}
```

---

## 4. Default Arguments Deep Dive

### Basic Default Arguments:

```cpp
#include <iostream>
#include <string>

void printMessage(std::string msg, int times = 1, char end = '\n')
{
    for (int i = 0; i < times; i++)
    {
        std::cout << msg << end;
    }
}

void displayInfo(std::string name,
                int age = 0,
                std::string city = "Unknown",
                std::string country = "Unknown")
{
    std::cout << "Name: " << name << "\n";
    if (age > 0)
        std::cout << "Age: " << age << "\n";
    std::cout << "City: " << city << "\n";
    std::cout << "Country: " << country << "\n\n";
}

int main()
{
    printMessage("Hello");                    // Default: 1 time, '\n'
    printMessage("World", 3);                 // 3 times, '\n'
    printMessage("C++", 2, ' ');              // 2 times, space

    std::cout << "\n";

    displayInfo("Alice");
    displayInfo("Bob", 25);
    displayInfo("Charlie", 30, "New York");
    displayInfo("Diana", 28, "London", "UK");

    return 0;
}
```

### Default Arguments Rules:

```cpp
#include <iostream>

// ✅ CORRECT: Defaults on the right
void func1(int a, int b = 5, int c = 10)
{
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << "\n";
}

// ✅ CORRECT: All have defaults
void func2(int a = 1, int b = 2, int c = 3)
{
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << "\n";
}

// ❌ WRONG: Non-default after default
// void func3(int a = 1, int b, int c = 3)  // ERROR!

// ✅ Declaration and definition
void func4(int a, int b = 10);

int main()
{
    func1(1);           // a=1, b=5, c=10
    func1(1, 2);        // a=1, b=2, c=10
    func1(1, 2, 3);     // a=1, b=2, c=3

    func2();            // a=1, b=2, c=3
    func2(5);           // a=5, b=2, c=3
    func2(5, 6);        // a=5, b=6, c=3
    func2(5, 6, 7);     // a=5, b=6, c=7

    func4(100);         // Uses default b=10
    func4(100, 200);    // Overrides default

    return 0;
}

// Definition (don't repeat defaults)
void func4(int a, int b)
{
    std::cout << "a=" << a << ", b=" << b << "\n";
}
```

### Practical Default Arguments:

```cpp
#include <iostream>
#include <string>
#include <iomanip>

class BankAccount
{
private:
    std::string owner;
    double balance;
    std::string currency;

public:
    // Constructor with defaults
    BankAccount(std::string name, double initial = 0.0, std::string curr = "USD")
        : owner(name), balance(initial), currency(curr)
    {
        std::cout << "Account created for " << owner << "\n";
    }

    // Deposit with default message
    void deposit(double amount, bool showMessage = true)
    {
        balance += amount;
        if (showMessage)
        {
            std::cout << "Deposited " << amount << " " << currency << "\n";
        }
    }

    // Withdraw with default overdraft protection
    bool withdraw(double amount, bool allowOverdraft = false)
    {
        if (balance >= amount || allowOverdraft)
        {
            balance -= amount;
            std::cout << "Withdrawn " << amount << " " << currency << "\n";
            return true;
        }
        else
        {
            std::cout << "Insufficient funds!\n";
            return false;
        }
    }

    void displayBalance(bool detailed = false) const
    {
        if (detailed)
        {
            std::cout << "===== ACCOUNT DETAILS =====\n";
            std::cout << "Owner: " << owner << "\n";
            std::cout << "Balance: " << std::fixed << std::setprecision(2)
                     << balance << " " << currency << "\n";
            std::cout << "===========================\n";
        }
        else
        {
            std::cout << "Balance: " << balance << " " << currency << "\n";
        }
    }
};

int main()
{
    BankAccount acc1("Alice");                    // Default balance and currency
    BankAccount acc2("Bob", 1000.0);              // Custom balance
    BankAccount acc3("Charlie", 500.0, "EUR");    // Custom balance and currency

    acc1.deposit(500.0);           // With message
    acc1.deposit(200.0, false);    // Without message

    acc1.displayBalance();         // Simple display
    acc1.displayBalance(true);     // Detailed display

    acc1.withdraw(100.0);          // Normal withdrawal
    acc1.withdraw(1000.0);         // Will fail
    acc1.withdraw(1000.0, true);   // Allowed with overdraft

    return 0;
}
```

---

## 5. Default Arguments Best Practices

### Use Cases for Default Arguments:

```cpp
#include <iostream>
#include <string>
#include <vector>

// 1. Optional configuration
void connectToServer(std::string host,
                    int port = 80,
                    int timeout = 30,
                    bool useSSL = false)
{
    std::cout << "Connecting to " << host << ":" << port << "\n";
    std::cout << "Timeout: " << timeout << "s, SSL: " << (useSSL ? "Yes" : "No") << "\n";
}

// 2. Backward compatibility
class Logger
{
public:
    // Old interface
    void log(std::string message)
    {
        log(message, "INFO", true);
    }

    // New interface with defaults (maintains compatibility)
    void log(std::string message,
            std::string level = "INFO",
            bool timestamp = true)
    {
        if (timestamp)
        {
            std::cout << "[" << level << "] " << message << "\n";
        }
        else
        {
            std::cout << message << "\n";
        }
    }
};

// 3. Flexible formatting
void printTable(const std::vector<std::string>& data,
               int columnWidth = 15,
               char separator = '|',
               bool showHeader = true)
{
    if (showHeader)
    {
        std::cout << std::string(columnWidth * data.size(), '=') << "\n";
    }

    for (const auto& item : data)
    {
        std::cout << std::left << std::setw(columnWidth) << item << separator;
    }
    std::cout << "\n";
}

int main()
{
    // 1. Server connections
    connectToServer("example.com");                          // All defaults
    connectToServer("example.com", 443);                     // Custom port
    connectToServer("example.com", 443, 60);                 // Custom timeout
    connectToServer("example.com", 443, 60, true);          // With SSL

    std::cout << "\n";

    // 2. Logging
    Logger logger;
    logger.log("Application started");                       // Default level and timestamp
    logger.log("User login", "INFO");                        // Custom level
    logger.log("Debug info", "DEBUG", false);               // No timestamp

    std::cout << "\n";

    // 3. Tables
    std::vector<std::string> row = {"Name", "Age", "City"};
    printTable(row);                                         // All defaults
    printTable(row, 20);                                    // Custom width
    printTable(row, 20, ' ');                               // Custom separator
    printTable(row, 20, ' ', false);                        // No header

    return 0;
}
```

### Avoiding Common Pitfalls:

```cpp
#include <iostream>
#include <string>

// ❌ PROBLEM: Too many default parameters
// void createUser(std::string name,
//                 std::string email = "",
//                 int age = 0,
//                 std::string city = "",
//                 std::string country = "",
//                 std::string phone = "",
//                 bool active = true,
//                 std::string role = "user")  // Hard to use!

// ✅ BETTER: Use a configuration struct
struct UserConfig
{
    std::string email = "";
    int age = 0;
    std::string city = "";
    std::string country = "";
    std::string phone = "";
    bool active = true;
    std::string role = "user";
};

void createUser(std::string name, const UserConfig& config = UserConfig())
{
    std::cout << "Creating user: " << name << "\n";
    if (!config.email.empty())
        std::cout << "  Email: " << config.email << "\n";
    if (config.age > 0)
        std::cout << "  Age: " << config.age << "\n";
    // ... etc
}

// ❌ PROBLEM: Default pointer
// void process(int* data = nullptr)  // Dangerous!

// ✅ BETTER: Use reference or be explicit
void processSafe(int* data)
{
    if (data != nullptr)
    {
        std::cout << "Processing: " << *data << "\n";
    }
}

int main()
{
    // Simple usage
    createUser("Alice");

    // Complex usage
    UserConfig config;
    config.email = "alice@example.com";
    config.age = 25;
    config.city = "New York";
    createUser("Alice", config);

    return 0;
}
```

---

## 6. Combining Inline and Defaults

### Best of Both Worlds:

```cpp
#include <iostream>
#include <cmath>

// Inline functions with default arguments
inline double calculateCircleArea(double radius, double pi = 3.14159265359)
{
    return pi * radius * radius;
}

inline double calculateCircleCircumference(double radius, double pi = 3.14159265359)
{
    return 2 * pi * radius;
}

// Class with inline methods and default arguments
class Math
{
public:
    // Inline + default arguments
    inline static double power(double base, int exponent = 2)
    {
        return std::pow(base, exponent);
    }

    inline static double root(double value, int n = 2)
    {
        return std::pow(value, 1.0 / n);
    }

    inline static int round(double value, int decimals = 0)
    {
        double multiplier = std::pow(10, decimals);
        return std::round(value * multiplier) / multiplier;
    }
};

class Point
{
private:
    double x, y;

public:
    Point(double xVal = 0, double yVal = 0) : x(xVal), y(yVal) {}

    inline double distanceTo(const Point& other) const
    {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    inline void move(double dx = 0, double dy = 0)
    {
        x += dx;
        y += dy;
    }

    inline void display(bool detailed = false) const
    {
        if (detailed)
            std::cout << "Point(x=" << x << ", y=" << y << ")\n";
        else
            std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    // Inline + defaults
    std::cout << "Circle area (r=5): " << calculateCircleArea(5.0) << "\n";
    std::cout << "Circle area (r=5, custom pi): "
              << calculateCircleArea(5.0, 3.14) << "\n\n";

    // Static inline methods with defaults
    std::cout << "2^2: " << Math::power(2) << "\n";
    std::cout << "2^3: " << Math::power(2, 3) << "\n";
    std::cout << "√16: " << Math::root(16) << "\n";
    std::cout << "∛27: " << Math::root(27, 3) << "\n\n";

    // Point class
    Point p1;               // Default (0, 0)
    Point p2(3, 4);

    p1.display();
    p2.display(true);

    p1.move(1, 2);
    p1.display(true);

    std::cout << "Distance: " << p1.distanceTo(p2) << "\n";

    return 0;
}
```

---

## 7. Function Templates Introduction

### Basic Template Functions:

```cpp
#include <iostream>

// Template function with inline
template<typename T>
inline T max(T a, T b)
{
    return (a > b) ? a : b;
}

template<typename T>
inline T min(T a, T b)
{
    return (a < b) ? a : b;
}

// Template with default argument
template<typename T>
inline T clamp(T value, T minVal = 0, T maxVal = 100)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

int main()
{
    // Works with different types
    std::cout << "max(5, 3): " << max(5, 3) << "\n";
    std::cout << "max(5.5, 3.2): " << max(5.5, 3.2) << "\n";
    std::cout << "max('a', 'z'): " << max('a', 'z') << "\n\n";

    std::cout << "clamp(150): " << clamp(150) << "\n";
    std::cout << "clamp(150, 0, 200): " << clamp(150, 0, 200) << "\n";
    std::cout << "clamp(-10.5): " << clamp(-10.5, -5.0, 5.0) << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Inline Math Library

```cpp
#include <iostream>
#include <cmath>

namespace MathLib
{
    inline double square(double x) { return x * x; }
    inline double cube(double x) { return x * x * x; }

    inline double circleArea(double radius, double pi = 3.14159265359)
    {
        return pi * radius * radius;
    }

    inline double sphereVolume(double radius, double pi = 3.14159265359)
    {
        return (4.0 / 3.0) * pi * radius * radius * radius;
    }

    inline double cylinderVolume(double radius, double height, double pi = 3.14159265359)
    {
        return pi * radius * radius * height;
    }

    inline double coneVolume(double radius, double height, double pi = 3.14159265359)
    {
        return (1.0 / 3.0) * pi * radius * radius * height;
    }

    inline bool inRange(double value, double min = 0, double max = 100)
    {
        return value >= min && value <= max;
    }

    inline double percentage(double part, double whole)
    {
        return (part / whole) * 100.0;
    }
}

int main()
{
    using namespace MathLib;

    std::cout << "Square of 5: " << square(5) << "\n";
    std::cout << "Cube of 3: " << cube(3) << "\n\n";

    std::cout << "Circle area (r=5): " << circleArea(5) << "\n";
    std::cout << "Sphere volume (r=5): " << sphereVolume(5) << "\n";
    std::cout << "Cylinder volume (r=3, h=10): " << cylinderVolume(3, 10) << "\n";
    std::cout << "Cone volume (r=3, h=10): " << coneVolume(3, 10) << "\n\n";

    std::cout << "Is 50 in range [0,100]? " << (inRange(50) ? "Yes" : "No") << "\n";
    std::cout << "Is 150 in range [0,100]? " << (inRange(150) ? "Yes" : "No") << "\n";
    std::cout << "Is 75 in range [50,100]? " << (inRange(75, 50, 100) ? "Yes" : "No") << "\n\n";

    std::cout << "25 is " << percentage(25, 100) << "% of 100\n";

    return 0;
}
```

### Exercise 2: String Utilities

```cpp
#include <iostream>
#include <string>
#include <algorithm>

namespace StringUtils
{
    inline std::string toUpper(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    inline std::string toLower(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    inline std::string repeat(const std::string& str, int times = 1)
    {
        std::string result;
        for (int i = 0; i < times; i++)
            result += str;
        return result;
    }

    inline std::string center(const std::string& str, int width = 80, char fill = ' ')
    {
        if (str.length() >= width)
            return str;

        int padding = (width - str.length()) / 2;
        return std::string(padding, fill) + str + std::string(padding, fill);
    }

    inline std::string leftPad(const std::string& str, int width = 10, char fill = ' ')
    {
        if (str.length() >= width)
            return str;
        return std::string(width - str.length(), fill) + str;
    }

    inline std::string rightPad(const std::string& str, int width = 10, char fill = ' ')
    {
        if (str.length() >= width)
            return str;
        return str + std::string(width - str.length(), fill);
    }

    inline bool startsWith(const std::string& str, const std::string& prefix)
    {
        return str.substr(0, prefix.length()) == prefix;
    }

    inline bool endsWith(const std::string& str, const std::string& suffix)
    {
        if (suffix.length() > str.length())
            return false;
        return str.substr(str.length() - suffix.length()) == suffix;
    }
}

int main()
{
    using namespace StringUtils;

    std::string text = "Hello World";

    std::cout << "Original: " << text << "\n";
    std::cout << "Upper: " << toUpper(text) << "\n";
    std::cout << "Lower: " << toLower(text) << "\n\n";

    std::cout << "Repeat 'Hi' 3 times: " << repeat("Hi ", 3) << "\n\n";

    std::cout << "Centered: |" << center(text, 30) << "|\n";
    std::cout << "Left pad: |" << leftPad(text, 20) << "|\n";
    std::cout << "Right pad: |" << rightPad(text, 20) << "|\n\n";

    std::cout << "Starts with 'Hello': " << (startsWith(text, "Hello") ? "Yes" : "No") << "\n";
    std::cout << "Ends with 'World': " << (endsWith(text, "World") ? "Yes" : "No") << "\n";

    return 0;
}
```

---

## Practice Project: Configuration Manager

```cpp
#include <iostream>
#include <string>
#include <map>

class ConfigManager
{
private:
    std::map<std::string, std::string> settings;
    std::string appName;

public:
    ConfigManager(const std::string& name = "MyApp") : appName(name)
    {
        // Set defaults
        settings["theme"] = "light";
        settings["language"] = "en";
        settings["fontSize"] = "12";
        settings["autoSave"] = "true";
    }

    inline void set(const std::string& key, const std::string& value)
    {
        settings[key] = value;
    }

    inline std::string get(const std::string& key,
                          const std::string& defaultValue = "") const
    {
        auto it = settings.find(key);
        if (it != settings.end())
            return it->second;
        return defaultValue;
    }

    inline int getInt(const std::string& key, int defaultValue = 0) const
    {
        try
        {
            return std::stoi(get(key));
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    inline bool getBool(const std::string& key, bool defaultValue = false) const
    {
        std::string value = get(key);
        if (value == "true" || value == "1")
            return true;
        if (value == "false" || value == "0")
            return false;
        return defaultValue;
    }

    inline void display(bool showDefaults = true) const
    {
        std::cout << "\n=== " << appName << " Configuration ===\n";
        for (const auto& pair : settings)
        {
            if (showDefaults || !isDefault(pair.first))
            {
                std::cout << pair.first << ": " << pair.second << "\n";
            }
        }
        std::cout << "================================\n";
    }

    inline bool isDefault(const std::string& key) const
    {
        static std::map<std::string, std::string> defaults = {
            {"theme", "light"},
            {"language", "en"},
            {"fontSize", "12"},
            {"autoSave", "true"}
        };

        auto it = defaults.find(key);
        if (it != defaults.end())
        {
            return get(key) == it->second;
        }
        return false;
    }

    inline void reset(const std::string& key = "")
    {
        if (key.empty())
        {
            // Reset all to defaults
            settings["theme"] = "light";
            settings["language"] = "en";
            settings["fontSize"] = "12";
            settings["autoSave"] = "true";
            std::cout << "All settings reset to defaults\n";
        }
        else
        {
            // Reset specific key
            if (key == "theme") settings["theme"] = "light";
            else if (key == "language") settings["language"] = "en";
            else if (key == "fontSize") settings["fontSize"] = "12";
            else if (key == "autoSave") settings["autoSave"] = "true";
            std::cout << key << " reset to default\n";
        }
    }
};

int main()
{
    ConfigManager config("TextEditor");

    // Display defaults
    config.display();

    // Modify settings
    config.set("theme", "dark");
    config.set("fontSize", "14");
    config.set("language", "es");

    // Display modified
    config.display();

    // Get values with defaults
    std::cout << "\nReading settings:\n";
    std::cout << "Theme: " << config.get("theme") << "\n";
    std::cout << "Font size: " << config.getInt("fontSize") << "\n";
    std::cout << "Auto-save: " << (config.getBool("autoSave") ? "enabled" : "disabled") << "\n";
    std::cout << "Unknown key: " << config.get("unknown", "default value") << "\n";

    // Reset
    config.reset("theme");
    config.display();

    return 0;
}
```

---

## Key Takeaways

1. Inline suggests code insertion at call site
2. Use inline for small, frequently called functions
3. Inline is better than macros (type safe, debuggable)
4. Default arguments must be rightmost
5. Don't repeat defaults in function definition
6. Combine inline and defaults for efficient, flexible code
7. Templates work well with inline and defaults
8. Compiler decides whether to actually inline

---

## Summary Checklist

Before moving to Lesson 16, ensure you can:
- [ ] Understand inline functions and when to use them
- [ ] Know the difference between inline and macros
- [ ] Use default arguments effectively
- [ ] Follow default argument rules
- [ ] Combine inline with default arguments
- [ ] Understand basic function templates
- [ ] Avoid common inline and default pitfalls

---

## Next Lesson Preview

In **Lesson 16**, we'll explore:
- Array fundamentals
- Array declaration and initialization
- Accessing array elements
- Array size and bounds
- Arrays and functions
- Common array operations

**Functions mastered - arrays await!**
