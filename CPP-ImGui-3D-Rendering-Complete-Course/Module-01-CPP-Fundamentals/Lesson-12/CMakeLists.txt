cmake_minimum_required(VERSION 3.10)
project(Lesson12_references)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Main program
add_executable(main main.cpp)

# Examples
add_executable(example01 example01_references.cpp)
add_executable(example02 example02_references.cpp)
add_executable(example03 example03_references.cpp)
add_executable(example04 example04_references.cpp)
add_executable(example05 example05_references.cpp)

# Exercises
add_executable(exercise01 exercise01.cpp)
add_executable(exercise02 exercise02.cpp)
add_executable(exercise03 exercise03.cpp)

# Solutions
add_executable(solution01 solution01.cpp)
add_executable(solution02 solution02.cpp)
add_executable(solution03 solution03.cpp)

message(STATUS "Lesson 12 configured successfully!")
