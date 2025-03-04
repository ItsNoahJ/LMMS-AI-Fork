# Download Qt offline installer
$url = "https://download.qt.io/official_releases/qt/5.15/5.15.2/qt-opensource-windows-x86-5.15.2.exe"
$output = "qt-installer.exe"

Write-Host "Downloading Qt installer..."
Invoke-WebRequest -Uri $url -OutFile $output

Write-Host "Download complete. Please run the installer and follow the installation steps."
Write-Host "Make sure to install the following components:"
Write-Host "- Qt 5.15.2 MSVC 2019 64-bit"
Write-Host "- Qt Creator"
