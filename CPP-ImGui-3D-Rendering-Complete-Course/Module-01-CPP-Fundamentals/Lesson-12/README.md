# Lesson 12: Scope and Lifetime

**Duration:** 2 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand different types of scope in C++
- Differentiate between local, global, and block scope
- Understand variable lifetime and storage duration
- Use static variables effectively
- Avoid common scope-related errors
- Apply scope best practices in your programs

## 1. Introduction to Scope

Scope defines where in your program a variable can be accessed. C++ has several types of scope that determine visibility and accessibility of variables.

### 1.1 Local Scope

Variables declared inside a function have local scope - they're only accessible within that function.

```cpp
#include <iostream>
using namespace std;

void myFunction() {
    int localVar = 10;  // Local to myFunction
    cout << "Inside function: " << localVar << endl;
}

int main() {
    myFunction();

    // cout << localVar << endl;  // ERROR: localVar not accessible here

    int localVar = 20;  // Different variable, local to main
    cout << "In main: " << localVar << endl;

    return 0;
}
```

**Output:**
```
Inside function: 10
In main: 20
```

### 1.2 Global Scope

Variables declared outside all functions have global scope - they're accessible from anywhere in the file after their declaration.

```cpp
#include <iostream>
using namespace std;

int globalVar = 100;  // Global variable

void displayGlobal() {
    cout << "Global variable: " << globalVar << endl;
}

void modifyGlobal() {
    globalVar = 200;
}

int main() {
    cout << "Initial: " << globalVar << endl;

    displayGlobal();
    modifyGlobal();

    cout << "After modification: " << globalVar << endl;

    return 0;
}
```

**Output:**
```
Initial: 100
Global variable: 100
After modification: 200
```

### 1.3 Block Scope

Variables declared within a block (between `{}`) have block scope.

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 10;
    cout << "Outer x: " << x << endl;

    {
        int x = 20;  // Different variable, block scope
        cout << "Inner x: " << x << endl;

        int y = 30;  // Only exists in this block
        cout << "y: " << y << endl;
    }

    cout << "Outer x again: " << x << endl;
    // cout << y << endl;  // ERROR: y not accessible here

    return 0;
}
```

**Output:**
```
Outer x: 10
Inner x: 20
y: 30
Outer x again: 10
```

## 2. Scope Resolution

### 2.1 Scope Resolution Operator (::)

The scope resolution operator allows access to global variables when local variables have the same name.

```cpp
#include <iostream>
using namespace std;

int value = 100;  // Global variable

void demonstrateScope() {
    int value = 50;  // Local variable shadows global

    cout << "Local value: " << value << endl;
    cout << "Global value: " << ::value << endl;  // Use :: to access global
}

int main() {
    demonstrateScope();

    int value = 25;
    cout << "Main's value: " << value << endl;
    cout << "Global value: " << ::value << endl;

    return 0;
}
```

**Output:**
```
Local value: 50
Global value: 100
Main's value: 25
Global value: 100
```

### 2.2 Shadowing Variables

```cpp
#include <iostream>
using namespace std;

int count = 0;  // Global

void incrementCount() {
    int count = 10;  // Local, shadows global
    count++;         // Increments local, not global
    cout << "Local count: " << count << endl;
    cout << "Global count: " << ::count << endl;
}

int main() {
    incrementCount();
    cout << "Global count in main: " << count << endl;

    return 0;
}
```

## 3. Variable Lifetime and Storage Duration

### 3.1 Automatic Storage Duration

Local variables have automatic storage duration - they're created when the block is entered and destroyed when it exits.

```cpp
#include <iostream>
using namespace std;

void demonstrateAutomatic() {
    int autoVar = 0;
    autoVar++;
    cout << "Automatic variable: " << autoVar << endl;
}

int main() {
    demonstrateAutomatic();  // autoVar = 1
    demonstrateAutomatic();  // autoVar = 1 again (new variable each time)
    demonstrateAutomatic();  // autoVar = 1 again

    return 0;
}
```

**Output:**
```
Automatic variable: 1
Automatic variable: 1
Automatic variable: 1
```

### 3.2 Static Storage Duration

Static local variables retain their value between function calls.

```cpp
#include <iostream>
using namespace std;

void demonstrateStatic() {
    static int staticVar = 0;  // Initialized only once
    staticVar++;
    cout << "Static variable: " << staticVar << endl;
}

int main() {
    demonstrateStatic();  // staticVar = 1
    demonstrateStatic();  // staticVar = 2
    demonstrateStatic();  // staticVar = 3

    return 0;
}
```

**Output:**
```
Static variable: 1
Static variable: 2
Static variable: 3
```

### 3.3 Static vs Automatic Comparison

```cpp
#include <iostream>
using namespace std;

void counter() {
    static int staticCount = 0;
    int autoCount = 0;

    staticCount++;
    autoCount++;

    cout << "Static: " << staticCount << ", Automatic: " << autoCount << endl;
}

