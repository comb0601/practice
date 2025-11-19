cmake_minimum_required(VERSION 3.15)

project(Lessons01-20_3DMath/Lesson04_DotCrossProduct)

add_executable(Lessons01-20_3DMath/Lesson04_DotCrossProduct main.cpp)

target_link_libraries(Lessons01-20_3DMath/Lesson04_DotCrossProduct PRIVATE Math3D)

set_target_properties(Lessons01-20_3DMath/Lesson04_DotCrossProduct PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
