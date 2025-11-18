cmake_minimum_required(VERSION 3.15)
project(Lesson74_Instancing)
add_executable(Lesson74_Instancing main.cpp)
target_link_libraries(Lesson74_Instancing PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson74_Instancing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
