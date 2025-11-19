cmake_minimum_required(VERSION 3.15)

project(Lesson01_3DMathIntro)

add_executable(Lesson01_3DMathIntro main.cpp)

target_link_libraries(Lesson01_3DMathIntro PRIVATE Math3D)

# Set output directory
set_target_properties(Lesson01_3DMathIntro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
