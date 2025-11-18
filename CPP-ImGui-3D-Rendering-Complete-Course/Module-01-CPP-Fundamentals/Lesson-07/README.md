# Lesson 07: Break, Continue, and Nested Loops
**Duration: 2 hours**

## Learning Objectives
- Master break and continue statements
- Understand labeled breaks (C++ doesn't have them - learn alternatives)
- Work with deeply nested loops
- Optimize nested loop performance
- Recognize and avoid common nested loop pitfalls
- Write clean, readable nested loop code

## 1. The break Statement

### Basic break Usage

```cpp
#include <iostream>

int main() {
    // Stop loop when condition is met
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            break;  // Exit loop immediately
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 2 3 4

    return 0;
}
```

### break with while Loop

```cpp
#include <iostream>

int main() {
    int count = 0;

    while (true) {  // Infinite loop
        std::cout << "Count: " << count << std::endl;
        count++;

        if (count >= 5) {
            break;  // Exit when count reaches 5
        }
    }

    std::cout << "Loop ended" << std::endl;
    return 0;
}
```

### Practical Example: Finding a Value

```cpp
#include <iostream>

int main() {
    int numbers[] = {10, 25, 30, 45, 50, 65, 70};
    int size = 7;
    int target = 45;
    bool found = false;

    for (int i = 0; i < size; i++) {
        if (numbers[i] == target) {
            std::cout << "Found " << target << " at index " << i << std::endl;
            found = true;
            break;  // No need to continue searching
        }
    }

    if (!found) {
        std::cout << target << " not found" << std::endl;
    }

    return 0;
}
```

## 2. The continue Statement

### Basic continue Usage

```cpp
#include <iostream>

int main() {
    // Skip even numbers
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip rest of loop body, go to next iteration
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 3 5 7 9

    return 0;
}
```

### continue with while Loop

```cpp
#include <iostream>

int main() {
    int i = 0;

    while (i < 10) {
        i++;

        if (i % 3 == 0) {
            continue;  // Skip multiples of 3
        }

        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 2 4 5 7 8 10

    return 0;
}
```

### Practical Example: Input Validation

```cpp
#include <iostream>

int main() {
    std::cout << "Enter 5 positive numbers:" << std::endl;
    int count = 0;
    int sum = 0;

    while (count < 5) {
        int number;
        std::cout << "Number " << (count + 1) << ": ";
        std::cin >> number;

        if (number <= 0) {
            std::cout << "Invalid! Must be positive. Try again." << std::endl;
            continue;  // Don't count this input
        }

        sum += number;
        count++;
    }

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average: " << (sum / 5.0) << std::endl;

    return 0;
}
```

## 3. break vs continue

### Visual Comparison

```cpp
#include <iostream>

int main() {
    std::cout << "With break:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            break;  // Exit loop entirely
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 2 3 4

    std::cout << "With continue:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            continue;  // Skip only this iteration
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Output: 1 2 3 4 6 7 8 9 10

    return 0;
}
```

## 4. Nested Loops Basics

### Two-Level Nesting

```cpp
#include <iostream>

int main() {
    for (int i = 1; i <= 3; i++) {  // Outer loop
        for (int j = 1; j <= 4; j++) {  // Inner loop
            std::cout << "(" << i << "," << j << ") ";
        }
        std::cout << std::endl;
    }

    // Output:
    // (1,1) (1,2) (1,3) (1,4)
    // (2,1) (2,2) (2,3) (2,4)
    // (3,1) (3,2) (3,3) (3,4)

    return 0;
}
```

### Three-Level Nesting

```cpp
#include <iostream>

int main() {
    for (int i = 1; i <= 2; i++) {
        for (int j = 1; j <= 3; j++) {
            for (int k = 1; k <= 2; k++) {
                std::cout << i << j << k << " ";
            }
        }
        std::cout << std::endl;
    }

    // Output:
    // 111 112 121 122 131 132
    // 211 212 221 222 231 232

    return 0;
}
```

## 5. break in Nested Loops

### break Only Exits Inner Loop

```cpp
#include <iostream>

int main() {
    for (int i = 1; i <= 3; i++) {
        std::cout << "Outer loop: " << i << std::endl;

        for (int j = 1; j <= 5; j++) {
            if (j == 3) {
                break;  // Exits only the inner loop!
            }
            std::cout << "  Inner loop: " << j << std::endl;
        }
    }

    // Output:
    // Outer loop: 1
    //   Inner loop: 1
    //   Inner loop: 2
    // Outer loop: 2
    //   Inner loop: 1
    //   Inner loop: 2
    // Outer loop: 3
    //   Inner loop: 1
    //   Inner loop: 2

    return 0;
}
```

### Breaking Out of Multiple Loops (Using Flag)

```cpp
#include <iostream>

int main() {
    bool found = false;

    for (int i = 1; i <= 5 && !found; i++) {
        for (int j = 1; j <= 5; j++) {
            std::cout << "Checking (" << i << "," << j << ")" << std::endl;

            if (i * j == 12) {
                std::cout << "Found: " << i << " * " << j << " = 12" << std::endl;
                found = true;
                break;  // Exit inner loop
            }
        }
    }

    return 0;
}
```

### Breaking Out Using goto (Not Recommended!)

```cpp
#include <iostream>

int main() {
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            if (i * j == 12) {
                std::cout << "Found: " << i << " * " << j << " = 12" << std::endl;
                goto end_loops;  // Jump to label (avoid if possible!)
            }
        }
    }

end_loops:
    std::cout << "Search complete" << std::endl;
    return 0;
}
```

### Better Alternative: Extract to Function

```cpp
#include <iostream>

bool findProduct(int target) {
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            if (i * j == target) {
                std::cout << "Found: " << i << " * " << j << " = " << target << std::endl;
                return true;  // Returns from function, exiting all loops
            }
        }
    }
    return false;
}

int main() {
    findProduct(12);
    std::cout << "Search complete" << std::endl;
    return 0;
}
```

## 6. continue in Nested Loops

### continue Only Affects Inner Loop

```cpp
#include <iostream>

int main() {
    for (int i = 1; i <= 3; i++) {
        std::cout << "Outer: " << i << " - ";

        for (int j = 1; j <= 5; j++) {
            if (j == 3) {
                continue;  // Skip j=3 in inner loop
            }
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    // Output:
    // Outer: 1 - 1 2 4 5
    // Outer: 2 - 1 2 4 5
    // Outer: 3 - 1 2 4 5

    return 0;
}
```

## 7. Complex Nested Loop Examples

### Matrix Operations

```cpp
#include <iostream>

int main() {
    const int ROWS = 3;
    const int COLS = 4;
    int matrix[ROWS][COLS] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Print matrix
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    // Sum all elements
    int sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            sum += matrix[i][j];
        }
    }
    std::cout << "Sum: " << sum << std::endl;

    // Find maximum
    int max = matrix[0][0];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] > max) {
                max = matrix[i][j];
            }
        }
    }
    std::cout << "Maximum: " << max << std::endl;

    return 0;
}
```

### Pattern: Hollow Rectangle

```cpp
#include <iostream>

int main() {
    int rows = 5, cols = 10;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Print * on borders, space inside
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                std::cout << "* ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Pattern: Number Triangle

```cpp
#include <iostream>

int main() {
    int rows = 5;

    for (int i = 1; i <= rows; i++) {
        // Print spaces
        for (int j = 1; j <= rows - i; j++) {
            std::cout << " ";
        }
        // Print numbers ascending
        for (int k = 1; k <= i; k++) {
            std::cout << k;
        }
        // Print numbers descending
        for (int l = i - 1; l >= 1; l--) {
            std::cout << l;
        }
        std::cout << std::endl;
    }

    // Output:
    //     1
    //    121
    //   12321
    //  1234321
    // 123454321

    return 0;
}
```

### Finding Pairs

```cpp
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;
    int target = 7;

    std::cout << "Pairs that sum to " << target << ":" << std::endl;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {  // j starts at i+1 to avoid duplicates
            if (arr[i] + arr[j] == target) {
                std::cout << arr[i] << " + " << arr[j] << " = " << target << std::endl;
            }
        }
    }

    return 0;
}
```

## 8. Performance Considerations

### Minimize Inner Loop Work

```cpp
#include <iostream>

