# LMMS AI Fork

This project integrates LMMS (Linux MultiMedia Studio) with Google's Magenta AI music generation capabilities to create an AI-assisted music production platform.

## Project Structure

The project is organized as a cohesive integrated system with the following components:

- **LMMS**: Core DAW functionality (submodule)
- **Magenta**: Google's machine learning models for music generation (submodule)
- **Magenta-JS**: JavaScript implementation of Magenta (submodule)
- **TensorFlow**: Machine learning framework (submodule)
- **VCPKG**: C++ package manager (submodule)
- **AI Components**: Custom AI implementation for LMMS
  - Model Serving: Framework for serving AI models
  - Plugins: AI-powered instruments and effects
  - Utils: Utility classes for AI components
  - UI: User interface components for AI features

## Getting Started

### Prerequisites

- Git
- CMake 3.16 or higher
- Qt 5.15.2
- MinGW 8.1.0 (on Windows)
- Python 3.8 or higher (for Magenta)

### Setting Up the Development Environment

1. Clone the repository:
   ```
   git clone https://github.com/ItsNoahJ/LMMS-AI-Fork.git
   cd LMMS-AI-Fork
   ```

2. Run the integrated environment setup script:
   ```
   # On Windows
   scripts\setup_integrated_environment.bat
   
   # On Linux/macOS
   ./scripts/setup_integrated_environment.sh
   ```

3. Build the project:
   ```
   cd build
   cmake --build .
   ```

4. Run the application:
   ```
   # On Windows
   bin\lmms.exe
   
   # On Linux/macOS
   ./bin/lmms
   ```

## Project Integration

This project integrates multiple components into a cohesive whole through:

1. **Unified Build System**: A centralized CMake-based build system that builds all components together
2. **Shared Component Registry**: Components register themselves and discover other components
3. **Unified Interface Layer**: A consistent API layer that abstracts underlying implementations
4. **Centralized Configuration Management**: A central system for managing settings across components

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under multiple licenses:
- LMMS: GPL-2.0
- Magenta: Apache-2.0
- TensorFlow: Apache-2.0
- Custom code: GPL-2.0

See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- LMMS Team for the core DAW
- Google Magenta Team for the AI music generation capabilities
- TensorFlow Team for the machine learning framework
