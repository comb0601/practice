cmake_minimum_required(VERSION 3.15)
project(Lesson84_TerrainRendering)
add_executable(Lesson84_TerrainRendering main.cpp)
target_link_libraries(Lesson84_TerrainRendering PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson84_TerrainRendering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
