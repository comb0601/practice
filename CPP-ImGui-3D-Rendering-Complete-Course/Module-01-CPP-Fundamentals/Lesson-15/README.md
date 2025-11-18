# Lesson 15: Default Arguments and Function Pointers

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Use default arguments in functions effectively
- Understand the rules and limitations of default arguments
- Work with function pointers
- Pass functions as arguments to other functions
- Create callback mechanisms using function pointers
- Understand the relationship between functions and pointers

## 1. Default Arguments

### 1.1 Introduction to Default Arguments

Default arguments allow you to call a function without providing all arguments. The missing arguments use default values specified in the function declaration.

```cpp
#include <iostream>
using namespace std;

// Function with default arguments
void greet(const char* name = "Guest", const char* greeting = "Hello") {
    cout << greeting << ", " << name << "!" << endl;
}

int main() {
    greet();                        // Uses both defaults
    greet("Alice");                 // Uses default greeting
    greet("Bob", "Hi");             // Uses no defaults
    greet("Charlie", "Welcome");    // Uses no defaults

    return 0;
}
```

**Output:**
```
Hello, Guest!
Hello, Alice!
Hi, Bob!
Welcome, Charlie!
```

### 1.2 Rules for Default Arguments

```cpp
#include <iostream>
using namespace std;

// Default arguments must be rightmost
void function1(int a, int b = 10, int c = 20) {  // OK
    cout << a << ", " << b << ", " << c << endl;
}

// void function2(int a = 5, int b, int c);  // ERROR: default must be rightmost

// Default arguments in declaration, not definition
void function3(int x, int y = 5);  // Declaration with default

void function3(int x, int y) {     // Definition without default
    cout << x + y << endl;
}

int main() {
    function1(1);           // 1, 10, 20
    function1(1, 2);        // 1, 2, 20
    function1(1, 2, 3);     // 1, 2, 3

    function3(10);          // 15 (uses default y=5)
    function3(10, 7);       // 17

    return 0;
}
```

### 1.3 Practical Examples with Default Arguments

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

// Print array with optional separator
void printArray(int arr[], int size, const char* separator = ", ") {
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) {
            cout << separator;
        }
    }
    cout << endl;
}

// Calculate power with default exponent
double power(double base, int exponent = 2) {
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Format currency with default symbol
void printCurrency(double amount, const char* symbol = "$") {
    cout << fixed << setprecision(2);
    cout << symbol << amount << endl;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};

    printArray(numbers, 5);          // Default separator
    printArray(numbers, 5, " - ");   // Custom separator

    cout << "5^2 = " << power(5) << endl;         // Default exponent 2
    cout << "5^3 = " << power(5, 3) << endl;      // Custom exponent

    printCurrency(99.99);            // Default $
    printCurrency(50.00, "€");       // Custom symbol

    return 0;
}
```

## 2. Function Pointers Basics

### 2.1 Introduction to Function Pointers

A function pointer is a variable that stores the address of a function.

```cpp
#include <iostream>
using namespace std;

// Simple functions
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    // Declare function pointer
    int (*operation)(int, int);

    // Point to add function
    operation = add;
    cout << "5 + 3 = " << operation(5, 3) << endl;

    // Point to subtract function
    operation = subtract;
    cout << "5 - 3 = " << operation(5, 3) << endl;

    // Point to multiply function
    operation = multiply;
    cout << "5 * 3 = " << operation(5, 3) << endl;

    return 0;
}
```

### 2.2 Function Pointer Syntax

```cpp
#include <iostream>
using namespace std;

// Function that takes two ints and returns int
int add(int a, int b) { return a + b; }

// Function that takes int and returns void
void printSquare(int x) {
    cout << x * x << endl;
}

// Function that takes no parameters and returns double
double getPi() {
    return 3.14159;
}

int main() {
    // Function pointer declarations
    int (*mathOp)(int, int) = add;
    void (*printer)(int) = printSquare;
    double (*getter)() = getPi;

    // Using function pointers
    cout << "Sum: " << mathOp(5, 3) << endl;

    cout << "Square: ";
    printer(5);

    cout << "Pi: " << getter() << endl;

    return 0;
}
```

### 2.3 Arrays of Function Pointers

```cpp
#include <iostream>
using namespace std;

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int main() {
    // Array of function pointers
    int (*operations[4])(int, int) = {add, subtract, multiply, divide};

    const char* opNames[] = {"Add", "Subtract", "Multiply", "Divide"};

    int x = 10, y = 5;

    for (int i = 0; i < 4; i++) {
        cout << opNames[i] << ": " << operations[i](x, y) << endl;
    }

    return 0;
}
```

## 3. Passing Functions as Arguments

### 3.1 Callback Functions

```cpp
#include <iostream>
using namespace std;

