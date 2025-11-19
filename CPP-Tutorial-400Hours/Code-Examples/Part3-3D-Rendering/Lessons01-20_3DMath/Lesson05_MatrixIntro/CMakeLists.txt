cmake_minimum_required(VERSION 3.15)
project(Lesson05_MatrixIntro)
add_executable(Lesson05_MatrixIntro main.cpp)
target_link_libraries(Lesson05_MatrixIntro PRIVATE Math3D)
set_target_properties(Lesson05_MatrixIntro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
