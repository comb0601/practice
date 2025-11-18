# Lesson 96: Build Systems (CMake, MSBuild)

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-95

## Table of Contents
1. CMake Fundamentals
2. Advanced CMake
3. MSBuild Integration
4. Cross-Compilation

---

## 1. Complete CMake Project

```cmake
cmake_minimum_required(VERSION 3.15)

project(MyProject
    VERSION 1.0.0
    DESCRIPTION "My C++ Project"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Dependencies
find_package(Threads REQUIRED)

# Source files
add_library(mylib
    src/module1.cpp
    src/module2.cpp
    include/mylib/module1.h
    include/mylib/module2.h
)

target_include_directories(mylib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mylib
    PUBLIC
        Threads::Threads
)

# Executable
add_executable(myapp
    src/main.cpp
)

target_link_libraries(myapp
    PRIVATE
        mylib
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(TARGETS mylib myapp
    EXPORT MyProjectTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

# Export targets
install(EXPORT MyProjectTargets
    FILE MyProjectTargets.cmake
    NAMESPACE MyProject::
    DESTINATION lib/cmake/MyProject
)

# Package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    MyProjectConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfigVersion.cmake
    DESTINATION lib/cmake/MyProject
)
```

---

## 2. Usage

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Install
cmake --install build --prefix /usr/local

# Package
cd build && cpack
```

---

## Summary

✅ **CMake**: Universal build system
✅ **MSBuild**: Visual Studio integration
✅ **Packaging**: Distribution ready
✅ **Installation**: System integration
