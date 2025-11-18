cmake_minimum_required(VERSION 3.15)
project(Lesson98_MaterialEditor)
add_executable(Lesson98_MaterialEditor main.cpp)
target_link_libraries(Lesson98_MaterialEditor PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson98_MaterialEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
