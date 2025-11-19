cmake_minimum_required(VERSION 3.15)
project(Lesson62_SSAO)
add_executable(Lesson62_SSAO main.cpp)
target_link_libraries(Lesson62_SSAO PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson62_SSAO PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
