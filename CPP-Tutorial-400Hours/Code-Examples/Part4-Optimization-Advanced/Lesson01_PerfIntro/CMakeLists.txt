cmake_minimum_required(VERSION 3.15)
project(Lesson01_PerfIntro CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

add_executable(perf_intro main.cpp)

# Enable optimization reports for Visual Studio
if(MSVC)
    target_compile_options(perf_intro PRIVATE /Qpar-report:2)
endif()

# For GCC/Clang, enable optimization reports
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(perf_intro PRIVATE -fopt-info-vec-optimized)
endif()

# Installation
install(TARGETS perf_intro DESTINATION bin)

# Custom target for running the application
add_custom_target(run
    COMMAND perf_intro
    DEPENDS perf_intro
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
