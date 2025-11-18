cmake_minimum_required(VERSION 3.15)
project(Lesson86_VolumetricLighting)
add_executable(Lesson86_VolumetricLighting main.cpp)
target_link_libraries(Lesson86_VolumetricLighting PRIVATE Math3D Utils glad glfw ${OPENGL_LIBRARIES})
set_target_properties(Lesson86_VolumetricLighting PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons81-100_Modern
)
