cmake_minimum_required(VERSION 3.15)
project(CPP_Tutorial_400_Hours VERSION 1.0 LANGUAGES CXX)

# Global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform detection
if(WIN32)
    set(WINDOWS TRUE)
    add_definitions(-DWINDOWS -D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
elseif(APPLE)
    set(MACOS TRUE)
endif()

# Compiler-specific flags
if(MSVC)
    add_compile_options(/W4 /O2 /MP)
    # Enable exceptions
    add_compile_options(/EHsc)
else()
    add_compile_options(-Wall -Wextra -O2 -pthread)
endif()

# Build options
option(BUILD_PART1_FUNDAMENTALS "Build Part 1: C++ Fundamentals (Lessons 01-85)" ON)
option(BUILD_PART2_MFC "Build Part 2: MFC UI Development (Lessons 01-90) - Windows Only" OFF)
option(BUILD_PART3_3D_RENDERING "Build Part 3: 3D Rendering (Lessons 01-100)" ON)
option(BUILD_PART4_OPTIMIZATION "Build Part 4: Optimization & Advanced (Lessons 01-100)" ON)
option(BUILD_ALL_PARTS "Build all parts of the course" ON)

# Override individual options if BUILD_ALL_PARTS is ON
if(BUILD_ALL_PARTS)
    set(BUILD_PART1_FUNDAMENTALS ON)
    set(BUILD_PART3_3D_RENDERING ON)
    set(BUILD_PART4_OPTIMIZATION ON)
    # MFC is still OFF by default unless explicitly enabled
    if(WIN32)
        message(STATUS "Windows detected: MFC examples available but not built by default")
        message(STATUS "To build MFC examples, use: -DBUILD_PART2_MFC=ON")
    endif()
endif()

# Threading support (needed for many lessons)
find_package(Threads REQUIRED)

# Print header
message(STATUS "")
message(STATUS "===========================================================================")
message(STATUS "  C++ Tutorial 400 Hours - Master Build System")
message(STATUS "===========================================================================")
message(STATUS "")
message(STATUS "Total Course: 370+ Lessons across 4 major parts")
message(STATUS "")

# Part 1: C++ Fundamentals (85 lessons)
if(BUILD_PART1_FUNDAMENTALS)
    message(STATUS "Building Part 1: C++ Fundamentals (85 lessons)")
    add_subdirectory(Part1-Fundamentals)
else()
    message(STATUS "Skipping Part 1: C++ Fundamentals")
endif()

# Part 2: MFC UI Development (90 lessons) - Windows only
if(BUILD_PART2_MFC)
    if(WIN32)
        message(STATUS "Building Part 2: MFC UI Development (90 lessons)")
        message(WARNING "MFC examples require Visual Studio and MFC libraries")
        message(WARNING "These examples are best built using the .sln file directly")
        # Note: MFC projects are typically built using Visual Studio solution files
        # CMake support for MFC is limited, so we recommend using CPP-Tutorial-All.sln
    else()
        message(WARNING "Part 2 (MFC) can only be built on Windows")
        message(WARNING "Skipping MFC examples on non-Windows platform")
    endif()
else()
    message(STATUS "Skipping Part 2: MFC UI Development")
endif()

# Part 3: 3D Rendering (100 lessons)
if(BUILD_PART3_3D_RENDERING)
    message(STATUS "Building Part 3: 3D Rendering (100 lessons)")
    add_subdirectory(Part3-3D-Rendering)
else()
    message(STATUS "Skipping Part 3: 3D Rendering")
endif()

# Part 4: Optimization & Advanced Topics (100 lessons)
if(BUILD_PART4_OPTIMIZATION)
    message(STATUS "Building Part 4: Optimization & Advanced Topics (100 lessons)")
    add_subdirectory(Part4-Optimization-Advanced)
else()
    message(STATUS "Skipping Part 4: Optimization & Advanced Topics")
endif()

# Print summary
message(STATUS "")
message(STATUS "===========================================================================")
message(STATUS "  Build Configuration Summary")
message(STATUS "===========================================================================")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Parts to build:")
message(STATUS "  Part 1 - Fundamentals:        ${BUILD_PART1_FUNDAMENTALS}")
message(STATUS "  Part 2 - MFC UI:              ${BUILD_PART2_MFC}")
message(STATUS "  Part 3 - 3D Rendering:        ${BUILD_PART3_3D_RENDERING}")
message(STATUS "  Part 4 - Optimization:        ${BUILD_PART4_OPTIMIZATION}")
message(STATUS "")
message(STATUS "Output directories:")
message(STATUS "  Executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries:   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "")
message(STATUS "Quick build commands:")
message(STATUS "  All parts:       cmake -S . -B build && cmake --build build")
message(STATUS "  Part 1 only:     cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART1_FUNDAMENTALS=ON")
message(STATUS "  Part 3 only:     cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART3_3D_RENDERING=ON")
message(STATUS "  Part 4 only:     cmake -S . -B build -DBUILD_ALL_PARTS=OFF -DBUILD_PART4_OPTIMIZATION=ON")
message(STATUS "  Release build:   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release")
message(STATUS "")
message(STATUS "===========================================================================")
message(STATUS "")
