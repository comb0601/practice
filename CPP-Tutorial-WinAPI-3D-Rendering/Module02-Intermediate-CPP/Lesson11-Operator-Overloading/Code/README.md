# Lesson 11: Operator Overloading - Code Examples

This directory contains 15 complete C++ programs demonstrating operator overloading concepts.

## Files

1. **01_basic_operator_overloading.cpp** - Arithmetic operators (+, -, *, /) for Complex numbers
2. **02_comparison_operators.cpp** - Comparison operators (==, !=, <, >, <=, >=) for Point class
3. **03_stream_operators.cpp** - Stream operators (<<, >>) for I/O operations
4. **04_increment_decrement.cpp** - Prefix and postfix increment/decrement operators
5. **05_subscript_operator.cpp** - Subscript operator [] for array-like access
6. **06_assignment_operator.cpp** - Assignment operator with deep copy and self-assignment check
7. **07_function_call_operator.cpp** - Function call operator () for functors
8. **08_arrow_operator.cpp** - Arrow operator (->) for smart pointers
9. **09_type_conversion.cpp** - Type conversion operators
10. **10_compound_assignment.cpp** - Compound assignment operators (+=, -=, *=, /=)
11. **11_unary_operators.cpp** - Unary operators (+, -, !, ~)
12. **12_comma_operator.cpp** - Comma operator overloading
13. **13_new_delete_operators.cpp** - Memory allocation operators (new, delete)
14. **14_complete_class_example.cpp** - Complete Matrix class with all operators
15. **15_best_practices.cpp** - Best practices and guidelines for operator overloading

## Compilation

Each file can be compiled independently using:

```bash
g++ -std=c++17 filename.cpp -o output_name
```

For example:
```bash
g++ -std=c++17 01_basic_operator_overloading.cpp -o basic_op
./basic_op
```

## Topics Covered

- Arithmetic operator overloading
- Comparison operator overloading
- Stream I/O operators
- Increment/Decrement operators
- Subscript and function call operators
- Assignment operator (Rule of Three)
- Conversion operators
- Unary operators
- Memory management operators
- Best practices and pitfalls
