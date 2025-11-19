cmake_minimum_required(VERSION 3.15)
project(Lesson14_ViewMatrix)
add_executable(Lesson14_ViewMatrix main.cpp)
target_link_libraries(Lesson14_ViewMatrix PRIVATE Math3D)
set_target_properties(Lesson14_ViewMatrix PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
