cmake_minimum_required(VERSION 3.15)
project(Lesson09_SIMD CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simd_demo main.cpp)

# Enable SIMD instructions
if(MSVC)
    target_compile_options(simd_demo PRIVATE /arch:AVX2 /O2)
else()
    # GCC/Clang
    target_compile_options(simd_demo PRIVATE -mavx2 -O3 -march=native)
endif()

install(TARGETS simd_demo DESTINATION bin)
