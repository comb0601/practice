# Lesson 9: Nested Loops

**Duration**: 5 hours
**Difficulty**: Beginner to Intermediate

## Table of Contents
1. Introduction to Nested Loops
2. Understanding Loop Execution
3. Common Nested Loop Patterns
4. 2D Arrays and Matrices
5. Patterns and Shapes
6. Performance Considerations
7. Debugging Nested Loops
8. Exercises and Projects

---

## 1. Introduction to Nested Loops

Nested loops occur when one loop is placed inside another loop. They're essential for working with 2D data structures, creating patterns, and solving complex problems.

### Basic Concept:

```cpp
#include <iostream>

int main()
{
    std::cout << "Nested loop demonstration:\n\n";

    for (int outer = 1; outer <= 3; outer++)
    {
        std::cout << "Outer loop iteration: " << outer << "\n";

        for (int inner = 1; inner <= 4; inner++)
        {
            std::cout << "  Inner loop iteration: " << inner << "\n";
        }

        std::cout << "\n";
    }

    return 0;
}
```

**Output**:
```
Nested loop demonstration:

Outer loop iteration: 1
  Inner loop iteration: 1
  Inner loop iteration: 2
  Inner loop iteration: 3
  Inner loop iteration: 4

Outer loop iteration: 2
  Inner loop iteration: 1
  Inner loop iteration: 2
  Inner loop iteration: 3
  Inner loop iteration: 4

Outer loop iteration: 3
  Inner loop iteration: 1
  Inner loop iteration: 2
  Inner loop iteration: 3
  Inner loop iteration: 4
```

---

## 2. Understanding Loop Execution

### Execution Flow:

```cpp
#include <iostream>

int main()
{
    int executionCount = 0;

    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= 2; j++)
        {
            executionCount++;
            std::cout << "i=" << i << ", j=" << j
                      << " (execution #" << executionCount << ")\n";
        }
    }

    std::cout << "\nTotal executions: " << executionCount << "\n";
    std::cout << "Formula: outer_iterations * inner_iterations = "
              << 3 << " * " << 2 << " = " << 6 << "\n";

    return 0;
}
```

### Triple Nested Loops:

```cpp
#include <iostream>

int main()
{
    std::cout << "Triple nested loop:\n";

    for (int i = 1; i <= 2; i++)
    {
        std::cout << "i=" << i << "\n";

        for (int j = 1; j <= 2; j++)
        {
            std::cout << "  j=" << j << "\n";

            for (int k = 1; k <= 2; k++)
            {
                std::cout << "    k=" << k << " (i=" << i
                          << ", j=" << j << ", k=" << k << ")\n";
            }
        }
    }

    std::cout << "\nTotal iterations: 2 * 2 * 2 = 8\n";

    return 0;
}
```

---

## 3. Common Nested Loop Patterns

### Multiplication Table:

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "===== MULTIPLICATION TABLE (1-10) =====\n\n";

    // Print header
    std::cout << "    ";
    for (int i = 1; i <= 10; i++)
    {
        std::cout << std::setw(4) << i;
    }
    std::cout << "\n    ";
    for (int i = 0; i < 40; i++)
        std::cout << "-";
    std::cout << "\n";

    // Print table
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

### Sum Table:

```cpp
#include <iostream>

int main()
{
    std::cout << "Sum table (i + j):\n\n";

    for (int i = 0; i <= 5; i++)
    {
        for (int j = 0; j <= 5; j++)
        {
            std::cout << (i + j) << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Finding Pairs:

```cpp
#include <iostream>

int main()
{
    int target = 10;

    std::cout << "Pairs that sum to " << target << ":\n";

    for (int i = 0; i <= target; i++)
    {
        for (int j = 0; j <= target; j++)
        {
            if (i + j == target)
            {
                std::cout << i << " + " << j << " = " << target << "\n";
            }
        }
    }

    return 0;
}
```

---

## 4. 2D Arrays and Matrices

### Creating and Filling a Matrix:

```cpp
#include <iostream>

int main()
{
    const int ROWS = 3;
    const int COLS = 4;
    int matrix[ROWS][COLS];

    // Fill matrix with values
    int value = 1;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            matrix[i][j] = value++;
        }
    }

    // Display matrix
    std::cout << "Matrix:\n";
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Matrix Sum:

