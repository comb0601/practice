# Lesson 24: Recursion Deep Dive

## Duration
3 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Master recursive problem-solving techniques
- Understand recursion vs iteration trade-offs
- Implement tail recursion optimization
- Use memoization for recursive functions
- Solve complex problems using recursion
- Debug and optimize recursive algorithms
- Understand recursion depth and stack limitations

## Introduction

Recursion is a powerful programming technique where a function calls itself to solve smaller instances of the same problem. This lesson explores advanced recursive concepts, optimization techniques, and real-world applications.

## 1. Understanding Recursion Fundamentals

### Basic Recursive Patterns

```cpp
#include <iostream>
using namespace std;

// Simple countdown
void countdown(int n) {
    if (n <= 0) {  // Base case
        cout << "Blast off!" << endl;
        return;
    }
    cout << n << "..." << endl;
    countdown(n - 1);  // Recursive case
}

// Factorial
long long factorial(int n) {
    if (n <= 1) return 1;  // Base case
    return n * factorial(n - 1);  // Recursive case
}

// Fibonacci
int fibonacci(int n) {
    if (n <= 1) return n;  // Base cases
    return fibonacci(n - 1) + fibonacci(n - 2);  // Recursive case
}

// Sum of array
int arraySum(const int arr[], int size) {
    if (size <= 0) return 0;  // Base case
    return arr[size - 1] + arraySum(arr, size - 1);  // Recursive case
}

// Power function
double power(double base, int exponent) {
    if (exponent == 0) return 1;  // Base case
    if (exponent < 0) return 1.0 / power(base, -exponent);
    return base * power(base, exponent - 1);  // Recursive case
}

int main() {
    cout << "=== Countdown ===" << endl;
    countdown(5);
    cout << endl;

    cout << "=== Factorial ===" << endl;
    cout << "5! = " << factorial(5) << endl;
    cout << "10! = " << factorial(10) << endl;
    cout << endl;

    cout << "=== Fibonacci ===" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << "F(" << i << ") = " << fibonacci(i) << endl;
    }
    cout << endl;

    cout << "=== Array Sum ===" << endl;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "Sum = " << arraySum(arr, 10) << endl;
    cout << endl;

    cout << "=== Power ===" << endl;
    cout << "2^10 = " << power(2, 10) << endl;
    cout << "2^-3 = " << power(2, -3) << endl;

    return 0;
}
```

## 2. Tail Recursion

### Tail Recursion Optimization

```cpp
#include <iostream>
using namespace std;

// Non-tail recursive factorial (not optimizable)
long long factorialNonTail(int n) {
    if (n <= 1) return 1;
    return n * factorialNonTail(n - 1);  // Operation after recursive call
}

// Tail recursive factorial (optimizable)
long long factorialTail(int n, long long accumulator = 1) {
    if (n <= 1) return accumulator;
    return factorialTail(n - 1, n * accumulator);  // No operation after call
}

// Non-tail recursive sum
int sumNonTail(int n) {
    if (n <= 0) return 0;
    return n + sumNonTail(n - 1);
}

// Tail recursive sum
int sumTail(int n, int accumulator = 0) {
    if (n <= 0) return accumulator;
    return sumTail(n - 1, accumulator + n);
}

// Tail recursive Fibonacci
long long fibonacciTail(int n, long long a = 0, long long b = 1) {
    if (n == 0) return a;
    if (n == 1) return b;
    return fibonacciTail(n - 1, b, a + b);
}

// Tail recursive GCD (Greatest Common Divisor)
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);  // Tail recursive
}

// Tail recursive reverse print
void reversePrintTail(const int arr[], int index, int size) {
    if (index >= size) return;
    reversePrintTail(arr, index + 1, size);  // Recursive call first
    cout << arr[index] << " ";  // Print after recursion
}

int main() {
    cout << "=== Factorial Comparison ===" << endl;
    cout << "Non-tail 10! = " << factorialNonTail(10) << endl;
    cout << "Tail 10! = " << factorialTail(10) << endl;
    cout << endl;

    cout << "=== Sum Comparison ===" << endl;
    cout << "Non-tail sum(100) = " << sumNonTail(100) << endl;
    cout << "Tail sum(100) = " << sumTail(100) << endl;
    cout << endl;

    cout << "=== Fibonacci ===" << endl;
    cout << "Tail fib(20) = " << fibonacciTail(20) << endl;
    cout << endl;

    cout << "=== GCD ===" << endl;
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "GCD(100, 35) = " << gcd(100, 35) << endl;
    cout << endl;

    cout << "=== Reverse Print ===" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    reversePrintTail(arr, 0, 5);
    cout << endl;

    return 0;
}
```

