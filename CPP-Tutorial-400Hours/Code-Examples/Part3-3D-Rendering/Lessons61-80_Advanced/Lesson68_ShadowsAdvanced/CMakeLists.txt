cmake_minimum_required(VERSION 3.15)
project(Lesson68_ShadowsAdvanced)
add_executable(Lesson68_ShadowsAdvanced main.cpp)
target_link_libraries(Lesson68_ShadowsAdvanced PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson68_ShadowsAdvanced PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
