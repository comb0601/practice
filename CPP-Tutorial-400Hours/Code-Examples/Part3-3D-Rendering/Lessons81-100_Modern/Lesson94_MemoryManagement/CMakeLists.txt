cmake_minimum_required(VERSION 3.15)
project(Lesson94_MemoryManagement)
add_executable(Lesson94_MemoryManagement main.cpp)
target_link_libraries(Lesson94_MemoryManagement PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson94_MemoryManagement PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