// Callback function type
void processArray(int arr[], int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);
    }
}

// Different callback implementations
void printValue(int value) {
    cout << value << " ";
}

void printSquare(int value) {
    cout << (value * value) << " ";
}

void printDouble(int value) {
    cout << (value * 2) << " ";
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};

    cout << "Original values: ";
    processArray(numbers, 5, printValue);
    cout << endl;

    cout << "Squared values: ";
    processArray(numbers, 5, printSquare);
    cout << endl;

    cout << "Doubled values: ";
    processArray(numbers, 5, printDouble);
    cout << endl;

    return 0;
}
```

### 3.2 Mathematical Operations with Function Pointers

```cpp
#include <iostream>
using namespace std;

// Apply operation to two numbers
int applyOperation(int a, int b, int (*op)(int, int)) {
    return op(a, b);
}

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }
int max(int a, int b) { return (a > b) ? a : b; }

int main() {
    int x = 10, y = 5;

    cout << "Apply add: " << applyOperation(x, y, add) << endl;
    cout << "Apply multiply: " << applyOperation(x, y, multiply) << endl;
    cout << "Apply max: " << applyOperation(x, y, max) << endl;

    return 0;
}
```

### 3.3 Sorting with Custom Comparator

```cpp
#include <iostream>
using namespace std;

// Generic sorting function with comparator
void bubbleSort(int arr[], int size, bool (*compare)(int, int)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compare(arr[j], arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Comparator functions
bool ascending(int a, int b) {
    return a > b;  // Return true if they need to be swapped
}

bool descending(int a, int b) {
    return a < b;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers1[] = {64, 34, 25, 12, 22, 11, 90};
    int numbers2[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    bubbleSort(numbers1, size, ascending);
    cout << "Ascending: ";
    printArray(numbers1, size);

    bubbleSort(numbers2, size, descending);
    cout << "Descending: ";
    printArray(numbers2, size);

    return 0;
}
```

## 4. Advanced Function Pointer Usage

### 4.1 Calculator with Function Pointers

```cpp
#include <iostream>
using namespace std;

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int calculate(int a, int b, char op) {
    int (*operation)(int, int) = nullptr;

    switch (op) {
        case '+': operation = add; break;
        case '-': operation = subtract; break;
        case '*': operation = multiply; break;
        case '/': operation = divide; break;
        default: return 0;
    }

    if (operation != nullptr) {
        return operation(a, b);
    }

    return 0;
}

int main() {
    int x = 10, y = 5;

    cout << x << " + " << y << " = " << calculate(x, y, '+') << endl;
    cout << x << " - " << y << " = " << calculate(x, y, '-') << endl;
    cout << x << " * " << y << " = " << calculate(x, y, '*') << endl;
    cout << x << " / " << y << " = " << calculate(x, y, '/') << endl;

    return 0;
}
```

### 4.2 Menu System with Function Pointers

```cpp
#include <iostream>
using namespace std;

void option1() {
    cout << "Option 1 executed" << endl;
}

void option2() {
    cout << "Option 2 executed" << endl;
}

void option3() {
    cout << "Option 3 executed" << endl;
}

void displayMenu() {
    cout << "\n=== Menu ===" << endl;
    cout << "1. Option 1" << endl;
    cout << "2. Option 2" << endl;
    cout << "3. Option 3" << endl;
    cout << "4. Exit" << endl;
    cout << "Choice: ";
}

int main() {
    void (*menuOptions[3])() = {option1, option2, option3};

    int choice;
    do {
        displayMenu();
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
            menuOptions[choice - 1]();
        } else if (choice != 4) {
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 4);

    cout << "Goodbye!" << endl;

    return 0;
}
```

## 5. Combining Default Arguments and Function Pointers

```cpp
#include <iostream>
using namespace std;

// Default comparator function
bool defaultCompare(int a, int b) {
    return a > b;
}

// Search function with default comparator
int findFirst(int arr[], int size, int target, bool (*compare)(int, int) = defaultCompare) {
    for (int i = 0; i < size; i++) {
        if (compare(arr[i], target)) {
            return i;
        }
    }
    return -1;
}

bool equals(int a, int b) {
    return a == b;
}

bool greaterThan(int a, int b) {
    return a > b;
}

int main() {
    int numbers[] = {5, 12, 8, 20, 3, 15};
    int size = 6;

    // Use default comparator
    int index = findFirst(numbers, size, 10);
    cout << "First > 10 at index: " << index << endl;

    // Use custom comparator
    index = findFirst(numbers, size, 12, equals);
    cout << "First == 12 at index: " << index << endl;

    return 0;
}
```

## 6. Practical Examples

### Example 1: Event System

```cpp
#include <iostream>
using namespace std;

const int MAX_HANDLERS = 10;

struct EventSystem {
    void (*handlers[MAX_HANDLERS])();
    int handlerCount;
};

void initEventSystem(EventSystem& es) {
    es.handlerCount = 0;
}

void registerHandler(EventSystem& es, void (*handler)()) {
    if (es.handlerCount < MAX_HANDLERS) {
        es.handlers[es.handlerCount++] = handler;
    }
}

void triggerEvent(EventSystem& es) {
    cout << "Event triggered!" << endl;
    for (int i = 0; i < es.handlerCount; i++) {
        es.handlers[i]();
    }
}

// Event handlers
void onEventHandler1() {
    cout << "  Handler 1 executed" << endl;
}

void onEventHandler2() {
    cout << "  Handler 2 executed" << endl;
}

void onEventHandler3() {
    cout << "  Handler 3 executed" << endl;
}

int main() {
    EventSystem eventSystem;
    initEventSystem(eventSystem);

    registerHandler(eventSystem, onEventHandler1);
    registerHandler(eventSystem, onEventHandler2);
    registerHandler(eventSystem, onEventHandler3);

    triggerEvent(eventSystem);

    return 0;
}
```

### Example 2: String Transformation

```cpp
#include <iostream>
#include <cctype>
using namespace std;

// Transform each character in string using function pointer
void transformString(char* str, char (*transformer)(char)) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = transformer(str[i]);
    }
}

// Transformation functions
char toUpperChar(char c) {
    return toupper(c);
}

char toLowerChar(char c) {
    return tolower(c);
}

char rot13(char c) {
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + 13) % 26;
    } else if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + 13) % 26;
    }
    return c;
}