```cpp
#include <iostream>

int main()
{
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int sum = 0;

    // Sum all elements
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sum += matrix[i][j];
        }
    }

    std::cout << "Sum of all elements: " << sum << "\n";

    // Sum each row
    std::cout << "\nRow sums:\n";
    for (int i = 0; i < 3; i++)
    {
        int rowSum = 0;
        for (int j = 0; j < 3; j++)
        {
            rowSum += matrix[i][j];
        }
        std::cout << "Row " << i << ": " << rowSum << "\n";
    }

    // Sum each column
    std::cout << "\nColumn sums:\n";
    for (int j = 0; j < 3; j++)
    {
        int colSum = 0;
        for (int i = 0; i < 3; i++)
        {
            colSum += matrix[i][j];
        }
        std::cout << "Column " << j << ": " << colSum << "\n";
    }

    return 0;
}
```

### Matrix Transpose:

```cpp
#include <iostream>

int main()
{
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int transpose[3][3];

    // Calculate transpose
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            transpose[j][i] = matrix[i][j];
        }
    }

    // Display original
    std::cout << "Original Matrix:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Display transpose
    std::cout << "\nTranspose:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << transpose[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 5. Patterns and Shapes

### Rectangle:

```cpp
#include <iostream>

int main()
{
    int rows = 5;
    int cols = 10;

    std::cout << "Solid Rectangle:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    std::cout << "\nHollow Rectangle:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1)
                std::cout << "* ";
            else
                std::cout << "  ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Triangle Patterns:

```cpp
#include <iostream>

int main()
{
    int n = 5;

    // Right-aligned triangle
    std::cout << "Right-aligned triangle:\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    // Left-aligned triangle
    std::cout << "\nLeft-aligned triangle:\n";
    for (int i = 1; i <= n; i++)
    {
        // Print spaces
        for (int j = 1; j <= n - i; j++)
        {
            std::cout << "  ";
        }
        // Print stars
        for (int j = 1; j <= i; j++)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    // Inverted triangle
    std::cout << "\nInverted triangle:\n";
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

### Pyramid:

```cpp
#include <iostream>

int main()
{
    int height = 5;

    std::cout << "Pyramid:\n";
    for (int i = 1; i <= height; i++)
    {
        // Print spaces
        for (int j = 1; j <= height - i; j++)
        {
            std::cout << " ";
        }

        // Print stars
        for (int j = 1; j <= 2 * i - 1; j++)
        {
            std::cout << "*";
        }

        std::cout << "\n";
    }

    return 0;
}
```

### Diamond:

```cpp
#include <iostream>

int main()
{
    int size = 5;

    std::cout << "Diamond:\n";

    // Upper half (including middle)
    for (int i = 1; i <= size; i++)
    {
        // Spaces
        for (int j = 1; j <= size - i; j++)
        {
            std::cout << " ";
        }
        // Stars
        for (int j = 1; j <= 2 * i - 1; j++)
        {
            std::cout << "*";
        }
        std::cout << "\n";
    }

    // Lower half
    for (int i = size - 1; i >= 1; i--)
    {
        // Spaces
        for (int j = 1; j <= size - i; j++)
        {
            std::cout << " ";
        }
        // Stars
        for (int j = 1; j <= 2 * i - 1; j++)
        {
            std::cout << "*";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Number Patterns:

```cpp
#include <iostream>

int main()
{
    int n = 5;

    // Pattern 1: Incrementing numbers
    std::cout << "Pattern 1:\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }

    // Pattern 2: Row number repeated
    std::cout << "\nPattern 2:\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    // Pattern 3: Sequential numbers
    std::cout << "\nPattern 3:\n";
    int num = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << num++ << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 6. Performance Considerations

### Time Complexity:

```cpp
#include <iostream>
#include <chrono>

int main()
{
    // O(n^2) - quadratic time
    int n = 1000;
    int count = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Nested loops executed: " << count << " times\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "For n=" << n << ", iterations = n^2 = " << (n * n) << "\n";

    return 0;
}
```

### Optimizing Nested Loops:

```cpp
#include <iostream>

int main()
{
    const int SIZE = 5;
    int matrix[SIZE][SIZE] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    // ❌ Inefficient: Searching entire matrix each time
    int target = 13;
    bool found = false;

    for (int i = 0; i < SIZE && !found; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] == target)
            {
                std::cout << "Found " << target
                          << " at position (" << i << ", " << j << ")\n";
                found = true;
                break;  // Exit inner loop
            }
        }
    }

    // ✅ Better: Stop outer loop too with flag
    // (already implemented above with && !found)

    return 0;
}
```

---

## 7. Debugging Nested Loops

### Adding Debug Output:

```cpp
#include <iostream>

