cmake_minimum_required(VERSION 3.15)

# Utils - Header-only utility library
add_library(Utils INTERFACE)

target_include_directories(Utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Utils INTERFACE Math3D)

# Install headers
install(FILES
    ShaderLoader.h
    TextureLoader.h
    Camera.h
    DESTINATION include/Utils
)
