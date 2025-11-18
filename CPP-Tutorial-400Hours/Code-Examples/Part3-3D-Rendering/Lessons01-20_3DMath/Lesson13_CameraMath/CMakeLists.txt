cmake_minimum_required(VERSION 3.15)
project(Lesson13_CameraMath)
add_executable(Lesson13_CameraMath main.cpp)
target_link_libraries(Lesson13_CameraMath PRIVATE Math3D)
set_target_properties(Lesson13_CameraMath PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
