# Lesson 10: Function Overloading

**Duration:** 2 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand the concept of function overloading in C++
- Create overloaded functions with different parameter lists
- Understand how the compiler resolves overloaded function calls
- Use function overloading to create more intuitive APIs
- Avoid common pitfalls in function overloading

## 1. Introduction to Function Overloading

Function overloading allows you to define multiple functions with the same name but different parameter lists. The compiler determines which function to call based on the arguments provided.

### 1.1 Basic Function Overloading

```cpp
#include <iostream>
using namespace std;

// Overloaded functions with different number of parameters
int add(int a, int b) {
    cout << "Adding two integers" << endl;
    return a + b;
}

int add(int a, int b, int c) {
    cout << "Adding three integers" << endl;
    return a + b + c;
}

int main() {
    cout << "Result 1: " << add(5, 3) << endl;
    cout << "Result 2: " << add(5, 3, 2) << endl;

    return 0;
}
```

**Output:**
```
Adding two integers
Result 1: 8
Adding three integers
Result 2: 10
```

### 1.2 Overloading with Different Parameter Types

```cpp
#include <iostream>
using namespace std;

// Overloaded functions with different parameter types
void print(int value) {
    cout << "Integer: " << value << endl;
}

void print(double value) {
    cout << "Double: " << value << endl;
}

void print(const char* value) {
    cout << "String: " << value << endl;
}

void print(char value) {
    cout << "Character: " << value << endl;
}

int main() {
    print(42);
    print(3.14159);
    print("Hello, World!");
    print('A');

    return 0;
}
```

**Output:**
```
Integer: 42
Double: 3.14159
String: Hello, World!
Character: A
```

## 2. Rules for Function Overloading

### 2.1 Valid Overloading Criteria

Functions can be overloaded based on:
1. Number of parameters
2. Type of parameters
3. Order of parameters (if types differ)

```cpp
#include <iostream>
using namespace std;

// Different number of parameters
void display(int a) {
    cout << "One parameter: " << a << endl;
}

void display(int a, int b) {
    cout << "Two parameters: " << a << ", " << b << endl;
}

// Different types of parameters
void process(int a, double b) {
    cout << "int, double: " << a << ", " << b << endl;
}

void process(double a, int b) {
    cout << "double, int: " << a << ", " << b << endl;
}

int main() {
    display(5);
    display(5, 10);

    process(5, 3.14);
    process(3.14, 5);

    return 0;
}
```

### 2.2 Invalid Overloading

You **cannot** overload functions based solely on:
- Return type
- Parameter names

```cpp
// INVALID - Cannot overload based on return type alone
int getValue() { return 1; }
double getValue() { return 1.0; }  // ERROR!

// INVALID - Parameter names don't matter
void func(int x) { }
void func(int y) { }  // ERROR! Same signature as above
```

## 3. Overloading with Const Parameters

Functions can be overloaded based on const qualification.

```cpp
#include <iostream>
using namespace std;

void process(int& value) {
    value *= 2;
    cout << "Non-const reference: " << value << endl;
}

void process(const int& value) {
    cout << "Const reference: " << value << endl;
    // value *= 2;  // ERROR: Cannot modify const reference
}

int main() {
    int x = 10;
    const int y = 20;

    process(x);  // Calls non-const version
    process(y);  // Calls const version
    process(30); // Calls const version (temporary is const)

    return 0;
}
```

## 4. Practical Examples

### Example 1: Mathematical Operations

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// Calculate area - overloaded for different shapes

// Circle area
double area(double radius) {
    return M_PI * radius * radius;
}

// Rectangle area
double area(double length, double width) {
    return length * width;
}

// Triangle area
double area(double base, double height, bool isTriangle) {
    if (isTriangle) {
        return 0.5 * base * height;
    }
    return 0;
}

int main() {
    cout << "Circle area (radius 5): " << area(5.0) << endl;
    cout << "Rectangle area (4 x 6): " << area(4.0, 6.0) << endl;
    cout << "Triangle area (base 8, height 5): " << area(8.0, 5.0, true) << endl;

    return 0;
}
```

### Example 2: String and Character Processing

```cpp
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

