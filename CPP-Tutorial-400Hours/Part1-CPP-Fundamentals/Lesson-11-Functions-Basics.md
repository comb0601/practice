# Lesson 11: Functions Basics

**Duration**: 5 hours
**Difficulty**: Beginner to Intermediate

## Table of Contents
1. Introduction to Functions
2. Function Syntax and Structure
3. Function Parameters
4. Return Values
5. Function Prototypes
6. Void Functions
7. Scope and Lifetime
8. Exercises and Projects

---

## 1. Introduction to Functions

Functions are reusable blocks of code that perform specific tasks. They help organize code, reduce repetition, and make programs more maintainable.

### Why Use Functions:

```cpp
#include <iostream>

// Without functions - repetitive code
void badExample()
{
    std::cout << "==========\n";
    std::cout << "Welcome!\n";
    std::cout << "==========\n";

    // ...later in code...
    std::cout << "==========\n";
    std::cout << "Goodbye!\n";
    std::cout << "==========\n";
}

// With functions - reusable code
void printBorder()
{
    std::cout << "==========\n";
}

void goodExample()
{
    printBorder();
    std::cout << "Welcome!\n";
    printBorder();

    // ...later in code...
    printBorder();
    std::cout << "Goodbye!\n";
    printBorder();
}

int main()
{
    goodExample();
    return 0;
}
```

---

## 2. Function Syntax and Structure

### Basic Function Structure:

```cpp
#include <iostream>

// Function definition
void greet()
{
    std::cout << "Hello, World!\n";
}

int main()
{
    // Function call
    greet();
    greet();
    greet();

    return 0;
}
```

**Output**:
```
Hello, World!
Hello, World!
Hello, World!
```

### Function Components:

```cpp
#include <iostream>

// returnType functionName(parameters)
// {
//     function body
//     return value;
// }

int add(int a, int b)  // Header
{                       // Opening brace
    int result = a + b; // Function body
    return result;      // Return statement
}                       // Closing brace

int main()
{
    int sum = add(5, 3);  // Function call
    std::cout << "5 + 3 = " << sum << "\n";

    return 0;
}
```

---

## 3. Function Parameters

### Single Parameter:

```cpp
#include <iostream>

void printSquare(int number)
{
    int square = number * number;
    std::cout << number << " squared = " << square << "\n";
}

int main()
{
    printSquare(5);
    printSquare(10);
    printSquare(7);

    return 0;
}
```

### Multiple Parameters:

```cpp
#include <iostream>

void printRectangle(int width, int height)
{
    std::cout << "\nRectangle " << width << "x" << height << ":\n";

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }
}

int main()
{
    printRectangle(5, 3);
    printRectangle(8, 2);

    return 0;
}
```

### Different Data Types:

```cpp
#include <iostream>
#include <string>

void displayInfo(std::string name, int age, double height)
{
    std::cout << "\n===== PROFILE =====\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Age: " << age << " years\n";
    std::cout << "Height: " << height << " cm\n";
}

int main()
{
    displayInfo("Alice", 25, 165.5);
    displayInfo("Bob", 30, 180.0);

    return 0;
}
```

---

## 4. Return Values

### Returning a Value:

```cpp
#include <iostream>

int multiply(int a, int b)
{
    return a * b;
}

double divide(double a, double b)
{
    if (b != 0)
        return a / b;
    else
        return 0.0;
}

bool isEven(int number)
{
    return (number % 2 == 0);
}

int main()
{
    int product = multiply(6, 7);
    std::cout << "6 * 7 = " << product << "\n";

    double quotient = divide(15.0, 3.0);
    std::cout << "15.0 / 3.0 = " << quotient << "\n";

    bool even = isEven(10);
    std::cout << "Is 10 even? " << (even ? "Yes" : "No") << "\n";

    return 0;
}
```

### Using Return Values:

```cpp
#include <iostream>

int getMax(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int main()
{
    int x = 10, y = 20;

    // Store return value
    int maximum = getMax(x, y);
    std::cout << "Max of " << x << " and " << y << " = " << maximum << "\n";

    // Use directly
    std::cout << "Max of 5 and 15 = " << getMax(5, 15) << "\n";

    // Use in expression
    int result = getMax(100, 50) + 10;
    std::cout << "Max(100, 50) + 10 = " << result << "\n";

    return 0;
}
```

### Early Return:

```cpp
#include <iostream>

int factorial(int n)
{
    if (n < 0)
        return -1;  // Error code for negative input

    if (n == 0 || n == 1)
        return 1;  // Base case

    int result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }

    return result;
}

int main()
{
    std::cout << "5! = " << factorial(5) << "\n";
    std::cout << "0! = " << factorial(0) << "\n";
    std::cout << "-5! = " << factorial(-5) << " (error)\n";

    return 0;
}
```

