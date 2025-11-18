# Lesson 02: Your First C++ Program
**Duration: 2 hours**

## Learning Objectives
By the end of this lesson, you will:
- Understand the structure of a C++ program
- Know the role of the main() function
- Use basic input/output operations
- Understand namespaces
- Work with comments
- Handle compilation errors
- Create multi-file programs

## 1. Anatomy of a C++ Program

Let's analyze a complete C++ program line by line:

```cpp
// hello_world.cpp
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

### Line-by-Line Breakdown

#### Line 1: Comment
```cpp
// hello_world.cpp
```
- `//` creates a single-line comment
- Comments are ignored by the compiler
- Used for documentation and explanations

#### Line 2: Include Directive
```cpp
#include <iostream>
```
- `#include` is a preprocessor directive
- `<iostream>` is a standard library header for input/output
- Angle brackets `< >` indicate a system header
- This gives us access to `std::cout` and `std::cin`

#### Line 3: Blank Line
- Improves readability
- Ignored by compiler
- Good practice to separate sections

#### Line 4: Main Function Declaration
```cpp
int main() {
```
- `int` is the return type (integer)
- `main` is the function name (special function - program entry point)
- `()` contains parameters (empty here, can have parameters)
- `{` begins the function body

#### Line 5: Output Statement
```cpp
    std::cout << "Hello, World!" << std::endl;
```
- `std::cout` is the standard output stream (console)
- `<<` is the insertion operator (sends data to stream)
- `"Hello, World!"` is a string literal
- `std::endl` is end-of-line (newline + flush buffer)
- `;` ends the statement (required!)

#### Line 6: Return Statement
```cpp
    return 0;
```
- `return` exits the function
- `0` indicates successful execution (convention)
- Non-zero values indicate errors
- In `main()`, this is sent to the operating system

#### Line 7: Closing Brace
```cpp
}
```
- Ends the function body
- Must match every opening brace

## 2. The main() Function

The `main()` function is **special** and **required**:

### Valid main() Signatures

```cpp
// Version 1: No parameters
int main() {
    return 0;
}

// Version 2: Command-line arguments
int main(int argc, char* argv[]) {
    return 0;
}

// Version 3: Alternative parameter names
int main(int argc, char** argv) {
    return 0;
}
```

### Return Values
```cpp
int main() {
    return 0;   // Success
    return 1;   // Generic error
    return -1;  // Error (platform-dependent)
    return 42;  // Custom error code
}
```

### Testing Return Values

```bash
# Linux/Mac
./program
echo $?  # Prints return value

# Windows Command Prompt
program.exe
echo %ERRORLEVEL%

# Windows PowerShell
.\program.exe
echo $LASTEXITCODE
```

## 3. Comments in Detail

### Single-Line Comments
```cpp
// This is a single-line comment
int x = 5;  // Comment can be at end of line

// Multiple single-line comments
// can be stacked
// like this
```

### Multi-Line Comments
```cpp
/*
This is a multi-line comment.
It can span multiple lines.
Useful for longer explanations.
*/

int y = 10; /* Inline multi-line comment */ int z = 20;
```

### Documentation Comments (Doxygen-style)
```cpp
/**
 * @brief Calculates the sum of two numbers
 * @param a First number
 * @param b Second number
 * @return The sum of a and b
 */
int add(int a, int b) {
    return a + b;
}
```

### Comment Best Practices
```cpp
// GOOD: Explains WHY
// Using binary search because the data is sorted
int position = binarySearch(data, target);

// BAD: Explains WHAT (code already shows this)
// Assign 5 to x
int x = 5;

// GOOD: Explains complex logic
// Fast inverse square root - Quake III algorithm
// Used for vector normalization performance
float Q_rsqrt(float number) {
    // ... implementation
}
```

## 4. Input and Output in Detail

### Output with cout

```cpp
#include <iostream>

int main() {
    // Basic output
    std::cout << "Hello";

    // Multiple items
    std::cout << "Hello" << " " << "World";

    // Variables
    int age = 25;
    std::cout << "Age: " << age;

    // Multiple lines
    std::cout << "Line 1" << std::endl;
    std::cout << "Line 2" << std::endl;

    // Alternative newline
    std::cout << "Line 1\n";
    std::cout << "Line 2\n";

    // Chaining
    std::cout << "Name: " << "John" << ", Age: " << 30 << std::endl;

    return 0;
}
```

### Input with cin

```cpp
#include <iostream>

int main() {
    int age;
    std::string name;

    // Single input
    std::cout << "Enter your age: ";
    std::cin >> age;

    // Multiple inputs
    std::cout << "Enter your name: ";
    std::cin >> name;

    // Chained input
    std::cout << "Enter two numbers: ";
    int a, b;
    std::cin >> a >> b;

    // Display
    std::cout << "Hello " << name << ", age " << age << std::endl;
    std::cout << "Numbers: " << a << " and " << b << std::endl;

    return 0;
}
```

### Complete Input/Output Example