int main() {
    for (int i = 0; i < 5; i++) {
        counter();
    }

    return 0;
}
```

**Output:**
```
Static: 1, Automatic: 1
Static: 2, Automatic: 1
Static: 3, Automatic: 1
Static: 4, Automatic: 1
Static: 5, Automatic: 1
```

## 4. Function Scope and Parameters

### 4.1 Parameter Scope

Function parameters have local scope within the function.

```cpp
#include <iostream>
using namespace std;

void modifyParameter(int param) {
    param = 100;  // Only modifies local copy
    cout << "Inside function: " << param << endl;
}

int main() {
    int value = 50;
    cout << "Before function: " << value << endl;

    modifyParameter(value);

    cout << "After function: " << value << endl;  // Unchanged

    return 0;
}
```

### 4.2 Static Function Variables for State

```cpp
#include <iostream>
using namespace std;

int getUniqueID() {
    static int nextID = 1000;
    return nextID++;
}

int main() {
    cout << "ID 1: " << getUniqueID() << endl;
    cout << "ID 2: " << getUniqueID() << endl;
    cout << "ID 3: " << getUniqueID() << endl;
    cout << "ID 4: " << getUniqueID() << endl;

    return 0;
}
```

**Output:**
```
ID 1: 1000
ID 2: 1001
ID 3: 1002
ID 4: 1003
```

## 5. Practical Examples

### Example 1: Function Call Counter

```cpp
#include <iostream>
using namespace std;

void trackCalls(const char* message) {
    static int callCount = 0;
    callCount++;

    cout << "Call #" << callCount << ": " << message << endl;
}

int main() {
    trackCalls("First call");
    trackCalls("Second call");
    trackCalls("Third call");
    trackCalls("Fourth call");

    return 0;
}
```

### Example 2: Random Number Seed

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int getRandomNumber(int min, int max) {
    static bool seeded = false;

    if (!seeded) {
        srand(time(0));  // Seed only once
        seeded = true;
    }

    return min + rand() % (max - min + 1);
}

int main() {
    cout << "Random numbers between 1 and 100:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << getRandomNumber(1, 100) << endl;
    }

    return 0;
}
```

### Example 3: Fibonacci with Static Caching

```cpp
#include <iostream>
using namespace std;

int fibonacci(int n) {
    static int cache[50] = {0};  // Static array to cache results
    static bool initialized = false;

    if (!initialized) {
        cache[0] = 0;
        cache[1] = 1;
        initialized = true;
    }

    if (n < 2) return cache[n];

    if (cache[n] != 0) return cache[n];  // Return cached value

    cache[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return cache[n];
}

int main() {
    cout << "Fibonacci sequence:" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << "F(" << i << ") = " << fibonacci(i) << endl;
    }

    return 0;
}
```

### Example 4: Scope in Loops

```cpp
#include <iostream>
using namespace std;

int main() {
    // Loop variable scope
    for (int i = 0; i < 3; i++) {
        cout << "Loop 1, i = " << i << endl;
    }

    // Can declare another i - previous one is out of scope
    for (int i = 10; i < 13; i++) {
        cout << "Loop 2, i = " << i << endl;
    }

    // Block scope within loop
    for (int i = 0; i < 3; i++) {
        int squared = i * i;  // Created and destroyed each iteration
        cout << "i = " << i << ", squared = " << squared << endl;
    }

    return 0;
}
```

### Example 5: Game Score Tracker

```cpp
#include <iostream>
using namespace std;

void updateScore(int points) {
    static int totalScore = 0;
    static int numberOfUpdates = 0;

    totalScore += points;
    numberOfUpdates++;

    cout << "Points added: " << points << endl;
    cout << "Total score: " << totalScore << endl;
    cout << "Average points: " << (totalScore / numberOfUpdates) << endl;
    cout << "---" << endl;
}

int main() {
    updateScore(10);
    updateScore(25);
    updateScore(15);
    updateScore(30);

    return 0;
}
```

## 6. Scope Best Practices

### 6.1 Minimize Global Variables

```cpp
#include <iostream>
using namespace std;

// AVOID: Global variables can be modified anywhere
int globalCounter = 0;  // Risky

void incrementGlobal() {
    globalCounter++;
}

// PREFER: Encapsulated state with functions
int getCounter() {
    static int counter = 0;
    return counter;
}

void incrementCounter() {
    static int counter = 0;
    counter++;
}

int main() {
    // Both work, but static function variables are more controlled
    incrementGlobal();
    incrementCounter();

    return 0;
}
```

### 6.2 Use Smallest Scope Necessary

```cpp
#include <iostream>
using namespace std;

int main() {
    // BAD: Variable declared with wider scope than needed
    int result;
    int temp;

    for (int i = 0; i < 5; i++) {
        temp = i * i;
        cout << temp << endl;
    }

    // BETTER: Declare in smallest scope
    for (int i = 0; i < 5; i++) {
        int temp = i * i;  // Only exists when needed
        cout << temp << endl;
    }

    return 0;
}
```

## Exercises

### Exercise 1: Call Counter
Create a function that counts how many times it has been called and displays this count each time.

