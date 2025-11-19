cmake_minimum_required(VERSION 3.15)
project(Lesson34_Camera)
add_executable(Lesson34_Camera main.cpp)
target_link_libraries(Lesson34_Camera PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson34_Camera PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
