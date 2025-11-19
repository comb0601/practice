cmake_minimum_required(VERSION 3.15)
project(Lesson72_Tessellation)
add_executable(Lesson72_Tessellation main.cpp)
target_link_libraries(Lesson72_Tessellation PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson72_Tessellation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
