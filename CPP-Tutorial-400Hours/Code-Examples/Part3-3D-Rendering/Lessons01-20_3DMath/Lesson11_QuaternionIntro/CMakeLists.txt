cmake_minimum_required(VERSION 3.15)
project(Lesson11_QuaternionIntro)
add_executable(Lesson11_QuaternionIntro main.cpp)
target_link_libraries(Lesson11_QuaternionIntro PRIVATE Math3D)
set_target_properties(Lesson11_QuaternionIntro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons01-20
)
