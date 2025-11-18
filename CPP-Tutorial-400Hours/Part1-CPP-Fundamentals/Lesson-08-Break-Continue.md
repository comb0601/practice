# Lesson 8: Break and Continue

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Introduction to Loop Control
2. Break Statement
3. Continue Statement
4. Break vs Continue
5. Using Break in Nested Loops
6. Practical Applications
7. Best Practices and Alternatives
8. Exercises and Projects

---

## 1. Introduction to Loop Control

Loop control statements allow you to alter the normal flow of loops. The two main statements are `break` (exit loop immediately) and `continue` (skip to next iteration).

### Control Flow Overview:

```cpp
#include <iostream>

int main()
{
    std::cout << "Normal loop:\n";
    for (int i = 1; i <= 5; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    std::cout << "Loop with break:\n";
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
            break;  // Exit loop at 3
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    std::cout << "Loop with continue:\n";
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
            continue;  // Skip 3
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

**Output**:
```
Normal loop:
1 2 3 4 5

Loop with break:
1 2

Loop with continue:
1 2 4 5
```

---

## 2. Break Statement

### Basic Break Usage:

```cpp
#include <iostream>

int main()
{
    std::cout << "Searching for 5...\n";

    for (int i = 1; i <= 10; i++)
    {
        std::cout << "Checking " << i << "\n";

        if (i == 5)
        {
            std::cout << "Found it!\n";
            break;  // Exit loop immediately
        }
    }

    std::cout << "Loop ended\n";

    return 0;
}
```

**Output**:
```
Searching for 5...
Checking 1
Checking 2
Checking 3
Checking 4
Checking 5
Found it!
Loop ended
```

### Break in While Loop:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string password = "secret";
    std::string input;
    int attempts = 0;
    int maxAttempts = 3;

    while (attempts < maxAttempts)
    {
        std::cout << "Enter password: ";
        std::cin >> input;

        if (input == password)
        {
            std::cout << "Access granted!\n";
            break;  // Exit on success
        }

        attempts++;
        std::cout << "Wrong password. Attempts left: "
                  << (maxAttempts - attempts) << "\n";
    }

    if (input != password)
    {
        std::cout << "Account locked!\n";
    }

    return 0;
}
```

### Break in Do-While:

```cpp
#include <iostream>

int main()
{
    int choice;

    do
    {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Option A\n";
        std::cout << "2. Option B\n";
        std::cout << "3. Option C\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 4)
        {
            std::cout << "Exiting...\n";
            break;  // Exit menu
        }

        switch (choice)
        {
            case 1:
                std::cout << "Selected A\n";
                break;  // Exit switch, not loop!
            case 2:
                std::cout << "Selected B\n";
                break;
            case 3:
                std::cout << "Selected C\n";
                break;
            default:
                std::cout << "Invalid choice\n";
        }
    } while (true);  // Infinite loop, break exits

    return 0;
}
```

### Early Exit Pattern:

```cpp
#include <iostream>

int main()
{
    int target = 42;
    bool found = false;

    for (int i = 1; i <= 100; i++)
    {
        if (i == target)
        {
            found = true;
            std::cout << "Found " << target << " at position " << i << "\n";
            break;  // No need to continue searching
        }
    }

    if (!found)
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

---

## 3. Continue Statement

### Basic Continue Usage:

```cpp
#include <iostream>

int main()
{
    std::cout << "Printing odd numbers:\n";

    for (int i = 1; i <= 10; i++)
    {
        if (i % 2 == 0)  // If even
        {
            continue;  // Skip even numbers
        }

        std::cout << i << " ";
    }

    std::cout << "\n";

    return 0;
}
```

**Output**:
```
Printing odd numbers:
1 3 5 7 9
```

### Skip Specific Values:

```cpp
#include <iostream>

