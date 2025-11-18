# Lesson 09: Function Parameters and Return Values

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand different parameter passing mechanisms (by value, by reference, by pointer)
- Use return values effectively in functions
- Implement multiple return values using references and pointers
- Understand when to use each parameter passing mechanism
- Apply best practices for function design with parameters and return values

## 1. Introduction to Parameters and Return Values

Functions are the building blocks of C++ programs. The way we pass data into functions (parameters) and get data out (return values) is crucial for writing efficient and maintainable code.

### 1.1 Pass by Value

When passing by value, a copy of the argument is made and passed to the function. Changes to the parameter inside the function do not affect the original argument.

```cpp
#include <iostream>
using namespace std;

// Pass by value - the parameter is a copy
void incrementByValue(int num) {
    num++;  // Only modifies the local copy
    cout << "Inside function: " << num << endl;
}

int main() {
    int value = 10;
    cout << "Before function call: " << value << endl;

    incrementByValue(value);

    cout << "After function call: " << value << endl;
    // value is still 10 - the original was not modified

    return 0;
}
```

**Output:**
```
Before function call: 10
Inside function: 11
After function call: 10
```

### 1.2 Pass by Reference

When passing by reference, the function receives a reference to the original variable. Changes made to the parameter affect the original argument.

```cpp
#include <iostream>
using namespace std;

// Pass by reference - the parameter is an alias
void incrementByReference(int& num) {
    num++;  // Modifies the original variable
    cout << "Inside function: " << num << endl;
}

int main() {
    int value = 10;
    cout << "Before function call: " << value << endl;

    incrementByReference(value);

    cout << "After function call: " << value << endl;
    // value is now 11 - the original was modified

    return 0;
}
```

**Output:**
```
Before function call: 10
Inside function: 11
After function call: 11
```

### 1.3 Pass by Pointer

When passing by pointer, the function receives the memory address of the variable. We can dereference the pointer to modify the original value.

```cpp
#include <iostream>
using namespace std;

// Pass by pointer - the parameter is a memory address
void incrementByPointer(int* num) {
    (*num)++;  // Dereference and modify the original
    cout << "Inside function: " << *num << endl;
}

int main() {
    int value = 10;
    cout << "Before function call: " << value << endl;

    incrementByPointer(&value);  // Pass the address of value

    cout << "After function call: " << value << endl;
    // value is now 11 - the original was modified

    return 0;
}
```

**Output:**
```
Before function call: 10
Inside function: 11
After function call: 11
```

## 2. Return Values

Functions can return values to the caller using the `return` statement.

### 2.1 Basic Return Values

```cpp
#include <iostream>
using namespace std;

// Function that returns an integer
int add(int a, int b) {
    return a + b;  // Return the sum
}

// Function that returns a double
double calculateAverage(int a, int b, int c) {
    return (a + b + c) / 3.0;
}

// Function that returns a boolean
bool isEven(int num) {
    return (num % 2 == 0);
}

int main() {
    int sum = add(5, 3);
    cout << "Sum: " << sum << endl;

    double avg = calculateAverage(10, 20, 30);
    cout << "Average: " << avg << endl;

    if (isEven(4)) {
        cout << "4 is even" << endl;
    }

    return 0;
}
```

### 2.2 Returning by Reference

You can return references, but be careful not to return references to local variables!

```cpp
#include <iostream>
using namespace std;

// Global array for demonstration
int numbers[5] = {10, 20, 30, 40, 50};

// Return a reference to an element in the array
int& getElement(int index) {
    return numbers[index];
}

int main() {
    cout << "Original value: " << numbers[2] << endl;

    // We can modify the array element through the reference
    getElement(2) = 999;

    cout << "Modified value: " << numbers[2] << endl;

    return 0;
}
```

**Warning:** Never return a reference to a local variable:

```cpp
// WRONG - DO NOT DO THIS!
int& badFunction() {
    int localVar = 10;
    return localVar;  // Returning reference to local variable - UNDEFINED BEHAVIOR!
}
```

## 3. Multiple Return Values

C++ functions can only return one value directly, but we can use references or pointers to return multiple values.

### 3.1 Using References for Multiple Returns

```cpp
#include <iostream>
using namespace std;

// Calculate quotient and remainder
void divideWithRemainder(int dividend, int divisor, int& quotient, int& remainder) {
    quotient = dividend / divisor;
    remainder = dividend % divisor;
}

int main() {
    int quot, rem;

    divideWithRemainder(17, 5, quot, rem);

    cout << "17 / 5 = " << quot << " remainder " << rem << endl;

    return 0;
}
```

### 3.2 Using Pointers for Multiple Returns

```cpp
#include <iostream>
using namespace std;

// Find min and max in an array
void findMinMax(int arr[], int size, int* min, int* max) {
    if (size == 0) return;

    *min = arr[0];
    *max = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

int main() {
    int numbers[] = {34, 12, 78, 5, 90, 23};
    int minVal, maxVal;

    findMinMax(numbers, 6, &minVal, &maxVal);

    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;

    return 0;
}
```

## 4. Const Parameters

