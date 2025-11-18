cmake_minimum_required(VERSION 3.15)
project(Lesson19_BoundingVolumes)
add_executable(Lesson19_BoundingVolumes main.cpp)
target_link_libraries(Lesson19_BoundingVolumes PRIVATE Math3D)
set_target_properties(Lesson19_BoundingVolumes PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
