cmake_minimum_required(VERSION 3.15)

project(Lesson21_OpenGLSetup)

add_executable(Lesson21_OpenGLSetup main.cpp)

target_link_libraries(Lesson21_OpenGLSetup PRIVATE
    Math3D
    glad
    glfw
    ${OPENGL_LIBRARIES}
)

set_target_properties(Lesson21_OpenGLSetup PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons21-40
)
