cmake_minimum_required(VERSION 3.10)
project(Lesson03_Variables_DataTypes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Examples
add_executable(main main.cpp)
add_executable(example01_integer_types example01_integer_types.cpp)
add_executable(example02_floating_point example02_floating_point.cpp)
add_executable(example03_char_bool example03_char_bool.cpp)

# Exercises and solutions
add_executable(exercise01 exercise01.cpp)
add_executable(solution01 solution01.cpp)

message(STATUS "Lesson 03 configured successfully!")
