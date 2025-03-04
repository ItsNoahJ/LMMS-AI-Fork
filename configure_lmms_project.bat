@echo off
REM LMMS AI Fork Project Configuration Script
REM This script sets up the environment and configures the project for building

echo ===== LMMS AI Fork Project Configuration =====
echo.

REM First, run the environment setup script
call setup_qt_env_fixed.bat

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure the project with CMake
echo Configuring LMMS AI Fork project with CMake...
cmake -G "MinGW Makefiles" ^
      -DCMAKE_C_COMPILER="%MINGW_DIR%\bin\gcc.exe" ^
      -DCMAKE_CXX_COMPILER="%MINGW_DIR%\bin\g++.exe" ^
      -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
      ..

REM Check if CMake configuration was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===== Configuration Successful =====
    echo.
    echo The project has been configured successfully.
    echo.
    echo To build the project, run:
    echo   cd build
    echo   cmake --build .
    echo.
    echo To build a specific target, run:
    echo   cmake --build . --target [target_name]
    echo.
) else (
    echo.
    echo ===== Configuration Failed =====
    echo.
    echo The project configuration failed with error code %ERRORLEVEL%.
    echo Please check the error messages above for more information.
    echo.
)

REM Return to the original directory
cd ..

echo Configuration process completed.