int main()
{
    bool debug = true;  // Toggle debug mode

    for (int i = 0; i < 3; i++)
    {
        if (debug)
            std::cout << "=== Outer loop: i=" << i << " ===\n";

        for (int j = 0; j < 3; j++)
        {
            if (debug)
                std::cout << "  Inner loop: j=" << j << "\n";

            // Actual work
            int result = i * j;

            if (debug)
                std::cout << "  Result: " << i << " * " << j
                          << " = " << result << "\n";
        }

        if (debug)
            std::cout << "\n";
    }

    return 0;
}
```

### Tracking Iterations:

```cpp
#include <iostream>

int main()
{
    int outerCount = 0;
    int innerCount = 0;
    int totalCount = 0;

    for (int i = 0; i < 3; i++)
    {
        outerCount++;
        std::cout << "Outer iteration #" << outerCount << "\n";

        for (int j = 0; j < 4; j++)
        {
            innerCount++;
            totalCount++;
            std::cout << "  Inner iteration #" << innerCount
                      << " (total: " << totalCount << ")\n";
        }

        innerCount = 0;  // Reset for next outer iteration
        std::cout << "\n";
    }

    std::cout << "Summary:\n";
    std::cout << "Outer loop ran: " << outerCount << " times\n";
    std::cout << "Total iterations: " << totalCount << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Matrix Maximum

```cpp
#include <iostream>

int main()
{
    int matrix[4][4] = {
        {3, 8, 2, 9},
        {5, 1, 7, 4},
        {6, 12, 0, 3},
        {2, 7, 5, 8}
    };

    int maxVal = matrix[0][0];
    int maxRow = 0;
    int maxCol = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix[i][j] > maxVal)
            {
                maxVal = matrix[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }

    std::cout << "Maximum value: " << maxVal << "\n";
    std::cout << "Position: (" << maxRow << ", " << maxCol << ")\n";

    return 0;
}
```

### Exercise 2: Matrix Multiplication

```cpp
#include <iostream>

int main()
{
    int a[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int b[3][2] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    int result[2][2] = {0};  // Initialize to 0

    // Multiply matrices
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Display result
    std::cout << "Result of matrix multiplication:\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            std::cout << result[i][j] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Exercise 3: Pascal's Triangle

```cpp
#include <iostream>

int main()
{
    int rows = 6;

    std::cout << "Pascal's Triangle:\n\n";

    for (int i = 0; i < rows; i++)
    {
        // Print leading spaces
        for (int k = 0; k < rows - i - 1; k++)
        {
            std::cout << " ";
        }

        int val = 1;
        for (int j = 0; j <= i; j++)
        {
            std::cout << val << " ";
            val = val * (i - j) / (j + 1);
        }

        std::cout << "\n";
    }

    return 0;
}
```

### Exercise 4: Diagonal Sum

```cpp
#include <iostream>

int main()
{
    int matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int primaryDiagonalSum = 0;
    int secondaryDiagonalSum = 0;

    for (int i = 0; i < 4; i++)
    {
        // Primary diagonal (top-left to bottom-right)
        primaryDiagonalSum += matrix[i][i];

        // Secondary diagonal (top-right to bottom-left)
        secondaryDiagonalSum += matrix[i][3 - i];
    }

    std::cout << "Primary diagonal sum: " << primaryDiagonalSum << "\n";
    std::cout << "Secondary diagonal sum: " << secondaryDiagonalSum << "\n";

    return 0;
}
```

### Exercise 5: Spiral Matrix

```cpp
#include <iostream>

int main()
{
    const int N = 4;
    int matrix[N][N];

    int value = 1;
    int top = 0, bottom = N - 1;
    int left = 0, right = N - 1;

    while (top <= bottom && left <= right)
    {
        // Fill top row
        for (int i = left; i <= right; i++)
            matrix[top][i] = value++;
        top++;

        // Fill right column
        for (int i = top; i <= bottom; i++)
            matrix[i][right] = value++;
        right--;

        // Fill bottom row
        if (top <= bottom)
        {
            for (int i = right; i >= left; i--)
                matrix[bottom][i] = value++;
            bottom--;
        }

        // Fill left column
        if (left <= right)
        {
            for (int i = bottom; i >= top; i--)
                matrix[i][left] = value++;
            left++;
        }
    }

    // Display spiral matrix
    std::cout << "Spiral Matrix:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## Practice Project: Tic-Tac-Toe Game

```cpp
#include <iostream>

void displayBoard(char board[3][3])
{
    std::cout << "\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << " " << board[i][j];
            if (j < 2) std::cout << " |";
        }
        std::cout << "\n";
        if (i < 2) std::cout << "---|---|---\n";
    }
    std::cout << "\n";
}

bool checkWin(char board[3][3], char player)
{
    // Check rows
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
    }

    // Check columns
    for (int j = 0; j < 3; j++)
    {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
            return true;
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

bool isFull(char board[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

int main()
{
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char currentPlayer = 'X';
    int row, col;
    int moves = 0;

    std::cout << "===== TIC-TAC-TOE =====\n";
    std::cout << "Player X goes first\n";
    std::cout << "Enter row (0-2) and column (0-2)\n";

    while (true)
    {
        displayBoard(board);

        std::cout << "Player " << currentPlayer << "'s turn\n";
        std::cout << "Enter row: ";
        std::cin >> row;
        std::cout << "Enter column: ";
        std::cin >> col;

        // Validate input
        if (row < 0 || row > 2 || col < 0 || col > 2)
        {
            std::cout << "Invalid position! Try again.\n";
            continue;
        }

        if (board[row][col] != ' ')
        {
            std::cout << "Position already taken! Try again.\n";
            continue;
        }

        // Make move
        board[row][col] = currentPlayer;
        moves++;

        // Check for win
        if (checkWin(board, currentPlayer))
        {
            displayBoard(board);
            std::cout << "🎉 Player " << currentPlayer << " WINS! 🎉\n";
            break;
        }

        // Check for draw
        if (isFull(board))
        {
            displayBoard(board);
            std::cout << "It's a DRAW!\n";
            break;
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    std::cout << "Game Over! Total moves: " << moves << "\n";

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Wrong Loop Bounds
```cpp
❌ for (int i = 0; i <= 3; i++)  // Will access matrix[3][j]!
    for (int j = 0; j <= 3; j++)
        matrix[i][j] = 0;

✅ for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
        matrix[i][j] = 0;
```

### Mistake 2: Swapping Row/Column
```cpp
❌ for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++)
        matrix[j][i] = value;  // Wrong order!

✅ for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++)
        matrix[i][j] = value;
