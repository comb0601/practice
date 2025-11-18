# Lesson 5: Switch Statements and Ternary Operator

**Duration**: 5 hours
**Difficulty**: Beginner

## Table of Contents
1. Introduction to Switch Statements
2. Switch Statement Syntax
3. Fall-Through Behavior
4. Switch vs If-Else
5. Ternary Operator
6. Nested Ternary Operators
7. Combining Switch and Ternary
8. Exercises and Projects

---

## 1. Introduction to Switch Statements

Switch statements provide a cleaner way to handle multiple conditions based on a single value. They're especially useful when checking one variable against many possible values.

### When to Use Switch:

```cpp
#include <iostream>

int main()
{
    int choice = 2;

    // Using if-else (verbose)
    if (choice == 1)
        std::cout << "Option 1\n";
    else if (choice == 2)
        std::cout << "Option 2\n";
    else if (choice == 3)
        std::cout << "Option 3\n";
    else
        std::cout << "Invalid\n";

    // Using switch (cleaner)
    switch (choice)
    {
        case 1:
            std::cout << "Option 1\n";
            break;
        case 2:
            std::cout << "Option 2\n";
            break;
        case 3:
            std::cout << "Option 3\n";
            break;
        default:
            std::cout << "Invalid\n";
    }

    return 0;
}
```

---

## 2. Switch Statement Syntax

### Basic Structure:

```cpp
#include <iostream>

int main()
{
    int day = 3;

    switch (day)
    {
        case 1:
            std::cout << "Monday\n";
            break;
        case 2:
            std::cout << "Tuesday\n";
            break;
        case 3:
            std::cout << "Wednesday\n";
            break;
        case 4:
            std::cout << "Thursday\n";
            break;
        case 5:
            std::cout << "Friday\n";
            break;
        case 6:
            std::cout << "Saturday\n";
            break;
        case 7:
            std::cout << "Sunday\n";
            break;
        default:
            std::cout << "Invalid day!\n";
    }

    return 0;
}
```

### Character Switch:

```cpp
#include <iostream>

int main()
{
    char grade;

    std::cout << "Enter your grade (A-F): ";
    std::cin >> grade;

    switch (grade)
    {
        case 'A':
        case 'a':
            std::cout << "Excellent! 90-100%\n";
            break;
        case 'B':
        case 'b':
            std::cout << "Good! 80-89%\n";
            break;
        case 'C':
        case 'c':
            std::cout << "Average! 70-79%\n";
            break;
        case 'D':
        case 'd':
            std::cout << "Below Average! 60-69%\n";
            break;
        case 'F':
        case 'f':
            std::cout << "Fail! Below 60%\n";
            break;
        default:
            std::cout << "Invalid grade!\n";
    }

    return 0;
}
```

### Calculator Example:

```cpp
#include <iostream>

int main()
{
    double num1, num2, result;
    char operation;

    std::cout << "===== CALCULATOR =====\n";
    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> operation;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    switch (operation)
    {
        case '+':
            result = num1 + num2;
            std::cout << num1 << " + " << num2 << " = " << result << "\n";
            break;

        case '-':
            result = num1 - num2;
            std::cout << num1 << " - " << num2 << " = " << result << "\n";
            break;

        case '*':
            result = num1 * num2;
            std::cout << num1 << " * " << num2 << " = " << result << "\n";
            break;

        case '/':
            if (num2 != 0)
            {
                result = num1 / num2;
                std::cout << num1 << " / " << num2 << " = " << result << "\n";
            }
            else
            {
                std::cout << "Error: Division by zero!\n";
            }
            break;

        default:
            std::cout << "Error: Invalid operation!\n";
    }

    return 0;
}
```

---

## 3. Fall-Through Behavior

### Intentional Fall-Through:

```cpp
#include <iostream>

int main()
{
    int month = 4;

    std::cout << "Month " << month << " has ";

    switch (month)
    {
        case 1:  // January
        case 3:  // March
        case 5:  // May
        case 7:  // July
        case 8:  // August
        case 10: // October
        case 12: // December
            std::cout << "31 days\n";
            break;

        case 4:  // April
        case 6:  // June
        case 9:  // September
        case 11: // November
            std::cout << "30 days\n";
            break;

        case 2:  // February
            std::cout << "28 or 29 days\n";
            break;

        default:
            std::cout << "Invalid month!\n";
    }

    return 0;
}
```

### Weekend Checker:

