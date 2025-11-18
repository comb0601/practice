# Lesson 6: While and Do-While Loops

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Introduction to Loops
2. While Loop Basics
3. Do-While Loop
4. While vs Do-While
5. Loop Control and Termination
6. Input Validation with Loops
7. Common Loop Patterns
8. Exercises and Projects

---

## 1. Introduction to Loops

Loops allow you to execute code repeatedly without writing the same code multiple times. They're essential for automation, data processing, and handling repetitive tasks.

### Why We Need Loops:

```cpp
#include <iostream>

int main()
{
    // Without loops - repetitive and limited
    std::cout << "1\n";
    std::cout << "2\n";
    std::cout << "3\n";
    std::cout << "4\n";
    std::cout << "5\n";
    // What if we need 1000 numbers?

    // With loops - clean and scalable
    for (int i = 1; i <= 5; i++)
    {
        std::cout << i << "\n";
    }

    return 0;
}
```

---

## 2. While Loop Basics

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    int count = 1;

    while (count <= 5)
    {
        std::cout << "Count: " << count << "\n";
        count++;
    }

    std::cout << "Loop finished!\n";

    return 0;
}
```

**Output**:
```
Count: 1
Count: 2
Count: 3
Count: 4
Count: 5
Loop finished!
```

### How While Loop Works:

```cpp
#include <iostream>

int main()
{
    int i = 1;

    // 1. Check condition
    // 2. If true, execute body
    // 3. Go back to step 1
    // 4. If false, exit loop

    while (i <= 3)  // Condition checked first
    {
        std::cout << "i = " << i << "\n";
        i++;
    }

    return 0;
}
```

### Countdown Example:

```cpp
#include <iostream>

int main()
{
    int countdown = 10;

    std::cout << "Rocket Launch Countdown:\n";

    while (countdown > 0)
    {
        std::cout << countdown << "...\n";
        countdown--;
    }

    std::cout << "LIFTOFF! 🚀\n";

    return 0;
}
```

### Sum Calculator:

```cpp
#include <iostream>

int main()
{
    int n = 5;
    int sum = 0;
    int i = 1;

    while (i <= n)
    {
        sum += i;  // sum = sum + i
        i++;
    }

    std::cout << "Sum of 1 to " << n << " = " << sum << "\n";
    // Sum = 1 + 2 + 3 + 4 + 5 = 15

    return 0;
}
```

### Password Checker:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string correctPassword = "secret123";
    std::string input;
    int attempts = 0;
    int maxAttempts = 3;

    while (attempts < maxAttempts)
    {
        std::cout << "Enter password: ";
        std::cin >> input;

        if (input == correctPassword)
        {
            std::cout << "Access granted!\n";
            break;  // Exit loop
        }
        else
        {
            attempts++;
            std::cout << "Wrong password! Attempts left: "
                      << (maxAttempts - attempts) << "\n";
        }
    }

    if (attempts == maxAttempts)
    {
        std::cout << "Account locked!\n";
    }

    return 0;
}
```

---

## 3. Do-While Loop

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    int count = 1;

    do
    {
        std::cout << "Count: " << count << "\n";
        count++;
    } while (count <= 5);

    std::cout << "Loop finished!\n";

    return 0;
}
```

**Output** (same as while loop):
```
Count: 1
Count: 2
Count: 3
Count: 4
Count: 5
Loop finished!
```

### Key Difference:

```cpp
#include <iostream>

int main()
{
    // While loop - may not execute at all
    int x = 10;
    while (x < 5)
    {
        std::cout << "This will NOT print\n";
    }

    // Do-while loop - executes at least once
    int y = 10;
    do
    {
        std::cout << "This WILL print once\n";
    } while (y < 5);

    return 0;
}
```

**Output**:
```
This WILL print once
```

### Menu Example:

```cpp
#include <iostream>

