@echo off
REM ============================================================================
REM  C++ Tutorial 400 Hours - Windows Environment Setup Script
REM ============================================================================
REM  This script checks and helps setup the build environment for Windows
REM ============================================================================

setlocal enabledelayedexpansion

echo.
echo ============================================================================
echo   C++ Tutorial 400 Hours - Environment Setup (Windows)
echo ============================================================================
echo.
echo   Checking required tools and dependencies...
echo.

set ERROR_COUNT=0
set WARNING_COUNT=0

REM ============================================================================
REM Check for CMake
REM ============================================================================
echo [1/5] Checking for CMake...
where cmake >nul 2>&1
if %errorlevel% equ 0 (
    for /f "tokens=3" %%v in ('cmake --version ^| findstr /r "^cmake version"') do (
        echo   [OK] CMake found: version %%v
    )
) else (
    echo   [MISSING] CMake not found!
    echo   Download from: https://cmake.org/download/
    echo   Or install via: winget install Kitware.CMake
    set /a ERROR_COUNT+=1
)

REM ============================================================================
REM Check for Visual Studio
REM ============================================================================
echo.
echo [2/5] Checking for Visual Studio...

REM Try to find vswhere
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set VS_FOUND=0

if exist "!VSWHERE!" (
    for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
        set "VSINSTALLDIR=%%i"
        set VS_FOUND=1
    )

    for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property displayName`) do (
        set "VSNAME=%%i"
    )

    for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property catalog_productLineVersion`) do (
        set "VSVERSION=%%i"
    )
)

if !VS_FOUND! equ 1 (
    echo   [OK] Visual Studio found: !VSNAME! ^(!VSVERSION!^)
    echo   Path: !VSINSTALLDIR!

    REM Check for MFC component
    "!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.MFC -property installationPath >nul 2>&1
    if !errorlevel! equ 0 (
        echo   [OK] MFC component installed ^(required for Part 2^)
    ) else (
        echo   [WARNING] MFC component not installed
        echo   Part 2 ^(MFC UI Development^) will not be available
        echo   To install MFC, run Visual Studio Installer and add:
        echo   - Desktop development with C++
        echo   - C++ MFC for latest build tools
        set /a WARNING_COUNT+=1
    )
) else (
    echo   [MISSING] Visual Studio not found!
    echo   Download from: https://visualstudio.microsoft.com/downloads/
    echo   Required: Visual Studio 2019 or later with C++ Desktop Development
    set /a ERROR_COUNT+=1
)

REM ============================================================================
REM Check for Git
REM ============================================================================
echo.
echo [3/5] Checking for Git...
where git >nul 2>&1
if %errorlevel% equ 0 (
    for /f "tokens=3" %%v in ('git --version') do (
        echo   [OK] Git found: version %%v
    )
) else (
    echo   [WARNING] Git not found
    echo   Download from: https://git-scm.com/download/win
    echo   Or install via: winget install Git.Git
    set /a WARNING_COUNT+=1
)

REM ============================================================================
REM Check for Python (for run_examples.py)
REM ============================================================================
echo.
echo [4/5] Checking for Python...
where python >nul 2>&1
if %errorlevel% equ 0 (
    for /f "tokens=2" %%v in ('python --version 2^>^&1') do (
        echo   [OK] Python found: version %%v
    )
) else (
    echo   [WARNING] Python not found
    echo   Required for run_examples.py interactive script
    echo   Download from: https://www.python.org/downloads/
    echo   Or install via: winget install Python.Python.3.11
    set /a WARNING_COUNT+=1
)

REM ============================================================================
REM Check disk space
REM ============================================================================
echo.
echo [5/5] Checking disk space...
for /f "tokens=3" %%a in ('dir /-c ^| findstr /C:"bytes free"') do set FREE_SPACE=%%a
set FREE_GB=!FREE_SPACE:~0,-9!
if !FREE_GB! gtr 5 (
    echo   [OK] Sufficient disk space: ~!FREE_GB! GB free
) else (
    echo   [WARNING] Low disk space: ~!FREE_GB! GB free
    echo   Recommended: At least 5 GB for build artifacts
    set /a WARNING_COUNT+=1
)

REM ============================================================================
REM Print summary
REM ============================================================================
echo.
echo ============================================================================
echo   Environment Check Summary
echo ============================================================================
echo.

if !ERROR_COUNT! equ 0 (
    echo   [OK] All required tools are installed!
    echo.
    if !WARNING_COUNT! gtr 0 (
        echo   [INFO] !WARNING_COUNT! optional warning^(s^) - see above
        echo.
    )
    echo   You can now build the project:
    echo     build_all_windows.bat
    echo.
    echo   Or build specific parts:
    echo     cmake -S . -B build -DBUILD_PART1_FUNDAMENTALS=ON
    echo     cmake --build build --config Release
) else (
    echo   [ERROR] !ERROR_COUNT! required tool^(s^) missing!
    echo   [WARNING] !WARNING_COUNT! optional warning^(s^)
    echo.
    echo   Please install the missing tools and run this script again.
)

echo ============================================================================
echo.

REM ============================================================================
REM Optional: Offer to install dependencies
REM ============================================================================
if !ERROR_COUNT! gtr 0 (
    echo.
    echo Would you like to attempt automatic installation using winget? ^(Y/N^)
    set /p INSTALL_CHOICE=

    if /i "!INSTALL_CHOICE!"=="Y" (
        echo.
        echo Installing dependencies...

        where cmake >nul 2>&1
        if %errorlevel% neq 0 (
            echo Installing CMake...
            winget install Kitware.CMake
        )

        where python >nul 2>&1
        if %errorlevel% neq 0 (
            echo Installing Python...
            winget install Python.Python.3.11
        )

        echo.
        echo Installation complete! Please restart this terminal and run setup_environment.bat again.
        echo.
        echo Note: Visual Studio must be installed manually from:
        echo https://visualstudio.microsoft.com/downloads/
    )
)

pause
exit /b !ERROR_COUNT!