```cpp
#include <iostream>

int main()
{
    int day;

    std::cout << "Enter day (1-7): ";
    std::cin >> day;

    switch (day)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            std::cout << "Weekday - Time to work!\n";
            break;

        case 6:
        case 7:
            std::cout << "Weekend - Time to relax!\n";
            break;

        default:
            std::cout << "Invalid day number!\n";
    }

    return 0;
}
```

### Dangerous Fall-Through (Bug):

```cpp
#include <iostream>

int main()
{
    int choice = 1;

    std::cout << "Menu Selection: " << choice << "\n";

    // ❌ BUGGY CODE - Missing breaks!
    switch (choice)
    {
        case 1:
            std::cout << "You selected Option 1\n";
            // Missing break - falls through!
        case 2:
            std::cout << "You selected Option 2\n";
            // Missing break - falls through!
        case 3:
            std::cout << "You selected Option 3\n";
            break;
        default:
            std::cout << "Invalid option\n";
    }

    // Output will be:
    // You selected Option 1
    // You selected Option 2
    // You selected Option 3

    return 0;
}
```

---

## 4. Switch vs If-Else

### When to Use Switch:

```cpp
#include <iostream>

int main()
{
    int menuChoice = 3;

    // ✅ Good use of switch - checking single variable against constants
    switch (menuChoice)
    {
        case 1:
            std::cout << "New File\n";
            break;
        case 2:
            std::cout << "Open File\n";
            break;
        case 3:
            std::cout << "Save File\n";
            break;
        case 4:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Invalid choice\n";
    }

    return 0;
}
```

### When to Use If-Else:

```cpp
#include <iostream>

int main()
{
    double score = 85.5;

    // ✅ Good use of if-else - range checking
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
    else
    {
        std::cout << "Grade: F\n";
    }

    // ❌ Cannot use switch for ranges!

    return 0;
}
```

### Comparison Example:

```cpp
#include <iostream>

int main()
{
    int age = 25;
    bool hasLicense = true;

    // ❌ Cannot use switch for complex conditions
    if (age >= 18 && hasLicense)
    {
        std::cout << "Can drive\n";
    }

    // ✅ Switch only works with constant values
    char driverClass = 'B';

    switch (driverClass)
    {
        case 'A':
            std::cout << "Can drive motorcycles\n";
            break;
        case 'B':
            std::cout << "Can drive cars\n";
            break;
        case 'C':
            std::cout << "Can drive trucks\n";
            break;
    }

    return 0;
}
```

---

## 5. Ternary Operator

### Basic Syntax:

```cpp
#include <iostream>

int main()
{
    int age = 20;

    // if-else way
    std::string status1;
    if (age >= 18)
        status1 = "Adult";
    else
        status1 = "Minor";

    // Ternary way (compact)
    std::string status2 = (age >= 18) ? "Adult" : "Minor";

    std::cout << "Status: " << status2 << "\n";

    return 0;
}
```

### Practical Examples:

```cpp
#include <iostream>

int main()
{
    // Example 1: Max of two numbers
    int a = 10, b = 20;
    int max = (a > b) ? a : b;
    std::cout << "Max: " << max << "\n";

    // Example 2: Absolute value
    int num = -5;
    int absolute = (num >= 0) ? num : -num;
    std::cout << "Absolute value: " << absolute << "\n";

    // Example 3: Even/Odd
    int number = 7;
    std::string parity = (number % 2 == 0) ? "Even" : "Odd";
    std::cout << number << " is " << parity << "\n";

    // Example 4: Pass/Fail
    double score = 75.0;
    std::string result = (score >= 60.0) ? "Pass" : "Fail";
    std::cout << "Result: " << result << "\n";

    return 0;
}
```

### Ternary in Output:

```cpp
#include <iostream>

int main()
{
    int age = 25;
    double balance = 1500.50;
    bool isWeekend = true;

    // Direct use in cout
    std::cout << "You are " << ((age >= 18) ? "an adult" : "a minor") << "\n";
    std::cout << "Account status: " << ((balance > 0) ? "Active" : "Inactive") << "\n";
    std::cout << "Today is a " << (isWeekend ? "weekend" : "weekday") << "\n";

    return 0;
}
```

---

## 6. Nested Ternary Operators

### Basic Nesting:

```cpp
#include <iostream>

int main()
{
    int score = 85;

    // Nested ternary for grade assignment
    std::string grade = (score >= 90) ? "A" :
                        (score >= 80) ? "B" :
                        (score >= 70) ? "C" :
                        (score >= 60) ? "D" : "F";

    std::cout << "Score: " << score << "\n";
    std::cout << "Grade: " << grade << "\n";

    return 0;
}
```

### Complex Example:

