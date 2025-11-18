# Lesson 14: Recursion

**Duration**: 6 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Recursion
2. Base Cases and Recursive Cases
3. Classic Recursive Problems
4. Recursion vs Iteration
5. Multiple Recursive Calls
6. Tail Recursion
7. Recursion with Backtracking
8. Exercises and Projects

---

## 1. Introduction to Recursion

### What is Recursion?

Recursion is when a function calls itself. It's a powerful technique for solving problems that can be broken down into smaller, similar sub-problems.

```cpp
#include <iostream>

void countdown(int n)
{
    if (n <= 0)
    {
        std::cout << "Blast off!\n";
        return;
    }

    std::cout << n << "...\n";
    countdown(n - 1);  // Recursive call
}

int main()
{
    countdown(5);
    return 0;
}
```

**Output**:
```
5...
4...
3...
2...
1...
Blast off!
```

### How Recursion Works:

```cpp
#include <iostream>

void visualizeRecursion(int n)
{
    std::cout << "Entering function with n = " << n << "\n";

    if (n <= 0)
    {
        std::cout << "Base case reached!\n";
        return;
    }

    visualizeRecursion(n - 1);  // Recursive call

    std::cout << "Returning from n = " << n << "\n";
}

int main()
{
    visualizeRecursion(3);
    return 0;
}
```

**Output**:
```
Entering function with n = 3
Entering function with n = 2
Entering function with n = 1
Entering function with n = 0
Base case reached!
Returning from n = 1
Returning from n = 2
Returning from n = 3
```

---

## 2. Base Cases and Recursive Cases

### The Anatomy of Recursion:

Every recursive function needs:
1. **Base case**: Condition to stop recursion
2. **Recursive case**: Function calls itself with modified parameters

```cpp
#include <iostream>

// Calculate factorial: n! = n * (n-1) * (n-2) * ... * 1
int factorial(int n)
{
    // Base case
    if (n <= 1)
    {
        return 1;
    }

    // Recursive case
    return n * factorial(n - 1);
}

int main()
{
    for (int i = 0; i <= 6; i++)
    {
        std::cout << i << "! = " << factorial(i) << "\n";
    }

    return 0;
}
```

**Output**:
```
0! = 1
1! = 1
2! = 2
3! = 6
4! = 24
5! = 120
6! = 720
```

### Multiple Base Cases:

```cpp
#include <iostream>

// Fibonacci: F(n) = F(n-1) + F(n-2)
int fibonacci(int n)
{
    // Base cases
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void printFibonacci(int count)
{
    for (int i = 0; i < count; i++)
    {
        std::cout << fibonacci(i) << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::cout << "First 10 Fibonacci numbers:\n";
    printFibonacci(10);

    return 0;
}
```

**Output**:
```
First 10 Fibonacci numbers:
0 1 1 2 3 5 8 13 21 34
```

---

## 3. Classic Recursive Problems

### Sum of Numbers:

```cpp
#include <iostream>

// Sum from 1 to n
int sum(int n)
{
    if (n <= 0)
        return 0;

    return n + sum(n - 1);
}

// Sum of array elements
int sumArray(int arr[], int size)
{
    if (size == 0)
        return 0;

    return arr[0] + sumArray(arr + 1, size - 1);
}

int main()
{
    std::cout << "Sum 1 to 5: " << sum(5) << "\n";
    std::cout << "Sum 1 to 10: " << sum(10) << "\n\n";

    int numbers[] = {2, 4, 6, 8, 10};
    std::cout << "Sum of array: " << sumArray(numbers, 5) << "\n";

    return 0;
}
```

### Power Function:

