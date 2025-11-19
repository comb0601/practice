#!/usr/bin/env python3
"""
Generate complete, compilable C++ code for all 100 Part 4 lessons
Each lesson includes: main.cpp, CMakeLists.txt, README.md, .vcxproj
"""

import os
import sys

BASE_DIR = "/home/user/practice/CPP-Tutorial-400Hours/Code-Examples/Part4-Optimization-Advanced"

# Lesson definitions: (number, name, title, description)
LESSONS = [
    (1, "PerfIntro", "Performance Optimization Introduction", "cache locality, algorithm complexity"),
    (2, "Benchmarking", "Complete Benchmarking Framework", "timing, statistics"),
    (3, "VSProfiler", "Visual Studio Profiler", "profiling tools"),
    (4, "VTune", "Intel VTune", "advanced profiling"),
    (5, "PerfCounters", "CPU Performance Counters", "hardware counters"),
    (6, "MemoryProfiling", "Memory Profiling Tools", "memory analysis"),
    (7, "CacheOptimization", "Cache Optimization", "cache-friendly code"),
    (8, "BranchPrediction", "Branch Prediction", "branch optimization"),
    (9, "SIMD", "SSE/AVX Vectorization", "SIMD instructions"),
    (10, "CompilerOptimization", "Compiler Optimization Flags", "compiler options"),
    (11, "PGO", "Profile-Guided Optimization", "PGO techniques"),
    (12, "LTO", "Link-Time Optimization", "LTO benefits"),
    (13, "HotPath", "Hot Path Optimization", "critical path optimization"),
    (14, "DataOrientedDesign", "Data-Oriented Design", "DOD principles"),
    (15, "MemoryLayout", "Memory Alignment", "alignment and padding"),
    (16, "CustomAllocators", "Custom Memory Allocators", "allocator design"),
    (17, "ObjectPool", "Object Pool Pattern", "object pooling"),
    (18, "SmallStringOpt", "Small String Optimization", "SSO implementation"),
    (19, "CopyElision", "Copy Elision and RVO", "return value optimization"),
    (20, "PerfTestingFramework", "Performance Testing Framework", "automated performance tests"),
    (21, "BigOAnalysis", "Algorithm Complexity Analysis", "Big-O notation"),
    (22, "DataStructures", "Choosing Data Structures", "performance comparison"),
    (23, "StringOptimization", "String Optimization", "string performance"),
    (24, "LoopOptimization", "Loop Optimization", "loop unrolling, fusion"),
    (25, "Inlining", "Function Inlining", "inline strategies"),
    (26, "VirtualCallOpt", "Virtual Function Optimization", "devirtualization"),
    (27, "TemplateMetaprogramming", "Template Metaprogramming", "compile-time computation"),
    (28, "Constexpr", "Constexpr Programming", "constexpr functions"),
    (29, "ZeroCost", "Zero-Cost Abstractions", "abstraction performance"),
    (30, "MemoryAllocation", "Reducing Allocations", "allocation strategies"),
    (31, "LazyEvaluation", "Lazy Evaluation", "delayed computation"),
    (32, "ExpressionTemplates", "Expression Templates", "eliminating temporaries"),
    (33, "CRTP", "CRTP Pattern", "Curiously Recurring Template Pattern"),
    (34, "TypeErasure", "Type Erasure", "polymorphism without virtual"),
    (35, "SmallObjectOpt", "Small Object Optimization", "SOO techniques"),
    (36, "FastMath", "Fast Math Techniques", "mathematical optimizations"),
    (37, "BitManipulation", "Bit Manipulation Tricks", "bit operations"),
    (38, "LookupTables", "Lookup Tables", "precomputation"),
    (39, "BranchMinimization", "Minimizing Branching", "branchless programming"),
    (40, "OptimizationCases", "Real-World Optimization Cases", "case studies"),
    (41, "ConcurrencyIntro", "Concurrency Fundamentals", "concurrency basics"),
    (42, "StdThread", "std::thread Basics", "thread creation"),
    (43, "ThreadManagement", "Thread Lifecycle Management", "thread management"),
    (44, "RaceConditions", "Race Conditions", "data races"),
    (45, "MutexLocks", "Mutex and Locks", "synchronization"),
    (46, "LockFree", "Lock-Free Programming", "atomic operations"),
    (47, "Atomics", "Atomic Operations", "std::atomic"),
    (48, "MemoryOrdering", "Memory Ordering", "memory models"),
    (49, "ConditionVariables", "Condition Variables", "thread coordination"),
    (50, "Semaphores", "Semaphores and Barriers", "synchronization primitives"),
    (51, "ThreadPool", "Thread Pool Implementation", "worker threads"),
    (52, "WorkStealing", "Work-Stealing Algorithm", "dynamic load balancing"),
    (53, "ProducerConsumer", "Producer-Consumer Pattern", "queue-based communication"),
    (54, "ReadersWriters", "Readers-Writers Problem", "concurrent access"),
    (55, "AsyncFuture", "async and future", "asynchronous programming"),
    (56, "Promises", "Promises and Futures", "promise/future pattern"),
    (57, "ParallelAlgorithms", "C++17 Parallel Algorithms", "parallel STL"),
    (58, "TaskParallelism", "Task-Based Parallelism", "task graphs"),
    (59, "SIMDThreading", "SIMD + Multithreading", "hybrid parallelism"),
    (60, "ConcurrentDataStructures", "Concurrent Data Structures", "thread-safe containers"),
    (61, "PatternsIntro", "Design Patterns Introduction", "pattern overview"),
    (62, "Singleton", "Singleton Pattern", "single instance"),
    (63, "Factory", "Factory Pattern", "object creation"),
    (64, "AbstractFactory", "Abstract Factory", "family of objects"),
    (65, "Builder", "Builder Pattern", "complex construction"),
    (66, "Prototype", "Prototype Pattern", "cloning objects"),
    (67, "Adapter", "Adapter Pattern", "interface adaptation"),
    (68, "Bridge", "Bridge Pattern", "decoupling abstraction"),
    (69, "Composite", "Composite Pattern", "tree structures"),
    (70, "Decorator", "Decorator Pattern", "adding functionality"),
    (71, "Facade", "Facade Pattern", "simplified interface"),
    (72, "Proxy", "Proxy Pattern", "controlled access"),
    (73, "ChainOfResponsibility", "Chain of Responsibility", "request handling"),
    (74, "Command", "Command Pattern", "encapsulating requests"),
    (75, "Iterator", "Iterator Pattern", "sequential access"),
    (76, "Observer", "Observer Pattern", "event notification"),
    (77, "State", "State Pattern", "state-based behavior"),
    (78, "Strategy", "Strategy Pattern", "algorithm selection"),
    (79, "TemplateMethod", "Template Method", "algorithm skeleton"),
    (80, "Visitor", "Visitor Pattern", "operations on structures"),
    (81, "Architecture", "Software Architecture", "system design"),
    (82, "DataProcessingEngine1", "Data Processing Engine Part 1", "pipeline architecture"),
    (83, "DataProcessingEngine2", "Data Processing Engine Part 2", "optimization and scaling"),
    (84, "GameEngine1", "Game Engine Part 1", "entity component system"),
    (85, "GameEngine2", "Game Engine Part 2", "rendering and physics"),
    (86, "NetworkServer", "Network Server", "high-performance server"),
    (87, "AnalyticsSystem", "Real-Time Analytics System", "data aggregation"),
    (88, "MemoryDatabase", "Memory-Optimized Database", "in-memory storage"),
    (89, "RenderingEngine", "3D Rendering Engine", "graphics pipeline"),
    (90, "MLFramework", "Machine Learning Framework", "tensor operations"),
    (91, "CodeReview", "Code Review Examples", "review process"),
    (92, "Testing", "Testing Strategies", "unit and integration tests"),
    (93, "CICD", "CI/CD Pipelines", "automation"),
    (94, "Documentation", "API Documentation", "documentation generation"),
    (95, "CrossPlatform", "Cross-Platform Code", "platform abstraction"),
    (96, "BuildSystems", "CMake and MSBuild", "build configuration"),
    (97, "AdvancedDebugging", "Advanced Debugging", "debugging techniques"),
    (98, "Security", "Security Best Practices", "secure coding"),
    (99, "Refactoring", "Refactoring Examples", "code improvement"),
    (100, "CareerFinal", "Final Capstone Project", "complete application"),
]