```cpp
// io_example.cpp
#include <iostream>
#include <string>

int main() {
    // Declare variables
    std::string name;
    int age;
    double height;

    // Collect input
    std::cout << "=== User Information ===" << std::endl;

    std::cout << "Enter your name: ";
    std::cin >> name;

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Enter your height (in meters): ";
    std::cin >> height;

    // Display collected information
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << " years old" << std::endl;
    std::cout << "Height: " << height << " meters" << std::endl;

    return 0;
}
```

## 5. Namespaces

### What are Namespaces?

Namespaces prevent name conflicts by grouping related code.

```cpp
// Without namespaces - CONFLICT!
void print() { /* ... */ }  // Your function
void print() { /* ... */ }  // Library function - ERROR!

// With namespaces - NO CONFLICT
namespace MyCode {
    void print() { /* ... */ }
}

namespace Library {
    void print() { /* ... */ }
}

// Use them
MyCode::print();   // Calls your function
Library::print();  // Calls library function
```

### The std Namespace

The Standard Library uses the `std` namespace:

```cpp
#include <iostream>

int main() {
    // Full qualification (what we've been doing)
    std::cout << "Hello" << std::endl;

    return 0;
}
```

### Using Declarations

```cpp
#include <iostream>

int main() {
    // Bring specific names into scope
    using std::cout;
    using std::endl;

    cout << "Hello" << endl;  // No std:: needed

    return 0;
}
```

### Using Directives (Use Carefully!)

```cpp
#include <iostream>

// Brings EVERYTHING from std into global scope
using namespace std;

int main() {
    cout << "Hello" << endl;  // No std:: needed
    return 0;
}
```

### Namespace Best Practices

```cpp
// AVOID in header files - pollutes namespace
using namespace std;  // Don't do this in .h files!

// OK in source files for convenience
// main.cpp
using namespace std;  // Fine in .cpp files

// PREFERRED in any file
using std::cout;      // Explicit is better
using std::endl;
using std::string;

// ALWAYS SAFE - use std:: prefix
std::cout << "Explicit" << std::endl;
```

## 6. Compilation Errors and How to Fix Them

### Common Error 1: Missing Semicolon

```cpp
// ERROR
int main() {
    std::cout << "Hello"  // Missing semicolon
    return 0;
}
```

**Error Message:**
```
error: expected ';' before 'return'
```

**Fix:**
```cpp
std::cout << "Hello";  // Added semicolon
```

### Common Error 2: Missing Include

```cpp
// ERROR
int main() {
    std::cout << "Hello";  // cout not defined
    return 0;
}
```

**Error Message:**
```
error: 'cout' is not a member of 'std'
```

**Fix:**
```cpp
#include <iostream>  // Add this
```

### Common Error 3: Mismatched Braces

```cpp
// ERROR
int main() {
    if (true) {
        std::cout << "Yes";
    // Missing closing brace
    return 0;
}
```

**Error Message:**
```
error: expected '}' at end of input
```

**Fix:**
```cpp
int main() {
    if (true) {
        std::cout << "Yes";
    }  // Add this
    return 0;
}
```

### Common Error 4: Type Mismatch

```cpp
// ERROR
int main() {
    int x = "Hello";  // Can't assign string to int
    return 0;
}
```

**Error Message:**
```
error: invalid conversion from 'const char*' to 'int'
```

**Fix:**
```cpp
std::string x = "Hello";  // Use correct type
```

## 7. Complete Program Examples

### Example 1: Simple Calculator

```cpp
// simple_calculator.cpp
#include <iostream>

int main() {
    double num1, num2, result;
    char operation;

    std::cout << "=== Simple Calculator ===" << std::endl;
    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> operation;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    // Perform calculation
    if (operation == '+') {
        result = num1 + num2;
    } else if (operation == '-') {
        result = num1 - num2;
    } else if (operation == '*') {
        result = num1 * num2;
    } else if (operation == '/') {
        if (num2 != 0) {
            result = num1 / num2;
        } else {
            std::cout << "Error: Division by zero!" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Error: Invalid operation!" << std::endl;
        return 1;
    }

    std::cout << "Result: " << num1 << " " << operation << " "
              << num2 << " = " << result << std::endl;

    return 0;
}
```

### Example 2: Temperature Converter

```cpp
// temp_converter.cpp
#include <iostream>

int main() {
    double temperature;
    char unit;

    std::cout << "=== Temperature Converter ===" << std::endl;
    std::cout << "Enter temperature: ";
    std::cin >> temperature;

    std::cout << "Enter unit (C for Celsius, F for Fahrenheit): ";
    std::cin >> unit;

    if (unit == 'C' || unit == 'c') {
        double fahrenheit = (temperature * 9.0 / 5.0) + 32.0;
        std::cout << temperature << "°C = " << fahrenheit << "°F" << std::endl;
    } else if (unit == 'F' || unit == 'f') {
        double celsius = (temperature - 32.0) * 5.0 / 9.0;
        std::cout << temperature << "°F = " << celsius << "°C" << std::endl;
    } else {
        std::cout << "Error: Invalid unit!" << std::endl;
        return 1;
    }

    return 0;
}
```

### Example 3: Personal Information Display

