cmake_minimum_required(VERSION 3.15)
project(Lesson38_ModelLoading)
add_executable(Lesson38_ModelLoading main.cpp)
target_link_libraries(Lesson38_ModelLoading PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson38_ModelLoading PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
