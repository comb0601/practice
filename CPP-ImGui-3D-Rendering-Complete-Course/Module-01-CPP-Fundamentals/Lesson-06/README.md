# Lesson 06: Loops - for, while, do-while
**Duration: 3 hours**

## Learning Objectives
- Master the for loop for counted iterations
- Use while loops for condition-based repetition
- Understand do-while loops
- Choose the right loop for each situation
- Work with loop counters and ranges
- Avoid infinite loops
- Understand loop performance

## 1. The for Loop

### Basic Syntax

```cpp
for (initialization; condition; update) {
    // Loop body - executes while condition is true
}
```

### Your First for Loop

```cpp
#include <iostream>

int main() {
    // Print numbers 1 to 10
    for (int i = 1; i <= 10; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 2 3 4 5 6 7 8 9 10

    return 0;
}
```

### How for Loop Works

```cpp
#include <iostream>

int main() {
    // Step-by-step breakdown:
    // 1. int i = 0      - Execute once (initialization)
    // 2. i < 5          - Check condition
    // 3. Loop body      - Execute if condition true
    // 4. i++            - Update
    // 5. Go to step 2   - Repeat

    for (int i = 0; i < 5; i++) {
        std::cout << "Loop iteration: " << i << std::endl;
    }

    // Output:
    // Loop iteration: 0
    // Loop iteration: 1
    // Loop iteration: 2
    // Loop iteration: 3
    // Loop iteration: 4

    return 0;
}
```

### Different for Loop Patterns

```cpp
#include <iostream>

int main() {
    // Count up from 0
    std::cout << "Count up: ";
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Count down
    std::cout << "Count down: ";
    for (int i = 10; i > 0; i--) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Count by 2s
    std::cout << "Even numbers: ";
    for (int i = 0; i <= 10; i += 2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Count by 5s
    std::cout << "Multiples of 5: ";
    for (int i = 0; i <= 50; i += 5) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 2. Common for Loop Use Cases

### Summing Numbers

```cpp
#include <iostream>

int main() {
    int sum = 0;

    // Sum numbers from 1 to 100
    for (int i = 1; i <= 100; i++) {
        sum += i;
    }

    std::cout << "Sum of 1 to 100: " << sum << std::endl;
    // Output: 5050

    return 0;
}
```

### Multiplication Table

```cpp
#include <iostream>

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    std::cout << "Multiplication table for " << number << ":" << std::endl;
    for (int i = 1; i <= 10; i++) {
        std::cout << number << " x " << i << " = " << (number * i) << std::endl;
    }

    return 0;
}
```

### Factorial Calculation

```cpp
#include <iostream>

int main() {
    int n;
    long long factorial = 1;

    std::cout << "Enter a number: ";
    std::cin >> n;

    for (int i = 1; i <= n; i++) {
        factorial *= i;
    }

    std::cout << n << "! = " << factorial << std::endl;

    return 0;
}
```

### Pattern Printing

```cpp
#include <iostream>

int main() {
    // Print a triangle of stars
    int rows = 5;

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    // Output:
    // *
    // * *
    // * * *
    // * * * *
    // * * * * *

    return 0;
}
```

## 3. The while Loop

### Basic Syntax

```cpp
while (condition) {
    // Loop body - executes while condition is true
}
```

### Basic while Loop

```cpp
#include <iostream>

int main() {
    int count = 1;

    while (count <= 5) {
        std::cout << "Count: " << count << std::endl;
        count++;
    }

    return 0;
}
```

### While Loop with User Input

```cpp
#include <iostream>

int main() {
    int number;

    std::cout << "Enter numbers to sum (0 to stop):" << std::endl;

    int sum = 0;
    while (true) {
        std::cin >> number;

        if (number == 0) {
            break;  // Exit loop
        }

        sum += number;
    }

    std::cout << "Total sum: " << sum << std::endl;

    return 0;
}
```

### Password Validator

```cpp
#include <iostream>
#include <string>

