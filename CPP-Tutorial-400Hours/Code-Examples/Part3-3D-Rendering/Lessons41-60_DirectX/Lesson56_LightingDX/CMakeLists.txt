cmake_minimum_required(VERSION 3.15)
project(Lesson56_LightingDX)

if(WIN32)
    add_executable(Lesson56_LightingDX WIN32 main.cpp)
    target_link_libraries(Lesson56_LightingDX PRIVATE Math3D d3d11 d3dcompiler)
    set_target_properties(Lesson56_LightingDX PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons41-60
    )
else()
    message(STATUS "Skipping DirectX lesson on non-Windows platform")
endif()
