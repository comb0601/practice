cmake_minimum_required(VERSION 3.15)
project(Lesson90_PathTracing)
add_executable(Lesson90_PathTracing main.cpp)
target_link_libraries(Lesson90_PathTracing PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson90_PathTracing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
