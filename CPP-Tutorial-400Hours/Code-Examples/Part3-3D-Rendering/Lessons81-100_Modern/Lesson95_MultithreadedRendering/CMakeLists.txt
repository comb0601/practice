cmake_minimum_required(VERSION 3.15)
project(Lesson95_MultithreadedRendering)
add_executable(Lesson95_MultithreadedRendering main.cpp)
target_link_libraries(Lesson95_MultithreadedRendering PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson95_MultithreadedRendering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
