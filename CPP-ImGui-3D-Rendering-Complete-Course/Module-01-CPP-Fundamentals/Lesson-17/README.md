# Lesson 17: Multi-Dimensional Arrays

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand and work with multi-dimensional arrays
- Declare and initialize 2D arrays
- Process 2D arrays with nested loops
- Work with 3D arrays and higher dimensions
- Pass multi-dimensional arrays to functions
- Solve practical problems using 2D arrays

## 1. Introduction to 2D Arrays

### 1.1 Understanding 2D Arrays

A 2D array is an array of arrays. Think of it as a table with rows and columns.

```cpp
#include <iostream>
using namespace std;

int main() {
    // Declare a 3x4 2D array (3 rows, 4 columns)
    int matrix[3][4];

    // Initialize values
    int value = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = value++;
        }
    }

    // Print the matrix
    cout << "Matrix:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}
```

**Output:**
```
Matrix:
1    2    3    4
5    6    7    8
9    10   11   12
```

### 1.2 2D Array Initialization

```cpp
#include <iostream>
using namespace std;

int main() {
    // Method 1: Initialize all values
    int arr1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Method 2: Partial initialization
    int arr2[3][3] = {
        {1, 2},     // Rest filled with 0
        {4},        // Rest filled with 0
        {}          // All filled with 0
    };

    // Method 3: Initialize all to zero
    int arr3[3][3] = {0};

    // Method 4: Single row format
    int arr4[2][3] = {1, 2, 3, 4, 5, 6};

    // Print arr1
    cout << "arr1:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << arr1[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\narr2 (partial):" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << arr2[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
```

## 2. Working with 2D Arrays

### 2.1 Reading and Writing 2D Arrays

```cpp
#include <iostream>
using namespace std;

const int ROWS = 3;
const int COLS = 4;

void input2DArray(int arr[][COLS], int rows) {
    cout << "Enter " << rows * COLS << " values:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << "Element [" << i << "][" << j << "]: ";
            cin >> arr[i][j];
        }
    }
}

void print2DArray(int arr[][COLS], int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    int matrix[ROWS][COLS];

    input2DArray(matrix, ROWS);

    cout << "\nYou entered:" << endl;
    print2DArray(matrix, ROWS);

    return 0;
}
```

### 2.2 Matrix Operations

```cpp
#include <iostream>
using namespace std;

const int SIZE = 3;

void printMatrix(int mat[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << mat[i][j] << "\t";
        }
        cout << endl;
    }
}

// Matrix addition
void addMatrices(int a[][SIZE], int b[][SIZE], int result[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

// Matrix transpose
void transpose(int mat[][SIZE], int result[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

// Sum of diagonal elements
int diagonalSum(int mat[][SIZE]) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += mat[i][i];
    }
    return sum;
}

int main() {
    int mat1[SIZE][SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int mat2[SIZE][SIZE] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int sum[SIZE][SIZE];
    int trans[SIZE][SIZE];

    cout << "Matrix 1:" << endl;
    printMatrix(mat1);

    cout << "\nMatrix 2:" << endl;
    printMatrix(mat2);

    addMatrices(mat1, mat2, sum);
    cout << "\nSum:" << endl;
    printMatrix(sum);

    transpose(mat1, trans);
    cout << "\nTranspose of Matrix 1:" << endl;
    printMatrix(trans);

    cout << "\nDiagonal sum of Matrix 1: " << diagonalSum(mat1) << endl;

    return 0;
}
```

### 2.3 Searching in 2D Arrays

```cpp
#include <iostream>
using namespace std;

bool search2D(int arr[][4], int rows, int cols, int target, int& foundRow, int& foundCol) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == target) {
                foundRow = i;
                foundCol = j;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int matrix[3][4] = {
        {10, 20, 30, 40},
        {15, 25, 35, 45},
        {12, 22, 32, 42}
    };

    int target = 35;
    int row, col;

    if (search2D(matrix, 3, 4, target, row, col)) {
        cout << "Found " << target << " at position [" << row << "][" << col << "]" << endl;
    } else {
        cout << target << " not found" << endl;
    }

    return 0;
}
```

## 3. Common 2D Array Applications

### 3.1 Tic-Tac-Toe Board

