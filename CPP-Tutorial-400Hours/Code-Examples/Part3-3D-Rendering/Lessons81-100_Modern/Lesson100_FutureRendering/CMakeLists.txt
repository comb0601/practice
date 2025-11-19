cmake_minimum_required(VERSION 3.15)
project(Lesson100_FutureRendering)
add_executable(Lesson100_FutureRendering main.cpp)
target_link_libraries(Lesson100_FutureRendering PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson100_FutureRendering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