```cpp
// personal_info.cpp
#include <iostream>
#include <string>

int main() {
    // Variables for personal information
    std::string firstName, lastName, city;
    int age, birthYear;
    double height;

    // Collect information
    std::cout << "=== Personal Information Form ===" << std::endl;
    std::cout << std::endl;

    std::cout << "First Name: ";
    std::cin >> firstName;

    std::cout << "Last Name: ";
    std::cin >> lastName;

    std::cout << "Age: ";
    std::cin >> age;

    std::cout << "Birth Year: ";
    std::cin >> birthYear;

    std::cout << "Height (in cm): ";
    std::cin >> height;

    std::cout << "City: ";
    std::cin >> city;

    // Display formatted information
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "         PERSONAL PROFILE            " << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "Name:        " << firstName << " " << lastName << std::endl;
    std::cout << "Age:         " << age << " years old" << std::endl;
    std::cout << "Birth Year:  " << birthYear << std::endl;
    std::cout << "Height:      " << height << " cm" << std::endl;
    std::cout << "Location:    " << city << std::endl;
    std::cout << "=====================================" << std::endl;

    return 0;
}
```

## 8. Exercises

### Exercise 1: Hello with Name
Write a program that:
1. Asks for the user's name
2. Greets them personally
3. Example: "Enter name: John" → "Hello, John!"

### Exercise 2: Area Calculator
Write a program that:
1. Asks for length and width of a rectangle
2. Calculates the area
3. Displays the result

### Exercise 3: Age Calculator
Write a program that:
1. Asks for current year and birth year
2. Calculates age
3. Displays result with a message

### Exercise 4: Shopping Total
Write a program that:
1. Asks for price of 3 items
2. Calculates total
3. Adds 10% tax
4. Displays subtotal, tax, and grand total

### Exercise 5: BMI Calculator
Write a program that:
1. Asks for weight (kg) and height (m)
2. Calculates BMI = weight / (height * height)
3. Displays the BMI value

## 9. Solutions to Exercises

### Solution 1: Hello with Name
```cpp
#include <iostream>
#include <string>

int main() {
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cout << "Hello, " << name << "!" << std::endl;
    return 0;
}
```

### Solution 2: Area Calculator
```cpp
#include <iostream>

int main() {
    double length, width, area;

    std::cout << "Enter length: ";
    std::cin >> length;

    std::cout << "Enter width: ";
    std::cin >> width;

    area = length * width;

    std::cout << "Area: " << area << " square units" << std::endl;

    return 0;
}
```

### Solution 3: Age Calculator
```cpp
#include <iostream>

int main() {
    int currentYear, birthYear, age;

    std::cout << "Enter current year: ";
    std::cin >> currentYear;

    std::cout << "Enter birth year: ";
    std::cin >> birthYear;

    age = currentYear - birthYear;

    std::cout << "You are " << age << " years old." << std::endl;

    return 0;
}
```

### Solution 4: Shopping Total
```cpp
#include <iostream>

int main() {
    double item1, item2, item3;
    double subtotal, tax, total;

    std::cout << "Enter price of item 1: $";
    std::cin >> item1;

    std::cout << "Enter price of item 2: $";
    std::cin >> item2;

    std::cout << "Enter price of item 3: $";
    std::cin >> item3;

    subtotal = item1 + item2 + item3;
    tax = subtotal * 0.10;
    total = subtotal + tax;

    std::cout << "\n=== Receipt ===" << std::endl;
    std::cout << "Subtotal: $" << subtotal << std::endl;
    std::cout << "Tax (10%): $" << tax << std::endl;
    std::cout << "Total: $" << total << std::endl;

    return 0;
}
```

### Solution 5: BMI Calculator
```cpp
#include <iostream>

int main() {
    double weight, height, bmi;

    std::cout << "Enter weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter height (m): ";
    std::cin >> height;

    bmi = weight / (height * height);

    std::cout << "Your BMI is: " << bmi << std::endl;

    return 0;
}
```

## 10. Summary

### Key Concepts
- Every C++ program needs a `main()` function
- `#include` brings in library functionality
- `std::cout` for output, `std::cin` for input
- `<<` operator sends data to output stream
- `>>` operator reads data from input stream
- Statements end with semicolons
- Comments use `//` or `/* */`
- Namespaces prevent name conflicts
- `return 0;` indicates success

### Best Practices
- Use meaningful variable names
- Include appropriate headers
- Comment complex logic
- Check for input errors
- Use consistent indentation
- One statement per line (usually)
- Prefer `std::` prefix over `using namespace std`

## Checklist

- [ ] Understand main() function structure
- [ ] Can use std::cout for output
- [ ] Can use std::cin for input
- [ ] Know how to write comments
- [ ] Understand namespaces basics
- [ ] Can compile and run programs
- [ ] Can fix common compilation errors
- [ ] Completed all exercises

---

**Time to complete**: 2 hours
**Previous lesson**: [Lesson 01 - Introduction and Setup](../Lesson-01-Introduction-Setup/README.md)
**Next lesson**: [Lesson 03 - Variables and Data Types](../Lesson-03-Variables-DataTypes/README.md)
