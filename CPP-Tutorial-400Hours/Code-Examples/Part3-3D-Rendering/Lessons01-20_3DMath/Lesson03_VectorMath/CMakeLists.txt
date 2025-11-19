cmake_minimum_required(VERSION 3.15)

project(Lessons01-20_3DMath/Lesson03_VectorMath)

add_executable(Lessons01-20_3DMath/Lesson03_VectorMath main.cpp)

target_link_libraries(Lessons01-20_3DMath/Lesson03_VectorMath PRIVATE Math3D)

set_target_properties(Lessons01-20_3DMath/Lesson03_VectorMath PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
