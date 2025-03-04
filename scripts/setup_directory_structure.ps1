# LMMS-Magenta Directory Structure Setup Script
# This script creates the directory structure as defined in the integration plan

# Ensure the scripts directory exists
$scriptsDir = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not (Test-Path $scriptsDir)) {
    New-Item -ItemType Directory -Path $scriptsDir -Force
}

# Root directory is the parent of the scripts directory
$rootDir = Split-Path -Parent $scriptsDir

# Function to create directory if it doesn't exist
function EnsureDirectory {
    param (
        [string]$path
    )
    
    if (-not (Test-Path $path)) {
        Write-Host "Creating directory: $path"
        New-Item -ItemType Directory -Path $path -Force | Out-Null
    } else {
        Write-Host "Directory already exists: $path"
    }
}

# Function to create a README file in a directory
function CreateReadme {
    param (
        [string]$directory,
        [string]$content
    )
    
    $readmePath = Join-Path -Path $directory -ChildPath "README.md"
    if (-not (Test-Path $readmePath)) {
        Write-Host "Creating README in: $directory"
        Set-Content -Path $readmePath -Value $content
    } else {
        Write-Host "README already exists in: $directory"
    }
}

# Create main directories
Write-Host "Setting up LMMS-Magenta directory structure..."

# Core directories
EnsureDirectory "$rootDir\core\src"
EnsureDirectory "$rootDir\core\include"
CreateReadme "$rootDir\core" "# Core Components

This directory contains the core components of the LMMS-Magenta integration.

## Structure

- `src/`: Source code for core components
- `include/`: Header files for core components
"

# AI directories
EnsureDirectory "$rootDir\ai\model_serving\src"
EnsureDirectory "$rootDir\ai\model_serving\include"
EnsureDirectory "$rootDir\ai\plugins\src"
EnsureDirectory "$rootDir\ai\plugins\include"
EnsureDirectory "$rootDir\ai\utils\src"
EnsureDirectory "$rootDir\ai\utils\include"
EnsureDirectory "$rootDir\ai\ui\src"
EnsureDirectory "$rootDir\ai\ui\include"
CreateReadme "$rootDir\ai" "# AI Components

This directory contains the AI components of the LMMS-Magenta integration.

## Structure

- `model_serving/`: Model loading and inference components
- `plugins/`: AI-enabled LMMS plugins
- `utils/`: Utility classes for AI features
- `ui/`: User interface components for AI features
"

# Models directory
EnsureDirectory "$rootDir\models\musicvae"
EnsureDirectory "$rootDir\models\groovae"
EnsureDirectory "$rootDir\models\melodyrnn"
CreateReadme "$rootDir\models" "# AI Models

This directory contains the AI models used by the LMMS-Magenta integration.

## Structure

- `musicvae/`: MusicVAE models for pattern generation and interpolation
- `groovae/`: GrooVAE models for rhythm humanization and groove transfer
- `melodyrnn/`: MelodyRNN models for melody continuation and generation
"

# Scripts directory
EnsureDirectory "$rootDir\scripts\setup"
EnsureDirectory "$rootDir\scripts\build"
EnsureDirectory "$rootDir\scripts\tools"
CreateReadme "$rootDir\scripts" "# Scripts

This directory contains scripts for setting up, building, and maintaining the LMMS-Magenta integration.

## Structure

- `setup/`: Scripts for setting up the development environment
- `build/`: Scripts for building the project
- `tools/`: Utility scripts for development and maintenance
"

# External directory
EnsureDirectory "$rootDir\external"
CreateReadme "$rootDir\external" "# External Dependencies

This directory contains external dependencies used by the LMMS-Magenta integration.

External dependencies are managed using git submodules or CMake's FetchContent.
"

# Documentation directory
EnsureDirectory "$rootDir\docs\dev"
EnsureDirectory "$rootDir\docs\api"
EnsureDirectory "$rootDir\docs\user"
CreateReadme "$rootDir\docs" "# Documentation

This directory contains documentation for the LMMS-Magenta integration.

## Structure

- `dev/`: Developer documentation
- `api/`: API documentation
- `user/`: User documentation
"

# Tests directory
EnsureDirectory "$rootDir\tests\unit"
EnsureDirectory "$rootDir\tests\integration"
EnsureDirectory "$rootDir\tests\performance"
CreateReadme "$rootDir\tests" "# Tests

This directory contains tests for the LMMS-Magenta integration.

## Structure

- `unit/`: Unit tests for individual components
- `integration/`: Integration tests for component interactions
- `performance/`: Performance tests for AI features
"

# Include directory
EnsureDirectory "$rootDir\include\lmms-magenta"
CreateReadme "$rootDir\include" "# Include Directory

This directory contains public header files for the LMMS-Magenta integration.
"

# GitHub workflows directory
EnsureDirectory "$rootDir\.github\workflows"
CreateReadme "$rootDir\.github" "# GitHub Configuration

This directory contains GitHub-specific configuration files.

## Structure

- `workflows/`: GitHub Actions workflow definitions
"

# VS Code configuration
EnsureDirectory "$rootDir\.vscode"
CreateReadme "$rootDir\.vscode" "# VS Code Configuration

This directory contains VS Code-specific configuration files.
"

Write-Host "Directory structure setup complete!"