int main() {
    char text1[] = "Hello World";
    char text2[] = "Hello World";
    char text3[] = "Hello World";

    transformString(text1, toUpperChar);
    cout << "Uppercase: " << text1 << endl;

    transformString(text2, toLowerChar);
    cout << "Lowercase: " << text2 << endl;

    transformString(text3, rot13);
    cout << "ROT13: " << text3 << endl;

    return 0;
}
```

### Example 3: Numeric Integration

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// Numerical integration using function pointer
double integrate(double (*func)(double), double a, double b, int steps = 1000) {
    double width = (b - a) / steps;
    double sum = 0.0;

    for (int i = 0; i < steps; i++) {
        double x = a + i * width;
        sum += func(x) * width;
    }

    return sum;
}

// Functions to integrate
double square(double x) {
    return x * x;
}

double cube(double x) {
    return x * x * x;
}

double sine(double x) {
    return sin(x);
}

int main() {
    cout << fixed << setprecision(4);

    cout << "Integral of x^2 from 0 to 1: " << integrate(square, 0, 1) << endl;
    cout << "Integral of x^3 from 0 to 2: " << integrate(cube, 0, 2) << endl;
    cout << "Integral of sin(x) from 0 to pi: " << integrate(sine, 0, M_PI) << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Logger Function
Create a logging function with default log level and timestamp. Use function pointers to handle different log levels (info, warning, error).

### Exercise 2: Array Filter
Implement a filter function that takes an array and a predicate function pointer, returning count of elements that match the predicate.

### Exercise 3: Command Dispatcher
Build a command dispatcher system that maps string commands to function pointers and executes them.

### Exercise 4: Mathematical Expression Evaluator
Create a simple expression evaluator that uses function pointers to handle different operations.

### Exercise 5: Custom Sort with Multiple Criteria
Implement a sorting function that can sort by different criteria using function pointers with default arguments.

## Exercise Solutions

### Solution 1: Logger Function

```cpp
#include <iostream>
#include <ctime>
using namespace std;

enum LogLevel { INFO, WARNING, ERROR };

void defaultLogHandler(const char* message, LogLevel level) {
    const char* levelNames[] = {"INFO", "WARNING", "ERROR"};
    cout << "[" << levelNames[level] << "] " << message << endl;
}

void log(const char* message, LogLevel level = INFO,
         void (*handler)(const char*, LogLevel) = defaultLogHandler,
         bool includeTimestamp = false) {

    if (includeTimestamp) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0';  // Remove newline
        cout << "[" << dt << "] ";
    }

    handler(message, level);
}

void customHandler(const char* message, LogLevel level) {
    cout << ">>> " << message << " <<<" << endl;
}

int main() {
    log("Application started");
    log("This is a warning", WARNING);
    log("This is an error", ERROR);
    log("With timestamp", INFO, defaultLogHandler, true);
    log("Custom handler", INFO, customHandler);

    return 0;
}
```

### Solution 2: Array Filter

```cpp
#include <iostream>
using namespace std;

