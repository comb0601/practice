cmake_minimum_required(VERSION 3.14)
project(Lesson095_First_Triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ===================================================================
# External Dependencies for OpenGL
# ===================================================================

# GLAD - OpenGL Loader
add_library(glad STATIC
    ../../external/glad/src/glad.c
    ../../external/glad/include/glad/glad.h
    ../../external/glad/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC ../../external/glad/include)

# GLFW - Windowing library
include(FetchContent)
message(STATUS "Fetching GLFW for Lesson 95...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    GIT_SHALLOW TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ===================================================================
# Main OpenGL Triangle Program
# ===================================================================
add_executable(main main.cpp)
target_link_libraries(main PRIVATE glfw glad)

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE dl pthread)
elseif(APPLE)
    target_link_libraries(main PRIVATE "-framework OpenGL")
endif()

# ===================================================================
# Example programs (simplified versions)
# ===================================================================
add_executable(example01 example01.cpp)
target_link_libraries(example01 PRIVATE glfw glad)

add_executable(example02 example02.cpp)
target_link_libraries(example02 PRIVATE glfw glad)

add_executable(example03 example03.cpp)
target_link_libraries(example03 PRIVATE glfw glad)

add_executable(example04 example04.cpp)
target_link_libraries(example04 PRIVATE glfw glad)

add_executable(example05 example05.cpp)
target_link_libraries(example05 PRIVATE glfw glad)

# Link platform libraries for examples
if(UNIX AND NOT APPLE)
    target_link_libraries(example01 PRIVATE dl pthread)
    target_link_libraries(example02 PRIVATE dl pthread)
    target_link_libraries(example03 PRIVATE dl pthread)
    target_link_libraries(example04 PRIVATE dl pthread)
    target_link_libraries(example05 PRIVATE dl pthread)
elseif(APPLE)
    target_link_libraries(example01 PRIVATE "-framework OpenGL")
    target_link_libraries(example02 PRIVATE "-framework OpenGL")
    target_link_libraries(example03 PRIVATE "-framework OpenGL")
    target_link_libraries(example04 PRIVATE "-framework OpenGL")
    target_link_libraries(example05 PRIVATE "-framework OpenGL")
endif()

# ===================================================================
# Exercises and Solutions (basic OpenGL programs)
# ===================================================================
foreach(num RANGE 1 3)
    add_executable(exercise0${num} exercise0${num}.cpp)
    target_link_libraries(exercise0${num} PRIVATE glfw glad)

    add_executable(solution0${num} solution0${num}.cpp)
    target_link_libraries(solution0${num} PRIVATE glfw glad)

    if(UNIX AND NOT APPLE)
        target_link_libraries(exercise0${num} PRIVATE dl pthread)
        target_link_libraries(solution0${num} PRIVATE dl pthread)
    elseif(APPLE)
        target_link_libraries(exercise0${num} PRIVATE "-framework OpenGL")
        target_link_libraries(solution0${num} PRIVATE "-framework OpenGL")
    endif()
endforeach()

message(STATUS "Lesson 95 - First Triangle configured with OpenGL support")
message(STATUS "Dependencies: GLFW + GLAD")