---

## 5. Function Prototypes

### What is a Prototype:

```cpp
#include <iostream>

// Function prototype (declaration)
int add(int a, int b);
void greet();

int main()
{
    int sum = add(5, 3);
    std::cout << "Sum: " << sum << "\n";

    greet();

    return 0;
}

// Function definitions (can be after main)
int add(int a, int b)
{
    return a + b;
}

void greet()
{
    std::cout << "Hello from function!\n";
}
```

### Why Use Prototypes:

```cpp
#include <iostream>

// Prototypes allow functions to call each other
void functionA();
void functionB();

int main()
{
    functionA();
    return 0;
}

void functionA()
{
    std::cout << "In Function A\n";
    functionB();  // Can call B even though it's defined later
}

void functionB()
{
    std::cout << "In Function B\n";
}
```

---

## 6. Void Functions

### No Return Value:

```cpp
#include <iostream>

void printLine()
{
    std::cout << "------------------------\n";
}

void displayMenu()
{
    printLine();
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Options\n";
    std::cout << "4. Exit\n";
    printLine();
}

int main()
{
    displayMenu();
    return 0;
}
```

### Void with Parameters:

```cpp
#include <iostream>
#include <string>

void printNTimes(std::string message, int times)
{
    for (int i = 0; i < times; i++)
    {
        std::cout << message << "\n";
    }
}

void countdown(int start)
{
    for (int i = start; i >= 1; i--)
    {
        std::cout << i << "...\n";
    }
    std::cout << "GO!\n";
}

int main()
{
    printNTimes("Hello!", 3);
    std::cout << "\n";
    countdown(5);

    return 0;
}
```

---

## 7. Scope and Lifetime

### Local Variables:

```cpp
#include <iostream>

void function1()
{
    int x = 10;  // Local to function1
    std::cout << "function1: x = " << x << "\n";
}

void function2()
{
    int x = 20;  // Different x, local to function2
    std::cout << "function2: x = " << x << "\n";
}

int main()
{
    int x = 5;  // Local to main
    std::cout << "main: x = " << x << "\n";

    function1();
    function2();

    std::cout << "main: x still = " << x << "\n";

    return 0;
}
```

**Output**:
```
main: x = 5
function1: x = 10
function2: x = 20
main: x still = 5
```

### Global vs Local:

```cpp
#include <iostream>

int globalVar = 100;  // Global variable

void modifyGlobal()
{
    globalVar = 200;
    std::cout << "Inside function: globalVar = " << globalVar << "\n";
}

void useLocal()
{
    int localVar = 50;
    std::cout << "Local variable: " << localVar << "\n";
    std::cout << "Can also access global: " << globalVar << "\n";
}

int main()
{
    std::cout << "Initial globalVar: " << globalVar << "\n";

    modifyGlobal();
    std::cout << "After modifyGlobal: " << globalVar << "\n";

    useLocal();

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Calculator Functions

```cpp
#include <iostream>

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    if (b != 0)
        return a / b;
    else
    {
        std::cout << "Error: Division by zero!\n";
        return 0;
    }
}

int main()
{
    double num1, num2;
    char op;

    std::cout << "Simple Calculator\n";
    std::cout << "Enter expression (e.g., 5 + 3): ";
    std::cin >> num1 >> op >> num2;

    double result;

    switch (op)
    {
        case '+':
            result = add(num1, num2);
            break;
        case '-':
            result = subtract(num1, num2);
            break;
        case '*':
            result = multiply(num1, num2);
            break;
        case '/':
            result = divide(num1, num2);
            break;
        default:
            std::cout << "Invalid operator!\n";
            return 1;
    }

    std::cout << num1 << " " << op << " " << num2 << " = " << result << "\n";

    return 0;
}
```

### Exercise 2: Temperature Converter

```cpp
#include <iostream>

double celsiusToFahrenheit(double celsius)
{
    return (celsius * 9.0 / 5.0) + 32.0;
}

