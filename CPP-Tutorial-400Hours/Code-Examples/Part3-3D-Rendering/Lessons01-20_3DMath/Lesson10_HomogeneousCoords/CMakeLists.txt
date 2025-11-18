cmake_minimum_required(VERSION 3.15)
project(Lesson10_HomogeneousCoords)
add_executable(Lesson10_HomogeneousCoords main.cpp)
target_link_libraries(Lesson10_HomogeneousCoords PRIVATE Math3D)
set_target_properties(Lesson10_HomogeneousCoords PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
