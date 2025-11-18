cmake_minimum_required(VERSION 3.15)
project(Lesson65_IBL)
add_executable(Lesson65_IBL main.cpp)
target_link_libraries(Lesson65_IBL PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson65_IBL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