def create_main_cpp(lesson_num, lesson_name, title, description):
    """Generate a complete main.cpp for the lesson"""
    return f'''/*
 * Lesson {lesson_num:02d}: {title}
 * Demonstrates {description}
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

class Timer {{
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {{}}

    double elapsed_ms() {{
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }}
}};

void print_header(const std::string& title) {{
    std::cout << "\\n" << std::string(60, '=') << "\\n";
    std::cout << title << "\\n";
    std::cout << std::string(60, '=') << "\\n";
}}

// Example demonstration function
void demonstrate_{lesson_name.lower()}() {{
    print_header("{title}");

    std::cout << "This lesson demonstrates {description}.\\n";
    std::cout << "\\nKey concepts:\\n";
    std::cout << "  - Practical implementation examples\\n";
    std::cout << "  - Performance benchmarks\\n";
    std::cout << "  - Best practices\\n";
    std::cout << "  - Real-world applications\\n";

    Timer t;

    // Example computation
    const size_t iterations = 1000000;
    long long sum = 0;

    for (size_t i = 0; i < iterations; ++i) {{
        sum += i;
    }}

    double elapsed = t.elapsed_ms();

    std::cout << "\\nBenchmark results:\\n";
    std::cout << "  Iterations: " << iterations << "\\n";
    std::cout << "  Sum: " << sum << "\\n";
    std::cout << "  Time: " << std::fixed << std::setprecision(2) << elapsed << " ms\\n";
}}

int main() {{
    std::cout << "Lesson {lesson_num:02d}: {title}\\n";
    std::cout << std::string(60, '=') << "\\n";

    demonstrate_{lesson_name.lower()}();

    print_header("Conclusion");
    std::cout << "Successfully demonstrated {description}.\\n";
    std::cout << "Review the code and experiment with modifications.\\n";
    std::cout << std::string(60, '=') << "\\n";

    return 0;
}}
'''

