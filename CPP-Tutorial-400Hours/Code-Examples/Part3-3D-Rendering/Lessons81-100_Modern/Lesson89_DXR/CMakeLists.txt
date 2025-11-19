cmake_minimum_required(VERSION 3.15)
project(Lesson89_DXR)
add_executable(Lesson89_DXR main.cpp)
target_link_libraries(Lesson89_DXR PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson89_DXR PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
