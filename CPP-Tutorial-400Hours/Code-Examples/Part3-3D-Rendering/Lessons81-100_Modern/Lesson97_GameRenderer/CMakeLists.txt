cmake_minimum_required(VERSION 3.15)
project(Lesson97_GameRenderer)
add_executable(Lesson97_GameRenderer main.cpp)
target_link_libraries(Lesson97_GameRenderer PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson97_GameRenderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