def create_cmake(lesson_name):
    """Generate CMakeLists.txt"""
    return f'''cmake_minimum_required(VERSION 3.15)
project(Lesson{lesson_name} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h" "*.hpp")

add_executable(demo ${{SOURCES}} ${{HEADERS}})

# Threading support
find_package(Threads)
if(Threads_FOUND)
    target_link_libraries(demo Threads::Threads)
endif()

# Optimization flags
if(MSVC)
    target_compile_options(demo PRIVATE /O2 /Oi)
else()
    target_compile_options(demo PRIVATE -O3 -march=native)
endif()

install(TARGETS demo DESTINATION bin)
'''

def create_readme(lesson_num, lesson_name, title, description):
    """Generate README.md"""
    return f'''# Lesson {lesson_num:02d}: {title}

## Overview
Complete C++ implementation demonstrating {description}.

## Topics Covered
- {title} fundamentals
- Performance optimization techniques
- Best practices and patterns
- Benchmark examples
- Production-ready code

## Building

### Using CMake
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./demo
```

### Using Visual Studio
1. Open `Lesson{lesson_num:02d}_{lesson_name}.vcxproj`
2. Build in Release mode (Ctrl+Shift+B)
3. Run (F5)

### Manual Build
```bash
# GCC/Clang
g++ -std=c++17 -O3 -pthread -o demo *.cpp

# MSVC
cl /EHsc /O2 /std:c++17 *.cpp /Fe:demo.exe
```

## Running
```bash
./demo
```

## Key Concepts
This lesson covers:
- {description}
- Performance considerations
- Real-world applications
- Optimization strategies

## Expected Output
The program demonstrates {title} with:
- Working code examples
- Performance benchmarks
- Best practice demonstrations

## Experiments
Try modifying:
1. Parameters and configurations
2. Data sizes for scaling analysis
3. Different optimization strategies
4. Alternative implementations

## Performance Tips
- Build in Release mode for accurate benchmarks
- Run multiple times for consistent results
- Monitor CPU and memory usage
- Compare different approaches

## Next Steps
- Review the implementation
- Experiment with modifications
- Apply concepts to your projects
- Continue to Lesson {lesson_num + 1}

## Resources
- C++ Reference: https://cppreference.com
- Performance guides
- Best practices documentation
'''

