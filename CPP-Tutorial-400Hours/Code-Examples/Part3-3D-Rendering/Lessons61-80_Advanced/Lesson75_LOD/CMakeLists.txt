cmake_minimum_required(VERSION 3.15)
project(Lesson75_LOD)
add_executable(Lesson75_LOD main.cpp)
target_link_libraries(Lesson75_LOD PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson75_LOD PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
