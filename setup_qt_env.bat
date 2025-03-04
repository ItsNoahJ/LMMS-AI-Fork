@echo off
REM This script sets up the Qt environment variables for building LMMS AI Fork

echo Setting up Qt environment variables...

REM Qt installed via Chocolatey is at C:\Qt\5.15.2\mingw81_64
set "QT_PATH=C:\Qt\5.15.2\mingw81_64"

REM MinGW path from Chocolatey installation
set "MINGW_PATH=C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64"

if exist "%QT_PATH%\lib\cmake\Qt5" (
    echo Found Qt5 at: %QT_PATH%
    
    REM Set Qt5_DIR to point to the Qt5Config.cmake file
    set "Qt5_DIR=%QT_PATH%\lib\cmake\Qt5"
    echo Qt5_DIR is now set to: %Qt5_DIR%
    
    REM Add Qt bin directory to PATH
    set "PATH=%QT_PATH%\bin;%PATH%"
    echo Added Qt bin directory to PATH
    
    REM Add MinGW bin directory to PATH if it exists
    if exist "%MINGW_PATH%\bin" (
        set "PATH=%MINGW_PATH%\bin;%PATH%"
        echo Added MinGW bin directory to PATH: %MINGW_PATH%\bin
    ) else (
        echo WARNING: MinGW path not found at %MINGW_PATH%
    )
    
    REM Test if qmake is accessible
    "%QT_PATH%\bin\qmake.exe" --version > nul 2>&1
    if %ERRORLEVEL% == 0 (
        echo Successfully verified qmake is working!
    ) else (
        echo WARNING: qmake was not found in the PATH. You may need to restart your terminal.
    )
    
    REM Test if g++ is accessible
    "%MINGW_PATH%\bin\g++.exe" --version > nul 2>&1
    if %ERRORLEVEL% == 0 (
        echo Successfully verified g++ is working!
    ) else (
        echo WARNING: g++ was not found in the PATH. You may need to restart your terminal.
    )
    
    echo.
    echo Qt and MinGW environment setup complete!
    echo You can now build the project with CMake
) else (
    echo.
    echo ERROR: Could not find Qt5 installation at %QT_PATH%.
    echo Please install Qt 5.15.2 or set Qt5_DIR manually to your Qt installation.
    echo Example: set Qt5_DIR=C:\Qt\5.15.2\mingw81_64\lib\cmake\Qt5
)

echo.
echo To manually make these settings permanent, run:
echo setx Qt5_DIR "%Qt5_DIR%"
echo setx PATH "%MINGW_PATH%\bin;%QT_PATH%\bin;%PATH%"
echo.