Use `const` to indicate that a parameter should not be modified.

```cpp
#include <iostream>
using namespace std;

// The array cannot be modified through this function
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
        // arr[i] = 0;  // ERROR: cannot modify const parameter
    }
    cout << endl;
}

// Pass by const reference for efficiency (no copy) and safety (no modification)
void processLargeData(const int& data) {
    cout << "Processing: " << data << endl;
    // data = 100;  // ERROR: cannot modify const reference
}

int main() {
    int values[] = {1, 2, 3, 4, 5};
    printArray(values, 5);

    int largeValue = 1000000;
    processLargeData(largeValue);

    return 0;
}
```

## 5. Practical Examples

### Example 1: Temperature Converter

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

// Convert Celsius to Fahrenheit
double celsiusToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Convert Fahrenheit to Celsius
double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

// Convert temperature and return both original and converted values
void convertTemperature(double input, char fromUnit, double& result, char& toUnit) {
    if (fromUnit == 'C' || fromUnit == 'c') {
        result = celsiusToFahrenheit(input);
        toUnit = 'F';
    } else {
        result = fahrenheitToCelsius(input);
        toUnit = 'C';
    }
}

int main() {
    double temp, converted;
    char unit, resultUnit;

    cout << "Enter temperature and unit (C/F): ";
    cin >> temp >> unit;

    convertTemperature(temp, unit, converted, resultUnit);

    cout << fixed << setprecision(2);
    cout << temp << unit << " = " << converted << resultUnit << endl;

    return 0;
}
```

### Example 2: Circle Calculations

```cpp
#include <iostream>
#include <cmath>
using namespace std;

const double PI = 3.14159265358979323846;

// Calculate area and circumference of a circle
void calculateCircle(double radius, double& area, double& circumference) {
    area = PI * radius * radius;
    circumference = 2 * PI * radius;
}

// Validate radius (must be positive)
bool isValidRadius(double radius) {
    return radius > 0;
}

int main() {
    double r, area, circ;

    cout << "Enter circle radius: ";
    cin >> r;

    if (!isValidRadius(r)) {
        cout << "Error: Radius must be positive!" << endl;
        return 1;
    }

    calculateCircle(r, area, circ);

    cout << "Area: " << area << endl;
    cout << "Circumference: " << circ << endl;

    return 0;
}
```

### Example 3: String Manipulation

```cpp
#include <iostream>
#include <cctype>
using namespace std;

// Count vowels and consonants in a string
void countLetters(const char* str, int& vowels, int& consonants) {
    vowels = 0;
    consonants = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]);
        if (isalpha(ch)) {
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowels++;
            } else {
                consonants++;
            }
        }
    }
}

// Convert string to uppercase (modifies original)
void toUpperCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    char text[100] = "Hello World";
    int v, c;

    cout << "Original: " << text << endl;

    countLetters(text, v, c);
    cout << "Vowels: " << v << ", Consonants: " << c << endl;

    toUpperCase(text);
    cout << "Uppercase: " << text << endl;

    return 0;
}
```

### Example 4: Array Operations

```cpp
#include <iostream>
using namespace std;

// Calculate sum and average of array
void arrayStats(const int arr[], int size, int& sum, double& average) {
    sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    average = static_cast<double>(sum) / size;
}

// Find element in array, return index (-1 if not found)
int findElement(const int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;  // Not found
}

// Swap two integers
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int numbers[] = {5, 12, 8, 3, 19, 7};
    int size = 6;
    int total;
    double avg;

    arrayStats(numbers, size, total, avg);
    cout << "Sum: " << total << ", Average: " << avg << endl;

    int index = findElement(numbers, size, 8);
    if (index != -1) {
        cout << "Found 8 at index " << index << endl;
    }

    cout << "Before swap: " << numbers[0] << ", " << numbers[1] << endl;
    swap(numbers[0], numbers[1]);
    cout << "After swap: " << numbers[0] << ", " << numbers[1] << endl;

    return 0;
}
```

## 6. Best Practices

1. **Choose the right parameter passing mechanism:**
   - Use pass-by-value for small, simple types (int, char, etc.)
   - Use pass-by-const-reference for large objects you don't need to modify
   - Use pass-by-reference when you need to modify the argument
   - Use pass-by-pointer when you need optional parameters (can be nullptr)

2. **Use const when appropriate:**
   - Mark parameters as const if they shouldn't be modified
   - This prevents accidental modifications and clarifies intent

3. **Be careful with return types:**
   - Don't return references to local variables
   - Consider return value optimization for complex types

4. **Document your functions:**
   - Clearly indicate what parameters are inputs, outputs, or both
   - Specify valid ranges and constraints

## Exercises

### Exercise 1: Rectangle Calculator
Write functions to calculate the area and perimeter of a rectangle. The main function should accept length and width, then call appropriate functions to get both values.

### Exercise 2: Grade Statistics
Create a function that takes an array of grades and returns (via reference parameters) the highest grade, lowest grade, and average grade.

### Exercise 3: String Reversal
Write a function that reverses a C-string in place (modifies the original string). Test it with various inputs.

### Exercise 4: Prime Checker
Create a function that checks if a number is prime. Use it in conjunction with another function that finds all prime numbers in a given range and stores them in an array.

### Exercise 5: Coordinate Distance
Write a function that calculates the distance between two 2D points. The function should take four parameters (x1, y1, x2, y2) and return the distance.

## Exercise Solutions

### Solution 1: Rectangle Calculator

```cpp
#include <iostream>
using namespace std;

