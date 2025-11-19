cmake_minimum_required(VERSION 3.15)
project(Lesson50_TexturesDX)

if(WIN32)
    add_executable(Lesson50_TexturesDX WIN32 main.cpp)
    target_link_libraries(Lesson50_TexturesDX PRIVATE Math3D d3d11 d3dcompiler)
    set_target_properties(Lesson50_TexturesDX PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons41-60
    )
else()
    message(STATUS "Skipping DirectX lesson on non-Windows platform")
endif()