int filter(int arr[], int size, int result[], bool (*predicate)(int)) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (predicate(arr[i])) {
            result[count++] = arr[i];
        }
    }
    return count;
}

// Predicate functions
bool isEven(int n) { return n % 2 == 0; }
bool isOdd(int n) { return n % 2 != 0; }
bool isPositive(int n) { return n > 0; }
bool isGreaterThan10(int n) { return n > 10; }

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int numbers[] = {-5, 12, -3, 8, 15, 4, -1, 20, 7};
    int size = 9;
    int result[20];

    cout << "Original: ";
    printArray(numbers, size);

    int count = filter(numbers, size, result, isEven);
    cout << "Even numbers: ";
    printArray(result, count);

    count = filter(numbers, size, result, isPositive);
    cout << "Positive numbers: ";
    printArray(result, count);

    count = filter(numbers, size, result, isGreaterThan10);
    cout << "Greater than 10: ";
    printArray(result, count);

    return 0;
}
```

### Solution 3: Command Dispatcher

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void cmdHelp() { cout << "Available commands: help, status, info, exit" << endl; }
void cmdStatus() { cout << "System status: OK" << endl; }
void cmdInfo() { cout << "System info: Version 1.0" << endl; }

struct Command {
    const char* name;
    void (*handler)();
};

const Command commands[] = {
    {"help", cmdHelp},
    {"status", cmdStatus},
    {"info", cmdInfo}
};

const int commandCount = 3;

void dispatchCommand(const char* cmdName) {
    for (int i = 0; i < commandCount; i++) {
        if (strcmp(cmdName, commands[i].name) == 0) {
            commands[i].handler();
            return;
        }
    }
    cout << "Unknown command: " << cmdName << endl;
}

int main() {
    char input[50];

    cout << "Type 'help' for available commands, 'exit' to quit" << endl;

    while (true) {
        cout << "> ";
        cin >> input;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        dispatchCommand(input);
    }

    return 0;
}
```

### Solution 4: Mathematical Expression Evaluator

```cpp
#include <iostream>
using namespace std;

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { return b != 0 ? a / b : 0; }

double evaluate(double a, char op, double b) {
    double (*operation)(double, double) = nullptr;

    switch (op) {
        case '+': operation = add; break;
        case '-': operation = subtract; break;
        case '*': operation = multiply; break;
        case '/': operation = divide; break;
        default:
            cout << "Invalid operator" << endl;
            return 0;
    }

    return operation(a, b);
}

int main() {
    double num1, num2;
    char op;

    cout << "Enter expression (e.g., 10 + 5): ";
    cin >> num1 >> op >> num2;

    double result = evaluate(num1, op, num2);
    cout << "Result: " << result << endl;

    return 0;
}
```

### Solution 5: Custom Sort with Multiple Criteria

```cpp
#include <iostream>
using namespace std;

// Default comparator (ascending)
bool ascending(int a, int b) {
    return a > b;
}

bool descending(int a, int b) {
    return a < b;
}

bool evenFirst(int a, int b) {
    bool aEven = (a % 2 == 0);
    bool bEven = (b % 2 == 0);

    if (aEven && !bEven) return false;
    if (!aEven && bEven) return true;
    return a > b;
}

void customSort(int arr[], int size, bool (*compare)(int, int) = ascending) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compare(arr[j], arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    customSort(arr1, size);  // Default: ascending
    cout << "Ascending: ";
    printArray(arr1, size);

    customSort(arr2, size, descending);
    cout << "Descending: ";
    printArray(arr2, size);

    customSort(arr3, size, evenFirst);
    cout << "Even first: ";
    printArray(arr3, size);

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Default arguments** provide default values for function parameters
- Default arguments must be **rightmost** in parameter list
- **Function pointers** store addresses of functions
- Functions can be passed as **arguments** to other functions
- **Callback mechanisms** enable flexible, reusable code
- Function pointers enable **runtime polymorphism**
- Combining default arguments and function pointers creates powerful, flexible APIs

## Checklist

- [ ] I understand default arguments and their rules
- [ ] I can use default arguments effectively
- [ ] I understand function pointer syntax
- [ ] I can declare and use function pointers
- [ ] I can pass functions as arguments
- [ ] I completed all exercises and understand the solutions
- [ ] I understand callback patterns
- [ ] I can combine default arguments with function pointers

## Additional Resources

- **Next Lesson:** Arrays - Declaration and Initialization
- **Related Topics:** Lambda Functions (C++11), std::function, Function Objects
- **Practice:** Refactor existing code to use callbacks and default arguments

---

**Congratulations!** You've completed Lesson 15. You now understand default arguments and function pointers, which are powerful features for creating flexible and reusable C++ code.
