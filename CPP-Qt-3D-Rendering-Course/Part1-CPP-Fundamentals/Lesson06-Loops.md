# Lesson 06: Loops in C++

**Duration: 3 hours**

## Table of Contents
1. While Loop
2. Do-While Loop
3. For Loop
4. Range-Based For Loop (C++11)
5. Loop Control Statements
6. Nested Loops
7. Practical Examples

## 1. While Loop

### Basic While Loop

```cpp
#include <iostream>

int main() {
    int count = 1;

    while (count <= 5) {
        std::cout << "Count: " << count << std::endl;
        count++;
    }

    std::cout << "Loop finished!" << std::endl;

    return 0;
}
```

### Input Validation

```cpp
#include <iostream>

int main() {
    int age;

    std::cout << "Enter your age (1-120): ";
    std::cin >> age;

    while (age < 1 || age > 120) {
        std::cout << "Invalid! Please enter age between 1 and 120: ";
        std::cin >> age;
    }

    std::cout << "Valid age: " << age << std::endl;

    return 0;
}
```

### Sum Calculator

```cpp
#include <iostream>

int main() {
    int num, sum = 0;

    std::cout << "Enter numbers (0 to stop): " << std::endl;

    std::cin >> num;
    while (num != 0) {
        sum += num;
        std::cin >> num;
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
```

## 2. Do-While Loop

### Basic Do-While

```cpp
#include <iostream>

int main() {
    int count = 1;

    do {
        std::cout << "Count: " << count << std::endl;
        count++;
    } while (count <= 5);

    return 0;
}
```

### Difference from While

```cpp
#include <iostream>

int main() {
    // While loop - may not execute at all
    int x = 10;
    while (x < 5) {
        std::cout << "This won't print" << std::endl;
        x++;
    }

    // Do-while - executes at least once
    int y = 10;
    do {
        std::cout << "This prints once: " << y << std::endl;
        y++;
    } while (y < 5);

    return 0;
}
```

### Menu System

```cpp
#include <iostream>

int main() {
    int choice;

    do {
        std::cout << "\n=== Menu ===" << std::endl;
        std::cout << "1. Option 1" << std::endl;
        std::cout << "2. Option 2" << std::endl;
        std::cout << "3. Option 3" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "You selected Option 1" << std::endl;
                break;
            case 2:
                std::cout << "You selected Option 2" << std::endl;
                break;
            case 3:
                std::cout << "You selected Option 3" << std::endl;
                break;
            case 4:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 4);

    return 0;
}
```

## 3. For Loop

### Basic For Loop

```cpp
#include <iostream>

int main() {
    // Print numbers 1 to 10
    for (int i = 1; i <= 10; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Countdown
    for (int i = 10; i >= 1; i--) {
        std::cout << i << " ";
    }
    std::cout << "Liftoff!" << std::endl;

    return 0;
}
```

### For Loop Components

```cpp
#include <iostream>

int main() {
    // for (initialization; condition; increment)
    for (int i = 0; i < 5; i++) {
        std::cout << "i = " << i << std::endl;
    }

    // Multiple variables
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << std::endl;
    }

    // Step by 2
    for (int i = 0; i <= 10; i += 2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

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

### Factorial Calculator

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

## 4. Range-Based For Loop (C++11)

### Basic Range-Based For

```cpp
#include <iostream>
#include <vector>

int main() {
    // Array
    int numbers[] = {1, 2, 3, 4, 5};

    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Vector
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};

    for (std::string name : names) {
        std::cout << name << std::endl;
    }

    return 0;
}
```

### With References

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Without reference (copy)
    for (int num : numbers) {
        num *= 2;  // Doesn't modify original
    }

    std::cout << "After copy loop: ";
    for (int num : numbers) {
        std::cout << num << " ";  // Still 1 2 3 4 5
    }
    std::cout << std::endl;

    // With reference (modifies original)
    for (int& num : numbers) {
        num *= 2;  // Modifies original
    }

    std::cout << "After reference loop: ";
    for (int num : numbers) {
        std::cout << num << " ";  // Now 2 4 6 8 10
    }
    std::cout << std::endl;

    return 0;
}
```

### With Auto Keyword

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> cities = {"New York", "London", "Tokyo"};

    // Auto deduces type
    for (auto city : cities) {
        std::cout << city << std::endl;
    }

    // Const reference for efficiency
    for (const auto& city : cities) {
        std::cout << city << std::endl;
    }

    return 0;
}
```

## 5. Loop Control Statements

### Break Statement

```cpp
#include <iostream>