void calculateRectangle(double length, double width, double& area, double& perimeter) {
    area = length * width;
    perimeter = 2 * (length + width);
}

int main() {
    double len, wid, area, perim;

    cout << "Enter rectangle length: ";
    cin >> len;
    cout << "Enter rectangle width: ";
    cin >> wid;

    calculateRectangle(len, wid, area, perim);

    cout << "Area: " << area << endl;
    cout << "Perimeter: " << perim << endl;

    return 0;
}
```

### Solution 2: Grade Statistics

```cpp
#include <iostream>
using namespace std;

void gradeStatistics(const int grades[], int size, int& highest, int& lowest, double& average) {
    if (size == 0) return;

    highest = grades[0];
    lowest = grades[0];
    int sum = 0;

    for (int i = 0; i < size; i++) {
        if (grades[i] > highest) highest = grades[i];
        if (grades[i] < lowest) lowest = grades[i];
        sum += grades[i];
    }

    average = static_cast<double>(sum) / size;
}

int main() {
    int grades[] = {85, 92, 78, 95, 88, 76, 90};
    int size = 7;
    int high, low;
    double avg;

    gradeStatistics(grades, size, high, low, avg);

    cout << "Highest grade: " << high << endl;
    cout << "Lowest grade: " << low << endl;
    cout << "Average grade: " << avg << endl;

    return 0;
}
```

### Solution 3: String Reversal

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void reverseString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main() {
    char text1[] = "Hello";
    char text2[] = "C++ Programming";
    char text3[] = "12345";

    cout << "Original: " << text1 << endl;
    reverseString(text1);
    cout << "Reversed: " << text1 << endl;

    cout << "\nOriginal: " << text2 << endl;
    reverseString(text2);
    cout << "Reversed: " << text2 << endl;

    cout << "\nOriginal: " << text3 << endl;
    reverseString(text3);
    cout << "Reversed: " << text3 << endl;

    return 0;
}
```

### Solution 4: Prime Checker

```cpp
#include <iostream>
#include <cmath>
using namespace std;

bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;

    int sqrtNum = sqrt(num);
    for (int i = 3; i <= sqrtNum; i += 2) {
        if (num % i == 0) return false;
    }

    return true;
}

int findPrimesInRange(int start, int end, int primes[], int maxSize) {
    int count = 0;

    for (int i = start; i <= end && count < maxSize; i++) {
        if (isPrime(i)) {
            primes[count++] = i;
        }
    }

    return count;  // Return number of primes found
}

int main() {
    int primes[100];
    int count = findPrimesInRange(1, 50, primes, 100);

    cout << "Prime numbers between 1 and 50:" << endl;
    for (int i = 0; i < count; i++) {
        cout << primes[i] << " ";
    }
    cout << endl;
    cout << "Total primes found: " << count << endl;

    return 0;
}
```

### Solution 5: Coordinate Distance

```cpp
#include <iostream>
#include <cmath>
using namespace std;

double calculateDistance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    double x1, y1, x2, y2;

    cout << "Enter first point (x1 y1): ";
    cin >> x1 >> y1;

    cout << "Enter second point (x2 y2): ";
    cin >> x2 >> y2;

    double distance = calculateDistance(x1, y1, x2, y2);

    cout << "Distance between points: " << distance << endl;

    // Test with known values
    cout << "\nTest: Distance from (0,0) to (3,4) = "
         << calculateDistance(0, 0, 3, 4) << " (should be 5)" << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Pass by value** creates a copy of the argument; changes don't affect the original
- **Pass by reference** allows functions to modify the original argument efficiently
- **Pass by pointer** provides similar functionality with explicit address passing
- **Return values** allow functions to send data back to the caller
- **Multiple return values** can be achieved using reference or pointer parameters
- **Const parameters** prevent modification and clarify intent
- **Best practices** for choosing the right parameter passing mechanism

## Checklist

- [ ] I understand the difference between pass-by-value, pass-by-reference, and pass-by-pointer
- [ ] I can use return values effectively in functions
- [ ] I can implement functions that return multiple values using references
- [ ] I understand when to use const parameters
- [ ] I can choose the appropriate parameter passing mechanism for different scenarios
- [ ] I completed all exercises and understand the solutions
- [ ] I can write functions with clear, well-designed interfaces
- [ ] I understand the dangers of returning references to local variables

## Additional Resources

- **Next Lesson:** Function Overloading
- **Related Topics:** References, Pointers, Function Design Patterns
- **Practice:** Try implementing functions for mathematical operations, string processing, and array manipulation

---

**Congratulations!** You've completed Lesson 09. You now have a solid understanding of function parameters and return values, which are essential for writing modular and efficient C++ code.