int main()
{
    int choice;

    do
    {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Start Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Settings\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                std::cout << "Starting new game...\n";
                break;
            case 2:
                std::cout << "Loading saved game...\n";
                break;
            case 3:
                std::cout << "Opening settings...\n";
                break;
            case 4:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
```

---

## 4. While vs Do-While

### Comparison Example:

```cpp
#include <iostream>

int main()
{
    std::cout << "===== WHILE LOOP =====\n";
    int i = 1;
    while (i <= 3)
    {
        std::cout << "While: " << i << "\n";
        i++;
    }

    std::cout << "\n===== DO-WHILE LOOP =====\n";
    int j = 1;
    do
    {
        std::cout << "Do-While: " << j << "\n";
        j++;
    } while (j <= 3);

    return 0;
}
```

### When to Use Each:

```cpp
#include <iostream>

int main()
{
    // Use WHILE when you may not need to execute at all
    int filesFound = 0;
    while (filesFound > 0)
    {
        std::cout << "Processing file...\n";
        filesFound--;
    }
    // This never executes if filesFound is 0

    // Use DO-WHILE when you need at least one execution
    int userInput;
    do
    {
        std::cout << "Enter a positive number: ";
        std::cin >> userInput;
    } while (userInput <= 0);
    // Always asks at least once

    return 0;
}
```

---

## 5. Loop Control and Termination

### Infinite Loops:

```cpp
#include <iostream>

int main()
{
    // ❌ INFINITE LOOP - Never ends!
    // while (true)
    // {
    //     std::cout << "Forever...\n";
    // }

    // ✅ Proper termination
    int count = 0;
    while (count < 5)
    {
        std::cout << count << "\n";
        count++;  // Don't forget this!
    }

    return 0;
}
```

### Early Exit with Break:

```cpp
#include <iostream>

int main()
{
    int number = 1;

    while (number <= 10)
    {
        if (number == 5)
        {
            std::cout << "Found 5! Exiting early.\n";
            break;  // Exit loop immediately
        }
        std::cout << number << " ";
        number++;
    }

    std::cout << "\nLoop ended\n";
    // Output: 1 2 3 4 Found 5! Exiting early.

    return 0;
}
```

### Multiple Exit Conditions:

```cpp
#include <iostream>

int main()
{
    int attempts = 0;
    int maxAttempts = 5;
    bool success = false;

    while (attempts < maxAttempts && !success)
    {
        std::cout << "Attempt " << (attempts + 1) << "\n";

        // Simulate some process
        if (attempts == 3)
        {
            success = true;
            std::cout << "Success!\n";
        }

        attempts++;
    }

    if (!success)
    {
        std::cout << "Failed after " << maxAttempts << " attempts\n";
    }

    return 0;
}
```

---

## 6. Input Validation with Loops

### Number Range Validation:

```cpp
#include <iostream>

int main()
{
    int age;

    do
    {
        std::cout << "Enter your age (1-120): ";
        std::cin >> age;

        if (age < 1 || age > 120)
        {
            std::cout << "Invalid age! Try again.\n";
        }
    } while (age < 1 || age > 120);

    std::cout << "Valid age entered: " << age << "\n";

    return 0;
}
```

### Yes/No Validation:

```cpp
#include <iostream>

int main()
{
    char response;
    bool validInput = false;

    while (!validInput)
    {
        std::cout << "Do you want to continue? (Y/N): ";
        std::cin >> response;

        response = toupper(response);

        if (response == 'Y' || response == 'N')
        {
            validInput = true;
        }
        else
        {
            std::cout << "Invalid input! Please enter Y or N.\n";
        }
    }

    if (response == 'Y')
    {
        std::cout << "Continuing...\n";
    }
    else
    {
        std::cout << "Exiting...\n";
    }

    return 0;
}
```

### Menu with Validation:

```cpp
#include <iostream>

int main()
{
    int choice;
    bool validChoice = false;

    do
    {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "1. Option A\n";
        std::cout << "2. Option B\n";
        std::cout << "3. Option C\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice (1-4): ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            validChoice = true;
        }
        else
        {
            std::cout << "Invalid choice! Please enter 1-4.\n";
        }
    } while (!validChoice);

    std::cout << "You selected option " << choice << "\n";

    return 0;
}
```

---

## 7. Common Loop Patterns

### Sum Accumulation:

```cpp
#include <iostream>

int main()
{
    int n = 5;
    int sum = 0;
    int i = 1;

    while (i <= n)
    {
        sum += i;
        i++;
    }

    std::cout << "Sum: " << sum << "\n";

    return 0;
}
```

### Product Accumulation:

```cpp
#include <iostream>

int main()
{
    int n = 5;
    int factorial = 1;
    int i = 1;

    while (i <= n)
    {
        factorial *= i;
        i++;
    }

    std::cout << n << "! = " << factorial << "\n";
    // 5! = 120

    return 0;
}
```

### Search Pattern:

```cpp
#include <iostream>

int main()
{
    int target = 7;
    int current = 1;
    bool found = false;

    while (current <= 10 && !found)
    {
        if (current == target)
        {
            found = true;
            std::cout << "Found " << target << "!\n";
        }
        current++;
    }

    if (!found)
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

### Reverse Digits:

```cpp
#include <iostream>

int main()
{
    int number = 12345;
    int reversed = 0;

    std::cout << "Original: " << number << "\n";

    while (number > 0)
    {
        int digit = number % 10;      // Get last digit
        reversed = reversed * 10 + digit;
        number /= 10;                 // Remove last digit
    }

    std::cout << "Reversed: " << reversed << "\n";
    // Output: 54321

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Multiplication Table

```cpp
#include <iostream>

int main()
{
    int number, i = 1;

    std::cout << "Enter a number: ";
    std::cin >> number;

    std::cout << "\n===== Multiplication Table of " << number << " =====\n";

    while (i <= 10)
    {
        std::cout << number << " x " << i << " = " << (number * i) << "\n";
        i++;
    }

    return 0;
}
```

### Exercise 2: Digit Counter

```cpp
#include <iostream>

int main()
{
    int number, count = 0;

    std::cout << "Enter a number: ";
    std::cin >> number;

    // Handle negative numbers
    if (number < 0)
        number = -number;

    // Handle zero
    if (number == 0)
    {
        count = 1;
    }
    else
    {
        while (number > 0)
        {
            number /= 10;
            count++;
        }
    }

    std::cout << "Number of digits: " << count << "\n";

    return 0;
}
```

### Exercise 3: Sum of Digits

```cpp
#include <iostream>

int main()
{
    int number, sum = 0;

    std::cout << "Enter a number: ";
    std::cin >> number;

    int original = number;

    if (number < 0)
        number = -number;

    while (number > 0)
    {
        sum += number % 10;
        number /= 10;
    }

    std::cout << "Sum of digits in " << original << " = " << sum << "\n";

    return 0;
}
```

### Exercise 4: Number Guessing Game

```cpp
#include <iostream>

int main()
{
    int secretNumber = 42;
    int guess;
    int attempts = 0;

    std::cout << "===== NUMBER GUESSING GAME =====\n";
    std::cout << "I'm thinking of a number between 1 and 100\n\n";

    do
    {
        std::cout << "Enter your guess: ";
        std::cin >> guess;
        attempts++;

        if (guess < secretNumber)
        {
            std::cout << "Too low! Try again.\n";
        }
        else if (guess > secretNumber)
        {
            std::cout << "Too high! Try again.\n";
        }
        else
        {
            std::cout << "\nCongratulations! You guessed it!\n";
            std::cout << "Number: " << secretNumber << "\n";
            std::cout << "Attempts: " << attempts << "\n";
        }
    } while (guess != secretNumber);

    return 0;
}
```

### Exercise 5: Prime Number Checker

```cpp
#include <iostream>

int main()
{
    int number;

    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number <= 1)
    {
        std::cout << number << " is NOT a prime number\n";
        return 0;
    }

    bool isPrime = true;
    int i = 2;

    while (i <= number / 2)
    {
        if (number % i == 0)
        {
            isPrime = false;
            break;
        }
        i++;
    }

    if (isPrime)
    {
        std::cout << number << " is a PRIME number\n";
    }
    else
    {
        std::cout << number << " is NOT a prime number\n";
    }

    return 0;
}
```

---

## Practice Project: Banking System

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    double balance = 1000.00;
    int choice;
    double amount;
    int pin = 1234;
    int enteredPin;
    int attempts = 0;
    bool authenticated = false;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "===== BANKING SYSTEM =====\n\n";

    // PIN Authentication
    while (attempts < 3 && !authenticated)
    {
        std::cout << "Enter PIN: ";
        std::cin >> enteredPin;

        if (enteredPin == pin)
        {
            authenticated = true;
            std::cout << "Authentication successful!\n\n";
        }
        else
        {
            attempts++;
            if (attempts < 3)
            {
                std::cout << "Incorrect PIN. Attempts left: "
                          << (3 - attempts) << "\n";
            }
        }
    }

    if (!authenticated)
    {
        std::cout << "Account locked due to multiple failed attempts.\n";
        return 1;
    }

    // Main Banking Loop
    do
    {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "1. Check Balance\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. View Transaction History\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice (1-5): ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:  // Check Balance
                std::cout << "\nCurrent Balance: $" << balance << "\n";
                break;

            case 2:  // Deposit
                do
                {
                    std::cout << "\nEnter deposit amount: $";
                    std::cin >> amount;

                    if (amount <= 0)
                    {
                        std::cout << "Invalid amount! Must be positive.\n";
                    }
                    else if (amount > 10000)
                    {
                        std::cout << "Maximum deposit per transaction: $10,000\n";
                    }
                } while (amount <= 0 || amount > 10000);

                balance += amount;
                std::cout << "Deposit successful!\n";
                std::cout << "New Balance: $" << balance << "\n";
                break;

            case 3:  // Withdraw
                do
                {
                    std::cout << "\nEnter withdrawal amount: $";
                    std::cin >> amount;

                    if (amount <= 0)
                    {
                        std::cout << "Invalid amount! Must be positive.\n";
                    }
                    else if (amount > balance)
                    {
                        std::cout << "Insufficient funds!\n";
                        std::cout << "Available balance: $" << balance << "\n";
                    }
                    else if (amount > 500)
                    {
                        std::cout << "Maximum withdrawal per transaction: $500\n";
                    }
                } while (amount <= 0 || amount > balance || amount > 500);

                balance -= amount;
                std::cout << "Withdrawal successful!\n";
                std::cout << "New Balance: $" << balance << "\n";
                break;

            case 4:  // Transaction History
                std::cout << "\n[Transaction history feature coming soon]\n";
                break;

            case 5:  // Exit
                std::cout << "\nThank you for using our banking system!\n";
                std::cout << "Final Balance: $" << balance << "\n";
                break;

            default:
                std::cout << "\nInvalid choice! Please enter 1-5.\n";
        }

        // Small delay for better UX
        if (choice != 5)
        {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }

    } while (choice != 5);

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Infinite Loop (Forgot to Update)
```cpp
❌ int i = 0;
while (i < 5) {
    cout << i << "\n";
    // Forgot i++; - infinite loop!
}

✅ int i = 0;
while (i < 5) {
    cout << i << "\n";
    i++;
}
```

### Mistake 2: Off-by-One Error
```cpp
❌ int i = 1;
while (i < 5)  // Prints 1-4, not 1-5!
    cout << i++ << "\n";

✅ int i = 1;
while (i <= 5)  // Prints 1-5
    cout << i++ << "\n";
```

### Mistake 3: Wrong Semicolon Placement
```cpp
❌ while (count < 5);  // Empty loop!
{
    cout << count << "\n";
    count++;
}

✅ while (count < 5)
{
    cout << count << "\n";
    count++;
}
```

### Mistake 4: Condition Never True
```cpp
❌ int x = 10;
while (x < 5)  // Never enters loop
    cout << x << "\n";
```

---

## Key Takeaways

1. ✅ While loop checks condition before executing
2. ✅ Do-while loop executes at least once, then checks condition
3. ✅ Always update loop control variable to avoid infinite loops
4. ✅ Use break to exit loops early
5. ✅ Do-while is perfect for menus and input validation
6. ✅ While is better when loop may not execute at all
7. ✅ Multiple conditions can control loop termination
8. ✅ Input validation is a common use case for loops

---

## Summary Checklist

Before moving to Lesson 7, ensure you can:
- [ ] Write while loops correctly
- [ ] Write do-while loops correctly
- [ ] Choose between while and do-while appropriately
- [ ] Avoid infinite loops
- [ ] Use break for early exit
- [ ] Validate user input with loops
- [ ] Implement common loop patterns
- [ ] Debug loop-related issues

---

## Next Lesson Preview

In **Lesson 7**, we'll learn about:
- For loops structure and syntax
- For loop variations
- Nested for loops
- Loop variable scope
- When to use for vs while

**Great job mastering while loops!** 🔄
