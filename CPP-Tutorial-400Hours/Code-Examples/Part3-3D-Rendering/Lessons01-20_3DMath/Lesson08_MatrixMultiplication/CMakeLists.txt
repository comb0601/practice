cmake_minimum_required(VERSION 3.15)
project(Lesson08_MatrixMultiplication)
add_executable(Lesson08_MatrixMultiplication main.cpp)
target_link_libraries(Lesson08_MatrixMultiplication PRIVATE Math3D)
set_target_properties(Lesson08_MatrixMultiplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
