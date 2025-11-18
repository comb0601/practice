# Lesson 7: For Loops

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Introduction to For Loops
2. For Loop Syntax and Structure
3. For Loop Variations
4. Range-Based For Loop (C++11)
5. Nested For Loops
6. For vs While Loops
7. Loop Variable Scope
8. Exercises and Projects

---

## 1. Introduction to For Loops

For loops are ideal when you know exactly how many times you need to repeat something. They combine initialization, condition checking, and update in one compact line.

### Why Use For Loops:

```cpp
#include <iostream>

int main()
{
    // While loop version
    int i = 0;
    while (i < 5)
    {
        std::cout << i << " ";
        i++;
    }
    std::cout << "\n";

    // For loop version (more compact)
    for (int j = 0; j < 5; j++)
    {
        std::cout << j << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 2. For Loop Syntax and Structure

### Basic Structure:

```cpp
#include <iostream>

int main()
{
    // for (initialization; condition; update)
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "Iteration " << i << "\n";
    }

    return 0;
}
```

**Output**:
```
Iteration 1
Iteration 2
Iteration 3
Iteration 4
Iteration 5
```

### How It Works:

```cpp
#include <iostream>

int main()
{
    // Step by step execution:
    for (int i = 0; i < 3; i++)
    {
        std::cout << "i = " << i << "\n";
    }

    // 1. Initialize: i = 0
    // 2. Check: i < 3? Yes (0 < 3)
    // 3. Execute body
    // 4. Update: i++ (i becomes 1)
    // 5. Check: i < 3? Yes (1 < 3)
    // 6. Execute body
    // 7. Update: i++ (i becomes 2)
    // 8. Check: i < 3? Yes (2 < 3)
    // 9. Execute body
    // 10. Update: i++ (i becomes 3)
    // 11. Check: i < 3? No (3 < 3 is false)
    // 12. Exit loop

    return 0;
}
```

### Common Patterns:

```cpp
#include <iostream>

int main()
{
    // Count from 1 to 10
    std::cout << "Count up: ";
    for (int i = 1; i <= 10; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // Count from 10 to 1 (countdown)
    std::cout << "Count down: ";
    for (int i = 10; i >= 1; i--)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // Count by 2s (even numbers)
    std::cout << "Even numbers: ";
    for (int i = 0; i <= 10; i += 2)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // Count by 5s
    std::cout << "By 5s: ";
    for (int i = 0; i <= 50; i += 5)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 3. For Loop Variations

### Multiple Initializations:

```cpp
#include <iostream>

int main()
{
    // Multiple variables
    for (int i = 0, j = 10; i < 5; i++, j--)
    {
        std::cout << "i = " << i << ", j = " << j << "\n";
    }

    return 0;
}
```

### Empty Components:

```cpp
#include <iostream>

int main()
{
    // Initialization outside
    int i = 0;
    for (; i < 5; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // Update inside loop body
    int j = 0;
    for (; j < 5;)
    {
        std::cout << j << " ";
        j++;
    }
    std::cout << "\n";

    // Infinite loop (all parts empty)
    // for (;;)
    // {
    //     std::cout << "Infinite\n";
    //     break;  // Need break to exit
    // }

    return 0;
}
```

### Different Update Styles:

```cpp
#include <iostream>

int main()
{
    // Multiply by 2 each time
    for (int i = 1; i <= 100; i *= 2)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
    // Output: 1 2 4 8 16 32 64

    // Divide by 2
    for (int i = 100; i >= 1; i /= 2)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
    // Output: 100 50 25 12 6 3 1

    return 0;
}
```

---

## 4. Range-Based For Loop (C++11)

### Basic Syntax:

```cpp
#include <iostream>
#include <vector>

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};

    // Traditional for loop
    for (int i = 0; i < 5; i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    // Range-based for loop (cleaner)
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### With Vectors:

```cpp
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "David"};

    // Range-based for loop
    for (std::string name : names)
    {
        std::cout << "Hello, " << name << "!\n";
    }

    // With auto keyword (type deduction)
    for (auto name : names)
    {
        std::cout << "Hi, " << name << "!\n";
    }

    return 0;
}
```

### Reference vs Copy:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};

    // By value (copy) - doesn't modify original
    for (int num : numbers)
    {
        num *= 2;
    }
    std::cout << "After copy: " << numbers[0] << "\n";  // Still 1

    // By reference - modifies original
    for (int& num : numbers)
    {
        num *= 2;
    }
    std::cout << "After reference: " << numbers[0] << "\n";  // Now 2

    // Const reference - read-only, efficient
    for (const int& num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 5. Nested For Loops

### Basic Nesting:

```cpp
#include <iostream>