int main()
{
    std::cout << "Numbers except multiples of 3:\n";

    for (int i = 1; i <= 20; i++)
    {
        if (i % 3 == 0)
        {
            continue;  // Skip multiples of 3
        }

        std::cout << i << " ";
    }

    std::cout << "\n";

    return 0;
}
```

### Continue in While Loop:

```cpp
#include <iostream>

int main()
{
    int i = 0;

    while (i < 10)
    {
        i++;

        if (i % 2 == 0)
        {
            continue;  // Skip rest of loop for even numbers
        }

        std::cout << i << " ";
    }

    std::cout << "\n";

    return 0;
}
```

### Input Validation:

```cpp
#include <iostream>

int main()
{
    int validCount = 0;
    int targetCount = 5;

    std::cout << "Enter 5 positive numbers:\n";

    while (validCount < targetCount)
    {
        int num;
        std::cout << "Number " << (validCount + 1) << ": ";
        std::cin >> num;

        if (num <= 0)
        {
            std::cout << "Invalid! Must be positive.\n";
            continue;  // Don't count invalid numbers
        }

        std::cout << "Valid number: " << num << "\n";
        validCount++;
    }

    std::cout << "Thank you!\n";

    return 0;
}
```

---

## 4. Break vs Continue

### Side-by-Side Comparison:

```cpp
#include <iostream>

int main()
{
    std::cout << "===== BREAK =====\n";
    std::cout << "Exits loop completely:\n";
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
        {
            std::cout << "Breaking at 3\n";
            break;  // Stops loop entirely
        }
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    std::cout << "===== CONTINUE =====\n";
    std::cout << "Skips current iteration:\n";
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
        {
            std::cout << "\nSkipping 3\n";
            continue;  // Skips just this iteration
        }
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

**Output**:
```
===== BREAK =====
Exits loop completely:
1 2 Breaking at 3

===== CONTINUE =====
Skips current iteration:
1 2
Skipping 3
4 5
```

### Practical Example:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {10, -5, 20, 0, 30, -15, 40};
    int size = 7;

    std::cout << "Processing numbers with CONTINUE:\n";
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        if (numbers[i] < 0)
        {
            std::cout << "Skipping negative: " << numbers[i] << "\n";
            continue;  // Skip negative numbers
        }
        sum += numbers[i];
        std::cout << "Added: " << numbers[i] << " (sum: " << sum << ")\n";
    }
    std::cout << "Final sum: " << sum << "\n\n";

    std::cout << "Processing numbers with BREAK:\n";
    sum = 0;
    for (int i = 0; i < size; i++)
    {
        if (numbers[i] == 0)
        {
            std::cout << "Found zero! Stopping.\n";
            break;  // Stop at zero
        }
        sum += numbers[i];
        std::cout << "Added: " << numbers[i] << " (sum: " << sum << ")\n";
    }
    std::cout << "Final sum: " << sum << "\n";

    return 0;
}
```

---

## 5. Using Break in Nested Loops

### Break Only Exits Innermost Loop:

```cpp
#include <iostream>

int main()
{
    std::cout << "Break in nested loops:\n";

    for (int i = 1; i <= 3; i++)
    {
        std::cout << "Outer loop: " << i << "\n";

        for (int j = 1; j <= 5; j++)
        {
            if (j == 3)
            {
                std::cout << "  Breaking inner loop\n";
                break;  // Only exits inner loop!
            }
            std::cout << "  Inner: " << j << "\n";
        }
    }

    std::cout << "Done\n";

    return 0;
}
```

**Output**:
```
Break in nested loops:
Outer loop: 1
  Inner: 1
  Inner: 2
  Breaking inner loop
Outer loop: 2
  Inner: 1
  Inner: 2
  Breaking inner loop
Outer loop: 3
  Inner: 1
  Inner: 2
  Breaking inner loop
Done
```

### Breaking Out of Both Loops:

```cpp
#include <iostream>

