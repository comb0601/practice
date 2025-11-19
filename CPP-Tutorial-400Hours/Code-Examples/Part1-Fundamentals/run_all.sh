#!/bin/bash
# ============================================================================
# Run All Script for Part 1 Fundamentals - Linux/Mac
# ============================================================================
# This script runs all 83 compiled lesson examples sequentially
# ============================================================================

echo "============================================================================"
echo "Running C++ Tutorial Part 1 - Fundamentals"
echo "Total Lessons: 83 executables"
echo "============================================================================"
echo ""

# Check if build directory exists
if [ ! -d "build/bin" ]; then
    echo "ERROR: No compiled executables found"
    echo "Please run ./build_all.sh first"
    exit 1
fi

BIN_DIR="build/bin"

echo "Binary directory: $BIN_DIR"
echo ""
echo "Press Ctrl+C at any time to stop"
echo ""
read -p "Press Enter to start..."

# Define all lesson executables in order
LESSONS=(
    "Lesson01_HelloWorld"
    "Lesson01_FirstProgram"
    "Lesson02_Variables"
    "Lesson02_DataTypes"
    "Lesson02_InputOutput"
    "Lesson03_Operators"
    "Lesson03_Calculator"
    "Lesson04_IfElse"
    "Lesson04_Grading"
    "Lesson05_Switch"
    "Lesson06_WhileLoop"
    "Lesson07_ForLoop"
    "Lesson08_BreakContinue"
    "Lesson09_NestedLoops"
    "Lesson10_Patterns"
    "Lesson11_Functions"
    "Lesson12_Parameters"
    "Lesson13_Overloading"
    "Lesson14_Recursion"
    "Lesson15_DefaultArgs"
    "Lesson16_Arrays"
    "Lesson17_2DArrays"
    "Lesson18_ArrayAlgorithms"
    "Lesson19_CStrings"
    "Lesson20_StdString"
    "Lesson21_Pointers"
    "Lesson22_PointerArithmetic"
    "Lesson23_PointersArrays"
    "Lesson24_PointersFunctions"
    "Lesson25_DynamicMemory"
    "Lesson26_References"
    "Lesson27_ConstPointers"
    "Lesson28_PointerVsReference"
    "Lesson29_NullPointers"
    "Lesson30_SmartPointers"
    "Lesson31_Classes"
    "Lesson32_Constructors"
    "Lesson33_MemberFunctions"
    "Lesson34_AccessSpecifiers"
    "Lesson35_Encapsulation"
    "Lesson36_StaticMembers"
    "Lesson37_FriendFunctions"
    "Lesson38_OperatorOverload1"
    "Lesson39_OperatorOverload2"
    "Lesson40_CopyConstructor"
    "Lesson41_Inheritance"
    "Lesson42_InheritanceTypes"
    "Lesson43_MultipleInheritance"
    "Lesson44_Polymorphism"
    "Lesson45_AbstractClasses"
    "Lesson46_FunctionTemplates"
    "Lesson47_ClassTemplates"
    "Lesson48_TemplateSpecialization"
    "Lesson49_VariadicTemplates"
    "Lesson50_STLOverview"
    "Lesson51_Vector"
    "Lesson52_List"
    "Lesson53_Stack"
    "Lesson54_Set"
    "Lesson55_Map"
    "Lesson56_UnorderedMap"
    "Lesson57_Algorithms1"
    "Lesson58_Algorithms2"
    "Lesson59_Iterators"
    "Lesson60_Lambdas"
    "Lesson61_Exceptions"
    "Lesson62_CustomExceptions"
    "Lesson63_ExceptionSafety"
    "Lesson64_FileIO"
    "Lesson65_BinaryFileIO"
    "Lesson66_FileStreams"
    "Lesson67_StringStreams"
    "Lesson68_ModernCPP"
    "Lesson69_MoveSemantics"
    "Lesson70_SmartPointersAdvanced"
    "Lesson71_Threads"
    "Lesson72_ThreadSync"
    "Lesson73_Mutex"
    "Lesson74_ConditionVariables"
    "Lesson75_AsyncFuture"
    "Lesson76_Namespaces"
    "Lesson77_Preprocessor"
    "Lesson78_Compilation"
    "Lesson79_Debugging"
    "Lesson80_BestPractices"
)

count=0
errors=0
total=${#LESSONS[@]}

for lesson in "${LESSONS[@]}"; do
    ((count++))
    echo ""
    echo "============================================================================"
    echo "[$count/$total] Running $lesson"
    echo "============================================================================"
    echo ""

    if [ -f "$BIN_DIR/$lesson" ]; then
        "$BIN_DIR/$lesson"
        exit_code=$?
        if [ $exit_code -ne 0 ]; then
            echo ""
            echo "WARNING: $lesson returned error code $exit_code"
            ((errors++))
        fi
    else
        echo "ERROR: $lesson not found"
        ((errors++))
    fi

    echo ""
    read -p "Press Enter to continue to next lesson..."
done

echo ""
echo "============================================================================"
echo "ALL LESSONS COMPLETED!"
echo "============================================================================"
echo "Total lessons run: $count"
echo "Errors encountered: $errors"
echo "============================================================================"
echo ""