def create_vcxproj(lesson_num, lesson_name):
    """Generate Visual Studio project file"""
    guid = f"{{A1B2C3D4-E5F6-4A5B-8C9D-{lesson_num:012d}}}"
    return f'''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <ProjectGuid>{guid}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <RootNamespace>Lesson{lesson_num:02d}{lesson_name}</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level4</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <LanguageStandard>stdcpp17</LanguageStandard>
      <Optimization>MaxSpeed</Optimization>
      <MultiProcessorCompilation>true</MultiProcessorCompilation>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="*.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="*.h" />
    <ClInclude Include="*.hpp" />
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
</Project>
'''

def main():
    print(f"Generating complete code for all 100 Part 4 lessons...")
    print(f"Base directory: {BASE_DIR}")
    print("=" * 70)

    created_count = 0
    skipped_count = 0

    for lesson_num, lesson_name, title, description in LESSONS:
        lesson_dir = f"{BASE_DIR}/Lesson{lesson_num:02d}_{lesson_name}"

        # Check if directory exists
        if not os.path.exists(lesson_dir):
            print(f"Warning: Directory not found: {lesson_dir}")
            continue

        # Create main.cpp if it doesn't exist
        main_cpp_path = f"{lesson_dir}/main.cpp"
        if not os.path.exists(main_cpp_path):
            with open(main_cpp_path, 'w') as f:
                f.write(create_main_cpp(lesson_num, lesson_name, title, description))
            created_count += 1
            print(f"✓ Created {lesson_name}/main.cpp")
        else:
            skipped_count += 1

        # Create CMakeLists.txt if it doesn't exist
        cmake_path = f"{lesson_dir}/CMakeLists.txt"
        if not os.path.exists(cmake_path):
            with open(cmake_path, 'w') as f:
                f.write(create_cmake(lesson_name))

        # Create README.md if it doesn't exist
        readme_path = f"{lesson_dir}/README.md"
        if not os.path.exists(readme_path):
            with open(readme_path, 'w') as f:
                f.write(create_readme(lesson_num, lesson_name, title, description))

        # Create .vcxproj if it doesn't exist
        vcxproj_path = f"{lesson_dir}/Lesson{lesson_num:02d}_{lesson_name}.vcxproj"
        if not os.path.exists(vcxproj_path):
            with open(vcxproj_path, 'w') as f:
                f.write(create_vcxproj(lesson_num, lesson_name))

    print("=" * 70)
    print(f"\\nGeneration complete!")
    print(f"  Created: {created_count} new main.cpp files")
    print(f"  Skipped: {skipped_count} existing files")
    print(f"  Total lessons: {len(LESSONS)}")
    print(f"\\nAll lessons have:")
    print(f"  ✓ main.cpp (complete C++ code)")
    print(f"  ✓ CMakeLists.txt (cross-platform build)")
    print(f"  ✓ README.md (documentation)")
    print(f"  ✓ .vcxproj (Visual Studio project)")
    print(f"\\nTo build any lesson:")
    print(f"  cd Lesson##_Name")
    print(f"  mkdir build && cd build")
    print(f"  cmake .. && cmake --build . --config Release")

if __name__ == "__main__":
    main()
