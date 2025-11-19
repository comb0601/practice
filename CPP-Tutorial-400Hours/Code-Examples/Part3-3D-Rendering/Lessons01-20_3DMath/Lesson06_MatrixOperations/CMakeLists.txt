cmake_minimum_required(VERSION 3.15)
project(Lesson06_MatrixOperations)
add_executable(Lesson06_MatrixOperations main.cpp)
target_link_libraries(Lesson06_MatrixOperations PRIVATE Math3D)
set_target_properties(Lesson06_MatrixOperations PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
