cmake_minimum_required(VERSION 3.15)
project(CPP_3D_Rendering_Course VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_OPENGL_EXAMPLES "Build OpenGL examples" ON)
option(BUILD_DIRECTX_EXAMPLES "Build DirectX examples" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Platform detection
if(WIN32)
    set(WINDOWS TRUE)
    add_definitions(-DWINDOWS -D_CRT_SECURE_NO_WARNINGS)
endif()

# Find required packages
find_package(OpenGL)

# External dependencies directory
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External)

# GLFW
if(BUILD_OPENGL_EXAMPLES)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    if(EXISTS ${EXTERNAL_DIR}/glfw)
        add_subdirectory(${EXTERNAL_DIR}/glfw)
    endif()
endif()

# GLAD
if(BUILD_OPENGL_EXAMPLES)
    if(EXISTS ${EXTERNAL_DIR}/glad)
        add_library(glad STATIC
            ${EXTERNAL_DIR}/glad/src/glad.c
            ${EXTERNAL_DIR}/glad/include/glad/glad.h
            ${EXTERNAL_DIR}/glad/include/KHR/khrplatform.h
        )
        target_include_directories(glad PUBLIC ${EXTERNAL_DIR}/glad/include)
    endif()
endif()

# GLM (header-only)
if(EXISTS ${EXTERNAL_DIR}/glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${EXTERNAL_DIR}/glm)
endif()

# STB Image (header-only)
if(EXISTS ${EXTERNAL_DIR}/stb)
    add_library(stb_image INTERFACE)
    target_include_directories(stb_image INTERFACE ${EXTERNAL_DIR}/stb)
endif()

# Common Math Library
add_subdirectory(Common/Math3D)

# Common Utilities
add_subdirectory(Common/Utils)

# Part 1: 3D Math (Lessons 01-20)
add_subdirectory(Lessons01-20_3DMath)

# Part 2: OpenGL (Lessons 21-40)
if(BUILD_OPENGL_EXAMPLES AND OPENGL_FOUND)
    add_subdirectory(Lessons21-40_OpenGL)
endif()

# Part 3: DirectX (Lessons 41-60)
if(BUILD_DIRECTX_EXAMPLES AND WIN32)
    add_subdirectory(Lessons41-60_DirectX)
endif()

# Part 4: Advanced Rendering (Lessons 61-80)
add_subdirectory(Lessons61-80_Advanced)

# Part 5: Modern Rendering (Lessons 81-100)
add_subdirectory(Lessons81-100_Modern)

# Print configuration summary
message(STATUS "===== Configuration Summary =====")
message(STATUS "Build OpenGL Examples: ${BUILD_OPENGL_EXAMPLES}")
message(STATUS "Build DirectX Examples: ${BUILD_DIRECTX_EXAMPLES}")
message(STATUS "OpenGL Found: ${OPENGL_FOUND}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "================================")
