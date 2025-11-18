# Lesson 4: Control Flow (if, else, else if)

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Introduction to Control Flow
2. The if Statement
3. The if-else Statement
4. The else if Ladder
5. Nested if Statements
6. Compound Conditions
7. Short-Circuit Evaluation
8. Exercises and Projects

---

## 1. Introduction to Control Flow

Control flow statements allow your program to make decisions and execute different code based on conditions. This is fundamental to creating intelligent, responsive programs.

### Why Control Flow Matters:

```cpp
#include <iostream>

int main()
{
    int age = 20;

    // Without control flow - prints both messages
    std::cout << "You are a minor\n";
    std::cout << "You are an adult\n";

    // This is wrong! We need control flow to decide which message to show

    return 0;
}
```

---

## 2. The if Statement

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    int age = 18;

    if (age >= 18)
    {
        std::cout << "You are an adult!\n";
        std::cout << "You can vote!\n";
    }

    std::cout << "Program continues...\n";

    return 0;
}
```

**Output**:
```
You are an adult!
You can vote!
Program continues...
```

### Single Statement (No Braces):

```cpp
#include <iostream>

int main()
{
    int temperature = 35;

    // Single statement - braces optional but recommended
    if (temperature > 30)
        std::cout << "It's hot outside!\n";

    // Multiple statements - braces required
    if (temperature > 30)
    {
        std::cout << "It's hot outside!\n";
        std::cout << "Stay hydrated!\n";
    }

    return 0;
}
```

### Practical Examples:

```cpp
#include <iostream>

int main()
{
    // Example 1: Password check
    int password = 1234;
    int userInput = 1234;

    if (userInput == password)
    {
        std::cout << "Access granted!\n";
    }

    // Example 2: Grade check
    double score = 85.5;

    if (score >= 90.0)
    {
        std::cout << "Excellent! Grade: A\n";
    }

    // Example 3: Even number check
    int number = 42;

    if (number % 2 == 0)
    {
        std::cout << number << " is even\n";
    }

    return 0;
}
```

---

## 3. The if-else Statement

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    int age = 15;

    if (age >= 18)
    {
        std::cout << "You are an adult\n";
    }
    else
    {
        std::cout << "You are a minor\n";
    }

    return 0;
}
```

**Output**:
```
You are a minor
```

### Multiple Examples:

```cpp
#include <iostream>

int main()
{
    // Example 1: Temperature
    int temp = 25;

    if (temp > 30)
    {
        std::cout << "Turn on the AC\n";
    }
    else
    {
        std::cout << "Temperature is comfortable\n";
    }

    // Example 2: Login
    std::string username = "admin";
    std::string password = "secret123";

    if (username == "admin" && password == "secret123")
    {
        std::cout << "Login successful!\n";
    }
    else
    {
        std::cout << "Invalid credentials!\n";
    }

    // Example 3: Positive/Negative
    int number = -5;

    if (number >= 0)
    {
        std::cout << number << " is positive or zero\n";
    }
    else
    {
        std::cout << number << " is negative\n";
    }

    return 0;
}
```

---

## 4. The else if Ladder

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    double score = 85.0;

    if (score >= 90.0)
    {
        std::cout << "Grade: A\n";
    }
    else if (score >= 80.0)
    {
        std::cout << "Grade: B\n";
    }
    else if (score >= 70.0)
    {
        std::cout << "Grade: C\n";
    }
    else if (score >= 60.0)
    {
        std::cout << "Grade: D\n";
    }
    else
    {
        std::cout << "Grade: F\n";
    }

    return 0;
}
```

**Output**:
```
Grade: B
```

### Complete Grading System:

```cpp
#include <iostream>
#include <string>

int main()
{
    double score;

    std::cout << "Enter your score (0-100): ";
    std::cin >> score;

    std::string grade;
    std::string message;

    if (score > 100 || score < 0)
    {
        std::cout << "Invalid score!\n";
    }
    else if (score >= 90)
    {
        grade = "A";
        message = "Excellent work!";
    }
    else if (score >= 80)
    {
        grade = "B";
        message = "Great job!";
    }
    else if (score >= 70)
    {
        grade = "C";
        message = "Good effort!";
    }
    else if (score >= 60)
    {
        grade = "D";
        message = "You passed, but need improvement";
    }
    else
    {
        grade = "F";
        message = "Please try again";
    }

    if (score >= 0 && score <= 100)
    {
        std::cout << "\n===== GRADE REPORT =====\n";
        std::cout << "Score: " << score << "\n";
        std::cout << "Grade: " << grade << "\n";
        std::cout << "Comment: " << message << "\n";
    }

    return 0;
}
```

### Day of Week Example:

```cpp
#include <iostream>

