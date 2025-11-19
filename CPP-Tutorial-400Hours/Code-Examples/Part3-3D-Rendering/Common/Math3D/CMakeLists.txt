cmake_minimum_required(VERSION 3.15)

# Math3D - Header-only 3D math library
add_library(Math3D INTERFACE)

target_include_directories(Math3D INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Install headers
install(FILES
    Vector3.h
    Matrix4.h
    Quaternion.h
    Math3D.h
    DESTINATION include/Math3D
)