int main() {
    // Find first number divisible by 7
    for (int i = 1; i <= 100; i++) {
        if (i % 7 == 0) {
            std::cout << "First number divisible by 7: " << i << std::endl;
            break;  // Exit loop
        }
    }

    // Search in array
    int numbers[] = {5, 12, 8, 21, 15};
    int target = 21;
    bool found = false;

    for (int num : numbers) {
        if (num == target) {
            std::cout << "Found " << target << "!" << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << target << " not found" << std::endl;
    }

    return 0;
}
```

### Continue Statement

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

    // Skip specific values
    for (int i = 1; i <= 10; i++) {
        if (i == 5 || i == 7) {
            continue;  // Skip 5 and 7
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Goto Statement (Not Recommended)

```cpp
#include <iostream>

int main() {
    int i = 0;

    loop_start:
    if (i < 5) {
        std::cout << i << " ";
        i++;
        goto loop_start;
    }

    std::cout << std::endl;

    // Better to use actual loops!

    return 0;
}
```

## 6. Nested Loops

### Basic Nested Loops

```cpp
#include <iostream>

int main() {
    // Multiplication table
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            std::cout << i * j << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Pattern Printing

```cpp
#include <iostream>

int main() {
    // Right triangle
    std::cout << "Right Triangle:" << std::endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    // Square
    std::cout << "\nSquare:" << std::endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    // Pyramid
    std::cout << "\nPyramid:" << std::endl;
    for (int i = 1; i <= 5; i++) {
        // Print spaces
        for (int j = 0; j < 5 - i; j++) {
            std::cout << " ";
        }
        // Print stars
        for (int j = 0; j < 2 * i - 1; j++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Matrix Operations

```cpp
#include <iostream>

int main() {
    const int ROWS = 3;
    const int COLS = 4;

    int matrix[ROWS][COLS];

    // Fill matrix
    int value = 1;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = value++;
        }
    }

    // Display matrix
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## 7. Practical Examples

### Prime Number Finder

```cpp
#include <iostream>

int main() {
    int n;

    std::cout << "Enter a number: ";
    std::cin >> n;

    bool isPrime = true;

    if (n <= 1) {
        isPrime = false;
    } else {
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
    }

    if (isPrime) {
        std::cout << n << " is prime" << std::endl;
    } else {
        std::cout << n << " is not prime" << std::endl;
    }

    return 0;
}
```

### Fibonacci Sequence

```cpp
#include <iostream>

int main() {
    int n;

    std::cout << "How many Fibonacci numbers? ";
    std::cin >> n;

    long long a = 0, b = 1, next;

    std::cout << "Fibonacci sequence:" << std::endl;

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            std::cout << a << " ";
            continue;
        }
        if (i == 1) {
            std::cout << b << " ";
            continue;
        }

        next = a + b;
        a = b;
        b = next;
        std::cout << next << " ";
    }

    std::cout << std::endl;

    return 0;
}
```

### Number Guessing Game

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Initialize random seed
    std::srand(std::time(0));

    int secret = std::rand() % 100 + 1;  // Random number 1-100
    int guess;
    int attempts = 0;

    std::cout << "=== Number Guessing Game ===" << std::endl;
    std::cout << "I'm thinking of a number between 1 and 100" << std::endl;

    do {
        std::cout << "Enter your guess: ";
        std::cin >> guess;
        attempts++;

        if (guess < secret) {
            std::cout << "Too low!" << std::endl;
        } else if (guess > secret) {
            std::cout << "Too high!" << std::endl;
        } else {
            std::cout << "Correct! You got it in " << attempts << " attempts!" << std::endl;
        }
    } while (guess != secret);

    return 0;
}
```

## Complete Project: Student Management System

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int choice;
    int studentCount = 0;
    const int MAX_STUDENTS = 10;
    std::string names[MAX_STUDENTS];
    double scores[MAX_STUDENTS];

    do {
        std::cout << "\n=== Student Management System ===" << std::endl;
        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. View All Students" << std::endl;
        std::cout << "3. Calculate Average" << std::endl;
        std::cout << "4. Find Highest Score" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                if (studentCount >= MAX_STUDENTS) {
                    std::cout << "Maximum students reached!" << std::endl;
                    break;
                }

                std::cin.ignore();
                std::cout << "Enter student name: ";
                std::getline(std::cin, names[studentCount]);

                std::cout << "Enter score: ";
                std::cin >> scores[studentCount];

                studentCount++;
                std::cout << "Student added successfully!" << std::endl;
                break;
            }

            case 2: {
                if (studentCount == 0) {
                    std::cout << "No students to display!" << std::endl;
                    break;
                }

                std::cout << "\n=== Student List ===" << std::endl;
                std::cout << std::left << std::setw(20) << "Name"
                          << std::right << std::setw(10) << "Score" << std::endl;
                std::cout << std::string(30, '-') << std::endl;

                for (int i = 0; i < studentCount; i++) {
                    std::cout << std::left << std::setw(20) << names[i]
                              << std::right << std::setw(10) << std::fixed
                              << std::setprecision(2) << scores[i] << std::endl;
                }
                break;
            }

            case 3: {
                if (studentCount == 0) {
                    std::cout << "No students!" << std::endl;
                    break;
                }

                double sum = 0;
                for (int i = 0; i < studentCount; i++) {
                    sum += scores[i];
                }

                double average = sum / studentCount;
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Average score: " << average << std::endl;
                break;
            }

            case 4: {
                if (studentCount == 0) {
                    std::cout << "No students!" << std::endl;
                    break;
                }

                double highest = scores[0];
                int highestIndex = 0;

                for (int i = 1; i < studentCount; i++) {
                    if (scores[i] > highest) {
                        highest = scores[i];
                        highestIndex = i;
                    }
                }

                std::cout << "Highest score: " << names[highestIndex]
                          << " with " << highest << std::endl;
                break;
            }

            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 5);

    return 0;
}
```

## Summary

You learned:
- While loops for condition-based iteration
- Do-while loops for guaranteed execution
- For loops for counted iteration
- Range-based for loops (C++11)
- Loop control with break and continue
- Nested loops for multi-dimensional iteration
- Practical loop applications

## Next Lesson

Lesson 07: Functions and Function Overloading