int main()
{
    int day;

    std::cout << "Enter day number (1-7): ";
    std::cin >> day;

    if (day == 1)
    {
        std::cout << "Monday - Start of the work week\n";
    }
    else if (day == 2)
    {
        std::cout << "Tuesday\n";
    }
    else if (day == 3)
    {
        std::cout << "Wednesday - Midweek\n";
    }
    else if (day == 4)
    {
        std::cout << "Thursday\n";
    }
    else if (day == 5)
    {
        std::cout << "Friday - Almost weekend!\n";
    }
    else if (day == 6)
    {
        std::cout << "Saturday - Weekend!\n";
    }
    else if (day == 7)
    {
        std::cout << "Sunday - Weekend!\n";
    }
    else
    {
        std::cout << "Invalid day number!\n";
    }

    return 0;
}
```

---

## 5. Nested if Statements

### Basic Nesting:

```cpp
#include <iostream>

int main()
{
    int age = 25;
    bool hasLicense = true;

    if (age >= 18)
    {
        std::cout << "Age requirement met\n";

        if (hasLicense)
        {
            std::cout << "You can drive!\n";
        }
        else
        {
            std::cout << "You need a license to drive\n";
        }
    }
    else
    {
        std::cout << "You are too young to drive\n";
    }

    return 0;
}
```

### Complex Nesting Example:

```cpp
#include <iostream>

int main()
{
    double income = 50000;
    int dependents = 2;
    bool hasDeductions = true;

    std::cout << "===== TAX CALCULATOR =====\n\n";
    std::cout << "Income: $" << income << "\n";
    std::cout << "Dependents: " << dependents << "\n\n";

    double taxRate;

    if (income <= 10000)
    {
        taxRate = 0.0;
        std::cout << "Tax bracket: No tax\n";
    }
    else if (income <= 40000)
    {
        taxRate = 0.10;
        std::cout << "Tax bracket: 10%\n";

        if (dependents > 0)
        {
            taxRate -= 0.01 * dependents;
            std::cout << "Dependent deduction applied\n";
        }
    }
    else if (income <= 80000)
    {
        taxRate = 0.20;
        std::cout << "Tax bracket: 20%\n";

        if (dependents > 0)
        {
            taxRate -= 0.01 * dependents;
            std::cout << "Dependent deduction applied\n";
        }

        if (hasDeductions)
        {
            taxRate -= 0.02;
            std::cout << "Standard deduction applied\n";
        }
    }
    else
    {
        taxRate = 0.30;
        std::cout << "Tax bracket: 30%\n";

        if (dependents > 2)
        {
            taxRate -= 0.02;
            std::cout << "Large family deduction applied\n";
        }
    }

    double tax = income * taxRate;
    std::cout << "\nEffective tax rate: " << (taxRate * 100) << "%\n";
    std::cout << "Tax amount: $" << tax << "\n";

    return 0;
}
```

### Login System with Nested Checks:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string username, password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    std::cout << "===== LOGIN SYSTEM =====\n\n";

    std::cout << "Enter username: ";
    std::cin >> username;

    if (username == "admin" || username == "user")
    {
        std::cout << "Username found\n";
        std::cout << "Enter password: ";
        std::cin >> password;

        if (username == "admin")
        {
            if (password == "admin123")
            {
                std::cout << "\n✓ Admin login successful!\n";
                std::cout << "Access level: Full\n";
            }
            else
            {
                std::cout << "\n✗ Incorrect admin password\n";
            }
        }
        else if (username == "user")
        {
            if (password == "user123")
            {
                std::cout << "\n✓ User login successful!\n";
                std::cout << "Access level: Standard\n";
            }
            else
            {
                std::cout << "\n✗ Incorrect user password\n";
            }
        }
    }
    else
    {
        std::cout << "\n✗ Username not found\n";
    }

    return 0;
}
```