```cpp
#include <iostream>

// Calculate base^exponent
int power(int base, int exponent)
{
    // Base case
    if (exponent == 0)
        return 1;

    if (exponent < 0)
    {
        std::cout << "Negative exponents not supported\n";
        return 0;
    }

    // Recursive case
    return base * power(base, exponent - 1);
}

// Optimized version using divide and conquer
int powerFast(int base, int exponent)
{
    if (exponent == 0)
        return 1;

    int half = powerFast(base, exponent / 2);

    if (exponent % 2 == 0)
        return half * half;
    else
        return base * half * half;
}

int main()
{
    std::cout << "2^5 = " << power(2, 5) << "\n";
    std::cout << "3^4 = " << power(3, 4) << "\n";
    std::cout << "5^0 = " << power(5, 0) << "\n\n";

    std::cout << "Fast: 2^10 = " << powerFast(2, 10) << "\n";

    return 0;
}
```

### Greatest Common Divisor (GCD):

```cpp
#include <iostream>

// Euclidean algorithm
int gcd(int a, int b)
{
    // Base case
    if (b == 0)
        return a;

    // Recursive case
    return gcd(b, a % b);
}

int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

int main()
{
    std::cout << "GCD(48, 18) = " << gcd(48, 18) << "\n";
    std::cout << "GCD(100, 50) = " << gcd(100, 50) << "\n";
    std::cout << "GCD(17, 19) = " << gcd(17, 19) << "\n\n";

    std::cout << "LCM(12, 18) = " << lcm(12, 18) << "\n";

    return 0;
}
```

### Digit Sum:

```cpp
#include <iostream>

int digitSum(int n)
{
    // Base case
    if (n == 0)
        return 0;

    // Recursive case: last digit + sum of remaining digits
    return (n % 10) + digitSum(n / 10);
}

int countDigits(int n)
{
    if (n == 0)
        return 0;

    return 1 + countDigits(n / 10);
}

int reverseNumber(int n, int reversed = 0)
{
    if (n == 0)
        return reversed;

    return reverseNumber(n / 10, reversed * 10 + n % 10);
}

int main()
{
    int num = 12345;

    std::cout << "Number: " << num << "\n";
    std::cout << "Digit sum: " << digitSum(num) << "\n";
    std::cout << "Digit count: " << countDigits(num) << "\n";
    std::cout << "Reversed: " << reverseNumber(num) << "\n";

    return 0;
}
```

---

## 4. Recursion vs Iteration

### Same Problem, Different Approaches:

```cpp
#include <iostream>
#include <chrono>

// Recursive factorial
int factorialRecursive(int n)
{
    if (n <= 1)
        return 1;
    return n * factorialRecursive(n - 1);
}

// Iterative factorial
int factorialIterative(int n)
{
    int result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// Recursive Fibonacci (inefficient)
int fibRecursive(int n)
{
    if (n <= 1)
        return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// Iterative Fibonacci (efficient)
int fibIterative(int n)
{
    if (n <= 1)
        return n;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++)
    {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main()
{
    std::cout << "=== FACTORIAL ===\n";
    std::cout << "Recursive 5! = " << factorialRecursive(5) << "\n";
    std::cout << "Iterative 5! = " << factorialIterative(5) << "\n\n";

    std::cout << "=== FIBONACCI ===\n";
    int n = 20;

    auto start = std::chrono::high_resolution_clock::now();
    int result1 = fibRecursive(n);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    int result2 = fibIterative(n);
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Recursive fib(" << n << ") = " << result1
              << " (Time: " << duration1.count() << " μs)\n";
    std::cout << "Iterative fib(" << n << ") = " << result2
              << " (Time: " << duration2.count() << " μs)\n";

    return 0;
}
```

### When to Use Recursion:

```cpp
#include <iostream>
#include <string>

// Tree traversal - natural for recursion
struct TreeNode
{
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

void printTree(TreeNode* node, int depth = 0)
{
    if (node == nullptr)
        return;

    // Print right subtree
    printTree(node->right, depth + 1);

    // Print current node
    std::cout << std::string(depth * 4, ' ') << node->value << "\n";

    // Print left subtree
    printTree(node->left, depth + 1);
}

int treeHeight(TreeNode* node)
{
    if (node == nullptr)
        return 0;

    int leftHeight = treeHeight(node->left);
    int rightHeight = treeHeight(node->right);

    return 1 + std::max(leftHeight, rightHeight);
}

int main()
{
    // Build a simple tree
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(7);
    root->right->right = new TreeNode(9);

    std::cout << "Tree structure:\n";
    printTree(root);

    std::cout << "\nTree height: " << treeHeight(root) << "\n";

    // Cleanup (in practice, use smart pointers)
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
```

