cmake_minimum_required(VERSION 3.15)
project(Lesson33_Transformations)
add_executable(Lesson33_Transformations main.cpp)
target_link_libraries(Lesson33_Transformations PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson33_Transformations PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