int main() {
    std::string password;
    const std::string CORRECT_PASSWORD = "secret123";
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "Enter password: ";
        std::cin >> password;

        if (password == CORRECT_PASSWORD) {
            std::cout << "Access granted!" << std::endl;
            break;
        } else {
            attempts++;
            std::cout << "Wrong password. ";
            std::cout << (MAX_ATTEMPTS - attempts) << " attempts remaining." << std::endl;
        }
    }

    if (attempts == MAX_ATTEMPTS) {
        std::cout << "Account locked!" << std::endl;
    }

    return 0;
}
```

### Guessing Game

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;  // Random 1-100
    int guess;
    int attempts = 0;

    std::cout << "Guess the number (1-100)!" << std::endl;

    while (guess != secretNumber) {
        std::cout << "Your guess: ";
        std::cin >> guess;
        attempts++;

        if (guess < secretNumber) {
            std::cout << "Too low!" << std::endl;
        } else if (guess > secretNumber) {
            std::cout << "Too high!" << std::endl;
        } else {
            std::cout << "Correct! You won in " << attempts << " attempts!" << std::endl;
        }
    }

    return 0;
}
```

## 4. The do-while Loop

### Basic Syntax

```cpp
do {
    // Loop body - executes at least once
} while (condition);
```

### Difference from while

```cpp
#include <iostream>

int main() {
    // while loop - may not execute at all
    int x = 10;
    while (x < 5) {
        std::cout << "This never prints" << std::endl;
    }

    // do-while loop - always executes at least once
    int y = 10;
    do {
        std::cout << "This prints once even though condition is false" << std::endl;
    } while (y < 5);

    return 0;
}
```

### Menu System with do-while

```cpp
#include <iostream>

int main() {
    int choice;

    do {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. Play Game" << std::endl;
        std::cout << "2. Settings" << std::endl;
        std::cout << "3. High Scores" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Starting game..." << std::endl;
                break;
            case 2:
                std::cout << "Opening settings..." << std::endl;
                break;
            case 3:
                std::cout << "Displaying high scores..." << std::endl;
                break;
            case 4:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
```

### Input Validation with do-while

```cpp
#include <iostream>

int main() {
    int age;

    do {
        std::cout << "Enter your age (0-120): ";
        std::cin >> age;

        if (age < 0 || age > 120) {
            std::cout << "Invalid age! Try again." << std::endl;
        }
    } while (age < 0 || age > 120);

    std::cout << "Age accepted: " << age << std::endl;

    return 0;
}
```

## 5. Nested Loops