```cpp
#include <iostream>
using namespace std;

const int SIZE = 3;

void initBoard(char board[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[][SIZE]) {
    cout << "\n  0   1   2" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j];
            if (j < SIZE - 1) cout << " | ";
        }
        cout << endl;
        if (i < SIZE - 1) cout << " -----------" << endl;
    }
    cout << endl;
}

bool makeMove(char board[][SIZE], int row, int col, char player) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return false;
    }
    if (board[row][col] != ' ') {
        return false;
    }
    board[row][col] = player;
    return true;
}

int main() {
    char board[SIZE][SIZE];
    initBoard(board);

    printBoard(board);

    makeMove(board, 1, 1, 'X');
    makeMove(board, 0, 0, 'O');
    makeMove(board, 2, 2, 'X');

    printBoard(board);

    return 0;
}
```

### 3.2 Seating Chart

```cpp
#include <iostream>
using namespace std;

const int ROWS = 5;
const int COLS = 6;

void initSeating(char seats[][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            seats[i][j] = 'O';  // O = available
        }
    }
}

void displaySeating(char seats[][COLS]) {
    cout << "\n  ";
    for (int j = 0; j < COLS; j++) {
        cout << j + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < ROWS; i++) {
        cout << (char)('A' + i) << " ";
        for (int j = 0; j < COLS; j++) {
            cout << seats[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nO = Available, X = Taken" << endl;
}

bool reserveSeat(char seats[][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return false;
    }
    if (seats[row][col] == 'X') {
        return false;  // Already taken
    }
    seats[row][col] = 'X';
    return true;
}

int main() {
    char auditorium[ROWS][COLS];
    initSeating(auditorium);

    displaySeating(auditorium);

    reserveSeat(auditorium, 2, 3);  // Row C, Seat 4
    reserveSeat(auditorium, 0, 0);  // Row A, Seat 1
    reserveSeat(auditorium, 4, 5);  // Row E, Seat 6

    displaySeating(auditorium);

    return 0;
}
```

## 4. 3D Arrays and Higher Dimensions

### 4.1 3D Arrays

```cpp
#include <iostream>
using namespace std;

int main() {
    // 3D array: [depth][rows][cols]
    int cube[2][3][4];

    // Initialize with sequential values
    int value = 1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                cube[i][j][k] = value++;
            }
        }
    }

    // Print the 3D array
    for (int i = 0; i < 2; i++) {
        cout << "Layer " << i << ":" << endl;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                cout << cube[i][j][k] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
```

### 4.2 3D Array Application - RGB Image

```cpp
#include <iostream>
using namespace std;

const int HEIGHT = 3;
const int WIDTH = 4;
const int CHANNELS = 3;  // RGB

void initImage(int image[][WIDTH][CHANNELS]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            image[i][j][0] = i * 50;      // R
            image[i][j][1] = j * 50;      // G
            image[i][j][2] = (i + j) * 25; // B
        }
    }
}

void printImage(int image[][WIDTH][CHANNELS]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << "(" << image[i][j][0] << ","
                 << image[i][j][1] << ","
                 << image[i][j][2] << ") ";
        }
        cout << endl;
    }
}

int main() {
    int image[HEIGHT][WIDTH][CHANNELS];

    initImage(image);

    cout << "Image (RGB values):" << endl;
    printImage(image);

    return 0;
}
```

## 5. Practical Examples

### Example 1: Grade Book System

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

const int STUDENTS = 4;
const int SUBJECTS = 3;

void inputGrades(int grades[][SUBJECTS], const char* names[], const char* subjects[]) {
    for (int i = 0; i < STUDENTS; i++) {
        cout << "\nEnter grades for " << names[i] << ":" << endl;
        for (int j = 0; j < SUBJECTS; j++) {
            cout << "  " << subjects[j] << ": ";
            cin >> grades[i][j];
        }
    }
}

