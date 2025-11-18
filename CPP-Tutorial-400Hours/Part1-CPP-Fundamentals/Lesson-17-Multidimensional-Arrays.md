# Lesson 17: Multidimensional Arrays

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Multidimensional Arrays
2. Two-Dimensional Arrays
3. Initializing 2D Arrays
4. Accessing 2D Array Elements
5. 2D Arrays and Functions
6. Matrices and Matrix Operations
7. Three-Dimensional Arrays
8. Exercises and Projects

---

## 1. Introduction to Multidimensional Arrays

### What are Multidimensional Arrays?

Multidimensional arrays are arrays of arrays. The most common are 2D arrays, which can be visualized as tables or matrices.

```cpp
#include <iostream>

int main()
{
    // 1D array: list of numbers
    int numbers[5] = {1, 2, 3, 4, 5};

    // 2D array: table of numbers (3 rows, 4 columns)
    int table[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    std::cout << "2D Array (3x4):\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << table[i][j] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

**Output**:
```
2D Array (3x4):
1       2       3       4
5       6       7       8
9       10      11      12
```

---

## 2. Two-Dimensional Arrays

### Declaration and Memory Layout:

```cpp
#include <iostream>

int main()
{
    // Declaration
    int matrix[3][4];  // 3 rows, 4 columns

    // Memory layout is row-major (consecutive rows)
    std::cout << "Memory addresses:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << &matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Calculate total size
    std::cout << "\nTotal size: " << sizeof(matrix) << " bytes\n";
    std::cout << "Elements: " << sizeof(matrix) / sizeof(int) << "\n";

    return 0;
}
```

### Filling a 2D Array:

```cpp
#include <iostream>

int main()
{
    const int ROWS = 5;
    const int COLS = 5;
    int grid[ROWS][COLS];

    // Fill with sequential numbers
    int count = 1;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            grid[i][j] = count++;
        }
    }

    // Display
    std::cout << "Grid:\n";
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            std::cout << grid[i][j] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 3. Initializing 2D Arrays

### Various Initialization Methods:

```cpp
#include <iostream>

int main()
{
    // Complete initialization
    int matrix1[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // Flat initialization (also valid)
    int matrix2[2][3] = {1, 2, 3, 4, 5, 6};

    // Partial initialization (rest are 0)
    int matrix3[3][3] = {
        {1, 2},
        {3},
        {4, 5, 6}
    };

    // Zero initialization
    int matrix4[3][3] = {};

    // Display matrix3
    std::cout << "Partially initialized matrix:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << matrix3[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Identity Matrix:

```cpp
#include <iostream>

int main()
{
    const int SIZE = 4;
    int identity[SIZE][SIZE] = {};

    // Create identity matrix
    for (int i = 0; i < SIZE; i++)
    {
        identity[i][i] = 1;
    }

    std::cout << "Identity Matrix:\n";
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            std::cout << identity[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 4. Accessing 2D Array Elements

### Row and Column Access:

```cpp
#include <iostream>

int main()
{
    int scores[3][4] = {
        {85, 92, 78, 95},
        {88, 76, 93, 89},
        {91, 84, 87, 90}
    };

    // Access specific element
    std::cout << "Element at [1][2]: " << scores[1][2] << "\n\n";

    // Access entire row
    std::cout << "Row 0: ";
    for (int j = 0; j < 4; j++)
    {
        std::cout << scores[0][j] << " ";
    }
    std::cout << "\n\n";

    // Access entire column
    std::cout << "Column 2: ";
    for (int i = 0; i < 3; i++)
    {
        std::cout << scores[i][2] << " ";
    }
    std::cout << "\n\n";

    // Modify elements
    scores[1][2] = 100;

    std::cout << "After modification:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << scores[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Finding Elements:

```cpp
#include <iostream>

void findElement(int arr[][4], int rows, int cols, int target)
{
    bool found = false;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == target)
            {
                std::cout << "Found " << target << " at [" << i << "][" << j << "]\n";
                found = true;
            }
        }
    }

    if (!found)
    {
        std::cout << target << " not found\n";
    }
}

int main()
{
    int data[3][4] = {
        {10, 20, 30, 40},
        {50, 60, 70, 80},
        {90, 100, 110, 120}
    };

    findElement(data, 3, 4, 70);
    findElement(data, 3, 4, 999);

    return 0;
}
```

---

## 5. 2D Arrays and Functions

### Passing 2D Arrays to Functions:

```cpp
#include <iostream>

