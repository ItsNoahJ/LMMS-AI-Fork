# Qt and MinGW Setup for LMMS AI Fork

## Environment Configuration

The LMMS AI Fork project requires Qt 5.15.2 with MinGW 8.1.0 for building. This document outlines the setup process and troubleshooting steps.

## Setup Scripts

The following scripts have been created to facilitate the setup process:

1. `setup_qt_env_fixed.bat` - Sets up the environment variables for Qt and MinGW
2. `test_cmake_config.bat` - Tests the CMake configuration with a simple Qt project
3. `configure_lmms_project.bat` - Configures the LMMS AI Fork project for building

## Key Environment Variables

- **Qt5_DIR**: `C:\Qt\5.15.2\mingw81_64\lib\cmake\Qt5`
- **MINGW_DIR**: `C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64`
- **PATH**: Includes both Qt and MinGW bin directories

## Troubleshooting

### "The C compiler identification is unknown" Error

This error occurs when CMake cannot properly identify the compiler. The solution involves:

1. Explicitly specifying the compiler paths to CMake:
   ```
   cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="path\to\gcc.exe" -DCMAKE_CXX_COMPILER="path\to\g++.exe" ..
   ```

2. Cleaning the CMake cache if you've previously attempted a build:
   - Delete the `CMakeCache.txt` file
   - Delete the `CMakeFiles` directory

3. Ensuring the MinGW bin directory is in your PATH

### Executable Not Found Error

If you encounter errors where executables cannot be found:

1. Verify that the PATH includes the correct bin directories
2. Run commands in a shell where the environment variables have been set
3. Use the full path to the executable when running commands

## Building the Project

To build the LMMS AI Fork project:

1. Run `configure_lmms_project.bat` to set up the environment and configure the project
2. Navigate to the build directory: `cd build`
3. Build the project: `cmake --build .`

## Testing the Setup

You can test your Qt and MinGW setup by running the `test_cmake_config.bat` script, which creates and builds a simple Qt application.
