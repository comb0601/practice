@echo off
REM ============================================================================
REM Run All Script for Part 1 Fundamentals - Windows
REM ============================================================================
REM This script runs all 83 compiled lesson examples sequentially
REM ============================================================================

setlocal enabledelayedexpansion

echo ============================================================================
echo Running C++ Tutorial Part 1 - Fundamentals
echo Total Lessons: 83 executables
echo ============================================================================
echo.

REM Check if build directory exists
if not exist "build\bin\Release" (
    if not exist "build\bin\Debug" (
        echo ERROR: No compiled executables found
        echo Please run build_all.bat first
        pause
        exit /b 1
    )
    set "BIN_DIR=build\bin\Debug"
    set "CONFIG=Debug"
) else (
    set "BIN_DIR=build\bin\Release"
    set "CONFIG=Release"
)

echo Using %CONFIG% configuration
echo Binary directory: %BIN_DIR%
echo.
echo Press Ctrl+C at any time to stop
echo.
pause

REM Define all lesson executables in order
set LESSONS=^
Lesson01_HelloWorld ^
Lesson01_FirstProgram ^
Lesson02_Variables ^
Lesson02_DataTypes ^
Lesson02_InputOutput ^
Lesson03_Operators ^
Lesson03_Calculator ^
Lesson04_IfElse ^
Lesson04_Grading ^
Lesson05_Switch ^
Lesson06_WhileLoop ^
Lesson07_ForLoop ^
Lesson08_BreakContinue ^
Lesson09_NestedLoops ^
Lesson10_Patterns ^
Lesson11_Functions ^
Lesson12_Parameters ^
Lesson13_Overloading ^
Lesson14_Recursion ^
Lesson15_DefaultArgs ^
Lesson16_Arrays ^
Lesson17_2DArrays ^
Lesson18_ArrayAlgorithms ^
Lesson19_CStrings ^
Lesson20_StdString ^
Lesson21_Pointers ^
Lesson22_PointerArithmetic ^
Lesson23_PointersArrays ^
Lesson24_PointersFunctions ^
Lesson25_DynamicMemory ^
Lesson26_References ^
Lesson27_ConstPointers ^
Lesson28_PointerVsReference ^
Lesson29_NullPointers ^
Lesson30_SmartPointers ^
Lesson31_Classes ^
Lesson32_Constructors ^
Lesson33_MemberFunctions ^
Lesson34_AccessSpecifiers ^
Lesson35_Encapsulation ^
Lesson36_StaticMembers ^
Lesson37_FriendFunctions ^
Lesson38_OperatorOverload1 ^
Lesson39_OperatorOverload2 ^
Lesson40_CopyConstructor ^
Lesson41_Inheritance ^
Lesson42_InheritanceTypes ^
Lesson43_MultipleInheritance ^
Lesson44_Polymorphism ^
Lesson45_AbstractClasses ^
Lesson46_FunctionTemplates ^
Lesson47_ClassTemplates ^
Lesson48_TemplateSpecialization ^
Lesson49_VariadicTemplates ^
Lesson50_STLOverview ^
Lesson51_Vector ^
Lesson52_List ^
Lesson53_Stack ^
Lesson54_Set ^
Lesson55_Map ^
Lesson56_UnorderedMap ^
Lesson57_Algorithms1 ^
Lesson58_Algorithms2 ^
Lesson59_Iterators ^
Lesson60_Lambdas ^
Lesson61_Exceptions ^
Lesson62_CustomExceptions ^
Lesson63_ExceptionSafety ^
Lesson64_FileIO ^
Lesson65_BinaryFileIO ^
Lesson66_FileStreams ^
Lesson67_StringStreams ^
Lesson68_ModernCPP ^
Lesson69_MoveSemantics ^
Lesson70_SmartPointersAdvanced ^
Lesson71_Threads ^
Lesson72_ThreadSync ^
Lesson73_Mutex ^
Lesson74_ConditionVariables ^
Lesson75_AsyncFuture ^
Lesson76_Namespaces ^
Lesson77_Preprocessor ^
Lesson78_Compilation ^
Lesson79_Debugging ^
Lesson80_BestPractices

set count=0
set errors=0

for %%L in (%LESSONS%) do (
    set /a count+=1
    echo.
    echo ============================================================================
    echo [!count!/83] Running %%L.exe
    echo ============================================================================
    echo.

    if exist "%BIN_DIR%\%%L.exe" (
        "%BIN_DIR%\%%L.exe"
        if !errorlevel! neq 0 (
            echo.
            echo WARNING: %%L.exe returned error code !errorlevel!
            set /a errors+=1
        )
    ) else (
        echo ERROR: %%L.exe not found
        set /a errors+=1
    )

    echo.
    echo Press any key to continue to next lesson...
    pause >nul
)

echo.
echo ============================================================================
echo ALL LESSONS COMPLETED!
echo ============================================================================
echo Total lessons run: !count!
echo Errors encountered: !errors!
echo ============================================================================
echo.

pause
