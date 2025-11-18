# Lesson 11: Recursion

**Duration:** 2.5 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand the concept of recursion and how it works
- Write recursive functions with proper base cases
- Trace recursive function execution
- Compare recursion with iteration
- Recognize problems suitable for recursive solutions
- Optimize recursive functions and avoid common pitfalls

## 1. Introduction to Recursion

Recursion is a programming technique where a function calls itself to solve a problem by breaking it down into smaller, similar subproblems.

### 1.1 Basic Recursive Function

```cpp
#include <iostream>
using namespace std;

// Simple countdown using recursion
void countdown(int n) {
    // Base case: stop recursion
    if (n <= 0) {
        cout << "Liftoff!" << endl;
        return;
    }

    // Recursive case
    cout << n << "..." << endl;
    countdown(n - 1);  // Recursive call with smaller value
}

int main() {
    countdown(5);
    return 0;
}
```

**Output:**
```
5...
4...
3...
2...
1...
Liftoff!
```

### 1.2 Anatomy of a Recursive Function

Every recursive function needs:
1. **Base case**: Condition to stop recursion
2. **Recursive case**: Function calls itself with modified parameters
3. **Progress towards base case**: Each call should move closer to the base case

```cpp
#include <iostream>
using namespace std;

// Calculate factorial: n! = n * (n-1) * (n-2) * ... * 1
int factorial(int n) {
    // Base case
    if (n <= 1) {
        return 1;
    }

    // Recursive case
    return n * factorial(n - 1);
}

int main() {
    cout << "5! = " << factorial(5) << endl;
    cout << "0! = " << factorial(0) << endl;
    cout << "7! = " << factorial(7) << endl;

    return 0;
}
```

**Trace of factorial(5):**
```
factorial(5) = 5 * factorial(4)
             = 5 * 4 * factorial(3)
             = 5 * 4 * 3 * factorial(2)
             = 5 * 4 * 3 * 2 * factorial(1)
             = 5 * 4 * 3 * 2 * 1
             = 120
```

## 2. Common Recursive Patterns

### 2.1 Fibonacci Sequence

```cpp
#include <iostream>
using namespace std;

// Calculate nth Fibonacci number
// F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
int fibonacci(int n) {
    // Base cases
    if (n == 0) return 0;
    if (n == 1) return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    cout << "Fibonacci sequence:" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << "F(" << i << ") = " << fibonacci(i) << endl;
    }

    return 0;
}
```

### 2.2 Sum of Array Elements

```cpp
#include <iostream>
using namespace std;

// Recursive sum of array elements
int sumArray(int arr[], int size) {
    // Base case: empty array
    if (size == 0) {
        return 0;
    }

    // Recursive case: first element + sum of rest
    return arr[0] + sumArray(arr + 1, size - 1);
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};

    cout << "Sum = " << sumArray(numbers, 5) << endl;

    return 0;
}
```

### 2.3 Power Function

```cpp
#include <iostream>
using namespace std;

// Calculate base^exponent using recursion
int power(int base, int exponent) {
    // Base case
    if (exponent == 0) {
        return 1;
    }

    // Recursive case
    return base * power(base, exponent - 1);
}

// Optimized version using divide and conquer
int powerOptimized(int base, int exponent) {
    if (exponent == 0) return 1;

    int half = powerOptimized(base, exponent / 2);

    if (exponent % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}

int main() {
    cout << "2^5 = " << power(2, 5) << endl;
    cout << "3^4 = " << power(3, 4) << endl;

    cout << "2^10 (optimized) = " << powerOptimized(2, 10) << endl;

    return 0;
}
```

## 3. Recursion with Strings

### 3.1 String Reversal

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// Reverse a string using recursion
void reverseString(char* str, int start, int end) {
    // Base case
    if (start >= end) {
        return;
    }

    // Swap characters
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;

    // Recursive call
    reverseString(str, start + 1, end - 1);
}

int main() {
    char text[] = "Hello World";

    cout << "Original: " << text << endl;
    reverseString(text, 0, strlen(text) - 1);
    cout << "Reversed: " << text << endl;

    return 0;
}
```

### 3.2 Palindrome Checker

```cpp
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

// Check if string is a palindrome
bool isPalindrome(const char* str, int start, int end) {
    // Base case: single character or empty
    if (start >= end) {
        return true;
    }

    // Check current characters
    if (tolower(str[start]) != tolower(str[end])) {
        return false;
    }

    // Recursive call
    return isPalindrome(str, start + 1, end - 1);
}

int main() {
    const char* test1 = "racecar";
    const char* test2 = "hello";
    const char* test3 = "A man a plan a canal Panama";

    cout << test1 << " is " << (isPalindrome(test1, 0, strlen(test1) - 1) ? "" : "not ")
         << "a palindrome" << endl;

    cout << test2 << " is " << (isPalindrome(test2, 0, strlen(test2) - 1) ? "" : "not ")
         << "a palindrome" << endl;

    return 0;
}
```

## 4. Mathematical Recursion

### 4.1 Greatest Common Divisor (GCD)

```cpp
#include <iostream>
using namespace std;