---

## 5. Multiple Recursive Calls

### Fibonacci Tree:

```cpp
#include <iostream>

void printFibonacciTree(int n, int depth = 0)
{
    std::cout << std::string(depth * 2, ' ') << "fib(" << n << ")\n";

    if (n <= 1)
        return;

    printFibonacciTree(n - 1, depth + 1);
    printFibonacciTree(n - 2, depth + 1);
}

int main()
{
    std::cout << "Fibonacci call tree for fib(5):\n";
    printFibonacciTree(5);

    return 0;
}
```

### Tower of Hanoi:

```cpp
#include <iostream>

void towerOfHanoi(int n, char from, char to, char aux)
{
    if (n == 1)
    {
        std::cout << "Move disk 1 from " << from << " to " << to << "\n";
        return;
    }

    // Move n-1 disks from 'from' to 'aux' using 'to'
    towerOfHanoi(n - 1, from, aux, to);

    // Move the largest disk from 'from' to 'to'
    std::cout << "Move disk " << n << " from " << from << " to " << to << "\n";

    // Move n-1 disks from 'aux' to 'to' using 'from'
    towerOfHanoi(n - 1, aux, to, from);
}

int main()
{
    int disks = 3;

    std::cout << "Tower of Hanoi solution for " << disks << " disks:\n";
    towerOfHanoi(disks, 'A', 'C', 'B');

    return 0;
}
```

### Permutations:

```cpp
#include <iostream>
#include <string>

void generatePermutations(std::string str, int left, int right)
{
    if (left == right)
    {
        std::cout << str << "\n";
        return;
    }

    for (int i = left; i <= right; i++)
    {
        // Swap
        std::swap(str[left], str[i]);

        // Recurse
        generatePermutations(str, left + 1, right);

        // Backtrack (swap back)
        std::swap(str[left], str[i]);
    }
}

int main()
{
    std::string word = "ABC";

    std::cout << "All permutations of " << word << ":\n";
    generatePermutations(word, 0, word.length() - 1);

    return 0;
}
```

---

## 6. Tail Recursion

### What is Tail Recursion?

Tail recursion occurs when the recursive call is the last operation in the function. Some compilers can optimize tail recursion to iteration.

```cpp
#include <iostream>

// NOT tail recursive - multiplication happens after recursive call
int factorialNotTail(int n)
{
    if (n <= 1)
        return 1;

    return n * factorialNotTail(n - 1);  // Not tail recursive
}

// Tail recursive - uses accumulator
int factorialTail(int n, int accumulator = 1)
{
    if (n <= 1)
        return accumulator;

    return factorialTail(n - 1, n * accumulator);  // Tail recursive
}

// NOT tail recursive
int sumNotTail(int n)
{
    if (n <= 0)
        return 0;

    return n + sumNotTail(n - 1);  // Addition after recursive call
}

// Tail recursive
int sumTail(int n, int accumulator = 0)
{
    if (n <= 0)
        return accumulator;

    return sumTail(n - 1, accumulator + n);  // Tail recursive
}

int main()
{
    std::cout << "Factorial 5: " << factorialTail(5) << "\n";
    std::cout << "Sum 1-10: " << sumTail(10) << "\n";

    return 0;
}
```

### Converting to Tail Recursion:

```cpp
#include <iostream>

// Original Fibonacci (not tail recursive)
int fibNormal(int n)
{
    if (n <= 1)
        return n;
    return fibNormal(n - 1) + fibNormal(n - 2);
}

// Tail recursive Fibonacci
int fibTail(int n, int prev = 0, int curr = 1)
{
    if (n == 0)
        return prev;
    if (n == 1)
        return curr;

    return fibTail(n - 1, curr, prev + curr);
}

int main()
{
    std::cout << "Fibonacci 10 (normal): " << fibNormal(10) << "\n";
    std::cout << "Fibonacci 10 (tail): " << fibTail(10) << "\n";

    return 0;
}
```

