cmake_minimum_required(VERSION 3.15)
project(Lesson41_DirectXIntro)

if(WIN32)
    add_executable(Lesson41_DirectXIntro WIN32 main.cpp)
    target_link_libraries(Lesson41_DirectXIntro PRIVATE Math3D d3d11 d3dcompiler)
    set_target_properties(Lesson41_DirectXIntro PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons41-60
    )
else()
    message(STATUS "Skipping DirectX lesson on non-Windows platform")
endif()
