cmake_minimum_required(VERSION 3.15)
project(Lesson23_Pipeline)
add_executable(Lesson23_Pipeline main.cpp)
target_link_libraries(Lesson23_Pipeline PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson23_Pipeline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
