# LMMS-Magenta Comprehensive Integration Plan - Part 2: Directory Structure

## Unified Directory Structure

Based on modern C++ project organization best practices and the specific needs of the LMMS-Magenta project, we propose the following directory structure:

```
lmms-magenta/
├── core/                  # Core LMMS codebase
├── ai/                    # All AI-related code
│   ├── model_serving/     # Model serving framework
│   ├── plugins/           # AI plugins (instruments, effects)
│   ├── utils/             # Common utilities
│   └── ui/                # AI-specific UI components
├── models/                # Pre-trained model files
│   ├── musicvae/
│   ├── groovae/
│   └── melodyrnn/
├── scripts/               # Build, development, and utility scripts
│   ├── setup/             # Environment setup scripts
│   ├── build/             # Build scripts
│   └── tools/             # Development tools
├── external/              # External dependencies (git submodules)
│   ├── tensorflow/
│   ├── magenta/
│   └── other_deps/
├── docs/                  # Documentation
│   ├── dev/               # Developer documentation
│   ├── api/               # API documentation
│   └── user/              # End-user documentation
├── tests/                 # Tests for all components
│   ├── unit/              # Unit tests
│   ├── integration/       # Integration tests
│   └── performance/       # Performance tests
├── include/               # Public headers
│   └── lmms-magenta/      # Project namespace
├── .github/               # GitHub-specific files (CI workflows, etc.)
├── .vscode/               # IDE configuration
└── CMakeLists.txt         # Root CMake file
```

## Directory Structure Rationale

### Core Directory

The `core/` directory contains the core LMMS codebase. This is separated from the AI components to maintain a clear distinction between the original DAW functionality and the AI extensions.

### AI Directory

The `ai/` directory contains all AI-related code, organized into subdirectories:

- `model_serving/`: Framework for loading and running AI models
- `plugins/`: AI-powered instruments and effects
- `utils/`: Common utilities for AI components
- `ui/`: AI-specific UI components

This organization follows the principle of separation of concerns, making it clear which code is responsible for which functionality.

### Models Directory

The `models/` directory contains pre-trained model files, organized by model type. This separation from the code ensures that models can be updated independently of the code.

### Scripts Directory

The `scripts/` directory contains various scripts for building, development, and other utilities. This includes:

- `setup/`: Scripts for setting up the development environment
- `build/`: Scripts for building the project
- `tools/`: Miscellaneous development tools

### External Directory

The `external/` directory contains external dependencies managed as git submodules. This approach, recommended by modern CMake practices, ensures that dependencies are versioned and can be updated in a controlled manner.

### Documentation Directory

The `docs/` directory contains all documentation, organized by audience:

- `dev/`: Documentation for developers
- `api/`: API documentation
- `user/`: End-user documentation

### Tests Directory

The `tests/` directory contains all tests, organized by type:

- `unit/`: Tests for individual components
- `integration/`: Tests for component interactions
- `performance/`: Tests for performance benchmarks

### Include Directory

The `include/` directory contains public headers that should be accessible to other projects. Following modern C++ practices, these are organized under a project namespace directory.

## Migration Strategy

Migrating to this directory structure will involve the following steps:

1. Create the new directory structure
2. Move existing code to the appropriate directories
3. Update include paths and references
4. Update build system to reflect the new structure

This migration should be done incrementally, starting with the most independent components and working towards more integrated ones. Each step should be tested to ensure functionality is preserved.
