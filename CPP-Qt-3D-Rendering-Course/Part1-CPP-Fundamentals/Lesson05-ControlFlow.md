# Lesson 05: Control Flow Statements

**Duration: 2.5 hours**

## Table of Contents
1. If Statements
2. If-Else Statements
3. Else-If Chains
4. Nested If Statements
5. Switch Statements
6. Conditional Operator
7. Practical Examples

## 1. If Statements

### Basic If Statement

```cpp
#include <iostream>

int main() {
    int age = 20;

    if (age >= 18) {
        std::cout << "You are an adult" << std::endl;
    }

    std::cout << "Program continues..." << std::endl;

    return 0;
}
```

### If with Multiple Statements

```cpp
#include <iostream>

int main() {
    int score = 85;

    if (score >= 60) {
        std::cout << "Congratulations!" << std::endl;
        std::cout << "You passed the exam" << std::endl;
        std::cout << "Your score: " << score << std::endl;
    }

    return 0;
}
```

### Common Conditions

```cpp
#include <iostream>

int main() {
    int number = 42;

    // Equality check
    if (number == 42) {
        std::cout << "Number is 42" << std::endl;
    }

    // Range check
    if (number >= 1 && number <= 100) {
        std::cout << "Number is between 1 and 100" << std::endl;
    }

    // Multiple conditions
    bool hasPermission = true;
    bool isActive = true;

    if (hasPermission && isActive) {
        std::cout << "Access granted" << std::endl;
    }

    return 0;
}
```

## 2. If-Else Statements

### Basic If-Else

```cpp
#include <iostream>

int main() {
    int age = 16;

    if (age >= 18) {
        std::cout << "You can vote" << std::endl;
    } else {
        std::cout << "You cannot vote yet" << std::endl;
    }

    return 0;
}
```

### Practical Examples

```cpp
#include <iostream>

int main() {
    // Example 1: Even or Odd
    int number = 7;

    if (number % 2 == 0) {
        std::cout << number << " is even" << std::endl;
    } else {
        std::cout << number << " is odd" << std::endl;
    }

    // Example 2: Password validation
    std::string password = "secret123";

    if (password.length() >= 8) {
        std::cout << "Password is strong enough" << std::endl;
    } else {
        std::cout << "Password is too short" << std::endl;
    }

    // Example 3: Temperature check
    double temp = 25.5;

    if (temp > 30) {
        std::cout << "It's hot outside" << std::endl;
    } else {
        std::cout << "Temperature is comfortable" << std::endl;
    }

    return 0;
}
```

## 3. Else-If Chains

### Multiple Conditions

```cpp
#include <iostream>

int main() {
    int score = 75;

    if (score >= 90) {
        std::cout << "Grade: A" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C" << std::endl;
    } else if (score >= 60) {
        std::cout << "Grade: D" << std::endl;
    } else {
        std::cout << "Grade: F" << std::endl;
    }

    return 0;
}
```

### Range Checking

```cpp
#include <iostream>

int main() {
    int age;

    std::cout << "Enter your age: ";
    std::cin >> age;

    if (age < 0) {
        std::cout << "Invalid age" << std::endl;
    } else if (age < 13) {
        std::cout << "You are a child" << std::endl;
    } else if (age < 20) {
        std::cout << "You are a teenager" << std::endl;
    } else if (age < 60) {
        std::cout << "You are an adult" << std::endl;
    } else {
        std::cout << "You are a senior" << std::endl;
    }

    return 0;
}
```

### BMI Calculator

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double weight, height;

    std::cout << "Enter weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter height (m): ";
    std::cin >> height;

    double bmi = weight / (height * height);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Your BMI: " << bmi << std::endl;

    if (bmi < 18.5) {
        std::cout << "Classification: Underweight" << std::endl;
    } else if (bmi < 25) {
        std::cout << "Classification: Normal weight" << std::endl;
    } else if (bmi < 30) {
        std::cout << "Classification: Overweight" << std::endl;
    } else {
        std::cout << "Classification: Obese" << std::endl;
    }

    return 0;
}
```

## 4. Nested If Statements

### Basic Nesting

```cpp
#include <iostream>

int main() {
    int age = 25;
    bool hasLicense = true;

    if (age >= 18) {
        if (hasLicense) {
            std::cout << "You can drive" << std::endl;
        } else {
            std::cout << "You need a license" << std::endl;
        }
    } else {
        std::cout << "You are too young to drive" << std::endl;
    }

    return 0;
}
```

### Multiple Level Nesting

```cpp
#include <iostream>

