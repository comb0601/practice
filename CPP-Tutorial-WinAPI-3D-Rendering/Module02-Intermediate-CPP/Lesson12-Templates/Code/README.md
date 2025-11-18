# Lesson 12: Templates - Code Examples

This directory contains 15 executable C++ programs demonstrating template concepts.

## Compilation

Compile any example with:
```bash
g++ -std=c++11 filename.cpp -o filename
./filename
```

For examples using C++17 features:
```bash
g++ -std=c++17 filename.cpp -o filename
./filename
```

## Examples

### Basic Templates
1. **01_basic_function_template.cpp** - Basic function templates with type deduction
2. **02_multiple_type_parameters.cpp** - Templates with multiple type parameters
3. **06_template_specialization.cpp** - Full template specialization for specific types
4. **07_non_type_parameters.cpp** - Non-type template parameters and compile-time constants

### Container Templates
3. **03_class_template_array.cpp** - Generic array class template
4. **04_class_template_stack.cpp** - Type-safe stack implementation
5. **05_class_template_pair.cpp** - Generic pair class with two types
11. **11_template_linked_list.cpp** - Generic linked list with full operations
12. **12_template_queue.cpp** - Queue and priority queue templates

### Advanced Templates
8. **08_variadic_templates.cpp** - Variable number of template arguments
9. **09_template_min_max.cpp** - Comprehensive min/max implementations
10. **10_template_sort.cpp** - Generic sorting algorithms
13. **13_template_binary_search.cpp** - Binary search and search algorithms
14. **14_template_smart_pointer.cpp** - Simple unique pointer implementation
15. **15_template_comprehensive_demo.cpp** - Combining multiple template concepts

## Topics Covered

- Function templates
- Class templates
- Template specialization
- Non-type template parameters
- Variadic templates
- Template type deduction
- Move semantics with templates
- RAII with templates
- Generic algorithms
- Container templates

## Learning Path

1. Start with `01_basic_function_template.cpp` for fundamentals
2. Progress through `02-05` for class templates
3. Study `06-07` for specialization and non-type parameters
4. Explore `08` for variadic templates
5. Practice with `09-13` for algorithms
6. Master `14-15` for advanced concepts

## Notes

- All examples are self-contained with `main()` functions
- Templates are defined in headers (single file here for simplicity)
- Demonstrates compile-time polymorphism
- Shows type safety and zero runtime overhead
- Examples progress from simple to complex
