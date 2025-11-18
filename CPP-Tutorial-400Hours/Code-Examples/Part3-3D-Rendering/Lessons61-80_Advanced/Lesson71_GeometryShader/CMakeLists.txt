cmake_minimum_required(VERSION 3.15)
project(Lesson71_GeometryShader)
add_executable(Lesson71_GeometryShader main.cpp)
target_link_libraries(Lesson71_GeometryShader PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson71_GeometryShader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
