cmake_minimum_required(VERSION 3.15)
project(Lesson92_GraphicsAPIs)
add_executable(Lesson92_GraphicsAPIs main.cpp)
target_link_libraries(Lesson92_GraphicsAPIs PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson92_GraphicsAPIs PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
