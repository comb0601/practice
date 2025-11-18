cmake_minimum_required(VERSION 3.15)
project(Lesson70_Reflections)
add_executable(Lesson70_Reflections main.cpp)
target_link_libraries(Lesson70_Reflections PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson70_Reflections PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
