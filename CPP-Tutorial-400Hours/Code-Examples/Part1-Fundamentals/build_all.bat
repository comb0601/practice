@echo off
REM ============================================================================
REM Build All Script for Part 1 Fundamentals - Windows
REM ============================================================================
REM This script builds all 83 C++ lesson examples using CMake and Visual Studio
REM ============================================================================

echo ============================================================================
echo Building C++ Tutorial Part 1 - Fundamentals
echo Total Lessons: 83 executables
echo ============================================================================
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build
cd build

echo [1/3] Configuring CMake for Visual Studio...
echo.

REM Configure with CMake (using Visual Studio generator)
cmake .. -G "Visual Studio 16 2019" -A x64
if %errorlevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed
    echo If you don't have Visual Studio 2019, try:
    echo   - Visual Studio 17 2022: cmake .. -G "Visual Studio 17 2022" -A x64
    echo   - Visual Studio 15 2017: cmake .. -G "Visual Studio 15 2017" -A x64
    pause
    exit /b 1
)

echo.
echo [2/3] Building Debug configuration...
echo.

REM Build Debug configuration
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo.
    echo ERROR: Debug build failed
    pause
    exit /b 1
)

echo.
echo [3/3] Building Release configuration...
echo.

REM Build Release configuration
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo.
    echo ERROR: Release build failed
    pause
    exit /b 1
)

cd ..

echo.
echo ============================================================================
echo BUILD SUCCESSFUL!
echo ============================================================================
echo Debug executables:   build\bin\Debug\
echo Release executables: build\bin\Release\
echo.
echo To run all examples, execute: run_all.bat
echo To run a specific example: build\bin\Debug\Lesson01_HelloWorld.exe
echo ============================================================================
echo.

pause