```cpp
#include <iostream>

int main()
{
    int temperature = 25;

    // Nested ternary for weather advice
    std::string advice = (temperature < 0)   ? "Freezing - Heavy coat" :
                         (temperature < 10)  ? "Cold - Jacket needed" :
                         (temperature < 20)  ? "Cool - Light jacket" :
                         (temperature < 30)  ? "Comfortable - Perfect" :
                                               "Hot - Stay hydrated";

    std::cout << "Temperature: " << temperature << "°C\n";
    std::cout << "Advice: " << advice << "\n";

    return 0;
}
```

### Warning: Readability:

```cpp
#include <iostream>

int main()
{
    int x = 5, y = 10, z = 3;

    // ❌ Too complex - hard to read!
    int result = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);

    // ✅ Better - use if-else for complex logic
    int max;
    if (x > y && x > z)
        max = x;
    else if (y > z)
        max = y;
    else
        max = z;

    std::cout << "Max: " << max << "\n";

    return 0;
}
```

---

## 7. Combining Switch and Ternary

### Menu System:

```cpp
#include <iostream>

int main()
{
    int choice;
    bool isPremium = true;

    std::cout << "===== MENU =====\n";
    std::cout << "1. Basic Feature\n";
    std::cout << "2. Standard Feature\n";
    std::cout << "3. Premium Feature\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            std::cout << "Basic Feature activated\n";
            break;

        case 2:
            std::cout << "Standard Feature activated\n";
            break;

        case 3:
            // Using ternary inside switch
            std::cout << (isPremium ? "Premium Feature activated"
                                    : "Premium subscription required!") << "\n";
            break;

        default:
            std::cout << "Invalid choice\n";
    }

    return 0;
}
```

### Price Calculator:

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    char memberType;
    double basePrice = 100.0;
    double finalPrice;

    std::cout << "Enter member type (B=Bronze, S=Silver, G=Gold): ";
    std::cin >> memberType;

    switch (memberType)
    {
        case 'B':
        case 'b':
            finalPrice = basePrice * 0.95;  // 5% discount
            std::cout << "Bronze Member - 5% discount\n";
            break;

        case 'S':
        case 's':
            finalPrice = basePrice * 0.90;  // 10% discount
            std::cout << "Silver Member - 10% discount\n";
            break;

        case 'G':
        case 'g':
            finalPrice = basePrice * 0.85;  // 15% discount
            std::cout << "Gold Member - 15% discount\n";
            break;

        default:
            finalPrice = basePrice;
            std::cout << "Non-Member - No discount\n";
    }

    // Using ternary for shipping
    double shipping = (finalPrice > 50) ? 0.0 : 5.99;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nBase Price: $" << basePrice << "\n";
    std::cout << "Final Price: $" << finalPrice << "\n";
    std::cout << "Shipping: " << ((shipping == 0) ? "FREE" : "$5.99") << "\n";
    std::cout << "Total: $" << (finalPrice + shipping) << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Season Determiner

```cpp
#include <iostream>

int main()
{
    int month;

    std::cout << "Enter month number (1-12): ";
    std::cin >> month;

    std::string season;

    switch (month)
    {
        case 12:
        case 1:
        case 2:
            season = "Winter";
            break;

        case 3:
        case 4:
        case 5:
            season = "Spring";
            break;

        case 6:
        case 7:
        case 8:
            season = "Summer";
            break;

        case 9:
        case 10:
        case 11:
            season = "Fall";
            break;

        default:
            season = "Invalid month";
    }

    std::cout << "Season: " << season << "\n";

    return 0;
}
```

### Exercise 2: Vowel or Consonant

```cpp
#include <iostream>

int main()
{
    char letter;

    std::cout << "Enter a letter: ";
    std::cin >> letter;

    // Convert to lowercase
    letter = tolower(letter);

    switch (letter)
    {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            std::cout << letter << " is a VOWEL\n";
            break;

        default:
            if ((letter >= 'a' && letter <= 'z'))
                std::cout << letter << " is a CONSONANT\n";
            else
                std::cout << "Not a valid letter\n";
    }

    return 0;
}
```

### Exercise 3: Traffic Light Simulator

```cpp
#include <iostream>

int main()
{
    char light;

    std::cout << "===== TRAFFIC LIGHT =====\n";
    std::cout << "Enter light color (R/Y/G): ";
    std::cin >> light;

    light = toupper(light);

    switch (light)
    {
        case 'R':
            std::cout << "\n🔴 RED Light\n";
            std::cout << "Action: STOP\n";
            std::cout << "Wait for green light\n";
            break;

        case 'Y':
            std::cout << "\n🟡 YELLOW Light\n";
            std::cout << "Action: SLOW DOWN\n";
            std::cout << "Prepare to stop\n";
            break;

        case 'G':
            std::cout << "\n🟢 GREEN Light\n";
            std::cout << "Action: GO\n";
            std::cout << "Proceed with caution\n";
            break;

        default:
            std::cout << "\n✗ Invalid light color!\n";
    }

    return 0;
}
```

