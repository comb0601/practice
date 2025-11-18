cmake_minimum_required(VERSION 3.15)
project(Lesson76_FrustumCullingGPU)
add_executable(Lesson76_FrustumCullingGPU main.cpp)
target_link_libraries(Lesson76_FrustumCullingGPU PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson76_FrustumCullingGPU PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
