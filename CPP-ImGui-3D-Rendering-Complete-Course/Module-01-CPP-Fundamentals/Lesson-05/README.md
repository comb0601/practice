# Lesson 05: Control Flow - if, else, switch
**Duration: 2.5 hours**

## Learning Objectives
- Master if, else if, and else statements
- Use comparison operators in conditions
- Understand logical operators for complex conditions
- Master the switch statement
- Understand the ternary operator
- Write clean, readable conditional code
- Avoid common pitfalls

## 1. The if Statement

### Basic Syntax

```cpp
#include <iostream>

int main() {
    int age = 20;

    if (age >= 18) {
        std::cout << "You are an adult" << std::endl;
    }

    return 0;
}
```

### How it Works

```cpp
if (condition) {
    // Code executes if condition is true
}
// Code continues here regardless
```

### Conditions

```cpp
#include <iostream>

int main() {
    int x = 10;

    // Comparison operators
    if (x > 5) {
        std::cout << "x is greater than 5" << std::endl;
    }

    if (x == 10) {
        std::cout << "x equals 10" << std::endl;
    }

    if (x != 0) {
        std::cout << "x is not zero" << std::endl;
    }

    // Boolean values
    bool isStudent = true;
    if (isStudent) {
        std::cout << "Discount applied" << std::endl;
    }

    return 0;
}
```

## 2. The else Statement

### if-else Pattern

```cpp
#include <iostream>

int main() {
    int temperature = 25;

    if (temperature > 30) {
        std::cout << "It's hot outside!" << std::endl;
    } else {
        std::cout << "Temperature is comfortable" << std::endl;
    }

    return 0;
}
```

### Complete Example

```cpp
#include <iostream>

int main() {
    int score;
    std::cout << "Enter your score (0-100): ";
    std::cin >> score;

    if (score >= 50) {
        std::cout << "You passed!" << std::endl;
    } else {
        std::cout << "You failed. Try again." << std::endl;
    }

    return 0;
}
```

## 3. The else if Statement

### Multiple Conditions

```cpp
#include <iostream>

int main() {
    int score;
    std::cout << "Enter your score: ";
    std::cin >> score;

    if (score >= 90) {
        std::cout << "Grade: A - Excellent!" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B - Good job!" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C - Average" << std::endl;
    } else if (score >= 60) {
        std::cout << "Grade: D - Below average" << std::endl;
    } else {
        std::cout << "Grade: F - Failed" << std::endl;
    }

    return 0;
}
```

### Order Matters

```cpp
#include <iostream>

int main() {
    int age = 25;

    // CORRECT - checks from most specific to least
    if (age >= 65) {
        std::cout << "Senior citizen" << std::endl;
    } else if (age >= 18) {
        std::cout << "Adult" << std::endl;
    } else if (age >= 13) {
        std::cout << "Teenager" << std::endl;
    } else {
        std::cout << "Child" << std::endl;
    }

    // WRONG - first condition catches everything!
    // if (age >= 0) {  // This would always be true!
    //     std::cout << "Person" << std::endl;
    // } else if (age >= 18) {  // Never reached!
    //     std::cout << "Adult" << std::endl;
    // }

    return 0;
}
```

## 4. Nested if Statements

### if Inside if

```cpp
#include <iostream>

int main() {
    int age, citizenship;

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Are you a citizen? (1=yes, 0=no): ";
    std::cin >> citizenship;

    if (age >= 18) {
        if (citizenship == 1) {
            std::cout << "You can vote!" << std::endl;
        } else {
            std::cout << "Must be a citizen to vote" << std::endl;
        }
    } else {
        std::cout << "Must be 18 or older to vote" << std::endl;
    }

    return 0;
}
```

### Complex Nesting

```cpp
#include <iostream>

int main() {
    int age;
    bool hasLicense;
    bool hasCar;

    std::cout << "Age: ";
    std::cin >> age;
    std::cout << "Have license? (1/0): ";
    std::cin >> hasLicense;
    std::cout << "Have car? (1/0): ";
    std::cin >> hasCar;

    if (age >= 16) {
        if (hasLicense) {
            if (hasCar) {
                std::cout << "You can drive your car!" << std::endl;
            } else {
                std::cout << "You need to get a car" << std::endl;
            }
        } else {
            std::cout << "You need to get a license first" << std::endl;
        }
    } else {
        std::cout << "You're too young to drive" << std::endl;
    }

    return 0;
}
```

## 5. Logical Operators

### AND (&&) Operator

