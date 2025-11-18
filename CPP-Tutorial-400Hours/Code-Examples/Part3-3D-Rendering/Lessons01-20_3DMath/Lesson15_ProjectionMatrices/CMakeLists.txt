cmake_minimum_required(VERSION 3.15)
project(Lesson15_ProjectionMatrices)
add_executable(Lesson15_ProjectionMatrices main.cpp)
target_link_libraries(Lesson15_ProjectionMatrices PRIVATE Math3D)
set_target_properties(Lesson15_ProjectionMatrices PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