---

## 7. Recursion with Backtracking

### Subset Generation:

```cpp
#include <iostream>
#include <vector>

void generateSubsets(std::vector<int>& nums, std::vector<int>& current,
                    int index)
{
    // Print current subset
    std::cout << "{ ";
    for (int num : current)
    {
        std::cout << num << " ";
    }
    std::cout << "}\n";

    // Generate subsets by including elements from index onwards
    for (size_t i = index; i < nums.size(); i++)
    {
        current.push_back(nums[i]);           // Include
        generateSubsets(nums, current, i + 1); // Recurse
        current.pop_back();                    // Backtrack
    }
}

int main()
{
    std::vector<int> numbers = {1, 2, 3};
    std::vector<int> current;

    std::cout << "All subsets of {1, 2, 3}:\n";
    generateSubsets(numbers, current, 0);

    return 0;
}
```

### N-Queens Problem:

```cpp
#include <iostream>
#include <vector>

class NQueens
{
private:
    int size;
    std::vector<std::vector<int>> board;
    int solutionCount;

    bool isSafe(int row, int col)
    {
        // Check column
        for (int i = 0; i < row; i++)
        {
            if (board[i][col] == 1)
                return false;
        }

        // Check upper left diagonal
        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
        {
            if (board[i][j] == 1)
                return false;
        }

        // Check upper right diagonal
        for (int i = row, j = col; i >= 0 && j < size; i--, j++)
        {
            if (board[i][j] == 1)
                return false;
        }

        return true;
    }

    void printBoard()
    {
        std::cout << "\nSolution " << solutionCount << ":\n";
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                std::cout << (board[i][j] ? "Q " : ". ");
            }
            std::cout << "\n";
        }
    }

    void solve(int row)
    {
        if (row == size)
        {
            solutionCount++;
            printBoard();
            return;
        }

        for (int col = 0; col < size; col++)
        {
            if (isSafe(row, col))
            {
                board[row][col] = 1;        // Place queen
                solve(row + 1);              // Recurse
                board[row][col] = 0;        // Backtrack
            }
        }
    }

public:
    NQueens(int n) : size(n), solutionCount(0)
    {
        board.resize(n, std::vector<int>(n, 0));
    }

    void findSolutions()
    {
        solve(0);
        std::cout << "\nTotal solutions: " << solutionCount << "\n";
    }
};

int main()
{
    NQueens queens(4);
    queens.findSolutions();

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: String Recursion

```cpp
#include <iostream>
#include <string>

// Reverse string
std::string reverse(std::string str)
{
    if (str.length() <= 1)
        return str;

    return reverse(str.substr(1)) + str[0];
}

// Check palindrome
bool isPalindrome(const std::string& str, int left, int right)
{
    if (left >= right)
        return true;

    if (str[left] != str[right])
        return false;

    return isPalindrome(str, left + 1, right - 1);
}

// Count occurrences of character
int countChar(const std::string& str, char ch, int index = 0)
{
    if (index >= str.length())
        return 0;

    int count = (str[index] == ch) ? 1 : 0;
    return count + countChar(str, ch, index + 1);
}

// Remove character
std::string removeChar(const std::string& str, char ch)
{
    if (str.empty())
        return "";

    if (str[0] == ch)
        return removeChar(str.substr(1), ch);
    else
        return str[0] + removeChar(str.substr(1), ch);
}

