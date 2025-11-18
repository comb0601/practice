cmake_minimum_required(VERSION 3.15)
project(Lesson69_Cubemaps)
add_executable(Lesson69_Cubemaps main.cpp)
target_link_libraries(Lesson69_Cubemaps PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson69_Cubemaps PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