// Euclidean algorithm for GCD
int gcd(int a, int b) {
    // Base case
    if (b == 0) {
        return a;
    }

    // Recursive case
    return gcd(b, a % b);
}

int main() {
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "GCD(100, 35) = " << gcd(100, 35) << endl;
    cout << "GCD(17, 19) = " << gcd(17, 19) << endl;

    return 0;
}
```

### 4.2 Binary Search (Recursive)

```cpp
#include <iostream>
using namespace std;

// Recursive binary search
int binarySearch(int arr[], int left, int right, int target) {
    // Base case: element not found
    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;

    // Base case: element found
    if (arr[mid] == target) {
        return mid;
    }

    // Recursive cases
    if (arr[mid] > target) {
        return binarySearch(arr, left, mid - 1, target);
    } else {
        return binarySearch(arr, mid + 1, right, target);
    }
}

int main() {
    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 67, 78};
    int size = 11;

    int target = 23;
    int index = binarySearch(sorted, 0, size - 1, target);

    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << target << " not found" << endl;
    }

    return 0;
}
```

## 5. Recursive Data Structure Traversal

### 5.1 Print Numbers in Range

```cpp
#include <iostream>
using namespace std;

// Print numbers from start to end
void printRange(int start, int end) {
    if (start > end) {
        return;
    }

    cout << start << " ";
    printRange(start + 1, end);
}

// Print numbers in reverse
void printRangeReverse(int start, int end) {
    if (start > end) {
        return;
    }

    cout << end << " ";
    printRangeReverse(start, end - 1);
}

int main() {
    cout << "Numbers 1 to 10: ";
    printRange(1, 10);
    cout << endl;

    cout << "Numbers 10 to 1: ";
    printRangeReverse(1, 10);
    cout << endl;

    return 0;
}
```

### 5.2 Digit Sum

```cpp
#include <iostream>
using namespace std;

// Calculate sum of digits recursively
int digitSum(int n) {
    // Base case
    if (n == 0) {
        return 0;
    }

    // Recursive case: last digit + sum of remaining digits
    return (n % 10) + digitSum(n / 10);
}

int main() {
    cout << "Sum of digits of 12345: " << digitSum(12345) << endl;
    cout << "Sum of digits of 999: " << digitSum(999) << endl;

    return 0;
}
```

## 6. Recursion vs Iteration

### 6.1 Factorial - Both Approaches

```cpp
#include <iostream>
using namespace std;

// Recursive factorial
int factorialRecursive(int n) {
    if (n <= 1) return 1;
    return n * factorialRecursive(n - 1);
}

// Iterative factorial
int factorialIterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n = 6;

    cout << "Recursive: " << n << "! = " << factorialRecursive(n) << endl;
    cout << "Iterative: " << n << "! = " << factorialIterative(n) << endl;

    return 0;
}
```

**Comparison:**
- **Recursion**: More elegant, easier to understand for some problems
- **Iteration**: Often more efficient, no risk of stack overflow
- **When to use recursion**: Tree structures, divide-and-conquer, naturally recursive problems

## 7. Advanced Example: Tower of Hanoi

```cpp
#include <iostream>
using namespace std;

// Solve Tower of Hanoi puzzle
void towerOfHanoi(int n, char from, char to, char aux) {
    // Base case: only one disk
    if (n == 1) {
        cout << "Move disk 1 from " << from << " to " << to << endl;
        return;
    }

    // Move n-1 disks from 'from' to 'aux' using 'to'
    towerOfHanoi(n - 1, from, aux, to);

    // Move the largest disk from 'from' to 'to'
    cout << "Move disk " << n << " from " << from << " to " << to << endl;

    // Move n-1 disks from 'aux' to 'to' using 'from'
    towerOfHanoi(n - 1, aux, to, from);
}

int main() {
    int disks = 3;

    cout << "Solving Tower of Hanoi for " << disks << " disks:" << endl;
    towerOfHanoi(disks, 'A', 'C', 'B');

    return 0;
}
```

## Exercises

### Exercise 1: Count Down and Up
Write a recursive function that counts down from n to 1, then counts back up to n.

### Exercise 2: Array Maximum
Write a recursive function to find the maximum element in an array.

### Exercise 3: String Length
Implement a recursive function to calculate the length of a C-string without using strlen.

### Exercise 4: Decimal to Binary
Create a recursive function that converts a decimal number to binary (prints binary representation).

### Exercise 5: Pascal's Triangle
Write a recursive function to calculate the value at position (row, col) in Pascal's triangle.

## Exercise Solutions

### Solution 1: Count Down and Up

```cpp
#include <iostream>
using namespace std;

void countDownUp(int n) {
    if (n == 0) {
        return;
    }

    cout << n << " ";           // Print on the way down
    countDownUp(n - 1);         // Recursive call
    cout << n << " ";           // Print on the way up
}

