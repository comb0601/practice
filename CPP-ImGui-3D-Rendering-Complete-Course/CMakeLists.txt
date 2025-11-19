cmake_minimum_required(VERSION 3.14)
project(CPP_ImGui_3D_Rendering_Course)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_OPENGL_LESSONS "Build OpenGL lessons (requires GLFW)" ON)
option(BUILD_IMGUI_LESSONS "Build ImGui lessons (requires ImGui + OpenGL)" ON)
option(BUILD_ALL_LESSONS "Build all lessons" OFF)

message(STATUS "==============================================")
message(STATUS "CPP to ImGui & 3D Rendering Complete Course")
message(STATUS "==============================================")
message(STATUS "Build OpenGL Lessons: ${BUILD_OPENGL_LESSONS}")
message(STATUS "Build ImGui Lessons: ${BUILD_IMGUI_LESSONS}")
message(STATUS "==============================================")

# ===================================================================
# External Dependencies
# ===================================================================

# GLAD - OpenGL Loader (custom implementation)
add_library(glad STATIC
    external/glad/src/glad.c
    external/glad/include/glad/glad.h
    external/glad/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW - Windowing library
include(FetchContent)

if(BUILD_OPENGL_LESSONS OR BUILD_IMGUI_LESSONS)
    message(STATUS "Fetching GLFW...")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
        GIT_SHALLOW TRUE
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
    message(STATUS "GLFW fetched successfully")
endif()

# GLM - Mathematics library (header-only)
if(BUILD_OPENGL_LESSONS OR BUILD_IMGUI_LESSONS)
    message(STATUS "Fetching GLM...")
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glm)
    message(STATUS "GLM fetched successfully")
endif()

# ImGui - Immediate Mode GUI
if(BUILD_IMGUI_LESSONS)
    message(STATUS "Fetching ImGui...")
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.9
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(imgui)

    # Create ImGui library
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC glfw glad)
    message(STATUS "ImGui library created successfully")
endif()

# ===================================================================
# Helper function to create OpenGL executables
# ===================================================================
function(add_opengl_executable target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE glfw glad)
    if(TARGET glm)
        target_link_libraries(${target_name} PRIVATE glm::glm)
    endif()

    # Platform-specific libraries
    if(UNIX AND NOT APPLE)
        target_link_libraries(${target_name} PRIVATE dl pthread)
    endif()

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# ===================================================================
# Helper function to create ImGui executables
# ===================================================================
function(add_imgui_executable target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE imgui glfw glad)
    if(TARGET glm)
        target_link_libraries(${target_name} PRIVATE glm::glm)
    endif()

    # Platform-specific libraries
    if(UNIX AND NOT APPLE)
        target_link_libraries(${target_name} PRIVATE dl pthread)
    endif()

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# ===================================================================
# Build lessons based on options
# ===================================================================

# Note: Individual lessons can still be built by navigating to their
# directories and running cmake there. This top-level CMake is for
# building lessons with external dependencies (OpenGL, ImGui, etc.)

message(STATUS "")
message(STATUS "To build individual lessons:")
message(STATUS "  cd Module-XX/Lesson-YYY")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  cmake --build .")
message(STATUS "")
message(STATUS "To build OpenGL lessons with dependencies:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  cmake --build .")
message(STATUS "")
message(STATUS "Dependencies managed:")
message(STATUS "  - GLAD (OpenGL Loader)")
message(STATUS "  - GLFW (Windowing)")
message(STATUS "  - GLM (Mathematics)")
message(STATUS "  - ImGui (UI Framework)")
message(STATUS "==============================================")
