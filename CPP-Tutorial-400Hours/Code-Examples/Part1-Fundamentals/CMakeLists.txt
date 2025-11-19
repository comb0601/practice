cmake_minimum_required(VERSION 3.10)
project(CPP_Tutorial_Part1_Fundamentals)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable all warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For multi-configuration generators (like Visual Studio)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
endforeach()

# Create executables for all lessons
# Lesson 01 - Introduction
add_executable(Lesson01_HelloWorld Lesson01_HelloWorld.cpp)
add_executable(Lesson01_FirstProgram Lesson01_FirstProgram.cpp)

# Lesson 02 - Variables and Data Types
add_executable(Lesson02_Variables Lesson02_Variables.cpp)
add_executable(Lesson02_DataTypes Lesson02_DataTypes.cpp)
add_executable(Lesson02_InputOutput Lesson02_InputOutput.cpp)

# Lesson 03 - Operators
add_executable(Lesson03_Operators Lesson03_Operators.cpp)
add_executable(Lesson03_Calculator Lesson03_Calculator.cpp)

# Lesson 04 - Conditionals
add_executable(Lesson04_IfElse Lesson04_IfElse.cpp)
add_executable(Lesson04_Grading Lesson04_Grading.cpp)

# Lesson 05 - Switch Statement
add_executable(Lesson05_Switch Lesson05_Switch.cpp)

# Lesson 06 - While Loops
add_executable(Lesson06_WhileLoop Lesson06_WhileLoop.cpp)

# Lesson 07 - For Loops
add_executable(Lesson07_ForLoop Lesson07_ForLoop.cpp)

# Lesson 08 - Break and Continue
add_executable(Lesson08_BreakContinue Lesson08_BreakContinue.cpp)

# Lesson 09 - Nested Loops
add_executable(Lesson09_NestedLoops Lesson09_NestedLoops.cpp)

# Lesson 10 - Pattern Printing
add_executable(Lesson10_Patterns Lesson10_Patterns.cpp)

# Lesson 11 - Functions
add_executable(Lesson11_Functions Lesson11_Functions.cpp)

# Lesson 12 - Function Parameters
add_executable(Lesson12_Parameters Lesson12_Parameters.cpp)

# Lesson 13 - Function Overloading
add_executable(Lesson13_Overloading Lesson13_Overloading.cpp)

# Lesson 14 - Recursion
add_executable(Lesson14_Recursion Lesson14_Recursion.cpp)

# Lesson 15 - Default Arguments
add_executable(Lesson15_DefaultArgs Lesson15_DefaultArgs.cpp)

# Lesson 16 - Arrays
add_executable(Lesson16_Arrays Lesson16_Arrays.cpp)

# Lesson 17 - 2D Arrays
add_executable(Lesson17_2DArrays Lesson17_2DArrays.cpp)

# Lesson 18 - Array Algorithms
add_executable(Lesson18_ArrayAlgorithms Lesson18_ArrayAlgorithms.cpp)

# Lesson 19 - C-Style Strings
add_executable(Lesson19_CStrings Lesson19_CStrings.cpp)

# Lesson 20 - std::string
add_executable(Lesson20_StdString Lesson20_StdString.cpp)

# Lesson 21 - Pointers
add_executable(Lesson21_Pointers Lesson21_Pointers.cpp)

# Lesson 22 - Pointer Arithmetic
add_executable(Lesson22_PointerArithmetic Lesson22_PointerArithmetic.cpp)

# Lesson 23 - Pointers and Arrays
add_executable(Lesson23_PointersArrays Lesson23_PointersArrays.cpp)

# Lesson 24 - Pointers and Functions
add_executable(Lesson24_PointersFunctions Lesson24_PointersFunctions.cpp)

# Lesson 25 - Dynamic Memory
add_executable(Lesson25_DynamicMemory Lesson25_DynamicMemory.cpp)

# Lesson 26 - References
add_executable(Lesson26_References Lesson26_References.cpp)

# Lesson 27 - Const Pointers
add_executable(Lesson27_ConstPointers Lesson27_ConstPointers.cpp)

# Lesson 28 - Pointer vs Reference
add_executable(Lesson28_PointerVsReference Lesson28_PointerVsReference.cpp)

# Lesson 29 - Null Pointers
add_executable(Lesson29_NullPointers Lesson29_NullPointers.cpp)

# Lesson 30 - Smart Pointers
add_executable(Lesson30_SmartPointers Lesson30_SmartPointers.cpp)

# Lesson 31 - Classes
add_executable(Lesson31_Classes Lesson31_Classes.cpp)

# Lesson 32 - Constructors
add_executable(Lesson32_Constructors Lesson32_Constructors.cpp)

# Lesson 33 - Member Functions
add_executable(Lesson33_MemberFunctions Lesson33_MemberFunctions.cpp)

# Lesson 34 - Access Specifiers
add_executable(Lesson34_AccessSpecifiers Lesson34_AccessSpecifiers.cpp)

# Lesson 35 - Encapsulation
add_executable(Lesson35_Encapsulation Lesson35_Encapsulation.cpp)

# Lesson 36 - Static Members
add_executable(Lesson36_StaticMembers Lesson36_StaticMembers.cpp)

# Lesson 37 - Friend Functions
add_executable(Lesson37_FriendFunctions Lesson37_FriendFunctions.cpp)

# Lesson 38 - Operator Overloading Part 1
add_executable(Lesson38_OperatorOverload1 Lesson38_OperatorOverload1.cpp)

# Lesson 39 - Operator Overloading Part 2
add_executable(Lesson39_OperatorOverload2 Lesson39_OperatorOverload2.cpp)