int main() {
    const int SIZE = 100;

    // BAD - Calculates size every iteration
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Some work...
        }
    }

    // GOOD - Size is constant, compiler optimizes this anyway
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Same work...
        }
    }

    return 0;
}
```

### Break Early When Possible

```cpp
#include <iostream>

int main() {
    int matrix[5][5];
    bool foundZero = false;

    // Initialize matrix...
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = i * j;
        }
    }

    // Find first zero - break early
    for (int i = 0; i < 5 && !foundZero; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == 0) {
                std::cout << "Found zero at (" << i << "," << j << ")" << std::endl;
                foundZero = true;
                break;
            }
        }
    }

    return 0;
}
```

## 9. Complete Examples

### Example 1: Bubble Sort

```cpp
#include <iostream>

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;

    std::cout << "Original array: ";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // Bubble sort
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        // Optimization: break if no swaps made
        if (!swapped) {
            break;
        }
    }

    std::cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Example 2: Matrix Multiplication

```cpp
#include <iostream>

int main() {
    int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int b[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    int result[2][2] = {{0, 0}, {0, 0}};

    // Matrix multiplication
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 3; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Print result
    std::cout << "Result:" << std::endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            std::cout << result[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Example 3: Prime Number Grid

```cpp
#include <iostream>

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    const int ROWS = 10;
    const int COLS = 10;

    std::cout << "Prime numbers grid (1-100):" << std::endl;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int num = i * COLS + j + 1;

            if (isPrime(num)) {
                std::cout << num;
            } else {
                std::cout << ".";
            }
            std::cout << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## 10. Exercises

