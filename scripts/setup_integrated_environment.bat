@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo LMMS-Magenta Integrated Environment Setup
echo ===================================================
echo.

:: Check if Git is installed
where git >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Git is not installed or not in the PATH.
    echo Please install Git from https://git-scm.com/downloads
    exit /b 1
)

:: Check if CMake is installed
where cmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake is not installed or not in the PATH.
    echo Please install CMake from https://cmake.org/download/
    exit /b 1
)

:: Set the project root directory
set "PROJECT_ROOT=%~dp0.."
cd "%PROJECT_ROOT%"

echo Setting up the LMMS-Magenta integrated environment...
echo Project root: %PROJECT_ROOT%
echo.

:: Initialize and update all submodules
echo Initializing and updating Git submodules...
git submodule update --init --recursive
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to initialize and update Git submodules.
    exit /b 1
)
echo Submodules initialized successfully.
echo.

:: Set up VCPKG
echo Setting up VCPKG...
cd "%PROJECT_ROOT%\vcpkg"
call bootstrap-vcpkg.bat
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to bootstrap VCPKG.
    exit /b 1
)
echo VCPKG set up successfully.
echo.

:: Install required packages with VCPKG
echo Installing required packages with VCPKG...
vcpkg install qt5:x64-windows
vcpkg install fftw3:x64-windows
vcpkg install libsndfile:x64-windows
vcpkg install libsamplerate:x64-windows
vcpkg install libogg:x64-windows
vcpkg install libvorbis:x64-windows
vcpkg install flac:x64-windows
echo Required packages installed successfully.
echo.

:: Set up Qt environment
echo Setting up Qt environment...
call "%PROJECT_ROOT%\scripts\setup_qt_env_fixed.bat"
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to set up Qt environment.
    exit /b 1
)
echo Qt environment set up successfully.
echo.

:: Create build directory
echo Creating build directory...
if not exist "%PROJECT_ROOT%\build" mkdir "%PROJECT_ROOT%\build"
cd "%PROJECT_ROOT%\build"
echo Build directory created successfully.
echo.

:: Configure the project with CMake
echo Configuring the project with CMake...
cmake -G "MinGW Makefiles" ^
      -DCMAKE_TOOLCHAIN_FILE="%PROJECT_ROOT%\vcpkg\scripts\buildsystems\vcpkg.cmake" ^
      -DCMAKE_C_COMPILER="%QT_MINGW_PATH%\bin\gcc.exe" ^
      -DCMAKE_CXX_COMPILER="%QT_MINGW_PATH%\bin\g++.exe" ^
      -DCMAKE_MAKE_PROGRAM="%QT_MINGW_PATH%\bin\mingw32-make.exe" ^
      -DCMAKE_BUILD_TYPE=Debug ^
      ..

if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to configure the project with CMake.
    exit /b 1
)
echo Project configured successfully.
echo.

echo ===================================================
echo LMMS-Magenta integrated environment setup complete!
echo ===================================================
echo.
echo To build the project, run:
echo   cd "%PROJECT_ROOT%\build"
echo   mingw32-make
echo.
echo To run the application, run:
echo   cd "%PROJECT_ROOT%\build"
echo   bin\lmms.exe
echo.

endlocal
