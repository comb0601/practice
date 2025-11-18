cmake_minimum_required(VERSION 3.10)
project(Lesson02_First_Program)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Main demonstration program
add_executable(main main.cpp)

# Example programs
add_executable(example01_hello_world example01_hello_world.cpp)
add_executable(example02_main_function example02_main_function.cpp)
add_executable(example03_comments example03_comments.cpp)
add_executable(example04_basic_io example04_basic_io.cpp)
add_executable(example05_namespaces example05_namespaces.cpp)
add_executable(example06_simple_calculator example06_simple_calculator.cpp)
add_executable(example07_temperature_converter example07_temperature_converter.cpp)

# Exercise programs (starter code)
add_executable(exercise01 exercise01.cpp)
add_executable(exercise02 exercise02.cpp)
add_executable(exercise03 exercise03.cpp)
add_executable(exercise04 exercise04.cpp)
add_executable(exercise05 exercise05.cpp)

# Solution programs
add_executable(solution01 solution01.cpp)
add_executable(solution02 solution02.cpp)
add_executable(solution03 solution03.cpp)
add_executable(solution04 solution04.cpp)
add_executable(solution05 solution05.cpp)

# Set output directory
set_target_properties(
    main
    example01_hello_world
    example02_main_function
    example03_comments
    example04_basic_io
    example05_namespaces
    example06_simple_calculator
    example07_temperature_converter
    exercise01
    exercise02
    exercise03
    exercise04
    exercise05
    solution01
    solution02
    solution03
    solution04
    solution05
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Print build information
message(STATUS "=== Lesson 02: Your First C++ Program ===")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "=========================================")
