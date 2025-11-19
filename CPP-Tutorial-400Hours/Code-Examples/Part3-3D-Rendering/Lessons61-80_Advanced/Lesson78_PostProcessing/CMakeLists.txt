cmake_minimum_required(VERSION 3.15)
project(Lesson78_PostProcessing)
add_executable(Lesson78_PostProcessing main.cpp)
target_link_libraries(Lesson78_PostProcessing PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson78_PostProcessing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