```

### Mistake 3: Performance Issues
```cpp
❌ // O(n^3) - very slow for large n
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++)
            // Consider if all three loops are necessary
```

---

## Key Takeaways

1. ✅ Nested loops execute inner loop completely for each outer iteration
2. ✅ Total iterations = outer * inner (or outer * inner * innermost)
3. ✅ Essential for 2D arrays and matrices
4. ✅ Great for creating patterns and shapes
5. ✅ Use break with flags to exit all loops
6. ✅ Watch for performance with deep nesting
7. ✅ Debug with print statements showing loop variables
8. ✅ Always validate array bounds carefully

---

## Summary Checklist

Before moving to Lesson 10, ensure you can:
- [ ] Write and understand nested loops
- [ ] Calculate total iterations in nested loops
- [ ] Work with 2D arrays using nested loops
- [ ] Create patterns (triangles, diamonds, etc.)
- [ ] Understand time complexity (O(n^2))
- [ ] Debug nested loops effectively
- [ ] Use break/continue in nested structures
- [ ] Optimize nested loop performance

---

## Next Lesson Preview

In **Lesson 10**, we'll learn about:
- Real-world loop applications
- Algorithm patterns with loops
- Searching and sorting basics
- Data processing techniques
- Loop efficiency tips

**Amazing progress on nested loops!** 🔄🔄