### Exercise 1: Matrix Transpose
Write a program to transpose a 3x3 matrix

### Exercise 2: Pascal's Triangle
Generate Pascal's triangle with n rows

### Exercise 3: Multiplication Table Grid
Create a 10x10 multiplication table with borders

### Exercise 4: Find All Pythagorean Triples
Find all Pythagorean triples (a² + b² = c²) where a, b, c < 30

### Exercise 5: Pattern - Hourglass
Print an hourglass pattern using asterisks

## 11. Solutions

### Solution 1: Matrix Transpose

```cpp
#include <iostream>

int main() {
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int transpose[3][3];

    // Transpose
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }

    // Print original
    std::cout << "Original:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Print transpose
    std::cout << "\nTranspose:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << transpose[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Solution 2: Pascal's Triangle

```cpp
#include <iostream>

int main() {
    int n = 6;

    for (int line = 0; line < n; line++) {
        // Print spaces
        for (int i = 0; i < n - line; i++) {
            std::cout << " ";
        }

        int coef = 1;
        for (int i = 0; i <= line; i++) {
            std::cout << coef << " ";
            coef = coef * (line - i) / (i + 1);
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Solution 3: Multiplication Table with Borders

```cpp
#include <iostream>
#include <iomanip>

int main() {
    const int SIZE = 10;

    // Top border
    std::cout << "     ";
    for (int i = 1; i <= SIZE; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;

    std::cout << "    +";
    for (int i = 0; i < SIZE * 4; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Table
    for (int i = 1; i <= SIZE; i++) {
        std::cout << std::setw(3) << i << " |";
        for (int j = 1; j <= SIZE; j++) {
            std::cout << std::setw(4) << (i * j);
        }
        std::cout << std::endl;
    }

    return 0;
}
```

### Solution 4: Pythagorean Triples

```cpp
#include <iostream>

int main() {
    std::cout << "Pythagorean triples (a² + b² = c², all < 30):" << std::endl;

    for (int a = 1; a < 30; a++) {
        for (int b = a; b < 30; b++) {  // b starts at a to avoid duplicates
            for (int c = b; c < 30; c++) {
                if (a * a + b * b == c * c) {
                    std::cout << a << "² + " << b << "² = " << c << "²" << std::endl;
                }
            }
        }
    }

    return 0;
}
```

### Solution 5: Hourglass Pattern

```cpp
#include <iostream>

int main() {
    int n = 5;

    // Upper half (inverted triangle)
    for (int i = n; i >= 1; i--) {
        // Spaces
        for (int j = 0; j < n - i; j++) {
            std::cout << " ";
        }
        // Stars
        for (int k = 0; k < 2 * i - 1; k++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // Lower half (normal triangle)
    for (int i = 2; i <= n; i++) {
        // Spaces
        for (int j = 0; j < n - i; j++) {
            std::cout << " ";
        }
        // Stars
        for (int k = 0; k < 2 * i - 1; k++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## Summary

### Key Points
- **break**: Exits loop immediately
- **continue**: Skips to next iteration
- **break in nested loops**: Only exits innermost loop
- **Alternatives to break all**: Flag variable, function return, goto (avoid)
- **Nested loop complexity**: O(n²) or worse - be careful with performance
- **Early termination**: Use break with flags to exit early

### Best Practices
- Use break for early exit when result found
- Use continue to skip invalid data
- Limit nesting depth (max 2-3 levels)
- Extract complex nested logic to functions
- Add comments for complex nested loops
- Consider performance with large datasets

## Checklist
- [ ] Understand break statement
- [ ] Understand continue statement
- [ ] Can work with nested loops
- [ ] Know how to break out of multiple loops
- [ ] Can optimize nested loop performance
- [ ] Can write readable nested code
- [ ] Completed all exercises

---

**Time to complete**: 2 hours
**Previous lesson**: [Lesson 06 - Loops](../Lesson-06/README.md)
**Next lesson**: [Lesson 08 - Functions Basics](../Lesson-08/README.md)
