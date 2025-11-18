# Lesson 03: Control Flow (if, switch, loops)
**Estimated Time: 4 hours**

## Table of Contents
1. [Introduction to Control Flow](#introduction-to-control-flow)
2. [if Statements](#if-statements)
3. [else and else if](#else-and-else-if)
4. [Nested if Statements](#nested-if-statements)
5. [switch Statements](#switch-statements)
6. [while Loops](#while-loops)
7. [do-while Loops](#do-while-loops)
8. [for Loops](#for-loops)
9. [Nested Loops](#nested-loops)
10. [break and continue](#break-and-continue)
11. [Complete Examples](#complete-examples)
12. [Exercises](#exercises)

## Introduction to Control Flow

Control flow statements allow your program to make decisions and repeat actions. Without them, your code would only execute line by line from top to bottom.

### Types of Control Flow:
1. **Selection**: if, else, switch (choose between alternatives)
2. **Iteration**: while, for, do-while (repeat actions)
3. **Jump**: break, continue, return (alter flow)

## if Statements

### Basic Syntax

```cpp
if (condition)
{
    // Code executes if condition is true
}
```

### Simple Example

```cpp
#include <iostream>

int main()
{
    int age = 20;

    if (age >= 18)
    {
        std::cout << "You are an adult.\n";
    }

    std::cout << "Program continues...\n";
    return 0;
}
```

### Without Braces (Single Statement)

```cpp
if (age >= 18)
    std::cout << "Adult\n";  // Only this line is in the if

// NOT RECOMMENDED: Easy to make mistakes
```

### Comparison Operators Reminder

```cpp
int x = 10;

if (x == 10)   // Equal to
if (x != 5)    // Not equal to
if (x > 5)     // Greater than
if (x < 20)    // Less than
if (x >= 10)   // Greater than or equal to
if (x <= 10)   // Less than or equal to
```

## else and else if

### if-else

```cpp
int age = 15;

if (age >= 18)
{
    std::cout << "You can vote.\n";
}
else
{
    std::cout << "You cannot vote yet.\n";
}
```

### if-else if-else

```cpp
int score = 85;

if (score >= 90)
{
    std::cout << "Grade: A\n";
}
else if (score >= 80)
{
    std::cout << "Grade: B\n";
}
else if (score >= 70)
{
    std::cout << "Grade: C\n";
}
else if (score >= 60)
{
    std::cout << "Grade: D\n";
}
else
{
    std::cout << "Grade: F\n";
}
```

### Logical Operators

```cpp
int age = 25;
bool hasLicense = true;

// AND (&&) - both must be true
if (age >= 18 && hasLicense)
{
    std::cout << "You can drive.\n";
}

// OR (||) - at least one must be true
if (age < 18 || !hasLicense)
{
    std::cout << "You cannot drive.\n";
}

// NOT (!) - inverts the condition
if (!hasLicense)
{
    std::cout << "No license.\n";
}
```

### Complex Conditions

```cpp
int age = 25;
double income = 50000;
bool employed = true;

if ((age >= 18 && age <= 65) && (income > 30000 || employed))
{
    std::cout << "Loan approved!\n";
}
else
{
    std::cout << "Loan denied.\n";
}
```

## Nested if Statements

### Basic Nesting

```cpp
int age = 20;
bool hasTicket = true;

if (age >= 18)
{
    if (hasTicket)
    {
        std::cout << "You can enter the concert.\n";
    }
    else
    {
        std::cout << "You need a ticket.\n";
    }
}
else
{
    std::cout << "You're too young.\n";
}
```

### Deep Nesting Example

```cpp
int score = 95;
bool extraCredit = true;

if (score >= 90)
{
    if (extraCredit)
    {
        std::cout << "Grade: A+ (with extra credit)\n";
    }
    else
    {
        std::cout << "Grade: A\n";
    }
}
else if (score >= 80)
{
    std::cout << "Grade: B\n";
}
else
{
    std::cout << "Keep studying!\n";
}
```

## switch Statements

### Basic Syntax

```cpp
switch (expression)
{
    case value1:
        // Code for value1
        break;
    case value2:
        // Code for value2
        break;
    default:
        // Code if no case matches
        break;
}
```

### Simple Example

```cpp
#include <iostream>

int main()
{
    int day = 3;

    switch (day)
    {
        case 1:
            std::cout << "Monday\n";
            break;
        case 2:
            std::cout << "Tuesday\n";
            break;
        case 3:
            std::cout << "Wednesday\n";
            break;
        case 4:
            std::cout << "Thursday\n";
            break;
        case 5:
            std::cout << "Friday\n";
            break;
        case 6:
            std::cout << "Saturday\n";
            break;
        case 7:
            std::cout << "Sunday\n";
            break;
        default:
            std::cout << "Invalid day\n";
            break;
    }

    return 0;
}
```

### Fall-Through Behavior

```cpp
char grade = 'B';

switch (grade)
{
    case 'A':
    case 'a':
        std::cout << "Excellent!\n";
        break;
    case 'B':
    case 'b':
        std::cout << "Good job!\n";
        break;
    case 'C':
    case 'c':
        std::cout << "Satisfactory.\n";
        break;
    default:
        std::cout << "Need improvement.\n";
        break;
}
```

### Multiple Statements Per Case

```cpp
int choice = 2;

switch (choice)
{
    case 1:
        std::cout << "You selected option 1\n";
        std::cout << "Processing option 1...\n";
        std::cout << "Option 1 complete!\n";
        break;
    case 2:
        std::cout << "You selected option 2\n";
        std::cout << "Processing option 2...\n";
        std::cout << "Option 2 complete!\n";
        break;
    default:
        std::cout << "Invalid choice\n";
        break;
}
```

## while Loops

### Basic Syntax

```cpp
while (condition)
{
    // Code to repeat
}
```

### Simple Counting

```cpp
#include <iostream>

int main()
{
    int count = 1;

    while (count <= 5)
    {
        std::cout << "Count: " << count << "\n";
        count++;  // IMPORTANT: Must update condition variable
    }

    std::cout << "Done!\n";
    return 0;
}
```

Output:
```
Count: 1
Count: 2
Count: 3
Count: 4
Count: 5
Done!
```

### Input Validation

```cpp
#include <iostream>

int main()
{
    int number;

    std::cout << "Enter a positive number: ";
    std::cin >> number;

    while (number <= 0)
    {
        std::cout << "Invalid! Must be positive.\n";
        std::cout << "Try again: ";
        std::cin >> number;
    }

    std::cout << "Thank you! You entered: " << number << "\n";
    return 0;
}
```

### Sentinel Value Loop

```cpp
#include <iostream>

int main()
{
    int sum = 0;
    int number;

    std::cout << "Enter numbers (0 to stop):\n";

    std::cin >> number;
    while (number != 0)
    {
        sum += number;
        std::cin >> number;
    }

    std::cout << "Sum: " << sum << "\n";
    return 0;
}
```

## do-while Loops

### Basic Syntax

```cpp
do
{
    // Code executes at least once
} while (condition);
```

### Key Difference from while
- `while`: Checks condition BEFORE executing
- `do-while`: Checks condition AFTER executing (runs at least once)

### Example

```cpp
#include <iostream>

int main()
{
    int choice;

    do
    {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Option 1\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Option 3\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                std::cout << "You chose option 1\n";
                break;
            case 2:
                std::cout << "You chose option 2\n";
                break;
            case 3:
                std::cout << "You chose option 3\n";
                break;
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
```

## for Loops

### Basic Syntax

```cpp
for (initialization; condition; update)
{
    // Code to repeat
}
```

### Components:
1. **Initialization**: Runs once at start
2. **Condition**: Checked before each iteration
3. **Update**: Runs after each iteration

### Simple Counting

```cpp
#include <iostream>

int main()
{
    // Count from 1 to 10
    for (int i = 1; i <= 10; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

Output: `1 2 3 4 5 6 7 8 9 10`

### Counting Backward

```cpp
for (int i = 10; i >= 1; i--)
{
    std::cout << i << " ";
}
std::cout << "Blastoff!\n";
```

### Different Step Sizes

```cpp
// Count by 2s
for (int i = 0; i <= 20; i += 2)
{
    std::cout << i << " ";
}
std::cout << "\n";

// Count by 5s
for (int i = 0; i <= 50; i += 5)
{
    std::cout << i << " ";
}
std::cout << "\n";
```

### Multiple Variables

```cpp
for (int i = 0, j = 10; i < j; i++, j--)
{
    std::cout << "i: " << i << ", j: " << j << "\n";
}
```

## Nested Loops

### Multiplication Table

```cpp
#include <iostream>

int main()
{
    std::cout << "Multiplication Table (1-10)\n\n";

    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            std::cout << (i * j) << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Pattern Printing - Triangle

```cpp
#include <iostream>

int main()
{
    int rows = 5;

    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

Output:
```
*
* *
* * *
* * * *
* * * * *
```

### Pattern Printing - Rectangle

```cpp
int width = 10;
int height = 5;

for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        std::cout << "# ";
    }
    std::cout << "\n";
}
```

## break and continue

### break Statement

Exits the loop immediately.

```cpp
// Find first number divisible by 7
for (int i = 1; i <= 100; i++)
{
    if (i % 7 == 0)
    {
        std::cout << "First number divisible by 7: " << i << "\n";
        break;  // Exit loop
    }
}
```

### continue Statement

Skips rest of current iteration, continues with next.

```cpp
// Print odd numbers only
for (int i = 1; i <= 10; i++)
{
    if (i % 2 == 0)
    {
        continue;  // Skip even numbers
    }
    std::cout << i << " ";
}
std::cout << "\n";
```

Output: `1 3 5 7 9`

### break in Nested Loops

```cpp
bool found = false;
for (int i = 0; i < 10 && !found; i++)
{
    for (int j = 0; j < 10; j++)
    {
        if (i * j == 24)
        {
            std::cout << "Found: " << i << " * " << j << " = 24\n";
            found = true;
            break;  // Exits inner loop only
        }
    }
}
```

## Complete Examples

### Example 1: Number Guessing Game

```cpp
#include <iostream>

int main()
{
    const int SECRET_NUMBER = 42;
    int guess;
    int attempts = 0;
    const int MAX_ATTEMPTS = 5;

    std::cout << "=== Number Guessing Game ===\n";
    std::cout << "Guess a number between 1 and 100\n";
    std::cout << "You have " << MAX_ATTEMPTS << " attempts.\n\n";

    while (attempts < MAX_ATTEMPTS)
    {
        std::cout << "Attempt " << (attempts + 1) << ": ";
        std::cin >> guess;
        attempts++;

        if (guess == SECRET_NUMBER)
        {
            std::cout << "Congratulations! You won!\n";
            std::cout << "It took you " << attempts << " attempts.\n";
            break;
        }
        else if (guess < SECRET_NUMBER)
        {
            std::cout << "Too low!\n";
        }
        else
        {
            std::cout << "Too high!\n";
        }

        if (attempts == MAX_ATTEMPTS)
        {
            std::cout << "\nGame Over! The number was " << SECRET_NUMBER << "\n";
        }
    }

    return 0;
}
```

### Example 2: Prime Number Checker

```cpp
#include <iostream>

int main()
{
    int number;

    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number <= 1)
    {
        std::cout << number << " is not prime.\n";
        return 0;
    }

    bool isPrime = true;

    for (int i = 2; i * i <= number; i++)
    {
        if (number % i == 0)
        {
            isPrime = false;
            break;
        }
    }

    if (isPrime)
    {
        std::cout << number << " is prime.\n";
    }
    else
    {
        std::cout << number << " is not prime.\n";
    }

    return 0;
}
```

### Example 3: Factorial Calculator

```cpp
#include <iostream>

int main()
{
    int n;

    std::cout << "Enter a number: ";
    std::cin >> n;

    if (n < 0)
    {
        std::cout << "Factorial is not defined for negative numbers.\n";
        return 0;
    }

    long long factorial = 1;

    for (int i = 1; i <= n; i++)
    {
        factorial *= i;
    }

    std::cout << n << "! = " << factorial << "\n";

    return 0;
}
```

### Example 4: Fibonacci Sequence

```cpp
#include <iostream>

int main()
{
    int n;

    std::cout << "How many Fibonacci numbers to generate? ";
    std::cin >> n;

    if (n <= 0)
    {
        std::cout << "Please enter a positive number.\n";
        return 0;
    }

    long long first = 0, second = 1;

    std::cout << "Fibonacci sequence:\n";

    for (int i = 1; i <= n; i++)
    {
        std::cout << first << " ";

        long long next = first + second;
        first = second;
        second = next;
    }

    std::cout << "\n";

    return 0;
}
```

### Example 5: Simple Calculator with Menu

```cpp
#include <iostream>

int main()
{
    int choice;
    double num1, num2;

    do
    {
        std::cout << "\n=== Calculator ===\n";
        std::cout << "1. Addition\n";
        std::cout << "2. Subtraction\n";
        std::cout << "3. Multiplication\n";
        std::cout << "4. Division\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 0)
        {
            std::cout << "Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 4)
        {
            std::cout << "Invalid choice!\n";
            continue;
        }

        std::cout << "Enter first number: ";
        std::cin >> num1;
        std::cout << "Enter second number: ";
        std::cin >> num2;

        switch (choice)
        {
            case 1:
                std::cout << "Result: " << (num1 + num2) << "\n";
                break;
            case 2:
                std::cout << "Result: " << (num1 - num2) << "\n";
                break;
            case 3:
                std::cout << "Result: " << (num1 * num2) << "\n";
                break;
            case 4:
                if (num2 != 0)
                {
                    std::cout << "Result: " << (num1 / num2) << "\n";
                }
                else
                {
                    std::cout << "Error: Division by zero!\n";
                }
                break;
        }

    } while (true);

    return 0;
}
```

## Common Mistakes

### 1. Infinite Loop

```cpp
// WRONG - never updates i
int i = 0;
while (i < 10)
{
    std::cout << i << "\n";
    // Missing: i++;
}

// CORRECT
int i = 0;
while (i < 10)
{
    std::cout << i << "\n";
    i++;
}
```

### 2. Off-by-One Error

```cpp
// WRONG - prints 0 to 9 (10 numbers, not 11)
for (int i = 0; i < 10; i++)
{
    std::cout << i << "\n";
}

// CORRECT - prints 0 to 10 (11 numbers)
for (int i = 0; i <= 10; i++)
{
    std::cout << i << "\n";
}
```

### 3. Missing break in switch

```cpp
// WRONG - falls through
switch (day)
{
    case 1:
        std::cout << "Monday\n";
        // Missing break - will also print Tuesday!
    case 2:
        std::cout << "Tuesday\n";
        break;
}

// CORRECT
switch (day)
{
    case 1:
        std::cout << "Monday\n";
        break;
    case 2:
        std::cout << "Tuesday\n";
        break;
}
```

### 4. Using = instead of ==

```cpp
// WRONG - assigns 10 to x, then evaluates as true
if (x = 10)
{
    // Always executes
}

// CORRECT
if (x == 10)
{
    // Executes only when x is 10
}
```

## Exercises

### Exercise 1: Grade Calculator
Write a program that:
1. Asks for 5 test scores
2. Calculates the average
3. Displays letter grade based on average

### Exercise 2: Number Pattern
Print this pattern:
```
1
22
333
4444
55555
```

### Exercise 3: Prime Numbers in Range
Print all prime numbers between 1 and 100.

### Exercise 4: Multiplication Quiz
Create a quiz that:
1. Generates 10 random multiplication problems
2. Keeps score
3. Displays final score

Hint: Use `rand()` for random numbers (we'll cover this properly later)

### Exercise 5: Power Calculator
Write a program that calculates x^n (x to the power of n) using a loop.
Don't use the pow() function.

## Summary

You learned:
- ✅ if, else, and else if statements
- ✅ switch statements for multiple choices
- ✅ while loops for condition-based repetition
- ✅ do-while loops that execute at least once
- ✅ for loops for counter-based repetition
- ✅ Nested loops for multi-dimensional iterations
- ✅ break and continue for loop control
- ✅ Common pitfalls and how to avoid them

## Next Lesson Preview

In Lesson 04, we'll explore:
- Functions and modular programming
- Function parameters and return values
- Function overloading
- Default parameters
- Inline functions

---

**Congratulations on completing Lesson 03!**

**Practice time**: Spend 3 hours creating programs that use different types of loops and conditional statements. Try to solve real problems like calculators, games, or pattern generators.