```cpp
#include <iostream>

int main() {
    int age = 20;
    bool hasLicense = true;

    // Both conditions must be true
    if (age >= 16 && hasLicense) {
        std::cout << "You can drive!" << std::endl;
    } else {
        std::cout << "You cannot drive" << std::endl;
    }

    return 0;
}
```

### OR (||) Operator

```cpp
#include <iostream>

int main() {
    char day;
    std::cout << "Enter day (M/T/W/R/F/S/U): ";
    std::cin >> day;

    // At least one condition must be true
    if (day == 'S' || day == 'U') {
        std::cout << "It's the weekend!" << std::endl;
    } else {
        std::cout << "It's a weekday" << std::endl;
    }

    return 0;
}
```

### NOT (!) Operator

```cpp
#include <iostream>

int main() {
    bool isRaining = false;

    // Inverts the condition
    if (!isRaining) {
        std::cout << "Good weather for a walk!" << std::endl;
    } else {
        std::cout << "Bring an umbrella" << std::endl;
    }

    return 0;
}
```

### Combining Logical Operators

```cpp
#include <iostream>

int main() {
    int age;
    bool isStudent;
    bool hasID;

    std::cout << "Age: ";
    std::cin >> age;
    std::cout << "Student? (1/0): ";
    std::cin >> isStudent;
    std::cout << "Has ID? (1/0): ";
    std::cin >> hasID;

    // Complex condition with AND and OR
    if ((age < 18 || isStudent) && hasID) {
        std::cout << "Discount: 20%" << std::endl;
    } else if (age >= 65 && hasID) {
        std::cout << "Senior discount: 30%" << std::endl;
    } else {
        std::cout << "Regular price" << std::endl;
    }

    return 0;
}
```

## 6. The switch Statement

### Basic Switch

```cpp
#include <iostream>

int main() {
    int choice;
    std::cout << "Menu:\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Settings\n";
    std::cout << "4. Exit\n";
    std::cout << "Choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "Starting new game..." << std::endl;
            break;
        case 2:
            std::cout << "Loading saved game..." << std::endl;
            break;
        case 3:
            std::cout << "Opening settings..." << std::endl;
            break;
        case 4:
            std::cout << "Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }

    return 0;
}
```

### Why break is Important

```cpp
#include <iostream>

int main() {
    int day = 2;

    std::cout << "Without break:" << std::endl;
    switch (day) {
        case 1:
            std::cout << "Monday" << std::endl;
        case 2:
            std::cout << "Tuesday" << std::endl;  // Starts here
        case 3:
            std::cout << "Wednesday" << std::endl; // Also executes!
        default:
            std::cout << "Other day" << std::endl; // Also executes!
    }
    // Output: Tuesday, Wednesday, Other day

    std::cout << "\nWith break:" << std::endl;
    switch (day) {
        case 1:
            std::cout << "Monday" << std::endl;
            break;
        case 2:
            std::cout << "Tuesday" << std::endl;  // Starts and stops here
            break;
        case 3:
            std::cout << "Wednesday" << std::endl;
            break;
        default:
            std::cout << "Other day" << std::endl;
            break;
    }
    // Output: Tuesday

    return 0;
}
```

### Fall-through on Purpose

```cpp
#include <iostream>

int main() {
    char grade;
    std::cout << "Enter your grade (A-F): ";
    std::cin >> grade;

    switch (grade) {
        case 'A':
        case 'a':
            std::cout << "Excellent!" << std::endl;
            break;
        case 'B':
        case 'b':
            std::cout << "Good job!" << std::endl;
            break;
        case 'C':
        case 'c':
            std::cout << "Average" << std::endl;
            break;
        case 'D':
        case 'd':
            std::cout << "Below average" << std::endl;
            break;
        case 'F':
        case 'f':
            std::cout << "Failed" << std::endl;
            break;
        default:
            std::cout << "Invalid grade!" << std::endl;
            break;
    }

    return 0;
}
```

### Switch with char

```cpp
#include <iostream>

int main() {
    char operation;
    double num1, num2;

    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> operation;
    std::cout << "Enter two numbers: ";
    std::cin >> num1 >> num2;

    switch (operation) {
        case '+':
            std::cout << "Result: " << (num1 + num2) << std::endl;
            break;
        case '-':
            std::cout << "Result: " << (num1 - num2) << std::endl;
            break;
        case '*':
            std::cout << "Result: " << (num1 * num2) << std::endl;
            break;
        case '/':
            if (num2 != 0) {
                std::cout << "Result: " << (num1 / num2) << std::endl;
            } else {
                std::cout << "Error: Division by zero!" << std::endl;
            }
            break;
        default:
            std::cout << "Invalid operation!" << std::endl;
            break;
    }

    return 0;
}
```