int main()
{
    // Outer loop runs 3 times
    for (int i = 1; i <= 3; i++)
    {
        std::cout << "Outer loop: " << i << "\n";

        // Inner loop runs 2 times for each outer iteration
        for (int j = 1; j <= 2; j++)
        {
            std::cout << "  Inner loop: " << j << "\n";
        }
    }

    return 0;
}
```

**Output**:
```
Outer loop: 1
  Inner loop: 1
  Inner loop: 2
Outer loop: 2
  Inner loop: 1
  Inner loop: 2
Outer loop: 3
  Inner loop: 1
  Inner loop: 2
```

### Multiplication Table:

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "===== MULTIPLICATION TABLE =====\n\n";

    // Header row
    std::cout << "   ";
    for (int i = 1; i <= 10; i++)
    {
        std::cout << std::setw(4) << i;
    }
    std::cout << "\n   ";
    for (int i = 0; i < 40; i++) std::cout << "-";
    std::cout << "\n";

    // Table body
    for (int i = 1; i <= 10; i++)
    {
        std::cout << std::setw(2) << i << " |";
        for (int j = 1; j <= 10; j++)
        {
            std::cout << std::setw(4) << (i * j);
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Rectangle Pattern:

```cpp
#include <iostream>

int main()
{
    int rows = 5;
    int cols = 10;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

**Output**:
```
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
```

### Triangle Pattern:

```cpp
#include <iostream>

int main()
{
    int n = 5;

    // Right triangle
    std::cout << "Right Triangle:\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    std::cout << "\nInverted Triangle:\n";
    for (int i = n; i >= 1; i--)
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

---

## 6. For vs While Loops

### When to Use For:

```cpp
#include <iostream>

int main()
{
    // ✅ Known number of iterations - Use FOR
    for (int i = 0; i < 10; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // ✅ Iterating over ranges - Use FOR
    for (int i = 0; i < 100; i += 5)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### When to Use While:

```cpp
#include <iostream>
#include <string>

int main()
{
    // ✅ Unknown iterations - Use WHILE
    std::string password;
    while (password != "secret")
    {
        std::cout << "Enter password: ";
        std::cin >> password;
    }

    // ✅ Event-driven loops - Use WHILE
    bool gameRunning = true;
    while (gameRunning)
    {
        // Process game events
        // gameRunning = false; when done
        break;  // For this example
    }

    return 0;
}
```

### Conversion Examples:

```cpp
#include <iostream>

int main()
{
    // For loop
    for (int i = 0; i < 5; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // Equivalent while loop
    int j = 0;
    while (j < 5)
    {
        std::cout << j << " ";
        j++;
    }
    std::cout << "\n";

    return 0;
}
```

---

## 7. Loop Variable Scope

### Variable Lifetime:

```cpp
#include <iostream>

int main()
{
    // i only exists inside the loop
    for (int i = 0; i < 3; i++)
    {
        std::cout << i << " ";
    }
    // std::cout << i;  // ERROR: i doesn't exist here!

    std::cout << "\n";

    // Declare outside if needed after loop
    int j;
    for (j = 0; j < 3; j++)
    {
        std::cout << j << " ";
    }
    std::cout << "\nAfter loop, j = " << j << "\n";

    return 0;
}
```

### Multiple Loops:

```cpp
#include <iostream>

int main()
{
    // Each loop can have its own 'i'
    for (int i = 0; i < 3; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    for (int i = 10; i < 13; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Sum of Numbers

```cpp
#include <iostream>

int main()
{
    int n;
    std::cout << "Enter a number: ";
    std::cin >> n;

    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }

    std::cout << "Sum of 1 to " << n << " = " << sum << "\n";

    return 0;
}
```

### Exercise 2: Factorial Calculator

```cpp
#include <iostream>

int main()
{
    int n;
    std::cout << "Enter a number: ";
    std::cin >> n;

    long long factorial = 1;

    for (int i = 1; i <= n; i++)
    {
        factorial *= i;
    }

    std::cout << n << "! = " << factorial << "\n";

    return 0;
}
```

### Exercise 3: Fibonacci Sequence

```cpp
#include <iostream>

int main()
{
    int n;
    std::cout << "How many Fibonacci numbers? ";
    std::cin >> n;

    int a = 0, b = 1;

    std::cout << "Fibonacci sequence: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << a << " ";

        int next = a + b;
        a = b;
        b = next;
    }
    std::cout << "\n";

    return 0;
}
```

### Exercise 4: Prime Numbers in Range

```cpp
#include <iostream>

int main()
{
    int start, end;

    std::cout << "Enter range (start end): ";
    std::cin >> start >> end;

    std::cout << "Prime numbers between " << start << " and " << end << ":\n";

    for (int num = start; num <= end; num++)
    {
        if (num < 2) continue;

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
        }
    }

    std::cout << "\n";

    return 0;
}
```

### Exercise 5: Number Pattern

```cpp
#include <iostream>

