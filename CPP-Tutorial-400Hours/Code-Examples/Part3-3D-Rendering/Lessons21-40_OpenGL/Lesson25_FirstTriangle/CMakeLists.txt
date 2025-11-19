cmake_minimum_required(VERSION 3.15)

project(Lesson25_FirstTriangle)

add_executable(Lesson25_FirstTriangle main.cpp)

target_link_libraries(Lesson25_FirstTriangle PRIVATE
    Math3D
    glad
    glfw
    ${OPENGL_LIBRARIES}
)

set_target_properties(Lesson25_FirstTriangle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)

# Copy shaders to build directory
configure_file(vertex.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/vertex.glsl COPYONLY)
configure_file(fragment.glsl ${CMAKE_BINARY_DIR}/bin/Lessons21-40/fragment.glsl COPYONLY)
