cmake_minimum_required(VERSION 3.15)
project(LessonIterator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h" "*.hpp")

add_executable(demo ${SOURCES} ${HEADERS})

# Threading support
find_package(Threads)
if(Threads_FOUND)
    target_link_libraries(demo Threads::Threads)
endif()

# Optimization flags
if(MSVC)
    target_compile_options(demo PRIVATE /O2 /Oi)
else()
    target_compile_options(demo PRIVATE -O3 -march=native)
endif()

install(TARGETS demo DESTINATION bin)