int main()
{
    int n = 5;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

**Output**:
```
1
1 2
1 2 3
1 2 3 4
1 2 3 4 5
```

---

## Practice Project: ASCII Art Generator

```cpp
#include <iostream>

int main()
{
    int choice;

    std::cout << "===== ASCII ART GENERATOR =====\n";
    std::cout << "1. Pyramid\n";
    std::cout << "2. Diamond\n";
    std::cout << "3. Hollow Rectangle\n";
    std::cout << "4. Number Triangle\n";
    std::cout << "5. Christmas Tree\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    int size;
    std::cout << "Enter size: ";
    std::cin >> size;

    switch (choice)
    {
        case 1:  // Pyramid
            std::cout << "\nPyramid:\n";
            for (int i = 1; i <= size; i++)
            {
                // Spaces
                for (int j = 0; j < size - i; j++)
                    std::cout << " ";
                // Stars
                for (int j = 0; j < 2 * i - 1; j++)
                    std::cout << "*";
                std::cout << "\n";
            }
            break;

        case 2:  // Diamond
            std::cout << "\nDiamond:\n";
            // Upper half
            for (int i = 1; i <= size; i++)
            {
                for (int j = 0; j < size - i; j++)
                    std::cout << " ";
                for (int j = 0; j < 2 * i - 1; j++)
                    std::cout << "*";
                std::cout << "\n";
            }
            // Lower half
            for (int i = size - 1; i >= 1; i--)
            {
                for (int j = 0; j < size - i; j++)
                    std::cout << " ";
                for (int j = 0; j < 2 * i - 1; j++)
                    std::cout << "*";
                std::cout << "\n";
            }
            break;

        case 3:  // Hollow Rectangle
            std::cout << "\nHollow Rectangle:\n";
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size * 2; j++)
                {
                    if (i == 0 || i == size - 1 || j == 0 || j == size * 2 - 1)
                        std::cout << "*";
                    else
                        std::cout << " ";
                }
                std::cout << "\n";
            }
            break;

        case 4:  // Number Triangle
            std::cout << "\nNumber Triangle:\n";
            for (int i = 1; i <= size; i++)
            {
                for (int j = 1; j <= i; j++)
                {
                    std::cout << j << " ";
                }
                std::cout << "\n";
            }
            break;

        case 5:  // Christmas Tree
            std::cout << "\nChristmas Tree:\n";
            // Tree top
            for (int i = 1; i <= size; i++)
            {
                for (int j = 0; j < size - i; j++)
                    std::cout << " ";
                for (int j = 0; j < 2 * i - 1; j++)
                    std::cout << "*";
                std::cout << "\n";
            }
            // Tree trunk
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < size - 2; j++)
                    std::cout << " ";
                std::cout << "|||" << "\n";
            }
            break;

        default:
            std::cout << "Invalid choice!\n";
    }

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Off-by-One Error
```cpp
❌ for (int i = 0; i < 5; i++)  // Runs 5 times (0-4)
✅ for (int i = 0; i <= 5; i++)  // Runs 6 times (0-5)

❌ for (int i = 1; i < 10; i++)  // Runs 9 times (1-9)
✅ for (int i = 1; i <= 10; i++)  // Runs 10 times (1-10)
```

### Mistake 2: Modifying Loop Variable
```cpp
❌ for (int i = 0; i < 10; i++) {
    i = i + 2;  // Don't do this!
}

✅ for (int i = 0; i < 10; i += 3) {
    // Correct way to skip values
}
```

### Mistake 3: Infinite Loop
```cpp
❌ for (int i = 0; i >= 0; i++)  // Always true!
✅ for (int i = 0; i < 10; i++)
```

### Mistake 4: Wrong Comparison
```cpp
❌ for (int i = 10; i > 0; i++)  // Never ends!
✅ for (int i = 10; i > 0; i--)  // Countdown
```

---

## Key Takeaways

1. ✅ For loops are best for known iteration counts
2. ✅ Three parts: initialization, condition, update
3. ✅ Loop variable scope is limited to the loop
4. ✅ Can have multiple variables in initialization
5. ✅ Range-based for loops are cleaner for arrays/containers
6. ✅ Nested loops are powerful for 2D patterns
7. ✅ Watch for off-by-one errors
8. ✅ Use for when you can, while when you must

---

## Summary Checklist

Before moving to Lesson 8, ensure you can:
- [ ] Write basic for loops correctly
- [ ] Understand all three components of a for loop
- [ ] Use different update patterns (++, --, +=, etc.)
- [ ] Write nested for loops
- [ ] Create patterns with nested loops
- [ ] Use range-based for loops with arrays
- [ ] Understand loop variable scope
- [ ] Choose between for and while appropriately

---

## Next Lesson Preview

In **Lesson 8**, we'll learn about:
- Break statement in depth
- Continue statement
- Labeled breaks (goto alternatives)
- Loop control best practices
- Combining break and continue

**Fantastic work on for loops!** 🎯