## 7. Ternary Operator (Conditional Operator)

### Basic Usage

```cpp
#include <iostream>

int main() {
    int age = 20;

    // Ternary operator: condition ? value_if_true : value_if_false
    std::string status = (age >= 18) ? "Adult" : "Minor";
    std::cout << "Status: " << status << std::endl;

    // Equivalent if-else
    std::string status2;
    if (age >= 18) {
        status2 = "Adult";
    } else {
        status2 = "Minor";
    }

    return 0;
}
```

### Practical Examples

```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;

    // Find maximum
    int max = (a > b) ? a : b;
    std::cout << "Max: " << max << std::endl;

    // Find minimum
    int min = (a < b) ? a : b;
    std::cout << "Min: " << min << std::endl;

    // Determine even/odd
    int num = 17;
    std::string parity = (num % 2 == 0) ? "even" : "odd";
    std::cout << num << " is " << parity << std::endl;

    // Absolute value
    int x = -5;
    int absValue = (x < 0) ? -x : x;
    std::cout << "Absolute value: " << absValue << std::endl;

    return 0;
}
```

## 8. Complete Examples

### Example 1: Login System

```cpp
#include <iostream>
#include <string>

int main() {
    std::string username, password;
    const std::string CORRECT_USER = "admin";
    const std::string CORRECT_PASS = "1234";

    std::cout << "=== Login System ===" << std::endl;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (username == CORRECT_USER && password == CORRECT_PASS) {
        std::cout << "\nLogin successful!" << std::endl;
        std::cout << "Welcome, " << username << "!" << std::endl;
    } else if (username != CORRECT_USER) {
        std::cout << "\nError: Invalid username" << std::endl;
    } else {
        std::cout << "\nError: Invalid password" << std::endl;
    }

    return 0;
}
```

### Example 2: BMI Calculator with Interpretation

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double weight, height, bmi;

    std::cout << "=== BMI Calculator ===" << std::endl;
    std::cout << "Weight (kg): ";
    std::cin >> weight;
    std::cout << "Height (m): ";
    std::cin >> height;

    bmi = weight / (height * height);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\nYour BMI: " << bmi << std::endl;

    // Interpret BMI
    if (bmi < 18.5) {
        std::cout << "Category: Underweight" << std::endl;
        std::cout << "Recommendation: Consider gaining weight" << std::endl;
    } else if (bmi < 25.0) {
        std::cout << "Category: Normal weight" << std::endl;
        std::cout << "Recommendation: Maintain current weight" << std::endl;
    } else if (bmi < 30.0) {
        std::cout << "Category: Overweight" << std::endl;
        std::cout << "Recommendation: Consider losing weight" << std::endl;
    } else {
        std::cout << "Category: Obese" << std::endl;
        std::cout << "Recommendation: Consult a doctor" << std::endl;
    }

    return 0;
}
```

### Example 3: Ticket Pricing System

```cpp
#include <iostream>

int main() {
    int age;
    bool isStudent, isSenior;
    char dayType;
    double basePrice = 15.0;
    double finalPrice;

    std::cout << "=== Movie Ticket Pricing ===" << std::endl;
    std::cout << "Age: ";
    std::cin >> age;
    std::cout << "Student? (1=yes, 0=no): ";
    std::cin >> isStudent;
    std::cout << "Day type (W=weekday, E=weekend): ";
    std::cin >> dayType;

    // Calculate price based on conditions
    if (age < 5) {
        finalPrice = 0.0;  // Free for very young children
        std::cout << "\nChildren under 5 watch for free!" << std::endl;
    } else if (age <= 12) {
        finalPrice = basePrice * 0.5;  // 50% off for children
        std::cout << "\nChild ticket (50% off)" << std::endl;
    } else if (age >= 65) {
        finalPrice = basePrice * 0.6;  // 40% off for seniors
        std::cout << "\nSenior ticket (40% off)" << std::endl;
    } else if (isStudent) {
        finalPrice = basePrice * 0.7;  // 30% off for students
        std::cout << "\nStudent ticket (30% off)" << std::endl;
    } else {
        finalPrice = basePrice;
        std::cout << "\nAdult ticket" << std::endl;
    }

    // Weekend surcharge
    if (dayType == 'E' || dayType == 'e') {
        finalPrice += 3.0;
        std::cout << "Weekend surcharge: +$3.00" << std::endl;
    }

    std::cout << "\nFinal price: $" << finalPrice << std::endl;

    return 0;
}
```

### Example 4: Simple Quiz

```cpp
#include <iostream>