### Multiplication Table (Full)

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "Multiplication Table (1-10):" << std::endl;

    // Print header
    std::cout << "    ";
    for (int i = 1; i <= 10; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;
    std::cout << "   ";
    for (int i = 0; i < 40; i++) std::cout << "-";
    std::cout << std::endl;

    // Print table
    for (int i = 1; i <= 10; i++) {
        std::cout << std::setw(2) << i << " |";
        for (int j = 1; j <= 10; j++) {
            std::cout << std::setw(4) << (i * j);
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Pattern: Rectangle

```cpp
#include <iostream>

int main() {
    int rows = 5, cols = 10;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Pattern: Right Triangle

```cpp
#include <iostream>

int main() {
    int rows = 5;

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    // Output:
    // *
    // * *
    // * * *
    // * * * *
    // * * * * *

    return 0;
}
```

### Pattern: Pyramid

```cpp
#include <iostream>

int main() {
    int rows = 5;

    for (int i = 1; i <= rows; i++) {
        // Print spaces
        for (int j = 1; j <= rows - i; j++) {
            std::cout << " ";
        }
        // Print stars
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // Output:
    //     *
    //    ***
    //   *****
    //  *******
    // *********

    return 0;
}
```

### Pattern: Number Pyramid

```cpp
#include <iostream>

int main() {
    int rows = 5;

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    // Output:
    // 1
    // 1 2
    // 1 2 3
    // 1 2 3 4
    // 1 2 3 4 5

    return 0;
}
```

## 6. Loop Control Statements

### break - Exit Loop Immediately

```cpp
#include <iostream>

int main() {
    // Find first number divisible by both 3 and 7
    for (int i = 1; i <= 100; i++) {
        if (i % 3 == 0 && i % 7 == 0) {
            std::cout << "Found: " << i << std::endl;
            break;  // Exit loop
        }
    }

    return 0;
}
```

### continue - Skip to Next Iteration

```cpp
#include <iostream>

int main() {
    // Print odd numbers only
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 3 5 7 9

    return 0;
}
```

### break in Nested Loops

```cpp
#include <iostream>

int main() {
    bool found = false;

    for (int i = 1; i <= 10 && !found; i++) {
        for (int j = 1; j <= 10; j++) {
            if (i * j == 24) {
                std::cout << i << " x " << j << " = 24" << std::endl;
                found = true;
                break;  // Breaks inner loop only
            }
        }
    }

    return 0;
}
```

## 7. Infinite Loops

### Intentional Infinite Loop

```cpp
#include <iostream>

int main() {
    // Server-like infinite loop
    while (true) {
        std::cout << "Server running..." << std::endl;

        // Exit condition
        char input;
        std::cout << "Press 'q' to quit: ";
        std::cin >> input;

        if (input == 'q') {
            break;
        }
    }

    std::cout << "Server stopped." << std::endl;
    return 0;
}
```

### Accidental Infinite Loops (AVOID!)

```cpp
// WRONG - Infinite loop!
int i = 0;
while (i < 10) {
    std::cout << i << std::endl;
    // Forgot to increment i!
}

// WRONG - Infinite loop!
for (int i = 0; i >= 0; i++) {  // Condition always true
    std::cout << i << std::endl;
}

// WRONG - Infinite loop!
int x = 10;
while (x > 0) {
    std::cout << x << std::endl;
    x++;  // Should be x--, but increments instead!
}
```

## 8. for vs while vs do-while

### When to Use for Loop
- When you know the number of iterations
- When you have a counter variable
- For array/collection iteration (later lessons)

```cpp
// Good use of for
for (int i = 0; i < 10; i++) {
    // Do something 10 times
}
```

### When to Use while Loop
- When condition-based (don't know iterations count)
- When reading input until sentinel value
- When waiting for an event

```cpp
// Good use of while
while (userWantsToContinue) {
    // Keep processing
}
```

### When to Use do-while Loop
- When body must execute at least once
- Menu systems
- Input validation

```cpp
// Good use of do-while
do {
    // Show menu and get choice
} while (choice != EXIT);
```

## 9. Complete Examples

### Example 1: Prime Number Checker

```cpp
#include <iostream>

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    bool isPrime = true;

    if (number <= 1) {
        isPrime = false;
    } else {
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                isPrime = false;
                break;
            }
        }
    }

    if (isPrime) {
        std::cout << number << " is prime" << std::endl;
    } else {
        std::cout << number << " is not prime" << std::endl;
    }

    return 0;
}
```

### Example 2: Fibonacci Sequence

```cpp
#include <iostream>

int main() {
    int n;
    std::cout << "How many Fibonacci numbers? ";
    std::cin >> n;

    long long a = 0, b = 1;

    std::cout << "Fibonacci sequence: ";
    for (int i = 0; i < n; i++) {
        std::cout << a << " ";

        long long next = a + b;
        a = b;
        b = next;
    }
    std::cout << std::endl;

    return 0;
}
```

### Example 3: Calculator with Loop

```cpp
#include <iostream>

int main() {
    char continueCalc = 'y';

    while (continueCalc == 'y' || continueCalc == 'Y') {
        double num1, num2;
        char operation;

        std::cout << "\nEnter calculation (e.g., 5 + 3): ";
        std::cin >> num1 >> operation >> num2;

        switch (operation) {
            case '+':
                std::cout << "Result: " << (num1 + num2) << std::endl;
                break;
            case '-':
                std::cout << "Result: " << (num1 - num2) << std::endl;
                break;
            case '*':
                std::cout << "Result: " << (num1 * num2) << std::endl;
                break;
            case '/':
                if (num2 != 0) {
                    std::cout << "Result: " << (num1 / num2) << std::endl;
                } else {
                    std::cout << "Error: Division by zero!" << std::endl;
                }
                break;
            default:
                std::cout << "Invalid operation!" << std::endl;
                break;
        }

        std::cout << "Continue? (y/n): ";
        std::cin >> continueCalc;
    }

    std::cout << "Calculator closed." << std::endl;
    return 0;
}
```

### Example 4: ATM Simulation

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double balance = 1000.0;
    int choice;

    do {
        std::cout << "\n=== ATM Menu ===" << std::endl;
        std::cout << "1. Check Balance" << std::endl;
        std::cout << "2. Deposit" << std::endl;
        std::cout << "3. Withdraw" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Balance: $" << balance << std::endl;
                break;

            case 2: {
                double amount;
                std::cout << "Enter deposit amount: $";
                std::cin >> amount;
                if (amount > 0) {
                    balance += amount;
                    std::cout << "Deposited: $" << amount << std::endl;
                } else {
                    std::cout << "Invalid amount!" << std::endl;
                }
                break;
            }

            case 3: {
                double amount;
                std::cout << "Enter withdrawal amount: $";
                std::cin >> amount;
                if (amount > 0 && amount <= balance) {
                    balance -= amount;
                    std::cout << "Withdrawn: $" << amount << std::endl;
                } else if (amount > balance) {
                    std::cout << "Insufficient funds!" << std::endl;
                } else {
                    std::cout << "Invalid amount!" << std::endl;
                }
                break;
            }

            case 4:
                std::cout << "Thank you for using our ATM!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
```

## 10. Exercises

### Exercise 1: Sum of Digits
Write a program to sum the digits of a number (e.g., 1234 → 1+2+3+4 = 10)

### Exercise 2: Reverse Number
Reverse the digits of a number (e.g., 1234 → 4321)

### Exercise 3: Prime Numbers in Range
Print all prime numbers between 1 and 100

### Exercise 4: GCD Calculator
Calculate the Greatest Common Divisor of two numbers using Euclidean algorithm

### Exercise 5: Diamond Pattern
Print a diamond pattern with asterisks

## 11. Solutions

### Solution 1: Sum of Digits

```cpp
#include <iostream>

int main() {
    int number, sum = 0;
    std::cout << "Enter a number: ";
    std::cin >> number;

    int temp = number;
    while (temp > 0) {
        sum += temp % 10;  // Get last digit
        temp /= 10;        // Remove last digit
    }

    std::cout << "Sum of digits: " << sum << std::endl;
    return 0;
}
```

### Solution 2: Reverse Number

```cpp
#include <iostream>

int main() {
    int number, reversed = 0;
    std::cout << "Enter a number: ";
    std::cin >> number;

    while (number > 0) {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    }

    std::cout << "Reversed: " << reversed << std::endl;
    return 0;
}
```

### Solution 3: Prime Numbers in Range

```cpp
#include <iostream>

int main() {
    std::cout << "Prime numbers from 1 to 100:" << std::endl;

    for (int num = 2; num <= 100; num++) {
        bool isPrime = true;

        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            std::cout << num << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
```

### Solution 4: GCD Calculator

```cpp
#include <iostream>

int main() {
    int a, b;
    std::cout << "Enter two numbers: ";
    std::cin >> a >> b;

    // Euclidean algorithm
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    std::cout << "GCD: " << a << std::endl;
    return 0;
}
```

### Solution 5: Diamond Pattern

```cpp
#include <iostream>

int main() {
    int n = 5;

    // Upper half
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - i; j++) {
            std::cout << " ";
        }
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // Lower half
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= n - i; j++) {
            std::cout << " ";
        }
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## Summary

### Key Points
- **for loop**: Best when you know iteration count
- **while loop**: Best for condition-based loops
- **do-while loop**: Guarantees at least one execution
- **break**: Exit loop immediately
- **continue**: Skip to next iteration
- **Nested loops**: Loop inside another loop

### Best Practices
- Always ensure loops will terminate
- Use meaningful loop variable names (i, j, k for simple counters)
- Avoid modifying loop counter inside loop body (for for-loops)
- Use break and continue wisely - don't overuse
- Comment complex loop logic

## Checklist
- [ ] Understand for loop syntax and use
- [ ] Understand while loop syntax and use
- [ ] Understand do-while loop syntax and use
- [ ] Can write nested loops
- [ ] Know when to use break and continue
- [ ] Can avoid infinite loops
- [ ] Can choose appropriate loop type
- [ ] Completed all exercises

---

**Time to complete**: 3 hours
**Previous lesson**: [Lesson 05 - Control Flow](../Lesson-05/README.md)
**Next lesson**: [Lesson 07 - Break, Continue, and Nested Loops](../Lesson-07/README.md)
