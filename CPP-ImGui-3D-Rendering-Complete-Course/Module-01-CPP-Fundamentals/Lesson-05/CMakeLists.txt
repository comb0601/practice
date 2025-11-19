cmake_minimum_required(VERSION 3.10)
project(Lesson05_Control_Flow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
add_executable(example01_if_statement example01_if_statement.cpp)
add_executable(example02_if_else example02_if_else.cpp)
add_executable(example03_else_if example03_else_if.cpp)
add_executable(example04_switch example04_switch.cpp)
add_executable(example05_ternary example05_ternary.cpp)

add_executable(exercise01 exercise01.cpp)
add_executable(exercise02 exercise02.cpp)
add_executable(exercise03 exercise03.cpp)

add_executable(solution01 solution01.cpp)
add_executable(solution02 solution02.cpp)
add_executable(solution03 solution03.cpp)

message(STATUS "Lesson 05 configured successfully!")
