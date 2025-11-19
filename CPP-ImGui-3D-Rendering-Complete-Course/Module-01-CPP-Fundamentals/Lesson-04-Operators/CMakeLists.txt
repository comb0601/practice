cmake_minimum_required(VERSION 3.10)
project(Lesson04_Operators)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
add_executable(example01_arithmetic example01_arithmetic.cpp)
add_executable(exercise01 exercise01.cpp)
add_executable(solution01 solution01.cpp)

message(STATUS "Lesson 04 configured successfully!")