void printGradeBook(int grades[][SUBJECTS], const char* names[], const char* subjects[]) {
    cout << "\n" << setw(15) << "Student";
    for (int j = 0; j < SUBJECTS; j++) {
        cout << setw(10) << subjects[j];
    }
    cout << setw(10) << "Average" << endl;
    cout << string(55, '-') << endl;

    for (int i = 0; i < STUDENTS; i++) {
        cout << setw(15) << names[i];
        int sum = 0;
        for (int j = 0; j < SUBJECTS; j++) {
            cout << setw(10) << grades[i][j];
            sum += grades[i][j];
        }
        double avg = sum / (double)SUBJECTS;
        cout << setw(10) << fixed << setprecision(1) << avg << endl;
    }
}

int main() {
    int grades[STUDENTS][SUBJECTS];
    const char* students[] = {"Alice", "Bob", "Charlie", "Diana"};
    const char* subjects[] = {"Math", "English", "Science"};

    inputGrades(grades, students, subjects);
    printGradeBook(grades, students, subjects);

    return 0;
}
```

### Example 2: Matrix Multiplication

```cpp
#include <iostream>
using namespace std;

void multiplyMatrices(int a[][3], int b[][2], int result[][2], int rows1, int cols1, int cols2) {
    // Initialize result to 0
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
        }
    }

    // Perform multiplication
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void printMatrix(int mat[][2], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << mat[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    int mat1[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int mat2[3][2] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    int result[2][2];

    multiplyMatrices(mat1, mat2, result, 2, 3, 2);

    cout << "Result of matrix multiplication:" << endl;
    printMatrix(result, 2, 2);

    return 0;
}
```

## Exercises

### Exercise 1: Sudoku Board
Create a 9x9 Sudoku board and implement functions to print it and check if a number can be placed at a given position.

### Exercise 2: Pascal's Triangle
Generate and display Pascal's triangle using a 2D array.

### Exercise 3: Image Rotation
Implement a function that rotates a 2D array 90 degrees clockwise.

### Exercise 4: Magic Square
Create a function that checks if a 3x3 matrix is a magic square (all rows, columns, and diagonals sum to the same value).

### Exercise 5: Conway's Game of Life
Implement one generation of Conway's Game of Life on a 2D grid.

## Exercise Solutions

### Solution 1: Sudoku Board

```cpp
#include <iostream>
using namespace std;

const int SIZE = 9;

void printSudoku(int board[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) {
            cout << "------+-------+------" << endl;
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) {
                cout << "| ";
            }
            if (board[i][j] == 0) {
                cout << ". ";
            } else {
                cout << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}

bool canPlace(int board[][SIZE], int row, int col, int num) {
    // Check row
    for (int j = 0; j < SIZE; j++) {
        if (board[row][j] == num) return false;
    }

    // Check column
    for (int i = 0; i < SIZE; i++) {
        if (board[i][col] == num) return false;
    }

    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[boxRow + i][boxCol + j] == num) return false;
        }
    }

    return true;
}

int main() {
    int board[SIZE][SIZE] = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };

    printSudoku(board);

    cout << "\nCan place 2 at [0][2]? " << (canPlace(board, 0, 2, 2) ? "Yes" : "No") << endl;
    cout << "Can place 5 at [0][2]? " << (canPlace(board, 0, 2, 5) ? "Yes" : "No") << endl;

    return 0;
}
```

### Solution 2: Pascal's Triangle

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

const int ROWS = 10;

void generatePascal(int triangle[][ROWS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == 0 || j == i) {
                triangle[i][j] = 1;
            } else {
                triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j];
            }
        }
    }
}

void printPascal(int triangle[][ROWS]) {
    for (int i = 0; i < ROWS; i++) {
        // Print leading spaces
        for (int k = 0; k < ROWS - i - 1; k++) {
            cout << "  ";
        }

        // Print values
        for (int j = 0; j <= i; j++) {
            cout << setw(4) << triangle[i][j];
        }
        cout << endl;
    }
}

int main() {
    int pascal[ROWS][ROWS] = {0};

    generatePascal(pascal);
    printPascal(pascal);

    return 0;
}
```

### Solution 3: Image Rotation

```cpp
#include <iostream>
using namespace std;

const int SIZE = 4;

void rotate90Clockwise(int mat[][SIZE], int result[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[j][SIZE-1-i] = mat[i][j];
        }
    }
}

void printMatrix(int mat[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int original[SIZE][SIZE] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int rotated[SIZE][SIZE];

    cout << "Original:" << endl;
    printMatrix(original);

    rotate90Clockwise(original, rotated);

    cout << "\nRotated 90° clockwise:" << endl;
    printMatrix(rotated);

    return 0;
}
```