int main()
{
    std::string text = "hello";

    std::cout << "Original: " << text << "\n";
    std::cout << "Reversed: " << reverse(text) << "\n\n";

    std::string palindrome = "racecar";
    std::cout << palindrome << " is palindrome: "
              << (isPalindrome(palindrome, 0, palindrome.length() - 1) ? "Yes" : "No") << "\n";

    std::cout << "hello is palindrome: "
              << (isPalindrome(text, 0, text.length() - 1) ? "Yes" : "No") << "\n\n";

    std::cout << "Count 'l' in 'hello': " << countChar(text, 'l') << "\n\n";

    std::cout << "Remove 'l' from 'hello': " << removeChar(text, 'l') << "\n";

    return 0;
}
```

### Exercise 2: Array Recursion

```cpp
#include <iostream>

// Find maximum
int findMax(int arr[], int size)
{
    if (size == 1)
        return arr[0];

    int maxRest = findMax(arr, size - 1);
    return (arr[size - 1] > maxRest) ? arr[size - 1] : maxRest;
}

// Binary search
int binarySearch(int arr[], int left, int right, int target)
{
    if (left > right)
        return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
        return mid;

    if (arr[mid] > target)
        return binarySearch(arr, left, mid - 1, target);
    else
        return binarySearch(arr, mid + 1, right, target);
}

// Check if sorted
bool isSorted(int arr[], int size)
{
    if (size <= 1)
        return true;

    if (arr[0] > arr[1])
        return false;

    return isSorted(arr + 1, size - 1);
}

// Print array
void printArray(int arr[], int size)
{
    if (size == 0)
    {
        std::cout << "\n";
        return;
    }

    std::cout << arr[0] << " ";
    printArray(arr + 1, size - 1);
}

int main()
{
    int numbers[] = {3, 7, 2, 9, 5, 1, 8};
    int size = 7;

    std::cout << "Array: ";
    printArray(numbers, size);

    std::cout << "Maximum: " << findMax(numbers, size) << "\n\n";

    int sorted[] = {1, 3, 5, 7, 9, 11, 13};
    std::cout << "Sorted array: ";
    printArray(sorted, 7);

    std::cout << "Is sorted: " << (isSorted(sorted, 7) ? "Yes" : "No") << "\n";
    std::cout << "Is unsorted array sorted: " << (isSorted(numbers, size) ? "Yes" : "No") << "\n\n";

    int target = 7;
    int index = binarySearch(sorted, 0, 6, target);
    std::cout << "Binary search for " << target << ": ";
    if (index != -1)
        std::cout << "found at index " << index << "\n";
    else
        std::cout << "not found\n";

    return 0;
}
```

### Exercise 3: Mathematical Recursion

```cpp
#include <iostream>

// Sum of digits
int digitSum(int n)
{
    if (n == 0)
        return 0;
    return (n % 10) + digitSum(n / 10);
}

// Count digits
int countDigits(int n)
{
    if (n == 0)
        return 0;
    return 1 + countDigits(n / 10);
}

// Convert decimal to binary
void decimalToBinary(int n)
{
    if (n == 0)
        return;

    decimalToBinary(n / 2);
    std::cout << (n % 2);
}

// Sum of series: 1 + 1/2 + 1/3 + ... + 1/n
double harmonicSum(int n)
{
    if (n == 1)
        return 1.0;
    return 1.0 / n + harmonicSum(n - 1);
}

