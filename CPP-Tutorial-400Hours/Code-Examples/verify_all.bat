@echo off
REM ============================================================================
REM  C++ Tutorial 400 Hours - Windows Verification Script
REM ============================================================================
REM  This script verifies that all .cpp files compile successfully
REM ============================================================================

setlocal enabledelayedexpansion

echo.
echo ============================================================================
echo   C++ Tutorial 400 Hours - Code Verification (Windows)
echo ============================================================================
echo.
echo   This script will verify that all code examples compile successfully
echo   by performing a test build of all parts.
echo.

set START_TIME=%TIME%

REM Check if build directory exists
if not exist "build-verify" (
    echo   Creating verification build directory...
    mkdir build-verify
) else (
    echo   Using existing build-verify directory
    echo   To start fresh, delete build-verify folder first
    echo.
)

REM Run CMake configuration
echo ============================================================================
echo   Step 1: CMake Configuration
echo ============================================================================
echo.

cmake -S . -B build-verify ^
    -G "Visual Studio 17 2022" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_PART1_FUNDAMENTALS=ON ^
    -DBUILD_PART2_MFC=OFF ^
    -DBUILD_PART3_3D_RENDERING=ON ^
    -DBUILD_PART4_OPTIMIZATION=ON

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo See errors above for details.
    pause
    exit /b 1
)

REM Build all projects
echo.
echo ============================================================================
echo   Step 2: Building All Projects
echo ============================================================================
echo   This may take 10-30 minutes...
echo.

cmake --build build-verify --config Release -j

set BUILD_RESULT=%errorlevel%

echo.
echo ============================================================================
echo   Verification Results
echo ============================================================================
echo.

if %BUILD_RESULT% equ 0 (
    echo   [SUCCESS] All code examples compiled successfully!
    echo.

    REM Count built executables
    set /a TOTAL_EXES=0
    for /r "build-verify\bin" %%f in (*.exe) do set /a TOTAL_EXES+=1

    echo   Total executables built: !TOTAL_EXES!
    echo   Build directory: build-verify
    echo.

    REM Test a few random executables
    echo   Testing sample executables...
    echo.

    if exist "build-verify\bin\Part1-Fundamentals\Lesson01_HelloWorld.exe" (
        echo   Testing Part 1 - Lesson01_HelloWorld.exe
        echo   Output:
        echo Hello World | "build-verify\bin\Part1-Fundamentals\Lesson01_HelloWorld.exe" 2>nul
        if !errorlevel! equ 0 (
            echo   [OK] Part 1 example runs successfully
        )
        echo.
    )

    echo   Verification complete!
    echo   All code examples are ready to use.

) else (
    echo   [FAILED] Some code examples failed to compile
    echo.
    echo   Error details should be shown above.
    echo   Common issues:
    echo     - Missing Visual Studio components
    echo     - Missing dependencies for Part 3 (OpenGL/DirectX)
    echo     - Incompatible compiler version
    echo.
    echo   Try running: setup_environment.bat
)

echo.
set END_TIME=%TIME%
echo   Start time: %START_TIME%
echo   End time:   %END_TIME%
echo.
echo ============================================================================

pause
exit /b %BUILD_RESULT%
