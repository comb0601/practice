cmake_minimum_required(VERSION 3.15)
project(Lesson63_PBRTheory)
add_executable(Lesson63_PBRTheory main.cpp)
target_link_libraries(Lesson63_PBRTheory PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson63_PBRTheory PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