int main()
{
    bool found = false;
    int target = 15;

    std::cout << "Searching for " << target << "...\n";

    for (int i = 1; i <= 5 && !found; i++)  // Add condition to outer loop
    {
        for (int j = 1; j <= 5; j++)
        {
            int value = i * j;
            std::cout << i << " * " << j << " = " << value << "\n";

            if (value == target)
            {
                std::cout << "Found it!\n";
                found = true;
                break;  // Exit inner loop
            }
        }
        // Outer loop condition (!found) will exit outer loop too
    }

    return 0;
}
```

### Using Flag Variable:

```cpp
#include <iostream>

int main()
{
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int target = 5;
    bool found = false;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (matrix[i][j] == target)
            {
                std::cout << "Found " << target
                          << " at position (" << i << ", " << j << ")\n";
                found = true;
                break;  // Exit inner loop
            }
        }

        if (found)
            break;  // Exit outer loop
    }

    if (!found)
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

---

## 6. Practical Applications

### Prime Number Finder:

```cpp
#include <iostream>

int main()
{
    int n;
    std::cout << "Enter a number: ";
    std::cin >> n;

    if (n <= 1)
    {
        std::cout << n << " is not prime\n";
        return 0;
    }

    bool isPrime = true;

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            isPrime = false;
            break;  // Found divisor, no need to continue
        }
    }

    if (isPrime)
        std::cout << n << " is prime\n";
    else
        std::cout << n << " is not prime\n";

    return 0;
}
```

### Data Processing with Continue:

```cpp
#include <iostream>

int main()
{
    double temperatures[] = {20.5, -999, 22.3, 18.9, -999, 25.1, 21.0};
    int size = 7;
    double sum = 0;
    int count = 0;

    std::cout << "Processing temperature data...\n";

    for (int i = 0; i < size; i++)
    {
        if (temperatures[i] == -999)  // Missing data marker
        {
            std::cout << "Skipping missing data at index " << i << "\n";
            continue;
        }

        sum += temperatures[i];
        count++;
        std::cout << "Temperature " << i << ": " << temperatures[i] << "°C\n";
    }

    if (count > 0)
    {
        double average = sum / count;
        std::cout << "\nAverage temperature: " << average << "°C\n";
    }

    return 0;
}
```

### Menu with Early Exit:

```cpp
#include <iostream>

int main()
{
    int balance = 1000;

    while (true)  // Infinite loop
    {
        std::cout << "\n===== ATM =====\n";
        std::cout << "1. Check Balance\n";
        std::cout << "2. Withdraw\n";
        std::cout << "3. Deposit\n";
        std::cout << "4. Exit\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 4)
        {
            std::cout << "Thank you!\n";
            break;  // Exit menu
        }

        switch (choice)
        {
            case 1:
                std::cout << "Balance: $" << balance << "\n";
                break;

            case 2:
            {
                int amount;
                std::cout << "Withdraw amount: $";
                std::cin >> amount;

                if (amount > balance)
                {
                    std::cout << "Insufficient funds!\n";
                    continue;  // Skip to menu
                }

                balance -= amount;
                std::cout << "Withdrew $" << amount << "\n";
                break;
            }

            case 3:
            {
                int amount;
                std::cout << "Deposit amount: $";
                std::cin >> amount;

                if (amount <= 0)
                {
                    std::cout << "Invalid amount!\n";
                    continue;
                }

                balance += amount;
                std::cout << "Deposited $" << amount << "\n";
                break;
            }

            default:
                std::cout << "Invalid choice!\n";
        }
    }

    return 0;
}
```

---

## 7. Best Practices and Alternatives

### When to Use Break:

```cpp
#include <iostream>

int main()
{
    // ✅ Good: Search and early exit
    int target = 7;
    for (int i = 0; i < 100; i++)
    {
        if (i == target)
        {
            std::cout << "Found!\n";
            break;
        }
    }

    // ❌ Bad: Could use proper condition instead
    int count = 0;
    while (true)
    {
        if (count >= 10)
            break;
        count++;
    }

    // ✅ Better: Use proper loop condition
    count = 0;
    while (count < 10)
    {
        count++;
    }

    return 0;
}
```