int main() {
    int score = 0;
    int answer;

    std::cout << "=== C++ Quiz ===" << std::endl;
    std::cout << "\nQuestion 1: What does 'int' stand for?" << std::endl;
    std::cout << "1. Internet\n2. Integer\n3. Internal\n4. Integrate" << std::endl;
    std::cout << "Answer: ";
    std::cin >> answer;

    if (answer == 2) {
        std::cout << "Correct!" << std::endl;
        score++;
    } else {
        std::cout << "Wrong! Correct answer is 2 (Integer)" << std::endl;
    }

    std::cout << "\nQuestion 2: What is 5 + 3 * 2?" << std::endl;
    std::cout << "1. 16\n2. 11\n3. 13\n4. 10" << std::endl;
    std::cout << "Answer: ";
    std::cin >> answer;

    if (answer == 2) {
        std::cout << "Correct!" << std::endl;
        score++;
    } else {
        std::cout << "Wrong! Correct answer is 2 (11)" << std::endl;
    }

    std::cout << "\nQuestion 3: Which symbol is used for comments?" << std::endl;
    std::cout << "1. #\n2. //\n3. /*\n4. Both 2 and 3" << std::endl;
    std::cout << "Answer: ";
    std::cin >> answer;

    if (answer == 4) {
        std::cout << "Correct!" << std::endl;
        score++;
    } else {
        std::cout << "Wrong! Correct answer is 4 (Both // and /*)" << std::endl;
    }

    // Final score
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Score: " << score << " out of 3" << std::endl;

    if (score == 3) {
        std::cout << "Perfect! Excellent work!" << std::endl;
    } else if (score == 2) {
        std::cout << "Good job! Almost there!" << std::endl;
    } else if (score == 1) {
        std::cout << "Keep studying!" << std::endl;
    } else {
        std::cout << "Please review the material" << std::endl;
    }

    return 0;
}
```

## 9. Common Mistakes

### Mistake 1: Using = instead of ==

```cpp
int x = 5;

// WRONG - Assignment, not comparison!
if (x = 10) {  // This assigns 10 to x and always returns true!
    std::cout << "This always executes" << std::endl;
}

// CORRECT - Comparison
if (x == 10) {  // This compares x with 10
    std::cout << "x equals 10" << std::endl;
}
```

### Mistake 2: Missing Braces

```cpp
int age = 15;

// Dangerous - only one line is conditional!
if (age >= 18)
    std::cout << "You can vote" << std::endl;
    std::cout << "Welcome!" << std::endl;  // Always executes!

// Better - use braces
if (age >= 18) {
    std::cout << "You can vote" << std::endl;
    std::cout << "Welcome!" << std::endl;  // Only executes if true
}
```

### Mistake 3: Comparing Floats Directly

```cpp
#include <cmath>

double result = 0.1 + 0.2;  // Due to floating-point precision, might not be exactly 0.3

// WRONG
if (result == 0.3) {  // Might fail!
    std::cout << "Equal" << std::endl;
}

// CORRECT - Use epsilon for comparison
const double EPSILON = 0.0001;
if (std::abs(result - 0.3) < EPSILON) {
    std::cout << "Approximately equal" << std::endl;
}
```

## 10. Exercises

### Exercise 1: Leap Year Checker
Write a program to check if a year is a leap year.
Rules:
- Divisible by 4 AND
- (Not divisible by 100 OR divisible by 400)

### Exercise 2: Grade Calculator with +/-
Enhance the grade calculator to include + and - grades (A+, A, A-, B+, etc.)

### Exercise 3: Triangle Type
Given three sides, determine if they form:
- Equilateral (all equal)
- Isosceles (two equal)
- Scalene (none equal)
- Or not a valid triangle

### Exercise 4: Electricity Bill
Calculate electricity bill:
- First 100 units: $0.50/unit
- Next 100 units: $0.75/unit
- Above 200 units: $1.20/unit

### Exercise 5: Rock Paper Scissors
Create a rock-paper-scissors game against the computer.

## 11. Solutions

### Solution 1: Leap Year

```cpp
#include <iostream>

int main() {
    int year;
    std::cout << "Enter a year: ";
    std::cin >> year;

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        std::cout << year << " is a leap year" << std::endl;
    } else {
        std::cout << year << " is not a leap year" << std::endl;
    }

    return 0;
}
```

### Solution 2: Enhanced Grade Calculator

```cpp
#include <iostream>

