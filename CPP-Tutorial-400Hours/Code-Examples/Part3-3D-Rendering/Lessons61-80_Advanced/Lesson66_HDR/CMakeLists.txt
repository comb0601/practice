cmake_minimum_required(VERSION 3.15)
project(Lesson66_HDR)
add_executable(Lesson66_HDR main.cpp)
target_link_libraries(Lesson66_HDR PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson66_HDR PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
