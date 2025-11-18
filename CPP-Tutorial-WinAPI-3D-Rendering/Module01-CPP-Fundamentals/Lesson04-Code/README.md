# Lesson 04: Functions - Code Examples

This directory contains 15 complete, executable C++ programs demonstrating functions and their various applications.

## Programs List

### 01_basic_functions.cpp
**Description:** Basic function declaration, definition, and calling.
**Concepts:** Function prototypes, void functions, return values, parameters
**Compilation:** `g++ 01_basic_functions.cpp -o 01_basic_functions`
**Execution:** `./01_basic_functions`

### 02_parameter_passing.cpp
**Description:** Pass by value, pass by reference, and pass by const reference.
**Concepts:** Parameter passing mechanisms, references, arrays as parameters
**Compilation:** `g++ 02_parameter_passing.cpp -o 02_parameter_passing`
**Execution:** `./02_parameter_passing`

### 03_function_overloading.cpp
**Description:** Function overloading with different parameter types and counts.
**Concepts:** Function overloading, compile-time polymorphism
**Compilation:** `g++ 03_function_overloading.cpp -o 03_function_overloading`
**Execution:** `./03_function_overloading`

### 04_recursive_functions.cpp
**Description:** Recursive functions for factorial, Fibonacci, GCD, etc.
**Concepts:** Recursion, base case, recursive case, binary search
**Compilation:** `g++ 04_recursive_functions.cpp -o 04_recursive_functions`
**Execution:** `./04_recursive_functions`

### 05_math_library.cpp
**Description:** Custom math library with various mathematical functions.
**Concepts:** Mathematical algorithms, prime checking, GCD, LCM, combinations, permutations
**Compilation:** `g++ 05_math_library.cpp -o 05_math_library`
**Execution:** `./05_math_library`

### 06_string_utilities.cpp
**Description:** String manipulation and utility functions.
**Concepts:** String operations, character checking, palindrome, word count
**Compilation:** `g++ 06_string_utilities.cpp -o 06_string_utilities`
**Execution:** `./06_string_utilities`

### 07_default_arguments.cpp
**Description:** Functions with default parameter values.
**Concepts:** Default arguments, optional parameters
**Compilation:** `g++ 07_default_arguments.cpp -o 07_default_arguments`
**Execution:** `./07_default_arguments`

### 08_inline_functions.cpp
**Description:** Inline functions for performance optimization.
**Concepts:** Inline keyword, function call overhead, optimization
**Compilation:** `g++ 08_inline_functions.cpp -o 08_inline_functions -O2`
**Execution:** `./08_inline_functions`

### 09_array_functions.cpp
**Description:** Functions for array manipulation and operations.
**Concepts:** Array operations, max/min, sum, average, search, reverse
**Compilation:** `g++ 09_array_functions.cpp -o 09_array_functions`
**Execution:** `./09_array_functions`

### 10_validation_functions.cpp
**Description:** Input validation and checking functions.
**Concepts:** Validation, email check, password strength, range checking
**Compilation:** `g++ 10_validation_functions.cpp -o 10_validation_functions`
**Execution:** `./10_validation_functions`

### 11_conversion_functions.cpp
**Description:** Various conversion and formatting functions.
**Concepts:** Temperature conversion, number system conversion, string conversion
**Compilation:** `g++ 11_conversion_functions.cpp -o 11_conversion_functions`
**Execution:** `./11_conversion_functions`

### 12_geometry_functions.cpp
**Description:** Geometric calculations for areas, perimeters, and volumes.
**Concepts:** Circle, rectangle, triangle, sphere, cube, cylinder, cone calculations
**Compilation:** `g++ 12_geometry_functions.cpp -o 12_geometry_functions`
**Execution:** `./12_geometry_functions`

### 13_sorting_functions.cpp
**Description:** Implementation of sorting algorithms (bubble, selection, insertion).
**Concepts:** Sorting algorithms, algorithm complexity, performance comparison
**Compilation:** `g++ 13_sorting_functions.cpp -o 13_sorting_functions`
**Execution:** `./13_sorting_functions`

### 14_searching_functions.cpp
**Description:** Linear and binary search implementations.
**Concepts:** Search algorithms, iterative vs recursive, algorithm efficiency
**Compilation:** `g++ 14_searching_functions.cpp -o 14_searching_functions`
**Execution:** `./14_searching_functions`

### 15_calculator_app.cpp
**Description:** Complete calculator application using functions.
**Concepts:** Menu-driven program, modular design, function composition
**Compilation:** `g++ 15_calculator_app.cpp -o 15_calculator_app`
**Execution:** `./15_calculator_app`

## Compilation Instructions

### Compile All Programs
```bash
g++ 01_basic_functions.cpp -o 01_basic_functions
g++ 02_parameter_passing.cpp -o 02_parameter_passing
g++ 03_function_overloading.cpp -o 03_function_overloading
g++ 04_recursive_functions.cpp -o 04_recursive_functions
g++ 05_math_library.cpp -o 05_math_library
g++ 06_string_utilities.cpp -o 06_string_utilities
g++ 07_default_arguments.cpp -o 07_default_arguments
g++ 08_inline_functions.cpp -o 08_inline_functions -O2
g++ 09_array_functions.cpp -o 09_array_functions
g++ 10_validation_functions.cpp -o 10_validation_functions
g++ 11_conversion_functions.cpp -o 11_conversion_functions
g++ 12_geometry_functions.cpp -o 12_geometry_functions
g++ 13_sorting_functions.cpp -o 13_sorting_functions
g++ 14_searching_functions.cpp -o 14_searching_functions
g++ 15_calculator_app.cpp -o 15_calculator_app
```

### Compile All at Once (Bash)
```bash
for file in *.cpp; do g++ "$file" -o "${file%.cpp}"; done
```

## Learning Path

1. **Start with basics**: 01_basic_functions.cpp
2. **Understand parameters**: 02_parameter_passing.cpp
3. **Learn overloading**: 03_function_overloading.cpp
4. **Master recursion**: 04_recursive_functions.cpp
5. **Build libraries**: 05_math_library.cpp, 06_string_utilities.cpp
6. **Advanced features**: 07_default_arguments.cpp, 08_inline_functions.cpp
7. **Array operations**: 09_array_functions.cpp
8. **Practical applications**: 10-12 (validation, conversion, geometry)
9. **Algorithms**: 13_sorting_functions.cpp, 14_searching_functions.cpp
10. **Complete application**: 15_calculator_app.cpp

## Key Concepts Covered

- Function declaration and definition
- Function prototypes
- Parameter passing (by value, by reference, by const reference)
- Return values
- Function overloading
- Recursive functions
- Default arguments
- Inline functions
- Array and string manipulation
- Mathematical operations
- Validation functions
- Conversion functions
- Geometric calculations
- Sorting algorithms (Bubble, Selection, Insertion)
- Searching algorithms (Linear, Binary)
- Modular programming
- Code reusability

## Function Design Best Practices

1. **Single Responsibility**: Each function should do one thing well
2. **Meaningful Names**: Use descriptive function names
3. **Parameter Validation**: Check inputs for validity
4. **Return Values**: Return meaningful results
5. **Documentation**: Comment complex functions
6. **Modularity**: Break complex problems into smaller functions
7. **Reusability**: Design functions to be reusable

## Notes

- All programs demonstrate practical applications of functions
- Code is well-commented for learning purposes
- Examples progress from basic to advanced
- Emphasis on real-world use cases
- Performance considerations included where relevant