int main() {
    int score;
    std::cout << "Enter score (0-100): ";
    std::cin >> score;

    if (score >= 97) {
        std::cout << "Grade: A+" << std::endl;
    } else if (score >= 93) {
        std::cout << "Grade: A" << std::endl;
    } else if (score >= 90) {
        std::cout << "Grade: A-" << std::endl;
    } else if (score >= 87) {
        std::cout << "Grade: B+" << std::endl;
    } else if (score >= 83) {
        std::cout << "Grade: B" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B-" << std::endl;
    } else if (score >= 77) {
        std::cout << "Grade: C+" << std::endl;
    } else if (score >= 73) {
        std::cout << "Grade: C" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C-" << std::endl;
    } else if (score >= 67) {
        std::cout << "Grade: D+" << std::endl;
    } else if (score >= 63) {
        std::cout << "Grade: D" << std::endl;
    } else if (score >= 60) {
        std::cout << "Grade: D-" << std::endl;
    } else {
        std::cout << "Grade: F" << std::endl;
    }

    return 0;
}
```

### Solution 3: Triangle Type

```cpp
#include <iostream>

int main() {
    double a, b, c;
    std::cout << "Enter three sides: ";
    std::cin >> a >> b >> c;

    // Check if valid triangle
    if (a + b <= c || a + c <= b || b + c <= a) {
        std::cout << "Not a valid triangle!" << std::endl;
    } else if (a == b && b == c) {
        std::cout << "Equilateral triangle" << std::endl;
    } else if (a == b || b == c || a == c) {
        std::cout << "Isosceles triangle" << std::endl;
    } else {
        std::cout << "Scalene triangle" << std::endl;
    }

    return 0;
}
```

### Solution 4: Electricity Bill

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int units;
    double bill;

    std::cout << "Enter units consumed: ";
    std::cin >> units;

    if (units <= 100) {
        bill = units * 0.50;
    } else if (units <= 200) {
        bill = 100 * 0.50 + (units - 100) * 0.75;
    } else {
        bill = 100 * 0.50 + 100 * 0.75 + (units - 200) * 1.20;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total bill: $" << bill << std::endl;

    return 0;
}
```

### Solution 5: Rock Paper Scissors

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));  // Seed random number generator

    int player, computer;

    std::cout << "Rock Paper Scissors!" << std::endl;
    std::cout << "1. Rock\n2. Paper\n3. Scissors" << std::endl;
    std::cout << "Your choice: ";
    std::cin >> player;

    computer = (rand() % 3) + 1;  // Random 1-3

    std::cout << "\nYou chose: ";
    switch (player) {
        case 1: std::cout << "Rock"; break;
        case 2: std::cout << "Paper"; break;
        case 3: std::cout << "Scissors"; break;
        default: std::cout << "Invalid"; return 1;
    }

    std::cout << "\nComputer chose: ";
    switch (computer) {
        case 1: std::cout << "Rock"; break;
        case 2: std::cout << "Paper"; break;
        case 3: std::cout << "Scissors"; break;
    }
    std::cout << std::endl;

    if (player == computer) {
        std::cout << "\nIt's a tie!" << std::endl;
    } else if ((player == 1 && computer == 3) ||
               (player == 2 && computer == 1) ||
               (player == 3 && computer == 2)) {
        std::cout << "\nYou win!" << std::endl;
    } else {
        std::cout << "\nComputer wins!" << std::endl;
    }

    return 0;
}
```

## Summary

### Key Points
- **if** executes code when condition is true
- **else** provides alternative when condition is false
- **else if** checks multiple conditions in sequence
- **switch** is efficient for checking single variable against many values
- **Logical operators** (&&, ||, !) combine conditions
- **Ternary operator** (?:) is shorthand for simple if-else
- Always use **==** for comparison, not **=**
- Use braces {} even for single statements

### Best Practices
- Use meaningful condition names
- Keep conditions simple and readable
- Use parentheses for clarity
- Always include default case in switch
- Don't forget break in switch
- Avoid deep nesting - refactor complex logic

## Checklist
- [ ] Understand if, else if, else
- [ ] Can write complex conditions
- [ ] Master logical operators
- [ ] Know when to use switch vs if
- [ ] Understand fall-through in switch
- [ ] Can use ternary operator
- [ ] Avoid common mistakes
- [ ] Completed all exercises

---

**Time to complete**: 2.5 hours
**Previous lesson**: [Lesson 04 - Operators](../Lesson-04-Operators/README.md)
**Next lesson**: [Lesson 06 - Loops](../Lesson-06/README.md)