## 3. Memoization for Optimization

### Caching Results

```cpp
#include <iostream>
#include <map>
#include <chrono>
using namespace std;

// Slow Fibonacci without memoization
long long fibSlow(int n) {
    if (n <= 1) return n;
    return fibSlow(n - 1) + fibSlow(n - 2);
}

// Fibonacci with memoization
map<int, long long> fibMemo;

long long fibFast(int n) {
    if (n <= 1) return n;

    // Check if already calculated
    if (fibMemo.find(n) != fibMemo.end()) {
        return fibMemo[n];
    }

    // Calculate and store
    fibMemo[n] = fibFast(n - 1) + fibFast(n - 2);
    return fibMemo[n];
}

// Grid paths problem with memoization
map<pair<int, int>, long long> pathMemo;

long long gridPaths(int x, int y) {
    // Base cases
    if (x == 0 || y == 0) return 1;
    if (x < 0 || y < 0) return 0;

    // Check memo
    pair<int, int> key = {x, y};
    if (pathMemo.find(key) != pathMemo.end()) {
        return pathMemo[key];
    }

    // Calculate and memoize
    pathMemo[key] = gridPaths(x - 1, y) + gridPaths(x, y - 1);
    return pathMemo[key];
}

// Longest Common Subsequence with memoization
map<pair<int, int>, int> lcsMemo;

int lcs(const string& s1, const string& s2, int i, int j) {
    if (i < 0 || j < 0) return 0;

    pair<int, int> key = {i, j};
    if (lcsMemo.find(key) != lcsMemo.end()) {
        return lcsMemo[key];
    }

    if (s1[i] == s2[j]) {
        lcsMemo[key] = 1 + lcs(s1, s2, i - 1, j - 1);
    } else {
        lcsMemo[key] = max(lcs(s1, s2, i - 1, j), lcs(s1, s2, i, j - 1));
    }

    return lcsMemo[key];
}

int main() {
    cout << "=== Fibonacci Performance ===" << endl;

    auto start = chrono::high_resolution_clock::now();
    long long result1 = fibSlow(30);
    auto end = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    long long result2 = fibFast(30);
    end = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Slow fib(30) = " << result1 << " in " << duration1 << " ms" << endl;
    cout << "Fast fib(30) = " << result2 << " in " << duration2 << " ms" << endl;
    cout << "Speedup: " << (duration1 / max(duration2, 1LL)) << "x faster" << endl;
    cout << endl;

    cout << "=== Grid Paths ===" << endl;
    cout << "Paths in 5x5 grid: " << gridPaths(5, 5) << endl;
    cout << "Paths in 10x10 grid: " << gridPaths(10, 10) << endl;
    cout << endl;

    cout << "=== Longest Common Subsequence ===" << endl;
    string s1 = "ABCDGH";
    string s2 = "AEDFHR";
    cout << "LCS(\"" << s1 << "\", \"" << s2 << "\") = "
         << lcs(s1, s2, s1.length() - 1, s2.length() - 1) << endl;

    return 0;
}
```

## 4. Advanced Recursive Patterns

### Backtracking and Divide-and-Conquer

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Binary search (divide and conquer)
int binarySearch(const vector<int>& arr, int target, int left, int right) {
    if (left > right) return -1;  // Not found

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binarySearch(arr, target, left, mid - 1);
    return binarySearch(arr, target, mid + 1, right);
}

// Quick sort (divide and conquer)
void quickSort(vector<int>& arr, int low, int high) {
    if (low >= high) return;

    // Partition
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    int pivotIndex = i + 1;

    // Recursive calls
    quickSort(arr, low, pivotIndex - 1);
    quickSort(arr, pivotIndex + 1, high);
}

// Merge sort
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// N-Queens problem (backtracking)
bool isSafe(const vector<vector<int>>& board, int row, int col, int n) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) return false;
    }

    // Check upper left diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) return false;
    }

    // Check upper right diagonal
    for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) return false;
    }

    return true;
}

