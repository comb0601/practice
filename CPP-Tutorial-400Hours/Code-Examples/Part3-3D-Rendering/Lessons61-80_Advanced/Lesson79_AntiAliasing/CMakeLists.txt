cmake_minimum_required(VERSION 3.15)
project(Lesson79_AntiAliasing)
add_executable(Lesson79_AntiAliasing main.cpp)
target_link_libraries(Lesson79_AntiAliasing PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson79_AntiAliasing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
