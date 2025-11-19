cmake_minimum_required(VERSION 3.15)
project(Lesson07_Transformations)
add_executable(Lesson07_Transformations main.cpp)
target_link_libraries(Lesson07_Transformations PRIVATE Math3D)
set_target_properties(Lesson07_Transformations PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
