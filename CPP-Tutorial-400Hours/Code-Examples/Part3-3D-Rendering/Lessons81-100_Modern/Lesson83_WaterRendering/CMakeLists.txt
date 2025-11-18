cmake_minimum_required(VERSION 3.15)
project(Lesson83_WaterRendering)
add_executable(Lesson83_WaterRendering main.cpp)
target_link_libraries(Lesson83_WaterRendering PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson83_WaterRendering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