int main() {
    cout << "Count down and up from 5:" << endl;
    countDownUp(5);
    cout << endl;

    return 0;
}
```

**Output:**
```
5 4 3 2 1 1 2 3 4 5
```

### Solution 2: Array Maximum

```cpp
#include <iostream>
using namespace std;

int findMax(int arr[], int size) {
    // Base case: only one element
    if (size == 1) {
        return arr[0];
    }

    // Recursive case: max of first element and max of rest
    int maxOfRest = findMax(arr + 1, size - 1);
    return (arr[0] > maxOfRest) ? arr[0] : maxOfRest;
}

// Alternative implementation using index
int findMaxByIndex(int arr[], int index, int size) {
    // Base case: last element
    if (index == size - 1) {
        return arr[index];
    }

    // Recursive case
    int maxOfRest = findMaxByIndex(arr, index + 1, size);
    return (arr[index] > maxOfRest) ? arr[index] : maxOfRest;
}

int main() {
    int numbers[] = {3, 7, 2, 9, 4, 1, 8};
    int size = 7;

    cout << "Maximum element: " << findMax(numbers, size) << endl;
    cout << "Maximum element (by index): " << findMaxByIndex(numbers, 0, size) << endl;

    return 0;
}
```

### Solution 3: String Length

```cpp
#include <iostream>
using namespace std;

int stringLength(const char* str) {
    // Base case: null terminator
    if (*str == '\0') {
        return 0;
    }

    // Recursive case: 1 + length of rest
    return 1 + stringLength(str + 1);
}

// Alternative using index
int stringLengthByIndex(const char* str, int index) {
    if (str[index] == '\0') {
        return 0;
    }

    return 1 + stringLengthByIndex(str, index + 1);
}

int main() {
    const char* text = "Hello, World!";

    cout << "Length of \"" << text << "\": " << stringLength(text) << endl;
    cout << "Length (by index): " << stringLengthByIndex(text, 0) << endl;

    return 0;
}
```

### Solution 4: Decimal to Binary

```cpp
#include <iostream>
using namespace std;

void decimalToBinary(int n) {
    // Base case
    if (n == 0) {
        return;
    }

    // Recursive call first (to print in correct order)
    decimalToBinary(n / 2);

    // Print current bit
    cout << (n % 2);
}

// Version that returns string would require std::string
void printBinary(int n) {
    if (n == 0) {
        cout << "0";
        return;
    }

    decimalToBinary(n);
}

int main() {
    int numbers[] = {0, 5, 10, 42, 255};

    for (int i = 0; i < 5; i++) {
        cout << numbers[i] << " in binary: ";
        printBinary(numbers[i]);
        cout << endl;
    }

    return 0;
}
```

### Solution 5: Pascal's Triangle

```cpp
#include <iostream>
using namespace std;

// Calculate value at position (row, col) in Pascal's triangle
int pascal(int row, int col) {
    // Base cases: edges of triangle are always 1
    if (col == 0 || col == row) {
        return 1;
    }

    // Recursive case: sum of two values above
    return pascal(row - 1, col - 1) + pascal(row - 1, col);
}

// Print Pascal's triangle
void printPascalTriangle(int rows) {
    for (int row = 0; row < rows; row++) {
        // Print leading spaces
        for (int i = 0; i < rows - row - 1; i++) {
            cout << " ";
        }

        // Print values in current row
        for (int col = 0; col <= row; col++) {
            cout << pascal(row, col) << " ";
        }

        cout << endl;
    }
}

int main() {
    cout << "Pascal's Triangle (7 rows):" << endl;
    printPascalTriangle(7);

    cout << "\nValue at position (5, 2): " << pascal(5, 2) << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Recursion** is when a function calls itself to solve a problem
- Every recursive function needs a **base case** and **recursive case**
- Recursion can make code more elegant and easier to understand
- **Tail recursion** can be optimized by compilers
- Recursion uses the **call stack** and can cause stack overflow if not careful
- Many problems have both **recursive and iterative** solutions
- Recursion is natural for **tree structures** and **divide-and-conquer** algorithms

## Checklist

- [ ] I understand what recursion is and how it works
- [ ] I can identify base cases and recursive cases in recursive functions
- [ ] I can trace the execution of recursive functions
- [ ] I can write recursive functions for common problems
- [ ] I understand the relationship between recursion and the call stack
- [ ] I completed all exercises and understand the solutions
- [ ] I can compare recursive and iterative solutions
- [ ] I know when recursion is appropriate and when to avoid it

## Additional Resources

- **Next Lesson:** Scope and Lifetime
- **Related Topics:** Call Stack, Stack Overflow, Memoization, Dynamic Programming
- **Practice:** Try solving problems on platforms like LeetCode that have recursive solutions

---

**Congratulations!** You've completed Lesson 11. You now understand recursion and can apply it to solve problems in an elegant way.
