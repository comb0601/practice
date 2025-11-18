cmake_minimum_required(VERSION 3.15)
project(Lesson59_RenderToTexture)

if(WIN32)
    add_executable(Lesson59_RenderToTexture WIN32 main.cpp)
    target_link_libraries(Lesson59_RenderToTexture PRIVATE Math3D d3d11 d3dcompiler)
    set_target_properties(Lesson59_RenderToTexture PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons41-60
    )
else()
    message(STATUS "Skipping DirectX lesson on non-Windows platform")
endif()
