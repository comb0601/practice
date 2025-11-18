cmake_minimum_required(VERSION 3.15)
project(Lesson18_RayIntersection)
add_executable(Lesson18_RayIntersection main.cpp)
target_link_libraries(Lesson18_RayIntersection PRIVATE Math3D)
set_target_properties(Lesson18_RayIntersection PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
