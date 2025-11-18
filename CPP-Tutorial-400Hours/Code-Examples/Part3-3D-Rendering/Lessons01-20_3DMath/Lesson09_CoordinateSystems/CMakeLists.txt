cmake_minimum_required(VERSION 3.15)
project(Lesson09_CoordinateSystems)
add_executable(Lesson09_CoordinateSystems main.cpp)
target_link_libraries(Lesson09_CoordinateSystems PRIVATE Math3D)
set_target_properties(Lesson09_CoordinateSystems PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
