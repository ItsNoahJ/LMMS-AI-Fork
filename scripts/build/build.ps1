# LMMS-Magenta Build Script
# This script builds the LMMS-Magenta project

param(
    [switch]$Release,
    [switch]$NoTests,
    [switch]$Docs
)

# Ensure we're in the scripts directory
$scriptsDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildScriptsDir = Split-Path -Parent $scriptsDir
$rootDir = Split-Path -Parent $buildScriptsDir

Write-Host "Building LMMS-Magenta..."

# Determine build type
$buildType = "Debug"
if ($Release) {
    $buildType = "Release"
}

# Create build directory
$buildDir = Join-Path -Path $rootDir -ChildPath "build"
if (-not (Test-Path $buildDir)) {
    Write-Host "Creating build directory..."
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Configure CMake arguments
$cmakeArgs = "-DCMAKE_BUILD_TYPE=$buildType"

if ($NoTests) {
    $cmakeArgs = "$cmakeArgs -DBUILD_TESTING=OFF"
}

if ($Docs) {
    $cmakeArgs = "$cmakeArgs -DBUILD_DOCS=ON"
}

# Configure the project
Write-Host "Configuring the project with arguments: $cmakeArgs"
Push-Location $buildDir
Invoke-Expression "cmake $cmakeArgs .."

# Build the project
Write-Host "Building the project..."
cmake --build . --config $buildType

# Run tests if enabled
if (-not $NoTests) {
    Write-Host "Running tests..."
    ctest --output-on-failure --build-config $buildType
}

# Build documentation if enabled
if ($Docs) {
    Write-Host "Building documentation..."
    cmake --build . --target docs
}

Pop-Location

Write-Host "Build complete!"
