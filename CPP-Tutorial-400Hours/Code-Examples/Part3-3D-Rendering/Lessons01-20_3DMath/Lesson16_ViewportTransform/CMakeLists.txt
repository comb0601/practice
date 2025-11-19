cmake_minimum_required(VERSION 3.15)
project(Lesson16_ViewportTransform)
add_executable(Lesson16_ViewportTransform main.cpp)
target_link_libraries(Lesson16_ViewportTransform PRIVATE Math3D)
set_target_properties(Lesson16_ViewportTransform PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
