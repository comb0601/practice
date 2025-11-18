# Lesson 02: Variables & Data Types - Code Examples

This directory contains 15 complete, executable C++ programs demonstrating variables and data types.

## Programs List

### 01_basic_variables.cpp
**Description:** Demonstrates declaration and initialization of all basic data types (int, float, double, char, bool, string).
**Concepts:** Variable declaration, initialization, data types
**Compilation:** `g++ 01_basic_variables.cpp -o 01_basic_variables`
**Execution:** `./01_basic_variables`

### 02_simple_calculator.cpp
**Description:** Performs basic arithmetic operations (add, subtract, multiply, divide, modulo) on two numbers.
**Concepts:** Arithmetic operators, user input, conditional logic
**Compilation:** `g++ 02_simple_calculator.cpp -o 02_simple_calculator`
**Execution:** `./02_simple_calculator`

### 03_type_conversion.cpp
**Description:** Demonstrates implicit and explicit type conversion, casting, mixed arithmetic.
**Concepts:** Type casting, static_cast, implicit conversion, data loss
**Compilation:** `g++ 03_type_conversion.cpp -o 03_type_conversion`
**Execution:** `./03_type_conversion`

### 04_temperature_converter.cpp
**Description:** Converts between Celsius, Fahrenheit, and Kelvin temperature scales.
**Concepts:** Mathematical formulas, user choice, formatted output
**Compilation:** `g++ 04_temperature_converter.cpp -o 04_temperature_converter`
**Execution:** `./04_temperature_converter`

### 05_bmi_calculator.cpp
**Description:** Calculates Body Mass Index (BMI) and provides health category assessment.
**Concepts:** Mathematical calculations, conditional logic, health metrics
**Compilation:** `g++ 05_bmi_calculator.cpp -o 05_bmi_calculator`
**Execution:** `./05_bmi_calculator`

### 06_currency_converter.cpp
**Description:** Converts between USD, EUR, GBP, and JPY currencies.
**Concepts:** Exchange rates, multi-currency conversion, constants
**Compilation:** `g++ 06_currency_converter.cpp -o 06_currency_converter`
**Execution:** `./06_currency_converter`

### 07_input_output_demo.cpp
**Description:** Demonstrates various input/output operations and formatting techniques.
**Concepts:** I/O manipulators, setw, setprecision, setfill, formatting
**Compilation:** `g++ 07_input_output_demo.cpp -o 07_input_output_demo`
**Execution:** `./07_input_output_demo`

### 08_constants_demo.cpp
**Description:** Shows different ways to define and use constants (const, #define, constexpr).
**Concepts:** Constants, const pointers, compile-time constants
**Compilation:** `g++ 08_constants_demo.cpp -o 08_constants_demo`
**Execution:** `./08_constants_demo`

### 09_arithmetic_operations.cpp
**Description:** Comprehensive demonstration of all arithmetic operators and math functions.
**Concepts:** Operators, increment/decrement, compound assignment, math library
**Compilation:** `g++ 09_arithmetic_operations.cpp -o 09_arithmetic_operations`
**Execution:** `./09_arithmetic_operations`

### 10_data_type_sizes.cpp
**Description:** Displays size and range of various data types using sizeof and limits.
**Concepts:** sizeof operator, numeric_limits, memory allocation
**Compilation:** `g++ 10_data_type_sizes.cpp -o 10_data_type_sizes`
**Execution:** `./10_data_type_sizes`

### 11_circle_calculator.cpp
**Description:** Calculates area, circumference, and diameter of a circle.
**Concepts:** Mathematical formulas, PI constant, geometric calculations
**Compilation:** `g++ 11_circle_calculator.cpp -o 11_circle_calculator`
**Execution:** `./11_circle_calculator`

### 12_time_converter.cpp
**Description:** Converts seconds to days, hours, minutes, and seconds format.
**Concepts:** Integer division, modulo operator, time calculations
**Compilation:** `g++ 12_time_converter.cpp -o 12_time_converter`
**Execution:** `./12_time_converter`

### 13_variable_scope.cpp
**Description:** Demonstrates local, global, block, and static variable scope.
**Concepts:** Scope, variable lifetime, shadowing, static variables
**Compilation:** `g++ 13_variable_scope.cpp -o 13_variable_scope`
**Execution:** `./13_variable_scope`

### 14_compound_interest.cpp
**Description:** Calculates compound interest for investments with different compounding frequencies.
**Concepts:** Financial calculations, pow function, compound interest formula
**Compilation:** `g++ 14_compound_interest.cpp -o 14_compound_interest`
**Execution:** `./14_compound_interest`

### 15_rectangle_calculator.cpp
**Description:** Calculates area, perimeter, and diagonal of a rectangle.
**Concepts:** Geometric calculations, Pythagorean theorem, square detection
**Compilation:** `g++ 15_rectangle_calculator.cpp -o 15_rectangle_calculator`
**Execution:** `./15_rectangle_calculator`

## Compilation Instructions

### Compile All Programs
```bash
g++ 01_basic_variables.cpp -o 01_basic_variables
g++ 02_simple_calculator.cpp -o 02_simple_calculator
g++ 03_type_conversion.cpp -o 03_type_conversion
g++ 04_temperature_converter.cpp -o 04_temperature_converter
g++ 05_bmi_calculator.cpp -o 05_bmi_calculator
g++ 06_currency_converter.cpp -o 06_currency_converter
g++ 07_input_output_demo.cpp -o 07_input_output_demo
g++ 08_constants_demo.cpp -o 08_constants_demo
g++ 09_arithmetic_operations.cpp -o 09_arithmetic_operations
g++ 10_data_type_sizes.cpp -o 10_data_type_sizes
g++ 11_circle_calculator.cpp -o 11_circle_calculator
g++ 12_time_converter.cpp -o 12_time_converter
g++ 13_variable_scope.cpp -o 13_variable_scope
g++ 14_compound_interest.cpp -o 14_compound_interest
g++ 15_rectangle_calculator.cpp -o 15_rectangle_calculator
```

### Compile with C++11 Standard (Recommended)
```bash
g++ -std=c++11 <filename>.cpp -o <output_name>
```

### Compile All at Once (Bash)
```bash
for file in *.cpp; do g++ "$file" -o "${file%.cpp}"; done
```

## Learning Path

1. Start with **01_basic_variables.cpp** to understand data types
2. Learn operations with **02_simple_calculator.cpp** and **09_arithmetic_operations.cpp**
3. Understand conversions with **03_type_conversion.cpp**
4. Practice with real-world examples (04-06, 11-12, 14-15)
5. Master I/O with **07_input_output_demo.cpp**
6. Learn constants with **08_constants_demo.cpp**
7. Understand scope with **13_variable_scope.cpp**
8. Explore sizes and limits with **10_data_type_sizes.cpp**

## Key Concepts Covered

- Primitive data types (int, float, double, char, bool)
- String type
- Variable declaration and initialization
- Type conversion and casting
- Arithmetic operators
- Input/output operations
- Constants and const-correctness
- Variable scope and lifetime
- Mathematical operations
- Practical applications

## Notes

- All programs include error checking where appropriate
- Programs use meaningful variable names and comments
- Each program is self-contained and can be compiled independently
- Output is formatted for readability
- Real-world examples demonstrate practical applications