---

## 6. Compound Conditions

### Using AND (&&):

```cpp
#include <iostream>

int main()
{
    int age = 25;
    double income = 30000;
    bool hasGoodCredit = true;

    // All conditions must be true
    if (age >= 21 && income >= 25000 && hasGoodCredit)
    {
        std::cout << "Loan approved!\n";
    }
    else
    {
        std::cout << "Loan denied\n";

        // Show reasons
        if (age < 21)
            std::cout << "Reason: Age too young\n";
        if (income < 25000)
            std::cout << "Reason: Insufficient income\n";
        if (!hasGoodCredit)
            std::cout << "Reason: Poor credit\n";
    }

    return 0;
}
```

### Using OR (||):

```cpp
#include <iostream>

int main()
{
    int day = 6;

    // Any condition being true is enough
    if (day == 6 || day == 7)
    {
        std::cout << "It's the weekend!\n";
    }
    else
    {
        std::cout << "It's a weekday\n";
    }

    // Multiple OR conditions
    char grade = 'B';
    if (grade == 'A' || grade == 'B' || grade == 'C')
    {
        std::cout << "You passed!\n";
    }
    else
    {
        std::cout << "You failed\n";
    }

    return 0;
}
```

### Combining AND and OR:

```cpp
#include <iostream>

int main()
{
    int age = 25;
    bool isStudent = false;
    bool isSenior = false;
    double ticketPrice = 15.0;

    std::cout << "===== MOVIE TICKET PRICING =====\n";
    std::cout << "Base price: $" << ticketPrice << "\n\n";

    // Discount for students OR seniors, but only if under 65 or over 13
    if ((isStudent || isSenior) && (age >= 13 && age <= 65))
    {
        ticketPrice *= 0.8;  // 20% discount
        std::cout << "Discount applied: 20%\n";
    }
    else if (age < 13)
    {
        ticketPrice *= 0.5;  // 50% discount for children
        std::cout << "Child discount: 50%\n";
    }

    std::cout << "Final price: $" << ticketPrice << "\n";

    return 0;
}
```

---

## 7. Short-Circuit Evaluation

### How it Works:

```cpp
#include <iostream>

int main()
{
    int x = 5;
    int y = 0;

    // AND (&&) - If first is false, second is NOT evaluated
    if (y != 0 && x / y > 2)  // Safe: division won't happen if y is 0
    {
        std::cout << "This won't print\n";
    }

    // OR (||) - If first is true, second is NOT evaluated
    if (x > 0 || y / x > 2)  // Safe: division won't happen since x > 0
    {
        std::cout << "First condition was true\n";
    }

    return 0;
}
```

### Practical Example:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string input = "";

    std::cout << "Enter a word (or press Enter to skip): ";
    std::getline(std::cin, input);

    // Check if string is not empty AND first character is 'A'
    // Short-circuit prevents accessing input[0] when string is empty
    if (input.length() > 0 && input[0] == 'A')
    {
        std::cout << "Word starts with 'A'\n";
    }
    else if (input.length() == 0)
    {
        std::cout << "No input provided\n";
    }
    else
    {
        std::cout << "Word does not start with 'A'\n";
    }

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Age Category Classifier

```cpp
#include <iostream>

int main()
{
    int age;

    std::cout << "Enter your age: ";
    std::cin >> age;

    if (age < 0)
    {
        std::cout << "Invalid age!\n";
    }
    else if (age <= 12)
    {
        std::cout << "You are a child\n";
    }
    else if (age <= 19)
    {
        std::cout << "You are a teenager\n";
    }
    else if (age <= 59)
    {
        std::cout << "You are an adult\n";
    }
    else
    {
        std::cout << "You are a senior\n";
    }

    return 0;
}
```

### Exercise 2: Temperature Advisor

```cpp
#include <iostream>

int main()
{
    double temp;

    std::cout << "Enter temperature in Celsius: ";
    std::cin >> temp;

    std::cout << "\n===== WEATHER ADVISOR =====\n";

    if (temp < 0)
    {
        std::cout << "Freezing! Wear heavy winter coat\n";
    }
    else if (temp < 10)
    {
        std::cout << "Cold! Wear a jacket\n";
    }
    else if (temp < 20)
    {
        std::cout << "Cool! Long sleeves recommended\n";
    }
    else if (temp < 30)
    {
        std::cout << "Comfortable! Perfect weather\n";
    }
    else
    {
        std::cout << "Hot! Stay hydrated\n";
    }

    return 0;
}
```