int main() {
    int age = 20;
    bool isStudent = true;
    double income = 15000;

    if (age >= 18) {
        if (isStudent) {
            if (income < 20000) {
                std::cout << "Eligible for student discount" << std::endl;
            } else {
                std::cout << "Income too high for discount" << std::endl;
            }
        } else {
            std::cout << "Not a student" << std::endl;
        }
    } else {
        std::cout << "Must be 18 or older" << std::endl;
    }

    return 0;
}
```

### Login System Example

```cpp
#include <iostream>
#include <string>

int main() {
    std::string username, password;
    bool isActive = true;

    std::cout << "Username: ";
    std::cin >> username;

    std::cout << "Password: ";
    std::cin >> password;

    if (username == "admin") {
        if (password == "admin123") {
            if (isActive) {
                std::cout << "Login successful!" << std::endl;
                std::cout << "Welcome, Administrator" << std::endl;
            } else {
                std::cout << "Account is deactivated" << std::endl;
            }
        } else {
            std::cout << "Incorrect password" << std::endl;
        }
    } else {
        std::cout << "Username not found" << std::endl;
    }

    return 0;
}
```

## 5. Switch Statements

### Basic Switch

```cpp
#include <iostream>

int main() {
    int day = 3;

    switch (day) {
        case 1:
            std::cout << "Monday" << std::endl;
            break;
        case 2:
            std::cout << "Tuesday" << std::endl;
            break;
        case 3:
            std::cout << "Wednesday" << std::endl;
            break;
        case 4:
            std::cout << "Thursday" << std::endl;
            break;
        case 5:
            std::cout << "Friday" << std::endl;
            break;
        case 6:
            std::cout << "Saturday" << std::endl;
            break;
        case 7:
            std::cout << "Sunday" << std::endl;
            break;
        default:
            std::cout << "Invalid day" << std::endl;
    }

    return 0;
}
```

### Switch Without Break (Fall-through)

```cpp
#include <iostream>

int main() {
    int month = 2;

    switch (month) {
        case 12:
        case 1:
        case 2:
            std::cout << "Winter" << std::endl;
            break;
        case 3:
        case 4:
        case 5:
            std::cout << "Spring" << std::endl;
            break;
        case 6:
        case 7:
        case 8:
            std::cout << "Summer" << std::endl;
            break;
        case 9:
        case 10:
        case 11:
            std::cout << "Fall" << std::endl;
            break;
        default:
            std::cout << "Invalid month" << std::endl;
    }

    return 0;
}
```

### Character-based Switch

```cpp
#include <iostream>

int main() {
    char grade;

    std::cout << "Enter grade (A-F): ";
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
            std::cout << "Invalid grade" << std::endl;
    }

    return 0;
}
```

### Calculator with Switch

```cpp
#include <iostream>

int main() {
    double num1, num2, result;
    char operation;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> operation;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    switch (operation) {
        case '+':
            result = num1 + num2;
            std::cout << "Result: " << result << std::endl;
            break;
        case '-':
            result = num1 - num2;
            std::cout << "Result: " << result << std::endl;
            break;
        case '*':
            result = num1 * num2;
            std::cout << "Result: " << result << std::endl;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                std::cout << "Result: " << result << std::endl;
            } else {
                std::cout << "Error: Division by zero!" << std::endl;
            }
            break;
        default:
            std::cout << "Invalid operator!" << std::endl;
    }

    return 0;
}
```

## 6. Conditional Operator

### Ternary Operator Basics

```cpp
#include <iostream>

int main() {
    int age = 20;

    // condition ? value_if_true : value_if_false
    std::string status = (age >= 18) ? "Adult" : "Minor";
    std::cout << "Status: " << status << std::endl;

    // Finding maximum
    int a = 10, b = 20;
    int max = (a > b) ? a : b;
    std::cout << "Maximum: " << max << std::endl;

    // Absolute value
    int num = -5;
    int absolute = (num >= 0) ? num : -num;
    std::cout << "Absolute value: " << absolute << std::endl;

    return 0;
}
```

### Nested Ternary (Use Carefully!)

```cpp
#include <iostream>

int main() {
    int score = 85;

    // Nested ternary for grades
    char grade = (score >= 90) ? 'A' :
                 (score >= 80) ? 'B' :
                 (score >= 70) ? 'C' :
                 (score >= 60) ? 'D' : 'F';

    std::cout << "Grade: " << grade << std::endl;

    // Better: use if-else for readability
    char grade2;
    if (score >= 90) grade2 = 'A';
    else if (score >= 80) grade2 = 'B';
    else if (score >= 70) grade2 = 'C';
    else if (score >= 60) grade2 = 'D';
    else grade2 = 'F';

    return 0;
}
```

## 7. Practical Examples

### Example 1: Triangle Validator

```cpp
#include <iostream>

