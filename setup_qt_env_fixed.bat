@echo off
REM Setup Qt and MinGW environment for LMMS AI Fork

REM Set Qt directory
set QT_DIR=C:\Qt\5.15.2\mingw81_64
set Qt5_DIR=%QT_DIR%\lib\cmake\Qt5

REM Set MinGW directory
set MINGW_DIR=C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64

REM Add Qt and MinGW to PATH
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%

REM Set CMake variables explicitly
set CMAKE_C_COMPILER=%MINGW_DIR%\bin\gcc.exe
set CMAKE_CXX_COMPILER=%MINGW_DIR%\bin\g++.exe

REM Verify the environment
echo Qt5_DIR is set to: %Qt5_DIR%
echo MinGW directory is: %MINGW_DIR%
echo PATH includes Qt and MinGW: %PATH%

REM Test compiler availability
echo Testing compiler availability...
where gcc
where g++
where qmake

echo Environment setup complete.
echo.
echo To use this environment, run:
echo cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="%MINGW_DIR%\bin\gcc.exe" -DCMAKE_CXX_COMPILER="%MINGW_DIR%\bin\g++.exe" ..
echo.
