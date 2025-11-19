cmake_minimum_required(VERSION 3.15)
project(Lesson02_Benchmarking CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

add_executable(benchmarking_framework
    main.cpp
    benchmark.h
)

# Enable optimizations
if(MSVC)
    target_compile_options(benchmarking_framework PRIVATE /O2 /Oi)
else()
    target_compile_options(benchmarking_framework PRIVATE -O3)
endif()

install(TARGETS benchmarking_framework DESTINATION bin)
