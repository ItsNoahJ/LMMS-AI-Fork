# LMMS-Magenta Development Environment Setup

## Overview

This guide provides instructions for setting up the development environment for the LMMS-Magenta project. Following these steps will ensure you have all the necessary tools and dependencies to work on the project.

## Prerequisites

### Required Software

- **C++ Development Environment**
  - Windows: Visual Studio 2019 or later with C++ workload
  - macOS: Xcode with command-line tools
  - Linux: GCC 7.0 or later, Clang 6.0 or later

- **CMake 3.10 or later**: For building the project
  - Download from: https://cmake.org/download/

- **Qt 5.15 or later**: For UI components
  - Download from: https://www.qt.io/download

- **Python 3.7 or later**: For scripts and model conversion
  - Download from: https://www.python.org/downloads/

- **Git**: For version control
  - Download from: https://git-scm.com/downloads

### Optional Software

- **Visual Studio Code**: Recommended IDE
  - Download from: https://code.visualstudio.com/

- **Qt Creator**: Alternative IDE for UI development
  - Included with Qt installation

- **Python Virtual Environment**: For isolating Python dependencies
  - `python -m venv venv`

## Getting the Source Code

1. Clone the main repository:
   ```bash
   git clone https://github.com/yourusername/lmms-magenta.git
   cd lmms-magenta
   ```

2. Initialize and update submodules:
   ```bash
   git submodule init
   git submodule update
   ```

## Setting Up Dependencies

### TensorFlow Lite

1. Download TensorFlow Lite:
   ```bash
   cd third_party
   git clone https://github.com/tensorflow/tensorflow.git
   cd tensorflow
   git checkout v2.9.0  # Or the version specified in the project
   ```

2. Build TensorFlow Lite:
   ```bash
   cd tensorflow/lite
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```

### Magenta

1. Set up Python virtual environment:
   ```bash
   cd ../../..  # Back to project root
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

2. Install Magenta:
   ```bash
   pip install magenta
   ```

### LMMS

1. Build LMMS:
   ```bash
   cd lmms
   cmake -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ```

## Building the Project

1. Configure the build:
   ```bash
   cd ..  # Back to project root
   cmake -B build -DCMAKE_BUILD_TYPE=Debug
   ```

2. Build the project:
   ```bash
   cmake --build build
   ```

## Running the Project

1. Run LMMS with AI features:
   ```bash
   ./build/lmms/lmms
   ```

## Development Workflow

### Building Changes

After making changes to the code:

1. Rebuild the project:
   ```bash
   cmake --build build
   ```

### Running Tests

1. Run unit tests:
   ```bash
   cd build
   ctest
   ```

### Debugging

1. Using Visual Studio Code:
   - Open the project in VS Code
   - Set breakpoints in the code
   - Start debugging using the provided launch configuration

2. Using Visual Studio:
   - Open the generated solution file in `build/lmms-magenta.sln`
   - Set breakpoints in the code
   - Start debugging

3. Using Qt Creator:
   - Open the project in Qt Creator
   - Set breakpoints in the code
   - Start debugging

## Project Structure

### Key Directories

- **lmms/**: Core LMMS codebase
- **implementation/**: AI implementation
  - **model_serving/**: Model serving framework
  - **plugins/**: AI plugins
  - **utils/**: Utility classes
  - **ui_components/**: UI components
- **models/**: Pre-trained models
- **third_party/**: Third-party dependencies
- **docs/**: Documentation
- **tests/**: Test suite

### Key Files

- **CMakeLists.txt**: Main build file
- **README.md**: Project overview
- **.context/**: Project context and documentation

## Common Issues and Solutions

### Build Errors

1. **Missing Qt**: Ensure Qt is installed and `CMAKE_PREFIX_PATH` points to it
2. **Missing TensorFlow Lite**: Ensure TensorFlow Lite is built correctly
3. **Compiler Errors**: Ensure you have a compatible C++ compiler

### Runtime Errors

1. **Missing Models**: Ensure models are downloaded and in the correct location
2. **Plugin Loading Errors**: Ensure plugins are built correctly
3. **TensorFlow Lite Errors**: Check TensorFlow Lite integration

## Getting Help

If you encounter issues not covered in this guide:

1. Check the project documentation in the `docs/` directory
2. Search for similar issues in the issue tracker
3. Ask for help in the project's communication channels

## Next Steps

1. Familiarize yourself with the [project architecture](./component_relationships.md)
2. Review the [implementation roadmap](./roadmap.md)
3. Check the [integration plan](./integration_plan.md) for ongoing integration efforts
