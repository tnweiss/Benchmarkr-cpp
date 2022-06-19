@echo off

set VERSION=0.0.3

cd "C:/Windows/Temp"

:: get the artiface
if "%1" == "Skip-Download" (
    :: download the resource
    echo "Downloading Benchmarkr Version %VERSION%..."
    wget "https://github.com/tnweiss/Benchmarkr-cpp/releases/download/%VERSION%/windows.zip" -O windows.zip
) else (
    echo "Skipping Benchmarkr download"
)

:: unpack
7z x windows.zip

xcopy install "C:\Program Files (x86)\benchmarkr" /S /D /I /Y
ls "C:\Program Files (x86)\benchmarkr"

:: add binary to the path
echo "Adding binary to path"
setx PATH "C:\Program Files (x86)\benchmarkr\bin;%PATH%" /m

echo "Done"
