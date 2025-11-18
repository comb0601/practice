cmake_minimum_required(VERSION 3.15)
project(Lesson96_3DEngine)
add_executable(Lesson96_3DEngine main.cpp)
target_link_libraries(Lesson96_3DEngine PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson96_3DEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
