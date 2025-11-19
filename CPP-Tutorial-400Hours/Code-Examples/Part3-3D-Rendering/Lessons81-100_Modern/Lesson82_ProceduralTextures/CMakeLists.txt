cmake_minimum_required(VERSION 3.15)
project(Lesson82_ProceduralTextures)
add_executable(Lesson82_ProceduralTextures main.cpp)
target_link_libraries(Lesson82_ProceduralTextures PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson82_ProceduralTextures PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
