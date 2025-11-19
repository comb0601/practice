cmake_minimum_required(VERSION 3.15)
project(Lesson17_FrustumCulling)
add_executable(Lesson17_FrustumCulling main.cpp)
target_link_libraries(Lesson17_FrustumCulling PRIVATE Math3D)
set_target_properties(Lesson17_FrustumCulling PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