### Exercise 4: Simple ATM

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    double balance = 1000.00;
    int choice;
    double amount;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "===== ATM MACHINE =====\n";
    std::cout << "1. Check Balance\n";
    std::cout << "2. Deposit\n";
    std::cout << "3. Withdraw\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            std::cout << "\nCurrent Balance: $" << balance << "\n";
            break;

        case 2:
            std::cout << "Enter deposit amount: $";
            std::cin >> amount;
            if (amount > 0)
            {
                balance += amount;
                std::cout << "Deposit successful!\n";
                std::cout << "New Balance: $" << balance << "\n";
            }
            else
            {
                std::cout << "Invalid amount!\n";
            }
            break;

        case 3:
            std::cout << "Enter withdrawal amount: $";
            std::cin >> amount;
            if (amount > 0 && amount <= balance)
            {
                balance -= amount;
                std::cout << "Withdrawal successful!\n";
                std::cout << "New Balance: $" << balance << "\n";
            }
            else
            {
                std::cout << (amount > balance ? "Insufficient funds!\n"
                                                : "Invalid amount!\n");
            }
            break;

        case 4:
            std::cout << "Thank you for using our ATM!\n";
            break;

        default:
            std::cout << "Invalid choice!\n";
    }

    return 0;
}
```

### Exercise 5: Roman Numeral Converter (1-10)

```cpp
#include <iostream>

int main()
{
    int number;

    std::cout << "Enter a number (1-10): ";
    std::cin >> number;

    std::cout << "Roman Numeral: ";

    switch (number)
    {
        case 1:
            std::cout << "I\n";
            break;
        case 2:
            std::cout << "II\n";
            break;
        case 3:
            std::cout << "III\n";
            break;
        case 4:
            std::cout << "IV\n";
            break;
        case 5:
            std::cout << "V\n";
            break;
        case 6:
            std::cout << "VI\n";
            break;
        case 7:
            std::cout << "VII\n";
            break;
        case 8:
            std::cout << "VIII\n";
            break;
        case 9:
            std::cout << "IX\n";
            break;
        case 10:
            std::cout << "X\n";
            break;
        default:
            std::cout << "Invalid input (must be 1-10)\n";
    }

    return 0;
}
```

---

## Practice Project: Restaurant Ordering System

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    int category, item;
    double price = 0.0;
    std::string orderName;
    char size;
    bool validOrder = true;

    std::cout << "===== RESTAURANT ORDERING SYSTEM =====\n\n";
    std::cout << "MENU CATEGORIES:\n";
    std::cout << "1. Burgers\n";
    std::cout << "2. Pizza\n";
    std::cout << "3. Drinks\n";
    std::cout << "4. Desserts\n";
    std::cout << "\nSelect category: ";
    std::cin >> category;

    switch (category)
    {
        case 1:  // Burgers
            std::cout << "\n--- BURGERS ---\n";
            std::cout << "1. Classic Burger - $5.99\n";
            std::cout << "2. Cheese Burger - $6.99\n";
            std::cout << "3. Deluxe Burger - $8.99\n";
            std::cout << "Select item: ";
            std::cin >> item;

            switch (item)
            {
                case 1:
                    orderName = "Classic Burger";
                    price = 5.99;
                    break;
                case 2:
                    orderName = "Cheese Burger";
                    price = 6.99;
                    break;
                case 3:
                    orderName = "Deluxe Burger";
                    price = 8.99;
                    break;
                default:
                    validOrder = false;
            }
            break;

        case 2:  // Pizza
            std::cout << "\n--- PIZZA ---\n";
            std::cout << "1. Margherita\n";
            std::cout << "2. Pepperoni\n";
            std::cout << "3. Veggie\n";
            std::cout << "Select item: ";
            std::cin >> item;

            std::cout << "Size (S/M/L): ";
            std::cin >> size;
            size = toupper(size);

            double basePrice;
            switch (item)
            {
                case 1:
                    orderName = "Margherita Pizza";
                    basePrice = 8.99;
                    break;
                case 2:
                    orderName = "Pepperoni Pizza";
                    basePrice = 10.99;
                    break;
                case 3:
                    orderName = "Veggie Pizza";
                    basePrice = 9.99;
                    break;
                default:
                    validOrder = false;
                    basePrice = 0;
            }

            // Size multiplier
            switch (size)
            {
                case 'S':
                    price = basePrice * 0.8;
                    orderName += " (Small)";
                    break;
                case 'M':
                    price = basePrice;
                    orderName += " (Medium)";
                    break;
                case 'L':
                    price = basePrice * 1.5;
                    orderName += " (Large)";
                    break;
                default:
                    validOrder = false;
            }
            break;

        case 3:  // Drinks
            std::cout << "\n--- DRINKS ---\n";
            std::cout << "1. Coke - $1.99\n";
            std::cout << "2. Sprite - $1.99\n";
            std::cout << "3. Water - $0.99\n";
            std::cout << "4. Coffee - $2.99\n";
            std::cout << "Select item: ";
            std::cin >> item;

            switch (item)
            {
                case 1:
                    orderName = "Coke";
                    price = 1.99;
                    break;
                case 2:
                    orderName = "Sprite";
                    price = 1.99;
                    break;
                case 3:
                    orderName = "Water";
                    price = 0.99;
                    break;
                case 4:
                    orderName = "Coffee";
                    price = 2.99;
                    break;
                default:
                    validOrder = false;
            }
            break;

        case 4:  // Desserts
            std::cout << "\n--- DESSERTS ---\n";
            std::cout << "1. Ice Cream - $3.99\n";
            std::cout << "2. Cake - $4.99\n";
            std::cout << "3. Brownie - $3.49\n";
            std::cout << "Select item: ";
            std::cin >> item;

            switch (item)
            {
                case 1:
                    orderName = "Ice Cream";
                    price = 3.99;
                    break;
                case 2:
                    orderName = "Cake";
                    price = 4.99;
                    break;
                case 3:
                    orderName = "Brownie";
                    price = 3.49;
                    break;
                default:
                    validOrder = false;
            }
            break;

        default:
            std::cout << "\nInvalid category!\n";
            validOrder = false;
    }

    if (validOrder)
    {
        double tax = price * 0.08;  // 8% tax
        double total = price + tax;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n===== ORDER SUMMARY =====\n";
        std::cout << "Item: " << orderName << "\n";
        std::cout << "Price: $" << price << "\n";
        std::cout << "Tax (8%): $" << tax << "\n";
        std::cout << "Total: $" << total << "\n";
        std::cout << "\nThank you for your order!\n";
    }
    else
    {
        std::cout << "\nOrder cancelled - Invalid selection\n";
    }

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Forgetting Break
```cpp
❌ switch (x) {
    case 1:
        cout << "One\n";  // Falls through!
    case 2:
        cout << "Two\n";
}

