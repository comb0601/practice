@echo off
REM ============================================================================
REM  C++ Tutorial 400 Hours - Complete Windows Build Script
REM ============================================================================
REM  This script builds ALL parts of the C++ tutorial course
REM  Parts: 1=Fundamentals, 2=MFC, 3=3D Rendering, 4=Optimization
REM ============================================================================

setlocal enabledelayedexpansion

echo.
echo ============================================================================
echo   C++ Tutorial 400 Hours - Complete Build System
echo ============================================================================
echo.
echo   Building ALL 370+ lessons across 4 major parts
echo.
echo   Part 1: C++ Fundamentals (85 lessons)
echo   Part 2: MFC UI Development (90 lessons) - Optional
echo   Part 3: 3D Rendering (100 lessons)
echo   Part 4: Optimization ^& Advanced (100 lessons)
echo.
echo ============================================================================
echo.

REM Check for CMake
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found! Please install CMake or run setup_environment.bat
    pause
    exit /b 1
)

REM Check for Visual Studio
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [WARNING] Visual Studio compiler not found in PATH
    echo [INFO] Attempting to locate Visual Studio...

    REM Try to find vswhere
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "!VSWHERE!" (
        for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
            set "VSINSTALLDIR=%%i"
        )

        if exist "!VSINSTALLDIR!\Common7\Tools\VsDevCmd.bat" (
            echo [INFO] Found Visual Studio at: !VSINSTALLDIR!
            echo [INFO] Setting up Visual Studio environment...
            call "!VSINSTALLDIR!\Common7\Tools\VsDevCmd.bat"
        )
    )
)

REM Parse command line arguments
set BUILD_TYPE=Release
set BUILD_MFC=OFF
set GENERATOR=Visual Studio 17 2022

:parse_args
if "%~1"=="" goto done_parsing
if /i "%~1"=="Debug" set BUILD_TYPE=Debug
if /i "%~1"=="Release" set BUILD_TYPE=Release
if /i "%~1"=="MFC" set BUILD_MFC=ON
if /i "%~1"=="-h" goto show_help
if /i "%~1"=="--help" goto show_help
shift
goto parse_args

:done_parsing

echo.
echo [CONFIGURATION]
echo   Build Type: %BUILD_TYPE%
echo   Include MFC: %BUILD_MFC%
echo   Generator: %GENERATOR%
echo.

REM Create build directory
set BUILD_DIR=build-windows
echo [STEP 1/4] Creating build directory: %BUILD_DIR%
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM Configure CMake
echo.
echo [STEP 2/4] Configuring CMake...
echo ============================================================================

cmake -S . -B "%BUILD_DIR%" ^
    -G "%GENERATOR%" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DBUILD_PART1_FUNDAMENTALS=ON ^
    -DBUILD_PART2_MFC=%BUILD_MFC% ^
    -DBUILD_PART3_3D_RENDERING=ON ^
    -DBUILD_PART4_OPTIMIZATION=ON ^
    -DBUILD_ALL_PARTS=ON

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

REM Build
echo.
echo [STEP 3/4] Building all projects...
echo ============================================================================
echo   This may take 10-30 minutes depending on your system
echo ============================================================================

cmake --build "%BUILD_DIR%" --config %BUILD_TYPE% -j

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

REM Organize output
echo.
echo [STEP 4/4] Organizing build output...
echo ============================================================================

set OUTPUT_DIR=output-windows-%BUILD_TYPE%
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

echo   Copying executables to %OUTPUT_DIR%...
xcopy /Y /E /I "%BUILD_DIR%\bin\*" "%OUTPUT_DIR%\bin\" >nul 2>&1
xcopy /Y /E /I "%BUILD_DIR%\lib\*" "%OUTPUT_DIR%\lib\" >nul 2>&1

REM Generate summary
echo.
echo ============================================================================
echo   BUILD COMPLETE!
echo ============================================================================
echo.
echo   Build Type: %BUILD_TYPE%
echo   Build Directory: %BUILD_DIR%
echo   Output Directory: %OUTPUT_DIR%
echo.

REM Count executables
set /a TOTAL_EXES=0
for /r "%OUTPUT_DIR%\bin" %%f in (*.exe) do set /a TOTAL_EXES+=1

echo   Total Executables Built: %TOTAL_EXES%
echo.
echo   To run examples:
echo     1. Navigate to %OUTPUT_DIR%\bin
echo     2. Run any lesson executable
echo     3. Or use: python run_examples.py
echo.
echo ============================================================================

pause
exit /b 0

:show_help
echo.
echo Usage: build_all_windows.bat [options]
echo.
echo Options:
echo   Debug       Build in Debug mode
echo   Release     Build in Release mode (default)
echo   MFC         Include MFC examples (requires Visual Studio with MFC)
echo   -h, --help  Show this help message
echo.
echo Examples:
echo   build_all_windows.bat
echo   build_all_windows.bat Debug
echo   build_all_windows.bat Release MFC
echo.
exit /b 0
