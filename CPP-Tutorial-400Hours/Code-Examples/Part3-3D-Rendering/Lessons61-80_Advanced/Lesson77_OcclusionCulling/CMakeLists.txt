cmake_minimum_required(VERSION 3.15)
project(Lesson77_OcclusionCulling)
add_executable(Lesson77_OcclusionCulling main.cpp)
target_link_libraries(Lesson77_OcclusionCulling PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson77_OcclusionCulling PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