### Exercise 3: BMI Calculator

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    double weight, height, bmi;

    std::cout << "===== BMI CALCULATOR =====\n\n";
    std::cout << "Enter weight (kg): ";
    std::cin >> weight;
    std::cout << "Enter height (m): ";
    std::cin >> height;

    bmi = weight / (height * height);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nYour BMI: " << bmi << "\n";

    if (bmi < 18.5)
    {
        std::cout << "Category: Underweight\n";
        std::cout << "Recommendation: Consult a nutritionist\n";
    }
    else if (bmi < 25.0)
    {
        std::cout << "Category: Normal weight\n";
        std::cout << "Recommendation: Maintain current lifestyle\n";
    }
    else if (bmi < 30.0)
    {
        std::cout << "Category: Overweight\n";
        std::cout << "Recommendation: Exercise and balanced diet\n";
    }
    else
    {
        std::cout << "Category: Obese\n";
        std::cout << "Recommendation: Consult a doctor\n";
    }

    return 0;
}
```

### Exercise 4: Triangle Validator

```cpp
#include <iostream>

int main()
{
    double side1, side2, side3;

    std::cout << "Enter three sides of a triangle:\n";
    std::cout << "Side 1: ";
    std::cin >> side1;
    std::cout << "Side 2: ";
    std::cin >> side2;
    std::cout << "Side 3: ";
    std::cin >> side3;

    // Triangle inequality theorem
    if (side1 + side2 > side3 &&
        side2 + side3 > side1 &&
        side1 + side3 > side2)
    {
        std::cout << "\n✓ Valid triangle!\n";

        // Check triangle type
        if (side1 == side2 && side2 == side3)
        {
            std::cout << "Type: Equilateral (all sides equal)\n";
        }
        else if (side1 == side2 || side2 == side3 || side1 == side3)
        {
            std::cout << "Type: Isosceles (two sides equal)\n";
        }
        else
        {
            std::cout << "Type: Scalene (all sides different)\n";
        }
    }
    else
    {
        std::cout << "\n✗ Invalid triangle!\n";
        std::cout << "The sides don't satisfy triangle inequality\n";
    }

    return 0;
}
```

### Exercise 5: Leap Year Checker

```cpp
#include <iostream>