### Solution 4: Magic Square

```cpp
#include <iostream>
using namespace std;

const int SIZE = 3;

bool isMagicSquare(int mat[][SIZE]) {
    // Calculate the magic sum (should be same for all rows, cols, diagonals)
    int magicSum = 0;
    for (int j = 0; j < SIZE; j++) {
        magicSum += mat[0][j];
    }

    // Check rows
    for (int i = 0; i < SIZE; i++) {
        int rowSum = 0;
        for (int j = 0; j < SIZE; j++) {
            rowSum += mat[i][j];
        }
        if (rowSum != magicSum) return false;
    }

    // Check columns
    for (int j = 0; j < SIZE; j++) {
        int colSum = 0;
        for (int i = 0; i < SIZE; i++) {
            colSum += mat[i][j];
        }
        if (colSum != magicSum) return false;
    }

    // Check main diagonal
    int diagSum = 0;
    for (int i = 0; i < SIZE; i++) {
        diagSum += mat[i][i];
    }
    if (diagSum != magicSum) return false;

    // Check anti-diagonal
    diagSum = 0;
    for (int i = 0; i < SIZE; i++) {
        diagSum += mat[i][SIZE-1-i];
    }
    if (diagSum != magicSum) return false;

    return true;
}

int main() {
    int magic[SIZE][SIZE] = {
        {2, 7, 6},
        {9, 5, 1},
        {4, 3, 8}
    };

    int notMagic[SIZE][SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "First matrix is " << (isMagicSquare(magic) ? "" : "not ") << "a magic square" << endl;
    cout << "Second matrix is " << (isMagicSquare(notMagic) ? "" : "not ") << "a magic square" << endl;

    return 0;
}
```

### Solution 5: Conway's Game of Life

```cpp
#include <iostream>
using namespace std;

const int ROWS = 10;
const int COLS = 10;

void printGrid(int grid[][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << (grid[i][j] ? "■ " : "□ ");
        }
        cout << endl;
    }
}

int countNeighbors(int grid[][COLS], int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int newRow = row + i;
            int newCol = col + j;

            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS) {
                count += grid[newRow][newCol];
            }
        }
    }
    return count;
}

void nextGeneration(int current[][COLS], int next[][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = countNeighbors(current, i, j);

            if (current[i][j] == 1) {
                // Cell is alive
                next[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                // Cell is dead
                next[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
}

int main() {
    int grid[ROWS][COLS] = {0};

    // Create a glider pattern
    grid[1][2] = 1;
    grid[2][3] = 1;
    grid[3][1] = 1;
    grid[3][2] = 1;
    grid[3][3] = 1;

    cout << "Initial state:" << endl;
    printGrid(grid);

    int newGrid[ROWS][COLS];
    nextGeneration(grid, newGrid);

    cout << "\nNext generation:" << endl;
    printGrid(newGrid);

    return 0;
}
```

## Summary

In this lesson, you learned:

- **2D arrays** are arrays of arrays, like tables with rows and columns
- 2D arrays are accessed using **two indices**: `arr[row][col]`
- **Nested loops** are used to process 2D arrays
- Common 2D array operations: matrix math, searching, transformations
- **3D arrays** and higher dimensions follow the same principles
- 2D arrays are useful for **games, images, grids**, and tabular data

## Checklist

- [ ] I understand 2D arrays and how they're structured in memory
- [ ] I can declare and initialize multi-dimensional arrays
- [ ] I can use nested loops to process 2D arrays
- [ ] I can pass 2D arrays to functions
- [ ] I understand common matrix operations
- [ ] I completed all exercises and understand the solutions
- [ ] I can work with 3D arrays
- [ ] I can apply 2D arrays to solve practical problems

## Additional Resources

- **Next Lesson:** C-Style Strings
- **Related Topics:** Dynamic 2D Arrays, Matrix Libraries, Image Processing
- **Practice:** Implement more matrix algorithms and game boards

---

**Congratulations!** You've completed Lesson 17. You now understand multi-dimensional arrays, which are essential for working with grids, matrices, images, and complex data structures.
