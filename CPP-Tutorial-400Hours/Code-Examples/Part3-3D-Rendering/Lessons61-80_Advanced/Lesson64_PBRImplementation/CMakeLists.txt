cmake_minimum_required(VERSION 3.15)
project(Lesson64_PBRImplementation)
add_executable(Lesson64_PBRImplementation main.cpp)
target_link_libraries(Lesson64_PBRImplementation PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson64_PBRImplementation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