int main()
{
    int year;

    std::cout << "Enter a year: ";
    std::cin >> year;

    bool isLeap = false;

    // Leap year rules:
    // 1. Divisible by 4
    // 2. If divisible by 100, must also be divisible by 400
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
            {
                isLeap = true;
            }
        }
        else
        {
            isLeap = true;
        }
    }

    if (isLeap)
    {
        std::cout << year << " is a LEAP year\n";
        std::cout << "February has 29 days\n";
    }
    else
    {
        std::cout << year << " is NOT a leap year\n";
        std::cout << "February has 28 days\n";
    }

    return 0;
}
```

---

## Practice Project: Student Grade Management System

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    std::string studentName;
    int studentID;
    double homework, midterm, final, attendance;

    std::cout << "===== GRADE MANAGEMENT SYSTEM =====\n\n";

    // Input
    std::cout << "Student Name: ";
    std::getline(std::cin, studentName);

    std::cout << "Student ID: ";
    std::cin >> studentID;

    std::cout << "Homework Score (0-100): ";
    std::cin >> homework;

    std::cout << "Midterm Score (0-100): ";
    std::cin >> midterm;

    std::cout << "Final Exam Score (0-100): ";
    std::cin >> final;

    std::cout << "Attendance Percentage (0-100): ";
    std::cin >> attendance;

    // Validation
    if (homework < 0 || homework > 100 ||
        midterm < 0 || midterm > 100 ||
        final < 0 || final > 100 ||
        attendance < 0 || attendance > 100)
    {
        std::cout << "\n✗ ERROR: Invalid scores! All values must be 0-100\n";
        return 1;
    }

    // Calculate weighted average
    // Homework: 20%, Midterm: 30%, Final: 40%, Attendance: 10%
    double totalScore = (homework * 0.20) +
                        (midterm * 0.30) +
                        (final * 0.40) +
                        (attendance * 0.10);

    // Determine letter grade
    std::string letterGrade;
    std::string status;

    if (totalScore >= 90)
    {
        letterGrade = "A";
        status = "Excellent";
    }
    else if (totalScore >= 80)
    {
        letterGrade = "B";
        status = "Good";
    }
    else if (totalScore >= 70)
    {
        letterGrade = "C";
        status = "Satisfactory";
    }
    else if (totalScore >= 60)
    {
        letterGrade = "D";
        status = "Pass";
    }
    else
    {
        letterGrade = "F";
        status = "Fail";
    }

    // Check for special conditions
    bool perfectAttendance = (attendance == 100);
    bool honor = (totalScore >= 95);
    bool probation = (totalScore < 70);

    // Output Report
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n========================================\n";
    std::cout << "          GRADE REPORT                  \n";
    std::cout << "========================================\n";
    std::cout << "Student: " << studentName << "\n";
    std::cout << "ID: " << studentID << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Homework:    " << std::setw(6) << homework << " (20%)\n";
    std::cout << "Midterm:     " << std::setw(6) << midterm << " (30%)\n";
    std::cout << "Final Exam:  " << std::setw(6) << final << " (40%)\n";
    std::cout << "Attendance:  " << std::setw(6) << attendance << " (10%)\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Total Score: " << std::setw(6) << totalScore << "\n";
    std::cout << "Letter Grade: " << letterGrade << "\n";
    std::cout << "Status: " << status << "\n";

    // Special mentions
    if (honor)
    {
        std::cout << "\n⭐ HONOR ROLL - Outstanding Achievement!\n";
    }

    if (perfectAttendance)
    {
        std::cout << "✓ Perfect Attendance Award!\n";
    }

    if (probation)
    {
        std::cout << "\n⚠ Academic Probation - Improvement Needed\n";
    }

    std::cout << "========================================\n";

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Using = Instead of ==
```cpp
❌ if (x = 5)  // Assignment, not comparison!
✅ if (x == 5)  // Correct comparison
```

### Mistake 2: Missing Braces
```cpp
❌
if (x > 0)
    std::cout << "Positive\n";
    std::cout << "Number\n";  // Always executes!

✅
if (x > 0)
{
    std::cout << "Positive\n";
    std::cout << "Number\n";  // Only executes if x > 0
}
```

### Mistake 3: Wrong Condition Order
```cpp
❌
if (score >= 60)
    grade = "D";
else if (score >= 90)  // Never reached!
    grade = "A";

✅
if (score >= 90)
    grade = "A";
else if (score >= 60)
    grade = "D";
```

### Mistake 4: Forgetting Semicolons
```cpp
❌ if (x > 0);
    {
        // Always executes!
    }

✅ if (x > 0)
    {
        // Executes only if x > 0
    }
```

---

## Key Takeaways

1. ✅ `if` statements execute code only when a condition is true
2. ✅ `else` provides an alternative when the condition is false
3. ✅ `else if` allows multiple mutually exclusive conditions
4. ✅ Nested `if` statements enable complex decision trees
5. ✅ Use `&&` for AND conditions (all must be true)
6. ✅ Use `||` for OR conditions (at least one must be true)
7. ✅ Short-circuit evaluation prevents unnecessary operations
8. ✅ Always use braces `{}` for clarity and safety

---

## Summary Checklist

Before moving to Lesson 5, ensure you can:
- [ ] Write basic if statements
- [ ] Use if-else for binary decisions
- [ ] Create else if ladders for multiple conditions
- [ ] Nest if statements for complex logic
- [ ] Combine conditions with && and ||
- [ ] Understand short-circuit evaluation
- [ ] Validate user input properly
- [ ] Avoid common if statement mistakes
- [ ] Write clean, readable conditional code

---

## Next Lesson Preview

In **Lesson 5**, we'll learn about:
- Switch statements for multiple values
- Ternary operator (?:) for compact conditions
- When to use switch vs if-else
- Fall-through behavior
- Best practices for switch statements

**Great progress on Control Flow!** 🚀