# Lesson 40 - Copy Constructor
add_executable(Lesson40_CopyConstructor Lesson40_CopyConstructor.cpp)

# Lesson 41 - Inheritance
add_executable(Lesson41_Inheritance Lesson41_Inheritance.cpp)

# Lesson 42 - Inheritance Types
add_executable(Lesson42_InheritanceTypes Lesson42_InheritanceTypes.cpp)

# Lesson 43 - Multiple Inheritance
add_executable(Lesson43_MultipleInheritance Lesson43_MultipleInheritance.cpp)

# Lesson 44 - Polymorphism
add_executable(Lesson44_Polymorphism Lesson44_Polymorphism.cpp)

# Lesson 45 - Abstract Classes
add_executable(Lesson45_AbstractClasses Lesson45_AbstractClasses.cpp)

# Lesson 46 - Function Templates
add_executable(Lesson46_FunctionTemplates Lesson46_FunctionTemplates.cpp)

# Lesson 47 - Class Templates
add_executable(Lesson47_ClassTemplates Lesson47_ClassTemplates.cpp)

# Lesson 48 - Template Specialization
add_executable(Lesson48_TemplateSpecialization Lesson48_TemplateSpecialization.cpp)

# Lesson 49 - Variadic Templates
add_executable(Lesson49_VariadicTemplates Lesson49_VariadicTemplates.cpp)

# Lesson 50 - STL Overview
add_executable(Lesson50_STLOverview Lesson50_STLOverview.cpp)

# Lesson 51 - Vector
add_executable(Lesson51_Vector Lesson51_Vector.cpp)

# Lesson 52 - List
add_executable(Lesson52_List Lesson52_List.cpp)

# Lesson 53 - Stack
add_executable(Lesson53_Stack Lesson53_Stack.cpp)

# Lesson 54 - Set
add_executable(Lesson54_Set Lesson54_Set.cpp)

# Lesson 55 - Map
add_executable(Lesson55_Map Lesson55_Map.cpp)

# Lesson 56 - Unordered Map
add_executable(Lesson56_UnorderedMap Lesson56_UnorderedMap.cpp)

# Lesson 57 - Algorithms Part 1
add_executable(Lesson57_Algorithms1 Lesson57_Algorithms1.cpp)

# Lesson 58 - Algorithms Part 2
add_executable(Lesson58_Algorithms2 Lesson58_Algorithms2.cpp)

# Lesson 59 - Iterators
add_executable(Lesson59_Iterators Lesson59_Iterators.cpp)

# Lesson 60 - Lambdas
add_executable(Lesson60_Lambdas Lesson60_Lambdas.cpp)

# Lesson 61 - Exceptions
add_executable(Lesson61_Exceptions Lesson61_Exceptions.cpp)

# Lesson 62 - Custom Exceptions
add_executable(Lesson62_CustomExceptions Lesson62_CustomExceptions.cpp)

# Lesson 63 - Exception Safety
add_executable(Lesson63_ExceptionSafety Lesson63_ExceptionSafety.cpp)

# Lesson 64 - File I/O
add_executable(Lesson64_FileIO Lesson64_FileIO.cpp)

# Lesson 65 - Binary File I/O
add_executable(Lesson65_BinaryFileIO Lesson65_BinaryFileIO.cpp)

# Lesson 66 - File Streams
add_executable(Lesson66_FileStreams Lesson66_FileStreams.cpp)

# Lesson 67 - String Streams
add_executable(Lesson67_StringStreams Lesson67_StringStreams.cpp)

# Lesson 68 - Modern C++
add_executable(Lesson68_ModernCPP Lesson68_ModernCPP.cpp)

# Lesson 69 - Move Semantics
add_executable(Lesson69_MoveSemantics Lesson69_MoveSemantics.cpp)

# Lesson 70 - Smart Pointers Advanced
add_executable(Lesson70_SmartPointersAdvanced Lesson70_SmartPointersAdvanced.cpp)

# Lesson 71 - Threads
add_executable(Lesson71_Threads Lesson71_Threads.cpp)
target_link_libraries(Lesson71_Threads pthread)

# Lesson 72 - Thread Synchronization
add_executable(Lesson72_ThreadSync Lesson72_ThreadSync.cpp)
target_link_libraries(Lesson72_ThreadSync pthread)

# Lesson 73 - Mutex
add_executable(Lesson73_Mutex Lesson73_Mutex.cpp)
target_link_libraries(Lesson73_Mutex pthread)

# Lesson 74 - Condition Variables
add_executable(Lesson74_ConditionVariables Lesson74_ConditionVariables.cpp)
target_link_libraries(Lesson74_ConditionVariables pthread)

# Lesson 75 - Async and Future
add_executable(Lesson75_AsyncFuture Lesson75_AsyncFuture.cpp)
target_link_libraries(Lesson75_AsyncFuture pthread)

# Lesson 76 - Namespaces
add_executable(Lesson76_Namespaces Lesson76_Namespaces.cpp)

# Lesson 77 - Preprocessor
add_executable(Lesson77_Preprocessor Lesson77_Preprocessor.cpp)

# Lesson 78 - Compilation Process
add_executable(Lesson78_Compilation Lesson78_Compilation.cpp)

# Lesson 79 - Debugging
add_executable(Lesson79_Debugging Lesson79_Debugging.cpp)

# Lesson 80 - Best Practices
add_executable(Lesson80_BestPractices Lesson80_BestPractices.cpp)

# Print summary message
message(STATUS "==================================================")
message(STATUS "C++ Tutorial Part 1 - Fundamentals")
message(STATUS "Total: 83 lesson executables configured")
message(STATUS "C++ Standard: C++17")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "==================================================")
