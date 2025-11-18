cmake_minimum_required(VERSION 3.10)
project(Lesson131_ImGui_Introduction)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Main program
add_executable(main main.cpp)

# Examples (5 total)
add_executable(example01 example01.cpp)
add_executable(example02 example02.cpp)
add_executable(example03 example03.cpp)
add_executable(example04 example04.cpp)
add_executable(example05 example05.cpp)

# Exercises (3 total)
add_executable(exercise01 exercise01.cpp)
add_executable(exercise02 exercise02.cpp)
add_executable(exercise03 exercise03.cpp)

# Solutions (3 total)
add_executable(solution01 solution01.cpp)
add_executable(solution02 solution02.cpp)
add_executable(solution03 solution03.cpp)

message(STATUS "Lesson 131 - ImGui Introduction configured")