// Must specify column size
void print2DArray(int arr[][4], int rows)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void fill2DArray(int arr[][4], int rows, int value)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            arr[i][j] = value;
        }
    }
}

int sum2DArray(int arr[][4], int rows)
{
    int sum = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sum += arr[i][j];
        }
    }
    return sum;
}

int main()
{
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    std::cout << "Original matrix:\n";
    print2DArray(matrix, 3);

    std::cout << "\nSum: " << sum2DArray(matrix, 3) << "\n\n";

    fill2DArray(matrix, 3, 99);
    std::cout << "After filling with 99:\n";
    print2DArray(matrix, 3);

    return 0;
}
```

---

## 6. Matrices and Matrix Operations

### Matrix Addition:

```cpp
#include <iostream>

void addMatrices(int a[][3], int b[][3], int result[][3], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void printMatrix(int matrix[][3], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

int main()
{
    int matrix1[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int matrix2[2][3] = {
        {7, 8, 9},
        {10, 11, 12}
    };

    int result[2][3];

    addMatrices(matrix1, matrix2, result, 2, 3);

    std::cout << "Matrix 1:\n";
    printMatrix(matrix1, 2, 3);

    std::cout << "\nMatrix 2:\n";
    printMatrix(matrix2, 2, 3);

    std::cout << "\nResult:\n";
    printMatrix(result, 2, 3);

    return 0;
}
```

### Matrix Multiplication:

```cpp
#include <iostream>

void multiplyMatrices(int a[][3], int b[][2], int result[][2], int r1, int c1, int c2)
{
    // Initialize result to 0
    for (int i = 0; i < r1; i++)
    {
        for (int j = 0; j < c2; j++)
        {
            result[i][j] = 0;
        }
    }

    // Multiply
    for (int i = 0; i < r1; i++)
    {
        for (int j = 0; j < c2; j++)
        {
            for (int k = 0; k < c1; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

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

    int result[2][2];

    multiplyMatrices(a, b, result, 2, 3, 2);

    std::cout << "Result of multiplication:\n";
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

### Matrix Transpose:

```cpp
#include <iostream>

void transpose(int matrix[][3], int result[][2], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = matrix[i][j];
        }
    }
}

int main()
{
    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int transposed[3][2];

    transpose(matrix, transposed, 2, 3);

    std::cout << "Original (2x3):\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nTransposed (3x2):\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            std::cout << transposed[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 7. Three-Dimensional Arrays

### 3D Array Basics:

```cpp
#include <iostream>

int main()
{
    // 3D array: [depth][rows][cols]
    int cube[2][3][4] = {
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}
        },
        {
            {13, 14, 15, 16},
            {17, 18, 19, 20},
            {21, 22, 23, 24}
        }
    };

    // Access elements
    std::cout << "Element at [1][2][3]: " << cube[1][2][3] << "\n\n";

    // Print all layers
    for (int d = 0; d < 2; d++)
    {
        std::cout << "Layer " << d << ":\n";
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << cube[d][i][j] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Tic-Tac-Toe Board

```cpp
#include <iostream>

class TicTacToe
{
private:
    char board[3][3];

public:
    TicTacToe()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                board[i][j] = ' ';
            }
        }
    }

    void display()
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

    bool makeMove(int row, int col, char player)
    {
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ')
        {
            board[row][col] = player;
            return true;
        }
        return false;
    }

    char checkWinner()
    {
        // Check rows
        for (int i = 0; i < 3; i++)
        {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return board[i][0];
        }

        // Check columns
        for (int j = 0; j < 3; j++)
        {
            if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j])
                return board[0][j];
        }

        // Check diagonals
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return board[0][0];

        if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return board[0][2];

        return ' ';  // No winner
    }
};

int main()
{
    TicTacToe game;
    char currentPlayer = 'X';
    int moves = 0;

    game.display();

    while (moves < 9)
    {
        int row, col;
        std::cout << "Player " << currentPlayer << ", enter row and column (0-2): ";
        std::cin >> row >> col;

        if (game.makeMove(row, col, currentPlayer))
        {
            game.display();
            moves++;

            char winner = game.checkWinner();
            if (winner != ' ')
            {
                std::cout << "Player " << winner << " wins!\n";
                return 0;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
        else
        {
            std::cout << "Invalid move! Try again.\n";
        }
    }

    std::cout << "It's a draw!\n";
    return 0;
}
```

---

## Practice Project: Seating Chart Manager

```cpp
#include <iostream>
#include <string>

const int ROWS = 5;
const int COLS = 10;

class SeatingChart
{
private:
    std::string seats[ROWS][COLS];

public:
    SeatingChart()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                seats[i][j] = "Empty";
            }
        }
    }

    void display()
    {
        std::cout << "\n=== SEATING CHART ===\n";
        std::cout << "    ";
        for (int j = 1; j <= COLS; j++)
        {
            std::cout << j << "   ";
        }
        std::cout << "\n";

        for (int i = 0; i < ROWS; i++)
        {
            std::cout << (char)('A' + i) << "  ";
            for (int j = 0; j < COLS; j++)
            {
                if (seats[i][j] == "Empty")
                    std::cout << "[ ] ";
                else
                    std::cout << "[X] ";
            }
            std::cout << "\n";
        }
    }

    bool reserveSeat(int row, int col, const std::string& name)
    {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
        {
            if (seats[row][col] == "Empty")
            {
                seats[row][col] = name;
                return true;
            }
        }
        return false;
    }

    bool cancelReservation(int row, int col)
    {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
        {
            if (seats[row][col] != "Empty")
            {
                seats[row][col] = "Empty";
                return true;
            }
        }
        return false;
    }

    int getAvailableSeats()
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (seats[i][j] == "Empty")
                    count++;
            }
        }
        return count;
    }
};

int main()
{
    SeatingChart chart;
    int choice;

    do
    {
        std::cout << "\n=== SEATING SYSTEM ===\n";
        std::cout << "1. View seating chart\n";
        std::cout << "2. Reserve seat\n";
        std::cout << "3. Cancel reservation\n";
        std::cout << "4. View available seats\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                chart.display();
                break;

            case 2:
            {
                char rowChar;
                int col;
                std::string name;

                std::cout << "Enter row (A-E): ";
                std::cin >> rowChar;
                std::cout << "Enter column (1-10): ";
                std::cin >> col;
                std::cout << "Enter name: ";
                std::cin.ignore();
                std::getline(std::cin, name);

                int row = rowChar - 'A';
                if (chart.reserveSeat(row, col - 1, name))
                {
                    std::cout << "Seat reserved successfully!\n";
                }
                else
                {
                    std::cout << "Seat unavailable!\n";
                }
                break;
            }

            case 3:
            {
                char rowChar;
                int col;

                std::cout << "Enter row (A-E): ";
                std::cin >> rowChar;
                std::cout << "Enter column (1-10): ";
                std::cin >> col;

                int row = rowChar - 'A';
                if (chart.cancelReservation(row, col - 1))
                {
                    std::cout << "Reservation cancelled!\n";
                }
                else
                {
                    std::cout << "No reservation found!\n";
                }
                break;
            }

            case 4:
                std::cout << "Available seats: " << chart.getAvailableSeats() << "\n";
                break;
        }

    } while (choice != 0);

    return 0;
}
```

---

## Key Takeaways

1. 2D arrays represent tables or matrices
2. Declaration: `type arrayName[rows][cols]`
3. Must specify all dimensions except first when passing to functions
4. Access: `array[row][column]`
5. Use nested loops to traverse 2D arrays
6. 3D arrays add another dimension for depth/layers
7. Matrix operations require understanding of mathematical concepts
8. Multidimensional arrays use row-major order in memory

---

## Summary Checklist

Before moving to Lesson 18, ensure you can:
- [ ] Declare and initialize 2D arrays
- [ ] Access 2D array elements
- [ ] Pass 2D arrays to functions
- [ ] Perform matrix operations
- [ ] Work with 3D arrays
- [ ] Understand memory layout
- [ ] Traverse multidimensional arrays
- [ ] Apply arrays to practical problems

---

## Next Lesson Preview

In **Lesson 18**, we'll explore:
- Advanced array manipulation
- Sorting algorithms
- Searching algorithms
- Array transformations
- Dynamic array concepts

**Multidimensional arrays conquered!**
