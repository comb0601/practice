cmake_minimum_required(VERSION 3.15)
project(Lesson80_ParticleSystems)
add_executable(Lesson80_ParticleSystems main.cpp)
target_link_libraries(Lesson80_ParticleSystems PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson80_ParticleSystems PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons61-80_Advanced
)
