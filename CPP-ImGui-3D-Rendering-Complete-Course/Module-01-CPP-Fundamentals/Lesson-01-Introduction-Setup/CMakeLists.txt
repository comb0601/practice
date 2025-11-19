cmake_minimum_required(VERSION 3.10)
project(Lesson01_Introduction_Setup)

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
add_executable(example02_basic_output example02_basic_output.cpp)
add_executable(example03_program_structure example03_program_structure.cpp)
add_executable(example04_namespace example04_namespace.cpp)
add_executable(example05_compilation_demo example05_compilation_demo.cpp)

# Test setup program (already exists)
add_executable(test_setup test_setup.cpp)

# Exercise programs (starter code)
add_executable(exercise01 exercise01.cpp)
add_executable(exercise02 exercise02.cpp)
add_executable(exercise03 exercise03.cpp)

# Solution programs
add_executable(solution01 solution01.cpp)
add_executable(solution02 solution02.cpp)
add_executable(solution03 solution03.cpp)

# Set output directory
set_target_properties(
    main
    example01_hello_world
    example02_basic_output
    example03_program_structure
    example04_namespace
    example05_compilation_demo
    test_setup
    exercise01
    exercise02
    exercise03
    solution01
    solution02
    solution03
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Print build information
message(STATUS "=== Lesson 01: Introduction and Setup ===")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "=========================================")
