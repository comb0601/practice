cmake_minimum_required(VERSION 3.15)
project(Lesson20_MathLibrary)
add_executable(Lesson20_MathLibrary main.cpp)
target_link_libraries(Lesson20_MathLibrary PRIVATE Math3D)
set_target_properties(Lesson20_MathLibrary PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
