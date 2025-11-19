cmake_minimum_required(VERSION 3.15)
project(Lesson12_QuaternionRotations)
add_executable(Lesson12_QuaternionRotations main.cpp)
target_link_libraries(Lesson12_QuaternionRotations PRIVATE Math3D)
set_target_properties(Lesson12_QuaternionRotations PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
