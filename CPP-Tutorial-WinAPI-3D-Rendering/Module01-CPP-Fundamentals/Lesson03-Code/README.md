# Lesson 03: Control Flow - Code Examples

This directory contains 15 complete, executable C++ programs demonstrating control flow statements.

## Programs List

### 01_if_else_basic.cpp
**Description:** Demonstrates if, if-else, if-else-if, and nested if statements.
**Concepts:** Conditional statements, logical operators, nested conditions, leap year calculation
**Compilation:** `g++ 01_if_else_basic.cpp -o 01_if_else_basic`
**Execution:** `./01_if_else_basic`

### 02_switch_statement.cpp
**Description:** Demonstrates switch-case statements with various applications.
**Concepts:** Switch statement, fall-through, multiple cases, calculator, day/month lookup
**Compilation:** `g++ 02_switch_statement.cpp -o 02_switch_statement`
**Execution:** `./02_switch_statement`

### 03_while_loop.cpp
**Description:** Demonstrates while and do-while loops with practical examples.
**Concepts:** While loop, do-while loop, input validation, factorial, digit sum, number reversal
**Compilation:** `g++ 03_while_loop.cpp -o 03_while_loop`
**Execution:** `./03_while_loop`

### 04_for_loop.cpp
**Description:** Demonstrates for loops with various applications.
**Concepts:** For loops, iteration, multiplication table, Fibonacci series, prime checking, power calculation
**Compilation:** `g++ 04_for_loop.cpp -o 04_for_loop`
**Execution:** `./04_for_loop`

### 05_nested_loops.cpp
**Description:** Demonstrates nested loops for patterns and tables.
**Concepts:** Nested loops, pattern printing, multiplication table, Floyd's triangle, prime numbers
**Compilation:** `g++ 05_nested_loops.cpp -o 05_nested_loops`
**Execution:** `./05_nested_loops`

### 06_pattern_printing.cpp
**Description:** Prints advanced patterns including pyramid, diamond, and butterfly.
**Concepts:** Complex nested loops, pattern logic, Pascal's triangle, alphabet patterns
**Compilation:** `g++ 06_pattern_printing.cpp -o 06_pattern_printing`
**Execution:** `./06_pattern_printing`

### 07_guessing_game.cpp
**Description:** Interactive number guessing game with hints and scoring.
**Concepts:** Random numbers, game loop, user interaction, conditional hints, replay functionality
**Compilation:** `g++ 07_guessing_game.cpp -o 07_guessing_game`
**Execution:** `./07_guessing_game`

### 08_menu_system.cpp
**Description:** Interactive calculator with menu-driven interface.
**Concepts:** Menu systems, switch in loops, multiple operations, user interface design
**Compilation:** `g++ 08_menu_system.cpp -o 08_menu_system`
**Execution:** `./08_menu_system`

### 09_break_continue.cpp
**Description:** Demonstrates break and continue statements in loops.
**Concepts:** Break statement, continue statement, loop control, early termination, skipping iterations
**Compilation:** `g++ 09_break_continue.cpp -o 09_break_continue`
**Execution:** `./09_break_continue`

### 10_logical_operators.cpp
**Description:** Comprehensive demonstration of AND, OR, and NOT logical operators.
**Concepts:** Logical operators (&&, ||, !), combined conditions, truth tables, login systems
**Compilation:** `g++ 10_logical_operators.cpp -o 10_logical_operators`
**Execution:** `./10_logical_operators`

### 11_comparison_operators.cpp
**Description:** Demonstrates all comparison operators with practical examples.
**Concepts:** Comparison operators (==, !=, >, <, >=, <=), min/max finding, three-number comparison
**Compilation:** `g++ 11_comparison_operators.cpp -o 11_comparison_operators`
**Execution:** `./11_comparison_operators`