bool solveNQueens(vector<vector<int>>& board, int row, int n) {
    if (row >= n) return true;  // All queens placed

    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;  // Place queen

            if (solveNQueens(board, row + 1, n)) {
                return true;
            }

            board[row][col] = 0;  // Backtrack
        }
    }

    return false;
}

void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            cout << (cell ? "Q " : ". ");
        }
        cout << endl;
    }
}

int main() {
    cout << "=== Binary Search ===" << endl;
    vector<int> sortedArr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int target = 11;
    int index = binarySearch(sortedArr, target, 0, sortedArr.size() - 1);
    cout << "Found " << target << " at index " << index << endl;
    cout << endl;

    cout << "=== Quick Sort ===" << endl;
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    cout << "Before: ";
    for (int x : arr1) cout << x << " ";
    cout << endl;

    quickSort(arr1, 0, arr1.size() - 1);

    cout << "After:  ";
    for (int x : arr1) cout << x << " ";
    cout << endl << endl;

    cout << "=== Merge Sort ===" << endl;
    vector<int> arr2 = {64, 34, 25, 12, 22, 11, 90};
    cout << "Before: ";
    for (int x : arr2) cout << x << " ";
    cout << endl;

    mergeSort(arr2, 0, arr2.size() - 1);

    cout << "After:  ";
    for (int x : arr2) cout << x << " ";
    cout << endl << endl;

    cout << "=== 8-Queens Solution ===" << endl;
    int n = 8;
    vector<vector<int>> board(n, vector<int>(n, 0));

    if (solveNQueens(board, 0, n)) {
        printBoard(board);
    } else {
        cout << "No solution found" << endl;
    }

    return 0;
}
```

## 5. Tree and Graph Recursion

### Recursive Data Structure Traversal

```cpp
#include <iostream>
#include <vector>
using namespace std;