### When to Use Continue:

```cpp
#include <iostream>

int main()
{
    // ✅ Good: Skip invalid data
    for (int i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
            continue;
        std::cout << i << " ";
    }
    std::cout << "\n";

    // ❌ Overuse: Could use if instead
    for (int i = 0; i < 10; i++)
    {
        if (i % 2 != 0)
        {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";

    return 0;
}
```

### Avoid Goto (Use Break Instead):

```cpp
#include <iostream>

int main()
{
    // ❌ Using goto (avoid this!)
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (i * j > 10)
                goto exit_loops;  // Don't do this!
        }
    }
    exit_loops:

    // ✅ Better: Use flag variable
    bool shouldExit = false;
    for (int i = 0; i < 5 && !shouldExit; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (i * j > 10)
            {
                shouldExit = true;
                break;
            }
        }
    }

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: First N Primes

```cpp
#include <iostream>

int main()
{
    int count;
    std::cout << "How many prime numbers? ";
    std::cin >> count;

    int found = 0;
    int num = 2;

    std::cout << "First " << count << " prime numbers:\n";

    while (found < count)
    {
        bool isPrime = true;

        for (int i = 2; i * i <= num; i++)
        {
            if (num % i == 0)
            {
                isPrime = false;
                break;
            }
        }

        if (isPrime)
        {
            std::cout << num << " ";
            found++;
        }

        num++;
    }

    std::cout << "\n";

    return 0;
}
```

### Exercise 2: Find and Replace

```cpp
#include <iostream>

int main()
{
    int numbers[] = {1, 5, 3, 7, 5, 9, 5, 2};
    int size = 8;
    int target = 5;
    int replacement = 99;
    int replacements = 0;

    std::cout << "Original array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < size; i++)
    {
        if (numbers[i] == target)
        {
            numbers[i] = replacement;
            replacements++;
        }
    }

    std::cout << "Modified array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";
    std::cout << "Replacements made: " << replacements << "\n";

    return 0;
}
```

### Exercise 3: Password Validator

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string password;

    while (true)
    {
        std::cout << "\nCreate a password:\n";
        std::cout << "Rules: At least 8 characters, no spaces\n";
        std::cout << "Enter password: ";
        std::cin >> password;

        // Check length
        if (password.length() < 8)
        {
            std::cout << "Too short! Must be at least 8 characters.\n";
            continue;
        }

        // Check for spaces
        bool hasSpace = false;
        for (char c : password)
        {
            if (c == ' ')
            {
                hasSpace = true;
                break;
            }
        }

        if (hasSpace)
        {
            std::cout << "Cannot contain spaces!\n";
            continue;
        }

        // Password is valid
        std::cout << "Password accepted!\n";
        break;
    }

    return 0;
}
```

### Exercise 4: Sum Until Negative

```cpp
#include <iostream>

int main()
{
    int sum = 0;
    int count = 0;

    std::cout << "Enter numbers (negative to stop):\n";

    while (true)
    {
        int num;
        std::cout << "Number: ";
        std::cin >> num;

        if (num < 0)
        {
            std::cout << "Stopping (negative number entered)\n";
            break;
        }

        sum += num;
        count++;
        std::cout << "Current sum: " << sum << "\n";
    }

    if (count > 0)
    {
        std::cout << "\nTotal: " << sum << "\n";
        std::cout << "Average: " << (sum / static_cast<double>(count)) << "\n";
    }
    else
    {
        std::cout << "No numbers entered\n";
    }

    return 0;
}
```

### Exercise 5: Skip Multiples

```cpp
#include <iostream>

int main()
{
    int skip;
    std::cout << "Enter a number to skip its multiples: ";
    std::cin >> skip;

    std::cout << "Numbers from 1-50 (skipping multiples of " << skip << "):\n";

    for (int i = 1; i <= 50; i++)
    {
        if (i % skip == 0)
        {
            continue;
        }

        std::cout << i << " ";

        if (i % 10 == 0)
            std::cout << "\n";
    }

    std::cout << "\n";

    return 0;
}
```