// Ackermann function (grows very fast)
int ackermann(int m, int n)
{
    if (m == 0)
        return n + 1;
    if (n == 0)
        return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int main()
{
    int num = 12345;

    std::cout << "Number: " << num << "\n";
    std::cout << "Digit sum: " << digitSum(num) << "\n";
    std::cout << "Digit count: " << countDigits(num) << "\n";

    std::cout << "Binary: ";
    if (num == 0)
        std::cout << "0";
    else
        decimalToBinary(num);
    std::cout << "\n\n";

    std::cout << "Harmonic sum (10): " << harmonicSum(10) << "\n\n";

    std::cout << "Ackermann(2, 3) = " << ackermann(2, 3) << "\n";

    return 0;
}
```

---

## Practice Project: Maze Solver

```cpp
#include <iostream>
#include <vector>

class MazeSolver
{
private:
    std::vector<std::vector<int>> maze;
    std::vector<std::vector<int>> solution;
    int rows, cols;

public:
    MazeSolver(std::vector<std::vector<int>> m) : maze(m)
    {
        rows = maze.size();
        cols = maze[0].size();
        solution.resize(rows, std::vector<int>(cols, 0));
    }

    void printMaze()
    {
        std::cout << "\nMaze (0=path, 1=wall):\n";
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::cout << maze[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    void printSolution()
    {
        std::cout << "\nSolution (1=path taken):\n";
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (solution[i][j] == 1)
                    std::cout << "* ";
                else if (maze[i][j] == 1)
                    std::cout << "# ";
                else
                    std::cout << ". ";
            }
            std::cout << "\n";
        }
    }

    bool isSafe(int x, int y)
    {
        return (x >= 0 && x < rows && y >= 0 && y < cols &&
                maze[x][y] == 0 && solution[x][y] == 0);
    }

    bool solveMazeUtil(int x, int y)
    {
        // Reached destination
        if (x == rows - 1 && y == cols - 1)
        {
            solution[x][y] = 1;
            return true;
        }

        if (isSafe(x, y))
        {
            solution[x][y] = 1;  // Mark as part of solution

            // Try moving right
            if (solveMazeUtil(x, y + 1))
                return true;

            // Try moving down
            if (solveMazeUtil(x + 1, y))
                return true;

            // Try moving left
            if (solveMazeUtil(x, y - 1))
                return true;

            // Try moving up
            if (solveMazeUtil(x - 1, y))
                return true;

            // Backtrack
            solution[x][y] = 0;
            return false;
        }

        return false;
    }

    bool solve()
    {
        if (solveMazeUtil(0, 0))
        {
            std::cout << "\nSolution found!\n";
            printSolution();
            return true;
        }
        else
        {
            std::cout << "\nNo solution exists!\n";
            return false;
        }
    }
};

int main()
{
    std::vector<std::vector<int>> maze = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    MazeSolver solver(maze);
    solver.printMaze();
    solver.solve();

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Missing Base Case
```cpp
// ❌ INFINITE RECURSION
int bad(int n) {
    return n + bad(n - 1);  // No base case!
}

// ✅ CORRECT
int good(int n) {
    if (n <= 0)  // Base case
        return 0;
    return n + good(n - 1);
}
```

### Mistake 2: Wrong Base Case
```cpp
// ❌ WRONG: Will cause stack overflow for n < 0
int factorial(int n) {
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

// ✅ CORRECT: Handles negative input
int factorial(int n) {
    if (n <= 1)  // Covers n <= 0
        return 1;
    return n * factorial(n - 1);
}
```

### Mistake 3: Not Making Progress
```cpp
// ❌ INFINITE: n never changes
int bad(int n) {
    if (n == 0)
        return 0;
    return n + bad(n);  // Same n!
}

// ✅ CORRECT: n decreases
int good(int n) {
    if (n == 0)
        return 0;
    return n + good(n - 1);  // n-1 moves toward base case
}
```

---

## Key Takeaways

1. Recursion requires base case and recursive case
2. Base case stops the recursion
3. Recursive case must progress toward base case
4. Some problems are naturally recursive (trees, backtracking)
5. Recursion can be less efficient than iteration
6. Tail recursion can be optimized by compiler
7. Use recursion for clarity, iteration for performance
8. Watch out for stack overflow with deep recursion

---

## Summary Checklist

Before moving to Lesson 15, ensure you can:
- [ ] Write recursive functions with proper base cases
- [ ] Understand how recursion works (call stack)
- [ ] Solve classic recursive problems
- [ ] Compare recursion vs iteration
- [ ] Implement tail recursion
- [ ] Use backtracking with recursion
- [ ] Debug recursive functions
- [ ] Recognize when to use recursion

---

## Next Lesson Preview

In **Lesson 15**, we'll explore:
- Inline functions
- When to use inline
- Default function arguments
- Function templates introduction
- Best practices for function optimization

**Master recursion - it's powerful!**