// Count occurrences - overloaded for char and string

// Count character in a string
int count(const char* str, char target) {
    int cnt = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == target) cnt++;
    }
    return cnt;
}

// Count substring in a string
int count(const char* str, const char* target) {
    int cnt = 0;
    int targetLen = strlen(target);
    int strLen = strlen(str);

    for (int i = 0; i <= strLen - targetLen; i++) {
        bool match = true;
        for (int j = 0; j < targetLen; j++) {
            if (str[i + j] != target[j]) {
                match = false;
                break;
            }
        }
        if (match) cnt++;
    }

    return cnt;
}

// Count words in a string
int count(const char* str) {
    int cnt = 0;
    bool inWord = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            cnt++;
        }
    }

    return cnt;
}

int main() {
    const char* text = "Hello World! Hello C++!";

    cout << "Character 'l' count: " << count(text, 'l') << endl;
    cout << "Substring 'Hello' count: " << count(text, "Hello") << endl;
    cout << "Word count: " << count(text) << endl;

    return 0;
}
```

### Example 3: Array Operations

```cpp
#include <iostream>
using namespace std;

// Find maximum - overloaded for different array types

int findMax(const int arr[], int size) {
    int maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

double findMax(const double arr[], int size) {
    double maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

char findMax(const char arr[], int size) {
    char maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

int main() {
    int intArr[] = {5, 12, 8, 23, 7};
    double doubleArr[] = {3.14, 2.71, 1.41, 9.87};
    char charArr[] = {'x', 'a', 'z', 'm', 'b'};

    cout << "Max int: " << findMax(intArr, 5) << endl;
    cout << "Max double: " << findMax(doubleArr, 4) << endl;
    cout << "Max char: " << findMax(charArr, 5) << endl;

    return 0;
}
```

### Example 4: Date and Time Formatting

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

// Format date - overloaded for different input types

// Format from individual components
void formatDate(int day, int month, int year) {
    cout << setfill('0') << setw(2) << day << "/"
         << setw(2) << month << "/"
         << year << endl;
}

// Format from string (assumes YYYY-MM-DD)
void formatDate(const char* isoDate) {
    // Simple parsing for demonstration
    int year, month, day;
    sscanf(isoDate, "%d-%d-%d", &year, &month, &day);
    formatDate(day, month, year);
}

// Format with custom separator
void formatDate(int day, int month, int year, char separator) {
    cout << setfill('0') << setw(2) << day << separator
         << setw(2) << month << separator
         << year << endl;
}

int main() {
    cout << "Date format 1: ";
    formatDate(15, 8, 2024);

    cout << "Date format 2: ";
    formatDate("2024-08-15");

    cout << "Date format 3: ";
    formatDate(15, 8, 2024, '-');

    return 0;
}
```

### Example 5: Distance Calculations

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// Calculate distance - overloaded for different coordinate systems

// 1D distance
double distance(double x1, double x2) {
    return abs(x2 - x1);
}

// 2D distance
double distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

// 3D distance
double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

int main() {
    cout << "1D distance (3 to 8): " << distance(3.0, 8.0) << endl;
    cout << "2D distance (0,0) to (3,4): " << distance(0.0, 0.0, 3.0, 4.0) << endl;
    cout << "3D distance (0,0,0) to (2,2,1): "
         << distance(0.0, 0.0, 0.0, 2.0, 2.0, 1.0) << endl;

    return 0;
}
```

## 5. Function Overloading vs Default Arguments

Sometimes default arguments can achieve similar results to overloading:

```cpp
#include <iostream>
using namespace std;

// Using overloading
void printMessage(const char* msg) {
    cout << msg << endl;
}

void printMessage(const char* msg, int times) {
    for (int i = 0; i < times; i++) {
        cout << msg << endl;
    }
}

// Using default arguments (alternative approach)
void displayMessage(const char* msg, int times = 1) {
    for (int i = 0; i < times; i++) {
        cout << msg << endl;
    }
}

int main() {
    // Both approaches work
    printMessage("Hello");
    printMessage("Hello", 3);

    displayMessage("World");
    displayMessage("World", 3);

    return 0;
}
```

## 6. Best Practices

1. **Use overloading for related operations**: Overload functions that perform similar operations on different types
2. **Keep signatures distinct**: Ensure overloaded functions have clearly different parameter lists
3. **Avoid ambiguity**: Be careful with implicit type conversions that might cause ambiguous calls
4. **Consider default arguments**: Sometimes default arguments are clearer than overloading
5. **Document your overloads**: Make it clear what each version does

## Exercises

### Exercise 1: Temperature Converter
Create overloaded functions named `convert` that:
- Convert Celsius to Fahrenheit (1 parameter)
- Convert Fahrenheit to Celsius (1 parameter, different name for clarity)
- Convert between any two temperature scales (2 parameters: value and scale indicator)

### Exercise 2: Volume Calculator
Write overloaded functions named `volume` to calculate the volume of:
- A cube (1 parameter: side length)
- A rectangular prism (3 parameters: length, width, height)
- A cylinder (2 parameters: radius, height)
- A sphere (1 parameter: radius)

### Exercise 3: Array Reversal
Create overloaded `reverse` functions that reverse:
- An integer array
- A double array
- A character array (C-string)

### Exercise 4: Power Function
Implement overloaded `power` functions that:
- Calculate integer power of an integer (returns int)
- Calculate integer power of a double (returns double)
- Calculate double power of a double using the pow function from cmath (returns double)

### Exercise 5: Min/Max Functions
Create overloaded `min` and `max` functions that work with:
- Two integers
- Three integers
- Two doubles
- Three doubles
- An array of integers
- An array of doubles

## Exercise Solutions

### Solution 1: Temperature Converter

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

// Celsius to Fahrenheit
double convertToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Fahrenheit to Celsius
double convertToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

// Generic converter with scale indicators
double convert(double value, char fromScale, char toScale) {
    if (fromScale == toScale) return value;

    // Convert to Celsius first
    double celsius = value;
    if (fromScale == 'F' || fromScale == 'f') {
        celsius = convertToCelsius(value);
    } else if (fromScale == 'K' || fromScale == 'k') {
        celsius = value - 273.15;
    }

    // Convert from Celsius to target
    if (toScale == 'F' || toScale == 'f') {
        return convertToFahrenheit(celsius);
    } else if (toScale == 'K' || toScale == 'k') {
        return celsius + 273.15;
    }

    return celsius;
}

int main() {
    cout << fixed << setprecision(2);

    cout << "25°C to Fahrenheit: " << convertToFahrenheit(25.0) << "°F" << endl;
    cout << "77°F to Celsius: " << convertToCelsius(77.0) << "°C" << endl;
    cout << "100°C to Kelvin: " << convert(100.0, 'C', 'K') << "K" << endl;
    cout << "300K to Fahrenheit: " << convert(300.0, 'K', 'F') << "°F" << endl;

    return 0;
}
```

### Solution 2: Volume Calculator

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// Cube volume
double volume(double side) {
    return side * side * side;
}

// Rectangular prism volume
double volume(double length, double width, double height) {
    return length * width * height;
}

// Cylinder volume (using bool to distinguish from rectangular prism)
double volume(double radius, double height, bool isCylinder) {
    if (isCylinder) {
        return M_PI * radius * radius * height;
    }
    return 0;
}

// Sphere volume
double volumeSphere(double radius) {
    return (4.0 / 3.0) * M_PI * radius * radius * radius;
}

int main() {
    cout << fixed << setprecision(2);

    cout << "Cube (side 5): " << volume(5.0) << endl;
    cout << "Rectangular prism (2x3x4): " << volume(2.0, 3.0, 4.0) << endl;
    cout << "Cylinder (radius 3, height 5): " << volume(3.0, 5.0, true) << endl;
    cout << "Sphere (radius 4): " << volumeSphere(4.0) << endl;

    return 0;
}
```

### Solution 3: Array Reversal

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// Reverse integer array
void reverse(int arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

// Reverse double array
void reverse(double arr[], int size) {
    for (int i = 0; i < size / 2; i++) {
        double temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

// Reverse character array (C-string)
void reverse(char arr[]) {
    int len = strlen(arr);
    for (int i = 0; i < len / 2; i++) {
        char temp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = temp;
    }
}

int main() {
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3, 4.4};
    char charArr[] = "Hello";

    cout << "Before reverse:" << endl;
    cout << "Int: "; for (int i = 0; i < 5; i++) cout << intArr[i] << " "; cout << endl;
    cout << "Double: "; for (int i = 0; i < 4; i++) cout << doubleArr[i] << " "; cout << endl;
    cout << "Char: " << charArr << endl;

    reverse(intArr, 5);
    reverse(doubleArr, 4);
    reverse(charArr);

    cout << "\nAfter reverse:" << endl;
    cout << "Int: "; for (int i = 0; i < 5; i++) cout << intArr[i] << " "; cout << endl;
    cout << "Double: "; for (int i = 0; i < 4; i++) cout << doubleArr[i] << " "; cout << endl;
    cout << "Char: " << charArr << endl;

    return 0;
}
```

### Solution 4: Power Function

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// Integer base, integer exponent
int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Double base, integer exponent
double power(double base, int exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Double base, double exponent
double power(double base, double exponent) {
    return pow(base, exponent);
}

int main() {
    cout << "2^3 (int): " << power(2, 3) << endl;
    cout << "2.5^3 (double base, int exp): " << power(2.5, 3) << endl;
    cout << "2.5^2.5 (double): " << power(2.5, 2.5) << endl;
    cout << "4^0.5 (square root): " << power(4.0, 0.5) << endl;

    return 0;
}
```

### Solution 5: Min/Max Functions

```cpp
#include <iostream>
using namespace std;

// Min/Max for two integers
int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

// Min/Max for three integers
int min(int a, int b, int c) { return min(min(a, b), c); }
int max(int a, int b, int c) { return max(max(a, b), c); }

// Min/Max for two doubles
double min(double a, double b) { return (a < b) ? a : b; }
double max(double a, double b) { return (a > b) ? a : b; }

// Min/Max for three doubles
double min(double a, double b, double c) { return min(min(a, b), c); }
double max(double a, double b, double c) { return max(max(a, b), c); }

// Min/Max for integer array
int min(const int arr[], int size) {
    int minVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) minVal = arr[i];
    }
    return minVal;
}

int max(const int arr[], int size) {
    int maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

// Min/Max for double array
double min(const double arr[], int size) {
    double minVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) minVal = arr[i];
    }
    return minVal;
}

double max(const double arr[], int size) {
    double maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

int main() {
    cout << "Min of 5 and 3: " << min(5, 3) << endl;
    cout << "Max of 5, 3, 8: " << max(5, 3, 8) << endl;
    cout << "Min of 3.14 and 2.71: " << min(3.14, 2.71) << endl;

    int intArr[] = {7, 2, 9, 1, 5};
    double doubleArr[] = {3.14, 1.41, 2.71, 0.58};

    cout << "Min in int array: " << min(intArr, 5) << endl;
    cout << "Max in int array: " << max(intArr, 5) << endl;
    cout << "Min in double array: " << min(doubleArr, 4) << endl;
    cout << "Max in double array: " << max(doubleArr, 4) << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Function overloading** allows multiple functions with the same name but different parameter lists
- Functions can be overloaded based on **number, type, and order** of parameters
- **Cannot overload** based solely on return type
- **Const qualification** can be used for overloading
- Overloading makes APIs more intuitive and flexible
- Sometimes **default arguments** are a better choice than overloading

## Checklist

- [ ] I understand the concept of function overloading
- [ ] I can create overloaded functions with different parameter lists
- [ ] I understand the rules for valid function overloading
- [ ] I know what criteria cannot be used for overloading
- [ ] I can use const qualification in overloading
- [ ] I completed all exercises and understand the solutions
- [ ] I can decide when to use overloading vs default arguments
- [ ] I understand how the compiler resolves overloaded function calls

## Additional Resources

- **Next Lesson:** Recursion
- **Related Topics:** Templates, Default Arguments, Operator Overloading
- **Practice:** Try creating overloaded functions for common operations in your projects

---

**Congratulations!** You've completed Lesson 10. You now understand function overloading and can use it to create more flexible and intuitive function interfaces.
