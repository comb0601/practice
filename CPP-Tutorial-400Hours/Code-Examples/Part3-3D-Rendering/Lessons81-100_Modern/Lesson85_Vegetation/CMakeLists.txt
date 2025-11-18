cmake_minimum_required(VERSION 3.15)
project(Lesson85_Vegetation)
add_executable(Lesson85_Vegetation main.cpp)
target_link_libraries(Lesson85_Vegetation PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson85_Vegetation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
