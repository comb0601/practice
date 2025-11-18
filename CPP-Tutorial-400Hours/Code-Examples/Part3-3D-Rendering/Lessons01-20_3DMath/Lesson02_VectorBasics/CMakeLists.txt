cmake_minimum_required(VERSION 3.15)

project(Lesson02_VectorBasics)

add_executable(Lesson02_VectorBasics main.cpp)

target_link_libraries(Lesson02_VectorBasics PRIVATE Math3D)

set_target_properties(Lesson02_VectorBasics PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
