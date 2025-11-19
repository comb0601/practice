cmake_minimum_required(VERSION 3.15)
project(Lesson99_BestPractices)
add_executable(Lesson99_BestPractices main.cpp)
target_link_libraries(Lesson99_BestPractices PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson99_BestPractices PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