int main() {
    double a, b, c;

    std::cout << "Enter three sides of triangle: ";
    std::cin >> a >> b >> c;

    if (a <= 0 || b <= 0 || c <= 0) {
        std::cout << "Invalid: sides must be positive" << std::endl;
    } else if (a + b <= c || b + c <= a || a + c <= b) {
        std::cout << "Invalid: does not form a triangle" << std::endl;
    } else {
        std::cout << "Valid triangle!" << std::endl;

        if (a == b && b == c) {
            std::cout << "Type: Equilateral" << std::endl;
        } else if (a == b || b == c || a == c) {
            std::cout << "Type: Isosceles" << std::endl;
        } else {
            std::cout << "Type: Scalene" << std::endl;
        }
    }

    return 0;
}
```

### Example 2: Menu System

```cpp
#include <iostream>
#include <string>

int main() {
    int choice;

    std::cout << "=== Main Menu ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Settings" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter choice: ";
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
    }

    return 0;
}
```

### Example 3: Shipping Calculator

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double weight;
    std::string destination;
    double shippingCost = 0;

    std::cout << "Enter package weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter destination (domestic/international): ";
    std::cin >> destination;

    if (destination == "domestic") {
        if (weight <= 1) {
            shippingCost = 5.00;
        } else if (weight <= 5) {
            shippingCost = 10.00;
        } else if (weight <= 10) {
            shippingCost = 15.00;
        } else {
            shippingCost = 15.00 + (weight - 10) * 2.00;
        }
    } else if (destination == "international") {
        if (weight <= 1) {
            shippingCost = 15.00;
        } else if (weight <= 5) {
            shippingCost = 30.00;
        } else if (weight <= 10) {
            shippingCost = 50.00;
        } else {
            shippingCost = 50.00 + (weight - 10) * 5.00;
        }
    } else {
        std::cout << "Invalid destination!" << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Shipping cost: $" << shippingCost << std::endl;

    return 0;
}
```

## Complete Project: Student Grade System

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    std::string studentName;
    double test1, test2, test3, finalExam;

    // Input
    std::cout << "=== Student Grade System ===" << std::endl;
    std::cout << "Student name: ";
    std::cin.ignore();
    std::getline(std::cin, studentName);

    std::cout << "Test 1 score (0-100): ";
    std::cin >> test1;

    std::cout << "Test 2 score (0-100): ";
    std::cin >> test2;

    std::cout << "Test 3 score (0-100): ";
    std::cin >> test3;

    std::cout << "Final exam score (0-100): ";
    std::cin >> finalExam;

    // Validation
    if (test1 < 0 || test1 > 100 ||
        test2 < 0 || test2 > 100 ||
        test3 < 0 || test3 > 100 ||
        finalExam < 0 || finalExam > 100) {
        std::cout << "Error: Scores must be between 0 and 100" << std::endl;
        return 1;
    }

    // Calculate average (tests: 50%, final: 50%)
    double testAverage = (test1 + test2 + test3) / 3.0;
    double finalGrade = (testAverage * 0.5) + (finalExam * 0.5);

    // Determine letter grade
    char letterGrade;
    std::string performance;

    if (finalGrade >= 90) {
        letterGrade = 'A';
        performance = "Excellent";
    } else if (finalGrade >= 80) {
        letterGrade = 'B';
        performance = "Good";
    } else if (finalGrade >= 70) {
        letterGrade = 'C';
        performance = "Average";
    } else if (finalGrade >= 60) {
        letterGrade = 'D';
        performance = "Below Average";
    } else {
        letterGrade = 'F';
        performance = "Failed";
    }

    // Determine pass/fail
    bool passed = (finalGrade >= 60);

    // Display results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Grade Report ===" << std::endl;
    std::cout << "Student: " << studentName << std::endl;
    std::cout << "Test 1: " << test1 << std::endl;
    std::cout << "Test 2: " << test2 << std::endl;
    std::cout << "Test 3: " << test3 << std::endl;
    std::cout << "Test Average: " << testAverage << std::endl;
    std::cout << "Final Exam: " << finalExam << std::endl;
    std::cout << "Final Grade: " << finalGrade << std::endl;
    std::cout << "Letter Grade: " << letterGrade << std::endl;
    std::cout << "Performance: " << performance << std::endl;
    std::cout << "Status: " << (passed ? "PASSED" : "FAILED") << std::endl;

    return 0;
}
```

## Summary

You learned:
- If statements for single conditions
- If-else for binary decisions
- Else-if chains for multiple conditions
- Nested if statements
- Switch statements for multi-way branching
- Ternary conditional operator
- Practical decision-making patterns

## Next Lesson

Lesson 06: Loops (for, while, do-while)
