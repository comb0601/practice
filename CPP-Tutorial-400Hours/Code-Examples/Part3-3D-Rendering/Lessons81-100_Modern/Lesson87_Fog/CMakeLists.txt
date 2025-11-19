cmake_minimum_required(VERSION 3.15)
project(Lesson87_Fog)
add_executable(Lesson87_Fog main.cpp)
target_link_libraries(Lesson87_Fog PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson87_Fog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