### 12_conditional_operator.cpp
**Description:** Demonstrates the ternary conditional operator (? :).
**Concepts:** Ternary operator, nested ternary, inline conditions, compact syntax
**Compilation:** `g++ 12_conditional_operator.cpp -o 12_conditional_operator`
**Execution:** `./12_conditional_operator`

### 13_prime_numbers.cpp
**Description:** Comprehensive prime number operations (checking, listing, counting, summing).
**Concepts:** Prime number algorithms, optimization, range operations, counting
**Compilation:** `g++ 13_prime_numbers.cpp -o 13_prime_numbers`
**Execution:** `./13_prime_numbers`

### 14_factorial_fibonacci.cpp
**Description:** Factorial calculations and Fibonacci sequence generation.
**Concepts:** Factorial, Fibonacci sequence, golden ratio, sequence checking
**Compilation:** `g++ 14_factorial_fibonacci.cpp -o 14_factorial_fibonacci`
**Execution:** `./14_factorial_fibonacci`

### 15_number_patterns.cpp
**Description:** Various number patterns and mathematical sequences.
**Concepts:** Number patterns, Floyd's triangle, palindrome pyramid, sequences (arithmetic, geometric, triangular), Collatz sequence
**Compilation:** `g++ 15_number_patterns.cpp -o 15_number_patterns`
**Execution:** `./15_number_patterns`

## Compilation Instructions

### Compile All Programs
```bash
g++ 01_if_else_basic.cpp -o 01_if_else_basic
g++ 02_switch_statement.cpp -o 02_switch_statement
g++ 03_while_loop.cpp -o 03_while_loop
g++ 04_for_loop.cpp -o 04_for_loop
g++ 05_nested_loops.cpp -o 05_nested_loops
g++ 06_pattern_printing.cpp -o 06_pattern_printing
g++ 07_guessing_game.cpp -o 07_guessing_game
g++ 08_menu_system.cpp -o 08_menu_system
g++ 09_break_continue.cpp -o 09_break_continue
g++ 10_logical_operators.cpp -o 10_logical_operators
g++ 11_comparison_operators.cpp -o 11_comparison_operators
g++ 12_conditional_operator.cpp -o 12_conditional_operator
g++ 13_prime_numbers.cpp -o 13_prime_numbers
g++ 14_factorial_fibonacci.cpp -o 14_factorial_fibonacci
g++ 15_number_patterns.cpp -o 15_number_patterns
```

### Compile All at Once (Bash)
```bash
for file in *.cpp; do g++ "$file" -o "${file%.cpp}"; done
```

## Learning Path

1. Start with **01_if_else_basic.cpp** to understand conditional statements
2. Learn switch statements with **02_switch_statement.cpp**
3. Master loops: **03_while_loop.cpp** and **04_for_loop.cpp**
4. Explore nested loops: **05_nested_loops.cpp** and **06_pattern_printing.cpp**
5. Practice with interactive programs: **07_guessing_game.cpp** and **08_menu_system.cpp**
6. Understand loop control: **09_break_continue.cpp**
7. Master operators: **10_logical_operators.cpp**, **11_comparison_operators.cpp**, **12_conditional_operator.cpp**
8. Apply concepts: **13_prime_numbers.cpp**, **14_factorial_fibonacci.cpp**, **15_number_patterns.cpp**

## Key Concepts Covered

- If, if-else, if-else-if statements
- Switch-case statements
- While and do-while loops
- For loops
- Nested loops
- Break and continue statements
- Logical operators (&&, ||, !)
- Comparison operators (==, !=, >, <, >=, <=)
- Ternary conditional operator (? :)
- Pattern printing techniques
- Number sequences and algorithms
- Interactive programs
- Menu-driven applications

## Notes

- All programs are interactive and provide clear output
- Pattern programs demonstrate the power of nested loops
- Game and menu programs show practical application of control flow
- Mathematical programs (prime, factorial, Fibonacci) demonstrate algorithmic thinking
- All programs include input validation where appropriate
