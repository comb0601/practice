cmake_minimum_required(VERSION 3.15)
project(Lesson29_ShaderPrograms)
add_executable(Lesson29_ShaderPrograms main.cpp)
target_link_libraries(Lesson29_ShaderPrograms PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson29_ShaderPrograms PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
