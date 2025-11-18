cmake_minimum_required(VERSION 3.15)
project(Lesson73_ComputeShader)
add_executable(Lesson73_ComputeShader main.cpp)
target_link_libraries(Lesson73_ComputeShader PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson73_ComputeShader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
