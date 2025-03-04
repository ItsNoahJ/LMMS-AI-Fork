# LMMS-Magenta Development Environment Setup Script
# This script sets up the development environment for the LMMS-Magenta project

# Ensure we're in the scripts directory
$scriptsDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$setupDir = Split-Path -Parent $scriptsDir
$rootDir = Split-Path -Parent $setupDir

Write-Host "Setting up LMMS-Magenta development environment..."

# Check for required tools
Write-Host "Checking for required tools..."

# Check for CMake
$cmakeVersion = $null
try {
    $cmakeVersion = (cmake --version | Select-Object -First 1).Split(' ')[2]
    Write-Host "Found CMake version $cmakeVersion"
} catch {
    Write-Host "CMake not found. Please install CMake 3.16 or later."
    exit 1
}

# Check for Python
$pythonVersion = $null
try {
    $pythonVersion = (python --version).Split(' ')[1]
    Write-Host "Found Python version $pythonVersion"
} catch {
    Write-Host "Python not found. Please install Python 3.7 or later."
    exit 1
}

# Check for Qt
$qtVersion = $null
try {
    $qtVersion = (qmake --version | Select-Object -First 1).Split(' ')[3]
    Write-Host "Found Qt version $qtVersion"
} catch {
    Write-Host "Qt not found. Please install Qt 5.15 or later."
    exit 1
}

# Check for Git
$gitVersion = $null
try {
    $gitVersion = (git --version).Split(' ')[2]
    Write-Host "Found Git version $gitVersion"
} catch {
    Write-Host "Git not found. Please install Git."
    exit 1
}

# Create build directory
$buildDir = Join-Path -Path $rootDir -ChildPath "build"
if (-not (Test-Path $buildDir)) {
    Write-Host "Creating build directory..."
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Install Python dependencies
Write-Host "Installing Python dependencies..."
python -m pip install --upgrade pip
python -m pip install tensorflow tensorflow-hub magenta mkdocs mkdocs-material

# Configure the project
Write-Host "Configuring the project..."
Push-Location $buildDir
cmake -DBUILD_TESTING=ON -DBUILD_DOCS=ON ..
Pop-Location

Write-Host "Development environment setup complete!"
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Build the project: cd build && cmake --build ."
Write-Host "2. Run the tests: cd build && ctest"
Write-Host "3. Build the documentation: cd build && cmake --build . --target docs"