---

## Practice Project: Number Game with Lives

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));  // Seed random number generator

    int secretNumber = rand() % 100 + 1;  // 1-100
    int guess;
    int lives = 5;
    int attempts = 0;

    std::cout << "===== NUMBER GUESSING GAME =====\n";
    std::cout << "Guess the number between 1-100\n";
    std::cout << "You have " << lives << " lives!\n\n";

    while (lives > 0)
    {
        std::cout << "Lives remaining: " << lives << "\n";
        std::cout << "Enter guess: ";
        std::cin >> guess;
        attempts++;

        // Validate input
        if (guess < 1 || guess > 100)
        {
            std::cout << "Invalid! Must be 1-100.\n";
            continue;  // Don't count invalid guess
        }

        if (guess == secretNumber)
        {
            std::cout << "\n🎉 CONGRATULATIONS! 🎉\n";
            std::cout << "You guessed it!\n";
            std::cout << "Number: " << secretNumber << "\n";
            std::cout << "Attempts: " << attempts << "\n";
            std::cout << "Lives left: " << lives << "\n";
            break;  // Win!
        }
        else if (guess < secretNumber)
        {
            std::cout << "Too low!\n";
            lives--;
        }
        else
        {
            std::cout << "Too high!\n";
            lives--;
        }

        // Give hints
        int diff = abs(guess - secretNumber);
        if (diff <= 5)
            std::cout << "Hint: Very close!\n";
        else if (diff <= 15)
            std::cout << "Hint: Getting warm...\n";
        else
            std::cout << "Hint: Cold!\n";

        std::cout << "\n";
    }

    if (lives == 0)
    {
        std::cout << "\n💀 GAME OVER! 💀\n";
        std::cout << "You ran out of lives.\n";
        std::cout << "The number was: " << secretNumber << "\n";
        std::cout << "Total attempts: " << attempts << "\n";
    }

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Break in Wrong Place
```cpp
❌ switch (x) {
    case 1:
        cout << "One\n";
        break;  // This breaks switch, not loop!
}
```

### Mistake 2: Continue After Loop Variable Update
```cpp
❌ for (int i = 0; i < 10; i++) {
    if (condition)
        continue;
    i++;  // This line will be skipped!
}
```

### Mistake 3: Infinite Loop with Continue
```cpp
❌ int i = 0;
while (i < 10) {
    if (condition)
        continue;  // Skips i++, infinite loop!
    i++;
}
```

### Mistake 4: Forgetting Break in Switch
```cpp
❌ switch (x) {
    case 1:
        cout << "One\n";
        // Falls through!
    case 2:
        cout << "Two\n";
}
```

---

## Key Takeaways

1. ✅ `break` exits the loop immediately
2. ✅ `continue` skips to next iteration
3. ✅ Break only exits innermost loop
4. ✅ Use flags to break nested loops
5. ✅ Don't overuse - proper conditions are often better
6. ✅ Avoid `goto` - use `break` and flags instead
7. ✅ `continue` is great for skipping invalid data
8. ✅ Always ensure loop can terminate

---

## Summary Checklist

Before moving to Lesson 9, ensure you can:
- [ ] Use break to exit loops early
- [ ] Use continue to skip iterations
- [ ] Understand break vs continue differences
- [ ] Handle break in nested loops with flags
- [ ] Validate input with break/continue
- [ ] Avoid infinite loops with continue
- [ ] Use break/continue appropriately
- [ ] Write clean loop control logic

---

## Next Lesson Preview

In **Lesson 9**, we'll learn about:
- Creating nested loops effectively
- Common nested loop patterns
- 2D arrays and matrices
- Performance considerations
- Debugging nested loops

**Excellent work on loop control!** 🚦