### Exercise 2: Max Value Tracker
Write a function that keeps track of the maximum value it has ever been called with and returns it.

### Exercise 3: Scope Demonstration
Write a program that demonstrates the difference between global, local, and static variables by modifying them in different functions.

### Exercise 4: ID Generator
Create a function that generates sequential ID numbers starting from 1. Each call should return the next ID in sequence.

### Exercise 5: Statistics Accumulator
Write a function that accepts a number and maintains running statistics (count, sum, average, min, max) across all calls.

## Exercise Solutions

### Solution 1: Call Counter

```cpp
#include <iostream>
using namespace std;

void callCounter() {
    static int count = 0;
    count++;
    cout << "This function has been called " << count << " time(s)" << endl;
}

int main() {
    cout << "Calling function multiple times:" << endl;
    callCounter();
    callCounter();
    callCounter();
    callCounter();
    callCounter();

    return 0;
}
```

### Solution 2: Max Value Tracker

```cpp
#include <iostream>
using namespace std;

int trackMax(int value) {
    static int maxValue = value;  // Initialize with first value
    static bool firstCall = true;

    if (firstCall) {
        firstCall = false;
    } else if (value > maxValue) {
        maxValue = value;
    }

    cout << "Current value: " << value << ", Max so far: " << maxValue << endl;
    return maxValue;
}

int main() {
    trackMax(10);
    trackMax(25);
    trackMax(15);
    trackMax(40);
    trackMax(30);

    return 0;
}
```

### Solution 3: Scope Demonstration

```cpp
#include <iostream>
using namespace std;

int globalVar = 100;  // Global variable

void demonstrateScope() {
    static int staticVar = 0;  // Static local variable
    int localVar = 0;          // Automatic local variable

    globalVar += 10;
    staticVar += 10;
    localVar += 10;

    cout << "Global: " << globalVar << ", Static: " << staticVar
         << ", Local: " << localVar << endl;
}

int main() {
    cout << "Initial global: " << globalVar << endl;
    cout << "\nCalling function 3 times:" << endl;

    demonstrateScope();
    demonstrateScope();
    demonstrateScope();

    cout << "\nFinal global: " << globalVar << endl;

    return 0;
}
```

### Solution 4: ID Generator

```cpp
#include <iostream>
using namespace std;

int generateID() {
    static int nextID = 1;
    return nextID++;
}

void resetIDGenerator() {
    // Note: Cannot directly reset static variable from outside
    // This would require a different approach
}

int main() {
    cout << "Generating IDs:" << endl;

    for (int i = 0; i < 5; i++) {
        int id = generateID();
        cout << "Generated ID: " << id << endl;
    }

    cout << "\nGenerating more IDs:" << endl;
    for (int i = 0; i < 3; i++) {
        int id = generateID();
        cout << "Generated ID: " << id << endl;
    }

    return 0;
}
```

### Solution 5: Statistics Accumulator

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

void accumulateStats(double value) {
    static int count = 0;
    static double sum = 0.0;
    static double minVal = 0.0;
    static double maxVal = 0.0;
    static bool firstCall = true;

    if (firstCall) {
        minVal = value;
        maxVal = value;
        firstCall = false;
    }

    count++;
    sum += value;

    if (value < minVal) minVal = value;
    if (value > maxVal) maxVal = value;

    double average = sum / count;

    cout << fixed << setprecision(2);
    cout << "Value: " << value << endl;
    cout << "  Count: " << count << endl;
    cout << "  Sum: " << sum << endl;
    cout << "  Average: " << average << endl;
    cout << "  Min: " << minVal << endl;
    cout << "  Max: " << maxVal << endl;
    cout << "---" << endl;
}

int main() {
    cout << "Accumulating statistics:" << endl;

    accumulateStats(10.5);
    accumulateStats(25.3);
    accumulateStats(15.7);
    accumulateStats(8.2);
    accumulateStats(30.1);

    return 0;
}
```

## Summary

In this lesson, you learned:

- **Scope** determines where variables can be accessed
- **Local scope**: Variables in functions
- **Global scope**: Variables outside functions
- **Block scope**: Variables in code blocks
- **Automatic storage**: Variables created/destroyed with scope
- **Static storage**: Variables that persist between function calls
- **Scope resolution operator** (::) accesses global variables
- **Best practices**: Minimize globals, use smallest scope necessary

## Checklist

- [ ] I understand the different types of scope in C++
- [ ] I can differentiate between local, global, and block scope
- [ ] I understand variable lifetime and storage duration
- [ ] I know when to use static variables
- [ ] I can use the scope resolution operator
- [ ] I completed all exercises and understand the solutions
- [ ] I understand the dangers of global variables
- [ ] I can choose appropriate scope for my variables

## Additional Resources

- **Next Lesson:** Header Files and Multiple Files
- **Related Topics:** Namespaces, Storage Classes, Memory Management
- **Practice:** Refactor code to use appropriate scopes

---

**Congratulations!** You've completed Lesson 12. You now understand scope and lifetime in C++, which are crucial for writing well-organized and bug-free code.