double fahrenheitToCelsius(double fahrenheit)
{
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double celsiusToKelvin(double celsius)
{
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin)
{
    return kelvin - 273.15;
}

void displayMenu()
{
    std::cout << "\n===== TEMPERATURE CONVERTER =====\n";
    std::cout << "1. Celsius to Fahrenheit\n";
    std::cout << "2. Fahrenheit to Celsius\n";
    std::cout << "3. Celsius to Kelvin\n";
    std::cout << "4. Kelvin to Celsius\n";
    std::cout << "Enter choice: ";
}

int main()
{
    int choice;
    double temp, result;

    displayMenu();
    std::cin >> choice;

    std::cout << "Enter temperature: ";
    std::cin >> temp;

    switch (choice)
    {
        case 1:
            result = celsiusToFahrenheit(temp);
            std::cout << temp << "°C = " << result << "°F\n";
            break;
        case 2:
            result = fahrenheitToCelsius(temp);
            std::cout << temp << "°F = " << result << "°C\n";
            break;
        case 3:
            result = celsiusToKelvin(temp);
            std::cout << temp << "°C = " << result << "K\n";
            break;
        case 4:
            result = kelvinToCelsius(temp);
            std::cout << temp << "K = " << result << "°C\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
    }

    return 0;
}
```

### Exercise 3: Number Analysis

```cpp
#include <iostream>

bool isPrime(int n)
{
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

bool isPerfect(int n)
{
    int sum = 0;
    for (int i = 1; i <= n / 2; i++)
    {
        if (n % i == 0)
            sum += i;
    }
    return sum == n;
}

bool isArmstrong(int n)
{
    int original = n;
    int sum = 0;
    int digits = 0;

    // Count digits
    int temp = n;
    while (temp > 0)
    {
        digits++;
        temp /= 10;
    }

    // Calculate sum of powers
    temp = n;
    while (temp > 0)
    {
        int digit = temp % 10;
        int power = 1;
        for (int i = 0; i < digits; i++)
            power *= digit;
        sum += power;
        temp /= 10;
    }

    return sum == original;
}

void analyzeNumber(int n)
{
    std::cout << "\n===== ANALYSIS OF " << n << " =====\n";
    std::cout << "Prime: " << (isPrime(n) ? "Yes" : "No") << "\n";
    std::cout << "Perfect: " << (isPerfect(n) ? "Yes" : "No") << "\n";
    std::cout << "Armstrong: " << (isArmstrong(n) ? "Yes" : "No") << "\n";
    std::cout << "Even: " << (n % 2 == 0 ? "Yes" : "No") << "\n";
}

int main()
{
    int number;

    std::cout << "Enter a number to analyze: ";
    std::cin >> number;

    analyzeNumber(number);

    return 0;
}
```

### Exercise 4: Area Calculators

```cpp
#include <iostream>
#include <cmath>

double areaCircle(double radius)
{
    return M_PI * radius * radius;
}

double areaRectangle(double length, double width)
{
    return length * width;
}

double areaTriangle(double base, double height)
{
    return 0.5 * base * height;
}

double areaSquare(double side)
{
    return side * side;
}

int main()
{
    int choice;

    std::cout << "===== AREA CALCULATOR =====\n";
    std::cout << "1. Circle\n";
    std::cout << "2. Rectangle\n";
    std::cout << "3. Triangle\n";
    std::cout << "4. Square\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    double area;

    switch (choice)
    {
        case 1:
        {
            double radius;
            std::cout << "Enter radius: ";
            std::cin >> radius;
            area = areaCircle(radius);
            std::cout << "Area of circle: " << area << "\n";
            break;
        }
        case 2:
        {
            double length, width;
            std::cout << "Enter length: ";
            std::cin >> length;
            std::cout << "Enter width: ";
            std::cin >> width;
            area = areaRectangle(length, width);
            std::cout << "Area of rectangle: " << area << "\n";
            break;
        }
        case 3:
        {
            double base, height;
            std::cout << "Enter base: ";
            std::cin >> base;
            std::cout << "Enter height: ";
            std::cin >> height;
            area = areaTriangle(base, height);
            std::cout << "Area of triangle: " << area << "\n";
            break;
        }
        case 4:
        {
            double side;
            std::cout << "Enter side: ";
            std::cin >> side;
            area = areaSquare(side);
            std::cout << "Area of square: " << area << "\n";
            break;
        }
        default:
            std::cout << "Invalid choice!\n";
    }

    return 0;
}
```

### Exercise 5: Validation Functions

```cpp
#include <iostream>
#include <string>

bool isValidAge(int age)
{
    return age >= 0 && age <= 120;
}

bool isValidScore(double score)
{
    return score >= 0.0 && score <= 100.0;
}

bool isValidEmail(std::string email)
{
    bool hasAt = false;
    bool hasDot = false;
    int atPos = -1;

    for (int i = 0; i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            if (hasAt) return false;  // Multiple @
            hasAt = true;
            atPos = i;
        }
        if (email[i] == '.' && i > atPos + 1)
            hasDot = true;
    }

    return hasAt && hasDot && atPos > 0;
}

int getValidAge()
{
    int age;
    do
    {
        std::cout << "Enter age (0-120): ";
        std::cin >> age;

        if (!isValidAge(age))
            std::cout << "Invalid age!\n";

    } while (!isValidAge(age));

    return age;
}

int main()
{
    std::cout << "===== VALIDATION DEMO =====\n\n";

    int age = getValidAge();
    std::cout << "Valid age entered: " << age << "\n\n";

    std::string testEmails[] = {
        "user@example.com",
        "invalid.email",
        "@example.com",
        "user@domain."
    };

    std::cout << "Email validation:\n";
    for (auto email : testEmails)
    {
        std::cout << email << ": "
                  << (isValidEmail(email) ? "Valid" : "Invalid") << "\n";
    }

    return 0;
}
```

---

## Practice Project: Quiz Game with Functions

```cpp
#include <iostream>
#include <string>

void displayWelcome()
{
    std::cout << "========================================\n";
    std::cout << "      WELCOME TO THE QUIZ GAME!        \n";
    std::cout << "========================================\n\n";
}

bool askQuestion(std::string question, std::string options[], char correct)
{
    std::cout << question << "\n";
    for (int i = 0; i < 4; i++)
    {
        char letter = 'A' + i;
        std::cout << letter << ". " << options[i] << "\n";
    }

    char answer;
    std::cout << "Your answer: ";
    std::cin >> answer;
    answer = toupper(answer);

    if (answer == correct)
    {
        std::cout << "✓ Correct!\n\n";
        return true;
    }
    else
    {
        std::cout << "✗ Wrong! Correct answer: " << correct << "\n\n";
        return false;
    }
}

void displayResults(int score, int total)
{
    std::cout << "========================================\n";
    std::cout << "           QUIZ RESULTS                \n";
    std::cout << "========================================\n";
    std::cout << "Score: " << score << " / " << total << "\n";

    double percentage = (score * 100.0) / total;
    std::cout << "Percentage: " << percentage << "%\n";

    if (percentage >= 90)
        std::cout << "Grade: A (Excellent!)\n";
    else if (percentage >= 80)
        std::cout << "Grade: B (Great!)\n";
    else if (percentage >= 70)
        std::cout << "Grade: C (Good)\n";
    else if (percentage >= 60)
        std::cout << "Grade: D (Pass)\n";
    else
        std::cout << "Grade: F (Fail)\n";

    std::cout << "========================================\n";
}

int main()
{
    displayWelcome();

    int score = 0;
    int totalQuestions = 5;

    // Question 1
    std::string q1Options[] = {"Tokyo", "Beijing", "Seoul", "Bangkok"};
    if (askQuestion("What is the capital of Japan?", q1Options, 'A'))
        score++;

    // Question 2
    std::string q2Options[] = {"Mars", "Venus", "Jupiter", "Saturn"};
    if (askQuestion("Which is the largest planet?", q2Options, 'C'))
        score++;

    // Question 3
    std::string q3Options[] = {"Au", "Ag", "Fe", "Cu"};
    if (askQuestion("What is the chemical symbol for gold?", q3Options, 'A'))
        score++;

    // Question 4
    std::string q4Options[] = {"1912", "1920", "1945", "1939"};
    if (askQuestion("In what year did World War II begin?", q4Options, 'D'))
        score++;

    // Question 5
    std::string q5Options[] = {"Pacific", "Atlantic", "Indian", "Arctic"};
    if (askQuestion("What is the largest ocean?", q5Options, 'A'))
        score++;

    displayResults(score, totalQuestions);

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Missing Return
```cpp
❌ int getValue() {
    int x = 10;
    // Forgot return!
}

✅ int getValue() {
    int x = 10;
    return x;
}
```

### Mistake 2: Wrong Parameter Order
```cpp
❌ divide(10, 2);  // If function expects (denominator, numerator)

✅ divide(2, 10);  // Correct order
```

### Mistake 3: Function Before Declaration
```cpp
❌ int main() {
    myFunction();  // Error: myFunction not declared yet
}

void myFunction() {
    // ...
}

✅ void myFunction();  // Prototype

int main() {
    myFunction();  // OK
}

void myFunction() {
    // ...
}
```

---

## Key Takeaways

1. ✅ Functions organize code into reusable blocks
2. ✅ Parameters pass data into functions
3. ✅ Return values send data back from functions
4. ✅ Prototypes declare functions before use
5. ✅ Void functions don't return a value
6. ✅ Local variables exist only within their function
7. ✅ Functions make code more maintainable
8. ✅ Good function names describe their purpose

---

## Summary Checklist

Before moving to Lesson 12, ensure you can:
- [ ] Write function definitions
- [ ] Use function parameters correctly
- [ ] Return values from functions
- [ ] Write function prototypes
- [ ] Create void functions
- [ ] Understand variable scope
- [ ] Call functions properly
- [ ] Organize code with functions

---

## Next Lesson Preview

In **Lesson 12**, we'll learn about:
- Pass by value vs pass by reference
- Default parameters
- Function documentation
- Best practices for parameters
- Return value patterns

**Great work on function basics!** 📋
