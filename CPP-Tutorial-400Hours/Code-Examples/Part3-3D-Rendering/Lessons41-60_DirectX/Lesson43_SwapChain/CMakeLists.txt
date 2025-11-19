cmake_minimum_required(VERSION 3.15)
project(Lesson43_SwapChain)

if(WIN32)
    add_executable(Lesson43_SwapChain WIN32 main.cpp)
    target_link_libraries(Lesson43_SwapChain PRIVATE Math3D d3d11 d3dcompiler)
    set_target_properties(Lesson43_SwapChain PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Lessons41-60
    )
else()
    message(STATUS "Skipping DirectX lesson on non-Windows platform")
endif()
