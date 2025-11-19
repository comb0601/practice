cmake_minimum_required(VERSION 3.15)
project(Lesson93_GPUOptimization)
add_executable(Lesson93_GPUOptimization main.cpp)
target_link_libraries(Lesson93_GPUOptimization PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson93_GPUOptimization PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
