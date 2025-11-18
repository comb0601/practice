cmake_minimum_required(VERSION 3.15)
project(Lesson61_DeferredRendering)
add_executable(Lesson61_DeferredRendering main.cpp)
target_link_libraries(Lesson61_DeferredRendering PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson61_DeferredRendering PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