// Binary tree node
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Recursive tree traversals
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void preorder(TreeNode* root) {
    if (!root) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

// Tree height
int height(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

// Count nodes
int countNodes(TreeNode* root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Sum of all nodes
int sumNodes(TreeNode* root) {
    if (!root) return 0;
    return root->data + sumNodes(root->left) + sumNodes(root->right);
}

// Search in tree
bool search(TreeNode* root, int value) {
    if (!root) return false;
    if (root->data == value) return true;
    return search(root->left, value) || search(root->right, value);
}

// Graph DFS (using adjacency list)
void dfs(const vector<vector<int>>& graph, int node, vector<bool>& visited) {
    visited[node] = true;
    cout << node << " ";

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(graph, neighbor, visited);
        }
    }
}

int main() {
    // Build sample tree
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    cout << "=== Tree Traversals ===" << endl;
    cout << "Inorder:   "; inorder(root); cout << endl;
    cout << "Preorder:  "; preorder(root); cout << endl;
    cout << "Postorder: "; postorder(root); cout << endl;
    cout << endl;

    cout << "=== Tree Properties ===" << endl;
    cout << "Height: " << height(root) << endl;
    cout << "Node count: " << countNodes(root) << endl;
    cout << "Sum of nodes: " << sumNodes(root) << endl;
    cout << "Search for 5: " << (search(root, 5) ? "Found" : "Not found") << endl;
    cout << "Search for 10: " << (search(root, 10) ? "Found" : "Not found") << endl;
    cout << endl;

    cout << "=== Graph DFS ===" << endl;
    vector<vector<int>> graph = {
        {1, 2},     // Node 0
        {0, 3, 4},  // Node 1
        {0, 5},     // Node 2
        {1},        // Node 3
        {1, 5},     // Node 4
        {2, 4}      // Node 5
    };

    vector<bool> visited(6, false);
    cout << "DFS from node 0: ";
    dfs(graph, 0, visited);
    cout << endl;

    // Clean up
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

## Exercises

### Exercise 1: Recursive String Operations
Implement recursive functions for: string reversal, palindrome checking, substring counting, and permutation generation.

### Exercise 2: Tower of Hanoi
Solve the classic Tower of Hanoi problem recursively and visualize the moves.

### Exercise 3: Maze Solver
Implement a recursive backtracking algorithm to find a path through a maze.

### Exercise 4: Subset Sum Problem
Use recursion to find all subsets of an array that sum to a target value.

### Exercise 5: Expression Evaluator
Create a recursive descent parser to evaluate mathematical expressions.

## Exercise Solutions

### Solution 1: Recursive String Operations

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Reverse string recursively
string reverseString(const string& str) {
    if (str.length() <= 1) return str;
    return reverseString(str.substr(1)) + str[0];
}

// Check palindrome recursively
bool isPalindrome(const string& str, int left, int right) {
    if (left >= right) return true;
    if (str[left] != str[right]) return false;
    return isPalindrome(str, left + 1, right - 1);
}

// Count substring occurrences
int countSubstring(const string& str, const string& sub, int pos = 0) {
    if (pos + sub.length() > str.length()) return 0;

    int count = (str.substr(pos, sub.length()) == sub) ? 1 : 0;
    return count + countSubstring(str, sub, pos + 1);
}

// Generate permutations
void generatePermutations(string str, int left, int right, vector<string>& result) {
    if (left == right) {
        result.push_back(str);
        return;
    }

    for (int i = left; i <= right; i++) {
        swap(str[left], str[i]);
        generatePermutations(str, left + 1, right, result);
        swap(str[left], str[i]);  // Backtrack
    }
}

// Remove adjacent duplicates
string removeAdjacentDuplicates(const string& str) {
    if (str.length() <= 1) return str;

    string result;
    if (str[0] != str[1]) {
        result = str[0] + removeAdjacentDuplicates(str.substr(1));
    } else {
        // Skip both duplicates
        size_t i = 1;
        while (i < str.length() && str[i] == str[0]) i++;
        if (i < str.length()) {
            result = removeAdjacentDuplicates(str.substr(i));
        }
    }
    return result;
}

int main() {
    string test1 = "Hello World";
    string test2 = "radar";
    string test3 = "abcabcabc";

    cout << "=== String Reversal ===" << endl;
    cout << "Original: " << test1 << endl;
    cout << "Reversed: " << reverseString(test1) << endl;
    cout << endl;

    cout << "=== Palindrome Check ===" << endl;
    cout << test2 << " is palindrome? "
         << (isPalindrome(test2, 0, test2.length() - 1) ? "Yes" : "No") << endl;
    cout << test1 << " is palindrome? "
         << (isPalindrome(test1, 0, test1.length() - 1) ? "Yes" : "No") << endl;
    cout << endl;

    cout << "=== Substring Count ===" << endl;
    cout << "Occurrences of 'abc' in '" << test3 << "': "
         << countSubstring(test3, "abc") << endl;
    cout << endl;

    cout << "=== Permutations ===" << endl;
    string perm = "ABC";
    vector<string> permutations;
    generatePermutations(perm, 0, perm.length() - 1, permutations);
    cout << "Permutations of '" << perm << "':" << endl;
    for (const auto& p : permutations) {
        cout << p << " ";
    }
    cout << endl << endl;

    cout << "=== Remove Adjacent Duplicates ===" << endl;
    string dup = "aabbbaac";
    cout << "Original: " << dup << endl;
    cout << "After removal: " << removeAdjacentDuplicates(dup) << endl;

    return 0;
}
```

### Solution 2: Tower of Hanoi

```cpp
#include <iostream>
#include <vector>
using namespace std;

int moveCount = 0;

void towerOfHanoi(int n, char source, char destination, char auxiliary) {
    if (n == 1) {
        moveCount++;
        cout << "Move " << moveCount << ": Disk 1 from " << source
             << " to " << destination << endl;
        return;
    }

    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoi(n - 1, source, auxiliary, destination);

    // Move the largest disk from source to destination
    moveCount++;
    cout << "Move " << moveCount << ": Disk " << n << " from " << source
         << " to " << destination << endl;

    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoi(n - 1, auxiliary, destination, source);
}

// Version with visualization
class TowerOfHanoiVisual {
private:
    vector<vector<int>> towers;
    int numDisks;
    int moves;

public:
    TowerOfHanoiVisual(int n) : numDisks(n), moves(0) {
        towers.resize(3);
        for (int i = n; i >= 1; i--) {
            towers[0].push_back(i);
        }
    }

    void solve() {
        moves = 0;
        cout << "Initial state:" << endl;
        display();
        cout << endl;

        solveTower(numDisks, 0, 2, 1);

        cout << "\nTotal moves: " << moves << endl;
        cout << "Minimum moves required: " << ((1 << numDisks) - 1) << endl;
    }

private:
    void solveTower(int n, int from, int to, int aux) {
        if (n == 1) {
            moveDisk(from, to);
            return;
        }

        solveTower(n - 1, from, aux, to);
        moveDisk(from, to);
        solveTower(n - 1, aux, to, from);
    }

    void moveDisk(int from, int to) {
        moves++;
        int disk = towers[from].back();
        towers[from].pop_back();
        towers[to].push_back(disk);

        cout << "Move " << moves << ": Disk " << disk << " from "
             << (char)('A' + from) << " to " << (char)('A' + to) << endl;
        display();
        cout << endl;
    }

    void display() {
        cout << "  A    B    C" << endl;
        cout << "-------------" << endl;

        int maxHeight = numDisks;
        for (int level = maxHeight - 1; level >= 0; level--) {
            for (int tower = 0; tower < 3; tower++) {
                cout << " ";
                if (level < towers[tower].size()) {
                    int disk = towers[tower][level];
                    cout << disk;
                } else {
                    cout << "|";
                }
                cout << "   ";
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "=== Tower of Hanoi (3 disks) ===" << endl;
    moveCount = 0;
    towerOfHanoi(3, 'A', 'C', 'B');
    cout << "\nTotal moves: " << moveCount << endl;
    cout << endl;

    cout << "=== Tower of Hanoi with Visualization (4 disks) ===" << endl;
    TowerOfHanoiVisual tower(4);
    tower.solve();

    return 0;
}
```

### Solution 3: Maze Solver

```cpp
#include <iostream>
#include <vector>
using namespace std;

class MazeSolver {
private:
    vector<vector<int>> maze;
    vector<vector<int>> solution;
    int rows, cols;

public:
    MazeSolver(vector<vector<int>> m) : maze(m) {
        rows = maze.size();
        cols = maze[0].size();
        solution = vector<vector<int>>(rows, vector<int>(cols, 0));
    }

    bool solve() {
        if (solveMazeUtil(0, 0)) {
            cout << "Solution found:" << endl;
            printSolution();
            return true;
        }

        cout << "No solution exists" << endl;
        return false;
    }

private:
    bool isSafe(int x, int y) {
        return (x >= 0 && x < rows && y >= 0 && y < cols &&
                maze[x][y] == 1 && solution[x][y] == 0);
    }

    bool solveMazeUtil(int x, int y) {
        // Reached destination
        if (x == rows - 1 && y == cols - 1) {
            solution[x][y] = 1;
            return true;
        }

        if (isSafe(x, y)) {
            solution[x][y] = 1;  // Mark as part of solution

            // Move right
            if (solveMazeUtil(x, y + 1)) return true;

            // Move down
            if (solveMazeUtil(x + 1, y)) return true;

            // Move left
            if (solveMazeUtil(x, y - 1)) return true;

            // Move up
            if (solveMazeUtil(x - 1, y)) return true;

            // Backtrack
            solution[x][y] = 0;
            return false;
        }

        return false;
    }

    void printSolution() {
        for (const auto& row : solution) {
            for (int cell : row) {
                cout << (cell ? "* " : ". ");
            }
            cout << endl;
        }
    }
};

int main() {
    // 1 = path, 0 = wall
    vector<vector<int>> maze1 = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };

    cout << "Maze 1:" << endl;
    MazeSolver solver1(maze1);
    solver1.solve();
    cout << endl;

    vector<vector<int>> maze2 = {
        {1, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1}
    };

    cout << "Maze 2:" << endl;
    MazeSolver solver2(maze2);
    solver2.solve();

    return 0;
}
```

### Solution 4: Subset Sum Problem

```cpp
#include <iostream>
#include <vector>
using namespace std;

class SubsetSum {
public:
    void findSubsets(const vector<int>& arr, int target) {
        vector<int> subset;
        cout << "Subsets that sum to " << target << ":" << endl;
        findSubsetsUtil(arr, target, 0, subset);
    }

private:
    void findSubsetsUtil(const vector<int>& arr, int target,
                        int index, vector<int>& subset) {
        // Found a valid subset
        if (target == 0) {
            printSubset(subset);
            return;
        }

        // No more elements or target negative
        if (index >= arr.size() || target < 0) {
            return;
        }

        // Include current element
        subset.push_back(arr[index]);
        findSubsetsUtil(arr, target - arr[index], index + 1, subset);
        subset.pop_back();  // Backtrack

        // Exclude current element
        findSubsetsUtil(arr, target, index + 1, subset);
    }

    void printSubset(const vector<int>& subset) {
        cout << "{ ";
        for (int num : subset) {
            cout << num << " ";
        }
        cout << "}" << endl;
    }
};

// Count all possible subsets
class SubsetCounter {
public:
    int countSubsets(const vector<int>& arr, int target, int index = 0) {
        if (target == 0) return 1;  // Found one subset
        if (index >= arr.size() || target < 0) return 0;

        // Include + Exclude
        return countSubsets(arr, target - arr[index], index + 1) +
               countSubsets(arr, target, index + 1);
    }
};

int main() {
    vector<int> arr = {3, 34, 4, 12, 5, 2};
    int target = 9;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl << endl;

    SubsetSum solver;
    solver.findSubsets(arr, target);

    cout << endl;

    SubsetCounter counter;
    int count = counter.countSubsets(arr, target);
    cout << "Total number of subsets: " << count << endl;

    return 0;
}
```

### Solution 5: Expression Evaluator

```cpp
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class ExpressionEvaluator {
private:
    string expr;
    size_t pos;

public:
    ExpressionEvaluator(const string& expression) : expr(expression), pos(0) {}

    double evaluate() {
        pos = 0;
        return parseExpression();
    }

private:
    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) {
            pos++;
        }
    }

    double parseExpression() {
        double result = parseTerm();

        while (pos < expr.length()) {
            skipWhitespace();
            if (pos >= expr.length()) break;

            char op = expr[pos];
            if (op == '+' || op == '-') {
                pos++;
                double term = parseTerm();
                if (op == '+') {
                    result += term;
                } else {
                    result -= term;
                }
            } else {
                break;
            }
        }

        return result;
    }

    double parseTerm() {
        double result = parseFactor();

        while (pos < expr.length()) {
            skipWhitespace();
            if (pos >= expr.length()) break;

            char op = expr[pos];
            if (op == '*' || op == '/') {
                pos++;
                double factor = parseFactor();
                if (op == '*') {
                    result *= factor;
                } else {
                    if (factor != 0) {
                        result /= factor;
                    } else {
                        cerr << "Error: Division by zero" << endl;
                    }
                }
            } else {
                break;
            }
        }

        return result;
    }

    double parseFactor() {
        skipWhitespace();

        // Handle parentheses
        if (expr[pos] == '(') {
            pos++;
            double result = parseExpression();
            if (expr[pos] == ')') {
                pos++;
            }
            return result;
        }

        // Handle unary minus
        if (expr[pos] == '-') {
            pos++;
            return -parseFactor();
        }

        // Parse number
        return parseNumber();
    }

    double parseNumber() {
        skipWhitespace();

        string numStr;
        while (pos < expr.length() &&
               (isdigit(expr[pos]) || expr[pos] == '.')) {
            numStr += expr[pos++];
        }

        return stod(numStr);
    }
};

int main() {
    vector<string> expressions = {
        "3 + 5 * 2",
        "(3 + 5) * 2",
        "10 - 2 * 3",
        "100 / (2 + 3) * 4",
        "-5 + 10",
        "3.5 * 2 + 1.5",
        "((2 + 3) * (4 - 1)) / 5"
    };

    cout << "=== Expression Evaluator ===" << endl;
    for (const auto& expr : expressions) {
        ExpressionEvaluator evaluator(expr);
        double result = evaluator.evaluate();
        cout << expr << " = " << result << endl;
    }

    return 0;
}
```

## Summary

In this lesson, you learned:
- Core recursive concepts and patterns
- Tail recursion optimization techniques
- Memoization for improving recursive performance
- Advanced recursive algorithms (divide-and-conquer, backtracking)
- Tree and graph traversal using recursion
- How to choose between recursion and iteration
- Debugging and optimizing recursive functions

## Checklist

- [ ] I understand how recursion works fundamentally
- [ ] I can identify base cases and recursive cases
- [ ] I know when to use tail recursion
- [ ] I can implement memoization to optimize recursive functions
- [ ] I understand divide-and-conquer and backtracking patterns
- [ ] I can implement recursive tree and graph algorithms
- [ ] I completed all exercises and understand their solutions
- [ ] I can debug stack overflow issues in recursive functions

## Next Steps
In the next lesson, we'll explore Function Templates, learning how to write generic, reusable functions that work with multiple data types.