✅ switch (x) {
    case 1:
        cout << "One\n";
        break;
    case 2:
        cout << "Two\n";
        break;
}
```

### Mistake 2: Using Non-Constant Values
```cpp
int x = 5;
❌ switch (y) {
    case x:  // Error: x is not a constant!
        break;
}

✅ switch (y) {
    case 5:  // Constant value
        break;
}
```

### Mistake 3: Complex Ternary
```cpp
❌ int x = a ? b ? c : d : e ? f : g;  // Unreadable!

✅ Use if-else for complex logic
```

### Mistake 4: Wrong Ternary Syntax
```cpp
❌ string s = (x > 0) ? "positive";  // Incomplete!

✅ string s = (x > 0) ? "positive" : "not positive";
```

---

## Key Takeaways

1. ✅ Use `switch` for checking one variable against multiple constant values
2. ✅ Always use `break` unless intentional fall-through
3. ✅ Include a `default` case for invalid inputs
4. ✅ Ternary operator `?:` is great for simple if-else
5. ✅ Switch only works with integers, chars, and enums
6. ✅ Nested ternary can become unreadable quickly
7. ✅ Use if-else for ranges and complex conditions
8. ✅ Combine switch and ternary for powerful logic

---

## Summary Checklist

Before moving to Lesson 6, ensure you can:
- [ ] Write switch statements correctly
- [ ] Use break statements properly
- [ ] Implement fall-through when needed
- [ ] Choose between switch and if-else appropriately
- [ ] Use ternary operator for simple conditions
- [ ] Nest ternary operators (with caution)
- [ ] Handle default cases in switch
- [ ] Combine switch and ternary effectively

---

## Next Lesson Preview

In **Lesson 6**, we'll learn about:
- While loops
- Do-while loops
- Loop conditions and termination
- Infinite loops and how to avoid them
- Input validation with loops

**Excellent work mastering decision-making!** 🎯
