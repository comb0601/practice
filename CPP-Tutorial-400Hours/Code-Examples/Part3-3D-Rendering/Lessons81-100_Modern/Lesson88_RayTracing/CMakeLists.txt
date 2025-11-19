cmake_minimum_required(VERSION 3.15)
project(Lesson88_RayTracing)
add_executable(Lesson88_RayTracing main.cpp)
target_link_libraries(Lesson88_RayTracing PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson88_RayTracing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
